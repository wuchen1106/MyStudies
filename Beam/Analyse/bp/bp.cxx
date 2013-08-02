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

std::string m_MonitorPlane;
std::string m_runName;
std::string m_input;
std::string m_OutputDir;
int m_beginNo = -1;
int m_totalNo = -1;
int verbose = 0;
int nEvents = 0;
int printModule = 1;
int PDGEncoding = 13;
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
	while((result=getopt(argc,argv,"hb:t:v:n:m:r:d:p:P:i:"))!=-1){
		switch(result){
			/* INPUTS */
			case 'm':
				m_MonitorPlane = optarg;
				printf("Monitor plane: %s\n",m_MonitorPlane.c_str());
				break;
			case 'r':
				m_runName=optarg;
			case 'i':
				m_input=optarg;
				printf("input configuration file: %s\n",m_input.c_str());
				break;
			case 'd':
				m_OutputDir=optarg;
				printf("Output Directroy: %s\n",m_OutputDir.c_str());
				break;
			case 'v':
				verbose = atoi(optarg);
				printf("verbose level: %d\n",verbose);
				break;
			case 'b':
				m_beginNo = atoi(optarg);
				printf("CPU index starts from%d\n",m_beginNo);
				break;
			case 't':
				m_totalNo = atoi(optarg);
				printf("Total CPU numbers%d\n",m_totalNo);
				break;
			case 'n':
				nEvents = atoi(optarg);
				printf("nEvent: %d\n",nEvents);
				break;
			case 'p':
				printModule = atoi(optarg);
				printf("printModule: %d\n",printModule);
				break;
			case 'P':
				PDGEncoding = atoi(optarg);
				printf("PDGEncoding: %d\n",PDGEncoding);
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
	fMyRootInterface->set_OutputDir(m_OutputDir);
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
	fMyRootInterface->read(m_input);
	if (m_beginNo!=-1&&m_totalNo!=-1){
		fMyRootInterface->set_beginCPU(0,m_beginNo);
		fMyRootInterface->set_NCPU(0,m_totalNo);
	}
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

		// For output
		double x;
		double y;
		double z;
		double px;
		double py;
		double pz;
		double t;

		// Get info
		int MonitorC_nHits = 0;
		std::vector<int> MonitorC_pid;
		std::vector<double> MonitorC_t;
		std::vector<double> MonitorC_x;
		std::vector<double> MonitorC_y;
		std::vector<double> MonitorC_z;
		std::vector<double> MonitorC_px;
		std::vector<double> MonitorC_py;
		std::vector<double> MonitorC_pz;
		int MonitorE_nHits = 0;
		std::vector<int> MonitorE_pid;
		std::vector<double> MonitorE_t;
		std::vector<double> MonitorE_x;
		std::vector<double> MonitorE_y;
		std::vector<double> MonitorE_z;
		std::vector<double> MonitorE_px;
		std::vector<double> MonitorE_py;
		std::vector<double> MonitorE_pz;

		fMyRootInterface->get_value("MonitorE_nHits",MonitorE_nHits);
		fMyRootInterface->get_value("MonitorE_t",MonitorE_t,ns);
		fMyRootInterface->get_value("MonitorE_pid",MonitorE_pid);
		fMyRootInterface->get_value("MonitorE_x",MonitorE_x,cm);
		fMyRootInterface->get_value("MonitorE_y",MonitorE_y,cm);
		fMyRootInterface->get_value("MonitorE_z",MonitorE_z,cm);
		fMyRootInterface->get_value("MonitorE_px",MonitorE_px,GeV);
		fMyRootInterface->get_value("MonitorE_py",MonitorE_py,GeV);
		fMyRootInterface->get_value("MonitorE_pz",MonitorE_pz,GeV);
		fMyRootInterface->get_value("MonitorC_nHits",MonitorC_nHits);
		fMyRootInterface->get_value("MonitorC_t",MonitorC_t,ns);
		fMyRootInterface->get_value("MonitorC_pid",MonitorC_pid);
		fMyRootInterface->get_value("MonitorC_x",MonitorC_x,cm);
		fMyRootInterface->get_value("MonitorC_y",MonitorC_y,cm);
		fMyRootInterface->get_value("MonitorC_z",MonitorC_z,cm);
		fMyRootInterface->get_value("MonitorC_px",MonitorC_px,GeV);
		fMyRootInterface->get_value("MonitorC_py",MonitorC_py,GeV);
		fMyRootInterface->get_value("MonitorC_pz",MonitorC_pz,GeV);

		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Got info"<<std::endl;

		// find particles
		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"###Searching particles in Monitor"<<std::endl;

		bool filled = false;
		for ( int i_mon = 0; i_mon < MonitorE_nHits; i_mon++ ){
			if (MonitorE_pid[i_mon] == PDGEncoding ){
				filled = true;
				if (verbose >= Verbose_ParticleInfo || iEvent%printModule == 0)
					std::cout<<prefix_ParticleInfoStart
				             <<"  Found Particle! i_mon = "<<i_mon
				             <<", pid = "<<MonitorE_pid[i_mon]
				             <<", px = "<<MonitorE_px[i_mon]
				             <<"MeV, py = "<<MonitorE_py[i_mon]
				             <<"MeV, pz = "<<MonitorE_pz[i_mon]
				             <<"MeV"
						     <<std::endl;
				x=MonitorE_x[i_mon];
				y=MonitorE_y[i_mon];
				z=MonitorE_z[i_mon];
				px=MonitorE_px[i_mon];
				py=MonitorE_py[i_mon];
				pz=MonitorE_pz[i_mon];
				t=MonitorE_t[i_mon];
				fMyRootInterface->set_ovalue("x",x/mm);
				fMyRootInterface->set_ovalue("y",y/mm);
				fMyRootInterface->set_ovalue("z",z/mm);
				fMyRootInterface->set_ovalue("px",px/MeV);
				fMyRootInterface->set_ovalue("py",py/MeV);
				fMyRootInterface->set_ovalue("pz",pz/MeV);
				fMyRootInterface->set_ovalue("t",t/ns);
				if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Set oTrees"<<std::endl;
				fMyRootInterface->Fill();
				if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Filled"<<std::endl;
			}
		}
		for ( int i_mon = 0; i_mon < MonitorC_nHits; i_mon++ ){
			if (MonitorC_pid[i_mon] == PDGEncoding ){
				filled = true;
				if (verbose >= Verbose_ParticleInfo || iEvent%printModule == 0)
					std::cout<<prefix_ParticleInfoStart
				             <<"  Found Particle! i_mon = "<<i_mon
				             <<", pid = "<<MonitorC_pid[i_mon]
				             <<", px = "<<MonitorC_px[i_mon]
				             <<"MeV, py = "<<MonitorC_py[i_mon]
				             <<"MeV, pz = "<<MonitorC_pz[i_mon]
				             <<"MeV"
						     <<std::endl;
				x=MonitorC_x[i_mon];
				y=MonitorC_y[i_mon];
				z=MonitorC_z[i_mon];
				px=MonitorC_px[i_mon];
				py=MonitorC_py[i_mon];
				pz=MonitorC_pz[i_mon];
				t=MonitorC_t[i_mon];
				fMyRootInterface->set_ovalue("x",x/mm);
				fMyRootInterface->set_ovalue("y",y/mm);
				fMyRootInterface->set_ovalue("z",z/mm);
				fMyRootInterface->set_ovalue("px",px/MeV);
				fMyRootInterface->set_ovalue("py",py/MeV);
				fMyRootInterface->set_ovalue("pz",pz/MeV);
				fMyRootInterface->set_ovalue("t",t/ns);
				if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Set oTrees"<<std::endl;
				fMyRootInterface->Fill();
				if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Filled"<<std::endl;
			}
		}
		if (filled)	inc_Ncut("Got particles");

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
	m_MonitorPlane="blt0";
	m_OutputDir="result";
	m_input="input";
	verbose = 0;
	nEvents = 0;
	printModule = 10000;
	PDGEncoding = 13;
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
	fprintf(stderr,"\t -P\n");
	fprintf(stderr,"\t\t PDGEncoding\n");
	fprintf(stderr,"\t -h\n");
	fprintf(stderr,"\t\t Usage message.\n");
	fprintf(stderr,"\t -b\n");
	fprintf(stderr,"\t\tCPU index starts from\n");
	fprintf(stderr,"\t -t\n");
	fprintf(stderr,"\t\tTotal CPU numbers\n");
	fprintf(stderr,"[example]\n");
	fprintf(stderr,"\t\t%s -m ab -v 20 -n 100\n",prog_name);
}

void init_Ncut(){
	Ncut.clear();
	Ncut_message.clear();
}

void inc_Ncut(std::string mess){
	int iCut = -1;
	for (int i = 0; i < Ncut_message.size(); i++){
		if (Ncut_message[i]==mess){
			iCut = i;
			break;
		}
	}
	if (iCut == -1){
		iCut = Ncut_message.size();
		Ncut_message.push_back(mess);
		Ncut.push_back(0);
	}
	Ncut[iCut]++;
}

void dump_Ncut(){
	for (int i = 0; i < Ncut.size(); i++ ){
		std::cout<<"  "<<Ncut_message[i]<<": "<<Ncut[i]<<std::endl;
	}
}
