{
	// About this run
	std::vector<TString> DirName;
	std::vector<int> nRuns;
	TString FileName = "";
	TString InFileName = "/home/chen/MyWorkArea/g4sim/data/MT1.OT.g60cm10mm.005T.g4s.QBH.root";
	// ########Should Modify#########
	TString runName = "OT";
	FileName="OT.140331.rec.root";
	//DirName.push_back("/home/chen/MyWorkArea/Data/raw/g4sim/CDCHit.OT.g60cm10mm.005T.140331.g4s.QBH");
	//nRuns.push_back(100);
	//DirName.push_back("/scratchfs/bes/wuc/MyWorkArea/Data/raw/g4sim/BLTCDC.OT.g60cm10mm.005T.1p5_0927_11_p5.g4s.QBH");
	//nRuns.push_back(100);
	//DirName.push_back("/scratchfs/bes/wuc/MyWorkArea/Data/raw/g4sim/BLTCDC.mum.g60cm10mm.005T.1p5_0927_11_p5.g4s.QBH");
	//nRuns.push_back(100);
	// ########Should Modify#########

	TFile * f = new TFile(runName+".root","RECREATE");
	TTree *tree  = new TTree("tree","tree");
	int  C_nHits = 0;
	std::vector<int>     *C_volID;
	std::vector<int>     *C_pid;
	std::vector<int>     *C_ptid;
	std::vector<int>     *C_ppid;
	std::vector<double>  *C_edep;
	std::vector<double>  *C_t;
	std::vector<double>  *C_px;
	std::vector<double>  *C_py;
	std::vector<double>  *C_pz;
	std::vector<double>  *C_x;
	std::vector<double>  *C_y;
	std::vector<double>  *C_z;
	std::vector<double>  *C_oopx;
	std::vector<double>  *C_oopy;
	std::vector<double>  *C_oopz;
	std::vector<double>  *C_oox;
	std::vector<double>  *C_ooy;
	std::vector<double>  *C_ooz;
	std::vector<std::string>  *C_ovolName;
	std::vector<std::string>  *C_oprocess;
	std::vector<int>     *McTruth_pid;
	std::vector<int>     *McTruth_tid;
	std::vector<double>  *McTruth_px;
	std::vector<double>  *McTruth_py;
	std::vector<double>  *McTruth_pz;
	std::vector<double>  *McTruth_x;
	std::vector<double>  *McTruth_y;
	std::vector<double>  *McTruth_z;
	int     ipid;
	int  evt_num;
	int  run_num;
	int R0;
	int R1;
	double weight;
	tree->Branch("C_nHits",&C_nHits);
	tree->Branch("evt_num",&evt_num);
	tree->Branch("run_num",&run_num);
	tree->Branch("R0",&R0);
	tree->Branch("R1",&R1);
	tree->Branch("C_volID",&C_volID);
	tree->Branch("C_ppid",&C_ppid);
	tree->Branch("C_pid",&C_pid);
	tree->Branch("C_edep",&C_edep);
	tree->Branch("C_t",&C_t);
	tree->Branch("C_oprocess",&C_oprocess);
	tree->Branch("C_ovolName",&C_ovolName);
	tree->Branch("C_px",&C_px);
	tree->Branch("C_py",&C_py);
	tree->Branch("C_pz",&C_pz);
	tree->Branch("C_x",&C_x);
	tree->Branch("C_y",&C_y);
	tree->Branch("C_z",&C_z);
	tree->Branch("C_oopx",&C_oopx);
	tree->Branch("C_oopy",&C_oopy);
	tree->Branch("C_oopz",&C_oopz);
	tree->Branch("C_oox",&C_oox);
	tree->Branch("C_ooy",&C_ooy);
	tree->Branch("C_ooz",&C_ooz);
	tree->Branch("McTruth_pid",&McTruth_pid);
	tree->Branch("McTruth_px",&McTruth_px);
	tree->Branch("McTruth_py",&McTruth_py);
	tree->Branch("McTruth_pz",&McTruth_pz);
	tree->Branch("McTruth_x",&McTruth_x);
	tree->Branch("McTruth_y",&McTruth_y);
	tree->Branch("McTruth_z",&McTruth_z);
	tree->Branch("weight",&weight);

	TChain * c = new TChain("tree");
	if (FileName!="")
		c->Add(FileName);
	std::stringstream buff;

	c->SetBranchAddress("C_nHits",&C_nHits);
	c->SetBranchAddress("evt_num",&evt_num);
	c->SetBranchAddress("run_num",&run_num);
	double dR0, dR1;
	bool doubleRandom = false;
	std::string title = c->GetBranch("R1")->GetTitle();
	if ((title).substr(0,4)=="R1/D"){
		doubleRandom = true;
	}
	if (doubleRandom){ c->SetBranchAddress("R0",&dR0);
		c->SetBranchAddress("R1",&dR1);
	}
	else{
		c->SetBranchAddress("R0",&R0);
		c->SetBranchAddress("R1",&R1);
	}
	c->SetBranchAddress("C_volID",&C_volID);
	c->SetBranchAddress("C_ppid",&C_ppid);
	c->SetBranchAddress("C_ptid",&C_ptid);
	c->SetBranchAddress("C_pid",&C_pid);
	c->SetBranchAddress("C_edep",&C_edep);
	c->SetBranchAddress("C_t",&C_t);
	c->SetBranchAddress("C_oprocess",&C_oprocess);
	c->SetBranchAddress("C_ovolName",&C_ovolName);
	c->SetBranchAddress("C_px",&C_px);
	c->SetBranchAddress("C_py",&C_py);
	c->SetBranchAddress("C_pz",&C_pz);
	c->SetBranchAddress("C_x",&C_x);
	c->SetBranchAddress("C_y",&C_y);
	c->SetBranchAddress("C_z",&C_z);
	c->SetBranchAddress("McTruth_pid",&McTruth_pid);
	c->SetBranchAddress("McTruth_tid",&McTruth_tid);
	c->SetBranchAddress("McTruth_px",&McTruth_px);
	c->SetBranchAddress("McTruth_py",&McTruth_py);
	c->SetBranchAddress("McTruth_pz",&McTruth_pz);
	c->SetBranchAddress("McTruth_x",&McTruth_x);
	c->SetBranchAddress("McTruth_y",&McTruth_y);
	c->SetBranchAddress("McTruth_z",&McTruth_z);
	c->SetBranchAddress("weight",&weight);

	int nEvents = c->GetEntries();
	std::cout<<"nEvents = "<<nEvents<<std::endl;
	std::stringstream buff;
	for (int iEvent = 0; iEvent < nEvents; iEvent++ ){
		if (iEvent%5000==0) std::cout<<(double)iEvent/nEvents*100<<" % ..."<<std::endl;
		c->GetEntry(iEvent);
		C_oopx = new std::vector<double>;
		C_oopy = new std::vector<double>;
		C_oopz = new std::vector<double>;
		C_oox = new std::vector<double>;
		C_ooy = new std::vector<double>;
		C_ooz = new std::vector<double>;
		if (C_nHits>0){
			for ( int j = 0; j<C_nHits;j++){
				std::string name = (*C_ovolName)[j];
				if (name.substr(0,7)=="CdcCell") (*C_ovolName)[j]="CdcCell";
				else if (name.substr(0,9)=="EndPlateD") (*C_ovolName)[j]="EndPlateD";
				else if (name.substr(0,9)=="EndPlateU") (*C_ovolName)[j]="EndPlateU";
				else if (name.substr(0,12)=="CdcFieldWire") (*C_ovolName)[j]="CdcFieldWire";
				else if (name.substr(0,13)=="CdcSignalWire") (*C_ovolName)[j]="CdcSignalWire";
				// get oo
				for (int iMc = 0; iMc < McTruth_tid->size(); iMc++){
					if ((*McTruth_tid)[iMc]==(*C_ptid)[j]){
						C_oopx->push_back((*McTruth_px)[iMc]);
						C_oopy->push_back((*McTruth_py)[iMc]);
						C_oopz->push_back((*McTruth_pz)[iMc]);
						C_oox->push_back((*McTruth_x)[iMc]);
						C_ooy->push_back((*McTruth_y)[iMc]);
						C_ooz->push_back((*McTruth_z)[iMc]);
						break;
					}
					if (iMc==McTruth_tid->size()-1){
						C_oopx->push_back(0);
						C_oopy->push_back(0);
						C_oopz->push_back(0);
						C_oox->push_back(0);
						C_ooy->push_back(0);
						C_ooz->push_back(0);
					}
				}
			}
			if (doubleRandom){ R0 = dR0; R1 = dR1;}
			//std::cout<<"("<<R0<<","<<R1<<") ("<<dR0<<","<<dR1<<")"<<std::endl;
			//			weight /= 10;
			tree->Fill();
		}
	}
	tree->Write();
}
