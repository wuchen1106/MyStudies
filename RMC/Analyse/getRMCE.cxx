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

TString MyData = getenv("MYDATA");
TString MyWork = getenv("MYWORKAREA");

void setName(std::string &name){
	if (name.substr(0,7)=="CdcCell") name="CdcCell";
	else if (name.substr(0,8)=="EndPlate") name="EndPlate";
	else if (name.substr(0,12)=="CdcFieldWire") name="CdcFieldWire";
	else if (name.substr(0,13)=="CdcSignalWire") name="CdcSignalWire";
	else if (name.substr(0,6)=="TriChe") name="TriChe";
	else if (name.substr(0,6)=="TriSci") name="TriSci";
	else if (name.substr(0,4)=="Yoke") name="Yoke";
	else if (name.substr(0,4)=="CrWa") name="CrWa";
	else if (name.substr(0,4)=="CrCo") name="CrCo";
	else if (name.substr(0,4)=="Coil") name="Coil";
	else if (name.substr(0,5)=="CoSup") name="CoSup";
	else if (name.substr(0,7)=="Blocker") name="Blocker";
	else if (name.substr(0,7)=="Window4") name="Window4";
}

int main(int argc, char *argv[]){
	std::stringstream buff;
	// About this run
	TString parName = "rmc";
	TString suffixName = "150919.W500um.OptD2.1mmCFRP.DD35.1cmLead";
	TString runName = parName+"."+suffixName;
	std::vector<TString> DirName;
	std::vector<int> nRuns;
	std::vector<TString> FileNames;
	 // ########Should Modify#########
//	FileNames.push_back(runName+".root");
	DirName.push_back(MyData+"/A9."+runName);
	nRuns.push_back(100);
	 // ########Should Modify#########

	// input
	int  M_nHits = 0;

	int  evt_num;
	int  run_num;
	double weight;

	std::vector<int>     *M_volID = 0;
	std::vector<std::string>  *M_volName = 0;
	std::vector<int>     *M_tid = 0;
	std::vector<int>     *M_pid = 0;
	std::vector<double>  *M_t = 0;
	std::vector<double>  *M_px = 0;
	std::vector<double>  *M_py = 0;
	std::vector<double>  *M_pz = 0;
	std::vector<double>  *M_x = 0;
	std::vector<double>  *M_y = 0;
	std::vector<double>  *M_z = 0;

	std::vector<int>     *McTruth_pid = 0;
	std::vector<int>     *McTruth_ppid = 0;
	std::vector<int>     *McTruth_ptid = 0;
	std::vector<int>     *McTruth_tid = 0;
	std::vector<double>  *McTruth_time = 0;
	std::vector<double>  *McTruth_px = 0;
	std::vector<double>  *McTruth_py = 0;
	std::vector<double>  *McTruth_pz = 0;
	std::vector<double>  *McTruth_x = 0;
	std::vector<double>  *McTruth_y = 0;
	std::vector<double>  *McTruth_z = 0;
	std::vector<std::string>  *McTruth_volume = 0;
	std::vector<std::string>  *McTruth_process = 0;
	std::vector<std::string>  *McTruth_particleName = 0;

	TChain *c = new TChain("tree");
	std::cout<<"FileNames.size() = "<<(FileNames.size())<<std::endl;
	for (int i = 0; i<FileNames.size(); i++){
		std::cout<<"FileNames["<<i<<"] = \""<<FileNames[i]<<"\""<<std::endl;
		c->Add(FileNames[i]);
	}
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


	c->SetBranchAddress("M_nHits",&M_nHits);
	c->SetBranchAddress("M_volName",&M_volName);
	c->SetBranchAddress("M_volID",&M_volID);
	c->SetBranchAddress("M_tid",&M_tid);
	c->SetBranchAddress("M_pid",&M_pid);
	c->SetBranchAddress("M_t",&M_t);
	c->SetBranchAddress("M_px",&M_px);
	c->SetBranchAddress("M_py",&M_py);
	c->SetBranchAddress("M_pz",&M_pz);
	c->SetBranchAddress("M_x",&M_x);
	c->SetBranchAddress("M_y",&M_y);
	c->SetBranchAddress("M_z",&M_z);

	c->SetBranchAddress("McTruth_pid",&McTruth_pid);
	c->SetBranchAddress("McTruth_ppid",&McTruth_ppid);
	c->SetBranchAddress("McTruth_ptid",&McTruth_ptid);
	c->SetBranchAddress("McTruth_tid",&McTruth_tid);
	c->SetBranchAddress("McTruth_time",&McTruth_time);
	c->SetBranchAddress("McTruth_px",&McTruth_px);
	c->SetBranchAddress("McTruth_py",&McTruth_py);
	c->SetBranchAddress("McTruth_pz",&McTruth_pz);
	c->SetBranchAddress("McTruth_x",&McTruth_x);
	c->SetBranchAddress("McTruth_y",&McTruth_y);
	c->SetBranchAddress("McTruth_z",&McTruth_z);
	c->SetBranchAddress("McTruth_process",&McTruth_process);
	c->SetBranchAddress("McTruth_volume",&McTruth_volume);
	c->SetBranchAddress("McTruth_particleName",&McTruth_particleName);
	c->SetBranchAddress("weight",&weight);

	// output
	TFile * f = new TFile(runName+".root","RECREATE");
	TTree *tree  = new TTree("t","t");

	int cpid; 

	// about common
	double O_t = 0;
	double O_px = 0;
	double O_py = 0;
	double O_pz = 0;
	double O_x = 0;
	double O_y = 0;
	double O_z = 0;

	// about topo
	std::vector<double> *o_t = 0;
	std::vector<double> *o_px = 0;
	std::vector<double> *o_py = 0;
	std::vector<double> *o_pz = 0;
	std::vector<double> *o_x = 0;
	std::vector<double> *o_y = 0;
	std::vector<double> *o_z = 0;
	std::vector<int> *o_dep = 0;
	std::vector<std::string> *process = 0;
	std::vector<std::string> *volume = 0;
	std::vector<std::string> *particle = 0;
	std::vector<int> *pid = 0;
	std::vector<int> *ppid = 0;

	tree->Branch("evt_num",&evt_num);
	tree->Branch("run_num",&run_num);
	tree->Branch("topo",&topo);
	tree->Branch("weight",&weight);

	// about common
	tree->Branch("pid",&cpid);
	tree->Branch("t",&O_t);
	tree->Branch("px",&O_px);
	tree->Branch("py",&O_py);
	tree->Branch("pz",&O_pz);
	tree->Branch("x",&O_x);
	tree->Branch("y",&O_y);
	tree->Branch("z",&O_z);

	// about topo
	tree->Branch("it",&o_t);
	tree->Branch("ipx",&o_px);
	tree->Branch("ipy",&o_py);
	tree->Branch("ipz",&o_pz);
	tree->Branch("ix",&o_x);
	tree->Branch("iy",&o_y);
	tree->Branch("iz",&o_z);
	tree->Branch("idep",&o_dep);
	tree->Branch("iprocess",&process);
	tree->Branch("ivolume",&volume);
	tree->Branch("iparticle",&particle);
	tree->Branch("ipid",&pid);
	tree->Branch("ippid",&ppid);

	// Loop in events
	Long64_t nEvents = c->GetEntries();
	std::cout<<"nEvents = "<<nEvents<<std::endl;
	int printModulo = 1e4;
	int printModulo2 = printModulo;
	for (Long64_t iEvent = 0; iEvent < nEvents; iEvent++ ){
		if (iEvent%printModulo==0) std::cout<<(double)iEvent/nEvents*100<<" % ..."<<std::endl;
		c->GetEntry(iEvent);
		if (iEvent%printModulo==0) std::cout<<"# evt_num = "<<iEvent<<", M_nHits = "<<M_nHits<<std::endl;
		if (M_nHits>0){

			// about topo
			if(o_t) delete o_t; o_t  = new std::vector<double>;
			if(o_px) delete o_px; o_px  = new std::vector<double>;
			if(o_py) delete o_py; o_py  = new std::vector<double>;
			if(o_pz) delete o_pz; o_pz  = new std::vector<double>;
			if(o_x) delete o_x; o_x  = new std::vector<double>;
			if(o_y) delete o_y; o_y  = new std::vector<double>;
			if(o_z) delete o_z; o_z  = new std::vector<double>;
			if(o_dep) delete o_dep; o_dep  = new std::vector<int>;
			if(process) delete process; process  = new std::vector<std::string>;
			if(volume) delete volume; volume  = new std::vector<std::string>;
			if(particle) delete particle; particle  = new std::vector<std::string>;
			if(pid) delete pid; pid  = new std::vector<int>;
			if(ppid) delete ppid; ppid  = new std::vector<int>;

			int pretid = -1;
			if (iEvent%printModulo==0) std::cout<<"==> Start looping in M hits, nHits = "<<M_nHits<<std::endl;
			for ( int iHit = 0; iHit<M_nHits;iHit++){
				int ttid = (*M_tid)[iHit];
				if (ttid!=pretid){ // new track
					// common
					O_t=(*M_t)[iHit];
					O_px=(*M_px)[iHit]*1000;
					O_py=(*M_py)[iHit]*1000;
					O_pz=(*M_pz)[iHit]*1000;
					O_x=(*M_x)[iHit]*10;
					O_y=(*M_y)[iHit]*10;
					O_z=(*M_z)[iHit]*10;
					if (fabs((*M_pid)[iHit])==11||(*M_pid)[iHit]==22){
						if (sqrt((*M_px)[iHit]*(*M_px)[iHit]+(*M_py)[iHit]*(*M_py)[iHit]+(*M_pz)[iHit]*(*M_pz)[iHit])<10e-3) continue;
						if (fmod((*M_t)[iHit],1170)<200) continue;
					}
					else if (fabs((*M_pid)[iHit])!=13&&fabs((*M_pid)[iHit])!=211) continue;

					if (iEvent%printModulo==0) std::cout<<"		Found new track!"<<std::endl;

					cpid = 0;
					pretid = ttid;

					// about topo
					if(o_t) delete o_t; o_t  = new std::vector<double>;
					if(o_px) delete o_px; o_px  = new std::vector<double>;
					if(o_py) delete o_py; o_py  = new std::vector<double>;
					if(o_pz) delete o_pz; o_pz  = new std::vector<double>;
					if(o_x) delete o_x; o_x  = new std::vector<double>;
					if(o_y) delete o_y; o_y  = new std::vector<double>;
					if(o_z) delete o_z; o_z  = new std::vector<double>;
					if(process) delete process; process  = new std::vector<std::string>;
					if(o_dep) delete o_dep; o_dep  = new std::vector<int>;
					if(volume) delete volume; volume  = new std::vector<std::string>;
					if(particle) delete particle; particle  = new std::vector<std::string>;
					if(pid) delete pid; pid  = new std::vector<int>;
					if(ppid) delete ppid; ppid  = new std::vector<int>;

					int maxdepth = 1;
					int idepth = 0;
					if (iEvent%printModulo2==0) std::cout<<"			==> Start looping in McTruth, nMc = "<<McTruth_pid->size()<<std::endl;
					for(int iMc = McTruth_pid->size()-1; iMc>=0; iMc--){
						if(ttid==(*McTruth_tid)[iMc]){
							if (cpid==0) cpid = (*McTruth_pid)[iMc];
//							if (idepth>maxdepth) break;
							ppid->push_back((*McTruth_ppid)[iMc]);
							pid->push_back((*McTruth_pid)[iMc]);
							process->push_back((*McTruth_process)[iMc]);
							particle->push_back((*McTruth_particleName)[iMc]);
							std::string name = (*McTruth_volume)[iMc];
							setName(name);
							volume->push_back(name);
							o_t->push_back((*McTruth_time)[iMc]);
							o_px->push_back((*McTruth_px)[iMc]*1000);
							o_py->push_back((*McTruth_py)[iMc]*1000);
							o_pz->push_back((*McTruth_pz)[iMc]*1000);
							o_x->push_back((*McTruth_x)[iMc]*10);
							o_y->push_back((*McTruth_y)[iMc]*10);
							o_z->push_back((*McTruth_z)[iMc]*10);
							o_dep->push_back(idepth);
							idepth++;
							ttid = (*McTruth_ptid)[iMc];
							if (iEvent%printModulo2==0) std::cout<<"				=>"<<iMc
								                                                       <<","<<idepth
								                                                       <<":"<<(*pid)[pid->size()-1]
								                                                       <<","<<ttid
								                                                       <<","<<(*o_px)[pid->size()-1]
								                                                       <<","<<(*volume)[pid->size()-1]
								                                                       <<","<<(*process)[pid->size()-1]
								                                                       <<std::endl;
						}
					}
					gettopo(topo,process,pid,o_px,o_py,o_pz);
					if (iEvent%printModulo2==0) std::cout<<"			<== End looping in McTruth, nMc = "<<McTruth_pid->size()<<std::endl;
					tree->Fill();
				}
			}
			if (iEvent%printModulo==0) std::cout<<"<== End looping in M hits"<<std::endl;
		}
	}
	tree->Write();
	f->Close();
	return 0;
}
