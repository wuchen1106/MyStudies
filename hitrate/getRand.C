{
	TString MyData = getenv("MYDATA");
	// About this run
	std::vector<TString> DirName;
	std::vector<int> nRuns;
	std::vector<TString> FileNames;
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
	//Long64_t nEvents_per_run = 176779;
	Long64_t nEvents_per_run = 353558;
	 // ########Should Modify#########
	TString InFileName = "/home/chen/MyWorkArea/Simulate/comet/data/MT1.EP.OT.g60cm10mm.005T.g4s.QBH.root";
	if (parName == "pim"){
		InFileName = "/home/chen/MyWorkArea/Simulate/comet/data/MT1.pim.g60cm10mm.005T.g4s.QBH.root";
	}

	TChain *ci = new TChain("t");
	ci->Add(InFileName);
	Long64_t nEvent_ci = ci->GetEntries();

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

	int  C_nHits = 0;
	double  weight;
	int     R0;
	int     R1;
	int     pid;
	int     ppid;
	double  x;
	double  y;
	double  z;
	double  t;
	double  px;
	double  py;
	double  pz;
	double  ox;
	double  oy;
	double  oz;
	double  ot;
	double  opx;
	double  opy;
	double  opz;
	int  evt_num;
	int  run_num;
	std::string * process;
	std::string * volume;
	c->SetBranchAddress("C_nHits",&C_nHits);
	c->SetBranchAddress("evt_num",&evt_num);
	c->SetBranchAddress("run_num",&run_num);
	c->SetBranchAddress("R0",&R0);
	c->SetBranchAddress("R1",&R1);
	ci->SetBranchAddress("pid",&pid);
	ci->SetBranchAddress("x",&x);
	ci->SetBranchAddress("y",&y);
	ci->SetBranchAddress("z",&z);
	ci->SetBranchAddress("t",&t);
	ci->SetBranchAddress("ox",&ox);
	ci->SetBranchAddress("oy",&oy);
	ci->SetBranchAddress("oz",&oz);
	ci->SetBranchAddress("ot",&ot);
	ci->SetBranchAddress("px",&px);
	ci->SetBranchAddress("py",&py);
	ci->SetBranchAddress("pz",&pz);
	ci->SetBranchAddress("opx",&opx);
	ci->SetBranchAddress("opy",&opy);
	ci->SetBranchAddress("opz",&opz);
	ci->SetBranchAddress("weight",&weight);
	ci->SetBranchAddress("process",&process);
	ci->SetBranchAddress("volume",&volume);

	TFile * f = new TFile(runName+".rand.root","RECREATE");
	TTree *tree  = new TTree("t","t");
	tree->Branch("R0",&R0);
	tree->Branch("R1",&R1);
	tree->Branch("pid",&pid);
	tree->Branch("x",&x);
	tree->Branch("y",&y);
	tree->Branch("z",&z);
	tree->Branch("px",&px);
	tree->Branch("py",&py);
	tree->Branch("pz",&pz);
	tree->Branch("t",&t);
	tree->Branch("ox",&ox);
	tree->Branch("oy",&oy);
	tree->Branch("oz",&oz);
	tree->Branch("ot",&ot);
	tree->Branch("px",&px);
	tree->Branch("py",&py);
	tree->Branch("pz",&pz);
	tree->Branch("opx",&opx);
	tree->Branch("opy",&opy);
	tree->Branch("opz",&opz);
	tree->Branch("weight",&weight);
	tree->Branch("process",&process);
	tree->Branch("volume",&volume);

	Long64_t nEvents = c->GetEntries();
	std::cout<<"nEvents = "<<nEvents<<std::endl;
	std::stringstream buff;
	int iRun = 0;
	int  pre_run_num = -1;
	for (Long64_t iEvent = 0; iEvent < nEvents; iEvent++ ){
		if (iEvent%5000==0) std::cout<<(double)iEvent/nEvents*100<<" % ..."<<std::endl;
		c->GetEntry(iEvent);
		if (C_nHits>0){
			ci->GetEntry((evt_num+iRun*nEvents_per_run)%nEvent_ci);
			tree->Fill();
		}
		if (run_num!=pre_run_num){
			pre_run_num = run_num;
			iRun++;
		}
	}

	tree->Write();
}
