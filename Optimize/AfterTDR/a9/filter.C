void filter(){

//	int pid = 11;
//	TString par = "em";
	int pid = -11;
	TString par = "ep";
//	int pid = -13;
//	TString par = "mup";
//	int pid = 13;
//	TString par = "mum";
//	int pid = -211;
//	TString par = "pim";

//	TString runName = "a_p3_35";
//	TString runName = "a_p3_35_B_5";
	TString runName = "a_p3_35_B_5.4v";
//	TString runName = "a.4v";

	TChain *c = new TChain("t");
	c->Add("result/"+runName+"."+par+".output.root");
	int R0;
	int R1;
	double x;
	double y;
	double z;
	double t = 0;
	double px;
	double py;
	double pz;
	double cy;
	double cz;
	double weight;
	c->SetBranchAddress("R0",&R0);
	c->SetBranchAddress("R1",&R1);
	c->SetBranchAddress("x",&x);
	c->SetBranchAddress("y",&y);
	c->SetBranchAddress("z",&z);
	c->SetBranchAddress("px",&px);
	c->SetBranchAddress("py",&py);
	c->SetBranchAddress("pz",&pz);
	c->SetBranchAddress("coy",&cy);
	c->SetBranchAddress("coz",&cz);
	c->SetBranchAddress("weight",&weight);

	TFile *f1 = new TFile(runName+"."+par+".BU.root","RECREATE");
	TTree *t_tree1 = new TTree("t","t");
	t_tree1->Branch("R0",&R0);
	t_tree1->Branch("R1",&R1);
	t_tree1->Branch("pid",&pid);
	t_tree1->Branch("x",&x);
	t_tree1->Branch("y",&y);
	t_tree1->Branch("z",&z);
	t_tree1->Branch("px",&px);
	t_tree1->Branch("py",&py);
	t_tree1->Branch("pz",&pz);
	t_tree1->Branch("t",&t);
	t_tree1->Branch("weight",&weight);

	TFile *f2 = new TFile(runName+"."+par+".BD.root","RECREATE");
	TTree *t_tree2 = new TTree("t","t");
	t_tree2->Branch("R0",&R0);
	t_tree2->Branch("R1",&R1);
	t_tree2->Branch("pid",&pid);
	t_tree2->Branch("x",&x);
	t_tree2->Branch("y",&y);
	t_tree2->Branch("z",&z);
	t_tree2->Branch("px",&px);
	t_tree2->Branch("py",&py);
	t_tree2->Branch("pz",&pz);
	t_tree2->Branch("t",&t);
	t_tree2->Branch("weight",&weight);

	TFile *f3 = new TFile(runName+"."+par+".E.root","RECREATE");
	TTree *t_tree3 = new TTree("t","t");
	t_tree3->Branch("R0",&R0);
	t_tree3->Branch("R1",&R1);
	t_tree3->Branch("pid",&pid);
	t_tree3->Branch("x",&x);
	t_tree3->Branch("y",&y);
	t_tree3->Branch("z",&z);
	t_tree3->Branch("px",&px);
	t_tree3->Branch("py",&py);
	t_tree3->Branch("pz",&pz);
	t_tree3->Branch("t",&t);
	t_tree3->Branch("weight",&weight);

	TFile *f4 = new TFile(runName+"."+par+".D.root","RECREATE");
	TTree *t_tree4 = new TTree("t","t");
	t_tree4->Branch("R0",&R0);
	t_tree4->Branch("R1",&R1);
	t_tree4->Branch("pid",&pid);
	t_tree4->Branch("x",&x);
	t_tree4->Branch("y",&y);
	t_tree4->Branch("z",&z);
	t_tree4->Branch("px",&px);
	t_tree4->Branch("py",&py);
	t_tree4->Branch("pz",&pz);
	t_tree4->Branch("t",&t);
	t_tree4->Branch("weight",&weight);

	Long64_t nEvents = c->GetEntries();
	std::cout<<"nEvents = "<<nEvents<<std::endl;
	for (Long64_t iEvent = 0; iEvent < nEvents; iEvent++){
		if (iEvent%1000==0) std::cout<<(double)iEvent/nEvents*100<<" % ..."<<std::endl;
		c->GetEntry(iEvent);

		bool pass = false;

		//BU
		if (cz<-2300){
			if (y>0){
				t_tree1->Fill();
			}
			else{
				t_tree2->Fill();
			}
		}
		else if (cz<-1000){
			t_tree3->Fill();
		}
		else{
			t_tree4->Fill();
		}
	}
	f1->cd();
	t_tree1->Write();
	f2->cd();
	t_tree2->Write();
	f3->cd();
	t_tree3->Write();
	f4->cd();
	t_tree4->Write();
}
