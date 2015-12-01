#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "TString.h"
#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"

TString MyData = getenv("MYDATA");
TString MyWork = getenv("MYWORKAREA");

void gettopo(int &topo,std::vector<std::string> * process,std::vector<int> * pid,std::vector<double> *o_px,std::vector<double> *o_py,std::vector<double> *o_pz);
void setName(std::string &name);
int gettype(double che_time[128], double sci_time[128],int che_ihit[128], int sci_ihit[128],int &type, int &thehit);
int rotate(int n0, int dn);

int main(int argc, char *argv[]){
	double me = 0.511; // MeV
	std::vector<TString> DirName;
	std::vector<int> nRuns;
	std::vector<TString> FileNames;
	std::vector<double> RelSizes;
	std::vector<int> RunSeparators;
	std::vector<int> RunTypes;
	//______________________________________________________________________________________________________
	// About this run
	if (argc <= 3) {
//		std::cerr<<"Should provide runname, dir/file, input files"<<std::endl;
		return -1;
	}
	TString runName = argv[1];
	TString opt = argv[2];
	if (opt=="dir"){
		int ndirs = (argc-3);
		if (ndirs%4!=0){
//			std::cerr<<"Should provide dir name, n jobs, weight, type"<<std::endl;
			return -1;
		}
		ndirs/=4;
		for (int i = 0; i<ndirs; i++){
			DirName.push_back(argv[3+i]);
			nRuns.push_back((int)strtol(argv[3+i+ndirs],NULL,10));
			RelSizes.push_back((double)strtof(argv[3+i+ndirs*2],NULL));
			RunTypes.push_back((int)strtol(argv[3+i+ndirs*3],NULL,10));
		}
	}
	else {
		int nfiles = argc-3;
		if (nfiles%3!=0){
//			std::cerr<<"Should provide file name, weight"<<std::endl;
			return -1;
		}
		nfiles/=3;
		for (int i = 0; i<nfiles; i++){
			FileNames.push_back(argv[3+i]);
			RelSizes.push_back((double)strtof(argv[3+i+nfiles],NULL));
			RunTypes.push_back((int)strtol(argv[3+i+nfiles*2],NULL,10));
		}
	}

	//______________________________________________________________________________________________________
	// input
	int  C_nHits = 0;
	int  M_nHits = 0;

	int  evt_num;
	int  run_num;
	double weight = 1;

	std::vector<int>     *C_cellID = 0;
	std::vector<int>     *C_layerID = 0;
	std::vector<int>     *C_tid = 0;
	std::vector<double>  *C_edep = 0;
	std::vector<double>  *C_stepL = 0;
	std::vector<double>  *C_driftD = 0;
	std::vector<double>  *C_driftDtrue = 0;
	std::vector<double>  *C_tstart = 0;
	std::vector<double>  *C_tstop = 0;
	std::vector<int>     *C_posflag = 0;
	std::vector<int>     *C_nPair = 0;
	std::vector<double>  *C_t = 0;
	std::vector<double>  *C_px = 0;
	std::vector<double>  *C_py = 0;
	std::vector<double>  *C_pz = 0;
	std::vector<double>  *C_x = 0;
	std::vector<double>  *C_y = 0;
	std::vector<double>  *C_z = 0;
	std::vector<double>  *C_wx = 0;
	std::vector<double>  *C_wy = 0;
	std::vector<double>  *C_wz = 0;

	std::vector<int>     *M_volID = 0;
	std::vector<std::string>  *M_volName = 0;
	std::vector<int>     *M_tid = 0;
	std::vector<int>     *M_pid = 0;
	std::vector<double>  *M_edep = 0;
	std::vector<double>  *M_stepL = 0;
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
//	std::cerr<<"FileNames.size() = "<<(FileNames.size())<<std::endl;
	for (int i = 0; i<FileNames.size(); i++){
//		std::cerr<<"FileNames["<<i<<"] = \""<<FileNames[i]<<"\", "<<RelSizes[i]<<std::endl;
		c->Add(FileNames[i]);
		RunSeparators.push_back(c->GetEntries());
	}
//	std::cerr<<"nRuns = "<<nRuns.size()<<std::endl;
	for (int iRun = 0; iRun < nRuns.size(); iRun++ ){
//		std::cerr<<"DirName["<<iRun<<"] = \""<<DirName[iRun]<<"\", "<<nRuns[iRun]<<", "<<RelSizes[iRun]<<std::endl;
		for (int i = 0; i<nRuns[iRun]; i++){
			c->Add(Form(MyData+"/"+DirName[iRun]+"/%d_job0.raw",i));
		}
		RunSeparators.push_back(c->GetEntries());
	}

	c->SetBranchAddress("evt_num",&evt_num);
	c->SetBranchAddress("run_num",&run_num);

	c->SetBranchAddress("CdcCell_nHits",&C_nHits);
	c->SetBranchAddress("CdcCell_layerID",&C_layerID);
	c->SetBranchAddress("CdcCell_cellID",&C_cellID);
	c->SetBranchAddress("CdcCell_tid",&C_tid);
	c->SetBranchAddress("CdcCell_edep",&C_edep);
	c->SetBranchAddress("CdcCell_stepL",&C_stepL);
	c->SetBranchAddress("CdcCell_driftD",&C_driftD);
	c->SetBranchAddress("CdcCell_driftDtrue",&C_driftDtrue);
	c->SetBranchAddress("CdcCell_tstop",&C_tstop);
	c->SetBranchAddress("CdcCell_tstart",&C_tstart);
	c->SetBranchAddress("CdcCell_posflag",&C_posflag);
	c->SetBranchAddress("CdcCell_nPair",&C_nPair);
	c->SetBranchAddress("CdcCell_t",&C_t);
	c->SetBranchAddress("CdcCell_px",&C_px);
	c->SetBranchAddress("CdcCell_py",&C_py);
	c->SetBranchAddress("CdcCell_pz",&C_pz);
	c->SetBranchAddress("CdcCell_x",&C_x);
	c->SetBranchAddress("CdcCell_y",&C_y);
	c->SetBranchAddress("CdcCell_z",&C_z);
	c->SetBranchAddress("CdcCell_wx",&C_wx);
	c->SetBranchAddress("CdcCell_wy",&C_wy);
	c->SetBranchAddress("CdcCell_wz",&C_wz);

	c->SetBranchAddress("M_nHits",&M_nHits);
	c->SetBranchAddress("M_volName",&M_volName);
	c->SetBranchAddress("M_volID",&M_volID);
	c->SetBranchAddress("M_tid",&M_tid);
	c->SetBranchAddress("M_pid",&M_pid);
	c->SetBranchAddress("M_edep",&M_edep);
	c->SetBranchAddress("M_stepL",&M_stepL);
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

	//______________________________________________________________________________________________________
	// output
	TFile * f = new TFile(runName+".root","RECREATE");
	TTree *tree  = new TTree("t","t");

	// about CTH hits
	int nHits;
	int type; 
	int tritype; 
	int tripos; 

	// about common
	int O_pid; 
	double O_t = 0;
	double O_px = 0;
	double O_py = 0;
	double O_pz = 0;
	double O_x = 0;
	double O_y = 0;
	double O_z = 0;

	// about topo
	int topo;
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
	tree->Branch("nHits",&nHits);
	tree->Branch("type",&type);
	tree->Branch("tritype",&tritype);
	tree->Branch("tripos",&tripos);
	tree->Branch("topo",&topo);
	tree->Branch("weight",&weight);

	// about common
	tree->Branch("pid",&O_pid);
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

	// Statistics
	double sci_time[128];
	double che_time[128];
	int che_ihit[128];
	int sci_ihit[128];
	int nHitsSU = 0;
	int nHitsCU = 0;
	int nHitsSD = 0;
	int nHitsCD = 0;
	int nHitsSUl = 0;
	int nHitsCUl = 0;
	int nHitsSDl = 0;
	int nHitsCDl = 0;
	double edepSU = 0;
	double edepCU = 0;
	double edepSD = 0;
	double edepCD = 0;
	double edepSUl = 0;
	double edepCUl = 0;
	double edepSDl = 0;
	double edepCDl = 0;
	double triggerrate = 0;
	double triggerrate2 = 0;

	// histograms
	TH1D* hSU = new TH1D("hSU","hSU",117,0,1170);
	TH1D* hCU = new TH1D("hCU","hCU",117,0,1170);
	TH1D* hSD = new TH1D("hSD","hSD",117,0,1170);
	TH1D* hCD = new TH1D("hCD","hCD",117,0,1170);
	TH1D* hTYPE = new TH1D("hTYPE","hTYPE",85,-5,80);
	TH1D* hTriRate = new TH1D("hTriRate","hTriRate",117,0,1170);
	TH1D* hSUedep = new TH1D("hSUedep","hSUedep",256,0,5000);
	TH1D* hCUedep = new TH1D("hCUedep","hCUedep",256,0,5000);
	TH1D* hSDedep = new TH1D("hSDedep","hSDedep",256,0,5000);
	TH1D* hCDedep = new TH1D("hCDedep","hCDedep",256,0,5000);

	//______________________________________________________________________________________________________
	// Loop in events
	Long64_t nEvents = c->GetEntries();
//	std::cerr<<"nEvents = "<<nEvents<<std::endl;
	int printModulo = 1e4;
	int printModulo2 = printModulo;
	int iRun = 0;
	for (Long64_t iEvent = 0; iEvent < nEvents; iEvent++ ){
//		if (iEvent%printModulo==0) std::cerr<<(double)iEvent/nEvents*100<<" % ..."<<std::endl;
		c->GetEntry(iEvent);
		if (iEvent>RunSeparators[iRun]) iRun++;
		weight=8.06e6/1e9/RelSizes[iRun]/1170e-6;
		//=====================================================================
		// Get CDC hit rate
		// Get CDC track info
		for ( int iHit = 0; iHit<C_nHits;iHit++){
		}
		//=====================================================================
		// Get CTH hit rate
		// Get CTH track info
		nHits = 0;
		for (int itri = 0; itri<128; itri++){
			che_time[itri] = -1;
			che_ihit[itri] = -1;
			sci_time[itri] = -1;
			sci_ihit[itri] = -1;
		}
		for ( int iHit = 0; iHit<M_nHits;iHit++){
			if ((*M_volName)[iHit]=="TriCheU"||(*M_volName)[iHit]=="TriCheLU"){
				tritype = 0;
				tripos = -1;
			}
			else if ((*M_volName)[iHit]=="TriCheD"||(*M_volName)[iHit]=="TriCheLD"){
				tritype = 0;
				tripos = 1;
			}
			else if ((*M_volName)[iHit]=="TriSciU"){
				tritype = 1;
				tripos = -1;
			}
			else if ((*M_volName)[iHit]=="TriSciD"){
				tritype = 1;
				tripos = 1;
			}
			else {
				tritype = -1;
				tripos = 0;
			}
			double hittime = (*M_t)[iHit];
			double hittimemod = fmod(hittime,1170);
			if (tritype==0&&fabs((*M_pid)[iHit])==11){
				double pa = sqrt((*M_px)[iHit]*(*M_px)[iHit]+(*M_py)[iHit]*(*M_py)[iHit]+(*M_pz)[iHit]*(*M_pz)[iHit])*1000;
				double beta = pa/sqrt(pa*pa+me*me);
				if (beta>1/1.5){
					che_time[(*M_volID)[iHit]+(tripos+1)*32] = hittime;
					che_ihit[(*M_volID)[iHit]+(tripos+1)*32] = iHit;
					if (tripos>0){
						nHitsCD+=weight;
						edepCD+=(*M_edep)[iHit]*weight;
						if (hittimemod>200){
							nHitsCDl+=weight;
							edepCDl+=(*M_edep)[iHit]*weight;
						}
						hCD->Fill(hittimemod,weight);
						hCDedep->Fill((*M_edep)[iHit]*1e6,weight);
					}
					else {
						nHitsCU+=weight;
						edepCU+=(*M_edep)[iHit]*weight;
						if (hittimemod>200){
							nHitsCUl+=weight;
							edepCUl+=(*M_edep)[iHit]*weight;
						}
						hCU->Fill(hittimemod,weight);
						hCUedep->Fill((*M_edep)[iHit]*1e6,weight);
					}
				}
			}
			else if (tritype==1){
				if ((*M_edep)[iHit]>630e-6){
					sci_time[(*M_volID)[iHit]+(tripos+1)*32] = hittime;
					sci_ihit[(*M_volID)[iHit]+(tripos+1)*32] = iHit;
				}
				if (tripos>0){
					if ((*M_edep)[iHit]>63e-6){
						nHitsSD+=weight;
					}
					edepSD+=(*M_edep)[iHit]*weight;
					if (hittimemod>200){
						if ((*M_edep)[iHit]>63e-6){
							nHitsSDl+=weight;
						}
						edepSDl+=(*M_edep)[iHit]*weight;
					}
					hSD->Fill(hittimemod,weight*(*M_edep)[iHit]);
					hSDedep->Fill((*M_edep)[iHit]*1e6,weight);
				}
				else{
					if ((*M_edep)[iHit]>63e-6){
						nHitsSU+=weight;
					}
					edepSU+=(*M_edep)[iHit]*weight;
					if (hittimemod>200){
						if ((*M_edep)[iHit]>63e-6){
							nHitsSUl+=weight;
						}
						edepSUl+=(*M_edep)[iHit]*weight;
					}
					hSU->Fill(hittimemod,weight*(*M_edep)[iHit]);
					hSUedep->Fill((*M_edep)[iHit]*1e6,weight);
				}
			}
			nHits++;
		}
		int thehit;
		gettype(che_time,sci_time,che_ihit,sci_ihit,type,thehit);
		hTYPE->Fill(type,weight);
		if (thehit<0) continue;
		int ttid = (*M_tid)[thehit];
		// common
		O_pid = 0;
		O_t=fmod((*M_t)[thehit],1170);
		O_px=(*M_px)[thehit]*1000;
		O_py=(*M_py)[thehit]*1000;
		O_pz=(*M_pz)[thehit]*1000;
		O_x=(*M_x)[thehit]*10;
		O_y=(*M_y)[thehit]*10;
		O_z=(*M_z)[thehit]*10;
		if ((type>=53&&type<=56)||(type>=49&&type<=51)||(type>=73&&type<=77)||(type>=69&&type<=72)){
			hTriRate->Fill(O_t,weight);
			if (O_t>200)
				triggerrate+=weight;
			if (O_t>500)
				triggerrate2+=weight;
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
			int idepth = 0;
			if (RunTypes[iRun]==1||RunTypes[iRun]==2){// dio tail or pmc
				ppid->push_back(-1);
				pid->push_back(-13);
				process->push_back("NULL");
				particle->push_back("mu-");
				volume->push_back("TS2");
				o_t->push_back(0);
				o_px->push_back(0);
				o_py->push_back(0);
				o_pz->push_back(0);
				o_x->push_back(0);
				o_y->push_back(0);
				o_z->push_back(0);
				o_dep->push_back(idepth);
				idepth++;
			}
			for(int iMc = McTruth_pid->size()-1; iMc>=0; iMc--){
				if(ttid==(*McTruth_tid)[iMc]){
					if (O_pid==0) O_pid = (*McTruth_pid)[iMc];
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
				}
			}
			gettopo(topo,process,pid,o_px,o_py,o_pz);
			tree->Fill(); // stack the previous track
		}
	}

	hCU->Write();
	hCD->Write();
	hSU->Write();
	hSD->Write();
	hCUedep->Write();
	hCDedep->Write();
	hSUedep->Write();
	hSDedep->Write();
	hTriRate->Write();
	hTYPE->Write();
	tree->Write();
	f->Close();
	std::cout<<(nHitsCU-nHitsCUl)/64.*1.17<<" "<<nHitsCUl/64.*1.17
		<<" "<<(nHitsCD-nHitsCDl)/64.*1.17<<" "<<nHitsCDl/64.*1.17
		<<" "<<(nHitsSU-nHitsSUl)/64.*1.17<<" "<<nHitsSUl/64.*1.17
		<<" "<<(nHitsSD-nHitsSDl)/64.*1.17<<" "<<nHitsSDl/64.*1.17
		<<" "<<(edepSU-edepSUl)/64.*1.17<<" "<<edepSUl/64.*1.17
		<<" "<<(edepSD-edepSDl)/64.*1.17<<" "<<edepSDl/64.*1.17
		<<" "<<triggerrate<<" "<<triggerrate2<<std::endl;
	return 0;
}

int rotate(int n0, int dn){
	int n1 = n0+dn;
	if (n0<64&&n1>=64) n1-=64;
	if (n0>=64&&n1<64) n1+=64;
	if (n1>=128) n1-=64;
	if (n1<0) n1+=64;
	return n1;
}

int gettype(double che_time[128], double sci_time[128],int che_ihit[128], int sci_ihit[128],int &type, int &thehit){
	// option D
	type = 0;
	thehit = -1;
	std::vector<int> vtype;
	std::vector<int> vihit;
	for (int itri = 0; itri<128; itri++){
		double time = sci_time[itri];
		if (time==-1) continue;
		// how many hit in sci in row? (starting from this counter)
		int nsci = 1;
		for (int delta = 1; delta<=2; delta++){
			int jtri = rotate(itri,delta);
			if (sci_time[jtri]!=-1&&fabs(sci_time[jtri]-time)<10) nsci++;
			else break;
		}
		//			std::cout<<itri<<"-"<<nsci<<"->"<<rotate(itri,nsci)<<std::endl;
		for (int delta = -2; delta<=2; delta++){
			int jtri = rotate(itri,delta);
			if (che_time[jtri]==-1||fabs(che_time[jtri]-time)>=10) continue;
			int nche = 1;
			for (int delta = 1; delta<=2; delta++){
				int ktri = rotate(jtri,delta);
				if (che_time[ktri]!=1&&fabs(che_time[ktri]-time)<10) nche++;
				else break;
			}
			//				std::cout<<"  "<<jtri<<"-"<<nche<<"->"<<rotate(jtri,nche)<<std::endl;
			//				std::cout<<"  "<<nsci*100+nche*10+delta<<std::endl;
			vtype.push_back(nsci*20+nche*5+delta);
			vihit.push_back(sci_ihit[itri]);
		}
	}
	int nhitmax = -1;
	for (int it = 0; it<vtype.size(); it++){
		int nhit = (vtype[it]+2)/5+(vtype[it]+2)/20;
		if (nhitmax<nhit) nhitmax = nhit;
	}
	int ndeltamin = 10;
	for (int it = 0; it<vtype.size(); it++){
		if (nhitmax!=(vtype[it]+2)/5+(vtype[it]+2)/20) continue;
		if (fabs(ndeltamin)>fabs(vtype[it]%5)){
			ndeltamin = vtype[it]%10;
			thehit = vihit[it];
			type = vtype[it];
		}
	}
	return 0;
}

void setName(std::string &name){
	if (name.substr(0,7)=="CdcCell") name="CdcCell";
	else if (name.substr(0,8)=="EndPlate") name="EndPlate";
	else if (name.substr(0,12)=="CdcFieldWire") name="CdcFieldWire";
	else if (name.substr(0,13)=="CdcSignalWire") name="CdcSignalWire";
	else if (name.substr(0,6)=="TriChe") name="TriChe";
	else if (name.substr(0,6)=="TriSci") name="TriSci";
	else if (name.substr(0,7)=="TriSciL") name="TriSciL";
	else if (name.substr(0,7)=="TriSciP") name="TriSciP";
	else if (name.substr(0,7)=="TriCheP") name="TriCheP";
	else if (name.substr(0,4)=="Yoke") name="Yoke";
	else if (name=="CrCoTS23_I2_inner") name="I2inner";
	else if (name=="CrCoTS23_I3_inner") name="I3inner";
	else if (name=="CrWaTSBS_I4_inner") name="I4inner";
	else if (name=="CrWaTSBS_I5_inner") name="I5inner";
	else if (name.substr(0,4)=="CrWa") name="CrWa";
	else if (name.substr(0,4)=="CrCo") name="CrCo";
	else if (name.substr(0,4)=="Coil") name="Coil";
	else if (name.substr(0,5)=="CoSup") name="CoSup";
	else if (name.substr(0,7)=="Blocker") name="Blocker";
	else if (name.substr(0,9)=="TS3Window") name="TS3Window";
}

void gettopo(int &topo,std::vector<std::string> * process,std::vector<int> * pid,std::vector<double> *o_px,std::vector<double> *o_py,std::vector<double> *o_pz){
	if (pid->size()<=1) topo = 0; // from primary
	else if ((*pid)[pid->size()-1]==2112) topo = -1; // from neutron
	else if ((*pid)[pid->size()-1]==-211) topo = -2; // from pion
	else if ((*pid)[pid->size()-1]==13){ // from muon
		if ((*process)[process->size()-2]=="muMinusCaptureAtRest"){
			topo=1000;
			if (pid->size()==2){ // directly from muon
				topo += 0;
			}
			else{ // indirectly from muon
				if ((*pid)[pid->size()-2]==22) topo += 1;
				else if ((*pid)[pid->size()-2]==2212) topo += 2;
				else if ((*pid)[pid->size()-2]==11){
					double px_temp = (*o_px)[o_px->size()-2];
					double py_temp = (*o_py)[o_py->size()-2];
					double pz_temp = (*o_pz)[o_pz->size()-2];
					double pa_temp=sqrt(px_temp*px_temp+py_temp*py_temp+pz_temp*pz_temp);
					if (pa_temp>1) topo += 3;
					else topo += 4;
				}
				else if ((*pid)[pid->size()-2]==2112) topo += 5;
				else topo += 6;
			}
		}
		else if ((*process)[process->size()-2]=="Decay"){
			topo=2000;
		}
		else if ((*process)[process->size()-2]=="muIoni"){
			topo=3000;
		}
	}
	else topo = 1; // from other particles
	return;
}
