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
#include "TMath.h"

#include "CdcGeometryParameter.hh"
#include "MyRootInterface.hxx"

std::string m_workMode;
std::string m_prefix;
std::string m_suffix;
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
int writeModule = 10000;
int UseWeight = 0;
int PDGEncoding = 1;
double m_norm = 1;
double PulseInterval = 1170*ns;
//double PulseInterval = 1751*ns;
double nProtonPerPulse = 2.5e12/s*PulseInterval;
bool backup = false;
std::string CdcFile = "";
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
	while((result=getopt(argc,argv,"hb:t:v:n:N:m:M:r:D:O:d:p:P:w:W:x:y:i:C:"))!=-1){
		switch(result){
			/* INPUTS */
			case 'm':
				m_workMode = optarg;
				printf("Work Mode: %s\n",m_workMode.c_str());
				break;
			case 'x':
				m_prefix = optarg;
				printf("m_prefix: %s\n",m_prefix.c_str());
				break;
			case 'y':
				m_suffix = optarg;
				printf("m_suffix: %s\n",m_suffix.c_str());
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
			case 'N':
				m_norm = atof(optarg);
				printf("norm: %lf\n",m_norm);
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
			case 'w':
				writeModule = atoi(optarg);
				printf("writeModule: %d\n",writeModule);
				break;
			case 'W':
				UseWeight = atoi(optarg);
				printf("UseWeight : %d\n",UseWeight);
				break;
			case 'C':
				CdcFile = optarg;
				printf("CdcFile: %s\n",CdcFile.c_str());
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
	TH2D *h2d_temp=0;
	TH1D *h1d_temp2=0;
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
	int nHists = fMyRootInterface->get_TH1D_size();
	for (int iHist = 0; iHist < nHists; iHist++ ){
		std::string name = fMyRootInterface->get_nameForH1D(iHist);
		fMyRootInterface->set_nameForH1D(iHist,m_prefix+"_"+name+m_suffix);
	}
	nHists = fMyRootInterface->get_TH2D_size();
	for (int iHist = 0; iHist < nHists; iHist++ ){
		std::string name = fMyRootInterface->get_nameForH2D(iHist);
		fMyRootInterface->set_nameForH2D(iHist,m_prefix+"_"+name+m_suffix);
	}
	if (m_InputDir!="") fMyRootInterface->set_DirNames(0,m_InputDir);
	fMyRootInterface->set_OutputName(m_runName);
	fMyRootInterface->init();

	//************SET Statistics********************
	if (verbose >= Verbose_SectorInfo ) std::cout<<prefix_SectorInfo<<"In SET Statistics###"<<std::endl;
	//=>About Statistical
	init_Ncut();
	bool CellHitCheck[5000];
	double CellHitCount[5000][100];
	for (int i = 0; i < 5000; i++){
		for (int j = 0; j < 100; j++){
			CellHitCount[i][j] = 0;
		}
	}
	bool TriggerHitCheck[2];
	double TriggerHitCount[2][100];
	for (int i = 0; i < 2; i++){
		for (int j = 0; j < 100; j++){
			TriggerHitCount[i][j] = 0;
		}
	}

	// for initial information
	int ini_tid;
	int ini_pid;
	int ini_ppid = 0;
	double ini_t;
	double ini_x;
	double ini_y;
	double ini_z;
	double ini_px;
	double ini_py;
	double ini_pz;
	double ini_ot;
	double ini_ox;
	double ini_oy;
	double ini_oz;
	double ini_opx;
	double ini_opy;
	double ini_opz;
	std::string *p_process = 0;
	std::string *p_volume = 0;
	std::string ini_process = "NONE";
	std::string ini_volume = "NONE";
	double weight0 = 1;

	// General
	int evt_num;
	int run_num;

	// MonitorSD
	int Monitor_nHits = 0;
	std::vector<int> Monitor_tid;
	std::vector<int> Monitor_pid;
	std::vector<int> Monitor_ppid;
	std::vector<std::string> Monitor_oprocess;
	std::vector<std::string> Monitor_ovolName;
	std::vector<std::string> Monitor_volName;
	std::vector<double> Monitor_t;
	std::vector<double> Monitor_x;
	std::vector<double> Monitor_y;
	std::vector<double> Monitor_z;
	std::vector<double> Monitor_edep;
	std::vector<int>    Monitor_volID;
	std::vector<double> Monitor_px;
	std::vector<double> Monitor_py;
	std::vector<double> Monitor_pz;
	std::vector<double> Monitor_ot;
	std::vector<double> Monitor_ox;
	std::vector<double> Monitor_oy;
	std::vector<double> Monitor_oz;
	std::vector<double> Monitor_opx;
	std::vector<double> Monitor_opy;
	std::vector<double> Monitor_opz;
	std::vector<int> Monitor_charge;
	std::vector<int> Monitor_stopped;
	std::vector<double> Monitor_stop_time;

	// McTruth
	int McTruth_nTracks = 0;
	std::vector<int> McTruth_tid;
	std::vector<int> McTruth_pid;
	std::vector<int> McTruth_charge;
	std::vector<int> McTruth_ppid;
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

	// for volumes
	std::vector<std::string> Volumes;
	Volumes.push_back("CdcCell");
//	Volumes.push_back("Trigger");

	//***********************************If We Need Original File***********************************
	TChain* m_TChain = new TChain("t");
	int m_OriginalNum=0;
	if (m_OriginalFile!="NONE"){ // we need original file to get original information for primary particles. e.g. MT1 & A9
		m_TChain->Add(m_OriginalFile.c_str());
		m_OriginalNum = m_TChain->GetEntries();
		m_TChain->SetBranchAddress("t",&ini_t);
		m_TChain->SetBranchAddress("x",&ini_x);
		m_TChain->SetBranchAddress("y",&ini_y);
		m_TChain->SetBranchAddress("z",&ini_z);
		m_TChain->SetBranchAddress("px",&ini_px);
		m_TChain->SetBranchAddress("py",&ini_py);
		m_TChain->SetBranchAddress("pz",&ini_pz);
		m_TChain->SetBranchAddress("ot",&ini_ot);
		m_TChain->SetBranchAddress("ox",&ini_ox);
		m_TChain->SetBranchAddress("oy",&ini_oy);
		m_TChain->SetBranchAddress("oz",&ini_oz);
		m_TChain->SetBranchAddress("opx",&ini_opx);
		m_TChain->SetBranchAddress("opy",&ini_opy);
		m_TChain->SetBranchAddress("opz",&ini_opz);
		m_TChain->SetBranchAddress("pid",&ini_pid);
		m_TChain->SetBranchAddress("tid",&ini_tid);
		m_TChain->SetBranchAddress("ppid",&ini_ppid);
		m_TChain->SetBranchAddress("process",&p_process);
		m_TChain->SetBranchAddress("volume",&p_volume);
// FIXME: temperary way		
//		m_TChain->SetBranchAddress("weight",&weight0);
		if (verbose >= Verbose_SectorInfo ) std::cout<<prefix_SectorInfo<<"Need original file, m_OriginalNum = "<<m_OriginalNum<<std::endl;
	}
	//**********************************************************************************************

	CdcGeometryParameter *pCdcGeometryParameter = new CdcGeometryParameter("cdc");
	pCdcGeometryParameter->InitFromFile(CdcFile);
	TH1D *hProtonPuls = new TH1D("ProtonPuls","ProtonPuls",40000,-20000,20000);
	for (int i = 1; i<=40000; i++){
		double x = hProtonPuls->GetBinCenter(i);
//		double v = TMath::Gaus(x,0,1,kTRUE);
		double v = 0;
		if (x<50&&x>-50) v = 1;
		hProtonPuls->SetBinContent(i,v);
	}
	hProtonPuls->Scale(1/hProtonPuls->Integral());

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
			ini_t*=ns;
			ini_x*=mm;
			ini_y*=mm;
			ini_z*=mm;
			ini_px*=MeV;
			ini_py*=MeV;
			ini_pz*=MeV;
			ini_ot*=ns;
			ini_ox*=mm;
			ini_oy*=mm;
			ini_oz*=mm;
			ini_opx*=MeV;
			ini_opy*=MeV;
			ini_opz*=MeV;
			if (p_process)ini_process=*p_process;
			if (p_volume)ini_volume=*p_volume;
		}

		// Get info
		fMyRootInterface->get_value("evt_num",evt_num);
		fMyRootInterface->get_value("run_num",run_num);
		McTruth_nTracks = 0; // in case there's no McTruth info
		if (m_MonitorPlane=="McTruth"){
			if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Getting info for McTruth"<<std::endl;
			fMyRootInterface->get_value("McTruth_nTracks",McTruth_nTracks);
			fMyRootInterface->get_value("McTruth_time",McTruth_time,ns);
			fMyRootInterface->get_value("McTruth_tid",McTruth_tid);
			fMyRootInterface->get_value("McTruth_pid",McTruth_pid);
			fMyRootInterface->get_value("McTruth_charge",McTruth_charge);
			fMyRootInterface->get_value("McTruth_ptid",McTruth_ptid);
			fMyRootInterface->get_value("McTruth_ppid",McTruth_ppid);
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
		Monitor_nHits = 0; // in case this volume does not exist
		fMyRootInterface->get_value("C_nHits",Monitor_nHits);
		fMyRootInterface->get_value("C_t",Monitor_t,ns);
		fMyRootInterface->get_value("C_tid",Monitor_tid);
		fMyRootInterface->get_value("C_pid",Monitor_pid);
		fMyRootInterface->get_value("C_ppid",Monitor_ppid);
		fMyRootInterface->get_value("C_oprocess",Monitor_oprocess);
		fMyRootInterface->get_value("C_ovolName",Monitor_ovolName);
		fMyRootInterface->get_value("C_volName",Monitor_volName);
		int edep_error = -1;
		edep_error = fMyRootInterface->get_value("C_edep",Monitor_edep,GeV);
		int volID_error = -1;
		volID_error = fMyRootInterface->get_value("C_volID",Monitor_volID);
		int ot_error = -1;
		ot_error = fMyRootInterface->get_value("C_ot",Monitor_ot,ns);
		fMyRootInterface->get_value("C_ox",Monitor_ox,cm);
		fMyRootInterface->get_value("C_oy",Monitor_oy,cm);
		fMyRootInterface->get_value("C_oz",Monitor_oz,cm);
		fMyRootInterface->get_value("C_opx",Monitor_opx,GeV);
		fMyRootInterface->get_value("C_opy",Monitor_opy,GeV);
		fMyRootInterface->get_value("C_opz",Monitor_opz,GeV);
		fMyRootInterface->get_value("C_x",Monitor_x,cm);
		fMyRootInterface->get_value("C_y",Monitor_y,cm);
		fMyRootInterface->get_value("C_z",Monitor_z,cm);
		fMyRootInterface->get_value("C_charge",Monitor_charge);
		fMyRootInterface->get_value("C_px",Monitor_px,GeV);
		fMyRootInterface->get_value("C_py",Monitor_py,GeV);
		fMyRootInterface->get_value("C_pz",Monitor_pz,GeV);
		bool st_error = -1;
		st_error = fMyRootInterface->get_value("C_stopped",Monitor_stopped);
		fMyRootInterface->get_value("C_stop_time",Monitor_stop_time,ns);

		// find particles
		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"###Getting information"<<std::endl;

		if (m_MonitorPlane!="McTruth"){
			// Get initial particle
			double ini_pa = sqrt(ini_px*ini_px+ini_py*ini_py+ini_pz*ini_pz);
			double ini_pt = sqrt(ini_px*ini_px+ini_py*ini_py);
			double ini_theta = ini_pa?acos(ini_pz/ini_pa):0;
			// Get weight
			double Gamma, E, Beta;
			if (UseWeight==-211){
				E = sqrt(ini_pa*ini_pa+M_PION*M_PION);
				Beta = ini_pa/E;
				Gamma = sqrt(1./(1.-Beta*Beta));
			}
			for (int j = 0; j < 5000; j++){
				CellHitCheck[j] = false;
			}
			for (int j = 0; j < 2; j++){
				TriggerHitCheck[j] = false;
			}
			// Get Volume Monitor information if exist
			for ( int i_mon = 0; i_mon < Monitor_nHits; i_mon++ ){
				for ( int i_MP = 0; i_MP<Volumes.size(); i_MP++ ){
					std::string Volume=Volumes[i_MP];
//	To simplify, don't use volName to distinguish. -- Chen Wu @ 131203
//					if (Volume!=Monitor_volName[i_mon]) continue;
					if (verbose >= Verbose_ParticleInfo || iEvent%printModule == 0)
						std::cout<<prefix_ParticleInfoStart
								 <<"  got "<<Monitor_pid[i_mon]<<" in "<<Volume<<"!"
								 <<",  tid = "<<Monitor_tid[i_mon]
								 <<",  i_MP = "<<i_MP
								 <<std::endl;
					if (!PDGEncoding // all particles
						||PDGEncoding==-1&&Monitor_pid[i_mon]>=1e7 // only nuclears
						||PDGEncoding==1&&Monitor_pid[i_mon]<1e7 // only elementary particles
						||Monitor_pid[i_mon] == PDGEncoding
						||PDGEncoding==2&&Monitor_pid[i_mon]!=13&&Monitor_pid[i_mon]!=-211&&Monitor_pid[i_mon]!=11&&Monitor_pid[i_mon]<1e7 // only other particles
						||PDGEncoding==-2&&(!st_error&&Monitor_stopped[i_mon])&&(Monitor_pid[i_mon]==13||Monitor_pid[i_mon]==-211) // only stopped particles
						||PDGEncoding==-3&&(Monitor_pid[i_mon]==13||Monitor_pid[i_mon]==-211||Monitor_pid[i_mon]!=11) // only other particles
						){
						if (verbose >= Verbose_ParticleInfo || iEvent%printModule == 0)
							std::cout<<prefix_ParticleInfoStart
									 <<"  Passed PDG cut!"
									 <<std::endl;
						double weight = weight0;
						if (UseWeight==-211){
							if (Monitor_tid[i_mon]==1){
								if (!st_error&&Monitor_stopped[i_mon]){
									weight *= exp(-(Monitor_stop_time[i_mon]-ini_t)/tau/Gamma);
								}
								else{
									weight *= exp(-(Monitor_t[i_mon]-ini_t)/tau/Gamma);
								}
							}
							else{
								if (verbose >= Verbose_ParticleInfo || iEvent%printModule == 0)
									std::cout<<prefix_ParticleInfoStart
											 <<"  UseWeight mode! Only need primary particle!"
											 <<std::endl;
								weight = 0;
//								continue;
							}
						}
						double mon_px = Monitor_opx[i_mon];
						double mon_py = Monitor_opy[i_mon];
						double mon_pz = Monitor_opz[i_mon];
						double mon_pa = sqrt(mon_px*mon_px+mon_py*mon_py+mon_pz*mon_pz);
						if (PDGEncoding==-3){
							if (Monitor_pid[i_mon]==13){
								if (mon_pa<75*MeV) continue;
							}
							else if (Monitor_pid[i_mon]==11){
								if (mon_pa<100*MeV) continue;
							}
							else if (Monitor_pid[i_mon]!=-211){
								continue;
							}
						}
						if (Volume=="CdcCell"){
							if (volID_error){
								std::cout<<"We need volID for CdcCell hits!"<<std::endl;
								break;
							}
							int volID = Monitor_volID[i_mon];
							if (!CellHitCheck[volID]){
								if (edep_error||Monitor_edep[i_mon]>0.5*keV){
									CellHitCheck[volID] = true;
									for (int i = 0; i <= 5; i++){
										double shifttime = (i-2)*PulseInterval;
										for (int i_time = 0; i_time < 100; i_time++){
											double down_time = i_time*20*ns;
											double up_time = down_time + 20*ns;
// FIXME:
// This is only temperary way:
											int binDown = hProtonPuls->FindBin((shifttime+down_time-(Monitor_t[i_mon]+ini_t))/ns);
											int binUp = hProtonPuls->FindBin((shifttime+up_time-(Monitor_t[i_mon]+ini_t))/ns);
//											int binDown = hProtonPuls->FindBin((shifttime+down_time-Monitor_t[i_mon])/ns);
//											int binUp = hProtonPuls->FindBin((shifttime+up_time-Monitor_t[i_mon])/ns);
											double W = hProtonPuls->Integral(binDown,binUp);
//											std::cout<<"t = "<<(Monitor_t[i_mon]+ini_t)/ns<<", shifttime = "<<shifttime/ns<<", down_time = "<<down_time/ns<<", up_time = "<<up_time/ns<<std::endl;
//											std::cout<<"tdown = "<<(shifttime+down_time-(Monitor_t[i_mon]+ini_t))/ns<<std::endl;
//											std::cout<<"tup = "<<(shifttime+up_time-(Monitor_t[i_mon]+ini_t))/ns<<std::endl;
//											std::cout<<"binDown = "<<binDown/ns<<std::endl;
//											std::cout<<"binUp = "<<binUp/ns<<std::endl;
//											std::cout<<"W = "<<W<<std::endl;
											CellHitCount[volID][i_time]+=W*weight0;
//											if (W*weight0)
//												std::cout<<"CellHitCount["<<volID<<"]["<<i_time<<"] = "<<CellHitCount[volID][i_time]<<std::endl;
										}
									}
								}
							}
						}
						if (Volume=="Trigger"){
							int volID = 0;
							if (volID_error){
								if (Monitor_z[i_mon]<0)
									volID=0;
								else
									volID=1;
							}
							else{
								volID=Monitor_volID[i_mon];
							}
							if (!TriggerHitCheck[volID]){
								if (edep_error||Monitor_edep[i_mon]>0.5*keV){
									TriggerHitCheck[volID] = true;
									for (int i_time = 0; i_time < 100; i_time++){
										double down_time = i_time*20*ns;
										double up_time = down_time + 20*ns;
										int binDown = hProtonPuls->FindBin((down_time-Monitor_t[i_mon])/ns);
										int binUp = hProtonPuls->FindBin((up_time-Monitor_t[i_mon])/ns);
										double W = hProtonPuls->Integral(binDown,binUp);
										TriggerHitCount[volID][i_time]+=W*weight0;
									}
								}
							}
						}
						/*
						if (Monitor_tid[i_mon]==prevtid&&i_MP==previ_MP&&(st_error||!Monitor_stopped[i_mon])){
							if (verbose >= Verbose_ParticleInfo || iEvent%printModule == 0)
								std::cout<<prefix_ParticleInfoStart
										 <<"  Occurred once!"
										 <<std::endl;
							continue;
						}
						if (verbose >= Verbose_ParticleInfo || iEvent%printModule == 0)
							std::cout<<prefix_ParticleInfoStart
									 <<"  First hit!"
									 <<std::endl;
						*/
						if (verbose >= Verbose_ParticleInfo || iEvent%printModule == 0)
							std::cout<<prefix_ParticleInfoStart
									 <<"  Filling ..."
									 <<std::endl;
						// Hash
						fMyRootInterface->set_ovalue("evt_num",evt_num);
						fMyRootInterface->set_ovalue("run_num",run_num);
						fMyRootInterface->set_ovalue("tid",Monitor_tid[i_mon]);
						// for simulation
						fMyRootInterface->set_ovalue("pid",Monitor_pid[i_mon]);
						fMyRootInterface->set_ovalue("x" ,Monitor_x[i_mon]/mm);
						fMyRootInterface->set_ovalue("y" ,Monitor_y[i_mon]/mm);
						fMyRootInterface->set_ovalue("z" ,Monitor_z[i_mon]/mm);
						fMyRootInterface->set_ovalue("px",Monitor_opx[i_mon]/MeV);
						fMyRootInterface->set_ovalue("py",Monitor_opy[i_mon]/MeV);
						fMyRootInterface->set_ovalue("pz",Monitor_opz[i_mon]/MeV);
						if (!st_error&&Monitor_stopped[i_mon]){
							fMyRootInterface->set_ovalue("t" ,Monitor_stop_time[i_mon]/ns);
						}
						else{
							fMyRootInterface->set_ovalue("t" ,Monitor_t[i_mon]/ns);
						}
						// original
						if (Monitor_tid[i_mon]!=1||m_OriginalFile=="NONE"){ // we don't need original file to get initial information for primary particles. e.g. PTACS
							if (!ot_error)
								fMyRootInterface->set_ovalue("ot",Monitor_ot[i_mon]/ns);
							else 
								fMyRootInterface->set_ovalue("ot",Monitor_t[i_mon]/ns); 
//							fMyRootInterface->set_ovalue("ox",Monitor_ox[i_mon]/mm);
//							fMyRootInterface->set_ovalue("oy",Monitor_oy[i_mon]/mm);
//							fMyRootInterface->set_ovalue("oz",Monitor_oz[i_mon]/mm);
							fMyRootInterface->set_ovalue("opx",Monitor_opx[i_mon]/MeV);
							fMyRootInterface->set_ovalue("opy",Monitor_opy[i_mon]/MeV);
							fMyRootInterface->set_ovalue("opz",Monitor_opz[i_mon]/MeV);
							fMyRootInterface->set_ovalue("ppid",Monitor_ppid[i_mon]);
							fMyRootInterface->set_ovalue("process",Monitor_oprocess[i_mon]);
							fMyRootInterface->set_ovalue("volume",Monitor_ovolName[i_mon]);
						}
						else{ // We should use initial information instead
							fMyRootInterface->set_ovalue("ot",ini_ot/ns);
							fMyRootInterface->set_ovalue("ox",ini_ox/mm);
							fMyRootInterface->set_ovalue("oy",ini_oy/mm);
							fMyRootInterface->set_ovalue("oz",ini_oz/mm);
							fMyRootInterface->set_ovalue("opx",ini_opx/MeV);
							fMyRootInterface->set_ovalue("opy",ini_opy/MeV);
							fMyRootInterface->set_ovalue("opz",ini_opz/MeV);
							fMyRootInterface->set_ovalue("ppid",ini_ppid);
							fMyRootInterface->set_ovalue("process",ini_process);
							fMyRootInterface->set_ovalue("volume",ini_volume);
						}
						// initial
						fMyRootInterface->set_ovalue("i_t",ini_t/ns);
						fMyRootInterface->set_ovalue("i_x",ini_x/mm);
						fMyRootInterface->set_ovalue("i_y",ini_y/mm);
						fMyRootInterface->set_ovalue("i_z",ini_z/mm);
						fMyRootInterface->set_ovalue("i_px",ini_px/MeV);
						fMyRootInterface->set_ovalue("i_py",ini_py/MeV);
						fMyRootInterface->set_ovalue("i_pz",ini_pz/MeV);
						// auxiliary
						fMyRootInterface->set_ovalue("weight",weight);
						fMyRootInterface->set_ovalue("cvolume",Volume);
						fMyRootInterface->Fill();
					}
				}
			}
		}

		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfo<<"Finished!"<<std::endl;
		if (iEvent%writeModule==0){
			fMyRootInterface->Write();
			if (verbose >= Verbose_EventInfo || iEvent%writeModule == 0) std::cout<<prefix_EventInfoStart<<"Written file"<<std::endl;
		}

	}/* end of loop in events*/
	int nLayers = pCdcGeometryParameter->get_LayerNo();
	int nCell_max = 0;
	for ( int ilayer = 0; ilayer<nLayers; ilayer++){
		int nCells = pCdcGeometryParameter->get_layer_cell_num(ilayer);
		if (nCell_max<nCells) nCell_max = nCells;
	}
	double totalhits = 0;
	for (int i_time = 0; i_time < 100; i_time++){
		double hitRate_layer[100];
		for (int i_layer = 0; i_layer < nLayers; i_layer++){
			hitRate_layer[i_layer] = 0;
		}
		buff.str("");
		buff.clear();
		buff<<m_prefix<<"_CdcHit2D_"<<i_time/5<<m_suffix;
		if ((index_temp = fMyRootInterface->get_TH2D_index(buff.str())) != -1 ){
			h2d_temp=fMyRootInterface->get_TH2D(index_temp);
		}
		else{
			std::cout<<"Cannot find "<<buff.str()<<"!!!"<<std::endl;
		}
		for ( int i_vol = 0; i_vol < 50000; i_vol++){
			int layerId = 0;
			int cellId = 0;
			int error = pCdcGeometryParameter->get_layerIdcellId(i_vol,&layerId,&cellId);
			if (!error){
				double nCells = pCdcGeometryParameter->get_layer_cell_num(layerId);
//				std::cout<<"nCells["<<layerId<<"] = "<<nCells<<std::endl;
				double nHits = CellHitCount[i_vol][i_time];
				totalhits+=nHits;
				double hitRate = nHits*nProtonPerPulse*5e4/m_norm/nCells; // kHz
				hitRate_layer[layerId]+=hitRate;
				if (h2d_temp) h2d_temp->Fill(cellId,layerId,hitRate);
			}
			else{
				std::cout<<"Cannot find i_vol = "<<i_vol<<std::endl;
				break;
			}
		}
		buff.str("");
		buff.clear();
		buff<<m_prefix<<"_CdcHit1D_"<<i_time/5<<m_suffix;
		for (int i_layer = 0; i_layer < nLayers; i_layer++){
			std::cout<<"hitRate_layer["<<i_layer<<"] = "<<hitRate_layer[i_layer]<<std::endl;
		}
		if ((index_temp = fMyRootInterface->get_TH1D_index(buff.str())) != -1 ){
			h1d_temp=fMyRootInterface->get_TH1D(index_temp);
			for (int i_layer = 0; i_layer < nLayers; i_layer++){
				h1d_temp->AddBinContent(i_layer+1,hitRate_layer[i_layer]);
			}
		}
		else{
			std::cout<<"Cannot find "<<buff.str()<<"!!!"<<std::endl;
		}
		buff.str("");
		buff.clear();
		buff<<m_prefix<<"_CellLayer0"<<m_suffix;
		if ((index_temp = fMyRootInterface->get_TH1D_index(buff.str())) != -1 ){
			h1d_temp=fMyRootInterface->get_TH1D(index_temp);
			h1d_temp->SetBinContent(i_time+1,hitRate_layer[0]);
		}
		else{
			std::cout<<"Cannot find "<<buff.str()<<"!!!"<<std::endl;
		}
		buff.str("");
		buff.clear();
		buff<<m_prefix<<"_CellLayer1"<<m_suffix;
		if ((index_temp = fMyRootInterface->get_TH1D_index(buff.str())) != -1 ){
			h1d_temp=fMyRootInterface->get_TH1D(index_temp);
			h1d_temp->SetBinContent(i_time+1,hitRate_layer[1]);
		}
		else{
			std::cout<<"Cannot find "<<buff.str()<<"!!!"<<std::endl;
		}
		buff.str("");
		buff.clear();
		buff<<m_prefix<<"_CellLayer2"<<m_suffix;
		if ((index_temp = fMyRootInterface->get_TH1D_index(buff.str())) != -1 ){
			h1d_temp=fMyRootInterface->get_TH1D(index_temp);
			h1d_temp->SetBinContent(i_time+1,hitRate_layer[2]);
		}
		else{
			std::cout<<"Cannot find "<<buff.str()<<"!!!"<<std::endl;
		}
		buff.str("");
		buff.clear();
		buff<<m_prefix<<"_CellLayer1m"<<m_suffix;
		if ((index_temp = fMyRootInterface->get_TH1D_index(buff.str())) != -1 ){
			h1d_temp=fMyRootInterface->get_TH1D(index_temp);
			h1d_temp->SetBinContent(i_time+1,hitRate_layer[nLayers-2]);
		}
		else{
			std::cout<<"Cannot find "<<buff.str()<<"!!!"<<std::endl;
		}
		buff.str("");
		buff.clear();
		buff<<m_prefix<<"_CellLayer0m"<<m_suffix;
		if ((index_temp = fMyRootInterface->get_TH1D_index(buff.str())) != -1 ){
			h1d_temp=fMyRootInterface->get_TH1D(index_temp);
			h1d_temp->SetBinContent(i_time+1,hitRate_layer[nLayers-1]);
		}
		else{
			std::cout<<"Cannot find "<<buff.str()<<"!!!"<<std::endl;
		}
		buff.str("");
		buff.clear();
		buff<<m_prefix<<"_Trigger0Hit"<<m_suffix;
		if ((index_temp = fMyRootInterface->get_TH1D_index(buff.str())) != -1 ){
			h1d_temp=fMyRootInterface->get_TH1D(index_temp);
//			std::cout<<"h1d_temp = fMyRootInterface->get_TH1D("<<index_temp<<")"<<std::endl;
			double nHits = TriggerHitCount[0][i_time];
			double hitRate = nHits*nProtonPerPulse*5e4/m_norm; // kHz
//			std::cout<<"  hitRate = "<<hitRate<<std::endl;
			h1d_temp->SetBinContent(i_time+1,hitRate);
//			std::cout<<buff.str()<<"->SetBinContent("<<i_time+1<<","<<hitRate<<")"<<std::endl;
		}
		else{
			std::cout<<"Cannot find "<<buff.str()<<"!!!"<<std::endl;
		}
		buff.str("");
		buff.clear();
		buff<<m_prefix<<"_Trigger1Hit"<<m_suffix;
		if ((index_temp = fMyRootInterface->get_TH1D_index(buff.str())) != -1 ){
			h1d_temp=fMyRootInterface->get_TH1D(index_temp);
//			std::cout<<"h1d_temp = fMyRootInterface->get_TH1D("<<index_temp<<")"<<std::endl;
			double nHits = TriggerHitCount[1][i_time];
			double hitRate = nHits*nProtonPerPulse*5e4/m_norm; // kHz
//			std::cout<<"  hitRate = "<<hitRate<<std::endl;
			h1d_temp->SetBinContent(i_time+1,hitRate);
//			std::cout<<buff.str()<<"->SetBinContent("<<i_time+1<<","<<hitRate<<")"<<std::endl;
		}
		else{
			std::cout<<"Cannot find "<<buff.str()<<"!!!"<<std::endl;
		}
	}
	std::cout<<"totalhits = "<<totalhits<<std::endl;

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
	m_prefix="test";
	m_suffix="";
	m_InputDir="";
	m_runName="test";
	m_OriginalFile="NONE";
	m_workMode="monitor";
	m_MonitorPlane="CDC";
	m_OutputDir="result";
	m_input="input";
	verbose = 0;
	nEvents = 0;
	printModule = 10000;
	writeModule = 10000;
	UseWeight = 0;
	PDGEncoding = 1;
	m_norm=1e10;
	backup = false;
	CdcFile = "CdcFile";
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
