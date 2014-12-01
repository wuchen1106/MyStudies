{
	std::stringstream buf;
	double scalefactor = 8e4;
	std::vector<int> tids;
	TH1D * hori_pi = new TH1D("hori_pi","hori_pi",64,0,350);
	TH1D * hori_mu = new TH1D("hori_mu","hori_mu",64,0,350);
	TH1D * hflip_pi = new TH1D("hflip_pi","hflip_pi",64,0,350);
	TH1D * hflip_mu = new TH1D("hflip_mu","hflip_mu",64,0,350);

//	TString runName = "testCapture111026";
	TString runName = "testCapture140625";

	TChain * cori = new TChain("tree","tree");
	//cori->Add("/home/chen/MyWorkArea/Simulate/comet/output/"+runName+"ori.root");

	TChain * cflip = new TChain("tree","tree");
	//cflip->Add("/home/chen/MyWorkArea/Simulate/comet/output/"+runName+"flip.root");

	for (int i = 0; i<5; i++){
		buf.str(""); buf.clear();
		buf<<"/home/chen/MyWorkArea/Data/"<<runName<<"ori/"<<i<<"_job0.raw";
		cori->Add(buf.str().c_str());
		buf.str(""); buf.clear();
		buf<<"/home/chen/MyWorkArea/Data/"<<runName<<"flip/"<<i<<"_job0.raw";
		cflip->Add(buf.str().c_str());
	}

	std::vector<double> * vpx=0;
	std::vector<double> * vpy=0;
	std::vector<double> * vpz=0;
	std::vector<int> * vpid=0;
	std::vector<int> * vtid=0;
	std::vector<std::string> * vvolname=0;

	cori->SetBranchAddress("V_px",&vpx);
	cori->SetBranchAddress("V_py",&vpy);
	cori->SetBranchAddress("V_pz",&vpz);
	cori->SetBranchAddress("V_pid",&vpid);
	cori->SetBranchAddress("V_tid",&vtid);
	cori->SetBranchAddress("V_volName",&vvolname);
	for ( int i = 0; i< cori->GetEntries(); i++){
		cori->GetEntry(i);
		tids->clear();
		bool found = false;
		for ( int k = 0; k<vtid->size(); k++){
			if ((*vvolname)[k]!="TS1") continue;
			//std::cout<<(*vvolname)[k]<<":"<<(*vtid)[k]<<std::endl;
			for ( int j  = 0; j<tids.size(); j++){
				if ((*vtid)[k]==tids[j]){
					found = true;
					break;
				}
			}
			if (found) continue;
			else{
				tids.push_back((*vtid)[k]);
				double px = (*vpx)[k];
				double py = (*vpy)[k];
				double pz = (*vpz)[k];
				double pa = sqrt(px*px+py*py+pz*pz)*1000;
				if ((*vpid)[k]==13)
					hori_mu->Fill(pa);
				else if ((*vpid)[k]==-211)
					hori_pi->Fill(pa);
			}
		}
	}

	cflip->SetBranchAddress("V_px",&vpx);
	cflip->SetBranchAddress("V_py",&vpy);
	cflip->SetBranchAddress("V_pz",&vpz);
	cflip->SetBranchAddress("V_pid",&vpid);
	cflip->SetBranchAddress("V_tid",&vtid);
	cflip->SetBranchAddress("V_volName",&vvolname);
	for ( int i = 0; i< cflip->GetEntries(); i++){
		cflip->GetEntry(i);
		tids->clear();
		bool found = false;
		for ( int k = 0; k<vtid->size(); k++){
			if ((*vvolname)[k]!="TS1") continue;
			//std::cout<<(*vvolname)[k]<<":"<<(*vtid)[k]<<std::endl;
			for ( int j  = 0; j<tids.size(); j++){
				if ((*vtid)[k]==tids[j]){
					found = true;
				}
			}
			if (found) continue;
			else{
				tids.push_back((*vtid)[k]);
				double px = (*vpx)[k];
				double py = (*vpy)[k];
				double pz = (*vpz)[k];
				double pa = sqrt(px*px+py*py+pz*pz)*1000;
				if ((*vpid)[k]==13)
					hflip_mu->Fill(pa);
				else if ((*vpid)[k]==-211)
					hflip_pi->Fill(pa);
			}
		}
	}

	gStyle->SetOptStat(0);
	hflip_pi->SetLineColor(kBlue);
	hflip_pi->SetFillColor(kBlue);
	hflip_pi->SetFillStyle(3003);
	hflip_pi->Draw();
//	hflip_pi->SetTitle("Momentum Distribution @ 3 Meter Monitor");
	hflip_pi->SetTitle("Momentum Distribution @ Beginning of 90^{o} Bending");
	hflip_pi->GetXaxis()->SetTitle("p_{tot} [MeV/c]");
	hflip_mu->SetLineColor(kMagenta);
	hflip_mu->SetFillColor(kMagenta);
	hflip_mu->SetFillStyle(3001);
	hflip_mu->Draw("SAME");
	hori_pi->SetLineColor(kBlack);
	hori_pi->SetLineWidth(3);
	hori_pi->Draw("SAME");
	hori_mu->SetLineColor(kRed);
	hori_mu->SetLineWidth(3);
	hori_mu->Draw("SAME");

	TLegend *legend = new TLegend(0.7,0.7,0.9,0.9);
	buf.str("");buf.clear();
	buf<<"#pi^{-} (Flipped Map): "<<hflip_pi.Integral()/scalefactor;
	legend->AddEntry(hflip_pi,buf.str().c_str());
	buf.str("");buf.clear();
	buf<<"#mu^{-} (Flipped Map): "<<hflip_mu.Integral()/scalefactor;
	legend->AddEntry(hflip_mu,buf.str().c_str());
	buf.str("");buf.clear();
	buf<<"#pi^{-} (Original Map): "<<hori_pi.Integral()/scalefactor;
	legend->AddEntry(hori_pi,buf.str().c_str());
	buf.str("");buf.clear();
	buf<<"#mu^{-} (Original Map): "<<hori_mu.Integral()/scalefactor;
	legend->AddEntry(hori_mu,buf.str().c_str());
	legend->Draw("SAME");

}
