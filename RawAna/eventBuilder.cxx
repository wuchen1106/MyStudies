#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "TRandom.h"
#include "TString.h"
#include "TChain.h"
#include "TFile.h"
#include "TF1.h"
#include "TTree.h"
#include "TH1D.h"

TString MyData = getenv("MYDATA");
TString MyWork = getenv("MYWORKAREA");

Long64_t Nentries1pbI = 0;
Long64_t NeventsComb = 0;
Long64_t Nentries1I = 0;

int  C_nHits = 0;
int  M_nHits = 0;

int tsep = 1170;
int tleftCDC = 700;
//int trightCDC = tsep+tleftCDC;
int trightCDC = tsep+450;
int tleftCTH = tleftCDC;
int trightCTH = tsep;
int tresCTH = 10;

TF1 * f_xt = 0;

std::vector<int>     *C_cellID = 0;
std::vector<int>     *C_layerID = 0;
std::vector<int>     *C_tid = 0;
std::vector<double>  *C_edep = 0;
std::vector<double>  *C_stepL = 0;
std::vector<double>  *C_driftD = 0;
std::vector<double>  *C_driftDtrue = 0;
std::vector<double>  *C_tstart = 0;
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

int  oC_nHits = 0;
int  oM_nHits = 0;

std::vector<int>     *oC_cellID = 0;
std::vector<int>     *oC_layerID = 0;
std::vector<double>  *oC_edep = 0;
std::vector<double>  *oC_stepL = 0;
std::vector<double>  *oC_driftD = 0;
std::vector<double>  *oC_tstart = 0;
std::vector<int>     *oC_posflag = 0;
std::vector<int>     *oC_nPair = 0;
std::vector<double>  *oC_t = 0;
std::vector<double>  *oC_driftT = 0;
std::vector<double>  *oC_px = 0;
std::vector<double>  *oC_py = 0;
std::vector<double>  *oC_pz = 0;
std::vector<double>  *oC_x = 0;
std::vector<double>  *oC_y = 0;
std::vector<double>  *oC_z = 0;
std::vector<double>  *oC_wx = 0;
std::vector<double>  *oC_wy = 0;
std::vector<double>  *oC_wz = 0;
std::vector<int>     *oC_hittype = 0;
double				 oC_mt = 0;

std::vector<int>     *oM_volID = 0;
std::vector<std::string>  *oM_volName = 0;
std::vector<double>  *oM_edep = 0;
std::vector<double>  *oM_stepL = 0;
std::vector<double>  *oM_t = 0;
std::vector<double>  *oM_px = 0;
std::vector<double>  *oM_py = 0;
std::vector<double>  *oM_pz = 0;
std::vector<double>  *oM_x = 0;
std::vector<double>  *oM_y = 0;
std::vector<double>  *oM_z = 0;
std::vector<int>     *oM_hittype = 0;

double doCombine(int type, double offset, int ibunch);
void setBranches(TChain *c);
void branches(TTree * tree);
void ReSet();
double x2t(double x);

int main(int argc, char *argv[]){
	TRandom random;
	double me = 0.511e-3; // GeV
	std::vector<TString> DirName;
	std::vector<int> nProc;
	std::vector<TString> FileNames;
	std::vector<double> RelSizes;
	std::vector<int> RunTypes;
	std::vector<Long64_t> RunSeparator;
	//______________________________________________________________________________________________________
	// get the xt
	TFile * ifile_xt = new TFile("xt.GAS1-1.GASFILE.1850.0.33.6900681.0.root");
	f_xt = (TF1*) ifile_xt->Get("f1");

	//______________________________________________________________________________________________________
	// About this run
	if (argc <= 4) {
		std::cerr<<"Should provide combtype, dir/file, run size for beam.all, input files"<<std::endl;
		return -1;
	}
	int combtype = (int)strtol(argv[1],NULL,10);
	TString opt = argv[2];
	int noEntries = (int)strtol(argv[3],NULL,10);
	if (opt=="dir"){
		int ndirs = (argc-4);
		if (ndirs%4!=0){
			std::cerr<<"Should provide dir name, n jobs, weight, type"<<std::endl;
			return -1;
		}
		ndirs/=4;
		for (int i = 0; i<ndirs; i++){
			DirName.push_back(argv[4+i]);
			nProc.push_back((int)strtol(argv[4+i+ndirs],NULL,10));
			RelSizes.push_back((double)strtof(argv[4+i+ndirs*2],NULL));
			RunTypes.push_back((int)strtol(argv[4+i+ndirs*3],NULL,10));
		}
	}
	else {
		int nfiles = argc-4;
		if (nfiles%3!=0){
			std::cerr<<"Should provide file name, weight"<<std::endl;
			return -1;
		}
		nfiles/=3;
		for (int i = 0; i<nfiles; i++){
			FileNames.push_back(argv[4+i]);
			RelSizes.push_back((double)strtof(argv[4+i+nfiles],NULL));
			RunTypes.push_back((int)strtol(argv[4+i+nfiles*2],NULL,10));
		}
	}

	//______________________________________________________________________________________________________
	// input
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

	TChain *c1 = new TChain("tree"); // beam all
	TChain *c2 = new TChain("tree"); // dio.tail 
	TChain *c3 = new TChain("tree"); // pmc
	TChain *c0 = new TChain("tree"); // signal
	double RelSizes1 = 0;
	double RelSizes2 = 0;
	double RelSizes3 = 0;
	std::cerr<<"FileNames.size() = "<<(FileNames.size())<<std::endl;
	for (int i = 0; i<FileNames.size(); i++){
		std::cerr<<"FileNames["<<i<<"] = \""<<FileNames[i]<<"\", "<<RelSizes[i]<<std::endl;
		if (RunTypes[i]==1){
			RelSizes1 = RelSizes[i];
			std::cout<<"RelSizes1 = "<<RelSizes1<<std::endl;
			c1->Add(FileNames[i]);
		}
		else if (RunTypes[i]==0){
			c0->Add(FileNames[i]);
		}
		else if (RunTypes[i]==2){
			RelSizes2 = RelSizes[i];
			c2->Add(FileNames[i]);
		}
		else if (RunTypes[i]==3){
			RelSizes3 = RelSizes[i];
			c3->Add(FileNames[i]);
		}
	}
	std::cerr<<"DirName.size() = "<<DirName.size()<<std::endl;
	std::cerr<<"DirName                                            nProc RelSize Type"<<std::endl;
	int nProc1 = 0;
	for (int iRun = 0; iRun < nProc.size(); iRun++ ){
		std::cerr<<"DirName["<<iRun<<"] = \""<<DirName[iRun]<<"\", "<<nProc[iRun]<<", "<<RelSizes[iRun]<<", "<<RunTypes[iRun]<<std::endl;
		if (RunTypes[iRun]==1) nProc1 = nProc[iRun];
		if (RunTypes[iRun]==1)
			RelSizes1 = RelSizes[iRun];
		else if (RunTypes[iRun]==2)
			RelSizes2 = RelSizes[iRun];
		else if (RunTypes[iRun]==3)
			RelSizes3 = RelSizes[iRun];
		for (int i = 0; i<nProc[iRun]; i++){
			if (RunTypes[iRun]==1){
				c1->Add(Form(MyData+"/"+DirName[iRun]+"/%d_job0.raw",i));
				RunSeparator.push_back(c1->GetEntries());
			}
			else if (RunTypes[iRun]==0)
				c0->Add(Form(MyData+"/"+DirName[iRun]+"/%d_job0.raw",i));
			else if (RunTypes[iRun]==2)
				c2->Add(Form(MyData+"/"+DirName[iRun]+"/%d_job0.raw",i));
			else
				c3->Add(Form(MyData+"/"+DirName[iRun]+"/%d_job0.raw",i));
		}
	}

	setBranches(c1);
	setBranches(c2);
	setBranches(c3);
	setBranches(c0);

	int evt_num;
	c1->SetBranchAddress("evt_num",&evt_num);
	c1->SetBranchAddress("McTruth_pid",&McTruth_pid);
	c1->SetBranchAddress("McTruth_ppid",&McTruth_ppid);
	c1->SetBranchAddress("McTruth_ptid",&McTruth_ptid);
	c1->SetBranchAddress("McTruth_tid",&McTruth_tid);
	c1->SetBranchAddress("McTruth_time",&McTruth_time);
	c1->SetBranchAddress("McTruth_px",&McTruth_px);
	c1->SetBranchAddress("McTruth_py",&McTruth_py);
	c1->SetBranchAddress("McTruth_pz",&McTruth_pz);
	c1->SetBranchAddress("McTruth_x",&McTruth_x);
	c1->SetBranchAddress("McTruth_y",&McTruth_y);
	c1->SetBranchAddress("McTruth_z",&McTruth_z);
	c1->SetBranchAddress("McTruth_process",&McTruth_process);
	c1->SetBranchAddress("McTruth_volume",&McTruth_volume);
	c1->SetBranchAddress("McTruth_particleName",&McTruth_particleName);

	int topo;
	TChain * co = new TChain("t");
	co->Add("/scratchfs/bes/wuc/MyWorkArea/Simulate/comet/data/CyDet.ALL.150919.W500um.OptD2.1mmCFRP.DD35.151117.big.root");
	co->SetBranchAddress("topo",&topo);

	//______________________________________________________________________________________________________
	// output
	TFile * f = 0;
	if (combtype==0)
		f = new TFile("noise.root","RECREATE");
	else
		f = new TFile("signal.root","RECREATE");
	TTree *tree  = new TTree("tree","tree");
	branches(tree);
	//______________________________________________________________________________________________________
	// Determine cycle size
	double Nentries1 = c1->GetEntries();
	std::cout<<"Nentries1 = "<<Nentries1<<std::endl;
	double Nentries2 = c2->GetEntries();
	double Nentries3 = c3->GetEntries();
	double Nprotons1 = RelSizes1*1e9;
	double Nprotons2 = RelSizes2*1e9;
	double Nprotons3 = RelSizes3*1e9;
	double Nprotonspb = 2.5e12*tsep*1e-9*2.48/0.9;
	double Nentries1pb = Nentries1/Nprotons1*Nprotonspb;
	double Nentries2pb = Nentries2/Nprotons2*Nprotonspb;
	double Nentries3pb = Nentries3/Nprotons3*Nprotonspb;
	Nentries1pbI = (Long64_t) Nentries1pb;
	NeventsComb = (Long64_t) (Nentries1/Nentries1pbI);
	Nentries1I = (Long64_t) (Nentries1pbI*NeventsComb);
	double prob2 = Nentries2pb;
	double prob3 = Nentries3pb;
	if (c0->GetEntries()<NeventsComb&&combtype){
		std::cout<<"ERROR: not enough signal tracks!"<<std::endl;
		std::cout<<c3->GetEntries()<<"<"<<NeventsComb<<std::endl;
		return 1;
	}
	std::cerr<<"Totally combined events: "<<NeventsComb<<std::endl;
	std::cerr<<"Entries per event: "<<Nentries1pbI<<std::endl;
	std::cerr<<"prob2: "<<prob2<<std::endl;
	std::cerr<<"prob3: "<<prob3<<std::endl;

	//______________________________________________________________________________________________________
	// Prepare t0 shift by proton time spread
	float *t0_0 = (float*) malloc(sizeof(float)*NeventsComb);
	float *t0_1 = (float*) malloc(sizeof(float)*Nentries1I);
	float *t0_2 = (float*) malloc(sizeof(float)*Nentries2);
	float *t0_3 = (float*) malloc(sizeof(float)*Nentries3);
	random.RndmArray(NeventsComb,t0_0);
	random.RndmArray(Nentries1I,t0_1);
	random.RndmArray(Nentries2,t0_2);
	random.RndmArray(Nentries3,t0_3);

	//______________________________________________________________________________________________________
	// Loop in combined events
	int printModulo = 1e0;
	Long64_t cEntry0 = 0;
	Long64_t cEntry2 = 0;
	Long64_t cEntry3 = 0;
	for (Long64_t iEvent = 0; iEvent < NeventsComb; iEvent++ ){
//	for (Long64_t iEvent = 2; iEvent < 3; iEvent++ ){
		if (iEvent%printModulo==0) std::cerr<<iEvent<<std::endl;
		ReSet();
		for (int iBunch = -1; iBunch<=1; iBunch++){
			Long64_t sEntry = iEvent*Nentries1pbI+iBunch*Nentries1pbI;
			if (sEntry<0) sEntry+=Nentries1I;
			if (sEntry>Nentries1I) sEntry-=Nentries1I;
			for (Long64_t iEntry = sEntry; iEntry <Nentries1pbI+sEntry; iEntry++){
				c1->GetEntry(iEntry);
				int type = 0;
				int ifile = 0;
				for (ifile = 0; ifile<nProc1; ifile++){
					if (RunSeparator[ifile]>iEntry){
						break;
					}
				}
				Long64_t ioEntry = ifile*noEntries+evt_num;
				co->GetEntry(ioEntry);
				if (topo==-1000) type = 3;

				// is proton from muon capture?
				bool foundit = false;
				for (int iMC = 0; iMC<McTruth_pid->size(); iMC++){
					if ((*McTruth_pid)[iMC]==2212&&(*McTruth_process)[iMC]=="muMinusCaptureAtRest"){ // should record the position and simulate later
						foundit = true;
						std::cout<<"Found proton from muon capture @"<<iEntry<<std::endl;
					}
				}
				if (foundit) continue;
				doCombine(type,t0_1[iEntry]*100-50,iBunch);
			}
			int n2 = random.Poisson(prob2);
			for (int i2 = 0; i2<n2; i2++){
				c2->GetEntry(cEntry2);
				doCombine(-1,t0_2[cEntry2]*100-50,iBunch);
				cEntry2++;
			}
			int n3 = random.Poisson(prob3);
			for (int i3 = 0; i3<n3; i3++){
				c3->GetEntry(cEntry3);
				doCombine(-3,t0_3[cEntry3]*100-50,iBunch);
				cEntry3++;
			}
		}
		oC_mt = tleftCDC;
		if (combtype){
			c0->GetEntry(cEntry0);
			oC_mt = doCombine(1,t0_0[cEntry0]*100-50,0);
			cEntry0++;
		}
		tree->Fill(); // stack the previous track
	}

	tree->Write();
	f->Close();
	return 0;
}

double doCombine(int etype, double offset, int ibunch){
	double mt = tleftCTH;
	bool foundmt = false;
	// ========================================
	// CTH
	double tshiftsig = 0;
	for (int iHit = 0; iHit<M_nHits; iHit++){
		int type=etype;
		std::string volName = (*M_volName)[iHit];
		if (volName!="TriCheU"
		  &&volName!="TriCheD"
		  &&volName!="TriCheLU"
		  &&volName!="TriCheLD"
		  &&volName!="TriSciU"
		  &&volName!="TriSciD"
		  )
		  	continue;
		if (volName=="TriCheU"
		  ||volName=="TriCheD"
		  ||volName=="TriCheLU"
		  ||volName=="TriCheLD"
		  ){
		  	int pid = (*M_pid)[iHit];
		  	double m = 0;
		  	if (fabs(pid)==11) m = 0.511e-3;
			else if (fabs(pid)==13) m = 0.1056584;
			else if (fabs(pid)==211) m = 0.139570;
			else if (fabs(pid)==2212) m = 0.9382723;
			else continue;
			double px = (*M_px)[iHit];
			double py = (*M_py)[iHit];
			double pz = (*M_pz)[iHit];
			double pa = sqrt(px*px+py*py+pz*pz);
			double beta = pa/sqrt(pa*pa+m*m);
			if (beta<1/1.5) continue;
		}
		int volID = (*M_volID)[iHit];
		double t = (*M_t)[iHit]+offset;
		if (ibunch==-1){
			if (t<tsep+tleftCTH) continue;
			t=fmod(t,tsep);
		}
		else if (ibunch==1){
			t += tsep;
		}
		if (type==1){
			tshiftsig=fmod(t,tsep)-t;
			t+=tshiftsig;
			std::cout<<"signal track!"<<std::endl;
			if ((*M_tid)[iHit]==1){
				type = (1);
				if (!foundmt){
					mt = t;
					foundmt = true;
					std::cout<<"  mt = "<<(*M_t)[iHit]<<"+"<<offset<<"+"<<tshiftsig<<"="<<t<<std::endl;
					if (mt<tleftCTH||mt>trightCTH) mt=tleftCTH;
				}
			}
			else{
				type = (2);
			}
		}
		else if (type==-1){
			if ((*M_tid)[iHit]==1)
				type = (-1);
			else
				type = (-2);
		}
		else if (type==-2){
			if ((*M_tid)[iHit]==1)
				type = (-3);
			else
				type = (-4);
		}
		if (t<tleftCTH||t>trightCTH) continue; // not in the detection window
		bool foundit = false;
		for (int jHit = 0; jHit<oM_nHits; jHit++){
			if ((*oM_volName)[jHit]==volName&&(*oM_volID)[jHit]==volID){// overlapping
				if (fabs((*oM_t)[jHit]-t)>tresCTH) continue; // still can be separated
				foundit = true;
				(*oM_edep)[jHit] += (*M_edep)[iHit];
				(*oM_stepL)[jHit] += (*M_stepL)[iHit];
				if ((*oM_t)[jHit]>t){ // cover old one
					(*oM_t)[jHit] = t;
					(*oM_px)[jHit] = (*M_px)[iHit];
					(*oM_py)[jHit] = (*M_py)[iHit];
					(*oM_pz)[jHit] = (*M_pz)[iHit];
					(*oM_x)[jHit] = (*M_x)[iHit];
					(*oM_y)[jHit] = (*M_y)[iHit];
					(*oM_z)[jHit] = (*M_z)[iHit];
					(*oM_hittype)[jHit] = type;
				}
				break;
			}
		}
		if (!foundit){// new hit
			oM_volID->push_back(volID);
			oM_volName->push_back(volName);
			oM_t->push_back(t);
			oM_edep->push_back((*M_edep)[iHit]);
			oM_stepL->push_back((*M_stepL)[iHit]);
			oM_px->push_back((*M_px)[iHit]);
			oM_py->push_back((*M_py)[iHit]);
			oM_pz->push_back((*M_pz)[iHit]);
			oM_x->push_back((*M_x)[iHit]);
			oM_y->push_back((*M_y)[iHit]);
			oM_z->push_back((*M_z)[iHit]);
			oM_hittype->push_back(type);
			oM_nHits++;
		}
	}
	// ========================================
	// CDC
	for (int iHit = 0; iHit<C_nHits; iHit++){
		int type=etype;
		int layerID = (*C_layerID)[iHit];
		int cellID = (*C_cellID)[iHit];
		double driftD = (*C_driftDtrue)[iHit];
		double driftT = x2t(driftD);
		double t = (*C_t)[iHit]+offset;
		double tshift = 0;
		if (ibunch==-1){
			if (t+driftT<tsep+tleftCDC) continue;
			tshift=fmod(t+driftT-tleftCDC,tsep)-t-driftT+tleftCDC;
		}
		else if (ibunch==1){
			tshift = tsep;
		}
		t+=tshift;
		if (type==1){
			t+=tshiftsig;
		}
		if (type==1){
			if ((*C_tid)[iHit]==1)
				type = (1);
			else
				type = (2);
		}
		else if (type==-1){
			if ((*C_tid)[iHit]==1)
				type = (-1);
			else
				type = (-2);
		}
		else if (type==-3){
			if ((*C_tid)[iHit]==1)
				type = (-3);
			else
				type = (-4);
		}
		if (t+driftT<tleftCDC||t+driftT>trightCDC) continue; // not in the detection window
		bool foundit = false;
		for (int jHit = 0; jHit<oC_nHits; jHit++){
			if ((*oC_layerID)[jHit]==layerID&&(*oC_cellID)[jHit]==cellID){// overlapping
				foundit = true;
				if ((*oC_t)[jHit]+(*oC_driftT)[jHit]>t+driftT){ // cover old one
					(*oC_t)[jHit] = t;
					(*oC_driftT)[jHit] = driftT;
					(*oC_tstart)[jHit] = t+driftT;
					(*oC_driftD)[jHit] = driftD;
					(*oC_edep)[jHit] = (*C_edep)[iHit];
					(*oC_stepL)[jHit] = (*C_stepL)[iHit];
					(*oC_posflag)[jHit] = (*C_posflag)[iHit];
					(*oC_nPair)[jHit] = (*C_nPair)[iHit];
					(*oC_px)[jHit] = (*C_px)[iHit];
					(*oC_py)[jHit] = (*C_py)[iHit];
					(*oC_pz)[jHit] = (*C_pz)[iHit];
					(*oC_x)[jHit] = (*C_x)[iHit];
					(*oC_y)[jHit] = (*C_y)[iHit];
					(*oC_z)[jHit] = (*C_z)[iHit];
					(*oC_wx)[jHit] = (*C_wx)[iHit];
					(*oC_wy)[jHit] = (*C_wy)[iHit];
					(*oC_wz)[jHit] = (*C_wz)[iHit];
					(*oC_hittype)[jHit] = type;
				}
				break;
			}
		}
		if (!foundit){// new hit
			oC_cellID->push_back(cellID);
			oC_layerID->push_back(layerID);
			oC_t->push_back(t);
			oC_driftT->push_back(driftT);
			oC_tstart->push_back(t+driftT);
			oC_driftD->push_back(driftD);
			oC_edep->push_back((*C_edep)[iHit]);
			oC_stepL->push_back((*C_stepL)[iHit]);
			oC_posflag->push_back((*C_posflag)[iHit]);
			oC_nPair->push_back((*C_nPair)[iHit]);
			oC_px->push_back((*C_px)[iHit]);
			oC_py->push_back((*C_py)[iHit]);
			oC_pz->push_back((*C_pz)[iHit]);
			oC_x->push_back((*C_x)[iHit]);
			oC_y->push_back((*C_y)[iHit]);
			oC_z->push_back((*C_z)[iHit]);
			oC_wx->push_back((*C_wx)[iHit]);
			oC_wy->push_back((*C_wy)[iHit]);
			oC_wz->push_back((*C_wz)[iHit]);
			oC_hittype->push_back(type);
			oC_nHits++;
		}
	}
	return mt;
}

void setBranches(TChain *c){
	c->SetBranchAddress("CdcCell_nHits",&C_nHits);
	c->SetBranchAddress("CdcCell_layerID",&C_layerID);
	c->SetBranchAddress("CdcCell_cellID",&C_cellID);
	c->SetBranchAddress("CdcCell_tid",&C_tid);
	c->SetBranchAddress("CdcCell_edep",&C_edep);
	c->SetBranchAddress("CdcCell_stepL",&C_stepL);
	c->SetBranchAddress("CdcCell_driftD",&C_driftD);
	c->SetBranchAddress("CdcCell_driftDtrue",&C_driftDtrue);
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
}

void branches(TTree * tree){
	tree->Branch("CdcCell_nHits",&oC_nHits);
	tree->Branch("CdcCell_layerID",&oC_layerID);
	tree->Branch("CdcCell_cellID",&oC_cellID);
	tree->Branch("CdcCell_edep",&oC_edep);
	tree->Branch("CdcCell_stepL",&oC_stepL);
	tree->Branch("CdcCell_driftD",&oC_driftD);
	tree->Branch("CdcCell_driftT",&oC_driftT);
	tree->Branch("CdcCell_tstart",&oC_tstart);
	tree->Branch("CdcCell_posflag",&oC_posflag);
	tree->Branch("CdcCell_nPair",&oC_nPair);
	tree->Branch("CdcCell_t",&oC_t);
	tree->Branch("CdcCell_px",&oC_px);
	tree->Branch("CdcCell_py",&oC_py);
	tree->Branch("CdcCell_pz",&oC_pz);
	tree->Branch("CdcCell_x",&oC_x);
	tree->Branch("CdcCell_y",&oC_y);
	tree->Branch("CdcCell_z",&oC_z);
	tree->Branch("CdcCell_wx",&oC_wx);
	tree->Branch("CdcCell_wy",&oC_wy);
	tree->Branch("CdcCell_wz",&oC_wz);
	tree->Branch("CdcCell_hittype",&oC_hittype);
	tree->Branch("CdcCell_mt",&oC_mt);

	tree->Branch("M_nHits",&oM_nHits);
	tree->Branch("M_volName",&oM_volName);
	tree->Branch("M_volID",&oM_volID);
	tree->Branch("M_edep",&oM_edep);
	tree->Branch("M_stepL",&oM_stepL);
	tree->Branch("M_t",&oM_t);
	tree->Branch("M_px",&oM_px);
	tree->Branch("M_py",&oM_py);
	tree->Branch("M_pz",&oM_pz);
	tree->Branch("M_x",&oM_x);
	tree->Branch("M_y",&oM_y);
	tree->Branch("M_z",&oM_z);
	tree->Branch("M_hittype",&oM_hittype);
}

void ReSet(){
	oC_nHits = 0;
	oM_nHits = 0;

	if(oC_cellID) delete oC_cellID; oC_cellID = new std::vector<int>;
	if(oC_layerID) delete oC_layerID; oC_layerID = new std::vector<int>;
	if(oC_edep) delete oC_edep; oC_edep = new std::vector<double>;
	if(oC_stepL) delete oC_stepL; oC_stepL = new std::vector<double>;
	if(oC_driftD) delete oC_driftD; oC_driftD = new std::vector<double>;
	if(oC_driftT) delete oC_driftT; oC_driftT = new std::vector<double>;
	if(oC_tstart) delete oC_tstart; oC_tstart = new std::vector<double>;
	if(oC_posflag) delete oC_posflag; oC_posflag = new std::vector<int>;
	if(oC_nPair) delete oC_nPair; oC_nPair = new std::vector<int>;
	if(oC_t) delete oC_t; oC_t = new std::vector<double>;
	if(oC_px) delete oC_px; oC_px = new std::vector<double>;
	if(oC_py) delete oC_py; oC_py = new std::vector<double>;
	if(oC_pz) delete oC_pz; oC_pz = new std::vector<double>;
	if(oC_x) delete oC_x; oC_x = new std::vector<double>;
	if(oC_y) delete oC_y; oC_y = new std::vector<double>;
	if(oC_z) delete oC_z; oC_z = new std::vector<double>;
	if(oC_wx) delete oC_wx; oC_wx = new std::vector<double>;
	if(oC_wy) delete oC_wy; oC_wy = new std::vector<double>;
	if(oC_wz) delete oC_wz; oC_wz = new std::vector<double>;
	if(oC_hittype) delete oC_hittype; oC_hittype = new std::vector<int>;
	
	if(oM_volID) delete oM_volID; oM_volID = new std::vector<int>;
	if(oM_volName) delete oM_volName; oM_volName = new std::vector<std::string>;
	if(oM_edep) delete oM_edep; oM_edep = new std::vector<double>;
	if(oM_stepL) delete oM_stepL; oM_stepL = new std::vector<double>;
	if(oM_t) delete oM_t; oM_t = new std::vector<double>;
	if(oM_px) delete oM_px; oM_px = new std::vector<double>;
	if(oM_py) delete oM_py; oM_py = new std::vector<double>;
	if(oM_pz) delete oM_pz; oM_pz = new std::vector<double>;
	if(oM_x) delete oM_x; oM_x = new std::vector<double>;
	if(oM_y) delete oM_y; oM_y = new std::vector<double>;
	if(oM_z) delete oM_z; oM_z = new std::vector<double>;
	if(oM_hittype) delete oM_hittype; oM_hittype = new std::vector<int>;
}

double x2t(double x){
	return f_xt->Eval(fabs(x));
}
