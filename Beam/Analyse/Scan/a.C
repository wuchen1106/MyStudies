#include <iostream>
#include <string>
#include <sstream>

#include "TChain.h"

int main(int argc, char* argv[]){
	std::string dir = "/home/chen/MyWorkArea/Data/raw/comet_g4/phase_1";
	std::stringstream buff;

	TChain *c = new TChain("tree");
	for ( int i = 0; i < 0; i++ ){
		buff.str("");
		buff.clear();
		buff<<dir<<"/"<<i<<"_job0.raw";
		c->AddFile(buff.str().c_str());
		std::cout<<"Added "<<buff.str()<<std::endl;
	}

	std::string file = "/home/chen/MyWorkArea/Data/other/Andy/graphite-proton-target_length-60cm.root";
	c->AddFile(file.c_str());

	c->SetScanField(0);
	// *****************ts2_0*****************************
	// pion
	c->Scan("ts2_0.trackID:ts2_0.position.fX*10:ts2_0.position.fY*10:ts2_0.position.fZ*10-750:ts2_0.direction.fX*ts2_0.momentum:ts2_0.direction.fY*ts2_0.momentum:ts2_0.direction.fZ*ts2_0.momentum:ts2_0.globalTime","ts2_0.PDGEncoding==-211&&ts2_0.direction.fZ>0&&ts2_0.origin.fX<450.15","colsize=15 precision=14");
	// electron
	//c->Scan("ts2_0.trackID:ts2_0.position.fX*10:ts2_0.position.fY*10:ts2_0.position.fZ*10-750:ts2_0.direction.fX*ts2_0.momentum:ts2_0.direction.fY*ts2_0.momentum:ts2_0.direction.fZ*ts2_0.momentum:ts2_0.globalTime","ts2_0.PDGEncoding==11&&ts2_0.direction.fZ>0&&ts2_0.origin.fX<450.15","colsize=15 precision=14");
	// neutron
	//c->Scan("ts2_0.trackID:ts2_0.position.fX*10:ts2_0.position.fY*10:ts2_0.position.fZ*10-750:ts2_0.direction.fX*ts2_0.momentum:ts2_0.direction.fY*ts2_0.momentum:ts2_0.direction.fZ*ts2_0.momentum:ts2_0.globalTime","ts2_0.PDGEncoding==2112&&ts2_0.direction.fZ>0&&ts2_0.origin.fX<450.15","colsize=15 precision=14");
	// muon
	//c->Scan("ts2_0.trackID:ts2_0.position.fX*10:ts2_0.position.fY*10:ts2_0.position.fZ*10-750:ts2_0.direction.fX*ts2_0.momentum:ts2_0.direction.fY*ts2_0.momentum:ts2_0.direction.fZ*ts2_0.momentum:ts2_0.globalTime","ts2_0.PDGEncoding==13&&ts2_0.direction.fZ>0&&ts2_0.origin.fX<450.15","colsize=15 precision=14");
	// ******************blt0****************************
	// pion
	//c->Scan("blt0.trackID:blt0.position.fX*10:blt0.position.fY*10:blt0.position.fZ*10-750:blt0.direction.fX*blt0.momentum:blt0.direction.fY*blt0.momentum:blt0.direction.fZ*blt0.momentum:blt0.globalTime","blt0.PDGEncoding==-211&&blt0.direction.fZ>0&&blt0.origin.fX<300.05","colsize=15 precision=14");
	// electron
	//c->Scan("blt0.trackID:blt0.position.fX*10:blt0.position.fY*10:blt0.position.fZ*10-750:blt0.direction.fX*blt0.momentum:blt0.direction.fY*blt0.momentum:blt0.direction.fZ*blt0.momentum:blt0.globalTime","blt0.PDGEncoding==11&&blt0.direction.fZ>0&&blt0.origin.fX<300.05","colsize=15 precision=14");
	// neutron
	//c->Scan("blt0.trackID:blt0.position.fX*10:blt0.position.fY*10:blt0.position.fZ*10-750:blt0.direction.fX*blt0.momentum:blt0.direction.fY*blt0.momentum:blt0.direction.fZ*blt0.momentum:blt0.globalTime","blt0.PDGEncoding==2112&&blt0.direction.fZ>0&&blt0.origin.fX<300.05","colsize=15 precision=14");
	// muon
	//c->Scan("blt0.trackID:blt0.position.fX*10:blt0.position.fY*10:blt0.position.fZ*10-750:blt0.direction.fX*blt0.momentum:blt0.direction.fY*blt0.momentum:blt0.direction.fZ*blt0.momentum:blt0.globalTime","blt0.PDGEncoding==13&&blt0.direction.fZ>0&&blt0.origin.fX<300.05","colsize=15 precision=14");
	return 0;
}
