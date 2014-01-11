//	kWhite =0,   kBlack =1,   kGray=920,
//	kRed   =632, kGreen =416, kBlue=600, kYellow=400, kMagenta=616, kCyan=432,
//	kOrange=800, kSpring=820, kTeal=840, kAzure =860, kViolet =880, kPink=900
//##########################################################################################################################

{
	std::vector<double> sf_Cal;
	std::vector<double> e_Cal;
	std::vector<double> esig_Cal;
	std::vector<double> r_Cal;
	std::vector<double> rsig_Cal;
	std::vector<double> ratio_CalU1;
	std::vector<double> ratio_CalU3;
	std::vector<double> ratio_CalG3;

	std::vector<double> sf_Exp;
	std::vector<double> e_Exp;
	std::vector<double> esig_Exp;
	std::vector<double> r_Exp;
	std::vector<double> rsig_Exp;
	std::vector<double> sfC_Exp;

	double sf;
	double e;
	double esig;
	double r;
	double rsig;
	double ratioU1;
	double ratioU3;
	double ratioG3;
	double sfC;

	TFile *_file0 = TFile::Open("points_Cal_50umAl.root");
	TTree *t = (TTree*) _file0->Get("t");
	t->SetBranchAddress("sf",&sf);
	t->SetBranchAddress("e",&e);
	t->SetBranchAddress("esig",&esig);
	t->SetBranchAddress("r",&r);
	t->SetBranchAddress("rsig",&rsig);
	t->SetBranchAddress("ratioU1",&ratioU1);
	t->SetBranchAddress("ratioU3",&ratioU3);
	t->SetBranchAddress("ratioG3",&ratioG3);
	int n = t->GetEntries();
	for (int i = 0; i< n; i++){
		t->GetEntry(i);
		sf_Cal.push_back(sf);
		e_Cal.push_back(e);
		esig_Cal.push_back(esig);
		r_Cal.push_back(r);
		rsig_Cal.push_back(rsig);
		ratio_CalU1.push_back(ratioU1);
		ratio_CalU3.push_back(ratioU3);
		ratio_CalG3.push_back(ratioG3);
	}

	_file0 = TFile::Open("points_Exp.root");
	t = (TTree*) _file0->Get("t");
	t->SetBranchAddress("sf",&sf);
	t->SetBranchAddress("e",&e);
	t->SetBranchAddress("esig",&esig);
//	t->SetBranchAddress("r",&r);
//	t->SetBranchAddress("rsig",&rsig);
	t->SetBranchAddress("sfC",&sfC);
	int n = t->GetEntries();
	for (int i = 0; i< n; i++){
		t->GetEntry(i);
		sf_Exp.push_back(sf);
		e_Exp.push_back(e);
		esig_Exp.push_back(esig);
		r_Exp.push_back(42.22*exp(1.795*log(e)));
		rsig_Exp.push_back(esig/e*r*1.795);
		sfC_Exp.push_back(sfC);
	}

	TAxis *axisx = 0;
	TAxis *axisy = 0;

	TGraph *ge_Cal = new TGraph(e_Cal.size(),&sf_Cal[0],&e_Cal[0]);
	ge_Cal->SetTitle("Energy Deposition");
	axisx = ge_Cal->GetXaxis();
	axisx->SetTitle("Scaling Factor (w.r.t 28 MeV/c)");
	axisy = ge_Cal->GetYaxis();
	axisy->SetTitle("Energy Deposition (MeV)");

	TGraph *gesig_Cal = new TGraph(esig_Cal.size(),&sf_Cal[0],&esig_Cal[0]);
	gesig_Cal->SetTitle("Sigma Energy Deposition");
	axisx = gesig_Cal->GetXaxis();
	axisx->SetTitle("Scaling Factor (w.r.t 28 MeV/c)");
	axisy = gesig_Cal->GetYaxis();
	axisy->SetTitle("Sigma Energy Deposition (MeV)");

	TGraph *gr_Cal = new TGraph(r_Cal.size(),&sf_Cal[0],&r_Cal[0]);
	gr_Cal->SetTitle("Depth of Stopping Position");
	axisx = gr_Cal->GetXaxis();
	axisx->SetTitle("Scaling Factor (w.r.t 28 MeV/c)");
	axisy = gr_Cal->GetYaxis();
	axisy->SetTitle("Depth of Stopping Position (um)");

	TGraph *grsig_Cal = new TGraph(rsig_Cal.size(),&sf_Cal[0],&rsig_Cal[0]);
	grsig_Cal->SetTitle("Sigma of Depth of Stopping Position");
	axisx = grsig_Cal->GetXaxis();
	axisx->SetTitle("Scaling Factor (w.r.t 28 MeV/c)");
	axisy = grsig_Cal->GetYaxis();
	axisy->SetTitle("Sigma of Depth of Stopping Position (um)");

	TGraph *gratio_CalU1 = new TGraph(ratio_CalU1.size(),&sf_Cal[0],&ratio_CalU1[0]);
	gratio_CalU1->SetTitle("Stopping Ratio");
	axisx = gratio_CalU1->GetXaxis();
	axisx->SetTitle("Scaling Factor (w.r.t 28 MeV/c)");
	axisy = gratio_CalU1->GetYaxis();
	axisy->SetTitle("Stopping Ratio");

	TGraph *gratio_CalU3 = new TGraph(ratio_CalU3.size(),&sf_Cal[0],&ratio_CalU3[0]);
	gratio_CalU3->SetTitle("Stopping Ratio");
	axisx = gratio_CalU3->GetXaxis();
	axisx->SetTitle("Scaling Factor (w.r.t 28 MeV/c)");
	axisy = gratio_CalU3->GetYaxis();
	axisy->SetTitle("Stopping Ratio");

	TGraph *gratio_CalG3 = new TGraph(ratio_CalG3.size(),&sf_Cal[0],&ratio_CalG3[0]);
	gratio_CalG3->SetTitle("Stopping Ratio");
	axisx = gratio_CalG3->GetXaxis();
	axisx->SetTitle("Scaling Factor (w.r.t 28 MeV/c)");
	axisy = gratio_CalG3->GetYaxis();
	axisy->SetTitle("Stopping Ratio");

	TGraph *ge_Exp = new TGraph(e_Exp.size(),&sf_Exp[0],&e_Exp[0]);
	TGraph *gesig_Exp = new TGraph(esig_Exp.size(),&sf_Exp[0],&esig_Exp[0]);
	TGraph *gr_Exp = new TGraph(r_Exp.size(),&sf_Exp[0],&r_Exp[0]);
	TGraph *grsig_Exp = new TGraph(rsig_Exp.size(),&sf_Exp[0],&rsig_Exp[0]);
	TGraph *gsfC_Exp = new TGraph(sfC_Exp.size(),&sf_Exp[0],&sfC_Exp[0]);

	ge_Cal->SetMarkerStyle(20);
	ge_Cal->SetMarkerColor(600);
	ge_Cal->SetLineColor(600);
	gesig_Cal->SetMarkerStyle(20);
	gesig_Cal->SetMarkerColor(600);
	gesig_Cal->SetLineColor(600);
	gr_Cal->SetMarkerStyle(20);
	gr_Cal->SetMarkerColor(600);
	gr_Cal->SetLineColor(600);
	grsig_Cal->SetMarkerStyle(20);
	grsig_Cal->SetMarkerColor(600);
	grsig_Cal->SetLineColor(600);
	gratio_CalU1->SetMarkerStyle(20);
	gratio_CalU3->SetMarkerStyle(20);
	gratio_CalG3->SetMarkerStyle(20);
	gratio_CalU1->SetMarkerColor(600);
	gratio_CalU3->SetMarkerColor(600);
	gratio_CalG3->SetMarkerColor(600);
	gratio_CalU1->SetLineColor(600);
	gratio_CalU3->SetLineColor(600);
	gratio_CalG3->SetLineColor(600);

	ge_Exp->SetMarkerStyle(20);
	ge_Exp->SetMarkerColor(632);
	ge_Exp->SetLineColor(632);
	gesig_Exp->SetMarkerStyle(20);
	gesig_Exp->SetMarkerColor(632);
	gesig_Exp->SetLineColor(632);
	gr_Exp->SetMarkerStyle(20);
	gr_Exp->SetMarkerColor(632);
	gr_Exp->SetLineColor(632);
	grsig_Exp->SetMarkerStyle(20);
	grsig_Exp->SetMarkerColor(632);
	grsig_Exp->SetLineColor(632);

	TCanvas *c1 = new TCanvas("c1","c1",1024,768);
	c1->cd();
	TPad *pad1 = new TPad("pad1","",0,0.5,0.5,1);
	TPad *pad2 = new TPad("pad2","",0.5,0.5,1,1);
	TPad *pad3 = new TPad("pad3","",0,0,0.5,0.5);
	TPad *pad4 = new TPad("pad4","",0.5,0,1,0.5);
	pad1->Draw();
	pad2->Draw();
	pad3->Draw();
	pad4->Draw();

	TLegend *legend1 = 0;

	gStyle->SetOptStat(0);
	pad1->SetGridx(1);
	pad1->SetGridy(1);
	pad2->SetGridx(1);
	pad2->SetGridy(1);
	pad3->SetGridx(1);
	pad3->SetGridy(1);
	pad4->SetGridx(1);
	pad4->SetGridy(1);

	pad1->cd();
	ge_Cal->Draw("LAP");
	ge_Exp->Draw("LPSAME");
	legend1 = new TLegend(0.1,0.7,0.5,0.9);
	legend1->AddEntry(ge_Cal,"MC Simulation","LP");
	legend1->AddEntry(ge_Exp,"Experiment","LP");
	legend1->Draw("same");

	pad2->cd();
	gr_Cal->Draw("LAP");
	gr_Exp->Draw("LPSAME");
//	legend1 = new TLegend(0.1,0.7,0.5,0.9);
//	legend1->AddEntry(gr_Cal,"MC Simulation","LP");
//	legend1->AddEntry(gr_Exp,"Experiment","LP");
//	legend1->Draw("same");

	pad3->cd();
	gesig_Cal->Draw("LAP");
	gesig_Exp->Draw("LPSAME");
//	legend1 = new TLegend(0.1,0.7,0.5,0.9);
//	legend1->AddEntry(gesig_Cal,"MC Simulation","LP");
//	legend1->AddEntry(gesig_Exp,"Experiment","LP");
//	legend1->Draw("same");

	pad4->cd();
	grsig_Cal->Draw("LAP");
	grsig_Exp->Draw("LPSAME");
//	legend1 = new TLegend(0.1,0.7,0.5,0.9);
//	legend1->AddEntry(grsig_Cal,"MC Simulation","LP");
//	legend1->AddEntry(grsig_Exp,"Experiment","LP");
//	legend1->Draw("same");

	TCanvas *c2 = new TCanvas("c2","c2",1024,768);
	c2->cd();
	gratio_CalU1->SetLineColor(kRed);
	gratio_CalU1->SetMarkerColor(kRed);
	gratio_CalU3->SetLineColor(kBlue);
	gratio_CalU3->SetMarkerColor(kBlue);
	gratio_CalG3->SetLineColor(kGreen);
	gratio_CalG3->SetMarkerColor(kGreen);
	gratio_CalU1->Draw("LAP");
	gratio_CalU3->Draw("LPSAME");
	gratio_CalG3->Draw("LPSAME");
	legend1 = new TLegend(0.1,0.7,0.5,0.9);
	legend1->AddEntry(gratio_CalU1,"Uniform, FWHM = 1%","LP");
	legend1->AddEntry(gratio_CalU3,"Uniform, FWHM = 3%","LP");
	legend1->AddEntry(gratio_CalG3,"Gaussian, FWHM = 3%","LP");
	legend1->Draw("same");

	TCanvas *c3 = new TCanvas("c3","c3",1024,768);
	c3->cd();
	gsfC_Exp->Draw("LAP");
}
