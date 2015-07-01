{

	// About this run
	std::vector<TString> DirName;
	std::vector<int> nRuns;
	std::vector<TString> FileNames;
	 // ########Should Modify#########
	TString parName = "All";
	TString suffixName = "141109.140625ori.gaussian";
	TString runName = parName+"."+suffixName;
	FileNames.push_back(runName+".root");
	TString option = "proton.mum";
	//TString option = "noise.mumneutron";
	//TString option = "noise.OT";
	 // ########Should Modify#########

	TChain * c = new TChain("tree","tree");
	for ( int i  = 0; i<FileNames.size(); i++){
		c->Add(FileNames[i]);
	}
	TFile * f = new TFile("result/"+option+"."+suffixName+".root","RECREATE");
	TTree *tree  = new TTree("tree","tree");
	int C_nHits;
	std::vector<double> * C_t = 0;
	std::vector<double> * C_driftD = 0;
	std::vector<double> * C_driftDtrue = 0;
	std::vector<double> * C_tstart = 0;
	std::vector<double> * C_tstop = 0;
	std::vector<double> * C_edep = 0;
	std::vector<int> * C_cellID = 0;
	std::vector<int> * C_layerID = 0;
	std::vector<int> * C_posflag = 0;
	std::vector<double> * C_wx = 0;
	std::vector<double> * C_wy = 0;
	std::vector<double> * C_wz = 0;
	std::vector<double> * C_x = 0;
	std::vector<double> * C_y = 0;
	std::vector<double> * C_z = 0;
	std::vector<double> * C_px = 0;
	std::vector<double> * C_py = 0;
	std::vector<double> * C_pz = 0;
	int type = 0;
	int topo = 0;
	int cpid = 0;
	double weight;
	c->SetBranchAddress("type",&type);
	c->SetBranchAddress("topo",&topo);
	c->SetBranchAddress("cpid",&cpid);
	c->SetBranchAddress("weight",&weight);
	c->SetBranchAddress("nHits",&C_nHits);
	c->SetBranchAddress("O_t",&C_t);
	c->SetBranchAddress("O_driftD",&C_driftD);
	c->SetBranchAddress("O_driftDtrue",&C_driftDtrue);
	c->SetBranchAddress("O_tstart",&C_tstart);
	c->SetBranchAddress("O_tstop",&C_tstop);
	c->SetBranchAddress("O_edep",&C_edep);
	c->SetBranchAddress("O_cellID",&C_cellID);
	c->SetBranchAddress("O_layerID",&C_layerID);
	c->SetBranchAddress("O_posflag",&C_posflag);
	c->SetBranchAddress("O_wx",&C_wx);
	c->SetBranchAddress("O_wy",&C_wy);
	c->SetBranchAddress("O_wz",&C_wz);
	c->SetBranchAddress("O_x",&C_x);
	c->SetBranchAddress("O_y",&C_y);
	c->SetBranchAddress("O_z",&C_z);
	c->SetBranchAddress("O_px",&C_px);
	c->SetBranchAddress("O_py",&C_py);
	c->SetBranchAddress("O_pz",&C_pz);

	tree->Branch("type",&type);
	tree->Branch("topo",&topo);
	tree->Branch("cpid",&cpid);
	tree->Branch("weight",&weight);
	tree->Branch("nHits",&C_nHits);
	tree->Branch("O_t",&C_t);
	tree->Branch("O_driftD",&C_driftD);
	tree->Branch("O_driftDtrue",&C_driftDtrue);
	tree->Branch("O_tstart",&C_tstart);
	tree->Branch("O_tstop",&C_tstop);
	tree->Branch("O_edep",&C_edep);
	tree->Branch("O_cellID",&C_cellID);
	tree->Branch("O_layerID",&C_layerID);
	tree->Branch("O_posflag",&C_posflag);
	tree->Branch("O_wx",&C_wx);
	tree->Branch("O_wy",&C_wy);
	tree->Branch("O_wz",&C_wz);
	tree->Branch("O_x",&C_x);
	tree->Branch("O_y",&C_y);
	tree->Branch("O_z",&C_z);
	tree->Branch("O_px",&C_px);
	tree->Branch("O_py",&C_py);
	tree->Branch("O_pz",&C_pz);

	for ( Long64_t iEntry = 0; iEntry<c->GetEntries()/0.845/10; iEntry++ ) {
		c->GetEntry(iEntry);
		if (iEntry%1000==0) std::cout<<iEntry<<std::endl;
		if (type!=0) continue;

		if (option == "proton.mum"){
			if (topo!=1000||cpid!=2212) continue;
		}
		else if (option == "noise.mumneutron"){
			if (topo!=1005) continue;
		}
		else if (option == "noise.OT"){
			if (topo==1000&&(cpid==2212||cpid>1e6)
				||topo==1005
				||topo==-2
					) continue;
		}
		tree->Fill();
	}
	tree->Write();
	f->Close();
}
