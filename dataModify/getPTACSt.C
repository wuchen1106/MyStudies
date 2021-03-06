{
	std::vector<int> pids;
	pids.push_back(-211); // pi-
	pids.push_back(13);   // mu-
	pids.push_back(11);   // e-
	pids.push_back(211);  // pi+
	pids.push_back(-13);  // mu+
	pids.push_back(-11);  // e+
	pids.push_back(2112); // neutron
	pids.push_back(2212); // proton
	pids.push_back(22);   // gamma

	std::vector<TString> pnames;
	pnames.push_back("pion-");
	pnames.push_back("muon-");
	pnames.push_back("electron-");
	pnames.push_back("pion+");
	pnames.push_back("muon+");
	pnames.push_back("electron+");
	pnames.push_back("neutron");
	pnames.push_back("proton");
	pnames.push_back("gamma");

	std::vector<int> pcharges;
	pcharges.push_back(-1); // pi-
	pcharges.push_back(-1); // mu-
	pcharges.push_back(-1); // e-
	pcharges.push_back(1);  // pi+
	pcharges.push_back(1);  // mu+
	pcharges.push_back(1);  // e+
	pcharges.push_back(0);  // neutron
	pcharges.push_back(1);  // proton
	pcharges.push_back(0);  // gamma

	std::vector<int> pcolors;
	pcolors.push_back(4); // pi-
	pcolors.push_back(2); // mu-
	pcolors.push_back(3); // e-
	pcolors.push_back(4); // pi+
	pcolors.push_back(2); // mu+
	pcolors.push_back(3); // e+
	pcolors.push_back(6); // neutron
	pcolors.push_back(6); // proton
	pcolors.push_back(1); // gamma

	TH1D* hists[20];
	for (int i=0; i<pnames.size(); i++){
		TH1D *h = new TH1D(pnames[i],pnames[i],200,0,2000);
		h->GetXaxis()->SetTitle("Time (ns)");
		h->GetYaxis()->SetTitle("count / initial proton");
		if (pcharges[i] == 0) h->SetLineStyle(9);
		else if (pcharges[i] < 0) h->SetLineStyle(1);
		else h->SetLineStyle(2);
		h->SetLineColor(pcolors[i]);
		hists[i]=h;
	}

	TFile *f = 0;
	for (int i = 0; i<1; i++){
		std::cout<<"##In File "<<i<<std::endl;
		double nProtons = 1;
		if (i==0){
			f = new TFile("PTACS.EP.EP.t16cm6mm.g4s.QBH.root");
//			f = new TFile("PTACS.EP_DUMP.EP.g60cm10mm.g4s.QBH.root");
//			f = new TFile("PTACS.EP.EP.g60cm10mm.g4s.QBH.root");
			nProtons = 1E7;
		}
		double weight = 1;
		double px;
		double py;
		double pz;
		double time;
		int pid;
		TTree *t = (TTree*)f->Get("t");
//		t->SetBranchAddress("weight",&weight);
		t->SetBranchAddress("px",&px);
		t->SetBranchAddress("py",&py);
		t->SetBranchAddress("pz",&pz);
		t->SetBranchAddress("t",&time);
		t->SetBranchAddress("pid",&pid);
		int nEvents = t->GetEntries();
		for (int iEvent = 0; iEvent<nEvents; iEvent++){
			t->GetEntry(iEvent);
			if (iEvent%100==0) std::cout<<(double)iEvent/nEvents*100<<"% ..."<<std::endl;
			if (iEvent%100==0) std::cout<<"pid = "<<pid<<std::endl;
//			double pa = sqrt(px*px+py*py+pz*pz);
			for (int ipid = 0; ipid<pids.size(); ipid++){
				if (pids[ipid]==pid){
					hists[ipid]->Fill(time,weight/nProtons);
					if (iEvent%100==0) std::cout<<"Found "<<pnames[ipid]<<"!"<<std::endl;
					break;
				}
			}
		}
	}

//	TFile *f = new TFile("MT1.g40cm10mm.0018T.root");
//	for (int i = 0; i<pids.size(); i++){
//		hists[i]=(TH1D*) f->Get(pnames[i]);
//	}

	gStyle->SetOptStat(0);
	gStyle->SetPadTickX(1);
	gStyle->SetPadTickY(1);
	TCanvas *c = new TCanvas("c");
	TPad *apad = new TPad("p","p",0,0,1,1);
	apad->Draw();
	apad->cd();
	gPad->SetLogy(1);
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	TFile *fo = new TFile("output.root","RECREATE");
	double maximum = 0;
	maximum = 0.003;
//	for (int i = 0; i<pids.size(); i++){
//		double currentMaximum = hists[i]->GetMaximum();
//		std::cout<<"currentMaximum for "<<pnames[i]<<" is "<<currentMaximum<<std::endl;
//		if (maximum < currentMaximum) maximum = currentMaximum;
//		std::cout<<"maximum = "<<maximum<<std::endl;
//	}
//	maximum*=2;
	TLegend *legend = new TLegend(0.8,0.1,1,0.9);
	std::stringstream buff;
	for (int i = 0; i<pids.size(); i++){
		std::cout<<"Setting for "<<pnames[i]<<": SetRangeUser(1e-9,"<<maximum<<")"<<std::endl;
		hists[i]->GetYaxis()->SetRangeUser(1e-8,maximum);
		if (i==0){
//			hists[i]->SetTitle("Particles Back to Capture Section (Graphite, 60cm)");
			hists[i]->SetTitle("Particles at The End of Capture Section (Graphite, 60cm)");
			hists[i]->Draw();
		}
		else hists[i]->Draw("SAME");
		double num = hists[i]->Integral();
		buff.str("");
		buff.clear();
		buff<<std::scientific;
		buff<<pnames[i]<<": "<<std::setprecision(2)<<num;
		legend->AddEntry(hists[i],buff.str().c_str());
		hists[i]->Write();
	}
	legend->Draw("SAME");
}
