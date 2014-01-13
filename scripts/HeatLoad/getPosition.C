{
	double npPerBuntch = 2.5E12*1.17e-6;
	double density = 19.3; // g/cm3
	double squre = 3.1415926*(20*20-14*14);
	TFile *f = new TFile("output.root","RECREATE");
	TChain *c1 = new TChain("tree");
	TChain *c2 = new TChain("tree");
	TChain *c3 = new TChain("tree");
	TChain *c4 = new TChain("tree");
	c1->Add("CDC.EP_OT.g40cm10mm.005T.Vac0928_140_140_39.g4s.QBH/0_job0.raw");
	c2->Add("CDC.EP_em.g40cm10mm.005T.Vac0928_140_140_39.g4s.QBH/0_job0.raw");
	c3->Add("CDC.EP_mum.g40cm10mm.005T.Vac0928_140_140_39.g4s.QBH/0_job0.raw");
	c4->Add("CDC.pim.g40cm10mm.005T.Vac0928_140_140_39.g4s.QBH/0_job0.raw");
	/*
	TH2D *h1 = new TH2D("h1","Heat Load (GeV/g) In At The Front End of The Collimator for One Bunch (2.9*10^{6} p^{+}s)",400,-20,20,400,-20,20);
	h1->GetXaxis()->SetTitle("x (cm)");
	h1->GetYaxis()->SetTitle("y (cm)");
	TH1D *h2 = new TH1D("h2","Heat Load Along The Collimator for One Bunch (2.9*10^{6} p^{+}s)",110,-230,-120);
	h2->GetXaxis()->SetTitle("z (cm)");
	h2->GetYaxis()->SetTitle("Energy Deposition (GeV)");
	std::vector<double> *vedep;
	std::vector<double> *vx;
	std::vector<double> *vy;
	std::vector<double> *vz;
	std::vector<double> *vt;
	std::vector<int> *vpid;
	c->SetBranchAddress("BLTCollimator_edep",&vedep);
	c->SetBranchAddress("BLTCollimator_x",&vx);
	c->SetBranchAddress("BLTCollimator_y",&vy);
	c->SetBranchAddress("BLTCollimator_z",&vz);
	c->SetBranchAddress("BLTCollimator_t",&vt);
	c->SetBranchAddress("BLTCollimator_pid",&vpid);
	std::vector<int> pids;
	std::vector<double> edeps;
	pids.push_back(-211); // pi-
	pids.push_back(13);   // mu-
	pids.push_back(11);   // e-
	pids.push_back(211);  // pi+
	pids.push_back(-13);  // mu+
	pids.push_back(-11);  // e+
	pids.push_back(2112); // neutron
	pids.push_back(2212); // proton
	pids.push_back(22);   // gamma
	for (int i = 0; i<pids.size(); i++){
		edeps.push_back(0);
	}
	double total = 0;
	int nEvents = c->GetEntries();
	std::cout<<nEvents<<" Events inside"<<std::endl;
	for (int iEvent  = 0; iEvent<nEvents; iEvent++){
		c->GetEntry(iEvent);
		if (iEvent%1000==0){
			std::cout<<(double)iEvent/nEvents*100<<"% ..."<<std::endl;
		}
		for (int i = 0; i< vedep->size(); i++ ){
			double edep = (*vedep)[i];
			double x = (*vx)[i];
			double y = (*vy)[i];
			double z = (*vz)[i];
			double t = (*vt)[i];
			int pid = (*vpid)[i];
			if (z>=-227.46&&z<-226.46){
				h1->Fill(x,y,edep/(density*1e-2));
			}
			h2->Fill(z,edep);
			for (int j = 0; j<pids.size(); j++){
				if (pid==pids[j]) edeps[j] += edep;
			}
			total += edep;
		}
	}
	h1->Scale(npPerBuntch/1e8);
	h2->Scale(npPerBuntch/1e8);
	h1->Write();
	h2->Write();
	std::cout<<"##Breack Down:"<<std::endl;
	std::cout<<"total: "<<total*npPerBuntch/1e8<<"GeV"<<std::endl;
	for (int i = 0; i<pids.size(); i++){
		std::cout<<pids[i]<<": "<<edeps[i]*npPerBuntch/1e8<<"GeV"<<std::endl;
	}
	*/
}
