{
	TChain* c1 = new TChain("t");
	TChain* c2 = new TChain("tree");
	TTree * t = new TTree("t","t");
	//c1->Add("CDC.mum_o75_o100.em.g40cm10mm.005T.Vac0731_200_200_39.g4s.QBH.root");
	c1->Add("BLT.mum.EP.g40cm10mm.005T.Vac0928_140_140.g4s.QBH.root");
	//c1->Add("BLT.mum.EP.g40cm10mm.005T.Vac0731_200_200.g4s.QBH.root");
	//c2->Add("/home/chen/MyWorkArea/Data/raw/g4sim/CDC.pim.g40cm10mm.003T.Vac0731Al.g4s.QBH/0_job0.raw");
	//c2->Add("BLT.em.EP.g40cm10mm.005T.Vac0731_200_200.g4s.QBH.old.root");
	std::stringstream buff;
//	for (int i = 0; i< 10; i++){
//		buff.str("");
//		buff.clear();
////		buff<<"/home/chen/MyWorkArea/Data/raw/g4sim/CDC.EP_OT.g40cm10mm.005T.Vac0731_200_200_39.g4s.QBH/"<<i<<"_job0.raw";
//		buff<<"BLT.pim.EP.g40cm10mm.005T.Vac0731_200_200.g4s.QBH/BLT.pim.EP.g40cm10mm.005T.Vac0731_200_200.g4s.QBH."<<i<<".root";
//		c2->Add(buff.str().c_str());
//	}
//	c2->Add("../output/raw_g4sim.root");

	TFile *f = new TFile("output.root","RECREATE");

	int evt_num;
	int run_num;
	int ini_tid;
	int ini_pid;
	int ini_ppid = 0;
	double ini_t;
	double ini_x;
	double ini_y;
	double ini_z;
	double ini_px;
	double ini_py;
	double ini_pz;
	double ini_ot;
	double ini_ox;
	double ini_oy;
	double ini_oz;
	double ini_opx;
	double ini_opy;
	double ini_opz;
	int    ini_ini_pid;
	double ini_ini_t;
	double ini_ini_x;
	double ini_ini_y;
	double ini_ini_z;
	double ini_ini_px;
	double ini_ini_py;
	double ini_ini_pz;
	int    ini_ini_ini_pid;
	double ini_ini_ini_t;
	double ini_ini_ini_x;
	double ini_ini_ini_y;
	double ini_ini_ini_z;
	double ini_ini_ini_px;
	double ini_ini_ini_py;
	double ini_ini_ini_pz;
	std::string *p_process = 0;
	std::string *p_volume = 0;
	std::string *p_cvolume = 0;
	std::string ini_process = "NONE";
	std::string ini_volume = "NONE";
	double weight0 = 1;
	double Radius;

	int evt_num2;
	int run_num2;
	int ini_tid2;
	int ini_pid2;
	int ini_ppid2 = 0;
	double ini_t2;
	double ini_x2;
	double ini_y2;
	double ini_z2;
	double ini_px2;
	double ini_py2;
	double ini_pz2;
	double ini_ot2;
	double ini_ox2;
	double ini_oy2;
	double ini_oz2;
	double ini_opx2;
	double ini_opy2;
	double ini_opz2;
	std::string *p_process2 = 0;
	std::string *p_volume2 = 0;
	std::string ini_process2 = "NONE";
	std::string ini_volume2 = "NONE";
	double weight02 = 1;

	int Monitor_nHits = 0;
	std::vector<int> *Monitor_tid;
	std::vector<int> *Monitor_pid;
	std::vector<int> *Monitor_ppid;
	std::vector<std::string> *Monitor_oprocess;
	std::vector<std::string> *Monitor_ovolName;
	std::vector<double> *Monitor_t;
	std::vector<double> *Monitor_x;
	std::vector<double> *Monitor_y;
	std::vector<double> *Monitor_z;
	std::vector<double> *Monitor_px;
	std::vector<double> *Monitor_py;
	std::vector<double> *Monitor_pz;
	std::vector<double> *Monitor_ot;
	std::vector<double> *Monitor_ox;
	std::vector<double> *Monitor_oy;
	std::vector<double> *Monitor_oz;
	std::vector<double> *Monitor_opx;
	std::vector<double> *Monitor_opy;
	std::vector<double> *Monitor_opz;
	std::vector<int> *Monitor_charge;
	std::vector<int> *Monitor_stopped;
	std::vector<double> *Monitor_stop_time;

	c1->SetBranchAddress("evt_num",&evt_num);
	c1->SetBranchAddress("run_num",&run_num);
	c1->SetBranchAddress("t",&ini_t);
	c1->SetBranchAddress("x",&ini_x);
	c1->SetBranchAddress("y",&ini_y);
	c1->SetBranchAddress("z",&ini_z);
	c1->SetBranchAddress("px",&ini_px);
	c1->SetBranchAddress("py",&ini_py);
	c1->SetBranchAddress("pz",&ini_pz);
	c1->SetBranchAddress("ot",&ini_ot);
	c1->SetBranchAddress("ox",&ini_ox);
	c1->SetBranchAddress("oy",&ini_oy);
	c1->SetBranchAddress("oz",&ini_oz);
	c1->SetBranchAddress("opx",&ini_opx);
	c1->SetBranchAddress("opy",&ini_opy);
	c1->SetBranchAddress("opz",&ini_opz);
	c1->SetBranchAddress("pid",&ini_pid);
	c1->SetBranchAddress("tid",&ini_tid);
	c1->SetBranchAddress("ppid",&ini_ppid);
	c1->SetBranchAddress("i_t",&ini_ini_t);
	c1->SetBranchAddress("i_x",&ini_ini_x);
	c1->SetBranchAddress("i_y",&ini_ini_y);
	c1->SetBranchAddress("i_z",&ini_ini_z);
	c1->SetBranchAddress("i_px",&ini_ini_px);
	c1->SetBranchAddress("i_py",&ini_ini_py);
	c1->SetBranchAddress("i_pz",&ini_ini_pz);
	c1->SetBranchAddress("i_pid",&ini_ini_pid);
	c1->SetBranchAddress("ii_t",&ini_ini_ini_t);
	c1->SetBranchAddress("ii_x",&ini_ini_ini_x);
	c1->SetBranchAddress("ii_y",&ini_ini_ini_y);
	c1->SetBranchAddress("ii_z",&ini_ini_ini_z);
	c1->SetBranchAddress("ii_px",&ini_ini_ini_px);
	c1->SetBranchAddress("ii_py",&ini_ini_ini_py);
	c1->SetBranchAddress("ii_pz",&ini_ini_ini_pz);
	c1->SetBranchAddress("ii_pid",&ini_ini_ini_pid);
	c1->SetBranchAddress("process",&p_process);
	c1->SetBranchAddress("volume",&p_volume);
	c1->SetBranchAddress("cvolume",&p_cvolume);
	c1->SetBranchAddress("weight",&weight0);
	c1->SetBranchAddress("R",&Radius);

//	c2->SetBranchAddress("evt_num",&evt_num2);
//	c2->SetBranchAddress("run_num",&run_num2);
//	c2->SetBranchAddress("t",&ini_t2);
//	c2->SetBranchAddress("x",&ini_x2);
//	c2->SetBranchAddress("y",&ini_y2);
//	c2->SetBranchAddress("z",&ini_z2);
//	c2->SetBranchAddress("px",&ini_px2);
//	c2->SetBranchAddress("py",&ini_py2);
//	c2->SetBranchAddress("pz",&ini_pz2);
//	c2->SetBranchAddress("ot",&ini_ot2);
//	c2->SetBranchAddress("ox",&ini_ox2);
//	c2->SetBranchAddress("oy",&ini_oy2);
//	c2->SetBranchAddress("oz",&ini_oz2);
//	c2->SetBranchAddress("opx",&ini_opx2);
//	c2->SetBranchAddress("opy",&ini_opy2);
//	c2->SetBranchAddress("opz",&ini_opz2);
//	c2->SetBranchAddress("pid",&ini_pid2);
//	c2->SetBranchAddress("tid",&ini_tid2);
//	c2->SetBranchAddress("ppid",&ini_ppid2);
//	c2->SetBranchAddress("process",&p_process2);
//	c2->SetBranchAddress("volume",&p_volume2);
//	c2->SetBranchAddress("weight",&weight02);

	TString Volume = "CdcCell";
//	TString Volume = "CDCMonitor";
//	TString Volume = "McTruth";
//	c2->SetBranchAddress(Volume+"_nTracks",&Monitor_nHits);
	c2->SetBranchAddress(Volume+"_nHits",&Monitor_nHits);
//	c2->SetBranchAddress(Volume+"_t",&Monitor_t);
//	c2->SetBranchAddress(Volume+"_tid",&Monitor_tid);
//	c2->SetBranchAddress(Volume+"_pid",&Monitor_pid);
//	c2->SetBranchAddress(Volume+"_ppid",&Monitor_ppid);
//	c2->SetBranchAddress(Volume+"_oprocess",&Monitor_oprocess);
//	c2->SetBranchAddress(Volume+"_ovolName",&Monitor_ovolName);
//	c2->SetBranchAddress(Volume+"_ot",&Monitor_ot);
//	c2->SetBranchAddress(Volume+"_ox",&Monitor_ox);
//	c2->SetBranchAddress(Volume+"_oy",&Monitor_oy);
//	c2->SetBranchAddress(Volume+"_oz",&Monitor_oz);
//	c2->SetBranchAddress(Volume+"_opx",&Monitor_opx);
//	c2->SetBranchAddress(Volume+"_opy",&Monitor_opy);
//	c2->SetBranchAddress(Volume+"_opz",&Monitor_opz);
//	c2->SetBranchAddress(Volume+"_x",&Monitor_x);
//	c2->SetBranchAddress(Volume+"_y",&Monitor_y);
//	c2->SetBranchAddress(Volume+"_z",&Monitor_z);
//	c2->SetBranchAddress(Volume+"_charge",&Monitor_charge);
//	c2->SetBranchAddress(Volume+"_px",&Monitor_px);
//	c2->SetBranchAddress(Volume+"_py",&Monitor_py);
//	c2->SetBranchAddress(Volume+"_pz",&Monitor_pz);
//	c2->SetBranchAddress(Volume+"_stopped",&Monitor_stopped);
//	c2->SetBranchAddress(Volume+"_stop_time",&Monitor_stop_time);

//	t->Branch("evt_num",&evt_num);
//	t->Branch("run_num",&run_num);
//	t->Branch("tid",&ini_tid);
	t->Branch("pid",&ini_pid);
	t->Branch("x",&ini_x);
	t->Branch("y",&ini_y);
	t->Branch("z",&ini_z);
	t->Branch("px",&ini_px);
	t->Branch("py",&ini_py);
	t->Branch("pz",&ini_pz);
	t->Branch("t",&ini_t);
//	t->Branch("ox",&ini_ox);
//	t->Branch("oy",&ini_oy);
//	t->Branch("oz",&ini_oz);
//	t->Branch("opx",&ini_opx);
//	t->Branch("opy",&ini_opy);
//	t->Branch("opz",&ini_opz);
//	t->Branch("ot",&ini_ot);
//	t->Branch("ppid",&ini_ppid);
//	t->Branch("process",&p_process);
//	t->Branch("volume",&p_volume);
//	t->Branch("i_t",&ini_ini_t);
//	t->Branch("i_x",&ini_ini_x);
//	t->Branch("i_y",&ini_ini_y);
//	t->Branch("i_z",&ini_ini_z);
//	t->Branch("i_px",&ini_ini_px);
//	t->Branch("i_py",&ini_ini_py);
//	t->Branch("i_pz",&ini_ini_pz);
//	t->Branch("i_pid",&ini_ini_pid);
//	t->Branch("cvolume",&p_cvolume);
//	t->Branch("weight",&weight0);
//	t->Branch("ii_t",&ini_ini_ini_t);
//	t->Branch("ii_x",&ini_ini_ini_x);
//	t->Branch("ii_y",&ini_ini_ini_y);
//	t->Branch("ii_z",&ini_ini_ini_z);
//	t->Branch("ii_px",&ini_ini_ini_px);
//	t->Branch("ii_py",&ini_ini_ini_py);
//	t->Branch("ii_pz",&ini_ini_ini_pz);
//	t->Branch("ii_pid",&ini_ini_ini_pid);
//	t->Branch("R",&Radius);

	int iEvent2 = 0;
	int nMatch = 0;
	int nNotMatch = 0;
	int nEvents = c1->GetEntries();
	printf("nEvents = %d\n",nEvents);


	/*
	//Add missed event
	for (int iEvent1 = 0; iEvent1<nEvents; iEvent1++, iEvent2++){
		c1->GetEntry(iEvent1);
		c2->GetEntry(iEvent2);
		bool Match=true;
		double diff = fabs((ini_px-ini_px2)/ini_px);
//		if (run_num!=run_num2||evt_num!=evt_num2){
		if (diff>1e-4){
			Match = false;
		}
		if (!Match){
//			printf("%d Not Match! run1 = %d, evt1 = %d, run2 = %d, evt2 = %d\n",iEvent1,run_num,evt_num,run_num2,evt_num2);
			printf("%d Not Match! px1 = %lf, px2 = %lf, diff = %lf%\n",iEvent1,ini_px,ini_px2,diff);
			nNotMatch++;
			iEvent1--;
			weight0 = 0;
		}
		else{
			nMatch++;
		}
		//printf("Match!!!\n");
		t->Fill();
	}
	printf("Match: %d\n",nMatch);
	printf("Not Match: %d\n",nNotMatch);
	*/

	/*
	//Add missed event
	for (int iEvent1 = 0; iEvent1<nEvents; iEvent1++, iEvent2++){
		c1->GetEntry(iEvent1);
		c2->GetEntry(iEvent2);
		bool Match=true;
		if (Monitor_nHits>0){
			if (fabs((*Monitor_px)[0]*1000-ini_px)>1e-4*fabs(ini_px)){
				Match = false;
			}
			if (!Match){
				printf("%d Not Match\n",iEvent1);
				nNotMatch++;
				iEvent1--;
				weight0 = 0;
			}
			else{
				nMatch++;
			}
		}
		//printf("Match!!!\n");
		t->Fill();
	}
	printf("Match: %d\n",nMatch);
	printf("Not Match: %d\n",nNotMatch);
	*/

	/*
	//Get the matched events
	for (int iEvent1 = 0; iEvent1<nEvents; iEvent1++, iEvent2++){
		c1->GetEntry(iEvent1);
		c2->GetEntry(iEvent2);
		bool Match=true;
		for ( int i_mon = 0; i_mon < Monitor_nHits; prevtid=(*Monitor_tid)[i_mon], i_mon++ ){
			if ((*Monitor_tid)[i_mon]==1&&fabs((*Monitor_opx)[i_mon]*1000-ini_px)>1e-4*fabs(ini_px)){
				printf("%d,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",(*Monitor_tid)[i_mon],(*Monitor_opx)[i_mon]*1000,(*Monitor_opy)[i_mon]*1000,(*Monitor_opz)[i_mon]*1000,ini_px,ini_py,ini_pz,weight0);
				Match = false;
				break;
			}
		}
		if (!Match){
			printf("%d,%d,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",iEvent1,(*Monitor_tid)[i_mon],(*Monitor_opx)[i_mon]*1000,(*Monitor_opy)[i_mon]*1000,(*Monitor_opz)[i_mon]*1000,ini_px,ini_py,ini_pz,weight0);
			printf("NOT Match!!!\n");
			nNotMatch++;
			iEvent2--;
			continue;
		}
		nMatch++;
		//printf("Match!!!\n");
		t->Fill();
	}
	printf("Match: %d\n",nMatch);
	printf("Not Match: %d\n",nNotMatch);
	*/

	/*
	// get high energy electron
	int count = 0;
	for (int iEvent = 0; iEvent<nEvents; iEvent++){
		if (iEvent%1000==0) std::cout<<(double)iEvent/nEvents<<"..."<<std::endl;
		c1->GetEntry(iEvent);
		if ((ini_pz>0&&ini_z>750)||(ini_pz<0&&ini_z<-750)||ini_z<-1050) continue;
//		if (Radius<500) continue;
		if (ini_pid==11&&sqrt(ini_px*ini_px+ini_py*ini_py+ini_pz*ini_pz)>30){
			TVector3 B(0,0,1);
			TVector3 p(ini_px,ini_py,0);
			TVector3 R(ini_x,ini_y,0);
			TVector3 r = - p.Cross(B);
			r = r*(10/2.99792458); // MeV, mm
			TVector3 D = R + r;
			D = D + D*(r.Mag()/D.Mag());
			Radius = D.Mag();
			t->Fill();
			count++;
		}
	}
	*/

	/*
	// get high energy electron
	int count = 0;
	for (int iEvent = 0; iEvent<nEvents; iEvent++){
		if (iEvent%1000==0) std::cout<<(double)iEvent/nEvents<<"..."<<std::endl;
		c1->GetEntry(iEvent);
		c2->GetEntry(iEvent);
		for (int i = 0; i<Monitor_nHits; i++){
			if ((*Monitor_pid)[i] == 11){
				double px = (*Monitor_px)[i]*1000;
				double py = (*Monitor_py)[i]*1000;
				double pz = (*Monitor_pz)[i]*1000;
				double pa = sqrt(px*px+py*py+pz*pz);
				if (pa>100){
					ini_px = px;
					ini_py = py;
					ini_pz = pz;
					ini_x = (*Monitor_x)[i]*10;
					ini_y = (*Monitor_y)[i]*10;
					ini_z = (*Monitor_z)[i]*10;
					ini_pid = 11;
					init_ppid = (*Monitor_ppid)[i];
					init_tid = (*Monitor_tid)[i];
					TVector3 B(0,0,1);
					TVector3 p(px,py,0);
					TVector3 R(ini_x,ini_y,0);
					TVector3 r = - p.Cross(B);
					r = r*(10/2.99792458); // MeV, mm
					TVector3 D = R + r;
					D = D + D*(r.Mag()/D.Mag());
					Radius = D.Mag();
					t->Fill();
					count++;
					t->Fill();
					count++;
				}
			}
		}
	}
	*/

	/*
	// 
	int count = 0;
	for (int iEvent = 0; iEvent<nEvents; iEvent++){
		if (iEvent%1000==0) std::cout<<(double)iEvent/nEvents<<"..."<<std::endl;
		c1->GetEntry(iEvent);
		c2->GetEntry(iEvent);
		if (Monitor_nHits>0){
			t->Fill();
		}
	}
	*/

	for (int iEvent = 0; iEvent<nEvents; iEvent++){
		if (iEvent%1000==0) std::cout<<(double)iEvent/nEvents<<"..."<<std::endl;
		c1->GetEntry(iEvent);
		if (ini_pid==13){
			t->Fill();
		}
	}

	t->Write();
}
