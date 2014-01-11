{
	double Br_RPC = 0.02;
	double P_cap = 0.61;
	double Br_sig = 3e-15;

	double P_RPC2sig = 8E-5;
	double A_geo = 0.3;

	double Factor = Br_RPC*P_RPC2sig/P_cap/Br_sig/A_geo;

	double nm_min = 0.45;
	double nm_max = 0.6;

	TFile *_file0 = TFile::Open("Optimize.1170s100.root");
	TTree *t = (TTree*) _file0->Get("t");
	std::vector<double> v1x;
	std::vector<double> v1y;
	std::vector<double> v2x;
	std::vector<double> v2y;
	std::vector<double> v3x;
	std::vector<double> v3y;
	std::vector<double> vv1x;
	std::vector<double> vv1y;
	std::vector<double> vv2x;
	std::vector<double> vv2y;
	std::vector<double> vv3x;
	std::vector<double> vv3y;
	double right;
	double left;
	double nPion;
	double nMuon;
	double rMuon;
	double ratio;
	t->SetBranchAddress("right",&right);
	t->SetBranchAddress("left",&left);
	t->SetBranchAddress("nPion",&nPion);
	t->SetBranchAddress("nMuon",&nMuon);
	t->SetBranchAddress("AccTime",&rMuon);
	int n = t->GetEntries();
	double bkg_max = 0;
	for (int i = 0; i< n; i++){
		t->GetEntry(i);
		ratio=nPion*Factor/nMuon;
		if (ratio>bkg_max) bkg_max=ratio;
	}
	for (int i = 0; i< n; i++){
		t->GetEntry(i);
		ratio=nPion*Factor/nMuon;
		v1x.push_back(left);
		v1y.push_back(ratio);
		vv1x.push_back(left);
		vv1y.push_back((rMuon-nm_min)*bkg_max/(nm_max-nm_min));
	}
	TGraph *g1 = new TGraph(v1x.size(),&v1x[0],&v1y[0]);
	//	TGraph *g2 = new TGraph(v2x.size(),&v2x[0],&v2y[0]);
	//	TGraph *g3 = new TGraph(v3x.size(),&v3x[0],&v3y[0]);
	TGraph *gg1 = new TGraph(vv1x.size(),&vv1x[0],&vv1y[0]);
	//	TGraph *gg2 = new TGraph(vv2x.size(),&vv2x[0],&vv2y[0]);
	//	TGraph *gg3 = new TGraph(vv3x.size(),&vv3x[0],&vv3y[0]);
	g1->SetTitle("Time Window Optimization (t_{Sep}=1.170 us, t_{right} = 1.150 us)");
	TAxis *axisx = g1->GetXaxis();
	axisx->SetTitle("t_{left} (ns)");
	TAxis *axisy = g1->GetYaxis();
	axisy->SetTitle("Beam Delayed RPC Background per Signal Event");

	//   h1->SetMarkerStyle(20);
	//   h2->SetMarkerStyle(20);
	//   h3->SetMarkerStyle(20);
	//   h1->SetMarkerColor(1);
	//   h2->SetMarkerColor(600);
	//   h3->SetMarkerColor(632);
	//   hh1->SetMarkerStyle(4);
	//   hh2->SetMarkerStyle(4);
	//   hh3->SetMarkerStyle(4);
	//   hh1->SetMarkerColor(1);
	//   hh2->SetMarkerColor(600);
	//   hh3->SetMarkerColor(632);

	g1->SetMarkerStyle(20);
	//   g2->SetMarkerStyle(20);
	//   g3->SetMarkerStyle(20);
	g1->SetMarkerColor(1);
	//   g2->SetMarkerColor(600);
	//   g3->SetMarkerColor(632);
	g1->SetLineColor(1);
	//  g2->SetLineColor(600);
	//  g3->SetLineColor(632);
	gg1->SetMarkerStyle(4);
	// gg2->SetMarkerStyle(4);
	// gg3->SetMarkerStyle(4);
	gg1->SetMarkerColor(632);
	// gg2->SetMarkerColor(600);
	// gg3->SetMarkerColor(632);
	gg1->SetLineColor(632);
	// gg2->SetLineColor(600);
	// gg3->SetLineColor(632);

	gStyle->SetOptStat(0);
	gStyle->SetPadTickX(1);

	TCanvas *c1 = new TCanvas("c1");
	TPad *pad1 = new TPad("pad1","",0,0,1,1);
	TPad *pad2 = new TPad("pad2","",0,0,1,1);
	pad2->SetFillStyle(4000); //will be transparent
	Double_t ymin = 0;
	Double_t ymax = 0.04;
	Double_t xmin = 450;
	Double_t xmax = 650;
	Double_t dy = (ymax-ymin)/0.8; //10 per cent margins top and bottom
	Double_t dx = (xmax-xmin)/0.8; //10 per cent margins left and right
	pad1->Range(xmin-0.1*dx,ymin-0.1*dy,xmax+0.1*dx,ymax+0.1*dy);
	pad1->Draw();
	pad1->cd();
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	g1->GetHistogram()->SetMaximum(bkg_max);
	g1->GetHistogram()->SetMinimum(0);
	g1->Draw("LAP");
	gg1->GetHistogram()->SetMaximum(nm_max);
	gg1->GetHistogram()->SetMinimum(nm_min);
	gg1->Draw("LPSAME");
	TLegend *legend1 = new TLegend(0.7,0.7,0.9,0.9);
	legend1->AddEntry(g1,"RPC Background","LP");
	legend1->AddEntry(gg1,"Time Acceptance","LP");
	legend1->Draw("same");

	//   c1->cd();
//	ymin = 0.45;
//	ymax = 0.65;
	Double_t xmin = 450;
	Double_t xmax = 650;
	Double_t dy = (nm_max-nm_min)/0.8; //10 per cent margins top and bottom
	Double_t dx = (xmax-xmin)/0.8; //10 per cent margins left and right
	pad2->Range(xmin-0.1*dx,nm_min-0.1*dy,xmax+0.1*dx,nm_max+0.1*dy);
	pad2->Draw();
	pad2->cd();

	TGaxis *axis = new TGaxis(650,nm_min,650,nm_max,nm_min,nm_max,50510,"+L");
	axis->SetLabelColor(kRed);
	axis->SetTitle("#mu^{-} stopped per proton");
	axis->Draw();

}
