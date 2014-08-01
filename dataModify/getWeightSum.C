{
	TChain *c = new TChain("t");
//	c->Add("pim.2p4.17.root");
//	c->Add("pim.1p5.17.root");
	c->Add("MT1.pim.pim.g60cm10mm.005T.g4s.QBH.root");
//	c->Add("MT1.pim.pim.g40cm10mm.005T.g4s.QBH.root");
	double weight;
	c->SetBranchAddress("weight",&weight);
	int nEvents = c->GetEntries();
	std::cout<<"nEvents = "<<nEvents<<std::endl;
	double count = 0;
	for (int iEvent = 0; iEvent < nEvents; iEvent++){
		c->GetEntry(iEvent);
		count+=weight;
	}
	std::cout<<"count = "<<count<<std::endl;
}
