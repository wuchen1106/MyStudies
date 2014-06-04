{
	TChain * chain = new TChain("t");
	chain->Add("optimize.root");

	int opt;
	double ot;
	double pim;
	chain->SetBranchAddress("ot",&ot);
	chain->SetBranchAddress("pim",&pim);
	chain->SetBranchAddress("opt",&opt);
	std::vector<double> v_ot;
	std::vector<double> v_pim;
	std::vector<double> v_opt;
	std::vector<double> v_tot;

	Long64_t nEvents = chain->GetEntries();
	for (Long64_t iEvent = 0; iEvent<nEvents; iEvent++){
		chain->GetEntry(iEvent);
		v_ot.push_back(ot);
		v_pim.push_back(pim);
		v_tot.push_back(ot+pim);
		v_opt.push_back(opt);
	}

	TGraph * g_ot = new TGraph(v_opt.size(),&v_opt[0],&v_ot[0]);
	TGraph * g_pim = new TGraph(v_opt.size(),&v_opt[0],&v_pim[0]);
	TGraph * g_tot = new TGraph(v_opt.size(),&v_opt[0],&v_tot[0]);

	g_tot->SetTitle("Occupancy @ Second Inner Most Layer");
	TAxis * axisx = g_tot->GetXaxis();
	axisx->SetTitle("Option Index");
	TAxis * axisy = g_tot->GetYaxis();
	axisy->SetTitle("Occupancy @ Second Inner Most Layer (%)");
	g_tot->GetHistogram()->SetMinimum(0);

	g_ot->SetMarkerStyle(20);
	g_pim->SetMarkerStyle(20);
	g_tot->SetMarkerStyle(20);
	g_ot->SetMarkerColor(600);
	g_pim->SetMarkerColor(800);
	g_tot->SetMarkerColor(1);
	g_ot->SetLineColor(600);
	g_pim->SetLineColor(800);
	g_tot->SetLineColor(1);

	TCanvas * c1 = new TCanvas();
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	g_tot->Draw("LAP");
	g_ot->Draw("LPSAME");
	g_pim->Draw("LPSAME");

	TLegend * legend = new TLegend(0.1,0.7,0.5,0.9);
	legend->AddEntry(g_tot,"In Total","LP");
	legend->AddEntry(g_ot,"From Other Beam Particles","LP");
	legend->AddEntry(g_pim,"From Beam #pi^{-}","LP");
	legend->Draw("same");

}
