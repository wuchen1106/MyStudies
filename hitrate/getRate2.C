TString MyData = getenv("MYDATA");

int cellNo[18];
int cellNoIntegral[18];
int count = 0;
cellNoIntegral[count]=                        198;cellNo[count]=198;count++;
cellNoIntegral[count]=cellNoIntegral[count-1]+204;cellNo[count]=204;count++;
cellNoIntegral[count]=cellNoIntegral[count-1]+210;cellNo[count]=210;count++;
cellNoIntegral[count]=cellNoIntegral[count-1]+216;cellNo[count]=216;count++;
cellNoIntegral[count]=cellNoIntegral[count-1]+222;cellNo[count]=222;count++;
cellNoIntegral[count]=cellNoIntegral[count-1]+228;cellNo[count]=228;count++;
cellNoIntegral[count]=cellNoIntegral[count-1]+234;cellNo[count]=234;count++;
cellNoIntegral[count]=cellNoIntegral[count-1]+240;cellNo[count]=240;count++;
cellNoIntegral[count]=cellNoIntegral[count-1]+246;cellNo[count]=246;count++;
cellNoIntegral[count]=cellNoIntegral[count-1]+252;cellNo[count]=252;count++;
cellNoIntegral[count]=cellNoIntegral[count-1]+258;cellNo[count]=258;count++;
cellNoIntegral[count]=cellNoIntegral[count-1]+264;cellNo[count]=264;count++;
cellNoIntegral[count]=cellNoIntegral[count-1]+270;cellNo[count]=270;count++;
cellNoIntegral[count]=cellNoIntegral[count-1]+276;cellNo[count]=276;count++;
cellNoIntegral[count]=cellNoIntegral[count-1]+282;cellNo[count]=282;count++;
cellNoIntegral[count]=cellNoIntegral[count-1]+288;cellNo[count]=288;count++;
cellNoIntegral[count]=cellNoIntegral[count-1]+294;cellNo[count]=294;count++;
cellNoIntegral[count]=cellNoIntegral[count-1]+300;cellNo[count]=300;count++;

int get_layer_index(int volID){
	for (int i = 0; i< 18; i++){
		if (volID<cellNoIntegral[i]) break;
	}
	return i;
}

void getRate2(){

	// About this run
	std::vector<TString> DirName;
	std::vector<int> nRuns;
	std::vector<TString> FileNames;
	 // ########Should Modify#########
	TString runName = "ALL.150901.W100um.OptD2.DD28.TH10mm";
//	FileNames.push_back("result/OT.root");
	FileNames.push_back(runName+".root");
	//DirName.push_back("/scratchfs/bes/wuc/MyWorkArea/Data/raw/g4sim/BLTCDC.em.g60cm10mm.005T.1p5_0927_11_p5.g4s.QBH");
	//nRuns.push_back(100);
	//DirName.push_back("/scratchfs/bes/wuc/MyWorkArea/Data/raw/g4sim/BLTCDC.OT.g60cm10mm.005T.1p5_0927_11_p5.g4s.QBH");
	//nRuns.push_back(100);
	//DirName.push_back("/scratchfs/bes/wuc/MyWorkArea/Data/raw/g4sim/BLTCDC.mum.g60cm10mm.005T.1p5_0927_11_p5.g4s.QBH");
	//nRuns.push_back(100);
//	DirName.push_back(MyData+"/raw/g4sim/CDCHit.OT.g60cm10mm.005T.140331.g4s.QBH");
//	nRuns.push_back(100);
	double nProtons = 1e9;
	 // ########Should Modify#########

	// Beam Structure
	double PulseInterval = 1170; // ns
	double dutyFactor = 1./3;
	double proton_rate = 2.5e12; // Hz
	double right_end = 1150; // ns
	double left_end = 700; // ns
	TFile * f = new TFile("result/Curves.s100.root");

	 // ########Should Modify#########
	TH1D *hCurve = (TH1D*) f->Get("ProtonPuls"); // for beam particles and stopped pions
	//TH1D *hCurve = (TH1D*) f->Get("Convoluted"); // for stopped muons
	 // ########Should Modify#########
	hCurve->RebinX(20);

	// CDC Info
	double W_He = 41; // eV
	double W_iC4H10 = 23; // eV
	double gain = 1e5; // and  <--- smaller than 1e5 due to space charge? 
	double cdc_length = 150 ; // cm
	double tsep = 1170; // ns
	double edep2charge = proton_rate/nProtons*(0.9/W_He+0.1/W_iC4H10)*gain*1.6e-19/cdc_length*24*3600; // C/cm/day
	double hit2rate = proton_rate/nProtons/1000; // kHz
	//	TFile *f = 0;

	TChain *c = new TChain("tree");
	std::cout<<"FileNames.size() = "<<(FileNames.size())<<std::endl;
	for (int i = 0; i<FileNames.size(); i++){
		std::cout<<"FileNames["<<i<<"] = \""<<FileNames[i]<<"\""<<std::endl;
		c->Add(FileNames[i]);
	}
	std::stringstream buff;
	std::cout<<"nRuns = "<<nRuns.size()<<std::endl;
	for (int iRun = 0; iRun < nRuns.size(); iRun++ ){
		for (int i = 0; i<nRuns[iRun]; i++){
			buff.str("");
			buff.clear();
			buff<<DirName[iRun]<<"/"<<i<<"_job0.raw";
			c->Add( buff.str().c_str());
		}
	}

    int Color_1 = 632;
    int Color_2 = 800;
    int Color_3 = 880;
    int Color_4 = 600;

    int Style_1 = 20; 
    int Style_2 = 20; 
    int Style_3 = 20; 
    int Style_4 = 20; 
    
    TString xtitle1 = "Time (ns)";
    TString ytitle1 = "Hit Rate per Cell (kHz)";

    TString hName1 = "Innermost Layer";
    TString hName2 = "Seccond Innermost Layer";
    TString hName3 = "Seccond Outmost Layer";
    TString hName4 = "Outmost Layer";

	TString tilte = "Hit Rate in CDC Caused by Beam Particles (Stopped #mu^{-} #pi^{-} Not Included)";

	// hit rate in different cdc layers
    TH1D *h1 = new TH1D("h1",tilte,100,0,PulseInterval);
    h1->GetYaxis()->SetTitle(ytitle1);
    h1->GetXaxis()->SetTitle(xtitle1);
    h1->SetMarkerStyle(Style_1);
    h1->SetMarkerColor(Color_1);
    h1->SetLineColor(Color_1);
    TH1D *h2 = new TH1D("h2",tilte,100,0,PulseInterval);
    h2->SetMarkerStyle(Style_2);
    h2->SetMarkerColor(Color_2);
    h2->SetLineColor(Color_2);
    TH1D *h3 = new TH1D("h3",tilte,100,0,PulseInterval);
    h3->SetMarkerStyle(Style_3);
    h3->SetMarkerColor(Color_3);
    h3->SetLineColor(Color_3);
    TH1D *h4 = new TH1D("h4",tilte,100,0,PulseInterval);
    h4->SetMarkerStyle(Style_4);
    h4->SetMarkerColor(Color_4);
    h4->SetLineColor(Color_4);
    TH1D *h5 = new TH1D("h5",tilte,200,0,1000);
//    h5->SetMarkerStyle(Style_4);
//    h5->SetMarkerColor(Color_4);
//    h5->SetLineColor(Color_4);

	// hit rate in Upstream Trigger counter
    TH1D *h11 = new TH1D("h11",tilte,100,0,PulseInterval);
    h11->GetYaxis()->SetTitle(ytitle1);
    h11->GetXaxis()->SetTitle(xtitle1);
    h11->SetMarkerStyle(Style_1);
    h11->SetMarkerColor(Color_1);
    h11->SetLineColor(Color_1);
    TH1D *h12 = new TH1D("h12",tilte,100,0,PulseInterval);
    h12->SetMarkerStyle(Style_2);
    h12->SetMarkerColor(Color_2);
    h12->SetLineColor(Color_2);
    TH1D *h13 = new TH1D("h13",tilte,100,0,PulseInterval);
    h13->SetMarkerStyle(Style_3);
    h13->SetMarkerColor(Color_3);
    h13->SetLineColor(Color_3);

	// hit rate in Downstream Trigger counter
    TH1D *h21 = new TH1D("h21",tilte,100,0,PulseInterval);
    h21->GetYaxis()->SetTitle(ytitle1);
    h21->GetXaxis()->SetTitle(xtitle1);
    h21->SetMarkerStyle(Style_1);
    h21->SetMarkerColor(Color_1);
    h21->SetLineColor(Color_1);
    TH1D *h22 = new TH1D("h22",tilte,100,0,PulseInterval);
    h22->SetMarkerStyle(Style_2);
    h22->SetMarkerColor(Color_2);
    h22->SetLineColor(Color_2);
    TH1D *h23 = new TH1D("h23",tilte,100,0,PulseInterval);
    h23->SetMarkerStyle(Style_3);
    h23->SetMarkerColor(Color_3);
    h23->SetLineColor(Color_3);

	// Momentum Distribution in Upstream Trigger counter
    TH1D *h31 = new TH1D("h31",tilte,100,0,PulseInterval);
    h31->GetYaxis()->SetTitle(ytitle1);
    h31->GetXaxis()->SetTitle(xtitle1);
    h31->SetMarkerStyle(Style_1);
    h31->SetMarkerColor(Color_1);
    h31->SetLineColor(Color_1);
    TH1D *h32 = new TH1D("h32",tilte,100,0,PulseInterval);
    h32->SetMarkerStyle(Style_2);
    h32->SetMarkerColor(Color_2);
    h32->SetLineColor(Color_2);
    TH1D *h33 = new TH1D("h33",tilte,100,0,PulseInterval);
    h33->SetMarkerStyle(Style_3);
    h33->SetMarkerColor(Color_3);
    h33->SetLineColor(Color_3);

	// Momentum Distribution in Downstream Trigger counter
    TH1D *h41 = new TH1D("h41",tilte,100,0,PulseInterval);
    h41->GetYaxis()->SetTitle(ytitle1);
    h41->GetXaxis()->SetTitle(xtitle1);
    h41->SetMarkerStyle(Style_1);
    h41->SetMarkerColor(Color_1);
    h41->SetLineColor(Color_1);
    TH1D *h42 = new TH1D("h42",tilte,100,0,PulseInterval);
    h42->SetMarkerStyle(Style_2);
    h42->SetMarkerColor(Color_2);
    h42->SetLineColor(Color_2);
    TH1D *h43 = new TH1D("h43",tilte,100,0,PulseInterval);
    h43->SetMarkerStyle(Style_3);
    h43->SetMarkerColor(Color_3);
    h43->SetLineColor(Color_3);

	std::vector<double> vLayerID;
	std::vector<double> vHitrate;
	std::vector<double> vCharge;
	for (int i = 0; i < 18; i++){
		vLayerID.push_back(i+1);
		vHitrate.push_back(0);
		vCharge.push_back(0);
	}

	std::vector<int> *C_volID;
	std::vector<double> *C_edep;
	std::vector<double> *C_t;
	std::vector<double> *C_px;
	std::vector<double> *C_py;
	std::vector<double> *C_pz;
	double weight;
	c->SetBranchAddress("C_volID",&C_volID);
	c->SetBranchAddress("C_edep",&C_edep);
	c->SetBranchAddress("C_t",&C_t);
	c->SetBranchAddress("C_px",&C_px);
	c->SetBranchAddress("C_py",&C_py);
	c->SetBranchAddress("C_pz",&C_pz);
	c->SetBranchAddress("weight",&weight);
	double edep = -1;
	double time = -1;
	int volID = -1;
	f = new TFile(runName+".output.root","RECREATE");
	TTree *t  = new TTree("t","t");
	int nEvents = c->GetEntries();
	std::cout<<"nEvents = "<<nEvents<<std::endl;
	std::stringstream buff;
	for (int iEvent = 0; iEvent < nEvents; iEvent++ ){
		if (iEvent%5000==0) std::cout<<(double)iEvent/nEvents*100<<" % ..."<<std::endl;
		c->GetEntry(iEvent);
//		if (iEvent>=11094) weight*=100./31;
		int nHits = C_edep->size();
		int hitcount[18] = {0};
		bool foundhit = false;
		int layerID_max = 0;
		double pa = -1;
		for (int iHit = 0; iHit < nHits; iHit++){
//			if (pa==-1){
//				double px = (*C_px)[iHit]*1000;
//				double py = (*C_py)[iHit]*1000;
//				double pz = (*C_pz)[iHit]*1000;
//				pa = sqrt(px*px+py*py+pz*pz);
//			}
			volID = (*C_volID)[iHit];
			time = (*C_t)[iHit];
			edep = (*C_edep)[iHit]*1e9;
			int layerID = get_layer_index(volID);
//			vHitrate[layerID]+=weight*hit2rate/cellNo[layerID];
			vHitrate[layerID]+=weight*hit2rate*tsep*1e-9*100*1000/cellNo[layerID];
			vCharge[layerID]+=edep*edep2charge/cellNo[layerID];
			hitcount[layerID]++;
			foundhit = true;
			if (layerID>layerID_max) layerID_max = layerID;
			for (int i_window = 0; i_window <5; i_window++){
				double newtime = time+hCurve->GetRandom()+i_window*PulseInterval;
				if (layerID==0) h1->Fill(newtime,weight*hit2rate*100/cellNo[layerID]);
				else if (layerID==1) h2->Fill(newtime,weight*hit2rate*100/cellNo[layerID]);
				else if (layerID==17) h3->Fill(newtime,weight*hit2rate*100/cellNo[layerID]);
				else if (layerID==16) h4->Fill(newtime,weight*hit2rate*100/cellNo[layerID]);
			}
		}
		if (nHits>0){
			double px = (*C_px)[0]*1000;
			double py = (*C_py)[0]*1000;
			double pz = (*C_pz)[0]*1000;
			double pa = sqrt(px*px+py*py+pz*pz);
			if (pa>10)
				h5->Fill(pa,weight);
		}
//		if (foundhit){
		if (0){
			TH1D * h = new TH1D("h","h",18,0,18);
			for (int ilayer = 0;ilayer<18; ilayer++){
				h->Fill(ilayer,hitcount[ilayer]);
			}
			std::cout<<"In Event["<<iEvent<<"]"<<std::endl;
			buff.str("");
			buff.clear();
			buff<<"pa = "<<pa<<" MeV/c";
			h->SetTitle(buff.str().c_str());
			TCanvas *c2 = (TCanvas*)gROOT->GetListOfCanvases()->FindObject("c2");
			if (!c2) c2 = new TCanvas("c2","c2",10,10,1000,700);
			c2->Update();
			h->Draw();
			buff.str("");
			buff.clear();
			buff<<(int)pa<<"_"<<layerID_max<<".png";
			printf("\nDouble click in the bottom right corner of the pad to continue\n");
			c2->SaveAs(buff.str().c_str());
			c2->WaitPrimitive();
		}
	}

	TGraph * g1 = new TGraph(vLayerID.size(),&(vLayerID[0]),&(vHitrate[0]));
//	g1->SetTitle(runName+": Hit Rate Per Cell (kHz)");
	g1->SetTitle(runName+": Occupancy (%)");
	g1->GetHistogram()->GetXaxis()->SetTitle("Layer ID (1-18)");
	g1->SetName("g1");
	TGraph * g2 = new TGraph(vLayerID.size(),&(vLayerID[0]),&(vCharge[0]));
	g2->GetHistogram()->GetXaxis()->SetTitle("Layer ID (1-18)");
	g2->SetTitle(runName+":Q (C/cm/day)");
	g2->SetName("g2");

	TCanvas *c1 = new TCanvas("c","c");
//	TPad * p1 = new TPad("p1","p1",0,0,1./2,0.5);
	TPad * p1 = new TPad("p1","p1",0,0,1,1);
	TPad * p2 = new TPad("p2","p2",1./2,0,2./2,0.5);
	TPad * p3 = new TPad("p3","p3",0,0.5,1,1);
	p1->Draw();
//	p2->Draw();
//	p3->Draw();
	p1->SetGridx(1);
	p1->SetGridy(1);
	p2->SetGridx(1);
	p2->SetGridy(1);
	p3->SetGridx(1);
	p3->SetGridy(1);
	gStyle->SetPalette(1);
	gStyle->SetOptStat(0);
	gStyle->SetPadTickX(1);
	gStyle->SetPadTickY(1);

	p1->cd();
	g1->SetMarkerStyle(3);
	g1->Draw("LAP");
	p2->cd();
	g2->SetMarkerStyle(3);
	g2->Draw("LAP");

	p3->cd();
	p3->SetLogy(1);
    h1->Draw("LP");
    h2->Draw("LPSAME");
    h3->Draw("LPSAME");
    h4->Draw("LPSAME");

    TLegend *legend = new TLegend(0.8,0.8,1,1);
    legend->AddEntry(h1,hName1);
    legend->AddEntry(h2,hName2);
    legend->AddEntry(h3,hName3);
    legend->AddEntry(h4,hName4);
    legend->Draw("SAME");

	c1->SaveAs(runName+"_rate.png");
	c1->SaveAs(runName+"_rate.pdf");

	g1->Write();
	g2->Write();
	h1->Write();
	h2->Write();
	h3->Write();
	h4->Write();
	h5->Write();
	t->Write();
}
