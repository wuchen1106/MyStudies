{
	TRandom rand;
	bool drawwire=false;
	std::stringstream buf;
	double wx,wy,wz,dd,ddt;
	TCanvas * c = 0;
	if (drawwire){
		c = new TCanvas("c","c",1440,900);
	}
	TPad * p1 = new TPad("p1","p1",0,0,0.5,0.5);
	TPad * p2 = new TPad("p2","p2",0.5,0,1,0.5);
	TPad * p3 = new TPad("p3","p3",0,0.5,1,1);
	p1->Draw("SAME");
	p2->Draw("SAME");
	p3->Draw("SAME");
	//TCanvas * c = new TCanvas("c","c",800,600);
	TRandom rand;

	std::vector<int> vncells;
	vncells.push_back(396/2);
	vncells.push_back(408/2);
	vncells.push_back(420/2);
	vncells.push_back(432/2);
	vncells.push_back(444/2);
	vncells.push_back(456/2);
	vncells.push_back(468/2);
	vncells.push_back(480/2);
	vncells.push_back(492/2);
	vncells.push_back(504/2);
	vncells.push_back(516/2);
	vncells.push_back(528/2);
	vncells.push_back(540/2);
	vncells.push_back(552/2);
	vncells.push_back(564/2);
	vncells.push_back(576/2);
	vncells.push_back(588/2);
	vncells.push_back(600/2);
	std::vector<int> vstartNo;
	int nlayers = vncells.size();
	int ncells = 0;
	for ( int i; i<nlayers; i++){
		vstartNo.push_back(ncells);
		ncells += vncells[i];
	} 
//	printf("%d layers, %d cells\n",nlayers,ncells);
	int nsegs = 64;

	TBox sigwire[4482];
	TBox trigseg[256];
	double lsig = 1;
	double hsig = 1;
	//double ltrig = vncells[0]*lsig/((double)nsegs);
	double ltrig = vncells[nlayers-1]*lsig/((double)nsegs);
	double htrig = 1;
	for (int ilayer = 0; ilayer<nlayers; ilayer++){
		lsig=1.*vncells[nlayers-1]/vncells[ilayer];
		for (int icell = 0; icell<vncells[ilayer]; icell++){
			int index = vstartNo[ilayer] + icell;
			//double x = lsig*icell+(vncells[vncells.size()-1]-vncells[ilayer])/2.-pow(-1,ilayer)*3;
			double x = lsig*icell;
			double y = hsig*ilayer;
			//printf("%d,%d->%lf,%lf\n",ilayer,icell,x,y);
			sigwire[index] = new TBox(x,y,x+lsig,y+hsig);
			sigwire[index]->SetLineColor(kBlack);
			//if (index%2==0) sigwire[index]->SetFillColor(kRed);
			//else sigwire[index]->SetFillColor(kBlue);
		}
	}
	//for ( int ilayer = 0; ilayer<4; ilayer++){
	for ( int ilayer = 2; ilayer<4; ilayer++){
		for ( int iseg = 0; iseg<nsegs; iseg++){
			int index = ilayer*nsegs + iseg;
			//double x = ltrig*(iseg<nsegs/4.?nsegs-iseg-1:iseg-nsegs/4)+48;
			double x = ltrig*(iseg<nsegs/4.?nsegs-iseg-1:iseg-nsegs/4);
			double y = htrig*ilayer-4;
			//printf("%d,%d->%lf,%lf\n",ilayer,icell,x,y);
			trigseg[index] = new TBox(x,y,x+ltrig,y+htrig);
			trigseg[index]->SetLineColor(kBlack);
			//if (index%2==0) trigseg[index]->SetFillColor(kRed);
			//else trigseg[index]->SetFillColor(kBlue);
		}
	}

	double edep_cut = 5e-6; // GeV
	double twindow_left = 200; // ns
	//TFile * ifile = new TFile("signal.electron.150um.G41001.smeared.withwire80um.AllDisks.map140905.root");
	TFile * ifile = new TFile("hitmap.3.root");
	TTree * it = (TTree*) ifile->Get("tree");

	int CdcCell_nHits = 0;
	std::vector<int> * CdcCell_cellID = 0;
	std::vector<int> * CdcCell_layerID = 0;
	std::vector<double> * CdcCell_edep = 0;
	std::vector<int> * CdcCell_hittype = 0;
	int M_nHits = 0;
	std::vector<std::string> * M_volName = 0;
	std::vector<int> * M_volID = 0;
	std::vector<int> * M_tid = 0;

	it->SetBranchAddress("CdcCell_nHits",&CdcCell_nHits);
	it->SetBranchAddress("CdcCell_cellID",&CdcCell_cellID);
	it->SetBranchAddress("CdcCell_layerID",&CdcCell_layerID);
	it->SetBranchAddress("CdcCell_edep",&CdcCell_edep);
	it->SetBranchAddress("CdcCell_hittype",&CdcCell_hittype);
	it->SetBranchAddress("M_nHits",&M_nHits);
	it->SetBranchAddress("M_volName",&M_volName);
	it->SetBranchAddress("M_volID",&M_volID);
	it->SetBranchAddress("M_tid",&M_tid);

//	int nhit_total = 0;
//	int nhit_edepcut = 0;
//	int nhitnoise_total = 0;
//	int nhitnoise_edepcut = 0;
//	int ntrack_total = 0;
//	int ntrack_mtcut = 0;
//	for ( int i = 0; i<it->GetEntries(); i++){
//		it->GetEntry(i);
//		for ( int j = 0; j<CdcCell_nHits; j++){
//			if ((*CdcCell_hittype)[j]!=0){
//				nhitnoise_total++;
//				if ((*CdcCell_edep)[j]<edep_cut){
//					nhitnoise_edepcut++;
//				}
//			}
//			else{
//				nhit_total++;
//				if ((*CdcCell_edep)[j]<edep_cut){
//					nhit_edepcut++;
//				}
//			}
//		}
//		ntrack_total++;
//		if (CdcCell_mt>=twindow_left) ntrack_mtcut++;
//	}
//	std::cout<<"=============================="<<std::endl;
//	std::cout<<"Dots: All hits"<<std::endl;
//	std::cout<<"	Position:	x-y coordinate of the point where the track entered that cell"<<std::endl;
//	std::cout<<"Line: signal hits"<<std::endl;
//	std::cout<<"	Draw based on the position of the Dots of signal track"<<std::endl;
//	std::cout<<"Circles: All hits"<<std::endl;
//	std::cout<<"	Position:	x-y coordinate of the hit sense wire at the z plane at that hit"<<std::endl;
//	std::cout<<"	Radius:	    driftD reverted from driftT, where driftT is t_hit-t_triggered (not exactly like this yet...)"<<std::endl;
//	std::cout<<"	Color:"<<std::endl;
//	std::cout<<"		Red:	Signal hits passed edep cut"<<std::endl;
//	std::cout<<"		Yellow:		Signal hits not passed edep cut"<<std::endl;
//	std::cout<<"		Grey:		Signal hits corrupted by noise"<<std::endl;
//	std::cout<<"		Blue:	Noise hits passed edep cut"<<std::endl;
//	std::cout<<"		Green:		Noise hits not passed edep cut"<<std::endl;
//	std::cout<<"=============================="<<std::endl;
//	std::cout<<"Total signal tracks: "<<ntrack_total<<std::endl;
//	std::cout<<"Signal tracks triggered after "<<twindow_left<<"ns: "<<ntrack_mtcut<<std::endl;
//	std::cout<<"Total signal hits: "<<nhit_total<<std::endl;
//	std::cout<<"Signal hits with edep<"<<edep_cut*1e6<<"keV: "<<nhit_edepcut<<std::endl;
//	std::cout<<"Total noise hits: "<<nhitnoise_total<<std::endl;
//	std::cout<<"Noise hits with edep<"<<edep_cut*1e6<<"keV: "<<nhitnoise_edepcut<<std::endl;

	TH2D * h0 = new TH2D("h0","",(vncells[nlayers-1]+2)*10+20,-11,vncells[nlayers-1]+11,(nlayers+2)*10,-5,nlayers+1);
	gStyle->SetOptStat(0);

	TH1D * h1dsig = 0;
	TH1D * h1dtot = 0;
	TH1D * h1dbkg = 0;

	TH1D * h2dsig = 0;
	TH1D * h2dtot = 0;
	TH1D * h2dbkg = 0;

	TH1D *	h2mtot = new TH1D("h2mtot2","Max Hits by Layer (total)",100,0,100);
	TH1D *	h2mbkg = new TH1D("h2mbkg","Max Hits by Layer",100,0,100);
	TH1D *	h2msig= new TH1D("h2msig","Max Hits by Layer",100,0,100);
	TH1D *	h1mtot = new TH1D("h1mtot2","Max Hits by Sector (total)",500,0,500);
	TH1D *	h1mbkg = new TH1D("h1mbkg","Max Hits by Sector",500,0,500);

	h1mbkg->SetLineColor(kBlue);
	h1mtot->SetLineColor(kBlack);
	h2mbkg->SetLineColor(kBlue);
	h2msig->SetLineColor(kRed);
	h2mtot->SetLineColor(kBlack);

	for ( int i = 0 ; i<it->GetEntries(); i++){
	//for ( int i = 0 ; i<it->GetEntries()/10; i++){
		if (i%1000==0) printf("%lf%...\n",(double)i/it->GetEntries()*100);
		//printf("###%d\n",i);
		it->GetEntry(i);

		if(h1dsig) delete h1dsig;
		if(h1dbkg) delete h1dbkg;
		if(h1dtot) delete h1dtot;
		h1dsig = new TH1D("h1sig","h1sig",20,0,300);
		h1dtot = new TH1D("h1tot","h1tot",20,0,300);
		h1dbkg = new TH1D("h1bkg","h1bkg",20,0,300);
		h1dtot->SetLineColor(kBlack);
		h1dtot->SetFillColor(kBlack);
		h1dtot->SetFillStyle(3003);
		h1dsig->SetFillColor(kRed);
		h1dsig->SetFillStyle(3001);
		h1dbkg->SetLineColor(kBlue);
		h1dbkg->SetFillColor(kBlue);
		h1dbkg->SetFillStyle(3002);
		if(h2dsig) delete h2dsig;
		if(h2dbkg) delete h2dbkg;
		if(h2dtot) delete h2dtot;
		h2dsig = new TH1D("h2sig","h2sig",18,0,18);
		h2dtot = new TH1D("h2tot","h2tot",18,0,18);
		h2dbkg = new TH1D("h2bkg","h2bkg",18,0,18);
		h2dtot->SetLineColor(kBlack);
		h2dtot->SetFillColor(kBlack);
		h2dtot->SetFillStyle(3003);
		h2dsig->SetFillColor(kRed);
		h2dsig->SetFillStyle(3001);
		h2dbkg->SetLineColor(kBlue);
		h2dbkg->SetFillColor(kBlue);
		h2dbkg->SetFillStyle(3002);

		if (drawwire){
			for (int index = 0; index<4482; index++){
				sigwire[index]->SetFillStyle(0);
			}
			for (int index = 0; index<256; index++){
				trigseg[index]->SetFillStyle(0);
			}
		}

		int maxlayer = 0;
		for (int j = 0; j<CdcCell_nHits; j++){
			if ((*CdcCell_hittype)[j]==0){
				if (maxlayer<(*CdcCell_layerID)[j])
					maxlayer = (*CdcCell_layerID)[j];
			}
		}
		if (maxlayer<5) continue;

		for (int j = 0; j<16; j++){
			int layer = rand.Uniform(2);
			if (layer>=1) layer = 2;
			else layer = 0;
			int seg = rand.Uniform(nsegs);
			int index = layer*nsegs+seg;
			trigseg[index]->SetFillStyle(1001);
			trigseg[index]->SetFillColor(kBlue);
			index = (layer+1)*nsegs+seg;
			trigseg[index]->SetFillStyle(1001);
			trigseg[index]->SetFillColor(kBlue);
		}

		int sigseg = 0;
		int siglayer = 0;
		for (int j = 0; j<M_nHits; j++){
			int layer = 0;
			if ((*M_volName)[j]=="TriCheU") layer = 1;
			else if ((*M_volName)[j]=="TriSciD") layer = 2;
			else if ((*M_volName)[j]=="TriCheD") layer = 3;
			if ((*M_tid)[j]==1){
				int index = 2*nsegs+(*M_volID)[j];
				trigseg[index]->SetFillColor(kRed);
				trigseg[index]->SetFillStyle(1001);
				sigseg = (*M_volID)[j];
				siglayer = layer;
				index = 3*nsegs+(*M_volID)[j];
				trigseg[index]->SetFillColor(kRed);
				trigseg[index]->SetFillStyle(1001);
				break;
			}
		}
		if (siglayer<2) continue;
		sigseg = (sigseg<nsegs/4.?nsegs-sigseg-1:sigseg-nsegs/4);
		double sigphi = sigseg/((double)nsegs);
		double noisephi = rand.Uniform(1);
		//printf("sigseg = %d, sigphi= %lf, noisephi = %lf\n",(int)sigseg,sigphi, noisephi);

		for (int j = 0; j<CdcCell_nHits; j++){
			int index = vstartNo[(*CdcCell_layerID)[j]]+(*CdcCell_cellID)[j];
			int ilayer = (*CdcCell_layerID)[j];
			double  deltaphisig = (*CdcCell_cellID)[j]/((double)vncells[ilayer])-sigphi;
			double  deltaphinoise = (*CdcCell_cellID)[j]/((double)vncells[ilayer])-noisephi;
			if (deltaphisig>0.5) deltaphisig-=1;
			if (deltaphisig<-0.5) deltaphisig+=1;
			if (deltaphinoise>0.5) deltaphinoise-=1;
			if (deltaphinoise<-0.5) deltaphinoise+=1;
			//printf("%d, [%d,%d]: %lf, %lf;\n",(*CdcCell_hittype)[j],ilayer,(*CdcCell_cellID)[j],deltaphisig,deltaphinoise);
			if (drawwire){
				//if ((*CdcCell_hittype)[j]!=0&&(*CdcCell_edep)[j]<edep_cut)
				//if ((*CdcCell_edep)[j]<edep_cut)
					sigwire[index]->SetFillStyle(1001);
			}
			//if (((*CdcCell_cellID)[j]-sigcell)>40||((*CdcCell_cellID)[j]-sigcell)<0) continue;
			if ((*CdcCell_hittype)[j]==0){
				//h1dsig->Fill((*CdcCell_layerID)[j]);
				//h1dtot->Fill((*CdcCell_layerID)[j]);
				if (drawwire){
					sigwire[index]->SetFillColor(kRed);
				}
				h1dsig->Fill((*CdcCell_cellID)[j]+(vncells[vncells.size()-1]-vncells[ilayer])/2.-pow(-1,ilayer)*3);
				h1dtot->Fill((*CdcCell_cellID)[j]+(vncells[vncells.size()-1]-vncells[ilayer])/2.-pow(-1,ilayer)*3);
				if (fabs(deltaphisig)<0.05&&(*CdcCell_layerID)[j]<4){
					h2dsig->Fill((*CdcCell_layerID)[j]);
				}
				if (fabs(deltaphinoise)<0.05&&(*CdcCell_layerID)[j]<4){
					h2dtot->Fill((*CdcCell_layerID)[j]);
				}
			}
			else if ((*CdcCell_edep)[j]<edep_cut){
				//h1dtot->Fill((*CdcCell_layerID)[j]);
				if (drawwire){
					sigwire[index]->SetFillColor(kBlue);
				}
				h1dbkg->Fill((*CdcCell_cellID)[j]+(vncells[vncells.size()-1]-vncells[ilayer])/2.-pow(-1,ilayer)*3);
				h1dtot->Fill((*CdcCell_cellID)[j]+(vncells[vncells.size()-1]-vncells[ilayer])/2.-pow(-1,ilayer)*3);
				if (fabs(deltaphisig)<0.05&&(*CdcCell_layerID)[j]<4){
					h2dsig->Fill((*CdcCell_layerID)[j]);
				}
				if (fabs(deltaphinoise)<0.05&&(*CdcCell_layerID)[j]<4){
					h2dtot->Fill((*CdcCell_layerID)[j]);
					h2dbkg->Fill((*CdcCell_layerID)[j]);
				}
			}
			else{
				//h1dtot->Fill((*CdcCell_layerID)[j]);
				if (drawwire){
					sigwire[index]->SetFillColor(kGreen);
				}
			}
		}

//		h1mbkg->Fill(h1dbkg->GetMaximum());
//		h1mtot->Fill(h1dtot->GetMaximum());
//		h2mbkg->Fill(h2dbkg->GetMaximum());
//		h2mtot->Fill(h2dtot->GetMaximum());

		h1mbkg->Fill(h1dbkg->Integral());
		h1mtot->Fill(h1dtot->Integral());
		h2mbkg->Fill(h2dbkg->Integral());
		h2msig->Fill(h2dsig->Integral());
		h2mtot->Fill(h2dtot->Integral());

		//=====================================
		if (drawwire){
			p1->cd();
			h1dtot->GetYaxis()->SetRangeUser(0,h1dtot->GetMaximum()+2);
			h1dtot->SetTitle(Form("Hits in Layers (max: noise%d total%d)",(int)h1dbkg->GetMaximum(),(int)h1dtot->GetMaximum()));
			//h1dtot->SetTitle(Form("%d %d",(int)h1dbkg->GetBinContent(h1dbkg->GetMaximumBin()+1,(int)h1dtot->GetBinContent(h1dtot->GetMaximumBin()+1))));
			h1dtot->GetXaxis()->SetTitle("CellID");
			h1dtot->Draw();
			h1dbkg->Draw("SAME");
			h1dsig->Draw("SAME");
			p2->cd();
			h2dsig->GetYaxis()->SetRangeUser(0,h2dtot->GetMaximum()+2);
			h2dsig->SetTitle(Form("Hits in sectors (max: noise%d total%d)",(int)h2dbkg->GetMaximum(),(int)h2dtot->GetMaximum()));
			//h2dtot->SetTitle(Form("%d %d",(int)h2dbkg->GetBinContent(h2dbkg->GetMaximumBin()+1,(int)h2dtot->GetBinContent(h2dtot->GetMaximumBin()+1))));
			h2dsig->GetXaxis()->SetTitle("LayerID");
			h2dsig->Draw();
			h2dbkg->Draw("SAME");
			h2dtot->Draw("SAME");
			p3->cd();
			h0->SetTitle(Form("Event #%d",i));
			h0->Draw();
			for (int index = 0; index<4482; index++){
				sigwire[index]->Draw("SAME");
			}
			for (int index = 0; index<256; index++){
				trigseg[index]->Draw("SAME");
			}
		}
		//=====================================

		//FIXME
		if (drawwire){
//			c->SaveAs(Form("Trigger/%d.png",i));
			c->WaitPrimitive();
		}
//		c->Update();
//		while(1){}
	}
	new TCanvas();
	h1mbkg->Draw();
	h1mtot->Draw("SAME");
	new TCanvas();
	h2mbkg->Draw();
	h2mtot->Draw("SAME");
	h2msig->Draw("SAME");
}
