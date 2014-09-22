#include <vector>
#include <sstream>
#include <iostream>
#include <math.h>

#include "TRandom.h"
#include "TChain.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1D.h"

int main(int argc, char** argv){
	int dict[18][350];

	double tsep = 1170;

	TH1D * h_xt = (TH1D*)(new TFile("/home/chen/MyWorkArea/Simulate/comet/data/xt.root"))->Get("xt");
	TH1D * h_stoptime = (TH1D*)(new TFile("/home/chen/MyWorkArea/Simulate/comet/data/ST.mum.g60cm10mm.005T.BL.g4s.QBH.root"))->Get("h10");

//	TFile * ifile = new TFile("/home/chen/MyWorkArea/Simulate/comet/output/signal.geantino.root");
	TFile * ifile = new TFile("/home/chen/MyWorkArea/Simulate/comet/output/raw_g4sim.root");
	TTree * it = (TTree*) ifile->Get("tree");
	TFile * of = new TFile("output.root","RECREATE");
	TTree * ot = new TTree("tree","tree");

	// for input
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

	// for bkg
	TChain * c_BKG = new TChain("tree");
	c_BKG->Add("/home/chen/MyWorkArea/MyStudies/hitrate/result/ALL.cdc.root");
//	c_BKG->Add("/home/chen/MyWorkArea/MyStudies/hitrate/result/n0.root");
	int nBKG = c_BKG->GetEntries();
	// FIXME
	nBKG = 0;
	int index = 0;
	int BKG_nHits = 0;
	int type = 0;
	int topo = 0;
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
	c_BKG->SetBranchAddress("type",&type);
	c_BKG->SetBranchAddress("nHits",&BKG_nHits);
	c_BKG->SetBranchAddress("topo",&topo);
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

	int nbkg = nBKG/1.e9*2.5e12*tsep*1e-9*2.4;
	bool triggerd = false;
	double firsthittime = 0;
	double decaytime = 0;
	int nGoodHit = 0;
	for ( int i = 0; i<it->GetEntries(); i++){
		for(int j = 0; j<18; j++){
			for (int k = 0; k<350; k++){
				dict[j][k]=-1;
			}
		}
		if (i%1==0) printf("%lf%...\n",(double)i/it->GetEntries()*100);
		it->GetEntry(i);
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

		decaytime = h_stoptime->GetRandom()-864*log(gRandom->Uniform());
		double shifttime = 0;
		shifttime = fmod(O_mt+decaytime,tsep)-O_mt;
		O_mt += shifttime;

		double hittime;
		double tof;
		double starttime;
		double stoptime;
		int hittype;
		for (int j = 0; j<CdcCell_nHits; j++){
			if ((*CdcCell_tid)[j]==1){
				hittype = 0;
			}
			else{
				hittype = 1;
			}
			starttime = (*CdcCell_tstart)[j] + shifttime;
			stoptime = (*CdcCell_tstop)[j] + shifttime;
			hittime = (*CdcCell_t)[j] + shifttime;
			tof = (*CdcCell_t)[j];
			if (dict[(*CdcCell_layerID)[j]][(*CdcCell_cellID)[j]]==-1){
				dict[(*CdcCell_layerID)[j]][(*CdcCell_cellID)[j]]=O_nHits;
				O_nHits++;
				O_t->push_back(hittime);
				O_tof->push_back(tof);
				O_tstart->push_back(starttime);
				O_tstop->push_back(stoptime);
				O_wx->push_back((*CdcCell_wx)[j]);
				O_wy->push_back((*CdcCell_wy)[j]);
				O_wz->push_back((*CdcCell_wz)[j]);
				O_x->push_back((*CdcCell_x)[j]);
				O_y->push_back((*CdcCell_y)[j]);
				O_z->push_back((*CdcCell_z)[j]);
				O_px->push_back((*CdcCell_px)[j]);
				O_py->push_back((*CdcCell_py)[j]);
				O_pz->push_back((*CdcCell_pz)[j]);
				O_driftD->push_back((*CdcCell_driftD)[j]); // no need to monify, by now...
				O_driftDtrue->push_back((*CdcCell_driftDtrue)[j]);
				O_cellID->push_back((*CdcCell_cellID)[j]);
				O_layerID->push_back((*CdcCell_layerID)[j]);
				O_edep->push_back((*CdcCell_edep)[j]);
				O_posflag->push_back((*CdcCell_posflag)[j]);
				O_hittype->push_back(hittype);
			}
			else{
				std::cout<<"WTF?"<<std::endl;
			}
		}
		// mixture bkg
		for (int ibkg = 0; ibkg<nbkg; ibkg++){
			c_BKG->GetEntry((int)(gRandom->Uniform()*nBKG)%nBKG);
//			index = index+ibkg;
//			int nindex = index%nBKG;
//			c_BKG->GetEntry(nindex);
			CdcCell_nHits+=BKG_nHits;
			for (int ibkghit = 0; ibkghit<BKG_nHits; ibkghit++){
				// get time
				int ihit = dict[((*BKG_layerID)[ibkghit])][((*BKG_cellID)[ibkghit])];
				double dt = fmod((*BKG_tstart)[ibkghit]-O_mt,tsep);
				if (dt<=0){ // before trigger
					if (ihit==-1) continue; // new hit, no need
					if (stoptime>(*O_tstart)[ihit]-200){
						if ((*O_hittype)[ihit]==0) (*O_hittype)[ihit] = -1; // if this overlapped hit is from signal, then mark as -1
						(*O_edep)[ihit] = (*O_edep)[ihit]+(*BKG_edep)[ibkghit];
						(*O_driftD)[ihit] = 0;
						continue; // overlapped this hit from before trigger time; actually this hit would not be recorded.
					}
					else{ // can be separate
						continue; // not overlapped, ignore
					}
				}
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
				starttime = O_mt+dt;
				hittime = dt+O_mt+(*BKG_t)[ibkghit]-(*BKG_tstart)[ibkghit];
				stoptime = dt+O_mt+(*BKG_tstop)[ibkghit]-(*BKG_tstart)[ibkghit];
				// ----- new?
				if (ihit==-1){
					O_nHits++;
					dict[((*BKG_layerID)[ibkghit])][((*BKG_cellID)[ibkghit])]=O_nHits;
					O_t->push_back(hittime);
					O_tof->push_back(O_mt-hittime);
					O_wx->push_back((*BKG_wx)[ibkghit]/10);
					O_wy->push_back((*BKG_wy)[ibkghit]/10);
					O_wz->push_back((*BKG_wz)[ibkghit]/10);
					O_x->push_back((*BKG_x)[ibkghit]/10);
					O_y->push_back((*BKG_y)[ibkghit]/10);
					O_z->push_back((*BKG_z)[ibkghit]/10);
					O_px->push_back((*BKG_px)[ibkghit]/1000);
					O_py->push_back((*BKG_py)[ibkghit]/1000);
					O_pz->push_back((*BKG_pz)[ibkghit]/1000);
					O_driftD->push_back(dd);
					O_driftDtrue->push_back((*BKG_driftDtrue)[ibkghit]);
					O_cellID->push_back((*BKG_cellID)[ibkghit]);
					O_layerID->push_back((*BKG_layerID)[ibkghit]);
					O_edep->push_back((*BKG_edep)[ibkghit]);
					O_posflag->push_back((*BKG_posflag)[ibkghit]);
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
						(*O_edep)[ihit] = (*O_edep)[ihit] + (*BKG_edep)[ibkghit];
					}
				}
			}
		}
		ot->Fill();
	}
	ot->Write();
	of->Close();
	return 0;
}
