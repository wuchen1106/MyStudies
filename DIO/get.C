{
	TFile * infile  = new TFile("DIO.root");
	TH1D * h_dio = (TH1D*) infile->Get("h");
	h_dio->Scale(1./3e-15/0.61*0.39/h_dio->Integral());

	TH1D * h_reso[5];
	TString runname = "40um";
	//TString runname = "63um";
	infile  = new TFile("mom_p_"+runname+"_102MeV.root");
	h_reso[0] = (TH1D*) infile->Get("hp_"+runname+"_102MeV");
	h_reso[1] = (TH1D*) infile->Get("hp_"+runname+"_102MeV");
	h_reso[2] = (TH1D*) infile->Get("hp_"+runname+"_102MeV");
	infile  = new TFile("mom_p_"+runname+"_103MeV.root");
	h_reso[3] = (TH1D*) infile->Get("hp_"+runname+"_103MeV");
	infile  = new TFile("mom_p_"+runname+"_104MeV.root");
	h_reso[4] = (TH1D*) infile->Get("hp_"+runname+"_104MeV");
	for ( int i = 0; i<5; i++){
		h_reso[i]->Scale(1/(double)(h_reso[i]->Integral()));
	}

	int nbins_reso = h_reso[0]->GetNbinsX();
	double l_reso = h_reso[0]->GetBinLowEdge(1);
	double r_reso = h_reso[0]->GetBinLowEdge(1+nbins_reso);
	int bin0 = h_reso[0]->FindBin(0);

	std::vector<int> bins;
	bins.push_back(h_dio->FindBin(100));
	bins.push_back(h_dio->FindBin(101));
	bins.push_back(h_dio->FindBin(102));
	bins.push_back(h_dio->FindBin(103));
	bins.push_back(h_dio->FindBin(104));
	bins.push_back(h_dio->FindBin(105));

	TH1D * h_fin[6];
	for (int i = 0; i<5; i++){
		h_fin[i] = new TH1D(Form("h_fin%d",i),Form("DIO: %d-%d MeV/c",100+i,101+i),nbins_reso,l_reso+105,r_reso+105);
	}
	h_fin[5]  = new TH1D(Form("h_fin%d",i),"Signal",nbins_reso,l_reso+105,r_reso+105);
	h_fin[0]->SetFillColor(kOrange);
	h_fin[1]->SetFillColor(kGreen);
	h_fin[2]->SetFillColor(kBlue);
	h_fin[3]->SetFillColor(kMagenta);
	h_fin[4]->SetFillColor(kCyan);
	h_fin[5]->SetLineColor(kRed);

	for ( int isec = 0; isec<bins.size()-1; isec++){
		for ( int ibin = bins[isec]; ibin<bins[isec+1]; ibin++){
			double p = h_dio->GetBinCenter(ibin);
			double weight = h_dio->GetBinContent(ibin);
			int bintemp = h_fin[0]->FindBin(p);
			for ( int jbin = 1; jbin<=nbins_reso+bintemp-bin0; jbin++){
				h_fin[isec]->SetBinContent(jbin,h_fin[isec]->GetBinContent(jbin)+weight*h_reso[isec]->GetBinContent(jbin-bintemp+bin0));
			}
		}
	}
	int binshift = h_fin[0]->FindBin(104.973)-h_fin[0]->FindBin(104);
	std::cout<<binshift<<std::endl;
	for ( int jbin = 1; jbin<=nbins_reso-binshift; jbin++){
		h_fin[5]->SetBinContent(jbin+binshift,h_fin[5]->GetBinContent(jbin+binshift)+h_reso[4]->GetBinContent(jbin));
	}

	THStack * h_fint = new THStack("h_fint","Total");
	h_fin[0]->GetXaxis()->SetTitle("pFit (MeV/c)");
	h_fin[0]->GetYaxis()->SetTitle("Counts per 0.1 MeV/c");
	for (int i = 0; i<5; i++){
		h_fint->Add(h_fin[i]);
	}
	h_fin[0]->GetXaxis()->SetRangeUser(101.5,106);
	h_fint->Draw();
	h_fin[5]->Scale(1/h_fin[5]->Integral());
	h_fin[5]->Draw("SAME");
}
