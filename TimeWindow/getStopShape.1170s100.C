//	kWhite =0,   kBlack =1,   kGray=920,
//	kRed   =632, kGreen =416, kBlue=600, kYellow=400, kMagenta=616, kCyan=432,
//	kOrange=800, kSpring=820, kTeal=840, kAzure =860, kViolet =880, kPink=900
//##########################################################################################################################
{
	std::stringstream buff;

	double minimum = 1e-21;
	TString par = "pi";
	double NProton = 47052886/5437151*1e9;
//	double minimum = 1e-11;
//	TString par = "mu";
//	double NProton = 7418778/860498*1e8;

	double time_right = 1150;
	double time_left = 490;

	TFile *f2 = new TFile("../CollAndStop/"+par+"on.11.p5.output.root");
	TH1D *hPion = (TH1D*) f2->Get("h1");
	TH1D *hPionIni = (TH1D*) f2->Get("h0");
	hPion->Scale(1/sqrt(hPion->Integral()));
	hPionIni->Scale(1/sqrt(hPionIni->Integral()));
	hPion->Scale(1/NProton);
	hPionIni->Scale(1/NProton);
	double tSep = 1170;

	double weight;
	double time;

	height1 = 4e-4;
	height2 = 4e-4;

	int Color_1 = 1;
	int Color_2 = 600;
	int Color_3 = 632;
	
	TString title1 = "#"+par+"^{-} Stop Time (Before smear)";
	TString xtitle1 = "Time (ns)";
	TString ytitle1 = "#"+par+"^{-} Stopped / P^{+} / 5.849 ns";
	
//	TString title2 = "#"+par+"^{-} Stopped in Time Window (After Smear, Width = 100 ns)";
	TString title2 = "#"+par+"^{-} Decayed/Captured in Time Window (After Smear)";
	TString xtitle2 = "Left End of Time Window (ns)";
	TString ytitle2 = "#"+par+"^{-} Stopped / P^{+} in Time Window";
	
	TString title3 = "#"+par+"^{-} Decay/Capture Time (After smear)";
	TString xtitle3 = "Time (ns)";
	TString ytitle3 = "#"+par+"^{-} Stopped / P^{+} / 5.849 ns";

	TH1D *h1_1 = hPionIni;
	TH1D *h2_1 = hPion->Clone("integral");
	TH1D *h3_1 = hPion;
	int nBins = h2_1->GetNbinsX();
	int BinMax = h2_1->FindBin(time_right);
	for (int iBin  = 0; iBin<h2_1->GetNbinsX(); iBin++){
		h2_1->SetBinContent(iBin, hPion->Integral(iBin,BinMax));
	}


	gStyle->SetOptStat(0);
	gStyle->SetPadTickX(1);
	gStyle->SetPadTickY(1);
	TCanvas *c = new TCanvas("c");
	TPad *apad = new TPad("p1","p1",0,0,1/3.,1);
	TPad *bpad = new TPad("p2","p2",2/3.,0,1,1);
	TPad *cpad = new TPad("p2","p2",1/3.,0,2/3.,1);
	apad->Draw();
	bpad->Draw();
	cpad->Draw();

	apad->cd();
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	gPad->SetLogy(1);
	if (h1_1) h1_1->GetYaxis()->SetTitle(ytitle1);
	if (h1_1) h1_1->GetYaxis()->SetTitleOffset(1.5);
	if (h1_1) h1_1->GetXaxis()->SetTitle(xtitle1);
	if (h1_1) h1_1->GetYaxis()->SetRangeUser(minimum,h1_1->GetMaximum()*2);
	if (h1_1) h1_1->SetTitle(title1);
	if (h1_1) h1_1->SetMarkerColor(Color_1);
	if (h1_1) h1_1->SetLineColor(Color_1);
	if (h1_1) h1_1->Draw();

	bpad->cd();
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	gPad->SetLogy(1);
	if (h2_1) h2_1->GetYaxis()->SetTitle(ytitle2);
	if (h2_1) h2_1->GetYaxis()->SetTitleOffset(1.5);
	if (h2_1) h2_1->GetXaxis()->SetTitle(xtitle2);
	if (h2_1) h2_1->GetYaxis()->SetRangeUser(minimum,h2_1->GetMaximum()*2);
	if (h2_1) h2_1->SetTitle(title2);
	if (h2_1) h2_1->SetMarkerColor(Color_1);
	if (h2_1) h2_1->SetLineColor(Color_1);
	if (h2_1) h2_1->Draw();
	TPaveText *sum = new TPaveText(0.3,0.7,0.9,0.9,"brNDC");
	sum->AddText("Info");
	buff.str("");
	buff.clear();
	buff<<"Seperation of Proton Bunches: "<<tSep<<" ns."<<std::endl;
	sum->AddText(buff.str().c_str());
	buff.str("");
	buff.clear();
	buff<<"Proton Pulse Shape: 100 ns squre wave"<<std::endl;
	sum->AddText(buff.str().c_str());
	buff.str("");
	buff.clear();
	buff<<"Right End of Time Window: "<<time_right<<" ns."<<std::endl;
	sum->AddText(buff.str().c_str());
	sum->Draw();

	cpad->cd();
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	gPad->SetLogy(1);
	if (h3_1) h3_1->GetYaxis()->SetTitle(ytitle3);
	if (h3_1) h3_1->GetYaxis()->SetTitleOffset(1.5);
	if (h3_1) h3_1->GetXaxis()->SetTitle(xtitle3);
	if (h3_1) h3_1->GetYaxis()->SetRangeUser(minimum,h3_1->GetMaximum()*2);
	if (h3_1) h3_1->SetTitle(title3);
	if (h3_1) h3_1->SetMarkerColor(Color_1);
	if (h3_1) h3_1->SetLineColor(Color_1);
	if (h3_1) h3_1->Draw();

	std::cout<<"N = "<<h2_1->GetBinContent(h2_1->FindBin(time_left))<<" , Acc = "<<h2_1->GetBinContent(h2_1->FindBin(time_left))/(h1_1->Integral())<<std::endl;
}
