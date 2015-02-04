{
	TFile * f = new TFile("TS2.BL.root");

	TH1D * h_a_mupi_x = (TH1D*) f->Get("h_a_mupi_x");
	TH1D * h_a_mupi_y = (TH1D*) f->Get("h_a_mupi_y");
	TH1D * h_a_mupi_p = (TH1D*) f->Get("h_a_mupi_p");

	TH1D * h_a_bkgc_x = (TH1D*) f->Get("h_a_bkgc_x");
	TH1D * h_a_bkgc_y = (TH1D*) f->Get("h_a_bkgc_y");
	TH1D * h_a_bkgc_p = (TH1D*) f->Get("h_a_bkgc_p");

	TH1D * h_b_mupi_x = (TH1D*) f->Get("h_b_mupi_x");
	TH1D * h_b_mupi_y = (TH1D*) f->Get("h_b_mupi_y");
	TH1D * h_b_mupi_p = (TH1D*) f->Get("h_b_mupi_p");

	TH1D * h_b_bkgc_x = (TH1D*) f->Get("h_b_bkgc_x");
	TH1D * h_b_bkgc_y = (TH1D*) f->Get("h_b_bkgc_y");
	TH1D * h_b_bkgc_p = (TH1D*) f->Get("h_b_bkgc_p");

	TH1D * h_c_mupi_x = (TH1D*) f->Get("h_c_mupi_x");
	TH1D * h_c_mupi_y = (TH1D*) f->Get("h_c_mupi_y");
	TH1D * h_c_mupi_p = (TH1D*) f->Get("h_c_mupi_p");

	TH1D * h_c_bkgc_x = (TH1D*) f->Get("h_c_bkgc_x");
	TH1D * h_c_bkgc_y = (TH1D*) f->Get("h_c_bkgc_y");
	TH1D * h_c_bkgc_p = (TH1D*) f->Get("h_c_bkgc_p");

	TH1D * h_d_mupi_x = (TH1D*) f->Get("h_d_mupi_x");
	TH1D * h_d_mupi_y = (TH1D*) f->Get("h_d_mupi_y");
	TH1D * h_d_mupi_p = (TH1D*) f->Get("h_d_mupi_p");

	TH1D * h_d_bkgc_x = (TH1D*) f->Get("h_d_bkgc_x");
	TH1D * h_d_bkgc_y = (TH1D*) f->Get("h_d_bkgc_y");
	TH1D * h_d_bkgc_p = (TH1D*) f->Get("h_d_bkgc_p");

	TCanvas * ca = new TCanvas("ca","ca",1280,640);
	TPad * pa1 = new TPad("pa1","pa1",0,0,1./3,1);
	TPad * pa2 = new TPad("pa2","pa2",1./3,0,2./3,1);
	TPad * pa3 = new TPad("pa3","pa3",2./3,0,1,1);
	pa1->Draw();
	pa2->Draw();
	pa3->Draw();
	pa1->cd();
	h_a_bkgc_x->SetLineColor(kBlue);
	h_a_bkgc_x->Draw();
	h_a_mupi_x->SetLineColor(kRed);
	h_a_mupi_x->Draw("SAME");
	pa2->cd();
	h_a_bkgc_y->SetLineColor(kBlue);
	h_a_bkgc_y->Draw();
	h_a_mupi_y->SetLineColor(kRed);
	h_a_mupi_y->Draw("SAME");
	pa3->cd();
	h_a_bkgc_p->SetLineColor(kBlue);
	h_a_bkgc_p->Draw();
	h_a_mupi_p->SetLineColor(kRed);
	h_a_mupi_p->Draw("SAME");

	TCanvas * cb = new TCanvas("cb","cb",1280,640);
	TPad * pb1 = new TPad("pb1","pb1",0,0,1./3,1);
	TPad * pb2 = new TPad("pb2","pb2",1./3,0,2./3,1);
	TPad * pb3 = new TPad("pb3","pb3",2./3,0,1,1);
	pb1->Draw();
	pb2->Draw();
	pb3->Draw();
	pb1->cd();
	h_b_bkgc_x->SetLineColor(kBlue);
	h_b_bkgc_x->Draw();
	h_b_mupi_x->SetLineColor(kRed);
	h_b_mupi_x->Draw("SAME");
	pb2->cd();
	h_b_bkgc_y->SetLineColor(kBlue);
	h_b_bkgc_y->Draw();
	h_b_mupi_y->SetLineColor(kRed);
	h_b_mupi_y->Draw("SAME");
	pb3->cd();
	h_b_bkgc_p->SetLineColor(kBlue);
	h_b_bkgc_p->Draw();
	h_b_mupi_p->SetLineColor(kRed);
	h_b_mupi_p->Draw("SAME");

	TCanvas * cc = new TCanvas("cc","cc",1280,640);
	TPad * pc1 = new TPad("pc1","pc1",0,0,1./3,1);
	TPad * pc2 = new TPad("pc2","pc2",1./3,0,2./3,1);
	TPad * pc3 = new TPad("pc3","pc3",2./3,0,1,1);
	pc1->Draw();
	pc2->Draw();
	pc3->Draw();
	pc1->cd();
	h_c_bkgc_x->SetLineColor(kBlue);
	h_c_bkgc_x->Draw();
	h_c_mupi_x->SetLineColor(kRed);
	h_c_mupi_x->Draw("SAME");
	pc2->cd();
	h_c_bkgc_y->SetLineColor(kBlue);
	h_c_bkgc_y->Draw();
	h_c_mupi_y->SetLineColor(kRed);
	h_c_mupi_y->Draw("SAME");
	pc3->cd();
	h_c_bkgc_p->SetLineColor(kBlue);
	h_c_bkgc_p->Draw();
	h_c_mupi_p->SetLineColor(kRed);
	h_c_mupi_p->Draw("SAME");

	TCanvas * cd = new TCanvas("cd","cd",1280,640);
	TPad * pd1 = new TPad("pd1","pd1",0,0,1./3,1);
	TPad * pd2 = new TPad("pd2","pd2",1./3,0,2./3,1);
	TPad * pd3 = new TPad("pd3","pd3",2./3,0,1,1);
	pd1->Draw();
	pd2->Draw();
	pd3->Draw();
	pd1->cd();
	h_d_bkgc_x->SetLineColor(kBlue);
	h_d_bkgc_x->Draw();
	h_d_mupi_x->SetLineColor(kRed);
	h_d_mupi_x->Draw("SAME");
	pd2->cd();
	h_d_bkgc_y->SetLineColor(kBlue);
	h_d_bkgc_y->Draw();
	h_d_mupi_y->SetLineColor(kRed);
	h_d_mupi_y->Draw("SAME");
	pd3->cd();
	h_d_bkgc_p->SetLineColor(kBlue);
	h_d_bkgc_p->Draw();
	h_d_mupi_p->SetLineColor(kRed);
	h_d_mupi_p->Draw("SAME");

}
