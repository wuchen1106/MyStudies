{
	TChain * chain = new TChain("t");
	chain->Add("optimize.root");

	double BLK_rem  = 832;
	double BLK_rep  = 380;
	double BLK_rmup = 40.6;
	double BLK_rmum = 49.1;
	double BLK_rgam = 1737;
	double BLK_rpim = 2.2;

	double o140331K_rem  = 693;
	double o140331K_rep  = 2233;
	double o140331K_rmup = 45.5;
	double o140331K_rmum = 92.3;
	double o140331K_rgam = 4454;
	double o140331K_rpim = BLK_rpim;

	double c_rem  = 693;
	double c_rep  = 2233;
	double c_rmup = 45.5;
	double c_rmum = 92.3;
	double c_rgam = 4454;
	double c_rpim = 399;

	double rem  = o140331K_rem ;
	double rep  = o140331K_rep ;
	double rmup = BLK_rmup;
	double rmum = BLK_rmum;
	double rgam = BLK_rgam;
	double rpim = BLK_rpim;

	int opt;
	double em;
	double ep;
	double mup;
	double mum;
	double gam;
	double pim;
	double tot;
	chain->SetBranchAddress("em",&em);
	chain->SetBranchAddress("ep",&ep);
	chain->SetBranchAddress("mum",&mum);
	chain->SetBranchAddress("mup",&mup);
	chain->SetBranchAddress("gam",&gam);
	chain->SetBranchAddress("pim",&pim);
	chain->SetBranchAddress("tot",&tot);
	chain->SetBranchAddress("opt",&opt);
	std::vector<double> v_em;
	std::vector<double> v_ep;
	std::vector<double> v_mup;
	std::vector<double> v_mum;
	std::vector<double> v_gam;
	std::vector<double> v_pim;
	std::vector<double> v_tot;
	std::vector<double> v_opt;

	Long64_t nEvents = chain->GetEntries();
	for (Long64_t iEvent = 0; iEvent<nEvents; iEvent++){
		chain->GetEntry(iEvent);
		if (opt==1){
			rem  = BLK_rem ;
			rep  = BLK_rep ;
			rmup = BLK_rmup;
			rmum = BLK_rmum;
			rgam = BLK_rgam;
			rpim = BLK_rpim;
		}
		else if (opt == 3){
			rem  = o140331K_rem ;
			rep  = o140331K_rep ;
			rmup = o140331K_rmup;
			rmum = o140331K_rmum;
			rgam = BLK_rgam;
			rpim = BLK_rpim;
		}
		else if (opt == 7){
			rem  = c_rem ;
			rep  = c_rep ;
			rmup = c_rmup;
			rmum = c_rmum;
			rgam = c_rgam;
			rpim = c_rpim;
		}
		else{
			rem  = o140331K_rem ;
			rep  = o140331K_rep ;
			rmup = BLK_rmup;
			rmum = BLK_rmum;
			rgam = BLK_rgam;
			rpim = BLK_rpim;
		}
		v_em.push_back(em/rem);
		v_ep.push_back(ep/rep);
		v_mum.push_back(mum/rmum);
		v_mup.push_back(mup/rmup);
		v_gam.push_back(gam/rgam);
		v_pim.push_back(pim/rpim);
//		v_tot.push_back(tot);
		v_tot.push_back(em/rem+ep/rep+mum/rmum+mup/rmup+gam/rgam+pim/rpim);
		v_opt.push_back(opt);
		std::cout<<"opt = "<<opt<<std::endl;
		std::cout<<"	r_em = "<<em<<"/"<<rem<<" = "<<em/rem<<std::endl;
		std::cout<<"	r_ep = "<<ep<<"/"<<rep<<" = "<<ep/rep<<std::endl;
		std::cout<<"	r_mum = "<<mum<<"/"<<rmum<<" = "<<mum/rmum<<std::endl;
		std::cout<<"	r_mup = "<<mup<<"/"<<rmup<<" = "<<mup/rmup<<std::endl;
		std::cout<<"	r_gam = "<<gam<<"/"<<rgam<<" = "<<gam/rgam<<std::endl;
		std::cout<<"	r_pim = "<<pim<<"/"<<rpim<<" = "<<pim/rpim<<std::endl;
//		v_tot.push_back(tot);
		std::cout<<"	N_tot = "<<tot<<", r_tot = "<<em/rem+ep/rep+mum/rmum+mup/rmup+gam/rgam+pim/rpim<<std::endl;
	}

	TGraph * g_em = new TGraph(v_opt.size(),&v_opt[0],&v_em[0]);
	TGraph * g_ep = new TGraph(v_opt.size(),&v_opt[0],&v_ep[0]);
	TGraph * g_mup = new TGraph(v_opt.size(),&v_opt[0],&v_mum[0]);
	TGraph * g_mum = new TGraph(v_opt.size(),&v_opt[0],&v_mup[0]);
	TGraph * g_gam = new TGraph(v_opt.size(),&v_opt[0],&v_gam[0]);
	TGraph * g_pim = new TGraph(v_opt.size(),&v_opt[0],&v_pim[0]);
	TGraph * g_tot = new TGraph(v_opt.size(),&v_opt[0],&v_tot[0]);

	g_tot->SetTitle("Number of #gammas Hitting CDC per 10^{6} Initial Proton");
	TAxis * axisx = g_tot->GetXaxis();
	axisx->SetTitle("Option Index");
	TAxis * axisy = g_tot->GetYaxis();
	axisy->SetTitle("Number of #gammas Hitting CDC per 10^{6} Initial Proton");
	g_tot->GetHistogram()->SetMinimum(0);

	g_em->SetMarkerStyle(20);
	g_ep->SetMarkerStyle(20);
	g_mum->SetMarkerStyle(20);
	g_mup->SetMarkerStyle(20);
	g_gam->SetMarkerStyle(20);
	g_pim->SetMarkerStyle(20);
	g_tot->SetMarkerStyle(20);
	g_em->SetMarkerColor(600);
	g_ep->SetMarkerColor(632);
	g_mum->SetMarkerColor(416);
	g_mup->SetMarkerColor(880);
	g_gam->SetMarkerColor(432);
	g_pim->SetMarkerColor(800);
	g_tot->SetMarkerColor(1);
	g_em->SetLineColor(600);
	g_ep->SetLineColor(632);
	g_mum->SetLineColor(416);
	g_mup->SetLineColor(880);
	g_gam->SetLineColor(432);
	g_pim->SetLineColor(800);
	g_tot->SetLineColor(1);

	TCanvas * c1 = new TCanvas();
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	g_tot->Draw("LAP");
	g_em->Draw("LPSAME");
	g_ep->Draw("LPSAME");
	g_mum->Draw("LPSAME");
	g_mup->Draw("LPSAME");
	g_gam->Draw("LPSAME");
	g_pim->Draw("LPSAME");

	TLegend * legend = new TLegend(0.1,0.7,0.5,0.9);
	legend->AddEntry(g_tot,"In Total","LP");
	legend->AddEntry(g_em,"From Beam e^{-}","LP");
	legend->AddEntry(g_ep,"From Beam e^{+}","LP");
	legend->AddEntry(g_mum,"From Beam #mu^{-}","LP");
	legend->AddEntry(g_mup,"From Beam #mu^{+}","LP");
	legend->AddEntry(g_gam,"From Beam #gamma","LP");
	legend->AddEntry(g_pim,"From Beam #pi^{-}","LP");
	legend->Draw("same");

}
