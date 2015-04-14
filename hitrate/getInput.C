{
//	TFile * f = new TFile("result/ALL.140625M01.root");
	TFile * f = new TFile("result/OTWC.0508_100cm_1e9.root");
	TTree * t = (TTree*) f->Get("tree");

	int evt_num = 0;
	int run_num = 0;
	std::vector<double> * O_x = 0;
	std::vector<double> * O_y = 0;
	std::vector<double> * O_z = 0;
	std::vector<double> * O_px = 0;
	std::vector<double> * O_py = 0;
	std::vector<double> * O_pz = 0;
	std::vector<double> * O_t = 0;
	std::vector<double> * o_x = 0;
	std::vector<double> * o_y = 0;
	std::vector<double> * o_z = 0;
	std::vector<double> * o_px = 0;
	std::vector<double> * o_py = 0;
	std::vector<double> * o_pz = 0;
	std::vector<double> * o_t = 0;
	std::vector<int> * o_pid = 0;
	std::vector<int> * o_dep = 0;
	int type = -1;
	int topo = -1;
	int cpid = -1;
	t->SetBranchAddress("evt_num",&evt_num);
	t->SetBranchAddress("run_num",&run_num);
	t->SetBranchAddress("O_t",&O_t);
	t->SetBranchAddress("O_x",&O_x);
	t->SetBranchAddress("O_y",&O_y);
	t->SetBranchAddress("O_z",&O_z);
	t->SetBranchAddress("O_px",&O_px);
	t->SetBranchAddress("O_py",&O_py);
	t->SetBranchAddress("O_pz",&O_pz);
	t->SetBranchAddress("cpid",&cpid);
	t->SetBranchAddress("t",&o_t);
	t->SetBranchAddress("x",&o_x);
	t->SetBranchAddress("y",&o_y);
	t->SetBranchAddress("z",&o_z);
	t->SetBranchAddress("px",&o_px);
	t->SetBranchAddress("py",&o_py);
	t->SetBranchAddress("pz",&o_pz);
	t->SetBranchAddress("pid",&o_pid);
	t->SetBranchAddress("type",&type);
	t->SetBranchAddress("topo",&topo);
	t->SetBranchAddress("dep",&o_dep);

	TFile * of = new TFile("output.root","RECREATE");
	TTree * ot = new TTree("t","t");
	double x = 0;
	double y = 0;
	double z = 0;
	double px = 0;
	double py = 0;
	double pz = 0;
	int pid = 0;
	int spid = 0;
	int fpid = 0;
	double time = 0;
	ot->Branch("evt_num",&evt_num);
	ot->Branch("run_num",&run_num);
	ot->Branch("t",&time);
	ot->Branch("x",&x);
	ot->Branch("y",&y);
	ot->Branch("z",&z);
	ot->Branch("px",&px);
	ot->Branch("py",&py);
	ot->Branch("pz",&pz);
	ot->Branch("pid",&pid);
	ot->Branch("spid",&spid);
	ot->Branch("fpid",&fpid);

	for (int i = 0; i<t->GetEntries(); i++){
		t->GetEntry(i);

		// get charged track
//		if (type!=0||evt_num!=33864||run_num!=3576) continue;
		if (type!=0||topo!=0) continue;
		time = (*O_t)[0];
		x = (*O_x)[0];
		y = (*O_y)[0];
//		z = (*O_z)[0]+5952.5; // for after 110625
		z = (*O_z)[0]+120.5+5952.5; // for BL
		px = (*O_px)[0];
		py = (*O_py)[0];
		pz = (*O_pz)[0];
		pid = cpid;
		ot->Fill();

		// get parent
	//	if (fabs((*o_pid)[o_pid->size()-1])==-211){
//		if (type==0){
		/*
		if (fabs((*o_pid)[0])==11&&type==0){
			if (o_t->size()==1){
				time = (*o_t)[0];
				x = (*o_x)[0];
				y = (*o_y)[0];
				z = (*o_z)[0];
				px = (*o_px)[0];
				py = (*o_py)[0];
				pz = (*o_pz)[0];
				pid = (*o_pid)[0];
			}
			else{
				time = (*o_t)[1];
				x = (*o_x)[1];
				y = (*o_y)[1];
				z = (*o_z)[1];
//				if (z>=5000) continue;
				px = (*o_px)[1];
				py = (*o_py)[1];
				pz = (*o_pz)[1];
				pid = (*o_pid)[1];
			}
			if (pid==22){
				px /= 100;
				py /= 100;
				pz /= 100;
			}
			spid = (*o_pid)[0];
			fpid = (*o_pid)[o_pid->size()-1];
			ot->Fill();
		}
		*/
	}
	ot->Write();
}
