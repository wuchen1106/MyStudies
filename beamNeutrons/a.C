a(){
	double mass = 0.9395656;

	TString runName = "mum";

	TFile * ifile = TFile::Open("result/"+runName+".root");
	TTree * tree = (TTree*) ifile->Get("tree");

	std::vector<double> *v_px;
	std::vector<double> *v_py;
	std::vector<double> *v_pz;
	std::vector<int>    *v_pid;
	std::vector<std::string>    *v_oprocess;
	std::vector<std::string>    *v_ovolName;

	tree->SetBranchAddress("V2_px",&v_px);
	tree->SetBranchAddress("V2_py",&v_py);
	tree->SetBranchAddress("V2_pz",&v_pz);
	tree->SetBranchAddress("V2_pid",&v_pid);
	tree->SetBranchAddress("V2_oprocess",&v_oprocess);
	tree->SetBranchAddress("V2_ovolName",&v_ovolName);

	TFile * ofile = new TFile("result/"+runName+".output.root","RECREATE");
	TTree * otree  =new TTree("t","t");

	double e;
	double theta;
	std::string * volume;
	std::string * process;

	otree->Branch("e",&e);
	otree->Branch("theta",&theta);
	otree->Branch("volume",&volume);
	otree->Branch("process",&process);

	for (int iEvent = 0; iEvent<tree->GetEntries(); iEvent++){
		tree->GetEntry(iEvent);
		for (int iHit = 0; iHit<v_px->size(); iHit++){
			if ((*v_pid)[iHit]==2112){
				double px = (*v_px)[iHit];
				double py = (*v_py)[iHit];
				double pz = (*v_pz)[iHit];
				e = sqrt(px*px+py*py+pz*pz+mass*mass)-mass;
				theta = atan(sqrt(px*px+py*py)/pz);
				process = &((*v_oprocess)[iHit]);

				std::string vol = (*v_ovolName)[iHit];
				if (vol[0]=='C'&&vol[1]=='o'){
					vol = "Coils";
				}
				else if (vol[0]=='C'&&vol[1]=='r'){
					vol = "Cryostats";
				}
				else if (vol[0]=='E'&&vol[1]=='n'){
					vol = "Endplates";
				}
				else if (vol[0]=='Y'&&vol[1]=='o'){
					vol = "Yokes";
				}
				else if (vol[0]=='T'&&vol[1]=='r'){
					vol = "Triggers";
				}
				else if (vol[0]=='W'&&vol[6]=='4'){
					vol = "Window4";
				}

				volume = &vol;
				otree->Fill();
			}
		}
	}

	otree->Write();
}
