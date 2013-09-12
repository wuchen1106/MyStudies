{
	TChain *c = new TChain("t");
	c->Add("/home/chen/MyWorkArea/g4sim/data/CDC.EP_mum.ST.g40cm10mm.005T.Vac0731_200_200_39.g4s.QBH.root");
//	c->Add("/home/chen/MyWorkArea/g4sim/data/CDC.pim.ST.g40cm10mm.005T.Vac0731_200_200_39.g4s.QBH.root");
//	c->Add("/home/chen/MyWorkArea/MyStudies/RMC/result/MuonStop/Chen/output.root");

	double t;
	double weight;
	c->SetBranchAddress("t",&t);
	c->SetBranchAddress("weight",&weight);
	int nEvents = c->GetEntries();
	TH1D *h1 = new TH1D("h1","h1",200,0,1751);
	for (int iEvent = 0; iEvent<nEvents; iEvent++){
		c->GetEntry(iEvent);
		double decayTime = -864*log(gRandom->Uniform());
		t+=decayTime;
		if (iEvent%100==0) std::cout<<((double)iEvent)/nEvents*100<<"%..."<<std::endl;
		for (int ibin = 1; ibin <= 200; ibin++){
			double time = h1->GetBinCenter(ibin);
			for (int j = 0; j <= 2; j++){
				double timep = (j-1)*1751+time;
				double w = TMath::Gaus(timep-t,0,100,kTRUE)*1751./200;
				h1->AddBinContent(ibin,w*weight);
			}
		}
	}
	h1->Scale(1./1e9);

//	TFile *f = new TFile("output.root");
//	TH1D* h1 = f.Get("h1");

	double N = h1->Integral();
//	double N = 1;
	TH1D *h2 = new TH1D("h2","h2",200,0,1751);
	for (int i = 1; i<=121; i++){
		double n = h1->Integral(i,121);
		h2->SetBinContent(i,n/N);
	}
	TH1D *h3 = new TH1D("h3","h3",200,0,1751);
	for (int i = 122; i<=200; i++){
		double n = h1->Integral(122,i);
		h3->SetBinContent(i,n/N);
	}
}
