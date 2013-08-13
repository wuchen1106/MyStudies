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
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"

#include "MyRootInterface.hxx"

std::string m_workMode;
std::string m_MonitorPlane;
std::string m_runName;
std::string m_input;
std::string m_OutputDir;
std::string m_InputDir;
std::string m_OriginalFile;
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
	while((result=getopt(argc,argv,"hb:t:v:n:m:M:r:D:O:d:p:P:i:"))!=-1){
		switch(result){
			/* INPUTS */
			case 'm':
				m_workMode = optarg;
				printf("Work Mode: %s\n",m_workMode.c_str());
				break;
			case 'M':
				m_MonitorPlane = optarg;
				printf("Monitor plane: %s\n",m_MonitorPlane.c_str());
				break;
			case 'r':
				m_runName=optarg;
				printf("runName: %s\n",m_runName.c_str());
				break;
			case 'i':
				m_input=optarg;
				printf("input configuration file: %s\n",m_input.c_str());
				break;
			case 'D':
				m_InputDir=optarg;
				printf("Input Directroy: %s\n",m_InputDir.c_str());
				break;
			case 'O':
				m_OriginalFile=optarg;
				printf("OriginalFile: %s\n",m_OriginalFile.c_str());
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
	int Verbose_SectorInfo = 5; //å¤§æ¦çæµç¨æåµ
	std::string prefix_SectorInfo = "### ";
	int Verbose_HistInfo = 10; //æåªäºhist,ä»ä¹æ¶åè¾åºäºï¼åæ°å¦ä½
	std::string prefix_HistInfo= "  [Histograms] ";
	int Verbose_Statistics = 10; //è·ç»è®¡ç¸å³ç(æçï¼åè¾¨ï¼ç²å­é´å«çæåµ)
	std::string prefix_Statistics="  [Statistics] ";
	int Verbose_FileInfo = 10; //æåªäºFileList,é½æå¤å°file
	std::string prefix_FileInfo="  [FileInfo] ";
	int Verbose_EffInfo = 15; //Efficiency info
	std::string prefix_EffInfo="  [EffInfo] ";
	int Verbose_EventInfo = 20; //æ¯ä¸ªeventçåºæ¬æµç¨
	std::string prefix_EventInfoStart="    =>[EventInfo] ";
	std::string prefix_EventInfo="      [EventInfo] ";
	int Verbose_ParticleInfo=25; //æ¯ä¸ªparticleçåºæ¬ä¿¡æ¯
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
	double M_PION = 0.139570*GeV;
	double M_MUON = 105.6584*MeV; //mass of muon in MeV
	double M_ELE = 0.510999*MeV; //mass of electron in MeV
	double M_U = 931.494061*MeV; //atomic mass unit in MeV
	double M_p = 0.9382723*GeV;  // mass of proton// proton mass unit in GeV
	double tau = 26*ns; // mean life time of pion

	//##########################Prepare histograms############################
	if (verbose >= Verbose_SectorInfo ) std::cout<<prefix_SectorInfo<<"In SET HISTOGRAMS###"<<std::endl;
	fMyRootInterface->read(m_input);
	if (m_beginNo!=-1&&m_totalNo!=-1){
		fMyRootInterface->set_beginCPU(0,m_beginNo);
		fMyRootInterface->set_NCPU(0,m_totalNo);
	}
	fMyRootInterface->set_DirNames(0,m_InputDir);
	fMyRootInterface->set_OutputName(m_runName);
	fMyRootInterface->init();

	//************SET Statistics********************
	if (verbose >= Verbose_SectorInfo ) std::cout<<prefix_SectorInfo<<"In SET Statistics###"<<std::endl;
	//=>About Statistical
	init_Ncut();

	// For output
	int pid;
	int tid;
	int ppid;
	double x;
	double y;
	double z;
	double px;
	double py;
	double pz;
	double t;
	std::string particle;
	std::string process;
	std::string volume;
	std::string *p_particle = 0;
	std::string *p_process = 0;
	std::string *p_volume = 0;
	double ot;
	double ox;
	double oy;
	double oz;
	double opx;
	double opy;
	double opz;

	// For input
	int evt_num;
	int run_num;
	int Monitor_nHits = 0;
	std::vector<int> Monitor_tid;
	std::vector<int> Monitor_pid;
	std::vector<int> Monitor_ppid;
	std::vector<std::string> Monitor_oprocess;
	std::vector<std::string> Monitor_ovolName;
	std::vector<double> Monitor_t;
	std::vector<double> Monitor_x;
	std::vector<double> Monitor_y;
	std::vector<double> Monitor_z;
	std::vector<double> Monitor_px;
	std::vector<double> Monitor_py;
	std::vector<double> Monitor_pz;
	std::vector<double> Monitor_ox;
	std::vector<double> Monitor_oy;
	std::vector<double> Monitor_oz;
	std::vector<double> Monitor_opx;
	std::vector<double> Monitor_opy;
	std::vector<double> Monitor_opz;

	int McTruth_nTracks = 0;
	std::vector<int> McTruth_tid;
	std::vector<int> McTruth_pid;
	std::vector<int> McTruth_charge;
	std::vector<int> McTruth_ptid;
	std::vector<std::string> McTruth_particleName;
	std::vector<std::string> McTruth_process;
	std::vector<std::string> McTruth_volume;
	std::vector<double> McTruth_time;
	std::vector<double> McTruth_x;
	std::vector<double> McTruth_y;
	std::vector<double> McTruth_z;
	std::vector<double> McTruth_px;
	std::vector<double> McTruth_py;
	std::vector<double> McTruth_pz;

	int CDCMonitor_nHits = 0;
	std::vector<int> CDCMonitor_tid;
	std::vector<int> CDCMonitor_pid;
	std::vector<int> CDCMonitor_ppid;
	std::vector<double> CDCMonitor_t;
	std::vector<double> CDCMonitor_x;
	std::vector<double> CDCMonitor_y;
	std::vector<double> CDCMonitor_z;
	std::vector<double> CDCMonitor_px;
	std::vector<double> CDCMonitor_py;
	std::vector<double> CDCMonitor_pz;
	std::vector<double> CDCMonitor_ox;
	std::vector<double> CDCMonitor_oy;
	std::vector<double> CDCMonitor_oz;
	std::vector<double> CDCMonitor_opx;
	std::vector<double> CDCMonitor_opy;
	std::vector<double> CDCMonitor_opz;
	std::vector<double> CDCMonitor_oprocess;
	std::vector<double> CDCMonitor_ovolName;
	int Trigger_nHits = 0;
	std::vector<int> Trigger_tid;
	std::vector<int> Trigger_pid;
	std::vector<int> Trigger_ppid;
	std::vector<double> Trigger_t;
	std::vector<double> Trigger_x;
	std::vector<double> Trigger_y;
	std::vector<double> Trigger_z;
	std::vector<double> Trigger_px;
	std::vector<double> Trigger_py;
	std::vector<double> Trigger_pz;
	std::vector<double> Trigger_ox;
	std::vector<double> Trigger_oy;
	std::vector<double> Trigger_oz;
	std::vector<double> Trigger_opx;
	std::vector<double> Trigger_opy;
	std::vector<double> Trigger_opz;
	std::vector<double> Trigger_oprocess;
	std::vector<double> Trigger_ovolName;
	int InnerCylinder_nHits = 0;
	std::vector<int> InnerCylinder_tid;
	std::vector<int> InnerCylinder_pid;
	std::vector<int> InnerCylinder_ppid;
	std::vector<double> InnerCylinder_t;
	std::vector<double> InnerCylinder_x;
	std::vector<double> InnerCylinder_y;
	std::vector<double> InnerCylinder_z;
	std::vector<double> InnerCylinder_px;
	std::vector<double> InnerCylinder_py;
	std::vector<double> InnerCylinder_pz;
	std::vector<double> InnerCylinder_ox;
	std::vector<double> InnerCylinder_oy;
	std::vector<double> InnerCylinder_oz;
	std::vector<double> InnerCylinder_opx;
	std::vector<double> InnerCylinder_opy;
	std::vector<double> InnerCylinder_opz;
	std::vector<double> InnerCylinder_oprocess;
	std::vector<double> InnerCylinder_ovolName;
	int BLTMonitor_nHits = 0;
	std::vector<int> BLTMonitor_tid;
	std::vector<int> BLTMonitor_pid;
	std::vector<int> BLTMonitor_ppid;
	std::vector<double> BLTMonitor_t;
	std::vector<double> BLTMonitor_x;
	std::vector<double> BLTMonitor_y;
	std::vector<double> BLTMonitor_z;
	std::vector<double> BLTMonitor_px;
	std::vector<double> BLTMonitor_py;
	std::vector<double> BLTMonitor_pz;
	std::vector<double> BLTMonitor_ox;
	std::vector<double> BLTMonitor_oy;
	std::vector<double> BLTMonitor_oz;
	std::vector<double> BLTMonitor_opx;
	std::vector<double> BLTMonitor_opy;
	std::vector<double> BLTMonitor_opz;
	std::vector<double> BLTMonitor_oprocess;
	std::vector<double> BLTMonitor_ovolName;
	int Target_nHits = 0;
	std::vector<int> Target_tid;
	std::vector<int> Target_pid;
	std::vector<int> Target_ppid;
	std::vector<double> Target_t;
	std::vector<double> Target_x;
	std::vector<double> Target_y;
	std::vector<double> Target_z;
	std::vector<double> Target_px;
	std::vector<double> Target_py;
	std::vector<double> Target_pz;
	std::vector<int> Target_stopped;
	std::vector<double> Target_ox;
	std::vector<double> Target_oy;
	std::vector<double> Target_oz;
	std::vector<double> Target_opx;
	std::vector<double> Target_opy;
	std::vector<double> Target_opz;
	std::vector<double> Target_oprocess;
	std::vector<double> Target_ovolName;

	//**********************************************************************************************
	TChain* m_TChain = new TChain("t");
	int m_OriginalNum=0;
	if (m_OriginalFile!="NONE"){ // we need original file to get original information for primary particles. e.g. MT1 & A9
		m_TChain->Add(m_OriginalFile.c_str());
		m_OriginalNum = m_TChain->GetEntries();
		if (m_workMode=="monitor"){
			m_TChain->SetBranchAddress("t",&ot);
		}
		else if (m_workMode=="A9"||m_workMode=="A9pim"){
			m_TChain->SetBranchAddress("ot",&ot);
		}
		m_TChain->SetBranchAddress("ox",&ox);
		m_TChain->SetBranchAddress("oy",&oy);
		m_TChain->SetBranchAddress("oz",&oz);
		m_TChain->SetBranchAddress("opx",&opx);
		m_TChain->SetBranchAddress("opy",&opy);
		m_TChain->SetBranchAddress("opz",&opz);
		m_TChain->SetBranchAddress("particle",&p_particle);
		m_TChain->SetBranchAddress("process",&p_process);
		m_TChain->SetBranchAddress("volume",&p_volume);
		particle=*p_particle;
		process=*p_process;
		volume=*p_volume;
		if (verbose >= Verbose_SectorInfo ) std::cout<<prefix_SectorInfo<<"Need original file, m_OriginalNum = "<<m_OriginalNum<<std::endl;
	}
	//**********************************************************************************************

	//=======================================================================================================
	//************DO THE DIRTY WORK*******************
	if (verbose >= Verbose_SectorInfo ) std::cout<<prefix_SectorInfo<<"In DO THE DIRTY WORK###"<<std::endl;
	Long64_t nEvent = fMyRootInterface->get_Entries();
	for( Long64_t iEvent = 0; iEvent < (nEvents&&nEvents<nEvent?nEvents:nEvent); iEvent++ ){
		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"In Event "<<iEvent<<std::endl;
		fMyRootInterface->GetEntry(iEvent);
		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Got entries"<<std::endl;
		if (m_OriginalFile!="NONE"){ // we need original file to get original information for primary particles. e.g. MT1 & A9
			if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"m_TChain->GetEntry("<<iEvent%m_OriginalNum<<")"<<std::endl;
			m_TChain->GetEntry(iEvent%m_OriginalNum);
			if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Got entries for original file"<<std::endl;
			ot*=ns;
			ox*=mm;
			oy*=mm;
			oz*=mm;
			opx*=MeV;
			opy*=MeV;
			opz*=MeV;
		}
		inc_Ncut("Got entries");
		if (iEvent%printModule==0){
			fMyRootInterface->Write();
			if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Written file"<<std::endl;
		}

		// Get info

		fMyRootInterface->get_value("evt_num",evt_num);
		fMyRootInterface->get_value("run_num",run_num);

		if (m_workMode=="McTruth"){
			if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Getting info for McTruth"<<std::endl;
			fMyRootInterface->get_value("McTruth_nTracks",McTruth_nTracks);
			fMyRootInterface->get_value("McTruth_time",McTruth_time,ns);
			fMyRootInterface->get_value("McTruth_tid",McTruth_tid);
			fMyRootInterface->get_value("McTruth_pid",McTruth_pid);
			fMyRootInterface->get_value("McTruth_charge",McTruth_charge);
			fMyRootInterface->get_value("McTruth_ptid",McTruth_ptid);
			fMyRootInterface->get_value("McTruth_particleName",McTruth_particleName);
			fMyRootInterface->get_value("McTruth_process",McTruth_process);
			fMyRootInterface->get_value("McTruth_volume",McTruth_volume);
			fMyRootInterface->get_value("McTruth_x",McTruth_x,cm);
			fMyRootInterface->get_value("McTruth_y",McTruth_y,cm);
			fMyRootInterface->get_value("McTruth_z",McTruth_z,cm);
			fMyRootInterface->get_value("McTruth_px",McTruth_px,GeV);
			fMyRootInterface->get_value("McTruth_py",McTruth_py,GeV);
			fMyRootInterface->get_value("McTruth_pz",McTruth_pz,GeV);
		}
		else if (m_workMode=="A9"||m_workMode=="A9pim"){
			fMyRootInterface->get_value("McTruth_nTracks",McTruth_nTracks);
			fMyRootInterface->get_value("McTruth_time",McTruth_time,ns);
			fMyRootInterface->get_value("McTruth_x",McTruth_x,cm);
			fMyRootInterface->get_value("McTruth_y",McTruth_y,cm);
			fMyRootInterface->get_value("McTruth_z",McTruth_z,cm);
			fMyRootInterface->get_value("McTruth_px",McTruth_px,GeV);
			fMyRootInterface->get_value("McTruth_py",McTruth_py,GeV);
			fMyRootInterface->get_value("McTruth_pz",McTruth_pz,GeV);
			fMyRootInterface->get_value("CDCMonitor_nHits",CDCMonitor_nHits);
			fMyRootInterface->get_value("CDCMonitor_t",CDCMonitor_t,ns);
			fMyRootInterface->get_value("CDCMonitor_tid",CDCMonitor_tid);
			fMyRootInterface->get_value("CDCMonitor_pid",CDCMonitor_pid);
			fMyRootInterface->get_value("CDCMonitor_ppid",CDCMonitor_ppid);
			fMyRootInterface->get_value("CDCMonitor_oprocess",CDCMonitor_oprocess);
			fMyRootInterface->get_value("CDCMonitor_ovolName",CDCMonitor_ovolName);
			fMyRootInterface->get_value("CDCMonitor_ox",CDCMonitor_ox,cm);
			fMyRootInterface->get_value("CDCMonitor_oy",CDCMonitor_oy,cm);
			fMyRootInterface->get_value("CDCMonitor_oz",CDCMonitor_oz,cm);
			fMyRootInterface->get_value("CDCMonitor_opx",CDCMonitor_opx,GeV);
			fMyRootInterface->get_value("CDCMonitor_opy",CDCMonitor_opy,GeV);
			fMyRootInterface->get_value("CDCMonitor_opz",CDCMonitor_opz,GeV);
			fMyRootInterface->get_value("CDCMonitor_x",CDCMonitor_x,cm);
			fMyRootInterface->get_value("CDCMonitor_y",CDCMonitor_y,cm);
			fMyRootInterface->get_value("CDCMonitor_z",CDCMonitor_z,cm);
			fMyRootInterface->get_value("CDCMonitor_px",CDCMonitor_px,GeV);
			fMyRootInterface->get_value("CDCMonitor_py",CDCMonitor_py,GeV);
			fMyRootInterface->get_value("CDCMonitor_pz",CDCMonitor_pz,GeV);
			fMyRootInterface->get_value("BLTMonitor_nHits",BLTMonitor_nHits);
			fMyRootInterface->get_value("BLTMonitor_t",BLTMonitor_t,ns);
			fMyRootInterface->get_value("BLTMonitor_tid",BLTMonitor_tid);
			fMyRootInterface->get_value("BLTMonitor_pid",BLTMonitor_pid);
			fMyRootInterface->get_value("BLTMonitor_ppid",BLTMonitor_ppid);
			fMyRootInterface->get_value("BLTMonitor_oprocess",BLTMonitor_oprocess);
			fMyRootInterface->get_value("BLTMonitor_ovolName",BLTMonitor_ovolName);
			fMyRootInterface->get_value("BLTMonitor_ox",BLTMonitor_ox,cm);
			fMyRootInterface->get_value("BLTMonitor_oy",BLTMonitor_oy,cm);
			fMyRootInterface->get_value("BLTMonitor_oz",BLTMonitor_oz,cm);
			fMyRootInterface->get_value("BLTMonitor_opx",BLTMonitor_opx,GeV);
			fMyRootInterface->get_value("BLTMonitor_opy",BLTMonitor_opy,GeV);
			fMyRootInterface->get_value("BLTMonitor_opz",BLTMonitor_opz,GeV);
			fMyRootInterface->get_value("BLTMonitor_x",BLTMonitor_x,cm);
			fMyRootInterface->get_value("BLTMonitor_y",BLTMonitor_y,cm);
			fMyRootInterface->get_value("BLTMonitor_z",BLTMonitor_z,cm);
			fMyRootInterface->get_value("BLTMonitor_px",BLTMonitor_px,GeV);
			fMyRootInterface->get_value("BLTMonitor_py",BLTMonitor_py,GeV);
			fMyRootInterface->get_value("BLTMonitor_pz",BLTMonitor_pz,GeV);
			fMyRootInterface->get_value("InnerCylinder_nHits",InnerCylinder_nHits);
			fMyRootInterface->get_value("InnerCylinder_t",InnerCylinder_t,ns);
			fMyRootInterface->get_value("InnerCylinder_tid",InnerCylinder_tid);
			fMyRootInterface->get_value("InnerCylinder_pid",InnerCylinder_pid);
			fMyRootInterface->get_value("InnerCylinder_ppid",InnerCylinder_ppid);
			fMyRootInterface->get_value("InnerCylinder_oprocess",InnerCylinder_oprocess);
			fMyRootInterface->get_value("InnerCylinder_ovolName",InnerCylinder_ovolName);
			fMyRootInterface->get_value("InnerCylinder_ox",InnerCylinder_ox,cm);
			fMyRootInterface->get_value("InnerCylinder_oy",InnerCylinder_oy,cm);
			fMyRootInterface->get_value("InnerCylinder_oz",InnerCylinder_oz,cm);
			fMyRootInterface->get_value("InnerCylinder_opx",InnerCylinder_opx,GeV);
			fMyRootInterface->get_value("InnerCylinder_opy",InnerCylinder_opy,GeV);
			fMyRootInterface->get_value("InnerCylinder_opz",InnerCylinder_opz,GeV);
			fMyRootInterface->get_value("InnerCylinder_x",InnerCylinder_x,cm);
			fMyRootInterface->get_value("InnerCylinder_y",InnerCylinder_y,cm);
			fMyRootInterface->get_value("InnerCylinder_z",InnerCylinder_z,cm);
			fMyRootInterface->get_value("InnerCylinder_px",InnerCylinder_px,GeV);
			fMyRootInterface->get_value("InnerCylinder_py",InnerCylinder_py,GeV);
			fMyRootInterface->get_value("InnerCylinder_pz",InnerCylinder_pz,GeV);
			fMyRootInterface->get_value("Trigger_nHits",Trigger_nHits);
			fMyRootInterface->get_value("Trigger_t",Trigger_t,ns);
			fMyRootInterface->get_value("Trigger_tid",Trigger_tid);
			fMyRootInterface->get_value("Trigger_pid",Trigger_pid);
			fMyRootInterface->get_value("Trigger_ppid",Trigger_ppid);
			fMyRootInterface->get_value("Trigger_oprocess",Trigger_oprocess);
			fMyRootInterface->get_value("Trigger_ovolName",Trigger_ovolName);
			fMyRootInterface->get_value("Trigger_ox",Trigger_ox,cm);
			fMyRootInterface->get_value("Trigger_oy",Trigger_oy,cm);
			fMyRootInterface->get_value("Trigger_oz",Trigger_oz,cm);
			fMyRootInterface->get_value("Trigger_opx",Trigger_opx,GeV);
			fMyRootInterface->get_value("Trigger_opy",Trigger_opy,GeV);
			fMyRootInterface->get_value("Trigger_opz",Trigger_opz,GeV);
			fMyRootInterface->get_value("Trigger_x",Trigger_x,cm);
			fMyRootInterface->get_value("Trigger_y",Trigger_y,cm);
			fMyRootInterface->get_value("Trigger_z",Trigger_z,cm);
			fMyRootInterface->get_value("Trigger_px",Trigger_px,GeV);
			fMyRootInterface->get_value("Trigger_py",Trigger_py,GeV);
			fMyRootInterface->get_value("Trigger_pz",Trigger_pz,GeV);
			fMyRootInterface->get_value("Target_nHits",Target_nHits);
			fMyRootInterface->get_value("Target_t",Target_t,ns);
			fMyRootInterface->get_value("Target_tid",Target_tid);
			fMyRootInterface->get_value("Target_pid",Target_pid);
			fMyRootInterface->get_value("Target_ppid",Target_ppid);
			fMyRootInterface->get_value("Target_oprocess",Target_oprocess);
			fMyRootInterface->get_value("Target_ovolName",Target_ovolName);
			fMyRootInterface->get_value("Target_ox",Target_ox,cm);
			fMyRootInterface->get_value("Target_oy",Target_oy,cm);
			fMyRootInterface->get_value("Target_oz",Target_oz,cm);
			fMyRootInterface->get_value("Target_opx",Target_opx,GeV);
			fMyRootInterface->get_value("Target_opy",Target_opy,GeV);
			fMyRootInterface->get_value("Target_opz",Target_opz,GeV);
			fMyRootInterface->get_value("Target_x",Target_x,cm);
			fMyRootInterface->get_value("Target_y",Target_y,cm);
			fMyRootInterface->get_value("Target_z",Target_z,cm);
			fMyRootInterface->get_value("Target_px",Target_px,GeV);
			fMyRootInterface->get_value("Target_py",Target_py,GeV);
			fMyRootInterface->get_value("Target_pz",Target_pz,GeV);
			fMyRootInterface->get_value("Target_stopped",Target_stopped);
		}
		else if (m_workMode=="monitor"){
			fMyRootInterface->get_value(m_MonitorPlane+"Monitor_nHits",Monitor_nHits);
			fMyRootInterface->get_value(m_MonitorPlane+"Monitor_t",Monitor_t,ns);
			fMyRootInterface->get_value(m_MonitorPlane+"Monitor_tid",Monitor_tid);
			fMyRootInterface->get_value(m_MonitorPlane+"Monitor_pid",Monitor_pid);
			fMyRootInterface->get_value(m_MonitorPlane+"Monitor_ppid",Monitor_ppid);
			fMyRootInterface->get_value(m_MonitorPlane+"Monitor_oprocess",Monitor_oprocess);
			fMyRootInterface->get_value(m_MonitorPlane+"Monitor_ovolName",Monitor_ovolName);
			fMyRootInterface->get_value(m_MonitorPlane+"Monitor_ox",Monitor_ox,cm);
			fMyRootInterface->get_value(m_MonitorPlane+"Monitor_oy",Monitor_oy,cm);
			fMyRootInterface->get_value(m_MonitorPlane+"Monitor_oz",Monitor_oz,cm);
			fMyRootInterface->get_value(m_MonitorPlane+"Monitor_opx",Monitor_opx,GeV);
			fMyRootInterface->get_value(m_MonitorPlane+"Monitor_opy",Monitor_opy,GeV);
			fMyRootInterface->get_value(m_MonitorPlane+"Monitor_opz",Monitor_opz,GeV);
			fMyRootInterface->get_value(m_MonitorPlane+"Monitor_x",Monitor_x,cm);
			fMyRootInterface->get_value(m_MonitorPlane+"Monitor_y",Monitor_y,cm);
			fMyRootInterface->get_value(m_MonitorPlane+"Monitor_z",Monitor_z,cm);
			fMyRootInterface->get_value(m_MonitorPlane+"Monitor_px",Monitor_px,GeV);
			fMyRootInterface->get_value(m_MonitorPlane+"Monitor_py",Monitor_py,GeV);
			fMyRootInterface->get_value(m_MonitorPlane+"Monitor_pz",Monitor_pz,GeV);
		}

		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Got info"<<std::endl;

		// find particles
		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"###Getting information"<<std::endl;

		bool filled = false;
		if (m_workMode=="monitor"){
			for ( int i_mon = 0; i_mon < Monitor_nHits; i_mon++ ){
				if (!PDGEncoding // all particles
					||PDGEncoding==-1&&Monitor_pid[i_mon]>=1e7 // only nuclears
					||PDGEncoding==1&&Monitor_pid[i_mon]<1e7 // only elementary particles
					||Monitor_pid[i_mon] == PDGEncoding){
					if (verbose >= Verbose_ParticleInfo || iEvent%printModule == 0)
						std::cout<<prefix_ParticleInfoStart
								 <<"  Found Particle! i_mon = "<<i_mon
								 <<", pid = "<<Monitor_pid[i_mon]
								 <<", tid = "<<Monitor_tid[i_mon]
								 <<", px = "<<Monitor_px[i_mon]
								 <<"MeV, py = "<<Monitor_py[i_mon] <<"MeV, pz = "<<Monitor_pz[i_mon] <<"MeV"
								 <<std::endl;
					// Fill
					pid=Monitor_pid[i_mon];
					tid=Monitor_tid[i_mon];
					ppid=Monitor_ppid[i_mon];
					x=Monitor_x[i_mon];
					y=Monitor_y[i_mon];
					z=Monitor_z[i_mon];
					px=Monitor_px[i_mon];
					py=Monitor_py[i_mon];
					pz=Monitor_pz[i_mon];
					t=Monitor_t[i_mon];
					if (tid!=1||m_OriginalFile=="NONE"){ // we don't need original file to get original information for primary particles. e.g. PTACS
						ot=-1;
						ox=Monitor_ox[i_mon];
						oy=Monitor_oy[i_mon];
						oz=Monitor_oz[i_mon];
						opx=Monitor_opx[i_mon];
						opy=Monitor_opy[i_mon];
						opz=Monitor_opz[i_mon];
						std::cout<<"Monitor_oprocess["<<i_mon<<"] = \""<<Monitor_oprocess[i_mon]<<"\", @ ["<<(void*)&(Monitor_oprocess[i_mon])<<"]"<<std::endl;
						particle="";
						process=Monitor_oprocess[i_mon];
						volume=Monitor_ovolName[i_mon];
					}
					fMyRootInterface->set_ovalue("evt_num",evt_num);
					fMyRootInterface->set_ovalue("run_num",run_num);
					fMyRootInterface->set_ovalue("pid",pid);
					fMyRootInterface->set_ovalue("tid",tid);
					fMyRootInterface->set_ovalue("ppid",ppid);
					fMyRootInterface->set_ovalue("x",x/mm);
					fMyRootInterface->set_ovalue("y",y/mm);
					fMyRootInterface->set_ovalue("z",z/mm);
					fMyRootInterface->set_ovalue("px",px/MeV);
					fMyRootInterface->set_ovalue("py",py/MeV);
					fMyRootInterface->set_ovalue("pz",pz/MeV);
					fMyRootInterface->set_ovalue("t",t/ns);
					fMyRootInterface->set_ovalue("ot",ot/ns);
					fMyRootInterface->set_ovalue("ox",ox/mm);
					fMyRootInterface->set_ovalue("oy",oy/mm);
					fMyRootInterface->set_ovalue("oz",oz/mm);
					fMyRootInterface->set_ovalue("opx",opx/MeV);
					fMyRootInterface->set_ovalue("opy",opy/MeV);
					fMyRootInterface->set_ovalue("opz",opz/MeV);
					fMyRootInterface->set_ovalue("particle",particle);
					fMyRootInterface->set_ovalue("process",process);
					fMyRootInterface->set_ovalue("volume",volume);
					if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Set oTrees"<<std::endl;
					fMyRootInterface->Fill();
					if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Filled"<<std::endl;
					filled=true;
				}
			}
		}
		else if (m_workMode=="A9pim"){
			if (McTruth_nTracks>0){
				double Mc_px = McTruth_px[0];
				double Mc_py = McTruth_py[0];
				double Mc_pz = McTruth_pz[0];
				double Mc_pa = sqrt(Mc_px*Mc_px+Mc_py*Mc_py+Mc_pz*Mc_pz);
				double Mc_pt = sqrt(Mc_px*Mc_px+Mc_py*Mc_py);
				double Mc_y = McTruth_y[0];
				double Mc_time = McTruth_time[0];

				// Get weight
				double Mc_w = 0;
				double CDC_w = 0;
				double BLT_w = 0;
				double Target_w = 0;

				double Target_time;
				double BLT_time;
				double CDC_time;

				double E = sqrt(Mc_pa*Mc_pa+M_PION*M_PION);
				double Beta = Mc_pa/E;
				double Gamma = sqrt(1./(1.-Beta*Beta));
				Mc_w = exp(-(Mc_time-ot)/tau/Gamma);
				// Get info
				if ( (index_temp = fMyRootInterface->get_TH1D_index("pa_total")) != -1 ){
					fMyRootInterface->get_TH1D(index_temp)->Fill(Mc_pa/MeV,Mc_w);
				}
				if ( (index_temp = fMyRootInterface->get_TH1D_index("pt_total")) != -1 ){
					fMyRootInterface->get_TH1D(index_temp)->Fill(Mc_pt/MeV,Mc_w);
				}
				if ( (index_temp = fMyRootInterface->get_TH1D_index("pz_total")) != -1 ){
					fMyRootInterface->get_TH1D(index_temp)->Fill(Mc_pz/MeV,Mc_w);
				}
				if ( (index_temp = fMyRootInterface->get_TH1D_index("y_total")) != -1 ){
					fMyRootInterface->get_TH1D(index_temp)->Fill(Mc_y/mm,Mc_w);
				}
				if ( (index_temp = fMyRootInterface->get_TH2D_index("paVSy_total")) != -1 ){
					fMyRootInterface->get_TH2D(index_temp)->Fill(Mc_pa/MeV,Mc_y/mm,Mc_w);
				}
				// Got Cdc Region?
				bool got_CDC = false;
				for ( int i_mon = 0; i_mon < CDCMonitor_nHits; i_mon++ ){
					if (CDCMonitor_tid[i_mon]==1){
						got_CDC = true;
						CDC_time = CDCMonitor_t[i_mon];
						CDC_w = exp(-(CDC_time-ot)/tau/Gamma);
						break;
					}
				}
				// Got Target Region?
				// Got stopped?
				bool got_Target = false;
				bool got_stopped = false;
				for ( int i_mon = 0; i_mon < Target_nHits; i_mon++ ){
					if (Target_tid[i_mon]==1){
						got_Target = true;
						if (Target_stopped[i_mon]){
							got_stopped = true;
							Target_time = Target_t[i_mon];
							Target_w = exp(-(Target_time-ot)/tau/Gamma);
							break;
						}
					}
					else if (Target_pid[i_mon]==13){
						std::cout<<"Hey a moun from else where!"<<std::endl;
						std::cout<<"  px = "<<Target_px[i_mon]/MeV<<"MeV"
								 <<", py = "<<Target_py[i_mon]/MeV<<"MeV"
								 <<", pz = "<<Target_pz[i_mon]/MeV<<"MeV"
								 <<", tid = "<<Target_tid[i_mon]
								 <<", stopped?"<<Target_stopped[i_mon]
								 <<std::endl;
					}
				}
				// Got recoiled?
				bool got_recoiled = false;
				for ( int i_mon = 0; i_mon < BLTMonitor_nHits; i_mon++ ){
					if (BLTMonitor_tid[i_mon]==1&&BLTMonitor_pz[i_mon]<0){
						got_recoiled = true;
						BLT_time = BLTMonitor_t[i_mon];
						BLT_w = exp(-(BLT_time-ot)/tau/Gamma);
						break;
					}
				}
				if (got_CDC){

					if ( (index_temp = fMyRootInterface->get_TH1D_index("pa_CDC")) != -1 ){
						fMyRootInterface->get_TH1D(index_temp)->Fill(Mc_pa/MeV,CDC_w);
					}
					if ( (index_temp = fMyRootInterface->get_TH1D_index("pt_CDC")) != -1 ){
						fMyRootInterface->get_TH1D(index_temp)->Fill(Mc_pt/MeV,CDC_w);
					}
					if ( (index_temp = fMyRootInterface->get_TH1D_index("pz_CDC")) != -1 ){
						fMyRootInterface->get_TH1D(index_temp)->Fill(Mc_pz/MeV,CDC_w);
					}
					if ( (index_temp = fMyRootInterface->get_TH1D_index("y_CDC")) != -1 ){
						fMyRootInterface->get_TH1D(index_temp)->Fill(Mc_y/mm,CDC_w);
					}
					if ( (index_temp = fMyRootInterface->get_TH2D_index("paVSy_CDC")) != -1 ){
						fMyRootInterface->get_TH2D(index_temp)->Fill(Mc_pa/MeV,Mc_y/mm,CDC_w);
					}
				}
				if (got_Target){
					if ( (index_temp = fMyRootInterface->get_TH1D_index("pa_Target")) != -1 ){
						fMyRootInterface->get_TH1D(index_temp)->Fill(Mc_pa/MeV,Target_w);
					}
					if ( (index_temp = fMyRootInterface->get_TH1D_index("pt_Target")) != -1 ){
						fMyRootInterface->get_TH1D(index_temp)->Fill(Mc_pt/MeV,Target_w);
					}
					if ( (index_temp = fMyRootInterface->get_TH1D_index("pz_Target")) != -1 ){
						fMyRootInterface->get_TH1D(index_temp)->Fill(Mc_pz/MeV,Target_w);
					}
					if ( (index_temp = fMyRootInterface->get_TH1D_index("y_Target")) != -1 ){
						fMyRootInterface->get_TH1D(index_temp)->Fill(Mc_y/mm,Target_w);
					}
					if ( (index_temp = fMyRootInterface->get_TH2D_index("paVSy_Target")) != -1 ){
						fMyRootInterface->get_TH2D(index_temp)->Fill(Mc_pa/MeV,Mc_y/mm,Target_w);
					}
				}
				if (got_stopped){
					if ( (index_temp = fMyRootInterface->get_TH1D_index("pa_stop")) != -1 ){
						fMyRootInterface->get_TH1D(index_temp)->Fill(Mc_pa/MeV,Target_w);
					}
					if ( (index_temp = fMyRootInterface->get_TH1D_index("pt_stop")) != -1 ){
						fMyRootInterface->get_TH1D(index_temp)->Fill(Mc_pt/MeV,Target_w);
					}
					if ( (index_temp = fMyRootInterface->get_TH1D_index("pz_stop")) != -1 ){
						fMyRootInterface->get_TH1D(index_temp)->Fill(Mc_pz/MeV,Target_w);
					}
					if ( (index_temp = fMyRootInterface->get_TH1D_index("y_stop")) != -1 ){
						fMyRootInterface->get_TH1D(index_temp)->Fill(Mc_y/mm,Target_w);
					}
					if ( (index_temp = fMyRootInterface->get_TH2D_index("paVSy_stop")) != -1 ){
						fMyRootInterface->get_TH2D(index_temp)->Fill(Mc_pa/MeV,Mc_y/mm,Target_w);
					}
				}
				if (got_recoiled){
					if ( (index_temp = fMyRootInterface->get_TH1D_index("pa_recoil")) != -1 ){
						fMyRootInterface->get_TH1D(index_temp)->Fill(Mc_pa/MeV,BLT_w);
					}
					if ( (index_temp = fMyRootInterface->get_TH1D_index("pt_recoil")) != -1 ){
						fMyRootInterface->get_TH1D(index_temp)->Fill(Mc_pt/MeV,BLT_w);
					}
					if ( (index_temp = fMyRootInterface->get_TH1D_index("pz_recoil")) != -1 ){
						fMyRootInterface->get_TH1D(index_temp)->Fill(Mc_pz/MeV,BLT_w);
					}
					if ( (index_temp = fMyRootInterface->get_TH1D_index("y_recoil")) != -1 ){
						fMyRootInterface->get_TH1D(index_temp)->Fill(Mc_y/mm,BLT_w);
					}
					if ( (index_temp = fMyRootInterface->get_TH2D_index("paVSy_recoil")) != -1 ){
						fMyRootInterface->get_TH2D(index_temp)->Fill(Mc_pa/MeV,Mc_y/mm,BLT_w);
					}
				}
			}
		}
		else if (m_workMode=="A9"){
			if (McTruth_nTracks>0){
				double Mc_px = McTruth_px[0];
				double Mc_py = McTruth_py[0];
				double Mc_pz = McTruth_pz[0];
				double Mc_pa = sqrt(Mc_px*Mc_px+Mc_py*Mc_py+Mc_pz*Mc_pz);
				double Mc_pt = sqrt(Mc_px*Mc_px+Mc_py*Mc_py);
				double Mc_y = McTruth_y[0];
				if ( (index_temp = fMyRootInterface->get_TH1D_index("pa_total")) != -1 ){
					fMyRootInterface->get_TH1D(index_temp)->Fill(Mc_pa/MeV);
				}
				if ( (index_temp = fMyRootInterface->get_TH1D_index("pt_total")) != -1 ){
					fMyRootInterface->get_TH1D(index_temp)->Fill(Mc_pt/MeV);
				}
				if ( (index_temp = fMyRootInterface->get_TH1D_index("pz_total")) != -1 ){
					fMyRootInterface->get_TH1D(index_temp)->Fill(Mc_pz/MeV);
				}
				if ( (index_temp = fMyRootInterface->get_TH1D_index("y_total")) != -1 ){
					fMyRootInterface->get_TH1D(index_temp)->Fill(Mc_y/mm);
				}
				if ( (index_temp = fMyRootInterface->get_TH2D_index("paVSy_total")) != -1 ){
					fMyRootInterface->get_TH2D(index_temp)->Fill(Mc_pa/MeV,Mc_y/mm);
				}
				// Got Cdc Region?
				bool got_CDC = false;
				for ( int i_mon = 0; i_mon < CDCMonitor_nHits; i_mon++ ){
					if (CDCMonitor_tid[i_mon]==1&&CDCMonitor_pid[i_mon]==PDGEncoding)
						got_CDC = true;
				}
				// Got Target Region?
				// Got stopped?
				bool got_Target = false;
				bool got_stopped = false;
				for ( int i_mon = 0; i_mon < Target_nHits; i_mon++ ){
					if (Target_tid[i_mon]==1&&Target_pid[i_mon]==PDGEncoding){
						got_Target = true;
						if (Target_stopped[i_mon]){
							got_stopped = true;
						}
					}
					else if (Target_pid[i_mon]==PDGEncoding){
						//std::cout<<"Hey a particle from else where!"<<std::endl;
						//std::cout<<"  px = "<<Target_px[i_mon]/MeV<<"MeV"
						//         <<", py = "<<Target_py[i_mon]/MeV<<"MeV"
						//         <<", pz = "<<Target_pz[i_mon]/MeV<<"MeV"
						//         <<", tid = "<<Target_tid[i_mon]
						//         <<", pid = "<<Target_pid[i_mon]
						//         <<", stopped?"<<Target_stopped[i_mon]
						//         <<std::endl;
					}
				}
				// Got recoiled?
				bool got_recoiled = false;
				for ( int i_mon = 0; i_mon < BLTMonitor_nHits; i_mon++ ){
					if (BLTMonitor_tid[i_mon]==1&&BLTMonitor_pid[i_mon]==PDGEncoding&&BLTMonitor_pz[i_mon]<0)
						got_recoiled = true;
				}
				if (got_CDC){
					if ( (index_temp = fMyRootInterface->get_TH1D_index("pa_CDC")) != -1 ){
						fMyRootInterface->get_TH1D(index_temp)->Fill(Mc_pa/MeV);
					}
					if ( (index_temp = fMyRootInterface->get_TH1D_index("pt_CDC")) != -1 ){
						fMyRootInterface->get_TH1D(index_temp)->Fill(Mc_pt/MeV);
					}
					if ( (index_temp = fMyRootInterface->get_TH1D_index("pz_CDC")) != -1 ){
						fMyRootInterface->get_TH1D(index_temp)->Fill(Mc_pz/MeV);
					}
					if ( (index_temp = fMyRootInterface->get_TH1D_index("y_CDC")) != -1 ){
						fMyRootInterface->get_TH1D(index_temp)->Fill(Mc_y/mm);
					}
					if ( (index_temp = fMyRootInterface->get_TH2D_index("paVSy_CDC")) != -1 ){
						fMyRootInterface->get_TH2D(index_temp)->Fill(Mc_pa/MeV,Mc_y/mm);
					}
				}
				if (got_Target){
					if ( (index_temp = fMyRootInterface->get_TH1D_index("pa_Target")) != -1 ){
						fMyRootInterface->get_TH1D(index_temp)->Fill(Mc_pa/MeV);
					}
					if ( (index_temp = fMyRootInterface->get_TH1D_index("pt_Target")) != -1 ){
						fMyRootInterface->get_TH1D(index_temp)->Fill(Mc_pt/MeV);
					}
					if ( (index_temp = fMyRootInterface->get_TH1D_index("pz_Target")) != -1 ){
						fMyRootInterface->get_TH1D(index_temp)->Fill(Mc_pz/MeV);
					}
					if ( (index_temp = fMyRootInterface->get_TH1D_index("y_Target")) != -1 ){
						fMyRootInterface->get_TH1D(index_temp)->Fill(Mc_y/mm);
					}
					if ( (index_temp = fMyRootInterface->get_TH2D_index("paVSy_Target")) != -1 ){
						fMyRootInterface->get_TH2D(index_temp)->Fill(Mc_pa/MeV,Mc_y/mm);
					}
				}
				if (got_stopped){
					if ( (index_temp = fMyRootInterface->get_TH1D_index("pa_stop")) != -1 ){
						fMyRootInterface->get_TH1D(index_temp)->Fill(Mc_pa/MeV);
					}
					if ( (index_temp = fMyRootInterface->get_TH1D_index("pt_stop")) != -1 ){
						fMyRootInterface->get_TH1D(index_temp)->Fill(Mc_pt/MeV);
					}
					if ( (index_temp = fMyRootInterface->get_TH1D_index("pz_stop")) != -1 ){
						fMyRootInterface->get_TH1D(index_temp)->Fill(Mc_pz/MeV);
					}
					if ( (index_temp = fMyRootInterface->get_TH1D_index("y_stop")) != -1 ){
						fMyRootInterface->get_TH1D(index_temp)->Fill(Mc_y/mm);
					}
					if ( (index_temp = fMyRootInterface->get_TH2D_index("paVSy_stop")) != -1 ){
						fMyRootInterface->get_TH2D(index_temp)->Fill(Mc_pa/MeV,Mc_y/mm);
					}
				}
				if (got_recoiled){
					if ( (index_temp = fMyRootInterface->get_TH1D_index("pa_recoil")) != -1 ){
						fMyRootInterface->get_TH1D(index_temp)->Fill(Mc_pa/MeV);
					}
					if ( (index_temp = fMyRootInterface->get_TH1D_index("pt_recoil")) != -1 ){
						fMyRootInterface->get_TH1D(index_temp)->Fill(Mc_pt/MeV);
					}
					if ( (index_temp = fMyRootInterface->get_TH1D_index("pz_recoil")) != -1 ){
						fMyRootInterface->get_TH1D(index_temp)->Fill(Mc_pz/MeV);
					}
					if ( (index_temp = fMyRootInterface->get_TH1D_index("y_recoil")) != -1 ){
						fMyRootInterface->get_TH1D(index_temp)->Fill(Mc_y/mm);
					}
					if ( (index_temp = fMyRootInterface->get_TH2D_index("paVSy_recoil")) != -1 ){
						fMyRootInterface->get_TH2D(index_temp)->Fill(Mc_pa/MeV,Mc_y/mm);
					}
				}
			}
		}
		else if (m_workMode=="McTruth"){
			for ( int iMc = 0; iMc < McTruth_nTracks; iMc++ ){
				if (!PDGEncoding // all particles
					||PDGEncoding==-1&&McTruth_pid[iMc]>=1e7 // only nuclears
					||PDGEncoding==1&&McTruth_pid[iMc]<1e7 // only elementary particles
					||McTruth_pid[iMc] == PDGEncoding){
					if (verbose >= Verbose_ParticleInfo || iEvent%printModule == 0)
						std::cout<<prefix_ParticleInfoStart
								 <<"  Found Particle! iMc = "<<iMc
								 <<", pid = "<<McTruth_pid[iMc]
								 <<", tid = "<<McTruth_tid[iMc]
								 <<", px = "<<McTruth_px[iMc]
								 <<"MeV, py = "<<McTruth_py[iMc]
								 <<"MeV, pz = "<<McTruth_pz[iMc]
								 <<"MeV"
								 <<std::endl;
					double pa = sqrt(McTruth_px[iMc]*McTruth_px[iMc]+McTruth_py[iMc]*McTruth_py[iMc]+McTruth_pz[iMc]*McTruth_pz[iMc]);
					double pt = sqrt(McTruth_px[iMc]*McTruth_px[iMc]+McTruth_py[iMc]*McTruth_py[iMc]);
					double theta = acos(McTruth_pz[iMc]/pa);
					double r  = sqrt(McTruth_x[iMc]*McTruth_x[iMc]+McTruth_y[iMc]*McTruth_y[iMc]);
					if (pa<1.5*MeV) continue;
					if (McTruth_time[iMc]<1400*ns||McTruth_time[iMc]>1e9*ns) continue;
					if (McTruth_charge[iMc]==0) continue;
					fMyRootInterface->set_ovalue("pid",McTruth_pid[iMc]);
					fMyRootInterface->set_ovalue("x",McTruth_x[iMc]/mm);
					fMyRootInterface->set_ovalue("y",McTruth_y[iMc]/mm);
					fMyRootInterface->set_ovalue("z",McTruth_z[iMc]/mm);
					fMyRootInterface->set_ovalue("px",McTruth_px[iMc]/MeV);
					fMyRootInterface->set_ovalue("py",McTruth_py[iMc]/MeV);
					fMyRootInterface->set_ovalue("pz",McTruth_pz[iMc]/MeV);
					fMyRootInterface->set_ovalue("t",McTruth_time[iMc]/ns);
					fMyRootInterface->set_ovalue("particle",McTruth_particleName[iMc]);
					fMyRootInterface->set_ovalue("process",McTruth_process[iMc]);
					fMyRootInterface->set_ovalue("volume",McTruth_volume[iMc]);
					if (McTruth_pid[iMc]==11){
						fMyRootInterface->Fill("em_pa",pa/MeV);
						fMyRootInterface->Fill("em_pt",pt/MeV);
						fMyRootInterface->Fill("em_theta",theta/rad);
						fMyRootInterface->Fill("em_time",McTruth_time[iMc]/ns);
						fMyRootInterface->Fill("em_zr",McTruth_z[iMc]/mm,r/mm);
					}
					else if (McTruth_pid[iMc]==-11){
						fMyRootInterface->Fill("ep_pa",pa/MeV);
						fMyRootInterface->Fill("ep_pt",pt/MeV);
						fMyRootInterface->Fill("ep_theta",theta/rad);
						fMyRootInterface->Fill("ep_time",McTruth_time[iMc]/ns);
						fMyRootInterface->Fill("ep_zr",McTruth_z[iMc]/mm,r/mm);
					}
					else if (McTruth_pid[iMc]==13){
						fMyRootInterface->Fill("mum_pa",pa/MeV);
						fMyRootInterface->Fill("mum_pt",pt/MeV);
						fMyRootInterface->Fill("mum_theta",theta/rad);
						fMyRootInterface->Fill("mum_time",McTruth_time[iMc]/ns);
						fMyRootInterface->Fill("mum_zr",McTruth_z[iMc]/mm,r/mm);
					}
					else if (McTruth_pid[iMc]==-211){
						fMyRootInterface->Fill("pim_pa",pa/MeV);
						fMyRootInterface->Fill("pim_pt",pt/MeV);
						fMyRootInterface->Fill("pim_theta",theta/rad);
						fMyRootInterface->Fill("pim_time",McTruth_time[iMc]/ns);
						fMyRootInterface->Fill("pim_zr",McTruth_z[iMc]/mm,r/mm);
					}
					else if (McTruth_pid[iMc]==2212){
						fMyRootInterface->Fill("pp_pa",pa/MeV);
						fMyRootInterface->Fill("pp_pt",pt/MeV);
						fMyRootInterface->Fill("pp_theta",theta/rad);
						fMyRootInterface->Fill("pp_time",McTruth_time[iMc]/ns);
						fMyRootInterface->Fill("pp_zr",McTruth_z[iMc]/mm,r/mm);
					}
					if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Set oTrees"<<std::endl;
					fMyRootInterface->Fill();
					if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Filled"<<std::endl;
					filled=true;
				}
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
	m_InputDir="";
	m_OriginalFile="NONE";
	m_workMode="monitor";
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
