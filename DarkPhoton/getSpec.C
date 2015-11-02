{
	double m_e = 0.511; // MeV
	double m_Ap = 10; // MeV
	double list_m[2];
	list_m[0] = 0.;
	list_m[1] = m_Ap;

	double Ebeam = 1890; // MeV
	double pbeam = sqrt(Ebeam*Ebeam-m_e*m_e);

	TGenPhaseSpace genPhase;
	TLorentzVector P0(0,0,pbeam,Ebeam+m_e);
	TLorentzVector * Pe,Pa;
	genPhase.SetDecay(P0,2,list_m,"");

	TFile * ofile = new TFile("output.root","RECREATE");
	TTree * otree = new TTree("t","t");
	double ELAB_e = 0;
	double ELAB_Ap = 0;
	double phiLAB_e = 0;
	double phiLAB_Ap = 0;
	otree->Branch("phie",&phiLAB_e);
	otree->Branch("phia",&phiLAB_Ap);
	otree->Branch("ee",&ELAB_e);
	otree->Branch("ea",&ELAB_Ap);
	otree->Branch("me",&m_e);
	otree->Branch("ma",&m_Ap);

	TH1D * he1 = new TH1D("he1","he1",1024,0,10000);
	TH1D * he2 = new TH1D("he2","he2",1024,0,10000);

	for (int i =0; i<10000; i++){
		genPhase.Generate();
		Pe = genPhase.GetDecay(0);
		Pa = genPhase.GetDecay(1);
		phiLAB_e=Pe->Theta();
		phiLAB_Ap=Pa->Theta();
		ELAB_e=Pe->Energy();
		ELAB_Ap=Pa->Energy();
		m_e=Pe->M();
		m_Ap=Pa->M();
		he1->Fill(ELAB_e);
		he2->Fill(ELAB_Ap);
		otree->Fill();
//		printf("gamma: %lf,%lf,%lf,%lf,%lf\n",Pe->X(),Pe->Y(),Pe->Z(),Pe->M(),Pe->Energy());
//		printf("Ap: %lf,%lf,%lf,%lf,%lf\n",Pa->X(),Pa->Y(),Pa->Z(),Pa->M(),Pa->Energy());
	}
	otree->Write();
	he1->Write();
	he2->Write();
	ofile->Close();
}
