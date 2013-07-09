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
bool backup = false;

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
	//*************read parameter**********
	//if (argc==1) {
	//	print_usage(argv[0]);
	//	return -1;
	//}
	init_args();
	int result;
	while((result=getopt(argc,argv,"hbv:n:m:p:"))!=-1){
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
	int N4 = 0;
	int N5 = 0;
	int N6 = 0;
	int N7 = 0;

	//=======================================================================================================
	//************READ THE FILES********************
	if (verbose >= Verbose_SectorInfo ) std::cout<<prefix_SectorInfo<<"In READ THE FILES###"<<std::endl;
	std::string TreeName = "tree";
	TChain *m_TChain = new TChain(TreeName.c_str());

	int iStart = 0;
	int nBit = 2;
	for ( int iFile = 0; iFile < DirNames.size(); iFile++ ){
		int nCPU = NCPU[iFile];
		int njob = NJob[iFile];
		if ( verbose >= Verbose_FileInfo) std::cout<<prefix_FileInfo<<"FileList \""<<DirNames[iFile]<<"\" with runname = \""<<RunNames[iFile]<<"\" has "<<NJob[iFile]<<" jobs on "<<NCPU[iFile]<<" CPUs"<<std::endl;
		for (int i = iStart; i < iStart + nCPU; i ++){
			for (int j = iStart; j < iStart + njob; j ++){
				buff.str("");
				buff.clear();
				buff<<DirNames[iFile]<<"/"<<i<<"_job"<<j<<RunNames[iFile]<<".raw";
				m_TChain->Add(buff.str().c_str());
			}
		}
	}
	for ( int iFile = 0; iFile < oFileName.size(); iFile++ ){
		m_TChain->Add(oFileName[iFile].c_str());
	}

	//=======================================================================================================
	//************SET Branches********************
	int evt_num = 0;
	int run_num = 0;
	int McTruth_nTracks;
	std::vector<int> *McTruth_pid = 0;
	std::vector<int> *McTruth_tid = 0;
	std::vector<int> *McTruth_ptid = 0;
	std::vector<int> *McTruth_time = 0;
	std::vector<double> *McTruth_px = 0;
	std::vector<double> *McTruth_py = 0;
	std::vector<double> *McTruth_pz = 0;
	std::vector<double> *McTruth_e = 0;
	std::vector<double> *McTruth_x = 0;
	std::vector<double> *McTruth_y = 0;
	std::vector<double> *McTruth_z = 0;
	std::vector<int> *McTruth_charge = 0;
	std::vector<std::string> *McTruth_particleName = 0;
	std::vector<std::string> *McTruth_process = 0;
	std::vector<std::string> *McTruth_volume = 0;

	int ProcessCounting_nSteps = 0;
	std::vector<int> *ProcessCounting_pid = 0;
	std::vector<int> *ProcessCounting_tid = 0;
	std::vector<int> *ProcessCounting_nSec = 0;
	std::vector<double> *ProcessCounting_time = 0;
	std::vector<double> *ProcessCounting_stepL = 0;
	std::vector<double> *ProcessCounting_prePx = 0;
	std::vector<double> *ProcessCounting_prePy = 0;
	std::vector<double> *ProcessCounting_prePz = 0;
	std::vector<double> *ProcessCounting_postPx = 0;
	std::vector<double> *ProcessCounting_postPy = 0;
	std::vector<double> *ProcessCounting_postPz = 0;
	std::vector<double> *ProcessCounting_dE = 0;
	std::vector<double> *ProcessCounting_dTheta = 0;
	std::vector<double> *ProcessCounting_e = 0;
	std::vector<double> *ProcessCounting_preX = 0;
	std::vector<double> *ProcessCounting_preY = 0;
	std::vector<double> *ProcessCounting_preZ = 0;
	std::vector<double> *ProcessCounting_postX = 0;
	std::vector<double> *ProcessCounting_postY = 0;
	std::vector<double> *ProcessCounting_postZ = 0;
	std::vector<int> *ProcessCounting_charge = 0;
	std::vector<std::string> *ProcessCounting_particleName = 0;
	std::vector<std::string> *ProcessCounting_process = 0;
	std::vector<int> *ProcessCounting_ASDI_msc = 0;
	std::vector<int> *ProcessCounting_ASDI_hIoni = 0;
	std::vector<int> *ProcessCounting_ASDI_hadElastic = 0;
	std::vector<int> *ProcessCounting_ASDI_Transportation = 0;
	std::vector<int> *ProcessCounting_ASDI_Decay = 0;
	std::vector<int> *ProcessCounting_ASDI_ionIoni = 0;
	std::vector<int> *ProcessCounting_ASDI_hBrems = 0;
	std::vector<int> *ProcessCounting_ASDI_hPairProd = 0;
	std::vector<int> *ProcessCounting_ASDI_eBrem = 0;
	std::vector<int> *ProcessCounting_ASDI_eIoni = 0;
	std::vector<int> *ProcessCounting_PSDI_eIoni = 0;
	std::vector<int> *ProcessCounting_PSDI_eBrem = 0;
	std::vector<int> *ProcessCounting_PSDI_msc = 0;
	std::vector<int> *ProcessCounting_PSDI_hPairProd = 0;
	std::vector<int> *ProcessCounting_PSDI_hBrems = 0;
	std::vector<int> *ProcessCounting_PSDI_hIoni = 0;
	std::vector<int> *ProcessCounting_PSDI_hadElastic = 0;
	std::vector<int> *ProcessCounting_PSDI_Transportation = 0;
	std::vector<int> *ProcessCounting_PSDI_Decay = 0;
	std::vector<int> *ProcessCounting_PSDI_ionIoni = 0;
	std::vector<std::string> *ProcessCounting_volume = 0;

	TBranch *bMcTruth_pid = 0;
	TBranch *bMcTruth_tid = 0;
	TBranch *bMcTruth_ptid = 0;
	TBranch *bMcTruth_time = 0;             
	TBranch *bMcTruth_px = 0;              
	TBranch *bMcTruth_py = 0;              
	TBranch *bMcTruth_pz = 0;              
	TBranch *bMcTruth_e = 0;               
	TBranch *bMcTruth_x = 0;                
	TBranch *bMcTruth_y = 0;                
	TBranch *bMcTruth_z = 0;                
	TBranch *bMcTruth_charge = 0;
	TBranch *bMcTruth_particleName = 0;
	TBranch *bMcTruth_process = 0;
	TBranch *bMcTruth_volume = 0;

	TBranch *bProcessCounting_pid = 0;
	TBranch *bProcessCounting_tid = 0;
	TBranch *bProcessCounting_nSec = 0;
	TBranch *bProcessCounting_time = 0;
	TBranch *bProcessCounting_stepL = 0;
	TBranch *bProcessCounting_prePx = 0;
	TBranch *bProcessCounting_prePy = 0;
	TBranch *bProcessCounting_prePz = 0;
	TBranch *bProcessCounting_postPx = 0;
	TBranch *bProcessCounting_postPy = 0;
	TBranch *bProcessCounting_postPz = 0;
	TBranch *bProcessCounting_dE = 0;
	TBranch *bProcessCounting_dTheta = 0;
	TBranch *bProcessCounting_e = 0;
	TBranch *bProcessCounting_preX = 0;
	TBranch *bProcessCounting_preY = 0;
	TBranch *bProcessCounting_preZ = 0;
	TBranch *bProcessCounting_postX = 0;
	TBranch *bProcessCounting_postY = 0;
	TBranch *bProcessCounting_postZ = 0;
	TBranch *bProcessCounting_charge = 0;
	TBranch *bProcessCounting_particleName = 0;
	TBranch *bProcessCounting_process = 0;
	TBranch *bProcessCounting_ASDI_msc = 0;
	TBranch *bProcessCounting_ASDI_hIoni = 0;
	TBranch *bProcessCounting_ASDI_hadElastic = 0;
	TBranch *bProcessCounting_ASDI_Transportation = 0;
	TBranch *bProcessCounting_ASDI_Decay = 0;
	TBranch *bProcessCounting_ASDI_ionIoni = 0;
	TBranch *bProcessCounting_ASDI_hBrems = 0;
	TBranch *bProcessCounting_ASDI_hPairProd = 0;
	TBranch *bProcessCounting_ASDI_eBrem = 0;
	TBranch *bProcessCounting_ASDI_eIoni = 0;
	TBranch *bProcessCounting_PSDI_eIoni = 0;
	TBranch *bProcessCounting_PSDI_eBrem = 0;
	TBranch *bProcessCounting_PSDI_msc = 0;
	TBranch *bProcessCounting_PSDI_hPairProd = 0;
	TBranch *bProcessCounting_PSDI_hBrems = 0;
	TBranch *bProcessCounting_PSDI_hIoni = 0;
	TBranch *bProcessCounting_PSDI_hadElastic = 0;
	TBranch *bProcessCounting_PSDI_Transportation = 0;
	TBranch *bProcessCounting_PSDI_Decay = 0;
	TBranch *bProcessCounting_PSDI_ionIoni = 0;
	TBranch *bProcessCounting_volume = 0;

	m_TChain->SetBranchAddress("evt_num", &evt_num);
	m_TChain->SetBranchAddress("run_num", &run_num);
	m_TChain->SetBranchAddress("McTruth_nTracks", &McTruth_nTracks);
	m_TChain->SetBranchAddress("McTruth_pid", &McTruth_pid, &bMcTruth_pid);
	m_TChain->SetBranchAddress("McTruth_tid", &McTruth_tid, &bMcTruth_tid);
	m_TChain->SetBranchAddress("McTruth_ptid", &McTruth_ptid, &bMcTruth_ptid);
	m_TChain->SetBranchAddress("McTruth_time", &McTruth_time, &bMcTruth_time);             
	m_TChain->SetBranchAddress("McTruth_px", &McTruth_px, &bMcTruth_px);              
	m_TChain->SetBranchAddress("McTruth_py", &McTruth_py, &bMcTruth_py);              
	m_TChain->SetBranchAddress("McTruth_pz", &McTruth_pz, &bMcTruth_pz);              
	m_TChain->SetBranchAddress("McTruth_e", &McTruth_e, &bMcTruth_e);               
	m_TChain->SetBranchAddress("McTruth_x", &McTruth_x, &bMcTruth_x);                
	m_TChain->SetBranchAddress("McTruth_y", &McTruth_y, &bMcTruth_y);                
	m_TChain->SetBranchAddress("McTruth_z", &McTruth_z, &bMcTruth_z);                
	m_TChain->SetBranchAddress("McTruth_charge", &McTruth_charge, &bMcTruth_charge);
	m_TChain->SetBranchAddress("McTruth_particleName", &McTruth_particleName, &bMcTruth_particleName);
	m_TChain->SetBranchAddress("McTruth_process", &McTruth_process, &bMcTruth_process);
	m_TChain->SetBranchAddress("McTruth_volume", &McTruth_volume, &bMcTruth_volume);

	m_TChain->SetBranchAddress("ProcessCounting_nSteps", &ProcessCounting_nSteps);
	m_TChain->SetBranchAddress("ProcessCounting_pid", &ProcessCounting_pid, &bProcessCounting_pid);
	m_TChain->SetBranchAddress("ProcessCounting_tid", &ProcessCounting_tid, &bProcessCounting_tid);
	m_TChain->SetBranchAddress("ProcessCounting_nSec", &ProcessCounting_nSec, &bProcessCounting_nSec);
	m_TChain->SetBranchAddress("ProcessCounting_time", &ProcessCounting_time, &bProcessCounting_time);
	m_TChain->SetBranchAddress("ProcessCounting_stepL", &ProcessCounting_stepL, &bProcessCounting_stepL);
	m_TChain->SetBranchAddress("ProcessCounting_prePx", &ProcessCounting_prePx, &bProcessCounting_prePx);
	m_TChain->SetBranchAddress("ProcessCounting_prePy", &ProcessCounting_prePy, &bProcessCounting_prePy);
	m_TChain->SetBranchAddress("ProcessCounting_prePz", &ProcessCounting_prePz, &bProcessCounting_prePz);
	m_TChain->SetBranchAddress("ProcessCounting_postPx", &ProcessCounting_postPx, &bProcessCounting_postPx);
	m_TChain->SetBranchAddress("ProcessCounting_postPy", &ProcessCounting_postPy, &bProcessCounting_postPy);
	m_TChain->SetBranchAddress("ProcessCounting_postPz", &ProcessCounting_postPz, &bProcessCounting_postPz);
	m_TChain->SetBranchAddress("ProcessCounting_dE", &ProcessCounting_dE, &bProcessCounting_dE);
	m_TChain->SetBranchAddress("ProcessCounting_dTheta", &ProcessCounting_dTheta, &bProcessCounting_dTheta);
	m_TChain->SetBranchAddress("ProcessCounting_e", &ProcessCounting_e, &bProcessCounting_e);
	m_TChain->SetBranchAddress("ProcessCounting_preX", &ProcessCounting_preX, &bProcessCounting_preX);
	m_TChain->SetBranchAddress("ProcessCounting_preY", &ProcessCounting_preY, &bProcessCounting_preY);
	m_TChain->SetBranchAddress("ProcessCounting_preZ", &ProcessCounting_preZ, &bProcessCounting_preZ);
	m_TChain->SetBranchAddress("ProcessCounting_postX", &ProcessCounting_postX, &bProcessCounting_postX);
	m_TChain->SetBranchAddress("ProcessCounting_postY", &ProcessCounting_postY, &bProcessCounting_postY);
	m_TChain->SetBranchAddress("ProcessCounting_postZ", &ProcessCounting_postZ, &bProcessCounting_postZ);
	m_TChain->SetBranchAddress("ProcessCounting_charge", &ProcessCounting_charge, &bProcessCounting_charge);
	m_TChain->SetBranchAddress("ProcessCounting_particleName", &ProcessCounting_particleName, &bProcessCounting_particleName);
	m_TChain->SetBranchAddress("ProcessCounting_process", &ProcessCounting_process, &bProcessCounting_process);
	m_TChain->SetBranchAddress("ProcessCounting_ASDI_msc", &ProcessCounting_ASDI_msc, &bProcessCounting_ASDI_msc);
	m_TChain->SetBranchAddress("ProcessCounting_ASDI_hIoni", &ProcessCounting_ASDI_hIoni, &bProcessCounting_ASDI_hIoni);
	m_TChain->SetBranchAddress("ProcessCounting_ASDI_hadElastic", &ProcessCounting_ASDI_hadElastic, &bProcessCounting_ASDI_hadElastic);
	m_TChain->SetBranchAddress("ProcessCounting_ASDI_Transportation", &ProcessCounting_ASDI_Transportation, &bProcessCounting_ASDI_Transportation);
	m_TChain->SetBranchAddress("ProcessCounting_ASDI_Decay", &ProcessCounting_ASDI_Decay, &bProcessCounting_ASDI_Decay);
	m_TChain->SetBranchAddress("ProcessCounting_ASDI_ionIoni", &ProcessCounting_ASDI_ionIoni, &bProcessCounting_ASDI_ionIoni);
	m_TChain->SetBranchAddress("ProcessCounting_ASDI_hBrems", &ProcessCounting_ASDI_hBrems, &bProcessCounting_ASDI_hBrems);
	m_TChain->SetBranchAddress("ProcessCounting_ASDI_hPairProd", &ProcessCounting_ASDI_hPairProd, &bProcessCounting_ASDI_hPairProd);
	m_TChain->SetBranchAddress("ProcessCounting_ASDI_eBrem", &ProcessCounting_ASDI_eBrem, &bProcessCounting_ASDI_eBrem);
	m_TChain->SetBranchAddress("ProcessCounting_ASDI_eIoni", &ProcessCounting_ASDI_eIoni, &bProcessCounting_ASDI_eIoni);
	m_TChain->SetBranchAddress("ProcessCounting_PSDI_eIoni", &ProcessCounting_PSDI_eIoni, &bProcessCounting_PSDI_eIoni);
	m_TChain->SetBranchAddress("ProcessCounting_PSDI_eBrem", &ProcessCounting_PSDI_eBrem, &bProcessCounting_PSDI_eBrem);
	m_TChain->SetBranchAddress("ProcessCounting_PSDI_msc", &ProcessCounting_PSDI_msc, &bProcessCounting_PSDI_msc);
	m_TChain->SetBranchAddress("ProcessCounting_PSDI_hPairProd", &ProcessCounting_PSDI_hPairProd, &bProcessCounting_PSDI_hPairProd);
	m_TChain->SetBranchAddress("ProcessCounting_PSDI_hBrems", &ProcessCounting_PSDI_hBrems, &bProcessCounting_PSDI_hBrems);
	m_TChain->SetBranchAddress("ProcessCounting_PSDI_hIoni", &ProcessCounting_PSDI_hIoni, &bProcessCounting_PSDI_hIoni);
	m_TChain->SetBranchAddress("ProcessCounting_PSDI_hadElastic", &ProcessCounting_PSDI_hadElastic, &bProcessCounting_PSDI_hadElastic);
	m_TChain->SetBranchAddress("ProcessCounting_PSDI_Transportation", &ProcessCounting_PSDI_Transportation, &bProcessCounting_PSDI_Transportation);
	m_TChain->SetBranchAddress("ProcessCounting_PSDI_Decay", &ProcessCounting_PSDI_Decay, &bProcessCounting_PSDI_Decay);
	m_TChain->SetBranchAddress("ProcessCounting_PSDI_ionIoni", &ProcessCounting_PSDI_ionIoni, &bProcessCounting_PSDI_ionIoni);
	m_TChain->SetBranchAddress("ProcessCounting_volume", &ProcessCounting_volume, &bProcessCounting_volume);

	TTree* d_tree = new TTree( "t", "t" );

	int d_evt_num;
	int d_run_num;
	double d_r;
	double d_x;
	double d_y;
	double d_z;
	double d_t;
	double d_tcap;
	double d_relz;
	int d_i;
	int d_sto;
	int d_cap;

	d_tree->Branch("evt_num", &d_evt_num, "evt_num/I");
	d_tree->Branch("run_num", &d_run_num, "run_num/I");
	d_tree->Branch("r", &d_r, "r/D");
	d_tree->Branch("x", &d_x, "x/D");
	d_tree->Branch("y", &d_y, "y/D");
	d_tree->Branch("z", &d_z, "z/D");
	d_tree->Branch("relz", &d_relz, "relz/D");
	d_tree->Branch("t", &d_t, "t/D");
	d_tree->Branch("tcap", &d_tcap, "tcap/D");
	d_tree->Branch("i", &d_i, "i/I");
	d_tree->Branch("sto", &d_sto, "sto/I");
	d_tree->Branch("cap", &d_cap, "cap/I");

	//=======================================================================================================
	//************DO THE DIRTY WORK*******************
	if (verbose >= Verbose_SectorInfo) std::cout<<prefix_SectorInfo<<"In DO THE DIRTY WORK ###"<<std::endl;

	if (!strcmp(m_workMode,"gen")){
		Long64_t nEvent = m_TChain->GetEntries();
		for( Long64_t iEvent = 0; iEvent < nEvent; iEvent++ ){
			if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"In Event "<<iEvent<<std::endl;
			N0++;
			Long64_t tentry = m_TChain->LoadTree(iEvent);
			if(bMcTruth_pid) bMcTruth_pid->GetEntry(tentry);
			if(bMcTruth_tid) bMcTruth_tid->GetEntry(tentry);
			if(bMcTruth_ptid) bMcTruth_ptid->GetEntry(tentry);
			if(bMcTruth_time) bMcTruth_time->GetEntry(tentry);             
			if(bMcTruth_px) bMcTruth_px->GetEntry(tentry);              
			if(bMcTruth_py) bMcTruth_py->GetEntry(tentry);              
			if(bMcTruth_pz) bMcTruth_pz->GetEntry(tentry);              
			if(bMcTruth_e) bMcTruth_e->GetEntry(tentry);               
			if(bMcTruth_x) bMcTruth_x->GetEntry(tentry);                
			if(bMcTruth_y) bMcTruth_y->GetEntry(tentry);                
			if(bMcTruth_z) bMcTruth_z->GetEntry(tentry);                
			if(bMcTruth_charge) bMcTruth_charge->GetEntry(tentry);
			if(bMcTruth_particleName) bMcTruth_particleName->GetEntry(tentry);
			if(bMcTruth_process) bMcTruth_process->GetEntry(tentry);
			if(bMcTruth_volume) bMcTruth_volume->GetEntry(tentry);

			if(bProcessCounting_pid) bProcessCounting_pid->GetEntry(tentry);
			if(bProcessCounting_tid) bProcessCounting_tid->GetEntry(tentry);
			if(bProcessCounting_nSec) bProcessCounting_nSec->GetEntry(tentry);
			if(bProcessCounting_time) bProcessCounting_time->GetEntry(tentry);
			if(bProcessCounting_stepL) bProcessCounting_stepL->GetEntry(tentry);
			if(bProcessCounting_prePx) bProcessCounting_prePx->GetEntry(tentry);
			if(bProcessCounting_prePy) bProcessCounting_prePy->GetEntry(tentry);
			if(bProcessCounting_prePz) bProcessCounting_prePz->GetEntry(tentry);
			if(bProcessCounting_postPx) bProcessCounting_postPx->GetEntry(tentry);
			if(bProcessCounting_postPy) bProcessCounting_postPy->GetEntry(tentry);
			if(bProcessCounting_postPz) bProcessCounting_postPz->GetEntry(tentry);
			if(bProcessCounting_dE) bProcessCounting_dE->GetEntry(tentry);
			if(bProcessCounting_dTheta) bProcessCounting_dTheta->GetEntry(tentry);
			if(bProcessCounting_e) bProcessCounting_e->GetEntry(tentry);
			if(bProcessCounting_preX) bProcessCounting_preX->GetEntry(tentry);
			if(bProcessCounting_preY) bProcessCounting_preY->GetEntry(tentry);
			if(bProcessCounting_preZ) bProcessCounting_preZ->GetEntry(tentry);
			if(bProcessCounting_postX) bProcessCounting_postX->GetEntry(tentry);
			if(bProcessCounting_postY) bProcessCounting_postY->GetEntry(tentry);
			if(bProcessCounting_postZ) bProcessCounting_postZ->GetEntry(tentry);
			if(bProcessCounting_charge) bProcessCounting_charge->GetEntry(tentry);
			if(bProcessCounting_particleName) bProcessCounting_particleName->GetEntry(tentry);
			if(bProcessCounting_process) bProcessCounting_process->GetEntry(tentry);
			if(bProcessCounting_ASDI_msc) bProcessCounting_ASDI_msc->GetEntry(tentry);
			if(bProcessCounting_ASDI_hIoni) bProcessCounting_ASDI_hIoni->GetEntry(tentry);
			if(bProcessCounting_ASDI_hadElastic) bProcessCounting_ASDI_hadElastic->GetEntry(tentry);
			if(bProcessCounting_ASDI_Transportation) bProcessCounting_ASDI_Transportation->GetEntry(tentry);
			if(bProcessCounting_ASDI_Decay) bProcessCounting_ASDI_Decay->GetEntry(tentry);
			if(bProcessCounting_ASDI_ionIoni) bProcessCounting_ASDI_ionIoni->GetEntry(tentry);
			if(bProcessCounting_ASDI_hBrems) bProcessCounting_ASDI_hBrems->GetEntry(tentry);
			if(bProcessCounting_ASDI_hPairProd) bProcessCounting_ASDI_hPairProd->GetEntry(tentry);
			if(bProcessCounting_ASDI_eBrem) bProcessCounting_ASDI_eBrem->GetEntry(tentry);
			if(bProcessCounting_ASDI_eIoni) bProcessCounting_ASDI_eIoni->GetEntry(tentry);
			if(bProcessCounting_PSDI_eIoni) bProcessCounting_PSDI_eIoni->GetEntry(tentry);
			if(bProcessCounting_PSDI_eBrem) bProcessCounting_PSDI_eBrem->GetEntry(tentry);
			if(bProcessCounting_PSDI_msc) bProcessCounting_PSDI_msc->GetEntry(tentry);
			if(bProcessCounting_PSDI_hPairProd) bProcessCounting_PSDI_hPairProd->GetEntry(tentry);
			if(bProcessCounting_PSDI_hBrems) bProcessCounting_PSDI_hBrems->GetEntry(tentry);
			if(bProcessCounting_PSDI_hIoni) bProcessCounting_PSDI_hIoni->GetEntry(tentry);
			if(bProcessCounting_PSDI_hadElastic) bProcessCounting_PSDI_hadElastic->GetEntry(tentry);
			if(bProcessCounting_PSDI_Transportation) bProcessCounting_PSDI_Transportation->GetEntry(tentry);
			if(bProcessCounting_PSDI_Decay) bProcessCounting_PSDI_Decay->GetEntry(tentry);
			if(bProcessCounting_PSDI_ionIoni) bProcessCounting_PSDI_ionIoni->GetEntry(tentry);
			if(bProcessCounting_volume) bProcessCounting_volume->GetEntry(tentry);

			m_TChain->GetEntry(iEvent);
			if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Got entries"<<std::endl;

			// Got stopped?
			int nPlates = 17;
			double thickness = 0.02;
			double space = 5;
			double right_most_end = (nPlates+1)/2.*space+thickness/2.;
			int index_OB = -1;
			for ( int i_step = 0; i_step < ProcessCounting_nSteps; i_step++ ){
				double z = (*ProcessCounting_postZ)[i_step];
				if (z>=right_most_end){
					index_OB = i_step;
					break;
				}
			}

			// Got captured? which step?
			int index_cap = -1;
			for ( int i_step = 0; i_step < ProcessCounting_nSteps; i_step++ ){
				std::string processName = (*ProcessCounting_process)[i_step];
				if (processName == "muMinusCaptureAtRest"){
					index_cap = i_step;
					break;
				}
			}

			// Position and which plate?
			double hit_x = index_cap==-1?0:(*ProcessCounting_postX)[index_cap];
			double hit_y = index_cap==-1?0:(*ProcessCounting_postY)[index_cap];
			double hit_z = index_cap==-1?0:(*ProcessCounting_postZ)[index_cap];
			double hit_t = index_cap==-1?0:(*ProcessCounting_time)[index_cap];
			double hit_r = sqrt(hit_x*hit_x+hit_y*hit_y);
			double hit_relZ = 0;
			int hit_iPlate = -1;
			for ( int i_plate = 0; i_plate < nPlates; i_plate++ ){
				double plateZ = (i_plate*2 - nPlates +1)/2*space;
				double leftEnd = plateZ - thickness/2;
				double rightEnd = plateZ + thickness/2;
				if ( hit_z >= leftEnd && hit_z <= rightEnd){
					hit_iPlate = i_plate;
					hit_relZ = hit_z - plateZ;
					break;
				}
			}

			// Fill the tree
			d_evt_num = evt_num;
			d_run_num = run_num;
			d_r = hit_r;
			d_x = hit_x;
			d_y = hit_y;
			d_z = hit_z;
			d_relz = hit_relZ;
			double decayTime = -880*log(G4UniformRand());
			d_t = hit_t+decayTime;
			d_tcap = hit_t;
			d_i = hit_iPlate;
			d_sto = (index_OB==-1);
			d_cap = (index_cap != -1);
			d_tree->Fill();

			if (index_OB!=-1)
				continue;
			N1++;
			if ( index_cap == -1 )
				continue;
			N2++;
			if ( hit_iPlate == -1 )
				continue;
			N3++;

			if ( (index_temp = get_TH1D("relz")) != -1 ){
				vecH1D[index_temp]->Fill(hit_relZ*10); //cm -> mm
			}
			if ( (index_temp = get_TH1D("iplate")) != -1 ){
				vecH1D[index_temp]->Fill(hit_iPlate);
			}
			if ( (index_temp = get_TH1D("r")) != -1 ){
				vecH1D[index_temp]->Fill(hit_r);
			}
			buff.str("");
			buff.clear();
			buff<<"relz_"<<hit_iPlate;
			if ( (index_temp = get_TH1D(buff.str())) != -1 ){
				vecH1D[index_temp]->Fill(hit_relZ*10); //cm -> mm
			}
			buff.str("");
			buff.clear();
			buff<<"r_"<<hit_iPlate;
			if ( (index_temp = get_TH1D(buff.str())) != -1 ){
				vecH1D[index_temp]->Fill(hit_r);
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
	std::cout<<"N4 = "<<N4<<std::endl;
	std::cout<<"N5 = "<<N5<<std::endl;
	std::cout<<"N6 = "<<N6<<std::endl;
	std::cout<<"N7 = "<<N7<<std::endl;

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
		gPad->SetGridx(1);
		gPad->SetGridy(1);
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
		if (verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<"    Integral of ("<<nameForH1D[i]<<"): "<<vecH1D[i]->Integral()<<std::endl;
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
			if (verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<"    Integral of ("<<nameForH1D[i]<<"): "<<vecH1D[i]->Integral()<<std::endl;
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
		gPad->SetGridx(1);
		gPad->SetGridy(1);
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
		gPad->SetGridx(1);
		gPad->SetGridy(1);
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

	d_tree->Write();
	file->Close();
	std::string backupFileName = OutputDir + "backup.root";

	if (backup){
		TFile *file2 = new TFile(backupFileName.c_str(),"RECREATE");
		m_TChain->CloneTree(-1,"fast");
		file2->Write();
		file2->Close();
	}

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
	backup = false;
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
	fprintf(stderr,"\t -b\n");
	fprintf(stderr,"\t\t restore backup file.\n");
	fprintf(stderr,"[example]\n");
	fprintf(stderr,"\t\t%s -m ab -v 20 -n 100\n",prog_name);
}
