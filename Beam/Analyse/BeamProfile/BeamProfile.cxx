#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <math.h>
#include <string>

#include "globals.hh"

#include "TH1D.h"
#include "TH2D.h"

#include "MyRootInterface.hxx"

std::string m_rootfile;
std::string m_prefix;
std::string m_suffix;
std::string m_workMode;
std::string m_title;
int verbose = 0;
int nEvents = 0;
int printModule = 1;
double m_scale = 0;
bool backup = false;
double m_miny = 0;

void init_args();
void print_usage(char* prog_name);

int main(int argc, char** argv){

	clock_t t_START = clock();

	//=======================================
	//*************read parameter**********
	init_args();
	int result;
	while((result=getopt(argc,argv,"hbv:n:m:t:s:x:y:p:l:"))!=-1){
		switch(result){
			/* INPUTS */
			case 'm':
				m_workMode = optarg;
				printf("work mode: %s\n",m_workMode.c_str());
				break;
			case 'x':
				m_prefix = optarg;
				printf("m_prefix: %s\n",m_prefix.c_str());
				break;
			case 'y':
				m_suffix = optarg;
				printf("m_suffix: %s\n",m_suffix.c_str());
				break;
			case 't':
				m_title=optarg;
				printf("run name: %s\n",m_title.c_str());
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
			case 's':
				m_scale = atof(optarg);
				printf("scale factor (inverse): %e\n",m_scale);
				break;
			case 'l':
				m_miny = atof(optarg);
				printf("minimum y (for log style only): %e\n",m_miny);
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
	if (m_workMode == "argu"){
		if (argc-optind<1){
			std::cout<<"This is \"argu\" mode which need arguments: rootfile"<<std::endl;
			std::cout<<"Insufficient names!"<<std::endl;
			return -1;
		}
		m_rootfile = argv[optind++];
		std::cout<<"m_rootfile= "<<m_rootfile<<std::endl;
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
	fMyRootInterface->add_oFileName(m_rootfile);
	int nHists = fMyRootInterface->get_TH1D_size();
	for (int iHist = 0; iHist < nHists; iHist++ ){
		fMyRootInterface->set_titleForH1D(iHist,m_title);
		std::string name = fMyRootInterface->get_nameForH1D(iHist);
		fMyRootInterface->set_nameForH1D(iHist,m_prefix+"_"+name+"_"+m_suffix);
		fMyRootInterface->set_normForH1D(iHist,m_scale);
		// add log style
		name_temp = fMyRootInterface->get_nameForH1D(iHist);
		title_temp = fMyRootInterface->get_titleForH1D(iHist);
		xName_temp = fMyRootInterface->get_xNameForH1D(iHist);
		yName_temp = fMyRootInterface->get_yNameForH1D(iHist);
		bin1_temp = fMyRootInterface->get_bin1ForH1D(iHist);
		left1_temp = fMyRootInterface->get_left1ForH1D(iHist);
		right1_temp = fMyRootInterface->get_right1ForH1D(iHist);
		minx_temp = fMyRootInterface->get_minxForH1D(iHist);
		miny_temp = fMyRootInterface->get_minyForH1D(iHist);
		color_temp = fMyRootInterface->get_colorForH1D(iHist);
		compare_temp = fMyRootInterface->get_compareForH1D(iHist);
		xlog_temp = fMyRootInterface->get_xlogForH1D(iHist);
		ylog_temp = fMyRootInterface->get_ylogForH1D(iHist);
		marker_temp = fMyRootInterface->get_markerForH1D(iHist);
		norm_temp = fMyRootInterface->get_normForH1D(iHist);
		drawOpt_temp = fMyRootInterface->get_drawOptForH1D(iHist);

		name_temp = m_prefix+"_"+name+"_log_"+m_suffix;
		miny_temp = m_miny;
		ylog_temp = 1;
		fMyRootInterface->add_TH1D(name_temp,title_temp,xName_temp,yName_temp,bin1_temp,left1_temp,right1_temp,minx_temp,miny_temp,color_temp,compare_temp,xlog_temp,ylog_temp,marker_temp,norm_temp,drawOpt_temp);
	}
	nHists = fMyRootInterface->get_TH2D_size();
	for (int iHist = 0; iHist < nHists; iHist++ ){
		fMyRootInterface->set_titleForH2D(iHist,m_title);
		std::string name = fMyRootInterface->get_nameForH2D(iHist);
		fMyRootInterface->set_nameForH2D(iHist,m_prefix+"_"+name+"_"+m_suffix);
	}
	fMyRootInterface->set_OutputName(m_prefix+"_"+m_suffix+"_");
	fMyRootInterface->init();

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

	//=======================================================================================================
	//************DO THE DIRTY WORK*******************
	if (verbose >= Verbose_SectorInfo ) std::cout<<prefix_SectorInfo<<"In DO THE DIRTY WORK###"<<std::endl;
	Long64_t nEvent = fMyRootInterface->get_Entries();
	for( Long64_t iEvent = 0; iEvent < (nEvents&&nEvents<nEvent?nEvents:nEvent); iEvent++ ){
		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"In Event "<<iEvent<<std::endl;
		N0++;
		fMyRootInterface->GetEntry(iEvent);
		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Got entries"<<std::endl;

		double x;
		double y;
		double z;
		double px;
		double py;
		double pz;
		double t;
		double ox;
		double oy;
		double oz;

		fMyRootInterface->get_value("x",x,mm);
		fMyRootInterface->get_value("y",y,mm);
		fMyRootInterface->get_value("z",z,mm);
		fMyRootInterface->get_value("px",px,MeV);
		fMyRootInterface->get_value("py",py,MeV);
		fMyRootInterface->get_value("pz",pz,MeV);
		fMyRootInterface->get_value("t",t,ns);
		fMyRootInterface->get_value("ox",ox,mm);
		fMyRootInterface->get_value("oy",oy,mm);
		fMyRootInterface->get_value("oz",oz,mm);

		double pa = sqrt(px*px+py*py+pz*pz);
		double pt = sqrt(px*px+py*py);
		double r = sqrt(x*x+y*y);
		double theta = acos(pz/pa);
		index_temp = fMyRootInterface->get_TH1D_index(m_prefix+"_"+"pa"+"_"+m_suffix);
		if (index_temp!=-1) fMyRootInterface->get_TH1D(index_temp)->Fill(pa/MeV);
		index_temp = fMyRootInterface->get_TH1D_index(m_prefix+"_"+"pt"+"_"+m_suffix);
		if (index_temp!=-1) fMyRootInterface->get_TH1D(index_temp)->Fill(pt/MeV);
		index_temp = fMyRootInterface->get_TH1D_index(m_prefix+"_"+"pz"+"_"+m_suffix);
		if (index_temp!=-1)	fMyRootInterface->get_TH1D(index_temp)->Fill(pz/MeV);
		index_temp = fMyRootInterface->get_TH1D_index(m_prefix+"_"+"pa_tail"+"_"+m_suffix);
		if (index_temp!=-1) fMyRootInterface->get_TH1D(index_temp)->Fill(pa/MeV);
		index_temp = fMyRootInterface->get_TH1D_index(m_prefix+"_"+"pt_tail"+"_"+m_suffix);
		if (index_temp!=-1) fMyRootInterface->get_TH1D(index_temp)->Fill(pt/MeV);
		index_temp = fMyRootInterface->get_TH1D_index(m_prefix+"_"+"pz_tail"+"_"+m_suffix);
		if (index_temp!=-1)	fMyRootInterface->get_TH1D(index_temp)->Fill(pz/MeV);
		index_temp = fMyRootInterface->get_TH1D_index(m_prefix+"_"+"theta"+"_"+m_suffix);
		if (index_temp!=-1) fMyRootInterface->get_TH1D(index_temp)->Fill(theta/MeV);
		index_temp = fMyRootInterface->get_TH1D_index(m_prefix+"_"+"gTime"+"_"+m_suffix);
		if (index_temp!=-1) fMyRootInterface->get_TH1D(index_temp)->Fill(t/ns);
		index_temp = fMyRootInterface->get_TH1D_index(m_prefix+"_"+"gTime_tail"+"_"+m_suffix);
		if (index_temp!=-1) fMyRootInterface->get_TH1D(index_temp)->Fill(t/ns);
		index_temp = fMyRootInterface->get_TH1D_index(m_prefix+"_"+"r"+"_"+m_suffix);
		if (index_temp!=-1) fMyRootInterface->get_TH1D(index_temp)->Fill(r/mm);
		index_temp = fMyRootInterface->get_TH1D_index(m_prefix+"_"+"ox"+"_"+m_suffix);
		if (index_temp!=-1) fMyRootInterface->get_TH1D(index_temp)->Fill(ox/mm);
		index_temp = fMyRootInterface->get_TH1D_index(m_prefix+"_"+"oy"+"_"+m_suffix);
		if (index_temp!=-1) fMyRootInterface->get_TH1D(index_temp)->Fill(oy/mm);
		index_temp = fMyRootInterface->get_TH1D_index(m_prefix+"_"+"oz"+"_"+m_suffix);
		if (index_temp!=-1) fMyRootInterface->get_TH1D(index_temp)->Fill(oz/mm);

		index_temp = fMyRootInterface->get_TH1D_index(m_prefix+"_"+"pa"+"_log_"+m_suffix);
		if (index_temp!=-1) fMyRootInterface->get_TH1D(index_temp)->Fill(pa/MeV);
		index_temp = fMyRootInterface->get_TH1D_index(m_prefix+"_"+"pt"+"_log_"+m_suffix);
		if (index_temp!=-1) fMyRootInterface->get_TH1D(index_temp)->Fill(pt/MeV);
		index_temp = fMyRootInterface->get_TH1D_index(m_prefix+"_"+"pz"+"_log_"+m_suffix);
		if (index_temp!=-1)	fMyRootInterface->get_TH1D(index_temp)->Fill(pz/MeV);
		index_temp = fMyRootInterface->get_TH1D_index(m_prefix+"_"+"pa_tail"+"_log_"+m_suffix);
		if (index_temp!=-1) fMyRootInterface->get_TH1D(index_temp)->Fill(pa/MeV);
		index_temp = fMyRootInterface->get_TH1D_index(m_prefix+"_"+"pt_tail"+"_log_"+m_suffix);
		if (index_temp!=-1) fMyRootInterface->get_TH1D(index_temp)->Fill(pt/MeV);
		index_temp = fMyRootInterface->get_TH1D_index(m_prefix+"_"+"pz_tail"+"_log_"+m_suffix);
		if (index_temp!=-1)	fMyRootInterface->get_TH1D(index_temp)->Fill(pz/MeV);
		index_temp = fMyRootInterface->get_TH1D_index(m_prefix+"_"+"theta"+"_log_"+m_suffix);
		if (index_temp!=-1) fMyRootInterface->get_TH1D(index_temp)->Fill(theta/MeV);
		index_temp = fMyRootInterface->get_TH1D_index(m_prefix+"_"+"gTime"+"_log_"+m_suffix);
		if (index_temp!=-1) fMyRootInterface->get_TH1D(index_temp)->Fill(t/ns);
		index_temp = fMyRootInterface->get_TH1D_index(m_prefix+"_"+"gTime_tail"+"_log_"+m_suffix);
		if (index_temp!=-1) fMyRootInterface->get_TH1D(index_temp)->Fill(t/ns);
		index_temp = fMyRootInterface->get_TH1D_index(m_prefix+"_"+"r"+"_log_"+m_suffix);
		if (index_temp!=-1) fMyRootInterface->get_TH1D(index_temp)->Fill(r/mm);
		index_temp = fMyRootInterface->get_TH1D_index(m_prefix+"_"+"ox"+"_log_"+m_suffix);
		if (index_temp!=-1) fMyRootInterface->get_TH1D(index_temp)->Fill(ox/mm);
		index_temp = fMyRootInterface->get_TH1D_index(m_prefix+"_"+"oy"+"_log_"+m_suffix);
		if (index_temp!=-1) fMyRootInterface->get_TH1D(index_temp)->Fill(oy/mm);
		index_temp = fMyRootInterface->get_TH1D_index(m_prefix+"_"+"oz"+"_log_"+m_suffix);
		if (index_temp!=-1) fMyRootInterface->get_TH1D(index_temp)->Fill(oz/mm);

		index_temp = fMyRootInterface->get_TH2D_index(m_prefix+"_"+"thetar"+"_"+m_suffix);
		if (index_temp!=-1) fMyRootInterface->get_TH2D(index_temp)->Fill(theta/rad,r/mm);
		index_temp = fMyRootInterface->get_TH2D_index(m_prefix+"_"+"patheta"+"_"+m_suffix);
		if (index_temp!=-1) fMyRootInterface->get_TH2D(index_temp)->Fill(pa/MeV,theta/rad);
		index_temp = fMyRootInterface->get_TH2D_index(m_prefix+"_"+"paox"+"_"+m_suffix);
		if (index_temp!=-1) fMyRootInterface->get_TH2D(index_temp)->Fill(pa/MeV,ox/mm);
		index_temp = fMyRootInterface->get_TH2D_index(m_prefix+"_"+"ptox"+"_"+m_suffix);
		if (index_temp!=-1) fMyRootInterface->get_TH2D(index_temp)->Fill(pt/MeV,ox/mm);
		index_temp = fMyRootInterface->get_TH2D_index(m_prefix+"_"+"pzox"+"_"+m_suffix);
		if (index_temp!=-1) fMyRootInterface->get_TH2D(index_temp)->Fill(pz/MeV,ox/mm);
		index_temp = fMyRootInterface->get_TH2D_index(m_prefix+"_"+"paoy"+"_"+m_suffix);
		if (index_temp!=-1) fMyRootInterface->get_TH2D(index_temp)->Fill(pa/MeV,oy/mm);
		index_temp = fMyRootInterface->get_TH2D_index(m_prefix+"_"+"ptoy"+"_"+m_suffix);
		if (index_temp!=-1) fMyRootInterface->get_TH2D(index_temp)->Fill(pt/MeV,oy/mm);
		index_temp = fMyRootInterface->get_TH2D_index(m_prefix+"_"+"pzoy"+"_"+m_suffix);
		if (index_temp!=-1) fMyRootInterface->get_TH2D(index_temp)->Fill(pz/MeV,oy/mm);
		index_temp = fMyRootInterface->get_TH2D_index(m_prefix+"_"+"paoz"+"_"+m_suffix);
		if (index_temp!=-1) fMyRootInterface->get_TH2D(index_temp)->Fill(pa/MeV,oz/mm);
		index_temp = fMyRootInterface->get_TH2D_index(m_prefix+"_"+"ptoz"+"_"+m_suffix);
		if (index_temp!=-1) fMyRootInterface->get_TH2D(index_temp)->Fill(pt/MeV,oz/mm);
		index_temp = fMyRootInterface->get_TH2D_index(m_prefix+"_"+"pzoz"+"_"+m_suffix);
		if (index_temp!=-1) fMyRootInterface->get_TH2D(index_temp)->Fill(pz/MeV,oz/mm);
		index_temp = fMyRootInterface->get_TH2D_index(m_prefix+"_"+"par"+"_"+m_suffix);
		if (index_temp!=-1) fMyRootInterface->get_TH2D(index_temp)->Fill(pa/MeV,r/mm);
		index_temp = fMyRootInterface->get_TH2D_index(m_prefix+"_"+"ptr"+"_"+m_suffix);
		if (index_temp!=-1) fMyRootInterface->get_TH2D(index_temp)->Fill(pt/MeV,r/mm);
		index_temp = fMyRootInterface->get_TH2D_index(m_prefix+"_"+"pzr"+"_"+m_suffix);
		if (index_temp!=-1) fMyRootInterface->get_TH2D(index_temp)->Fill(pz/MeV,r/mm);
		index_temp = fMyRootInterface->get_TH2D_index(m_prefix+"_"+"patheta_tail"+"_"+m_suffix);
		if (index_temp!=-1) fMyRootInterface->get_TH2D(index_temp)->Fill(pa/MeV,theta/rad);
		index_temp = fMyRootInterface->get_TH2D_index(m_prefix+"_"+"paox_tail"+"_"+m_suffix);
		if (index_temp!=-1) fMyRootInterface->get_TH2D(index_temp)->Fill(pa/MeV,ox/mm);
		index_temp = fMyRootInterface->get_TH2D_index(m_prefix+"_"+"ptox_tail"+"_"+m_suffix);
		if (index_temp!=-1) fMyRootInterface->get_TH2D(index_temp)->Fill(pt/MeV,ox/mm);
		index_temp = fMyRootInterface->get_TH2D_index(m_prefix+"_"+"pzox_tail"+"_"+m_suffix);
		if (index_temp!=-1) fMyRootInterface->get_TH2D(index_temp)->Fill(pz/MeV,ox/mm);
		index_temp = fMyRootInterface->get_TH2D_index(m_prefix+"_"+"paoy_tail"+"_"+m_suffix);
		if (index_temp!=-1) fMyRootInterface->get_TH2D(index_temp)->Fill(pa/MeV,oy/mm);
		index_temp = fMyRootInterface->get_TH2D_index(m_prefix+"_"+"ptoy_tail"+"_"+m_suffix);
		if (index_temp!=-1) fMyRootInterface->get_TH2D(index_temp)->Fill(pt/MeV,oy/mm);
		index_temp = fMyRootInterface->get_TH2D_index(m_prefix+"_"+"pzoy_tail"+"_"+m_suffix);
		if (index_temp!=-1) fMyRootInterface->get_TH2D(index_temp)->Fill(pz/MeV,oy/mm);
		index_temp = fMyRootInterface->get_TH2D_index(m_prefix+"_"+"paoz_tail"+"_"+m_suffix);
		if (index_temp!=-1) fMyRootInterface->get_TH2D(index_temp)->Fill(pa/MeV,oz/mm);
		index_temp = fMyRootInterface->get_TH2D_index(m_prefix+"_"+"ptoz_tail"+"_"+m_suffix);
		if (index_temp!=-1) fMyRootInterface->get_TH2D(index_temp)->Fill(pt/MeV,oz/mm);
		index_temp = fMyRootInterface->get_TH2D_index(m_prefix+"_"+"pzoz_tail"+"_"+m_suffix);
		if (index_temp!=-1) fMyRootInterface->get_TH2D(index_temp)->Fill(pz/MeV,oz/mm);
		index_temp = fMyRootInterface->get_TH2D_index(m_prefix+"_"+"par_tail"+"_"+m_suffix);
		if (index_temp!=-1) fMyRootInterface->get_TH2D(index_temp)->Fill(pa/MeV,r/mm);
		index_temp = fMyRootInterface->get_TH2D_index(m_prefix+"_"+"ptr_tail"+"_"+m_suffix);
		if (index_temp!=-1) fMyRootInterface->get_TH2D(index_temp)->Fill(pt/MeV,r/mm);
		index_temp = fMyRootInterface->get_TH2D_index(m_prefix+"_"+"pzr_tail"+"_"+m_suffix);
		if (index_temp!=-1) fMyRootInterface->get_TH2D(index_temp)->Fill(pz/MeV,r/mm);

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
	std::cout<<"N0 = "<<N0<<std::endl;
	std::cout<<"N1 = "<<N1<<std::endl;
	std::cout<<"N2 = "<<N2<<std::endl;
	std::cout<<"N3 = "<<N3<<std::endl;

	fMyRootInterface->dump();
	return 0;
}

void init_args()
{
	m_rootfile="";
	m_prefix="";
	m_suffix="";
	m_title="";
	m_workMode="file";
	verbose = 0;
	nEvents = 0;
	printModule = 10000;
	m_scale = 0;
	m_miny= 0;
	backup = false;
}

void print_usage(char* prog_name)
{
	fprintf(stderr,"Usage %s [options (args)] [input files]\n",prog_name);
	fprintf(stderr,"[options]\n");
	fprintf(stderr,"\t -m\n");
	fprintf(stderr,"\t\t choose work mode: [gen(default), com]\n");
	fprintf(stderr,"\t -t\n");
	fprintf(stderr,"\t\t set title\n");
	fprintf(stderr,"\t -v\n");
	fprintf(stderr,"\t\t verbose level\n");
	fprintf(stderr,"\t -n\n");
	fprintf(stderr,"\t\t nEvent\n");
	fprintf(stderr,"\t -p\n");
	fprintf(stderr,"\t\t printModule\n");
	fprintf(stderr,"\t -s\n");
	fprintf(stderr,"\t\t m_scale\n");
	fprintf(stderr,"\t -l\n");
	fprintf(stderr,"\t\t m_miny\n");
	fprintf(stderr,"\t -x\n");
	fprintf(stderr,"\t\t m_prefix\n");
	fprintf(stderr,"\t -y\n");
	fprintf(stderr,"\t\t m_suffix\n");
	fprintf(stderr,"\t -h\n");
	fprintf(stderr,"\t\t Usage message.\n");
	fprintf(stderr,"\t -b\n");
	fprintf(stderr,"\t\t restore backup file.\n");
	fprintf(stderr,"[example]\n");
	fprintf(stderr,"\t\t%s -m ab -v 20 -n 100\n",prog_name);
}
