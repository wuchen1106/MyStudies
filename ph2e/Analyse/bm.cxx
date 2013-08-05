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
	std::string title_temp = "";
	std::string  xName_temp = "";
	std::string  yName_temp = "";
	int  bin1_temp = 0;
	double  left1_temp = 0;
	double  right1_temp = 0;
	double  minx_temp = 0;
	double  miny_temp = 0;
	int  color_temp = 0;
	int  compare_temp = 0;
	int  xlog_temp = 0;
	int  ylog_temp = 0;
	int  marker_temp = 0;
	double  norm_temp = 0;
	std::string  drawOpt_temp = "";
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

	h1d_temp = fMyRootInterface->get_TH1D(0);
	bin1_temp = fMyRootInterface->get_bin1ForH1D(0);
	left1_temp = fMyRootInterface->get_left1ForH1D(0);
	right1_temp = fMyRootInterface->get_right1ForH1D(0);

	//************SET Statistics********************
	if (verbose >= Verbose_SectorInfo ) std::cout<<prefix_SectorInfo<<"In SET Statistics###"<<std::endl;
	std::vector<double> count;
	std::vector<double> count_IW;
	std::vector<double> count_TG;
	std::vector<double> count_IW_PP;
	std::vector<double> count_TG_PP;
	std::vector<double> count_IW_CS;
	std::vector<double> count_TG_CS;
	std::vector<double> count_PP;
	std::vector<double> count_CS;
	//=>About Statistical
	init_Ncut();
	for ( int i = 0 ; i < bin1_temp; i++ ){
		count.push_back(0);
		count_IW.push_back(0);
		count_TG.push_back(0);
		count_IW_PP.push_back(0);
		count_TG_PP.push_back(0);
		count_IW_CS.push_back(0);
		count_TG_CS.push_back(0);
		count_PP.push_back(0);
		count_CS.push_back(0);
	}

	//=======================================================================================================
	//************DO THE DIRTY WORK*******************
	if (verbose >= Verbose_SectorInfo ) std::cout<<prefix_SectorInfo<<"In DO THE DIRTY WORK###"<<std::endl;
	Long64_t nEvent = fMyRootInterface->get_Entries();
	if (nEvents&&nEvent>nEvents) nEvent = nEvents;
	if (verbose >= Verbose_SectorInfo ) std::cout<<prefix_SectorInfo<<nEvent<<" Events to be dealed with###"<<std::endl;
	for( Long64_t iEvent = 0; iEvent < nEvent; iEvent++ ){
		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"In Event "<<iEvent<<std::endl;
		fMyRootInterface->GetEntry(iEvent);
		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Got entries"<<std::endl;
		inc_Ncut("Got entries");

		// For output

		// Get info
		int McTruth_nTracks = 0;
		std::vector<double> McTruth_px;
		std::vector<double> McTruth_py;
		std::vector<double> McTruth_pz;
		std::vector<std::string> McTruth_process;
		std::vector<std::string> McTruth_volume;

		fMyRootInterface->get_value("McTruth_nTracks",McTruth_nTracks);
		fMyRootInterface->get_value("McTruth_px",McTruth_px,GeV);
		fMyRootInterface->get_value("McTruth_py",McTruth_py,GeV);
		fMyRootInterface->get_value("McTruth_pz",McTruth_pz,GeV);
		fMyRootInterface->get_value("McTruth_process",McTruth_process);
		fMyRootInterface->get_value("McTruth_volume",McTruth_volume);

		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Got info"<<std::endl;

		// Get info
		double pa_max = 0;
		std::string volume = "";
		std::string process = "";
		for (int iMc = 0; iMc < McTruth_nTracks; iMc++ ){
			double px = McTruth_px[iMc];
			double py = McTruth_py[iMc];
			double pz = McTruth_pz[iMc];
			double pa = sqrt(px*px+py*py+pz*pz);
			if (pa > pa_max){
				pa_max = pa;
				volume = McTruth_volume[iMc];
				process = McTruth_process[iMc];
			}
		}

		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"pa = "<<pa_max/MeV<<"MeV, from "<<volume<<std::endl;

		for (int i_bin = 1; i_bin <= bin1_temp; i_bin++){
			double pa = h1d_temp->GetBinCenter(i_bin)*MeV;
			if (verbose >= Verbose_ParticleInfo || iEvent%printModule == 0) std::cout<<prefix_ParticleInfoStart<<"pamin = "<<pa/MeV<<"MeV"<<std::endl;
			if (pa_max > pa){
				if (verbose >= Verbose_ParticleInfo || iEvent%printModule == 0) std::cout<<prefix_ParticleInfoStart<<"Found it!!!"<<std::endl;
				if( volume == "Target" ){
					if (verbose >= Verbose_ParticleInfo || iEvent%printModule == 0) std::cout<<prefix_ParticleInfoStart<<"From Target!!!"<<std::endl;
					count_TG[i_bin-1]++;
					if (process=="conv") count_TG_PP[i_bin-1]++;
					else if (process=="compt") count_TG_CS[i_bin-1]++;
				}
				else if( volume == "InnerCylinder" ){
					if (verbose >= Verbose_ParticleInfo || iEvent%printModule == 0) std::cout<<prefix_ParticleInfoStart<<"From InnerCylinder!!!"<<std::endl;
					count_IW[i_bin-1]++;
					if (process=="conv") count_IW_PP[i_bin-1]++;
					else if (process=="compt") count_IW_CS[i_bin-1]++;
				}
				if (process=="conv") count_PP[i_bin-1]++;
				else if (process=="compt") count_CS[i_bin-1]++;
				count[i_bin-1]++;
			}
		}

		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfo<<"Finished!"<<std::endl;
	}/* end of loop in events*/

	//Fill Histograms
	for (int i_bin = 1; i_bin <= bin1_temp; i_bin++){
		double ratio = 0;
		if (count_IW[i_bin-1]!=0){
			ratio = ((double)(count_TG[i_bin-1]))/((double)(count_IW[i_bin-1]));
		}
		h1d_temp->SetBinContent(i_bin,ratio);
		if (verbose >= Verbose_Statistics)
			std::cout<<prefix_Statistics
					 <<"minpa="<<h1d_temp->GetBinCenter(i_bin)<<" MeV/c"
					 <<", count_TG["<<i_bin-1<<"]"<<"="<<count_TG[i_bin-1]
					 <<", count_IW["<<i_bin-1<<"]"<<"="<<count_IW[i_bin-1]
					 <<", ratio = "<<ratio
					 <<std::endl;
	}
	h1d_temp=fMyRootInterface->get_TH1D(1);
	for (int i_bin = 1; i_bin <= bin1_temp; i_bin++){
		double ratio = 0;
		if (count_IW_PP[i_bin-1]!=0){
			ratio = ((double)(count_TG_PP[i_bin-1]))/((double)(count_IW_PP[i_bin-1]));
		}
		h1d_temp->SetBinContent(i_bin,ratio);
		if (verbose >= Verbose_Statistics)
			std::cout<<prefix_Statistics
					 <<"minpa="<<h1d_temp->GetBinCenter(i_bin)<<" MeV/c"
					 <<", count_TG_PP["<<i_bin-1<<"]"<<"="<<count_TG_PP[i_bin-1]
					 <<", count_IW_PP["<<i_bin-1<<"]"<<"="<<count_IW_PP[i_bin-1]
					 <<", ratio = "<<ratio
					 <<std::endl;
	}
	h1d_temp=fMyRootInterface->get_TH1D(2);
	for (int i_bin = 1; i_bin <= bin1_temp; i_bin++){
		double ratio = 0;
		if (count_IW_CS[i_bin-1]!=0){
			ratio = ((double)(count_TG_CS[i_bin-1]))/((double)(count_IW_CS[i_bin-1]));
		}
		h1d_temp->SetBinContent(i_bin,ratio);
		if (verbose >= Verbose_Statistics)
			std::cout<<prefix_Statistics
					 <<"minpa="<<h1d_temp->GetBinCenter(i_bin)<<" MeV/c"
					 <<", count_TG_CS["<<i_bin-1<<"]"<<"="<<count_TG_CS[i_bin-1]
					 <<", count_IW_CS["<<i_bin-1<<"]"<<"="<<count_IW_CS[i_bin-1]
					 <<", ratio = "<<ratio
					 <<std::endl;
	}
	h1d_temp=fMyRootInterface->get_TH1D(3);
	for (int i_bin = 1; i_bin <= bin1_temp; i_bin++){
		double ratio = 0;
		if (count[i_bin-1]!=0){
			ratio = ((double)(count_PP[i_bin-1]))/((double)(count[i_bin-1]));
		}
		h1d_temp->SetBinContent(i_bin,ratio);
		if (verbose >= Verbose_Statistics)
			std::cout<<prefix_Statistics
					 <<"minpa="<<h1d_temp->GetBinCenter(i_bin)<<" MeV/c"
					 <<", count["<<i_bin-1<<"]"<<"="<<count[i_bin-1]
					 <<", count_PP["<<i_bin-1<<"]"<<"="<<count_PP[i_bin-1]
					 <<", ratio = "<<ratio
					 <<std::endl;
	}
	h1d_temp=fMyRootInterface->get_TH1D(4);
	for (int i_bin = 1; i_bin <= bin1_temp; i_bin++){
		double ratio = 0;
		if (count[i_bin-1]!=0){
			ratio = ((double)(count_CS[i_bin-1]))/((double)(count[i_bin-1]));
		}
		h1d_temp->SetBinContent(i_bin,ratio);
		if (verbose >= Verbose_Statistics)
			std::cout<<prefix_Statistics
					 <<"minpa="<<h1d_temp->GetBinCenter(i_bin)<<" MeV/c"
					 <<", count["<<i_bin-1<<"]"<<"="<<count[i_bin-1]
					 <<", count_CS["<<i_bin-1<<"]"<<"="<<count_CS[i_bin-1]
					 <<", ratio = "<<ratio
					 <<std::endl;
	}

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
