void filter(){
	TChain *c = new TChain("t");
	c->Add("MT1.EP.em.g60cm10mm.005T.g4s.QBH.root");
	c->Add("MT1.EP.mum.g60cm10mm.005T.g4s.QBH.root");
	c->Add("MT1.EP.OT.g60cm10mm.005T.g4s.QBH.root");
//	c->Add("MT1.pim.pim.g60cm10mm.005T.g4s.QBH.root");
	int evt_num;
	int run_num;
	int pid;
	int tid;
	int ppid;
	double x;
	double y;
	double z;
	double px;
	double py;
	double pz;
	double t;
	double ot;
	double ox;
	double oy;
	double oz;
	double opx;
	double opy;
	double opz;
	std::string * particle;
	std::string * process;
	std::string * volume;
	double weight;
	c->SetBranchAddress("evt_num",&evt_num);
	c->SetBranchAddress("run_num",&run_num);
	c->SetBranchAddress("pid",&pid);
	c->SetBranchAddress("tid",&tid);
	c->SetBranchAddress("ppid",&ppid);
	c->SetBranchAddress("x",&x);
	c->SetBranchAddress("y",&y);
	c->SetBranchAddress("z",&z);
	c->SetBranchAddress("px",&px);
	c->SetBranchAddress("py",&py);
	c->SetBranchAddress("pz",&pz);
	c->SetBranchAddress("t",&t);
	c->SetBranchAddress("ot",&ot);
	c->SetBranchAddress("ox",&ox);
	c->SetBranchAddress("oy",&oy);
	c->SetBranchAddress("oz",&oz);
	c->SetBranchAddress("opx",&opx);
	c->SetBranchAddress("opy",&opy);
	c->SetBranchAddress("opz",&opz);
	c->SetBranchAddress("particle",&particle);
	c->SetBranchAddress("process",&process);
	c->SetBranchAddress("volume",&volume);
	c->SetBranchAddress("weight",&weight);

	TFile *f = new TFile("output.root","RECREATE");
	TTree *t_tree = new TTree("t","t");

	t_tree->Branch("evt_num",&evt_num);
	t_tree->Branch("run_num",&run_num);
	t_tree->Branch("pid",&pid);
	t_tree->Branch("tid",&tid);
	t_tree->Branch("ppid",&ppid);
	t_tree->Branch("x",&x);
	t_tree->Branch("y",&y);
	t_tree->Branch("z",&z);
	t_tree->Branch("px",&px);
	t_tree->Branch("py",&py);
	t_tree->Branch("pz",&pz);
	t_tree->Branch("t",&t);
	t_tree->Branch("ot",&ot);
	t_tree->Branch("ox",&ox);
	t_tree->Branch("oy",&oy);
	t_tree->Branch("oz",&oz);
	t_tree->Branch("opx",&opx);
	t_tree->Branch("opy",&opy);
	t_tree->Branch("opz",&opz);
	t_tree->Branch("particle",&particle);
	t_tree->Branch("process",&process);
	t_tree->Branch("volume",&volume);
	t_tree->Branch("weight",&weight);

	int nEvents = c->GetEntries();
	std::cout<<"nEvents = "<<nEvents<<std::endl;
	for (int iEvent = 0; iEvent < nEvents; iEvent++){
		if (iEvent%1000==0) std::cout<<(double)iEvent/nEvents*100<<" % ..."<<std::endl;
		c->GetEntry(iEvent);
		//>>>>>>>>>>>>>>>>>>>
		//Modify here
		double pa = sqrt(px*px+py*py+pz*pz);
		if (pid==22||fabs(pid)==11){
			if (pa<95) continue;
		}
		else if(fabs(pid)==13){
			if (pa<75) continue;
		}
		else if(fabs(pid)==2112){
			continue;
		}
		//<<<<<<<<<<<<<<<<<<<
		t_tree->Fill();
	}
	t_tree->Write();
}
/*
evt_num   
run_num   
pid       
tid       
ppid      
x         
y         
z         
px        
py        
pz        
t         
ot        
ox        
oy        
oz        
opx       
opy       
opz       
particle  
process   
volume    
weight    
*/
