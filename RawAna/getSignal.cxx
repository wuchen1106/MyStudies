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

int gettype(double che_time[128], double sci_time[128],int che_ihit[128], int sci_ihit[128],int &type, int &thehit);
int rotate(int n0, int dn);

int main(int argc, char *argv[]){
	double me = 0.511e-3; // GeV
	std::vector<TString> DirName;
	std::vector<int> nRuns;
	std::vector<TString> FileNames;
	std::vector<double> Sizes;
	int Ntotal = 0;
	int Nsingle = 0;
	int Nmultiple = 0;
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
		if (ndirs%3!=0){
//			std::cerr<<"Should provide dir name, n jobs, weight, type"<<std::endl;
			return -1;
		}
		ndirs/=3;
		for (int i = 0; i<ndirs; i++){
			DirName.push_back(argv[3+i]);
			nRuns.push_back((int)strtol(argv[3+i+ndirs],NULL,10));
			Sizes.push_back((double)strtof(argv[3+i+ndirs*2],NULL));
			Ntotal+=Sizes[i];
		}
	}
	else {
		int nfiles = argc-3;
		if (nfiles%2!=0){
//			std::cerr<<"Should provide file name, weight"<<std::endl;
			return -1;
		}
		nfiles/=2;
		for (int i = 0; i<nfiles; i++){
			FileNames.push_back(argv[3+i]);
			Sizes.push_back((double)strtof(argv[3+i+nfiles],NULL));
			Ntotal+=Sizes[i];
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
//		std::cerr<<"FileNames["<<i<<"] = \""<<FileNames[i]<<std::endl;
		c->Add(FileNames[i]);
	}
//	std::cerr<<"nRuns = "<<nRuns.size()<<std::endl;
	for (int iRun = 0; iRun < nRuns.size(); iRun++ ){
//		std::cerr<<"DirName["<<iRun<<"] = \""<<DirName[iRun]<<"\", "<<nRuns[iRun]<<std::endl;
		for (int i = 0; i<nRuns[iRun]; i++){
			c->Add(Form(MyData+"/"+DirName[iRun]+"/%d_job0.raw",i));
		}
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
	TFile * ofile = new TFile("result/"+runName+".root","RECREATE");
	TTree * otree = new TTree("tree","tree");
	int tri_nHits = 0;
	int tri_pos = 0;
	int type = 0;
	int maxlid = 0;
	int maxcontinuedIn = 0;
	int maxcontinuedOut = 0;
	int cdc_nHits = 0;
	int cdc_nHitsT = 0;
	int nturn = 0;
	double theta = 0;
	double zc = 0;
	double zcl = 0;
	double zs = 0;
	double px,py,pz;
	int dir = 0;
	double O_t;
	otree->Branch("tn",&tri_nHits);
	otree->Branch("tp",&tri_pos);
	otree->Branch("type",&type);
	otree->Branch("cn",&cdc_nHits);
	otree->Branch("ctn",&cdc_nHitsT);
	otree->Branch("ccni",&maxcontinuedIn);
	otree->Branch("ccno",&maxcontinuedOut);
	otree->Branch("cml",&maxlid);
	otree->Branch("nt",&nturn);
	otree->Branch("theta",&theta);
	otree->Branch("zc",&zc);
	otree->Branch("zcl",&zcl);
	otree->Branch("zs",&zs);
	otree->Branch("px",&px);
	otree->Branch("py",&py);
	otree->Branch("pz",&pz);
	otree->Branch("dir",&dir);
	otree->Branch("mt",&O_t);
	otree->Branch("evt_num",&evt_num);

	otree->Branch("CdcCell_nHits",&C_nHits);
	otree->Branch("CdcCell_layerID",&C_layerID);
	otree->Branch("CdcCell_cellID",&C_cellID);
	otree->Branch("CdcCell_tid",&C_tid);
	otree->Branch("CdcCell_edep",&C_edep);
	otree->Branch("CdcCell_stepL",&C_stepL);
	otree->Branch("CdcCell_driftD",&C_driftD);
	otree->Branch("CdcCell_driftDtrue",&C_driftDtrue);
	otree->Branch("CdcCell_tstop",&C_tstop);
	otree->Branch("CdcCell_tstart",&C_tstart);
	otree->Branch("CdcCell_posflag",&C_posflag);
	otree->Branch("CdcCell_nPair",&C_nPair);
	otree->Branch("CdcCell_t",&C_t);
	otree->Branch("CdcCell_px",&C_px);
	otree->Branch("CdcCell_py",&C_py);
	otree->Branch("CdcCell_pz",&C_pz);
	otree->Branch("CdcCell_x",&C_x);
	otree->Branch("CdcCell_y",&C_y);
	otree->Branch("CdcCell_z",&C_z);
	otree->Branch("CdcCell_wx",&C_wx);
	otree->Branch("CdcCell_wy",&C_wy);
	otree->Branch("CdcCell_wz",&C_wz);

	otree->Branch("M_nHits",&M_nHits);
	otree->Branch("M_volName",&M_volName);
	otree->Branch("M_volID",&M_volID);
	otree->Branch("M_tid",&M_tid);
	otree->Branch("M_pid",&M_pid);
	otree->Branch("M_edep",&M_edep);
	otree->Branch("M_stepL",&M_stepL);
	otree->Branch("M_t",&M_t);
	otree->Branch("M_px",&M_px);
	otree->Branch("M_py",&M_py);
	otree->Branch("M_pz",&M_pz);
	otree->Branch("M_x",&M_x);
	otree->Branch("M_y",&M_y);
	otree->Branch("M_z",&M_z);
	// Statistics
	double sci_time[128];
	double che_time[128];
	int che_ihit[128];
	int sci_ihit[128];

	// histograms
	TH1D* hTYPE = new TH1D("hTYPE","hTYPE",85,-5,80);
	TH1D* hTriRate = new TH1D("hTriRate","hTriRate",117,0,1170);
	TH1D *hCDCedep = new TH1D("hCDCedep","Energy Deposit of Signal Hits in CDC",200,-10,2);
	hCDCedep->GetYaxis()->SetTitle("Count");
	hCDCedep->GetXaxis()->SetTickLength(0);
	hCDCedep->GetXaxis()->SetTitleOffset(3);
	hCDCedep->GetXaxis()->SetLabelOffset(3);

	//______________________________________________________________________________________________________
	// Loop in events
	Long64_t nEntries = c->GetEntries();
	std::vector<int> vtype;
	std::vector<int> vpos;
	std::vector<double> vtime;
//	std::cout<<nEntries<<" entries to check!"<<std::endl;
	for (Long64_t iEntry = 0; iEntry<nEntries; iEntry++){
//		if (iEntry%1000==0) std::cout<<(double)iEntry/nEntries*100<<"%..."<<std::endl;
		c->GetEntry(iEntry);

		int thimc = -1;
		int thetid = -1;
		for (int imc = 0; imc<McTruth_pz->size(); imc++){
			if ((*McTruth_pid)[imc]==11){
				px = (*McTruth_px)[imc]*1000;
				py = (*McTruth_py)[imc]*1000;
				pz = (*McTruth_pz)[imc]*1000;
				if (sqrt(px*px+py*py+pz*pz)>100){
					thimc = imc;
					thetid = (*McTruth_tid)[imc];
					break;
				}
			}
		}
		if (thimc<0) continue;

		theta = acos((*McTruth_pz)[thimc]/sqrt((*McTruth_px)[thimc]*(*McTruth_px)[thimc]+(*McTruth_py)[thimc]*(*McTruth_py)[thimc]));
		zc = 0;
		zcl = 0;
		zs = 0;

		cdc_nHitsT = C_layerID->size();
		maxlid = 0;
		maxcontinuedIn = 0;
		maxcontinuedOut = 0;
		cdc_nHits = 0;
		nturn = 0;

		int currentlayersIn = 0;
		int currentlayersOut = 0;
		int prelid = 0;
		for ( int ihit = 0; ihit<C_layerID->size(); ihit++){
			if ((*C_tid)[ihit]!=thetid) continue;
			if (((*C_t)[ihit]-(*McTruth_time)[thimc])/7+1>nturn) nturn = ((*C_t)[ihit]-(*McTruth_time)[thimc])/7+1;
			if ((*C_t)[ihit]-(*McTruth_time)[thimc]>7) continue;
			if (maxlid<(*C_layerID)[ihit]) maxlid=(*C_layerID)[ihit];
			if ((*C_layerID)[ihit]-prelid==1) currentlayersIn++;
			else if ((*C_layerID)[ihit]-prelid==-1) currentlayersOut++;
			else if ((*C_layerID)[ihit]!=prelid) {currentlayersIn=0;currentlayersOut=0;}
			if (maxcontinuedOut<currentlayersOut) maxcontinuedOut=currentlayersOut;
			if (maxcontinuedIn<currentlayersIn) maxcontinuedIn=currentlayersIn;
			prelid=(*C_layerID)[ihit];
			cdc_nHits++;
			hCDCedep->Fill(log((*C_edep)[ihit]/1e6));
		}
		//std::cout<<maxlid<<","<<cdc_nHits<<","<<maxcontinuedOut<<","<<maxcontinuedU<<std::endl;

		for (int itri = 0; itri<128; itri++){
			che_time[itri] = -1;
			sci_time[itri] = -1;
		}
		tri_nHits = 0;
		O_t = 0;
		for ( int ihit = 0; ihit<M_t->size(); ihit++){
			if ((*M_tid)[ihit]!=thetid) continue;
			if ((*M_volName)[0]=="TriCheLU"
					||(*M_volName)[0]=="TriCheU"
					||(*M_volName)[0]=="TriSciU"
			   ){
				dir = -1;
			}
			else if ((*M_volName)[0]=="TriSciD"
					||(*M_volName)[0]=="TriCheLD"
					||(*M_volName)[0]=="TriCheD"
					){
				dir = 1;
			}
			else continue;
			if (!O_t)O_t = fmod((*M_t)[ihit],1170);
			tri_nHits++;
			double px = (*M_px)[ihit];
			double py = (*M_py)[ihit];
			double pz = (*M_pz)[ihit];
			double pa = sqrt(px*px+py*py+pz*pz);
			double beta = pa/sqrt(pa*pa+me*me);
			if ((*M_volName)[ihit]=="TriCheD"
				||(*M_volName)[ihit]=="TriCheLD"
					){ // Cherenkov
				if (beta>1/1.5){
					che_time[(*M_volID)[ihit]+64] = (*M_t)[ihit];
					if ((*M_volName)[ihit]=="TriCheD"){
						if (!zc) zc = (*M_z)[ihit];
					}
					else{
						if (!zcl) zcl = (*M_z)[ihit];
					}
				}
			}
			else  if ((*M_volName)[ihit]=="TriCheU"
				||(*M_volName)[ihit]=="TriCheLU"
					){ // Cherenkov
				if (beta>1/1.5){
					che_time[(*M_volID)[ihit]] = (*M_t)[ihit];
					if ((*M_volName)[ihit]=="TriCheU"){
						if (!zc) zc = (*M_z)[ihit];
					}
					else{
						if (!zcl) zcl = (*M_z)[ihit];
					}
				}
			}
			else if ((*M_volName)[ihit]=="TriSciD"
//					||(*M_volName)[ihit]=="TriSciLD"
					){ // Scintillator
				if((*M_edep)[ihit]>630e-6){
					sci_time[(*M_volID)[ihit]+64] = (*M_t)[ihit];
					if (!zs) zs = (*M_z)[ihit];
				}
			}
			else if ((*M_volName)[ihit]=="TriSciU"
//					||(*M_volName)[ihit]=="TriSciLU"
					){ // Scintillator
				if((*M_edep)[ihit]>630e-6){
					sci_time[(*M_volID)[ihit]] = (*M_t)[ihit];
					if (!zs) zs = (*M_z)[ihit];
				}
			}
		}
		if (tri_nHits){
			if (!cdc_nHitsT) tri_nHits*=-1;
			else if ((*C_t)[0]>(*M_t)[0]) tri_nHits*=-1;
		}

		// option D
		int thehit = -1;
		gettype(che_time,sci_time,che_ihit,sci_ihit,type,thehit);
//		std::cout<<"type = "<<type<<std::endl;
		hTYPE->Fill(type);
		if ((type>=53&&type<=56)||(type>=49&&type<=51)||(type>=73&&type<=77)||(type>=69&&type<=72)){
			hTriRate->Fill(O_t);
		}
		// FIXME
		otree->Fill();
		if (nturn==1&&cdc_nHitsT>0&&tri_nHits>0&&maxlid>=4&&((type>=53&&type<=56)||(type>=49&&type<=51)||(type>=73&&type<=77)||(type>=69&&type<=72))) Nsingle++;
		if (nturn>1&&cdc_nHitsT>0&&tri_nHits>0&&maxlid>=4&&((type>=53&&type<=56)||(type>=49&&type<=51)||(type>=73&&type<=77)||(type>=69&&type<=72))) Nmultiple++;
//		if (cdc_nHitsT>0&&tri_nHits>0&&maxlid>=4&&((type>=53&&type<=56)||(type>=49&&type<=51)||(type>=73&&type<=77)||(type>=69&&type<=72)))
//			otree->Fill();
	}
	std::cout<<(double)Nsingle/Ntotal*100<<" "<<(double)Nmultiple/Ntotal*100<<std::endl;
	otree->Write();
	hCDCedep->Write();
	hTYPE->Write();
	hTriRate->Write();
	ofile->Close();
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
