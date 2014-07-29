anadrift(){

	double duration = 20; // ns

	std::vector<TString> Names;
	Names.push_back("BeamFlash_CdcEndPlate_run1195");
	Names.push_back("BeamFlash_CdcCell_run1195");
	Names.push_back("signal_run1196");
	Names.push_back("BeamFlash_CdcCylinder_run1195");
//	Names.push_back("BeamFlash_CdcWire_run1195");
	Names.push_back("BeamFlash_Others_run1195");

	std::vector<int> Colors;
	Colors.push_back(1); // Black
	Colors.push_back(600); // Blue
	Colors.push_back(632); // Red
	Colors.push_back(416); // Green
	Colors.push_back(616); // Magenta
	Colors.push_back(432); // Cyan

	TCanvas * c1 = new TCanvas("c1","c1");
	TLegend * l1 = new TLegend(0.15,0.55,0.5,0.9);
	TCanvas * c2 = new TCanvas("c2","c2");
	TLegend * l2 = new TLegend(0.15,0.55,0.5,0.9);
	TCanvas * c3 = new TCanvas("c3","c3");
	TLegend * l3 = new TLegend(0.15,0.55,0.5,0.9);
	for ( int iFile = 0; iFile<Names.size(); iFile++){
		TString filename = "result/"+Names[iFile]+".output.root";
		TFile * f = TFile::Open(filename);
		TTree *t  = (TTree*) f->Get("t");
		double t1, t2;
		t->SetBranchAddress("t1",&t1);
		t->SetBranchAddress("t2",&t2);
		TH1D * h1d1 = new TH1D("t1","Smallest Drift Time",256,0,1000);
		TH1D * h1d2 = new TH1D("t2","Largest Drift Time",256,0,1000);
		TH1D * h1d3 = new TH1D("t11","Smallest Drift Time",128,0,50);
		h1d1->SetLineColor(Colors[iFile]);
		h1d1->GetXaxis()->SetTitle("t1 [ns]");
		h1d2->SetLineColor(Colors[iFile]);
		h1d2->GetXaxis()->SetTitle("t2 [ns]");
		h1d3->SetLineColor(Colors[iFile]);
		h1d3->GetXaxis()->SetTitle("t1 [ns]");
		for(int iEntry = 0; iEntry<t->GetEntries(); iEntry++){
			t->GetEntry(iEntry);
			h1d1->Fill(t1);
			h1d2->Fill(t2);
			h1d3->Fill(t1);
		}
		h1d1->Scale(1./h1d1->Integral(););
		h1d2->Scale(1./h1d2->Integral(););
		h1d3->Scale(1./h1d3->Integral(););
		l1->AddEntry(h1d1,Names[iFile]);
		l2->AddEntry(h1d2,Names[iFile]);
		l3->AddEntry(h1d1,Names[iFile]);
		c1->cd();
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	gStyle->SetPadTickX(1);
	gStyle->SetPadTickY(1);
	gStyle->SetOptStat(0);
		if (iFile==0)	h1d1->Draw();
		else	h1d1->Draw("SAME");
		c2->cd();
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	gStyle->SetPadTickX(1);
	gStyle->SetPadTickY(1);
	gStyle->SetOptStat(0);
		if (iFile==0)	h1d2->Draw();
		else	h1d2->Draw("SAME");
		c3->cd();
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	gStyle->SetPadTickX(1);
	gStyle->SetPadTickY(1);
	gStyle->SetOptStat(0);
		if (iFile==0)	h1d3->Draw();
		else	h1d3->Draw("SAME");
	}
	c1->cd();
	l1->Draw("SAME");
	c2->cd();
	l2->Draw("SAME");
	c3->cd();
	l3->Draw("SAME");
}
