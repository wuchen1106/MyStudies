{

	std::vector<int> opt;
	std::vector<int> color;

	opt.push_back(0); color.push_back(kBlack);
//	opt.push_back(50); color.push_back(kRed);
//	opt.push_back(100); color.push_back(kBlue);
//	opt.push_back(150); color.push_back(kOrange);
//	opt.push_back(200); color.push_back(kMagenta);
//	opt.push_back(250); color.push_back(kGreen);
//	opt.push_back(300); color.push_back(kCyan);

	TString runName = "7650";
	std::stringstream buf;
	TChain * c[10];
	TH1D* hmu[10];
	TH1D* hpi[10];

	std::vector<double> * vpx;
	std::vector<double> * vpy;
	std::vector<double> * vpz; 
	std::vector<int> * vtid; 
	std::vector<int> * vpid; 
	std::vector<std::string> * vname; 
//	TCanvas * c1 = new TCanvas("c1","c1");
//	TCanvas * c2 = new TCanvas("c2","c2");
	TLegend *l1 = new TLegend(0.7,0.7,0.9,0.9);
	TLegend *l2 = new TLegend(0.7,0.7,0.9,0.9);

	for (int iopt = 0; iopt<opt.size(); iopt++){
		c[iopt] = new TChain("tree","tree");
		for ( int i = 0; i<10; i++)	{
			buf.str("");buf.clear();
			buf<<"/home/chen/MyWorkArea/Data/PT."<<runName<<"."<<opt[iopt]<<".flip.g496p02QBH/"<<i<<"_job0.raw";
			c[iopt]->Add(buf.str().c_str());
		}
		std::cout<<"c["<<iopt<<"]->GetEntries()="<<c[iopt]->GetEntries()<<std::endl;
		buf.str("");buf.clear();
		buf<<"hmu"<<opt[iopt];
		hmu[iopt]=new TH1D(buf.str().c_str(),buf.str().c_str(),32,0,300);
		buf.str("");buf.clear();
		buf<<"hpi"<<opt[iopt];
		hpi[iopt]=new TH1D(buf.str().c_str(),buf.str().c_str(),32,0,500);
		hmu[iopt]->SetLineColor(color[iopt]);
		hpi[iopt]->SetLineColor(color[iopt]);
		c[iopt]->SetBranchAddress("V_px",&vpx);
		c[iopt]->SetBranchAddress("V_py",&vpy);
		c[iopt]->SetBranchAddress("V_pz",&vpz);
		c[iopt]->SetBranchAddress("V_pid",&vpid);
		c[iopt]->SetBranchAddress("V_tid",&vtid);
		c[iopt]->SetBranchAddress("V_volName",&vname);
		int pretid == -1;
		for ( int i = 0; i<c[iopt]->GetEntries();i++){
			c[iopt]->GetEntry(i);
			for(int j = 0; j<vtid->size(); j++){
				if ((*vname)[j]=="TS1"){
//				if ((*vname)[j]=="CS"){
					if ((*vtid)[j]!=pretid){
						if ((*vpid)[j]==13){
							hmu[iopt]->Fill(sqrt((*vpx)[j]*(*vpx)[j]+(*vpy)[j]*(*vpy)[j]+(*vpz)[j]*(*vpz)[j])*1000);
						}
						else if ((*vpid)[j]==-211){
							hpi[iopt]->Fill(sqrt((*vpx)[j]*(*vpx)[j]+(*vpy)[j]*(*vpy)[j]+(*vpz)[j]*(*vpz)[j])*1000);
						}
						pretid=(*vtid)[j];
					}
				}
			}
		}
		if (iopt==0){
			c1->cd();
			hmu[iopt]->SetTitle("#mu^{-} Spectrum @ The Beginning of The 90^{o} Bending Solenoid");
			hmu[iopt]->GetXaxis()->SetTitle("p_{tot} [MeV/c]");
			hmu[iopt]->Draw("LP");
			c2->cd();
			hpi[iopt]->SetTitle("#pi^{-} Spectrum @ The Beginning of The 90^{o} Bending Solenoid");
			hpi[iopt]->GetXaxis()->SetTitle("p_{tot} [MeV/c]");
			hpi[iopt]->Draw("LP");
		}
		else{
			c1->cd();
			hmu[iopt]->Draw("LPSAME");
			c2->cd();
			hpi[iopt]->Draw("LPSAME");
		}
		buf.str("");buf.clear();
		buf<<"d = "<<opt[iopt]<<": "<<hmu[iopt]->Integral()/0.9;
		l1->AddEntry(hmu[iopt],buf.str().c_str());
		buf.str("");buf.clear();
		buf<<"d = "<<opt[iopt]<<": "<<hpi[iopt]->Integral()/0.9;
		l2->AddEntry(hpi[iopt],buf.str().c_str());
		if (iopt==opt.size()-1){
			c1->cd(); l1->Draw("SAME");
			c2->cd(); l2->Draw("SAME");
		}
	}
}
