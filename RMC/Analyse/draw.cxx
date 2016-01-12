#include <iostream>
#include <math.h>

#include "TStyle.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TLegend.h"
#include "TH1D.h"
#include "TF1.h"
#include "TH1F.h"
#include "TChain.h"

int main(int argc, char ** argv){
	double NmuStop = 1.02e16;
	double eff_geom =  0.348*0.73*0.6641;
	double eff_other = 0.9*0.8*0.8*0.4862;

	TCanvas * canv = new TCanvas("c","c",1024,768);
	gStyle->SetPalette(1);
	gStyle->SetOptStat(0);
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	gPad->SetTickx(1);
	gPad->SetTicky(1);
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
	TH1D * hrmce = new TH1D("hrmce","hrmce",1040,0.95,104.95);
	double binwidth = hrmce->GetBinWidth(1);
	TFile * infile2 = new TFile("../../../Simulate/comet/data/RMC_Al_bind.root");
	TF1 * frmc = (TF1*)infile2->Get("f1");
	TFile * infile3 = new TFile("../../../Simulate/comet/data/DIO.root");
	TH1F * hdio = (TH1F*)infile3->Get("h1");
	TF1 * fdio_raw = (TF1*)infile2->Get("f1");
	TF1 * fdio = new TF1("fdio",Form("f1/1056.584*%d",NmuStop*0.39*eff_other*eff_geom),57,104.973);

//	TFile * infile4 = new TFile("../../DIO/mom_p_63um_102MeV.root");
//	TH1D * hreso = (TH1D*)infile4->Get("hp_63um_102MeV");
//	hreso->Scale(1./hreso->Integral());
	TFile * infile4 = new TFile("../../DIO/histall_2225.root");
	TH1F * hreso = (TH1F*)infile4->Get("fit1_res_pa");
	TH1F * hfit = (TH1F*)infile4->Get("fit1_fit_pa");
	hreso->Scale(1./hreso->Integral());
	hfit->Scale(1./hfit->Integral());

	double Nperbin = 4.27867e9*binwidth/10;
	for (Long64_t i = 0; i<c->GetEntries(); i++){
		if (i%10000==0) std::cout<<i<<std::endl;
		c->GetEntry(i);
		double pa = sqrt(px*px+py*py+pz*pz);
		double ipa = sqrt(ipx*ipx+ipy*ipy+ipz*ipz);
		int ibin = hrmce->FindBin(ipa);
		double pamin = hrmce->GetBinLowEdge(ibin);
		double pamax = pamin+binwidth;
		if (pamax>101.855) pamax = 101.855;
		if (pamin>101.855) continue;
		double weight = 1/Nperbin*frmc->Integral(pamin,pamax)*1.84e-5; // per moun capture
		hrmce->Fill(pa,weight);
	}
	hrmce->Scale(NmuStop*0.61*eff_other);
	hdio->Scale(NmuStop*0.39*eff_other*eff_geom);

	TH1D * hrmce_sm = new TH1D("hrmce_sm","hrmce_sm",1040,0.95,104.95);
	TH1D * hdio_sm = new TH1D("hdio_sm","hdio_sm",1040,0.95,104.95);
	for (int ibin = 1; ibin<=1040; ibin++){
		if (ibin%100==0) std::cout<<ibin<<std::endl;
		for (int jbin = 1; jbin<=200; jbin++){
			if (ibin-100+jbin>0&&ibin-100+jbin<1041)
				hrmce_sm->AddBinContent(ibin-100+jbin,hrmce->GetBinContent(ibin)*hreso->GetBinContent(jbin));
			if (ibin-150+jbin>0&&ibin-150+jbin<1041)
				hdio_sm->AddBinContent(ibin-150+jbin,hdio->GetBinContent(ibin)*hfit->GetBinContent(jbin/2));
		}
	}

	hrmce->GetXaxis()->SetTitle("Momentum [MeV/c]");
	hrmce_sm->GetXaxis()->SetTitle("Momentum [MeV/c]");
	hdio->GetXaxis()->SetTitle("Momentum [MeV/c]");
	hdio_sm->GetXaxis()->SetTitle("Momentum [MeV/c]");
	hrmce->GetYaxis()->SetTitle("Count/110 Days/100keV/c");
	hrmce_sm->GetYaxis()->SetTitle("Count/110 Days/100keV/c");
	hdio->GetYaxis()->SetTitle("Count/110 Days/100keV/c");
	hdio_sm->GetYaxis()->SetTitle("Count/110 Days/100keV/c");
	hrmce->SetTitle("Electrons Tracks Accepted by CyDet");
	hrmce_sm->SetTitle("Electrons Tracks Accepted by CyDet");
	hdio->SetTitle("Electrons Tracks Accepted by CyDet");
	hdio_sm->SetTitle("Electrons Tracks Accepted by CyDet");
	fdio->SetTitle("Electrons Tracks Accepted by CyDet per 100 keV/c");

//	hrmce->Draw();
//	TLegend * l =new TLegend(0.7,0.7,0.85,0.85);
//	l->AddEntry(hrmce,"RMC");
//	l->AddEntry(hdio,"DIO");
//	l->Draw("SAME");

	TFile * ofile = new TFile("output.root","RECREATE");
	hrmce->Write();
	hdio->Write();
	hrmce_sm->Write();
	hdio_sm->Write();
	hreso->Write();
	hfit->Write();
	fdio->Write();
	ofile->Close();
	return 0;
}
