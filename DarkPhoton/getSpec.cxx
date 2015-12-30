#include <iostream>
#include <stdio.h>
#include "TGenPhaseSpace.h"
#include "TLorentzVector.h"
#include "TFile.h"
#include "TTree.h"
#include "TRandom.h"

int main(int argc, char** argv){
	double m_e = 0.511; // MeV
	double m_Ap = 30; // MeV
	double list_m[2];
	list_m[0] = 0.;
	list_m[1] = m_Ap;

	double Ebeam = 1890+m_e; // MeV
	double pbeam = sqrt(Ebeam*Ebeam-m_e*m_e);

	TGenPhaseSpace genPhase;
	TLorentzVector P0(0,0,pbeam,Ebeam+m_e);
	TLorentzVector * Pe,* Pa;
	genPhase.SetDecay(P0,2,list_m,"");

	TFile * ofile = new TFile("output.root","RECREATE");
	TTree * otree = new TTree("t","t");
	double ELAB_e = 0;
	double phiLAB_e = 0;
	double thetaLAB_e = 0;
	double ELAB_Ap = 0;
	double phiLAB_Ap = 0;
	double thetaLAB_Ap = 0;
	double phi = 0;
	double miss;
//	otree->Branch("phi",&phi);
//	otree->Branch("phie",&phiLAB_e);
	otree->Branch("thetae",&thetaLAB_e);
	otree->Branch("ee",&ELAB_e);
//	otree->Branch("phia",&phiLAB_Ap);
//	otree->Branch("thetaa",&thetaLAB_Ap);
//	otree->Branch("ea",&ELAB_Ap);
//	otree->Branch("me",&m_e);
//	otree->Branch("ma",&m_Ap);
//	otree->Branch("miss",&miss);

	TRandom random;

	for (int i =0; i<100000000; i++){
		genPhase.Generate();
		if (i%1000000==0) std::cout<<i/100000000.*100<<"%..."<<std::endl;

		Pe = genPhase.GetDecay(0);
		Pa = genPhase.GetDecay(1);

		thetaLAB_e=Pe->Theta();
		ELAB_e=Pe->Energy();
		if (thetaLAB_e<1.31e-2||thetaLAB_e>4.0154e-2) continue;
//		thetaLAB_Ap=Pa->Theta();
//		ELAB_Ap=Pa->Energy();
//		m_e=Pe->M();
//		m_Ap=Pa->M();

//		std::cout<<"E:"<<Pe->Energy();
		Pe->SetE(ELAB_e*(1+random.Gaus(0,((2000-ELAB_e)/1800.*3.7+0.2)/100)));
//		Pe->SetE(ELAB_e*(1+random.Gaus(0,5.08e-2)));
//		std::cout<<"->"<<Pe->Energy()<<std::endl;
		double temp = thetaLAB_e+random.Gaus(0,((2000-ELAB_e)/1800.*1.3+1.2))/4000;
//		double temp = thetaLAB_e+random.Gaus(0,2.5)/4000;
//		std::cout<<"temp = "<<thetaLAB_e+random.Gaus(0,((2000-ELAB_e)/1800.*1.3+1.2))/4000<<std::endl;
//		std::cout<<"Theta:"<<Pe->Theta();
		Pe->SetTheta(temp);
//		std::cout<<"->"<<Pe->Theta()<<std::endl;

//		miss = (P0-(*Pe)).M();
		thetaLAB_e=Pe->Theta();
		ELAB_e=Pe->Energy();

		otree->Fill();
//		printf("gamma: %lf,%lf,%lf,%lf,%lf\n",Pe->X(),Pe->Y(),Pe->Z(),Pe->M(),Pe->Energy());
//		printf("Ap: %lf,%lf,%lf,%lf,%lf\n",Pa->X(),Pa->Y(),Pa->Z(),Pa->M(),Pa->Energy());
	}
	otree->Write();
	ofile->Close();
	return 0;
}
