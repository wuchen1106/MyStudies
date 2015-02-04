{
	TFile * f = new TFile("/home/chen/MyWorkArea/Simulate/comet/output/raw_g4sim.root");
	TTree * t = (TTree*) f->Get("tree");
	std::vector<std::string> *vol;
	std::vector<double> *edep;
	std::vector<int> *pid;
	t->SetBranchAddress("Coil_volName",&vol);
	t->SetBranchAddress("Coil_edepAll",&edep);
	t->SetBranchAddress("Coil_pid",&pid);

	double htot[10] = {0};
	double hneu[10] = {0};
	double hgam[10] = {0};
	int N = t->GetEntries();
	for (int i = 0; i<t->GetEntries(); i++){
		if (i%10==0) printf("%ld%\n",(double)i/N*100);
		t->GetEntry(i);
		for ( int j=0; j<vol->size(); j++){
			int index = 0;
			if ((*vol)[j]=="CoilCS0")	index = 0;
			else if ((*vol)[j]=="CoilCS1")	index = 1;
			else if ((*vol)[j]=="CoilMS0")	index = 2;
			else if ((*vol)[j]=="CoilMS1")	index = 3;
			else if ((*vol)[j]=="CoilTS1a")	index = 4;
			else if ((*vol)[j]=="CoilTS1b")	index = 5;
			else if ((*vol)[j]=="CoilTS1c")	index = 6;
			else if ((*vol)[j]=="CoilTS1d")	index = 7;
			else if ((*vol)[j]=="CoilTS1e")	index = 8;
			else if ((*vol)[j]=="CoilTS1f")	index = 9;
			if ((*pid)[j]==2112) hneu[index]+=(*edep)[j];
			else if ((*pid)[j]==22) hgam[index]+=(*edep)[j];
			htot[index]+=(*edep)[j];
		}
	}
	for (int i = 0; i<10; i++){
		//printf("%d %lf %lf %lf\n",i,htot[i],hneu[i],hgam[i]);
		printf("%d %lf\n",i,htot[i]);
	}
}
