{
	double PI = 3.141592653589793238;
	double M_MUON = 105.6584; //mass of muon in MeV

	TString name = "100umAl_1p10_1per";
//	TString name = "1500umSi_1p4_3per";
	TString DESC = "SF=1.4, FWHM=3%";
//	TString type = "1500umSi,45deg";
//	TString type = "65umSi,45deg";
	TString type = "100umAl,45deg";
	TFile *_file0 = TFile::Open(name+".root");
	TTree * t= (TTree*) _file0->Get("tree");
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
	std::vector<int> * tid = 0;
	std::vector<std::string> * volName = 0;
	t->SetBranchAddress("T_edep",&edep);
	t->SetBranchAddress("T_volName",&volName);
	t->SetBranchAddress("T_px",&px);
	t->SetBranchAddress("T_py",&py);
	t->SetBranchAddress("T_pz",&pz);
	t->SetBranchAddress("T_Opx",&Opx);
	t->SetBranchAddress("T_Opy",&Opy);
	t->SetBranchAddress("T_Opz",&Opz);
	t->SetBranchAddress("T_Ox",&x);
	t->SetBranchAddress("T_Oy",&y);
	t->SetBranchAddress("T_Oz",&z);
	t->SetBranchAddress("T_tid",&tid);
	int nEvents = t->GetEntries();
	double EMin = 0.1;
	double EMax = 5;
	double RMin = 0;
	double RMax = 100;
	double EkinMin = 0.1;
	double EkinMax = 5;

	if (type == "1500umSi,45deg"){
//		RMax = 1500;
		RMax = 500;
	}
	else if (type == "65umSi,45deg"){
	}

	TFile * ofile = new TFile("output.root","RECREATE");
	TTree * ntree = new TTree("t","t");
	double pos_x;
	double pos_y;
	double pos_z;
	ntree->Branch("x",&pos_x);
	ntree->Branch("y",&pos_y);
	ntree->Branch("z",&pos_z);

/*
	TH1D* h1 = new TH1D("edep","Energy Deposition, " + DESC + ", " + type,150,EMin,EMax);
	h1->GetXaxis()->SetTitle("Energy Deposition (MeV)");
	h1->GetYaxis()->SetTitle("Count");
	TH1D* h2 = new TH1D("edepS","Energy Deposition, " + DESC + ", " + type,150,EMin,EMax);
	TH1D* h3 = new TH1D("edepN","Not Stopped",150,EMin,EMax);

	TH1D* hh1 = new TH1D("R","Stoppting Depth, " + DESC + ", " + type,150,RMin,RMax);
	hh1->GetXaxis()->SetTitle("Depth (um)");
	hh1->GetYaxis()->SetTitle("Count");
	TH1D* hh2 = new TH1D("RS","Stoppting Depth, " + DESC + ", " + type,150,RMin,RMax);
	hh2->GetXaxis()->SetTitle("Depth (um)");
	hh2->GetYaxis()->SetTitle("Count");
	TH1D* hh3 = new TH1D("RN","Not Stopped",150,RMin,RMax);

	TH1D* hhh1 = new TH1D("Ek","Kinetic Energy, " + DESC + ", " + type,150,EkinMin,EkinMax);
	hhh1->GetXaxis()->SetTitle("E_{kin} (MeV)");
	hhh1->GetYaxis()->SetTitle("Count");
	TH1D* hhh2 = new TH1D("EkS","Stopped",150,EkinMin,EkinMax);
	hhh2->GetXaxis()->SetTitle("E_{kin} (MeV)");
	hhh2->GetYaxis()->SetTitle("Count");
	TH1D* hhh3 = new TH1D("EkN","Not Stopped",150,EkinMin,EkinMax);
	*/

	for ( int iEvent = 0; iEvent < nEvents; iEvent++ ){
		t->GetEntry(iEvent);
		int nHits = 0;
		if (edep)
			nHits = edep->size();
		for (int iHit = 0; iHit < nHits; iHit++ ){
			double depth;
			if (type == "1500umSi,45deg"){
				if ((*volName)[iHit]!="ESi1") continue;
				depth = (((*x)[iHit]+(*z)[iHit])/sqrt(2)-0.04675+0.075)*10000;
			}
			else if (type == "65umSi,45deg"){
				if ((*volName)[iHit]!="dESi1") continue;
				depth = (((*x)[iHit]+(*z)[iHit])/sqrt(2)+0.22175+65e-4)*10000;
			}
			else if (type == "100umAl,45deg"){
				if ((*volName)[iHit]!="Target") continue;
				depth = ((-(*x)[iHit]+(*z)[iHit])/sqrt(2)+100e-4/2)*10000;
			}
//			h1->Fill((*edep)[iHit]*1000);
//			double ipx = (*px)[iHit];
//			double ipy = (*py)[iHit];
//			double ipz = (*pz)[iHit];
//			double pa = sqrt(ipx*ipx+ipy*ipy+ipz*ipz)*1000;
//			double ekin = sqrt(M_MUON*M_MUON+pa*pa)-M_MUON;
//			hh1->Fill(depth);
//			hhh1->Fill(ekin);
			bool stopped = (*Opx)[iHit]==0&&(*Opy)[iHit]==0&&(*Opz)[iHit]==0;
			if ((*tid)[iHit]==1){
				if (stopped){
					pos_x = (*x)[iHit]*10;
					pos_y = (*y)[iHit]*10;
					pos_z = (*z)[iHit]*10;
					ntree->Fill();
//					h2->Fill((*edep)[iHit]*1000);
//					hh2->Fill(depth);
//					hhh2->Fill(ekin);
				}
				else {
//					h3->Fill((*edep)[iHit]*1000);
//					hh3->Fill(depth);
//					hhh3->Fill(ekin);
				}
			}
		}
	}
	ntree->Write();

	/*
	gStyle->SetOptStat(1111);
	gStyle->SetOptFit(111);

	TCanvas * c1 = new TCanvas("c1","c1",1200,600);
	c1->cd();
	TPad *pad1 = new TPad("pad1","",0,0,0.3333,1);
	TPad *pad2 = new TPad("pad2","",0.3333,0,0.6666,1);
	TPad *pad3 = new TPad("pad3","",0.6666,0,1,1);
	pad1->Draw();
	pad2->Draw();
	pad3->Draw();
	pad1->SetGridx(1);
	pad1->SetGridy(1);
	pad2->SetGridx(1);
	pad2->SetGridy(1);
	pad3->SetGridx(1);
	pad3->SetGridy(1);

	pad1->cd();
	hhh1->SetLineColor(1);
	hhh1->Draw();
	hhh2->SetLineColor(632);
	hhh2->SetFillColor(632);
	hhh2->SetFillStyle(3004);
	hhh2->Draw("SAME");
	hhh3->SetLineColor(600);
	hhh3->SetFillColor(600);
	hhh3->SetFillStyle(3005);
	hhh3->Draw("SAME");
	TLegend *legend = new TLegend(0.5,0.6,0.99,0.77);
	legend->AddEntry(hhh1,"All Particles Hitting Target");
	legend->AddEntry(hhh2,"Stopped Muons");
	legend->AddEntry(hhh3,"Unstopped Muons");
	legend->Draw("SAME");

	pad2->cd();
	h1->SetLineColor(1);
	h1->Draw();
	h2->SetLineColor(632);
	h2->SetFillColor(632);
	h2->SetFillStyle(3004);
	h2->Draw("SAME");
	TF1 *f1 = new TF1("f1","gaus",EMin,EMax);
	h2->Fit("f1","h2");
	h3->SetLineColor(600);
	h3->SetFillColor(600);
	h3->SetFillStyle(3005);
	h3->Draw("SAME");
//	TLegend *legend = new TLegend(0.1,0.7,0.5,0.9);
//	legend->AddEntry(h1,"All Particles Hitting Target");
//	legend->AddEntry(h2,"Stopped Muons");
//	legend->AddEntry(h3,"Unstopped Muons");
//	legend->Draw("SAME");

	pad3->cd();
//	hh1->SetLineColor(1);
//	hh1->Draw();
	hh2->SetLineColor(632);
	hh2->SetFillColor(632);
	hh2->SetFillStyle(3004);
	TF1 *f2 = new TF1("f2","gaus",RMin,RMax);
	hh2->Fit("f2","hh2");
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

	c1->Print(name+".pdf");
	c1->Print(name+".png");
	*/
}
