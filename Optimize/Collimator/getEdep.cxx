#include <vector>
#include <iostream>
#include <math.h>

#include "TChain.h"
#include "TFile.h"
#include "TH2D.h"

int main(int argc, char ** argv){
	TChain * c = new TChain("tree","tree");
	for ( int i = 1; i<argc; i++){
		c->Add(argv[i]);
	}
	double T = 10000./1405633*1e7/2.5e12;

	std::vector<double> * in_edep = 0;
	std::vector<double> * in_stepL = 0;
	std::vector<double> * in_x = 0;
	std::vector<double> * in_y = 0;
	std::vector<double> * in_z = 0;
	std::vector<double> * in_px = 0;
	std::vector<double> * in_py = 0;
	std::vector<double> * in_pz = 0;
	std::vector<double> * in_Opx = 0;
	std::vector<double> * in_Opy = 0;
	std::vector<double> * in_Opz = 0;
	std::vector<int> * in_volID = 0;
	std::vector<int> * in_tid = 0;
	std::vector<int> * V_tid = 0;
	std::vector<double> * V_px = 0;
	std::vector<double> * V_py = 0;
	std::vector<double> * V_pz = 0;
	std::vector<double> * Mc_px = 0;
	std::vector<double> * Mc_py = 0;
	std::vector<double> * Mc_pz = 0;
	c->SetBranchAddress("Coll_tid",&in_tid);
	c->SetBranchAddress("Coll_edep",&in_edep);
	c->SetBranchAddress("Coll_stepL",&in_stepL);
	c->SetBranchAddress("Coll_x",&in_x);
	c->SetBranchAddress("Coll_y",&in_y);
	c->SetBranchAddress("Coll_z",&in_z);
	c->SetBranchAddress("Coll_px",&in_px);
	c->SetBranchAddress("Coll_py",&in_py);
	c->SetBranchAddress("Coll_pz",&in_pz);
	c->SetBranchAddress("Coll_Opx",&in_Opx);
	c->SetBranchAddress("Coll_Opy",&in_Opy);
	c->SetBranchAddress("Coll_Opz",&in_Opz);
	c->SetBranchAddress("V_tid",&V_tid);
	c->SetBranchAddress("V_px",&V_px);
	c->SetBranchAddress("V_py",&V_py);
	c->SetBranchAddress("V_pz",&V_pz);
	c->SetBranchAddress("McTruth_px",&Mc_px);
	c->SetBranchAddress("McTruth_py",&Mc_py);
	c->SetBranchAddress("McTruth_pz",&Mc_pz);

	TFile * ofile = new TFile("output.root","RECREATE");
	TTree * otree = new TTree("t","t");
	double pai,pao,pl,sl;
	int n;
	otree->Branch("n",&n);
	otree->Branch("pao",&pao);
	otree->Branch("pai",&pai);
	otree->Branch("pl",&pl);
	otree->Branch("sl",&sl);
	TH2D * h = new TH2D("h","h",45,45,90,40,-20,20);
	TH1D * h1 = new TH1D("h1","h1",40,0,20);
	TH1D * h2[10];
	for (int i = 0; i<10; i++){
		h2[i] = new TH1D(Form("h2_%d",i),Form("h2_%d",i),20,0,5);
	}

	double load= 0;

	Long64_t N = c->GetEntries();
	for (Long64_t i = 0; i<N; i++){
		c->GetEntry(i);
		n = 0;
		pao = 0;
		sl = 0;
		double px = (*Mc_px)[0];
		double py = (*Mc_py)[0];
		double pz = (*Mc_pz)[0];
		pai = sqrt(px*px+py*py+pz*pz);
		pl = pz/sqrt(2)-px/sqrt(2);
		for (int j = 0; j<V_tid->size(); j++){
			if (pao==0){
				if ((*V_tid)[j]!=1) continue;
				double px = (*V_px)[j];
				double py = (*V_py)[j];
				double pz = (*V_pz)[j];
				pao = sqrt(px*px+py*py+pz*pz);
			}
			else break;
		}
		for (int j = 0; j<in_edep->size(); j++){
			if ((*in_tid)[j]!=1) continue;
			n++;
			double x = 300-(*in_x)[j];
			double z = (*in_y)[j];
			double y = 300-(*in_z)[j];
			double px = (*in_px)[j];
			double py = (*in_py)[j];
			double pz = (*in_pz)[j];
			double pa = sqrt(px*px+py*py+pz*pz);
			double phi = atan(x/y);
			double dR = 300-sqrt(x*x+y*y);
			double r = sqrt(z*z+dR*dR);
			if (phi<0||phi>3.14159265358/2||r>60) continue;
//			h->Fill(phi*180/3.14159265358,r,(*in_edep)[j]/(3.28986813367594211e+00*r)*1.6e-10/T);
			h->Fill(phi*180/3.14159265358,(*in_y)[j],(*in_edep)[j]/(3.28986813367594211e+00*r)*1.6e-10/T);
			if (j<10){
				h2[j]->Fill((*in_stepL)[j]);
			}
			load+=(*in_edep)[j]*1.6e-10/T;
			sl+=(*in_stepL)[j];
			if (pao==0){
				px = (*in_Opx)[j];
				py = (*in_Opy)[j];
				pz = (*in_Opz)[j];
				pa = sqrt(px*px+py*py+pz*pz);
				pao = pa;
			}
		}
		otree->Fill();
		h1->Fill(in_edep->size());
	}
	std::cout<<"Load:"<<load<<std::endl;

	otree->Write();
	h->Write();
	h1->Write();
	for (int i = 0; i<10; i++){
		h2[i]->Write();
	}

	return 0;
}
