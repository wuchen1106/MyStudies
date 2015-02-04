a(){
	TString MyData = getenv("MYDATA");
	// About this run
	TString runDir = "/home/chen/MyWorkArea/Data/20140710_YangYe_SimPHITS/";
//	TString runName = "AfterBending_BM2";
	TString runName = "BigMonitor_Inner";
	TChain *c = new TChain("RooTracker");
	c->Add(runDir+runName+".root");
	runName = "BigMonitor_Outer";
	c->Add(runDir+runName+".root");

	int pid;
	float iweight = 0;
	float itime = 0;
	float ix = 0;
	float iy = 0;
	float iz = 0;
	float ipx = 0;
	float ipy = 0;
	float ipz = 0;
	float energy = 0;
	double weight = 0;
	double time = 0;
	double x = 0;
	double y = 0;
	double z = 0;
	double px = 0;
	double py = 0;
	double pz = 0;
	c->SetBranchAddress("pdgcode",&pid);
	c->SetBranchAddress("time",&itime);
	c->SetBranchAddress("x",&ix);
	c->SetBranchAddress("y",&iy);
	c->SetBranchAddress("z",&iz);
	c->SetBranchAddress("px",&ipx);
	c->SetBranchAddress("py",&ipy);
	c->SetBranchAddress("pz",&ipz);
	c->SetBranchAddress("energy",&energy);
	c->SetBranchAddress("weight",&iweight);

	f = new TFile(runName+".ini.root","RECREATE");
	TTree *t  = new TTree("t","t");
	t->Branch("pid",&pid);
	t->Branch("t",&time);
	t->Branch("x",&x);
	t->Branch("y",&y);
	t->Branch("z",&z);
	t->Branch("px",&px);
	t->Branch("py",&py);
	t->Branch("pz",&pz);
	t->Branch("weight",&weight);

	int nEvents = c->GetEntries();
	std::cout<<"nEvents = "<<nEvents<<std::endl;
	std::stringstream buff;
	for (int iEvent = 0; iEvent < nEvents; iEvent++ ){
		if (iEvent%1000==0) std::cout<<(double)iEvent/nEvents*100<<" % ..."<<std::endl;
		c->GetEntry(iEvent);
		if (ipx<0){
			double m = 0;
			if (pid!=2112) continue;
			if (fabs(pid)==13) m = 0.1056584*1e3;
			else if (fabs(pid)==2112) m = 0.9395656*1e3;
			else if (fabs(pid)==2212) m = 0.9382723*1e3;
			else if (fabs(pid)==11) m = 0.000510999*1e3;
			else if (fabs(pid)==22) m = 0;
			else if (fabs(pid)==211) m = 0.139570*1e3;
			else continue;
			weight = iweight;
			time = itime;
			x = iz*10+7650;
			y = iy*10;
			z = -ix*10-3000-2795;
			double pa = sqrt((energy+m)*(energy+m)-m*m);
			double p0 = sqrt(ipx*ipx+ipy*ipy+ipz*ipz);
			px = ipz*pa/p0;
			py = ipy*pa/p0;
			pz = -ipx*pa/p0;
			t->Fill();
		}
	}
	t->Write();
}
