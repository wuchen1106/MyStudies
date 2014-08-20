std::stringstream buf;

void drawit(TCanvas *c, TPad* p, TH1D* h1, TH1D* h2, TH1D* h3, TString title, TString tx, TString ty, TString t1, TString t2, TString t3, int c1, int c2, int c3, bool log = false, int opt = 1){
	c->cd();p->Draw(); p->cd();
	p->SetGridx(1);
	p->SetGridy(1);
	if (log) p->SetLogy(1);
	h1->SetLineColor(c1);
	h2->SetLineColor(c2);
	h3->SetLineColor(c3);
	h1->SetTitle(title);
	h1->GetXaxis()->SetTitle(tx);
	h1->GetYaxis()->SetTitle(ty);
	h1->Scale(1./5e5/5e5);
	h2->Scale(1./5e5);
	h3->Scale(1./5e5);
	h1->Draw();
	h2->Draw("SAME");
	h3->Draw("SAME");
    if (opt){
		TLegend * legend = 0;
		if (opt==1){
			legend = new TLegend(0.1,0.7,0.55,0.9);
		}
		else if (opt == -1){
			legend = new TLegend(0.55,0.7,1,0.9);
		}
		buf.str("");buf.clear();buf<<t1<<": "<<h1->Integral();
		legend->AddEntry(h1,buf.str().c_str());
		buf.str("");buf.clear();buf<<t2<<": "<<h2->Integral();
		legend->AddEntry(h2,buf.str().c_str());
		buf.str("");buf.clear();buf<<t3<<": "<<h3->Integral();
		legend->AddEntry(h3,buf.str().c_str());
		legend->Draw("SAME");
	}
}

void drawit(TCanvas *c, TPad* p, TH1D* h1, TH1D* h2, TString title, TString tx, TString ty, TString t1, TString t2, int c1, int c2, bool log = false, int opt = 1){
	c->cd();p->Draw(); p->cd();
	p->SetGridx(1);
	p->SetGridy(1);
	if (log) p->SetLogy(1);
	h1->SetLineColor(c1);
	h2->SetLineColor(c2);
	h1->SetTitle(title);
	h1->GetXaxis()->SetTitle(tx);
	h1->GetYaxis()->SetTitle(ty);
	h1->Scale(1./5e5);
	h2->Scale(1./5e5);
	h1->Draw();
	h2->Draw("SAME");
    if (opt){
		TLegend * legend = 0;
		if (opt==1){
			legend = new TLegend(0.1,0.7,0.55,0.9);
		}
		else if (opt == -1){
			legend = new TLegend(0.55,0.7,1,0.9);
		}
		buf.str("");buf.clear();buf<<t1<<": "<<h1->Integral();
		legend->AddEntry(h1,buf.str().c_str());
		buf.str("");buf.clear();buf<<t2<<": "<<h2->Integral();
		legend->AddEntry(h2,buf.str().c_str());
		legend->Draw("SAME");
	}
}

int draw(){
	TFile * input = new TFile("output.root");

	gStyle->SetOptStat(0);
	gStyle->SetPadTickX(1);
	gStyle->SetPadTickY(1);

	TCanvas *c1 = new TCanvas("c1","c1",1024,768);
	TPad * p1_1 = new TPad("p1_1","p1_1",0,0,1./4,0.5);
	drawit(c1,p1_1,h_yy_F_n0_ekin,h_c_F_n0_ekin,"Forward Neutron: Kinetic Energy","log_{10}(E_{kin}/MeV)","Count per Proton","P","G",600,632,false,0);

	TPad * p1_2 = new TPad("p1_3","p1_3",1./4,0,2./4,0.5);
	c1->cd();p1_2->Draw(); p1_2->cd();
	drawit(c1,p1_2,h_c_F_pp_ekin,h_yy_F_pp_ekin,"Forward Proton: Kinetic Energy","E_{kin} [MeV]","Count per Proton","G","P",632,600,true,0);

	TPad * p1_3 = new TPad("p1_4","p1_4",2./4,0,3./4,0.5);
	c1->cd();p1_3->Draw(); p1_3->cd();
	drawit(c1,p1_3,h_yy_F_pim_ekin,h_c_F_pim_ekin,"Forward Pion Minus: Kinetic Energy","E_{kin} [MeV]","Count per Proton","P","G",600,632,false,0);

	TPad * p1_4 = new TPad("p1_5","p1_5",3./4,0,4./4,0.5);
	c1->cd();p1_4->Draw(); p1_4->cd();
	drawit(c1,p1_4,h_yy_F_pip_ekin,h_c_F_pip_ekin,"Forward Pion Plus: Kinetic Energy","E_{kin} [MeV]","Count per Proton","P","G",600,632,false,0);

	TPad * p2_1 = new TPad("p2_1","p2_1",0,0.5,1./4,1);
	c1->cd();p2_1->Draw(); p2_1->cd();
	drawit(c1,p2_1,h_yy_F_n0_cos,h_c_F_n0_cos,"Forward Neutron: cos(#theta)","cos(#theta)","Count per Proton","P","G",600,632,true);

	TPad * p2_2 = new TPad("p2_3","p2_3",1./4,0.5,2./4,1);
	c1->cd();p2_2->Draw(); p2_2->cd();
	drawit(c1,p2_2,h_yy_F_pp_cos,h_c_F_pp_cos,"Forward Proton: cos(#theta)","cos(#theta)","Count per Proton","P","G",600,632,true);

	TPad * p2_3 = new TPad("p2_4","p2_4",2./4,0.5,3./4,1);
	c1->cd();p2_3->Draw(); p2_3->cd();
	drawit(c1,p2_3,h_yy_F_pim_cos,h_c_F_pim_cos,"Forward Pion Minus: cos(#theta)","cos(#theta)","Count per Proton","P","G",600,632,true);

	TPad * p2_4 = new TPad("p2_5","p2_5",3./4,0.5,4./4,1);
	c1->cd();p2_4->Draw(); p2_4->cd();
	drawit(c1,p2_4,h_yy_F_pip_cos,h_c_F_pip_cos,"Forward Pion Plus: cos(#theta)","cos(#theta)","Count per Proton","P","G",600,632,true);

	// Backward
	TCanvas *c2 = new TCanvas("c2","c2",1024,768);
	TPad * p1_1 = new TPad("p1_1","p1_1",0,0,1./4,0.5);
	drawit(c2,p1_1,h_yyFL_B_n0_ekin,h_c_B_n0_ekin,h_yy_B_n0_ekin,"Backward Neutron: Kinetic Energy","log_{10}(E_{kin}/MeV)","Count per Proton","F","G","P",616,632,600,true,1);

	TPad * p1_2 = new TPad("p1_3","p1_3",1./4,0,2./4,0.5);
	c2->cd();p1_2->Draw(); p1_2->cd();
	drawit(c2,p1_2,h_c_B_pp_ekin,h_yy_B_pp_ekin,"Backward Proton: Kinetic Energy","E_{kin} [MeV]","Count per Proton","G","P",632,600,false,0);

	TPad * p1_3 = new TPad("p1_4","p1_4",2./4,0,3./4,0.5);
	c2->cd();p1_3->Draw(); p1_3->cd();
	drawit(c2,p1_3,h_c_B_pim_ekin,h_yy_B_pim_ekin,"Backward Pion Minus: Kinetic Energy","E_{kin} [MeV]","Count per Proton","G","P",632,600,false,0);

	TPad * p1_4 = new TPad("p1_5","p1_5",3./4,0,4./4,0.5);
	c2->cd();p1_4->Draw(); p1_4->cd();
	drawit(c2,p1_4,h_c_B_pip_ekin,h_yy_B_pip_ekin,"Backward Pion Plus: Kinetic Energy","E_{kin} [MeV]","Count per Proton","G","P",632,600,false,0);

	TPad * p2_1 = new TPad("p2_1","p2_1",0,0.5,1./4,1);
	c2->cd();p2_1->Draw(); p2_1->cd();
	drawit(c2,p2_1,h_c_B_n0_cos,h_yy_B_n0_cos,"Backward Neutron: cos(#theta)","cos(#theta)","Count per Proton","G","P",632,600,true,-1);

	TPad * p2_2 = new TPad("p2_3","p2_3",1./4,0.5,2./4,1);
	c2->cd();p2_2->Draw(); p2_2->cd();
	drawit(c2,p2_2,h_c_B_pp_cos,h_yy_B_pp_cos,"Backward Proton: cos(#theta)","cos(#theta)","Count per Proton","G","P",632,600,true,-1);

	TPad * p2_3 = new TPad("p2_4","p2_4",2./4,0.5,3./4,1);
	c2->cd();p2_3->Draw(); p2_3->cd();
	drawit(c2,p2_3,h_c_B_pim_cos,h_yy_B_pim_cos,"Backward Pion Minus: cos(#theta)","cos(#theta)","Count per Proton","G","P",632,600,true,-1);

	TPad * p2_4 = new TPad("p2_5","p2_5",3./4,0.5,4./4,1);
	c2->cd();p2_4->Draw(); p2_4->cd();
	drawit(c2,p2_4,h_c_B_pip_cos,h_yy_B_pip_cos,"Backward Pion Plus: cos(#theta)","cos(#theta)","Count per Proton","G","P",632,600,true,-1);

	return 0;
}
