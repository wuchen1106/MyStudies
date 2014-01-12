{
	TFile *f = 0;
	TLegend * legend;

	double time_right = 1150;
	double time_left = 700;
	double tSep = 1170;

	TChain *c = new TChain("tree");

	bool withStopPosition = true;
	double PulseInterval = 1170;

	f = new TFile("Curves.s100.root");
	std::cout<<"Integrating..."<<std::endl;

//	double minimum = 1e-11;
//	TH1D *hCurve = (TH1D*) f->Get("Convoluted");
//	double NperP = 860498./1e8;
//	TString parName = "mu";
//	TString DirName = "/scratchfs/bes/wuc/MyWorkArea/Data/raw/g4sim/BLTCDC.mum.g60cm10mm.005T.1p5_0927_11_p5.g4s.QBH";

	double minimum = 1e-21;
	TH1D *hCurve = (TH1D*) f->Get("ProtonPuls");
	double NperP = 706288./1e9;
	TString parName = "pi";
	TString DirName = "/scratchfs/bes/wuc/MyWorkArea/Data/raw/g4sim/BLTCDC.pim.g60cm10mm.005T.1p5_0927_11_p5.g4s.QBH";

	hCurve->RebinX(20);
	TString par = "#"+parName+"^{-}";
	TString runName = parName+"on.11.p5";
	TString option = "Rmin = 11 cm, L = 50 cm";

	std::stringstream buff;
	for (int i = 0; i<10; i++){
		buff.str("");
		buff.clear();
		buff<<DirName<<"/"<<i<<"_job0.raw";
		c->Add( buff.str().c_str());
	}

	TH2D * h01 = new TH2D("h01",par+" Before Collimator",50,0,180,50,-200,200);
	h01->GetXaxis()->SetTitle("Momentum Amplitude (MeV/c)");
	h01->GetYaxis()->SetTitle("y position (mm)");
	h01->GetYaxis()->SetTitleOffset(1.5);
	TH2D * h02 = new TH2D("h02",par+" After Collimator",50,0,180,50,-200,200);
	h02->GetXaxis()->SetTitle("Momentum Amplitude (MeV/c)");
	h02->GetYaxis()->SetTitle("y position (mm)");
	h02->GetYaxis()->SetTitleOffset(1.5);
	TH2D * h03 = new TH2D("h03",par+" Stopped in Target",50,0,180,50,-200,200);
	h03->GetXaxis()->SetTitle("Momentum Amplitude (MeV/c)");
	h03->GetYaxis()->SetTitle("y position (mm)");
	h03->GetYaxis()->SetTitleOffset(1.5);

	TH1D * h04 = new TH1D("h04",par+" Transverse Distribution",100,0,100);
	h04->GetXaxis()->SetTitle("Radius (mm)");
	h04->GetYaxis()->SetTitle("count/p^{+}");
	h04->GetYaxis()->SetTitleOffset(1.5);
//	TH1D * h05 = new TH1D("h05",par+" Longitudinal Distribution",200,-900,900);
	TH1D * h05 = new TH1D("h05",par+" Longitudinal Distribution",200,-1000,0);
	h05->GetXaxis()->SetTitle("z position (mm)");
	h05->GetYaxis()->SetTitle("count/p^{+}");
	h05->GetYaxis()->SetTitleOffset(1.5);

	TH1D *h10 = new TH1D("h10",par+" Stop Time (Before Smear)",200,0,PulseInterval);
	h10->GetXaxis()->SetTitle("Time (ns)");
	h10->GetYaxis()->SetTitle(par+" Stopped / P^{+} / 5.849 ns");
	h10->GetYaxis()->SetTitleOffset(1.5);
	TH1D *h11 = new TH1D("h11",par+" Capture/Decay Time (After Smear)",200,0,PulseInterval);
	h11->GetXaxis()->SetTitle("Time (ns)");
	h11->GetYaxis()->SetTitle(par+" Stopped / P^{+} / 5.849 ns");
	h11->GetYaxis()->SetTitleOffset(1.5);

	TH1D *h20 = new TH1D("h20","Arrival Time Before Collimator",200,0,PulseInterval);
	h20->GetXaxis()->SetTitle("Time (ns)");
	h20->GetYaxis()->SetTitle("count/p^{+}");
	h20->GetYaxis()->SetTitleOffset(1.5);
	TH1D *h30 = new TH1D("h30","Arrival Time Before Collimator",200,0,PulseInterval);
	h30->GetXaxis()->SetTitle("Time (ns)");
	h30->GetYaxis()->SetTitle("count/p^{+}");
	h30->GetYaxis()->SetTitleOffset(1.5);
	TH1D *h40 = new TH1D("h40","Arrival Time Before Collimator",200,0,PulseInterval);
	h40->GetXaxis()->SetTitle("Time (ns)");
	h40->GetYaxis()->SetTitle("count/p^{+}");
	h40->GetYaxis()->SetTitleOffset(1.5);

	TH1D *h1_1 = new TH1D("h1_1","Momentum Before Collimator",150,0,150);
	h1_1->GetXaxis()->SetTitle("Momentum Amplitude (MeV/c)");
	h1_1->GetYaxis()->SetTitle("count/p^{+}");
	h1_1->GetYaxis()->SetTitleOffset(1.5);
	TH1D *h1_2 = new TH1D("h1_2","Momentum Before Collimator",150,0,150);
	TH1D *h1_3 = new TH1D("h1_3","Momentum Before Collimator",150,0,150);

	TH1D *h2_1 = new TH1D("h2_1","y Position Before Collimator",150,-200,200);
	h2_1->GetXaxis()->SetTitle("y position (mm)");
	h2_1->GetYaxis()->SetTitle("count/p^{+}");
	h2_1->GetYaxis()->SetTitleOffset(1.5);
	TH1D *h2_2 = new TH1D("h2_2","y Position Before Collimator",150,-200,200);
	TH1D *h2_3 = new TH1D("h2_3","y Position Before Collimator",150,-200,200);

	std::vector<int> *T_tid;
	std::vector<std::string> *T_volName;
	std::vector<double> *McTruth_time;
	std::vector<double> *McTruth_x;
	std::vector<double> *McTruth_y;
	std::vector<double> *McTruth_z;
	std::vector<double> *McTruth_px;
	std::vector<double> *McTruth_py;
	std::vector<double> *McTruth_pz;
	std::vector<double> *T_Ox;
	std::vector<double> *T_Oy;
	std::vector<double> *T_Oz;
	std::vector<double> *T_Ot;
	int V_nHits;
	int T_nHits;

	double weight;
	if (withStopPosition){
		c->SetBranchAddress("T_Ox",&T_Ox);
		c->SetBranchAddress("T_Oy",&T_Oy);
		c->SetBranchAddress("T_Oz",&T_Oz);
		c->SetBranchAddress("T_Ot",&T_Ot);
	}
	c->SetBranchAddress("T_nHits",&T_nHits);
	c->SetBranchAddress("V_nHits",&V_nHits);
	c->SetBranchAddress("McTruth_time",&McTruth_time);
	c->SetBranchAddress("McTruth_px",&McTruth_px);
	c->SetBranchAddress("McTruth_py",&McTruth_py);
	c->SetBranchAddress("McTruth_pz",&McTruth_pz);
	c->SetBranchAddress("McTruth_x",&McTruth_x);
	c->SetBranchAddress("McTruth_y",&McTruth_y);
	c->SetBranchAddress("McTruth_z",&McTruth_z);
	c->SetBranchAddress("weight",&weight);
	double Ox = 0;
	double Oy = 0;
	double Oz = 0;
	double Ot = 0;
	double x = 0;
	double y = 0;
	double z = 0;
	double px = 0;
	double py = 0;
	double pz = 0;
	double time = 0;
	bool stopped = false;
	bool passed = false;
	double nPassed = 0;
	double nPassedH = 0;
	double nStopped = 0;
	double nTotal = 0;
	if (withStopPosition){
		f = new TFile(runName+".output.root","RECREATE");
	}
	TTree *t  = new TTree("t","t");
	if (withStopPosition){
		t->Branch("x",&Ox);
		t->Branch("y",&Oy);
		t->Branch("z",&Oz);
		t->Branch("t",&Ot);
		t->Branch("weight",&weight);
	}
	int nEvents = c->GetEntries();
	std::cout<<nEvents<<" events!!!"<<std::endl;
	for (int iEvent = 0; iEvent < nEvents; iEvent++ ){
		if (iEvent%1000==0) std::cout<<(double)iEvent/nEvents*100<<" % ..."<<std::endl;
		c->GetEntry(iEvent);
		nTotal += weight;
		stopped=false;
		passed=false;
		x = (*McTruth_x)[0]*10;
		y = (*McTruth_y)[0]*10;
		z = (*McTruth_z)[0]*10;
		px = (*McTruth_px)[0]*1000;
		py = (*McTruth_py)[0]*1000;
		pz = (*McTruth_pz)[0]*1000;
		time = (*McTruth_time)[0];
		double pa = sqrt(px*px+py*py+pz*pz);
		double r = -1;
		if (V_nHits>0) passed=true;
		if (T_nHits>0){
			if (withStopPosition){
				Ox = (*T_Ox)[0]*10;
				Oy = (*T_Oy)[0]*10;
				Oz = (*T_Oz)[0]*10;
				Ot = (*T_Ot)[0];
				r = sqrt(Ox*Ox+Oy*Oy);
			}
			stopped=true;
		}
		h01->Fill(pa,y,weight);
		h1_1->Fill(pa,weight);
		h2_1->Fill(y,weight);
		h20->Fill(time,weight);
		if (passed){
			nPassed+=weight;
			if (pa>75) nPassedH+=weight;
			h02->Fill(pa,y,weight);
			h1_2->Fill(pa,weight);
			h2_2->Fill(y,weight);
			h30->Fill(time,weight);
		}
		if (stopped){
			nStopped+=weight;
			h03->Fill(pa,y,weight);
			h1_3->Fill(pa,weight);
			h2_3->Fill(y,weight);
			h40->Fill(time,weight);
			if (withStopPosition){
				h04->Fill(r,weight);
				h05->Fill(Oz,weight);
				t->Fill();
			}
			h10->Fill((*T_Ot)[0],weight);
			for (int i_window = 0; i_window <5; i_window++){
				h11->Fill(Ot+hCurve->GetRandom()+(i_window-2)*PulseInterval,weight);
			}
		}
	}
	double nProton = nTotal/NperP;
	nPassedH/=nProton;
	nPassed/=nProton;
	nStopped/=nProton;
	h01->Scale(1/nProton);
	h02->Scale(1/nProton);
	h03->Scale(1/nProton);
	h04->Scale(1/nProton);
	h05->Scale(1/nProton);
	h10->Scale(1/nProton);
	h11->Scale(1/nProton);
	h20->Scale(1/nProton);
	h30->Scale(1/nProton);
	h40->Scale(1/nProton);
	h1_1->Scale(1/nProton);
	h1_2->Scale(1/nProton);
	h1_3->Scale(1/nProton);
	h2_1->Scale(1/nProton);
	h2_2->Scale(1/nProton);
	h2_3->Scale(1/nProton);
	h01->Write();
	h02->Write();
	h03->Write();
	h04->Write();
	h05->Write();
	h10->Write();
	h11->Write();
	h20->Write();
	h30->Write();
	h40->Write();
	h1_1->Write();
	h1_2->Write();
	h1_3->Write();
	h2_1->Write();
	h2_2->Write();
	h2_3->Write();

	TH1D *h12 = new TH1D("h12",par+" Decayed/Captured in Time Window (After Smear)",200,0,PulseInterval);
	h12->GetXaxis()->SetTitle("Left End of Time Window (ns)");
	h12->GetYaxis()->SetTitle(par+" Stopped / P^{+} in Time Window");
	h12->GetYaxis()->SetTitleOffset(1.5);
	int BinMax = h11->FindBin(time_right);
	for (int i = 1; i<=BinMax; i++){
		double n = h11->Integral(i,BinMax);
		h12->SetBinContent(i,n);
	}
	h12->Write();

	TPaveText *sum = new TPaveText(0.5,0.75,0.9,0.9,"brNDC");
	sum->SetName("sum");
	sum->AddText(option);
	buff.str("");
	buff.clear();
	buff<<"N_{"<<par<<"_{pass}}/N_{p^{+}} = "<<nPassed<<std::endl;
	sum->AddText(buff.str().c_str());
	if (parName=="mu"){
		buff.str("");
		buff.clear();
		buff<<"N_{"<<par<<"_{pass}>75MeV/c}/N_{p^{+}} = "<<nPassedH<<std::endl;
		sum->AddText(buff.str().c_str());
	}
	buff.str("");
	buff.clear();
	buff<<"N_{"<<par<<"_{stop}}/N_{p^{+}} = "<<nStopped<<std::endl;
	sum->AddText(buff.str().c_str());
	sum->Write();

	TPaveText *info = new TPaveText(0.3,0.7,0.9,0.9,"brNDC");
	info->SetName("Info");
	buff.str("");
	buff.clear();
	buff<<"Seperation of Proton Bunches: "<<tSep<<" ns."<<std::endl;
	info->AddText(buff.str().c_str());
	buff.str("");
	buff.clear();
	buff<<"Proton Pulse Shape: 100 ns squre wave"<<std::endl;
	info->AddText(buff.str().c_str());
	buff.str("");
	buff.clear();
	buff<<"Right End of Time Window: "<<time_right<<" ns."<<std::endl;
	info->AddText(buff.str().c_str());
	buff.str("");
	buff.clear();
	buff<<"N_{"<<par<<"_{In Window}} = "<<h12->GetBinContent(h12->FindBin(time_left))<<", A_{Time} = "<<h12->GetBinContent(h12->FindBin(time_left))/nStopped<<std::endl;
	info->AddText(buff.str().c_str());

//*************************************************************************************
	gStyle->SetPalette(1);
	gStyle->SetOptStat(0);
	gStyle->SetPadTickX(1);
	gStyle->SetPadTickY(1);

	// Draw pa VS y
	TCanvas *c1 = new TCanvas("c","c",1024,768);
	TPad * p1 = new TPad("p1","p1",0,0,1./3,1);
	TPad * p2 = new TPad("p2","p2",1./3,0,2./3,1);
	TPad * p3 = new TPad("p3","p3",2./3,0,3./3,1);
	p1->Draw();
	p2->Draw();
	p3->Draw();
	h01->SetContour(50);
	h02->SetContour(50);
	h03->SetContour(50);
	p1->cd();
	h01->Draw("CONT0 COLZ");
	p2->cd();
	h02->Draw("CONT0 COLZ");
	p3->cd();
	h03->Draw("CONT0 COLZ");
	sum->Draw();
	c1->SaveAs(runName+".paVSy.pdf");
	c1->SaveAs(runName+".paVSy.png");

	// Draw stop position
	if (withStopPosition){
		TCanvas *c2 = new TCanvas("c2","c2",1024,768);
		TPad * p4 = new TPad("p4","p4",0,0,1./2,1);
		TPad * p5 = new TPad("p5","p5",1./2,0,2./2,1);
		p4->Draw();
		p5->Draw();
		p4->cd();
		h04->Draw();
		p5->cd();
		h05->Draw();
		std::cout<<"nStopped = "<<nStopped<<std::endl;
		if (withStopPosition){
			t->Write();
		}
		c2->SaveAs(runName+".SP.pdf");
		c2->SaveAs(runName+".SP.png");
	}

	// Draw pa y and t
	TCanvas *c3 = new TCanvas("c3","c3",1024,768);
	TPad *apad = new TPad("pa","pa",0,0,1./3,1);
	TPad *bpad = new TPad("pb","pb",1./3,0,2../3,1);
	TPad *cpad = new TPad("pc","pc",2./3,0,1,1);
	apad->Draw();
	bpad->Draw();
	cpad->Draw();

	apad->cd();
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	h1_1->SetMarkerColor(1);
	h1_1->SetLineColor(1);
	h1_1->Draw();
	h1_2->SetMarkerColor(600);
	h1_2->SetLineColor(600);
	h1_2->Draw("SAME");
	h1_3->SetMarkerColor(632);
	h1_3->SetLineColor(632);
	h1_3->Draw("SAME");

	bpad->cd();
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	h2_1->SetMarkerColor(1);
	h2_1->SetLineColor(1);
	h2_1->Draw();
	h2_2->SetMarkerColor(600);
	h2_2->SetLineColor(600);
	h2_2->Draw("SAME");
	h2_3->SetMarkerColor(632);
	h2_3->SetLineColor(632);
	h2_3->Draw("SAME");
	c3->SaveAs(runName+".pa_y.pdf");
	c3->SaveAs(runName+".pa_y.png");

	cpad->cd();
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	gPad->SetLogy(1);
	h20->SetMarkerColor(1);
	h20->SetLineColor(1);
	h20->Draw();
	h30->SetMarkerColor(600);
	h30->SetLineColor(600);
	h30->Draw("SAME");
	h40->SetMarkerColor(632);
	h40->SetLineColor(632);
	h40->Draw("SAME");
	legend = new TLegend(0.5,0.6,0.9,0.75);
	legend->AddEntry(h20,par+" Reached Collimator");
	legend->AddEntry(h30,par+" Passed Collimator");
	legend->AddEntry(h40,par+" Stopped in Target");
	legend->Draw("SAME");
	sum->Draw();
	c3->SaveAs(runName+".pa_y.pdf");
	c3->SaveAs(runName+".pa_y.png");

	// Draw Time Distribution
	TCanvas *c4 = new TCanvas("c4","c4",1024,768);
	apad->Draw();
	bpad->Draw();
	cpad->Draw();

	apad->cd();
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	gPad->SetLogy(1);
	h10->GetYaxis()->SetRangeUser(minimum,h10->GetMaximum()*2);
	h10->Draw();

	bpad->cd();
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	gPad->SetLogy(1);
	h11->GetYaxis()->SetRangeUser(minimum,h11->GetMaximum()*2);
	h11->Draw();
	info->Draw();

	cpad->cd();
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	gPad->SetLogy(1);
	h12->GetYaxis()->SetRangeUser(minimum,h12->GetMaximum()*2);
	h12->Draw();
	c4->SaveAs(runName+".time.pdf");
	c4->SaveAs(runName+".time.png");
}
