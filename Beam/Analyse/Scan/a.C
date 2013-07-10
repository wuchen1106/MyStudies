#include <iostream>
#include <string>
#include <sstream>

#include "TChain.h"

int main(int argc, char* argv[]){
	std::string dir = "/home/chen/MyWorkArea/Data/raw/comet_g4/phase_1";
	std::stringstream buff;

	TChain *c = new TChain("tree");
	for ( int i = 0; i < 100; i++ ){
		buff.str("");
		buff.clear();
		buff<<dir<<"/"<<i<<"_job0.raw";
		c->AddFile(buff.str().c_str());
		std::cout<<"Added "<<buff.str()<<std::endl;
	}
	c->SetScanField(0);
	c->Scan("ts2_0.position.fX*10:ts2_0.position.fY*10:ts2_0.position.fZ*10-1500:ts2_0.direction.fX*ts2_0.momentum:ts2_0.direction.fY*ts2_0.momentum:ts2_0.direction.fZ*ts2_0.momentum:ts2_0.globalTime","ts2_0.PDGEncoding==11&&ts2_0.direction.fZ>0&&ts2_0.origin.fX<450.15","colsize=15 precision=14");
	//c->Scan("ts2_0.position.fX*10:ts2_0.position.fY*10:ts2_0.position.fZ*10-1500:ts2_0.direction.fX*ts2_0.momentum:ts2_0.direction.fY*ts2_0.momentum:ts2_0.direction.fZ*ts2_0.momentum:ts2_0.globalTime","ts2_0.PDGEncoding==2112&&ts2_0.direction.fZ>0&&ts2_0.origin.fX<450.15","colsize=15 precision=14");
	//c->Scan("ts2_0.position.fX*10:ts2_0.position.fY*10:ts2_0.position.fZ*10-1500:ts2_0.direction.fX*ts2_0.momentum:ts2_0.direction.fY*ts2_0.momentum:ts2_0.direction.fZ*ts2_0.momentum:ts2_0.globalTime","ts2_0.PDGEncoding==13&&ts2_0.direction.fZ>0&&ts2_0.origin.fX<450.15","colsize=15 precision=14");
	return 0;
}
