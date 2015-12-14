#include <iostream>
#include <vector>
#include <string>
#include "TTree.h"
#include "TFile.h"
#include "TChain.h"

int main(int argc,char ** argv){
	TChain * chain = new TChain("tree","tree");
	for (int i = 60; i<100; i++){
		chain->Add(Form("/scratchfs/bes/wuc/MyWorkArea/Data/TS1.ap.150919.g41001p02QBH/%d_job0.raw",i));
	}
	std::vector<int> * in_pid = 0;
	std::vector<double> * in_t = 0;
	std::vector<double> * in_x = 0;
	std::vector<double> * in_y = 0;
	std::vector<double> * in_z = 0;
	std::vector<double> * in_px = 0;
	std::vector<double> * in_py = 0;
	std::vector<double> * in_pz = 0;
	std::vector<std::string> * in_volName = 0;
	double weight; 
	chain->SetBranchAddress("V_t",&in_t);
	chain->SetBranchAddress("V_x",&in_x);
	chain->SetBranchAddress("V_y",&in_y);
	chain->SetBranchAddress("V_z",&in_z);
	chain->SetBranchAddress("V_px",&in_px);
	chain->SetBranchAddress("V_py",&in_py);
	chain->SetBranchAddress("V_pz",&in_pz);
	chain->SetBranchAddress("V_pid",&in_pid);
	chain->SetBranchAddress("V_volName",&in_volName);
	chain->SetBranchAddress("weight",&weight);

	double t,x,y,z,px,py,pz;
	int pid;
	TFile * ofile = new TFile("output.3.root","RECREATE");
	TTree * otree = new TTree("t","t");
	otree->Branch("x",&x);
	otree->Branch("y",&y);
	otree->Branch("z",&z);
	otree->Branch("px",&px);
	otree->Branch("py",&py);
	otree->Branch("pz",&pz);
	otree->Branch("t",&t);
	otree->Branch("pid",&pid);
	otree->Branch("weight",&weight);

	Long64_t N = chain->GetEntries();
	std::cout<<N<<" events to analyze"<<std::endl;
	for (Long64_t i = 0; i<N;  i++){
		chain->GetEntry(i);
		for (int j = 0; j<in_x->size(); j++){
			if ((*in_volName)[j]!="TS2") continue;
			pid = (*in_pid)[j];
			if (pid>1e6) continue;
			t = (*in_t)[j];
			x = (*in_x)[j]*10;
			y = (*in_y)[j]*10;
			z = (*in_z)[j]*10;
			px = (*in_px)[j]*1000;
			py = (*in_py)[j]*1000;
			pz = (*in_pz)[j]*1000;
			otree->Fill();
		}
	}
	otree->Write();
	ofile->Close();
}
