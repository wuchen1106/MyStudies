{
	TFile * fin = new TFile("input.root");
	TTree * tin = (TTree*) fin->Get("tree");
	double x;
	double y;
	double z;
	double px;
	double py;
	double pz;
	std::vector<double> *vx;
	std::vector<double> *vy;
	std::vector<double> *vz;
	std::vector<double> *vpx;
	std::vector<double> *vpy;
	std::vector<double> *vpz;
	tin->SetBranchAddress("ProcessCounting_preX",&vx);
	tin->SetBranchAddress("ProcessCounting_preY",&vy);
	tin->SetBranchAddress("ProcessCounting_preZ",&vz);
	tin->SetBranchAddress("ProcessCounting_prePx",&vpx);
	tin->SetBranchAddress("ProcessCounting_prePy",&vpy);
	tin->SetBranchAddress("ProcessCounting_prePz",&vpz);
	TFile * fout = new TFile("StepPosition.root","RECREATE");
	TTree * tout = new TTree("t","t");
	tout->Branch("x",&x);
	tout->Branch("y",&y);
	tout->Branch("z",&z);
	tout->Branch("px",&px);
	tout->Branch("py",&py);
	tout->Branch("pz",&pz);
	int nEvents = tin->GetEntries();
	for (int iEvent = 0; iEvent<nEvents; iEvent++){
		tin->GetEntry(iEvent);
		for (int j = 0; j < vx->size(); j++){
			x = (*vx)[j]*10;
			y = (*vy)[j]*10;
			z = (*vz)[j]*10;
			px = (*vpx)[j]*1000;
			py = (*vpy)[j]*1000;
			pz = (*vpz)[j]*1000;
			tout->Fill();
		}
	}
	tout->Write();
}
