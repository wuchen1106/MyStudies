{
//	TFile *_file0 = TFile::Open("CDC.EP_OT.em.g40cm10mm.005T.Vac0731_200_200_39.g4s.QBH.root");
//	TTree *t = (TTree*) _file0->Get("t");
//	TH1D *h1 = new TH1D("pa","pa",40,10,50);
//	double px;
//	double py;
//	double pz;
//	double weight;
//	t->SetBranchAddress("px",&px);
//	t->SetBranchAddress("py",&py);
//	t->SetBranchAddress("pz",&pz);
//	t->SetBranchAddress("weight",&weight);
//	int n = t->GetEntries();
//	for (int i = 0; i<n; i++){
//		t->GetEntry(i);
//		h1->Fill(sqrt(px*px+py*py+pz*pz),weight);
//	}
//	double N = h1->Integral();
//	std::cout<<"N = "<<N<<std::endl;

	TF1 * f1 = new TF1("exp","exp([0]-x*[1])",100,200);
	f1->SetParameters(8.29639e+00,1.90335e-01);
	h1 = (TH1D*) f1->GetHistogram();
	h1->SetName("pa");
	TFile * f = new TFile("output.root","RECREATE");
	h1->Write();
}
