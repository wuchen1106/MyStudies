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
	TH2D *h2d_temp=0;
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
	int nHists = fMyRootInterface->get_TH1D_size();
	for (int iHist = 0; iHist < nHists; iHist++ ){
		std::string name = fMyRootInterface->get_nameForH1D(iHist);
		fMyRootInterface->set_nameForH1D(iHist,m_prefix+"_"+name);
	}
	nHists = fMyRootInterface->get_TH2D_size();
	for (int iHist = 0; iHist < nHists; iHist++ ){
		std::string name = fMyRootInterface->get_nameForH2D(iHist);
		fMyRootInterface->set_nameForH2D(iHist,m_prefix+"_"+name);
	}
	fMyRootInterface->init();

	//************SET Statistics********************
	if (verbose >= Verbose_SectorInfo ) std::cout<<prefix_SectorInfo<<"In SET Statistics###"<<std::endl;
	//=>About Statistical

	//=======================================================================================================
	//************DO THE DIRTY WORK*******************
	if (verbose >= Verbose_SectorInfo ) std::cout<<prefix_SectorInfo<<"In DO THE DIRTY WORK###"<<std::endl;
	Long64_t nEvent = fMyRootInterface->get_Entries();
	for( Long64_t iEvent = 0; iEvent < (nEvents&&nEvents<nEvent?nEvents:nEvent); iEvent++ ){
		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"In Event "<<iEvent<<std::endl;
		fMyRootInterface->GetEntry(iEvent);
		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Got entries"<<std::endl;

		double x;
		double y;
		double z;
		double bx;
		double by;
		double bz;

		if (m_workMode == "UK"){
			fMyRootInterface->get_value("x",x,mm);
			fMyRootInterface->get_value("y",y,mm);
			fMyRootInterface->get_value("z",z,mm);
			fMyRootInterface->get_value("bx",bx,-tesla);
			fMyRootInterface->get_value("by",by,tesla);
			fMyRootInterface->get_value("bz",bz,-tesla);
			if (x>600*mm||x<-600*mm) continue;

			index_temp = fMyRootInterface->get_TH1D_index(m_prefix+"_"+"bx");
			if (index_temp!=-1) h1d_temp = fMyRootInterface->get_TH1D(index_temp);
			int ibin_z = h1d_temp->FindBin(z/mm);

			if (y==0&&x>-5*mm&&x<5*mm){
				index_temp = fMyRootInterface->get_TH1D_index(m_prefix+"_"+"bx");
				if (index_temp!=-1) fMyRootInterface->get_TH1D(index_temp)->SetBinContent(ibin_z,bx/tesla);
				index_temp = fMyRootInterface->get_TH1D_index(m_prefix+"_"+"by");
				if (index_temp!=-1) fMyRootInterface->get_TH1D(index_temp)->SetBinContent(ibin_z,by/tesla);
				index_temp = fMyRootInterface->get_TH1D_index(m_prefix+"_"+"bz");
				if (index_temp!=-1) fMyRootInterface->get_TH1D(index_temp)->SetBinContent(ibin_z,bz/tesla);
				index_temp = fMyRootInterface->get_TH1D_index(m_prefix+"_"+"bz");
			}
			if (z>6591&&z<6601&&y==0){
				index_temp = fMyRootInterface->get_TH1D_index(m_prefix+"_"+"MT1_by_x");
				if (index_temp!=-1) h1d_temp = fMyRootInterface->get_TH1D(index_temp);
				int ibin_x = h1d_temp->FindBin(x/mm);
				h1d_temp->SetBinContent(ibin_x,by/tesla);
			}
			if (z>6591&&z<6601&&x>-5*mm&&x<5*mm){
				index_temp = fMyRootInterface->get_TH1D_index(m_prefix+"_"+"MT1_by_y");
				if (index_temp!=-1) h1d_temp = fMyRootInterface->get_TH1D(index_temp);
				int ibin_y = h1d_temp->FindBin(y/mm);
				h1d_temp->SetBinContent(ibin_y,by/tesla);
			}

			std::string monitor="";
			if (z==0&&x<1500*mm){
				monitor="PTS";
			}
			else if (z==1570&&x<1500*mm){
				monitor="PCS1";
			}
			else if (z==2730&&x<1500*mm){
				monitor="PCS2";
			}
			else if (z==3750&&x<1500*mm){
				monitor="BP";
			}
			else if (z>6591&&z<6601){
				monitor="MT1";
			}
			else if (z>9697&&z<9707){
				monitor="BLT";
			}

			if (monitor!=""){
				index_temp = fMyRootInterface->get_TH2D_index(m_prefix+"_"+monitor+"_bx");
				if (index_temp!=-1) h2d_temp = fMyRootInterface->get_TH2D(index_temp);
				int ibin_x = h2d_temp->GetXaxis()->FindBin(x/mm);
				int ibin_y = h2d_temp->GetYaxis()->FindBin(y/mm);

				index_temp = fMyRootInterface->get_TH2D_index(m_prefix+"_"+monitor+"_bx");
				if (index_temp!=-1) fMyRootInterface->get_TH2D(index_temp)->SetBinContent(ibin_x,ibin_y,bx/tesla);
				index_temp = fMyRootInterface->get_TH2D_index(m_prefix+"_"+monitor+"_by");
				if (index_temp!=-1) fMyRootInterface->get_TH2D(index_temp)->SetBinContent(ibin_x,ibin_y,by/tesla);
				index_temp = fMyRootInterface->get_TH2D_index(m_prefix+"_"+monitor+"_bz");
				if (index_temp!=-1) fMyRootInterface->get_TH2D(index_temp)->SetBinContent(ibin_x,ibin_y,bz/tesla);
			}

			index_temp = fMyRootInterface->get_TH2D_index(m_prefix+"_"+"bx_zx");
			if (index_temp!=-1) h2d_temp = fMyRootInterface->get_TH2D(index_temp);
			int ibin_x = h2d_temp->GetYaxis()->FindBin(x/mm);
			ibin_z = h2d_temp->GetYaxis()->FindBin(z/mm);
			index_temp = fMyRootInterface->get_TH2D_index(m_prefix+"_"+"bx_zx");
			if (index_temp!=-1) fMyRootInterface->get_TH2D(index_temp)->SetBinContent(ibin_z,ibin_x,bx/tesla);
			index_temp = fMyRootInterface->get_TH2D_index(m_prefix+"_"+"by_zx");
			if (index_temp!=-1) fMyRootInterface->get_TH2D(index_temp)->SetBinContent(ibin_z,ibin_x,by/tesla);
			index_temp = fMyRootInterface->get_TH2D_index(m_prefix+"_"+"bz_zx");
			if (index_temp!=-1) fMyRootInterface->get_TH2D(index_temp)->SetBinContent(ibin_z,ibin_x,bz/tesla);

			index_temp = fMyRootInterface->get_TH2D_index(m_prefix+"_"+"bx_zy");
			if (index_temp!=-1) h2d_temp = fMyRootInterface->get_TH2D(index_temp);
			int ibin_y = h2d_temp->GetYaxis()->FindBin(y/mm)+1;
			ibin_z = h2d_temp->GetYaxis()->FindBin(z/mm)+1;
			index_temp = fMyRootInterface->get_TH2D_index(m_prefix+"_"+"bx_zy");
			if (index_temp!=-1) fMyRootInterface->get_TH2D(index_temp)->SetBinContent(ibin_z,ibin_y,bx/tesla);
			index_temp = fMyRootInterface->get_TH2D_index(m_prefix+"_"+"by_zy");
			if (index_temp!=-1) fMyRootInterface->get_TH2D(index_temp)->SetBinContent(ibin_z,ibin_y,by/tesla);
			index_temp = fMyRootInterface->get_TH2D_index(m_prefix+"_"+"bz_zy");
			if (index_temp!=-1) fMyRootInterface->get_TH2D(index_temp)->SetBinContent(ibin_z,ibin_y,bz/tesla);
		}
		else if (m_workMode == "sasaki"){
			fMyRootInterface->get_value("x",x,m);
			fMyRootInterface->get_value("y",y,m);
			fMyRootInterface->get_value("z",z,m);
			fMyRootInterface->get_value("bx",bx,tesla);
			fMyRootInterface->get_value("by",by,tesla);
			fMyRootInterface->get_value("bz",bz,-tesla);
			double r = x;
			double br = bx;

			if (y==0&&x==0){
				index_temp = fMyRootInterface->get_TH1D_index(m_prefix+"_"+"br");
				if (index_temp!=-1) h1d_temp = fMyRootInterface->get_TH1D(index_temp);
				int ibin_z = h1d_temp->FindBin(z/m);

				index_temp = fMyRootInterface->get_TH1D_index(m_prefix+"_"+"br");
				if (index_temp!=-1) fMyRootInterface->get_TH1D(index_temp)->SetBinContent(ibin_z,br/tesla);
				index_temp = fMyRootInterface->get_TH1D_index(m_prefix+"_"+"bz");
				if (index_temp!=-1) fMyRootInterface->get_TH1D(index_temp)->SetBinContent(ibin_z,bz/tesla);
			}

			std::string monitor="";
			if (fabs(z-10.7124*m)<1*cm){
				monitor="BESS";
			}
			else if (fabs(z-12.5024*m)<1*cm){
				monitor="DET";
			}

			if (monitor!=""){
				index_temp = fMyRootInterface->get_TH1D_index(m_prefix+"_"+monitor+"_br_r");
				if (index_temp!=-1) h1d_temp = fMyRootInterface->get_TH1D(index_temp);
				int ibin_r = h1d_temp->FindBin(r/m);
				h1d_temp->SetBinContent(ibin_r,br/tesla);
				index_temp = fMyRootInterface->get_TH1D_index(m_prefix+"_"+monitor+"_bz_r");
				if (index_temp!=-1) h1d_temp = fMyRootInterface->get_TH1D(index_temp);
				ibin_r = h1d_temp->FindBin(r/m);
				h1d_temp->SetBinContent(ibin_r,bz/tesla);
			}

			index_temp = fMyRootInterface->get_TH2D_index(m_prefix+"_br_zr");
			if (index_temp!=-1) h2d_temp = fMyRootInterface->get_TH2D(index_temp);
			int ibin_z = h2d_temp->GetXaxis()->FindBin(z/m);
			int ibin_r = h2d_temp->GetYaxis()->FindBin(r/m);

			index_temp = fMyRootInterface->get_TH2D_index(m_prefix+"_br_zr");
			if (index_temp!=-1) fMyRootInterface->get_TH2D(index_temp)->SetBinContent(ibin_z,ibin_r,br/tesla);
			index_temp = fMyRootInterface->get_TH2D_index(m_prefix+"_bz_zr");
			if (index_temp!=-1) fMyRootInterface->get_TH2D(index_temp)->SetBinContent(ibin_z,ibin_r,bz/tesla);
		}

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

	fMyRootInterface->dump();
	return 0;
}

void init_args()
{
	m_rootfile="";
	m_prefix="";
	m_suffix="";
	m_title="";
	m_workMode="UK";
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
