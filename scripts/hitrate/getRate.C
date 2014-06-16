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

void getRate(){

	// About Drift time
	int nSample = 10;
	TFile * f_drifttime = TFile::Open("drifttime.root");
	TH2D * tminVStmax = (TH2D*) f_drifttime->Get("tminVStmax");

	// About this run
	TString parName = "OT";
	TString suffixName = "0508_100cm_1e9";
	TString runName = parName+"."+suffixName;
	std::vector<TString> DirName;
	std::vector<int> nRuns;
	std::vector<TString> FileNames;
	 // ########Should Modify#########
//	FileNames.push_back("result/OT.root");
//	FileNames.push_back(runName+".root");
	//DirName.push_back("/scratchfs/bes/wuc/MyWorkArea/Data/raw/g4sim/BLTCDC.em.g60cm10mm.005T.1p5_0927_11_p5.g4s.QBH");
	//nRuns.push_back(100);
	//DirName.push_back("/scratchfs/bes/wuc/MyWorkArea/Data/raw/g4sim/BLTCDC.OT.g60cm10mm.005T.1p5_0927_11_p5.g4s.QBH");
	//nRuns.push_back(100);
	//DirName.push_back("/scratchfs/bes/wuc/MyWorkArea/Data/raw/g4sim/BLTCDC.mum.g60cm10mm.005T.1p5_0927_11_p5.g4s.QBH");
	//nRuns.push_back(100);
	DirName.push_back(MyData+"/CDCHit."+parName+".g60cm10mm.005T."+suffixName+".g4s.QBH");
	nRuns.push_back(100);
//	DirName.push_back(MyData+"/raw/g4sim/CDCHit.pim.g60cm10mm.005T.0508.g4s.QBH");
//	nRuns.push_back(50);
	double nProtons = 1e8;
	if (parName == "pim")
		nProtons = 1e9;
	else if (parName == "OT")
		nProtons = 1e8;
	nProtons = 1e9;
//	nProtons*=19./20;
	 // ########Should Modify#########

	// Beam Structure
	double PulseInterval = 1170; // ns
	double dutyFactor = 1./2.4;
	double proton_rate = 2.5e12; // Hz
	double left_end = 700; // ns
	double duration = 400*2; // ns
	TFile * f = new TFile("result/Curves.s100.root");

	 // ########Should Modify#########
	TH1D *hCurve = (TH1D*) f->Get("ProtonPuls"); // for beam particles and stopped pions
	//TH1D *hCurve = (TH1D*) f->Get("Convoluted"); // for stopped muons
	 // ########Should Modify#########
//	hCurve->RebinX(20);

	// CDC Info
	double W_He = 41; // eV
	double W_iC4H10 = 23; // eV
	double gain = 1e5; // and  <--- smaller than 1e5 due to space charge? 
	double cdc_length = 150 ; // cm
	double edep2charge = proton_rate/nProtons*(0.9/W_He+0.1/W_iC4H10)*gain*1.6e-19/cdc_length*24*3600*1e3; // mC/cm/day
	double hit2rate = proton_rate/nProtons/1000; // kHz
	double scale2bunch = proton_rate*PulseInterval*1e-9/nProtons/dutyFactor;
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
    
    TString xtitle1 = "Signal Arrival Time (ns)";
    TString xtitle2 = "Hit Time (ns)";
    buff.str("");
    buff.clear();
    buff<<"nHits/Bunch/bin("<<PulseInterval/100<<"ns)";
    TString ytitle1 = buff.str().c_str();
    TString ytitle2 = buff.str().c_str();

    TString hName1 = "Innermost Layer";
    TString hName2 = "Seccond Innermost Layer";
    TString hName3 = "Seccond Outmost Layer";
    TString hName4 = "Outmost Layer";

	TString title = "Signal Arrival Time of Noise in CDC";
	TString title2 = "Hit Time of Noise in CDC";

	// hit rate in different cdc layers
    TH1D * h[18];
    for ( int i =0; i <18; i++){
    	buff.str("");
    	buff.clear();
    	buff<<"hOccu"<<i;
		h[i] = new TH1D(buff.str().c_str(),title,100,0,PulseInterval);
		h[i]->GetYaxis()->SetTitle(ytitle1);
		h[i]->GetXaxis()->SetTitle(xtitle1);
		h[i]->SetMarkerSize(0.5);
		h[i]->SetMarkerStyle(Style_1);
    }
    h[0]->SetMarkerStyle(Style_1);
    h[0]->SetMarkerColor(Color_1);
    h[0]->SetLineColor(Color_1);
    h[1]->SetMarkerStyle(Style_2);
    h[1]->SetMarkerColor(Color_2);
    h[1]->SetLineColor(Color_2);
    h[2]->SetMarkerStyle(Style_3);
    h[2]->SetMarkerColor(Color_3);
    h[2]->SetLineColor(Color_3);
    h[3]->SetMarkerStyle(Style_4);
    h[3]->SetMarkerColor(Color_4);
    h[3]->SetLineColor(Color_4);
    TH1D * h1[18];
    for ( int i =0; i <18; i++){
    	buff.str("");
    	buff.clear();
    	buff<<"hRate_"<<i;
		h1[i] = new TH1D(buff.str().c_str(),title2,100,0,PulseInterval);
		h1[i]->GetYaxis()->SetTitle(ytitle2);
		h1[i]->GetXaxis()->SetTitle(xtitle2);
		h1[i]->SetMarkerSize(0.5);
		h1[i]->SetMarkerStyle(Style_1);
    }
    h1[0]->SetMarkerStyle(Style_1);
    h1[0]->SetMarkerColor(Color_1);
    h1[0]->SetLineColor(Color_1);
    h1[1]->SetMarkerStyle(Style_2);
    h1[1]->SetMarkerColor(Color_2);
    h1[1]->SetLineColor(Color_2);
    h1[2]->SetMarkerStyle(Style_3);
    h1[2]->SetMarkerColor(Color_3);
    h1[2]->SetLineColor(Color_3);
    h1[3]->SetMarkerStyle(Style_4);
    h1[3]->SetMarkerColor(Color_4);
    h1[3]->SetLineColor(Color_4);
	TString title3 = "Momentum of Noise Tracks in CDC";
    TH1D *h5 = new TH1D("h5",title3,200,-9,4);
	h5->GetYaxis()->SetTitle("Count");
	h5->GetXaxis()->SetTickLength(0);
	h5->GetXaxis()->SetTitleOffset(3);
	h5->GetXaxis()->SetLabelOffset(3);
//    h5->SetMarkerStyle(Style_4);
//    h5->SetMarkerColor(Color_4);
//    h5->SetLineColor(Color_4);

	/*
	// hit rate in Upstream Trigger counter
    TH1D *h11 = new TH1D("h11",title,100,0,PulseInterval);
    h11->GetYaxis()->SetTitle(ytitle1);
    h11->GetXaxis()->SetTitle(xtitle1);
    h11->SetMarkerStyle(Style_1);
    h11->SetMarkerSize(0.5);
    h11->SetMarkerColor(Color_1);
    h11->SetLineColor(Color_1);
    TH1D *h12 = new TH1D("h12",title,100,0,PulseInterval);
    h12->SetMarkerStyle(Style_2);
    h12->SetMarkerSize(0.5);
    h12->SetMarkerColor(Color_2);
    h12->SetLineColor(Color_2);
    TH1D *h13 = new TH1D("h13",title,100,0,PulseInterval);
    h13->SetMarkerStyle(Style_3);
    h13->SetMarkerSize(0.5);
    h13->SetMarkerColor(Color_3);
    h13->SetLineColor(Color_3);

	// hit rate in Downstream Trigger counter
    TH1D *h21 = new TH1D("h21",title,100,0,PulseInterval);
    h21->GetYaxis()->SetTitle(ytitle1);
    h21->GetXaxis()->SetTitle(xtitle1);
    h21->SetMarkerStyle(Style_1);
    h21->SetMarkerSize(0.5);
    h21->SetMarkerColor(Color_1);
    h21->SetLineColor(Color_1);
    TH1D *h22 = new TH1D("h22",title,100,0,PulseInterval);
    h22->SetMarkerStyle(Style_2);
    h22->SetMarkerSize(0.5);
    h22->SetMarkerColor(Color_2);
    h22->SetLineColor(Color_2);
    TH1D *h23 = new TH1D("h23",title,100,0,PulseInterval);
    h23->SetMarkerStyle(Style_3);
    h23->SetMarkerSize(0.5);
    h23->SetMarkerColor(Color_3);
    h23->SetLineColor(Color_3);

	// Momentum Distribution in Upstream Trigger counter
    TH1D *h31 = new TH1D("h31",title,100,0,PulseInterval);
    h31->GetYaxis()->SetTitle(ytitle1);
    h31->GetXaxis()->SetTitle(xtitle1);
    h31->SetMarkerStyle(Style_1);
    h31->SetMarkerSize(0.5);
    h31->SetMarkerColor(Color_1);
    h31->SetLineColor(Color_1);
    TH1D *h32 = new TH1D("h32",title,100,0,PulseInterval);
    h32->SetMarkerStyle(Style_2);
    h32->SetMarkerSize(0.5);
    h32->SetMarkerColor(Color_2);
    h32->SetLineColor(Color_2);
    TH1D *h33 = new TH1D("h33",title,100,0,PulseInterval);
    h33->SetMarkerStyle(Style_3);
    h33->SetMarkerSize(0.5);
    h33->SetMarkerColor(Color_3);
    h33->SetLineColor(Color_3);

	// Momentum Distribution in Downstream Trigger counter
    TH1D *h41 = new TH1D("h41",title,100,0,PulseInterval);
    h41->GetYaxis()->SetTitle(ytitle1);
    h41->GetXaxis()->SetTitle(xtitle1);
    h41->SetMarkerStyle(Style_1);
    h41->SetMarkerSize(0.5);
    h41->SetMarkerColor(Color_1);
    h41->SetLineColor(Color_1);
    TH1D *h42 = new TH1D("h42",title,100,0,PulseInterval);
    h42->SetMarkerStyle(Style_2);
    h42->SetMarkerSize(0.5);
    h42->SetMarkerColor(Color_2);
    h42->SetLineColor(Color_2);
    TH1D *h43 = new TH1D("h43",title,100,0,PulseInterval);
    h43->SetMarkerStyle(Style_3);
    h43->SetMarkerSize(0.5);
    h43->SetMarkerColor(Color_3);
    h43->SetLineColor(Color_3);
    */

	std::vector<double> vLayerID;
	std::vector<double> vHitrate;
	std::vector<double> vHitCount;
	std::vector<double> vCharge;
	std::vector<double> vEx;
	std::vector<double> vEyHitCount;
	std::vector<double> vEyHitrate;
	std::vector<double> vEyChar;
	std::vector<int> vEntries;
	for (int i = 0; i < 18; i++){
		vLayerID.push_back(i+1);
		vHitrate.push_back(0);
		vHitCount.push_back(0);
		vCharge.push_back(0);
		vEx.push_back(0);
		vEyHitCount.push_back(0);
		vEyHitrate.push_back(0);
		vEyChar.push_back(0);
		vEntries.push_back(0);
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
		if (iEvent%1000==0) std::cout<<(double)iEvent/nEvents*100<<" % ..."<<std::endl;
		c->GetEntry(iEvent);
//		if (iEvent>=11094) weight*=100./31;
		int nHits = C_edep->size();
		int hitcount[18] = {0};
		bool foundhit = false;
		int layerID_max = 0;
		double pa = -1;
		double tmin, tmax;
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
//			if (iEvent%1==0){
//				std::cout<<volID<<std::endl;
//			}
			vHitrate[layerID]+=weight*hit2rate/cellNo[layerID];
			vCharge[layerID]+=edep*edep2charge/cellNo[layerID];
			hitcount[layerID]++;
			vEntries[layerID]++;
			foundhit = true;
			if (layerID>layerID_max) layerID_max = layerID;
			double newtime = time+hCurve->GetRandom();
			newtime -= ((int)(newtime/PulseInterval))*PulseInterval;
			if (newtime<0) newtime += PulseInterval;
			for (int iSample = 0; iSample < nSample; iSample++){
				tminVStmax->GetRandom2(tmin,tmax);
				double tarrival = newtime + tmin;
				double tstop = newtime + tmax;
				tarrival -= ((int)(tarrival/PulseInterval))*PulseInterval;
				tstop -= ((int)(tstop/PulseInterval))*PulseInterval;
				h[layerID]->Fill(tarrival,weight/nSample);
				bool gotthishit = false;
				if (left_end+duration>PulseInterval){
					if (tarrival>left_end) gotthishit = true;
					else if (tarrival<left_end+duration-PulseInterval) gotthishit = true;
					else if ((tstop>tarrival&&tstop>left_end)||(tstop<tarrival)) gotthishit = true;
				}
				if (gotthishit) vHitCount[layerID]+=weight/nSample;
			}
			h1[layerID]->Fill(newtime,weight);
		}
		if (nHits>0){
			double px = (*C_px)[0]*1000;
			double py = (*C_py)[0]*1000;
			double pz = (*C_pz)[0]*1000;
			double pa = sqrt(px*px+py*py+pz*pz);
//			if (pa>10)
				h5->Fill(log(pa),weight);
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
	double maxHits = 0;
	double maxCha = 0;
	double maxRate= 0;
	for ( int ilayer  =0; ilayer<18; ilayer++){
		h[ilayer]->Scale(scale2bunch);
		h1[ilayer]->Scale(scale2bunch);
		vHitCount[ilayer]*=scale2bunch;
		if (vEntries[ilayer]){
			vEyHitrate[ilayer] = sqrt(1./vEntries[ilayer])*vHitrate[ilayer];
			vEyHitCount[ilayer] = sqrt(1./vEntries[ilayer])*vHitCount[ilayer];
			vEyChar[ilayer] = sqrt(1./vEntries[ilayer])*vCharge[ilayer];
		}
		if (maxCha<vCharge[ilayer]*1.2) maxCha = vCharge[ilayer]*1.2;
		if (maxHits<vHitCount[ilayer]*1.2) maxHits = vHitCount[ilayer]*1.2;
		if (maxRate<vHitrate[ilayer]*1.2) maxRate = vHitrate[ilayer]*1.2;
	}
//	maxHits = 2.6;
//	maxCha = 0.17;
//	maxRate = 0.17;
//	if (parName == "pim"){
//		maxHits = 0.32;
//		maxCha = 0.06;
//		maxRate = 0.06;
//	}

	TGraphErrors * g1 = new TGraphErrors(vLayerID.size(),&(vLayerID[0]),&(vHitCount[0]),&(vEx[0]),&(vEyHitCount[0]));
	buff.str("");
	buff.clear();
	buff<<"nHits per Bunch in Each Layer in Measure Window ("<<left_end<<","<<left_end+duration<<")ns";
	g1->SetTitle(buff.str().c_str());
	g1->GetHistogram()->GetXaxis()->SetTitle("Layer ID (1-18)");
	g1->GetHistogram()->GetYaxis()->SetTitle("nHits/Bunch");
	g1->SetName("g1");
	TGraphErrors * g2 = new TGraphErrors(vLayerID.size(),&(vLayerID[0]),&(vCharge[0]),&(vEx[0]),&(vEyChar[0]));
	g2->GetHistogram()->GetXaxis()->SetTitle("Layer ID (1-18)");
	g2->GetHistogram()->GetYaxis()->SetTitle("nHits/Bunch");
	g2->SetTitle("Charge Accumulating Speed in Each Layer (mC/cm/day/wire)");
	g2->SetName("g2");
	TGraphErrors * g3 = new TGraphErrors(vLayerID.size(),&(vLayerID[0]),&(vHitrate[0]),&(vEx[0]),&(vEyHitrate[0]));
	g3->GetHistogram()->GetXaxis()->SetTitle("Layer ID (1-18)");
	g3->GetHistogram()->GetYaxis()->SetTitle("Hit Rate [kHz]");
	g3->SetTitle("Hit Rate in Each Layer over All Time");
	g3->SetName("g3");

	TCanvas *c1 = new TCanvas("c1","c1",768,512);
	TPad * p1 = new TPad("p1","p1",0   ,0   ,1   ,1./2);
	TPad * p2 = new TPad("p2","p2",0   ,1./2,1   ,1   );
	p1->Draw();
	p2->Draw();
	p1->SetGridx(1);
	p1->SetGridy(1);
	p2->SetGridx(1);
	p2->SetGridy(1);
	gStyle->SetPalette(1);
	gStyle->SetOptStat(0);
	gStyle->SetPadTickX(1);
	gStyle->SetPadTickY(1);

	p1->cd();
//	p1->SetLogy(1);
//	h[0]->GetYaxis()->SetRangeUser(0,maxHits);
	h[0]->GetXaxis()->SetTitleSize(0.05);
	h[0]->GetXaxis()->SetTitleOffset(0.8);
	h[0]->GetYaxis()->SetTitleSize(0.06);
	h[0]->GetYaxis()->SetTitleOffset(0.5);
    h[0]->Draw("LP");
    h[1]->Draw("LPSAME");
    h[2]->Draw("LPSAME");
    h[3]->Draw("LPSAME");

	p2->cd();
//	p2->SetLogy(1);
//	h1[0]->GetYaxis()->SetRangeUser(0,maxHits);
	h1[0]->GetXaxis()->SetTitleSize(0.05);
	h1[0]->GetXaxis()->SetTitleOffset(0.8);
	h1[0]->GetYaxis()->SetTitleSize(0.06);
	h1[0]->GetYaxis()->SetTitleOffset(0.5);
    h1[0]->Draw("LP");
    h1[1]->Draw("LPSAME");
    h1[2]->Draw("LPSAME");
    h1[3]->Draw("LPSAME");

    TLegend *legend = new TLegend(0.8,0.6,1,0.9);
    legend->AddEntry(h[0],hName1);
    legend->AddEntry(h[1],hName2);
    legend->AddEntry(h[2],hName3);
    legend->AddEntry(h[3],hName4);
    legend->Draw("SAME");

	c1->SaveAs(runName+"_time.png");
	c1->SaveAs(runName+"_time.pdf");

	TCanvas *c2 = new TCanvas("c2","c2",768,768);
	TPad * p3 = new TPad("p3","p3",0   ,0   ,1   ,1./3);
	TPad * p4 = new TPad("p4","p4",0   ,1./3,1   ,2./3);
	TPad * p5 = new TPad("p5","p5",0   ,2./3,1   ,1   );
	p3->Draw();
	p4->Draw();
	p5->Draw();
	p3->SetGridx(1);
	p3->SetGridy(1);
	p4->SetGridx(1);
	p4->SetGridy(1);
	p5->SetGridx(1);
	p5->SetGridy(1);

	p3->cd();
	g2->SetMarkerStyle(21);
	g2->SetMarkerSize(0.5);
	g2->GetYaxis()->SetRangeUser(0,maxCha);
	g2->GetXaxis()->SetTitleSize(0.05);
	g2->GetXaxis()->SetTitleOffset(0.8);
	g2->GetYaxis()->SetTitleSize(0.06);
	g2->GetYaxis()->SetTitleOffset(0.5);
	g2->Draw("LAP");
	p4->cd();
	g3->SetMarkerStyle(21);
	g3->SetMarkerSize(0.5);
	g3->GetYaxis()->SetRangeUser(0,maxRate);
	g3->GetXaxis()->SetTitleSize(0.05);
	g3->GetXaxis()->SetTitleOffset(0.8);
	g3->GetYaxis()->SetTitleSize(0.06);
	g3->GetYaxis()->SetTitleOffset(0.5);
	g3->Draw("LAP");
	p5->cd();
	g1->SetMarkerStyle(21);
	g1->SetMarkerSize(0.5);
	g1->GetYaxis()->SetRangeUser(0,maxHits);
	g1->GetXaxis()->SetTitleSize(0.05);
	g1->GetXaxis()->SetTitleOffset(0.8);
	g1->GetYaxis()->SetTitleSize(0.06);
	g1->GetYaxis()->SetTitleOffset(0.5);
	g1->Draw("LAP");

	c2->SaveAs(runName+"_bunch.png");
	c2->SaveAs(runName+"_bunch.pdf");

	TCanvas *c3 = new TCanvas("c3","c3",768,512);
//	gPad->SetLogy(1);
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	gStyle->SetOptStat(1);
	h5->Draw();
	TGaxis * axis = new TGaxis(-9,0,4,0,exp(-9),exp(4),50510,"G");
	axis->SetTitle("Momentum [MeV/c]");
	axis->Draw();

	c3->SaveAs(runName+"_mom.png");
	c3->SaveAs(runName+"_mom.pdf");

	g1->Write();
	g2->Write();
	g3->Write();
	for ( int i = 0; i< 18; i++){
		h[i]->Write();
		h1[i]->Write();
	}
	h5->Write();
	t->Write();
}
