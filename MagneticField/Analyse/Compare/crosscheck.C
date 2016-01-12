{
	TChain * c1 = new TChain("t");
	c1->Add("field.chen.root");
	TChain * c2 = new TChain("t");
	c2->Add("field.sakamoto.root");
	double x1, y1, z1, bx1, by1, bz1;
	double x2, y2, z2, bx2, by2, bz2;
	c1->SetBranchAddress("x",&x1);
	c1->SetBranchAddress("y",&y1);
	c1->SetBranchAddress("z",&z1);
	c1->SetBranchAddress("bx",&bx1);
	c1->SetBranchAddress("by",&by1);
	c1->SetBranchAddress("bz",&bz1);
	c2->SetBranchAddress("x",&x2);
	c2->SetBranchAddress("y",&y2);
	c2->SetBranchAddress("z",&z2);
	c2->SetBranchAddress("bx",&bx2);
	c2->SetBranchAddress("by",&by2);
	c2->SetBranchAddress("bz",&bz2);
	TFile * ofile = new TFile("output.root","RECREATE");
	TTree * otree = new TTree("t","t");
	otree->Branch("x1",&x1);
	otree->Branch("y1",&y1);
	otree->Branch("z1",&z1);
	otree->Branch("bx1",&bx1);
	otree->Branch("by1",&by1);
	otree->Branch("bz1",&bz1);
	otree->Branch("x2",&x2);
	otree->Branch("y2",&y2);
	otree->Branch("z2",&z2);
	otree->Branch("bx2",&bx2);
	otree->Branch("by2",&by2);
	otree->Branch("bz2",&bz2);
	for (int i2 = 0; i2<c2->GetEntries(); i2++){
		std::cout<<i2<<".."<<std::endl;
		c2->GetEntry(i2);
		int thei1 = 0;
		double ddmin = 1e9;
		for (int i1 = 0; i1<c1->GetEntries(); i1++){
			c1->GetEntry(i1);
			double dd = sqrt(pow(x1-x2,2)+pow(y1-y2,2)+pow(z1-z2,2));
			if (ddmin>dd){
				ddmin = dd;
				thei1 = i1;
			}
		}
		c1->GetEntry(thei1);
		otree->Fill();
	}
	otree->Write();
	ofile->Close();
}
