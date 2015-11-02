{
	double twindow = 10;
	double me = 0.511e-3; // GeV
	TChain * c = new TChain("tree");
	c->Add("ALL.150901.W100um.OptD2.DD35.TH200um.root");
	std::vector<double> * in_x = 0;
	std::vector<double> * in_y = 0;
	std::vector<double> * in_z = 0;
	std::vector<double> * in_px = 0;
	std::vector<double> * in_py = 0;
	std::vector<double> * in_pz = 0;
	std::vector<double> * in_t = 0;
	std::vector<int> * in_pid = 0;
	std::vector<std::string> * in_process = 0;
	std::vector<std::string> * in_volume = 0;
	int in_type;
	int in_topo;
	int in_nHits;
	int in_cpid;
	std::vector<int> * in_triType = 0;
	std::vector<int> * in_triID = 0;
	std::vector<int> * in_tripos = 0;
	std::vector<double> * in_triz = 0;
	std::vector<double> * in_trit = 0;
	std::vector<double> * in_triedep = 0;
	std::vector<double> * in_tripx = 0;
	std::vector<double> * in_tripy = 0;
	std::vector<double> * in_tripz = 0;
	c->SetBranchAddress("x",&in_x);
	c->SetBranchAddress("y",&in_y);
	c->SetBranchAddress("z",&in_z);
	c->SetBranchAddress("px",&in_px);
	c->SetBranchAddress("py",&in_py);
	c->SetBranchAddress("pz",&in_pz);
	c->SetBranchAddress("t",&in_t);
	c->SetBranchAddress("process",&in_process);
	c->SetBranchAddress("volume",&in_volume);
	c->SetBranchAddress("pid",&in_pid);
	c->SetBranchAddress("type",&in_type);
	c->SetBranchAddress("topo",&in_topo);
	c->SetBranchAddress("nHits",&in_nHits);
	c->SetBranchAddress("cpid",&in_cpid);
	c->SetBranchAddress("O_t",&in_trit);
	c->SetBranchAddress("O_z",&in_triz);
	c->SetBranchAddress("O_edep",&in_triedep);
	c->SetBranchAddress("O_px",&in_tripx);
	c->SetBranchAddress("O_py",&in_tripy);
	c->SetBranchAddress("O_pz",&in_tripz);
	c->SetBranchAddress("O_triType",&in_triType);
	c->SetBranchAddress("O_triID",&in_triID);
	c->SetBranchAddress("O_triPos",&in_tripos);
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
	otree->Branch("x",&in_x);
	otree->Branch("y",&in_y);
	otree->Branch("z",&in_z);
	otree->Branch("px",&in_px);
	otree->Branch("py",&in_py);
	otree->Branch("pz",&in_pz);
	otree->Branch("t",&in_t);
	otree->Branch("pid",&in_pid);
	otree->Branch("process",&in_process);
	otree->Branch("volume",&in_volume);
	double sci_time[128];
	double che_time[128];
	int totalfound = 0;
	int nHitsSU = 0;
	int nHitsCU = 0;
	int nHitsSD = 0;
	int nHitsCD = 0;
	Long64_t nEntries = c->GetEntries();
	for (Long64_t iEntry = 0; iEntry<nEntries; iEntry++){
		if (iEntry%1000==0) std::cout<<(double)iEntry/nEntries*100<<"%..."<<std::endl;
		c->GetEntry(iEntry);
		if (in_topo==-1) continue;
		if (fabs(in_cpid)!=11) continue;
		if (in_type!=1) continue;
		//if (in_nHits<2) continue;
		// option D
		if (in_nHits<4) continue;
		for (int itri = 0; itri<128; itri++){
			che_time[itri] = -1;
			sci_time[itri] = -1;
		}
		for ( int ihit = 0; ihit<in_nHits; ihit++){
			double px = (*in_tripx)[ihit];
			double py = (*in_tripy)[ihit];
			double pz = (*in_tripz)[ihit];
			double pa = sqrt(px*px+py*py+pz*pz);
			double beta = pa/sqrt(pa*pa+me*me);
			if ((*in_triType)[ihit]==0){
				if (beta>0.5){
					che_time[(*in_triID)[ihit]+((*in_tripos)[ihit]+1)*32] = (*in_trit)[ihit];
					if ((*in_tripos)[ihit]>0) nHitsCD++;
					else nHitsCU++;
				}
			}
			else if ((*in_triType)[ihit]==1&&(*in_triedep)[ihit]>630e-6){
				sci_time[(*in_triID)[ihit]+((*in_tripos)[ihit]+1)*32] = (*in_trit)[ihit];
				if ((*in_tripos)[ihit]>0) nHitsSD++;
				else nHitsSU++;
			}
			//else if ((*in_triType)[ihit]==-1&&beta>0.5){
			//	sci_time[(*in_triID)[ihit]+((*in_triz)[ihit]<6500)*64] = (*in_trit)[ihit];
			//}
		}
		int nfound = 0;
		// option D
		for (int itri = 0; itri<128; itri++){
			int jtri = itri+1;
			if (jtri==64) jtri=0;
			if (jtri==128) jtri=64;
			if (fabs(sci_time[itri]-che_time[itri])<twindow&&sci_time[itri]>200&&che_time[itri]>200){
				if (fabs(sci_time[jtri]-che_time[jtri])<twindow&&sci_time[jtri]>200&&che_time[jtri]>200){
					if (fabs(sci_time[itri]-che_time[jtri])<twindow&&fabs(sci_time[jtri]-che_time[itri])<twindow){
						nfound++;
					}
				}
			}
		}
		//if (!found) continue;
		totalfound+=nfound;

		if (nfound>0){
			//out_x = (*in_x)[0];
			//out_y = (*in_y)[0];
			//out_z = (*in_z)[0];
			//out_px = (*in_px)[0];
			//out_py = (*in_py)[0];
			//out_pz = (*in_pz)[0];
			//out_pid = (*in_pid)[0];
			//out_t = (*in_t)[0];
			otree->Fill();
		}
	}
	std::cout<<"totalfound = "<<totalfound<<", "<<(double)totalfound/1e9*2.5e12*2.6/1e3<<std::endl;
	std::cout<<"nHitsSU = "<<nHitsSU<<", "<<(double)nHitsSU/1e9*2.5e12*2.6/1e3/64<<" kHz"<<std::endl;
	std::cout<<"nHitsSD = "<<nHitsSD<<", "<<(double)nHitsSD/1e9*2.5e12*2.6/1e3/64<<" kHz"<<std::endl;
	std::cout<<"nHitsCU = "<<nHitsCU<<", "<<(double)nHitsCU/1e9*2.5e12*2.6/1e3/64<<" kHz"<<std::endl;
	std::cout<<"nHitsCD = "<<nHitsCD<<", "<<(double)nHitsCD/1e9*2.5e12*2.6/1e3/64<<" kHz"<<std::endl;
	otree->Write();
	ofile->Close();
}
