{
	TChain * c = new TChain("tree");
	c->Add("All.141109.140625ori.gaussian.root");
	std::vector<double> * in_x = 0;
	std::vector<double> * in_y = 0;
	std::vector<double> * in_z = 0;
	std::vector<double> * in_px = 0;
	std::vector<double> * in_py = 0;
	std::vector<double> * in_pz = 0;
	std::vector<double> * in_t = 0;
	std::vector<int> * in_pid = 0;
	int in_type;
	int in_nHits;
	int in_cpid;
	std::vector<int> * in_triType = 0;
	std::vector<int> * in_triID = 0;
	c->SetBranchAddress("x",&in_x);
	c->SetBranchAddress("y",&in_y);
	c->SetBranchAddress("z",&in_z);
	c->SetBranchAddress("px",&in_px);
	c->SetBranchAddress("py",&in_py);
	c->SetBranchAddress("pz",&in_pz);
	c->SetBranchAddress("t",&in_t);
	c->SetBranchAddress("pid",&in_pid);
	c->SetBranchAddress("type",&in_type);
	c->SetBranchAddress("nHits",&in_nHits);
	c->SetBranchAddress("cpid",&in_cpid);
	c->SetBranchAddress("O_triType",&in_triType);
	c->SetBranchAddress("O_triID",&in_triID);
	double out_x;
	double out_y;
	double out_z;
	double out_px;
	double out_py;
	double out_pz;
	double out_t;
	int out_pid;
	TFile * ofile = new TFile("output.root","RECREATE");
	TTree * otree = new TTree("t","t");
	otree->Branch("x",&out_x);
	otree->Branch("y",&out_y);
	otree->Branch("z",&out_z);
	otree->Branch("px",&out_px);
	otree->Branch("py",&out_py);
	otree->Branch("pz",&out_pz);
	otree->Branch("t",&out_t);
	otree->Branch("pid",&out_pid);
	int nHits_sci = 0;
	int nHits_che = 0;
	int sci_id[100];
	int che_id[100];
	Long64_t nEntries = c->GetEntries();
	for (Long64_t iEntry = 0; iEntry<nEntries; iEntry++){
		if (iEntry%1000==0) std::cout<<(double)iEntry/nEntries*100<<"%..."<<std::endl;
		c->GetEntry(iEntry);
		if (fabs(in_cpid)!=11) continue;
		if (in_type!=1) continue;
		if (in_nHits<2) continue;
		nHits_che = 0;
		nHits_sci = 0;
		for ( int ihit = 0; ihit<in_nHits; ihit++){
			if ((*in_triType)[ihit]==0){
				che_id[nHits_sci] = (*in_triID)[ihit];
				nHits_che++;
			}
			else if ((*in_triType)[ihit]==1){
				sci_id[nHits_sci] = (*in_triID)[ihit];
				nHits_sci++;
			}
		}
		bool found = false;
		for (int isci = 0; isci<nHits_sci; isci++){
			for (int iche = 0; iche<nHits_che; iche++){
				if (sci_id[isci]==che_id[iche]){
					found = true;
					break;
				}
			}
		}
		//if (!found) continue;

		out_x = (*in_x)[0];
		out_y = (*in_y)[0];
		out_z = (*in_z)[0];
		out_px = (*in_px)[0];
		out_py = (*in_py)[0];
		out_pz = (*in_pz)[0];
		out_pid = (*in_pid)[0];
		out_t = (*in_t)[0];
		otree->Fill();
	}
	otree->Write();
	ofile->Close();
}
