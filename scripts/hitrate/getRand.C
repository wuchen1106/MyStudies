{
TString MyData = getenv("MYDATA");
	// About this run
	std::vector<TString> DirName;
	std::vector<int> nRuns;
	TString FileName = "";
//	TString InFileName = "/home/chen/MyWorkArea/g4sim/data/MT1.pim.g60cm10mm.005T.g4s.QBH.root";
	TString InFileName = "/home/chen/MyWorkArea/g4sim/data/MT1.OT.g60cm10mm.005T.g4s.QBH.root";
	 // ########Should Modify#########
	TString runName = "OT";
//	FileName="result/pim.root";
	FileName="OT.140331.root";
	//DirName.push_back("/scratchfs/bes/wuc/MyWorkArea/Data/raw/g4sim/BLTCDC.em.g60cm10mm.005T.1p5_0927_11_p5.g4s.QBH");
	//nRuns.push_back(100);
	//DirName.push_back("/scratchfs/bes/wuc/MyWorkArea/Data/raw/g4sim/BLTCDC.OT.g60cm10mm.005T.1p5_0927_11_p5.g4s.QBH");
	//nRuns.push_back(100);
	//DirName.push_back("/scratchfs/bes/wuc/MyWorkArea/Data/raw/g4sim/BLTCDC.mum.g60cm10mm.005T.1p5_0927_11_p5.g4s.QBH");
	//nRuns.push_back(100);
//	DirName.push_back(MyData+"/raw/g4sim/CDCHit.OT.g60cm10mm.005T.140331.g4s.QBH");
//	nRuns.push_back(100);
	double nProtons = 1e10;
	 // ########Should Modify#########

	TChain *ci = new TChain("t");
	ci->Add(InFileName);

	TChain *c = new TChain("tree");
	if (FileName!="")
		c->Add(FileName);
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
	std::string * process;
	std::string * volume;
	c->SetBranchAddress("C_nHits",&C_nHits);
	c->SetBranchAddress("evt_num",&evt_num);
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

	TFile * f = new TFile(runName+".output.root","RECREATE");
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

	int nEvents = c->GetEntries();
	std::cout<<"nEvents = "<<nEvents<<std::endl;
	std::stringstream buff;
	for (int iEvent = 0; iEvent < nEvents; iEvent++ ){
		if (iEvent%5000==0) std::cout<<(double)iEvent/nEvents*100<<" % ..."<<std::endl;
		c->GetEntry(iEvent);
		if (C_nHits>0){
			ci->GetEntry(evt_num);
			tree->Fill();
		}
	}

	tree->Write();
}
