void Compare(){
	std::vector<double> ve;
	std::vector<double> vc1;
	std::vector<double> vce1;
	std::vector<double> vc2;

	TF1* f2 = new TF1("f2","[0]*pow(1-[1]/x,[2])*exp(-x/[3])",1.5,26);
	f2->SetParameter(0, 0.105); // A (MeV-1)
	f2->SetParameter(1, 1.4); // T_th (MeV)
	f2->SetParameter(2,1.3279); // alpha
	f2->SetParameter(3,3.1); // T_0

	TFile * f = TFile::Open("sobottka_all.root");
	TTree * t = (TTree*) f->Get("t");

	double e,c1;
	t->SetBranchAddress("e",&e);
	t->SetBranchAddress("c",&c1);

	double C1,C2;
	for (int i = 0; i<t->GetEntries(); i++){
		t->GetEntry(i);
		if (e<=1.4) continue;
		ve.push_back(e);
		vc1.push_back(c1);
		vce1.push_back(sqrt(c1));
		vc2.push_back(f2->Eval(e));
		C1+=c1;
		C2+=f2->Eval(e);
	}
	for(int i = 0; i<vc1.size(); i++){
		vc1[i] /= C1;
		vce1[i] /= C1;
		vc2[i] /= C2;
	}

	TGraphErrors * g1 = new TGraphErrors(ve.size(),&(ve[0]),&(vc1[0]),0,&(vce1[0]));
	TGraph * g2 = new TGraph(ve.size(),&(ve[0]),&(vc2[0]));
	g1->Draw("LAP");
	g2->Draw("SAME");
}
