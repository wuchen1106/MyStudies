{
	TFile *f = 0;

	TChain *c = new TChain("tree");

	std::stringstream buff;
	c->Add("input.root");

	int pid = 11;
	std::vector<double> *V_x;
	std::vector<double> *V_y;
	std::vector<double> *V_z;
	std::vector<double> *V_t;
	std::vector<double> *V_px;
	std::vector<double> *V_py;
	std::vector<double> *V_pz;
	int V_nHits;
	double weight = 1;

	c->SetBranchAddress("V_nHits",&V_nHits);
	c->SetBranchAddress("weight",&weight);
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
	int nEvents = c->GetEntries();
	std::cout<<nEvents<<" events!!!"<<std::endl;
	for (int iEvent = 0; iEvent < nEvents; iEvent++ ){
		if (iEvent%1000==0) std::cout<<(double)iEvent/nEvents*100<<" % ..."<<std::endl;
		c->GetEntry(iEvent);
		if (V_nHits<=0) continue;
		x = (*V_x)[0]*10;
		y = (*V_y)[0]*10;
		z = (*V_z)[0]*10;
		time = (*V_t)[0]*10;
		px = (*V_px)[0]*1000;
		py = (*V_py)[0]*1000;
		pz = (*V_pz)[0]*1000;
		t->Fill();
	}
	t->Write();
}
