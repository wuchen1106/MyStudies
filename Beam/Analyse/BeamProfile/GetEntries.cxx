#include <string>
#include <iostream>

#include "globals.hh"

#include "TChain.h"

int main(int argc, char** argv){

	std::string m_rootfile = argv[optind++];

	TChain *c = new TChain("t");
	c->AddFile(m_rootfile.c_str());
	int nEvents = c->GetEntries();
	std::cout<<nEvents<<std::endl;

	return nEvents;
}
