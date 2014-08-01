{
	TFile *f;
	f = new TFile("CDC.EP_OT.g40cm10mm.005T.Vac0928_140_140_39.g4s.QBH/output.root");
	TH2D *h1 = (TH2D*) f->Get("h1");
	TH1D *h2 = (TH1D*) f->Get("h2");

	f = new TFile("CDC.EP_em.g40cm10mm.005T.Vac0928_140_140_39.g4s.QBH/output.root");
	TH2D *ht1 = (TH2D*) f->Get("h1");
	TH1D *ht2 = (TH1D*) f->Get("h2");
	for (int i = 1; i<=400; i++){
		for (int j = 1; j<=400; j++){
			double x = -20 + i*0.1 - 0.05;
			double y = -20 + j*0.1 - 0.05;
			h1->Fill(x,y,ht1->GetBinContent(i,j));
		}
	}
	for (int i = 1; i<=110; i++){
		double x = -230 + i - 0.5;
		h2->Fill(x,ht2->GetBinContent(i));
	}

	f = new TFile("CDC.EP_mum.g40cm10mm.005T.Vac0928_140_140_39.g4s.QBH/output.root");
	TH2D *ht1 = (TH2D*) f->Get("h1");
	TH1D *ht2 = (TH1D*) f->Get("h2");
	for (int i = 1; i<=400; i++){
		for (int j = 1; j<=400; j++){
			double x = -20 + i*0.1 - 0.05;
			double y = -20 + j*0.1 - 0.05;
			h1->Fill(x,y,ht1->GetBinContent(i,j));
		}
	}
	for (int i = 1; i<=110; i++){
		double x = -230 + i - 0.5;
		h2->Fill(x,ht2->GetBinContent(i));
	}

	f = new TFile("CDC.pim.g40cm10mm.005T.Vac0928_140_140_39.g4s.QBH/output1.root");
	TH2D *ht1 = (TH2D*) f->Get("h1");
	TH1D *ht2 = (TH1D*) f->Get("h2");
	for (int i = 1; i<=400; i++){
		for (int j = 1; j<=400; j++){
			double x = -20 + i*0.1 - 0.05;
			double y = -20 + j*0.1 - 0.05;
			h1->Fill(x,y,ht1->GetBinContent(i,j));
		}
	}
	for (int i = 1; i<=110; i++){
		double x = -230 + i - 0.5;
		h2->Fill(x,ht2->GetBinContent(i));
	}

	f = new TFile("CDC.pim.g40cm10mm.005T.Vac0928_140_140_39.g4s.QBH/output2.root");
	TH2D *ht1 = (TH2D*) f->Get("h1");
	TH1D *ht2 = (TH1D*) f->Get("h2");
	for (int i = 1; i<=400; i++){
		for (int j = 1; j<=400; j++){
			double x = -20 + i*0.1 - 0.05;
			double y = -20 + j*0.1 - 0.05;
			h1->Fill(x,y,ht1->GetBinContent(i,j));
		}
	}
	for (int i = 1; i<=110; i++){
		double x = -230 + i - 0.5;
		h2->Fill(x,ht2->GetBinContent(i));
	}

	f = new TFile("output.root","RECREATE");
	h1->Write();
	h2->Write();
}
