#include <vector>
#include <iostream>
#include "TTree.h"
#include "TFile.h"
#include "TChain.h"

int main (int argc, char** argv){
	TChain * cin = new TChain("tree","tree");
	cin->Add("/scratchfs/bes/wuc/MyWorkArea/Simulate/comet/output/testPT.0.root");
	cin->Add("/scratchfs/bes/wuc/MyWorkArea/Simulate/comet/output/testPT.1.root");
	cin->Add("/scratchfs/bes/wuc/MyWorkArea/Simulate/comet/output/testPT.2.root");
	cin->Add("/scratchfs/bes/wuc/MyWorkArea/Simulate/comet/output/testPT.3.root");
	cin->Add("/scratchfs/bes/wuc/MyWorkArea/Simulate/comet/output/testPT.4.root");
	cin->Add("/scratchfs/bes/wuc/MyWorkArea/Simulate/comet/output/testPT.5.root");
	double x;
	double y;
	double z;
	double px;
	double py;
	double pz;
	std::vector<double> *vx = 0;
	std::vector<double> *vy = 0;
	std::vector<double> *vz = 0;
	std::vector<double> *vpx= 0 ;
	std::vector<double> *vpy= 0 ;
	std::vector<double> *vpz= 0 ;
	cin->SetBranchAddress("ProcessCounting_postX",&vx);
	cin->SetBranchAddress("ProcessCounting_postY",&vy);
	cin->SetBranchAddress("ProcessCounting_postZ",&vz);
	cin->SetBranchAddress("ProcessCounting_postPx",&vpx);
	cin->SetBranchAddress("ProcessCounting_postPy",&vpy);
	cin->SetBranchAddress("ProcessCounting_postPz",&vpz);
	TFile * fout = new TFile("StepPosition.root","RECREATE");
	TTree * tout = new TTree("t","t");
	tout->Branch("x",&x);
	tout->Branch("y",&y);
	tout->Branch("z",&z);
	tout->Branch("px",&px);
	tout->Branch("py",&py);
	tout->Branch("pz",&pz);
	int nEvents = cin->GetEntries();
	for (int iEvent = 0; iEvent<nEvents; iEvent++){
		if (iEvent%10000==0) std::cout<<iEvent<<std::endl;
		cin->GetEntry(iEvent);
		for (int j = 0; j < vx->size(); j++){
			x = (*vx)[j]*10;
			y = (*vy)[j]*10;
			z = (*vz)[j]*10;
			px = (*vpx)[j]*1000;
			py = (*vpy)[j]*1000;
			pz = (*vpz)[j]*1000;
			if (px<1000) continue;
			tout->Fill();
		}
	}
	tout->Write();
	return 0;
}
