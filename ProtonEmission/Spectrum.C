
Double_t fProtonEnergy(Double_t *x, Double_t *par)
{
     // from capture.pdf
     // x[0] : Kinetic energy E (MeV)
     // par[0] : A     (for Si 0.105 MeV^{-1})
     // par[1] : T_th  (for Si 1.4 MeV)
     // par[2] : alpha (for Si 1.328)   
     // par[3] : T_0   (for Si 3.1 MeV)
  
  Double_t E =x[0];
  Double_t f = par[0]
    *TMath::Power(1.0-par[1]/E,par[2])
    *TMath::Exp(-E/par[3]);
  return f;
}

void plotSiSpectrum()
{
  TF1 *f1 = new TF1("ProtonEnergy",fProtonEnergy,1.4,50.,4);
  f1->SetParameters(0.105, 1.4, 1.328, 3.1); // for Si
  f1->SetParNames("constant","T_{th}","#alpha","T_{0}");
  f1->Draw();
  f1->GetHistogram()->GetXaxis()->SetTitle("E (MeV)");
  f1->GetHistogram()->GetYaxis()->SetTitle("(protons/MeV/muonCapture)");
}

void plotIntegralSiSpectrum()
{
  TF1 *f1 = new TF1("ProtonEnergy",fProtonEnergy,1.4,50.,4);
  f1->SetParameters(0.105, 1.4, 1.328, 3.1); // for Si
  f1->SetParNames("constant","T_{th}","#alpha","T_{0}");
  f1->DrawIntegral();
  f1->GetHistogram()->GetXaxis()->SetTitle("E (MeV)");
  f1->GetHistogram()->GetYaxis()->SetTitle("(protons/muonCapture)");
}

void Sprectrum(){
	TCanvas *c1 = new TCanvas("c1","c1");
	plotSiSpectrum();
	TCanvas *c2 = new TCanvas("c2","c2");
	plotIntegralSiSpectrum();
	TCanvas *c3 = new TCanvas("c3","c3");
	TF1* f2 = new TF1("f2","[0]*pow(1-[1]/(sqrt(x*x+[4]*[4])-[4]),[2])*exp(-(sqrt(x*x+[4]*[4])-[4])/[3])",51.3,300);
	f2->SetParameter(0, 0.105); // A (MeV-1)
	f2->SetParameter(1, 1.4); // T_th (MeV)
	f2->SetParameter(2,1.3279); // alpha
	f2->SetParameter(3,3.1); // T_0
	f2->SetParameter(4,938.272); // Proton Mass (MeV)
	TH1D *h = f2->GetHistogram();
	h->Scale(1/h->Integral());
	h->SetName("h");
	TFile * f = new TFile("output.root","RECREATE");
	h->Write();
	f->Close();
}
