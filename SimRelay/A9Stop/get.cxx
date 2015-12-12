#include <iostream>
#include <sstream>
#include <vector>
#include <string>

#include "TPad.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TPaveText.h"
#include "TFile.h"
#include "TF1.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TTree.h"
#include "TString.h"
#include "TChain.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TRandom.h"

int main (int argc, char** argv){
	TRandom random;
	TFile *f = 0;
	TLegend * legend;
	TString MyData = getenv("MYDATA");
	TString MySim = getenv("MYWORKAREA");
	TString RunName = "none";
	MySim += "/Simulate/comet";

	double time_right = 1140;
	double time_left = 700;
	double tSep = 1170;

	TChain *c = new TChain("tree");

	bool withStopPosition = true;
	bool withCollPosition = false;
	double PulseInterval = 1170;

	f = new TFile("Curves.s100.root");
	TF1 * fMuonDecay_He = (TF1*) f->Get("MuonDecay_He");
	TF1 * fMuonDecay_Al = (TF1*) f->Get("MuonDecay_Al");
	TF1 * fPionDecay = (TF1*) f->Get("PionDecay");
	std::cout<<"Integrating..."<<std::endl;

//=======================User Setting============================	

	RunName = "none";
	std::string theVolume = "A9Container";
//	std::string theVolume = "Target";
	double nProton;

//	int PID = -211;
//	double minimum = 1e-23;
////	nProton = 1.14e19;
//	nProton = 1e11;
//	TString parName = "pi";
////	TString opt = "A9.ALL.150919.W500um.1mmCFRP.2";
//	TString opt = "150919.W500um.OptD2.1mmCFRP.DD35.151117";
//	//RunName = "";
//	TString DirName = MyData+"/A9.pim.stop."+opt;
////	TString DirName = MyData+"/"+opt;
//	int nProcs = 100;
//	int nJobs = 1;

	int PID = 13;
	double minimum = 1e-11;
	nProton = 1e9;
	TString parName = "mu";
	TString opt = "150919.W500um.OptD2.1mmCFRP.DD35.1cmLead";
	//RunName = MySim+"/output/Coll."+opt+".root";
	TString DirName = MyData+"/A9.mupim."+opt;
	int nProcs = 100;
	int nJobs = 1;

	TString par = "#"+parName+"^{-}";
//	TString runName = parName+"on.11.p5";
//	TString option = "Rmin = 11 cm, L = 50 cm";

	TString runName = parName+"on."+opt;
	TString option = opt;

//=======================User Setting============================	

	std::stringstream buff;
	if (RunName == "none" || RunName == ""){
		for (int i = 0; i<nProcs; i++){
			for (int j = 0; j<nJobs; j++){
				buff.str("");
				buff.clear();
				buff<<DirName<<"/"<<i<<"_job"<<j<<".raw";
				std::cout<<"Adding "<<buff.str()<<std::endl;
				c->Add( buff.str().c_str());
			}
		}
	}
	else{
		c->Add(RunName);
	}

	TH2D * h01 = new TH2D("h01",par+" Before BS Coil",50,0,180,50,-200,200);
	h01->GetXaxis()->SetTitle("Momentum Amplitude (MeV/c)");
	h01->GetYaxis()->SetTitle("y position (mm)");
	h01->GetYaxis()->SetTitleOffset(1.5);
	TH2D * h02 = new TH2D("h02",par+" After BS Coil",50,0,180,50,-200,200);
	h02->GetXaxis()->SetTitle("Momentum Amplitude (MeV/c)");
	h02->GetYaxis()->SetTitle("y position (mm)");
	h02->GetYaxis()->SetTitleOffset(1.5);
	TH2D * h03 = new TH2D("h03",par+" Stopped in Target",50,0,180,50,-200,200);
	h03->GetXaxis()->SetTitle("Momentum Amplitude (MeV/c)");
	h03->GetYaxis()->SetTitle("y position (mm)");
	h03->GetYaxis()->SetTitleOffset(1.5);

	TH1D * h04 = new TH1D("h04",par+" Transverse Distribution",100,0,100);
	h04->GetXaxis()->SetTitle("Radius (mm)");
	h04->GetYaxis()->SetTitle("count/p^{+}");
	h04->GetYaxis()->SetTitleOffset(1.5);
//	TH1D * h05 = new TH1D("h05",par+" Longitudinal Distribution",200,-750,250);
//	TH1D * h05 = new TH1D("h05",par+" Longitudinal Distribution",200,-1000,0);
//	TH1D * h05 = new TH1D("h05",par+" Longitudinal Distribution",200,5500,6500);
//	TH1D * h05 = new TH1D("h05",par+" Longitudinal Distribution",200,5900,6900);
	TH1D * h05 = new TH1D("h05",par+" Longitudinal Distribution",200,6000,7000);
	h05->GetXaxis()->SetTitle("z position (mm)");
	h05->GetYaxis()->SetTitle("count/p^{+}");
	h05->GetYaxis()->SetTitleOffset(1.5);

	TH1D *h10 = new TH1D("h10",par+" Stop Time (Before Smear)",200,0,PulseInterval);
	h10->GetXaxis()->SetTitle("Time (ns)");
	h10->GetYaxis()->SetTitle(par+" Stopped / P^{+} / 5.849 ns");
	h10->GetYaxis()->SetTitleOffset(1.5);
	TH1D *h11 = new TH1D("h11",par+" Capture/Decay Time (After Smear)",200,0,PulseInterval);
	h11->GetXaxis()->SetTitle("Time (ns)");
	h11->GetYaxis()->SetTitle(par+" Stopped / P^{+} / 5.849 ns");
	h11->GetYaxis()->SetTitleOffset(1.5);

	TH1D *h20 = new TH1D("h20","Arrival Time Before BS Coil",200,0,PulseInterval);
	h20->GetXaxis()->SetTitle("Time (ns)");
	h20->GetYaxis()->SetTitle("count/p^{+}");
	h20->GetYaxis()->SetTitleOffset(1.5);
	TH1D *h30 = new TH1D("h30","Arrival Time Before BS Coil",200,0,PulseInterval);
	h30->GetXaxis()->SetTitle("Time (ns)");
	h30->GetYaxis()->SetTitle("count/p^{+}");
	h30->GetYaxis()->SetTitleOffset(1.5);
	TH1D *h40 = new TH1D("h40","Arrival Time Before BS Coil",200,0,PulseInterval);
	h40->GetXaxis()->SetTitle("Time (ns)");
	h40->GetYaxis()->SetTitle("count/p^{+}");
	h40->GetYaxis()->SetTitleOffset(1.5);

	TH1D *h1_1 = new TH1D("h1_1","Momentum Before BS Coil",150,0,150);
	h1_1->GetXaxis()->SetTitle("Momentum Amplitude (MeV/c)");
	h1_1->GetYaxis()->SetTitle("count/p^{+}");
	h1_1->GetYaxis()->SetTitleOffset(1.5);
	TH1D *h1_2 = new TH1D("h1_2","Momentum Before BS Coil",150,0,150);
	TH1D *h1_3 = new TH1D("h1_3","Momentum Before BS Coil",150,0,150);

	TH1D *h2_1 = new TH1D("h2_1","y Position Before BS Coil",150,-200,200);
	h2_1->GetXaxis()->SetTitle("y position (mm)");
	h2_1->GetYaxis()->SetTitle("count/p^{+}");
	h2_1->GetYaxis()->SetTitleOffset(1.5);
	TH1D *h2_2 = new TH1D("h2_2","y Position Before BS Coil",150,-200,200);
	TH1D *h2_3 = new TH1D("h2_3","y Position Before BS Coil",150,-200,200);

	std::vector<int> *T_tid = 0;
	std::vector<double> *McTruth_time = 0;
	std::vector<int> *McTruth_pid = 0;
	std::vector<double> *McTruth_x = 0;
	std::vector<double> *McTruth_y = 0;
	std::vector<double> *McTruth_z = 0;
	std::vector<double> *McTruth_px = 0;
	std::vector<double> *McTruth_py = 0;
	std::vector<double> *McTruth_pz = 0;
	std::vector<double> *V_t = 0;
	std::vector<double> *V_x = 0;
	std::vector<double> *V_y = 0;
	std::vector<double> *V_z = 0;
	std::vector<double> *V_px = 0;
	std::vector<double> *V_py = 0;
	std::vector<double> *V_pz = 0;
	std::vector<double> *V_ot = 0;
	std::vector<std::string> *V_volName = 0;
	std::vector<int> *V_pid = 0;
	std::vector<int> *T_pid = 0;
	std::vector<double> *T_Ox = 0;
	std::vector<double> *T_Oy = 0;
	std::vector<double> *T_Oz = 0;
	std::vector<double> *T_Ot = 0;
	std::vector<std::string> *T_volName = 0;
	int V_nHits;
	int T_nHits;

	double weight;
	if (withStopPosition){
		c->SetBranchAddress("T_pid",&T_pid);
		c->SetBranchAddress("T_Ox",&T_Ox);
		c->SetBranchAddress("T_Oy",&T_Oy);
		c->SetBranchAddress("T_Oz",&T_Oz);
		c->SetBranchAddress("T_Ot",&T_Ot);
		c->SetBranchAddress("T_volName",&T_volName);
	}
	c->SetBranchAddress("T_nHits",&T_nHits);
	c->SetBranchAddress("V_nHits",&V_nHits);
	c->SetBranchAddress("McTruth_time",&McTruth_time);
	c->SetBranchAddress("McTruth_pid",&McTruth_pid);
	c->SetBranchAddress("McTruth_px",&McTruth_px);
	c->SetBranchAddress("McTruth_py",&McTruth_py);
	c->SetBranchAddress("McTruth_pz",&McTruth_pz);
	c->SetBranchAddress("McTruth_x",&McTruth_x);
	c->SetBranchAddress("McTruth_y",&McTruth_y);
	c->SetBranchAddress("McTruth_z",&McTruth_z);
	c->SetBranchAddress("V_pid",&V_pid);
	c->SetBranchAddress("V_t",&V_t);
	c->SetBranchAddress("V_px",&V_px);
	c->SetBranchAddress("V_py",&V_py);
	c->SetBranchAddress("V_pz",&V_pz);
	c->SetBranchAddress("V_x",&V_x);
	c->SetBranchAddress("V_y",&V_y);
	c->SetBranchAddress("V_z",&V_z);
	c->SetBranchAddress("V_ot",&V_ot);
	c->SetBranchAddress("V_volName",&V_volName);
//	TChain * chain2 = new TChain("t");
//	chain2->Add("/home/chen/MyWorkArea/g4sim/data/MT1.pim.g60cm10mm.005T.g4s.QBH.root");
//	chain2->SetBranchAddress("weight",&weight);
	c->SetBranchAddress("weight",&weight);
	double Ox = 0;
	double Oy = 0;
	double Oz = 0;
	double Ot = 0;
	double x = 0;
	double y = 0;
	double z = 0;
	double px = 0;
	double py = 0;
	double pz = 0;
	double time = 0;
	bool stopped = false;
	bool passed = false;
	double nPassed = 0;
	double nPassedH = 0;
	double nStopped = 0;
	double nTotal = 0;
	int v_pid = 0;
	double v_x = 0;
	double v_y = 0;
	double v_z = 0;
	double v_px = 0;
	double v_py = 0;
	double v_pz = 0;
	double v_t = 0;
	if (withStopPosition){
		f = new TFile("ST.a9."+runName+".output.root","RECREATE");
	}
	TTree *t  = new TTree("t","t");
	if (withStopPosition){
		t->Branch("x",&Ox);
		t->Branch("y",&Oy);
		t->Branch("z",&Oz);
		t->Branch("t",&Ot);
		t->Branch("weight",&weight);
	}
	TTree *t2;
	if (withCollPosition){
		t2 = new TTree("t","t");
		t2->Branch("pid",&V_pid);
		t2->Branch("x",&V_x);
		t2->Branch("y",&V_y);
		t2->Branch("z",&V_z);
		t2->Branch("t",&V_t);
		t2->Branch("px",&V_px);
		t2->Branch("py",&V_py);
		t2->Branch("pz",&V_pz);
		t2->Branch("weight",&weight);
	}
	double pion_life = 26;
	double m2 = 139.570*139.570;

	int nEvents = c->GetEntries();
	std::cout<<nEvents<<" events!!!"<<std::endl;
	for (Long64_t iEvent = 0; iEvent < nEvents; iEvent++ ){
		c->GetEntry(iEvent);
		double time0 = 0;
		if (V_nHits>0) time0 = (*V_ot)[0];
		double weight_initial = weight;
		double weight_passed = weight;
		double weight_stopped = weight;
		// FIXME
		double deltaT_initial = 0;
		double deltaT_passed = 0;
		double deltaT_stopped = 0;
		x = (*McTruth_x)[0]*10;
		y = (*McTruth_y)[0]*10;
		z = (*McTruth_z)[0]*10;
		px = (*McTruth_px)[0]*1000;
		py = (*McTruth_py)[0]*1000;
		pz = (*McTruth_pz)[0]*1000;
		time = fmod((*McTruth_time)[0],PulseInterval);
		double pa2 = px*px+py*py+pz*pz;
		double e2 = pa2+m2;
		double beta2 = pa2/e2;
		double gamma = 1/sqrt(1-beta2);

		deltaT_initial = (*McTruth_time)[0]-time0;
		if (fabs(PID)==211){
			weight_initial *= exp(-deltaT_initial/pion_life/gamma);
		}

		nTotal += weight;
//		chain2->GetEntry(iEvent);
		if (iEvent%10000==0){
			std::cout<<(double)iEvent/nEvents*100<<" % ..."<<std::endl;
		}
		// coll
		passed=false;
		for (int iHit = 0; iHit < V_pid->size(); iHit++){
			v_pid = (*V_pid)[iHit];
			if ((*V_volName)[iHit]!="DS"||v_pid!=PID) continue;
			passed = true;
			deltaT_passed = (*V_t)[iHit]-time0;
			if (fabs(PID)==211){
				weight_passed *= exp(-deltaT_passed/pion_life/gamma);
			}
			if (withCollPosition){
				v_x = (*V_x)[iHit]*10;
				v_y = (*V_y)[iHit]*10;
				v_z = (*V_z)[iHit]*10;
				v_px = (*V_px)[iHit]*1000;
				v_py = (*V_py)[iHit]*1000;
				v_pz = (*V_pz)[iHit]*1000;
				v_t = (*V_t)[iHit];
				t2->Fill();
			}
			else{
				break;
			}
		}

		// stop
		double pa = sqrt(px*px+py*py+pz*pz);
		double r = -1;
		if ( (*McTruth_pid)[0] == PID ){
			h01->Fill(pa,y,weight_initial);
			h1_1->Fill(pa,weight_initial);
			h2_1->Fill(y,weight_initial);
			h20->Fill(time,weight_initial);
			if (passed){
				nPassed+=weight_passed;
				if (pa>75) nPassedH+=weight_passed;
				h02->Fill(pa,y,weight_passed);
				h1_2->Fill(pa,weight_passed);
				h2_2->Fill(y,weight_passed);
				h30->Fill(time,weight_passed);
			}
		}
		for (int iHit = 0; iHit< T_nHits; iHit++){
			stopped = false;
			if (withStopPosition){
				if ((*T_volName)[iHit]!=theVolume) continue;
				int st_pid = (*T_pid)[iHit];
				if (st_pid==PID) stopped=true;
				Ox = (*T_Ox)[iHit]*10;
				Oy = (*T_Oy)[iHit]*10;
				Oz = (*T_Oz)[iHit]*10;
				Ot = (*T_Ot)[iHit];
				deltaT_stopped = Ot-time0;
				if (fabs(PID)==211){
					weight_stopped *= exp(-deltaT_stopped/pion_life/gamma);
				}
				if (fabs(PID)==211){
					Ot = (Ot+fPionDecay->GetRandom());
				}
				else if (fabs(PID)==13){
					if (theVolume=="A9Container"){
						Ot = (Ot+fMuonDecay_He->GetRandom());
					}
					else if (theVolume=="Target"){
						Ot = (Ot+fMuonDecay_Al->GetRandom());
					}
				}
				r = sqrt(Ox*Ox+Oy*Oy);
			}
			else{
				stopped=true;
			}
			if (stopped){
				nStopped+=weight_stopped;
				if ( (*McTruth_pid)[0] == PID ){
					h03->Fill(pa,y,weight_stopped);
					h1_3->Fill(pa,weight_stopped);
					h2_3->Fill(y,weight_stopped);
					h40->Fill(time,weight_stopped);
					if (withStopPosition){
						h04->Fill(r,weight_stopped);
						h05->Fill(Oz,weight_stopped);
					}
					h10->Fill(fmod((*T_Ot)[0],PulseInterval),weight_stopped);
					h11->Fill(fmod(Ot+random.Uniform(100)-50,PulseInterval),weight_stopped);
				}
				if (withStopPosition){
					weight = weight_stopped;
					t->Fill();
				}
			}
		}
	}
	std::cout<<"nProton = "<<nProton<<std::endl;
	std::cout<<"nStopped = "<<nStopped<<std::endl;
	nPassedH/=nProton;
	nPassed/=nProton;
	nStopped/=nProton;
	h01->Scale(1/nProton);
	h02->Scale(1/nProton);
	h03->Scale(1/nProton);
	h04->Scale(1/nProton);
	h05->Scale(1/nProton);
	h10->Scale(1/nProton);
	h11->Scale(1/nProton);
	h20->Scale(1/nProton);
	h30->Scale(1/nProton);
	h40->Scale(1/nProton);
	h1_1->Scale(1/nProton);
	h1_2->Scale(1/nProton);
	h1_3->Scale(1/nProton);
	h2_1->Scale(1/nProton);
	h2_2->Scale(1/nProton);
	h2_3->Scale(1/nProton);
	h01->Write();
	h02->Write();
	h03->Write();
	h04->Write();
	h05->Write();
	h10->Write();
	h11->Write();
	h20->Write();
	h30->Write();
	h40->Write();
	h1_1->Write();
	h1_2->Write();
	h1_3->Write();
	h2_1->Write();
	h2_2->Write();
	h2_3->Write();
	if (withStopPosition){
		t->Write();
	}

	TH1D *h12 = new TH1D("h12",par+" Decayed/Captured in Time Window (After Smear)",200,0,PulseInterval);
	h12->GetXaxis()->SetTitle("Left End of Time Window (ns)");
	h12->GetYaxis()->SetTitle(par+" Stopped / P^{+} in Time Window");
	h12->GetYaxis()->SetTitleOffset(1.5);
	int BinMax = h11->FindBin(time_right);
	for (int i = 1; i<=BinMax; i++){
		double n = h11->Integral(i,BinMax);
		h12->SetBinContent(i,n);
	}
	h12->Write();

	TPaveText *sum = new TPaveText(0.5,0.75,0.9,0.9,"brNDC");
	sum->SetName("sum");
	sum->AddText(option);
	buff.str("");
	buff.clear();
	buff<<"N_{"<<par<<"_{pass}}/N_{p^{+}} = "<<nPassed<<std::endl;
	sum->AddText(buff.str().c_str());
	if (parName=="mu"){
		buff.str("");
		buff.clear();
		buff<<"N_{"<<par<<"_{pass}>75MeV/c}/N_{p^{+}} = "<<nPassedH<<std::endl;
		sum->AddText(buff.str().c_str());
	}
	buff.str("");
	buff.clear();
	buff<<"N_{"<<par<<"_{stop}}/N_{p^{+}} = "<<nStopped<<std::endl;
	sum->AddText(buff.str().c_str());
	sum->Write();

//	f->Close();
	if (withCollPosition){
		f = new TFile("Coll."+runName+".output.root","RECREATE");
		t2->Write();
	}

	TPaveText *info = new TPaveText(0.2,0.5,0.8,0.7,"brNDC");
	info->SetName("Info");
	buff.str("");
	buff.clear();
	buff<<"Seperation of Proton Bunches: "<<tSep<<" ns."<<std::endl;
	info->AddText(buff.str().c_str());
	buff.str("");
	buff.clear();
	buff<<"Proton Pulse Shape: 100 ns squre wave"<<std::endl;
	info->AddText(buff.str().c_str());
	buff.str("");
	buff.clear();
	buff<<"Right End of Time Window: "<<time_right<<" ns."<<std::endl;
	info->AddText(buff.str().c_str());
	buff.str("");
	buff.clear();
	buff<<"N_{"<<par<<"_{In Window}} = "<<h12->GetBinContent(h12->FindBin(time_left))<<", A_{Time} = "<<h12->GetBinContent(h12->FindBin(time_left))/nStopped<<std::endl;
	info->AddText(buff.str().c_str());

//*************************************************************************************
	gStyle->SetPalette(1);
	gStyle->SetOptStat(0);
	gStyle->SetPadTickX(1);
	gStyle->SetPadTickY(1);

	// Draw pa VS y
	TCanvas *c1 = new TCanvas("c","c",1024,768);
	TPad * p1 = new TPad("p1","p1",0,0,1./3,1);
	TPad * p2 = new TPad("p2","p2",1./3,0,2./3,1);
	TPad * p3 = new TPad("p3","p3",2./3,0,3./3,1);
//	p1->Draw();
//	p2->Draw();
//	p3->Draw();
	h01->SetContour(50);
	h02->SetContour(50);
	h03->SetContour(50);
	p1->cd();
	p1->SetGridx(1);
	p1->SetGridy(1);
//	h01->Draw("CONT0 COLZ");
	p2->cd();
	p2->SetGridx(1);
	p2->SetGridy(1);
//	h02->Draw("CONT0 COLZ");
	p3->cd();
	p3->SetGridx(1);
	p3->SetGridy(1);
//	h03->Draw("CONT0 COLZ");
//	sum->Draw();
	c1->SaveAs(runName+".paVSy.pdf");
	c1->SaveAs(runName+".paVSy.png");

	// Draw stop position
	if (withStopPosition){
		TCanvas *c2 = new TCanvas("c2","c2",1024,768);
		TPad * p4 = new TPad("p4","p4",0,0,1./2,1);
		TPad * p5 = new TPad("p5","p5",1./2,0,2./2,1);
//		p4->Draw();
//		p5->Draw();
		p4->cd();
		p4->SetGridx(1);
		p4->SetGridy(1);
//		h04->Draw();
		p5->cd();
		p5->SetGridx(1);
		p5->SetGridy(1);
//		h05->Draw();
		c2->SaveAs(runName+".SP.pdf");
		c2->SaveAs(runName+".SP.png");
	}

	// Draw pa y and t
	TCanvas *c3 = new TCanvas("c3","c3",1024,768);
	TPad *apad = new TPad("pa","pa",0,0,1./3,1);
	TPad *bpad = new TPad("pb","pb",1./3,0,2./3,1);
	TPad *cpad = new TPad("pc","pc",2./3,0,1,1);
//	apad->Draw();
//	bpad->Draw();
//	cpad->Draw();

	apad->cd();
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	h1_1->SetMarkerColor(1);
	h1_1->SetLineColor(1);
//	h1_1->Draw();
	h1_1->Print();
	h1_2->SetMarkerColor(600);
	h1_2->SetLineColor(600);
//	h1_2->Draw("SAME");
	h1_3->SetMarkerColor(632);
	h1_3->SetLineColor(632);
//	h1_3->Draw("SAME");

	bpad->cd();
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	h2_1->SetMarkerColor(1);
	h2_1->SetLineColor(1);
//	h2_1->Draw();
	h2_2->SetMarkerColor(600);
	h2_2->SetLineColor(600);
//	h2_2->Draw("SAME");
	h2_3->SetMarkerColor(632);
	h2_3->SetLineColor(632);
//n	h2_3->Draw("SAME");
	c3->SaveAs(runName+".pa_y.pdf");
	c3->SaveAs(runName+".pa_y.png");

	cpad->cd();
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	gPad->SetLogy(1);
	h20->SetMarkerColor(1);
	h20->SetLineColor(1);
//	h20->Draw();
	h30->SetMarkerColor(600);
	h30->SetLineColor(600);
//	h30->Draw("SAME");
	h40->SetMarkerColor(632);
	h40->SetLineColor(632);
//	h40->Draw("SAME");
	legend = new TLegend(0.5,0.6,0.9,0.75);
	legend->SetTextSize(0.03);
	legend->AddEntry(h20,par+" Before BS Coil");
	legend->AddEntry(h30,par+" After BS Coil");
	legend->AddEntry(h40,par+" Stopped in Target");
//	legend->Draw("SAME");
//	sum->Draw();
	c3->SaveAs(runName+".pa_y.pdf");
	c3->SaveAs(runName+".pa_y.png");

	// Draw Time Distribution
	TCanvas *c4 = new TCanvas("c4","c4",1024,768);
	apad = new TPad("pa2","pa2",0,0,1./3,1);
	bpad = new TPad("pb2","pb2",1./3,0,2./3,1);
	cpad = new TPad("pc2","pc2",2./3,0,1,1);
//	apad->Draw();
//	bpad->Draw();
//	cpad->Draw();
//	apad->Draw();
//	bpad->Draw();
//	cpad->Draw();

	apad->cd();
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	gPad->SetLogy(1);
	h10->GetYaxis()->SetRangeUser(minimum,h10->GetMaximum()*2);
//	h10->Draw();

	bpad->cd();
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	gPad->SetLogy(1);
	h11->GetYaxis()->SetRangeUser(minimum,h11->GetMaximum()*2);
//	h11->Draw();
//	info->Draw();

	cpad->cd();
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	gPad->SetLogy(1);
	h12->GetYaxis()->SetRangeUser(minimum,h12->GetMaximum()*2);
//	h12->Draw();
	c4->SaveAs(runName+".time.pdf");
	c4->SaveAs(runName+".time.png");

	return 0;
}
