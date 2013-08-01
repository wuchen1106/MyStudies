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
std::string m_OutputDir;
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
	while((result=getopt(argc,argv,"hbv:n:m:r:d:p:P:"))!=-1){
		switch(result){
			/* INPUTS */
			case 'm':
				m_MonitorPlane = optarg;
				printf("Monitor plane: %s\n",m_MonitorPlane.c_str());
				break;
			case 'r':
				m_runName=optarg;
			case 'd':
				m_OutputDir=optarg;
				printf("Output Directroy: %s\n",m_OutputDir.c_str());
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
		std::string process;
		std::string volume;
		double ox;
		double oy;
		double oz;
		double opx;
		double opy;
		double opz;
		double ot;

		// Get info
		int evt_num;
		int run_num;
		int McTruth_nTracks = 0;
		std::vector<int> McTruth_tid;
		std::vector<int> McTruth_pid;
		std::vector<int> McTruth_ptid;
		std::vector<double> McTruth_x;
		std::vector<double> McTruth_y;
		std::vector<double> McTruth_z;
		std::vector<double> McTruth_px;
		std::vector<double> McTruth_py;
		std::vector<double> McTruth_pz;
		std::vector<double> McTruth_e;
		std::vector<double> McTruth_time;
		std::vector<std::string> McTruth_process;
		std::vector<std::string> McTruth_volume;
		int Monitor_nHits = 0;
		std::vector<int> Monitor_tid;
		std::vector<int> Monitor_pid;
		std::vector<double> Monitor_t;
		std::vector<double> Monitor_e;
		std::vector<double> Monitor_x;
		std::vector<double> Monitor_y;
		std::vector<double> Monitor_z;
		std::vector<double> Monitor_px;
		std::vector<double> Monitor_py;
		std::vector<double> Monitor_pz;

		fMyRootInterface->get_value("evt_num",evt_num);
		fMyRootInterface->get_value("run_num",run_num);
		fMyRootInterface->get_value("McTruth_nTracks",McTruth_nTracks);
		fMyRootInterface->get_value("McTruth_pid",McTruth_pid);
		fMyRootInterface->get_value("McTruth_tid",McTruth_tid);
		fMyRootInterface->get_value("McTruth_ptid",McTruth_ptid);
		fMyRootInterface->get_value("McTruth_x",McTruth_x,cm);
		fMyRootInterface->get_value("McTruth_y",McTruth_y,cm);
		fMyRootInterface->get_value("McTruth_z",McTruth_z,cm);
		fMyRootInterface->get_value("McTruth_px",McTruth_px,GeV);
		fMyRootInterface->get_value("McTruth_py",McTruth_py,GeV);
		fMyRootInterface->get_value("McTruth_pz",McTruth_pz,GeV);
		fMyRootInterface->get_value("McTruth_time",McTruth_time,ns);
		fMyRootInterface->get_value("McTruth_e",McTruth_e,GeV);
		fMyRootInterface->get_value("McTruth_process",McTruth_process);
		fMyRootInterface->get_value("McTruth_volume",McTruth_volume);
		if (m_MonitorPlane=="ptacs_shielding"){
			fMyRootInterface->get_value("PTACSMonitor_nHits",Monitor_nHits);
			fMyRootInterface->get_value("PTACSMonitor_t",Monitor_t,ns);
			fMyRootInterface->get_value("PTACSMonitor_e",Monitor_e,GeV);
			fMyRootInterface->get_value("PTACSMonitor_tid",Monitor_tid);
			fMyRootInterface->get_value("PTACSMonitor_pid",Monitor_pid);
			fMyRootInterface->get_value("PTACSMonitor_x",Monitor_x,cm);
			fMyRootInterface->get_value("PTACSMonitor_y",Monitor_y,cm);
			fMyRootInterface->get_value("PTACSMonitor_z",Monitor_z,cm);
			fMyRootInterface->get_value("PTACSMonitor_px",Monitor_px,GeV);
			fMyRootInterface->get_value("PTACSMonitor_py",Monitor_py,GeV);
			fMyRootInterface->get_value("PTACSMonitor_pz",Monitor_pz,GeV);
		}
		else if (m_MonitorPlane=="blt0"){
			fMyRootInterface->get_value("MT1Monitor_nHits",Monitor_nHits);
			fMyRootInterface->get_value("MT1Monitor_t",Monitor_t,ns);
			fMyRootInterface->get_value("MT1Monitor_e",Monitor_e,GeV);
			fMyRootInterface->get_value("MT1Monitor_tid",Monitor_tid);
			fMyRootInterface->get_value("MT1Monitor_pid",Monitor_pid);
			fMyRootInterface->get_value("MT1Monitor_x",Monitor_x,cm);
			fMyRootInterface->get_value("MT1Monitor_y",Monitor_y,cm);
			fMyRootInterface->get_value("MT1Monitor_z",Monitor_z,cm);
			fMyRootInterface->get_value("MT1Monitor_px",Monitor_px,GeV);
			fMyRootInterface->get_value("MT1Monitor_py",Monitor_py,GeV);
			fMyRootInterface->get_value("MT1Monitor_pz",Monitor_pz,GeV);
		}
		else{
			std::cout<<"Cannot recognize this Monitor plane: \""<<m_MonitorPlane<<"\"!!!"<<std::endl;
			return -1;
		}

		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Got info"<<std::endl;

		// find particles
		std::vector<int> hit_iMon;
		std::vector<bool> goot_hit;
		std::vector<int> hit_iMc;
		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"###Searching particles in Monitor"<<std::endl;
		for ( int i_mon = 0; i_mon < Monitor_nHits; i_mon++ ){
			if (Monitor_pid[i_mon] == PDGEncoding ){
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
				if (going_downstream){
					if (verbose >= Verbose_ParticleInfo || iEvent%printModule == 0)
						std::cout<<prefix_ParticleInfoStart
						         <<"going downstream"<<std::endl;
					hit_iMon.push_back(i_mon);
					hit_iMc.push_back(-1);
					goot_hit.push_back(true);
				}
			}
		}
		// come from upsteam or not?
		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"###Searching particles in McTruth"<<std::endl;
		for ( int i_hit = 0; i_hit < hit_iMon.size(); i_hit++ ){
			for ( int i_McT = 0; i_McT < McTruth_nTracks; i_McT++ ){
				if (Monitor_tid[hit_iMon[i_hit]]==McTruth_tid[i_McT]){
					if (verbose >= Verbose_ParticleInfo || iEvent%printModule == 0)
						std::cout<<prefix_ParticleInfoStart
					         <<"  Found hit["<<i_hit<<"] in McTruth!"
						     <<", McTruth_tid["<<i_McT<<"] = "<<McTruth_tid[i_McT]
						     <<", McTruth_pid["<<i_McT<<"] = "<<McTruth_pid[i_McT]
						     <<std::endl;
					hit_iMc[i_hit]=i_McT;
					if (m_MonitorPlane=="blt0"){
						if (McTruth_z[i_McT]>-2791.5*mm) goot_hit[i_hit]=false;
					}
					else if (m_MonitorPlane=="ptacs_shielding"){
						if (McTruth_x[i_McT]<6500*mm) goot_hit[i_hit] = false;
					}
					if (goot_hit[i_hit])
						if (verbose >= Verbose_ParticleInfo || iEvent%printModule == 0)
							std::cout<<prefix_ParticleInfoStart
								 <<"Comes from upstream"
								 <<std::endl;
				}
			}
		}
		
		bool filled = false;
		// Fill
		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"###Getting information"<<std::endl;
		for (int i_hit = 0; i_hit < hit_iMon.size(); i_hit++){
			pid=Monitor_pid[hit_iMon[i_hit]];
			tid=Monitor_tid[hit_iMon[i_hit]];
			if (verbose >= Verbose_ParticleInfo || iEvent%printModule == 0)
				std::cout<<prefix_ParticleInfoStart
					 <<"==>Info for hit["<<i_hit<<"]"
					 <<", iMc = "<<hit_iMc[i_hit]
					 <<", iMon = "<<hit_iMon[i_hit]
					 <<", pid = "<<pid
					 <<", tid = "<<tid
					 <<std::endl;
			if (!goot_hit[i_hit]) continue;
			if (verbose >= Verbose_ParticleInfo || iEvent%printModule == 0)
				std::cout<<prefix_ParticleInfoStart
					 <<"This comes from upstream and is going downstream"
					 <<std::endl;
			if (hit_iMc[i_hit]<0){
				//std::cout<<"Hey you found a hit which was not recorded by McTruth!"<<std::endl;
				//std::cout<<"    evt_num="<<evt_num<<", run_num = "<<run_num<<", tid = "<<Monitor_tid[hit_iMon[i_hit]]<<", pid = "<<Monitor_pid[hit_iMon[i_hit]]<<std::endl;;
				continue;
			}
			if (verbose >= Verbose_ParticleInfo || iEvent%printModule == 0)
				std::cout<<prefix_ParticleInfoStart
					 <<"We do have McTruth info for it"
					 <<std::endl;
			//std::cout<<"  McTruth_pid["<<hit_iMc[i_hit]<<"] = "<<McTruth_pid[hit_iMc[i_hit]]<<std::endl;
			ppid=0;
			for (int i_McT = 0; i_McT < McTruth_nTracks; i_McT++){
				if (McTruth_tid[i_McT]==McTruth_pid[hit_iMc[i_hit]]){
					ppid=McTruth_pid[i_McT];
					break;
					if (verbose >= Verbose_ParticleInfo || iEvent%printModule == 0)
						std::cout<<prefix_ParticleInfoStart
							 <<"Found its mother! ppid = "<<McTruth_pid[i_McT]
							 <<std::endl;
				}
			}
			if (m_MonitorPlane=="blt0"){
				x=Monitor_x[hit_iMon[i_hit]];
				y=Monitor_y[hit_iMon[i_hit]];
				z=Monitor_z[hit_iMon[i_hit]]+2791.5;
				px=Monitor_px[hit_iMon[i_hit]];
				py=Monitor_py[hit_iMon[i_hit]];
				pz=Monitor_pz[hit_iMon[i_hit]];
			}
			else if (m_MonitorPlane == "ptacs_shielding"){
				x=Monitor_z[hit_iMon[i_hit]]+5790.5*mm;
				y=Monitor_y[hit_iMon[i_hit]];
				z=-Monitor_x[hit_iMon[i_hit]]+6400*mm;
				px=Monitor_pz[hit_iMon[i_hit]];
				py=Monitor_py[hit_iMon[i_hit]];
				pz=-Monitor_px[hit_iMon[i_hit]];
			}
			t=Monitor_t[hit_iMon[i_hit]];
			ox=McTruth_z[hit_iMc[i_hit]]+5790.5*mm;
			oy=McTruth_y[hit_iMc[i_hit]];
			oz=-McTruth_x[hit_iMc[i_hit]]+7350*mm;
			opx=McTruth_pz[hit_iMon[i_hit]];
			opy=McTruth_py[hit_iMon[i_hit]];
			opz=-McTruth_px[hit_iMon[i_hit]];
			ot=McTruth_time[hit_iMc[i_hit]];
			process=McTruth_process[hit_iMc[i_hit]];
			volume=McTruth_volume[hit_iMc[i_hit]];
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
			fMyRootInterface->set_ovalue("ox",ox/mm);
			fMyRootInterface->set_ovalue("oy",oy/mm);
			fMyRootInterface->set_ovalue("oz",oz/mm);
			fMyRootInterface->set_ovalue("opx",opx/MeV);
			fMyRootInterface->set_ovalue("opy",opy/MeV);
			fMyRootInterface->set_ovalue("opz",opz/MeV);
			fMyRootInterface->set_ovalue("ot",ot/ns);
			fMyRootInterface->set_ovalue("process",process);
			fMyRootInterface->set_ovalue("volume",volume);
			if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Set oTrees"<<std::endl;
			fMyRootInterface->Fill();
			if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Filled"<<std::endl;
			filled=true;
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
	fprintf(stderr,"\t\t restore backup file.\n");
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
