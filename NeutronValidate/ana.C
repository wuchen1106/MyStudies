TString MyData = getenv("MYDATA");

void setName(std::string &name){
//	if (name.substr(0,7)=="CdcCell") name="CdcCell";
//	else if (name.substr(0,8)=="EndPlate") name="EndPlate";
//	else if (name.substr(0,12)=="CdcFieldWire") name="CdcFieldWire";
//	else if (name.substr(0,13)=="CdcSignalWire") name="CdcSignalWire";
//	else if (name.substr(0,6)=="TriChe") name="TriChe";
//	else if (name.substr(0,6)=="TriSci") name="TriSci";
//	else if (name.substr(0,4)=="Yoke") name="Yoke";
//	else if (name.substr(0,4)=="CrWa") name="CrWa";
//	else if (name.substr(0,4)=="CrCo") name="CrCo";
//	else if (name.substr(0,4)=="Coil") name="Coil";
//	else if (name.substr(0,5)=="CoSup") name="CoSup";
//	else if (name.substr(0,7)=="Blocker") name="Blocker";
//	else if (name.substr(0,7)=="Window4") name="Window4";
	return;
}

int ana(){
	// About this run
	std::vector<TString> DirName;
	std::vector<int> nRuns;
	std::vector<TString> FileNames;
	 // ########Should Modify#########
	DirName.push_back(MyData+"/20140729_Chen_Target001_G496P02");
	nRuns.push_back(50);
	TString yyBfilename = MyData+"/20140729_YangYe_Target001/TargetBackward.root";
	TString yyFfilename = MyData+"/20140729_YangYe_Target001/TargetForward.root";
	TString yyFLBfilename = MyData+"/20140731_YangYe_Target001/FLUKA.root";
	 // ########Should Modify#########

	// input
	int  M_nHits = 0;

	int  evt_num;
	int  run_num;
	double weight;

	std::vector<std::string>  *M_volName;
	std::vector<int>     *M_volID;
	std::vector<std::string>  *M_particleName;
	std::vector<int>     *M_pid;
	std::vector<int>     *M_tid;
	std::vector<double>  *M_t;
	std::vector<double>  *M_px;
	std::vector<double>  *M_py;
	std::vector<double>  *M_pz;
	std::vector<double>  *M_x;
	std::vector<double>  *M_y;
	std::vector<double>  *M_z;
	std::vector<double>  *M_opx;
	std::vector<double>  *M_opy;
	std::vector<double>  *M_opz;
	std::vector<double>  *M_ox;
	std::vector<double>  *M_oy;
	std::vector<double>  *M_oz;
	std::vector<double>  *M_ekin;
	std::vector<int>     *M_ptid;
	std::vector<int>     *M_ppid;
	std::vector<std::string>  *M_oprocess;
	std::vector<std::string>  *M_ovolName;

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

	c->SetBranchAddress("evt_num",&evt_num);
	c->SetBranchAddress("run_num",&run_num);
	c->SetBranchAddress("weight",&weight);

	c->SetBranchAddress("M_nHits",&M_nHits);
	c->SetBranchAddress("M_volName",&M_volName);
	c->SetBranchAddress("M_volID",&M_volID);
	c->SetBranchAddress("M_pid",&M_pid);
	c->SetBranchAddress("M_tid",&M_tid);
	c->SetBranchAddress("M_particleName",&M_particleName);
	c->SetBranchAddress("M_ekin",&M_ekin);
	c->SetBranchAddress("M_t",&M_t);
	c->SetBranchAddress("M_px",&M_px);
	c->SetBranchAddress("M_py",&M_py);
	c->SetBranchAddress("M_pz",&M_pz);
	c->SetBranchAddress("M_x",&M_x);
	c->SetBranchAddress("M_y",&M_y);
	c->SetBranchAddress("M_z",&M_z);
	c->SetBranchAddress("M_opx",&M_opx);
	c->SetBranchAddress("M_opy",&M_opy);
	c->SetBranchAddress("M_opz",&M_opz);
	c->SetBranchAddress("M_ox",&M_ox);
	c->SetBranchAddress("M_oy",&M_oy);
	c->SetBranchAddress("M_oz",&M_oz);
	c->SetBranchAddress("M_ppid",&M_ppid);
	c->SetBranchAddress("M_ptid",&M_ptid);
	c->SetBranchAddress("M_oprocess",&M_oprocess);
	c->SetBranchAddress("M_ovolName",&M_ovolName);

	//Yang Ye
	TFile * yyBfile = new TFile(yyBfilename);
	TTree *yyBtree  = (TTree*) yyBfile->Get("RooTracker");
	TFile * yyFfile = new TFile(yyFfilename);
	TTree *yyFtree  = (TTree*) yyFfile->Get("RooTracker");
	TFile * yyFLBfile = new TFile(yyFLBfilename);
	TTree *yyFLBtree  = (TTree*) yyFLBfile->Get("t");

	int yypid;
	float yyekin;
	float yypx;
	float yypy;
	float yypz;

	double yyeh,yyel,yyflux;

	yyBtree->SetBranchAddress("pdgcode",&yypid);
	yyBtree->SetBranchAddress("energy",&yyekin);
	yyBtree->SetBranchAddress("px",&yypx);
	yyBtree->SetBranchAddress("py",&yypy);
	yyBtree->SetBranchAddress("pz",&yypz);

	yyFtree->SetBranchAddress("pdgcode",&yypid);
	yyFtree->SetBranchAddress("energy",&yyekin);
	yyFtree->SetBranchAddress("px",&yypx);
	yyFtree->SetBranchAddress("py",&yypy);
	yyFtree->SetBranchAddress("pz",&yypz);

	yyFLBtree->SetBranchAddress("eh",&yyeh);
	yyFLBtree->SetBranchAddress("el",&yyel);
	yyFLBtree->SetBranchAddress("flux",&yyflux);

	// output
	TFile * f = new TFile("output.root","RECREATE");
	TTree *tree  = new TTree("t","t");

	double ekin;
	double t;
	double px;
	double py;
	double pz;
	double x;
	double y;
	double z;
	double opx;
	double opy;
	double opz;
	double ox;
	double oy;
	double oz;

	std::string *particleName;
	std::string *process;
	std::string *volume;
	std::string *volName;
	int pid;
	int tid;
	int ppid;
	int ptid;

	tree->Branch("evt_num",&evt_num);
	tree->Branch("run_num",&run_num);
	tree->Branch("weight",&weight);

	tree->Branch("ekin",&ekin);
	tree->Branch("t",&t);
	tree->Branch("px",&px);
	tree->Branch("py",&py);
	tree->Branch("pz",&pz);
	tree->Branch("x",&x);
	tree->Branch("y",&y);
	tree->Branch("z",&z);

	tree->Branch("opx",&opx);
	tree->Branch("opy",&opy);
	tree->Branch("opz",&opz);
	tree->Branch("ox",&ox);
	tree->Branch("oy",&oy);
	tree->Branch("oz",&oz);

	tree->Branch("particleName",&particleName);
	tree->Branch("process",&process);
	tree->Branch("volume",&volume);
	tree->Branch("volName",&volName);
	tree->Branch("pid",&pid);
	tree->Branch("tid",&tid);
	tree->Branch("ppid",&ppid);
	tree->Branch("ptid",&ptid);

	// Histos
	TH1D * h_yy_B_n0_ekin = new TH1D("h_yy_B_n0_ekin","h_yy_B_n0_ekin",128,-6,4);
	TH1D * h_yyFL_B_n0_ekin = new TH1D("h_yyFL_B_n0_ekin","h_yyFL_B_n0_ekin",128,-6,4);
	TH1D * h_yy_F_n0_ekin = new TH1D("h_yy_F_n0_ekin","h_yy_F_n0_ekin",128,-1.5,4);
	TH1D * h_yy_B_pp_ekin = new TH1D("h_yy_B_pp_ekin","h_yy_B_pp_ekin",128,0,300);
	TH1D * h_yy_F_pp_ekin = new TH1D("h_yy_F_pp_ekin","h_yy_F_pp_ekin",128,0,8100);
	TH1D * h_yy_B_pim_ekin = new TH1D("h_yy_B_pim_ekin","h_yy_B_pim_ekin",128,0,400);
	TH1D * h_yy_F_pim_ekin = new TH1D("h_yy_F_pim_ekin","h_yy_F_pim_ekin",128,0,2000);
	TH1D * h_yy_B_pip_ekin = new TH1D("h_yy_B_pip_ekin","h_yy_B_pip_ekin",128,0,400);
	TH1D * h_yy_F_pip_ekin = new TH1D("h_yy_F_pip_ekin","h_yy_F_pip_ekin",128,0,2000);

	TH1D * h_yy_B_n0_cos = new TH1D("h_yy_B_n0_cos","h_yy_B_n0_cos",128,-1,-0.4);
	TH1D * h_yy_F_n0_cos = new TH1D("h_yy_F_n0_cos","h_yy_F_n0_cos",128,0.4,1);
	TH1D * h_yy_B_pp_cos = new TH1D("h_yy_B_pp_cos","h_yy_B_pp_cos",128,-1,-0.4);
	TH1D * h_yy_F_pp_cos = new TH1D("h_yy_F_pp_cos","h_yy_F_pp_cos",128,0.4,1);
	TH1D * h_yy_B_pim_cos = new TH1D("h_yy_B_pim_cos","h_yy_B_pim_cos",128,-1,-0.4);
	TH1D * h_yy_F_pim_cos = new TH1D("h_yy_F_pim_cos","h_yy_F_pim_cos",128,0.4,1);
	TH1D * h_yy_B_pip_cos = new TH1D("h_yy_B_pip_cos","h_yy_B_pip_cos",128,-1,-0.4);
	TH1D * h_yy_F_pip_cos = new TH1D("h_yy_F_pip_cos","h_yy_F_pip_cos",128,0.4,1);

	TH1D * h_c_B_n0_ekin = new TH1D("h_c_B_n0_ekin","h_c_B_n0_ekin",128,-6,4);
	TH1D * h_c_F_n0_ekin = new TH1D("h_c_F_n0_ekin","h_c_F_n0_ekin",128,-1.5,4);
	TH1D * h_c_B_pp_ekin = new TH1D("h_c_B_pp_ekin","h_c_B_pp_ekin",128,0,300);
	TH1D * h_c_F_pp_ekin = new TH1D("h_c_F_pp_ekin","h_c_F_pp_ekin",128,0,8100);
	TH1D * h_c_B_pim_ekin = new TH1D("h_c_B_pim_ekin","h_c_B_pim_ekin",128,0,400);
	TH1D * h_c_F_pim_ekin = new TH1D("h_c_F_pim_ekin","h_c_F_pim_ekin",128,0,2000);
	TH1D * h_c_B_pip_ekin = new TH1D("h_c_B_pip_ekin","h_c_B_pip_ekin",128,0,400);
	TH1D * h_c_F_pip_ekin = new TH1D("h_c_F_pip_ekin","h_c_F_pip_ekin",128,0,2000);

	TH1D * h_c_B_n0_cos = new TH1D("h_c_B_n0_cos","h_c_B_n0_cos",128,-1,-0.4);
	TH1D * h_c_F_n0_cos = new TH1D("h_c_F_n0_cos","h_c_F_n0_cos",128,0.4,1);
	TH1D * h_c_B_pp_cos = new TH1D("h_c_B_pp_cos","h_c_B_pp_cos",128,-1,-0.4);
	TH1D * h_c_F_pp_cos = new TH1D("h_c_F_pp_cos","h_c_F_pp_cos",128,0.4,1);
	TH1D * h_c_B_pim_cos = new TH1D("h_c_B_pim_cos","h_c_B_pim_cos",128,-1,-0.4);
	TH1D * h_c_F_pim_cos = new TH1D("h_c_F_pim_cos","h_c_F_pim_cos",128,0.4,1);
	TH1D * h_c_B_pip_cos = new TH1D("h_c_B_pip_cos","h_c_B_pip_cos",128,-1,-0.4);
	TH1D * h_c_F_pip_cos = new TH1D("h_c_F_pip_cos","h_c_F_pip_cos",128,0.4,1);

	// Loop in events
	Long64_t nEvents = c->GetEntries();
	std::cout<<"nEvents = "<<nEvents<<std::endl;
	std::stringstream buff;
	int printModulo = 1000;
	for (Long64_t iEvent = 0; iEvent < nEvents; iEvent++ ){
		if (iEvent%printModulo==0) std::cout<<(double)iEvent/nEvents*100<<" % ..."<<std::endl;
		c->GetEntry(iEvent);
		for (int iHit = 0; iHit < M_nHits; iHit++){
			ekin = (*M_ekin)[iHit];
			t = (*M_t)[iHit];
			x = (*M_x)[iHit];
			y = (*M_y)[iHit];
			z = (*M_z)[iHit];
			px = (*M_px)[iHit];
			py = (*M_py)[iHit];
			pz = (*M_pz)[iHit];
			ox = (*M_ox)[iHit];
			oy = (*M_oy)[iHit];
			oz = (*M_oz)[iHit];
			opx = (*M_opx)[iHit];
			opy = (*M_opy)[iHit];
			opz = (*M_opz)[iHit];
			pid = (*M_pid)[iHit];
			tid = (*M_tid)[iHit];
			ppid = (*M_ppid)[iHit];
			ptid = (*M_ptid)[iHit];

			particleName= &((*M_particleName)[iHit]);
			volName = &((*M_volName)[iHit]);
			volume = &((*M_ovolName)[iHit]);
			process = &((*M_oprocess)[iHit]);
			tree->Fill();

			double cos = (pz/sqrt(px*px+py*py+pz*pz));

			if (*volName=="MonitorB"){
				if (pid==2212){
					h_c_B_pp_ekin->Fill(ekin*1000);
					h_c_B_pp_cos->Fill(cos);
				}
				else if (pid==2112){
					h_c_B_n0_ekin->Fill(log(ekin*1000)/log(10));
					h_c_B_n0_cos->Fill(cos);
				}
				else if (pid==-211){
					h_c_B_pim_ekin->Fill(ekin*1000);
					h_c_B_pim_cos->Fill(cos);
				}
				else if (pid==211){
					h_c_B_pip_ekin->Fill(ekin*1000);
					h_c_B_pip_cos->Fill(cos);
				}
			}
			else if (*volName=="MonitorF"){
				if (pid==2212){
					h_c_F_pp_ekin->Fill(ekin*1000);
					h_c_F_pp_cos->Fill(cos);
				}
				else if (pid==2112){
					h_c_F_n0_ekin->Fill(log(ekin*1000)/log(10));
					h_c_F_n0_cos->Fill(cos);
				}
				else if (pid==-211){
					h_c_F_pim_ekin->Fill(ekin*1000);
					h_c_F_pim_cos->Fill(cos);
				}
				else if (pid==211){
					h_c_F_pip_ekin->Fill(ekin*1000);
					h_c_F_pip_cos->Fill(cos);
				}
			}
		}
	}

	nEvents = yyFtree->GetEntries();
	std::cout<<"nEvents = "<<nEvents<<std::endl;
	for (Long64_t iEvent = 0; iEvent < nEvents; iEvent++ ){
		if (iEvent%printModulo==0) std::cout<<(double)iEvent/nEvents*100<<" % ..."<<std::endl;
		yyFtree->GetEntry(iEvent);
		double cos = (yypz/sqrt(yypx*yypx+yypy*yypy+yypz*yypz));

		if (yypid==2212){
			h_yy_F_pp_ekin->Fill(yyekin);
			h_yy_F_pp_cos->Fill(cos);
		}
		else if (yypid==2112){
			h_yy_F_n0_ekin->Fill(log(yyekin)/log(10));
			h_yy_F_n0_cos->Fill(cos);
		}
		else if (yypid==-211){
			h_yy_F_pim_ekin->Fill(yyekin);
			h_yy_F_pim_cos->Fill(cos);
		}
		else if (yypid==211){
			h_yy_F_pip_ekin->Fill(yyekin);
			h_yy_F_pip_cos->Fill(cos);
		}
	}

	nEvents = yyBtree->GetEntries();
	std::cout<<"nEvents = "<<nEvents<<std::endl;
	for (Long64_t iEvent = 0; iEvent < nEvents; iEvent++ ){
		if (iEvent%printModulo==0) std::cout<<(double)iEvent/nEvents*100<<" % ..."<<std::endl;
		yyBtree->GetEntry(iEvent);
		double cos = (yypz/sqrt(yypx*yypx+yypy*yypy+yypz*yypz));

		if (yypid==2212){
			h_yy_B_pp_ekin->Fill(yyekin);
			h_yy_B_pp_cos->Fill(cos);
		}
		else if (yypid==2112){
			h_yy_B_n0_ekin->Fill(log(yyekin)/log(10));
			h_yy_B_n0_cos->Fill(cos);
		}
		else if (yypid==-211){
			h_yy_B_pim_ekin->Fill(yyekin);
			h_yy_B_pim_cos->Fill(cos);
		}
		else if (yypid==211){
			h_yy_B_pip_ekin->Fill(yyekin);
			h_yy_B_pip_cos->Fill(cos);
		}
	}

	nEvents = yyFLBtree->GetEntries();
	std::cout<<"nEvents = "<<nEvents<<std::endl;
	double monitorarea = 3.1416*20*20;
	for (Long64_t iEvent = 0; iEvent < nEvents; iEvent++ ){
		if (iEvent%printModulo==0) std::cout<<(double)iEvent/nEvents*100<<" % ..."<<std::endl;
		yyFLBtree->GetEntry(iEvent);
		std::cout<<yyel<<","<<yyeh<<","<<yyflux<<std::endl;
		if (yyflux==0) continue;
		for ( int ibin = 1; ibin<=h_yyFL_B_n0_ekin->GetNbinsX(); ibin++){
			double left = h_yyFL_B_n0_ekin->GetBinLowEdge(ibin);
			double right = h_yyFL_B_n0_ekin->GetBinLowEdge(ibin+1);
			double left1 = log(yyel*1e3)/log(10);
			double right1 = log(yyeh*1e3)/log(10);
			double width = 0;
			if (left1<left){
				if (right1>left){
					if (right1<right){
						width = right1-left;
					}
					else{
						width = right-left;
					}
				}
			}
			else if (left1<right){
				if (right1<right){
					width = right1-left1;
				}
				else{
					width = right-left1;
				}
			}
			double weight = yyflux*monitorarea*5e5*width/(right1-left1);
			std::cout<<"("<<left<<","<<right<<")"<<" ("<<left1<<","<<right1<<")"<<" "<<width<<", "<<right1-left1<<"; "<<yyflux<<", "<<weight<<std::endl;
			h_yyFL_B_n0_ekin->AddBinContent(ibin,weight);
		}
	}

	// save
	h_yy_B_n0_ekin->Write();
	h_yyFL_B_n0_ekin->Write();
	h_yy_F_n0_ekin->Write();
	h_yy_B_pp_ekin->Write();
	h_yy_F_pp_ekin->Write();
	h_yy_B_pim_ekin->Write();
	h_yy_F_pim_ekin->Write();
	h_yy_B_pip_ekin->Write();
	h_yy_F_pip_ekin->Write();

	h_yy_B_n0_cos->Write();
	h_yy_F_n0_cos->Write();
	h_yy_B_pp_cos->Write();
	h_yy_F_pp_cos->Write();
	h_yy_B_pim_cos->Write();
	h_yy_F_pim_cos->Write();
	h_yy_B_pip_cos->Write();
	h_yy_F_pip_cos->Write();

	h_c_B_n0_ekin->Write();
	h_c_F_n0_ekin->Write();
	h_c_B_pp_ekin->Write();
	h_c_F_pp_ekin->Write();
	h_c_B_pim_ekin->Write();
	h_c_F_pim_ekin->Write();
	h_c_B_pip_ekin->Write();
	h_c_F_pip_ekin->Write();

	h_c_B_n0_cos->Write();
	h_c_F_n0_cos->Write();
	h_c_B_pp_cos->Write();
	h_c_F_pp_cos->Write();
	h_c_B_pim_cos->Write();
	h_c_F_pim_cos->Write();
	h_c_B_pip_cos->Write();
	h_c_F_pip_cos->Write();

	tree->Write();
	f->Close();
	return 0;
}
