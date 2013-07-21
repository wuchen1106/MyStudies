#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <math.h>

#include "globals.hh"
#include "Randomize.hh"

#include "TH1D.h"
#include "TH2D.h"
#include "Math/DistFunc.h"
#include "TROOT.h"

#include "MyRootInterface.hxx"

char m_workMode[128];
std::string m_runName;
int verbose = 0;
int nEvents = 0;
int printModule = 1;
bool backup = false;
double sig_up = 106;
double sig_low = 103.8;

void init_args();
void print_usage(char* prog_name);

int main(int argc, char* argv[]){

	clock_t t_START = clock();

	//=======================================
	//*************read parameter**********
	init_args();
	int result;
	while((result=getopt(argc,argv,"hbv:u:l:n:m:r:p:"))!=-1){
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
			case 'u':
				sig_up = atof(optarg)*MeV;
				printf("up edge of signal window in MeV/c: %d\n",sig_up/MeV);
				break;
			case 'l':
				sig_up = atof(optarg)*MeV;
				printf("low edge of signal window in MeV/c: %d\n",sig_low/MeV);
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
	MyRootInterface *fMyRootInterface = new MyRootInterface(verbose);
	fMyRootInterface->set_OutputDir("result");
	int index_temp = 0;
	TH1D *h1d_temp=0;
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
	fMyRootInterface->set_OutputName(m_runName);

	//=>Add histograms

	int ihist_res = 0;
	int ihist_SPEC = 1;

	int num_bin = fMyRootInterface->get_bin1ForH1D(ihist_SPEC); //How many bins do you want
	name_temp = fMyRootInterface->get_nameForH1D(ihist_SPEC);
	title_temp = fMyRootInterface->get_titleForH1D(ihist_SPEC);
	xName_temp = fMyRootInterface->get_xNameForH1D(ihist_SPEC);
	yName_temp = fMyRootInterface->get_yNameForH1D(ihist_SPEC);
	bin1_temp = fMyRootInterface->get_bin1ForH1D(ihist_SPEC);
	left1_temp = fMyRootInterface->get_left1ForH1D(ihist_SPEC);
	right1_temp = fMyRootInterface->get_right1ForH1D(ihist_SPEC);
	minx_temp = fMyRootInterface->get_minxForH1D(ihist_SPEC);
	miny_temp = fMyRootInterface->get_minyForH1D(ihist_SPEC);
	color_temp = fMyRootInterface->get_colorForH1D(ihist_SPEC);
	compare_temp = fMyRootInterface->get_compareForH1D(ihist_SPEC);
	xlog_temp = fMyRootInterface->get_xlogForH1D(ihist_SPEC);
	ylog_temp = fMyRootInterface->get_ylogForH1D(ihist_SPEC);
	marker_temp = fMyRootInterface->get_markerForH1D(ihist_SPEC);
	norm_temp = fMyRootInterface->get_normForH1D(ihist_SPEC);
	drawOpt_temp = fMyRootInterface->get_drawOptForH1D(ihist_SPEC);

	name_temp = "MisMeas_hist";
	title_temp = "Momentum Cut Efficiency";
	xName_temp = "p_{a}(MeV/c)";
	yName_temp = "probability";
	miny_temp = 1e-5;
	ylog_temp = 1;
	int ihist_MisMeas = fMyRootInterface->add_TH1D(name_temp,title_temp,xName_temp,yName_temp,bin1_temp,left1_temp,right1_temp,minx_temp,miny_temp,color_temp,compare_temp,xlog_temp,ylog_temp,marker_temp,norm_temp,drawOpt_temp);

	name_temp = "contribution";
	title_temp = "Contribution Probobility";
	xName_temp = "p_{a}(MeV/c)";
	yName_temp = "probability";
	miny_temp = 0;
	ylog_temp = 0;
	int ihist_contri = fMyRootInterface->add_TH1D(name_temp,title_temp,xName_temp,yName_temp,bin1_temp,left1_temp,right1_temp,minx_temp,miny_temp,color_temp,compare_temp,xlog_temp,ylog_temp,marker_temp,norm_temp,drawOpt_temp);

	// Initialize
	fMyRootInterface->init();

	// get histograms
	TH1D* h_res = fMyRootInterface->get_TH1D(ihist_res);
	h_res->Scale(1./h_res->Integral());

	TH1D* h_SPEC = fMyRootInterface->get_TH1D(ihist_SPEC);
	h_SPEC->Scale(1./h_SPEC->Integral());

	TH1D* h_contri = fMyRootInterface->get_TH1D(ihist_contri);
	TH1D* h_MisMeas = fMyRootInterface->get_TH1D(ihist_MisMeas);

	//************SET Statistics********************
	if (verbose >= Verbose_SectorInfo ) std::cout<<prefix_SectorInfo<<"In SET Statistics###"<<std::endl;
	//=>About Statistical
	int N0 = 0;
	int N1 = 0;
	int N2 = 0;
	int N3 = 0;
	int N4 = 0;
	int N5 = 0;
	int N6 = 0;
	int N7 = 0;

	TH1D* h_conTot = 0; // should use MeV
	if ( (index_temp = fMyRootInterface->get_TH1D_index("contriTotal")) != -1 ){
		h_conTot = fMyRootInterface->get_TH1D(index_temp);
	}
	else{
		std::cout<<"ERROR: Can not find contribution total histogram"<<std::endl;
		return -1;
	}
	int nbins_conTot = h_conTot->GetNbinsX();

	//=>Calculate mis-measurement probability
	for ( int i = 1; i <= num_bin; i++ ){
		double E = h_SPEC->GetBinCenter(i)*MeV;
		double probabilityE = 0;
		double res_min = E - sig_up;
		double res_max = E - sig_low;
		int ibinmin_res = h_res->FindBin(res_min/MeV);
		int ibinmax_res = h_res->FindBin(res_max/MeV);
		probabilityE = h_res->Integral(ibinmin_res,ibinmax_res);
		h_MisMeas->SetBinContent(i,probabilityE/MeV);
	}

	//=>Calculate total mis-measurement probability
	double prob_tot = 0;
	std::vector<double> probs;
	for ( int i = 0; i < nbins_conTot; i++ ){
		probs.push_back(0.);
	}
	for ( int i = 0; i < num_bin; i++ ){
		double E = h_SPEC->GetBinCenter(i)*MeV;
		double prob_SPECE = h_SPEC->GetBinContent(i);
		double prob_mis = h_MisMeas->GetBinContent(i);
		for (int i = 1; i <= nbins_conTot; i++){
			double res_limit = h_conTot->GetBinCenter(i)*MeV;
			double res_min = E - sig_up;
			double res_max = E - sig_low;
			//std::cout<<"    res_min = "<<res_min
			//	     <<", res_max = "<<res_max
			//	     <<", res_limit = "<<res_limit
			//	     <<std::endl;
			if (res_min<res_limit) res_min = res_limit;
			double probabilityE = 0;
			if (res_min>res_max){
				probabilityE=0;
			} 
			else{
				int ibinmin_res = h_res->FindBin(res_min);
				int ibinmax_res = h_res->FindBin(res_max);
				probabilityE = h_res->Integral(ibinmin_res,ibinmax_res);
			}
			//std::cout<<"     probabilityE = "<<probabilityE
			//	     <<std::endl;
			probs[i-1]+=prob_SPECE*probabilityE;
		}
	//	std::cout<<"bin["<<i
	//		     <<"], E = "<<E
	//		     <<", prob_SPECE = "<<prob_SPECE
	//		     <<", prob_mis = "<<prob_mis
	//		     <<", prob_tot = "<<prob_tot
	//		     <<" + "<<prob_SPECE * prob_mis
	//		     <<std::endl;
		prob_tot += prob_SPECE * prob_mis; 
		h_contri->SetBinContent(i,prob_SPECE * prob_mis);
	}
	for ( int i = 1; i <= nbins_conTot; i++ ){
		h_conTot->SetBinContent(i,probs[i-1]);
	}
	std::cout<<"prob_tot = "<<prob_tot<<std::endl;

	//=>For output
	clock_t t_END = clock();
	//#########################THEEND###############################
	//************WRITE AND OUTPUT********************
	if (verbose >= Verbose_SectorInfo) std::cout<<prefix_SectorInfo<<"In WRITE AND OUTPUT ###"<<std::endl;
	std::cout<<"\n################BENTCH MARK###################"<<std::endl;
	std::cout<<"TOTAL TIME COST IS:  "<<(double)(t_END-t_START)/CLOCKS_PER_SEC*1000<<"ms"<<std::endl;
	//std::cout<<"PRESET:              "<<(double)(t_PRESET-t_START)/CLOCKS_PER_SEC*1000<<"ms"<<std::endl;
	//std::cout<<"INITIALIZE:          "<<(double)(t_INITIALIZE-t_PRESET)/CLOCKS_PER_SEC*1000<<"ms"<<std::endl;
	//std::cout<<"GENERATEEVENTS:      "<<(double)(t_GENERATEEVENTS-t_BUILDHISTOGRAMS)/CLOCKS_PER_SEC*1000<<"ms"<<std::endl;
	//std::cout<<"TIME COST PER EVENT: "<<(double)(t_GENERATEEVENTS-t_BUILDHISTOGRAMS)/CLOCKS_PER_SEC/num_evt*1000<<"ms"<<std::endl;
	std::cout<<"##############################################\n"<<std::endl;
	std::cout<<"N0 = "<<N0<<std::endl;
	std::cout<<"N1 = "<<N1<<std::endl;
	std::cout<<"N2 = "<<N2<<std::endl;
	std::cout<<"N3 = "<<N3<<std::endl;

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
	sig_up = 106*MeV;
	sig_low = 103.8*MeV;
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
	fprintf(stderr,"\t -u\n");
	fprintf(stderr,"\t\t up edge of signal window (in MeV/c)\n");
	fprintf(stderr,"\t -l\n");
	fprintf(stderr,"\t\t low edge of signal window (in MeV/c)\n");
	fprintf(stderr,"\t -p\n");
	fprintf(stderr,"\t\t printModule\n");
	fprintf(stderr,"\t -h\n");
	fprintf(stderr,"\t\t Usage message.\n");
	fprintf(stderr,"\t -b\n");
	fprintf(stderr,"\t\t restore backup file.\n");
	fprintf(stderr,"[example]\n");
	fprintf(stderr,"\t\t%s -m ab -v 20 -n 100\n",prog_name);
}
