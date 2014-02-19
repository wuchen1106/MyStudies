{
	std::stringstream buf;
	TString g4simdata = "/home/chen/MyWorkArea/g4sim/data";
	TString mydata =    "/home/chen/MyWorkArea/Data";
//	TString g4simdata = "/scratchfs/bes/wuc/MyWorkArea/g4simData";
//	TString mydata =    "/scratchfs/bes/wuc/MyWorkArea/Data";
	TString InputFile = "";

//========================================================================================

	// When I generate antiprotons with weight, I falsely used 0.003499 as the Br of antiproton
	// According to new estimation, the Br should be smaller than 4.5e-5

//	bool outputTree = false;
	bool outputTree = true;

	TString ParticleName = "Antiproton";
	TString ParticleNameShort = "AP";

//	double pa_max = 500;
//	double pa_max = 1500;
	double pa_max = 6500;

	double pl_max = 2500;

//	double BrFactor = 1; // Used 4.5e-5 as Br for ap
////	double BrFactor = 4.5e-5/0.003449; // By the time I generated PTACS data, I used 3.449e-3 as Br for ap
//	TString CutName = "";
//	TString MonitorName = "Production";
//	TString TreeName = "t";

//	double BrFactor = 1; // Used 4.5e-5 as Br for ap
//	TString CutName = " (pl>0||pa<1GeV/c)";
//	TString MonitorName = "PTM";
//	TString InputFile = g4simdata+"/PT.AP_all.TH0.root";
//	TString TreeName = "t";
////	BrFactor /= 200; // I repeated 200 times for PTM

//	double BrFactor = 4.5e-5/0.003449; // By the time I generated PTACS data, I used 3.449e-3 as Br for ap
	double BrFactor = 1; // Used 4.5e-5 as Br for ap
	TString CutName = " (Reached PTACS)";
	TString MonitorName = "PTACS";
	TString InputFile = "PTM.AP.histos.root";
	TString TreeName = "tree";
	BrFactor /= 300; // I repeated 300 times for PTACS

//	double BrFactor = 1; // Used 4.5e-5 as Br for ap
//	TString CutName = " (Reached MT1)";
//	TString MonitorName = "MT1";
//	TString TreeName = "tree";
//	BrFactor /= 10; // I repeated 300 times for PTACS

	TChain *c = new TChain(TreeName);

//	// For antiproton at beginning
//	c->Add(g4simdata+"/PT.AP.TH0.root");
//	c->Add(g4simdata+"/PT.AP_all.TH0.root");

	// For antiproton at PTM
//	for (Long64_t i = 0; i<200; i++){
//		buf.str("");
//		buf.clear();
//		buf<<mydata+"/raw/g4sim/PTM.AP_all.g60cm10mm.005T.BL.g4s.QBH/"<<i<<"_job0.raw";
//		c->Add(buf.str().c_str());
//	}
//	c->Add(g4simdata+"/PTM.AP_1GeV.root");

//	// For antiproton at PTACS
	for (Long64_t i = 0; i<300; i++){
		buf.str("");
		buf.clear();
		buf<<mydata+"/raw/g4sim/PTACS.AP.g60cm10mm.005T.BL.g4s.QBH/"<<i<<"_job0.raw";
		c->Add(buf.str().c_str());
	}

//	// For antiproton at MT1
//	c->Add("/home/chen/MyWorkArea/g4sim/output/raw_g4sim.root");

//========================================================================================

	TFile *f_in = 0;
	TH1D *h_ipt0 = 0;
	TH1D *h_ipa0 = 0;
	TH1D *h_ipl0 = 0;
	TH1D *h_opt0 = 0;
	TH1D *h_opa0 = 0;
	TH1D *h_opl0 = 0;
	if (InputFile!= ""){
		f_in = new TFile(InputFile);
		h_ipt0 = (TH1D*)f_in->Get("h_ipt");
		h_ipa0 = (TH1D*)f_in->Get("h_ipa");
		h_ipl0 = (TH1D*)f_in->Get("h_ipl");
		h_opt0 = (TH1D*)f_in->Get("h_opt");
		h_opa0 = (TH1D*)f_in->Get("h_opa");
		h_opl0 = (TH1D*)f_in->Get("h_opl");
	}

	Long64_t pid = -2212;
	double time;
	double x;
	double y;
	double z;
	double opx;
	double opy;
	double opz;
	double ipx;
	double ipy;
	double ipz;
	double px;
	double py;
	double pz;
	double pl;
	double pt;
	double pa;
	double ipl;
	double ipt;
	double ipa;
	double opl;
	double opt;
	double opa;
	double weight;

	std::vector<double> *vt;
	std::vector<double> *vx;
	std::vector<double> *vy;
	std::vector<double> *vz;
	std::vector<double> *vpx;
	std::vector<double> *vpy;
	std::vector<double> *vpz;
	std::vector<double> *vopx;
	std::vector<double> *vopy;
	std::vector<double> *vopz;

	// For antiproton at beginning
	if (TreeName == "t"){
		c->SetBranchAddress("t",&time);
		c->SetBranchAddress("px",&px);
		c->SetBranchAddress("py",&py);
		c->SetBranchAddress("pz",&pz);
		c->SetBranchAddress("x",&x);
		c->SetBranchAddress("y",&y);
		c->SetBranchAddress("z",&z);
		c->SetBranchAddress("opx",&opx);
		c->SetBranchAddress("opy",&opy);
		c->SetBranchAddress("opz",&opz);
	}
	else if (TreeName == "tree"){
		c->SetBranchAddress("V_t",&vt);
		c->SetBranchAddress("V_x",&vx);
		c->SetBranchAddress("V_y",&vy);
		c->SetBranchAddress("V_z",&vz);
		c->SetBranchAddress("V_px",&vpx);
		c->SetBranchAddress("V_py",&vpy);
		c->SetBranchAddress("V_pz",&vpz);
		c->SetBranchAddress("V_opx",&vopx);
		c->SetBranchAddress("V_opy",&vopy);
		c->SetBranchAddress("V_opz",&vopz);
	}
	c->SetBranchAddress("weight",&weight);
	if (MonitorName!="Production"){
		c->SetBranchAddress("ipx",&ipx);
		c->SetBranchAddress("ipy",&ipy);
		c->SetBranchAddress("ipz",&ipz);
	}

	TFile * f_out = new TFile(MonitorName+"."+ParticleNameShort+".root","RECREATE");

	TTree * pTTree = 0;
	if (outputTree){
		pTTree = new TTree("t","t");
		pTTree->Branch("t",&time);
		pTTree->Branch("weight",&weight);
		pTTree->Branch("pid",&pid);
		pTTree->Branch("x",&x);
		pTTree->Branch("y",&y);
		pTTree->Branch("z",&z);
		pTTree->Branch("px",&px);
		pTTree->Branch("py",&py);
		pTTree->Branch("pz",&pz);
		pTTree->Branch("opx",&opx);
		pTTree->Branch("opy",&opy);
		pTTree->Branch("opz",&opz);
		if (MonitorName!="Production"){
			pTTree->Branch("ipx",&ipx);
			pTTree->Branch("ipy",&ipy);
			pTTree->Branch("ipz",&ipz);
		}
	}

	TH2D * h_opaVSipa = new TH2D("h_opaVSipa",ParticleName+" @ "+MonitorName+CutName,300,0,pa_max,300,0,pa_max);
	h_opaVSipa->GetXaxis()->SetTitle("Momentum Amplitude (MeV/c) @ Production");
	h_opaVSipa->GetYaxis()->SetTitle("Momentum Amplitude (MeV/c) @ Target Surface");
	h_opaVSipa->GetYaxis()->SetTitleOffset(1.2);

	TH2D * h_oplVSipl = new TH2D("h_oplVSipl",ParticleName+" @ "+MonitorName+CutName,300,-pa_max,pl_max,300,-pa_max,pl_max);
	h_oplVSipl->GetXaxis()->SetTitle("Longitudinal Momentum (MeV/c) @ Production");
	h_oplVSipl->GetYaxis()->SetTitle("Longitudinal Momentum (MeV/c) @ Target Surface");
	h_oplVSipl->GetYaxis()->SetTitleOffset(1.2);

	TH2D * h_ipaVSipl = new TH2D("h_ipaVSipl",ParticleName+" @ "+MonitorName+CutName,300,0,pa_max,300,-pa_max,pl_max);
	h_ipaVSipl->GetXaxis()->SetTitle("Momentum Amplitude (MeV/c) @ Target Surface");
	h_ipaVSipl->GetYaxis()->SetTitle("Longitudinal Momentum (MeV/c) @ Target Surface");
	h_ipaVSipl->GetYaxis()->SetTitleOffset(1.2);

	TH2D * h_opaVSopl = new TH2D("h_opaVSopl",ParticleName+" @ "+MonitorName+CutName,300,0,pa_max,300,-pa_max,pl_max);
	h_opaVSopl->GetXaxis()->SetTitle("Momentum Amplitude (MeV/c) @ Production");
	h_opaVSopl->GetYaxis()->SetTitle("Longitudinal Momentum (MeV/c) @ Production");
	h_opaVSopl->GetYaxis()->SetTitleOffset(1.2);

	TH1D * h_opa = new TH1D("h_opa",ParticleName+" @ "+MonitorName+CutName,300,0,pa_max);
	h_opa->GetXaxis()->SetTitle("Momentum Amplitude (MeV/c) @ Production");
	h_opa->GetYaxis()->SetTitle("Count per Incident Proton");

	TH1D * h_opl = new TH1D("h_opl",ParticleName+" @ "+MonitorName+CutName,300,-pa_max,pl_max);
	h_opl->GetXaxis()->SetTitle("Longitudinal Momentum (MeV/c) @ Production");
	h_opl->GetYaxis()->SetTitle("Count per Incident Proton");

	TH1D * h_ipa = new TH1D("h_ipa",ParticleName+" @ "+MonitorName+CutName,300,0,pa_max);
	h_ipa->GetXaxis()->SetTitle("Momentum Amplitude (MeV/c) @ Target Surface");
	h_ipa->GetYaxis()->SetTitle("Count per Incident Proton");

	TH1D * h_ipl = new TH1D("h_ipl",ParticleName+" @ "+MonitorName+CutName,300,-pa_max,pl_max);
	h_ipl->GetXaxis()->SetTitle("Longitudinal Momentum (MeV/c) @ Target Surface");
	h_ipl->GetYaxis()->SetTitle("Count per Incident Proton");

//	TFile * filetemp = new TFile("PTM.AP.histos.root");
//	h_ipl = (TH1D*) filetemp->Get("h_ipl");
//	h_ipa = (TH1D*) filetemp->Get("h_ipa");
//	h_opl = (TH1D*) filetemp->Get("h_opl");
//	h_opa = (TH1D*) filetemp->Get("h_opa");

	Long64_t n = c->GetEntries();
	std::cout<<n<<" events to be processed"<<std::endl;
	for (Long64_t i = 0; i<n; i++){
		c->GetEntry(i);
		if (i%10000==0)
			std::cout<<((double)i)/((double)n)*100<<"%..."<<std::endl;

		if (TreeName == "tree"){
			if (!vpx||vpx->size()==0) continue;
			time = (*vt)[0];
			x = (*vx)[0]*10;
			y = (*vy)[0]*10;
			z = (*vz)[0]*10;
			px = (*vpx)[0]*1000;
			py = (*vpy)[0]*1000;
			pz = (*vpz)[0]*1000;
			opx = (*vopx)[0]*1000;
			opy = (*vopy)[0]*1000;
			opz = (*vopz)[0]*1000;
		}

		weight *= BrFactor;

		if (MonitorName=="Production"){ // opx=px for Production
			opx=px;
			opy=py;
			opz=pz;
		}
		else if (MonitorName=="PTM"){
			ipx=px;
			ipy=py;
			ipz=pz;
		}

		if (MonitorName=="MT1"){
			pl=pz;
			pt=sqrt(px*px+py*py);
			pa=sqrt(pt*pt+pz*pz);
		}
		else{
			pl=-px;
			pt=sqrt(py*py+pz*pz);
			pa=sqrt(pt*pt+px*px);
		}

	//	if (time<500) continue;
	//	if (pa>1000) continue;

		opt=sqrt(opy*opy+opz*opz);
		opa=sqrt(opt*opt+opx*opx);
		opl=-opx;

		ipt=sqrt(ipy*ipy+ipz*ipz);
		ipa=sqrt(ipt*ipt+ipx*ipx);
		ipl=-ipx;

		h_ipaVSipl->Fill(ipa,ipl,weight);
		h_opaVSopl->Fill(opa,opl,weight);
		h_opaVSipa->Fill(opa,ipa,weight);
		h_oplVSipl->Fill(opl,ipl,weight);
		h_ipa->Fill(ipa,weight);
		h_ipl->Fill(ipl,weight);
		h_opa->Fill(opa,weight);
		h_opl->Fill(opl,weight);

		if (pTTree)
			pTTree->Fill();
	}

	if (pTTree)
		pTTree->Write();

	gStyle->SetPalette(1);
	gStyle->SetPadTickX(1);
	gStyle->SetPadTickY(1);

	if (MonitorName!="Production"){
		TCanvas *pTCanvas = new TCanvas("c1","c1");
		gPad->SetGridx(1);
		gPad->SetGridy(1);
		h_opaVSipa->SetContour(50);
		h_opaVSipa->Draw("COLZ");
		h_opaVSipa->Write();

		pTCanvas = new TCanvas("c2","c2");
		gPad->SetGridx(1);
		gPad->SetGridy(1);
		h_oplVSipl->SetContour(50);
		h_oplVSipl->Draw("COLZ");
		h_oplVSipl->Write();

		pTCanvas = new TCanvas("c4","c4");
		gPad->SetGridx(1);
		gPad->SetGridy(1);
		h_ipaVSipl->SetContour(50);
		h_ipaVSipl->Draw("COLZ");
		h_ipaVSipl->Write();

		pTCanvas = new TCanvas("c7","c7");
		gPad->SetGridx(1);
		gPad->SetGridy(1);
		gPad->SetLogy(1);
		if (h_opl0){
			h_opl0->SetLineColor(kBlack);
			h_opl0->GetXaxis()->SetTitle("Longitudinal Momentum (MeV/c)");
			h_opl0->SetTitle("Antiproton @ Production VS Antiproton @ "+MonitorName);
			h_opl0->Draw();
			h_ipl->SetLineColor(kRed);
			h_ipl->Draw("SAME");
			TLegend * l = new TLegend(0.7,0.5,0.9,0.7);
			l->AddEntry(h_opl0,"@ Production");
			l->AddEntry(h_ipl,"@ "+MonitorName);
			l->Draw("SAME");
		}
		else{
			h_ipl->Draw();
		}
		h_ipl->Write();

		pTCanvas = new TCanvas("c8","c8");
		gPad->SetGridx(1);
		gPad->SetGridy(1);
		gPad->SetLogy(1);
		if (h_opa0){
			h_opa0->SetLineColor(kBlack);
			h_opa0->GetXaxis()->SetTitle("Momentum Amplitude (MeV/c)");
			h_opa0->SetTitle("Antiproton @ Production VS Antiproton @ "+MonitorName);
			h_opa0->Draw();
			h_ipa->SetLineColor(kRed);
			h_ipa->Draw("SAME");
			TLegend * l = new TLegend(0.7,0.5,0.9,0.7);
			l->AddEntry(h_opa0,"@ Production");
			l->AddEntry(h_ipa,"@ "+MonitorName);
			l->Draw("SAME");
		}
		else{
			h_ipa->Draw();
		}
		h_ipa->Write();

		if (h_ipl0){
			pTCanvas = new TCanvas("c9","c9");
			gPad->SetGridx(1);
			gPad->SetGridy(1);
			gPad->SetLogy(1);
			h_ipl0->SetLineColor(kBlack);
			h_ipl0->GetXaxis()->SetTitle("Longitudinal Momentum (MeV/c)");
			h_ipl0->SetTitle("Antiproton Longitudinal Momentum @ Target Surface");
			h_ipl0->Draw();
			h_ipl->SetLineColor(kRed);
			h_ipl->Draw("SAME");
			TLegend * l = new TLegend(0.7,0.5,0.9,0.7);
			l->AddEntry(h_ipl0,"Antiprotons Reached Target Surface");
			l->AddEntry(h_ipl,"Antiprotons Reached "+MonitorName);
			l->Draw("SAME");
		}

		if (h_ipa0){
			pTCanvas = new TCanvas("c10","c10");
			gPad->SetGridx(1);
			gPad->SetGridy(1);
			gPad->SetLogy(1);
			h_ipa0->SetLineColor(kBlack);
			h_ipa0->GetXaxis()->SetTitle("Momentum Amplitude (MeV/c)");
			h_ipa0->SetTitle("Antiproton Momentum Amplitude @ Target Surface");
			h_ipa0->Draw();
			h_ipa->SetLineColor(kRed);
			h_ipa->Draw("SAME");
			TLegend * l = new TLegend(0.7,0.5,0.9,0.7);
			l->AddEntry(h_ipa0,"Antiprotons Reached Target Surface");
			l->AddEntry(h_ipa,"Antiprotons Reached "+MonitorName);
			l->Draw("SAME");
		}
	}

	pTCanvas = new TCanvas("c3","c3");
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	h_opaVSopl->SetContour(50);
	h_opaVSopl->Draw("COLZ");
	h_opaVSopl->Write();

	pTCanvas = new TCanvas("c5","c5");
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	gPad->SetLogy(1);
	if (h_opl0){
		h_opl0->SetLineColor(kBlack);
		h_opl0->GetXaxis()->SetTitle("Longitudinal Momentum (MeV/c)@ Production");
		h_opl0->SetTitle("Antiproton Longitudinal Momentum @ Production");
		h_opl0->Draw();
		h_opl->SetLineColor(kRed);
		h_opl->Draw("SAME");
		TLegend * l = new TLegend(0.7,0.5,0.9,0.7);
		l->AddEntry(h_opl0,"All Antiprotons");
		l->AddEntry(h_opl,"Antiprotons Reached "+MonitorName);
		l->Draw("SAME");
	}
	else{
		h_opl->Draw();
	}
	h_opl->Write();

	pTCanvas = new TCanvas("c6","c6");
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	gPad->SetLogy(1);
	if (h_opa0){
		h_opa0->SetLineColor(kBlack);
		h_opa0->GetXaxis()->SetTitle("Momentum Amplitude (MeV/c)@ Production");
		h_opa0->SetTitle("Antiproton Momentum Amplitude @ Production");
		h_opa0->Draw();
		h_opa->SetLineColor(kRed);
		h_opa->Draw("SAME");
		TLegend * l = new TLegend(0.7,0.5,0.9,0.7);
		l->AddEntry(h_opa0,"All Antiproton");
		l->AddEntry(h_opa,"Antiprotons Reached "+MonitorName);
		l->Draw("SAME");
	}
	else{
		h_opa->Draw();
	}
	h_opa->Write();
}
