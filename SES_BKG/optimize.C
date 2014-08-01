{
	double bkg_min = 0.018;
	double bkg_max = 0.1;
	double ses_min = 5e-16;
	double ses_max = 3.2e-15;
	Double_t xmin = 80;
	Double_t xmax = 380;

	std::vector<double> v1x;
	std::vector<double> v1y;
	std::vector<double> vv1x;
	std::vector<double> vv1y;

	double days = 90;
	for (int i=0; (days=90+i*10)<=360; i++){
		double SES = 1/((days*3600*24)*2.5e12*6.35e-4)/0.61/0.043;
		v1x.push_back(days);
		v1y.push_back(SES);
		double BKG = 0.019*days/90;
		BKG = ((BKG-bkg_min)/(bkg_max-bkg_min)*(ses_max-ses_min)+ses_min);
		vv1x.push_back(days);
		vv1y.push_back(BKG);
	}

	TGraph *g1 = new TGraph(v1x.size(),&v1x[0],&v1y[0]);
	TGraph *gg1 = new TGraph(vv1x.size(),&vv1x[0],&vv1y[0]);

	g1->SetTitle("SES VS Background Contamination");
	TAxis *axisx = g1->GetXaxis();
	axisx->SetTitle("Running Period (Days)");
	TAxis *axisy = g1->GetYaxis();
	axisy->SetTitle("SES");

	g1->SetMarkerStyle(20);
	g1->SetMarkerColor(632);
	g1->SetLineColor(632);
	gg1->SetMarkerStyle(20);
	gg1->SetMarkerColor(1);
	gg1->SetLineColor(1);

	gStyle->SetOptStat(0);
	gStyle->SetPadTickX(1);

	TCanvas *c1 = new TCanvas("c1");
	TPad *pad1 = new TPad("pad1","",0,0,1,1);
	TPad *pad2 = new TPad("pad2","",0,0,1,1);
	pad2->SetFillStyle(4000); //will be transparent
	Double_t dy = (ses_max-ses_min)/0.8; //10 per cent margins top and bottom
	Double_t dx = (xmax-xmin)/0.8; //10 per cent margins left and right
	pad1->Range(xmin-0.1*dx,ses_min-0.1*dy,xmax+0.1*dx,ses_max+0.1*dy);
	pad1->Draw();
	pad1->cd();
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	g1->GetHistogram()->SetMaximum(ses_max);
	g1->GetHistogram()->SetMinimum(ses_min);
	g1->Draw("LAP");
	gg1->GetHistogram()->SetMaximum(bkg_max);
	gg1->GetHistogram()->SetMinimum(bkg_min);
	gg1->Draw("LPSAME");
	TLegend *legend1 = new TLegend(0.7,0.7,0.9,0.9);
	legend1->AddEntry(g1,"SES","LP");
	legend1->AddEntry(gg1,"BKG","LP");
	legend1->Draw("same");

	//   c1->cd();
//	ses_min = 0.45;
//	ses_max = 0.65;
	Double_t dy = (bkg_max-bkg_min)/0.8; //10 per cent margins top and bottom
	Double_t dx = (xmax-xmin)/0.8; //10 per cent margins left and right
	pad2->Range(xmin-0.1*dx,bkg_min-0.1*dy,xmax+0.1*dx,bkg_max+0.1*dy);
	pad2->Draw();
	pad2->cd();
	TGaxis *axis = new TGaxis(xmax,bkg_min,xmax,bkg_max,bkg_min,bkg_max,50510,"+L");
	axis->SetLabelColor(kBlack);
	axis->SetTitle("Background Contamination");
	axis->Draw();

}
