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

// Fixed size dimensions of array or collections stored in the TTree if any.
const Int_t kMaxptacs_entry = 404;
const Int_t kMaxptacs_target = 63;
const Int_t kMaxptacs_shielding = 45;
const Int_t kMaxptacs_MARS = 19;
const Int_t kMaxptacs_proton_target_downstream = 22;
const Int_t kMaxptacs_proton_target_upstream = 52;
const Int_t kMaxptacs_proton_target_cylinder = 6337;
const Int_t kMaxptacs_beampipe = 2276;
const Int_t kMaxptacs_exit = 6559;
const Int_t kMaxmt1_entry = 13;
const Int_t kMaxmt1_22_5 = 2050184;
const Int_t kMaxmt1_45 = 792107;
const Int_t kMaxmt1_64_5 = 1547394;
const Int_t kMaxmt1_exit = 109909;
const Int_t kMaxblt0 = 52000;
const Int_t kMaxblt1 = 15;
const Int_t kMaxts2_0 = 16;
const Int_t kMaxts2_1 = 9;
const Int_t kMaxts2_tgt = 18;
const Int_t kMaxcdc_pre = 231;
const Int_t kMaxcdc_trigcount = 108;
const Int_t kMaxcdc_innerwall = 89;

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
	Int_t           ts2_0_;
	UInt_t          ts2_0_fUniqueID[kMaxts2_0];   //[ts2_0_]
	UInt_t          ts2_0_fBits[kMaxts2_0];   //[ts2_0_]
	Int_t           ts2_0_trackID[kMaxts2_0];   //[ts2_0_]
	Int_t           ts2_0_parentID[kMaxts2_0];   //[ts2_0_]
	Int_t           ts2_0_PDGEncoding[kMaxts2_0];   //[ts2_0_]
	Int_t           ts2_0_parentPDGEncoding[kMaxts2_0];   //[ts2_0_]
	Int_t           ts2_0_word_padding[kMaxts2_0];   //[ts2_0_]
	UInt_t          ts2_0_position_fUniqueID[kMaxts2_0];   //[ts2_0_]
	UInt_t          ts2_0_position_fBits[kMaxts2_0];   //[ts2_0_]
	Double_t        ts2_0_position_fX[kMaxts2_0];   //[ts2_0_]
	Double_t        ts2_0_position_fY[kMaxts2_0];   //[ts2_0_]
	Double_t        ts2_0_position_fZ[kMaxts2_0];   //[ts2_0_]
	UInt_t          ts2_0_origin_fUniqueID[kMaxts2_0];   //[ts2_0_]
	UInt_t          ts2_0_origin_fBits[kMaxts2_0];   //[ts2_0_]
	Double_t        ts2_0_origin_fX[kMaxts2_0];   //[ts2_0_]
	Double_t        ts2_0_origin_fY[kMaxts2_0];   //[ts2_0_]
	Double_t        ts2_0_origin_fZ[kMaxts2_0];   //[ts2_0_]
	UInt_t          ts2_0_direction_fUniqueID[kMaxts2_0];   //[ts2_0_]
	UInt_t          ts2_0_direction_fBits[kMaxts2_0];   //[ts2_0_]
	Double_t        ts2_0_direction_fX[kMaxts2_0];   //[ts2_0_]
	Double_t        ts2_0_direction_fY[kMaxts2_0];   //[ts2_0_]
	Double_t        ts2_0_direction_fZ[kMaxts2_0];   //[ts2_0_]
	Double_t        ts2_0_globalTime[kMaxts2_0];   //[ts2_0_]
	Double_t        ts2_0_localTime[kMaxts2_0];   //[ts2_0_]
	Double_t        ts2_0_properTime[kMaxts2_0];   //[ts2_0_]
	Double_t        ts2_0_totalEnergy[kMaxts2_0];   //[ts2_0_]
	Double_t        ts2_0_kineticEnergy[kMaxts2_0];   //[ts2_0_]
	Double_t        ts2_0_momentum[kMaxts2_0];   //[ts2_0_]
	Double_t        ts2_0_mass[kMaxts2_0];   //[ts2_0_]

   TBranch        *b_ts2_0_;   //!
   TBranch        *b_ts2_0_fUniqueID;   //!
   TBranch        *b_ts2_0_fBits;   //!
   TBranch        *b_ts2_0_trackID;   //!
   TBranch        *b_ts2_0_parentID;   //!
   TBranch        *b_ts2_0_PDGEncoding;   //!
   TBranch        *b_ts2_0_parentPDGEncoding;   //!
   TBranch        *b_ts2_0_word_padding;   //!
   TBranch        *b_ts2_0_position_fUniqueID;   //!
   TBranch        *b_ts2_0_position_fBits;   //!
   TBranch        *b_ts2_0_position_fX;   //!
   TBranch        *b_ts2_0_position_fY;   //!
   TBranch        *b_ts2_0_position_fZ;   //!
   TBranch        *b_ts2_0_origin_fUniqueID;   //!
   TBranch        *b_ts2_0_origin_fBits;   //!
   TBranch        *b_ts2_0_origin_fX;   //!
   TBranch        *b_ts2_0_origin_fY;   //!
   TBranch        *b_ts2_0_origin_fZ;   //!
   TBranch        *b_ts2_0_direction_fUniqueID;   //!
   TBranch        *b_ts2_0_direction_fBits;   //!
   TBranch        *b_ts2_0_direction_fX;   //!
   TBranch        *b_ts2_0_direction_fY;   //!
   TBranch        *b_ts2_0_direction_fZ;   //!
   TBranch        *b_ts2_0_globalTime;   //!
   TBranch        *b_ts2_0_localTime;   //!
   TBranch        *b_ts2_0_properTime;   //!
   TBranch        *b_ts2_0_totalEnergy;   //!
   TBranch        *b_ts2_0_kineticEnergy;   //!
   TBranch        *b_ts2_0_momentum;   //!
   TBranch        *b_ts2_0_mass;   //!

   m_TChain->SetBranchAddress("ts2_0", &ts2_0_, &b_ts2_0_);
   m_TChain->SetBranchAddress("ts2_0.fUniqueID", ts2_0_fUniqueID, &b_ts2_0_fUniqueID);
   m_TChain->SetBranchAddress("ts2_0.fBits", ts2_0_fBits, &b_ts2_0_fBits);
   m_TChain->SetBranchAddress("ts2_0.trackID", ts2_0_trackID, &b_ts2_0_trackID);
   m_TChain->SetBranchAddress("ts2_0.parentID", ts2_0_parentID, &b_ts2_0_parentID);
   m_TChain->SetBranchAddress("ts2_0.PDGEncoding", ts2_0_PDGEncoding, &b_ts2_0_PDGEncoding);
   m_TChain->SetBranchAddress("ts2_0.parentPDGEncoding", ts2_0_parentPDGEncoding, &b_ts2_0_parentPDGEncoding);
   m_TChain->SetBranchAddress("ts2_0.word_padding", ts2_0_word_padding, &b_ts2_0_word_padding);
   m_TChain->SetBranchAddress("ts2_0.position.fUniqueID", ts2_0_position_fUniqueID, &b_ts2_0_position_fUniqueID);
   m_TChain->SetBranchAddress("ts2_0.position.fBits", ts2_0_position_fBits, &b_ts2_0_position_fBits);
   m_TChain->SetBranchAddress("ts2_0.position.fX", ts2_0_position_fX, &b_ts2_0_position_fX);
   m_TChain->SetBranchAddress("ts2_0.position.fY", ts2_0_position_fY, &b_ts2_0_position_fY);
   m_TChain->SetBranchAddress("ts2_0.position.fZ", ts2_0_position_fZ, &b_ts2_0_position_fZ);
   m_TChain->SetBranchAddress("ts2_0.origin.fUniqueID", ts2_0_origin_fUniqueID, &b_ts2_0_origin_fUniqueID);
   m_TChain->SetBranchAddress("ts2_0.origin.fBits", ts2_0_origin_fBits, &b_ts2_0_origin_fBits);
   m_TChain->SetBranchAddress("ts2_0.origin.fX", ts2_0_origin_fX, &b_ts2_0_origin_fX);
   m_TChain->SetBranchAddress("ts2_0.origin.fY", ts2_0_origin_fY, &b_ts2_0_origin_fY);
   m_TChain->SetBranchAddress("ts2_0.origin.fZ", ts2_0_origin_fZ, &b_ts2_0_origin_fZ);
   m_TChain->SetBranchAddress("ts2_0.direction.fUniqueID", ts2_0_direction_fUniqueID, &b_ts2_0_direction_fUniqueID);
   m_TChain->SetBranchAddress("ts2_0.direction.fBits", ts2_0_direction_fBits, &b_ts2_0_direction_fBits);
   m_TChain->SetBranchAddress("ts2_0.direction.fX", ts2_0_direction_fX, &b_ts2_0_direction_fX);
   m_TChain->SetBranchAddress("ts2_0.direction.fY", ts2_0_direction_fY, &b_ts2_0_direction_fY);
   m_TChain->SetBranchAddress("ts2_0.direction.fZ", ts2_0_direction_fZ, &b_ts2_0_direction_fZ);
   m_TChain->SetBranchAddress("ts2_0.globalTime", ts2_0_globalTime, &b_ts2_0_globalTime);
   m_TChain->SetBranchAddress("ts2_0.localTime", ts2_0_localTime, &b_ts2_0_localTime);
   m_TChain->SetBranchAddress("ts2_0.properTime", ts2_0_properTime, &b_ts2_0_properTime);
   m_TChain->SetBranchAddress("ts2_0.totalEnergy", ts2_0_totalEnergy, &b_ts2_0_totalEnergy);
   m_TChain->SetBranchAddress("ts2_0.kineticEnergy", ts2_0_kineticEnergy, &b_ts2_0_kineticEnergy);
   m_TChain->SetBranchAddress("ts2_0.momentum", ts2_0_momentum, &b_ts2_0_momentum);
   m_TChain->SetBranchAddress("ts2_0.mass", ts2_0_mass, &b_ts2_0_mass);

	//=======================================================================================================
	//************DO THE DIRTY WORK*******************
	if (verbose >= Verbose_SectorInfo) std::cout<<prefix_SectorInfo<<"In DO THE DIRTY WORK ###"<<std::endl;

	if (!strcmp(m_workMode,"gen")){
		Long64_t nEvent = m_TChain->GetEntries();
		for( Long64_t iEvent = 0; iEvent < nEvent; iEvent++ ){
			if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"In Event "<<iEvent<<std::endl;
			N0++;
			Long64_t tentry = m_TChain->LoadTree(iEvent);
			if (b_ts2_0_) b_ts2_0_->GetEntry(tentry);
			if (b_ts2_0_fUniqueID) b_ts2_0_fUniqueID->GetEntry(tentry);
			if (b_ts2_0_fBits) b_ts2_0_fBits->GetEntry(tentry);
			if (b_ts2_0_trackID) b_ts2_0_trackID->GetEntry(tentry);
			if (b_ts2_0_parentID) b_ts2_0_parentID->GetEntry(tentry);
			if (b_ts2_0_PDGEncoding) b_ts2_0_PDGEncoding->GetEntry(tentry);
			if (b_ts2_0_parentPDGEncoding) b_ts2_0_parentPDGEncoding->GetEntry(tentry);
			if (b_ts2_0_word_padding) b_ts2_0_word_padding->GetEntry(tentry);
			if (b_ts2_0_position_fUniqueID) b_ts2_0_position_fUniqueID->GetEntry(tentry);
			if (b_ts2_0_position_fBits) b_ts2_0_position_fBits->GetEntry(tentry);
			if (b_ts2_0_position_fX) b_ts2_0_position_fX->GetEntry(tentry);
			if (b_ts2_0_position_fY) b_ts2_0_position_fY->GetEntry(tentry);
			if (b_ts2_0_position_fZ) b_ts2_0_position_fZ->GetEntry(tentry);
			if (b_ts2_0_origin_fUniqueID) b_ts2_0_origin_fUniqueID->GetEntry(tentry);
			if (b_ts2_0_origin_fBits) b_ts2_0_origin_fBits->GetEntry(tentry);
			if (b_ts2_0_origin_fX) b_ts2_0_origin_fX->GetEntry(tentry);
			if (b_ts2_0_origin_fY) b_ts2_0_origin_fY->GetEntry(tentry);
			if (b_ts2_0_origin_fZ) b_ts2_0_origin_fZ->GetEntry(tentry);
			if (b_ts2_0_direction_fUniqueID) b_ts2_0_direction_fUniqueID->GetEntry(tentry);
			if (b_ts2_0_direction_fBits) b_ts2_0_direction_fBits->GetEntry(tentry);
			if (b_ts2_0_direction_fX) b_ts2_0_direction_fX->GetEntry(tentry);
			if (b_ts2_0_direction_fY) b_ts2_0_direction_fY->GetEntry(tentry);
			if (b_ts2_0_direction_fZ) b_ts2_0_direction_fZ->GetEntry(tentry);
			if (b_ts2_0_globalTime) b_ts2_0_globalTime->GetEntry(tentry);
			if (b_ts2_0_localTime) b_ts2_0_localTime->GetEntry(tentry);
			if (b_ts2_0_properTime) b_ts2_0_properTime->GetEntry(tentry);
			if (b_ts2_0_totalEnergy) b_ts2_0_totalEnergy->GetEntry(tentry);
			if (b_ts2_0_kineticEnergy) b_ts2_0_kineticEnergy->GetEntry(tentry);
			if (b_ts2_0_momentum) b_ts2_0_momentum->GetEntry(tentry);
			if (b_ts2_0_mass) b_ts2_0_mass->GetEntry(tentry);

			m_TChain->GetEntry(iEvent);
			if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Got entries"<<std::endl;
			if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart
			                                                                      <<"nhits = "<<ts2_0_<<std::endl;

			// Found Muon from upstream?
			// Found Electron from upstream?

			//if ( (index_temp = get_TH1D("relz")) != -1 ){
			//	vecH1D[index_temp]->Fill(hit_relZ*10); //cm -> mm
			//}

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
