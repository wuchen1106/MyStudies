{
//	TFile * f = new TFile("/home/chen/MyWorkArea/Simulate/comet/output/testCapture.Scan.root");
//	TTree * it = (TTree*) f->Get("tree");
	TChain * c = new TChain("tree");
	std::stringstream buf;
	for(int i = 0; i < 200; i++){
		buf.str(""); buf.clear();
		buf<<"/home/chen/MyWorkArea/Data/testCapture.Scan/"<<i<<"_job0.raw";
		c->Add(buf.str().c_str());
	}

	std::vector<double> * McTruth_x;
	std::vector<double> * McTruth_y;
	std::vector<double> * McTruth_z;
	std::vector<double> * McTruth_px;
	std::vector<double> * McTruth_py;
	std::vector<double> * McTruth_pz;

	std::vector<std::string> * V_volName;
	std::vector<int> * V_pid;
	int T_nHits;

	c->SetBranchAddress("McTruth_x",&McTruth_x);
	c->SetBranchAddress("McTruth_y",&McTruth_y);
	c->SetBranchAddress("McTruth_z",&McTruth_z);
	c->SetBranchAddress("McTruth_px",&McTruth_px);
	c->SetBranchAddress("McTruth_py",&McTruth_py);
	c->SetBranchAddress("McTruth_pz",&McTruth_pz);
	c->SetBranchAddress("T_nHits",&T_nHits);

	TH2D * h1 = new TH2D("h1","Probability of a #pi^{-} to generate a stopped muon in the target",64,7300,8000,64,0,180);
	h1->GetXaxis()->SetTitle("x [mm]");
	h1->GetYaxis()->SetTitle("r [mm]");
	TH2D * h2 = new TH2D("h2","Probability of a #pi^{-} to generate a stopped muon in the target",64,0,140,64,-1,1);
	h2->GetXaxis()->SetTitle("p_{tot} [MeV/c]");
	h2->GetYaxis()->SetTitle("cos(#theta)");

	for ( Long64_t iEvent = 0; iEvent<c->GetEntries(); iEvent++){
		c->GetEntry(iEvent);
		if (T_nHits<=0) continue;
		double px = (*McTruth_px)[0];
		double py = (*McTruth_py)[0];
		double pz = (*McTruth_pz)[0];
		double x = (*McTruth_x)[0];
		double y = (*McTruth_y)[0];
		double z = (*McTruth_z)[0];
		h1->Fill(x*10,sqrt(z*z+y*y)*10,1/sqrt(z*z+y*y));
		h2->Fill(sqrt(px*px+py*py+pz*pz)*1000,px/sqrt(px*px+py*py+pz*pz));
	}
	h1->Scale(1./(1.e6*198/200)*64*64);
	h2->Scale(1./(1.e6*198/200)*64*64);
}
