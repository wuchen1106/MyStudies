{
	TChain *tree = new TChain("tree");
	tree->Add("/home/chen/MyWorkArea/Simulate/comet/output/g2e.root");

	double Me2 = 0.000510999*0.000510999;
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
	tree->SetBranchAddress("R0",&r0);
	tree->SetBranchAddress("R1",&r1);
	tree->SetBranchAddress("C_volID",&c_vid);
	tree->SetBranchAddress("C_px",&c_px);
	tree->SetBranchAddress("C_py",&c_py);
	tree->SetBranchAddress("C_pz",&c_pz);
	tree->SetBranchAddress("C_ptid",&c_ptid);
	tree->SetBranchAddress("C_tid",&c_tid);
	tree->SetBranchAddress("C_ovolName",&c_vol);

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
		for(int j = 0;j<c_vid->size();j++){
			if((*c_ptid)[j]!=1) continue;
			if((*c_tid)[j]!=pretid){
				found++;
				pretid=(*c_tid)[j];
				vol=&((*c_vol)[j]);
				if(found==1){
					px1=(*c_px)[j];
					py1=(*c_py)[j];
					pz1=(*c_pz)[j];
					tid1=(*c_tid)[j];
				}
				else if(found ==2){
					px2=(*c_px)[j];
					py2=(*c_py)[j];
					pz2=(*c_pz)[j];
					tid2=(*c_tid)[j];
				}
			}
			int nt=0;
			for(;nt<18;nt++){
				if ((*c_vid)[j]<cellNoIntegral[nt]) break;
			}
			if(found==1) {
				if(n1l<nt) n1l=nt;
				if(n1s>nt) n1s=nt;
				nc1++;
			}
			if(found==2) {
				if(n2l<nt) n2l=nt;
				if(n2s>nt) n2s=nt;
				nc2++;
			}
		}
		if(found==2){
			dn1=n1l-n1s;
			dn2=n2l-n2s;
			px=(*mc_px)[0];
			py=(*mc_py)[0];
			pz=(*mc_pz)[0];
			pt1=sqrt(px1*px1+py1*py1);
			pa1=sqrt(pz1*pz1+pt1*pt1);
			pt2=sqrt(px2*px2+py2*py2);
			pa2=sqrt(pz2*pz2+pt2*pt2);
			e1=sqrt(pa1*pa1+Me2);
			e2=sqrt(pa2*pa2+Me2);
			e=e1+e2;
			m=sqrt(e*e-(px1+px2)*(px1+px2)-(py1+py2)*(py1+py2)-(pz1+pz2)*(pz1+pz2));
			dp=sqrt((px1+px2-px)*(px1+px2-px)+(py1+py2-py)*(py1+py2-py)+(pz1+pz2-pz)*(pz1+pz2-pz));
			for(int j = 0; j< mc_tid->size(); j++){
				if((*mc_tid)[j]==tid1){
					px1t=(*mc_px)[j];
					py1t=(*mc_py)[j];
					pz1t=(*mc_pz)[j];
					pa1t=sqrt(px1t*px1t+py1t*py1t+pz1t*pz1t);
					e1t=sqrt(pa1t*pa1t+Me2);
					de1=e1t-e1;
					dp1=sqrt((px1t-px1)*(px1t-px1)+(py1t-py1)*(py1t-py1)+(pz1t-pz1)*(pz1t-pz1));
				}
				else if((*mc_tid)[j]==tid2){
					px2t=(*mc_px)[j];
					py2t=(*mc_py)[j];
					pz2t=(*mc_pz)[j];
					pa2t=sqrt(px2t*px2t+py2t*py2t+pz2t*pz2t);
					e2t=sqrt(pa2t*pa2t+Me2);
					de2=e2t-e2;
					dp2=sqrt((px2t-px2)*(px2t-px2)+(py2t-py2)*(py2t-py2)+(pz2t-pz2)*(pz2t-pz2));
				}
			}
			e=e1t+e2t;
			m0=sqrt(e*e-(px1t+px2t)*(px1t+px2t)-(py1t+py2t)*(py1t+py2t)-(pz1t+pz2t)*(pz1t+pz2t));
			dp0=sqrt((px1t+px2t-px)*(px1t+px2t-px)+(py1t+py2t-py)*(py1t+py2t-py)+(pz1t+pz2t-pz)*(pz1t+pz2t-pz));
			px*=1000.;
			py*=1000.;
			pz*=1000.;
			t->Fill();
		}
	}
	t->Write();
}

