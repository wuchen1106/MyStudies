{
	TChain * chain = new TChain("t");
	chain->Add("flux.root");

	double f;
	double opt = 0;
	char *run = new char[124];
	char *monitor = new char[124];
	chain->SetBranchAddress("run",run);
	chain->SetBranchAddress("monitor",monitor);
	chain->SetBranchAddress("f",&f);
	chain->SetBranchAddress("opt",&opt);
	std::vector<std::string> v_mon1;
	std::vector<double> v_opt1;
	std::vector<double> v_f1;
	std::vector<std::string> v_mon2;
	std::vector<double> v_opt2;
	std::vector<double> v_f2;

	Long64_t nEvents = chain->GetEntries();
	for (Long64_t iEvent = 0; iEvent<nEvents; iEvent++){
		chain->GetEntry(iEvent);
		TString srun;
		srun = run;
		if (srun=="140613"){
			v_mon1.push_back(monitor);
			v_f1.push_back(f);
			v_opt1.push_back(opt);
		}
		else if (srun=="140603G"){
			v_mon2.push_back(monitor);
			v_f2.push_back(f);
			v_opt2.push_back(opt);
		}
	}

	TGraph * g_1 = new TGraph(v_f1.size(),&v_opt1[0],&v_f1[0]);
	TGraph * g_2 = new TGraph(v_f2.size(),&v_opt2[0],&v_f2[0]);

	TCanvas * c1 = new TCanvas();
	c1->SetBottomMargin(0.25);

	g_1->SetTitle("Neutron Fluence (0.4 uA, 1/2.4 Duty Factor)");
	TH1F * h_1 = g_1->GetHistogram();
	TAxis * axisx = g_1->GetXaxis();
	axisx->SetTitle("");
	axisx->SetLabelSize(0.05);
	for (int i = 0; i<v_mon1.size(); i++){
		int ibin = h_1->FindBin(v_opt1[i]);
		axisx->SetBinLabel(ibin,v_mon1[i].c_str());
	}
	TAxis * axisy = g_1->GetYaxis();
	axisy->SetTitle("Fluence Hz/cm^{2}");
//	g_1->GetHistogram()->SetMinimum(1e3);
	g_1->GetYaxis()->SetRangeUser(1e3,1e8);

	g_1->SetMarkerStyle(20);
	g_2->SetMarkerStyle(20);
	g_1->SetMarkerColor(600);
	g_2->SetMarkerColor(800);
	g_1->SetLineColor(600);
	g_2->SetLineColor(800);

	gPad->SetGridx(1);
	gPad->SetGridy(1);
	gPad->SetLogy(1);

	g_1->Draw("LAP");
	g_2->Draw("LPSAME");

	TLegend * legend = new TLegend(0.5,0.7,0.9,0.9);
	legend->AddEntry(g_1,"PHITS 140613","LP");
	legend->AddEntry(g_2,"GEANT 140603","LP");
	legend->Draw("same");

}
