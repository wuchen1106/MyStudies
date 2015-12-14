#include <vector>
#include <iostream>
#include <math.h>

#include "TChain.h"
#include "TFile.h"
#include "TH2D.h"

int main(int argc, char ** argv){
	TChain * c = new TChain("tree","tree");
	for ( int i = 1; i<argc; i++){
		c->Add(argv[i]);
	}
	double T = 10000./1405633*1e7/2.5e12;

	std::vector<double> * in_edep = 0;
	std::vector<double> * in_x = 0;
	std::vector<double> * in_y = 0;
	std::vector<double> * in_z = 0;
	std::vector<std::string> * in_volName = 0;
	std::vector<int> * in_volID = 0;
	c->SetBranchAddress("T2_edep",&in_edep);
	c->SetBranchAddress("T2_Ox",&in_x);
	c->SetBranchAddress("T2_Oy",&in_y);
	c->SetBranchAddress("T2_Oz",&in_z);
	c->SetBranchAddress("T2_volName",&in_volName);
	c->SetBranchAddress("T2_volID",&in_volID);

	TFile * ofile = new TFile("output.root","RECREATE");
	TH2D * h = new TH2D("h","h",90,0,90,600,0,60);

	double load[5] = {0};

	Long64_t N = c->GetEntries();
	for (Long64_t i = 0; i<N; i++){
		c->GetEntry(i);
		for (int j = 0; j<in_edep->size(); j++){
			double x = 300-(*in_x)[j];
			double z = (*in_y)[j];
			double y = 300-(*in_z)[j];
			double phi = atan(x/y);
			double dR = 300-sqrt(x*x+y*y);
			double r = sqrt(z*z+dR*dR);
			//std::cout<<(*in_volName)[j]<<"["<<(*in_volID)[j]<<"]: ("<<(*in_x)[j]<<","<<(*in_x)[j]<<","<<(*in_x)[j]<<")->("<<phi*180/3.14159265358<<","<<r<<")"<<std::endl;
			if (phi<0||phi>3.14159265358/2||r>60) continue;
			h->Fill(phi*180/3.14159265358,r,(*in_edep)[j]/(3.28986813367594211e+00*r)*1.6e-10/T);
			int index = 0;
			if ((*in_volName)[j].substr(0,4)=="Coil") index = 1;
			else if ((*in_volName)[j].substr(0,4)=="CrCo") index = 2;
			else if ((*in_volName)[j].substr(0,6)=="Shield") index = 3;
			else if ((*in_volName)[j].substr(0,4)=="Yoke") index = 4;
			else if ((*in_volName)[j].substr(0,4)=="Coll") index = 0;
			load[index]+=(*in_edep)[j]*1.6e-10/T;
		}
	}
	std::cout<<"Load:"<<std::endl;
	std::cout<<"  Colllimator "<<load[0]<<" W"<<std::endl;
	std::cout<<"  Coil:       "<<load[1]<<" W"<<std::endl;
	std::cout<<"  Cryostat:   "<<load[2]<<" W"<<std::endl;
	std::cout<<"  Shield:     "<<load[3]<<" W"<<std::endl;
	std::cout<<"  Yoke:       "<<load[4]<<" W"<<std::endl;

	h->Write();

	return 0;
}
