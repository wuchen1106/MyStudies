{
	TCanvas *c1 = new TCanvas("c","c");
	TPad * p1 = new TPad("p1","p1",0,0,1./2,0.5);
	TPad * p2 = new TPad("p2","p2",1./2,0,2./2,0.5);
	TPad * p3 = new TPad("p3","p3",0,0.5,1,1);
	p1->Draw();
	p2->Draw();
	p3->Draw();
	p1->SetGridx(1);
	p1->SetGridy(1);
	p2->SetGridx(1);
	p2->SetGridy(1);
	p3->SetGridx(1);
	p3->SetGridy(1);
	gStyle->SetPalette(1);
	gStyle->SetOptStat(0);
	gStyle->SetPadTickX(1);
	gStyle->SetPadTickY(1);

	p1->cd();
	g1->GetHistogram()->Scale(2);
	g1->Draw("LAP");
	p2->cd();
	g2->GetHistogram()->Scale(2);
	g2->Draw("LAP");

	p3->cd();
	h1=(TH1D*)_file0->Get("Innermost Layer");
	h2=(TH1D*)_file0->Get("Seccond Innermost Layer");
	h3=(TH1D*)_file0->Get("Seccond Outmost Layer");
	h4=(TH1D*)_file0->Get("Outmost Layer");
	h1->Scale(2);
	h2->Scale(2);
	h3->Scale(2);
	h4->Scale(2);
    if (h1) h1->Draw("LP");
    if (h2) h2->Draw("LPSAME");
    if (h3) h3->Draw("LPSAME");
    if (h4) h4->Draw("LPSAME");

    TLegend *legend = new TLegend(0.8,0.8,1,1);
    if (h1) legend->AddEntry(h1,h1->GetName());
    if (h2) legend->AddEntry(h2,h2->GetName());
    if (h3) legend->AddEntry(h3,h3->GetName());
    if (h4) legend->AddEntry(h4,h4->GetName());
    legend->Draw("SAME");
}
