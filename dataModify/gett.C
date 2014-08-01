{
	TFile *_file0 = TFile::Open("CDC.EP_OT.em.g40cm10mm.005T.Vac0731_200_200_39.g4s.QBH.root");
	TTree *t = (TTree*) _file0->Get("t");
	double px;
	double py;
	double pz;
	double time;
	double weight;
	t->SetBranchAddress("px",&px);
	t->SetBranchAddress("py",&py);
	t->SetBranchAddress("pz",&pz);
	t->SetBranchAddress("t",&time);
	t->SetBranchAddress("weight",&weight);
	TH1D *h1 = new TH1D("Time","Time",1751,0,1751);
	double count = 0;
	int n = t->GetEntries();
	for (int i = 0; i<n; i++){
		t->GetEntry(i);
		if (sqrt(px*px+py*py+pz*pz)<20) continue;
		h1->Fill(time,weight);
		count += weight;
	}
	std::cout<<"n = "<<n<<std::endl;
	std::cout<<"count = "<<count<<std::endl;

//	TF1 * f1 = new TF1("exp","exp([0]-x*[1])",0,20000);
//	f1->SetParameters(1.16916e+01,8.00535e-02); // mum
//	//f1->SetParameters(1.50261e+01,1.75265e-01); // em
//	//f1->SetParameters(5.38545e+00,6.90385e-02); // pim
//	TH1D *h1 = new TH1D("Time","Time",1751,0,1751);
//	TFile * f = new TFile("/home/chen/MyWorkArea/MyStudies/Beam/result/TimeWindow/Curves.s100.root");
//	TH1D *hCurve = (TH1D*) f->Get("ProtonPuls");
//	int n = 1751*1;
//	for (int i = 0; i<n; i++){
//		if (i%1==0) std::cout<<((double)i)/n*100<<"%..."<<std::endl;
//		double weight = f1->Eval(i);
//        for (int i_window = 0; i_window <1; i_window++){
//            for (int ibin = 1; ibin <= 1751; ibin++){
//                double timeDown = h1->GetBinLowEdge(ibin);
//                double timeUp = h1->GetBinLowEdge(ibin+1);
//                double timep = (i_window-0)*1751+i;
//                int binDown = hCurve->FindBin(timeDown-timep);
//                int binUp = hCurve->FindBin(timeUp-timep)-1;
//                double w = hCurve->Integral(binDown,binUp);
//                h1->AddBinContent(ibin,w*weight);
//            }
//        }
//	}

	TFile * fo = new TFile("output.root","RECREATE");
	h1->Write();
	h1->Draw();
}
