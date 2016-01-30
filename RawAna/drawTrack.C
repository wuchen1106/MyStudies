{
	TFile * ifile_xt = new TFile("xt.GAS1-1.GASFILE.1850.0.33.6900681.0.root");
	TF1 * f_t2x = (TF1*) ifile_xt->Get("f2");

	double edep_cut = 5e-6; // GeV
	double twindow_left = 700; // ns
	//TFile * ifile = new TFile("result/signal.160121.uniform.trans.root");
	TFile * ifile = new TFile("result/signal.160126.root");
	TTree * it = (TTree*) ifile->Get("tree");
	TFile * of = new TFile("output.root","RECREATE");

	int CdcCell_nHits = 0;
	std::vector<double> * CdcCell_t = 0;
//	std::vector<double> * CdcCell_ox = 0;
//	std::vector<double> * CdcCell_oy = 0;
//	std::vector<double> * CdcCell_oz = 0;
	std::vector<double> * CdcCell_wx = 0;
	std::vector<double> * CdcCell_wy = 0;
	std::vector<double> * CdcCell_wz = 0;
	std::vector<double> * CdcCell_x = 0;
	std::vector<double> * CdcCell_y = 0;
	std::vector<double> * CdcCell_z = 0;
	std::vector<double> * CdcCell_px = 0;
	std::vector<double> * CdcCell_py = 0;
	std::vector<double> * CdcCell_pz = 0;
	std::vector<double> * CdcCell_driftD = 0;
	std::vector<double> * CdcCell_driftDtrue = 0;
	std::vector<double> * CdcCell_tstart = 0;
	std::vector<int> * CdcCell_cellID = 0;
	std::vector<int> * CdcCell_layerID = 0;
	std::vector<double> * CdcCell_edep = 0;
	std::vector<int> * CdcCell_tid = 0;
	std::vector<int> * CdcCell_posflag = 0;
	std::vector<int> * CdcCell_tid = 0;
	double CdcCell_mt = 0;
	int M_nHits = 0;
	int evt_num = 0;
	std::vector<int> * M_hittype = 0;
	std::vector<double> * M_t = 0;

	it->SetBranchAddress("evt_num",&evt_num);
	it->SetBranchAddress("CdcCell_nHits",&CdcCell_nHits);
	it->SetBranchAddress("CdcCell_t",&CdcCell_t);
//	it->SetBranchAddress("CdcCell_ox",&CdcCell_ox);
//	it->SetBranchAddress("CdcCell_oy",&CdcCell_oy);
//	it->SetBranchAddress("CdcCell_oz",&CdcCell_oz);
	it->SetBranchAddress("CdcCell_wx",&CdcCell_wx);
	it->SetBranchAddress("CdcCell_wy",&CdcCell_wy);
	it->SetBranchAddress("CdcCell_wz",&CdcCell_wz);
	it->SetBranchAddress("CdcCell_x",&CdcCell_x);
	it->SetBranchAddress("CdcCell_y",&CdcCell_y);
	it->SetBranchAddress("CdcCell_z",&CdcCell_z);
	it->SetBranchAddress("CdcCell_px",&CdcCell_px);
	it->SetBranchAddress("CdcCell_py",&CdcCell_py);
	it->SetBranchAddress("CdcCell_pz",&CdcCell_pz);
	it->SetBranchAddress("CdcCell_driftD",&CdcCell_driftD);
	it->SetBranchAddress("CdcCell_driftDtrue",&CdcCell_driftDtrue);
	it->SetBranchAddress("CdcCell_tstart",&CdcCell_tstart);
	it->SetBranchAddress("CdcCell_cellID",&CdcCell_cellID);
	it->SetBranchAddress("CdcCell_layerID",&CdcCell_layerID);
	it->SetBranchAddress("CdcCell_edep",&CdcCell_edep);
//	it->SetBranchAddress("CdcCell_tid",&CdcCell_tid);
	it->SetBranchAddress("CdcCell_posflag",&CdcCell_posflag);
	it->SetBranchAddress("CdcCell_tid",&CdcCell_tid);
	it->SetBranchAddress("CdcCell_mt",&CdcCell_mt);
	it->SetBranchAddress("M_nHits",&M_nHits);
	it->SetBranchAddress("M_t",&M_t);
	it->SetBranchAddress("M_hittype",&M_hittype);

	TEllipse * ewiret = 0;
	TEllipse * ewire = 0;
	double wx,wy,wz,dd,ddt;
	std::stringstream buf;
	TCanvas * c = new TCanvas("c","c",896,896);
	int nhit_total = 0;
	int nhit_edepcut = 0;
	int nhitnoise_total = 0;
	int nhitnoise_edepcut = 0;
	int ntrack_total = 0;
	int ntrack_mtcut = 0;
	for ( int i = 0; i<it->GetEntries(); i++){
		it->GetEntry(i);
		for ( int j = 0; j<CdcCell_nHits; j++){
			if ((*CdcCell_tid)[j]!=1){
				nhitnoise_total++;
				if ((*CdcCell_edep)[j]<edep_cut){
					nhitnoise_edepcut++;
				}
			}
			else{
				nhit_total++;
				if ((*CdcCell_edep)[j]<edep_cut){
					nhit_edepcut++;
				}
			}
		}
		ntrack_total++;
		if (CdcCell_mt>=twindow_left) ntrack_mtcut++;
	}
	std::cout<<"=============================="<<std::endl;
	std::cout<<"Dots: All hits"<<std::endl;
	std::cout<<"	Position:	x-y coordinate of the point where the track entered that cell"<<std::endl;
	std::cout<<"Line: signal hits"<<std::endl;
	std::cout<<"	Draw based on the position of the Dots of signal track"<<std::endl;
	std::cout<<"Circles: All hits"<<std::endl;
	std::cout<<"	Position:	x-y coordinate of the hit sense wire at the z plane at that hit"<<std::endl;
	std::cout<<"	Radius:	    driftD reverted from driftT, where driftT is t_hit-t_triggered (not exactly like this yet...)"<<std::endl;
	std::cout<<"	Color:"<<std::endl;
	std::cout<<"		Red:	Signal hits passed edep cut"<<std::endl;
	std::cout<<"		Yellow:		Signal hits not passed edep cut"<<std::endl;
	std::cout<<"		Gray:		Signal hits corrupted by noise"<<std::endl;
	std::cout<<"		Magenta:pmc hits passed edep cut"<<std::endl;
	std::cout<<"		Cyan:	dio.o50 hits passed edep cut"<<std::endl;
	std::cout<<"		Black:	pc hits passed edep cut"<<std::endl;
	std::cout<<"		Blue:	Noise hits passed edep cut"<<std::endl;
	std::cout<<"		Green:		Noise hits not passed edep cut"<<std::endl;
	std::cout<<"=============================="<<std::endl;
	std::cout<<"Total signal tracks: "<<ntrack_total<<std::endl;
	std::cout<<"Signal tracks triggered after "<<twindow_left<<"ns: "<<ntrack_mtcut<<std::endl;
	std::cout<<"Total signal hits: "<<nhit_total<<std::endl;
	std::cout<<"Signal hits with edep<"<<edep_cut*1e6<<"keV: "<<nhit_edepcut<<std::endl;
	std::cout<<"Total noise hits: "<<nhitnoise_total<<std::endl;
	std::cout<<"Noise hits with edep<"<<edep_cut*1e6<<"keV: "<<nhitnoise_edepcut<<std::endl;

	TH2D * h0 = new TH2D("h0","h0",5000,-85,85,5000,-85,85);
	gStyle->SetOptStat(0);
//	for ( int i = 0 ; i<it->GetEntries(); i++){
	for ( int i = 309; i<310; i++){
		if (i%100==0) printf("%lf%...\n",(double)i/it->GetEntries()*100);
//		if (i!=10) continue; // 0
		buf.str("");
		buf.clear();
		//FIXME
		it->Draw("CdcCell_y:CdcCell_x>>h","","",1,i);
		it->Draw("CdcCell_y:CdcCell_x>>h1",Form("CdcCell_edep>%lf",edep_cut),"",1,i);
		it->Draw("CdcCell_wy:CdcCell_wx>>h2","CdcCell_tid==1","",1,i);
		it->Draw("CdcCell_wy:CdcCell_wx>>h3",Form("CdcCell_edep>%lf&&CdcCell_tid==1",edep_cut),"",1,i);
		it->GetEntry(i);
		CdcCell_mt = 700;
		if (M_hittype){
			for (int j = 0; j<M_nHits; j++){
				if ((*M_hittype)[j]==1){
					CdcCell_mt = (*M_t)[j];
					break;
				}
			}
		}
		buf.str("");
		buf.clear();
		buf<<"#"<<i<<": Triggered @ "<<CdcCell_mt<<" ns, Ngood = ("<<h2->GetEntries()<<","<<h2->GetEntries()-h3->GetEntries()<<"), Nnoise = ("<<h->GetEntries()-h2->GetEntries()<<","<<h1->GetEntries()-h3->GetEntries()<<")";
		h0->SetTitle(buf.str().c_str());
		h0->Draw();
		buf.str("");
		buf.clear();
		buf<<"CdcCell_edep<"<<edep_cut<<"&&CdcCell_tid==1&&CdcCell_t<7&&CdcCell_driftD<=0.8";
		it->Draw("CdcCell_y:CdcCell_x",buf.str().c_str(),"SAMELINE",1,i); // draw the track
		it->SetMarkerStyle(7);
		it->SetMarkerColor(kBlack);
		it->Draw("CdcCell_y:CdcCell_x",buf.str().c_str(),"SAME",1,i); // draw the hit point of the track
		it->SetMarkerColor(kRed);
		it->Draw("CdcCell_wy:CdcCell_wx",buf.str().c_str(),"SAME",1,i); // draw the wire position of the hits of the track
		buf.str("");
		buf.clear();
		ewiret = new TEllipse(0,0,82,82);
		ewiret->SetFillStyle(0);
		ewiret->SetLineColor(kBlack);
		ewiret->Draw("SAME"); // draw outer wall
		ewiret = new TEllipse(0,0,50,50);
		ewiret->SetFillStyle(0);
		ewiret->SetLineColor(kBlack);
		ewiret->Draw("SAME"); // draw inner wall
		for (int j = 0; j<CdcCell_nHits; j++){
			// FIXME: time or distance?
			if((*CdcCell_t)[j]-7>0) continue;
			if((*CdcCell_driftD)[j]>0.8) continue;
//			if((*CdcCell_tstart)[j]-CdcCell_mt<0) continue;
			wx = (*CdcCell_wx)[j];
			wy = (*CdcCell_wy)[j];
			dd = (*CdcCell_driftD)[j];
//			ddt = f_t2x->Eval((*CdcCell_tstart)[j]-CdcCell_mt);
			ddt = dd;
//			ewire = new TEllipse(wx,wy,dd,dd);
//			ewire->SetLineColor(kBlue);
//			ewire->Draw("SAME");
			ewiret = new TEllipse(wx,wy,ddt,ddt);
			ewiret->SetFillStyle(0);
			//FIXME
//			if ((*CdcCell_t)[j]>7) continue;
			if ((*CdcCell_edep)[j]<edep_cut){
//				if ((*CdcCell_tid)[j]==1)
				if ((*CdcCell_tid)[j]==1||(*CdcCell_tid)[j]==2)
					ewiret->SetLineColor(kRed);
				else if ((*CdcCell_tid)[j]==3)
					ewiret->SetLineColor(kBlack);
//				else if ((*CdcCell_tid)[j]==-1)
				else if ((*CdcCell_tid)[j]==-1||(*CdcCell_tid)[j]==-2)
					ewiret->SetLineColor(kMagenta);
//				else if ((*CdcCell_tid)[j]==-3)
				else if ((*CdcCell_tid)[j]==-3||(*CdcCell_tid)[j]==-4)
					ewiret->SetLineColor(kCyan);
				else
					ewiret->SetLineColor(kBlue);
			}
			else{
				//FIXME
				//continue;
				if ((*CdcCell_tid)[j]==1)
					ewiret->SetLineColor(kOrange);
				else if ((*CdcCell_tid)[j]==3)
					ewiret->SetLineColor(kGray);
				else
					ewiret->SetLineColor(kGreen);
			}
			ewiret->Draw("SAME"); // Draw hits.
		}
		buf.str("");
		buf.clear();
		//FIXME
		buf<<i<<"_before.pdf";
//		buf<<i<<"_after.pdf";
//		c->SaveAs(buf.str().c_str());
	//	c->WaitPrimitive();
//		c->Update();
//		while(1){}
	}
//	of->Close();
}
