//TString runNumber = "022"; // 1.5m uniform 130731
//TString runNumber = "033"; // 1.5m uniform 130927 with absorber 2mm with 2 cm gap
//TString runNumber = "034"; // 1.5m uniform 130927 with no absorber
//TString runNumber = "035"; // 1.5m uniform 130927 with absorber 2mm with 10 cm gap 
//TString runNumber = "036"; // 1.5m uniform 130927 with absorber 2mm with 5 cm gap 
TString runNumber = "037"; // 1.5m uniform 130927 with absorber 2mm with 2 cm gap

void getBKG(){
	TChain *c = new TChain("tree");
	c->Add(runNumber+".root");
//	std::stringstream buff;
//	for (int i = 0; i<100; i++){
//		buff.str("");
//		buff.clear();
//		buff<<"/home/chen/MyWorkArea/Data/raw/g4sim/RPC.2p4_0731_root/"<<i<<"_job0.raw";
//		c->Add( buff.str().c_str());
//	}
	TChain *c0 = new TChain("t");
	c0->Add("/home/chen/MyWorkArea/g4sim/data/pim.2p4.17.root");

	TFile *f = new TFile("output.root","RECREATE");
	TTree *t = new TTree("t","t");

	TH1D * h10 = new TH1D("pa","Run"+runNumber+": Momentum At First Hit (MeV/c)",100,100,110);
	h10->GetXaxis()->SetTitle("Momentum At First Hit (MeV/c)");
	h10->GetYaxis()->SetTitle("Count");

	std::vector<int> *M_tid;
	std::vector<double> *M_t;
	std::vector<int> *C_tid;
	std::vector<double> *C_t;
	std::vector<double> *C_px;
	std::vector<double> *C_py;
	std::vector<double> *C_pz;
	std::vector<std::string> *C_oprocess;
	std::vector<std::string> *C_ovolName;
	double weight;
	int evt_num;
	c->SetBranchAddress("M_tid",&M_tid);
	c->SetBranchAddress("M_t",&M_t);
	c->SetBranchAddress("C_tid",&C_tid);
	c->SetBranchAddress("C_t",&C_t);
	c->SetBranchAddress("C_px",&C_px);
	c->SetBranchAddress("C_py",&C_py);
	c->SetBranchAddress("C_pz",&C_pz);
	c->SetBranchAddress("C_ovolName",&C_ovolName);
	c->SetBranchAddress("C_oprocess",&C_oprocess);
	c0->SetBranchAddress("weight",&weight);
	double time = -1;
	int nEvents = c->GetEntries();
	std::stringstream buff;
	double pa = -1;
	std::string *process;
	std::string *volume;
	t->Branch("process",&process);
	t->Branch("volume",&volume);
	t->Branch("pa",&pa);
	t->Branch("weight",&weight);
	for (int iEvent = 0; iEvent < nEvents; iEvent++ ){
		c->GetEntry(iEvent);
		c0->GetEntry(evt_num);
		int nHits = C_t->size();
		bool foundhit = false;
		int tid = -1;
		pa = -1;
		for (int iHit = 0; iHit < nHits; iHit++){
			double px = (*C_px)[iHit]*1000;
			double py = (*C_py)[iHit]*1000;
			double pz = (*C_pz)[iHit]*1000;
			double i_pa = sqrt(px*px+py*py+pz*pz);
			if (i_pa > pa){
				pa = i_pa;
				tid = (*C_tid)[iHit];
				time = (*C_t)[iHit];
				*process = (*C_oprocess)[iHit];
				*volume = (*C_ovolName)[iHit];
			}
			foundhit = true;
		}
		if (foundhit){
			double i_time = -1;
			int nHits = M_t->size();
			for (int iHit = 0; iHit < nHits; iHit++){
				double i_tid = (*M_tid)[iHit];
				if (i_tid == tid){
					i_time = (*M_t)[iHit];
					break;
				}
			}
			if (i_time>=time){
				foundhit = false;
			}
		}
		if (foundhit){
			if (*volume!="OuterCylinder"
				&&*volume!="EndPlate_0"
				&&*volume!="EndPlate_1"
				&&*volume!="Trigger_0"
				&&*volume!="Trigger_1"
				)
				h10->Fill(pa,weight);
			if (pa>100&&pa<110)
				t->Fill();
		}
	}

	double count = h10->Integral();
	std::cout<<"Found "<<count<<" good tracks!"<<std::endl;
	TCanvas *c1 = new TCanvas("c","c");
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	gStyle->SetPalette(1);
	gStyle->SetOptStat(0);
	gStyle->SetPadTickX(1);
	gStyle->SetPadTickY(1);
	h10->Draw();

	h10->Write();
	t->Write();
	f->Close();
}
