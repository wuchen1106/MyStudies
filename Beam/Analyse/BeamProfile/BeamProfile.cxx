#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <math.h>
#include <string>

#include "globals.hh"

#include "TH1D.h"
#include "TH2D.h"

#include "MyRootInterface.hxx"

char m_workMode[128];
std::string m_runName;
int verbose = 0;
int nEvents = 0;
int printModule = 1;
bool backup = false;

void init_args();
void print_usage(char* prog_name);

int main(int argc, char** argv){

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
	MyRootInterface *fMyRootInterface = new MyRootInterface(verbose);
	fMyRootInterface->set_OutputDir("result");
	int index_temp = 0;
	TH1D *h1d_temp=0;
	int bin_temp = 0;
	std::string name_temp = "";

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
	int N0 = 0;
	int N1 = 0;
	int N2 = 0;
	int N3 = 0;
	int N4 = 0;
	int N5 = 0;
	int N6 = 0;
	int N7 = 0;

	//=======================================================================================================
	//************DO THE DIRTY WORK*******************
	if (verbose >= Verbose_SectorInfo ) std::cout<<prefix_SectorInfo<<"In DO THE DIRTY WORK###"<<std::endl;
	Long64_t nEvent = fMyRootInterface->get_Entries();
	for( Long64_t iEvent = 0; iEvent < nEvent; iEvent++ ){
		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"In Event "<<iEvent<<std::endl;
		N0++;
		fMyRootInterface->GetEntry(iEvent);
		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Got entries"<<std::endl;

		index_temp = fMyRootInterface->get_TBranch_index("x");
		double x = fMyRootInterface->get_vec_double(index_temp)*mm;
		index_temp = fMyRootInterface->get_TBranch_index("y");
		double y = fMyRootInterface->get_vec_double(index_temp)*mm;
		index_temp = fMyRootInterface->get_TBranch_index("z");
		double z = fMyRootInterface->get_vec_double(index_temp)*mm;
		index_temp = fMyRootInterface->get_TBranch_index("px");
		double px = fMyRootInterface->get_vec_double(index_temp)*MeV;
		index_temp = fMyRootInterface->get_TBranch_index("py");
		double py = fMyRootInterface->get_vec_double(index_temp)*MeV;
		index_temp = fMyRootInterface->get_TBranch_index("pz");
		double pz = fMyRootInterface->get_vec_double(index_temp)*MeV;
		index_temp = fMyRootInterface->get_TBranch_index("t");
		double t = fMyRootInterface->get_vec_double(index_temp)*ns;
		index_temp = fMyRootInterface->get_TBranch_index("ox");
		double ox = fMyRootInterface->get_vec_double(index_temp)*mm;
		index_temp = fMyRootInterface->get_TBranch_index("oy");
		double oy = fMyRootInterface->get_vec_double(index_temp)*mm;
		index_temp = fMyRootInterface->get_TBranch_index("oz");
		double oz = fMyRootInterface->get_vec_double(index_temp)*mm;

		double pa = sqrt(px*px+py*py+pz*pz);
		double pt = sqrt(px*px+py*py);
		double r = sqrt(x*x+y*y);
		double theta = acos(pz/pa);
		index_temp = fMyRootInterface->get_TH1D_index(m_runName+"pa");
		if (index_temp!=-1) fMyRootInterface->get_TH1D(index_temp)->Fill(pa/MeV);
		index_temp = fMyRootInterface->get_TH1D_index(m_runName+"pt");
		if (index_temp!=-1) fMyRootInterface->get_TH1D(index_temp)->Fill(pt/MeV);
		index_temp = fMyRootInterface->get_TH1D_index(m_runName+"pz");
		if (index_temp!=-1)	fMyRootInterface->get_TH1D(index_temp)->Fill(pz/MeV);
		index_temp = fMyRootInterface->get_TH1D_index(m_runName+"theta");
		if (index_temp!=-1) fMyRootInterface->get_TH1D(index_temp)->Fill(theta/MeV);
		index_temp = fMyRootInterface->get_TH1D_index(m_runName+"gTime");
		if (index_temp!=-1) fMyRootInterface->get_TH1D(index_temp)->Fill(t/ns);
		index_temp = fMyRootInterface->get_TH1D_index(m_runName+"r");
		if (index_temp!=-1) fMyRootInterface->get_TH1D(index_temp)->Fill(r/mm);
		index_temp = fMyRootInterface->get_TH1D_index(m_runName+"ox");
		if (index_temp!=-1) fMyRootInterface->get_TH1D(index_temp)->Fill(ox/mm);
		index_temp = fMyRootInterface->get_TH1D_index(m_runName+"oy");
		if (index_temp!=-1) fMyRootInterface->get_TH1D(index_temp)->Fill(oy/mm);
		index_temp = fMyRootInterface->get_TH1D_index(m_runName+"oz");
		if (index_temp!=-1) fMyRootInterface->get_TH1D(index_temp)->Fill(oz/mm);

		index_temp = fMyRootInterface->get_TH2D_index(m_runName+"pax");
		if (index_temp!=-1) fMyRootInterface->get_TH2D(index_temp)->Fill(pa/MeV,x/mm);
		index_temp = fMyRootInterface->get_TH2D_index(m_runName+"ptx");
		if (index_temp!=-1) fMyRootInterface->get_TH2D(index_temp)->Fill(pt/MeV,x/mm);
		index_temp = fMyRootInterface->get_TH2D_index(m_runName+"pzx");
		if (index_temp!=-1) fMyRootInterface->get_TH2D(index_temp)->Fill(pz/MeV,x/mm);
		index_temp = fMyRootInterface->get_TH2D_index(m_runName+"paz");
		if (index_temp!=-1) fMyRootInterface->get_TH2D(index_temp)->Fill(pa/MeV,z/mm);
		index_temp = fMyRootInterface->get_TH2D_index(m_runName+"ptz");
		if (index_temp!=-1) fMyRootInterface->get_TH2D(index_temp)->Fill(pt/MeV,z/mm);
		index_temp = fMyRootInterface->get_TH2D_index(m_runName+"pzz");
		if (index_temp!=-1) fMyRootInterface->get_TH2D(index_temp)->Fill(pz/MeV,z/mm);

		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Done"<<std::endl;
	}

	//=>For output
	clock_t t_END = clock();
	//#########################THEEND###############################
	if (verbose >= Verbose_SectorInfo) std::cout<<prefix_SectorInfo<<"In WRITE AND OUTPUT ###"<<std::endl;
	std::cout<<"\n################BENTCH MARK###################"<<std::endl;
	std::cout<<"TOTAL TIME COST IS:  "<<(double)(t_END-t_START)/CLOCKS_PER_SEC*1000<<"ms"<<std::endl;
	//std::cout<<"PRESET:              "<<(double)(t_PRESET-t_START)/CLOCKS_PER_SEC*1000<<"ms"<<std::endl;
	//std::cout<<"INITIALIZE:          "<<(double)(t_INITIALIZE-t_PRESET)/CLOCKS_PER_SEC*1000<<"ms"<<std::endl;
	//std::cout<<"GENERATEEVENTS:      "<<(double)(t_GENERATEEVENTS-t_BUILDHISTOGRAMS)/CLOCKS_PER_SEC*1000<<"ms"<<std::endl;
	//std::cout<<"TIME COST PER EVENT: "<<(double)(t_GENERATEEVENTS-t_BUILDHISTOGRAMS)/CLOCKS_PER_SEC/num_evt*1000<<"ms"<<std::endl;
	std::cout<<"##############################################\n"<<std::endl;
	std::cout<<"N0 = "<<N0<<std::endl;
	std::cout<<"N1 = "<<N1<<std::endl;
	std::cout<<"N2 = "<<N2<<std::endl;
	std::cout<<"N3 = "<<N3<<std::endl;

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
