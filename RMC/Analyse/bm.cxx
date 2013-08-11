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
	while((result=getopt(argc,argv,"hbv:n:m:r:p:P:"))!=-1){
		switch(result){
			/* INPUTS */
			case 'm':
				m_MonitorPlane = optarg;
				printf("Monitor plane: %s\n",m_MonitorPlane.c_str());
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
	fMyRootInterface->set_OutputName("output");
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
		int evt_num;
		int run_num;
		int McTruth_nTracks = 0;
		std::vector<double> McTruth_x;
		std::vector<double> McTruth_y;
		std::vector<double> McTruth_z;
		std::vector<double> McTruth_px;
		std::vector<double> McTruth_py;
		std::vector<double> McTruth_pz;
		std::vector<double> McTruth_e;
		std::vector<double> McTruth_time;
		int CDCMonitor_nHits = 0;
		std::vector<int> CDCMonitor_tid;
		std::vector<int> CDCMonitor_pid;
		std::vector<double> CDCMonitor_t;
		std::vector<double> CDCMonitor_x;
		std::vector<double> CDCMonitor_y;
		std::vector<double> CDCMonitor_z;
		std::vector<double> CDCMonitor_px;
		std::vector<double> CDCMonitor_py;
		std::vector<double> CDCMonitor_pz;
		int BLTMonitor_nHits = 0;
		std::vector<int> BLTMonitor_tid;
		std::vector<int> BLTMonitor_pid;
		std::vector<double> BLTMonitor_t;
		std::vector<double> BLTMonitor_x;
		std::vector<double> BLTMonitor_y;
		std::vector<double> BLTMonitor_z;
		std::vector<double> BLTMonitor_px;
		std::vector<double> BLTMonitor_py;
		std::vector<double> BLTMonitor_pz;
		int Target_nHits = 0;
		std::vector<int> Target_tid;
		std::vector<int> Target_pid;
		std::vector<double> Target_t;
		std::vector<double> Target_x;
		std::vector<double> Target_y;
		std::vector<double> Target_z;
		std::vector<double> Target_px;
		std::vector<double> Target_py;
		std::vector<double> Target_pz;
		std::vector<int> Target_stopped;

		fMyRootInterface->get_value("evt_num",evt_num);
		fMyRootInterface->get_value("run_num",run_num);
		fMyRootInterface->get_value("McTruth_nTracks",McTruth_nTracks);
		fMyRootInterface->get_value("McTruth_x",McTruth_x,cm);
		fMyRootInterface->get_value("McTruth_y",McTruth_y,cm);
		fMyRootInterface->get_value("McTruth_z",McTruth_z,cm);
		fMyRootInterface->get_value("McTruth_px",McTruth_px,GeV);
		fMyRootInterface->get_value("McTruth_py",McTruth_py,GeV);
		fMyRootInterface->get_value("McTruth_pz",McTruth_pz,GeV);
		fMyRootInterface->get_value("McTruth_time",McTruth_time,ns);
		fMyRootInterface->get_value("McTruth_e",McTruth_e,GeV);
		fMyRootInterface->get_value("CDCMonitor_nHits",CDCMonitor_nHits);
		fMyRootInterface->get_value("CDCMonitor_t",CDCMonitor_t,ns);
		fMyRootInterface->get_value("CDCMonitor_tid",CDCMonitor_tid);
		fMyRootInterface->get_value("CDCMonitor_pid",CDCMonitor_pid);
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
		fMyRootInterface->get_value("BLTMonitor_x",BLTMonitor_x,cm);
		fMyRootInterface->get_value("BLTMonitor_y",BLTMonitor_y,cm);
		fMyRootInterface->get_value("BLTMonitor_z",BLTMonitor_z,cm);
		fMyRootInterface->get_value("BLTMonitor_px",BLTMonitor_px,GeV);
		fMyRootInterface->get_value("BLTMonitor_py",BLTMonitor_py,GeV);
		fMyRootInterface->get_value("BLTMonitor_pz",BLTMonitor_pz,GeV);
		fMyRootInterface->get_value("Target_nHits",Target_nHits);
		fMyRootInterface->get_value("Target_t",Target_t,ns);
		fMyRootInterface->get_value("Target_tid",Target_tid);
		fMyRootInterface->get_value("Target_pid",Target_pid);
		fMyRootInterface->get_value("Target_x",Target_x,cm);
		fMyRootInterface->get_value("Target_y",Target_y,cm);
		fMyRootInterface->get_value("Target_z",Target_z,cm);
		fMyRootInterface->get_value("Target_px",Target_px,GeV);
		fMyRootInterface->get_value("Target_py",Target_py,GeV);
		fMyRootInterface->get_value("Target_pz",Target_pz,GeV);
		fMyRootInterface->get_value("Target_stopped",Target_stopped);

		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Got info"<<std::endl;

		// Get info
		if (McTruth_nTracks<=0) continue;
		double opx = McTruth_px[0];
		double opy = McTruth_py[0];
		double opz = McTruth_pz[0];
		double opa = sqrt(opx*opx+opy*opy+opz*opz);
		double opt = sqrt(opx*opx+opy*opy);
		double oy = McTruth_y[0];
		if ( (index_temp = fMyRootInterface->get_TH1D_index("pa_total")) != -1 ){
			fMyRootInterface->get_TH1D(index_temp)->Fill(opa/MeV);
		}
		if ( (index_temp = fMyRootInterface->get_TH1D_index("pt_total")) != -1 ){
			fMyRootInterface->get_TH1D(index_temp)->Fill(opt/MeV);
		}
		if ( (index_temp = fMyRootInterface->get_TH1D_index("pz_total")) != -1 ){
			fMyRootInterface->get_TH1D(index_temp)->Fill(opz/MeV);
		}
		if ( (index_temp = fMyRootInterface->get_TH1D_index("y_total")) != -1 ){
			fMyRootInterface->get_TH1D(index_temp)->Fill(oy/mm);
		}
		if ( (index_temp = fMyRootInterface->get_TH2D_index("paVSy_total")) != -1 ){
			fMyRootInterface->get_TH2D(index_temp)->Fill(opa/MeV,oy/mm);
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
				fMyRootInterface->get_TH1D(index_temp)->Fill(opa/MeV);
			}
			if ( (index_temp = fMyRootInterface->get_TH1D_index("pt_CDC")) != -1 ){
				fMyRootInterface->get_TH1D(index_temp)->Fill(opt/MeV);
			}
			if ( (index_temp = fMyRootInterface->get_TH1D_index("pz_CDC")) != -1 ){
				fMyRootInterface->get_TH1D(index_temp)->Fill(opz/MeV);
			}
			if ( (index_temp = fMyRootInterface->get_TH1D_index("y_CDC")) != -1 ){
				fMyRootInterface->get_TH1D(index_temp)->Fill(oy/mm);
			}
			if ( (index_temp = fMyRootInterface->get_TH2D_index("paVSy_CDC")) != -1 ){
				fMyRootInterface->get_TH2D(index_temp)->Fill(opa/MeV,oy/mm);
			}
		}
		if (got_Target){
			if ( (index_temp = fMyRootInterface->get_TH1D_index("pa_Target")) != -1 ){
				fMyRootInterface->get_TH1D(index_temp)->Fill(opa/MeV);
			}
			if ( (index_temp = fMyRootInterface->get_TH1D_index("pt_Target")) != -1 ){
				fMyRootInterface->get_TH1D(index_temp)->Fill(opt/MeV);
			}
			if ( (index_temp = fMyRootInterface->get_TH1D_index("pz_Target")) != -1 ){
				fMyRootInterface->get_TH1D(index_temp)->Fill(opz/MeV);
			}
			if ( (index_temp = fMyRootInterface->get_TH1D_index("y_Target")) != -1 ){
				fMyRootInterface->get_TH1D(index_temp)->Fill(oy/mm);
			}
			if ( (index_temp = fMyRootInterface->get_TH2D_index("paVSy_Target")) != -1 ){
				fMyRootInterface->get_TH2D(index_temp)->Fill(opa/MeV,oy/mm);
			}
		}
		if (got_stopped){
			if ( (index_temp = fMyRootInterface->get_TH1D_index("pa_stop")) != -1 ){
				fMyRootInterface->get_TH1D(index_temp)->Fill(opa/MeV);
			}
			if ( (index_temp = fMyRootInterface->get_TH1D_index("pt_stop")) != -1 ){
				fMyRootInterface->get_TH1D(index_temp)->Fill(opt/MeV);
			}
			if ( (index_temp = fMyRootInterface->get_TH1D_index("pz_stop")) != -1 ){
				fMyRootInterface->get_TH1D(index_temp)->Fill(opz/MeV);
			}
			if ( (index_temp = fMyRootInterface->get_TH1D_index("y_stop")) != -1 ){
				fMyRootInterface->get_TH1D(index_temp)->Fill(oy/mm);
			}
			if ( (index_temp = fMyRootInterface->get_TH2D_index("paVSy_stop")) != -1 ){
				fMyRootInterface->get_TH2D(index_temp)->Fill(opa/MeV,oy/mm);
			}
		}
		if (got_recoiled){
			if ( (index_temp = fMyRootInterface->get_TH1D_index("pa_recoil")) != -1 ){
				fMyRootInterface->get_TH1D(index_temp)->Fill(opa/MeV);
			}
			if ( (index_temp = fMyRootInterface->get_TH1D_index("pt_recoil")) != -1 ){
				fMyRootInterface->get_TH1D(index_temp)->Fill(opt/MeV);
			}
			if ( (index_temp = fMyRootInterface->get_TH1D_index("pz_recoil")) != -1 ){
				fMyRootInterface->get_TH1D(index_temp)->Fill(opz/MeV);
			}
			if ( (index_temp = fMyRootInterface->get_TH1D_index("y_recoil")) != -1 ){
				fMyRootInterface->get_TH1D(index_temp)->Fill(oy/mm);
			}
			if ( (index_temp = fMyRootInterface->get_TH2D_index("paVSy_recoil")) != -1 ){
				fMyRootInterface->get_TH2D(index_temp)->Fill(opa/MeV,oy/mm);
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
	m_MonitorPlane="blt0";
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
