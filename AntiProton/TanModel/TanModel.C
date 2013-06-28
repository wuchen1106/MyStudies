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
#include "TGenPhaseSpace.h"

#include "globals.hh"
#include "Randomize.hh"

char m_workMode[128];
int verbose = 0;
int nEvents = 0;
int printModule = 1;

std::vector<std::string> oFileName;

std::vector<std::string> refFileName;
std::vector<std::string> refHistName;

std::vector<std::string> nameForH2D;
std::vector<std::string> titleForH2D;
std::vector<std::string> xNameForH2D;
std::vector<std::string> yNameForH2D;
std::vector<int>     bin1ForH2D;
std::vector<double>  left1ForH2D;
std::vector<double>  right1ForH2D;
std::vector<int>     bin2ForH2D;
std::vector<double>  left2ForH2D;
std::vector<double>  right2ForH2D;
std::vector<TH2D*>   vecH2D;

std::vector<std::string> nameForH1D;
std::vector<std::string> titleForH1D;
std::vector<int> compareForH1D;
std::vector<double> minxForH1D;
std::vector<double> minyForH1D;
std::vector<int> xlogForH1D;
std::vector<int> ylogForH1D;
std::vector<int> colorForH1D;
std::vector<int> markerForH1D;
std::vector<double> normForH1D;
std::vector<std::string> drawOptForH1D;
std::vector<std::string> xNameForH1D;
std::vector<std::string> yNameForH1D;
std::vector<int>     bin1ForH1D;
std::vector<double>  left1ForH1D;
std::vector<double>  right1ForH1D;
std::vector<TH1D*>   vecH1D;

std::vector<std::string> nameForGraph;
std::vector<std::string> titleForGraph;
std::vector<int> compareForGraph;
std::vector<double> minxForGraph;
std::vector<double> maxxForGraph;
std::vector<double> minyForGraph;
std::vector<int> xlogForGraph;
std::vector<int> ylogForGraph;
std::vector<int> colorForGraph;
std::vector<int> markerForGraph;
std::vector<std::string> drawOptForGraph;
std::vector<std::string> xNameForGraph;
std::vector<std::string> yNameForGraph;
std::vector<std::vector<double> > xForGraph;
std::vector<std::vector<double> > yForGraph;

int get_TH2D(std::string name);
int get_TH1D(std::string name);
int get_TGraph(std::string name);
bool ISEMPTY(std::string content);
void seperate_string(std::string line, std::vector<std::string> &strs, const char sep );
double string2double(std::string str);
void init_args();
void print_usage(char* prog_name);

int main(int argc, char* argv[]){
	std::stringstream buff;
	//=======================================
	//*************About Models**********
	double MX = 2.814;
	double Mp = 0.93957;
	double sigmga00 = 3.15;
	double a1 = 1.05e-4;
	double a2 = 10.1;
	double a3 = 0.5;
	double a4 = 7.9;
	double a5 = 0/465;
	double a6 = 3.7e-2;
	double a7 = 2.31;
	double a8 = 1.4e-2;
	double a9 = 3.02e-2;
	double a10 = 3.19;
	double a11 = 0.399;
	double a12 = 8.39;
	double epsilon = 0.23;

	//=======================================
	//*************read parameter**********
	//if (argc==1) {
	//	print_usage(argv[0]);
	//	return -1;
	//}
	init_args();
	int result;
	while((result=getopt(argc,argv,"hv:n:m:p:"))!=-1){
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
	int index_temp;

	//=>About Constant
	double PI = 3.141592653589793238;
	double FSC = 1/137.03599911; //fine structure constant
	double M_MUON = 0.1056584; //mass of muon in GeV
	double M_ELE = 0.510999e-3; //mass of electron in GeV
	double M_U = 0.931494061; //atomic mass unit in GeV
	double M_p = 0.9382723; // proton mass unit in GeV

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
	std::vector<std::string> DirNames; 
	std::vector<std::string> RunNames; 
	std::vector<int> NCPU;
	std::vector<int> NJob;
	double beamPx = 0;
	double beamPy = 0;
	double beamPz = 0;
	// read histList
	while(getline(fin_card,s_card)){
		if ( ISEMPTY(s_card) ) continue;
		std::vector<std::string> segments;
		seperate_string(s_card,segments,'|');
		int iterator = 1;
		if ( segments[0] == "TH1D" ){
			if(iterator<segments.size()) nameForH1D.push_back(segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) titleForH1D.push_back(segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) xNameForH1D.push_back(segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) yNameForH1D.push_back(segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) bin1ForH1D.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) left1ForH1D.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) right1ForH1D.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) minxForH1D.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) minyForH1D.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) colorForH1D.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) compareForH1D.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) xlogForH1D.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) ylogForH1D.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) markerForH1D.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) normForH1D.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) drawOptForH1D.push_back(segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
		}
		else if ( segments[0] == "TH2D" ){
			if(iterator<segments.size()) nameForH2D.push_back(segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) titleForH2D.push_back(segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) xNameForH2D.push_back(segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) yNameForH2D.push_back(segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) bin1ForH2D.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) left1ForH2D.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) right1ForH2D.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) bin2ForH2D.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) left2ForH2D.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) right2ForH2D.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
		}
		else if ( segments[0] == "FILE" ){
			if(iterator<segments.size()) DirNames.push_back(segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			std::string runname;
			if(iterator<segments.size()) runname = "_"+segments[iterator++]; else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if (runname=="_"){
				RunNames.push_back("");
			}
			else{
				RunNames.push_back(runname);
			}
			if(iterator<segments.size()) NCPU.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) NJob.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
		}
		else if ( segments[0] == "TGraph" ){
			if(iterator<segments.size()) nameForGraph.push_back(segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) titleForGraph.push_back(segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) xNameForGraph.push_back(segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) yNameForGraph.push_back(segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			std::vector<double> avec;
			xForGraph.push_back(avec);
			std::vector<double> bvec;
			yForGraph.push_back(bvec);
			if(iterator<segments.size()) colorForGraph.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) compareForGraph.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) minxForGraph.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) maxxForGraph.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) minyForGraph.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) xlogForGraph.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) ylogForGraph.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) markerForGraph.push_back(string2double(segments[iterator++])); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) drawOptForGraph.push_back(segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			int i = nameForGraph.size() - 1;
			if (verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<"Input vecGraph["<<i<<"]: "<<nameForGraph[i]<<", "<<titleForGraph[i]<<", "<<xNameForGraph[i]<<", "<<yNameForGraph[i]<<", Color="<<colorForGraph[i]<<", xlogSyle="<<xlogForGraph[i]<<", ylogSyle="<<ylogForGraph[i]<<", nCompare="<<compareForGraph[i]<<", markerStyle="<<markerForGraph[i]<<", drawOpt=\""<<drawOptForGraph[i]<<"\""<<std::endl;
		}
		else if (segments[0] == "refTH1D"){
			if(iterator<segments.size()) refFileName.push_back(segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) refHistName.push_back(segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
		}
		else if (segments[0] == "oFILE"){
			if(iterator<segments.size()) oFileName.push_back(segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
		}
		else if (segments[0] == "BEAM"){
			if(iterator<segments.size()) beamPx = string2double(segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) beamPy = string2double(segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
			if(iterator<segments.size()) beamPz = string2double(segments[iterator++]); else {std::cout<<"Not enough segments in"<<s_card<<"!!!"<<std::endl; return -1;}
		}
		else{
			std::cout<<"Cannot recogonize this line: "<<s_card<<std::endl;
			continue;
		}
	}

	//=> Get histograms in
	for ( int i = 0; i < nameForH2D.size(); i++ ){
		vecH2D.push_back(new TH2D(nameForH2D[i].c_str(),titleForH2D[i].c_str(),bin1ForH2D[i],left1ForH2D[i],right1ForH2D[i],bin2ForH2D[i],left2ForH2D[i],right2ForH2D[i]) );
	}
	for ( int i = 0; i < nameForH1D.size(); i++ ){
		vecH1D.push_back(new TH1D(nameForH1D[i].c_str(),titleForH1D[i].c_str(),bin1ForH1D[i],left1ForH1D[i],right1ForH1D[i]) );
	}
	for ( int i = 0; i < refFileName.size(); i++ ){
		TFile * fp_ref = new TFile(refFileName[i].c_str());
		if (fp_ref==NULL) {
			std::cout<<"ERROR: Can not find file: "<<refFileName[i]<<"!!!"<<std::endl;
			return -1;
		}
		TH1D* h1_ref = (TH1D*)fp_ref->Get(refHistName[i].c_str());
		if(h1_ref==NULL){
			std::cout<<"ERROR: Can not find histogram \""<<refHistName[i]<<"\"in file : "<<refFileName[i]<<"!!!"<<std::endl;
			return -1;
		}
		if ( (index_temp = get_TH1D(refHistName[i])) != -1 ){
			h1_ref->SetTitle(titleForH1D[index_temp].c_str());
			vecH1D[index_temp]=h1_ref;
		}
		else{
			std::cout<<"ERROR: Can not find histogram \""<<refHistName[i]<<"\"in vecH1D!!!"<<std::endl;
			return -1;
		}
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
	//************Get Fermi Momentum Distribution histogram********************
	std::string FN_FM = "../FermiMomentum/result/output.root";
	std::string HN_FM = "h";
	TFile* fp_FM = new TFile(FN_FM.c_str());
	if (fp_FM==NULL) {
		std::cout<<"ERROR: Can not find file: "<<FN_FM<<"!!!"<<std::endl;
		return -1;
	}
	TH1D* h_FM = (TH1D*)fp_FM->Get(HN_FM.c_str());
	if(h_FM==NULL){
		std::cout<<"ERROR: Can not find histogram \""<<HN_FM<<"\"in file : "<<FN_FM<<"!!!"<<std::endl;
		return -1;
	}

	//=======================================================================================================
	//************DO THE DIRTY WORK*******************
	if (verbose >= Verbose_SectorInfo) std::cout<<prefix_SectorInfo<<"In DO THE DIRTY WORK ###"<<std::endl;

	if (!strcmp(m_workMode,"com")){
	}
	else if (!strcmp(m_workMode,"gen")){
		TLorentzVector beam(0.,0.,0.,0.);
		beam.SetVectM(TVector3(beamPx,beamPy,beamPz), M_p);
		//(Momentum, Energy units are Gev/C, GeV)
		Double_t masses[4] = { M_p, M_p, M_p, M_p} ;
		TGenPhaseSpace event;
		//loop in events
		for( Long64_t iEvent = 0; iEvent < nEvents; iEvent++ ){
			if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"In Event "<<iEvent<<std::endl;
			N0++;
			// Generate a nucleon
			double pFermi = h_FM->GetRandom();
			G4double dir_x = 0., dir_y = 0., dir_z = 0.;
			G4bool dir_OK = false;
			while( !dir_OK ){
				dir_x=G4UniformRand()-0.5;
				dir_y=G4UniformRand()-0.5;
				dir_z=G4UniformRand()-0.5;
				if ( dir_x*dir_x + dir_y*dir_y + dir_z*dir_z <= 0.025 && dir_x*dir_x + dir_y*dir_y + dir_z*dir_z != 0) dir_OK = true;
			}
			TVector3 dir_3Vec(dir_x, dir_y, dir_z);
			dir_3Vec.SetMag(pFermi);

			// Generate the collision
			TLorentzVector target(0.,0.,0.,0.);
			target.SetVectM(dir_3Vec, 0.9382723);
			TLorentzVector W = beam + target;
			event.SetDecay(W, 4, masses);
			Double_t weight = event.Generate();
			TLorentzVector *pDaughter1 = event.GetDecay(0);
			TLorentzVector *pDaughter2 = event.GetDecay(1);
			TLorentzVector *pDaughter3 = event.GetDecay(2);
			TLorentzVector *pDaughter4 = event.GetDecay(3);
			TLorentzVector X = *pDaughter2 + *pDaughter3 + *pDaughter4;
			TLorentzVector pAllDaughters = *pDaughter1 + *pDaughter2 + *pDaughter3 + *pDaughter4;

			// Calculate weight given by Tan Model
			double s = W.M2();
			double Emax = (s - MX*MX + Mp*Mp)/2*sqrt(s);
			double xR = pDaughter1->E()/Emax;
			double pt = pDaughter1->Pt();
			double fr = 0;
			if ( a3 < xR ){
				fr = (sigmga00-a1)*pow(1-xR,a4);
			}
			else{
				fr = a1*exp(-a2*xR)+(sigmga00-a1)*pow(1-xR,a4);
			}
			double A = a5*exp(-a6*xR)+a7*exp(a8*xR);
			double B = a9*exp(-a10*(xR+a11))*pow(xR+a11,a12);
			double weight2 = fr*exp(-A*pt+B*pt*pt);

			// Get the total weight
			weight *=  weight2;
			weight /= (double) nEvents;

			if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"  weight = "<<weight
																										 <<", p("<<pAllDaughters.Px()
																										 <<", "<<pAllDaughters.Py()
																										 <<", "<<pAllDaughters.Pz()
																										 <<"), E="<<pAllDaughters.E()
																										 <<", p1("<<pDaughter1->Px()
																										 <<", "<<pDaughter1->Py()
																										 <<", "<<pDaughter1->Pz()
																										 <<"), E1="<<pDaughter1->E()
																										 <<", p2("<<pDaughter2->Px()
																										 <<", "<<pDaughter2->Py()
																										 <<", "<<pDaughter2->Pz()
																										 <<"), E2="<<pDaughter2->E()
																										 <<", p3("<<pDaughter3->Px()
																										 <<", "<<pDaughter3->Py()
																										 <<", "<<pDaughter3->Pz()
																										 <<"), E3="<<pDaughter3->E()
																										 <<", p4("<<pDaughter4->Px()
																										 <<", "<<pDaughter4->Py()
																										 <<", "<<pDaughter4->Pz()
																										 <<"), E4="<<pDaughter4->E()
																										 <<std::endl;
			if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"  CMS: p=("<<W.Px()
																										 <<", "<<W.Py()
																										 <<", "<<W.Pz()
																										 <<"), v=("<<W.BoostVector().X()
																										 <<", "<<W.BoostVector().Y()
																										 <<", "<<W.BoostVector().Z()
																										 <<") E="<<W.E()
																										 <<std::endl;

			if ( (index_temp = get_TH1D("Tan_Mx")) != -1 ){
				vecH1D[index_temp]->Fill(X.M(),weight);
			}
			if ( (index_temp = get_TH1D("Tan_Mx_log")) != -1 ){
				vecH1D[index_temp]->Fill(X.M(),weight);
			}
			if ( (index_temp = get_TH1D("Tan_pa")) != -1 ){
				vecH1D[index_temp]->Fill(pDaughter1->Rho(),weight);
			}
			if ( (index_temp = get_TH1D("Tan_pa_log")) != -1 ){
				vecH1D[index_temp]->Fill(pDaughter1->Rho(),weight);
			}
			if ( (index_temp = get_TH1D("Tan_pt")) != -1 ){
				vecH1D[index_temp]->Fill(pDaughter1->Pt(),weight);
			}
			if ( (index_temp = get_TH1D("Tan_pt_log")) != -1 ){
				vecH1D[index_temp]->Fill(pDaughter1->Pt(),weight);
			}
			if ( (index_temp = get_TH1D("Tan_pz")) != -1 ){
				vecH1D[index_temp]->Fill(pDaughter1->Pz(),weight);
			}
			if ( (index_temp = get_TH1D("Tan_pz_log")) != -1 ){
				vecH1D[index_temp]->Fill(pDaughter1->Pz(),weight);
			}
			if ( (index_temp = get_TH1D("Tan_vz")) != -1 ){
				vecH1D[index_temp]->Fill(pDaughter1->BoostVector().Z(),weight);
			}
			if ( (index_temp = get_TH1D("Tan_vz_log")) != -1 ){
				vecH1D[index_temp]->Fill(pDaughter1->BoostVector().Z(),weight);
			}
			if ( (index_temp = get_TH1D("Tan_theta")) != -1 ){
				vecH1D[index_temp]->Fill(pDaughter1->Theta(),weight);
			}
			if ( (index_temp = get_TH1D("Tan_theta_log")) != -1 ){
				vecH1D[index_temp]->Fill(pDaughter1->Theta(),weight);
			}
			pDaughter1->Boost(-W.BoostVector());
			pDaughter2->Boost(-W.BoostVector());
			pDaughter3->Boost(-W.BoostVector());
			pDaughter4->Boost(-W.BoostVector());
			pAllDaughters = *pDaughter1 + *pDaughter2 + *pDaughter3 + *pDaughter4;
			if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"  in CMS "
																										 <<", p("<<pAllDaughters.Px()
																										 <<", "<<pAllDaughters.Py()
																										 <<", "<<pAllDaughters.Pz()
																										 <<"), E="<<pAllDaughters.E()
																										 <<", p1("<<pDaughter1->Px()
																										 <<", "<<pDaughter1->Py()
																										 <<", "<<pDaughter1->Pz()
																										 <<"), E1="<<pDaughter1->E()
																										 <<", p2("<<pDaughter2->Px()
																										 <<", "<<pDaughter2->Py()
																										 <<", "<<pDaughter2->Pz()
																										 <<"), E2="<<pDaughter2->E()
																										 <<", p3("<<pDaughter3->Px()
																										 <<", "<<pDaughter3->Py()
																										 <<", "<<pDaughter3->Pz()
																										 <<"), E3="<<pDaughter3->E()
																										 <<", p4("<<pDaughter4->Px()
																										 <<", "<<pDaughter4->Py()
																										 <<", "<<pDaughter4->Pz()
																										 <<"), E4="<<pDaughter4->E()
																										 <<std::endl;
			if ( (index_temp = get_TH1D("Tan_costhetaCMS")) != -1 ){
				vecH1D[index_temp]->Fill(cos(pDaughter1->Theta()),weight);
			}
			if ( (index_temp = get_TH1D("Tan_paCMS")) != -1 ){
				vecH1D[index_temp]->Fill(pDaughter1->Rho(),weight);
			}
			if ( (index_temp = get_TH1D("Tan_paCMS_log")) != -1 ){
				vecH1D[index_temp]->Fill(pDaughter1->Rho(),weight);
			}
			if ( (index_temp = get_TH1D("Tan_ptCMS")) != -1 ){
				vecH1D[index_temp]->Fill(pDaughter1->Pt(),weight);
			}
			if ( (index_temp = get_TH1D("Tan_ptCMS_log")) != -1 ){
				vecH1D[index_temp]->Fill(pDaughter1->Pt(),weight);
			}
			if ( (index_temp = get_TH1D("Tan_ECMS")) != -1 ){
				vecH1D[index_temp]->Fill(pDaughter1->E(),weight);
			}
			if ( (index_temp = get_TH1D("Tan_ECMS_log")) != -1 ){
				vecH1D[index_temp]->Fill(pDaughter1->E(),weight);
			}

			if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfo<<"Finished!"<<std::endl;
		}/* end of loop in events*/
	}

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
		if (verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<"Output vecH1D["<<i<<"]: "<<nameForH1D[i]<<", "<<titleForH1D[i]<<", "<<xNameForH1D[i]<<", "<<yNameForH1D[i]<<", "<<bin1ForH1D[i]<<", "<<left1ForH1D[i]<<", "<<right1ForH1D[i]<<", Color="<<colorForH1D[i]<<", xlogSyle="<<xlogForH1D[i]<<", ylogSyle="<<ylogForH1D[i]<<", nCompare="<<compareForH1D[i]<<", markerStyle="<<markerForH1D[i]<<", normalize ="<<normForH1D[i]<<", drawOpt=\""<<drawOptForH1D[i]<<"\""<<std::endl;
		vecH1D[i]->SetLineColor(colorForH1D[i]);
		std::string name = vecH1D[i]->GetName();
		TCanvas* c = new TCanvas(name.c_str());
		int nCompare = compareForH1D[i];
		if ( nCompare ) if (verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<nCompare<<" histograms to be compared"<<std::endl;
		if (normForH1D[i]){
			if (normForH1D[i] == 1) vecH1D[i]->Scale(1./vecH1D[i]->Integral());
			else vecH1D[i]->Scale(1./normForH1D[i]);
		}
		double currentMaximum = vecH1D[i]->GetMaximum();
		if (verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<"    currentMaximum y value is ("<<currentMaximum<<")"<<std::endl;
		for ( int j = 1; j <= nCompare; j++ ){
			if (normForH1D[i+j]){
				if (normForH1D[i+j] == 1) vecH1D[i+j]->Scale(1./vecH1D[i+j]->Integral());
				else vecH1D[i+j]->Scale(1./normForH1D[i+j]);
			}
			double maximum = vecH1D[i+j]->GetMaximum();
			if (verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<"    Maximum y for "<<nameForH1D[i+j]<<" is ("<<maximum<<")"<<std::endl;
			if ( maximum > currentMaximum ){
				currentMaximum = maximum;
			}
		}
		if (verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<"  maximum y value is ("<<currentMaximum<<")"<<std::endl;
		if ( xlogForH1D[i] ) gPad->SetLogx(1);
		else gPad->SetLogx(0);
		if ( ylogForH1D[i] ) gPad->SetLogy(1);
		else gPad->SetLogy(0);
		if ( xlogForH1D[i] ){
			vecH1D[i]->GetXaxis()->SetRangeUser(minxForH1D[i],right1ForH1D[i]);
			if (verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<"  Logx! set xRange("<<minxForH1D[i]<<","<<right1ForH1D[i]<<")"<<std::endl;
		}
		else {
			vecH1D[i]->GetXaxis()->SetRangeUser(left1ForH1D[i],right1ForH1D[i]);
			if (verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<"        set xRange("<<left1ForH1D[i]<<","<<right1ForH1D[i]<<")"<<std::endl;
		}
		if ( ylogForH1D[i] ) {
			vecH1D[i]->GetYaxis()->SetRangeUser(minyForH1D[i],2*currentMaximum);
			if (verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<"  Logy! set yRange("<<minyForH1D[i]<<","<<2*currentMaximum<<")"<<std::endl;
		}
		else {
			vecH1D[i]->GetYaxis()->SetRangeUser(minyForH1D[i],1.05*currentMaximum);
			if (verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<"        set yRange("<<minyForH1D[i]<<","<<1.05*currentMaximum<<")"<<std::endl;
		}
		vecH1D[i]->SetMarkerStyle(markerForH1D[i]);
		vecH1D[i]->SetMarkerColor(colorForH1D[i]);
		vecH1D[i]->SetLineColor(colorForH1D[i]);
		vecH1D[i]->GetXaxis()->SetTitle(xNameForH1D[i].c_str());
		vecH1D[i]->GetYaxis()->SetTitle(yNameForH1D[i].c_str());
		vecH1D[i]->Draw(drawOptForH1D[i].c_str());
		vecH1D[i]->Write();
		for ( int j = 0; j < nCompare; j++ ){
			i++;
			if (verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<" ->"<<j<<", vecH1D["<<i<<"]: "<<nameForH1D[i]<<", "<<titleForH1D[i]<<", "<<xNameForH1D[i]<<", "<<yNameForH1D[i]<<", "<<bin1ForH1D[i]<<", "<<left1ForH1D[i]<<", "<<right1ForH1D[i]<<", Color="<<colorForH1D[i]<<", xlogSyle="<<xlogForH1D[i]<<", ylogSyle="<<ylogForH1D[i]<<", nCompare="<<compareForH1D[i]<<", markerStyle="<<markerForH1D[i]<<", normalize ="<<normForH1D[i]<<", drawOpt=\""<<drawOptForH1D[i]<<"\""<<std::endl;
			vecH1D[i]->SetLineColor(colorForH1D[i]);
			vecH1D[i]->SetMarkerStyle(markerForH1D[i]);
			vecH1D[i]->SetMarkerColor(colorForH1D[i]);
			vecH1D[i]->SetLineColor(colorForH1D[i]);
			std::string drawOpt = drawOptForH1D[i]+"SAME";
			vecH1D[i]->Draw(drawOpt.c_str());
		}
		std::string fileName = OutputDir + name + ".pdf";
		c->Print(fileName.c_str());
	}
	gStyle->SetOptStat(0);
	for ( int i = 0; i < vecH2D.size(); i++ ){
		if (verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<"Output vecH2D["<<i<<"]: "<<nameForH2D[i]<<", "<<titleForH2D[i]<<", "<<xNameForH2D[i]<<", "<<yNameForH2D[i]<<", "<<bin1ForH2D[i]<<", "<<left1ForH2D[i]<<", "<<right1ForH2D[i]<<", "<<bin2ForH2D[i]<<", "<<left2ForH2D[i]<<", "<<right2ForH2D[i]<<std::endl;
		std::string name = vecH2D[i]->GetName();
		TCanvas* c = new TCanvas(name.c_str());
		vecH2D[i]->GetXaxis()->SetTitle(xNameForH2D[i].c_str());
		vecH2D[i]->GetYaxis()->SetTitle(yNameForH2D[i].c_str());
		vecH2D[i]->Draw("COLZ");
		vecH2D[i]->Write();
		std::string fileName = OutputDir + name + ".pdf";
		c->Print(fileName.c_str());
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
		std::string name = nameForGraph[i];
		TCanvas* c = new TCanvas(nameForGraph[i].c_str());
		TGraph *aTGraph = new TGraph(sizeOfThisGraph,&xForGraph[i][0],&yForGraph[i][0]);
		aTGraph->SetTitle(titleForGraph[i].c_str());
		int nCompare = compareForGraph[i];
		if ( nCompare ) if (verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<nCompare<<" graphs to be compared"<<std::endl;
		std::vector<double> yforgraph = yForGraph[i];
		std::vector<double> xforgraph = xForGraph[i];
		double currentMaximum = *std::max_element(yforgraph.begin(),yforgraph.end());
		if (verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<"    currentMaximum y value is ("<<currentMaximum<<")"<<std::endl;
		for ( int j = 1; j <= nCompare; j++ ){
			double maximum = *std::max_element(yForGraph[i+j].begin(),yForGraph[i+j].end());
			if (verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<"    Maximum y for "<<nameForGraph[i+j]<<" is ("<<maximum<<")"<<std::endl;
			if ( maximum > currentMaximum ){
				currentMaximum = maximum;
			}
		}
		if (verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<"  maximum y value is ("<<currentMaximum<<")"<<std::endl;
		if ( xlogForGraph[i] ) gPad->SetLogx(1);
		else gPad->SetLogx(0);
		if ( ylogForGraph[i] ) gPad->SetLogy(1);
		else gPad->SetLogy(0);
		if ( xlogForGraph[i] ) aTGraph->GetXaxis()->SetRangeUser(minxForGraph[i],2*maxxForGraph[i]);
		else aTGraph->GetXaxis()->SetRangeUser(minxForGraph[i],1.05*maxxForGraph[i]);
		if ( ylogForGraph[i] ) aTGraph->GetYaxis()->SetRangeUser(minyForGraph[i],2*currentMaximum);
		else aTGraph->GetYaxis()->SetRangeUser(minyForGraph[i],1.05*currentMaximum);
		aTGraph->GetXaxis()->SetTitle(xNameForGraph[i].c_str());
		aTGraph->GetYaxis()->SetTitle(yNameForGraph[i].c_str());
		aTGraph->SetMarkerStyle(markerForGraph[i]);
		aTGraph->SetMarkerColor(colorForGraph[i]);
		aTGraph->SetLineColor(colorForGraph[i]);
		std::string drawOpt = "A"+drawOptForGraph[i];
		aTGraph->Draw(drawOpt.c_str());
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
			bTGraph->SetTitle(titleForGraph[i].c_str());
			bTGraph->GetXaxis()->SetTitle(xNameForGraph[i].c_str());
			bTGraph->GetYaxis()->SetTitle(yNameForGraph[i].c_str());
			bTGraph->SetLineColor(colorForGraph[i]);
			bTGraph->SetMarkerStyle(markerForGraph[i]);
			bTGraph->SetMarkerColor(colorForGraph[i]);
			bTGraph->SetLineColor(colorForGraph[i]);
			bTGraph->Draw(drawOptForGraph[i].c_str());
			bTGraph->Write();
		}
		std::string fileName = OutputDir + name + ".pdf";
		c->Print(fileName.c_str());
	}

	//TTree *m_TTree = m_TChain->CloneTree();
	//m_TTree->Write();
	//file->Write();
	file->Close();

	delete file;
	return 0;
}

int get_TH2D(std::string name){
	for ( int i = 0; i < vecH2D.size(); i++ ){
		if ( nameForH2D[i] == name ) return i;
	}
	std::cout<<"###!!!In get_TH2D: CAN NOT FIND "<<name<<"!!!"<<std::endl;
	return -1;
}

int get_TH1D(std::string name){
	for ( int i = 0; i < vecH1D.size(); i++ ){
		if ( nameForH1D[i] == name ) return i;
	}
	std::cout<<"###!!!In get_TH1D: CAN NOT FIND "<<name<<"!!!"<<std::endl;
	return -1;
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
		token.erase(token.find_last_not_of('\t')+1);
		token.erase(0,token.find_first_not_of('\t'));
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
	strcpy(m_workMode,"gen");
	verbose = 0;
	nEvents = 0;
	printModule = 10000;
}

void print_usage(char* prog_name)
{
	fprintf(stderr,"Usage %s [options (args)] [input files]\n",prog_name);
	fprintf(stderr,"[options]\n");
	fprintf(stderr,"\t -m\n");
	fprintf(stderr,"\t\t choose work mode: [gen(default), com]\n");
	fprintf(stderr,"\t -v\n");
	fprintf(stderr,"\t\t verbose level\n");
	fprintf(stderr,"\t -n\n");
	fprintf(stderr,"\t\t nEvent\n");
	fprintf(stderr,"\t -p\n");
	fprintf(stderr,"\t\t printModule\n");
	fprintf(stderr,"\t -h\n");
	fprintf(stderr,"\t\t Usage message.\n");
	fprintf(stderr,"[example]\n");
	fprintf(stderr,"\t\t%s -m ab -v 20 -n 100\n",prog_name);
}
