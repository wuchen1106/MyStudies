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

#include "MyRootInterface.hxx"

char m_workMode[128];
std::string m_runName;
int verbose = 0;
int nEvents = 0;
int bEvent = 0;
int printModule = 1;
bool backup = false;
std::vector<int> Ncut;
std::vector<std::string> Ncut_message;

void init_Ncut();
void inc_Ncut(std::string);
void dump_Ncut();
void init_args();
void print_usage(char* prog_name);

int main(int argc, char** argv){

	clock_t t_START = clock();

	//=======================================
	//*************read parameter**********
	init_args();
	int result;
	while((result=getopt(argc,argv,"B:hbv:n:m:r:p:"))!=-1){
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
			case 'B':
				bEvent = atoi(optarg);
				printf("bEvent: %d\n",bEvent);
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
	double M_p = 0.9382723*GeV;  // mass of proton
	double M_N0 = 0.9395656*GeV; // mass of neutron

	//##########################Prepare histograms############################
	if (verbose >= Verbose_SectorInfo ) std::cout<<prefix_SectorInfo<<"In SET HISTOGRAMS###"<<std::endl;
	fMyRootInterface->read("input");
	fMyRootInterface->set_OutputName(m_runName);
	fMyRootInterface->init();

	//************SET Statistics********************
	if (verbose >= Verbose_SectorInfo ) std::cout<<prefix_SectorInfo<<"In SET Statistics###"<<std::endl;
	//=>About Statistical
	init_Ncut();

	std::vector<int> Monitor_pid;
	std::vector<double> Monitor_e;
	std::vector<double> Monitor_t;
	std::vector<double> Monitor_x;
	std::vector<double> Monitor_y;
	std::vector<double> Monitor_z;
	std::vector<double> Monitor_px;
	std::vector<double> Monitor_py;
	std::vector<double> Monitor_pz;

	std::vector<std::string> volumes;
	volumes.push_back("MonitorA");
	volumes.push_back("MonitorB");
//	volumes.push_back("MonitorC");
//	volumes.push_back("MonitorD");
	volumes.push_back("MonitorE");
	volumes.push_back("MonitorF");

	//=======================================================================================================
	//************DO THE DIRTY WORK*******************
	if (verbose >= Verbose_SectorInfo ) std::cout<<prefix_SectorInfo<<"In DO THE DIRTY WORK###"<<std::endl;
	Long64_t nEvent = fMyRootInterface->get_Entries();
	for( Long64_t iEvent = bEvent; iEvent < (nEvents&&nEvents<nEvent?nEvents:nEvent); iEvent++ ){
		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"In Event "<<iEvent<<std::endl;
		fMyRootInterface->GetEntry(iEvent);
		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Got entries"<<std::endl;
		inc_Ncut("Got entries");
		for ( int i_VL = 0; i_VL < volumes.size(); i_VL++ ){
			std::string Volume = volumes[i_VL];
			fMyRootInterface->get_value(Volume+"_x",Monitor_x,cm);
			fMyRootInterface->get_value(Volume+"_y",Monitor_y,cm);
			fMyRootInterface->get_value(Volume+"_z",Monitor_z,cm);
			fMyRootInterface->get_value(Volume+"_px",Monitor_px,GeV);
			fMyRootInterface->get_value(Volume+"_py",Monitor_py,GeV);
			fMyRootInterface->get_value(Volume+"_pz",Monitor_pz,GeV);
			fMyRootInterface->get_value(Volume+"_t",Monitor_t,ns);
			fMyRootInterface->get_value(Volume+"_e",Monitor_e,GeV);
			fMyRootInterface->get_value(Volume+"_pid",Monitor_pid);

			for ( int i_Mon = 0; i_Mon < Monitor_t.size(); i_Mon++ ){
				if (Monitor_pid[i_Mon]!=2112) continue;
				std::string name = Volume;
				if (name == "MonitorF") name = "MonitorE";
				fMyRootInterface->set_ovalue("x",Monitor_x[i_Mon]/mm);
				fMyRootInterface->set_ovalue("y",Monitor_y[i_Mon]/mm);
				fMyRootInterface->set_ovalue("z",Monitor_z[i_Mon]/mm);
				fMyRootInterface->set_ovalue("px",Monitor_px[i_Mon]/MeV);
				fMyRootInterface->set_ovalue("py",Monitor_py[i_Mon]/MeV);
				fMyRootInterface->set_ovalue("pz",Monitor_pz[i_Mon]/MeV);
				fMyRootInterface->set_ovalue("t",Monitor_t[i_Mon]/ns);
				fMyRootInterface->set_ovalue("volume",name);
				fMyRootInterface->Fill();

				double px = Monitor_px[i_Mon];
				double py = Monitor_py[i_Mon];
				double pz = Monitor_pz[i_Mon];
				double pa = sqrt(px*px+py*py+pz*pz);
				double theta = pa==0?0:acos(pz/pa);
				double x = Monitor_x[i_Mon];
				double y = Monitor_y[i_Mon];
				double r = sqrt(x*x+y*y);
				double Ekin = sqrt(pa*pa+M_N0*M_N0) - M_N0;
				if (name!="MonitorA")
					std::cout<<"pa = "<<pa/MeV<<"MeV/c, Ekin = "<<Ekin/keV<<"keV"<<std::endl;
				double t = Monitor_t[i_Mon];

				if ((index_temp = fMyRootInterface->get_TH1D_index(name+"_t"))!=-1)
					fMyRootInterface->get_TH1D(index_temp)->Fill(t/ms);
				if ((index_temp = fMyRootInterface->get_TH1D_index(name+"_theta"))!=-1)
					fMyRootInterface->get_TH1D(index_temp)->Fill(theta/rad);
				if ((index_temp = fMyRootInterface->get_TH1D_index(name+"_r"))!=-1)
					fMyRootInterface->get_TH1D(index_temp)->Fill(r/cm);
				if ((index_temp = fMyRootInterface->get_TH1D_index(name+"_pa"))!=-1)
					fMyRootInterface->get_TH1D(index_temp)->Fill(pa/MeV);
				if ((index_temp = fMyRootInterface->get_TH1D_index(name+"_Ekin"))!=-1)
					fMyRootInterface->get_TH1D(index_temp)->Fill(Ekin/keV);

				if ((index_temp = fMyRootInterface->get_TH2D_index(name+"_thetaVSEkin"))!=-1)
					fMyRootInterface->get_TH2D(index_temp)->Fill(theta/rad,Ekin/keV);
				if ((index_temp = fMyRootInterface->get_TH2D_index(name+"_rVSEkin"))!=-1)
					fMyRootInterface->get_TH2D(index_temp)->Fill(r/cm,Ekin/keV);
			}
		}
		//************If you need to fill TH1D*****************

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
	dump_Ncut();

	fMyRootInterface->dump();
	return 0;
}

void init_args()
{
	strcpy(m_workMode,"gen");
	m_runName="output";
	verbose = 0;
	nEvents = 0;
	bEvent = 0;
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
