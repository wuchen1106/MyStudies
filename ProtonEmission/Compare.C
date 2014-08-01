void Compare(){
	std::vector<double> ve;
	std::vector<double> vc1;
	std::vector<double> vce1;
	std::vector<double> vc2;
	std::vector<double> vce3;
	std::vector<double> vc3;
	std::vector<double> vce4;
	std::vector<double> vc4;
	std::vector<double> vce5;
	std::vector<double> vc5;

	TF1* f2 = new TF1("f2","[0]*pow(1-[1]/x,[2])*exp(-x/[3])",1.5,26);
	f2->SetParameter(0, 0.105); // A (MeV-1)
	f2->SetParameter(1, 1.4); // T_th (MeV)
	f2->SetParameter(2,1.3279); // alpha
	f2->SetParameter(3,3.1); // T_0

	TFile * f = TFile::Open("sobottka_all.root");
	TTree * t = (TTree*) f->Get("t");

	// read from experiment and theory
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
		vc3.push_back(0);
		vc4.push_back(0);
		vc5.push_back(0);
		C1+=c1;
		C2+=f2->Eval(e);
	}
	for(int i = 0; i<vc1.size(); i++){
		vc1[i] /= C1;
		vce1[i] /= C1;
		vc2[i] /= C2;
	}

	// read from MC
	f = TFile::Open("/home/chen/MyWorkArea/Simulate/comet/output/AlCap.1um.1.G496P02.root");
	t = (TTree*) f->Get("tree");

	std::vector<double> * MC_ekin;
	std::vector<int> * MC_pid;
	std::vector<int> * MC_ptid;
	t->SetBranchAddress("McTruth_pid",&MC_pid);
	t->SetBranchAddress("McTruth_ptid",&MC_ptid);
	t->SetBranchAddress("McTruth_ekin",&MC_ekin);

	int C3 = 0;
	std::cout<<"Got "<<t->GetEntries()<<" entries!"<<std::endl;
	for (int i = 0; i<t->GetEntries(); i++){
		t->GetEntry(i);
//		std::cout<<i<<": "<<MC_pid->size()<<std::endl;
		for(int iMc = 0; iMc<MC_pid->size(); iMc++){
			int pid = (*MC_pid)[iMc];
			int ptid = (*MC_ptid)[iMc];
			if (pid==2212&&ptid==1){
//				std::cout<<"Found it!"<<std::endl;
				double ekin = (*MC_ekin)[iMc]*1000;
				int ibin = 0;
//				std::cout<<"ekin = "<<ekin<<", ve:["<<ve[0]<<","<<ve[ve.size()-1]<<"]"<<std::endl;
				if (ekin<(ve[0]-(ve[1]-ve[0])/2)) continue;
				for(; ibin<ve.size()-1; ibin++){
					if (ekin<(ve[ibin]+ve[ibin+1])/2) break;
				}
				if (ibin == ve.size()-1)
					if (ekin>(ve[ve.size()]+(ve[ve.size()-1]-ve[ve.size()-2])/2)) continue;
				vc3[ibin]++;
				C3++;
			}
		}
	}
	std::cout<<"C3 = "<<C3<<std::endl;
	for(int i = 0; i<vc3.size(); i++){
		vce3.push_back(sqrt(vc3[i]));
		vc3[i] /= C3;
		vce3[i] /= C3;
	}

	f = TFile::Open("/home/chen/MyWorkArea/Simulate/comet/output/AlCap.1um.7em1.G4100P01.root");
	t = (TTree*) f->Get("tree");

	std::vector<double> * MC_ekin;
	std::vector<int> * MC_pid;
	std::vector<int> * MC_ptid;
	t->SetBranchAddress("McTruth_pid",&MC_pid);
	t->SetBranchAddress("McTruth_ptid",&MC_ptid);
	t->SetBranchAddress("McTruth_ekin",&MC_ekin);

	double C4;
	std::cout<<"Got "<<t->GetEntries()<<" entries!"<<std::endl;
	for (int i = 0; i<t->GetEntries(); i++){
		t->GetEntry(i);
		for(int iMc = 0; iMc<MC_pid->size(); iMc++){
			int pid = (*MC_pid)[iMc];
			int ptid = (*MC_ptid)[iMc];
			if (pid==2212&&ptid==1){
				double ekin = (*MC_ekin)[iMc]*1000;
				int ibin = 0;
				if (ekin<(ve[0]-(ve[1]-ve[0])/2)) continue;
				for(; ibin<ve.size()-1; ibin++){
					if (ekin<(ve[ibin]+ve[ibin+1])/2) break;
				}
				if (ibin == ve.size()-1)
					if (ekin>(ve[ve.size()]+(ve[ve.size()-1]-ve[ve.size()-2])/2)) continue;
				vc4[ibin]++;
				C4++;
			}
		}
	}
	std::cout<<"C4 = "<<C4<<std::endl;
	for(int i = 0; i<vc4.size(); i++){
		vce4.push_back(sqrt(vc4[i]));
		vc4[i] /= C4;
		vce4[i] /= C4;
	}

	/*
	f = TFile::Open("/home/chen/MyWorkArea/Simulate/comet/output/AlCap.1um.1.G4100P02.root");
	t = (TTree*) f->Get("tree");

	std::vector<double> * MC_ekin;
	std::vector<int> * MC_pid;
	std::vector<int> * MC_ptid;
	t->SetBranchAddress("McTruth_pid",&MC_pid);
	t->SetBranchAddress("McTruth_ptid",&MC_ptid);
	t->SetBranchAddress("McTruth_ekin",&MC_ekin);

	double C5;
	std::cout<<"Got "<<t->GetEntries()<<" entries!"<<std::endl;
	for (int i = 0; i<t->GetEntries(); i++){
		t->GetEntry(i);
		for(int iMc = 0; iMc<MC_pid->size(); iMc++){
			int pid = (*MC_pid)[iMc];
			int ptid = (*MC_ptid)[iMc];
			if (pid==2212&&ptid==1){
				double ekin = (*MC_ekin)[iMc]*1000;
				int ibin = 0;
				if (ekin<(ve[0]-(ve[1]-ve[0])/2)) continue;
				for(; ibin<ve.size()-1; ibin++){
					if (ekin<(ve[ibin]+ve[ibin+1])/2) break;
				}
				if (ibin == ve.size()-1)
					if (ekin>(ve[ve.size()]+(ve[ve.size()-1]-ve[ve.size()-2])/2)) continue;
				vc5[ibin]++;
				C5++;
			}
		}
	}
	std::cout<<"C5 = "<<C5<<std::endl;
	for(int i = 0; i<vc5.size(); i++){
		vce5.push_back(sqrt(vc5[i]));
		vc5[i] /= C5;
		vce5[i] /= C5;
	}
	*/

	TGraphErrors * g1 = new TGraphErrors(ve.size(),&(ve[0]),&(vc1[0]),0,&(vce1[0]));
	TGraph * g2 = new TGraph(ve.size(),&(ve[0]),&(vc2[0]));
	TGraphErrors * g3 = new TGraphErrors(ve.size(),&(ve[0]),&(vc3[0]),0,&(vce3[0]));
	TGraphErrors * g4 = new TGraphErrors(ve.size(),&(ve[0]),&(vc4[0]),0,&(vce4[0]));
//	TGraphErrors * g5 = new TGraphErrors(ve.size(),&(ve[0]),&(vc5[0]),0,&(vce5[0]));
	g1->Draw("AP");
	g2->Draw("SAME");
	g3->Draw("SAME");
	g4->Draw("SAME");
//	g5->Draw("SAME");
}
