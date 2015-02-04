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

int main(int argc, char *argv[]){
	TString MyData = getenv("MYDATA");
	TString MyWork = getenv("MYWORKAREA");

	std::stringstream buff;
	// About this run
	TString runName = "raw_g4sim";
	std::vector<TString> DirName;
	std::vector<int> nRuns;
	std::vector<TString> FileNames;

	// ########Should Modify#########
	runName = "TS2.BL";
	FileNames.push_back(MyWork+"/Simulate/comet/output/"+runName+".root");
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

	// output
	TFile * f = new TFile(runName+".root","RECREATE");
	std::vector<TH1D*> v_hists;

	TH1D * h_a_mupi_x = new TH1D("h_a_mupi_x","h_a_mupi_x",128,-18,18); v_hists.push_back(h_a_mupi_x);
	TH1D * h_a_mupi_y = new TH1D("h_a_mupi_y","h_a_mupi_y",128,-18,18); v_hists.push_back(h_a_mupi_y);
	TH1D * h_a_mupi_p = new TH1D("h_a_mupi_p","h_a_mupi_p",256,0,300); v_hists.push_back(h_a_mupi_p);

	TH1D * h_a_bkgc_x = new TH1D("h_a_bkgc_x","h_a_bkgc_x",128,-18,18); v_hists.push_back(h_a_bkgc_x);
	TH1D * h_a_bkgc_y = new TH1D("h_a_bkgc_y","h_a_bkgc_y",128,-18,18); v_hists.push_back(h_a_bkgc_y);
	TH1D * h_a_bkgc_p = new TH1D("h_a_bkgc_p","h_a_bkgc_p",256,0,300); v_hists.push_back(h_a_bkgc_p);

	TH1D * h_b_mupi_x = new TH1D("h_b_mupi_x","h_b_mupi_x",128,-18,18); v_hists.push_back(h_b_mupi_x);
	TH1D * h_b_mupi_y = new TH1D("h_b_mupi_y","h_b_mupi_y",128,-18,18); v_hists.push_back(h_b_mupi_y);
	TH1D * h_b_mupi_p = new TH1D("h_b_mupi_p","h_b_mupi_p",256,0,300); v_hists.push_back(h_b_mupi_p);

	TH1D * h_b_bkgc_x = new TH1D("h_b_bkgc_x","h_b_bkgc_x",128,-18,18); v_hists.push_back(h_b_bkgc_x);
	TH1D * h_b_bkgc_y = new TH1D("h_b_bkgc_y","h_b_bkgc_y",128,-18,18); v_hists.push_back(h_b_bkgc_y);
	TH1D * h_b_bkgc_p = new TH1D("h_b_bkgc_p","h_b_bkgc_p",256,0,300); v_hists.push_back(h_b_bkgc_p);

	TH1D * h_c_mupi_x = new TH1D("h_c_mupi_x","h_c_mupi_x",128,-18,18); v_hists.push_back(h_c_mupi_x);
	TH1D * h_c_mupi_y = new TH1D("h_c_mupi_y","h_c_mupi_y",128,-18,18); v_hists.push_back(h_c_mupi_y);
	TH1D * h_c_mupi_p = new TH1D("h_c_mupi_p","h_c_mupi_p",256,0,300); v_hists.push_back(h_c_mupi_p);

	TH1D * h_c_bkgc_x = new TH1D("h_c_bkgc_x","h_c_bkgc_x",128,-18,18); v_hists.push_back(h_c_bkgc_x);
	TH1D * h_c_bkgc_y = new TH1D("h_c_bkgc_y","h_c_bkgc_y",128,-18,18); v_hists.push_back(h_c_bkgc_y);
	TH1D * h_c_bkgc_p = new TH1D("h_c_bkgc_p","h_c_bkgc_p",256,0,300); v_hists.push_back(h_c_bkgc_p);

	TH1D * h_d_mupi_x = new TH1D("h_d_mupi_x","h_d_mupi_x",128,-18,18); v_hists.push_back(h_d_mupi_x);
	TH1D * h_d_mupi_y = new TH1D("h_d_mupi_y","h_d_mupi_y",128,-18,18); v_hists.push_back(h_d_mupi_y);
	TH1D * h_d_mupi_p = new TH1D("h_d_mupi_p","h_d_mupi_p",256,0,300); v_hists.push_back(h_d_mupi_p);

	TH1D * h_d_bkgc_x = new TH1D("h_d_bkgc_x","h_d_bkgc_x",128,-18,18); v_hists.push_back(h_d_bkgc_x);
	TH1D * h_d_bkgc_y = new TH1D("h_d_bkgc_y","h_d_bkgc_y",128,-18,18); v_hists.push_back(h_d_bkgc_y);
	TH1D * h_d_bkgc_p = new TH1D("h_d_bkgc_p","h_d_bkgc_p",256,0,300); v_hists.push_back(h_d_bkgc_p);

	// Loop in events
	Long64_t nEvents = c->GetEntries();
	std::cout<<"nEvents = "<<nEvents<<std::endl;
	int printModulo = 1e3;
	for (Long64_t iEvent = 0; iEvent < nEvents; iEvent++ ){
		if (iEvent%printModulo==0) std::cout<<(double)iEvent/nEvents*100<<" % ..."<<std::endl;
		c->GetEntry(iEvent);
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
			if (pid==13||pid==-211){
				if ((*I_volName)[j]=="TS2a"){
					h_a_mupi_x->Fill(x,weight);
					h_a_mupi_y->Fill(y,weight);
					h_a_mupi_p->Fill(pa,weight);
				}
				else if ((*I_volName)[j]=="TS2b"){
					h_b_mupi_x->Fill(x,weight);
					h_b_mupi_y->Fill(y,weight);
					h_b_mupi_p->Fill(pa,weight);
				}
				else if ((*I_volName)[j]=="TS2c"){
					h_c_mupi_x->Fill(x,weight);
					h_c_mupi_y->Fill(y,weight);
					h_c_mupi_p->Fill(pa,weight);
				}
				else if ((*I_volName)[j]=="TS2d"){
					h_d_mupi_x->Fill(x,weight);
					h_d_mupi_y->Fill(y,weight);
					h_d_mupi_p->Fill(pa,weight);
				}
			}
			else if (abs(pid)==11||abs(pid)==2212||pid==-13||pid==211){
				if ((*I_volName)[j]=="TS2a"){
					h_a_bkgc_x->Fill(x,weight);
					h_a_bkgc_y->Fill(y,weight);
					h_a_bkgc_p->Fill(pa,weight);
				}
				else if ((*I_volName)[j]=="TS2b"){
					h_b_bkgc_x->Fill(x,weight);
					h_b_bkgc_y->Fill(y,weight);
					h_b_bkgc_p->Fill(pa,weight);
				}
				else if ((*I_volName)[j]=="TS2c"){
					h_c_bkgc_x->Fill(x,weight);
					h_c_bkgc_y->Fill(y,weight);
					h_c_bkgc_p->Fill(pa,weight);
				}
				else if ((*I_volName)[j]=="TS2d"){
					h_d_bkgc_x->Fill(x,weight);
					h_d_bkgc_y->Fill(y,weight);
					h_d_bkgc_p->Fill(pa,weight);
				}
			}
		}
	}

	// Save hists
	for (int i  = 0; i<v_hists.size(); i++){
		v_hists[i]->Write();
	}
	f->Close();

	return 0;
}
