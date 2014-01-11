{
	TString name = "1p4_1215";
	TFile *_file0 = TFile::Open(name+".root");
	TTree * t= (TTree*) _file0->Get("tree");
	std::vector<double> * edep = 0;
	std::vector<double> * x = 0;
	std::vector<double> * y = 0;
	std::vector<double> * z = 0;
	std::vector<int> * tid = 0;
	std::vector<std::string> * volName = 0;
	t->SetBranchAddress("T_edep",&edep);
	t->SetBranchAddress("T_volName",&volName);
	t->SetBranchAddress("T_Ox",&x);
	t->SetBranchAddress("T_Oy",&y);
	t->SetBranchAddress("T_Oz",&z);
	t->SetBranchAddress("T_tid",&tid);
	int nEvents = t->GetEntries();
	TH1D* h1 = new TH1D("edep","Energy Deposition in SiR2 (45 degree facing beam)",150,0,8);
	h1->GetXaxis()->SetTitle("Energy Deposition (MeV)");
	h1->GetYaxis()->SetTitle("Count");
	TH1D* h2 = new TH1D("r","Stopping Depth in SiR2 (45 degree facing beam)",300,0,1500);
	h2->GetXaxis()->SetTitle("Depth (um)");
	h2->GetYaxis()->SetTitle("Count");
	for ( int iEvent = 0; iEvent < nEvents; iEvent++ ){
		t->GetEntry(iEvent);
		int nHits = 0;
		if (edep)
			nHits = edep->size();
//		std::cout<<"Event["<<iEvent<<"]: "<<nHits<<std::endl;
		for (int iHit = 0; iHit < nHits; iHit++ ){
			if ((*volName)[iHit]!="ESi1") continue;
			if ((*tid)[iHit]!=1) continue;
			h1->Fill((*edep)[iHit]*1000);
			h2->Fill((((*x)[iHit]+(*z)[iHit])/sqrt(2)-0.04675+0.075)*10000);
		}
	}
	gStyle->SetOptStat(1111);
	gStyle->SetOptFit(111);
	gStyle->SetLabelSize(0.03,"x");
	gStyle->SetLabelSize(0.03,"y");
	TF1 *f1 = new TF1("f1","gaus",0,8);
	h1->Fit("f1","h1");
	TF1 *f2 = new TF1("f2","gaus",0,1500);
	h2->Fit("f2","h2");
	TCanvas * c1 = new TCanvas("c1","c1");
	c1->cd();
	h1->Draw();
	c1->Print(name+"_edep.pdf");
	c1->Print(name+"_edep.png");
	TCanvas * c2 = new TCanvas("c2","c2");
	c2->cd();
	h2->Draw();
	c2->Print(name+"_r.pdf");
	c2->Print(name+"_r.png");
}
