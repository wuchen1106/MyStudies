#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <math.h>
#include <string>
#include <vector>
#include <algorithm>

#include "globals.hh"
#include "Randomize.hh"

#include "TH1D.h"
#include "TH2D.h"
#include "TRandom.h"

#include "MyRootInterface.hxx"

char m_workMode[128];
std::string m_runName;
int verbose = 0;
int nEvents = 0;
int printModule = 1;
bool backup = false;
std::vector<int> Ncut;
std::vector<std::string> Ncut_message;

void init_Ncut();
void inc_Ncut(std::string);
void dump_Ncut();
void init_args();
void print_usage(char* prog_name);

int main(int argc, char* argv[]){

	clock_t t_START = clock();

	//=======================================
	//*************read parameter**********
	init_args();
	int result;
	while((result=getopt(argc,argv,"hbv:n:m:r:p:"))!=-1){
		switch(result){
			/* INPUTS */
			case 'm':
				strcpy(m_workMode,optarg);
				printf("work mode: %s\n",m_workMode);
				break;
			case 'r':
				m_runName=optarg;
				printf("run name: %s\n",m_runName.c_str());
				break;
			case 'v':
				verbose = atoi(optarg);
				printf("verbose level: %d\n",verbose);
				break;
			case 'b':
				backup = true;
				printf("restore backup file!\n");
				break;
			case 'n':
				nEvents = atoi(optarg);
				printf("nEvent: %d\n",nEvents);
				break;
			case 'p':
				printModule = atoi(optarg);
				printf("printModule: %d\n",printModule);
				break;
			case '?':
				printf("Wrong option! optopt=%c, optarg=%s\n", optopt, optarg);
				break;
			case 'h':
			default:
				print_usage(argv[0]);
				return 1;
		}
	}

	//=======================================
	//************Verbose Control***********
	int Verbose_SectorInfo = 5; //大概的流程情况
	std::string prefix_SectorInfo = "### ";
	int Verbose_HistInfo = 10; //有哪些hist,什么时候输出了，参数如何
	std::string prefix_HistInfo= "  [Histograms] ";
	int Verbose_Statistics = 10; //跟统计相关的(效率，分辨，粒子鉴别的情况)
	std::string prefix_Statistics="  [Statistics] ";
	int Verbose_FileInfo = 10; //有哪些FileList,都有多少file
	std::string prefix_FileInfo="  [FileInfo] ";
	int Verbose_EffInfo = 15; //Efficiency info
	std::string prefix_EffInfo="  [EffInfo] ";
	int Verbose_EventInfo = 20; //每个event的基本流程
	std::string prefix_EventInfoStart="    =>[EventInfo] ";
	std::string prefix_EventInfo="      [EventInfo] ";
	int Verbose_ParticleInfo=25; //每个particle的基本信息
	std::string prefix_ParticleInfoStart="    ->[ParticleInfo]";
	std::string prefix_ParticleInfo="      [ParticleInfo]";

	//##########################PRESET############################
	if (verbose >= Verbose_SectorInfo ) std::cout<<prefix_SectorInfo<<"In Preset###"<<std::endl;
	MyRootInterface *fMyRootInterface = new MyRootInterface(verbose,backup);
	fMyRootInterface->set_OutputDir("result");
	int index_temp = 0;
	TH1D *h1d_temp=0;
	int bin_temp = 0;
	std::string name_temp = "";
	std::stringstream buff;

	//=>About Constant
	double PI = 3.141592653589793238;
	double FSC = 1/137.03599911; //fine structure constant
	double M_MUON = 105.6584*MeV; //mass of muon in MeV
	double M_ELE = 0.510999*MeV; //mass of electron in MeV
	double M_U = 931.494061*MeV; //atomic mass unit in MeV
	double M_p = 0.9382723*GeV;  // mass of proton// proton mass unit in GeV

	//##########################Prepare histograms############################
	if (verbose >= Verbose_SectorInfo ) std::cout<<prefix_SectorInfo<<"In SET HISTOGRAMS###"<<std::endl;
	fMyRootInterface->read("input");
	fMyRootInterface->set_OutputName(m_runName);
	fMyRootInterface->init();

	//************SET Statistics********************
	if (verbose >= Verbose_SectorInfo ) std::cout<<prefix_SectorInfo<<"In SET Statistics###"<<std::endl;
	//=>About Statistical
	init_Ncut();

	//=======================================================================================================
	//************DO THE DIRTY WORK*******************
	if (verbose >= Verbose_SectorInfo ) std::cout<<prefix_SectorInfo<<"In DO THE DIRTY WORK###"<<std::endl;
	Long64_t nEvent = fMyRootInterface->get_Entries();
	for( Long64_t iEvent = 0; iEvent < (nEvents&&nEvents<nEvent?nEvents:nEvent); iEvent++ ){
		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"In Event "<<iEvent<<std::endl;
		fMyRootInterface->GetEntry(iEvent);
		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Got entries"<<std::endl;
		inc_Ncut("Got entries");

		// Get info
		int error;
		int nfail;
		double chi2;
		double tailfit_1_fit_pa;
		double tailfit_1_fit_pz;
		double tailfit_1_fit_py;
		double tailfit_1_fit_px;
		double fit_pa;
		double fit_px;
		double fit_py;
		double fit_pz;
		double ini_pa;
		double ini_px;
		double ini_py;
		double ini_pz;

		index_temp = fMyRootInterface->get_TBranch_index("error");
		if (index_temp!=-1) error = fMyRootInterface->get_vec_int(index_temp);
		index_temp = fMyRootInterface->get_TBranch_index("nfail");
		if (index_temp!=-1) nfail = fMyRootInterface->get_vec_int(index_temp);
		index_temp = fMyRootInterface->get_TBranch_index("chi2");
		if (index_temp!=-1) chi2 = fMyRootInterface->get_vec_double(index_temp);
		index_temp = fMyRootInterface->get_TBranch_index("tailfit_1_fit_px");
		if (index_temp!=-1) tailfit_1_fit_px = fMyRootInterface->get_vec_double(index_temp)*GeV;
		index_temp = fMyRootInterface->get_TBranch_index("tailfit_1_fit_py");
		if (index_temp!=-1) tailfit_1_fit_py = fMyRootInterface->get_vec_double(index_temp)*GeV;
		index_temp = fMyRootInterface->get_TBranch_index("tailfit_1_fit_pz");
		if (index_temp!=-1) tailfit_1_fit_pz = fMyRootInterface->get_vec_double(index_temp)*GeV;
		index_temp = fMyRootInterface->get_TBranch_index("fit_px");
		if (index_temp!=-1) fit_px = fMyRootInterface->get_vec_double(index_temp)*GeV;
		index_temp = fMyRootInterface->get_TBranch_index("fit_py");
		if (index_temp!=-1) fit_py = fMyRootInterface->get_vec_double(index_temp)*GeV;
		index_temp = fMyRootInterface->get_TBranch_index("fit_pz");
		if (index_temp!=-1) fit_pz = fMyRootInterface->get_vec_double(index_temp)*GeV;
		index_temp = fMyRootInterface->get_TBranch_index("ini_px");
		if (index_temp!=-1) ini_px = fMyRootInterface->get_vec_double(index_temp)*GeV;
		index_temp = fMyRootInterface->get_TBranch_index("ini_py");
		if (index_temp!=-1) ini_py = fMyRootInterface->get_vec_double(index_temp)*GeV;
		index_temp = fMyRootInterface->get_TBranch_index("ini_pz");
		if (index_temp!=-1) ini_pz = fMyRootInterface->get_vec_double(index_temp)*GeV;

		tailfit_1_fit_pa = sqrt(tailfit_1_fit_px*tailfit_1_fit_px+tailfit_1_fit_py*tailfit_1_fit_py+tailfit_1_fit_pz*tailfit_1_fit_pz);
		ini_pa = sqrt(ini_px*ini_px+ini_py*ini_py+ini_pz*ini_pz);
		fit_pa = sqrt(fit_px*fit_px+fit_py*fit_py+fit_pz*fit_pz);

		//std::cout<<"error = "<<error
		//	     <<", nfail = "<<nfail
		//	     <<",chi2 = "<<chi2
		//	     <<", tailfit_1_fit_pa = "<<tailfit_1_fit_pa
		//	     <<", fit_pa = "<<fit_pa
		//	     <<", ini_pa = "<<ini_pa
		//	     <<std::endl;
		if ( error==0 && nfail==0 && chi2>0 && chi2<2 && abs(tailfit_1_fit_pa-fit_pa)<2.0*MeV ){
			if ( (index_temp = fMyRootInterface->get_TH1D_index(m_runName+"fitpa")) != -1 ){
				fMyRootInterface->get_TH1D(index_temp)->Fill(fit_pa/MeV);
			}
			if ( (index_temp = fMyRootInterface->get_TH1D_index(m_runName+"inipa")) != -1 ){
				fMyRootInterface->get_TH1D(index_temp)->Fill(ini_pa/MeV);
			}
		}

		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfo<<"Finished!"<<std::endl;
	}/* end of loop in events*/

	//=>For output
	clock_t t_END = clock();
	//=======================================================================================================
	//************WRITE AND OUTPUT********************
	if (verbose >= Verbose_SectorInfo) std::cout<<prefix_SectorInfo<<"In WRITE AND OUTPUT ###"<<std::endl;
	std::cout<<"\n################BENTCH MARK###################"<<std::endl;
	std::cout<<"TOTAL TIME COST IS:  "<<(double)(t_END-t_START)/CLOCKS_PER_SEC*1000<<"ms"<<std::endl;
	//std::cout<<"PRESET:              "<<(double)(t_PRESET-t_START)/CLOCKS_PER_SEC*1000<<"ms"<<std::endl;
	//std::cout<<"INITIALIZE:          "<<(double)(t_INITIALIZE-t_PRESET)/CLOCKS_PER_SEC*1000<<"ms"<<std::endl;
	//std::cout<<"GENERATEEVENTS:      "<<(double)(t_GENERATEEVENTS-t_BUILDHISTOGRAMS)/CLOCKS_PER_SEC*1000<<"ms"<<std::endl;
	//std::cout<<"TIME COST PER EVENT: "<<(double)(t_GENERATEEVENTS-t_BUILDHISTOGRAMS)/CLOCKS_PER_SEC/num_evt*1000<<"ms"<<std::endl;
	std::cout<<"##############################################\n"<<std::endl;
	dump_Ncut();

	fMyRootInterface->dump();
	return 0;
}

void init_args()
{
	strcpy(m_workMode,"gen");
	verbose = 0;
	nEvents = 0;
	printModule = 10000;
	backup = false;
}

void print_usage(char* prog_name)
{
	fprintf(stderr,"Usage %s [options (args)] [input files]\n",prog_name);
	fprintf(stderr,"[options]\n");
	fprintf(stderr,"\t -m\n");
	fprintf(stderr,"\t\t choose work mode: [gen(default), com]\n");
	fprintf(stderr,"\t -r\n");
	fprintf(stderr,"\t\t set run name\n");
	fprintf(stderr,"\t -v\n");
	fprintf(stderr,"\t\t verbose level\n");
	fprintf(stderr,"\t -n\n");
	fprintf(stderr,"\t\t nEvent\n");
	fprintf(stderr,"\t -p\n");
	fprintf(stderr,"\t\t printModule\n");
	fprintf(stderr,"\t -h\n");
	fprintf(stderr,"\t\t Usage message.\n");
	fprintf(stderr,"\t -b\n");
	fprintf(stderr,"\t\t restore backup file.\n");
	fprintf(stderr,"[example]\n");
	fprintf(stderr,"\t\t%s -m ab -v 20 -n 100\n",prog_name);
}

void init_Ncut(){
	Ncut.clear();
	Ncut_message.clear();
}

void inc_Ncut(std::string mess){
	int index = -1;
	for (int i = 0; i < Ncut_message.size(); i++){
		if (Ncut_message[i]==mess){
			index = i;
			break;
		}
	}
	if (index == -1){
		index = Ncut_message.size();
		Ncut_message.push_back(mess);
		Ncut.push_back(0);
	}
	Ncut[index]++;
}

void dump_Ncut(){
	for (int i = 0; i < Ncut.size(); i++ ){
		std::cout<<"  "<<Ncut_message[i]<<": "<<Ncut[i]<<std::endl;
	}
}
