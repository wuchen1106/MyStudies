{
	TFile *f = 0;

	TChain *c = new TChain("tree");

	bool withStopPosition = true;
	double PulseInterval = 1170;

	f = new TFile("Curves.s100.root");
	std::cout<<"Integrating..."<<std::endl;
//	TH1D *hCurve = (TH1D*) f->Get("ProtonPuls");
	TH1D *hCurve = (TH1D*) f->Get("Convoluted");
	hCurve->RebinX(20);

	double NperP = 860498./1e8;
	TString par = "#mu^{-}";

//	double NperP = 706288/1e9;
//	TString par = "#pi^{-}";

//	TString runName = "muon.14.1";
//	TString option = "Rmin = 14 cm, L = 1 m";

//	TString runName = "pion.14.1";
//	TString option = "Rmin = 14 cm, L = 1 m";

//	TString runName = "muon.12.p5";
//	TString option = "Rmin = 12 cm, L = 50 cm";

//	TString runName = "pion.12.p5";
//	TString option = "Rmin = 12 cm, L = 50 cm";

//	TString runName = "muon.10.p25";
//	TString option = "Rmin = 10 cm, L = 25 cm";

//	TString runName = "pion.10.p25";
//	TString option = "Rmin = 10 cm, L = 25 cm";

	TString runName = "muon.11.p5"; // 7418778 events
	TString option = "Rmin = 11 cm, L = 50 cm";

//	TString runName = "pion.11.p5";
//	TString option = "Rmin = 11 cm, L = 50 cm";

//	TString runName = "muon.10.p5";
//	TString option = "Rmin = 10 cm, L = 50 cm";

//	TString runName = "pion.10.p5";
//	TString option = "Rmin = 10 cm, L = 50 cm";

//	c->Add(runName+".root");

	std::stringstream buff;
	for (int i = 0; i<10; i++){
		buff.str("");
		buff.clear();
		buff<<runName<<"/"<<i<<"_job0.raw";
		c->Add( buff.str().c_str());
	}

	TH2D * h01 = new TH2D("h01",par+" Before Collimator",50,0,180,50,-200,200);
	TH2D * h02 = new TH2D("h02",par+" After Collimator",50,0,180,50,-200,200);
	TH2D * h03 = new TH2D("h03",par+" Stopped in Target",50,0,180,50,-200,200);
	TH1D * h04 = new TH1D("h04",par+" Transverse Distribution",100,0,100);
//	TH1D * h05 = new TH1D("h05",par+" Longitudinal Distribution",200,-900,900);
	TH1D * h05 = new TH1D("h05",par+" Longitudinal Distribution",200,-1000,0);
	TH1D *h0 = new TH1D("h0","h0",200,0,PulseInterval);
	TH1D *h1 = new TH1D("h1","h1",200,0,PulseInterval);
	std::vector<int> *T_tid;
	std::vector<std::string> *T_volName;
	std::vector<double> *McTruth_x;
	std::vector<double> *McTruth_y;
	std::vector<double> *McTruth_z;
	std::vector<double> *McTruth_px;
	std::vector<double> *McTruth_py;
	std::vector<double> *McTruth_pz;
	std::vector<double> *T_Ox;
	std::vector<double> *T_Oy;
	std::vector<double> *T_Oz;
	std::vector<double> *T_Ot;
	int V_nHits;
	int T_nHits;

	double weight;
	if (withStopPosition){
		c->SetBranchAddress("T_Ox",&T_Ox);
		c->SetBranchAddress("T_Oy",&T_Oy);
		c->SetBranchAddress("T_Oz",&T_Oz);
		c->SetBranchAddress("T_Ot",&T_Ot);
	}
	c->SetBranchAddress("T_nHits",&T_nHits);
	c->SetBranchAddress("V_nHits",&V_nHits);
	c->SetBranchAddress("McTruth_px",&McTruth_px);
	c->SetBranchAddress("McTruth_py",&McTruth_py);
	c->SetBranchAddress("McTruth_pz",&McTruth_pz);
	c->SetBranchAddress("McTruth_x",&McTruth_x);
	c->SetBranchAddress("McTruth_y",&McTruth_y);
	c->SetBranchAddress("McTruth_z",&McTruth_z);
	c->SetBranchAddress("weight",&weight);
	double Ox = 0;
	double Oy = 0;
	double Oz = 0;
	double Ot = 0;
	double x = 0;
	double y = 0;
	double z = 0;
	double px = 0;
	double py = 0;
	double pz = 0;
	bool stopped = false;
	bool passed = false;
	double nPassed = 0;
	double nStopped = 0;
	double nTotal = 0;
	if (withStopPosition){
		f = new TFile(runName+".output.root","RECREATE");
	}
	TTree *t  = new TTree("t","t");
	if (withStopPosition){
		t->Branch("x",&Ox);
		t->Branch("y",&Oy);
		t->Branch("z",&Oz);
		t->Branch("t",&Ot);
		t->Branch("weight",&weight);
	}
	int nEvents = c->GetEntries();
	std::cout<<nEvents<<" events!!!"<<std::endl;
	for (int iEvent = 0; iEvent < nEvents; iEvent++ ){
		if (iEvent%1000==0) std::cout<<(double)iEvent/nEvents*100<<" % ..."<<std::endl;
		c->GetEntry(iEvent);
		nTotal += weight;
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
		if (V_nHits>0) passed=true;
		if (T_nHits>0){
			if (withStopPosition){
				Ox = (*T_Ox)[0]*10;
				Oy = (*T_Oy)[0]*10;
				Oz = (*T_Oz)[0]*10;
				Ot = (*T_Ot)[0];
				r = sqrt(Ox*Ox+Oy*Oy);
			}
			stopped=true;
		}
		h01->Fill(pa,y,weight);
		if (passed){
			nPassed+=weight;
			h02->Fill(pa,y,weight);
		}
		if (stopped){
			nStopped+=weight;
			h03->Fill(pa,y,weight);
			if (withStopPosition){
				h04->Fill(r,weight);
				h05->Fill(Oz,weight);
				t->Fill();
			}
			h0->Fill((*T_Ot)[0],weight);
			for (int i_window = 0; i_window <5; i_window++){
				h1->Fill(Ot+hCurve->GetRandom()+i_window*PulseInterval,weight);
			}
		}
	}
	h1->Scale(h1->Integral());
	h0->Scale(h0->Integral());
	TH1D *h2 = new TH1D("h2","h2",200,0,PulseInterval);
	for (int i = 1; i<=200; i++){
		double n = h1->Integral(i,200);
		h2->SetBinContent(i,n);
	}
	h0->Write();
	h1->Write();
	h2->Write();

	TCanvas *c1 = new TCanvas("c","c",1024,768);
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
	h01->SetContour(50);
	h02->SetContour(50);
	h03->SetContour(50);
	p1->cd();
	h01->Draw("CONT0 COLZ");
	p2->cd();
	h02->Draw("CONT0 COLZ");
	p3->cd();
	h03->Draw("CONT0 COLZ");
//	TPaveText *sum = new TPaveText(0.4,0.75,0.77,0.9,"brNDC");
	TPaveText *sum = new TPaveText(0.5,0.75,0.9,0.9,"brNDC");
	sum->AddText(option);
	buff.str("");
	buff.clear();
	buff<<"N_{"<<par<<"_{pass}}/N_{p^{+}} = "<<nPassed*NperP/nTotal<<std::endl;
	sum->AddText(buff.str().c_str());
	buff.str("");
	buff.clear();
	buff<<"N_{"<<par<<"_{stop}}/N_{p^{+}} = "<<nStopped*NperP/nTotal<<std::endl;
	sum->AddText(buff.str().c_str());
	sum->Draw();
	c1->SaveAs(runName+".paVSy.pdf");
	c1->SaveAs(runName+".paVSy.png");

	if (withStopPosition){
		TCanvas *c2 = new TCanvas("c2","c2",1024,768);
		TPad * p4 = new TPad("p4","p4",0,0,1./2,1);
		TPad * p5 = new TPad("p5","p5",1./2,0,2./2,1);
		p4->Draw();
		p5->Draw();
		p4->cd();
		h04->Draw();
		p5->cd();
		h05->Draw();
		std::cout<<"nStopped = "<<nStopped<<std::endl;
		if (withStopPosition){
			t->Write();
		}
		c2->SaveAs(runName+".SP.pdf");
		c2->SaveAs(runName+".SP.png");
	}
}
