#include <iostream>
#include <math.h>

#include "TStyle.h"
#include "TFile.h"
#include "TLegend.h"
#include "TH1D.h"
#include "TF1.h"
#include "TH1F.h"
#include "TChain.h"
#include "TGraph.h"

int main(int argc, char ** argv){

	// =================================================
	// constants about Detector
	double c_EffiTime = 0.2998; // 700ns
	//double c_EffiTime = 0.4862; // 500ns
	double c_EffiDAQ = 0.9;
	double c_EffiTrigger = 0.9;
	double c_EffiRec = 0.8;
	////_______TDR 2014________
	//double c_AccGeom = 0.37;
	//double c_TrackQuality = 0.66;
	//double c_MomSel = 0.93;
	//_______2016.01.04________
	double c_AccGeom = 0.2368;
	double c_TrackQuality = 0.6599;
	double c_MomSel = 0.8905;
	double c_MomLowEdge = 103.6;

	// Physics constants
	double c_Capture = 0.61;
	double c_RMC = 1.83e-5; // Branching ratio from 57 MeV
	double c_Mu2e = 3.1e-15;
	double c_UpperLimit = 1e-2;
	double c_RMCpmax = 101.855;
	double c_DIOpmax = 104.973;

	// Simulation constants
	double c_Margin = 2;
	double c_Nprocess = 100;
	double c_TimePerRMC = c_Margin/c_Nprocess*0.7e-3/3600; // hour
	double c_TimePerDIO = c_Margin/c_Nprocess*50e-3/3600; // hour

	// =================================================
	// How many stopped muons we need to get 1 signal
	//double nMuonStop = 1/c_Mu2e/c_Capture/c_AccGeom/c_TrackQuality/c_MomSel/c_EffiTime/c_EffiDAQ/c_EffiTrigger/c_EffiRec;
	double nMuonStop = 1/c_Mu2e/c_Capture/c_AccGeom/c_TrackQuality/c_MomSel; // Assuming efficiency (same for signal and background) is 100%
	std::cout<<"nMuonStop = " << nMuonStop<<std::endl;

	// =================================================
	// Get Spectrum
	TFile * ifile = new TFile("RMC_Al_bind.root");
	TF1 * fRMC = (TF1*)ifile->Get("f1"); // >57MeV/c Part Scaled to 1
	ifile = new TFile("DIO.root");
	TF1 * fDIO = (TF1*)ifile->Get("f1"); // Whole Range Scaled to 105.6584
	TF1 * fDIO_inRun = new TF1("fDIO_inRun",Form("f1/105.6584/10*%d",nMuonStop*(1-c_Capture)*c_AccGeom*c_TrackQuality),0,c_DIOpmax); // Whole Range Scaled to 1/10 (per 100 keV/c)

	// Get RMC simulaiton result
	TChain * c = new TChain("t","t");
	double px,py,pz;
	double ipx,ipy,ipz;
	c->Add("result/CyDet.A9.rmc.150919.W500um.OptD2.1mmCFRP.DD35.1cmLead.1.root");
	c->Add("result/CyDet.A9.rmc.150919.W500um.OptD2.1mmCFRP.DD35.1cmLead.2.root");
	c->Add("result/CyDet.A9.rmc.150919.W500um.OptD2.1mmCFRP.DD35.1cmLead.3.root");
	c->SetBranchAddress("px",&px);
	c->SetBranchAddress("py",&py);
	c->SetBranchAddress("pz",&pz);
	c->SetBranchAddress("ipx",&ipx);
	c->SetBranchAddress("ipy",&ipy);
	c->SetBranchAddress("ipz",&ipz);

	// =================================================
	// For output
	std::vector<double> vResolution;
	std::vector<double> vTimeDIO;
	std::vector<double> vTimeRMC;
	std::vector<double> vNDIO;
	std::vector<double> vNRMC;
	TH1D * hrmce = new TH1D("hrmce","Electrons from Radiative Muon Capture",1040,0.95,104.95);
	double binwidth = hrmce->GetBinWidth(1);
	TH1D * hdio = new TH1D("hdio",  "Electrons from Muon Decay in Orbit",1040,0.95,104.95);
	for (int ibin = 1; ibin<=1040; ibin++){
		double mom = hdio->GetBinLowEdge(ibin)+binwidth/2.;
		hdio->SetBinContent(ibin,fDIO_inRun->Eval(mom));
	}
	TH1D * hrmce_sm = new TH1D("hrmce_sm","Electrons from Radiative Muon Capture",1040,0.95,104.95);
	TH1D * hdio_sm = new TH1D("hdio_sm",  "Electrons from Muon Decay in Orbit",1040,0.95,104.95);
	TH1D * hrmcecon = new TH1D("hrmcecon","Electrons from Radiative Muon Capture",1040,0.95,104.95);
	TH1D * hdiocon = new TH1D("hdiocon","Electrons from Muon Decay in Orbit",1040,0.95,104.95);

	// =================================================
	// Get Resolution
//	TFile * infile4 = new TFile("mom_p_63um_102MeV.root");
//	TH1D * hreso = (TH1D*)infile4->Get("hp_63um_102MeV");
//	hreso->Scale(1./hreso->Integral());
//	TFile * infile4 = new TFile("hists_2221.root");
	TFile * infile4 = new TFile("histall_2225.root");
	TH1F * hreso = (TH1F*)infile4->Get("fit1_res_pa");
	TH1F * hfit = (TH1F*)infile4->Get("fit1_fit_pa");
	hreso->Scale(1./hreso->Integral());
	hfit->Scale(1./hfit->Integral());

	// How many events to generate?
	for (int i = 0; i<100; i++){
		double lower_DIO = c_MomLowEdge-(100.-i)/100.*10;
		double lower_RMC = lower_DIO-0.511/2; // Boost from Compton Scattering effect
		vResolution.push_back((100.-i)/100.*10);
		double nDIO = nMuonStop/c_UpperLimit*(1-c_Capture)*fDIO_inRun->Integral(lower_DIO,c_DIOpmax)/105.6584;
		double nRMC = nMuonStop/c_UpperLimit*c_Capture*c_RMC*fRMC->Integral(lower_RMC,c_RMCpmax);
		double tDIO = nDIO*c_TimePerDIO;
		double tRMC = nRMC*c_TimePerRMC;
		vTimeDIO.push_back(tDIO);
		vTimeRMC.push_back(tRMC);
		vNDIO.push_back(nDIO);
		vNRMC.push_back(nRMC);
	}

	// =================================================
	// Get RMCE and DIO spectrum
	double Nperbin = 4.27867e9/10*binwidth; // Number of RMC photons generated within each energy bin
	for (Long64_t i = 0; i<c->GetEntries(); i++){
		if (i%10000==0) std::cout<<i<<std::endl;
		c->GetEntry(i);
		double pa = sqrt(px*px+py*py+pz*pz);
		double ipa = sqrt(ipx*ipx+ipy*ipy+ipz*ipz);
		int ibin = hrmce->FindBin(ipa);
		double pamin = hrmce->GetBinLowEdge(ibin);
		double pamax = pamin+binwidth;
		if (pamax>c_RMCpmax) pamax = c_RMCpmax;
		if (pamin>c_RMCpmax) continue;
		double weight = 1./Nperbin*nMuonStop*c_Capture*c_RMC*fRMC->Integral(pamin,pamax); // during whole run
		hrmce->Fill(pa,weight);
	}

	// =================================================
	// Smear with resolution
	for (int ibin = 1; ibin<=1040; ibin++){
		if (ibin%100==0) std::cout<<ibin<<std::endl;
		for (int jbin = 1; jbin<=200; jbin++){
			if (ibin-100+jbin>0&&ibin-100+jbin<1041)
				hrmce_sm->AddBinContent(ibin-100+jbin,hrmce->GetBinContent(ibin)*hreso->GetBinContent(jbin));
			if (ibin-150+jbin>0&&ibin-150+jbin<1041)
				hdio_sm->AddBinContent(ibin-150+jbin,hdio->GetBinContent(ibin)*hfit->GetBinContent(jbin/2));
		}
	}

	// Prepare Output
	hrmce->GetXaxis()->SetTitle("Momentum [MeV/c]");
	hrmce_sm->GetXaxis()->SetTitle("Momentum [MeV/c]");
	hdio_sm->GetXaxis()->SetTitle("Momentum [MeV/c]");
	hrmce->GetYaxis()->SetTitle("Count/110 Days/100keV/c");
	hrmce_sm->GetYaxis()->SetTitle("Count/110 Days/100keV/c");
	hdio_sm->GetYaxis()->SetTitle("Count/110 Days/100keV/c");
	hrmce->SetTitle("Electrons Tracks Accepted by CyDet");
	hrmce_sm->SetTitle("Electrons Tracks Accepted by CyDet");
	hdio_sm->SetTitle("Electrons Tracks Accepted by CyDet");

	// Output
	TGraph * gtRMC = new TGraph(vResolution.size(),&(vResolution[0]),&(vTimeRMC[0]));
	TGraph * gtDIO = new TGraph(vResolution.size(),&(vResolution[0]),&(vTimeDIO[0]));
	TGraph * gnRMC = new TGraph(vResolution.size(),&(vResolution[0]),&(vNRMC[0]));
	TGraph * gnDIO = new TGraph(vResolution.size(),&(vResolution[0]),&(vNDIO[0]));
	gtRMC->SetMarkerStyle(20);
	gtDIO->SetMarkerStyle(20);
	gnRMC->SetMarkerStyle(20);
	gnDIO->SetMarkerStyle(20);
	gtRMC->SetTitle("tRMC");
	gtDIO->SetTitle("tDIO");
	gnRMC->SetTitle("nRMC");
	gnDIO->SetTitle("nDIO");
	gtRMC->SetName("tRMC");
	gtDIO->SetName("tDIO");
	gnRMC->SetName("nRMC");
	gnDIO->SetName("nDIO");

	TFile * ofile = new TFile("output.root","RECREATE");

	fDIO_inRun->Write();
	fRMC->Write();

	gtRMC->Write();
	gtDIO->Write();
	gnRMC->Write();
	gnDIO->Write();

	hrmce->Write();
	hdio->Write();
	hrmce_sm->Write();
	hdio_sm->Write();
	hrmcecon->Write();
	hdiocon->Write();

	hreso->Write();
	hfit->Write();

	ofile->Close();

	return 0;
}
