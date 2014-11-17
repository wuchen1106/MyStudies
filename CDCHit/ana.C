{
	TFile * f = new TFile("signal.140905M02.noise.root");
	TTree * t = (TTree*) f->Get("tree");
	std::vector<double> * CdcCell_px;
	std::vector<double> * CdcCell_py;
	std::vector<double> * CdcCell_pz;
	std::vector<int> * CdcCell_hittype;
	t->SetBranchAddress("CdcCell_px",&CdcCell_px);
	t->SetBranchAddress("CdcCell_py",&CdcCell_py);
	t->SetBranchAddress("CdcCell_pz",&CdcCell_pz);
	t->SetBranchAddress("CdcCell_hittype",&CdcCell_hittype);

	TFile * of = new TFile("output.root","RECREATE");
	TTree * ot = new TTree("t","t");
	double pa,px,py,pz,pamax;
	int imax, jmax, jsig, ientry;
	ot->Branch("px",&px);
	ot->Branch("py",&py);
	ot->Branch("pz",&pz);
	ot->Branch("ihit",&jmax);
	ot->Branch("i",&ientry);
	for (int i = 0; i<t->GetEntries(); i++){
		t->GetEntry(i);
		pamax = 0;
		imax = -1;
		jmax = -1;
		jsig = 0;
		ientry = i;
//		std::cout<<i<<": "<<CdcCell_hittype->size()<<" !"<<std::endl;
		for (int j = 0; j<CdcCell_hittype->size(); j++){
			if ((*CdcCell_hittype)[j]==0){
				px = (*CdcCell_px)[j];
				py = (*CdcCell_py)[j];
				pz = (*CdcCell_pz)[j];
				pa = sqrt(px*px+py*py+pz*pz);
				jsig++;
				if (pamax<pa){
					pamax=pa;
					imax = j;
					jmax = jsig;
				}
			}
		}
		if (imax>=0){
			px = (*CdcCell_px)[imax];
			py = (*CdcCell_py)[imax];
			pz = (*CdcCell_pz)[imax];
			ot->Fill();
		}
	}
	ot->Write();
}
