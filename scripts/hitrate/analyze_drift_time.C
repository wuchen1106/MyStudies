analyze_drift_time(){

	double duration = 20; // ns

//	TFile *_file0 = TFile::Open("new3.root");
//	TFile *_file0 = TFile::Open("result/signal_run1196.root");
//	TFile *_file0 = TFile::Open("result/BeamFlash_CdcCell_run1195.root");
//	TFile *_file0 = TFile::Open("result/BeamFlash_CdcCylinder_run1195.root");
//	TFile *_file0 = TFile::Open("result/BeamFlash_CdcEndPlate_run1195.root");
	TFile *_file0 = TFile::Open("result/BeamFlash_CdcWire_run1195.root");
//	TFile *_file0 = TFile::Open("result/BeamFlash_Others_run1195.root");
	TTree *t  = (TTree*) _file0->Get("t");
	int iev;
	int nwirehit;
	double drift_time[100000];
	double dist[100000];
	int ilayer[100000];
	int icell[100000];
	double px[100000];
	double py[100000];
	double pz[100000];
	double t0;
	t->SetBranchAddress("iev",&iev);
	t->SetBranchAddress("nhits",&nwirehit);
	t->SetBranchAddress("drift_time",drift_time);
	t->SetBranchAddress("dist",dist);
	t->SetBranchAddress("Layer",ilayer);
	t->SetBranchAddress("Cell",icell);
	t->SetBranchAddress("px",px);
	t->SetBranchAddress("py",py);
	t->SetBranchAddress("pz",pz);
//	t->SetBranchAddress("t0",&t0);

	std::vector<int> ilayer_icellT100;
	std::vector<double> tmin;
	std::vector<double> tmax;
	std::vector<double> dmin;
	std::vector<double> dmax;
	std::vector<double> nhits;
	std::vector<double> nhits_max;
	std::vector<double> nhits_total;
	std::vector<double> vpa;

	TFile * fout = new TFile("output.root","RECREATE");
	TH2D * h2 = new TH2D("tminVStmax","tmin VS tmax",128,0,1200,128,0,1200); 

	double t1,t2,d1,d2,pa;
	int nH, nh, nht, cellID, layerID;
	int ientry;
	TTree *ot  = new TTree("t","t");
	ot->Branch("iet",&ientry);
	ot->Branch("iev",&iev);
	ot->Branch("t0",&t0);
	ot->Branch("t1",&t1);
	ot->Branch("t2",&t2);
	ot->Branch("d1",&d1);
	ot->Branch("d2",&d2);
	ot->Branch("nh",&nh);
	ot->Branch("nH",&nH);
	ot->Branch("nht",&nht);
	ot->Branch("layer",&layerID);
	ot->Branch("cell",&cellID);
	ot->Branch("p",&pa);

	Long64_t printModulo = 1000;
	Long64_t nEvents  = t->GetEntries();
	std::cout<<"nEvents = "<<nEvents<<std::endl;
	for (Long64_t iEvent = 0; iEvent < nEvents; iEvent++){
//	for (Long64_t iEvent = 43; iEvent < 44; iEvent++){
		nhits.clear();
		nhits_total.clear();
		nhits_max.clear();
		tmin.clear();
		tmax.clear();
		dmax.clear();
		dmax.clear();
		vpa.clear();
		ilayer_icellT100.clear();
		if (iEvent%printModulo==0) std::cout<<"iEvent["<<iEvent<<"]:"<<std::endl; 
		t->GetEntry(iEvent);
		if (iEvent%printModulo==0) std::cout<<"	nhits = "<<nwirehit<<std::endl; 
		int pre_icell = -1;
		int pre_ihit = -1;
		for ( int iHit = 0; iHit < nwirehit; iHit++){
			//std::cout<<iHit<<"==>"<<"["<<ilayer[iHit]<<","<<icell[iHit]<<"]"<<std::endl;
			//std::cout<<"	time = "<<drift_time[iHit]<<std::endl;
			int cellcount = ilayer[iHit]+icell[iHit]*100;
			if (cellcount==pre_icell){ // still in the same cell
				//std::cout<<"	same"<<std::endl;
				nhits[pre_ihit]++;
				nhits_total[pre_ihit]++;
				//std::cout<<"	["<<nhits[pre_ihit]<<","<<nhits_max[pre_ihit]<<","<<nhits_total[pre_ihit]<<"]"<<std::endl;
			}
			else{ // now moved to another cell
				//std::cout<<"	another"<<std::endl;
				pre_icell = cellcount;
				int jHit = 0;
				for ( jHit = 0; jHit<ilayer_icellT100.size(); jHit++){
					if (ilayer_icellT100[jHit]==cellcount){
						break;
					}
				}
				if (pre_ihit>=0){
					if (nhits_max[pre_ihit]<nhits[pre_ihit]){
						nhits_max[pre_ihit] = nhits[pre_ihit];
					}
					//std::cout<<"	["<<nhits[pre_ihit]<<","<<nhits_max[pre_ihit]<<","<<nhits_total[pre_ihit]<<"]"<<std::endl;
				}
				pre_ihit = jHit; // move mark to the current cell
				if (jHit == ilayer_icellT100.size()){ // this is a new cell
					//std::cout<<"	new"<<std::endl;
					ilayer_icellT100.push_back(cellcount);
					tmin.push_back(drift_time[iHit]);
					tmax.push_back(drift_time[iHit]);
					dmin.push_back(dist[iHit]);
					dmax.push_back(dist[iHit]);
					vpa.push_back(sqrt(px[iHit]*px[iHit]+py[iHit]*py[iHit]+pz[iHit]*pz[iHit]));
					nhits.push_back(1);
					nhits_max.push_back(1);
					nhits_total.push_back(1);
					//std::cout<<"	["<<nhits[pre_ihit]<<","<<nhits_max[pre_ihit]<<","<<nhits_total[pre_ihit]<<"]"<<std::endl;
					continue;  // enough for this new cell
				}
				else{ // if this is an old cell, more things need to be done;
					//std::cout<<"	old"<<std::endl;
					nhits_total[pre_ihit]++;
					nhits[pre_ihit] = 1;
					vpa[pre_ihit] = sqrt(px[iHit]*px[iHit]+py[iHit]*py[iHit]+pz[iHit]*pz[iHit]);
					//std::cout<<"	["<<nhits[pre_ihit]<<","<<nhits_max[pre_ihit]<<","<<nhits_total[pre_ihit]<<"]"<<std::endl;
				}
			}
			if (tmin[jHit]>drift_time[iHit]) tmin[jHit]=drift_time[iHit];
			if (tmax[jHit]<drift_time[iHit]) tmax[jHit]=drift_time[iHit];
			if (dmin[jHit]>dist[iHit]) dmin[jHit]=dist[iHit];
			if (dmax[jHit]<dist[iHit]) dmax[jHit]=dist[iHit];
		}
		for (int iHit = 0; iHit<ilayer_icellT100.size();iHit++){
			h2->Fill(tmin[iHit],tmax[iHit]+duration);
			t1=tmin[iHit];
			t2=tmax[iHit];
		//	if (t1==t2) std::cout<<iEvent<<":"<<ilayer_icellT100[iHit]<<std::endl;
			d1=dmin[iHit];
			d2=dmax[iHit];
			nh=nhits[iHit];
			nH=nhits_max[iHit];
			nht=nhits_total[iHit];
			pa=vpa[iHit];
			ientry = iEvent;
			cellID = ilayer_icellT100[iHit]/100;
			layerID = ilayer_icellT100[iHit]-cellID*100l;
			ot->Fill();
		}
		ilayer_icellT100.clear();
		tmin.clear();
		tmax.clear();
		dmin.clear();
		dmax.clear();
	}
	h2->Write();
	ot->Write();
	ot->Print();
	fout->Close();
}
