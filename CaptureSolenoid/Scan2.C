{
//	TFile * f = new TFile("/home/chen/MyWorkArea/Simulate/comet/output/testCapture.Scan.root");
//	TTree * it = (TTree*) f->Get("tree");
	TChain * c = new TChain("tree");
	c->Add("/home/chen/MyWorkArea/Simulate/comet/output/production2.root");

	std::vector<double> * V_x;
	std::vector<double> * V_y;
	std::vector<double> * V_z;
	std::vector<double> * V_px;
	std::vector<double> * V_py;
	std::vector<double> * V_pz;
	std::vector<int> * V_pid;

	c->SetBranchAddress("V_pid",&V_pid);
	c->SetBranchAddress("V_x",&V_x);
	c->SetBranchAddress("V_y",&V_y);
	c->SetBranchAddress("V_z",&V_z);
	c->SetBranchAddress("V_px",&V_px);
	c->SetBranchAddress("V_py",&V_py);
	c->SetBranchAddress("V_pz",&V_pz);

	TH2D * h1 = new TH2D("h1","Distribution of #pi^{-} from the production target",64,7300,8000,64,0,180);
	h1->GetXaxis()->SetTitle("x [mm]");
	h1->GetYaxis()->SetTitle("r [mm]");
	TH2D * h2 = new TH2D("h2","Distribution of #pi^{-} from the production target",64,0,140,64,-1,1);
	h2->GetXaxis()->SetTitle("p_{tot} [MeV/c]");
	h2->GetYaxis()->SetTitle("cos(#theta)");

	for ( Long64_t iEvent = 0; iEvent<c->GetEntries(); iEvent++){
		c->GetEntry(iEvent);
		for ( int i = 0; i<V_x->size(); i++){
			double px = (*V_px)[i];
			double py = (*V_py)[i];
			double pz = (*V_pz)[i];
			double x = (*V_x)[i];
			double y = (*V_y)[i];
			double z = (*V_z)[i];
			int pid = (*V_pid)[i];
			if (pid!=-211) continue;
			if (x<730||x>800) continue;
			if (sqrt(px*px+py*py+pz*pz)*1000>140) continue;
			h1->Fill(x*10,sqrt(z*z+y*y)*10,1/sqrt(z*z+y*y));
			h2->Fill(sqrt(px*px+py*py+pz*pz)*1000,px/sqrt(px*px+py*py+pz*pz));
		}
	}
}
