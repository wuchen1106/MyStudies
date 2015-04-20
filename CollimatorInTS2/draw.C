{
	TFile * f = new TFile("Coll.root");

	TH1D * h_a_mupi_x = (TH1D*) f->Get("h_a_mupi_x");
	TH1D * h_a_mupi_y = (TH1D*) f->Get("h_a_mupi_y");
	TH1D * h_a_mupi_p = (TH1D*) f->Get("h_a_mupi_p");

	TH1D * h_b_mupi_x = (TH1D*) f->Get("h_b_mupi_x");
	TH1D * h_b_mupi_y = (TH1D*) f->Get("h_b_mupi_y");
	TH1D * h_b_mupi_p = (TH1D*) f->Get("h_b_mupi_p");

	TH1D * h_c_mupi_x = (TH1D*) f->Get("h_c_mupi_x");
	TH1D * h_c_mupi_y = (TH1D*) f->Get("h_c_mupi_y");
	TH1D * h_c_mupi_p = (TH1D*) f->Get("h_c_mupi_p");

	TH1D * h_d0_mupi_x = (TH1D*) f->Get("h_d0_mupi_x");
	TH1D * h_d0_mupi_y = (TH1D*) f->Get("h_d0_mupi_y");
	TH1D * h_d0_mupi_p = (TH1D*) f->Get("h_d0_mupi_p");

	TH1D * h_d1_mupi_x = (TH1D*) f->Get("h_d1_mupi_x");
	TH1D * h_d1_mupi_y = (TH1D*) f->Get("h_d1_mupi_y");
	TH1D * h_d1_mupi_p = (TH1D*) f->Get("h_d1_mupi_p");

	TH1D * h_d2_mupi_x = (TH1D*) f->Get("h_d2_mupi_x");
	TH1D * h_d2_mupi_y = (TH1D*) f->Get("h_d2_mupi_y");
	TH1D * h_d2_mupi_p = (TH1D*) f->Get("h_d2_mupi_p");

	TH1D * h_d3_mupi_x = (TH1D*) f->Get("h_d3_mupi_x");
	TH1D * h_d3_mupi_y = (TH1D*) f->Get("h_d3_mupi_y");
	TH1D * h_d3_mupi_p = (TH1D*) f->Get("h_d3_mupi_p");

	TH2D * h_a_mupi_xy = (TH2D*) f->Get("h_a_mupi_xy");
	TH2D * h_b_mupi_xy = (TH2D*) f->Get("h_b_mupi_xy");
	TH2D * h_c_mupi_xy = (TH2D*) f->Get("h_c_mupi_xy");
	TH2D * h_d0_mupi_xy = (TH2D*) f->Get("h_d0_mupi_xy");
	TH2D * h_d1_mupi_xy = (TH2D*) f->Get("h_d1_mupi_xy");
	TH2D * h_d2_mupi_xy = (TH2D*) f->Get("h_d2_mupi_xy");
	TH2D * h_d3_mupi_xy = (TH2D*) f->Get("h_d3_mupi_xy");

	TH2D * h_a_bkgm_xy = (TH2D*) f->Get("h_a_bkgm_xy");
	TH2D * h_b_bkgm_xy = (TH2D*) f->Get("h_b_bkgm_xy");
	TH2D * h_c_bkgm_xy = (TH2D*) f->Get("h_c_bkgm_xy");
	TH2D * h_d0_bkgm_xy = (TH2D*) f->Get("h_d0_bkgm_xy");
	TH2D * h_d1_bkgm_xy = (TH2D*) f->Get("h_d1_bkgm_xy");
	TH2D * h_d2_bkgm_xy = (TH2D*) f->Get("h_d2_bkgm_xy");
	TH2D * h_d3_bkgm_xy = (TH2D*) f->Get("h_d3_bkgm_xy");
	TH2D * h_a_bkgp_xy = (TH2D*) f->Get("h_a_bkgp_xy");
	TH2D * h_b_bkgp_xy = (TH2D*) f->Get("h_b_bkgp_xy");
	TH2D * h_c_bkgp_xy = (TH2D*) f->Get("h_c_bkgp_xy");
	TH2D * h_d0_bkgp_xy = (TH2D*) f->Get("h_d0_bkgp_xy");
	TH2D * h_d1_bkgp_xy = (TH2D*) f->Get("h_d1_bkgp_xy");
	TH2D * h_d2_bkgp_xy = (TH2D*) f->Get("h_d2_bkgp_xy");
	TH2D * h_d3_bkgp_xy = (TH2D*) f->Get("h_d3_bkgp_xy");

	TH1D * h_a_bkgp_x = (TH1D*) f->Get("h_a_bkgp_x");
	TH1D * h_a_bkgp_y = (TH1D*) f->Get("h_a_bkgp_y");
	TH1D * h_a_bkgp_p = (TH1D*) f->Get("h_a_bkgp_p");

	TH1D * h_a_bkgp_x = (TH1D*) f->Get("h_a_bkgp_x");
	TH1D * h_a_bkgp_y = (TH1D*) f->Get("h_a_bkgp_y");
	TH1D * h_a_bkgp_p = (TH1D*) f->Get("h_a_bkgp_p");

	TH1D * h_b_bkgp_x = (TH1D*) f->Get("h_b_bkgp_x");
	TH1D * h_b_bkgp_y = (TH1D*) f->Get("h_b_bkgp_y");
	TH1D * h_b_bkgp_p = (TH1D*) f->Get("h_b_bkgp_p");

	TH1D * h_c_bkgp_x = (TH1D*) f->Get("h_c_bkgp_x");
	TH1D * h_c_bkgp_y = (TH1D*) f->Get("h_c_bkgp_y");
	TH1D * h_c_bkgp_p = (TH1D*) f->Get("h_c_bkgp_p");

	TH1D * h_d0_bkgp_x = (TH1D*) f->Get("h_d0_bkgp_x");
	TH1D * h_d0_bkgp_y = (TH1D*) f->Get("h_d0_bkgp_y");
	TH1D * h_d0_bkgp_p = (TH1D*) f->Get("h_d0_bkgp_p");

	TH1D * h_d1_bkgp_x = (TH1D*) f->Get("h_d1_bkgp_x");
	TH1D * h_d1_bkgp_y = (TH1D*) f->Get("h_d1_bkgp_y");
	TH1D * h_d1_bkgp_p = (TH1D*) f->Get("h_d1_bkgp_p");

	TH1D * h_d2_bkgp_x = (TH1D*) f->Get("h_d2_bkgp_x");
	TH1D * h_d2_bkgp_y = (TH1D*) f->Get("h_d2_bkgp_y");
	TH1D * h_d2_bkgp_p = (TH1D*) f->Get("h_d2_bkgp_p");

	TH1D * h_d3_bkgp_x = (TH1D*) f->Get("h_d3_bkgp_x");
	TH1D * h_d3_bkgp_y = (TH1D*) f->Get("h_d3_bkgp_y");
	TH1D * h_d3_bkgp_p = (TH1D*) f->Get("h_d3_bkgp_p");

	TH1D * h_a_bkgm_x = (TH1D*) f->Get("h_a_bkgm_x");
	TH1D * h_a_bkgm_y = (TH1D*) f->Get("h_a_bkgm_y");
	TH1D * h_a_bkgm_p = (TH1D*) f->Get("h_a_bkgm_p");

	TH1D * h_a_bkgm_x = (TH1D*) f->Get("h_a_bkgm_x");
	TH1D * h_a_bkgm_y = (TH1D*) f->Get("h_a_bkgm_y");
	TH1D * h_a_bkgm_p = (TH1D*) f->Get("h_a_bkgm_p");

	TH1D * h_b_bkgm_x = (TH1D*) f->Get("h_b_bkgm_x");
	TH1D * h_b_bkgm_y = (TH1D*) f->Get("h_b_bkgm_y");
	TH1D * h_b_bkgm_p = (TH1D*) f->Get("h_b_bkgm_p");

	TH1D * h_c_bkgm_x = (TH1D*) f->Get("h_c_bkgm_x");
	TH1D * h_c_bkgm_y = (TH1D*) f->Get("h_c_bkgm_y");
	TH1D * h_c_bkgm_p = (TH1D*) f->Get("h_c_bkgm_p");

	TH1D * h_d0_bkgm_x = (TH1D*) f->Get("h_d0_bkgm_x");
	TH1D * h_d0_bkgm_y = (TH1D*) f->Get("h_d0_bkgm_y");
	TH1D * h_d0_bkgm_p = (TH1D*) f->Get("h_d0_bkgm_p");

	TH1D * h_d1_bkgm_x = (TH1D*) f->Get("h_d1_bkgm_x");
	TH1D * h_d1_bkgm_y = (TH1D*) f->Get("h_d1_bkgm_y");
	TH1D * h_d1_bkgm_p = (TH1D*) f->Get("h_d1_bkgm_p");

	TH1D * h_d2_bkgm_x = (TH1D*) f->Get("h_d2_bkgm_x");
	TH1D * h_d2_bkgm_y = (TH1D*) f->Get("h_d2_bkgm_y");
	TH1D * h_d2_bkgm_p = (TH1D*) f->Get("h_d2_bkgm_p");

	TH1D * h_d3_bkgm_x = (TH1D*) f->Get("h_d3_bkgm_x");
	TH1D * h_d3_bkgm_y = (TH1D*) f->Get("h_d3_bkgm_y");
	TH1D * h_d3_bkgm_p = (TH1D*) f->Get("h_d3_bkgm_p");

	/*
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

	TCanvas * cd = new TCanvas("cd0","cd0",1280,640);
	TPad * pd01 = new TPad("pd01","pd01",0,0,1./3,1);
	TPad * pd02 = new TPad("pd02","pd02",1./3,0,2./3,1);
	TPad * pd03 = new TPad("pd03","pd03",2./3,0,1,1);
	pd01->Draw();
	pd02->Draw();
	pd03->Draw();
	pd01->cd();
	h_d0_bkgc_x->SetLineColor(kBlue);
	h_d0_bkgc_x->Draw();
	h_d0_mupi_x->SetLineColor(kRed);
	h_d0_mupi_x->Draw("SAME");
	pd02->cd();
	h_d0_bkgc_y->SetLineColor(kBlue);
	h_d0_bkgc_y->Draw();
	h_d0_mupi_y->SetLineColor(kRed);
	h_d0_mupi_y->Draw("SAME");
	pd03->cd();
	h_d0_bkgc_p->SetLineColor(kBlue);
	h_d0_bkgc_p->Draw();
	h_d0_mupi_p->SetLineColor(kRed);
	h_d0_mupi_p->Draw("SAME");

	TCanvas * cd = new TCanvas("cd1","cd1",1280,640);
	TPad * pd11 = new TPad("pd11","pd11",0,0,1./3,1);
	TPad * pd12 = new TPad("pd12","pd12",1./3,0,2./3,1);
	TPad * pd13 = new TPad("pd13","pd13",2./3,0,1,1);
	pd11->Draw();
	pd12->Draw();
	pd13->Draw();
	pd11->cd();
	h_d1_bkgc_x->SetLineColor(kBlue);
	h_d1_bkgc_x->Draw();
	h_d1_mupi_x->SetLineColor(kRed);
	h_d1_mupi_x->Draw("SAME");
	pd12->cd();
	h_d1_bkgc_y->SetLineColor(kBlue);
	h_d1_bkgc_y->Draw();
	h_d1_mupi_y->SetLineColor(kRed);
	h_d1_mupi_y->Draw("SAME");
	pd13->cd();
	h_d1_bkgc_p->SetLineColor(kBlue);
	h_d1_bkgc_p->Draw();
	h_d1_mupi_p->SetLineColor(kRed);
	h_d1_mupi_p->Draw("SAME");

	TCanvas * cd = new TCanvas("cd2","cd2",1280,640);
	TPad * pd21 = new TPad("pd21","pd21",0,0,1./3,1);
	TPad * pd22 = new TPad("pd22","pd22",1./3,0,2./3,1);
	TPad * pd23 = new TPad("pd23","pd23",2./3,0,1,1);
	pd21->Draw();
	pd22->Draw();
	pd23->Draw();
	pd21->cd();
	h_d2_bkgc_x->SetLineColor(kBlue);
	h_d2_bkgc_x->Draw();
	h_d2_mupi_x->SetLineColor(kRed);
	h_d2_mupi_x->Draw("SAME");
	pd22->cd();
	h_d2_bkgc_y->SetLineColor(kBlue);
	h_d2_bkgc_y->Draw();
	h_d2_mupi_y->SetLineColor(kRed);
	h_d2_mupi_y->Draw("SAME");
	pd23->cd();
	h_d2_bkgc_p->SetLineColor(kBlue);
	h_d2_bkgc_p->Draw();
	h_d2_mupi_p->SetLineColor(kRed);
	h_d2_mupi_p->Draw("SAME");

	TCanvas * cd = new TCanvas("cd3","cd3",1280,640);
	TPad * pd31 = new TPad("pd31","pd31",0,0,1./3,1);
	TPad * pd32 = new TPad("pd32","pd32",1./3,0,2./3,1);
	TPad * pd33 = new TPad("pd33","pd33",2./3,0,1,1);
	pd31->Draw();
	pd32->Draw();
	pd33->Draw();
	pd31->cd();
	h_d3_bkgc_x->SetLineColor(kBlue);
	h_d3_bkgc_x->Draw();
	h_d3_mupi_x->SetLineColor(kRed);
	h_d3_mupi_x->Draw("SAME");
	pd32->cd();
	h_d3_bkgc_y->SetLineColor(kBlue);
	h_d3_bkgc_y->Draw();
	h_d3_mupi_y->SetLineColor(kRed);
	h_d3_mupi_y->Draw("SAME");
	pd33->cd();
	h_d3_bkgc_p->SetLineColor(kBlue);
	h_d3_bkgc_p->Draw();
	h_d3_mupi_p->SetLineColor(kRed);
	h_d3_mupi_p->Draw("SAME");
	*/

	TEllipse col_ell1(0,0,12,12);
	col_ell1.SetFillStyle(0);
	TEllipse col_ell2(0,0,18,18);
	col_ell2.SetFillStyle(0);
	TLine col_lu(-18,8.5,18,8.5);
	TLine col_ld(-18,-10,18,-10);

	TCanvas * ce = new TCanvas("ce","ce",1280,640);
	TPad * pe11 = new TPad("pe11","pe11",0,0.5,1./4,1);
	TPad * pe12 = new TPad("pe12","pe12",1./4,0.5,2./4,1);
	TPad * pe13 = new TPad("pe13","pe13",2./4,0.5,3./4,1);
	TPad * pe14 = new TPad("pe14","pe14",3./4,0.5,4./4,1);
	TPad * pe21 = new TPad("pe21","pe21",0,0,1./4,0.5);
	TPad * pe22 = new TPad("pe22","pe22",1./4,0,2./4,0.5);
	TPad * pe23 = new TPad("pe23","pe23",2./4,0,3./4,0.5);
	TPad * pe24 = new TPad("pe24","pe24",3./4,0,4./4,0.5);
	pe11->Draw();
	pe12->Draw();
	pe13->Draw();
	pe14->Draw();
	pe21->Draw();
	pe22->Draw();
	pe23->Draw();
	pe24->Draw();

	pe11->cd();
	h_e_mupi_xy->SetMarkerColor(kGreen);
	h_e_mupi_xy->SetMarkerStyle(6);
	h_e_bkgp_xy->SetMarkerColor(kRed);
	h_e_bkgm_xy->SetMarkerColor(kBlue);
	h_e_mupi_xy->Draw();
	h_e_bkgp_xy->Draw("SAME");
	h_e_bkgm_xy->Draw("SAME");
	col_ell1.Draw("SAME");
	col_ell2.Draw("SAME");
	col_lu.Draw("SAME");
	col_ld.Draw("SAME");

	pe12->cd();
	h_a_mupi_xy->SetMarkerColor(kGreen);
	h_a_mupi_xy->SetMarkerStyle(6);
	h_a_bkgp_xy->SetMarkerColor(kRed);
	h_a_bkgm_xy->SetMarkerColor(kBlue);
	h_a_mupi_xy->Draw();
	h_a_bkgm_xy->Draw("SAME");
	h_a_bkgp_xy->Draw("SAME");
	col_ell1.Draw("SAME");
	col_ell2.Draw("SAME");
	col_lu.Draw("SAME");
	col_ld.Draw("SAME");

	pe13->cd();
	h_b_mupi_xy->SetMarkerColor(kGreen);
	h_b_mupi_xy->SetMarkerStyle(6);
	h_b_bkgp_xy->SetMarkerColor(kRed);
	h_b_bkgm_xy->SetMarkerColor(kBlue);
	h_b_mupi_xy->Draw();
	h_b_bkgm_xy->Draw("SAME");
	h_b_bkgp_xy->Draw("SAME");
	col_ell1.Draw("SAME");
	col_ell2.Draw("SAME");
	col_lu.Draw("SAME");
	col_ld.Draw("SAME");

	pe14->cd();
	h_c_mupi_xy->SetMarkerColor(kGreen);
	h_c_mupi_xy->SetMarkerStyle(6);
	h_c_bkgp_xy->SetMarkerColor(kRed);
	h_c_bkgm_xy->SetMarkerColor(kBlue);
	h_c_mupi_xy->Draw();
	h_c_bkgp_xy->Draw("SAME");
	h_c_bkgm_xy->Draw("SAME");
	col_ell1.Draw("SAME");
	col_ell2.Draw("SAME");
	col_lu.Draw("SAME");
	col_ld.Draw("SAME");

	pe21->cd();
	h_d0_mupi_xy->SetMarkerColor(kGreen);
	h_d0_mupi_xy->SetMarkerStyle(6);
	h_d0_bkgp_xy->SetMarkerColor(kRed);
	h_d0_bkgm_xy->SetMarkerColor(kBlue);
	h_d0_mupi_xy->Draw();
	h_d0_bkgp_xy->Draw("SAME");
	h_d0_bkgm_xy->Draw("SAME");
	col_ell1.Draw("SAME");
	col_ell2.Draw("SAME");
	col_lu.Draw("SAME");
	col_ld.Draw("SAME");

	pe22->cd();
	h_d1_mupi_xy->SetMarkerColor(kGreen);
	h_d1_mupi_xy->SetMarkerStyle(6);
	h_d1_bkgp_xy->SetMarkerColor(kRed);
	h_d1_bkgm_xy->SetMarkerColor(kBlue);
	h_d1_mupi_xy->Draw();
	h_d1_bkgm_xy->Draw("SAME");
	h_d1_bkgp_xy->Draw("SAME");
	col_ell1.Draw("SAME");
	col_ell2.Draw("SAME");
	col_lu.Draw("SAME");
	col_ld.Draw("SAME");

	pe23->cd();
	h_d2_mupi_xy->SetMarkerColor(kGreen);
	h_d2_mupi_xy->SetMarkerStyle(6);
	h_d2_bkgp_xy->SetMarkerColor(kRed);
	h_d2_bkgm_xy->SetMarkerColor(kBlue);
	h_d2_mupi_xy->Draw();
	h_d2_bkgp_xy->Draw("SAME");
	h_d2_bkgm_xy->Draw("SAME");
	col_ell1.Draw("SAME");
	col_ell2.Draw("SAME");
	col_lu.Draw("SAME");
	col_ld.Draw("SAME");

	pe24->cd();
	h_d3_mupi_xy->SetMarkerColor(kGreen);
	h_d3_mupi_xy->SetMarkerStyle(6);
	h_d3_bkgp_xy->SetMarkerColor(kRed);
	h_d3_bkgm_xy->SetMarkerColor(kBlue);
	h_d3_mupi_xy->Draw("");
	h_d3_bkgp_xy->Draw("SAME");
	h_d3_bkgm_xy->Draw("SAME");
	col_ell1.Draw("SAME");
	col_ell2.Draw("SAME");
	col_lu.Draw("SAME");
	col_ld.Draw("SAME");

	TLine col_l2u(8.5,0,8.5,1e5);
	TLine col_l2d(-10,0,-10,1e5);

	TCanvas * c1 = new TCanvas("c1","c1",1280,640);
	TPad * pa11 = new TPad("pa11","pa11",0,0.5,1./4,1);
	TPad * pa12 = new TPad("pa12","pa12",1./4,0.5,2./4,1);
	TPad * pa13 = new TPad("pa13","pa13",2./4,0.5,3./4,1);
	TPad * pa14 = new TPad("pa14","pa14",3./4,0.5,4./4,1);
	TPad * pa21 = new TPad("pa21","pa21",0,0,1./4,0.5);
	TPad * pa22 = new TPad("pa22","pa22",1./4,0,2./4,0.5);
	TPad * pa23 = new TPad("pa23","pa23",2./4,0,3./4,0.5);
	TPad * pa24 = new TPad("pa24","pa24",3./4,0,4./4,0.5);
	pa11->Draw();
	pa12->Draw();
	pa13->Draw();
	pa14->Draw();
	pa21->Draw();
	pa22->Draw();
	pa23->Draw();
	pa24->Draw();

	pa11->cd();
	h_e_mupi_y->SetLineColor(kGreen);
	h_e_bkgp_y->SetLineColor(kRed);
	h_e_bkgm_y->SetLineColor(kBlue);
	h_e_mupi_y->Draw();
	h_e_bkgp_y->Draw("SAME");
	h_e_bkgm_y->Draw("SAME");
	col_l2u.Draw("SAME");
	col_l2d.Draw("SAME");

	pa12->cd();
	h_a_mupi_y->SetLineColor(kGreen);
	h_a_bkgp_y->SetLineColor(kRed);
	h_a_bkgm_y->SetLineColor(kBlue);
	h_a_mupi_y->Draw();
	h_a_bkgm_y->Draw("SAME");
	h_a_bkgp_y->Draw("SAME");
	col_l2u.Draw("SAME");
	col_l2d.Draw("SAME");

	pa13->cd();
	h_b_mupi_y->SetLineColor(kGreen);
	h_b_bkgp_y->SetLineColor(kRed);
	h_b_bkgm_y->SetLineColor(kBlue);
	h_b_mupi_y->Draw();
	h_b_bkgm_y->Draw("SAME");
	h_b_bkgp_y->Draw("SAME");
	col_l2u.Draw("SAME");
	col_l2d.Draw("SAME");

	pa14->cd();
	h_c_mupi_y->SetLineColor(kGreen);
	h_c_bkgp_y->SetLineColor(kRed);
	h_c_bkgm_y->SetLineColor(kBlue);
	h_c_mupi_y->Draw();
	h_c_bkgp_y->Draw("SAME");
	h_c_bkgm_y->Draw("SAME");
	col_l2u.Draw("SAME");
	col_l2d.Draw("SAME");

	pa21->cd();
	h_d0_mupi_y->SetLineColor(kGreen);
	h_d0_bkgp_y->SetLineColor(kRed);
	h_d0_bkgm_y->SetLineColor(kBlue);
	h_d0_mupi_y->Draw();
	h_d0_bkgp_y->Draw("SAME");
	h_d0_bkgm_y->Draw("SAME");
	col_l2u.Draw("SAME");
	col_l2d.Draw("SAME");

	pa22->cd();
	h_d1_mupi_y->SetLineColor(kGreen);
	h_d1_bkgp_y->SetLineColor(kRed);
	h_d1_bkgm_y->SetLineColor(kBlue);
	h_d1_mupi_y->Draw();
	h_d1_bkgm_y->Draw("SAME");
	h_d1_bkgp_y->Draw("SAME");
	col_l2u.Draw("SAME");
	col_l2d.Draw("SAME");

	pa23->cd();
	h_d2_mupi_y->SetLineColor(kGreen);
	h_d2_bkgp_y->SetLineColor(kRed);
	h_d2_bkgm_y->SetLineColor(kBlue);
	h_d2_mupi_y->Draw();
	h_d2_bkgp_y->Draw("SAME");
	h_d2_bkgm_y->Draw("SAME");
	col_l2u.Draw("SAME");
	col_l2d.Draw("SAME");

	pa24->cd();
	h_d3_mupi_y->SetLineColor(kGreen);
	h_d3_bkgp_y->SetLineColor(kRed);
	h_d3_bkgm_y->SetLineColor(kBlue);
	h_d3_mupi_y->Draw("");
	h_d3_bkgp_y->Draw("SAME");
	h_d3_bkgm_y->Draw("SAME");
	col_l2u.Draw("SAME");
	col_l2d.Draw("SAME");
}
