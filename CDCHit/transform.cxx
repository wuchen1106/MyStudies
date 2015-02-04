#include <vector>
#include <sstream>
#include <iostream>
#include <math.h>
#include <malloc.h>

#include "TRandom.h"
#include "TChain.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1D.h"

#define use_extra

int main(int argc, char** argv){

	// for general
	double tsep = 1170;

	// for bkg
	std::vector<std::string> v_filename;
	std::vector<double> v_effi;
	std::vector<int> v_nBKG;
	std::vector<int> v_nBKGneed;
	v_filename.push_back("/home/chen/MyWorkArea/MyStudies/hitrate/result/proton.mum.140905M02.root");
	v_effi.push_back(1/2.85);
//	v_filename.push_back("/home/chen/MyWorkArea/MyStudies/hitrate/result/proton.pim.140905M02.root");
//	v_effi.push_back(1);
	v_filename.push_back("/home/chen/MyWorkArea/MyStudies/hitrate/result/noise.OT.140905M02.root");
	v_effi.push_back(1);
	v_filename.push_back("/home/chen/MyWorkArea/MyStudies/hitrate/result/noise.mumneutron.140905M02.root");
	v_effi.push_back(1.6);
	int index = 0;

	int NMN_nHits_Max = 100;
	int * NMN_nHits = (int*)malloc(sizeof(int)*3*1300);
	double * NMN_t = (double*)malloc(sizeof(double)*3*1300*NMN_nHits_Max);
	double * NMN_driftD = (double*)malloc(sizeof(double)*3*1300*NMN_nHits_Max);
	double * NMN_driftDtrue = (double*)malloc(sizeof(double)*3*1300*NMN_nHits_Max);
	double * NMN_tstart = (double*)malloc(sizeof(double)*3*1300*NMN_nHits_Max);
	double * NMN_tstop = (double*)malloc(sizeof(double)*3*1300*NMN_nHits_Max);
	double * NMN_edep = (double*)malloc(sizeof(double)*3*1300*NMN_nHits_Max);
	int * NMN_cellID = (int*)malloc(sizeof(int)*3*1300*NMN_nHits_Max);
	int * NMN_layerID = (int*)malloc(sizeof(int)*3*1300*NMN_nHits_Max);
	int * NMN_posflag = (int*)malloc(sizeof(int)*3*1300*NMN_nHits_Max);
	double * NMN_wx = (double*)malloc(sizeof(double)*3*1300*NMN_nHits_Max);
	double * NMN_wy = (double*)malloc(sizeof(double)*3*1300*NMN_nHits_Max);
	double * NMN_wz = (double*)malloc(sizeof(double)*3*1300*NMN_nHits_Max);
	double * NMN_x = (double*)malloc(sizeof(double)*3*1300*NMN_nHits_Max);
	double * NMN_y = (double*)malloc(sizeof(double)*3*1300*NMN_nHits_Max);
	double * NMN_z = (double*)malloc(sizeof(double)*3*1300*NMN_nHits_Max);
	double * NMN_px = (double*)malloc(sizeof(double)*3*1300*NMN_nHits_Max);
	double * NMN_py = (double*)malloc(sizeof(double)*3*1300*NMN_nHits_Max);
	double * NMN_pz = (double*)malloc(sizeof(double)*3*1300*NMN_nHits_Max);

	int BKG_nHits = 0;
	std::vector<double> * BKG_t = 0;
	std::vector<double> * BKG_driftD = 0;
	std::vector<double> * BKG_driftDtrue = 0;
	std::vector<double> * BKG_tstart = 0;
	std::vector<double> * BKG_tstop = 0;
	std::vector<double> * BKG_edep = 0;
	std::vector<int> * BKG_cellID = 0;
	std::vector<int> * BKG_layerID = 0;
	std::vector<int> * BKG_posflag = 0;
	std::vector<double> * BKG_wx = 0;
	std::vector<double> * BKG_wy = 0;
	std::vector<double> * BKG_wz = 0;
	std::vector<double> * BKG_x = 0;
	std::vector<double> * BKG_y = 0;
	std::vector<double> * BKG_z = 0;
	std::vector<double> * BKG_px = 0;
	std::vector<double> * BKG_py = 0;
	std::vector<double> * BKG_pz = 0;
	double scalefactor = 1/1.e8*2.5e12*tsep*1e-9*2.4;
	for (int i = 0;i<v_filename.size(); i++){
		TChain * c_BKG = new TChain("tree");
		c_BKG->Add(v_filename[i].c_str());
		v_nBKG.push_back(c_BKG->GetEntries());
		v_nBKGneed.push_back(c_BKG->GetEntries()*v_effi[i]*scalefactor+1);
		std::cout<<"BKG["<<i<<"]: "<<c_BKG->GetEntries()*v_effi[i]*scalefactor+1<<" tracks"<<std::endl;
		c_BKG->SetBranchAddress("nHits",&BKG_nHits);
		c_BKG->SetBranchAddress("O_t",&BKG_t);
		c_BKG->SetBranchAddress("O_driftD",&BKG_driftD);
		c_BKG->SetBranchAddress("O_driftDtrue",&BKG_driftDtrue);
		c_BKG->SetBranchAddress("O_tstart",&BKG_tstart);
		c_BKG->SetBranchAddress("O_tstop",&BKG_tstop);
		c_BKG->SetBranchAddress("O_edep",&BKG_edep);
		c_BKG->SetBranchAddress("O_cellID",&BKG_cellID);
		c_BKG->SetBranchAddress("O_layerID",&BKG_layerID);
		c_BKG->SetBranchAddress("O_posflag",&BKG_posflag);
		c_BKG->SetBranchAddress("O_wx",&BKG_wx);
		c_BKG->SetBranchAddress("O_wy",&BKG_wy);
		c_BKG->SetBranchAddress("O_wz",&BKG_wz);
		c_BKG->SetBranchAddress("O_x",&BKG_x);
		c_BKG->SetBranchAddress("O_y",&BKG_y);
		c_BKG->SetBranchAddress("O_z",&BKG_z);
		c_BKG->SetBranchAddress("O_px",&BKG_px);
		c_BKG->SetBranchAddress("O_py",&BKG_py);
		c_BKG->SetBranchAddress("O_pz",&BKG_pz);
		for ( int j = 0; j<c_BKG->GetEntries(); j++){
			c_BKG->GetEntry(j);
			NMN_nHits[i*1300+j] = BKG_nHits;
			for (int k = 0; k<BKG_nHits; k++){
				NMN_t[i*1300*NMN_nHits_Max+j*NMN_nHits_Max+k] = (*BKG_t)[k];
				NMN_driftD[i*1300*NMN_nHits_Max+j*NMN_nHits_Max+k] = (*BKG_driftD)[k];
				NMN_driftDtrue[i*1300*NMN_nHits_Max+j*NMN_nHits_Max+k] = (*BKG_driftDtrue)[k];
				NMN_tstart[i*1300*NMN_nHits_Max+j*NMN_nHits_Max+k] = (*BKG_tstart)[k];
				NMN_tstop[i*1300*NMN_nHits_Max+j*NMN_nHits_Max+k] = (*BKG_tstop)[k];
				NMN_edep[i*1300*NMN_nHits_Max+j*NMN_nHits_Max+k] = (*BKG_edep)[k];
				NMN_cellID[i*1300*NMN_nHits_Max+j*NMN_nHits_Max+k] = (*BKG_cellID)[k];
				NMN_layerID[i*1300*NMN_nHits_Max+j*NMN_nHits_Max+k] = (*BKG_layerID)[k];
				NMN_posflag[i*1300*NMN_nHits_Max+j*NMN_nHits_Max+k] = (*BKG_posflag)[k];
				NMN_wx[i*1300*NMN_nHits_Max+j*NMN_nHits_Max+k] = (*BKG_wx)[k];
				NMN_wy[i*1300*NMN_nHits_Max+j*NMN_nHits_Max+k] = (*BKG_wy)[k];
				NMN_wz[i*1300*NMN_nHits_Max+j*NMN_nHits_Max+k] = (*BKG_wz)[k];
				NMN_x[i*1300*NMN_nHits_Max+j*NMN_nHits_Max+k] = (*BKG_x)[k];
				NMN_y[i*1300*NMN_nHits_Max+j*NMN_nHits_Max+k] = (*BKG_y)[k];
				NMN_z[i*1300*NMN_nHits_Max+j*NMN_nHits_Max+k] = (*BKG_z)[k];
				NMN_px[i*1300*NMN_nHits_Max+j*NMN_nHits_Max+k] = (*BKG_px)[k];
				NMN_py[i*1300*NMN_nHits_Max+j*NMN_nHits_Max+k] = (*BKG_py)[k];
				NMN_pz[i*1300*NMN_nHits_Max+j*NMN_nHits_Max+k] = (*BKG_pz)[k];
			}
		}
	}

	// for input
	TH1D * h_xt = (TH1D*)(new TFile("/home/chen/MyWorkArea/Simulate/comet/data/xt.root"))->Get("xt");

//	TFile * ifile = new TFile("/home/chen/MyWorkArea/Simulate/comet/output/signal.140905M02.root");
//	TFile * ifile = new TFile("/home/chen/MyWorkArea/Simulate/comet/output/raw_g4sim.root");
//	TFile * ifile = new TFile("/home/chen/MyWorkArea/Simulate/comet/output/signal.electron.150um.G41001.withwire.AllDisks.root");
	TFile * ifile = new TFile("/home/chen/MyWorkArea/Simulate/comet/output/signal.electron.wirehits.130927.root");
	TTree * it = (TTree*) ifile->Get("tree");

	int CdcCell_nHits = 0;
	std::vector<double> * CdcCell_t = 0;
	std::vector<double> * CdcCell_wx = 0;
	std::vector<double> * CdcCell_wy = 0;
	std::vector<double> * CdcCell_wz = 0;
	std::vector<double> * CdcCell_x = 0;
	std::vector<double> * CdcCell_y = 0;
	std::vector<double> * CdcCell_z = 0;
	std::vector<double> * CdcCell_px = 0;
	std::vector<double> * CdcCell_py = 0;
	std::vector<double> * CdcCell_pz = 0;
	std::vector<double> * CdcCell_driftD = 0;
	std::vector<double> * CdcCell_driftDtrue = 0;
	std::vector<double> * CdcCell_tstart = 0;
	std::vector<double> * CdcCell_tstop = 0;
	std::vector<int> * CdcCell_cellID = 0;
	std::vector<int> * CdcCell_layerID = 0;
	std::vector<double> * CdcCell_edep = 0;
	std::vector<int> * CdcCell_tid = 0;
	std::vector<int> * CdcCell_posflag = 0;

	int M_nHits = 0;
	std::vector<int> * M_tid = 0;
	std::vector<double> * M_x = 0;
	std::vector<double> * M_y = 0;
	std::vector<double> * M_z = 0;
	std::vector<double> * M_t = 0;
	int evt_num;
	int run_num;

	it->SetBranchAddress("evt_num",&evt_num);
	it->SetBranchAddress("run_num",&run_num);
	it->SetBranchAddress("CdcCell_nHits",&CdcCell_nHits);
	it->SetBranchAddress("CdcCell_t",&CdcCell_t);
	it->SetBranchAddress("CdcCell_wx",&CdcCell_wx);
	it->SetBranchAddress("CdcCell_wy",&CdcCell_wy);
	it->SetBranchAddress("CdcCell_wz",&CdcCell_wz);
	it->SetBranchAddress("CdcCell_x",&CdcCell_x);
	it->SetBranchAddress("CdcCell_y",&CdcCell_y);
	it->SetBranchAddress("CdcCell_z",&CdcCell_z);
	it->SetBranchAddress("CdcCell_px",&CdcCell_px);
	it->SetBranchAddress("CdcCell_py",&CdcCell_py);
	it->SetBranchAddress("CdcCell_pz",&CdcCell_pz);
	it->SetBranchAddress("CdcCell_driftD",&CdcCell_driftD);
	it->SetBranchAddress("CdcCell_driftDtrue",&CdcCell_driftDtrue);
	it->SetBranchAddress("CdcCell_tstart",&CdcCell_tstart);
	it->SetBranchAddress("CdcCell_tstop",&CdcCell_tstop);
	it->SetBranchAddress("CdcCell_cellID",&CdcCell_cellID);
	it->SetBranchAddress("CdcCell_layerID",&CdcCell_layerID);
	it->SetBranchAddress("CdcCell_edep",&CdcCell_edep);
	it->SetBranchAddress("CdcCell_tid",&CdcCell_tid);
	it->SetBranchAddress("CdcCell_posflag",&CdcCell_posflag);

	it->SetBranchAddress("M_nHits",&M_nHits);
	it->SetBranchAddress("M_tid",&M_tid);
	it->SetBranchAddress("M_x",&M_x);
	it->SetBranchAddress("M_y",&M_y);
	it->SetBranchAddress("M_z",&M_z);
	it->SetBranchAddress("M_t",&M_t);

	# ifdef use_extra
	int wire_nHits = 0;
	int T_nHits = 0;
	it->SetBranchAddress("T_nHits",&T_nHits);
	it->SetBranchAddress("wire_nHits",&wire_nHits);
	# endif

	// for output
	double O_mx = 0;
	double O_my = 0;
	double O_mz = 0;
	double O_mt = 0;
	int O_nHits = 0;
	std::vector<int> * O_hittype = 0;
	std::vector<double> * O_t = 0;
	std::vector<double> * O_tof = 0;
	std::vector<double> * O_wx = 0;
	std::vector<double> * O_wy = 0;
	std::vector<double> * O_wz = 0;
	std::vector<double> * O_x = 0;
	std::vector<double> * O_y = 0;
	std::vector<double> * O_z = 0;
	std::vector<double> * O_px = 0;
	std::vector<double> * O_py = 0;
	std::vector<double> * O_pz = 0;
	std::vector<double> * O_driftD = 0;
	std::vector<double> * O_driftDtrue = 0;
	std::vector<double> * O_tstop = 0;
	std::vector<double> * O_tstart = 0;
	std::vector<int> * O_cellID = 0;
	std::vector<int> * O_layerID = 0;
	std::vector<double> * O_edep = 0;
	std::vector<int> * O_tid = 0;
	std::vector<int> * O_posflag = 0;

	TFile * of = new TFile("output.root","RECREATE");
	TTree * ot = new TTree("tree","tree");

	ot->Branch("evt_num",&evt_num);
	ot->Branch("run_num",&run_num);
	ot->Branch("CdcCell_nHits",&O_nHits);
	ot->Branch("CdcCell_t",&O_t);
	ot->Branch("CdcCell_wx",&O_wx);
	ot->Branch("CdcCell_wy",&O_wy);
	ot->Branch("CdcCell_wz",&O_wz);
	ot->Branch("CdcCell_x",&O_x);
	ot->Branch("CdcCell_y",&O_y);
	ot->Branch("CdcCell_z",&O_z);
	ot->Branch("CdcCell_px",&O_px);
	ot->Branch("CdcCell_py",&O_py);
	ot->Branch("CdcCell_pz",&O_pz);
	ot->Branch("CdcCell_driftD",&O_driftD);
	ot->Branch("CdcCell_driftDtrue",&O_driftDtrue);
	ot->Branch("CdcCell_tof",&O_tof);
	ot->Branch("CdcCell_tstart",&O_tstart);
	ot->Branch("CdcCell_tstop",&O_tstop);
	ot->Branch("CdcCell_cellID",&O_cellID);
	ot->Branch("CdcCell_layerID",&O_layerID);
	ot->Branch("CdcCell_edep",&O_edep);
	ot->Branch("CdcCell_posflag",&O_posflag);
	ot->Branch("CdcCell_hittype",&O_hittype);

	ot->Branch("CdcCell_mx",&O_mx);
	ot->Branch("CdcCell_my",&O_my);
	ot->Branch("CdcCell_mz",&O_mz);
	ot->Branch("CdcCell_mt",&O_mt);

	# ifdef use_extra
	int O_T_nHits = 0;
	int O_wire_nHits = 0;
	ot->Branch("T_nHits",&O_T_nHits);
	ot->Branch("wire_nHits",&O_wire_nHits);
	# endif

	int dict[18][350];
	int dict2[200];

	bool triggerd = false;
	double firsthittime = 0;
	int nGoodHit = 0;
	for ( int i = 0; i<it->GetEntries(); i++){
//	for ( int i = 0; i<2e4; i++){
		for(int j = 0; j<18; j++){
			for (int k = 0; k<350; k++){
				dict[j][k]=-1;
			}
		}
		if (i%1==0) printf("%lf%...\n",(double)i/it->GetEntries()*100);
		it->GetEntry(i);
		# ifdef use_extra
		O_T_nHits = T_nHits-1;
		O_wire_nHits = wire_nHits;
		# endif
		if (CdcCell_nHits==0) continue;
		O_nHits = 0;
		O_hittype = new std::vector<int>;
		O_t = new std::vector<double>;
		O_tof = new std::vector<double>;
		O_wx = new std::vector<double>;
		O_wy = new std::vector<double>;
		O_wz = new std::vector<double>;
		O_x = new std::vector<double>;
		O_y = new std::vector<double>;
		O_z = new std::vector<double>;
		O_px = new std::vector<double>;
		O_py = new std::vector<double>;
		O_pz = new std::vector<double>;
		O_driftD = new std::vector<double>;
		O_driftDtrue = new std::vector<double>;
		O_tstart = new std::vector<double>;
		O_tstop = new std::vector<double>;
		O_cellID = new std::vector<int>;
		O_layerID = new std::vector<int>;
		O_edep = new std::vector<double>;
		O_tid = new std::vector<int>;
		O_posflag = new std::vector<int>;

		triggerd = false;
		firsthittime = 0;
		nGoodHit = 0;
		for (int j = 0; j<CdcCell_nHits; j++){
			if ((*CdcCell_tid)[j]==1){
				nGoodHit++;
				if (!firsthittime){
					firsthittime = (*CdcCell_t)[j];
				}
			}
		}
		if (nGoodHit<1) continue;
		for (int j = 0; j<M_nHits; j++){
			if ((*M_tid)[j]==1){
				if ((*M_t)[j]>firsthittime){
					triggerd = true;
					O_mt = (*M_t)[j];
					O_mx = (*M_x)[j];
					O_my = (*M_y)[j];
					O_mz = (*M_z)[j];
					break;
				}
			}
		}
		//FIXME
		if (!triggerd) continue;

		double shifttime = 0;
		shifttime = fmod(O_mt,tsep)-O_mt;
		O_mt += shifttime;

		// sort according to tof
		std::vector<int> hitindice;
		hitindice.resize(CdcCell_nHits);
		for (int j = 0; j<CdcCell_nHits; j++){
			hitindice[j] = j;
		}
		int temp;
		for (int j = 0; j<CdcCell_nHits; j++){
			for (int k = j+1; k<CdcCell_nHits; k++){
				if ((*CdcCell_t)[hitindice[j]]>(*CdcCell_t)[hitindice[k]]){
					temp = hitindice[j];
					hitindice[j] = hitindice[k];
					hitindice[k]=temp;
				}
			}
		}

		double hittime;
		double tof;
		double starttime;
		double stoptime;
		int hittype;
		for (int j = 0; j<CdcCell_nHits; j++){
			if ((*CdcCell_tid)[hitindice[j]]==1){
				hittype = 0;
			}
			else{
				hittype = 1;
			}
			starttime = (*CdcCell_tstart)[hitindice[j]] + shifttime;
			stoptime = (*CdcCell_tstop)[hitindice[j]] + shifttime;
			hittime = (*CdcCell_t)[hitindice[j]] + shifttime;
			tof = (*CdcCell_t)[hitindice[j]];
			if (dict[(*CdcCell_layerID)[hitindice[j]]][(*CdcCell_cellID)[hitindice[j]]]==-1){
				//FIXME
				double px = (*CdcCell_px)[hitindice[j]];
				double py = (*CdcCell_py)[hitindice[j]];
				double pz = (*CdcCell_pz)[hitindice[j]];
				double pa = sqrt(px*px+py*py+pz*pz);
				//if (tof>7||pa<0.103||hittype!=0) continue; // only first turn
				if (hittype!=0) continue; // only hits from the signal track

				dict[(*CdcCell_layerID)[hitindice[j]]][(*CdcCell_cellID)[hitindice[j]]]=O_nHits;
				O_nHits++;
				O_t->push_back(hittime);
				O_tof->push_back(tof);
				O_tstart->push_back(starttime);
				O_tstop->push_back(stoptime);
				O_wx->push_back((*CdcCell_wx)[hitindice[j]]);
				O_wy->push_back((*CdcCell_wy)[hitindice[j]]);
				O_wz->push_back((*CdcCell_wz)[hitindice[j]]);
				O_x->push_back((*CdcCell_x)[hitindice[j]]);
				O_y->push_back((*CdcCell_y)[hitindice[j]]);
				O_z->push_back((*CdcCell_z)[hitindice[j]]);
				O_px->push_back((*CdcCell_px)[hitindice[j]]);
				O_py->push_back((*CdcCell_py)[hitindice[j]]);
				O_pz->push_back((*CdcCell_pz)[hitindice[j]]);
				// FIXME: we can add smear
				O_driftD->push_back((*CdcCell_driftD)[hitindice[j]]+gRandom->Gaus(0,0.02)); // no need to monify, by now...
				O_driftDtrue->push_back((*CdcCell_driftDtrue)[hitindice[j]]);
				O_cellID->push_back((*CdcCell_cellID)[hitindice[j]]);
				O_layerID->push_back((*CdcCell_layerID)[hitindice[j]]);
				O_edep->push_back((*CdcCell_edep)[hitindice[j]]);
				O_posflag->push_back((*CdcCell_posflag)[hitindice[j]]);
				O_hittype->push_back(hittype);
			}
			else{
				std::cout<<"WTH?"<<std::endl;
			}
		}
		//FIXME
		if (O_nHits<=0) continue;

		// mixture bkg
		int total_count = 0;
		//FIXME
//		for(int ibkg = 0; ibkg < v_nBKGneed.size(); ibkg++){
		for(int ibkg = 0; ibkg < 0; ibkg++){
			int nbkg = v_nBKGneed[ibkg];
			int nBKG = v_nBKG[ibkg];
			std::cout<<"  =>Now mix in "<<nbkg<<" noise tracks from \""<<v_filename[ibkg]<<"\""<<std::endl;
			int count_bkg = 0;
			for (int idict = 0; idict<200; idict++){
				dict2[idict] = -1;
			}
			while(count_bkg<nbkg){
				int index = 0;
				while(1){
					index = (int)(gRandom->Uniform()*nBKG)%nBKG;
					std::cout<<"          # dict2["<<index<<"] = "<<dict2[index]<<std::endl;
					if (dict2[index]==-1){
						dict2[index] = 1;
						std::cout<<"          Yes! "<<std::endl;
						break;
					}
				}
				count_bkg++;
				int this_count = 0;
				int evt_index = ibkg*1300+index;
				std::cout<<"     + "<<count_bkg<<": Got "<<NMN_nHits[evt_index]<<" hits @ "<<index<<std::endl;
				//			index = index+ibkg;
				//			int nindex = index%nBKG;
				for (int ibkghit = 0; ibkghit<NMN_nHits[evt_index]; ibkghit++){
					// get time
					int hit_index = (ibkg*1300+index)*NMN_nHits_Max+ibkghit;
					int ihit = dict[(NMN_layerID[hit_index])][(NMN_cellID[hit_index])];
					double dt = fmod(NMN_tstart[hit_index]-O_mt,tsep);
					if (dt<0) dt+=tsep;
					starttime = O_mt+dt;
					hittime = dt+O_mt+NMN_t[hit_index]-NMN_tstart[hit_index];
					stoptime = dt+O_mt+NMN_tstop[hit_index]-NMN_tstart[hit_index];
					// FIXME don't know how to consider coming-earlier case...
					//if (ihit!=-1&&stoptime>(*O_tstart)[ihit]-200){
					//	if ((*O_hittype)[ihit]==0) (*O_hittype)[ihit] = -1; // if this overlapped hit is from signal, then mark as -1
					//	(*O_edep)[ihit] = (*O_edep)[ihit]+NMN_edep[hit_index];
					//	(*O_driftD)[ihit] = 0;
					//}
					// now dt > 0
					double dd = 0;
					int ibin = 0;
					for (ibin = 0; ibin<h_xt->GetNbinsX(); ibin++){
						if (dt<h_xt->GetBinContent(ibin)){
							dd=h_xt->GetBinCenter(ibin);
							break;
						}
					}
					if (ibin==h_xt->GetNbinsX()){
						continue; // too far away from trigger time, neglect
					}
					// ----- new?
					if (ihit==-1){
						O_nHits++;
						total_count++;
						this_count++;
						dict[(NMN_layerID[hit_index])][(NMN_cellID[hit_index])]=O_nHits;
						O_t->push_back(hittime);
						O_tstart->push_back(starttime);
						O_tstop->push_back(stoptime);
						O_tof->push_back(O_mt-hittime);
						O_wx->push_back(NMN_wx[hit_index]/10);
						O_wy->push_back(NMN_wy[hit_index]/10);
						O_wz->push_back(NMN_wz[hit_index]/10);
						O_x->push_back(NMN_x[hit_index]/10);
						O_y->push_back(NMN_y[hit_index]/10);
						O_z->push_back(NMN_z[hit_index]/10);
						O_px->push_back(NMN_px[hit_index]/1000);
						O_py->push_back(NMN_py[hit_index]/1000);
						O_pz->push_back(NMN_pz[hit_index]/1000);
						O_driftD->push_back(dd);
						O_driftDtrue->push_back(NMN_driftDtrue[hit_index]);
						O_cellID->push_back(NMN_cellID[hit_index]);
						O_layerID->push_back(NMN_layerID[hit_index]);
						O_edep->push_back(NMN_edep[hit_index]);
						O_posflag->push_back(NMN_posflag[hit_index]);
						O_hittype->push_back(2); // pure noise
						continue;
					}
					else{
						// ----- update?
						if (starttime<(*O_tstart)[ihit]){ // update!
							(*O_tstart)[ihit] = starttime;
							(*O_t)[ihit] = hittime;
							(*O_tof)[ihit] = O_mt-hittime;
							(*O_driftD)[ihit] = dd;
							(*O_edep)[ihit] = (*O_edep)[ihit] + NMN_edep[hit_index];
							(*O_hittype)[ihit] = 3;
						}
					}
				}// end of hits in bkg event
				std::cout<<"        Finished! Actually got "<<this_count<<" hits; Totallly we have "<<total_count<<" noise hits mixed inside"<<std::endl;
			}// end of events loop in bkg
		}// end of bkg loop
		std::cout<<"Filling ot @"<<(void*)ot<<"..."<<std::endl;
		ot->Fill();
		std::cout<<"Filled! ot->GetEntries() = "<<ot->GetEntries()<<std::endl;
	}// end of event loop
	ot->Write();
	of->Close();
	return 0;
}
