{
	TFile *f = 0;
	TLegend *legend = 0;
//	TString parName = "mu";
//	double minimum = 1e-11;
	TString parName = "pi";
	double minimum = 1e-21;

	TString par = "#"+parName+"^{-}";
	f = new TFile("result/ST.pion.150901.W100um.OptD2.DD35.TH200um.output.root");

	TH2D * h01  = (TH2D*) f->Get("h01");
	TH2D * h02  = (TH2D*) f->Get("h02");
	TH2D * h03  = (TH2D*) f->Get("h03");
	TH1D * h04  = (TH1D*) f->Get("h04");
	TH1D * h05  = (TH1D*) f->Get("h05");
	TH1D * h10  = (TH1D*) f->Get("h10");
	TH1D * h11  = (TH1D*) f->Get("h11");
	TH1D * h20  = (TH1D*) f->Get("h20");
	TH1D * h30  = (TH1D*) f->Get("h30");
	TH1D * h40  = (TH1D*) f->Get("h40");
	TH1D * h1_1 = (TH1D*) f->Get("h1_1");
	TH1D * h1_2 = (TH1D*) f->Get("h1_2");
	TH1D * h1_3 = (TH1D*) f->Get("h1_3");
	TH1D * h2_1 = (TH1D*) f->Get("h2_1");
	TH1D * h2_2 = (TH1D*) f->Get("h2_2");
	TH1D * h2_3 = (TH1D*) f->Get("h2_3");
	h01 ->GetXaxis()->SetTitleFont(22);
	h02 ->GetXaxis()->SetTitleFont(22);
	h03 ->GetXaxis()->SetTitleFont(22);
	h04 ->GetXaxis()->SetTitleFont(22);
	h05 ->GetXaxis()->SetTitleFont(22);
	h10 ->GetXaxis()->SetTitleFont(22);
	h11 ->GetXaxis()->SetTitleFont(22);
	h20 ->GetXaxis()->SetTitleFont(22);
	h30 ->GetXaxis()->SetTitleFont(22);
	h40 ->GetXaxis()->SetTitleFont(22);
	h1_1->GetXaxis()->SetTitleFont(22);
	h1_2->GetXaxis()->SetTitleFont(22);
	h1_3->GetXaxis()->SetTitleFont(22);
	h2_1->GetXaxis()->SetTitleFont(22);
	h2_2->GetXaxis()->SetTitleFont(22);
	h2_3->GetXaxis()->SetTitleFont(22);
	h01 ->GetYaxis()->SetTitleFont(22);
	h02 ->GetYaxis()->SetTitleFont(22);
	h03 ->GetYaxis()->SetTitleFont(22);
	h04 ->GetYaxis()->SetTitleFont(22);
	h05 ->GetYaxis()->SetTitleFont(22);
	h10 ->GetYaxis()->SetTitleFont(22);
	h11 ->GetYaxis()->SetTitleFont(22);
	h20 ->GetYaxis()->SetTitleFont(22);
	h30 ->GetYaxis()->SetTitleFont(22);
	h40 ->GetYaxis()->SetTitleFont(22);
	h1_1->GetYaxis()->SetTitleFont(22);
	h1_2->GetYaxis()->SetTitleFont(22);
	h1_3->GetYaxis()->SetTitleFont(22);
	h2_1->GetYaxis()->SetTitleFont(22);
	h2_2->GetYaxis()->SetTitleFont(22);
	h2_3->GetYaxis()->SetTitleFont(22);
	TPaveText *sum = (TPaveText*) f->Get("sum");
	TPaveText *info = (TPaveText*) f->Get("info");

//*************************************************************************************
	gStyle->SetPalette(1);
	gStyle->SetOptStat(0);
	gStyle->SetPadTickX(1);
	gStyle->SetPadTickY(1);

	// Draw pa VS y
	TCanvas *c1 = new TCanvas("c","c",1024,768);
	TPad * p1 = new TPad("p1","p1",0,0,1./3,1);
	TPad * p2 = new TPad("p2","p2",1./3,0,2./3,1);
	TPad * p3 = new TPad("p3","p3",2./3,0,3./3,1);
	p1->Draw();
	p2->Draw();
	p3->Draw();
	h01->SetContour(50);
	h02->SetContour(50);
	h03->SetContour(50);
	p1->cd();
	p1->SetGridx(1);
	p1->SetGridy(1);
	h01->Draw("CONT0 COLZ");
	p2->cd();
	p2->SetGridx(1);
	p2->SetGridy(1);
	h02->Draw("CONT0 COLZ");
	p3->cd();
	p3->SetGridx(1);
	p3->SetGridy(1);
	h03->Draw("CONT0 COLZ");
	sum->Draw();

	// Draw stop position

	TCanvas *c2 = new TCanvas("c2","c2",1024,768);
	TPad * p4 = new TPad("p4","p4",0,0,1./2,1);
	TPad * p5 = new TPad("p5","p5",1./2,0,2./2,1);
	p4->Draw();
	p5->Draw();
	p4->cd();
	p4->SetGridx(1);
	p4->SetGridy(1);
	h04->Draw();
	p5->cd();
	p5->SetGridx(1);
	p5->SetGridy(1);
	h05->Draw();

	// Draw pa y and t
	TCanvas *c3 = new TCanvas("c3","c3",1024,768);
	TPad *apad = new TPad("pa","pa",0,0,1./3,1);
	TPad *bpad = new TPad("pb","pb",1./3,0,2../3,1);
	TPad *cpad = new TPad("pc","pc",2./3,0,1,1);
	apad->Draw();
	bpad->Draw();
	cpad->Draw();

	apad->cd();
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	h1_1->SetMarkerColor(1);
	h1_1->SetLineColor(1);
	h1_1->Draw();
	h1_2->SetMarkerColor(600);
	h1_2->SetLineColor(600);
	h1_2->Draw("SAME");
	h1_3->SetMarkerColor(632);
	h1_3->SetLineColor(632);
	h1_3->Draw("SAME");

	bpad->cd();
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	h2_1->SetMarkerColor(1);
	h2_1->SetLineColor(1);
	h2_1->Draw();
	h2_2->SetMarkerColor(600);
	h2_2->SetLineColor(600);
	h2_2->Draw("SAME");
	h2_3->SetMarkerColor(632);
	h2_3->SetLineColor(632);
	h2_3->Draw("SAME");

	cpad->cd();
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	gPad->SetLogy(1);
	h20->SetMarkerColor(1);
	h20->SetLineColor(1);
	h20->Draw();
	h30->SetMarkerColor(600);
	h30->SetLineColor(600);
	h30->Draw("SAME");
	h40->SetMarkerColor(632);
	h40->SetLineColor(632);
	h40->Draw("SAME");
	legend = new TLegend(0.5,0.6,0.9,0.75);
	legend->AddEntry(h20,par+" Reached Collimator");
	legend->AddEntry(h30,par+" Passed Collimator");
	legend->AddEntry(h40,par+" Stopped in Target");
	legend->Draw("SAME");
	sum->Draw();

	// Draw Time Distribution
	TCanvas *c4 = new TCanvas("c4","c4",1024,768);
	apad->Draw();
	bpad->Draw();
	cpad->Draw();

	apad->cd();
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	gPad->SetLogy(1);
	h10->GetYaxis()->SetRangeUser(minimum,h10->GetMaximum()*2);
	h10->Draw();

	bpad->cd();
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	gPad->SetLogy(1);
	h11->GetYaxis()->SetRangeUser(minimum,h11->GetMaximum()*2);
	h11->Draw();
	if(info) info->Draw();

	cpad->cd();
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	gPad->SetLogy(1);
	h12->GetYaxis()->SetRangeUser(minimum,h12->GetMaximum()*2);
	h12->Draw();
}
