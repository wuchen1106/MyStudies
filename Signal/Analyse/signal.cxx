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
		int CdcCell_nHits = 0;
		std::vector<int> CdcCell_tid;
		std::vector<int> CdcCell_layerID;
		std::vector<double> CdcCell_t;
		std::vector<double> CdcCell_e;
		std::vector<double> CdcCell_x;
		std::vector<double> CdcCell_y;
		std::vector<double> CdcCell_z;
		std::vector<double> CdcCell_px;
		std::vector<double> CdcCell_py;
		std::vector<double> CdcCell_pz;
		int Trigger_nHits = 0;
		std::vector<int> Trigger_tid;
		std::vector<double> Trigger_t;

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
		index_temp = fMyRootInterface->get_TBranch_index("Trigger_nHits");
		if (index_temp!=-1) Trigger_nHits = fMyRootInterface->get_vec_int(index_temp);
		index_temp = fMyRootInterface->get_TBranch_index("Trigger_t");
		if (index_temp!=-1) Trigger_t = *(fMyRootInterface->get_vec_vecdouble(index_temp));
		for (int i = 0; i<Trigger_t.size();i++) Trigger_t[i] *= ns;
		index_temp = fMyRootInterface->get_TBranch_index("Trigger_tid");
		if (index_temp!=-1) Trigger_tid = *(fMyRootInterface->get_vec_vecint(index_temp));
		index_temp = fMyRootInterface->get_TBranch_index("CdcCell_nHits");
		if (index_temp!=-1) CdcCell_nHits = fMyRootInterface->get_vec_int(index_temp);
		index_temp = fMyRootInterface->get_TBranch_index("CdcCell_t");
		if (index_temp!=-1) CdcCell_t = *(fMyRootInterface->get_vec_vecdouble(index_temp));
		for (int i = 0; i<CdcCell_t.size();i++) CdcCell_t[i] *= ns;
		index_temp = fMyRootInterface->get_TBranch_index("CdcCell_e");
		if (index_temp!=-1) CdcCell_e = *(fMyRootInterface->get_vec_vecdouble(index_temp));
		for (int i = 0; i<CdcCell_e.size();i++) CdcCell_e[i] *= GeV;
		index_temp = fMyRootInterface->get_TBranch_index("CdcCell_tid");
		if (index_temp!=-1) CdcCell_tid = *(fMyRootInterface->get_vec_vecint(index_temp));
		index_temp = fMyRootInterface->get_TBranch_index("CdcCell_layerID");
		if (index_temp!=-1) CdcCell_layerID = *(fMyRootInterface->get_vec_vecint(index_temp));
		index_temp = fMyRootInterface->get_TBranch_index("CdcCell_x");
		if (index_temp!=-1) CdcCell_x = *(fMyRootInterface->get_vec_vecdouble(index_temp));
		for (int i = 0; i<CdcCell_x.size();i++) CdcCell_x[i] *= cm;
		index_temp = fMyRootInterface->get_TBranch_index("CdcCell_y");
		if (index_temp!=-1) CdcCell_y = *(fMyRootInterface->get_vec_vecdouble(index_temp));
		for (int i = 0; i<CdcCell_y.size();i++) CdcCell_y[i] *= cm;
		index_temp = fMyRootInterface->get_TBranch_index("CdcCell_z");
		if (index_temp!=-1) CdcCell_z = *(fMyRootInterface->get_vec_vecdouble(index_temp));
		for (int i = 0; i<CdcCell_z.size();i++) CdcCell_z[i] *= cm;
		index_temp = fMyRootInterface->get_TBranch_index("CdcCell_px");
		if (index_temp!=-1) CdcCell_px = *(fMyRootInterface->get_vec_vecdouble(index_temp));
		for (int i = 0; i<CdcCell_px.size();i++) CdcCell_px[i] *= GeV;
		index_temp = fMyRootInterface->get_TBranch_index("CdcCell_py");
		if (index_temp!=-1) CdcCell_py = *(fMyRootInterface->get_vec_vecdouble(index_temp));
		for (int i = 0; i<CdcCell_py.size();i++) CdcCell_py[i] *= GeV;
		index_temp = fMyRootInterface->get_TBranch_index("CdcCell_pz");
		if (index_temp!=-1) CdcCell_pz = *(fMyRootInterface->get_vec_vecdouble(index_temp));
		for (int i = 0; i<CdcCell_pz.size();i++) CdcCell_pz[i] *= GeV;

		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Got info"<<std::endl;

		// find electron
		int index = 0;
		if (index == -1 ) continue;
		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Found electron"<<std::endl;
		inc_Ncut("Found electrons");

		// get information of this electron
		double x = McTruth_x[index];
		double y = McTruth_y[index];
		double z = McTruth_z[index];
		double px = McTruth_px[index];
		double py = McTruth_py[index];
		double pz = McTruth_pz[index];
		double e = McTruth_e[index];
		double pa = sqrt(px*px+py*py+pz*pz);
		double theta = (pa==0?2*PI:acos(pz/pa));
		double t = McTruth_time[index];
		std::string process = McTruth_process[index];
		int process_id = 0;
		if (process=="conv") process_id = 1;
		else if (process=="compt") process_id = 2;
		std::string volume = McTruth_volume[index];

		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0)
			std::cout<<prefix_EventInfoStart
				<<" pa = "<<pa/MeV
				<<"MeV, process = \""<<process
				<<"\", volume = \""<<volume
				<<"\""
				<<std::endl;

		if (volume!="Target"&&volume!="InnerCylinder")
			continue;
		inc_Ncut("Electron from target/InnerCylinder");
		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"In Target or InnerCylinder"<<std::endl;

		if (CdcCell_nHits<=0) // not hit the Cdc
			continue;
		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Found CDC hits"<<std::endl;
		inc_Ncut("Has CDC hits");

		int tid = McTruth_tid[index];
		int i_CdcHit = -1;
		int maxLayer = -1;
		double CdcCell_firstHitTime = -1;
		for ( int i_hit = 0; i_hit < CdcCell_nHits; i_hit++ ){
			int i_tid = CdcCell_tid[i_hit];
			if (i_tid == tid){
				CdcCell_firstHitTime = CdcCell_t[i_hit];
				i_CdcHit = i_hit;
				if (CdcCell_layerID[i_hit] > maxLayer) maxLayer = CdcCell_layerID[i_hit];
			}
		}
		if ( maxLayer <4 ) // this electron not hit the Cdc
			continue;
		inc_Ncut("This electron hit the 5th layer of CDC");
		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"This electron hit the 5th layer of CDC"<<std::endl;

		if (Trigger_nHits<=0) // not hit the trigger
			continue;
		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Found Trigger hits"<<std::endl;
		inc_Ncut("Has trigger htis");

		double Trigger_firstHitTime = -1;
		for ( int i_hit = 0; i_hit < Trigger_nHits; i_hit++ ){
			int i_tid = Trigger_tid[i_hit];
			if (i_tid == tid){
				Trigger_firstHitTime = Trigger_t[i_hit];
				break;
			}
		}
		if ( Trigger_firstHitTime == -1) // this electron not hit the trigger
			continue;
		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Found electron hits Trigger"<<std::endl;
		inc_Ncut("trigger hit by this electron");

		if ( Trigger_firstHitTime <= CdcCell_firstHitTime ) // hit trigger first
			continue;
		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Found electron hits CDC first"<<std::endl;
		inc_Ncut("Hit CDC first");

		// Fill the histogram
		double deltat = gRandom->Gaus()*100*ns;
		double smeared_time = CdcCell_firstHitTime + deltat;
		double smeared_ini_time = t + deltat;

		double e_CdcHit = CdcCell_e[i_CdcHit];
		double depE = e - e_CdcHit;

		double hit_x = CdcCell_x[i_CdcHit];
		double hit_y = CdcCell_y[i_CdcHit];
		double hit_z = CdcCell_z[i_CdcHit];
		double hit_px = CdcCell_px[i_CdcHit];
		double hit_py = CdcCell_py[i_CdcHit];
		double hit_pz = CdcCell_pz[i_CdcHit];
		double hit_pa = sqrt(hit_px*hit_px+hit_py*hit_py+hit_pz*hit_pz);

		if ( (index_temp = fMyRootInterface->get_TH1D_index(m_runName+"pa")) != -1 ){
			fMyRootInterface->get_TH1D(index_temp)->Fill(hit_pa/MeV);
		}
		if ( (index_temp = fMyRootInterface->get_TH1D_index(m_runName+"theta")) != -1 ){
			fMyRootInterface->get_TH1D(index_temp)->Fill(theta/rad);
		}
		if ( (index_temp = fMyRootInterface->get_TH1D_index(m_runName+"depE")) != -1 ){
			fMyRootInterface->get_TH1D(index_temp)->Fill(depE/MeV);
		}
		if ( (index_temp = fMyRootInterface->get_TH1D_index(m_runName+"hit_time")) != -1 ){
			fMyRootInterface->get_TH1D(index_temp)->Fill(CdcCell_firstHitTime/ns);
		}
		if ( (index_temp = fMyRootInterface->get_TH1D_index(m_runName+"smeared_hit_time")) != -1 ){
			fMyRootInterface->get_TH1D(index_temp)->Fill(smeared_time/ns);
		}
		if ( (index_temp = fMyRootInterface->get_TH1D_index(m_runName+"smear_time")) != -1 ){
			fMyRootInterface->get_TH1D(index_temp)->Fill(deltat/ns);
		}

		// Fill the tree
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("evt_num")) != -1 )
			fMyRootInterface->set_ovec_double(index_temp,evt_num); 
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("run_num")) != -1 )
			fMyRootInterface->set_ovec_double(index_temp,run_num); 
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("x")) != -1 )
			fMyRootInterface->set_ovec_double(index_temp,x/mm); 
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("y")) != -1 )
			fMyRootInterface->set_ovec_double(index_temp,y/mm); 
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("z")) != -1 )
			fMyRootInterface->set_ovec_double(index_temp,z/mm); 
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("px")) != -1 )
			fMyRootInterface->set_ovec_double(index_temp,px/MeV); 
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("py")) != -1 )
			fMyRootInterface->set_ovec_double(index_temp,py/MeV); 
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("pz")) != -1 )
			fMyRootInterface->set_ovec_double(index_temp,pz/MeV); 
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("t")) != -1 )
			fMyRootInterface->set_ovec_double(index_temp,t); 
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("dt")) != -1 )
			fMyRootInterface->set_ovec_double(index_temp,deltat); 
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("st")) != -1 )
			fMyRootInterface->set_ovec_double(index_temp,smeared_ini_time); 
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("sht")) != -1 )
			fMyRootInterface->set_ovec_double(index_temp,smeared_time); 
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("prid")) != -1 )
			fMyRootInterface->set_ovec_int(index_temp,process_id); 
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("vid")) != -1 )
			fMyRootInterface->set_ovec_string(index_temp,volume); 
		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Set oTrees"<<std::endl;

		fMyRootInterface->Fill();
		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Filled"<<std::endl;

		if ( smeared_time < 700*ns || smeared_time > 1314*ns ) // hit trigger first
			continue;
		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Found electron hits CDC first"<<std::endl;
		inc_Ncut("Time window");


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
