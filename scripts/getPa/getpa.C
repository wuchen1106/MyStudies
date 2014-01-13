{
//	TString runName = "026"; TString discription = "No InnerWall, No Absorber";
//	TString runName = "027"; TString discription = "InnerWall 50um Mylar, No Absorber";
//	TString runName = "028"; TString discription = "InnerWall 100um Al-Mylar, No Absorber";
//	TString runName = "029"; TString discription = "InnerWall 100um Al-Mylar, Absorber 0.5mm CFRP";
//	TString runName = "031"; TString discription = "InnerWall 100um Al-Mylar, Absorber 1mm CFRP";
	TString runName = "032"; TString discription = "InnerWall 100um Al-Mylar, Absorber 1.5mm CFRP";
//	TString runName = "030"; TString discription = "InnerWall 100um Al-Mylar, Absorber 2mm CFRP";
	TChain *c = new TChain("tree");
	c->Add(runName+".root");
	TChain *c0 = new TChain("t");
	c0->Add("/home/chen/MyWorkArea/g4sim/data/MT1.EP.mum.g60cm10mm.005T.g4s.QBH.root");
	TH1D * h1 = new TH1D("h1","pa @ layer0: "+discription,1000,102.5,105.5);
	h1->GetXaxis()->SetTitle("pa (MeV/c)");
	std::vector<double> *C_px;
	std::vector<double> *C_py;
	std::vector<double> *C_pz;
	double weight;
	int evt_num;
	c->SetBranchAddress("evt_num",&evt_num);
	c->SetBranchAddress("C_px",&C_px);
	c->SetBranchAddress("C_py",&C_py);
	c->SetBranchAddress("C_pz",&C_pz);
	c0->SetBranchAddress("weight",&weight);
	double px = 0;
	double py = 0;
	double pz = 0;
	double pa = -1;
	TFile *f = new TFile("output.root","RECREATE");
	int nEvents = c->GetEntries();
	std::cout<<"nEvents = "<<nEvents<<std::endl;
	for (int iEvent = 0; iEvent < nEvents; iEvent++ ){
		if (iEvent%100==0)
			std::cout<<"iEvent["<<iEvent<<"]: "<<(double)iEvent/nEvents*100<<" % ..."<<std::endl;
		c->GetEntry(iEvent);
		c0->GetEntry(evt_num);
		int nHits = C_px->size();
		for (int iHit = 0; iHit < nHits; iHit++){
			px = (*C_px)[iHit]*1000;
			py = (*C_py)[iHit]*1000;
			pz = (*C_pz)[iHit]*1000;
			pa = sqrt(px*px+py*py+pz*pz);
//			h1->Fill(pa,weight);
			h1->Fill(pa);
		}
	}
	TCanvas *c1 = new TCanvas("c","c");
//	gStyle->SetOptStat(0);
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	h1->Draw();
	c1->SaveAs(runName+".png");
	h1->Write();
}
