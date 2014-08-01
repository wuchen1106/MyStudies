TString MyData = getenv("MYDATA");

void getElectrons(){

	// About this run
	 // ########Should Modify#########
	TString parName = "OTWC";
	TString suffixName = "0508_100cm_1e9";
//	TString suffixName = "140331";
	TString runName = parName+"."+suffixName;
	std::vector<TString> DirName;
	std::vector<int> nRuns;
	std::vector<TString> FileNames;
	DirName.push_back(MyData+"/CDCHit."+parName+".g60cm10mm.005T."+suffixName+".g4s.QBH");
	nRuns.push_back(100);
//	FileNames.push_back(runName+".root");
	 // ########Should Modify#########

	TChain *c = new TChain("tree");
	std::cout<<"FileNames.size() = "<<(FileNames.size())<<std::endl;
	for (int i = 0; i<FileNames.size(); i++){
		std::cout<<"FileNames["<<i<<"] = \""<<FileNames[i]<<"\""<<std::endl;
		c->Add(FileNames[i]);
	}
	std::stringstream buff;
	std::cout<<"nRuns = "<<nRuns.size()<<std::endl;
	for (int iRun = 0; iRun < nRuns.size(); iRun++ ){
		for (int i = 0; i<nRuns[iRun]; i++){
			buff.str("");
			buff.clear();
			buff<<DirName[iRun]<<"/"<<i<<"_job0.raw";
			c->Add( buff.str().c_str());
		}
	}

	std::vector<int> *C_pid;
	std::vector<int> *C_ppid;
	std::vector<int> *C_ptid;
	std::vector<double> *C_t;
	std::vector<double> *C_x;
	std::vector<double> *C_y;
	std::vector<double> *C_z;
	std::vector<double> *C_px;
	std::vector<double> *C_py;
	std::vector<double> *C_pz;
	std::vector<std::string> *C_oprocess;
	std::vector<std::string> *C_ovolName;
	std::vector<int> *M_ptid;
	std::vector<int> *M_tid;
	std::vector<int> *M_pid;
	std::vector<int> *M_ppid;
	std::vector<double> *M_px;
	std::vector<double> *M_py;
	std::vector<double> *M_pz;
	std::vector<std::string> *M_process;
	std::vector<std::string> *M_volume;
	double weight;
	c->SetBranchAddress("C_ppid",&C_ppid);
	c->SetBranchAddress("C_pid",&C_pid);
	c->SetBranchAddress("C_ptid",&C_ptid);
	c->SetBranchAddress("C_t",&C_t);
	c->SetBranchAddress("C_px",&C_px);
	c->SetBranchAddress("C_py",&C_py);
	c->SetBranchAddress("C_pz",&C_pz);
	c->SetBranchAddress("C_x",&C_x);
	c->SetBranchAddress("C_y",&C_y);
	c->SetBranchAddress("C_z",&C_z);
	c->SetBranchAddress("C_ovolName",&C_ovolName);
	c->SetBranchAddress("C_oprocess",&C_oprocess);
	c->SetBranchAddress("McTruth_ptid",&M_ptid);
	c->SetBranchAddress("McTruth_tid",&M_tid);
	c->SetBranchAddress("McTruth_pid",&M_pid);
	c->SetBranchAddress("McTruth_ppid",&M_ppid);
	c->SetBranchAddress("McTruth_px",&M_px);
	c->SetBranchAddress("McTruth_py",&M_py);
	c->SetBranchAddress("McTruth_pz",&M_pz);
	c->SetBranchAddress("McTruth_volume",&M_volume);
	c->SetBranchAddress("McTruth_process",&M_process);
	c->SetBranchAddress("weight",&weight);

	int pid = 0;
	int ppid = 0;
	double time = 0;
	double x = 0;
	double y = 0;
	double z = 0;
	double px = 0;
	double py = 0;
	double pz = 0;
	std::string * volume;
	std::string * process;
	double mmmpx = 0;
	double mmmpy = 0;
	double mmmpz = 0;
	int mmmpid = 0;
	int mmmppid = 0;
	std::string * mmmvolume;
	std::string * mmmprocess;
	double mmpx = 0;
	double mmpy = 0;
	double mmpz = 0;
	int mmpid = 0;
	int mmppid = 0;
	std::string * mmvolume;
	std::string * mmprocess;
	double mpx = 0;
	double mpy = 0;
	double mpz = 0;
	int mpid = 0;
	int mppid = 0;
	std::string * mvolume;
	std::string * mprocess;
	int nHits = 0;

	f = new TFile(runName+".electrons.root","RECREATE");
	TTree *t  = new TTree("t","t");
	t->Branch("nHits",&nHits);
	t->Branch("t",&time);
	t->Branch("pid",&pid);
	t->Branch("ppid",&ppid);
	t->Branch("x",&x);
	t->Branch("y",&y);
	t->Branch("z",&z);
	t->Branch("px",&px);
	t->Branch("py",&py);
	t->Branch("pz",&pz);
	t->Branch("volume",&volume);
	t->Branch("process",&process);
	t->Branch("mpid",&mpid);
	t->Branch("mppid",&mppid);
	t->Branch("mpx",&mpx);
	t->Branch("mpy",&mpy);
	t->Branch("mpz",&mpz);
	t->Branch("mvolume",&mvolume);
	t->Branch("mprocess",&mprocess);
	t->Branch("mmpid",&mmpid);
	t->Branch("mmppid",&mmppid);
	t->Branch("mmpx",&mmpx);
	t->Branch("mmpy",&mmpy);
	t->Branch("mmpz",&mmpz);
	t->Branch("mmvolume",&mmvolume);
	t->Branch("mmprocess",&mmprocess);
	t->Branch("mmmpid",&mmmpid);
	t->Branch("mmmppid",&mmmppid);
	t->Branch("mmmpx",&mmmpx);
	t->Branch("mmmpy",&mmmpy);
	t->Branch("mmmpz",&mmmpz);
	t->Branch("mmmvolume",&mmmvolume);
	t->Branch("mmmprocess",&mmmprocess);

	int nEvents = c->GetEntries();
	std::cout<<"nEvents = "<<nEvents<<std::endl;
	std::stringstream buff;
	for (int iEvent = 0; iEvent < nEvents; iEvent++ ){
		if (iEvent%1000==0) std::cout<<(double)iEvent/nEvents*100<<" % ..."<<std::endl;
		c->GetEntry(iEvent);
//		if (iEvent>=11094) weight*=100./31;

// *************************************
//		if ((*C_pid)[0]!=11) continue;
// *************************************
		int pretid = -1;
		for ( int ihit = 0; ihit < C_t->size(); ihit++){
			if ((*C_ptid)[ihit] == pretid){ // This is a old track. Accumulate and continue;
				nHits++;
				if (ihit==C_t->size()-1) t->Fill(); // This is the last hit, so Let's push
				continue;
			}
//			if ((*C_ovolName)[ihit]!="EndPlateU"&&(*C_ovolName)[ihit]!="EndPlateD") continue;
//			if ((*C_ovolName)[ihit]!="CdcSignalWire"&&(*C_ovolName)[ihit]!="CdcFieldWire") continue;
//			if ((*C_ovolName)[ihit]!="CdcCell") continue;
//			if ((*C_ovolName)[ihit]!="InnerCylinder"&&(*C_ovolName)[ihit]!="InnerCylinderI"&&(*C_ovolName)[ihit]!="OuterCylinder") continue;
//			if ((*C_ovolName)[ihit]=="EndPlateU"
//					||(*C_ovolName)[ihit]=="EndPlateD"
//					||(*C_ovolName)[ihit]=="CdcSignalWire"
//					||(*C_ovolName)[ihit]=="CdcFieldWire"
//					||(*C_ovolName)[ihit]=="CdcCell"
//					||(*C_ovolName)[ihit]=="InnerCylinder"
//					||(*C_ovolName)[ihit]=="InnerCylinderI"
//					||(*C_ovolName)[ihit]=="OuterCylinder")
//				continue;

			if (ihit!=0) t->Fill(); // This is a new track, and  we got tracks before.

			pretid = (*C_ptid)[ihit];  // now we search for this new track.
			nHits = 1;

			time = (*C_t)[ihit];
			pid = (*C_pid)[ihit];
			ppid = (*C_ppid)[ihit];
			px = (*C_px)[ihit]*1000;
			py = (*C_py)[ihit]*1000;
			pz = (*C_pz)[ihit]*1000;
			x = (*C_x)[ihit]*10;
			y = (*C_y)[ihit]*10;
			z = (*C_z)[ihit]*10;
			process = &((*C_oprocess)[ihit]);
			volume = &((*C_ovolName)[ihit]);
			int iMc = 0;
			for (; iMc < M_tid->size(); iMc++){
				if ((*M_tid)[iMc]==(*C_ptid)[ihit])
					break;
			}
			if (iMc==M_pid->size()){
				std::cout<<"Couldn't find the mother! ppid = "<<ppid<<std::endl;
				mpid = 0;
			}
			else{
				mpx = (*M_px)[iMc]*1000;
				mpy = (*M_py)[iMc]*1000;
				mpz = (*M_pz)[iMc]*1000;
				mpid = (*M_pid)[iMc];
				mppid = (*M_ppid)[iMc];
				mprocess = &((*M_process)[iMc]);
				mvolume = &((*M_volume)[iMc]);
			}
			if ((*M_tid)[iMc]==1){
				mmpid = 0;
			}
			else{
				int iiMc = 0;
				for (; iiMc < M_tid->size(); iiMc++){
					if ((*M_tid)[iiMc]==(*M_ptid)[iMc])
						break;
				}
				if (iiMc==M_pid->size()){
					std::cout<<"    Couldn't find mother's mother! tid = "<<(*M_ptid)[iMc]<<std::endl;
					mmpid = 0;
				}
				else{
					mmpx = (*M_px)[iiMc]*1000;
					mmpy = (*M_py)[iiMc]*1000;
					mmpz = (*M_pz)[iiMc]*1000;
					mmpid = (*M_pid)[iiMc];
					mmppid = (*M_ppid)[iiMc];
						/*
					mmprocess = &((*M_process)[iiMc]);
					mmvolume = &((*M_volume)[iiMc]);
					*/
				}
				if ((*M_tid)[iiMc]==1){
					mmmpid = 0;
				}
				else{
					int iiiMc = 0;
					for (; iiiMc < M_tid->size(); iiiMc++){
						if ((*M_tid)[iiiMc]==(*M_ptid)[iiMc])
							break;
					}
					if (iiiMc==M_pid->size()){
						std::cout<<"    Couldn't find mother's mother's mother! tid = "<<(*M_ptid)[iiMc]<<std::endl;
						mmmpid = 0;
					}
					else{
						mmmpx = (*M_px)[iiiMc]*1000;
						mmmpy = (*M_py)[iiiMc]*1000;
						mmmpz = (*M_pz)[iiiMc]*1000;
						mmmpid = (*M_pid)[iiiMc];
						mmmppid = (*M_ppid)[iiiMc];
						/*
						mmmprocess = &((*M_process)[iiiMc]);
						mmmvolume = &((*M_volume)[iiiMc]);
						*/
					}
				}
			}
			if (ihit==C_t->size()-1) t->Fill(); // This is the last hit, so only one hit in this track
		}

	}
	t->Write();
}
