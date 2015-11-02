{
	double twindow = 1000;
	double me = 0.511e-3; // GeV
	TChain * c = new TChain("tree");
	c->Add("/home/chen/MyWorkArea/Simulate/comet/output/CyDet.Acc.em.root");
	//c->Add("/home/chen/MyWorkArea/Simulate/comet/output/raw_g4sim.root");
	std::vector<int> * in_triID = 0;
	std::vector<double> * in_trit = 0;
	std::vector<double> * in_triedep = 0;
	std::vector<double> * in_tripx = 0;
	std::vector<double> * in_tripy = 0;
	std::vector<double> * in_tripz = 0;
	std::vector<std::string> * in_triName= 0;
	std::vector<int> * in_cdclid = 0;
	c->SetBranchAddress("CdcCell_layerID",&in_cdclid);
	c->SetBranchAddress("M_t",&in_trit);
	c->SetBranchAddress("M_edep",&in_triedep);
	c->SetBranchAddress("M_px",&in_tripx);
	c->SetBranchAddress("M_py",&in_tripy);
	c->SetBranchAddress("M_pz",&in_tripz);
	c->SetBranchAddress("M_volID",&in_triID);
	c->SetBranchAddress("M_volName",&in_triName);
	TFile * ofile = new TFile("output.root","RECREATE");
	TTree * otree = new TTree("t","t");
	double sci_time[128];
	double che_time[128];
	int totalfoundD = 0;
	int totalfoundU = 0;
	Long64_t nEntries = c->GetEntries();
	for (Long64_t iEntry = 0; iEntry<nEntries; iEntry++){
		if (iEntry%1000==0) std::cout<<(double)iEntry/nEntries*100<<"%..."<<std::endl;
		c->GetEntry(iEntry);
		int maxlid = 0;
		for ( int ihit = 0; ihit<in_cdclid->size(); ihit++){
			if (maxlid<(*in_cdclid)[ihit]) maxlid=(*in_cdclid)[ihit];
		}
		if (maxlid<4) continue;
		if (in_trit->size()<4) continue;
		for (int itri = 0; itri<128; itri++){
			che_time[itri] = -1;
			sci_time[itri] = -1;
		}
		for ( int ihit = 0; ihit<in_trit->size(); ihit++){
			double px = (*in_tripx)[ihit];
			double py = (*in_tripy)[ihit];
			double pz = (*in_tripz)[ihit];
			double pa = sqrt(px*px+py*py+pz*pz);
			double beta = pa/sqrt(pa*pa+me*me);
			if ((*in_triName)[ihit]=="TriCheD"
				||(*in_triName)[ihit]=="TriCheLD"
					){ // Cherenkov
				if (beta>0.5){
					che_time[(*in_triID)[ihit]+64] = (*in_trit)[ihit];
				}
			}
			else  if ((*in_triName)[ihit]=="TriCheU"
				||(*in_triName)[ihit]=="TriCheLU"
					){ // Cherenkov
				if (beta>0.5){
					che_time[(*in_triID)[ihit]] = (*in_trit)[ihit];
				}
			}
			else if ((*in_triName)[ihit]=="TriSciD"
//					||(*in_triName)[ihit]=="TriSciLD"
					){ // Scintillator
				if((*in_triedep)[ihit]>630e-6){
					sci_time[(*in_triID)[ihit]+64] = (*in_trit)[ihit];
				}
			}
			else if ((*in_triName)[ihit]=="TriSciU"
//					||(*in_triName)[ihit]=="TriSciLU"
					){ // Scintillator
				if((*in_triedep)[ihit]>630e-6){
					sci_time[(*in_triID)[ihit]] = (*in_trit)[ihit];
				}
			}
		}
		int nfoundU = 0;
		int nfoundD = 0;
		// option D
		for (int itri = 0; itri<128; itri++){
			int jtri = itri+1;
			if (jtri==64) jtri=0;
			if (jtri==128) jtri=64;
			if (fabs(sci_time[itri]-che_time[itri])<twindow&&sci_time[itri]>0&&che_time[itri]>0){
				if (fabs(sci_time[jtri]-che_time[jtri])<twindow&&sci_time[jtri]>0&&che_time[jtri]>0){
					if (fabs(sci_time[itri]-che_time[jtri])<twindow&&fabs(sci_time[jtri]-che_time[itri])<twindow){
						if (itri>=64) nfoundU++;
						else nfoundD++;
					}
				}
			}
		}
		//if (!found) continue;
		totalfoundU+=nfoundU;
		totalfoundD+=nfoundD;

		if (nfoundU>0){
			//out_x = (*in_x)[0];
			//out_y = (*in_y)[0];
			//out_z = (*in_z)[0];
			//out_px = (*in_px)[0];
			//out_py = (*in_py)[0];
			//out_pz = (*in_pz)[0];
			//out_pid = (*in_pid)[0];
			//out_t = (*in_t)[0];
			otree->Fill();
		}
	}
	std::cout<<"totalfoundU = "<<totalfoundU<<std::endl;
	std::cout<<"totalfoundD = "<<totalfoundD<<std::endl;
	otree->Write();
	ofile->Close();
}
