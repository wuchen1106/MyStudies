{
//	TFile * f = new TFile("/home/chen/MyWorkArea/Simulate/comet/output/testCapture.Scan.root");
//	TTree * it = (TTree*) f->Get("tree");
	TChain * c = new TChain("tree");
	std::stringstream buf;
	for(int i = 0; i < 200; i++){
	//for(int i = 0; i < 1; i++){
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
	int V_nHits;

	c->SetBranchAddress("McTruth_x",&McTruth_x);
	c->SetBranchAddress("McTruth_y",&McTruth_y);
	c->SetBranchAddress("McTruth_z",&McTruth_z);
	c->SetBranchAddress("McTruth_px",&McTruth_px);
	c->SetBranchAddress("McTruth_py",&McTruth_py);
	c->SetBranchAddress("McTruth_pz",&McTruth_pz);
	c->SetBranchAddress("T_nHits",&T_nHits);
	c->SetBranchAddress("V_nHits",&V_nHits);
	c->SetBranchAddress("V_volName",&V_volName);
	c->SetBranchAddress("V_pid",&V_pid);

	TFile * of = new TFile("output.root","RECREATE");
	TTree * ot = new TTree("t","t");
	double x,y,z,px,py,pz;
	int type;
	t->Branch("x",&x);
	t->Branch("y",&y);
	t->Branch("z",&z);
	t->Branch("px",&px);
	t->Branch("py",&py);
	t->Branch("pz",&pz);
	t->Branch("type",&type);

	TH2D * h1[5];
	TH2D * h2[5];
	for (int i = 0; i<5; i++){
		TString vol;
		if (i==0) vol = "CS";
		else if (i==1) vol = "3M";
		else if (i==2) vol = "TS1";
		else if (i==3) vol = "TS2";
		else if (i==4) vol = "Target (stop)";
		h1[i] = new TH2D(Form("h1%d",i),"Probability of a #pi^{-} to generate a muon or pion on "+vol,256,7300,8000,256,0,180);
		h1[i]->GetXaxis()->SetTitle("x [mm]");
		h1[i]->GetYaxis()->SetTitle("r [mm]");
		h2[i] = new TH2D(Form("h2%d",i),"Probability of a #pi^{-} to generate a muon or pion on "+vol,256,0,140,256,-1,1);
		h2[i]->GetXaxis()->SetTitle("p_{tot} [MeV/c]");
		h2[i]->GetYaxis()->SetTitle("cos(#theta)");
	}

	for ( Long64_t iEvent = 0; iEvent<c->GetEntries(); iEvent++){
		c->GetEntry(iEvent);
		type = 0;
		if (T_nHits>0) type = 5;
		else{
			for (int i = 0; i<V_nHits; i++){
				if (((*V_pid)[i]==13||(*V_pid)[i]==-211)){
					if(type<1&&(*V_volName)[i]=="CS")
						type = 1;
					else if(type<2&&(*V_volName)[i]=="3M")
						type = 2;
					else if(type<3&&(*V_volName)[i]=="TS1")
						type = 3;
					else if(type<4&&(*V_volName)[i]=="TS2")
						type = 4;
				}
			}
		}
		if (!type) continue;
		px = (*McTruth_px)[0]*1000;
		py = (*McTruth_py)[0]*1000;
		pz = (*McTruth_pz)[0]*1000;
		x = (*McTruth_x)[0]*10;
		y = (*McTruth_y)[0]*10;
		z = (*McTruth_z)[0]*10;
		for (int i = 0; i<type; i++){
			h1[i]->Fill(x,sqrt(z*z+y*y),1/sqrt(z*z+y*y));
			h2[i]->Fill(sqrt(px*px+py*py+pz*pz),px/sqrt(px*px+py*py+pz*pz));
		}
		ot->Fill();
	}
	for (int i = 0; i<5; i++){
		h1[i]->Scale(1./(1.e6*198/200)*64*64);
		h2[i]->Scale(1./(1.e6*198/200)*64*64);
		h1[i]->Write();
		h2[i]->Write();
	}
	//ot->Write();
}
