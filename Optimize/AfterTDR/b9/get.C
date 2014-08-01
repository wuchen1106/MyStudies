{
	TFile *f = 0;

	TChain *c = new TChain("tree");

	TString RunName = "Ti100";
	std::stringstream buff;
	for (int i = 0; i<30; i++){
		buff.str("");
		buff.clear();
		buff<<"/home/chen/MyWorkArea/Data/raw/g4sim/MT0.EP.EP.g60cm10mm.005T."<<RunName<<".g4s.QBH/"<<i<<"_job0.raw";
		c->Add(buff.str().c_str());
	}

	int PID = 1;

	int pid = 0;
	std::vector<double> *V_x;
	std::vector<double> *V_y;
	std::vector<double> *V_z;
	std::vector<double> *V_t;
	std::vector<double> *V_px;
	std::vector<double> *V_py;
	std::vector<double> *V_pz;
	std::vector<int> *V_pid;
	int V_nHits;
	double weight = 1;

	c->SetBranchAddress("V_nHits",&V_nHits);
//	c->SetBranchAddress("weight",&weight);
	c->SetBranchAddress("V_pid",&V_pid);
	c->SetBranchAddress("V_px",&V_px);
	c->SetBranchAddress("V_py",&V_py);
	c->SetBranchAddress("V_pz",&V_pz);
	c->SetBranchAddress("V_t",&V_t);
	c->SetBranchAddress("V_x",&V_x);
	c->SetBranchAddress("V_y",&V_y);
	c->SetBranchAddress("V_z",&V_z);

	double x = 0;
	double y = 0;
	double z = 0;
	double time = 0;
	double px = 0;
	double py = 0;
	double pz = 0;
	f = new TFile("output.root","RECREATE");
	TTree *t  = new TTree("t","t");
	t->Branch("x",&x);
	t->Branch("y",&y);
	t->Branch("z",&z);
	t->Branch("t",&time);
	t->Branch("px",&px);
	t->Branch("py",&py);
	t->Branch("pz",&pz);
	t->Branch("pid",&pid);
	t->Branch("weight",&weight);

	TH1D * h1 = new TH1D("h1","Momentum @ 45^{o} ("+RunName+")",100,0,250);
	h1->GetXaxis()->SetTitle("Momentum (MeV/c)");
	h1->GetYaxis()->SetTitle("Count per proton");

	int nEvents = c->GetEntries();
	std::cout<<nEvents<<" events!!!"<<std::endl;
	for (int iEvent = 0; iEvent < nEvents; iEvent++ ){
		if (iEvent%1000==0) std::cout<<(double)iEvent/nEvents*100<<" % ..."<<std::endl;
		c->GetEntry(iEvent);
		if (V_nHits<=0) continue;
		if ((*V_pid)[0]!=13&&(*V_pid)[0]!=-211) continue;
	//	if (PID&&PID!=1&&(*V_pid)[0]!=PID) continue;
	//	if (PID==1&&(*V_pid)[0]>1e6) continue;
		if ((*V_x)[0]>200) continue; // Don't need PTACS
		pid = (*V_pid)[0];
		x = (*V_x)[0]*10;
		y = (*V_y)[0]*10;
		z = (*V_z)[0]*10;
		time = (*V_t)[0]*10;
		px = (*V_px)[0]*1000;
		py = (*V_py)[0]*1000;
		pz = (*V_pz)[0]*1000;
		h1->Fill(sqrt(px*px+py*py+pz*pz),weight/1e7);
		if (iEvent%1000==0) std::cout<<"  h1->Fill("<<sqrt(px*px+py*py+pz*pz)<<", "<<weight/1e6<<")"<<std::endl;
		t->Fill();
	}
	h1->Print();
	h1->Write();
	t->Write();
}
