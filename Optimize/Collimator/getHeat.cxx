#include <vector>
#include <iostream>

#include "TChain.h"

int main(int argc, char ** argv){
	TChain * c = new TChain("tree","tree");
	for ( int i = 1; i<argc; i++){
		c->Add(argv[i]);
	}
	std::vector<double> * in_edep = 0;
	std::vector<double> * in_t = 0;
	std::vector<int> * in_volID = 0;
	c->SetBranchAddress("Coll_edep",&in_edep);
	c->SetBranchAddress("Coll_Ot",&in_t);
	c->SetBranchAddress("Coll_volID",&in_volID);

	double edep[10] = {0};
	double edept = 0;

	Long64_t N = c->GetEntries();
	for (Long64_t i = 0; i<N; i++){
		c->GetEntry(i);
		for (int j = 0; j<in_edep->size(); j++){
			edep[(*in_volID)[j]] += (*in_edep)[j]*1000;
			edept += (*in_edep)[j]*1000;
		}
	}

	std::cout<<"total: "<<edept<<std::endl;
	for (int i = 0; i<10; i++){
		std::cout<<i<<": "<<edep[i]<<std::endl;
	}

	return 0;
}
