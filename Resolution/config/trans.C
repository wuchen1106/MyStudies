{
	TFile *f = new TFile("mom_residual_run969.root","update");
	TH1D * h1 = (TH1D*) f->Get("h1");
	TH1D * h2 = new TH1D("hres","hres",100,-5,5);
//	std::cout<<h2->FindBin(0)<<" "<<h2->FindBin(100)<<std::endl;
	for (int ibin = 1; ibin <= 101; ibin++){
		h2->SetBinContent(ibin,h1->GetBinContent(102-ibin));
	}
	h2->Write();
}
