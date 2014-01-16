{
	double emass = 0.510999;
	TChain *c = new TChain("t");
	c->Add("dedx2.root");
	double ekin;
	double dedx;
	c->SetBranchAddress("E",&ekin);
	c->SetBranchAddress("dedx",&dedx);
	std::vector<double> vmom;
	std::vector<double> vdedx;
	int n = c->GetEntries();
	std::cout<<"n = "<<n<<std::endl;
	for (int i = 0; i<n; i++){
		c->GetEntry(i);
		//if (ekin>115) continue;
		double e = ekin + emass;
		double mom = sqrt(e*e-emass*emass);
		std::cout<<"E = "<<ekin<<", mom = "<<mom<<", dedx = "<<dedx<<std::endl;
		//vmom.push_back(mom);
		vmom.push_back(ekin);
		vdedx.push_back(dedx);
	}
	ROOT::Math::Interpolator interpolator(vmom,vdedx);
	double dedxS = interpolator.Eval(105);
	std::cout<<"dedxS = "<<dedxS<<std::endl;
	TGraph *g = new TGraph(vdedx.size(),&vmom[0],&vdedx[0]);
	g->SetTitle("dE/dx for e^{-} in Chamber Gas");
	g->GetYaxis()->SetTitle("dE/dx (MeV/cm)");
	g->GetXaxis()->SetTitle("E_{kin} (MeV)");
	//g->GetXaxis()->SetTitle("p_{e^{-}} (MeV/c)");
//	g->GetHistogram()->SetMinimum(5e-4);
//	g->GetHistogram()->SetMaximum(1e-3);
	g->SetMarkerStyle(20);
	TCanvas *c1 = new TCanvas("c1");
	TPad *pad1 = new TPad("pad1","",0,0,1,1);
	pad1->Draw();
	pad1->cd();
	gStyle->SetPadTickX(1);
	gStyle->SetPadTickY(1);
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	gPad->SetLogx(1);
	gPad->SetLogy(1);
	g->Draw("AP");
}
