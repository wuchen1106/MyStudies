#include <iostream>
//#include <iomanip>
//#include <fstream>
//#include <sstream>
//#include <stdio.h>
#include <stdlib.h>
//#include <math.h>
#include <vector>

#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"

int main(int argc, char *argv[]){
	double minR = 0;
	double PI = 3.1415926;
	TString runName="input";
	TString dirName=".";
	int nProton = 1e6;
	//=======================================
	//*************read parameter**********
	int result;
	while((result=getopt(argc,argv,"hd:r:n:m:"))!=-1){
		switch(result){
			/* INPUTS */
			case 'r':
				runName=optarg;
				break;
			case 'd':
				dirName=optarg;
				break;
			case 'n':
				nProton = atoi(optarg);
				break;
			case 'm':
				minR = atof(optarg);
				break;
			case '?':
				printf("Wrong option! optopt=%c, optarg=%s\n", optopt, optarg);
				break;
			case 'h':
			default:
				return 1;
		}
	}

	// About this run
	TChain *c = new TChain("t");
	c->Add(dirName+"/"+runName+".root");
	double x,y,z,px,py,pz,time;
	int pid;
	c->SetBranchAddress("t",&time);

	int nEvents = c->GetEntries();
	nEvents = 10000;
	std::cout<<"nEvents = "<<nEvents<<std::endl;
	std::vector<int> indice;
	std::vector<double> times;
	for (int iEvent = 0; iEvent < nEvents; iEvent++ ){
		c->GetEntry(iEvent);
		indice.push_back(iEvent);
		times.push_back(time);
	}

	double tmin = 1e14;
	int index;
	for (int i = 0; i< indice.size(); i++ ){
		tmin = 1e14;
		for (int j = i; j< indice.size(); j++ ){
			time=times[j];
			index=indice[j];
			if (tmin>time){ // smaller than ever
				indice[j] = indice[i];
				indice[i] = index;
				times[j] = times[i];
				times[i] = time;
				tmin = time;
			}
		}
	}

	c->SetBranchAddress("pid",&pid);
	c->SetBranchAddress("x",&x);
	c->SetBranchAddress("y",&y);
	c->SetBranchAddress("z",&z);
	c->SetBranchAddress("px",&px);
	c->SetBranchAddress("py",&py);
	c->SetBranchAddress("pz",&pz);

	TFile * f  = new TFile("output.root","RECREATE");
	TTree * ot = new TTree("t","t");
	ot->Branch("t",&time);
	ot->Branch("pid",&pid);
	ot->Branch("x",&x);
	ot->Branch("y",&y);
	ot->Branch("z",&z);
	ot->Branch("px",&px);
	ot->Branch("py",&py);
	ot->Branch("pz",&pz);

	for (int i = 0; i< indice.size(); i++ ){
		c->GetEntry(indice[i]);
		ot->Fill();
	}
	ot->Write();
	f->Close();
}
