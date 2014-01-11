{
//	TString name = "65um_m2per";
	TString name = "65um_1p1_3per";
	TString DESC = "SF=1.10, FWHM=3%";
	TFile *_file0 = TFile::Open(name+".root");
	TTree * tree= (TTree*) _file0->Get("tree");
	std::vector<double> * edep = 0;
	std::vector<double> * px = 0;
	std::vector<double> * py = 0;
	std::vector<double> * pz = 0;
	std::vector<double> * Opx = 0;
	std::vector<double> * Opy = 0;
	std::vector<double> * Opz = 0;
	std::vector<double> * x = 0;
	std::vector<double> * y = 0;
	std::vector<double> * z = 0;
	std::vector<double> * t = 0;
	std::vector<double> * t_SC = 0;
	std::vector<double> * edep_SC = 0;
	std::vector<int> * tid = 0;
	std::vector<std::string> * volName = 0;
	tree->SetBranchAddress("T_edep",&edep);
	tree->SetBranchAddress("T_volName",&volName);
	tree->SetBranchAddress("T_px",&px);
	tree->SetBranchAddress("T_py",&py);
	tree->SetBranchAddress("T_pz",&pz);
	tree->SetBranchAddress("T_Opx",&Opx);
	tree->SetBranchAddress("T_Opy",&Opy);
	tree->SetBranchAddress("T_Opz",&Opz);
	tree->SetBranchAddress("T_Ox",&x);
	tree->SetBranchAddress("T_Oy",&y);
	tree->SetBranchAddress("T_Oz",&z);
	tree->SetBranchAddress("T_tid",&tid);
	tree->SetBranchAddress("T_t",&t);
	tree->SetBranchAddress("M_t",&t_SC);
	tree->SetBranchAddress("M_edep",&edep_SC);
	int nEvents = tree->GetEntries();
	double paMin = 0;
	double paMax = 40;
	double EMin = 0.1;
	double EMax = 3;
//	double EMax = 8;
	double RMin = 0;
	double RMax = 75;
	TH1D* h1 = new TH1D("edep","Energy Deposition in SiR1 (45 degree facing beam), " + DESC,150,EMin,EMax);
	h1->GetXaxis()->SetTitle("Energy Deposition (MeV)");
	h1->GetYaxis()->SetTitle("Count");
	TH1D* h2 = new TH1D("edepS","Stopped",150,EMin,EMax);
	TH1D* h3 = new TH1D("edepN","Not Stopped",150,EMin,EMax);
	TH1D* hh1 = new TH1D("R","Stoppting Depth in SiR1 (45 degree facing beam), " + DESC,150,RMin,RMax);
	hh1->GetXaxis()->SetTitle("Depth (um)");
	hh1->GetYaxis()->SetTitle("Count");
	TH1D* hh2 = new TH1D("RS","Stoppting Depth in SiR1 (45 degree facing beam), " + DESC,150,RMin,RMax);
	hh2->GetXaxis()->SetTitle("Depth (um)");
	hh2->GetYaxis()->SetTitle("Count");
	TH1D* hh3 = new TH1D("RN","Not Stopped",150,RMin,RMax);
	TH2D* hcoin = new TH2D("coin1","Energy Deposition VS Time Diff, "+DESC, 1000, -20, 20, 150, EMin, EMax);
	TH2D* hcoin2 = new TH2D("coin2","Momentum VS Time Diff, "+DESC, 1000, -20, 20, 150, paMin, paMax);
	TH1D* hhh1 = new TH1D("tdiff","Time Difference with MuSC, " + DESC,150,RMin,RMax);
	hhh1->GetXaxis()->SetTitle("Time Difference (ns)");
	hhh1->GetYaxis()->SetTitle("Count");
	TH1D* hhh2 = new TH1D("pa","Momentum Upon Arrival, " + DESC,150,RMin,RMax);
	hhh2->GetXaxis()->SetTitle("Momentum Amplitude (MeV/c)");
	hhh2->GetYaxis()->SetTitle("Count");
	for ( int iEvent = 0; iEvent < nEvents; iEvent++ ){
		tree->GetEntry(iEvent);
		int nHits = 0;
		if (t_SC)
			nHits = t_SC->size();
		double tmin = 1e14;
		for (int iHit = 0; iHit < nHits; iHit++ ){
			if ((*edep_SC)[iHit]>0.0008&&tmin>(*t_SC)[iHit]){
				tmin = (*t_SC)[iHit];
			}
		}
		if (edep)
			nHits = edep->size();
//		std::cout<<"Event["<<iEvent<<"]: "<<nHits<<std::endl;
		for (int iHit = 0; iHit < nHits; iHit++ ){
			if ((*volName)[iHit]!="dESi1") continue;
//			if ((*tid)[iHit]!=1) continue;
			double ipx = (*px)[iHit];
			double ipy = (*py)[iHit];
			double ipz = (*pz)[iHit];
			double pa = sqrt(ipx*ipx+ipy*ipy+ipz*ipz);
			h1->Fill((*edep)[iHit]*1000);
			double depth = (((*x)[iHit]+(*z)[iHit])/sqrt(2)+0.22175+65e-4)*10000;
			hh1->Fill(depth);
			bool stopped = (*Opx)[iHit]==0&&(*Opy)[iHit]==0&&(*Opz)[iHit]==0;
			hcoin->Fill((*t)[iHit]-tmin,(*edep)[iHit]*1000);
			if ((*tid)[iHit]==1){
				if (stopped){
					h2->Fill((*edep)[iHit]*1000);
					hh2->Fill(depth);
				}
				else {
					h3->Fill((*edep)[iHit]*1000);
					hh3->Fill(depth);
				}
				hcoin2->Fill((*t)[iHit]-tmin,pa*1000);
				hhh1->Fill((*t)[iHit]-tmin);
				hhh2->Fill(pa*1000);
			}
		}
	}
	TCanvas * c1 = new TCanvas("c1","c1");
	c1->cd();
	TPad *pad1 = new TPad("pad1","",0,0,0.5,1);
	TPad *pad2 = new TPad("pad1","",0.5,0,1,1);
	pad1->Draw();
	pad2->Draw();
	pad1->SetGridx(1);
	pad1->SetGridy(1);
	pad2->SetGridx(1);
	pad2->SetGridy(1);
	pad1->cd();
	h1->SetLineColor(1);
	h1->Draw();
	h2->SetLineColor(632);
	h2->SetFillColor(632);
	h2->SetFillStyle(3004);
	h2->Draw("SAME");
	h3->SetLineColor(600);
	h3->SetFillColor(600);
	h3->SetFillStyle(3005);
	h3->Draw("SAME");
	TLegend *legend = new TLegend(0.1,0.7,0.5,0.9);
	legend->AddEntry(h1,"All Particles Hitting Target");
	legend->AddEntry(h2,"Stopped Muons");
	legend->AddEntry(h3,"Unstopped Muons");
	legend->Draw("SAME");
	pad2->cd();
//	hh1->SetLineColor(1);
//	hh1->Draw();
	hh2->SetLineColor(632);
	hh2->SetFillColor(632);
	hh2->SetFillStyle(3004);
//	hh2->Draw("SAME");
	hh2->Draw();
//	hh3->SetLineColor(600);
//	hh3->SetFillColor(600);
//	hh3->SetFillStyle(3005);
//	hh3->Draw("SAME");
//	TLegend *legend2 = new TLegend(0.1,0.7,0.5,0.9);
//	legend2->AddEntry(hh1,"All Particles Hitting Target");
//	legend2->AddEntry(hh2,"Stopped Muons");
//	legend2->AddEntry(hh3,"Unstopped Muons");
//	legend2->Draw("SAME");

//	c1->Print(name+".pdf");
//	c1->Print(name+".png");

	TCanvas * c2 = new TCanvas("c2","c2");
	c2->cd();
	hcoin->Draw();

	TCanvas * c3 = new TCanvas("c3","c3");
	c3->cd();
	hcoin2->Draw();

	TCanvas * c4 = new TCanvas("c4","c4");
	c4->cd();
	pad1 = new TPad("pad1","",0,0,0.5,1);
	pad2 = new TPad("pad1","",0.5,0,1,1);
	pad1->Draw();
	pad2->Draw();
	pad1->SetGridx(1);
	pad1->SetGridy(1);
	pad2->SetGridx(1);
	pad2->SetGridy(1);
	pad1->cd();
	hhh1->Draw();
	pad2->cd();
	hhh2->Draw();
}
