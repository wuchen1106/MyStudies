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

		// For output
		std::vector<int> pid;
		std::vector<int> ppid;
		std::vector<double> x;
		std::vector<double> y;
		std::vector<double> z;
		std::vector<double> px;
		std::vector<double> py;
		std::vector<double> pz;
		std::vector<double> t;
		std::vector<std::string> process;
		std::vector<std::string> volume;
		std::vector<double> hit_x;
		std::vector<double> hit_y;
		std::vector<double> hit_z;
		std::vector<double> hit_px;
		std::vector<double> hit_py;
		std::vector<double> hit_pz;
		std::vector<double> hit_t;

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
		int MonitorE_nHits = 0;
		std::vector<int> MonitorE_tid;
		std::vector<int> MonitorE_pid;
		std::vector<double> MonitorE_t;
		std::vector<double> MonitorE_e;
		std::vector<double> MonitorE_x;
		std::vector<double> MonitorE_y;
		std::vector<double> MonitorE_z;
		std::vector<double> MonitorE_px;
		std::vector<double> MonitorE_py;
		std::vector<double> MonitorE_pz;
		int MonitorC_nHits = 0;
		std::vector<int> MonitorC_tid;
		std::vector<int> MonitorC_pid;
		std::vector<double> MonitorC_t;
		std::vector<double> MonitorC_e;
		std::vector<double> MonitorC_x;
		std::vector<double> MonitorC_y;
		std::vector<double> MonitorC_z;
		std::vector<double> MonitorC_px;
		std::vector<double> MonitorC_py;
		std::vector<double> MonitorC_pz;

		index_temp = fMyRootInterface->get_TBranch_index("evt_num");
		if (index_temp!=-1) evt_num = fMyRootInterface->get_vec_int(index_temp);
		index_temp = fMyRootInterface->get_TBranch_index("run_num");
		if (index_temp!=-1) run_num = fMyRootInterface->get_vec_int(index_temp);
		index_temp = fMyRootInterface->get_TBranch_index("McTruth_nTracks");
		if (index_temp!=-1) McTruth_nTracks = fMyRootInterface->get_vec_int(index_temp);
		index_temp = fMyRootInterface->get_TBranch_index("McTruth_pid");
		if (index_temp!=-1) McTruth_pid = *(fMyRootInterface->get_vec_vecint(index_temp));
		index_temp = fMyRootInterface->get_TBranch_index("McTruth_tid");
		if (index_temp!=-1) McTruth_tid = *(fMyRootInterface->get_vec_vecint(index_temp));
		index_temp = fMyRootInterface->get_TBranch_index("McTruth_ptid");
		if (index_temp!=-1) McTruth_ptid = *(fMyRootInterface->get_vec_vecint(index_temp));
		index_temp = fMyRootInterface->get_TBranch_index("McTruth_x");
		if (index_temp!=-1) McTruth_x = *(fMyRootInterface->get_vec_vecdouble(index_temp));
		for (int i = 0; i<McTruth_x.size();i++) McTruth_x[i] *= cm;
		index_temp = fMyRootInterface->get_TBranch_index("McTruth_y");
		if (index_temp!=-1) McTruth_y = *(fMyRootInterface->get_vec_vecdouble(index_temp));
		for (int i = 0; i<McTruth_y.size();i++) McTruth_y[i] *= cm;
		index_temp = fMyRootInterface->get_TBranch_index("McTruth_z");
		if (index_temp!=-1) McTruth_z = *(fMyRootInterface->get_vec_vecdouble(index_temp));
		for (int i = 0; i<McTruth_z.size();i++) McTruth_z[i] *= cm;
		index_temp = fMyRootInterface->get_TBranch_index("McTruth_px");
		if (index_temp!=-1) McTruth_px = *(fMyRootInterface->get_vec_vecdouble(index_temp));
		for (int i = 0; i<McTruth_px.size();i++) McTruth_px[i] *= GeV;
		index_temp = fMyRootInterface->get_TBranch_index("McTruth_py");
		if (index_temp!=-1) McTruth_py = *(fMyRootInterface->get_vec_vecdouble(index_temp));
		for (int i = 0; i<McTruth_py.size();i++) McTruth_py[i] *= GeV;
		index_temp = fMyRootInterface->get_TBranch_index("McTruth_pz");
		if (index_temp!=-1) McTruth_pz = *(fMyRootInterface->get_vec_vecdouble(index_temp));
		for (int i = 0; i<McTruth_pz.size();i++) McTruth_pz[i] *= GeV;
		index_temp = fMyRootInterface->get_TBranch_index("McTruth_time");
		if (index_temp!=-1) McTruth_time = *(fMyRootInterface->get_vec_vecdouble(index_temp));
		for (int i = 0; i<McTruth_time.size();i++) McTruth_time[i] *= ns;
		index_temp = fMyRootInterface->get_TBranch_index("McTruth_e");
		if (index_temp!=-1) McTruth_e = *(fMyRootInterface->get_vec_vecdouble(index_temp));
		for (int i = 0; i<McTruth_e.size();i++) McTruth_e[i] *= GeV;
		index_temp = fMyRootInterface->get_TBranch_index("McTruth_process");
		if (index_temp!=-1) McTruth_process = *(fMyRootInterface->get_vec_vecstring(index_temp));
		index_temp = fMyRootInterface->get_TBranch_index("McTruth_volume");
		if (index_temp!=-1) McTruth_volume = *(fMyRootInterface->get_vec_vecstring(index_temp));
		index_temp = fMyRootInterface->get_TBranch_index("MonitorE_nHits");
		if (index_temp!=-1) MonitorE_nHits = fMyRootInterface->get_vec_int(index_temp);
		index_temp = fMyRootInterface->get_TBranch_index("MonitorE_t");
		if (index_temp!=-1) MonitorE_t = *(fMyRootInterface->get_vec_vecdouble(index_temp));
		for (int i = 0; i<MonitorE_t.size();i++) MonitorE_t[i] *= ns;
		index_temp = fMyRootInterface->get_TBranch_index("MonitorE_e");
		if (index_temp!=-1) MonitorE_e = *(fMyRootInterface->get_vec_vecdouble(index_temp));
		for (int i = 0; i<MonitorE_e.size();i++) MonitorE_e[i] *= GeV;
		index_temp = fMyRootInterface->get_TBranch_index("MonitorE_tid");
		if (index_temp!=-1) MonitorE_tid = *(fMyRootInterface->get_vec_vecint(index_temp));
		index_temp = fMyRootInterface->get_TBranch_index("MonitorE_pid");
		if (index_temp!=-1) MonitorE_pid = *(fMyRootInterface->get_vec_vecint(index_temp));
		index_temp = fMyRootInterface->get_TBranch_index("MonitorE_x");
		if (index_temp!=-1) MonitorE_x = *(fMyRootInterface->get_vec_vecdouble(index_temp));
		for (int i = 0; i<MonitorE_x.size();i++) MonitorE_x[i] *= cm;
		index_temp = fMyRootInterface->get_TBranch_index("MonitorE_y");
		if (index_temp!=-1) MonitorE_y = *(fMyRootInterface->get_vec_vecdouble(index_temp));
		for (int i = 0; i<MonitorE_y.size();i++) MonitorE_y[i] *= cm;
		index_temp = fMyRootInterface->get_TBranch_index("MonitorE_z");
		if (index_temp!=-1) MonitorE_z = *(fMyRootInterface->get_vec_vecdouble(index_temp));
		for (int i = 0; i<MonitorE_z.size();i++) MonitorE_z[i] *= cm;
		index_temp = fMyRootInterface->get_TBranch_index("MonitorE_px");
		if (index_temp!=-1) MonitorE_px = *(fMyRootInterface->get_vec_vecdouble(index_temp));
		for (int i = 0; i<MonitorE_px.size();i++) MonitorE_px[i] *= GeV;
		index_temp = fMyRootInterface->get_TBranch_index("MonitorE_py");
		if (index_temp!=-1) MonitorE_py = *(fMyRootInterface->get_vec_vecdouble(index_temp));
		for (int i = 0; i<MonitorE_py.size();i++) MonitorE_py[i] *= GeV;
		index_temp = fMyRootInterface->get_TBranch_index("MonitorE_pz");
		if (index_temp!=-1) MonitorE_pz = *(fMyRootInterface->get_vec_vecdouble(index_temp));
		for (int i = 0; i<MonitorE_pz.size();i++) MonitorE_pz[i] *= GeV;
		index_temp = fMyRootInterface->get_TBranch_index("MonitorC_nHits");
		if (index_temp!=-1) MonitorC_nHits = fMyRootInterface->get_vec_int(index_temp);
		index_temp = fMyRootInterface->get_TBranch_index("MonitorC_t");
		if (index_temp!=-1) MonitorC_t = *(fMyRootInterface->get_vec_vecdouble(index_temp));
		for (int i = 0; i<MonitorC_t.size();i++) MonitorC_t[i] *= ns;
		index_temp = fMyRootInterface->get_TBranch_index("MonitorC_e");
		if (index_temp!=-1) MonitorC_e = *(fMyRootInterface->get_vec_vecdouble(index_temp));
		for (int i = 0; i<MonitorC_e.size();i++) MonitorC_e[i] *= GeV;
		index_temp = fMyRootInterface->get_TBranch_index("MonitorC_tid");
		if (index_temp!=-1) MonitorC_tid = *(fMyRootInterface->get_vec_vecint(index_temp));
		index_temp = fMyRootInterface->get_TBranch_index("MonitorC_pid");
		if (index_temp!=-1) MonitorC_pid = *(fMyRootInterface->get_vec_vecint(index_temp));
		index_temp = fMyRootInterface->get_TBranch_index("MonitorC_x");
		if (index_temp!=-1) MonitorC_x = *(fMyRootInterface->get_vec_vecdouble(index_temp));
		for (int i = 0; i<MonitorC_x.size();i++) MonitorC_x[i] *= cm;
		index_temp = fMyRootInterface->get_TBranch_index("MonitorC_y");
		if (index_temp!=-1) MonitorC_y = *(fMyRootInterface->get_vec_vecdouble(index_temp));
		for (int i = 0; i<MonitorC_y.size();i++) MonitorC_y[i] *= cm;
		index_temp = fMyRootInterface->get_TBranch_index("MonitorC_z");
		if (index_temp!=-1) MonitorC_z = *(fMyRootInterface->get_vec_vecdouble(index_temp));
		for (int i = 0; i<MonitorC_z.size();i++) MonitorC_z[i] *= cm;
		index_temp = fMyRootInterface->get_TBranch_index("MonitorC_px");
		if (index_temp!=-1) MonitorC_px = *(fMyRootInterface->get_vec_vecdouble(index_temp));
		for (int i = 0; i<MonitorC_px.size();i++) MonitorC_px[i] *= GeV;
		index_temp = fMyRootInterface->get_TBranch_index("MonitorC_py");
		if (index_temp!=-1) MonitorC_py = *(fMyRootInterface->get_vec_vecdouble(index_temp));
		for (int i = 0; i<MonitorC_py.size();i++) MonitorC_py[i] *= GeV;
		index_temp = fMyRootInterface->get_TBranch_index("MonitorC_pz");
		if (index_temp!=-1) MonitorC_pz = *(fMyRootInterface->get_vec_vecdouble(index_temp));
		for (int i = 0; i<MonitorC_pz.size();i++) MonitorC_pz[i] *= GeV;

		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Got info"<<std::endl;

		// find muon
		std::vector<int> i_mon; // 0: no hit. 1: MonitorC. 2: MonitorE
		std::vector<int> i_hit;
		std::vector<int> tid;
		for ( int i = 0; i < MonitorC_nHits; i++ ){
			int pid = MonitorC_pid[i]; 
			if ( pid == -13 || pid == 13 || pid == 211 || pid == -211 || pid == 321 || pid == -321){
				i_hit.push_back(i);
				i_mon.push_back(1);
				tid.push_back(MonitorC_tid[i]);
			}
		}
		for ( int i = 0; i < MonitorE_nHits; i++ ){
			int pid = MonitorE_pid[i];
			if ( pid == -13 || pid == 13 || pid == 211 || pid == -211 || pid == 321 || pid == -321){
				i_hit.push_back(i);
				i_mon.push_back(2);
				tid.push_back(MonitorE_tid[i]);
			}
		}
		if (i_hit.size()==0) continue;
		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Found muon"<<std::endl;
		inc_Ncut("Found muon");

		// get information of this muon 
		for (int i_par = 0; i_par < i_hit.size(); i_par++){
			int i_McTruth = -1;
			int i_ppid = 0;
			int i_ptid = 0;
			for ( int i = 0; i < McTruth_nTracks; i++ ){
				int i_tid = McTruth_tid[i];
				if ( i_tid == tid[i_par] ){
					i_McTruth = i;
					i_ptid = McTruth_ptid[i_par];
					break;
				}
			}
			for ( int i = 0; i < McTruth_nTracks; i++ ){
				int i_tid = McTruth_tid[i];
				if ( i_tid == i_ptid ){
					i_ppid = McTruth_pid[i];
					break;
				}
			}
			pid.push_back(McTruth_pid[i_McTruth]);
			ppid.push_back(i_ppid);
			x.push_back(McTruth_x[i_McTruth]/mm);
			y.push_back(McTruth_y[i_McTruth]/mm);
			z.push_back(McTruth_z[i_McTruth]/mm);
			px.push_back(McTruth_px[i_McTruth]/MeV);
			py.push_back(McTruth_py[i_McTruth]/MeV);
			pz.push_back(McTruth_pz[i_McTruth]/MeV);
			t.push_back(McTruth_time[i_McTruth]/ns);
			process.push_back(McTruth_process[i_McTruth]);
			volume.push_back(McTruth_volume[i_McTruth]);
			if (i_mon[i_par]==1){
				hit_x.push_back(MonitorC_x[i_hit[i_par]]/mm);
				hit_y.push_back(MonitorC_y[i_hit[i_par]]/mm);
				hit_z.push_back(MonitorC_z[i_hit[i_par]]/mm);
				hit_px.push_back(MonitorC_px[i_hit[i_par]]/MeV);
				hit_py.push_back(MonitorC_py[i_hit[i_par]]/MeV);
				hit_pz.push_back(MonitorC_pz[i_hit[i_par]]/MeV);
				hit_t.push_back(MonitorC_t[i_hit[i_par]]/ns);
			}
			else if (i_mon[i_par]==2){
				hit_x.push_back(MonitorE_x[i_hit[i_par]]/mm);
				hit_y.push_back(MonitorE_y[i_hit[i_par]]/mm);
				hit_z.push_back(MonitorE_z[i_hit[i_par]]/mm);
				hit_px.push_back(MonitorE_px[i_hit[i_par]]/MeV);
				hit_py.push_back(MonitorE_py[i_hit[i_par]]/MeV);
				hit_pz.push_back(MonitorE_pz[i_hit[i_par]]/MeV);
				hit_t.push_back(MonitorE_t[i_hit[i_par]]/ns);
			}
		}

		// Fill the tree
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("evt_num")) != -1 )
			fMyRootInterface->set_ovec_int(index_temp,evt_num); 
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("run_num")) != -1 )
			fMyRootInterface->set_ovec_int(index_temp,run_num); 
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("nPar")) != -1 )
			fMyRootInterface->set_ovec_int(index_temp,i_hit.size()); 
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("pid")) != -1 )
			fMyRootInterface->set_ovec_vecint(index_temp,pid); 
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("ppid")) != -1 )
			fMyRootInterface->set_ovec_vecint(index_temp,ppid); 
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("x")) != -1 )
			fMyRootInterface->set_ovec_vecdouble(index_temp,x); 
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("y")) != -1 )
			fMyRootInterface->set_ovec_vecdouble(index_temp,y); 
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("z")) != -1 )
			fMyRootInterface->set_ovec_vecdouble(index_temp,z); 
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("px")) != -1 )
			fMyRootInterface->set_ovec_vecdouble(index_temp,px); 
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("py")) != -1 )
			fMyRootInterface->set_ovec_vecdouble(index_temp,py); 
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("pz")) != -1 )
			fMyRootInterface->set_ovec_vecdouble(index_temp,pz); 
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("t")) != -1 )
			fMyRootInterface->set_ovec_vecdouble(index_temp,t); 
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("hit_x")) != -1 )
			fMyRootInterface->set_ovec_vecdouble(index_temp,hit_x); 
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("hit_y")) != -1 )
			fMyRootInterface->set_ovec_vecdouble(index_temp,hit_y); 
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("hit_z")) != -1 )
			fMyRootInterface->set_ovec_vecdouble(index_temp,hit_z); 
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("hit_px")) != -1 )
			fMyRootInterface->set_ovec_vecdouble(index_temp,hit_px); 
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("hit_py")) != -1 )
			fMyRootInterface->set_ovec_vecdouble(index_temp,hit_py); 
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("hit_pz")) != -1 )
			fMyRootInterface->set_ovec_vecdouble(index_temp,hit_pz); 
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("hit_t")) != -1 )
			fMyRootInterface->set_ovec_vecdouble(index_temp,hit_t); 
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("process")) != -1 )
			fMyRootInterface->set_ovec_vecstring(index_temp,process); 
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("volume")) != -1 )
			fMyRootInterface->set_ovec_vecstring(index_temp,volume); 
		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Set oTrees"<<std::endl;

		fMyRootInterface->Fill();
		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Filled"<<std::endl;

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
