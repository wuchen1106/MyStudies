{
	std::vector<std::string> vMonName;
	vMonName.push_back("CS");
	vMonName.push_back("1p5M");
	vMonName.push_back("2p35M");
	vMonName.push_back("3M");
	vMonName.push_back("TS1");

	std::vector<int> pretid;
	pretid.resize(vMonName.size());

	TChain * c = new TChain("tree");
	//c->Add("/home/chen/MyWorkArea/Simulate/comet/output/testCapture.Yuki.withtarget.root");
	c->Add("/home/chen/MyWorkArea/Simulate/comet/output/testCapture.Ye.root");
	//c->Add("/home/chen/MyWorkArea/Simulate/comet/output/raw_g4sim.root");
	int evt_num;
	std::vector<int> *vpid = 0;
	std::vector<int> *vtid = 0;
	std::vector<double>* vox = 0;
	std::vector<double>* voy = 0;
	std::vector<double>* voz = 0;
	std::vector<double>* vx = 0;
	std::vector<double>* vy = 0;
	std::vector<double>* vz = 0;
	std::vector<double>* vpx = 0;
	std::vector<double>* vpy = 0;
	std::vector<double>* vpz = 0;
	std::vector<double>* vot = 0;
	std::vector<double>* vt = 0;
	std::vector<std::string>* vvn = 0;
	c->SetBranchAddress("evt_num",&evt_num);
	c->SetBranchAddress("V_pid",&vpid);
	c->SetBranchAddress("V_tid",&vtid);
	c->SetBranchAddress("V_ox",&vox);
	c->SetBranchAddress("V_oy",&voy);
	c->SetBranchAddress("V_oz",&voz);
	c->SetBranchAddress("V_x",&vx);
	c->SetBranchAddress("V_y",&vy);
	c->SetBranchAddress("V_z",&vz);
	c->SetBranchAddress("V_px",&vpx);
	c->SetBranchAddress("V_py",&vpy);
	c->SetBranchAddress("V_pz",&vpz);
	c->SetBranchAddress("V_ot",&vot);
	c->SetBranchAddress("V_t",&vt);
	c->SetBranchAddress("V_volName",&vvn);

	TFile * of = new TFile("output.root","RECREATE");
	TTree * ot = new TTree("t","t");
	double time,otime,ox,oy,oz,x,y,z,px,py,pz;
	int pid;
	char* vn = (char*) malloc(128);
	ot->Branch("evt_num",&evt_num);
	ot->Branch("pid",&pid);
	ot->Branch("vn",vn,"vn/C");
	ot->Branch("ot",&otime);
	ot->Branch("ox",&ox);
	ot->Branch("oy",&oy);
	ot->Branch("oz",&oz);
	ot->Branch("x",&x);
	ot->Branch("y",&y);
	ot->Branch("z",&z);
	ot->Branch("t",&time);
	ot->Branch("px",&px);
	ot->Branch("py",&py);
	ot->Branch("pz",&pz);
	Long64_t nEvents = c->GetEntries();
	for ( Long64_t iEvent = 0; iEvent<nEvents; iEvent++){
		if (iEvent%1000==0) printf("%lf%...\n",(double)iEvent/nEvents*100);
		c->GetEntry(iEvent);
		for (int i  = 0; i<pretid.size(); i++){
			pretid[i]= -1;
		}
		for (int i = 0; i<vtid->size(); i++){
			int j = 0;
			for (; j<vMonName.size(); j++){
				if (vMonName[j] == (*vvn)[i]) break;
			}
			if (j==vMonName.size()){
				printf("cannot find \"%s\"\n",(*vvn)[i]);
				continue;
			}
			if (pretid[j]!=(*vtid)[i]){
				pretid[j] = (*vtid)[i];
				pid = (*vpid)[i];
				ox = (*vox)[i];
				oy = (*voy)[i];
				oz = (*voz)[i];
				//otime = (*vot)[i];
				time = (*vt)[i];
				x = (*vx)[i];
				y = (*vy)[i];
				z = (*vz)[i];
				px = (*vpx)[i];
				py = (*vpy)[i];
				pz = (*vpz)[i];
				std::strcpy(vn,((*vvn)[i]).c_str());
				ot->Fill();
			}
		}
	}
	ot->Write();
}
