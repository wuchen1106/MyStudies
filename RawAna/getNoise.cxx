#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <math.h>

#include "TString.h"
#include "TRandom.h"
#include "TChain.h"
#include "TGraphErrors.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"

TString MyData = getenv("MYDATA");
TString MyWork = getenv("MYWORKAREA");

void gettopo(int &topo,std::vector<std::string> * process,std::vector<int> * pid,std::vector<double> *o_px,std::vector<double> *o_py,std::vector<double> *o_pz);
void setName(std::string &name);
int gettype(double che_time[128], double sci_time[128],int che_ihit[128], int sci_ihit[128],int &type, int &thehit);
int rotate(int n0, int dn);
double x2t(double x);

int main(int argc, char *argv[]){
	std::stringstream buff;
	double me = 0.511; // MeV
	std::vector<TString> DirName;
	std::vector<int> nProc;
	std::vector<TString> FileNames;
	std::vector<double> RelSizes;
	std::vector<int> RunSeparators;
	std::vector<int> RunTypes;
	std::vector<Long64_t> RunSeparator;
	TRandom random;

	//______________________________________________________________________________________________________
	//// constant
	int cellNo[18];
	int cellNoIntegral[18];
	int count = 0;
	cellNoIntegral[count]=                        198;cellNo[count]=198;count++;
	cellNoIntegral[count]=cellNoIntegral[count-1]+204;cellNo[count]=204;count++;
	cellNoIntegral[count]=cellNoIntegral[count-1]+210;cellNo[count]=210;count++;
	cellNoIntegral[count]=cellNoIntegral[count-1]+216;cellNo[count]=216;count++;
	cellNoIntegral[count]=cellNoIntegral[count-1]+222;cellNo[count]=222;count++;
	cellNoIntegral[count]=cellNoIntegral[count-1]+228;cellNo[count]=228;count++;
	cellNoIntegral[count]=cellNoIntegral[count-1]+234;cellNo[count]=234;count++;
	cellNoIntegral[count]=cellNoIntegral[count-1]+240;cellNo[count]=240;count++;
	cellNoIntegral[count]=cellNoIntegral[count-1]+246;cellNo[count]=246;count++;
	cellNoIntegral[count]=cellNoIntegral[count-1]+252;cellNo[count]=252;count++;
	cellNoIntegral[count]=cellNoIntegral[count-1]+258;cellNo[count]=258;count++;
	cellNoIntegral[count]=cellNoIntegral[count-1]+264;cellNo[count]=264;count++;
	cellNoIntegral[count]=cellNoIntegral[count-1]+270;cellNo[count]=270;count++;
	cellNoIntegral[count]=cellNoIntegral[count-1]+276;cellNo[count]=276;count++;
	cellNoIntegral[count]=cellNoIntegral[count-1]+282;cellNo[count]=282;count++;
	cellNoIntegral[count]=cellNoIntegral[count-1]+288;cellNo[count]=288;count++;
	cellNoIntegral[count]=cellNoIntegral[count-1]+294;cellNo[count]=294;count++;
	cellNoIntegral[count]=cellNoIntegral[count-1]+300;cellNo[count]=300;count++;
	double cdc_length[18] = {0};
	cdc_length[0] = 148.1688151;
	cdc_length[1] = 148.7330614;
	cdc_length[2] = 149.2973078;
	cdc_length[3] = 149.8615541;
	cdc_length[4] = 150.4258005;
	cdc_length[5] = 150.9900468;
	cdc_length[6] = 151.5542931;
	cdc_length[7] = 152.1185395;
	cdc_length[8] = 152.6827858;
	cdc_length[9 ] = 153.2470322;
	cdc_length[10] = 153.8112785;
	cdc_length[11] = 154.3755248;
	cdc_length[12] = 154.9397712;
	cdc_length[13] = 155.5040175;
	cdc_length[14] = 156.0682638;
	cdc_length[15] = 156.6325102;
	cdc_length[16] = 157.1967565;
	cdc_length[17] = 157.7610029;

	// Beam Structure
	double PulseInterval = 1170; // ns
	double dutyFactor = 0.9/2.48;
	double proton_rate = 2.5e12; // Hz
	double left_end = 0; // ns
	double duration = PulseInterval; // ns

	// CDC Info
	double W_He = 41; // eV
	double W_iC4H10 = 23; // eV
	double gain = 1e5; // and  <--- smaller than 1e5 due to space charge? 
	double edep2charge = 1/(0.9*W_He+0.1*W_iC4H10)*gain*1.6e-19*1000; // eV->mC
	double Nbunchperday = 24*3600/PulseInterval/1e-9*dutyFactor;

	//______________________________________________________________________________________________________
	// About this run
	if (argc <= 4) {
		std::cerr<<"Should provide runname, dir/file, size of a beam.all run, input files"<<std::endl;
		return -1;
	}
	TString runName = argv[1];
	TString opt = argv[2];
	int noEntries = (int)strtol(argv[3],NULL,10);
	if (opt=="dir"){
		int ndirs = (argc-4);
		if (ndirs%4!=0){
//			std::cerr<<"Should provide dir name, n jobs, weight, type"<<std::endl;
			return -1;
		}
		ndirs/=4;
		for (int i = 0; i<ndirs; i++){
			DirName.push_back(argv[4+i]);
			nProc.push_back((int)strtol(argv[4+i+ndirs],NULL,10));
			RelSizes.push_back((double)strtof(argv[4+i+ndirs*2],NULL));
			RunTypes.push_back((int)strtol(argv[4+i+ndirs*3],NULL,10));
		}
	}
	else {
		int nfiles = argc-4;
		if (nfiles%3!=0){
//			std::cerr<<"Should provide file name, weight"<<std::endl;
			return -1;
		}
		nfiles/=3;
		for (int i = 0; i<nfiles; i++){
			FileNames.push_back(argv[4+i]);
			RelSizes.push_back((double)strtof(argv[4+i+nfiles],NULL));
			RunTypes.push_back((int)strtol(argv[4+i+nfiles*2],NULL,10));
		}
	}

	//______________________________________________________________________________________________________
	// input
	int  C_nHits = 0;
	int  M_nHits = 0;

	int  evt_num;
	int  run_num;
	double weight = 1;

	std::vector<int>     *C_cellID = 0;
	std::vector<int>     *C_layerID = 0;
	std::vector<int>     *C_tid = 0;
	std::vector<int>     *C_pid = 0;
	std::vector<double>  *C_edep = 0;
	std::vector<double>  *C_stepL = 0;
	std::vector<double>  *C_driftD = 0;
	std::vector<double>  *C_driftDtrue = 0;
	std::vector<double>  *C_tstart = 0;
	std::vector<double>  *C_tstop = 0;
	std::vector<int>     *C_posflag = 0;
	std::vector<int>     *C_nPair = 0;
	std::vector<double>  *C_t = 0;
	std::vector<double>  *C_px = 0;
	std::vector<double>  *C_py = 0;
	std::vector<double>  *C_pz = 0;
	std::vector<double>  *C_x = 0;
	std::vector<double>  *C_y = 0;
	std::vector<double>  *C_z = 0;
	std::vector<double>  *C_wx = 0;
	std::vector<double>  *C_wy = 0;
	std::vector<double>  *C_wz = 0;

	std::vector<int>     *M_volID = 0;
	std::vector<std::string>  *M_volName = 0;
	std::vector<int>     *M_tid = 0;
	std::vector<int>     *M_pid = 0;
	std::vector<double>  *M_edep = 0;
	std::vector<double>  *M_stepL = 0;
	std::vector<double>  *M_t = 0;
	std::vector<double>  *M_px = 0;
	std::vector<double>  *M_py = 0;
	std::vector<double>  *M_pz = 0;
	std::vector<double>  *M_x = 0;
	std::vector<double>  *M_y = 0;
	std::vector<double>  *M_z = 0;

	std::vector<int>     *McTruth_pid = 0;
	std::vector<int>     *McTruth_ppid = 0;
	std::vector<int>     *McTruth_ptid = 0;
	std::vector<int>     *McTruth_tid = 0;
	std::vector<double>  *McTruth_time = 0;
	std::vector<double>  *McTruth_px = 0;
	std::vector<double>  *McTruth_py = 0;
	std::vector<double>  *McTruth_pz = 0;
	std::vector<double>  *McTruth_x = 0;
	std::vector<double>  *McTruth_y = 0;
	std::vector<double>  *McTruth_z = 0;
	std::vector<std::string>  *McTruth_volume = 0;
	std::vector<std::string>  *McTruth_process = 0;
	std::vector<std::string>  *McTruth_particleName = 0;

	TChain *c = new TChain("tree");
//	std::cerr<<"FileNames.size() = "<<(FileNames.size())<<std::endl;
	for (int i = 0; i<FileNames.size(); i++){
//		std::cerr<<"FileNames["<<i<<"] = \""<<FileNames[i]<<"\", "<<RelSizes[i]<<std::endl;
		c->Add(FileNames[i]);
		RunSeparators.push_back(c->GetEntries());
	}
//	std::cerr<<"nProc = "<<nProc.size()<<std::endl;
	int nProc1 = 0;
	for (int iRun = 0; iRun < nProc.size(); iRun++ ){
//		std::cerr<<"DirName["<<iRun<<"] = \""<<DirName[iRun]<<"\", "<<nProc[iRun]<<", "<<RelSizes[iRun]<<std::endl;
		for (int i = 0; i<nProc[iRun]; i++){
			c->Add(Form(MyData+"/"+DirName[iRun]+"/%d_job0.raw",i));
			if (RunTypes[iRun]==1){
				nProc1 = nProc[iRun];
				RunSeparator.push_back(c->GetEntries());
			}
		}
		RunSeparators.push_back(c->GetEntries());
	}

	c->SetBranchAddress("evt_num",&evt_num);
	c->SetBranchAddress("run_num",&run_num);

	c->SetBranchAddress("CdcCell_nHits",&C_nHits);
	c->SetBranchAddress("CdcCell_layerID",&C_layerID);
	c->SetBranchAddress("CdcCell_cellID",&C_cellID);
	c->SetBranchAddress("CdcCell_tid",&C_tid);
	c->SetBranchAddress("CdcCell_pid",&C_pid);
	c->SetBranchAddress("CdcCell_edep",&C_edep);
	c->SetBranchAddress("CdcCell_stepL",&C_stepL);
	c->SetBranchAddress("CdcCell_driftD",&C_driftD);
	c->SetBranchAddress("CdcCell_driftDtrue",&C_driftDtrue);
	c->SetBranchAddress("CdcCell_tstop",&C_tstop);
	c->SetBranchAddress("CdcCell_tstart",&C_tstart);
	c->SetBranchAddress("CdcCell_posflag",&C_posflag);
	c->SetBranchAddress("CdcCell_nPair",&C_nPair);
	c->SetBranchAddress("CdcCell_t",&C_t);
	c->SetBranchAddress("CdcCell_px",&C_px);
	c->SetBranchAddress("CdcCell_py",&C_py);
	c->SetBranchAddress("CdcCell_pz",&C_pz);
	c->SetBranchAddress("CdcCell_x",&C_x);
	c->SetBranchAddress("CdcCell_y",&C_y);
	c->SetBranchAddress("CdcCell_z",&C_z);
	c->SetBranchAddress("CdcCell_wx",&C_wx);
	c->SetBranchAddress("CdcCell_wy",&C_wy);
	c->SetBranchAddress("CdcCell_wz",&C_wz);

	c->SetBranchAddress("M_nHits",&M_nHits);
	c->SetBranchAddress("M_volName",&M_volName);
	c->SetBranchAddress("M_volID",&M_volID);
	c->SetBranchAddress("M_tid",&M_tid);
	c->SetBranchAddress("M_pid",&M_pid);
	c->SetBranchAddress("M_edep",&M_edep);
	c->SetBranchAddress("M_stepL",&M_stepL);
	c->SetBranchAddress("M_t",&M_t);
	c->SetBranchAddress("M_px",&M_px);
	c->SetBranchAddress("M_py",&M_py);
	c->SetBranchAddress("M_pz",&M_pz);
	c->SetBranchAddress("M_x",&M_x);
	c->SetBranchAddress("M_y",&M_y);
	c->SetBranchAddress("M_z",&M_z);

	c->SetBranchAddress("McTruth_pid",&McTruth_pid);
	c->SetBranchAddress("McTruth_ppid",&McTruth_ppid);
	c->SetBranchAddress("McTruth_ptid",&McTruth_ptid);
	c->SetBranchAddress("McTruth_tid",&McTruth_tid);
	c->SetBranchAddress("McTruth_time",&McTruth_time);
	c->SetBranchAddress("McTruth_px",&McTruth_px);
	c->SetBranchAddress("McTruth_py",&McTruth_py);
	c->SetBranchAddress("McTruth_pz",&McTruth_pz);
	c->SetBranchAddress("McTruth_x",&McTruth_x);
	c->SetBranchAddress("McTruth_y",&McTruth_y);
	c->SetBranchAddress("McTruth_z",&McTruth_z);
	c->SetBranchAddress("McTruth_process",&McTruth_process);
	c->SetBranchAddress("McTruth_volume",&McTruth_volume);
	c->SetBranchAddress("McTruth_particleName",&McTruth_particleName);
	c->SetBranchAddress("weight",&weight);

	// about topo
	int topo = -1;
	std::vector<double> *io_t = 0;
	std::vector<double> *io_px = 0;
	std::vector<double> *io_py = 0;
	std::vector<double> *io_pz = 0;
	std::vector<double> *io_x = 0;
	std::vector<double> *io_y = 0;
	std::vector<double> *io_z = 0;
	std::vector<int> *io_dep = 0;
	std::vector<std::string> *iprocess = 0;
	std::vector<std::string> *ivolume = 0;
	std::vector<std::string> *iparticle = 0;
	std::vector<int> *ipid = 0;
	std::vector<int> *ippid = 0;

	TChain * co = new TChain("t","t");
	co->Add("/scratchfs/bes/wuc/MyWorkArea/Simulate/comet/data/CyDet.ALL.150919.W500um.OptD2.1mmCFRP.DD35.151117.root");
	co->SetBranchAddress("topo",&topo);
	co->SetBranchAddress("it",&io_t);
	co->SetBranchAddress("ipx",&io_px);
	co->SetBranchAddress("ipy",&io_py);
	co->SetBranchAddress("ipz",&io_pz);
	co->SetBranchAddress("ix",&io_x);
	co->SetBranchAddress("iy",&io_y);
	co->SetBranchAddress("iz",&io_z);
	co->SetBranchAddress("idep",&io_dep);
	co->SetBranchAddress("process",&iprocess);
	co->SetBranchAddress("volume",&ivolume);
	co->SetBranchAddress("particle",&iparticle);
	co->SetBranchAddress("ipid",&ipid);
	co->SetBranchAddress("ippid",&ippid);

	//______________________________________________________________________________________________________
	// output
	TFile * ofile = new TFile("result/"+runName+".root","RECREATE");
	TTree *tree  = new TTree("t","t");

	// about CTH hits
	int trinHits;
	int type; 
	int tritype; 
	int tripos; 
	double O_t;

	// about CDC hits
	int cdcnHits;
	double pamax;

	// about topo
	std::vector<double> *o_t = 0;
	std::vector<double> *o_px = 0;
	std::vector<double> *o_py = 0;
	std::vector<double> *o_pz = 0;
	std::vector<double> *o_x = 0;
	std::vector<double> *o_y = 0;
	std::vector<double> *o_z = 0;
	std::vector<int> *o_dep = 0;
	std::vector<std::string> *process = 0;
	std::vector<std::string> *volume = 0;
	std::vector<std::string> *particle = 0;
	std::vector<int> *pid = 0;
	std::vector<int> *ppid = 0;

	tree->Branch("evt_num",&evt_num);
	tree->Branch("run_num",&run_num);
	tree->Branch("trinHits",&trinHits);
	tree->Branch("cdcnHits",&cdcnHits);
	tree->Branch("pam",&pamax);
	tree->Branch("type",&type);
	tree->Branch("tritype",&tritype);
	tree->Branch("tripos",&tripos);
	tree->Branch("weight",&weight);
	tree->Branch("mt",&O_t);

	tree->Branch("topo",&topo);
	tree->Branch("t",&o_t);
	tree->Branch("px",&o_px);
	tree->Branch("py",&o_py);
	tree->Branch("pz",&o_pz);
	tree->Branch("x",&o_x);
	tree->Branch("y",&o_y);
	tree->Branch("z",&o_z);
	tree->Branch("dep",&o_dep);
	tree->Branch("process",&process);
	tree->Branch("volume",&volume);
	tree->Branch("particle",&particle);
	tree->Branch("pid",&pid);
	tree->Branch("ppid",&ppid);

	// Statistics
	double sci_time[128];
	double che_time[128];
	int che_ihit[128];
	int sci_ihit[128];
	double nHitsSU = 0;
	double nHitsCU = 0;
	double nHitsSD = 0;
	double nHitsCD = 0;
	double nHitsSUl = 0;
	double nHitsCUl = 0;
	double nHitsSDl = 0;
	double nHitsCDl = 0;
	double edepSU = 0;
	double edepCU = 0;
	double edepSD = 0;
	double edepCD = 0;
	double edepSUl = 0;
	double edepCUl = 0;
	double edepSDl = 0;
	double edepCDl = 0;
	double triggerrate = 0;
	double triggerrate2 = 0;

	std::vector<double> vLayerID;
	std::vector<double> vHitCount;
	std::vector<double> vHitCount2;
	std::vector<double> vCharge;
	std::vector<double> vEx;
	std::vector<double> vEyHitCount;
	std::vector<double> vEyHitCount2;
	std::vector<double> vEyChar;
	std::vector<int> vEntries;
	std::vector<int> vEntries2;
	for (int i = 0; i < 18; i++){
		vLayerID.push_back(i+1);
		vHitCount.push_back(0);
		vHitCount2.push_back(0);
		vCharge.push_back(0);
		vEx.push_back(0);
		vEyHitCount.push_back(0);
		vEyHitCount2.push_back(0);
		vEyChar.push_back(0);
		vEntries.push_back(0);
		vEntries2.push_back(0);
	}

	// histograms
	TH1D* hSU = new TH1D("hSU","hSU",117,0,1170);
	TH1D* hCU = new TH1D("hCU","hCU",117,0,1170);
	TH1D* hSD = new TH1D("hSD","hSD",117,0,1170);
	TH1D* hCD = new TH1D("hCD","hCD",117,0,1170);
	TH1D* hTYPE = new TH1D("hTYPE","hTYPE",85,-5,80);
	TH1D* hTriRate = new TH1D("hTriRate","hTriRate",117,0,1170);
	TH1D* hSUedep = new TH1D("hSUedep","hSUedep",256,0,5000);
	TH1D* hCUedep = new TH1D("hCUedep","hCUedep",256,0,5000);
	TH1D* hSDedep = new TH1D("hSDedep","hSDedep",256,0,5000);
	TH1D* hCDedep = new TH1D("hCDedep","hCDedep",256,0,5000);

    int Color_1 = 632;
    int Color_2 = 800;
    int Color_3 = 880;
    int Color_4 = 600;

    int Style_1 = 20; 
    int Style_2 = 20; 
    int Style_3 = 20; 
    int Style_4 = 20; 
    
    TString xtitle1 = "Signal Arrival Time (ns)";
    TString xtitle2 = "Hit Time (ns)";
    buff.str("");
    buff.clear();
    buff<<"nHits/Bunch/"<<PulseInterval/100<<"ns";
    TString ytitle = buff.str().c_str();

    TString hName1 = "Innermost Layer";
    TString hName2 = "Seccond Innermost Layer";
    TString hName3 = "Seccond Outmost Layer";
    TString hName4 = "Outmost Layer";

	// hit rate in different cdc layers
    TH1D * hCDCt[18];
    TH1D * hCDCtstart[18];
    for ( int i =0; i <18; i++){
    	buff.str("");
    	buff.clear();
    	buff<<"hCDCt"<<i;
		hCDCt[i] = new TH1D(buff.str().c_str(),"Hit Time of Noise in CDC",100,0,PulseInterval);
		hCDCt[i]->GetYaxis()->SetTitle(ytitle);
		hCDCt[i]->GetXaxis()->SetTitle(xtitle1);
		hCDCt[i]->SetMarkerSize(0.5);
		hCDCt[i]->SetMarkerStyle(Style_1);
    	buff.str("");
    	buff.clear();
    	buff<<"hCDCtstart"<<i;
		hCDCtstart[i] = new TH1D(buff.str().c_str(),"Signal Arrival Time of Noise in CDC",100,0,PulseInterval);
		hCDCtstart[i]->GetYaxis()->SetTitle(ytitle);
		hCDCtstart[i]->GetXaxis()->SetTitle(xtitle2);
		hCDCtstart[i]->SetMarkerSize(0.5);
		hCDCtstart[i]->SetMarkerStyle(Style_1);
    }
    hCDCt[0]->SetMarkerStyle(Style_1);
    hCDCt[0]->SetMarkerColor(Color_1);
    hCDCt[0]->SetLineColor(Color_1);
    hCDCt[1]->SetMarkerStyle(Style_2);
    hCDCt[1]->SetMarkerColor(Color_2);
    hCDCt[1]->SetLineColor(Color_2);
    hCDCt[2]->SetMarkerStyle(Style_3);
    hCDCt[2]->SetMarkerColor(Color_3);
    hCDCt[2]->SetLineColor(Color_3);
    hCDCt[3]->SetMarkerStyle(Style_4);
    hCDCt[3]->SetMarkerColor(Color_4);
    hCDCt[3]->SetLineColor(Color_4);
    hCDCtstart[0]->SetMarkerStyle(Style_1);
    hCDCtstart[0]->SetMarkerColor(Color_1);
    hCDCtstart[0]->SetLineColor(Color_1);
    hCDCtstart[1]->SetMarkerStyle(Style_2);
    hCDCtstart[1]->SetMarkerColor(Color_2);
    hCDCtstart[1]->SetLineColor(Color_2);
    hCDCtstart[2]->SetMarkerStyle(Style_3);
    hCDCtstart[2]->SetMarkerColor(Color_3);
    hCDCtstart[2]->SetLineColor(Color_3);
    hCDCtstart[3]->SetMarkerStyle(Style_4);
    hCDCtstart[3]->SetMarkerColor(Color_4);
    hCDCtstart[3]->SetLineColor(Color_4);
    TH1D *hCDCedep[4];
    TH1D *hCDCmom[4];
    for (int i  =0; i<4; i++){
		hCDCedep[i] = new TH1D(Form("hCDCedep.%d",i),"Energy Deposit of Noise Hits in CDC",200,-10,2);
		hCDCedep[i]->GetYaxis()->SetTitle("Count");
		hCDCedep[i]->GetXaxis()->SetTickLength(0);
		hCDCedep[i]->GetXaxis()->SetTitleOffset(3);
		hCDCedep[i]->GetXaxis()->SetLabelOffset(3);
    	if (i==0) hCDCedep[i]->SetLineColor(kBlue);
		else if (i==1) hCDCedep[i]->SetLineColor(kRed);
		else if (i==2) hCDCedep[i]->SetLineColor(kBlack);
		else if (i==3) hCDCedep[i]->SetLineColor(kGreen);
		hCDCmom[i] = new TH1D(Form("hCDCmom.%d",i),"Momentum noise Tracks in CDC",200,-8,4);
		hCDCmom[i]->GetYaxis()->SetTitle("Count");
		hCDCmom[i]->GetXaxis()->SetTickLength(0);
		hCDCmom[i]->GetXaxis()->SetTitleOffset(3);
		hCDCmom[i]->GetXaxis()->SetLabelOffset(3);
    	if (i==0) hCDCmom[i]->SetLineColor(kBlue);
		else if (i==1) hCDCmom[i]->SetLineColor(kRed);
		else if (i==2) hCDCmom[i]->SetLineColor(kBlack);
		else if (i==3) hCDCmom[i]->SetLineColor(kGreen);
    }

	//______________________________________________________________________________________________________
	// Loop in events
	Long64_t nEvents = c->GetEntries();
//	std::cerr<<"nEvents = "<<nEvents<<std::endl;
	int printModulo = 1e4;
	int printModulo2 = printModulo;
	int iRun = 0;
	for (Long64_t iEvent = 0; iEvent < nEvents; iEvent++ ){
//		if (iEvent%printModulo==0) std::cerr<<(double)iEvent/nEvents*100<<" % ..."<<std::endl;
		c->GetEntry(iEvent);

		if (iEvent>RunSeparators[iRun]) iRun++;
		weight = proton_rate*PulseInterval*1e-9/dutyFactor/(1e9*RelSizes[iRun]); // per bunch
		//=====================================================================
		// Get CDC hit rate
		// Get CDC track info
		int layerID_max = 0;
		int cdchit = -1;
		int indexmax = -1;
		pamax = 0;
		cdcnHits = 0;
		for ( int iHit = 0; iHit<C_nHits;iHit++){
			cdcnHits++;
			int layerID = (*C_layerID)[iHit];
			double time = fmod((*C_t)[iHit]+random.Uniform(100)-50,1170);
			double edep = (*C_edep)[iHit]*1e9;
			double driftT = x2t((*C_driftDtrue)[iHit]);
			if (edep>5000)
				vEntries2[layerID]++;
			vEntries[layerID]++;
			vCharge[layerID]+=weight*edep*edep2charge/cdc_length[layerID]*Nbunchperday/cellNo[layerID];
			if (layerID>layerID_max) layerID_max = layerID;
			hCDCt[layerID]->Fill(time,weight);
			double tarrival = time + driftT;
			hCDCtstart[layerID]->Fill(fmod(tarrival,PulseInterval),weight);
			//if (tarrival>left_end&&tarrival<left_end+duration) vHitCount[layerID]+=weight;
			if (1){
				if (edep>5000)
					vHitCount2[layerID]+=weight;
				vHitCount[layerID]+=weight;
			}
			int index = 0;
			if (fabs((*C_pid)[iHit])==11)
				index = 0;
			else if ((*C_pid)[iHit]==2212)
				index = 1;
			else if ((*C_pid)[iHit]>1e6)
				index = 2;
			else
				index = 3;
			double px = (*C_px)[iHit];
			double py = (*C_py)[iHit];
			double pz = (*C_pz)[iHit];
			double pa = sqrt(px*px+py*py+pz*pz);
			if (pamax<pa){
				pamax = pa;
				indexmax = index;
				cdchit = iHit;
			}
			hCDCedep[index]->Fill(log(edep/1e6),weight);
		}
		if (indexmax>=0)
			hCDCmom[indexmax]->Fill(log(pamax),weight);
		//=====================================================================
		// Get CTH hit rate
		// Get CTH track info
		trinHits = 0;
		for (int itri = 0; itri<128; itri++){
			che_time[itri] = -1;
			che_ihit[itri] = -1;
			sci_time[itri] = -1;
			sci_ihit[itri] = -1;
		}
		for ( int iHit = 0; iHit<M_nHits;iHit++){
			if ((*M_volName)[iHit]=="TriCheU"||(*M_volName)[iHit]=="TriCheLU"){
				tritype = 0;
				tripos = -1;
			}
			else if ((*M_volName)[iHit]=="TriCheD"||(*M_volName)[iHit]=="TriCheLD"){
				tritype = 0;
				tripos = 1;
			}
			else if ((*M_volName)[iHit]=="TriSciU"){
				tritype = 1;
				tripos = -1;
			}
			else if ((*M_volName)[iHit]=="TriSciD"){
				tritype = 1;
				tripos = 1;
			}
			else {
				tritype = -1;
				tripos = 0;
			}
			double hittime = (*M_t)[iHit];
			double hittimemod = fmod(hittime,1170);
			if (tritype==0&&fabs((*M_pid)[iHit])==11){
				double pa = sqrt((*M_px)[iHit]*(*M_px)[iHit]+(*M_py)[iHit]*(*M_py)[iHit]+(*M_pz)[iHit]*(*M_pz)[iHit])*1000;
				double beta = pa/sqrt(pa*pa+me*me);
				if (beta>1/1.5){
					che_time[(*M_volID)[iHit]+(tripos+1)*32] = hittime;
					che_ihit[(*M_volID)[iHit]+(tripos+1)*32] = iHit;
					if (tripos>0){
						nHitsCD+=weight;
						edepCD+=(*M_edep)[iHit]*weight;
						if (hittimemod>200){
							nHitsCDl+=weight;
							edepCDl+=(*M_edep)[iHit]*weight;
						}
						hCD->Fill(hittimemod,weight);
						hCDedep->Fill((*M_edep)[iHit]*1e6,weight);
					}
					else {
						nHitsCU+=weight;
						edepCU+=(*M_edep)[iHit]*weight;
						if (hittimemod>200){
							nHitsCUl+=weight;
							edepCUl+=(*M_edep)[iHit]*weight;
						}
						hCU->Fill(hittimemod,weight);
						hCUedep->Fill((*M_edep)[iHit]*1e6,weight);
					}
				}
			}
			else if (tritype==1){
				if ((*M_edep)[iHit]>630e-6){
					sci_time[(*M_volID)[iHit]+(tripos+1)*32] = hittime;
					sci_ihit[(*M_volID)[iHit]+(tripos+1)*32] = iHit;
				}
				if (tripos>0){
					if ((*M_edep)[iHit]>63e-6){
						nHitsSD+=weight;
					}
					edepSD+=(*M_edep)[iHit]*weight;
					if (hittimemod>200){
						if ((*M_edep)[iHit]>63e-6){
							nHitsSDl+=weight;
						}
						edepSDl+=(*M_edep)[iHit]*weight;
					}
					hSD->Fill(hittimemod,weight*(*M_edep)[iHit]);
					hSDedep->Fill((*M_edep)[iHit]*1e6,weight);
				}
				else{
					if ((*M_edep)[iHit]>63e-6){
						nHitsSU+=weight;
					}
					edepSU+=(*M_edep)[iHit]*weight;
					if (hittimemod>200){
						if ((*M_edep)[iHit]>63e-6){
							nHitsSUl+=weight;
						}
						edepSUl+=(*M_edep)[iHit]*weight;
					}
					hSU->Fill(hittimemod,weight*(*M_edep)[iHit]);
					hSUedep->Fill((*M_edep)[iHit]*1e6,weight);
				}
			}
			trinHits++;
		}
		int trihit;
		gettype(che_time,sci_time,che_ihit,sci_ihit,type,trihit);
		if (trihit<0){ // no trigger
			if (M_nHits>0)trihit=0;
		}
		if (trihit>=0){
			O_t = fmod((*M_t)[trihit],1170);
		}
		hTYPE->Fill(type,weight);
		if ((type>=53&&type<=56)||(type>=49&&type<=51)||(type>=73&&type<=77)||(type>=69&&type<=72)){
			hTriRate->Fill(O_t,weight);
			if (O_t>500)
				triggerrate+=weight;
			if (O_t>700)
				triggerrate2+=weight;
		}

		// get topo
		if(o_t) delete o_t; o_t  = new std::vector<double>;
		if(o_px) delete o_px; o_px  = new std::vector<double>;
		if(o_py) delete o_py; o_py  = new std::vector<double>;
		if(o_pz) delete o_pz; o_pz  = new std::vector<double>;
		if(o_x) delete o_x; o_x  = new std::vector<double>;
		if(o_y) delete o_y; o_y  = new std::vector<double>;
		if(o_z) delete o_z; o_z  = new std::vector<double>;
		if(process) delete process; process  = new std::vector<std::string>;
		if(o_dep) delete o_dep; o_dep  = new std::vector<int>;
		if(volume) delete volume; volume  = new std::vector<std::string>;
		if(particle) delete particle; particle  = new std::vector<std::string>;
		if(pid) delete pid; pid  = new std::vector<int>;
		if(ppid) delete ppid; ppid  = new std::vector<int>;
		int tritid = -1; if(trihit>=0) tritid = (*M_tid)[trihit];
		int cdctid = -1; if(cdchit>=0) cdctid = (*C_tid)[cdchit];
		if (tritid==-1&&cdctid==-1) continue;
		int ttid = -1;
		int idepth = 0;
		for(int iMc = McTruth_pid->size()-1; iMc>0; iMc--){ // don't store the first entry
			if (ttid==-1){
				if(tritid==(*McTruth_tid)[iMc]) tritid = (*McTruth_ptid)[iMc];
				if(cdctid==(*McTruth_tid)[iMc]) cdctid = (*McTruth_ptid)[iMc];
				if (tritid==cdctid||tritid==-1||cdctid==-1){// common accestor
					ttid=(*McTruth_ptid)[iMc];
				}
			}
			else if(ttid==(*McTruth_tid)[iMc]){
				ppid->push_back((*McTruth_ppid)[iMc]);
				pid->push_back((*McTruth_pid)[iMc]);
				process->push_back((*McTruth_process)[iMc]);
				particle->push_back((*McTruth_particleName)[iMc]);
				std::string name = (*McTruth_volume)[iMc];
				setName(name);
				volume->push_back(name);
				o_t->push_back((*McTruth_time)[iMc]);
				o_px->push_back((*McTruth_px)[iMc]*1000);
				o_py->push_back((*McTruth_py)[iMc]*1000);
				o_pz->push_back((*McTruth_pz)[iMc]*1000);
				o_x->push_back((*McTruth_x)[iMc]*10);
				o_y->push_back((*McTruth_y)[iMc]*10);
				o_z->push_back((*McTruth_z)[iMc]*10);
				o_dep->push_back(idepth);
				idepth++;
				ttid = (*McTruth_ptid)[iMc];
			}
		}
		if (RunTypes[iRun]==2||RunTypes[iRun]==3){// dio tail or pmc
			ppid->push_back(13);
			process->push_back("muMinusCaptureAtRest");
			pid->push_back((*McTruth_pid)[0]);
			particle->push_back((*McTruth_particleName)[0]);
			volume->push_back((*McTruth_volume)[0]);
			o_t->push_back((*McTruth_time)[0]);
			o_px->push_back((*McTruth_px)[0]*1000);
			o_py->push_back((*McTruth_py)[0]*1000);
			o_pz->push_back((*McTruth_pz)[0]*1000);
			o_x->push_back((*McTruth_x)[0]*10);
			o_y->push_back((*McTruth_y)[0]*10);
			o_z->push_back((*McTruth_z)[0]*10);
			o_dep->push_back(idepth);
			idepth++;
			//
			ppid->push_back(0);
			process->push_back("NULL");
			pid->push_back(13);
			particle->push_back("mu-");
			volume->push_back("A9begin");
			o_t->push_back(0);
			o_px->push_back(0);
			o_py->push_back(0);
			o_pz->push_back(0);
			o_x->push_back(0);
			o_y->push_back(0);
			o_z->push_back(0);
			o_dep->push_back(idepth);
			idepth++;
		}
		else if (RunTypes[iRun]==1){// beam.all
			int ifile = 0;
			for (ifile = 0; ifile<nProc1; ifile++){
				if (RunSeparator[ifile]>iEvent){
					break;
				}
			}
			int ioEntry = ifile*noEntries+evt_num;
			co->GetEntry(ioEntry);
			for (int iipar = 0; iipar<ipid->size(); iipar++){
				ppid->push_back((*ippid)[iipar]);
				pid->push_back((*ipid)[iipar]);
				process->push_back((*iprocess)[iipar]);
				particle->push_back((*iparticle)[iipar]);
				volume->push_back((*ivolume)[iipar]);
				o_t->push_back((*io_t)[iipar]);
				o_px->push_back((*io_px)[iipar]);
				o_py->push_back((*io_py)[iipar]);
				o_pz->push_back((*io_pz)[iipar]);
				o_x->push_back((*io_x)[iipar]);
				o_y->push_back((*io_y)[iipar]);
				o_z->push_back((*io_z)[iipar]);
				o_dep->push_back((*io_dep)[iipar]);
			}
		}
		gettopo(topo,process,pid,o_px,o_py,o_pz);
		tree->Fill(); // stack the previous track
	}
	double maxCha = 0;
	double nHitsCDC = 0;
	double nHitsCDC2 = 0;
	for ( int ilayer  =0; ilayer<18; ilayer++){
		if (vEntries[ilayer]){
			vEyHitCount[ilayer] = sqrt(1./vEntries[ilayer])*vHitCount[ilayer];
			vEyChar[ilayer] = sqrt(1./vEntries[ilayer])*vCharge[ilayer];
		}
		if (vEntries2[ilayer]){
			vEyHitCount2[ilayer] = sqrt(1./vEntries2[ilayer])*vHitCount2[ilayer];
		}
		if (maxCha<vCharge[ilayer]) maxCha = vCharge[ilayer];
		nHitsCDC2+=vHitCount2[ilayer];
		nHitsCDC+=vHitCount[ilayer];
	}

	TGraphErrors * g0 = new TGraphErrors(vLayerID.size(),&(vLayerID[0]),&(vHitCount[0]),&(vEx[0]),&(vEyHitCount[0]));
	buff.str("");
	buff.clear();
	//buff<<"nHits per Bunch in Each Layer in Measure Window ("<<left_end<<","<<left_end+duration<<")ns";
	buff<<"nHits per Bunch in Each Layer (before ADC cut)";
	g0->SetTitle(buff.str().c_str());
	g0->GetHistogram()->GetXaxis()->SetTitle("Layer ID (1-18)");
	g0->GetHistogram()->GetYaxis()->SetTitle("nHits/Bunch");
	g0->SetName("g0");
	TGraphErrors * g1 = new TGraphErrors(vLayerID.size(),&(vLayerID[0]),&(vHitCount2[0]),&(vEx[0]),&(vEyHitCount[0]));
	buff.str("");
	buff.clear();
	//buff<<"nHits per Bunch in Each Layer in Measure Window ("<<left_end<<","<<left_end+duration<<")ns";
	buff<<"nHits per Bunch in Each Layer (after ADC cut)";
	g1->SetTitle(buff.str().c_str());
	g1->GetHistogram()->GetXaxis()->SetTitle("Layer ID (1-18)");
	g1->GetHistogram()->GetYaxis()->SetTitle("nHits/Bunch");
	g1->SetName("g1");
	TGraphErrors * g2 = new TGraphErrors(vLayerID.size(),&(vLayerID[0]),&(vCharge[0]),&(vEx[0]),&(vEyChar[0]));
	g2->GetHistogram()->GetXaxis()->SetTitle("Layer ID (1-18)");
	g2->GetHistogram()->GetYaxis()->SetTitle("Q/wire");
	g2->SetTitle("Charge Accumulating Speed in Each Layer Q:(mC/cm/day/wire)");
	g2->SetName("g2");

	hCU->Write();
	hCD->Write();
	hSU->Write();
	hSD->Write();
	hCUedep->Write();
	hCDedep->Write();
	hSUedep->Write();
	hSDedep->Write();
	hTriRate->Write();
	hTYPE->Write();
	for (int i = 0; i<4; i++){
		hCDCtstart[i]->Write();
		hCDCt[i]->Write();
		hCDCedep[i]->Write();
		hCDCmom[i]->Write();
	}
	g0->Write();
	g1->Write();
	g2->Write();
	tree->Write();
	ofile->Close();
	std::cout<<(nHitsCU-nHitsCUl)/64./(PulseInterval*1.e-6)<<" "<<nHitsCUl/64./(PulseInterval*1.e-6)
		<<" "<<(nHitsCD-nHitsCDl)/64./(PulseInterval*1.e-6)<<" "<<nHitsCDl/64./(PulseInterval*1.e-6)
		<<" "<<(nHitsSU-nHitsSUl)/64./(PulseInterval*1.e-6)<<" "<<nHitsSUl/64./(PulseInterval*1.e-6)
		<<" "<<(nHitsSD-nHitsSDl)/64./(PulseInterval*1.e-6)<<" "<<nHitsSDl/64./(PulseInterval*1.e-6)
		<<" "<<(edepSU-edepSUl)/64./(PulseInterval*1.e-6)<<" "<<edepSUl/64./(PulseInterval*1.e-6)
		<<" "<<(edepSD-edepSDl)/64./(PulseInterval*1.e-6)<<" "<<edepSDl/64./(PulseInterval*1.e-6)
		<<" "<<triggerrate/(PulseInterval*1.e-6)<<" "<<triggerrate2/(PulseInterval*1.e-6)
		<<" "<<nHitsCDC<<" "<<nHitsCDC2<<" "<<maxCha<<std::endl;
	return 0;
}

int rotate(int n0, int dn){
	int n1 = n0+dn;
	if (n0<64&&n1>=64) n1-=64;
	if (n0>=64&&n1<64) n1+=64;
	if (n1>=128) n1-=64;
	if (n1<0) n1+=64;
	return n1;
}

int gettype(double che_time[128], double sci_time[128],int che_ihit[128], int sci_ihit[128],int &type, int &thehit){
	// option D
	type = 0;
	thehit = -1;
	std::vector<int> vtype;
	std::vector<int> vihit;
	for (int itri = 0; itri<128; itri++){
		double time = sci_time[itri];
		if (time==-1) continue;
		// how many hit in sci in row? (starting from this counter)
		int nsci = 1;
		for (int delta = 1; delta<=2; delta++){
			int jtri = rotate(itri,delta);
			if (sci_time[jtri]!=-1&&fabs(sci_time[jtri]-time)<10) nsci++;
			else break;
		}
		//			std::cout<<itri<<"-"<<nsci<<"->"<<rotate(itri,nsci)<<std::endl;
		for (int delta = -2; delta<=2; delta++){
			int jtri = rotate(itri,delta);
			if (che_time[jtri]==-1||fabs(che_time[jtri]-time)>=10) continue;
			int nche = 1;
			for (int delta = 1; delta<=2; delta++){
				int ktri = rotate(jtri,delta);
				if (che_time[ktri]!=1&&fabs(che_time[ktri]-time)<10) nche++;
				else break;
			}
			//				std::cout<<"  "<<jtri<<"-"<<nche<<"->"<<rotate(jtri,nche)<<std::endl;
			//				std::cout<<"  "<<nsci*100+nche*10+delta<<std::endl;
			vtype.push_back(nsci*20+nche*5+delta);
			vihit.push_back(sci_ihit[itri]);
		}
	}
	int nhitmax = -1;
	for (int it = 0; it<vtype.size(); it++){
		int nhit = (vtype[it]+2)/5+(vtype[it]+2)/20;
		if (nhitmax<nhit) nhitmax = nhit;
	}
	int ndeltamin = 10;
	for (int it = 0; it<vtype.size(); it++){
		if (nhitmax!=(vtype[it]+2)/5+(vtype[it]+2)/20) continue;
		if (fabs(ndeltamin)>fabs(vtype[it]%5)){
			ndeltamin = vtype[it]%10;
			thehit = vihit[it];
			type = vtype[it];
		}
	}
	return 0;
}

void setName(std::string &name){
	if (name.substr(0,7)=="CdcCell") name="CdcCell";
	else if (name.substr(0,8)=="EndPlate") name="EndPlate";
	else if (name.substr(0,12)=="CdcFieldWire") name="CdcFieldWire";
	else if (name.substr(0,13)=="CdcSignalWire") name="CdcSignalWire";
	else if (name.substr(0,6)=="TriChe") name="TriChe";
	else if (name.substr(0,6)=="TriSci") name="TriSci";
	else if (name.substr(0,7)=="TriSciL") name="TriSciL";
	else if (name.substr(0,7)=="TriSciP") name="TriSciP";
	else if (name.substr(0,7)=="TriCheP") name="TriCheP";
	else if (name.substr(0,4)=="Yoke") name="Yoke";
	else if (name=="CrCoTS23_I2_inner") name="I2inner";
	else if (name=="CrCoTS23_I3_inner") name="I3inner";
	else if (name=="CrWaTSBS_I4_inner") name="I4inner";
	else if (name=="CrWaTSBS_I5_inner") name="I5inner";
	else if (name.substr(0,4)=="CrWa") name="CrWa";
	else if (name.substr(0,4)=="CrCo") name="CrCo";
	else if (name.substr(0,4)=="Coil") name="Coil";
	else if (name.substr(0,5)=="CoSup") name="CoSup";
	else if (name.substr(0,7)=="Blocker") name="Blocker";
	else if (name.substr(0,9)=="TS3Window") name="TS3Window";
}

void gettopo(int &topo,std::vector<std::string> * process,std::vector<int> * pid,std::vector<double> *o_px,std::vector<double> *o_py,std::vector<double> *o_pz){
	if (pid->size()<=1) topo = 0; // from primary
	else if ((*pid)[pid->size()-1]==2112) topo = -1; // from neutron
	else if ((*pid)[pid->size()-1]==-211) topo = -2; // from pion
	else if ((*pid)[pid->size()-1]==13){ // from muon
		if ((*process)[process->size()-2]=="muMinusCaptureAtRest"){
			topo=1000;
			if (pid->size()==2){ // directly from muon
				topo += 0;
			}
			else{ // indirectly from muon
				if ((*pid)[pid->size()-2]==22) topo += 1;
				else if ((*pid)[pid->size()-2]==2212) topo += 2;
				else if ((*pid)[pid->size()-2]==11){
					double px_temp = (*o_px)[o_px->size()-2];
					double py_temp = (*o_py)[o_py->size()-2];
					double pz_temp = (*o_pz)[o_pz->size()-2];
					double pa_temp=sqrt(px_temp*px_temp+py_temp*py_temp+pz_temp*pz_temp);
					if (pa_temp>1) topo += 3;
					else topo += 4;
				}
				else if ((*pid)[pid->size()-2]==2112) topo += 5;
				else topo += 6;
			}
		}
		else if ((*process)[process->size()-2]=="Decay"){
			topo=2000;
		}
		else if ((*process)[process->size()-2]=="muIoni"){
			topo=3000;
		}
	}
	else topo = 1; // from other particles
	return;
}

double x2t(double x){
	return x/0.8*350;
}
