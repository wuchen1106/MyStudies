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

	int Trigger_nHits = 0;
	std::vector<double> *Trigger_x = 0;
	std::vector<double> *Trigger_y = 0;
	std::vector<double> *Trigger_z = 0;
	std::vector<double> *Trigger_t = 0;
	std::vector<double> *Trigger_px = 0;
	std::vector<double> *Trigger_py = 0;
	std::vector<double> *Trigger_pz = 0;
	std::vector<double> *Trigger_e = 0;
	std::vector<double> *Trigger_edep = 0;
	std::vector<double> *Trigger_stepL = 0;
	std::vector<int> *Trigger_volID = 0;
	std::vector<std::string> *Trigger_volName = 0;
	std::vector<int> *Trigger_tid = 0;
	std::vector<int> *Trigger_pid = 0;
	std::vector<int> *Trigger_charge = 0;

	int CdcCell_nHits;
	std::vector<double> *CdcCell_x = 0;
	std::vector<double> *CdcCell_y = 0;
	std::vector<double> *CdcCell_z = 0;
	std::vector<double> *CdcCell_pIx = 0;
	std::vector<double> *CdcCell_pIy = 0;
	std::vector<double> *CdcCell_pIz = 0;
	std::vector<double> *CdcCell_pOx = 0;
	std::vector<double> *CdcCell_pOy = 0;
	std::vector<double> *CdcCell_pOz = 0;
	std::vector<double> *CdcCell_t = 0;
	std::vector<double> *CdcCell_px = 0;
	std::vector<double> *CdcCell_py = 0;
	std::vector<double> *CdcCell_pz = 0;
	std::vector<double> *CdcCell_e = 0;
	std::vector<double> *CdcCell_edep = 0;
	std::vector<double> *CdcCell_driftD_smeared = 0;
	std::vector<double> *CdcCell_driftD = 0;
	std::vector<double> *CdcCell_error = 0;
	std::vector<int> *CdcCell_status = 0;
	std::vector<int> *CdcCell_nTry = 0;
	std::vector<int> *CdcCell_layerID = 0;
	std::vector<int> *CdcCell_cellID = 0;
	std::vector<int> *CdcCell_tid = 0;
	std::vector<int> *CdcCell_pid = 0;

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

	TBranch *bTrigger_x = 0;
	TBranch *bTrigger_y = 0;
	TBranch *bTrigger_z = 0;
	TBranch *bTrigger_t = 0;
	TBranch *bTrigger_px = 0;
	TBranch *bTrigger_py = 0;
	TBranch *bTrigger_pz = 0;
	TBranch *bTrigger_e = 0;
	TBranch *bTrigger_edep = 0;
	TBranch *bTrigger_stepL = 0;
	TBranch *bTrigger_volID = 0;
	TBranch *bTrigger_volName = 0;
	TBranch *bTrigger_tid = 0;
	TBranch *bTrigger_pid = 0;
	TBranch *bTrigger_charge = 0;

	TBranch *bCdcCell_x = 0;
	TBranch *bCdcCell_y = 0;
	TBranch *bCdcCell_z = 0;
	TBranch *bCdcCell_pIx = 0;
	TBranch *bCdcCell_pIy = 0;
	TBranch *bCdcCell_pIz = 0;
	TBranch *bCdcCell_pOx = 0;
	TBranch *bCdcCell_pOy = 0;
	TBranch *bCdcCell_pOz = 0;
	TBranch *bCdcCell_t = 0;
	TBranch *bCdcCell_px = 0;
	TBranch *bCdcCell_py = 0;
	TBranch *bCdcCell_pz = 0;
	TBranch *bCdcCell_e = 0;
	TBranch *bCdcCell_edep = 0;
	TBranch *bCdcCell_driftD_smeared = 0;
	TBranch *bCdcCell_driftD = 0;
	TBranch *bCdcCell_error = 0;
	TBranch *bCdcCell_status = 0;
	TBranch *bCdcCell_nTry = 0;
	TBranch *bCdcCell_layerID = 0;
	TBranch *bCdcCell_cellID = 0;
	TBranch *bCdcCell_tid = 0;
	TBranch *bCdcCell_pid = 0;

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

	m_TChain->SetBranchAddress("Trigger_nHits", &Trigger_nHits);
	m_TChain->SetBranchAddress("Trigger_x", &Trigger_x, &bTrigger_x);
	m_TChain->SetBranchAddress("Trigger_y", &Trigger_y, &bTrigger_y);
	m_TChain->SetBranchAddress("Trigger_z", &Trigger_z, &bTrigger_z);
	m_TChain->SetBranchAddress("Trigger_t", &Trigger_t, &bTrigger_t);
	m_TChain->SetBranchAddress("Trigger_px", &Trigger_px, &bTrigger_px);
	m_TChain->SetBranchAddress("Trigger_py", &Trigger_py, &bTrigger_py);
	m_TChain->SetBranchAddress("Trigger_pz", &Trigger_pz, &bTrigger_pz);
	m_TChain->SetBranchAddress("Trigger_e", &Trigger_e, &bTrigger_e);
	m_TChain->SetBranchAddress("Trigger_edep", &Trigger_edep, &bTrigger_edep);
	m_TChain->SetBranchAddress("Trigger_stepL", &Trigger_stepL, &bTrigger_stepL);
	m_TChain->SetBranchAddress("Trigger_volID", &Trigger_volID, &bTrigger_volID);
	m_TChain->SetBranchAddress("Trigger_volName", &Trigger_volName, &bTrigger_volName);
	m_TChain->SetBranchAddress("Trigger_tid", &Trigger_tid, &bTrigger_tid);
	m_TChain->SetBranchAddress("Trigger_pid", &Trigger_pid, &bTrigger_pid);
	m_TChain->SetBranchAddress("Trigger_charge", &Trigger_charge, &bTrigger_charge);

	m_TChain->SetBranchAddress("CdcCell_nHits", &CdcCell_nHits);
	m_TChain->SetBranchAddress("CdcCell_x", &CdcCell_x, &bCdcCell_x);
	m_TChain->SetBranchAddress("CdcCell_y", &CdcCell_y, &bCdcCell_y);
	m_TChain->SetBranchAddress("CdcCell_z", &CdcCell_z, &bCdcCell_z);
	m_TChain->SetBranchAddress("CdcCell_pIx", &CdcCell_pIx, &bCdcCell_pIx);
	m_TChain->SetBranchAddress("CdcCell_pIy", &CdcCell_pIy, &bCdcCell_pIy);
	m_TChain->SetBranchAddress("CdcCell_pIz", &CdcCell_pIz, &bCdcCell_pIz);
	m_TChain->SetBranchAddress("CdcCell_pOx", &CdcCell_pOx, &bCdcCell_pOx);
	m_TChain->SetBranchAddress("CdcCell_pOy", &CdcCell_pOy, &bCdcCell_pOy);
	m_TChain->SetBranchAddress("CdcCell_pOz", &CdcCell_pOz, &bCdcCell_pOz);
	m_TChain->SetBranchAddress("CdcCell_t", &CdcCell_t, &bCdcCell_t);
	m_TChain->SetBranchAddress("CdcCell_px", &CdcCell_px, &bCdcCell_px);
	m_TChain->SetBranchAddress("CdcCell_py", &CdcCell_py, &bCdcCell_py);
	m_TChain->SetBranchAddress("CdcCell_pz", &CdcCell_pz, &bCdcCell_pz);
	m_TChain->SetBranchAddress("CdcCell_e", &CdcCell_e, &bCdcCell_e);
	m_TChain->SetBranchAddress("CdcCell_edep", &CdcCell_edep, &bCdcCell_edep);
	m_TChain->SetBranchAddress("CdcCell_driftD_smeared", &CdcCell_driftD_smeared, &bCdcCell_driftD_smeared);
	m_TChain->SetBranchAddress("CdcCell_driftD", &CdcCell_driftD, &bCdcCell_driftD);
	m_TChain->SetBranchAddress("CdcCell_error", &CdcCell_error, &bCdcCell_error);
	m_TChain->SetBranchAddress("CdcCell_status", &CdcCell_status, &bCdcCell_status);
	m_TChain->SetBranchAddress("CdcCell_nTry", &CdcCell_nTry, &bCdcCell_nTry);
	m_TChain->SetBranchAddress("CdcCell_layerID", &CdcCell_layerID, &bCdcCell_layerID);
	m_TChain->SetBranchAddress("CdcCell_cellID", &CdcCell_cellID, &bCdcCell_cellID);
	m_TChain->SetBranchAddress("CdcCell_tid", &CdcCell_tid, &bCdcCell_tid);
	m_TChain->SetBranchAddress("CdcCell_pid", &CdcCell_pid, &bCdcCell_pid);

	TTree* d_tree = new TTree( "t", "t" );

	int d_evt_num;
	int d_run_num;
	int d_pid;
	int d_tid;
	char d_vid[124];
	char d_prid[124];
	double d_mot_x;
	double d_mot_y;
	double d_mot_z;
	double d_mot_px;
	double d_mot_py;
	double d_mot_pz;
	int d_nhits;
	double d_hits_x[1000];
	double d_hits_y[1000];
	double d_hits_z[1000];
	double d_hits_t[1000];
	double d_hits_px[1000];
	double d_hits_py[1000];
	double d_hits_pz[1000];
	double d_tri_t;

	d_tree->Branch("evt_num", &d_evt_num, "evt_num/I");
	d_tree->Branch("run_num", &d_run_num, "run_num/I");
	d_tree->Branch("pid", &d_pid, "pid/I");
	d_tree->Branch("tid", &d_tid, "tid/I");
	d_tree->Branch("vid", d_vid, "vid[124]/C");
	d_tree->Branch("prid", d_prid, "prid[124]/C");
	d_tree->Branch("mot_x", &d_mot_x, "mot_x/D");
	d_tree->Branch("mot_y", &d_mot_y, "mot_y/D");
	d_tree->Branch("mot_z", &d_mot_z, "mot_z/D");
	d_tree->Branch("mot_px", &d_mot_px, "mot_px/D");
	d_tree->Branch("mot_py", &d_mot_py, "mot_py/D");
	d_tree->Branch("mot_pz", &d_mot_pz, "mot_pz/D");
	d_tree->Branch("nhits", &d_nhits, "nhits/I");
	d_tree->Branch("hits_x", d_hits_x, "hits_x[nhits]/D");
	d_tree->Branch("hits_y", d_hits_y, "hits_y[nhits]/D");
	d_tree->Branch("hits_z", d_hits_z, "hits_z[nhits]/D");
	d_tree->Branch("hits_t", d_hits_t, "hits_t[nhits]/D");
	d_tree->Branch("hits_px", d_hits_px, "hits_px[nhits]/D");
	d_tree->Branch("hits_py", d_hits_py, "hits_py[nhits]/D");
	d_tree->Branch("hits_pz", d_hits_pz, "hits_pz[nhits]/D");
	d_tree->Branch("tri_t", &d_tri_t, "tri_t/D");

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

			if(bTrigger_x) bTrigger_x->GetEntry(tentry);
			if(bTrigger_y) bTrigger_y->GetEntry(tentry);
			if(bTrigger_z) bTrigger_z->GetEntry(tentry);
			if(bTrigger_t) bTrigger_t->GetEntry(tentry);
			if(bTrigger_px) bTrigger_px->GetEntry(tentry);
			if(bTrigger_py) bTrigger_py->GetEntry(tentry);
			if(bTrigger_pz) bTrigger_pz->GetEntry(tentry);
			if(bTrigger_e) bTrigger_e->GetEntry(tentry);
			if(bTrigger_edep) bTrigger_edep->GetEntry(tentry);
			if(bTrigger_stepL) bTrigger_stepL->GetEntry(tentry);
			if(bTrigger_volID) bTrigger_volID->GetEntry(tentry);
			if(bTrigger_volName) bTrigger_volName->GetEntry(tentry);
			if(bTrigger_tid) bTrigger_tid->GetEntry(tentry);
			if(bTrigger_pid) bTrigger_pid->GetEntry(tentry);
			if(bTrigger_charge) bTrigger_charge->GetEntry(tentry);

			if(bCdcCell_x) bCdcCell_x->GetEntry(tentry);
			if(bCdcCell_y) bCdcCell_y->GetEntry(tentry);
			if(bCdcCell_z) bCdcCell_z->GetEntry(tentry);
			if(bCdcCell_pIx) bCdcCell_pIx->GetEntry(tentry);
			if(bCdcCell_pIy) bCdcCell_pIy->GetEntry(tentry);
			if(bCdcCell_pIz) bCdcCell_pIz->GetEntry(tentry);
			if(bCdcCell_pOx) bCdcCell_pOx->GetEntry(tentry);
			if(bCdcCell_pOy) bCdcCell_pOy->GetEntry(tentry);
			if(bCdcCell_pOz) bCdcCell_pOz->GetEntry(tentry);
			if(bCdcCell_t) bCdcCell_t->GetEntry(tentry);
			if(bCdcCell_px) bCdcCell_px->GetEntry(tentry);
			if(bCdcCell_py) bCdcCell_py->GetEntry(tentry);
			if(bCdcCell_pz) bCdcCell_pz->GetEntry(tentry);
			if(bCdcCell_e) bCdcCell_e->GetEntry(tentry);
			if(bCdcCell_edep) bCdcCell_edep->GetEntry(tentry);
			if(bCdcCell_driftD_smeared) bCdcCell_driftD_smeared->GetEntry(tentry);
			if(bCdcCell_driftD) bCdcCell_driftD->GetEntry(tentry);
			if(bCdcCell_error) bCdcCell_error->GetEntry(tentry);
			if(bCdcCell_status) bCdcCell_status->GetEntry(tentry);
			if(bCdcCell_nTry) bCdcCell_nTry->GetEntry(tentry);
			if(bCdcCell_layerID) bCdcCell_layerID->GetEntry(tentry);
			if(bCdcCell_cellID) bCdcCell_cellID->GetEntry(tentry);
			if(bCdcCell_tid) bCdcCell_tid->GetEntry(tentry);
			if(bCdcCell_pid) bCdcCell_pid->GetEntry(tentry);

			m_TChain->GetEntry(iEvent);

			// find the electron
			double pa_max = 0;
			int index = -1;
			for ( int i_par = 0; i_par<McTruth_nTracks; i_par++){
				int i_pid = (*McTruth_pid)[i_par];
				int i_ptid = (*McTruth_ptid)[i_par];
				if (i_pid==11&&i_ptid==1){
					double px = (*McTruth_px)[0];
					double py = (*McTruth_py)[0];
					double pz = (*McTruth_pz)[0];
					double pt = sqrt(px*px+py*py);
					double pa = sqrt(pt*pt+pz*pz);
					if (pa>pa_max){
						index = i_par;
						pa_max = pa;
					}
				}
			}
			if (index==-1){ // cannot find electrons from gamma
				continue;
			}
			N1++;
			if (pa_max<90){ // pa < 90MeV/c
				continue;
			}
			N2++;

			std::string process = (*McTruth_process)[index];
			std::string volume = (*McTruth_volume)[index];
			int tid = (*McTruth_tid)[index];

			if (verbose >= Verbose_EventInfo || iEvent%printModule == 0)
				std::cout<<prefix_EventInfoStart
					     <<" pa = "<<pa_max
					     <<"MeV/c, process = "<<process
					     <<std::endl;

			if (process == "compt"){
				if ( (index_temp = get_TH1D("compt_pa")) != -1 ){
					vecH1D[index_temp]->Fill(pa_max);
				}
			}
			if (process == "conv"){
				if ( (index_temp = get_TH1D("conv_pa")) != -1 ){
					vecH1D[index_temp]->Fill(pa_max);
				}
			}

			// prepare for d_tree
			d_nhits = 0;

			// Trigger system
			if (CdcCell_nHits<=0) // not hit the Cdc
				continue;
			N3++;
			double CdcCell_firstHitTime = -1;
			for ( int i_hit = 0; i_hit < CdcCell_nHits; i_hit++ ){
				int i_tid = (*CdcCell_tid)[i_hit];
				if (i_tid == tid){
					d_hits_x[d_nhits] = (*CdcCell_x)[i_hit];
					d_hits_y[d_nhits] = (*CdcCell_y)[i_hit];
					d_hits_z[d_nhits] = (*CdcCell_z)[i_hit];
					d_hits_t[d_nhits] = (*CdcCell_t)[i_hit];
					d_hits_px[d_nhits] = (*CdcCell_px)[i_hit];
					d_hits_py[d_nhits] = (*CdcCell_py)[i_hit];
					d_hits_pz[d_nhits] = (*CdcCell_pz)[i_hit];
					d_nhits++;
				}
			}
			CdcCell_firstHitTime = d_hits_t[0];
			if ( CdcCell_firstHitTime == -1) // this electron not hit the Cdc
				continue;
			N4++;
			if (Trigger_nHits<=0) // not hit the trigger
				continue;
			N5++;
			double Trigger_firstHitTime = -1;
			for ( int i_hit = 0; i_hit < Trigger_nHits; i_hit++ ){
				int i_tid = (*Trigger_tid)[i_hit];
				if (i_tid == tid){
					Trigger_firstHitTime = (*Trigger_t)[i_hit];
					break;
				}
			}
			if ( Trigger_firstHitTime == -1) // this electron not hit the trigger
				continue;
			N6++;

			// Fill the tree
			d_evt_num = evt_num;
			d_run_num = run_num;
			d_pid = 11;
			d_tid = tid;
			strcpy(d_vid,volume.c_str());
			strcpy(d_prid,process.c_str());
			d_tri_t = Trigger_firstHitTime;
			d_mot_x = (*McTruth_x)[0];
			d_mot_y = (*McTruth_y)[0];
			d_mot_z = (*McTruth_z)[0];
			d_mot_px = (*McTruth_px)[0];
			d_mot_py = (*McTruth_py)[0];
			d_mot_pz = (*McTruth_pz)[0];
			d_tree->Fill();

			if ( Trigger_firstHitTime <= CdcCell_firstHitTime ) // hit trigger first
				continue;
			N7++;

			if (process == "compt"){
				if ( (index_temp = get_TH1D("compt_pa_pass")) != -1 ){
					vecH1D[index_temp]->Fill(pa_max);
				}
			}
			if (process == "conv"){
				if ( (index_temp = get_TH1D("conv_pa_pass")) != -1 ){
					vecH1D[index_temp]->Fill(pa_max);
				}
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
