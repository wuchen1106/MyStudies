// get a distribution of different particles at different planes
// to modify the design of collimators
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "TString.h"
#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"

int main(int argc, char *argv[]){
	TString MyData = getenv("MYDATA");
	TString MyWork = getenv("MYWORKAREA");

	std::stringstream buff;
	// About this run
	TString runName = "Coll";
	std::vector<TString> DirName;
	std::vector<int> nRuns;
	std::vector<TString> FileNames;

	// ########Should Modify#########
	//runName = "TS2.BL";
	FileNames.push_back(MyWork+"/Simulate/comet/output/"+runName+".all.0.root");
	FileNames.push_back(MyWork+"/Simulate/comet/output/"+runName+".all.1.root");
	FileNames.push_back(MyWork+"/Simulate/comet/output/"+runName+".sig.0.root");
	FileNames.push_back(MyWork+"/Simulate/comet/output/"+runName+".sig.1.root");
	FileNames.push_back(MyWork+"/Simulate/comet/output/"+runName+".sig.2.root");
	FileNames.push_back(MyWork+"/Simulate/comet/output/"+runName+".sig.3.root");
//	DirName.push_back(MyData+"/raw/g4sim/CDCHit.pim.g60cm10mm.005T.0508.g4s.QBH");
//	nRuns.push_back(50);
	// ########Should Modify#########

	// get the TChain
	TChain *c = new TChain("tree");
	std::cout<<"FileNames.size() = "<<(FileNames.size())<<std::endl;
	for (int i = 0; i<FileNames.size(); i++){
		std::cout<<"FileNames["<<i<<"] = \""<<FileNames[i]<<"\""<<std::endl;
		c->Add(FileNames[i]);
	}
	std::cout<<"nRuns = "<<nRuns.size()<<std::endl;
	for (int iRun = 0; iRun < nRuns.size(); iRun++ ){
		for (int i = 0; i<nRuns[iRun]; i++){
			buff.str("");
			buff.clear();
			buff<<DirName[iRun]<<"/"<<i<<"_job0.raw";
			c->Add( buff.str().c_str());
		}
	}

	// input
	int  evt_num;
	int  run_num;
	double weight;

	int  I_nHits = 0;
	std::vector<int>     *I_pid = 0;
	std::vector<double>  *I_t = 0;
	std::vector<double>  *I_px = 0;
	std::vector<double>  *I_py = 0;
	std::vector<double>  *I_pz = 0;
	std::vector<double>  *I_x = 0;
	std::vector<double>  *I_y = 0;
	std::vector<double>  *I_z = 0;
	std::vector<std::string>  *I_volName = 0;
	int T_nHits = 0;
	std::vector<int>     *T_pid = 0;

	c->SetBranchAddress("evt_num",&evt_num);
	c->SetBranchAddress("run_num",&run_num);
	c->SetBranchAddress("weight",&weight);

	c->SetBranchAddress("V_nHits",&I_nHits);
	c->SetBranchAddress("V_pid",&I_pid);
	c->SetBranchAddress("V_t",&I_t);
	c->SetBranchAddress("V_px",&I_px);
	c->SetBranchAddress("V_py",&I_py);
	c->SetBranchAddress("V_pz",&I_pz);
	c->SetBranchAddress("V_x",&I_x);
	c->SetBranchAddress("V_y",&I_y);
	c->SetBranchAddress("V_z",&I_z);
	c->SetBranchAddress("V_volName",&I_volName);

	c->SetBranchAddress("T_nHits",&T_nHits);
	c->SetBranchAddress("T_pid",&T_pid);

	// output
	TFile * f = new TFile(runName+".root","RECREATE");
	std::vector<TH1D*> v_hists;
	std::vector<TH2D*> v_hists2;

	TH1D * h_e_mupi_x = new TH1D("h_e_mupi_x","0^{o}",128,-18,18); v_hists.push_back(h_e_mupi_x);
	TH1D * h_e_mupi_y = new TH1D("h_e_mupi_y","0^{o}",128,-18,18); v_hists.push_back(h_e_mupi_y);
	TH1D * h_e_mupi_p = new TH1D("h_e_mupi_p","0^{o}",256,0,300); v_hists.push_back(h_e_mupi_p);

	TH1D * h_e_bkgm_x = new TH1D("h_e_bkgm_x","0^{o}",128,-18,18); v_hists.push_back(h_e_bkgm_x);
	TH1D * h_e_bkgm_y = new TH1D("h_e_bkgm_y","0^{o}",128,-18,18); v_hists.push_back(h_e_bkgm_y);
	TH1D * h_e_bkgm_p = new TH1D("h_e_bkgm_p","0^{o}",256,0,300); v_hists.push_back(h_e_bkgm_p);

	TH1D * h_e_bkgp_x = new TH1D("h_e_bkgp_x","0^{o}",128,-18,18); v_hists.push_back(h_e_bkgp_x);
	TH1D * h_e_bkgp_y = new TH1D("h_e_bkgp_y","0^{o}",128,-18,18); v_hists.push_back(h_e_bkgp_y);
	TH1D * h_e_bkgp_p = new TH1D("h_e_bkgp_p","0^{o}",256,0,300); v_hists.push_back(h_e_bkgp_p);

	TH1D * h_a_mupi_x = new TH1D("h_a_mupi_x","20^{o}",128,-18,18); v_hists.push_back(h_a_mupi_x);
	TH1D * h_a_mupi_y = new TH1D("h_a_mupi_y","20^{o}",128,-18,18); v_hists.push_back(h_a_mupi_y);
	TH1D * h_a_mupi_p = new TH1D("h_a_mupi_p","20^{o}",256,0,300); v_hists.push_back(h_a_mupi_p);

	TH1D * h_a_bkgp_x = new TH1D("h_a_bkgp_x","20^{o}",128,-18,18); v_hists.push_back(h_a_bkgp_x);
	TH1D * h_a_bkgp_y = new TH1D("h_a_bkgp_y","20^{o}",128,-18,18); v_hists.push_back(h_a_bkgp_y);
	TH1D * h_a_bkgp_p = new TH1D("h_a_bkgp_p","20^{o}",256,0,300); v_hists.push_back(h_a_bkgp_p);

	TH1D * h_a_bkgm_x = new TH1D("h_a_bkgm_x","20^{o}",128,-18,18); v_hists.push_back(h_a_bkgm_x);
	TH1D * h_a_bkgm_y = new TH1D("h_a_bkgm_y","20^{o}",128,-18,18); v_hists.push_back(h_a_bkgm_y);
	TH1D * h_a_bkgm_p = new TH1D("h_a_bkgm_p","20^{o}",256,0,300); v_hists.push_back(h_a_bkgm_p);

	TH1D * h_b_mupi_x = new TH1D("h_b_mupi_x","35^{o}",128,-18,18); v_hists.push_back(h_b_mupi_x);
	TH1D * h_b_mupi_y = new TH1D("h_b_mupi_y","35^{o}",128,-18,18); v_hists.push_back(h_b_mupi_y);
	TH1D * h_b_mupi_p = new TH1D("h_b_mupi_p","35^{o}",256,0,300); v_hists.push_back(h_b_mupi_p);

	TH1D * h_b_bkgp_x = new TH1D("h_b_bkgp_x","35^{o}",128,-18,18); v_hists.push_back(h_b_bkgp_x);
	TH1D * h_b_bkgp_y = new TH1D("h_b_bkgp_y","35^{o}",128,-18,18); v_hists.push_back(h_b_bkgp_y);
	TH1D * h_b_bkgp_p = new TH1D("h_b_bkgp_p","35^{o}",256,0,300); v_hists.push_back(h_b_bkgp_p);

	TH1D * h_b_bkgm_x = new TH1D("h_b_bkgm_x","35^{o}",128,-18,18); v_hists.push_back(h_b_bkgm_x);
	TH1D * h_b_bkgm_y = new TH1D("h_b_bkgm_y","35^{o}",128,-18,18); v_hists.push_back(h_b_bkgm_y);
	TH1D * h_b_bkgm_p = new TH1D("h_b_bkgm_p","35^{o}",256,0,300); v_hists.push_back(h_b_bkgm_p);

	TH1D * h_c_mupi_x = new TH1D("h_c_mupi_x","45^{o}",128,-18,18); v_hists.push_back(h_c_mupi_x);
	TH1D * h_c_mupi_y = new TH1D("h_c_mupi_y","45^{o}",128,-18,18); v_hists.push_back(h_c_mupi_y);
	TH1D * h_c_mupi_p = new TH1D("h_c_mupi_p","45^{o}",256,0,300); v_hists.push_back(h_c_mupi_p);

	TH1D * h_c_bkgp_x = new TH1D("h_c_bkgp_x","45^{o}",128,-18,18); v_hists.push_back(h_c_bkgp_x);
	TH1D * h_c_bkgp_y = new TH1D("h_c_bkgp_y","45^{o}",128,-18,18); v_hists.push_back(h_c_bkgp_y);
	TH1D * h_c_bkgp_p = new TH1D("h_c_bkgp_p","45^{o}",256,0,300); v_hists.push_back(h_c_bkgp_p);

	TH1D * h_c_bkgm_x = new TH1D("h_c_bkgm_x","45^{o}",128,-18,18); v_hists.push_back(h_c_bkgm_x);
	TH1D * h_c_bkgm_y = new TH1D("h_c_bkgm_y","45^{o}",128,-18,18); v_hists.push_back(h_c_bkgm_y);
	TH1D * h_c_bkgm_p = new TH1D("h_c_bkgm_p","45^{o}",256,0,300); v_hists.push_back(h_c_bkgm_p);

	TH1D * h_d0_mupi_x = new TH1D("h_d0_mupi_x","55^{o}",128,-18,18); v_hists.push_back(h_d0_mupi_x);
	TH1D * h_d0_mupi_y = new TH1D("h_d0_mupi_y","55^{o}",128,-18,18); v_hists.push_back(h_d0_mupi_y);
	TH1D * h_d0_mupi_p = new TH1D("h_d0_mupi_p","55^{o}",256,0,300); v_hists.push_back(h_d0_mupi_p);

	TH1D * h_d0_bkgp_x = new TH1D("h_d0_bkgp_x","55^{o}",128,-18,18); v_hists.push_back(h_d0_bkgp_x);
	TH1D * h_d0_bkgp_y = new TH1D("h_d0_bkgp_y","55^{o}",128,-18,18); v_hists.push_back(h_d0_bkgp_y);
	TH1D * h_d0_bkgp_p = new TH1D("h_d0_bkgp_p","55^{o}",256,0,300); v_hists.push_back(h_d0_bkgp_p);

	TH1D * h_d0_bkgm_x = new TH1D("h_d0_bkgm_x","55^{o}",128,-18,18); v_hists.push_back(h_d0_bkgm_x);
	TH1D * h_d0_bkgm_y = new TH1D("h_d0_bkgm_y","55^{o}",128,-18,18); v_hists.push_back(h_d0_bkgm_y);
	TH1D * h_d0_bkgm_p = new TH1D("h_d0_bkgm_p","55^{o}",256,0,300); v_hists.push_back(h_d0_bkgm_p);

	TH1D * h_d1_mupi_x = new TH1D("h_d1_mupi_x","65^{o}",128,-18,18); v_hists.push_back(h_d1_mupi_x);
	TH1D * h_d1_mupi_y = new TH1D("h_d1_mupi_y","65^{o}",128,-18,18); v_hists.push_back(h_d1_mupi_y);
	TH1D * h_d1_mupi_p = new TH1D("h_d1_mupi_p","65^{o}",256,0,300); v_hists.push_back(h_d1_mupi_p);

	TH1D * h_d1_bkgm_x = new TH1D("h_d1_bkgm_x","65^{o}",128,-18,18); v_hists.push_back(h_d1_bkgm_x);
	TH1D * h_d1_bkgm_y = new TH1D("h_d1_bkgm_y","65^{o}",128,-18,18); v_hists.push_back(h_d1_bkgm_y);
	TH1D * h_d1_bkgm_p = new TH1D("h_d1_bkgm_p","65^{o}",256,0,300); v_hists.push_back(h_d1_bkgm_p);

	TH1D * h_d1_bkgp_x = new TH1D("h_d1_bkgp_x","65^{o}",128,-18,18); v_hists.push_back(h_d1_bkgp_x);
	TH1D * h_d1_bkgp_y = new TH1D("h_d1_bkgp_y","65^{o}",128,-18,18); v_hists.push_back(h_d1_bkgp_y);
	TH1D * h_d1_bkgp_p = new TH1D("h_d1_bkgp_p","65^{o}",256,0,300); v_hists.push_back(h_d1_bkgp_p);

	TH1D * h_d2_mupi_x = new TH1D("h_d2_mupi_x","75^{o}",128,-18,18); v_hists.push_back(h_d2_mupi_x);
	TH1D * h_d2_mupi_y = new TH1D("h_d2_mupi_y","75^{o}",128,-18,18); v_hists.push_back(h_d2_mupi_y);
	TH1D * h_d2_mupi_p = new TH1D("h_d2_mupi_p","75^{o}",256,0,300); v_hists.push_back(h_d2_mupi_p);

	TH1D * h_d2_bkgp_x = new TH1D("h_d2_bkgp_x","75^{o}",128,-18,18); v_hists.push_back(h_d2_bkgp_x);
	TH1D * h_d2_bkgp_y = new TH1D("h_d2_bkgp_y","75^{o}",128,-18,18); v_hists.push_back(h_d2_bkgp_y);
	TH1D * h_d2_bkgp_p = new TH1D("h_d2_bkgp_p","75^{o}",256,0,300); v_hists.push_back(h_d2_bkgp_p);

	TH1D * h_d2_bkgm_x = new TH1D("h_d2_bkgm_x","75^{o}",128,-18,18); v_hists.push_back(h_d2_bkgm_x);
	TH1D * h_d2_bkgm_y = new TH1D("h_d2_bkgm_y","75^{o}",128,-18,18); v_hists.push_back(h_d2_bkgm_y);
	TH1D * h_d2_bkgm_p = new TH1D("h_d2_bkgm_p","75^{o}",256,0,300); v_hists.push_back(h_d2_bkgm_p);

	TH1D * h_d3_mupi_x = new TH1D("h_d3_mupi_x","85^{o}",128,-18,18); v_hists.push_back(h_d3_mupi_x);
	TH1D * h_d3_mupi_y = new TH1D("h_d3_mupi_y","85^{o}",128,-18,18); v_hists.push_back(h_d3_mupi_y);
	TH1D * h_d3_mupi_p = new TH1D("h_d3_mupi_p","85^{o}",256,0,300); v_hists.push_back(h_d3_mupi_p);
                                                     
	TH1D * h_d3_bkgm_x = new TH1D("h_d3_bkgm_x","85^{o}",128,-18,18); v_hists.push_back(h_d3_bkgm_x);
	TH1D * h_d3_bkgm_y = new TH1D("h_d3_bkgm_y","85^{o}",128,-18,18); v_hists.push_back(h_d3_bkgm_y);
	TH1D * h_d3_bkgm_p = new TH1D("h_d3_bkgm_p","85^{o}",256,0,300); v_hists.push_back(h_d3_bkgm_p);
                                                     
	TH1D * h_d3_bkgp_x = new TH1D("h_d3_bkgp_x","85^{o}",128,-18,18); v_hists.push_back(h_d3_bkgp_x);
	TH1D * h_d3_bkgp_y = new TH1D("h_d3_bkgp_y","85^{o}",128,-18,18); v_hists.push_back(h_d3_bkgp_y);
	TH1D * h_d3_bkgp_p = new TH1D("h_d3_bkgp_p","85^{o}",256,0,300); v_hists.push_back(h_d3_bkgp_p);

	TH2D * h_e_mupi_xy = new TH2D("h_e_mupi_xy","0^{o}",128,-18,18,128,-18,18); v_hists2.push_back(h_e_mupi_xy);
	TH2D * h_a_mupi_xy = new TH2D("h_a_mupi_xy","20^{o}",128,-18,18,128,-18,18); v_hists2.push_back(h_a_mupi_xy);
	TH2D * h_b_mupi_xy = new TH2D("h_b_mupi_xy","35^{o}",128,-18,18,128,-18,18); v_hists2.push_back(h_b_mupi_xy);
	TH2D * h_c_mupi_xy = new TH2D("h_c_mupi_xy","45^{o}",128,-18,18,128,-18,18); v_hists2.push_back(h_c_mupi_xy);
	TH2D * h_d0_mupi_xy = new TH2D("h_d0_mupi_xy","55^{o}",128,-18,18,128,-18,18); v_hists2.push_back(h_d0_mupi_xy);
	TH2D * h_d1_mupi_xy = new TH2D("h_d1_mupi_xy","65^{o}",128,-18,18,128,-18,18); v_hists2.push_back(h_d1_mupi_xy);
	TH2D * h_d2_mupi_xy = new TH2D("h_d2_mupi_xy","75^{o}",128,-18,18,128,-18,18); v_hists2.push_back(h_d2_mupi_xy);
	TH2D * h_d3_mupi_xy = new TH2D("h_d3_mupi_xy","85^{o}",128,-18,18,128,-18,18); v_hists2.push_back(h_d3_mupi_xy);
	TH2D * h_e_bkgm_xy = new TH2D("h_e_bkgm_xy","0^{o}",128,-18,18,128,-18,18); v_hists2.push_back(h_e_bkgm_xy);
	TH2D * h_a_bkgm_xy = new TH2D("h_a_bkgm_xy","20^{o}",128,-18,18,128,-18,18); v_hists2.push_back(h_a_bkgm_xy);
	TH2D * h_b_bkgm_xy = new TH2D("h_b_bkgm_xy","35^{o}",128,-18,18,128,-18,18); v_hists2.push_back(h_b_bkgm_xy);
	TH2D * h_c_bkgm_xy = new TH2D("h_c_bkgm_xy","45^{o}",128,-18,18,128,-18,18); v_hists2.push_back(h_c_bkgm_xy);
	TH2D * h_d0_bkgm_xy = new TH2D("h_d0_bkgm_xy","55^{o}",128,-18,18,128,-18,18); v_hists2.push_back(h_d0_bkgm_xy);
	TH2D * h_d1_bkgm_xy = new TH2D("h_d1_bkgm_xy","65^{o}",128,-18,18,128,-18,18); v_hists2.push_back(h_d1_bkgm_xy);
	TH2D * h_d2_bkgm_xy = new TH2D("h_d2_bkgm_xy","75^{o}",128,-18,18,128,-18,18); v_hists2.push_back(h_d2_bkgm_xy);
	TH2D * h_d3_bkgm_xy = new TH2D("h_d3_bkgm_xy","85^{o}",128,-18,18,128,-18,18); v_hists2.push_back(h_d3_bkgm_xy);
	TH2D * h_e_bkgp_xy = new TH2D("h_e_bkgp_xy","0^{o}",128,-18,18,128,-18,18); v_hists2.push_back(h_e_bkgp_xy);
	TH2D * h_a_bkgp_xy = new TH2D("h_a_bkgp_xy","20^{o}",128,-18,18,128,-18,18); v_hists2.push_back(h_a_bkgp_xy);
	TH2D * h_b_bkgp_xy = new TH2D("h_b_bkgp_xy","35^{o}",128,-18,18,128,-18,18); v_hists2.push_back(h_b_bkgp_xy);
	TH2D * h_c_bkgp_xy = new TH2D("h_c_bkgp_xy","45^{o}",128,-18,18,128,-18,18); v_hists2.push_back(h_c_bkgp_xy);
	TH2D * h_d0_bkgp_xy = new TH2D("h_d0_bkgp_xy","55^{o}",128,-18,18,128,-18,18); v_hists2.push_back(h_d0_bkgp_xy);
	TH2D * h_d1_bkgp_xy = new TH2D("h_d1_bkgp_xy","65^{o}",128,-18,18,128,-18,18); v_hists2.push_back(h_d1_bkgp_xy);
	TH2D * h_d2_bkgp_xy = new TH2D("h_d2_bkgp_xy","75^{o}",128,-18,18,128,-18,18); v_hists2.push_back(h_d2_bkgp_xy);
	TH2D * h_d3_bkgp_xy = new TH2D("h_d3_bkgp_xy","85^{o}",128,-18,18,128,-18,18); v_hists2.push_back(h_d3_bkgp_xy);

	// Loop in events
	Long64_t nEvents = c->GetEntries();
	std::cout<<"nEvents = "<<nEvents<<std::endl;
	int printModulo = 1e3;
	for (Long64_t iEvent = 0; iEvent < nEvents; iEvent++ ){
		if (iEvent%printModulo==0) std::cout<<(double)iEvent/nEvents*100<<" % ..."<<std::endl;
		c->GetEntry(iEvent);
		bool found = false;
		for ( int k = 0; k<T_nHits; k++){
			if ((*T_pid)[k]==13){
				found = true;
			}
		}
		for (  int j = 0; j<I_nHits; j++){
			int pid = (*I_pid)[j];
			double px = (*I_px)[j];
			double py = (*I_py)[j];
			double pz = (*I_pz)[j];
			double x = (*I_x)[j];
			double z = (*I_z)[j];
			x = sqrt((300-x)*(300-x)+(300-z)*(300-z))-300;
			double y = (*I_y)[j];
			double pa = sqrt(px*px+py*py+pz*pz)*1000;
			if (found){
				if ((*I_volName)[j]=="TS2a"){
					h_a_mupi_x->Fill(x,weight);
					h_a_mupi_y->Fill(y,weight);
					h_a_mupi_p->Fill(pa,weight);
					h_a_mupi_xy->Fill(x,y,weight);
				}
				else if ((*I_volName)[j]=="TS2b"){
					h_b_mupi_xy->Fill(x,y,weight);
					h_b_mupi_x->Fill(x,weight);
					h_b_mupi_y->Fill(y,weight);
					h_b_mupi_p->Fill(pa,weight);
				}
				else if ((*I_volName)[j]=="TS2c"){
					h_c_mupi_xy->Fill(x,y,weight);
					h_c_mupi_x->Fill(x,weight);
					h_c_mupi_y->Fill(y,weight);
					h_c_mupi_p->Fill(pa,weight);
				}
				else if ((*I_volName)[j]=="TS2d0"){
					h_d0_mupi_xy->Fill(x,y,weight);
					h_d0_mupi_x->Fill(x,weight);
					h_d0_mupi_y->Fill(y,weight);
					h_d0_mupi_p->Fill(pa,weight);
				}
				else if ((*I_volName)[j]=="TS2d3"){
					h_d3_mupi_xy->Fill(x,y,weight);
					h_d3_mupi_x->Fill(x,weight);
					h_d3_mupi_y->Fill(y,weight);
					h_d3_mupi_p->Fill(pa,weight);
				}
				else if ((*I_volName)[j]=="TS2d2"){
					h_d2_mupi_xy->Fill(x,y,weight);
					h_d2_mupi_x->Fill(x,weight);
					h_d2_mupi_y->Fill(y,weight);
					h_d2_mupi_p->Fill(pa,weight);
				}
				else if ((*I_volName)[j]=="TS2d1"){
					h_d1_mupi_xy->Fill(x,y,weight);
					h_d1_mupi_x->Fill(x,weight);
					h_d1_mupi_y->Fill(y,weight);
					h_d1_mupi_p->Fill(pa,weight);
				}
				else if ((*I_volName)[j]=="TS2e"){
					h_e_mupi_xy->Fill(x,y,weight);
					h_e_mupi_x->Fill(x,weight);
					h_e_mupi_y->Fill(y,weight);
					h_e_mupi_p->Fill(pa,weight);
				}
			}
			// FIXME
//			else {
			else if (iEvent<5e4&&fmod((*I_t)[j],1170)>200&&((*I_pid)[j]==11||(((*I_pid)[j]==-211)&&pa>0)||(((*I_pid)[j]==-211)&&pa>55))){
				if ((*I_volName)[j]=="TS2a"){
					h_a_bkgm_xy->Fill(x,y,weight);
					h_a_bkgm_x->Fill(x,weight);
					h_a_bkgm_y->Fill(y,weight);
					h_a_bkgm_p->Fill(pa,weight);
				}
				else if ((*I_volName)[j]=="TS2b"){
					h_b_bkgm_xy->Fill(x,y,weight);
					h_b_bkgm_x->Fill(x,weight);
					h_b_bkgm_y->Fill(y,weight);
					h_b_bkgm_p->Fill(pa,weight);
				}
				else if ((*I_volName)[j]=="TS2c"){
					h_c_bkgm_xy->Fill(x,y,weight);
					h_c_bkgm_x->Fill(x,weight);
					h_c_bkgm_y->Fill(y,weight);
					h_c_bkgm_p->Fill(pa,weight);
				}
				else if ((*I_volName)[j]=="TS2d0"){
					h_d0_bkgm_xy->Fill(x,y,weight);
					h_d0_bkgm_x->Fill(x,weight);
					h_d0_bkgm_y->Fill(y,weight);
					h_d0_bkgm_p->Fill(pa,weight);
				}
				else if ((*I_volName)[j]=="TS2d1"){
					h_d1_bkgm_xy->Fill(x,y,weight);
					h_d1_bkgm_x->Fill(x,weight);
					h_d1_bkgm_y->Fill(y,weight);
					h_d1_bkgm_p->Fill(pa,weight);
				}
				else if ((*I_volName)[j]=="TS2e"){
					h_e_bkgm_xy->Fill(x,y,weight);
					h_e_bkgm_x->Fill(x,weight);
					h_e_bkgm_y->Fill(y,weight);
					h_e_bkgm_p->Fill(pa,weight);
				}
				else if ((*I_volName)[j]=="TS2d2"){
					h_d2_bkgm_xy->Fill(x,y,weight);
					h_d2_bkgm_x->Fill(x,weight);
					h_d2_bkgm_y->Fill(y,weight);
					h_d2_bkgm_p->Fill(pa,weight);
				}
				else if ((*I_volName)[j]=="TS2d3"){
					h_d3_bkgm_xy->Fill(x,y,weight);
					h_d3_bkgm_x->Fill(x,weight);
					h_d3_bkgm_y->Fill(y,weight);
					h_d3_bkgm_p->Fill(pa,weight);
				}
			}
			else if (iEvent<5e4&&fmod((*I_t)[j],1170)>200&&((*I_pid)[j]==-13||(*I_pid)[j]==-11)){
				if ((*I_volName)[j]=="TS2a"){
					h_a_bkgp_xy->Fill(x,y,weight);
					h_a_bkgp_x->Fill(x,weight);
					h_a_bkgp_y->Fill(y,weight);
					h_a_bkgp_p->Fill(pa,weight);
				}
				else if ((*I_volName)[j]=="TS2b"){
					h_b_bkgp_xy->Fill(x,y,weight);
					h_b_bkgp_x->Fill(x,weight);
					h_b_bkgp_y->Fill(y,weight);
					h_b_bkgp_p->Fill(pa,weight);
				}
				else if ((*I_volName)[j]=="TS2c"){
					h_c_bkgp_xy->Fill(x,y,weight);
					h_c_bkgp_x->Fill(x,weight);
					h_c_bkgp_y->Fill(y,weight);
					h_c_bkgp_p->Fill(pa,weight);
				}
				else if ((*I_volName)[j]=="TS2d0"){
					h_d0_bkgp_xy->Fill(x,y,weight);
					h_d0_bkgp_x->Fill(x,weight);
					h_d0_bkgp_y->Fill(y,weight);
					h_d0_bkgp_p->Fill(pa,weight);
				}
				else if ((*I_volName)[j]=="TS2d1"){
					h_d1_bkgp_xy->Fill(x,y,weight);
					h_d1_bkgp_x->Fill(x,weight);
					h_d1_bkgp_y->Fill(y,weight);
					h_d1_bkgp_p->Fill(pa,weight);
				}
				else if ((*I_volName)[j]=="TS2e"){
					h_e_bkgp_xy->Fill(x,y,weight);
					h_e_bkgp_x->Fill(x,weight);
					h_e_bkgp_y->Fill(y,weight);
					h_e_bkgp_p->Fill(pa,weight);
				}
				else if ((*I_volName)[j]=="TS2d2"){
					h_d2_bkgp_xy->Fill(x,y,weight);
					h_d2_bkgp_x->Fill(x,weight);
					h_d2_bkgp_y->Fill(y,weight);
					h_d2_bkgp_p->Fill(pa,weight);
				}
				else if ((*I_volName)[j]=="TS2d3"){
					h_d3_bkgp_xy->Fill(x,y,weight);
					h_d3_bkgp_x->Fill(x,weight);
					h_d3_bkgp_y->Fill(y,weight);
					h_d3_bkgp_p->Fill(pa,weight);
				}
			}
		}
	}

	// Save hists
	for (int i  = 0; i<v_hists.size(); i++){
		v_hists[i]->Write();
	}
	for (int i  = 0; i<v_hists2.size(); i++){
		v_hists2[i]->Write();
	}
	f->Close();

	return 0;
}
