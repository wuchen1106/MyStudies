#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "TString.h"
#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TVector3.h"

TString MyData = getenv("MYDATA");

//int getDADE(){
int main(int argc, char *argv[]){
	// About this run
	std::vector<TString> DirName;
	std::vector<int> nRuns;
	std::vector<TString> FileNames;
	 // ########Should Modify#########
//	DirName.push_back(MyData+"/20140802_Chen_Target002_G496P02");
//	DirName.push_back(MyData+"/20140803_Chen_Target002_G4100P01");
//	DirName.push_back(MyData+"/20140803_Chen_Target002_G496P02");
//	DirName.push_back(MyData+"/20140805_Chen_Iron56_18MeV_G496P02");
//	DirName.push_back(MyData+"/20140805_Chen_Iron56_1em9MeV_G496P02");
	DirName.push_back(MyData+"/20140805_Chen_Iron56_100MeV_G496P02");
//	DirName.push_back(MyData+"/20140805_Chen_Helium2_15p3MeV_G496P02");
	nRuns.push_back(4);
	 // ########Should Modify#########

	// input
	int  M_nHits = 0;
	int  McTruth_nTracks= 0;

	int  evt_num;
	int  run_num;
	int R0;
	int R1;

	std::vector<int>     *M_tid = 0;
	std::vector<double>  *M_ekin = 0;
	std::vector<double>  *M_px = 0;
	std::vector<double>  *M_py = 0;
	std::vector<double>  *M_pz = 0;

	std::vector<std::string>  *McTruth_particleName = 0;
	std::vector<int>     *McTruth_pid = 0;
	std::vector<double>  *McTruth_x = 0;
	std::vector<double>  *McTruth_px = 0;
	std::vector<double>  *McTruth_py = 0;
	std::vector<double>  *McTruth_pz = 0;
	std::vector<double>  *McTruth_ekin = 0;
	std::vector<double>  *McTruth_time = 0;
	std::vector<int>     *McTruth_ptid = 0;
	std::vector<std::string>  *McTruth_process = 0;

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
	c->SetBranchAddress("R0",&R0);
	c->SetBranchAddress("R1",&R1);

	c->SetBranchAddress("M_nHits",&M_nHits);
	c->SetBranchAddress("M_tid",&M_tid);
	c->SetBranchAddress("M_px",&M_px);
	c->SetBranchAddress("M_py",&M_py);
	c->SetBranchAddress("M_pz",&M_pz);
	c->SetBranchAddress("M_ekin",&M_ekin);

	c->SetBranchAddress("McTruth_nTracks",&McTruth_nTracks);
	c->SetBranchAddress("McTruth_particleName",&McTruth_particleName);
	c->SetBranchAddress("McTruth_pid",&McTruth_pid);
	c->SetBranchAddress("McTruth_time",&McTruth_time);
	c->SetBranchAddress("McTruth_ekin",&McTruth_ekin);
	c->SetBranchAddress("McTruth_x",&McTruth_x);
	c->SetBranchAddress("McTruth_px",&McTruth_px);
	c->SetBranchAddress("McTruth_py",&McTruth_py);
	c->SetBranchAddress("McTruth_pz",&McTruth_pz);
	c->SetBranchAddress("McTruth_ptid",&McTruth_ptid);
	c->SetBranchAddress("McTruth_process",&McTruth_process);

	// output
	TFile * f = new TFile("output.root","RECREATE");
	TTree *tree  = new TTree("t","t");

	double ekin;
	double paS;
	double theta;
	double phi;
	int nSecs;
	int gotit;
	std::string *process = 0;
	std::string *nuclear = 0;

	std::vector<std::string> *sec_particleName = 0;
	std::vector<double> * sec_time = 0;
	std::vector<double> * sec_ekin = 0;
	std::vector<double> * sec_theta = 0;
	std::vector<double> * sec_phi = 0;

	tree->Branch("evt_num",&evt_num);
	tree->Branch("run_num",&run_num);
	tree->Branch("R0",&R0);
	tree->Branch("R1",&R1);

	tree->Branch("nSecs",&nSecs);
	tree->Branch("gotit",&gotit);
	tree->Branch("ekin",&ekin);
	tree->Branch("paS",&paS);
	tree->Branch("theta",&theta);
	tree->Branch("phi",&phi);
	tree->Branch("process",&process);
	tree->Branch("nuclear",&nuclear);

	tree->Branch("s_particleName",&sec_particleName);
	tree->Branch("s_t",&sec_time);
	tree->Branch("s_ekin",&sec_ekin);
	tree->Branch("s_theta",&sec_theta);
	tree->Branch("s_phi",&sec_phi);

	// Loop in events
	Long64_t nEvents = c->GetEntries();
	std::cout<<"nEvents = "<<nEvents<<std::endl;
	int printModulo = 1;
	for (Long64_t iEvent = 0; iEvent < nEvents; iEvent++ ){
		if (iEvent%printModulo==0) std::cout<<(double)iEvent/nEvents*100<<" % ..."<<std::endl;
		c->GetEntry(iEvent);
		if(!sec_time) sec_time = new std::vector<double>;
		if(!sec_ekin) sec_ekin = new std::vector<double>;
		if(!sec_theta) sec_theta = new std::vector<double>;
		if(!sec_phi) sec_phi = new std::vector<double>;
		if(!sec_particleName) sec_particleName= new std::vector<std::string>;
		if(!nuclear) nuclear = new std::string;
		if(!process) process= new std::string;
		sec_time->clear();
		sec_ekin->clear();
		sec_phi->clear();
		sec_theta->clear();
		sec_particleName->clear();
		nSecs = 0;
		gotit = 0;
		paS = 0;
		TVector3 pS(0,0,0);
		*process = "!";
		*nuclear = "!";

		double x=1e14;
		if (McTruth_nTracks==1){
			*process = "NULL";
			*nuclear = "Primary";
		}
		else for (int iMc = 1; iMc < McTruth_nTracks; iMc++){
			if ((*McTruth_ptid)[iMc]==1){
				if (x==1e14) x=(*McTruth_x)[iMc];
				else if ((*McTruth_x)[iMc]!=x) continue;
				if (*process=="!") *process = ((*McTruth_process)[iMc]);
				if (*nuclear=="!"&&(*McTruth_pid)[iMc]>1.0001e9) *nuclear = ((*McTruth_particleName)[iMc]);
//				if (*nuclear=="!"&&(*McTruth_pid)[iMc]>2000) *nuclear = ((*McTruth_particleName)[iMc]);
				double time = (*McTruth_time)[iMc];
				double px = (*McTruth_px)[iMc];
				double py = (*McTruth_py)[iMc];
				double pz = (*McTruth_pz)[iMc];
				double theta = acos(pz/sqrt(px*px+py*py+pz*pz));
				double phi = atan(py/px);
				double ekin = (*McTruth_ekin)[iMc]*1000;
				pS+=TVector3(px,py,pz);
				sec_time->push_back(time);
				sec_ekin->push_back(ekin);
				sec_theta->push_back(theta);
				sec_phi->push_back(phi);
				sec_particleName->push_back((*McTruth_particleName)[iMc]);
				nSecs++;
			}
		}
		paS = pS.Mag();

		for (int iHit = 0; iHit < M_nHits; iHit++){
			if ((*M_tid)[iHit] == 1){
				double px = (*M_px)[iHit];
				double py = (*M_py)[iHit];
				double pz = (*M_pz)[iHit];
				theta = acos(pz/sqrt(px*px+py*py+pz*pz));
				phi = atan(py/px);
				ekin = (*M_ekin)[iHit]*1000;
				gotit = 1;
				break;
			}
		}

//		std::cout<<nSecs<<","<<gotit<<","<<*process<<","<<*nuclear<<std::endl;
//		std::cout<<"sec_particleName->size() = "<<sec_particleName->size()<<std::endl;
//		for(int i = 0; i<nSecs; i++){
//			std::cout<<(*sec_particleName)[i]<<",";
//		}
//		std::cout<<std::endl;

//		if (*process=="hadElastic"&&gotit==0)
//		if (*process=="NULL"&&theta!=0)
		tree->Fill();
	}

	tree->Write();
	f->Close();
	return 0;
}
