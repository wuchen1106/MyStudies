double nProtons = 1e10;
double PulseInterval = 1170;

TString runName = "emOT";
TString DirName = "/scratchfs/bes/wuc/MyWorkArea/Data/raw/g4sim/BLTCDC.em.g60cm10mm.005T.1p5_0927_11_p5.g4s.QBH";

double proton_rate = 2.5e12; // Hz
double W_He = 41; // eV
double W_iC4H10 = 23; // eV
double gain = 1e5; // and  <--- smaller than 1e5 due to space charge? 
double cdc_length = 150 ; // cm
double edep2charge = proton_rate/nProtons*(0.9/W_He+0.1/W_iC4H10)*gain*1.6e-19/cdc_length*24*3600; // C/cm/day
double hit2rate = proton_rate/nProtons/1000; // kHz

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

void getRate(){
	TFile *f = 0;

	TChain *c = new TChain("tree");
//	c->Add(runName+".root");
	std::stringstream buff;
	for (int i = 0; i<100; i++){
		buff.str("");
		buff.clear();
		buff<<DirName<<"/"<<i<<"_job0.raw";
		c->Add( buff.str().c_str());
	}
	DirName = "/scratchfs/bes/wuc/MyWorkArea/Data/raw/g4sim/BLTCDC.OT.g60cm10mm.005T.1p5_0927_11_p5.g4s.QBH";
	for (int i = 0; i<100; i++){
		buff.str("");
		buff.clear();
		buff<<DirName<<"/"<<i<<"_job0.raw";
		c->Add( buff.str().c_str());
	}
//	DirName = "/scratchfs/bes/wuc/MyWorkArea/Data/raw/g4sim/CDChit.mum.g60cm10mm.005T.1p5_0927_11_p5.g4s.QBH/raw";
//	for (int i = 0; i<100; i++){
//		buff.str("");
//		buff.clear();
//		buff<<DirName<<"/"<<i<<"_job0.raw";
//		c->Add( buff.str().c_str());
//	}

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

    TH1D *h1 = new TH1D("Innermost Layer","Hit Rate in CDC Caused by Beam Particles (Stopped #mu^{-} #pi^{-} Not Included)",150,0,PulseInterval);
    TH1D *h2 = new TH1D("Seccond Innermost Layer","Hit Rate in CDC Caused by Beam Particles (Stopped #mu^{-} #pi^{-} Not Included)",150,0,PulseInterval);
    TH1D *h3 = new TH1D("Seccond Outmost Layer","Hit Rate in CDC Caused by Beam Particles (Stopped #mu^{-} #pi^{-} Not Included)",150,0,PulseInterval);
    TH1D *h4 = new TH1D("Outmost Layer","Hit Rate in CDC Caused by Beam Particles (Stopped #mu^{-} #pi^{-} Not Included)",150,0,PulseInterval);

	std::vector<double> vLayerID;
	std::vector<double> vHitrate;
	std::vector<double> vCharge;
	for (int i = 0; i < 18; i++){
		vLayerID.push_back(i+1);
		vHitrate.push_back(0);
		vCharge.push_back(0);
	}


	f = new TFile("Curves.s100.root");
	std::cout<<"Integrating..."<<std::endl;
	TH1D *hCurve = (TH1D*) f->Get("ProtonPuls");
//	TH1D *hCurve = (TH1D*) f->Get("Convoluted");
	hCurve->RebinX(20);

	std::vector<int> *C_volID;
	std::vector<double> *C_edep;
	std::vector<double> *C_t;
	double weight;
	c->SetBranchAddress("C_volID",&C_volID);
	c->SetBranchAddress("C_edep",&C_edep);
	c->SetBranchAddress("C_t",&C_t);
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
		if (iEvent>=11094) weight*=100./31;
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
			vHitrate[layerID]+=hit2rate/cellNo[layerID];
			vCharge[layerID]+=edep*edep2charge/cellNo[layerID];
			hitcount[layerID]++;
			foundhit = true;
			if (layerID>layerID_max) layerID_max = layerID;
			for (int i_window = 0; i_window <5; i_window++){
				double newtime = time+hCurve->GetRandom()+i_window*PulseInterval;
				if (layerID==0) h1->Fill(newtime,weight);
				else if (layerID==1) h2->Fill(newtime,weight);
				else if (layerID==17) h3->Fill(newtime,weight);
				else if (layerID==16) h4->Fill(newtime,weight);
			}
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
	g1->SetTitle(runName+": Hit Rate Per Cell (kHz)");
	g1->GetHistogram()->GetXaxis()->SetTitle("Layer ID (1-18)");
	g1->SetName("g1");
	TGraph * g2 = new TGraph(vLayerID.size(),&(vLayerID[0]),&(vCharge[0]));
	g2->GetHistogram()->GetXaxis()->SetTitle("Layer ID (1-18)");
	g2->SetTitle(runName+":Q (C/cm/day)");
	g2->SetName("g2");

	TCanvas *c1 = new TCanvas("c","c");
	TPad * p1 = new TPad("p1","p1",0,0,1./2,0.5);
	TPad * p2 = new TPad("p2","p2",1./2,0,2./2,0.5);
	TPad * p3 = new TPad("p3","p3",0,0.5,1,1);
	p1->Draw();
	p2->Draw();
	p3->Draw();
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
    if (h1) h1->GetYaxis()->SetTitle(ytitle1);
//  if (h1) h1->GetYaxis()->SetTitleOffset(2);
    if (h1) h1->GetXaxis()->SetTitle(xtitle1);
    if (h1) h1->SetMarkerStyle(Style_1);
    if (h1) h1->SetMarkerColor(Color_1);
    if (h1) h1->SetLineColor(Color_1);
    if (h1) h1->Draw("LP");
    if (h2) h2->SetMarkerStyle(Style_2);
    if (h2) h2->SetMarkerColor(Color_2);
    if (h2) h2->SetLineColor(Color_2);
    if (h2) h2->Draw("LPSAME");
    if (h3) h3->SetMarkerStyle(Style_3);
    if (h3) h3->SetMarkerColor(Color_3);
    if (h3) h3->SetLineColor(Color_3);
    if (h3) h3->Draw("LPSAME");
    if (h4) h4->SetMarkerStyle(Style_4);
    if (h4) h4->SetMarkerColor(Color_4);
    if (h4) h4->SetLineColor(Color_4);
    if (h4) h4->Draw("LPSAME");

    TLegend *legend = new TLegend(0.8,0.8,1,1);
    if (h1) legend->AddEntry(h1,hName1);
    if (h2) legend->AddEntry(h2,hName2);
    if (h3) legend->AddEntry(h3,hName3);
    if (h4) legend->AddEntry(h4,hName4);
    legend->Draw("SAME");

	c1->SaveAs(runName+"_rate.png");
	c1->SaveAs(runName+"_rate.pdf");

	g1->Write();
	g2->Write();
	h1->Write();
	h2->Write();
	h3->Write();
	h4->Write();
	t->Write();
}
