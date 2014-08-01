{
	TChain *c = new TChain("tree");
	c->Add("016.root");
	TChain *c0 = new TChain("t");
	c0->Add("/home/chen/MyWorkArea/g4sim/data/MT1.pim.pim.g60cm10mm.005T.g4s.QBH.root");
	TH1D * h1 = new TH1D("h1","Time (ns)",150,0,1500);
	TH1D * h2 = new TH1D("h2","Momentum (MeV/c)",150,100,110);
	std::vector<int> *C_pid;
	std::vector<int> *C_tid;
	std::vector<double> *C_x;
	std::vector<double> *C_y;
	std::vector<double> *C_z;
	std::vector<double> *C_px;
	std::vector<double> *C_py;
	std::vector<double> *C_pz;
	std::vector<double> *C_t;
	std::vector<std::string> *M_volName;
	std::vector<int> *M_tid;
	std::vector<int> *M_t;
	double weight;
	int evt_num;
	c->SetBranchAddress("evt_num",&evt_num);
	c->SetBranchAddress("M_volName",&M_volName);
	c->SetBranchAddress("C_pid",&C_pid);
	c->SetBranchAddress("C_tid",&C_tid);
	c->SetBranchAddress("M_tid",&M_tid);
	c->SetBranchAddress("C_x",&C_x);
	c->SetBranchAddress("C_y",&C_y);
	c->SetBranchAddress("C_z",&C_z);
	c->SetBranchAddress("C_t",&C_t);
	c->SetBranchAddress("C_px",&C_px);
	c->SetBranchAddress("C_py",&C_py);
	c->SetBranchAddress("C_pz",&C_pz);
	c->SetBranchAddress("M_t",&M_t);
	c0->SetBranchAddress("weight",&weight);
	int tid = -1;
	std::string volName;
	double x = 0;
	double y = 0;
	double z = 0;
	double px = 0;
	double py = 0;
	double pz = 0;
	double pa = -1;
	double time = 0;
	bool good = false;
	TFile *f = new TFile("output.root","RECREATE");
	TTree *t  = new TTree("t","t");
	int pid = 11;
	t->Branch("pid",&pid);
	t->Branch("x",&x);
	t->Branch("y",&y);
	t->Branch("z",&z);
	t->Branch("t",&t);
	t->Branch("px",&px);
	t->Branch("py",&py);
	t->Branch("pz",&pz);
	t->Branch("weight",&weight);
	int nEvents = c->GetEntries();
	for (int iEvent = 0; iEvent < nEvents; iEvent++ ){
		c->GetEntry(iEvent);
		c0->GetEntry(evt_num);
		good=false;
		int nHits = C_px->size();
		std::cout<<__LINE__<<std::endl;
		for (int iHit = 0; iHit < nHits; iHit++){
			std::cout<<__LINE__<<std::endl;
			tid = (*C_tid)[iHit];
			pid = (*C_pid)[iHit];
			time = (*C_t)[iHit];
			if (pid==11){
				px = (*C_px)[iHit]*1000;
				py = (*C_py)[iHit]*1000;
				pz = (*C_pz)[iHit]*1000;
				pa = sqrt(px*px+py*py+pz*pz);
				for (int jHit = 0; jHit < M_tid->size(); jHit++){
					int jtid = (*M_tid)[jHit];
					volName = (*M_volName)[jHit];
					double jtime = (*M_t)[jHit];
					if (volName == "Trigger"&&jtid==tid){
						if (jtime>time)
							good = true;
						break;
					}
				}
				if (good){
					h1->Fill(time,weight);
					h2->Fill(pa,weight);
					t->Fill();
				}
			}
		}
	}
	TCanvas *c1 = new TCanvas("c","c");
	TPad * p4 = new TPad("p4","p4",0,0,1./2,1);
	TPad * p5 = new TPad("p5","p5",1./2,0,2./2,1);
	p4->Draw();
	p5->Draw();
	p4->cd();
	h1->Draw();
	p5->cd();
	h2->Draw();
	t->Write();
}
