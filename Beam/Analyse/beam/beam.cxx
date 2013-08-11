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

#include "MyRootInterface.hxx"

std::string m_workMode;
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
	while((result=getopt(argc,argv,"hb:t:v:n:m:M:r:d:p:P:i:"))!=-1){
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
	double ot;
	double ox;
	double oy;
	double oz;
	double opx;
	double opy;
	double opz;

	//**********************************************************************************************
	//**********************************************************************************************
	TChain* m_TChain = new TChain("t");
	m_TChain->Add("/scratchfs/bes/wuc/MyWorkArea/g4simData/g40cm10mm_PTACS_pim_QGSPBERTHPg4sim.root");
	m_TChain->SetBranchAddress("t",&ot);
	//**********************************************************************************************
	//**********************************************************************************************

	//=======================================================================================================
	//************DO THE DIRTY WORK*******************
	if (verbose >= Verbose_SectorInfo ) std::cout<<prefix_SectorInfo<<"In DO THE DIRTY WORK###"<<std::endl;
	Long64_t nEvent = fMyRootInterface->get_Entries();
//	for( Long64_t iEvent = 0; iEvent < 1000; iEvent++ ){
	for( Long64_t iEvent = 0; iEvent < (nEvents&&nEvents<nEvent?nEvents:nEvent); iEvent++ ){
		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"In Event "<<iEvent<<std::endl;
		fMyRootInterface->GetEntry(iEvent);
		m_TChain->GetEntry(iEvent);
		ot*=ns;
		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Got entries"<<std::endl;
		inc_Ncut("Got entries");
		if (iEvent%printModule==0) fMyRootInterface->Write();

		// Get info
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

		fMyRootInterface->get_value("evt_num",evt_num);
		fMyRootInterface->get_value("run_num",run_num);

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
								 <<"MeV, py = "<<Monitor_py[i_mon]
								 <<"MeV, pz = "<<Monitor_pz[i_mon]
								 <<"MeV"
								 <<std::endl;

					/*
					// going_downstream ?
					bool going_downstream=false;
					if (m_MonitorPlane=="blt0"){
						if (Monitor_pz[i_mon]>0){
							going_downstream=true;
						}
					}
					else if (m_MonitorPlane=="ptacs_shielding"){
						if (Monitor_px[i_mon]<0){
							going_downstream=true;
						}
					}

					if (!going_downstream) continue;

					if (verbose >= Verbose_ParticleInfo || iEvent%printModule == 0)
						std::cout<<prefix_ParticleInfoStart
								 <<"going downstream"<<std::endl;

					// comes_from_upstream ?
					bool comes_from_upstream = false;
					if (m_MonitorPlane=="blt0"){
						if (Monitor_oz[i_mon]<=-2791.5*mm) comes_from_upstream=true;
					}
					else if (m_MonitorPlane=="ptacs_shielding"){
						if (Monitor_ox[i_mon]>=6500*mm) comes_from_upstream = true;
					}
					if (!comes_from_upstream) continue;

					if (verbose >= Verbose_ParticleInfo || iEvent%printModule == 0)
						std::cout<<prefix_ParticleInfoStart
							 <<"Comes from upstream"
							 <<std::endl;
					*/

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
					ox=Monitor_ox[i_mon];
					oy=Monitor_oy[i_mon];
					oz=Monitor_oz[i_mon];
					opx=Monitor_opx[i_mon];
					opy=Monitor_opy[i_mon];
					opz=Monitor_opz[i_mon];
					process=Monitor_oprocess[i_mon];
					volume=Monitor_ovolName[i_mon];
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
					fMyRootInterface->set_ovalue("process",process);
					fMyRootInterface->set_ovalue("volume",volume);
					if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Set oTrees"<<std::endl;
					fMyRootInterface->Fill();
					if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Filled"<<std::endl;
					filled=true;
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
