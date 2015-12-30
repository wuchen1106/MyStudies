#include <iostream>
#include <vector>
#include <math.h>

#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TChain.h"

int main(int argc, char** argv){
	TChain * c = new TChain("t","t");
	c->Add(argv[1]);
	double x,y,z,px,py,pz,weight;
	c->SetBranchAddress("x",&x);
	c->SetBranchAddress("y",&y);
	c->SetBranchAddress("z",&z);
	c->SetBranchAddress("px",&px);
	c->SetBranchAddress("py",&py);
	c->SetBranchAddress("pz",&pz);
	c->SetBranchAddress("weight",&weight);
	Long64_t N = c->GetEntries();
	TFile * of = new TFile("output.root","RECREATE");
	TH1D * h1 = new TH1D("h1","h1",512,-1000,6000);
	TH1D * h2 = new TH1D("h2","h2",512,-1000,6000);
	TH2D * hh = new TH2D("hh","hh",512,-1000,6000,512,0,50e-9);
	double s1 = 0;
	double s2 = 0;
	Long64_t n1 = 0;
	Long64_t n2 = 0;
	for (Long64_t i = 0; i<N; i++){
		c->GetEntry(i);
		h1->Fill(px,weight);
		if (px<0||sqrt(px*px+py*py+pz*pz)<500){
			h2->Fill(px,weight);
			s2+=weight;
			n2++;
		}
		h1->Fill(px,weight);
		s1+=weight;
		hh->Fill(px,weight);
		n1++;
	}
	h1->Write();
	h2->Write();
	hh->Write();
	of->Close();
	std::cout<<s1<<" "<<s2<<std::endl;
	std::cout<<n1<<" "<<n2<<std::endl;
}
