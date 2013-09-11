{
	TChain *c = new TChain("t");
	c->Add("/home/chen/MyWorkArea/g4sim/data/CDC.EP_mum.ST.g40cm10mm.005T.Vac0731_200_200_39.g4s.QBH.root");
//	c->Add("/home/chen/MyWorkArea/MyStudies/RMC/result/MuonStop/Chen/output.root");
	double t;
	c->SetBranchAddress("t",&t);
	int nEvents = c->GetEntries();
	TH1D *h1 = new TH1D("h1","h1",200,-500,2000);
	for (int iEvent = 0; iEvent<nEvents; iEvent++){
		c->GetEntry(iEvent);
		double deltat = gRandom->Gaus()*100;
		double decayTime = -864*log(gRandom->Uniform());
		h1->Fill(t+deltat+decayTime);
//		h1->Fill(t+deltat);
	}
	TH1D *h2 = new TH1D("h2","h2",200,-500,2000);
	double N = h1->Integral();
	for (int i = 1; i<=200; i++){
		double n = h1->Integral(i,200);
		h2->SetBinContent(i,n/N);
	}
}
