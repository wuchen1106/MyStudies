#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>
#include <vector>
#include <algorithm>

#include <cstdlib>

#include "TChain.h"
#include "TTree.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TGraph.h"
#include "Math/DistFunc.h"
#include "TVector3.h"

char m_workMode[128];
int verbose = 0;
int nEventsLimit = 0;

std::vector<TString> nameForH2D;
std::vector<TString> titleForH2D;
std::vector<TString> xNameForH2D;
std::vector<TString> yNameForH2D;
std::vector<int>     bin1ForH2D;
std::vector<double>  left1ForH2D;
std::vector<double>  right1ForH2D;
std::vector<int>     bin2ForH2D;
std::vector<double>  left2ForH2D;
std::vector<double>  right2ForH2D;
std::vector<TH2D*>   vecH2D;

std::vector<TString> nameForH1D;
std::vector<TString> titleForH1D;
std::vector<int> compareForH1D;
std::vector<int> xlogForH1D;
std::vector<int> ylogForH1D;
std::vector<int> colorForH1D;
std::vector<int> markerForH1D;
std::vector<TString> drawOptForH1D;
std::vector<TString> xNameForH1D;
std::vector<TString> yNameForH1D;
std::vector<int>     bin1ForH1D;
std::vector<double>  left1ForH1D;
std::vector<double>  right1ForH1D;
std::vector<TH1D*>   vecH1D;

std::vector<TString> nameForGraph;
std::vector<TString> titleForGraph;
std::vector<int> compareForGraph;
std::vector<int> xlogForGraph;
std::vector<int> ylogForGraph;
std::vector<int> colorForGraph;
std::vector<int> markerForGraph;
std::vector<TString> drawOptForGraph;
std::vector<TString> xNameForGraph;
std::vector<TString> yNameForGraph;
std::vector<std::vector<double> > xForGraph;
std::vector<std::vector<double> > yForGraph;

TH2D* get_TH2D(std::string name);
TH1D* get_TH1D(std::string name);
int get_TGraph(std::string name);
bool ISEMPTY(std::string content);
void seperate_string(std::string line, std::vector<std::string> &strs, const char sep );
double string2double(std::string str);
void init_args();
void print_usage(char* prog_name);

int main(int argc, char* argv[]){
	std::stringstream buff;

	//=======================================
	//*************read parameter**********
	//if (argc==1) {
	//	print_usage(argv[0]);
	//	return -1;
	//}
	init_args();
	int result;
	while((result=getopt(argc,argv,"hv:n:m:"))!=-1){
		switch(result){
			/* INPUTS */
			case 'm':
				strcpy(m_workMode,optarg);
				printf("work mode: %s\n",m_workMode);
				break;
			case 'v':
				verbose = atoi(optarg);
				printf("verbose level: %d\n",verbose);
				break;
			case 'n':
				nEventsLimit = atoi(optarg);
				printf("nEvent limit: %d\n",nEventsLimit);
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

	//for (;optind<argc;optind++){
	//	m_input_files.push_back(argv[optind]);
	//}

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

	//=======================================================================================================
	//************PRESET********************
	if (verbose >= Verbose_SectorInfo ) std::cout<<prefix_SectorInfo<<"In PRESET###"<<std::endl;

	//=> About Histogram
	std::string histList = "histList";
	TH1D* TH1D_temp;
	TH2D* TH2D_temp;
	int index_temp;

	//=>About Constant
	double PI = 3.141592653589793238;
	double FSC = 1/137.03599911; //fine structure constant
	double M_MUON = 105.6584; //mass of muon in MeV
	double M_ELE = 0.510999; //mass of electron in MeV
	double M_U = 931.494061; //atomic mass unit in MeV

	//=>About output
	std::string OutputDir = "result/";


	//=======================================================================================================
	//************SET HISTOGRAMS********************
	if (verbose >= Verbose_SectorInfo ) std::cout<<prefix_SectorInfo<<"In SET HISTOGRAMS###"<<std::endl;
 
	//=>Read histList
	std::ifstream fin_card(histList.c_str());
	if(!fin_card){
		std::cout<<"Cannot find "<<histList<<std::endl;
	}
	std::string s_card;
	std::string histtype, histname, histtitle, dirname, runname, histdrawOpt, histxName, histyName;
	double histleft1, histright1, histleft2, histright2;
	int histbin1, histbin2, histcolor, histcompare, histlogx, histlogy, nCPU, njob, histmarker;
	std::vector<std::string> DirNames; 
	std::vector<std::string> RunNames; 
	std::vector<int> NCPU;
	std::vector<int> NJob;
	// read histList
	while(getline(fin_card,s_card)){
		if ( ISEMPTY(s_card) ) continue;
		std::vector<std::string> segments;
		seperate_string(s_card,segments,'|');
		if ( segments[0] == "TH1D" ){
			nameForH1D.push_back(segments[1]);
			titleForH1D.push_back(segments[2]);
			xNameForH1D.push_back(segments[3]);
			yNameForH1D.push_back(segments[4]);
			bin1ForH1D.push_back(string2double(segments[5]));
			left1ForH1D.push_back(string2double(segments[6]));
			right1ForH1D.push_back(string2double(segments[7]));
			colorForH1D.push_back(string2double(segments[8]));
			compareForH1D.push_back(string2double(segments[9]));
			xlogForH1D.push_back(string2double(segments[10]));
			ylogForH1D.push_back(string2double(segments[11]));
			markerForH1D.push_back(string2double(segments[12]));
			drawOptForH1D.push_back(segments[13]);
		}
		else if ( segments[0] == "TH2D" ){
			nameForH2D.push_back(segments[1]);
			titleForH2D.push_back(segments[2]);
			xNameForH2D.push_back(segments[3]);
			yNameForH2D.push_back(segments[4]);
			bin1ForH2D.push_back(string2double(segments[5]));
			left1ForH2D.push_back(string2double(segments[6]));
			right1ForH2D.push_back(string2double(segments[7]));
			bin2ForH2D.push_back(string2double(segments[8]));
			left2ForH2D.push_back(string2double(segments[9]));
			right2ForH2D.push_back(string2double(segments[10]));
		}
		else if ( segments[0] == "FILE" ){
			DirNames.push_back(segments[1]);
			if (segments[2]==""){
				RunNames.push_back("");
			}
			else{
				RunNames.push_back("_"+segments[2]);
			}
			NCPU.push_back(string2double(segments[3]));
			NJob.push_back(string2double(segments[4]));
		}
		else if ( segments[0] == "TGraph" ){
			nameForGraph.push_back(segments[1]);
			titleForGraph.push_back(segments[2]);
			xNameForGraph.push_back(segments[3]);
			yNameForGraph.push_back(segments[4]);
			std::vector<double> avec;
			xForGraph.push_back(avec);
			std::vector<double> bvec;
			yForGraph.push_back(bvec);
			colorForGraph.push_back(string2double(segments[5]));
			compareForGraph.push_back(string2double(segments[6]));
			xlogForGraph.push_back(string2double(segments[7]));
			ylogForGraph.push_back(string2double(segments[8]));
			markerForGraph.push_back(string2double(segments[9]));
			drawOptForGraph.push_back(segments[10]);
			int i = nameForGraph.size() - 1;
			if (verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<"Input vecGraph["<<i<<"]: "<<nameForGraph[i]<<", "<<titleForGraph[i]<<", "<<xNameForGraph[i]<<", "<<yNameForGraph[i]<<", Color="<<colorForGraph[i]<<", xlogSyle="<<xlogForGraph[i]<<", ylogSyle="<<ylogForGraph[i]<<", nCompare="<<compareForGraph[i]<<", markerStyle="<<markerForGraph[i]<<", drawOpt=\""<<drawOptForGraph[i]<<"\""<<std::endl;
		}
		else{
			std::cout<<"Cannot recogonize this line: "<<s_card<<std::endl;
			continue;
		}
	}

	//=> Get histograms in
	for ( int i = 0; i < nameForH2D.size(); i++ ){
		vecH2D.push_back(new TH2D(nameForH2D[i],titleForH2D[i],bin1ForH2D[i],left1ForH2D[i],right1ForH2D[i],bin2ForH2D[i],left2ForH2D[i],right2ForH2D[i]) );
	}
	for ( int i = 0; i < nameForH1D.size(); i++ ){
		vecH1D.push_back(new TH1D(nameForH1D[i],titleForH1D[i],bin1ForH1D[i],left1ForH1D[i],right1ForH1D[i]) );
	}

	//=======================================================================================================
	//************SET Statistics********************
	if (verbose >= Verbose_SectorInfo ) std::cout<<prefix_SectorInfo<<"In SET Statistics###"<<std::endl;

	//=>About Statistical
	int N0 = 0;
	int N1 = 0;
	int N2 = 0;
	int N3 = 0;

	//=======================================================================================================
	//************DO THE DIRTY WORK*******************
	if (verbose >= Verbose_SectorInfo) std::cout<<prefix_SectorInfo<<"In DO THE DIRTY WORK ###"<<std::endl;
	//loop in events
	for( Long64_t iEvent = 0; iEvent < nEventsLimit; iEvent++ ){
		N0++;
		if (verbose >= Verbose_EventInfo ) std::cout<<prefix_EventInfoStart<<"In Event "<<iEvent<<std::endl;


		if (verbose >= Verbose_EventInfo ) std::cout<<prefix_EventInfo<<"Finished!"<<std::endl;
	}/* end of loop in events*/

	//=======================================================================================================

	//=======================================================================================================
	//************WRITE AND OUTPUT********************
	if (verbose >= Verbose_SectorInfo) std::cout<<prefix_SectorInfo<<"In WRITE AND OUTPUT ###"<<std::endl;
	std::string outputFileName = OutputDir + "output.root";
	TFile *file = new TFile(outputFileName.c_str(),"RECREATE");
	std::cout<<"N0 = "<<N0<<std::endl;
	std::cout<<"N1 = "<<N1<<std::endl;
	std::cout<<"N2 = "<<N2<<std::endl;
	std::cout<<"N3 = "<<N3<<std::endl;

	gStyle->SetPalette(1);
	gStyle->SetOptStat(0);
	gStyle->SetPadTickX(1);
	gStyle->SetPadTickY(1);
	//  gStyle->SetTitleW(0.99);
	//  gStyle->SetTitleH(0.08);
	//Output these histograms
	for ( int i = 0; i < vecH1D.size(); i++ ){
		if (verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<"Output vecH1D["<<i<<"]: "<<nameForH1D[i]<<", "<<titleForH1D[i]<<", "<<xNameForH1D[i]<<", "<<yNameForH1D[i]<<", "<<bin1ForH1D[i]<<", "<<left1ForH1D[i]<<", "<<right1ForH1D[i]<<", Color="<<colorForH1D[i]<<", xlogSyle="<<xlogForH1D[i]<<", ylogSyle="<<ylogForH1D[i]<<", nCompare="<<compareForH1D[i]<<", markerStyle="<<markerForH1D[i]<<", drawOpt=\""<<drawOptForH1D[i]<<"\""<<std::endl;
		vecH1D[i]->SetLineColor(colorForH1D[i]);
		TString name = vecH1D[i]->GetName();
		TCanvas* c = new TCanvas(name);
		int nCompare = compareForH1D[i];
		if ( nCompare ) if (verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<nCompare<<" histograms to be compared"<<std::endl;
		double currentMaximum = vecH1D[i]->GetMaximum();
		for ( int j = 1; j <= nCompare; j++ ){
			double maximum = vecH1D[i+j]->GetMaximum();
			if ( maximum > currentMaximum ){
				currentMaximum = maximum;
			}
		}
		double xmax = vecH1D[i]->GetXaxis()->GetXmax();
		if ( nCompare ) if (verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<"  maximum y value is ("<<currentMaximum<<")"<<std::endl;
		if ( xlogForH1D[i] ) gPad->SetLogx(1);
		else gPad->SetLogx(0);
		if ( ylogForH1D[i] ) gPad->SetLogy(1);
		else gPad->SetLogy(0);
		if ( xlogForH1D[i] ) vecH1D[i]->GetXaxis()->SetRangeUser(1e-6,2*xmax);
		else vecH1D[i]->GetXaxis()->SetRangeUser(0.,1.05*xmax);
		if ( ylogForH1D[i] ) vecH1D[i]->GetYaxis()->SetRangeUser(1e-6,2*currentMaximum);
		else vecH1D[i]->GetYaxis()->SetRangeUser(0.,1.05*currentMaximum);
		vecH1D[i]->SetMarkerStyle(markerForH1D[i]);
		vecH1D[i]->SetMarkerColor(colorForH1D[i]);
		vecH1D[i]->SetLineColor(colorForH1D[i]);
		vecH1D[i]->GetXaxis()->SetTitle(xNameForH1D[i]);
		vecH1D[i]->GetYaxis()->SetTitle(yNameForH1D[i]);
		vecH1D[i]->Draw(drawOptForH1D[i]);
		vecH1D[i]->Write();
		for ( int j = 0; j < nCompare; j++ ){
			i++;
			if (verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<" ->"<<j<<", vecH1D["<<i<<"]: "<<nameForH1D[i]<<", "<<titleForH1D[i]<<", "<<xNameForH1D[i]<<", "<<yNameForH1D[i]<<", "<<bin1ForH1D[i]<<", "<<left1ForH1D[i]<<", "<<right1ForH1D[i]<<", Color="<<colorForH1D[i]<<", xlogSyle="<<xlogForH1D[i]<<", ylogSyle="<<ylogForH1D[i]<<", nCompare="<<compareForH1D[i]<<", markerStyle="<<markerForH1D[i]<<", drawOpt=\""<<drawOptForH1D[i]<<"\""<<std::endl;
			vecH1D[i]->SetLineColor(colorForH1D[i]);
			vecH1D[i]->SetMarkerStyle(markerForH1D[i]);
			vecH1D[i]->SetMarkerColor(colorForH1D[i]);
			vecH1D[i]->SetLineColor(colorForH1D[i]);
			TString drawOpt = drawOptForH1D[i]+"SAME";
			vecH1D[i]->Draw(drawOpt);
		}
		TString fileName = OutputDir + name + ".pdf";
		c->Print(fileName);
	}
	gStyle->SetOptStat(0);
	for ( int i = 0; i < vecH2D.size(); i++ ){
		if (verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<"Output vecH2D["<<i<<"]: "<<nameForH2D[i]<<", "<<titleForH2D[i]<<", "<<xNameForH2D[i]<<", "<<yNameForH2D[i]<<", "<<bin1ForH2D[i]<<", "<<left1ForH2D[i]<<", "<<right1ForH2D[i]<<", "<<bin2ForH2D[i]<<", "<<left2ForH2D[i]<<", "<<right2ForH2D[i]<<std::endl;
		TString name = vecH2D[i]->GetName();
		TCanvas* c = new TCanvas(name);
		vecH2D[i]->GetXaxis()->SetTitle(xNameForH2D[i]);
		vecH2D[i]->GetYaxis()->SetTitle(yNameForH2D[i]);
		vecH2D[i]->Draw("COLZ");
		vecH2D[i]->Write();
		TString fileName = OutputDir + name + ".pdf";
		c->Print(fileName);
	}
	for ( int i = 0; i < nameForGraph.size(); i++ ){
		int sizeOfThisGraph = xForGraph[i].size();
		if (verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<"Output vecGraph["<<i<<"]: "<<nameForGraph[i]<<", "<<titleForGraph[i]<<", "<<xNameForGraph[i]<<", "<<yNameForGraph[i]<<", Color="<<colorForGraph[i]<<", xlogSyle="<<xlogForGraph[i]<<", ylogSyle="<<ylogForGraph[i]<<", nCompare="<<compareForGraph[i]<<", markerStyle="<<markerForGraph[i]<<", drawOpt=\""<<drawOptForGraph[i]<<"\", size = "<<sizeOfThisGraph<<std::endl;
		if ( sizeOfThisGraph <= 0 ) continue;
		if (verbose >= Verbose_HistInfo){
			for ( int j = 0; j < sizeOfThisGraph; j++ ){
				std::cout<<prefix_HistInfo<<"  ["<<j<<"]: ("<<xForGraph[i][j]<<","<<yForGraph[i][j]<<")"<<std::endl;
			}
		}
		TString name = nameForGraph[i];
		TCanvas* c = new TCanvas(nameForGraph[i]);
		TGraph *aTGraph = new TGraph(sizeOfThisGraph,&xForGraph[i][0],&yForGraph[i][0]);
		aTGraph->SetTitle(titleForGraph[i]);
		int nCompare = compareForGraph[i];
		if ( nCompare ) if (verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<nCompare<<" graphs to be compared"<<std::endl;
		std::vector<double> yforgraph = yForGraph[i];
		std::vector<double> xforgraph = xForGraph[i];
		double xmin = min_eff;
		double xmax = max_eff;
		double currentMaximum = *std::max_element(yforgraph.begin(),yforgraph.end());
		for ( int j = 1; j <= nCompare; j++ ){
			double maximum = *std::max_element(yForGraph[i+j].begin(),yForGraph[i+j].end());
			if ( maximum > currentMaximum ){
				currentMaximum = maximum;
			}
		}
		if ( nCompare ) if (verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<"  maximum y value is ("<<currentMaximum<<")"<<std::endl;
		if ( xlogForGraph[i] ) gPad->SetLogx(1);
		else gPad->SetLogx(0);
		if ( ylogForGraph[i] ) gPad->SetLogy(1);
		else gPad->SetLogy(0);
		if ( xlogForGraph[i] ) aTGraph->GetXaxis()->SetRangeUser(1e-6,2*xmax);
		else aTGraph->GetXaxis()->SetRangeUser(0.95*xmin,1.05*xmax);
		if ( ylogForGraph[i] ) aTGraph->GetYaxis()->SetRangeUser(1e-6,2*currentMaximum);
		else aTGraph->GetYaxis()->SetRangeUser(0.,1.05*currentMaximum);
		aTGraph->GetXaxis()->SetTitle(xNameForGraph[i]);
		aTGraph->GetYaxis()->SetTitle(yNameForGraph[i]);
		aTGraph->SetMarkerStyle(markerForGraph[i]);
		aTGraph->SetMarkerColor(colorForGraph[i]);
		aTGraph->SetLineColor(colorForGraph[i]);
		TString drawOpt = "A"+drawOptForGraph[i];
		aTGraph->Draw(drawOpt);
		aTGraph->Write();
		for ( int j = 0; j < nCompare; j++ ){
			i++;
			int sizeOfThisGraph = xForGraph[i].size();
			if (verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<" ->"<<j<<", vecGraph["<<i<<"]: "<<nameForGraph[i]<<", "<<titleForGraph[i]<<", "<<xNameForGraph[i]<<", "<<yNameForGraph[i]<<", Color="<<colorForGraph[i]<<", xlogSyle="<<xlogForGraph[i]<<", ylogSyle="<<ylogForGraph[i]<<", nCompare="<<compareForGraph[i]<<", markerStyle="<<markerForGraph[i]<<", drawOpt=\""<<drawOptForGraph[i]<<"\", size = "<<sizeOfThisGraph<<std::endl;
			if ( sizeOfThisGraph <= 0 ) continue;
			if (verbose >= Verbose_HistInfo){
				for ( int k = 0; k < sizeOfThisGraph; k++ ){
					std::cout<<prefix_HistInfo<<"  ["<<k<<"]: ("<<xForGraph[i][k]<<","<<yForGraph[i][k]<<")"<<std::endl;
				}
			}
			TGraph *bTGraph = new TGraph(sizeOfThisGraph,&xForGraph[i][0],&yForGraph[i][0]);
			bTGraph->SetTitle(titleForGraph[i]);
			bTGraph->GetXaxis()->SetTitle(xNameForGraph[i]);
			bTGraph->GetYaxis()->SetTitle(yNameForGraph[i]);
			bTGraph->SetLineColor(colorForGraph[i]);
			bTGraph->SetMarkerStyle(markerForGraph[i]);
			bTGraph->SetMarkerColor(colorForGraph[i]);
			bTGraph->SetLineColor(colorForGraph[i]);
			bTGraph->Draw(drawOptForGraph[i]);
			bTGraph->Write();
		}
		TString fileName = OutputDir + name + ".pdf";
		c->Print(fileName);
	}

	//TTree *m_TTree = m_TChain->CloneTree();
	//m_TTree->Write();
	//file->Write();
	file->Close();

	delete file;
	return 0;
}

TH2D* get_TH2D(std::string name){
	for ( int i = 0; i < vecH2D.size(); i++ ){
		if ( nameForH2D[i] == name ) return vecH2D[i];
	}
	std::cout<<"###!!!In get_TH2D: CAN NOT FIND "<<name<<"!!!"<<std::endl;
	return 0;
}

TH1D* get_TH1D(std::string name){
	for ( int i = 0; i < vecH1D.size(); i++ ){
		if ( nameForH1D[i] == name ) return vecH1D[i];
	}
	std::cout<<"###!!!In get_TH1D: CAN NOT FIND "<<name<<"!!!"<<std::endl;
	return 0;
}

int get_TGraph(std::string name){
	for ( int i = 0; i < nameForGraph.size(); i++ ){
		if ( nameForGraph[i] == name ) return i;
	}
	std::cout<<"###!!!In get_TGraph: CAN NOT FIND "<<name<<"!!!"<<std::endl;
	return -1;
}

bool ISEMPTY(std::string content){
	bool flag = false;
	const char* c_card = content.c_str();
	int length = strlen(c_card);
	int offset = 0;
	for ( ; offset < length; offset++ ){
		if ( c_card[offset] != ' ' ) break;
	}
	if ( c_card[offset] == '#' || (c_card[offset] == '/' && c_card[offset+1] == '/') || length - offset == 0 ){
		flag = true;
	}
	return flag;
}

void seperate_string(std::string line, std::vector<std::string> &strs, const char sep ){
	std::string token;
	std::stringstream ss(line);
	while(std::getline(ss, token, sep)){
		token.erase(token.find_last_not_of(' ')+1);
		token.erase(0,token.find_first_not_of(' '));
		strs.push_back(token);
	}
}

double string2double(std::string str){
	double val;
	std::stringstream ss(str);
	ss>>val;
	return val;
}

void init_args()
{
	strcpy(m_workMode,"pr");
	verbose = 0;
	nEventsLimit = 0;
}

void print_usage(char* prog_name)
{
	fprintf(stderr,"Usage %s [options (args)] [input files]\n",prog_name);
	fprintf(stderr,"[options]\n");
	fprintf(stderr,"\t -m\n");
	fprintf(stderr,"\t\t choose work mode: [pr(default), ab]\n");
	fprintf(stderr,"\t -v\n");
	fprintf(stderr,"\t\t verbose level\n");
	fprintf(stderr,"\t -n\n");
	fprintf(stderr,"\t\t nEvent limit\n");
	fprintf(stderr,"\t -h\n");
	fprintf(stderr,"\t\t Usage message.\n");
	fprintf(stderr,"[example]\n");
	fprintf(stderr,"\t\t%s -m ab -v 20 -n 100\n",prog_name);
}
