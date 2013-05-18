#include <iostream>
#include <sstream>

#include <cstdlib>
#include <math.h>

#include "Randomize.hh"

#include "TChain.h"
#include "TBranch.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "Math/DistFunc.h"

using namespace std;

int main(int argc, char** argv){

	clock_t t_START = clock();
	//##########################PRESET############################
	stringstream buff;

	//histogram
	vector<TString> nameForH2D;
	vector<TString> titleForH2D;
	vector<int>     bin1ForH2D;
	vector<double>  left1ForH2D;
	vector<double>  right1ForH2D;
	vector<int>     bin2ForH2D;
	vector<double>  left2ForH2D;
	vector<double>  right2ForH2D;
	vector<TH2D*>   vecH2D;

	vector<TString> nameForH1D;
	vector<TString> titleForH1D;
	vector<int>     bin1ForH1D;
	vector<double>  left1ForH1D;
	vector<double>  right1ForH1D;
	vector<TH1D*>   vecH1D;

	//=>About Constant
	double PI = 3.141592653589793238;
	double FSC = 1/137.03599911; //fine structure constant
	double M_MUON = 105.6584; //mass of muon in MeV
	double M_ELE = 0.510999; //mass of electron in MeV
	double M_U = 931.494061; //atomic mass unit in MeV

	//=>About Histogram
	int num_bin = 100; //How many bins do you want
	if (argc >= 2){
		buff.str("");
		buff.clear();
		buff<<argv[1];
		buff>>num_bin;
	}
	int num_hists = 10; //How many bins do you want
	if (argc >= 3){
		buff.str("");
		buff.clear();
		buff<<argv[2];
		buff>>num_hists;
	}

	//Read the root file
  std::string tree_name = "tree";
  std::string filename = "raw_g4sim";
  std::string filepath = "/home/chen/g4sim/output";
  buff.clear();
  buff.str("");
  buff<<filepath<<"/"<<filename<<".root";
  TChain* m_TChain = new TChain(tree_name.c_str());
	std::cout<<"Reading \""<<buff.str()<<"\""<<std::endl;
  m_TChain->Add(buff.str().c_str());
//  int num_proc = 8;
//  int num = 1;
//  std::stringstream buff;
//  for ( int i = 0; i < num; i++ ){
//    for ( int i_proc = 0; i_proc < num_proc; i_proc++ ){
//      buff<<filepath<<"/"<<i_proc<<"_job"<<i<<filename<<".raw";
//      m_TChain->Add(buff.str().c_str());
//  //    std::cout<<buff.str()<<std::endl;
//      buff.clear();
//      buff.str("");
//    }
//  }
  int evt_num;
  int run_num;
  int McTruth_nTracks;
  vector<double>* McTruth_x = 0;
  vector<double>* McTruth_y = 0;
  vector<double>* McTruth_z = 0;
  vector<double>* McTruth_time = 0;
  vector<double>* McTruth_px = 0;
  vector<double>* McTruth_py = 0;
  vector<double>* McTruth_pz = 0;
  vector<int>* McTruth_charge = 0;
  vector<int>* McTruth_pid = 0;
  vector<int>* McTruth_tid = 0;
  vector<int>* McTruth_ptid = 0;
  vector<string>* McTruth_process = 0;
  vector<string>* McTruth_volume = 0;

  TBranch* bMcTruth_x = 0;
  TBranch* bMcTruth_y = 0;
  TBranch* bMcTruth_z = 0;
  TBranch* bMcTruth_time = 0;
  TBranch* bMcTruth_px = 0;
  TBranch* bMcTruth_py = 0;
  TBranch* bMcTruth_pz = 0;
  TBranch* bMcTruth_charge = 0;
  TBranch* bMcTruth_pid = 0;
  TBranch* bMcTruth_tid = 0;
  TBranch* bMcTruth_ptid = 0;
  TBranch* bMcTruth_process = 0;
  TBranch* bMcTruth_volume = 0;

	int ProcessCounting_nSteps;
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
	std::vector<double> *ProcessCounting_dTheta = 0;
	std::vector<double> *ProcessCounting_dE = 0;
	std::vector<double> *ProcessCounting_edepT = 0;
	std::vector<double> *ProcessCounting_edepN = 0;
	std::vector<double> *ProcessCounting_edepI = 0;
	std::vector<double> *ProcessCounting_e = 0;
	std::vector<double> *ProcessCounting_preX = 0;
	std::vector<double> *ProcessCounting_preY = 0;
	std::vector<double> *ProcessCounting_preZ = 0;
	std::vector<double> *ProcessCounting_postX = 0;
	std::vector<double> *ProcessCounting_postY = 0;
	std::vector<double> *ProcessCounting_postZ = 0;
	std::vector<std::string> *ProcessCounting_particleName = 0;
	std::vector<int> *ProcessCounting_charge = 0;
	std::vector<std::string> *ProcessCounting_process = 0;
	std::vector<int> *ProcessCounting_ASDI_msc = 0;
	std::vector<int> *ProcessCounting_ASDI_eBrem = 0;
	std::vector<int> *ProcessCounting_ASDI_eIoni = 0;
	std::vector<int> *ProcessCounting_PSDI_eBrem = 0;
	std::vector<int> *ProcessCounting_PSDI_eIoni = 0;
	std::vector<int> *ProcessCounting_PSDI_msc = 0;
	std::vector<std::string> *ProcessCounting_volume = 0;

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
	TBranch *bProcessCounting_dTheta = 0;
	TBranch *bProcessCounting_dE = 0;
	TBranch *bProcessCounting_edepT = 0;
	TBranch *bProcessCounting_edepN = 0;
	TBranch *bProcessCounting_edepI = 0;
	TBranch *bProcessCounting_e = 0;
	TBranch *bProcessCounting_preX = 0;
	TBranch *bProcessCounting_preY = 0;
	TBranch *bProcessCounting_preZ = 0;
	TBranch *bProcessCounting_postX = 0;
	TBranch *bProcessCounting_postY = 0;
	TBranch *bProcessCounting_postZ = 0;
	TBranch *bProcessCounting_particleName = 0;
	TBranch *bProcessCounting_charge = 0;
	TBranch *bProcessCounting_process = 0;
	TBranch *bProcessCounting_ASDI_msc = 0;
	TBranch *bProcessCounting_ASDI_eBrem = 0;
	TBranch *bProcessCounting_ASDI_eIoni = 0;
	TBranch *bProcessCounting_PSDI_eBrem = 0;
	TBranch *bProcessCounting_PSDI_eIoni = 0;
	TBranch *bProcessCounting_PSDI_msc = 0;
	TBranch *bProcessCounting_volume = 0;

  m_TChain->SetBranchAddress("evt_num", &evt_num);
  m_TChain->SetBranchAddress("run_num", &run_num);

  m_TChain->SetBranchAddress("McTruth_nTracks", &McTruth_nTracks);
  m_TChain->SetBranchAddress("McTruth_x", &McTruth_x, &bMcTruth_x);
  m_TChain->SetBranchAddress("McTruth_y", &McTruth_y, &bMcTruth_y);
  m_TChain->SetBranchAddress("McTruth_z", &McTruth_z, &bMcTruth_z);
  m_TChain->SetBranchAddress("McTruth_time", &McTruth_time, &bMcTruth_time);
  m_TChain->SetBranchAddress("McTruth_px", &McTruth_px, &bMcTruth_px);
  m_TChain->SetBranchAddress("McTruth_py", &McTruth_py, &bMcTruth_py);
  m_TChain->SetBranchAddress("McTruth_pz", &McTruth_pz, &bMcTruth_pz);
  m_TChain->SetBranchAddress("McTruth_charge", &McTruth_charge, &bMcTruth_charge);
  m_TChain->SetBranchAddress("McTruth_pid", &McTruth_pid, &bMcTruth_pid);
  m_TChain->SetBranchAddress("McTruth_tid", &McTruth_tid, &bMcTruth_tid);
  m_TChain->SetBranchAddress("McTruth_ptid", &McTruth_ptid, &bMcTruth_ptid);
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
	m_TChain->SetBranchAddress("ProcessCounting_dTheta", &ProcessCounting_dTheta, &bProcessCounting_dTheta);
	m_TChain->SetBranchAddress("ProcessCounting_dE", &ProcessCounting_dE, &bProcessCounting_dE);
	m_TChain->SetBranchAddress("ProcessCounting_edepT", &ProcessCounting_edepT, &bProcessCounting_edepT);
	m_TChain->SetBranchAddress("ProcessCounting_edepN", &ProcessCounting_edepN, &bProcessCounting_edepN);
	m_TChain->SetBranchAddress("ProcessCounting_edepI", &ProcessCounting_edepI, &bProcessCounting_edepI);
	m_TChain->SetBranchAddress("ProcessCounting_e", &ProcessCounting_e, &bProcessCounting_e);
	m_TChain->SetBranchAddress("ProcessCounting_preX", &ProcessCounting_preX, &bProcessCounting_preX);
	m_TChain->SetBranchAddress("ProcessCounting_preY", &ProcessCounting_preY, &bProcessCounting_preY);
	m_TChain->SetBranchAddress("ProcessCounting_preZ", &ProcessCounting_preZ, &bProcessCounting_preZ);
	m_TChain->SetBranchAddress("ProcessCounting_postX", &ProcessCounting_postX, &bProcessCounting_postX);
	m_TChain->SetBranchAddress("ProcessCounting_postY", &ProcessCounting_postY, &bProcessCounting_postY);
	m_TChain->SetBranchAddress("ProcessCounting_postZ", &ProcessCounting_postZ, &bProcessCounting_postZ);
	m_TChain->SetBranchAddress("ProcessCounting_particleName", &ProcessCounting_particleName, &bProcessCounting_particleName);
	m_TChain->SetBranchAddress("ProcessCounting_charge", &ProcessCounting_charge, &bProcessCounting_charge);
	m_TChain->SetBranchAddress("ProcessCounting_process", &ProcessCounting_process, &bProcessCounting_process);
	m_TChain->SetBranchAddress("ProcessCounting_ASDI_msc", &ProcessCounting_ASDI_msc, &bProcessCounting_ASDI_msc);
	m_TChain->SetBranchAddress("ProcessCounting_ASDI_eBrem", &ProcessCounting_ASDI_eBrem, &bProcessCounting_ASDI_eBrem);
	m_TChain->SetBranchAddress("ProcessCounting_ASDI_eIoni", &ProcessCounting_ASDI_eIoni, &bProcessCounting_ASDI_eIoni);
	m_TChain->SetBranchAddress("ProcessCounting_PSDI_eBrem", &ProcessCounting_PSDI_eBrem, &bProcessCounting_PSDI_eBrem);
	m_TChain->SetBranchAddress("ProcessCounting_PSDI_eIoni", &ProcessCounting_PSDI_eIoni, &bProcessCounting_PSDI_eIoni);
	m_TChain->SetBranchAddress("ProcessCounting_PSDI_msc", &ProcessCounting_PSDI_msc, &bProcessCounting_PSDI_msc);
	m_TChain->SetBranchAddress("ProcessCounting_volume", &ProcessCounting_volume, &bProcessCounting_volume);

	//find dE range and num_eIoni range
	double dE_min = 1e10;  // in MeV
	double dE_max = -1; // in MeV
	int num_eIoni_max = 0;
  Long64_t nEvent = m_TChain->GetEntries();
	std::cout<<"nEvent = "<<nEvent<<std::endl;
  for( Long64_t iEvent = 0; iEvent < nEvent; iEvent++ ){
		Long64_t tentry = m_TChain->LoadTree(iEvent);

    if(bMcTruth_x) bMcTruth_x->GetEntry(tentry);
    if(bMcTruth_y) bMcTruth_y->GetEntry(tentry);
    if(bMcTruth_z) bMcTruth_z->GetEntry(tentry);
    if(bMcTruth_time) bMcTruth_time->GetEntry(tentry);
    if(bMcTruth_px) bMcTruth_px->GetEntry(tentry);
    if(bMcTruth_py) bMcTruth_py->GetEntry(tentry);
    if(bMcTruth_pz) bMcTruth_pz->GetEntry(tentry);
    if(bMcTruth_charge) bMcTruth_charge->GetEntry(tentry);
    if(bMcTruth_pid) bMcTruth_pid->GetEntry(tentry);
    if(bMcTruth_tid) bMcTruth_tid->GetEntry(tentry);
    if(bMcTruth_ptid) bMcTruth_ptid->GetEntry(tentry);
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
    if(bProcessCounting_dTheta) bProcessCounting_dTheta->GetEntry(tentry);
    if(bProcessCounting_dE) bProcessCounting_dE->GetEntry(tentry);
    if(bProcessCounting_edepT) bProcessCounting_edepT->GetEntry(tentry);
    if(bProcessCounting_edepN) bProcessCounting_edepN->GetEntry(tentry);
    if(bProcessCounting_edepI) bProcessCounting_edepI->GetEntry(tentry);
    if(bProcessCounting_e) bProcessCounting_e->GetEntry(tentry);
    if(bProcessCounting_preX) bProcessCounting_preX->GetEntry(tentry);
    if(bProcessCounting_preY) bProcessCounting_preY->GetEntry(tentry);
    if(bProcessCounting_preZ) bProcessCounting_preZ->GetEntry(tentry);
    if(bProcessCounting_postX) bProcessCounting_postX->GetEntry(tentry);
    if(bProcessCounting_postY) bProcessCounting_postY->GetEntry(tentry);
    if(bProcessCounting_postZ) bProcessCounting_postZ->GetEntry(tentry);
    if(bProcessCounting_particleName) bProcessCounting_particleName->GetEntry(tentry);
    if(bProcessCounting_charge) bProcessCounting_charge->GetEntry(tentry);
    if(bProcessCounting_process) bProcessCounting_process->GetEntry(tentry);
    if(bProcessCounting_ASDI_msc) bProcessCounting_ASDI_msc->GetEntry(tentry);
    if(bProcessCounting_ASDI_eBrem) bProcessCounting_ASDI_eBrem->GetEntry(tentry);
    if(bProcessCounting_ASDI_eIoni) bProcessCounting_ASDI_eIoni->GetEntry(tentry);
    if(bProcessCounting_PSDI_eBrem) bProcessCounting_PSDI_eBrem->GetEntry(tentry);
    if(bProcessCounting_PSDI_eIoni) bProcessCounting_PSDI_eIoni->GetEntry(tentry);
    if(bProcessCounting_PSDI_msc) bProcessCounting_PSDI_msc->GetEntry(tentry);
    if(bProcessCounting_volume) bProcessCounting_volume->GetEntry(tentry);

    m_TChain->GetEntry(iEvent);

		double dE_total = 0;
		int num_eIoni = 0;
		for( int i = 0; i < ProcessCounting_nSteps; i++ ){
			if ( (*ProcessCounting_tid)[i] == 1 ){
				dE_total += (*ProcessCounting_edepT)[i];
				num_eIoni += (*ProcessCounting_nSec)[i];
			}
		}
		std::cout<<"  ["<<iEvent<<"]: "<<dE_total<<"MeV, "<<num_eIoni<<std::endl;
		if ( dE_total > dE_max ) dE_max = dE_total;
		if ( dE_total < dE_min ) dE_min = dE_total;
		if ( num_eIoni > num_eIoni_max ) num_eIoni_max = num_eIoni;
	}
	std::cout<<"dE_min = "<<dE_min<<"MeV"<<std::endl;
	std::cout<<"dE_max = "<<dE_max<<"MeV"<<std::endl;

	//=>Prepare histograms
	for ( int i = 0; i < num_hists; i++ ){
		double dE_left = dE_min + (dE_max - dE_min)/num_hists*i;
		double dE_right = dE_min + (dE_max - dE_min)/num_hists*(i+1);
		buff.str("");
		buff.clear();
		buff<<"num_eIoni_"<<i;
		nameForH1D.push_back(buff.str());
		buff.str("");
		buff.clear();
		buff<<"N_{eIoni} @ ("<<dE_left<<"MeV < dE < "<<dE_right<<"MeV)";
		titleForH1D.push_back(buff.str());
		bin1ForH1D.push_back((num_eIoni_max+1)*2);
		left1ForH1D.push_back(0);
		right1ForH1D.push_back(num_eIoni_max+1);
	}
	nameForH2D.push_back("N_dE");
	titleForH2D.push_back("N_{eIoni} vs dE(MeV)");
	bin1ForH2D.push_back((num_eIoni_max+1)*2);
	left1ForH2D.push_back(0);
	right1ForH2D.push_back(num_eIoni_max+1);
	bin2ForH2D.push_back(num_bin);
	left2ForH2D.push_back(dE_min);
	right2ForH2D.push_back(dE_max);

	for ( int i = 0; i < nameForH2D.size(); i++ ){
		vecH2D.push_back(new TH2D(nameForH2D[i],titleForH2D[i],bin1ForH2D[i],left1ForH2D[i],right1ForH2D[i],bin2ForH2D[i],left2ForH2D[i],right2ForH2D[i]) );
	}

	for ( int i = 0; i < nameForH1D.size(); i++ ){
		vecH1D.push_back(new TH1D(nameForH1D[i],titleForH1D[i],bin1ForH1D[i],left1ForH1D[i],right1ForH1D[i]) );
		std::cout<<"vecH1D["<<i<<"]: "<<nameForH1D[i]<<", "<<titleForH1D[i]<<", "<<bin1ForH1D[i]<<", "<<left1ForH1D[i]<<", "<<right1ForH1D[i]<<std::endl;
	}

	//=>About Statistical
	int N0 = 0;
	int N1 = 0;
	int N2 = 0;
	int N3 = 0;

	//=>generate histograms
  for( Long64_t iEvent = 0; iEvent < nEvent; iEvent++ ){
		Long64_t tentry = m_TChain->LoadTree(iEvent);

    if(bMcTruth_x) bMcTruth_x->GetEntry(tentry);
    if(bMcTruth_y) bMcTruth_y->GetEntry(tentry);
    if(bMcTruth_z) bMcTruth_z->GetEntry(tentry);
    if(bMcTruth_time) bMcTruth_time->GetEntry(tentry);
    if(bMcTruth_px) bMcTruth_px->GetEntry(tentry);
    if(bMcTruth_py) bMcTruth_py->GetEntry(tentry);
    if(bMcTruth_pz) bMcTruth_pz->GetEntry(tentry);
    if(bMcTruth_charge) bMcTruth_charge->GetEntry(tentry);
    if(bMcTruth_pid) bMcTruth_pid->GetEntry(tentry);
    if(bMcTruth_tid) bMcTruth_tid->GetEntry(tentry);
    if(bMcTruth_ptid) bMcTruth_ptid->GetEntry(tentry);
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
    if(bProcessCounting_dTheta) bProcessCounting_dTheta->GetEntry(tentry);
    if(bProcessCounting_dE) bProcessCounting_dE->GetEntry(tentry);
    if(bProcessCounting_e) bProcessCounting_e->GetEntry(tentry);
    if(bProcessCounting_preX) bProcessCounting_preX->GetEntry(tentry);
    if(bProcessCounting_preY) bProcessCounting_preY->GetEntry(tentry);
    if(bProcessCounting_preZ) bProcessCounting_preZ->GetEntry(tentry);
    if(bProcessCounting_postX) bProcessCounting_postX->GetEntry(tentry);
    if(bProcessCounting_postY) bProcessCounting_postY->GetEntry(tentry);
    if(bProcessCounting_postZ) bProcessCounting_postZ->GetEntry(tentry);
    if(bProcessCounting_particleName) bProcessCounting_particleName->GetEntry(tentry);
    if(bProcessCounting_charge) bProcessCounting_charge->GetEntry(tentry);
    if(bProcessCounting_process) bProcessCounting_process->GetEntry(tentry);
    if(bProcessCounting_ASDI_msc) bProcessCounting_ASDI_msc->GetEntry(tentry);
    if(bProcessCounting_ASDI_eBrem) bProcessCounting_ASDI_eBrem->GetEntry(tentry);
    if(bProcessCounting_ASDI_eIoni) bProcessCounting_ASDI_eIoni->GetEntry(tentry);
    if(bProcessCounting_PSDI_eBrem) bProcessCounting_PSDI_eBrem->GetEntry(tentry);
    if(bProcessCounting_PSDI_eIoni) bProcessCounting_PSDI_eIoni->GetEntry(tentry);
    if(bProcessCounting_PSDI_msc) bProcessCounting_PSDI_msc->GetEntry(tentry);
    if(bProcessCounting_volume) bProcessCounting_volume->GetEntry(tentry);

    m_TChain->GetEntry(iEvent);

		double dE_total = 0;
		int num_eIoni = 0;
		for( int i = 0; i < ProcessCounting_nSteps; i++ ){
			if ( (*ProcessCounting_tid)[i] == 1 ){
				dE_total += (*ProcessCounting_edepT)[i];
				num_eIoni += (*ProcessCounting_nSec)[i];
			}
		}
		std::cout<<"  ["<<iEvent<<"] dE_total = "<<dE_total<<"MeV"<<std::endl;
		vecH2D[0]->Fill(num_eIoni,dE_total);
		for ( int i = 0; i < num_hists; i++ ){
			double dE_left = dE_min + (dE_max - dE_min)/num_hists*i;
			double dE_right = dE_min + (dE_max - dE_min)/num_hists*(i+1);
			if ( dE_total >= dE_left && dE_total < dE_right ){
				vecH1D[i]->Fill(num_eIoni);
			}
		}
	}

	//=>For output
	std::string output_filename = "output.root";
	TFile output_file(output_filename.c_str(),"recreate");

	clock_t t_END = clock();

	//#########################THEEND###############################
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

  gStyle->SetPalette(1);
  gStyle->SetOptStat(0);
//  gStyle->SetTitleW(0.99);
//  gStyle->SetTitleH(0.08);

  for ( int i = 0; i < vecH2D.size(); i++ ){
  	TString name = vecH2D[i]->GetName();
  	TCanvas* c = new TCanvas(name);
  	//gPad->SetLogy(1);
  	vecH2D[i]->Draw("COLZ");
  	vecH2D[i]->Write();
  	TString fileName = name + ".pdf";
  	c->Print(fileName);
  }

	for ( int i = 0; i < vecH1D.size(); i++ ){
		TString name = vecH1D[i]->GetName();
		TCanvas* c = new TCanvas(name);
		if ( i == 1 ) gPad->SetLogy(1);
		else gPad->SetLogy(0);
		vecH1D[i]->Draw();
  	vecH1D[i]->Write();
		TString fileName = name + ".pdf";
		c->Print(fileName);
	}

	output_file.Close();
	return 0;
}
