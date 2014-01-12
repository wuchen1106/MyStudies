double total = 0;

TString runName = "1p5_0927_root";
TString DirName = "/scratchfs/bes/wuc/MyWorkArea/Data/raw/g4sim";
total = 246067;

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

void get(){
	TChain *c = new TChain("tree");
//	c->Add(runName+".root");
	std::stringstream buff;
	for (int i = 0; i<100; i++){
		buff.str("");
		buff.clear();
		buff<<DirName+"/RPC."<<runName<<"/"<<i<<"_job0.raw";
		c->Add( buff.str().c_str());
	}

	TFile *f = new TFile(runName+".output.root","RECREATE");
	TTree *t = new TTree("t","t");

	TH1D * h10 = new TH1D("pa","Run"+runName+": Momentum At First Hit (MeV/c)",100,100,110);
	h10->GetXaxis()->SetTitle("Momentum At First Hit (MeV/c)");
	h10->GetYaxis()->SetTitle("Count");

	std::vector<int> *M_tid;
	std::vector<double> *M_t;
	std::vector<int> *C_tid;
	std::vector<double> *C_t;
	std::vector<double> *C_px;
	std::vector<double> *C_py;
	std::vector<double> *C_pz;
	std::vector<int> *C_volID;
	std::vector<std::string> *M_oprocess;
	std::vector<std::string> *M_ovolName;
	double weight;
	c->SetBranchAddress("M_tid",&M_tid);
	c->SetBranchAddress("M_t",&M_t);
	c->SetBranchAddress("C_tid",&C_tid);
	c->SetBranchAddress("C_t",&C_t);
	c->SetBranchAddress("C_px",&C_px);
	c->SetBranchAddress("C_py",&C_py);
	c->SetBranchAddress("C_pz",&C_pz);
	c->SetBranchAddress("C_volID",&C_volID);
	c->SetBranchAddress("M_ovolName",&M_ovolName);
	c->SetBranchAddress("M_oprocess",&M_oprocess);
	c->SetBranchAddress("weight",&weight);
	double time = -1;
	std::stringstream buff;
	double pa = -1;
	std::string *process;
	std::string *volume;
	t->Branch("process",&process);
	t->Branch("volume",&volume);
	t->Branch("pa",&pa);
	t->Branch("weight",&weight);
	int nEvents = c->GetEntries();
	double nTarget=0;
	double nAbsorber=0;
	for (int iEvent = 0; iEvent < nEvents; iEvent++ ){
//		std::cout<<"______________iEvent = "<<iEvent<<"___________________"<<std::endl;
		c->GetEntry(iEvent);
		bool foundhit = false;
		int tid = -1;
		pa = -1;
		bool hitlayer5 = false;
		int nHits = C_t->size();
		int prevtid = -1;
		for (int iHit = 0; iHit < nHits; iHit++){
			if (prevtid!= (*C_tid)[iHit]){
				prevtid=(*C_tid)[iHit];
//				std::cout<<"new track: "<<(*C_tid)[iHit]<<std::endl;
				double px = (*C_px)[iHit]*1000;
				double py = (*C_py)[iHit]*1000;
				double pz = (*C_pz)[iHit]*1000;
				double i_pa = sqrt(px*px+py*py+pz*pz);
				if (i_pa > pa){
//					std::cout<<"    pa is max!"<<std::endl;
					pa = i_pa;
					tid = (*C_tid)[iHit];
					time = (*C_t)[iHit];
					hitlayer5 = false;
				}
			}
			if (!hitlayer5){
				int volID = (*C_volID)[iHit];
				int layerNo = get_layer_index(volID);
//				std::cout<<"layerNo = "<<layerNo<<", volID = "<<volID<<std::endl;
				if (layerNo==4) hitlayer5 = true;
			}
			foundhit = true;
		}
		if (foundhit&&hitlayer5){
			double i_time = -1;
//			std::cout<<"CDC tid = "<<tid<<std::endl;
			int nHits = M_t->size();
			for (int iHit = 0; iHit < nHits; iHit++){
				double i_tid = (*M_tid)[iHit];
//				std::cout<<"    i_tid = "<<i_tid<<std::endl;
				if (i_tid == tid){
					i_time = (*M_t)[iHit];
					*process = (*M_oprocess)[iHit];
					*volume = (*M_ovolName)[iHit];
					break;
				}
			}
//			if (i_time!=-1){
//				std::cout<<"  CDC: "<<time<<", Trigger: "<<i_time<<std::endl;
//			}
			if (i_time<=time){
				foundhit = false;
			}
		}
		if (foundhit&&hitlayer5){
			if (pa>100&&pa<110){
//			if (*volume!="OuterCylinder"
//				&&*volume!="EndPlate_0"
//				&&*volume!="EndPlate_1"
//				&&*volume!="Trigger_0"
//				&&*volume!="Trigger_1"
//				)
				if ((*volume) == "Target") nTarget+=weight;
				else if ((*volume) == "Absorber") nAbsorber+=weight;
				h10->Fill(pa,weight);
				t->Fill();
			}
		}
	}

	double count = h10->Integral();
	std::cout<<"Found "<<count<<" good tracks! "<<nTarget<<" from Target, "<<nAbsorber<<" from Absorber"<<std::endl;
	TCanvas *c1 = new TCanvas("c","c");
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	gStyle->SetPalette(1);
	gStyle->SetOptStat(0);
	gStyle->SetPadTickX(1);
	gStyle->SetPadTickY(1);
	h10->Draw();

	h10->Write();
	t->Write();
	f->Close();
}
