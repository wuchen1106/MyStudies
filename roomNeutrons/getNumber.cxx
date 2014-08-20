#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "TString.h"
#include "TChain.h"

int main(int argc, char *argv[]){
	double minR = 0;
	double PI = 3.1415926;
	TString runName="AfterBending_BM2";
	TString dirName="140603";
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
	double scale = 2.5E12*2.4/nProton;
	TChain *c = new TChain("RooTracker");
	c->Add(dirName+"/"+runName+".root");
	double area = 1;
	if (runName == "AfterBending_BM2"
	   )
		area = PI*70*70;
	else if (runName == "BeamMonitor_AND"
			)
		area = PI*18*18;
	else if (runName == "BeamMonitor_BeginCDC"
			||runName == "BeamMonitor_EndCDC"
			)
		area = PI*(83.5*83.5-49.8*49.8);
	else if (runName == "BeamMonitor_InnerCDC"
			)
		area = 2*PI*49.8*164.7;
	else if (runName == "BeamMonitor_OuterCDC"
			)
		area = 2*PI*83.5*164.7;
	else if (runName == "BeamMonitor_EndPlate"
			)
		area = PI*50*50;
	if (minR) area = PI*minR*minR;

	int pid;
	float weight = 0;
	float time = 0;
	float x = 0;
	float y = 0;
	float z = 0;
	float px = 0;
	float py = 0;
	float pz = 0;
	c->SetBranchAddress("pdgcode",&pid);
	c->SetBranchAddress("time",&time);
	c->SetBranchAddress("x",&x);
	c->SetBranchAddress("y",&y);
	c->SetBranchAddress("z",&z);
	c->SetBranchAddress("px",&px);
	c->SetBranchAddress("py",&py);
	c->SetBranchAddress("pz",&pz);
	c->SetBranchAddress("weight",&weight);

	int nEvents = c->GetEntries();
//	std::cout<<"nEvents = "<<nEvents<<std::endl;
	std::stringstream buff;
	double total = 0;
	for (int iEvent = 0; iEvent < nEvents; iEvent++ ){
//		if (iEvent%1000==0) std::cout<<(double)iEvent/nEvents*100<<" % ..."<<std::endl;
		c->GetEntry(iEvent);
		bool pass = false;
		if (runName == "AfterBending_BM2"
				||runName == "BeamMonitor_AND"
		   ){
			if (px<0) pass = true;
		}
		else if (runName == "BeamMonitor_BeginCDC"
				||runName == "BeamMonitor_EndCDC"
		   )
		   	pass = true;
		else if (runName == "BeamMonitor_InnerCDC"
		   )
		   	pass = true;
		else if (runName == "BeamMonitor_OuterCDC"
		   )
		   	pass = true;
		else if (runName == "BeamMonitor_EndPlate"
		   )
		   	pass = true;
		if (minR&&sqrt(y*y+(z+765)*(z+765))>minR)
			pass = false;
		if (pid!=2112)
			pass = false;

		if (pass){
			total+=weight;
		}
	}
	std::cout<<dirName<<"\t"
	         <<runName<<"\t"
	         <<area<<"\t"
	         <<total<<"\t"
	         <<total/area*scale
	         <<std::endl;
}
