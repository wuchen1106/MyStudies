#include <iostream>

#include "TChain.h"
#include "TString.h"
#include "TFile.h"
#include "TTree.h"

int main(int argc, char** argv){
	TChain * c =new TChain("t","t");
	for (int i = 1; i<argc; i++){
		std::cout<<i<<": "<<argv[i]<<std::endl;
		c->Add(argv[i]);
	}
	double x,y,z,px,py,pz,t,w;
	c->SetBranchAddress("x",&x);
	c->SetBranchAddress("y",&y);
	c->SetBranchAddress("z",&z);
	c->SetBranchAddress("px",&px);
	c->SetBranchAddress("py",&py);
	c->SetBranchAddress("pz",&pz);
	c->SetBranchAddress("weight",&w);
	c->SetBranchAddress("t",&t);

	int pid=-2212;
	TFile * ofile = new TFile ("output.root","RECREATE");
	TTree * otree = new TTree("t","t");
	otree->Branch("x",&x);
	otree->Branch("y",&y);
	otree->Branch("z",&z);
	otree->Branch("px",&px);
	otree->Branch("py",&py);
	otree->Branch("pz",&pz);
	otree->Branch("t",&t);
	otree->Branch("weight",&w);
	otree->Branch("pid",&pid);
	Long64_t N = c->GetEntries();
	for (Long64_t i = 0; i<N; i++){
		c->GetEntry(i);
		otree->Fill();
	}
	otree->Write();
	ofile->Close();

	return 0;
}
