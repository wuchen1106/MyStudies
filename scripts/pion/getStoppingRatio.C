{
	TChain *c = new TChain("tree");
	c->Add("014.root");
	std::stringstream buff;
//	for (int i = 0; i<50; i++){
//		buff.str("");
//		buff.clear();
//		buff<<"/home/chen/MyWorkArea/Data/raw/g4sim/BLTCDC.pim.g60cm10mm.005T.2p4_0731.g4s.QBH/raw/"<<i<<"_job0.raw";
//		c->Add( buff.str().c_str());
//	}
	TChain *c0 = new TChain("t");
	c0->Add("/home/chen/MyWorkArea/g4sim/data/MT1.pim.pim.g60cm10mm.005T.g4s.QBH.root");
	TH2D * h1 = new TH2D("h1","Before Collimator",50,0,180,50,-200,200);
	TH2D * h2 = new TH2D("h2","After Collimator",50,0,180,50,-200,200);
	TH2D * h3 = new TH2D("h3","Stopped in Target",50,0,180,50,-200,200);
	TH1D * h4 = new TH1D("h4","Transverse Distribution",100,0,100);
//	TH1D * h5 = new TH1D("h5","Longitudinal Distribution",200,-900,900);
	TH1D * h5 = new TH1D("h5","Longitudinal Distribution",200,-1000,0);
	std::vector<int> *T_tid;
	std::vector<std::string> *T_volName;
	std::vector<double> *McTruth_x;
	std::vector<double> *McTruth_y;
	std::vector<double> *McTruth_z;
	std::vector<double> *McTruth_px;
	std::vector<double> *McTruth_py;
	std::vector<double> *McTruth_pz;
	std::vector<double> *T_x;
	std::vector<double> *T_y;
	std::vector<double> *T_z;
	std::vector<double> *T_px;
	std::vector<double> *T_py;
	std::vector<double> *T_pz;
	std::vector<double> *T_t;
	std::vector<std::string> *M_volName;
	std::vector<int> *M_tid;
	std::vector<double> *M_px;
	std::vector<double> *M_py;
	std::vector<double> *M_pz;
	double weight;
	int evt_num;
	c->SetBranchAddress("evt_num",&evt_num);
	c->SetBranchAddress("T_volName",&T_volName);
	c->SetBranchAddress("M_volName",&M_volName);
	c->SetBranchAddress("T_tid",&T_tid);
	c->SetBranchAddress("M_tid",&M_tid);
	c->SetBranchAddress("T_Ox",&T_x);
	c->SetBranchAddress("T_Oy",&T_y);
	c->SetBranchAddress("T_Oz",&T_z);
	c->SetBranchAddress("T_t",&T_t);
	c->SetBranchAddress("T_Opx",&T_px);
	c->SetBranchAddress("T_Opy",&T_py);
	c->SetBranchAddress("T_Opz",&T_pz);
	c->SetBranchAddress("M_px",&M_px);
	c->SetBranchAddress("M_py",&M_py);
	c->SetBranchAddress("M_pz",&M_pz);
	c->SetBranchAddress("McTruth_px",&McTruth_px);
	c->SetBranchAddress("McTruth_py",&McTruth_py);
	c->SetBranchAddress("McTruth_pz",&McTruth_pz);
	c->SetBranchAddress("McTruth_x",&McTruth_x);
	c->SetBranchAddress("McTruth_y",&McTruth_y);
	c->SetBranchAddress("McTruth_z",&McTruth_z);
	c0->SetBranchAddress("weight",&weight);
	int tid = -1;
	std::string volName;
	double stx = 0;
	double sty = 0;
	double stz = 0;
	double x = 0;
	double y = 0;
	double z = 0;
	double px = 0;
	double py = 0;
	double pz = 0;
	double Ot = 0;
	double Opx = 0;
	double Opy = 0;
	double Opz = 0;
	int nEvents = c->GetEntries();
	bool stopped = false;
	bool passed = false;
	double nStopped = 0;
	double nPassed = 0;
	TFile *f = new TFile("output.root","RECREATE");
	TTree *t  = new TTree("t","t");
	int pid = -211;
	t->Branch("pid",&pid);
	t->Branch("x",&stx);
	t->Branch("y",&sty);
	t->Branch("z",&stz);
	t->Branch("t",&Ot);
	t->Branch("px",&Opx);
	t->Branch("py",&Opy);
	t->Branch("pz",&Opz);
	t->Branch("weight",&weight);
	for (int iEvent = 0; iEvent < nEvents; iEvent++ ){
		c->GetEntry(iEvent);
		c0->GetEntry(evt_num);
		stopped=false;
		passed=false;
		x = (*McTruth_x)[0]*10;
		y = (*McTruth_y)[0]*10;
		z = (*McTruth_z)[0]*10;
		px = (*McTruth_px)[0]*1000;
		py = (*McTruth_py)[0]*1000;
		pz = (*McTruth_pz)[0]*1000;
		double pa = sqrt(px*px+py*py+pz*pz);
		double r = -1;
		int nHits = T_px->size();
		for (int iHit = 0; iHit < nHits; iHit++){
			tid = (*T_tid)[iHit];
			volName = (*T_volName)[iHit];
			if (volName=="Target"&&tid==1){
				Opx = (*T_px)[iHit]*1000;
				Opy = (*T_py)[iHit]*1000;
				Opz = (*T_pz)[iHit]*1000;
				if (Opx==0&&Opy==0&&Opz==0){
					Ot = (*T_t)[iHit];
					stx = (*T_x)[iHit]*10;
					sty = (*T_y)[iHit]*10;
					stz = (*T_z)[iHit]*10;
					double r = sqrt(stx*stx+sty*sty);
					stopped=true;
				}
			}
		}
		nHits = M_px->size();
		for (int iHit = 0; iHit < nHits; iHit++){
			tid = (*M_tid)[iHit];
			volName = (*M_volName)[iHit];
			if (volName == "EndColl"&&tid==1){
				passed=true;
			}
		}
		h1->Fill(pa,y,weight);
		if (passed){
			nPassed+=weight;
			h2->Fill(pa,y,weight);
		}
		if (stopped){
			nStopped+=weight;
			h3->Fill(pa,y,weight);
			h4->Fill(r,weight);
			h5->Fill(stz,weight);
			t->Fill();
		}
	}
	TCanvas *c1 = new TCanvas("c","c");
	gStyle->SetPalette(1);
	gStyle->SetOptStat(0);
	gStyle->SetPadTickX(1);
	gStyle->SetPadTickY(1);
	TPad * p1 = new TPad("p1","p1",0,0,1./3,1);
	TPad * p2 = new TPad("p2","p2",1./3,0,2./3,1);
	TPad * p3 = new TPad("p3","p3",2./3,0,3./3,1);
	p1->Draw();
	p2->Draw();
	p3->Draw();
	h1->SetContour(50);
	h2->SetContour(50);
	h3->SetContour(50);
	p1->cd();
	h1->Draw("CONT0 COLZ");
	p2->cd();
	h2->Draw("CONT0 COLZ");
	p3->cd();
	h3->Draw("CONT0 COLZ");
	TCanvas *c2 = new TCanvas("c2","c2");
	TPad * p4 = new TPad("p4","p4",0,0,1./2,1);
	TPad * p5 = new TPad("p5","p5",1./2,0,2./2,1);
	p4->Draw();
	p5->Draw();
	p4->cd();
	h4->Draw();
	p5->cd();
	h5->Draw();
	std::cout<<"nStopped = "<<nStopped<<std::endl;
	t->Write();
}
