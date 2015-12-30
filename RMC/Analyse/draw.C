{
	TCanvas * canv = new TCanvas("c","c",1024,768);
	gStyle->SetPalette(1);
	gStyle->SetOptStat(0);
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	gPad->SetTickx(1);
	gPad->SetTicky(1);
	TChain * c = new TChain("t","t");
	double px,py,pz;
	c->Add("CyDet.A9.rmc.150919.W500um.OptD2.1mmCFRP.DD35.1cmLead.root");
	c->SetBranchAddress("px",&px);
	c->SetBranchAddress("py",&py);
	c->SetBranchAddress("pz",&pz);
	TH1D * h = new TH1D("h","h",551,49.95,105);
	double binwidth = h->GetBinWidth(1);
	TFile * infile2 = new TFile("../../../Simulate/comet/data/RMC_Al_bind.root");
	TF1 * frmc = (TF1*)infile->Get("f1");
	TFile * ofile = new TFile("output.root","RECREATE");
	for (Long64_t i = 0; i<c->GetEntries(); i++){
		c->GetEntry(i);
		double pa = sqrt(px*px+py*py+pz*pz);
		int ibin = h->FindBin(pa);
		double pamin = h->GetBinLowEdge(ibin);
		double pamax = pamin+binwidth;
		double weight = 1/4.27867e9*frmc->Integral(pamin,pamax)*1.84e-5; // per moun capture
	}
	h->Write();
	//c->Draw("sqrt(px**2+py**2+pz**2)>>h(100,100,105)","sqrt((x-py*10/3)**2+(y+px*10/3)**2)-sqrt(px**2+py**2)/3*10<120");
	//c->Draw("abs(atan(sqrt(px**2+py**2)/pz)*180/3.1415926)>>h(180,40,90)","sqrt((x-py*10/3)**2+(y+px*10/3)**2)-sqrt(px**2+py**2)/3*10<120");
	//c->Draw("nt","sqrt((x-py*10/3)**2+(y+px*10/3)**2)-sqrt(px**2+py**2)/3*10<120");
//	h->Scale(1./4.28e9*0.61*1.29e-8/3.5);
//	h->GetXaxis()->SetTitle("Momentum [MeV/c]");
//	h->GetYaxis()->SetTitle("Count/#mu^{-}_{stop}/50keV/c");
//	h->SetTitle("Electrons Tracks Accepted by CyDet");
//	h->Draw();
//	TFile * infile = new TFile("../../../Simulate/comet/data/DIO.root");
//	TF1 * f1 = (TF1*)infile->Get("f1");
//	f1->SetName("f1");
//	TF1 * ff = new TF1("ff","f1/105.6584*0.39/20*0.23",100,104.973);
//	ff->Draw("SAME");
//	TLegend * l =new TLegend(0.7,0.7,0.85,0.85);
//	l->AddEntry(h,"RMC");
//	l->AddEntry(ff,"DIO");
//	l->Draw("SAME");
}
