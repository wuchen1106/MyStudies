
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <math.h>
#include <string>
#include <vector>
#include <algorithm>

#include "globals.hh"
#include "Randomize.hh"

#include "TH1D.h"
#include "TH2D.h"
#include "TRandom.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TLegend.h"

int main(int argc, char** argv){
	double tsep = 1170; // ns

	std::vector<int> pids;
	pids.push_back(-211); // pi-
	pids.push_back(13);   // mu-
	pids.push_back(11);   // e-
	pids.push_back(211);  // pi+
	pids.push_back(-13);  // mu+
	pids.push_back(-11);  // e+
	pids.push_back(2112); // neutron
	pids.push_back(2212); // proton
	pids.push_back(22);   // gamma

	std::vector<TString> pnames;
	pnames.push_back("pion-");
	pnames.push_back("muon-");
	pnames.push_back("electron-");
	pnames.push_back("pion+");
	pnames.push_back("muon+");
	pnames.push_back("electron+");
	pnames.push_back("neutron");
	pnames.push_back("proton");
	pnames.push_back("gamma");

	std::vector<int> pcharges;
	pcharges.push_back(-1); // pi-
	pcharges.push_back(-1); // mu-
	pcharges.push_back(-1); // e-
	pcharges.push_back(1);  // pi+
	pcharges.push_back(1);  // mu+
	pcharges.push_back(1);  // e+
	pcharges.push_back(0);  // neutron
	pcharges.push_back(1);  // proton
	pcharges.push_back(0);  // gamma

	std::vector<int> pcolors;
	pcolors.push_back(4); // pi-
	pcolors.push_back(2); // mu-
	pcolors.push_back(3); // e-
	pcolors.push_back(4); // pi+
	pcolors.push_back(2); // mu+
	pcolors.push_back(3); // e+
	pcolors.push_back(6); // neutron
	pcolors.push_back(6); // proton
	pcolors.push_back(1); // gamma

	TH1D* h_pa[20];
	for (int i=0; i<pnames.size(); i++){
		TH1D *h = new TH1D(pnames[i],pnames[i],150,0,250);
		h->GetXaxis()->SetTitle("Momentum Amplitude (MeV/c)");
		h->GetYaxis()->SetTitle("count / initial proton");
		if (pcharges[i] == 0) h->SetLineStyle(9);
		else if (pcharges[i] < 0) h->SetLineStyle(1);
		else h->SetLineStyle(2);
		h->SetLineColor(pcolors[i]);
		h_pa[i]=h;
	}

	TH1D* h_time[20];
	for (int i=0; i<pnames.size(); i++){
		TH1D *h = new TH1D(pnames[i],pnames[i],150,0,tsep);
		h->GetXaxis()->SetTitle("Time Distribution (MeV/c)");
		h->GetYaxis()->SetTitle("count / initial proton");
		if (pcharges[i] == 0) h->SetLineStyle(9);
		else if (pcharges[i] < 0) h->SetLineStyle(1);
		else h->SetLineStyle(2);
		h->SetLineColor(pcolors[i]);
		h_time[i]=h;
	}

	TFile *f = 0;
	for (int i = 0; i<1; i++){
		std::cout<<"##In File "<<i<<std::endl;
		double nProtons = 1;
		if (i==0){
			f = new TFile("/scratchfs/bes/wuc/MyWorkArea/MyStudies/Beam/Analyse/beamnew/result/CS.root");
			nProtons = 1E7;
		}
		double weight = 1;
		double px;
		double py;
		double pz;
		double time;
		int pid;
		TTree *t = (TTree*)f->Get("t");
//		t->SetBranchAddress("weight",&weight);
		t->SetBranchAddress("px",&px);
		t->SetBranchAddress("py",&py);
		t->SetBranchAddress("pz",&pz);
		t->SetBranchAddress("t",&time);
		t->SetBranchAddress("pid",&pid);
		int nEvents = t->GetEntries();
		nEvents = 10000;
		for (int iEvent = 0; iEvent<nEvents; iEvent++){
			t->GetEntry(iEvent);
			if (iEvent%1000==0) std::cout<<(double)iEvent/nEvents*100<<"% ..."<<std::endl;
			if (iEvent%1000==0) std::cout<<"pid = "<<pid<<std::endl;
			double pa = sqrt(px*px+py*py+pz*pz);
			for (int ipid = 0; ipid<pids.size(); ipid++){
				if (pids[ipid]==pid){
					h_pa[ipid]->Fill(pa,weight/nProtons);
					h_time[ipid]->Fill(fmod(time,tsep),weight/nProtons);
					if (iEvent%1000==0) std::cout<<"Found "<<pnames[ipid]<<"!"<<std::endl;
					break;
				}
			}
		}
	}

//	TFile *f = new TFile("MT1.g40cm10mm.0018T.root");
//	for (int i = 0; i<pids.size(); i++){
//		h_pa[i]=(TH1D*) f->Get(pnames[i]);
//	}

	double maximumpa = 0;
	maximumpa = 0.05;
	for (int i = 0; i<pids.size(); i++){
		double currentmaximumpa = h_pa[i]->GetMaximum();
		std::cout<<"currentmaximumpa for "<<pnames[i]<<" is "<<currentmaximumpa<<std::endl;
		if (maximumpa < currentmaximumpa) maximumpa = currentmaximumpa;
		std::cout<<"maximumpa = "<<maximumpa<<std::endl;
	}
	maximumpa*=2;

	double maximumtime = 0;
	maximumtime = 0.05;
	for (int i = 0; i<pids.size(); i++){
		double currentmaximumtime = h_time[i]->GetMaximum();
		std::cout<<"currentmaximumtime for "<<pnames[i]<<" is "<<currentmaximumtime<<std::endl;
		if (maximumtime < currentmaximumtime) maximumtime = currentmaximumtime;
		std::cout<<"maximumtime = "<<maximumtime<<std::endl;
	}
	maximumtime*=2;

	std::stringstream buff;

	TFile *fo = new TFile("output.root","RECREATE");

	gStyle->SetOptStat(0);
	gStyle->SetPadTickX(1);
	gStyle->SetPadTickY(1);

	TLegend *legend1 = new TLegend(0.8,0.1,1,0.9);
	TCanvas * c1 = new TCanvas("c1","c1");
	TPad *apad = new TPad("p","p",0,0,1,1);
	apad->Draw();
	apad->cd();
	gPad->SetLogy(1);
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	for (int i = 0; i<pids.size(); i++){
		std::cout<<"Setting for "<<pnames[i]<<": SetRangeUser(1e-9,"<<maximumpa<<")"<<std::endl;
		h_pa[i]->GetYaxis()->SetRangeUser(1e-7,maximumpa);
		if (i==0){
//			h_pa[i]->SetTitle("Particles Back to Capture Section (Graphite, 60cm)");
			h_pa[i]->SetTitle("Particles at The end of Capture Section (Graphite, 60cm)");
			h_pa[i]->Draw();
		}
		else h_pa[i]->Draw("SAME");
		double num = h_pa[i]->Integral();
		buff.str("");
		buff.clear();
		buff<<std::scientific;
		buff<<pnames[i]<<": "<<std::setprecision(2)<<num;
		legend1->AddEntry(h_pa[i],buff.str().c_str());
		h_pa[i]->Write();
	}
	legend1->Draw("SAME");

	TLegend *legend2 = new TLegend(0.8,0.1,1,0.9);
	TCanvas * c2 = new TCanvas("c2","c2");
	TPad *apad2 = new TPad("p2","p2",0,0,1,1);
	apad2->Draw();
	apad2->cd();
	gPad->SetLogy(1);
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	for (int i = 0; i<pids.size(); i++){
		std::cout<<"Setting for "<<pnames[i]<<": SetRangeUser(1e-9,"<<maximumtime<<")"<<std::endl;
		h_time[i]->GetYaxis()->SetRangeUser(1e-7,maximumtime);
		if (i==0){
//			h_time[i]->SetTitle("Particles Back to Capture Section (Graphite, 60cm)");
			h_time[i]->SetTitle("Particles at The end of Capture Section (Graphite, 60cm)");
			h_time[i]->Draw();
		}
		else h_time[i]->Draw("SAME");
		double num = h_time[i]->Integral();
		buff.str("");
		buff.clear();
		buff<<std::scientific;
		buff<<pnames[i]<<": "<<std::setprecision(2)<<num;
		legend2->AddEntry(h_time[i],buff.str().c_str());
		h_time[i]->Write();
	}
	legend2->Draw("SAME");
}
