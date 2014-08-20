{
	TChain *c = new TChain("tree");
	c->Add("/home/chen/MyWorkArea/g4sim/output/reflect_WM.root");

	std::vector<double> *V2_t;
	std::vector<int> *V2_id;
	std::vector<double> *V_t;

	c->SetBranchAddress("V2_t",&V2_t);
	c->SetBranchAddress("V2_volID",&V2_id);
	c->SetBranchAddress("V_t",&V_t);

	TFile * f = new TFile("output.root","RECREATE");
	TTree * t1 = new TTree("t1","t1");
	TTree * t2 = new TTree("t2","t2");

	int id;
	int n;
	double dt;
	t1->Branch("id",&id);
	t1->Branch("n",&n);
	t1->Branch("dt",&dt);

	int tn;
	double tdt;
	double time;
	t2->Branch("dt",&tdt);
	t2->Branch("n",&tn);
	t2->Branch("t",&time);

	int nEvents = c->GetEntries();
	for (int iEvent = 0; iEvent<nEvents; iEvent++){
		c->GetEntry(iEvent);
		int last_id = 0;
		int this_id = 0;
		int start_id = 0;
		int end_id = 0;
		double start_time = 0;
		double end_time = 0;
		bool going_back = false;
		tn = 0;
		tdt = 0;
		for (int i = 0; i<V2_t->size(); i++){
			this_id = (*V2_id)[i];
			if (last_id>this_id&&going_back == false){ // start of reflection
				going_back = true;
				start_id = this_id;
				start_time = this_time;
			}
			if (last_id<this_id&&going_back == true){ // end of reflection
				going_back = false;
				end_id = this_id;
				end_time = this_time;
				id = start_id;
				n = end_id - start_id;
				dt = end_time - start_time;
				tn += n;
				tdt += dt;
				t1->Fill();
			}
			last_id = this_id;
		}
		time = (*V_t)[0];
		t2->Fill();
	}

	t1->Write();
	t2->Write();
}
