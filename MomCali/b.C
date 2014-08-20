{
	TChain *tree = new TChain("tree");
	tree->Add("/home/chen/MyWorkArea/Simulate/comet/output/g2e.root");

	double Me2 = 0.000510999*0.000510999;
	double Me = 0.000510999;
	int evt_num;
	int r0,r1;
	double px,py,pz;
	double px1t,py1t,pz1t,pa1t,pt1t,de1t,dp1t,e1t;
	double px2t,py2t,pz2t,pa2t,pt2t,de2t,dp2t,e2t;
	double px1,py1,pz1,pa1,pt1,de1,dp1,e1;
	double px2,py2,pz2,pa2,pt2,de2,dp2,e2;
	int dn1,dn2;
	int nc1,nc2;
	int n1l,n1s;
	int n2l,n2s;
	double m,m0,e,p,dp,dp0;
	std::string *vol;
	TFile * f  =new TFile("output.root","RECREATE");
	TTree* t = new TTree("t","t");
	t->Branch("R0",&r0);
	t->Branch("R1",&r1);
	t->Branch("px",&px);
	t->Branch("py",&py);
	t->Branch("pz",&pz);
	t->Branch("pa1",&pa1);
	t->Branch("pt1",&pt1);
	t->Branch("de1",&de1);
	t->Branch("dp1",&dp1);
	t->Branch("pa2",&pa2);
	t->Branch("pt2",&pt2);
	t->Branch("de2",&de2);
	t->Branch("dp2",&dp2);
	t->Branch("m",&m);
	t->Branch("m0",&m0);
	t->Branch("dp",&dp);
	t->Branch("dp0",&dp0);
	t->Branch("nl1",&dn1);
	t->Branch("n1",&nc1);
	t->Branch("nl2",&dn2);
	t->Branch("n2",&nc2);
	t->Branch("vol",&vol);
	t->Branch("evt_num",&evt_num);

	int cellNoIntegral[18] = {198,402,612,828,1050,1278,1512,1752,1998,2250,2508,2772,3042,3318,3600,3888,4182,4482};

	std::vector<double> *mc_e;
	std::vector<double> *mc_px;
	std::vector<double> *mc_py;
	std::vector<double> *mc_pz;
	std::vector<int> *c_tid;
	std::vector<int> *mc_tid;
	std::vector<int> *mc_ptid;
	std::vector<int> *mc_pid;
	std::vector<int> *c_vid;
	std::vector<double> *c_px;
	std::vector<double> *c_py;
	std::vector<double> *c_pz;
	std::vector<int> *c_ptid;
	std::vector<std::string> *c_vol;

	tree->SetBranchAddress("evt_num",&evt_num);
	tree->SetBranchAddress("McTruth_e",&mc_e);
	tree->SetBranchAddress("McTruth_px",&mc_px);
	tree->SetBranchAddress("McTruth_py",&mc_py);
	tree->SetBranchAddress("McTruth_pz",&mc_pz);
	tree->SetBranchAddress("McTruth_tid",&mc_tid);
	tree->SetBranchAddress("McTruth_ptid",&mc_ptid);
	tree->SetBranchAddress("McTruth_pid",&mc_pid);
	tree->SetBranchAddress("R0",&r0);
	tree->SetBranchAddress("R1",&r1);
	tree->SetBranchAddress("C_volID",&c_vid);
	tree->SetBranchAddress("C_px",&c_px);
	tree->SetBranchAddress("C_py",&c_py);
	tree->SetBranchAddress("C_pz",&c_pz);
	tree->SetBranchAddress("C_ptid",&c_ptid);
	tree->SetBranchAddress("C_tid",&c_tid);
	tree->SetBranchAddress("C_ovolName",&c_vol);

	TLorentzVector v;

	for (int i = 0;i<tree->GetEntries();i++){
		tree->GetEntry(i);
		int found = 0;
		int pretid = 0;
		int tid1 = 0;
		int tid2 = 0;
		n1l=-1;
		n1s=999;
		n2l=-1;
		n2s=999;
		nc1=0;
		nc2=0;

		int g1_tid = 0;
		int g2_tid = 0;
		int g1_found = 0;
		int g2_found = 0;
		for(int j = 0; j< mc_tid->size(); j++){
			if((*mc_ptid)[j]==1&&(*mc_pid)[j]==22){
				if (g1_tid==0)
					g1_tid = (*mc_tid)[j];
				else
					g2_tid = (*mc_tid)[j];
			}
			if (g1_tid&&g2_tid){
				for(int k = 0;k<c_vid->size();k++){
					if((*c_ptid)[k]!=g1_tid&&(*c_ptid)[k]!=g2_tid) continue;
					if((*c_tid)[k]!=pretid){
						if ((*c_ptid)[k]!=g1_tid)
							g1_found++;
						else
							g2_found++;
						px=(*c_px)[k];
						py=(*c_py)[k];
						pz=(*c_pz)[k];
						v+=TLorentzVector(px,py,pz,Me);
					}
				}
			}
		}
		if(g1_found==2&&g2_found==2){
			m=v.M();
			t->Fill();
		}
	}
	t->Write();
}

