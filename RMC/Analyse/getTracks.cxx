#include <vector>
#include <string>
#include <iostream>
#include <math.h>
#include <stdlib.h>

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"

TString MyData = getenv("MYDATA");
TString MyWork = getenv("MYWORKAREA");
double me = 0.511e-3; // GeV

int rotate(int n0, int dn){
	int n1 = n0+dn;
	if (n0<64&&n1>=64) n1-=64;
	if (n0>=64&&n1<64) n1+=64;
	if (n1>=128) n1-=64;
	if (n1<0) n1+=64;
	return n1;
}

int main(int argc, char** argv){
	TString runName = argv[1];

	TChain * c = new TChain("tree");
	for (int i = 0; i<100; i++){
		c->Add(Form(MyData+"/"+runName+"/%d_job0.raw",i));
	}

	int evt_num;
	int run_num;
	std::vector<int> * in_triID = 0;
	std::vector<int> * in_tritid= 0;
	std::vector<double> * in_trit = 0;
	std::vector<double> * in_triedep = 0;
	std::vector<double> * in_tripx = 0;
	std::vector<double> * in_tripy = 0;
	std::vector<double> * in_tripz = 0;
	std::vector<double> * in_triX = 0;
	std::vector<double> * in_triY = 0;
	std::vector<double> * in_triZ = 0;
	std::vector<std::string> * in_triName= 0;
	std::vector<std::string> * in_mcvol = 0;
	std::vector<std::string> * in_mcprocess = 0;
	std::vector<double> * in_mctime = 0;
	std::vector<double> * in_mcx = 0;
	std::vector<double> * in_mcy = 0;
	std::vector<double> * in_mcz = 0;
	std::vector<int> * in_mctid = 0;
	std::vector<int> * in_cdcpid = 0;
	std::vector<int> * in_cdclid = 0;
	std::vector<int> * in_cdccid = 0;
	std::vector<double> * in_cdct = 0;
	std::vector<double> * in_cdcdd = 0;
	std::vector<double> * in_cdcwx = 0;
	std::vector<double> * in_cdcwy = 0;
	std::vector<double> * in_cdcwz = 0;
	std::vector<double> * in_cdcx = 0;
	std::vector<double> * in_cdcy = 0;
	std::vector<double> * in_cdcz = 0;
	std::vector<double> * in_cdcpx = 0;
	std::vector<double> * in_cdcpy = 0;
	std::vector<double> * in_cdcpz = 0;
	std::vector<double> * in_cdctstop = 0;
	std::vector<double> * in_cdctstart = 0;
	std::vector<double> * in_cdcedep = 0;
	std::vector<int> * in_cdctid = 0;
	std::vector<int> * in_cdcpf = 0;

	c->SetBranchAddress("evt_num",&evt_num);
	c->SetBranchAddress("run_num",&run_num);

	c->SetBranchAddress("CdcCell_cellID",&in_cdccid);
	c->SetBranchAddress("CdcCell_layerID",&in_cdclid);
	c->SetBranchAddress("CdcCell_pid",&in_cdcpid);
	c->SetBranchAddress("CdcCell_t",&in_cdct);
	c->SetBranchAddress("CdcCell_x",&in_cdcx);
	c->SetBranchAddress("CdcCell_y",&in_cdcy);
	c->SetBranchAddress("CdcCell_z",&in_cdcz);
	c->SetBranchAddress("CdcCell_px",&in_cdcpx);
	c->SetBranchAddress("CdcCell_py",&in_cdcpy);
	c->SetBranchAddress("CdcCell_pz",&in_cdcpz);
	c->SetBranchAddress("CdcCell_driftDtrue",&in_cdcdd);
	c->SetBranchAddress("CdcCell_tid",&in_cdctid);
	c->SetBranchAddress("CdcCell_posflag",&in_cdcpf);
	c->SetBranchAddress("CdcCell_edep",&in_cdcedep);
	c->SetBranchAddress("CdcCell_tstop",&in_cdctstop);
	c->SetBranchAddress("CdcCell_tstart",&in_cdctstart);
	c->SetBranchAddress("CdcCell_wx",&in_cdcwx);
	c->SetBranchAddress("CdcCell_wy",&in_cdcwy);
	c->SetBranchAddress("CdcCell_wz",&in_cdcwz);

	c->SetBranchAddress("M_t",&in_trit);
	c->SetBranchAddress("M_edep",&in_triedep);
	c->SetBranchAddress("M_px",&in_tripx);
	c->SetBranchAddress("M_py",&in_tripy);
	c->SetBranchAddress("M_pz",&in_tripz);
	c->SetBranchAddress("M_x",&in_triX);
	c->SetBranchAddress("M_y",&in_triY);
	c->SetBranchAddress("M_z",&in_triZ);
	c->SetBranchAddress("M_tid",&in_tritid);
	c->SetBranchAddress("M_volID",&in_triID);
	c->SetBranchAddress("M_volName",&in_triName);
	c->SetBranchAddress("McTruth_time",&in_mctime);
	c->SetBranchAddress("McTruth_tid",&in_mctid);
	c->SetBranchAddress("McTruth_process",&in_mcprocess);
	c->SetBranchAddress("McTruth_volume",&in_mcvol);
	c->SetBranchAddress("McTruth_x",&in_mcx);
	c->SetBranchAddress("McTruth_y",&in_mcy);
	c->SetBranchAddress("McTruth_z",&in_mcz);

	TFile * ofile = new TFile("CyDet."+runName+".root","RECREATE");
	TTree * otree = new TTree("t","t");
	int tri_nHits;
	int tri_pos;
	int type;
	int maxlid = 0;
	int maxcontinuedIn = 0;
	int maxcontinuedOut = 0;
	int cdc_nHits = 0;
	int cdc_nHitsT = 0;
	int nturn = 0;
	double zc = 0;
	double zcl = 0;
	double zs = 0;
	int dir = 0;
	double o_t,o_x,o_y,o_z,o_px,o_py,o_pz;
	double o_ix,o_iy,o_iz;
	std::string * o_process = 0;
	std::string * o_vol = 0;
	int o_pid = 11;

	otree->Branch("evt_num",&evt_num);
	otree->Branch("run_num",&run_num);

	otree->Branch("tn",&tri_nHits);
	otree->Branch("tp",&tri_pos);
	otree->Branch("type",&type);
	otree->Branch("cn",&cdc_nHits);
	otree->Branch("ctn",&cdc_nHitsT);
	otree->Branch("ccni",&maxcontinuedIn);
	otree->Branch("ccno",&maxcontinuedOut);
	otree->Branch("cml",&maxlid);
	otree->Branch("nt",&nturn);
	otree->Branch("zc",&zc);
	otree->Branch("zcl",&zcl);
	otree->Branch("zs",&zs);
	otree->Branch("dir",&dir);

	otree->Branch("CdcCell_cellID",&in_cdccid);
	otree->Branch("CdcCell_layerID",&in_cdclid);
	otree->Branch("CdcCell_t",&in_cdct);
	otree->Branch("CdcCell_x",&in_cdcx);
	otree->Branch("CdcCell_y",&in_cdcy);
	otree->Branch("CdcCell_z",&in_cdcz);
	otree->Branch("CdcCell_px",&in_cdcpx);
	otree->Branch("CdcCell_py",&in_cdcpy);
	otree->Branch("CdcCell_pz",&in_cdcpz);
	otree->Branch("CdcCell_driftDtrue",&in_cdcdd);
	otree->Branch("CdcCell_tid",&in_cdctid);
	otree->Branch("CdcCell_posflag",&in_cdcpf);
	otree->Branch("CdcCell_edep",&in_cdcedep);
	otree->Branch("CdcCell_tstop",&in_cdctstop);
	otree->Branch("CdcCell_tstart",&in_cdctstart);
	otree->Branch("CdcCell_wx",&in_cdcwx);
	otree->Branch("CdcCell_wy",&in_cdcwy);
	otree->Branch("CdcCell_wz",&in_cdcwz);

	otree->Branch("M_t",&in_trit);
	otree->Branch("M_edep",&in_triedep);
	otree->Branch("M_px",&in_tripx);
	otree->Branch("M_py",&in_tripy);
	otree->Branch("M_pz",&in_tripz);
	otree->Branch("M_x",&in_triX);
	otree->Branch("M_y",&in_triY);
	otree->Branch("M_z",&in_triZ);
	otree->Branch("M_tid",&in_tritid);
	otree->Branch("M_volID",&in_triID);
	otree->Branch("M_volName",&in_triName);

	otree->Branch("pid",&o_pid);
	otree->Branch("t",&o_t);
	otree->Branch("x",&o_x);
	otree->Branch("y",&o_y);
	otree->Branch("z",&o_z);
	otree->Branch("px",&o_px);
	otree->Branch("py",&o_py);
	otree->Branch("pz",&o_pz);

	otree->Branch("volume",&o_vol);
	otree->Branch("process",&o_process);
	otree->Branch("ix",&o_ix);
	otree->Branch("iy",&o_iy);
	otree->Branch("iz",&o_iz);

	double sci_time[128];
	double che_time[128];
	Long64_t nEntries = c->GetEntries();
	std::vector<int> vtype;
	std::vector<int> vpos;
	std::vector<double> vtime;
	std::cout<<nEntries<<" entries to check!"<<std::endl;
	for (Long64_t iEntry = 0; iEntry<nEntries; iEntry++){
		if (iEntry%1000==0) std::cout<<(double)iEntry/nEntries*100<<"%..."<<std::endl;
		c->GetEntry(iEntry);

		int thetid = 0;
		bool found = false;
		int iCdc = 0;
		for (; iCdc<in_cdctid->size(); iCdc++){
			if ((*in_cdcpid)[iCdc]==11){
				o_px = (*in_cdcpx)[iCdc]*1000;
				o_py = (*in_cdcpy)[iCdc]*1000;
				o_pz = (*in_cdcpz)[iCdc]*1000;
				double pa = sqrt((*in_cdcpx)[iCdc]*(*in_cdcpx)[iCdc]+(*in_cdcpy)[iCdc]*(*in_cdcpy)[iCdc]+(*in_cdcpz)[iCdc]*(*in_cdcpz)[iCdc]);
				if (pa>0.1){
					o_x = (*in_cdcx)[iCdc]*10;
					o_y = (*in_cdcy)[iCdc]*10;
					o_z = (*in_cdcz)[iCdc]*10;
					o_t = (*in_cdct)[iCdc]*10;
					thetid = (*in_cdctid)[iCdc];
					found=true;
					break;
				}
			}
		} 
		if (!found) continue;
		int iMc = 0;
		for (;iMc < in_mctime->size(); iMc++){
			if ((*in_mctid)[iMc] == thetid){
				o_ix = (*in_mcx)[iMc]*10;
				o_iy = (*in_mcy)[iMc]*10;
				o_iz = (*in_mcz)[iMc]*10;
				o_process = &((*in_mcprocess)[iMc]);
				o_vol = &((*in_mcvol)[iMc]);
				break;
			}
		}
		int iTri = 0;
		for (;iTri < in_tritid->size(); iTri++){
			if ((*in_tritid)[iTri] == thetid) break;
		}

		zc = 0;
		zcl = 0;
		zs = 0;

		cdc_nHitsT = in_cdclid->size();
		maxlid = 0;
		maxcontinuedIn = 0;
		maxcontinuedOut = 0;
		cdc_nHits = 0;
		nturn = 0;

		int currentlayersIn = 0;
		int currentlayersOut = 0;
		int prelid = 0;
		for ( int ihit = 0; ihit<in_cdclid->size(); ihit++){
			if (((*in_cdct)[ihit]-(*in_mctime)[iMc])/7+1>nturn) nturn = ((*in_cdct)[ihit]-(*in_mctime)[iMc])/7+1;
			if ((*in_cdct)[ihit]-(*in_mctime)[iMc]>7) continue;
			if (maxlid<(*in_cdclid)[ihit]) maxlid=(*in_cdclid)[ihit];
			if ((*in_cdclid)[ihit]-prelid==1) currentlayersIn++;
			else if ((*in_cdclid)[ihit]-prelid==-1) currentlayersOut++;
			else if ((*in_cdclid)[ihit]!=prelid) {currentlayersIn=0;currentlayersOut=0;}
			if (maxcontinuedOut<currentlayersOut) maxcontinuedOut=currentlayersOut;
			if (maxcontinuedIn<currentlayersIn) maxcontinuedIn=currentlayersIn;
			prelid=(*in_cdclid)[ihit];
			cdc_nHits++;
		}

		for (int itri = 0; itri<128; itri++){
			che_time[itri] = -1;
			sci_time[itri] = -1;
		}
		tri_nHits = 0;
		if (in_triName->size()){
			if ((*in_triName)[iTri]=="TriCheLU"
					||(*in_triName)[iTri]=="TriCheLD"
					||(*in_triName)[iTri]=="TriCheU"
					||(*in_triName)[iTri]=="TriCheD"
			   ){
				dir = 1;
			}
			else if ((*in_triName)[iTri]=="TriSciU"
					||(*in_triName)[iTri]=="TriSciD"
					){
				dir = -1;
			}
		}
		for ( int ihit = 0; ihit<in_trit->size(); ihit++){
			if ((*in_triName)[ihit]=="TriChePD"
			  ||(*in_triName)[ihit]=="TriChePU"
			  ||(*in_triName)[ihit]=="TriSciPD"
			  ||(*in_triName)[ihit]=="TriSciPU"
			  ||(*in_triName)[ihit]=="TriSciLD"
			  ||(*in_triName)[ihit]=="TriSciLU"
					) continue;
			tri_nHits++;
			double px = (*in_tripx)[ihit];
			double py = (*in_tripy)[ihit];
			double pz = (*in_tripz)[ihit];
			double pa = sqrt(px*px+py*py+pz*pz);
			double beta = pa/sqrt(pa*pa+me*me);
			if ((*in_triName)[ihit]=="TriCheD"
				||(*in_triName)[ihit]=="TriCheLD"
					){ // Cherenkov
				if (beta>1/1.5){
//				if (1){
					che_time[(*in_triID)[ihit]+64] = (*in_trit)[ihit];
					if ((*in_triName)[ihit]=="TriCheD"){
						if (!zc) zc = (*in_triZ)[ihit];
					}
					else{
						if (!zcl) zcl = (*in_triZ)[ihit];
					}
				}
			}
			else  if ((*in_triName)[ihit]=="TriCheU"
				||(*in_triName)[ihit]=="TriCheLU"
					){ // Cherenkov
				if (beta>1/1.5){
//				if (1){
					che_time[(*in_triID)[ihit]] = (*in_trit)[ihit];
					if ((*in_triName)[ihit]=="TriCheU"){
						if (!zc) zc = (*in_triZ)[ihit];
					}
					else{
						if (!zcl) zcl = (*in_triZ)[ihit];
					}
				}
			}
			else if ((*in_triName)[ihit]=="TriSciD"
//					||(*in_triName)[ihit]=="TriSciLD"
					){ // Scintillator
				if((*in_triedep)[ihit]>630e-6){
//				if(1){
					sci_time[(*in_triID)[ihit]+64] = (*in_trit)[ihit];
					if (!zs) zs = (*in_triZ)[ihit];
				}
			}
			else if ((*in_triName)[ihit]=="TriSciU"
//					||(*in_triName)[ihit]=="TriSciLU"
					){ // Scintillator
				if((*in_triedep)[ihit]>630e-6){
//				if(1){
					sci_time[(*in_triID)[ihit]] = (*in_trit)[ihit];
					if (!zs) zs = (*in_triZ)[ihit];
				}
			}
		}
		if (tri_nHits){
			if (!cdc_nHitsT) tri_nHits*=-1;
			else if ((*in_cdct)[iCdc]>(*in_trit)[iTri]) tri_nHits*=-1;
		}

		// option D
		vtype.clear();
		vtime.clear();
		vpos.clear();
		for (int itri = 0; itri<128; itri++){
			int time = sci_time[itri];
			if (time==-1)
			continue;
			int nsci = 1;
			// how many hit in sci in row? (starting from this counter)
			for (int delta = 1; delta<=2; delta++){
				int jtri = rotate(itri,delta);
				if (sci_time[jtri]!=-1&&fabs(sci_time[jtri]-time)<10) nsci++;
				else break;
			}
			for (int delta = -2; delta<=2; delta++){
				int jtri = rotate(itri,delta);
				if (che_time[jtri]==-1||fabs(che_time[jtri]-time)>=10) continue;
				int nche = 1;
				for (int delta = 1; delta<=2; delta++){
					int ktri = rotate(jtri,delta);
					if (che_time[ktri]!=-1&&fabs(che_time[ktri]-time)<10) nche++;
					else break;
				}
				vtype.push_back(nsci*100+nche*10+delta);
				vtime.push_back(time);
				vpos.push_back(itri>=64?1:-1);
			}
		}
		int nhitmax = -1;
		for (int it = 0; it<vtype.size(); it++){
			if (nhitmax<vtype[it]/10) nhitmax = vtype[it]/10;
		}
		int ndeltamin = 10;
		for (int it = 0; it<vtype.size(); it++){
			if (nhitmax!=vtype[it]/10) continue;
			if (fabs(ndeltamin)>fabs(vtype[it]%10)){
				ndeltamin = vtype[it]%10;
				tri_pos = vpos[it];
			}
		}
		type = nhitmax*10+ndeltamin;
		if (cdc_nHits>=30&&maxlid>=4&&tri_nHits>0&&((type>=228&&type<=231)||(type>=219&&type<=221)||(type>=328&&type<=332)||(type>=319&&type<=322)))
			otree->Fill();
	}
	otree->Write();
	ofile->Close();
	return 0;
}
