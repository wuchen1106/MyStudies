{
	TFile *_file0 = TFile::Open("Numbers.root");
	TTree *t = (TTree*) _file0->Get("t");
	std::vector<double> v1_1x;
	std::vector<double> v1_1y;
	std::vector<double> v1_2x;
	std::vector<double> v1_2y;
	std::vector<double> v1_3x;
	std::vector<double> v1_3y;
	std::vector<double> v1_4x;
	std::vector<double> v1_4y;
	std::vector<double> v1_5x;
	std::vector<double> v1_5y;
	std::vector<double> v1_6x;
	std::vector<double> v1_6y;
	std::vector<double> v1_7x;
	std::vector<double> v1_7y;
	std::vector<double> v2_1x;
	std::vector<double> v2_1y;
	std::vector<double> v2_2x;
	std::vector<double> v2_2y;
	std::vector<double> v2_3x;
	std::vector<double> v2_3y;
	std::vector<double> v2_4x;
	std::vector<double> v2_4y;
	std::vector<double> v2_5x;
	std::vector<double> v2_5y;
	std::vector<double> v2_6x;
	std::vector<double> v2_6y;
	std::vector<double> v2_7x;
	std::vector<double> v2_7y;
	std::vector<double> v3_1x;
	std::vector<double> v3_1y;
	std::vector<double> v3_2x;
	std::vector<double> v3_2y;
	std::vector<double> v3_3x;
	std::vector<double> v3_3y;
	std::vector<double> v3_4x;
	std::vector<double> v3_4y;
	std::vector<double> v3_5x;
	std::vector<double> v3_5y;
	std::vector<double> v3_6x;
	std::vector<double> v3_6y;
	std::vector<double> v3_7x;
	std::vector<double> v3_7y;
	std::vector<double> v4_1x;
	std::vector<double> v4_1y;
	std::vector<double> v4_2x;
	std::vector<double> v4_2y;
	std::vector<double> v4_3x;
	std::vector<double> v4_3y;
	std::vector<double> v4_4x;
	std::vector<double> v4_4y;
	std::vector<double> v4_5x;
	std::vector<double> v4_5y;
	std::vector<double> v4_6x;
	std::vector<double> v4_6y;
	std::vector<double> v4_7x;
	std::vector<double> v4_7y;
	double PrTgtLen;
	double nProton;
	double mum_o75;
	double mum_stop;
	double pim_all;
	double pim_stop;
	double nDisks;
	double DiFd;
	double CoRadi;
	double minY;
	t->SetBranchAddress("PrTgtLen",&PrTgtLen);
	t->SetBranchAddress("nProton",&nProton);
	t->SetBranchAddress("mum_o75",&mum_o75);
	t->SetBranchAddress("mum_stop",&mum_stop);
	t->SetBranchAddress("pim_all",&pim_all);
	t->SetBranchAddress("pim_stop",&pim_stop);
	t->SetBranchAddress("nDisks",&nDisks);
	t->SetBranchAddress("DiFd",&DiFd);
	t->SetBranchAddress("CoRadi",&CoRadi);
	t->SetBranchAddress("minY",&minY);
	int n = t->GetEntries();
	for (int i = 0; i< n; i++){
		t->GetEntry(i);
		//		if (left==820) h3->Fill(right,ratio);
		//		else if (left==810) h2->Fill(right,ratio);
		//		else if (left==800) h1->Fill(right,ratio);
		//		if (left==820) hh3->Fill(right,nMuon);
		//		else if (left==810) hh2->Fill(right,nMuon);
		//		else if (left==800) hh1->Fill(right,nMuon);
		if ((PrTgtLen!=60&&CoRadi!=200)||(PrTgtLen==60&&CoRadi!=130)) continue;
		std::cout<<"mum_stop = "<<mum_stop
			     <<", pim_stop = "<<pim_stop
			     <<", minY = "<<minY
			     <<", DiFd = "<<DiFd
			     <<std::endl;
		if (PrTgtLen==60&&CoRadi==130&&DiFd==5) {
			if (mum_stop==-1) continue;
			if (mum_stop!=-1) v1_7x.push_back(minY);
			if (mum_stop!=-1) v1_7y.push_back(mum_stop/nProton);
			if (mum_o75!=-1) v2_7x.push_back(minY);
			if (mum_o75!=-1) v2_7y.push_back(mum_o75/mum_stop);
			if (pim_stop!=-1) v3_7x.push_back(minY);
			if (pim_stop!=-1) v3_7y.push_back(pim_stop/mum_stop);
			if (pim_all!=-1) v4_7x.push_back(minY);
			if (pim_all!=-1) v4_7y.push_back(pim_all/mum_stop);
		}
		else{
			if (DiFd==3) {
				if (mum_stop==-1) continue;
				if (mum_stop!=-1)
					if (nDisks==39) v1_1x.push_back(minY);
					else (nDisks==17) v1_6x.push_back(minY);
				if (mum_stop!=-1)
					if (nDisks==39) v1_1y.push_back(mum_stop/nProton);
					else (nDisks==17) v1_6y.push_back(mum_stop/nProton);
				if (mum_o75!=-1)
					if (nDisks==39) v2_1x.push_back(minY);
					else if (nDisks==17) v2_6x.push_back(minY);
				if (mum_o75!=-1)
					if (nDisks==39) v2_1y.push_back(mum_o75/mum_stop);
					else if (nDisks==17) v2_6y.push_back(mum_o75/mum_stop);
				if (pim_stop!=-1)
					if (nDisks==39) v3_1x.push_back(minY);
					else if (nDisks==17) v3_6x.push_back(minY);
				if (pim_stop!=-1)
					if (nDisks==39) v3_1y.push_back(pim_stop/mum_stop);
					else if (nDisks==17) v3_6y.push_back(pim_stop/mum_stop);
				if (pim_all!=-1)
					if (nDisks==39) v4_1x.push_back(minY);
					else if (nDisks==17) v4_6x.push_back(minY);
				if (pim_all!=-1)
					if (nDisks==39) v4_1y.push_back(pim_all/mum_stop);
					else if (nDisks==17) v4_6y.push_back(pim_all/mum_stop);
			}
			else if (DiFd==4) {
				if (mum_stop==-1) continue;
				if (mum_stop!=-1) v1_2x.push_back(minY);
				if (mum_stop!=-1) v1_2y.push_back(mum_stop/nProton);
				if (mum_o75!=-1) v2_2x.push_back(minY);
				if (mum_o75!=-1) v2_2y.push_back(mum_o75/mum_stop);
				if (pim_stop!=-1) v3_2x.push_back(minY);
				if (pim_stop!=-1) v3_2y.push_back(pim_stop/mum_stop);
				if (pim_all!=-1) v4_2x.push_back(minY);
				if (pim_all!=-1) v4_2y.push_back(pim_all/mum_stop);
			}
			else if (DiFd==5) {
				if (mum_stop==-1) continue;
				if (mum_stop!=-1) v1_3x.push_back(minY);
				if (mum_stop!=-1) v1_3y.push_back(mum_stop/nProton);
				if (mum_o75!=-1) v2_3x.push_back(minY);
				if (mum_o75!=-1) v2_3y.push_back(mum_o75/mum_stop);
				if (pim_stop!=-1) v3_3x.push_back(minY);
				if (pim_stop!=-1) v3_3y.push_back(pim_stop/mum_stop);
				if (pim_all!=-1) v4_3x.push_back(minY);
				if (pim_all!=-1) v4_3y.push_back(pim_all/mum_stop);
			}
			else if (DiFd==6) {
				if (mum_stop==-1) continue;
				if (mum_stop!=-1) v1_4x.push_back(minY);
				if (mum_stop!=-1) v1_4y.push_back(mum_stop/nProton);
				if (mum_o75!=-1) v2_4x.push_back(minY);
				if (mum_o75!=-1) v2_4y.push_back(mum_o75/mum_stop);
				if (pim_stop!=-1) v3_4x.push_back(minY);
				if (pim_stop!=-1) v3_4y.push_back(pim_stop/mum_stop);
				if (pim_all!=-1) v4_4x.push_back(minY);
				if (pim_all!=-1) v4_4y.push_back(pim_all/mum_stop);
			}
			else if (DiFd==7) {
				if (mum_stop==-1) continue;
				if (mum_stop!=-1) v1_5x.push_back(minY);
				if (mum_stop!=-1) v1_5y.push_back(mum_stop/nProton);
				if (mum_o75!=-1) v2_5x.push_back(minY);
				if (mum_o75!=-1) v2_5y.push_back(mum_o75/mum_stop);
				if (pim_stop!=-1) v3_5x.push_back(minY);
				if (pim_stop!=-1) v3_5y.push_back(pim_stop/mum_stop);
				if (pim_all!=-1) v4_5x.push_back(minY);
				if (pim_all!=-1) v4_5y.push_back(pim_all/mum_stop);
			}
		}
	}

	TGraph *g1_1 = 0; 
	TGraph *g1_2 = 0; 
	TGraph *g1_3 = 0; 
	TGraph *g1_4 = 0; 
	TGraph *g1_5 = 0; 
	TGraph *g1_6 = 0; 
	TGraph *g1_7 = 0; 

	TGraph *g2_1 = 0; 
	TGraph *g2_2 = 0; 
	TGraph *g2_3 = 0; 
	TGraph *g2_4 = 0; 
	TGraph *g2_5 = 0; 
	TGraph *g2_6 = 0; 
	TGraph *g2_7 = 0; 

	TGraph *g3_1 = 0; 
	TGraph *g3_2 = 0; 
	TGraph *g3_3 = 0; 
	TGraph *g3_4 = 0; 
	TGraph *g3_5 = 0; 
	TGraph *g3_6 = 0; 
	TGraph *g3_7 = 0; 

	TGraph *g4_1 = 0; 
	TGraph *g4_2 = 0; 
	TGraph *g4_3 = 0; 
	TGraph *g4_4 = 0; 
	TGraph *g4_5 = 0; 
	TGraph *g4_6 = 0; 
	TGraph *g4_7 = 0; 

	if(v1_1x.size()>0) g1_1 = new TGraph(v1_1x.size(),&v1_1x[0],&v1_1y[0]);
	if(v1_2x.size()>0) g1_2 = new TGraph(v1_2x.size(),&v1_2x[0],&v1_2y[0]);
	if(v1_3x.size()>0) g1_3 = new TGraph(v1_3x.size(),&v1_3x[0],&v1_3y[0]);
	if(v1_4x.size()>0) g1_4 = new TGraph(v1_4x.size(),&v1_4x[0],&v1_4y[0]);
	if(v1_5x.size()>0) g1_5 = new TGraph(v1_5x.size(),&v1_5x[0],&v1_5y[0]);
	if(v1_6x.size()>0) g1_6 = new TGraph(v1_6x.size(),&v1_6x[0],&v1_6y[0]);
	if(v1_7x.size()>0) g1_7 = new TGraph(v1_7x.size(),&v1_7x[0],&v1_7y[0]);

	if(v2_1x.size()>0) g2_1 = new TGraph(v2_1x.size(),&v2_1x[0],&v2_1y[0]);
	if(v2_2x.size()>0) g2_2 = new TGraph(v2_2x.size(),&v2_2x[0],&v2_2y[0]);
	if(v2_3x.size()>0) g2_3 = new TGraph(v2_3x.size(),&v2_3x[0],&v2_3y[0]);
	if(v2_4x.size()>0) g2_4 = new TGraph(v2_4x.size(),&v2_4x[0],&v2_4y[0]);
	if(v2_5x.size()>0) g2_5 = new TGraph(v2_5x.size(),&v2_5x[0],&v2_5y[0]);
	if(v2_6x.size()>0) g2_6 = new TGraph(v2_6x.size(),&v2_6x[0],&v2_6y[0]);
	if(v2_7x.size()>0) g2_7 = new TGraph(v2_7x.size(),&v2_7x[0],&v2_7y[0]);

	if(v3_1x.size()>0) g3_1 = new TGraph(v3_1x.size(),&v3_1x[0],&v3_1y[0]);
	if(v3_2x.size()>0) g3_2 = new TGraph(v3_2x.size(),&v3_2x[0],&v3_2y[0]);
	if(v3_3x.size()>0) g3_3 = new TGraph(v3_3x.size(),&v3_3x[0],&v3_3y[0]);
	if(v3_4x.size()>0) g3_4 = new TGraph(v3_4x.size(),&v3_4x[0],&v3_4y[0]);
	if(v3_5x.size()>0) g3_5 = new TGraph(v3_5x.size(),&v3_5x[0],&v3_5y[0]);
	if(v3_6x.size()>0) g3_6 = new TGraph(v3_6x.size(),&v3_6x[0],&v3_6y[0]);
	if(v3_7x.size()>0) g3_7 = new TGraph(v3_7x.size(),&v3_7x[0],&v3_7y[0]);

	if(v4_1x.size()>0) g4_1 = new TGraph(v4_1x.size(),&v4_1x[0],&v4_1y[0]);
	if(v4_2x.size()>0) g4_2 = new TGraph(v4_2x.size(),&v4_2x[0],&v4_2y[0]);
	if(v4_3x.size()>0) g4_3 = new TGraph(v4_3x.size(),&v4_3x[0],&v4_3y[0]);
	if(v4_4x.size()>0) g4_4 = new TGraph(v4_4x.size(),&v4_4x[0],&v4_4y[0]);
	if(v4_5x.size()>0) g4_5 = new TGraph(v4_5x.size(),&v4_5x[0],&v4_5y[0]);
	if(v4_6x.size()>0) g4_6 = new TGraph(v4_6x.size(),&v4_6x[0],&v4_6y[0]);
	if(v4_7x.size()>0) g4_7 = new TGraph(v4_7x.size(),&v4_7x[0],&v4_7y[0]);

	TAxis *axisx = 0;
	TAxis *axisy = 0;
	TGraph *g = 0;
	double ymin;
	double ymax;

	if (g1_1) g = g1_1;
	else if (g1_2) g = g1_2;
	else if (g1_3) g = g1_3;
	else if (g1_4) g = g1_4;
	else if (g1_5) g = g1_5;
	else if (g1_6) g = g1_6;
	else if (g1_7) g = g1_7;
	ymin = 1e14;
	ymax = 0;
	for (int i = 0; i<v1_1y.size(); i++){
		if (v1_1y[i]>ymax) ymax=v1_1y[i];
		if (v1_1y[i]<ymin) ymin=v1_1y[i];
	}
	for (int i = 0; i<v1_2y.size(); i++){
		if (v1_2y[i]>ymax) ymax=v1_2y[i];
		if (v1_2y[i]<ymin) ymin=v1_2y[i];
	}
	for (int i = 0; i<v1_3y.size(); i++){
		if (v1_3y[i]>ymax) ymax=v1_3y[i];
		if (v1_3y[i]<ymin) ymin=v1_3y[i];
	}
	for (int i = 0; i<v1_4y.size(); i++){
		if (v1_4y[i]>ymax) ymax=v1_4y[i];
		if (v1_4y[i]<ymin) ymin=v1_4y[i];
	}
	for (int i = 0; i<v1_5y.size(); i++){
		if (v1_5y[i]>ymax) ymax=v1_5y[i];
		if (v1_5y[i]<ymin) ymin=v1_5y[i];
	}
	for (int i = 0; i<v1_6y.size(); i++){
		if (v1_6y[i]>ymax) ymax=v1_6y[i];
		if (v1_6y[i]<ymin) ymin=v1_6y[i];
	}
	for (int i = 0; i<v1_7y.size(); i++){
		if (v1_7y[i]>ymax) ymax=v1_7y[i];
		if (v1_7y[i]<ymin) ymin=v1_7y[i];
	}
	std::cout<<"ymin = "<<ymin<<", ymax = "<<ymax<<std::endl;
	ymin = ymin/1.1;
	ymax = ymax*1.1;
	g->SetTitle("Numebr of #mu^{-} Stopped per Initial p^{+}");
	axisx = g->GetXaxis();
	axisx->SetTitle("y_{min} of the Collimator (mm)");
	axisy = g->GetYaxis();
	axisy->SetTitle("N_{#mu^{-}-stop}/p^{+}");
	g->GetHistogram()->SetMinimum(ymin);
	g->GetHistogram()->SetMaximum(ymax);

	if (g2_1) g = g2_1;
	else if (g2_2) g = g2_2;
	else if (g2_3) g = g2_3;
	else if (g2_4) g = g2_4;
	else if (g2_5) g = g2_5;
	else if (g2_6) g = g2_6;
	else if (g2_7) g = g2_7;
	ymin = 1e14;
	ymax = 0;
	for (int i = 0; i<v2_1y.size(); i++){
		if (v2_1y[i]>ymax) ymax=v2_1y[i];
		if (v2_1y[i]<ymin) ymin=v2_1y[i];
	}
	for (int i = 0; i<v2_2y.size(); i++){
		if (v2_2y[i]>ymax) ymax=v2_2y[i];
		if (v2_2y[i]<ymin) ymin=v2_2y[i];
	}
	for (int i = 0; i<v2_3y.size(); i++){
		if (v2_3y[i]>ymax) ymax=v2_3y[i];
		if (v2_3y[i]<ymin) ymin=v2_3y[i];
	}
	for (int i = 0; i<v2_4y.size(); i++){
		if (v2_4y[i]>ymax) ymax=v2_4y[i];
		if (v2_4y[i]<ymin) ymin=v2_4y[i];
	}
	for (int i = 0; i<v2_5y.size(); i++){
		if (v2_5y[i]>ymax) ymax=v2_5y[i];
		if (v2_5y[i]<ymin) ymin=v2_5y[i];
	}
	for (int i = 0; i<v2_6y.size(); i++){
		if (v2_6y[i]>ymax) ymax=v2_6y[i];
		if (v2_6y[i]<ymin) ymin=v2_6y[i];
	}
	for (int i = 0; i<v2_7y.size(); i++){
		if (v2_7y[i]>ymax) ymax=v2_7y[i];
		if (v2_7y[i]<ymin) ymin=v2_7y[i];
	}
	std::cout<<"ymin = "<<ymin<<", ymax = "<<ymax<<std::endl;
	ymin = ymin/1.1;
	ymax = ymax*1.1;
	g->SetTitle("Numebr of #mu^{-} (p>75 MeV/c) Entering CDC Region per Stopped #mu^{-}");
	axisx = g->GetXaxis();
	axisx->SetTitle("y_{min} of the Collimator (mm)");
	axisy = g->GetYaxis();
	axisy->SetTitle("N_{#mu^{-}}/#mu^{-}-stop");
	g->GetHistogram()->SetMinimum(ymin);
	g->GetHistogram()->SetMaximum(ymax);

	if (g3_1) g = g3_1;
	else if (g3_2) g = g3_2;
	else if (g3_3) g = g3_3;
	else if (g3_4) g = g3_4;
	else if (g3_5) g = g3_5;
	else if (g3_6) g = g3_6;
	else if (g3_7) g = g3_7;
	ymin = 1e14;
	ymax = 0;
	for (int i = 0; i<v3_1y.size(); i++){
		if (v3_1y[i]>ymax) ymax=v3_1y[i];
		if (v3_1y[i]<ymin) ymin=v3_1y[i];
	}
	for (int i = 0; i<v3_2y.size(); i++){
		if (v3_2y[i]>ymax) ymax=v3_2y[i];
		if (v3_2y[i]<ymin) ymin=v3_2y[i];
	}
	for (int i = 0; i<v3_3y.size(); i++){
		if (v3_3y[i]>ymax) ymax=v3_3y[i];
		if (v3_3y[i]<ymin) ymin=v3_3y[i];
	}
	for (int i = 0; i<v3_4y.size(); i++){
		if (v3_4y[i]>ymax) ymax=v3_4y[i];
		if (v3_4y[i]<ymin) ymin=v3_4y[i];
	}
	for (int i = 0; i<v3_5y.size(); i++){
		if (v3_5y[i]>ymax) ymax=v3_5y[i];
		if (v3_5y[i]<ymin) ymin=v3_5y[i];
	}
	for (int i = 0; i<v3_6y.size(); i++){
		if (v3_6y[i]>ymax) ymax=v3_6y[i];
		if (v3_6y[i]<ymin) ymin=v3_6y[i];
	}
	for (int i = 0; i<v3_7y.size(); i++){
		if (v3_7y[i]>ymax) ymax=v3_7y[i];
		if (v3_7y[i]<ymin) ymin=v3_7y[i];
	}
	std::cout<<"ymin = "<<ymin<<", ymax = "<<ymax<<std::endl;
	ymin = ymin/1.1;
	ymax = ymax*1.1;
	g->SetTitle("Numebr of #pi^{-} Stopped per Stopped #mu^{-}");
	axisx = g->GetXaxis();
	axisx->SetTitle("y_{min} of the Collimator (mm)");
	axisy = g->GetYaxis();
	axisy->SetTitle("N_{#pi^{-}-stop}/#mu^{-}-stop");
	g->GetHistogram()->SetMinimum(ymin);
	g->GetHistogram()->SetMaximum(ymax);

	if (g4_1) g = g4_1;
	else if (g4_2) g = g4_2;
	else if (g4_3) g = g4_3;
	else if (g4_4) g = g4_4;
	else if (g4_5) g = g4_5;
	else if (g4_6) g = g4_6;
	else if (g4_7) g = g4_7;
	ymin = 1e14;
	ymax = 0;
	for (int i = 0; i<v4_1y.size(); i++){
		if (v4_1y[i]>ymax) ymax=v4_1y[i];
		if (v4_1y[i]<ymin) ymin=v4_1y[i];
	}
	for (int i = 0; i<v4_2y.size(); i++){
		if (v4_2y[i]>ymax) ymax=v4_2y[i];
		if (v4_2y[i]<ymin) ymin=v4_2y[i];
	}
	for (int i = 0; i<v4_3y.size(); i++){
		if (v4_3y[i]>ymax) ymax=v4_3y[i];
		if (v4_3y[i]<ymin) ymin=v4_3y[i];
	}
	for (int i = 0; i<v4_4y.size(); i++){
		if (v4_4y[i]>ymax) ymax=v4_4y[i];
		if (v4_4y[i]<ymin) ymin=v4_4y[i];
	}
	for (int i = 0; i<v4_5y.size(); i++){
		if (v4_5y[i]>ymax) ymax=v4_5y[i];
		if (v4_5y[i]<ymin) ymin=v4_5y[i];
	}
	for (int i = 0; i<v4_6y.size(); i++){
		if (v4_6y[i]>ymax) ymax=v4_6y[i];
		if (v4_6y[i]<ymin) ymin=v4_6y[i];
	}
	for (int i = 0; i<v4_7y.size(); i++){
		if (v4_7y[i]>ymax) ymax=v4_7y[i];
		if (v4_7y[i]<ymin) ymin=v4_7y[i];
	}
	std::cout<<"ymin = "<<ymin<<", ymax = "<<ymax<<std::endl;
	ymin = ymin/1.1;
	ymax = ymax*1.1;
	g->SetTitle("Numebr of #pi^{-} Entering CDC Region per Stopped #mu^{-}");
	axisx = g->GetXaxis();
	axisx->SetTitle("y_{min} of the Collimator (mm)");
	axisy = g->GetYaxis();
	axisy->SetTitle("N_{#pi^{-}}/#mu^{-}-stop");
	g->GetHistogram()->SetMinimum(ymin);
	g->GetHistogram()->SetMaximum(ymax);

	if (g1_1) g1_1->SetMarkerStyle(20);
	if (g1_2) g1_2->SetMarkerStyle(20);
	if (g1_3) g1_3->SetMarkerStyle(20);
	if (g1_4) g1_4->SetMarkerStyle(20);
	if (g1_5) g1_5->SetMarkerStyle(20);
	if (g1_6) g1_6->SetMarkerStyle(20);
	if (g1_7) g1_7->SetMarkerStyle(20);
	if (g2_1) g2_1->SetMarkerStyle(20);
	if (g2_2) g2_2->SetMarkerStyle(20);
	if (g2_3) g2_3->SetMarkerStyle(20);
	if (g2_4) g2_4->SetMarkerStyle(20);
	if (g2_5) g2_5->SetMarkerStyle(20);
	if (g2_6) g2_6->SetMarkerStyle(20);
	if (g2_7) g2_7->SetMarkerStyle(20);
	if (g3_1) g3_1->SetMarkerStyle(20);
	if (g3_2) g3_2->SetMarkerStyle(20);
	if (g3_3) g3_3->SetMarkerStyle(20);
	if (g3_4) g3_4->SetMarkerStyle(20);
	if (g3_5) g3_5->SetMarkerStyle(20);
	if (g3_6) g3_6->SetMarkerStyle(20);
	if (g3_7) g3_7->SetMarkerStyle(20);
	if (g4_1) g4_1->SetMarkerStyle(20);
	if (g4_2) g4_2->SetMarkerStyle(20);
	if (g4_3) g4_3->SetMarkerStyle(20);
	if (g4_4) g4_4->SetMarkerStyle(20);
	if (g4_5) g4_5->SetMarkerStyle(20);
	if (g4_6) g4_6->SetMarkerStyle(20);
	if (g4_7) g4_7->SetMarkerStyle(20);
	if (g1_1) g1_1->SetLineColor(1);
	if (g1_2) g1_2->SetLineColor(600);
	if (g1_3) g1_3->SetLineColor(632);
	if (g1_4) g1_4->SetLineColor(416);
	if (g1_5) g1_5->SetLineColor(880);
	if (g1_6) g1_6->SetLineColor(1);
	if (g1_7) g1_7->SetLineColor(1);
	if (g2_1) g2_1->SetLineColor(1);
	if (g2_2) g2_2->SetLineColor(600);
	if (g2_3) g2_3->SetLineColor(632);
	if (g2_4) g2_4->SetLineColor(416);
	if (g2_5) g2_5->SetLineColor(880);
	if (g2_6) g2_6->SetLineColor(1);
	if (g2_7) g2_7->SetLineColor(1);
	if (g3_1) g3_1->SetLineColor(1);
	if (g3_2) g3_2->SetLineColor(600);
	if (g3_3) g3_3->SetLineColor(632);
	if (g3_4) g3_4->SetLineColor(416);
	if (g3_5) g3_5->SetLineColor(880);
	if (g3_6) g3_6->SetLineColor(1);
	if (g3_7) g3_7->SetLineColor(1);
	if (g4_1) g4_1->SetLineColor(1);
	if (g4_2) g4_2->SetLineColor(600);
	if (g4_3) g4_3->SetLineColor(632);
	if (g4_4) g4_4->SetLineColor(416);
	if (g4_5) g4_5->SetLineColor(880);
	if (g4_6) g4_6->SetLineColor(1);
	if (g4_7) g4_7->SetLineColor(1);
	if (g1_1) g1_1->SetMarkerColor(1);
	if (g1_2) g1_2->SetMarkerColor(600);
	if (g1_3) g1_3->SetMarkerColor(632);
	if (g1_4) g1_4->SetMarkerColor(416);
	if (g1_5) g1_5->SetMarkerColor(880);
	if (g1_6) g1_6->SetMarkerColor(1);
	if (g1_7) g1_7->SetMarkerColor(1);
	if (g2_1) g2_1->SetMarkerColor(1);
	if (g2_2) g2_2->SetMarkerColor(600);
	if (g2_3) g2_3->SetMarkerColor(632);
	if (g2_4) g2_4->SetMarkerColor(416);
	if (g2_5) g2_5->SetMarkerColor(880);
	if (g2_6) g2_6->SetMarkerColor(1);
	if (g2_7) g2_7->SetMarkerColor(1);
	if (g3_1) g3_1->SetMarkerColor(1);
	if (g3_2) g3_2->SetMarkerColor(600);
	if (g3_3) g3_3->SetMarkerColor(632);
	if (g3_4) g3_4->SetMarkerColor(416);
	if (g3_5) g3_5->SetMarkerColor(880);
	if (g3_6) g3_6->SetMarkerColor(1);
	if (g3_7) g3_7->SetMarkerColor(1);
	if (g4_1) g4_1->SetMarkerColor(1);
	if (g4_2) g4_2->SetMarkerColor(600);
	if (g4_3) g4_3->SetMarkerColor(632);
	if (g4_4) g4_4->SetMarkerColor(416);
	if (g4_5) g4_5->SetMarkerColor(880);
	if (g4_6) g4_6->SetMarkerColor(1);
	if (g4_7) g4_7->SetMarkerColor(1);

	gStyle->SetOptStat(0);
	gStyle->SetPadTickX(1);
	gStyle->SetPadTickY(1);

	TCanvas *c1 = new TCanvas("c1");
	TPad *pad1 = new TPad("pad1","",0,0.5,0.5,1);
	TPad *pad2 = new TPad("pad2","",0.5,0.5,1,1);
	TPad *pad3 = new TPad("pad3","",0,0,0.5,0.5);
	TPad *pad4 = new TPad("pad4","",0.5,0,1,0.5);
	pad1->Draw();
	pad2->Draw();
	pad3->Draw();
	pad4->Draw();

	TLegend *legend = 0;

	pad1->cd();
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	TString DrawOpt = "LAP";
	bool Drawn = false;
	if (g1_1) { g1_1->Draw(DrawOpt); if (!Drawn) {DrawOpt="LPSAME";Drawn=true;}}
	if (g1_2) { g1_2->Draw(DrawOpt); if (!Drawn) {DrawOpt="LPSAME";Drawn=true;}}
	if (g1_3) { g1_3->Draw(DrawOpt); if (!Drawn) {DrawOpt="LPSAME";Drawn=true;}}
	if (g1_4) { g1_4->Draw(DrawOpt); if (!Drawn) {DrawOpt="LPSAME";Drawn=true;}}
	if (g1_5) { g1_5->Draw(DrawOpt); if (!Drawn) {DrawOpt="LPSAME";Drawn=true;}}
	if (g1_6) { g1_6->Draw(DrawOpt); if (!Drawn) {DrawOpt="LPSAME";Drawn=true;}}
	if (g1_7) { g1_7->Draw(DrawOpt); if (!Drawn) {DrawOpt="LPSAME";Drawn=true;}}
	legend = new TLegend(0.1,0.7,0.5,0.9);
	if (g1_6) legend->AddEntry(g1_6,"B_{y} = 0.03 T, 17 disks, L_{tgt}=40cm, field: 130731","LP");
	if (g1_7) legend->AddEntry(g1_7,"B_{y} = 0.05 T, 39 disks, L_{tgt}=60cm, field: 130927","P");
	if (g1_1) legend->AddEntry(g1_1,"B_{y} = 0.03 T, 39 disks, L_{tgt}=40cm, field: 130731","LP");
	if (g1_2) legend->AddEntry(g1_2,"B_{y} = 0.04 T, 39 disks, L_{tgt}=40cm, field: 130731","LP");
	if (g1_3) legend->AddEntry(g1_3,"B_{y} = 0.05 T, 39 disks, L_{tgt}=40cm, field: 130731","LP");
	if (g1_4) legend->AddEntry(g1_4,"B_{y} = 0.06 T, 39 disks, L_{tgt}=40cm, field: 130731","LP");
	if (g1_5) legend->AddEntry(g1_5,"B_{y} = 0.07 T, 39 disks, L_{tgt}=40cm, field: 130731","LP");
	legend->Draw("same");

	pad2->cd();
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	DrawOpt = "LAP";
	Drawn = false;
	if (g2_1) { g2_1->Draw(DrawOpt); if (!Drawn) {DrawOpt="LPSAME";Drawn=true;}}
	if (g2_2) { g2_2->Draw(DrawOpt); if (!Drawn) {DrawOpt="LPSAME";Drawn=true;}}
	if (g2_3) { g2_3->Draw(DrawOpt); if (!Drawn) {DrawOpt="LPSAME";Drawn=true;}}
	if (g2_4) { g2_4->Draw(DrawOpt); if (!Drawn) {DrawOpt="LPSAME";Drawn=true;}}
	if (g2_5) { g2_5->Draw(DrawOpt); if (!Drawn) {DrawOpt="LPSAME";Drawn=true;}}
	if (g2_6) { g2_6->Draw(DrawOpt); if (!Drawn) {DrawOpt="LPSAME";Drawn=true;}}
	if (g2_7) { g2_7->Draw(DrawOpt); if (!Drawn) {DrawOpt="LPSAME";Drawn=true;}}
	legend = new TLegend(0.1,0.7,0.5,0.9);
	if (g2_6) legend->AddEntry(g1_6,"B_{y} = 0.03 T, 17 disks, L_{tgt}=40cm, field: 130731","LP");
	if (g2_7) legend->AddEntry(g1_7,"B_{y} = 0.05 T, 39 disks, L_{tgt}=60cm, field: 130927","P");
	if (g2_1) legend->AddEntry(g1_1,"B_{y} = 0.03 T, 39 disks, L_{tgt}=40cm, field: 130731","LP");
	if (g2_2) legend->AddEntry(g1_2,"B_{y} = 0.04 T, 39 disks, L_{tgt}=40cm, field: 130731","LP");
	if (g2_3) legend->AddEntry(g1_3,"B_{y} = 0.05 T, 39 disks, L_{tgt}=40cm, field: 130731","LP");
	if (g2_4) legend->AddEntry(g1_4,"B_{y} = 0.06 T, 39 disks, L_{tgt}=40cm, field: 130731","LP");
	if (g2_5) legend->AddEntry(g1_5,"B_{y} = 0.07 T, 39 disks, L_{tgt}=40cm, field: 130731","LP");
	legend->Draw("same");

	pad3->cd();
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	DrawOpt = "LAP";
	Drawn = false;
	if (g3_1) { g3_1->Draw(DrawOpt); if (!Drawn) {DrawOpt="LPSAME";Drawn=true;}}
	if (g3_2) { g3_2->Draw(DrawOpt); if (!Drawn) {DrawOpt="LPSAME";Drawn=true;}}
	if (g3_3) { g3_3->Draw(DrawOpt); if (!Drawn) {DrawOpt="LPSAME";Drawn=true;}}
	if (g3_4) { g3_4->Draw(DrawOpt); if (!Drawn) {DrawOpt="LPSAME";Drawn=true;}}
	if (g3_5) { g3_5->Draw(DrawOpt); if (!Drawn) {DrawOpt="LPSAME";Drawn=true;}}
	if (g3_6) { g3_6->Draw(DrawOpt); if (!Drawn) {DrawOpt="LPSAME";Drawn=true;}}
	if (g3_7) { g3_7->Draw(DrawOpt); if (!Drawn) {DrawOpt="LPSAME";Drawn=true;}}
	legend = new TLegend(0.1,0.7,0.5,0.9);
	if (g3_6) legend->AddEntry(g1_6,"B_{y} = 0.03 T, 17 disks, L_{tgt}=40cm, field: 130731","LP");
	if (g3_7) legend->AddEntry(g1_7,"B_{y} = 0.05 T, 39 disks, L_{tgt}=60cm, field: 130927","P");
	if (g3_1) legend->AddEntry(g1_1,"B_{y} = 0.03 T, 39 disks, L_{tgt}=40cm, field: 130731","LP");
	if (g3_2) legend->AddEntry(g1_2,"B_{y} = 0.04 T, 39 disks, L_{tgt}=40cm, field: 130731","LP");
	if (g3_3) legend->AddEntry(g1_3,"B_{y} = 0.05 T, 39 disks, L_{tgt}=40cm, field: 130731","LP");
	if (g3_4) legend->AddEntry(g1_4,"B_{y} = 0.06 T, 39 disks, L_{tgt}=40cm, field: 130731","LP");
	if (g3_5) legend->AddEntry(g1_5,"B_{y} = 0.07 T, 39 disks, L_{tgt}=40cm, field: 130731","LP");
	legend->Draw("same");

	pad4->cd();
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	DrawOpt = "LAP";
	Drawn = false;
	if (g4_1) { g4_1->Draw(DrawOpt); if (!Drawn) {DrawOpt="LPSAME";Drawn=true;}}
	if (g4_2) { g4_2->Draw(DrawOpt); if (!Drawn) {DrawOpt="LPSAME";Drawn=true;}}
	if (g4_3) { g4_3->Draw(DrawOpt); if (!Drawn) {DrawOpt="LPSAME";Drawn=true;}}
	if (g4_4) { g4_4->Draw(DrawOpt); if (!Drawn) {DrawOpt="LPSAME";Drawn=true;}}
	if (g4_5) { g4_5->Draw(DrawOpt); if (!Drawn) {DrawOpt="LPSAME";Drawn=true;}}
	if (g4_6) { g4_6->Draw(DrawOpt); if (!Drawn) {DrawOpt="LPSAME";Drawn=true;}}
	if (g4_7) { g4_7->Draw(DrawOpt); if (!Drawn) {DrawOpt="LPSAME";Drawn=true;}}
	legend = new TLegend(0.1,0.7,0.5,0.9);
	if (g4_6) legend->AddEntry(g1_6,"B_{y} = 0.03 T, 17 disks, L_{tgt}=40cm, field: 130731","LP");
	if (g4_7) legend->AddEntry(g1_7,"B_{y} = 0.05 T, 39 disks, L_{tgt}=60cm, field: 130927","P");
	if (g4_1) legend->AddEntry(g1_1,"B_{y} = 0.03 T, 39 disks, L_{tgt}=40cm, field: 130731","LP");
	if (g4_2) legend->AddEntry(g1_2,"B_{y} = 0.04 T, 39 disks, L_{tgt}=40cm, field: 130731","LP");
	if (g4_3) legend->AddEntry(g1_3,"B_{y} = 0.05 T, 39 disks, L_{tgt}=40cm, field: 130731","LP");
	if (g4_4) legend->AddEntry(g1_4,"B_{y} = 0.06 T, 39 disks, L_{tgt}=40cm, field: 130731","LP");
	if (g4_5) legend->AddEntry(g1_5,"B_{y} = 0.07 T, 39 disks, L_{tgt}=40cm, field: 130731","LP");
	legend->Draw("same");
}
