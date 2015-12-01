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
	TString runName = argv[1];
	double twindow = 1000;
	double me = 0.511e-3; // GeV
	TChain * c = new TChain("tree");
	//c->Add("/home/chen/MyWorkArea/Simulate/comet/output/CyDet.Acc.em."+runName+".root");
	//c->Add("/home/chen/MyWorkArea/Simulate/comet/output/CyDet.Acc.em.new.root");
	//c->Add("/home/chen/MyWorkArea/Simulate/comet/output/CyDet.Acc.em.root");
	//c->Add("/home/chen/MyWorkArea/Simulate/comet/output/CyDet.Acc.em.mrot.root");
	//c->Add("/home/chen/MyWorkArea/Simulate/comet/output/CyDet.Acc.ep.mrot.root");
	//c->Add("/home/chen/MyWorkArea/Simulate/comet/output/CyDet.Acc.em.rev.root");
	//c->Add("/home/chen/MyWorkArea/Simulate/comet/output/CyDet.Acc.ep.root");
	//c->Add("/home/chen/MyWorkArea/Simulate/comet/output/raw_g4sim.root");
	for (int i = 0; i < 100; i++){
		c->Add(Form("/scratchfs/bes/wuc/MyWorkArea/Data/A9.rmc.150919.W500um.OptD2.1mmCFRP.DD35.1cmLead/%d_job0.raw",i));
	}
	std::vector<int> * in_triID = 0;
	std::vector<int> * in_tritid= 0;
	std::vector<double> * in_trit = 0;
	std::vector<double> * in_triedep = 0;
	std::vector<double> * in_tripx = 0;
	std::vector<double> * in_tripy = 0;
	std::vector<double> * in_tripz = 0;
	std::vector<double> * in_triZ = 0;
	std::vector<std::string> * in_triName= 0;
	std::vector<int> * in_tritid = 0;
	std::vector<int> * in_cdclid = 0;
	std::vector<int> * in_cdctid = 0;
	std::vector<double> * in_cdct = 0;
	std::vector<int> * in_mcpid = 0;
	std::vector<int> * in_mctid = 0;
	std::vector<double> * in_mctime = 0;
	std::vector<double> * in_mcpx = 0;
	std::vector<double> * in_mcpy = 0;
	std::vector<double> * in_mcpz = 0;
	int evt_num = 0;
	c->SetBranchAddress("evt_num",&evt_num);
	c->SetBranchAddress("CdcCell_layerID",&in_cdclid);
	c->SetBranchAddress("CdcCell_tid",&in_cdctid);
	c->SetBranchAddress("CdcCell_t",&in_cdct);
	c->SetBranchAddress("McTruth_pid",&in_mcpid);
	c->SetBranchAddress("McTruth_tid",&in_mctid);
	c->SetBranchAddress("McTruth_time",&in_mctime);
	c->SetBranchAddress("McTruth_px",&in_mcpx);
	c->SetBranchAddress("McTruth_py",&in_mcpy);
	c->SetBranchAddress("McTruth_pz",&in_mcpz);
	c->SetBranchAddress("M_t",&in_trit);
	c->SetBranchAddress("M_edep",&in_triedep);
	c->SetBranchAddress("M_px",&in_tripx);
	c->SetBranchAddress("M_py",&in_tripy);
	c->SetBranchAddress("M_pz",&in_tripz);
	c->SetBranchAddress("M_z",&in_triZ);
	c->SetBranchAddress("M_tid",&in_tritid);
	c->SetBranchAddress("M_volID",&in_triID);
	c->SetBranchAddress("M_volName",&in_triName);
	c->SetBranchAddress("M_tid",&in_tritid);
	TFile * ofile = new TFile("CyDet."+runName+".root","RECREATE");
	TTree * otree = new TTree("t","t");
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
	otree->Branch("evt_num",&evt_num);
	double sci_time[128];
	double che_time[128];
	Long64_t nEntries = c->GetEntries();
	std::vector<int> vtype;
	std::vector<int> vpos;
	std::vector<double> vtime;
	std::cout<<nEntries<<" entries to check!"<<std::endl;
	for (Long64_t iEntry = 0; iEntry<nEntries; iEntry++){
//	for (Long64_t iEntry = 284; iEntry<=284; iEntry++){
		if (iEntry%1000==0) std::cout<<(double)iEntry/nEntries*100<<"%..."<<std::endl;
		c->GetEntry(iEntry);

		int thimc = -1;
		int thetid = -1;
		for (int imc = 0; imc<in_mcpz->size(); imc++){
			if ((*in_mcpid)[imc]==11){
				px = (*in_mcpx)[imc]*1000;
				py = (*in_mcpy)[imc]*1000;
				pz = (*in_mcpz)[imc]*1000;
				if (sqrt(px*px+py*py+pz*pz)>100){
					thimc = imc;
					thetid = (*in_mctid)[imc];
					break;
				}
			}
		}
		if (thimc<0) continue;

		theta = acos((*in_mcpz)[thimc]/sqrt((*in_mcpx)[thimc]*(*in_mcpx)[thimc]+(*in_mcpy)[thimc]*(*in_mcpy)[thimc]));
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
			if ((*in_cdctid)[ihit]!=thetid) continue;
			if (((*in_cdct)[ihit]-(*in_mctime)[thimc])/7+1>nturn) nturn = ((*in_cdct)[ihit]-(*in_mctime)[thimc])/7+1;
			if ((*in_cdct)[ihit]-(*in_mctime)[thimc]>7) continue;
			if (maxlid<(*in_cdclid)[ihit]) maxlid=(*in_cdclid)[ihit];
			if ((*in_cdclid)[ihit]-prelid==1) currentlayersIn++;
			else if ((*in_cdclid)[ihit]-prelid==-1) currentlayersOut++;
			else if ((*in_cdclid)[ihit]!=prelid) {currentlayersIn=0;currentlayersOut=0;}
			if (maxcontinuedOut<currentlayersOut) maxcontinuedOut=currentlayersOut;
			if (maxcontinuedIn<currentlayersIn) maxcontinuedIn=currentlayersIn;
			prelid=(*in_cdclid)[ihit];
			cdc_nHits++;
		}
		//std::cout<<maxlid<<","<<cdc_nHits<<","<<maxcontinuedOut<<","<<maxcontinuedU<<std::endl;

		for (int itri = 0; itri<128; itri++){
			che_time[itri] = -1;
			sci_time[itri] = -1;
		}
		tri_nHits = 0;
		if (in_triName->size()){
			if ((*in_triName)[0]=="TriCheLU"
					||(*in_triName)[0]=="TriCheLD"
					||(*in_triName)[0]=="TriCheU"
					||(*in_triName)[0]=="TriCheD"
			   ){
				dir = 1;
			}
			else if ((*in_triName)[0]=="TriSciU"
					||(*in_triName)[0]=="TriSciD"
					){
				dir = -1;
			}
		}
		for ( int ihit = 0; ihit<in_trit->size(); ihit++){
			if ((*in_tritid)[ihit]!=thetid) continue;
			if ((*in_triName)[ihit]=="TriChePD"
			  ||(*in_triName)[ihit]=="TriChePU"
			  ||(*in_triName)[ihit]=="TriSciPD"
			  ||(*in_triName)[ihit]=="TriSciPU"
			  ||(*in_triName)[ihit]=="TriSciLD"
			  ||(*in_triName)[ihit]=="TriSciLU"
					) continue;
			tri_nHits++;
//			double px = (*in_tripx)[ihit];
//			double py = (*in_tripy)[ihit];
//			double pz = (*in_tripz)[ihit];
//			double pa = sqrt(px*px+py*py+pz*pz);
//			double beta = pa/sqrt(pa*pa+me*me);
			if ((*in_triName)[ihit]=="TriCheD"
				||(*in_triName)[ihit]=="TriCheLD"
					){ // Cherenkov
//				if (beta>1/1.5){
				if (1){
					che_time[(*in_triID)[ihit]+64] = (*in_trit)[ihit];
//					std::cout<<"@"<<ihit<<": che_time["<<(*in_triID)[ihit]+64<<"]="<<(*in_trit)[ihit]<<std::endl;
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
//				if (beta>1/1.5){
				if (1){
					che_time[(*in_triID)[ihit]] = (*in_trit)[ihit];
//					std::cout<<"@"<<ihit<<": che_time["<<(*in_triID)[ihit]+64<<"]="<<(*in_trit)[ihit]<<std::endl;
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
			else if ((*in_cdct)[0]>(*in_trit)[0]) tri_nHits*=-1;
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
//			std::cout<<itri<<"-"<<nsci<<"->"<<rotate(itri,nsci-1)<<std::endl;
			for (int delta = -2; delta<=2; delta++){
				int jtri = rotate(itri,delta);
				if (che_time[jtri]==-1||fabs(che_time[jtri]-time)>=10) continue;
				int nche = 1;
				for (int delta = 1; delta<=2; delta++){
					int ktri = rotate(jtri,delta);
					if (che_time[ktri]!=-1&&fabs(che_time[ktri]-time)<10) nche++;
					else break;
				}
//				std::cout<<"  "<<jtri<<"-"<<nche<<"->"<<rotate(jtri,nche-1)<<std::endl;
//				std::cout<<"  "<<nsci*100+nche*10+delta<<std::endl;
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
//		std::cout<<"type = "<<type<<std::endl;
		otree->Fill();
	}
	otree->Write();
	ofile->Close();
	return 0;
}
