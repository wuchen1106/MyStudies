#include <vector>
#include <string>
#include <iostream>
#include <math.h>
#include "TH1D.h"
#include "TChain.h"
#include "TFile.h"
#include "TTree.h"

int rotate(int n0, int dn){
	int n1 = n0+dn;
	if (n0<64&&n1>=64) n1-=64;
	if (n0>=64&&n1<64) n1+=64;
	if (n1>=128) n1-=64;
	if (n1<0) n1+=64;
	return n1;
}

int main (int argc, char** argv){
	double twindow = 10;
	double me = 0.511; // MeV
	TChain * c = new TChain("tree");
	//TString runName = "ALL.150901.W100um.OptD2.DD35.TH200um";
	//TString runName = "ALL.150901.W100um.OptD2.DD28.TH200um";
	//TString runName = "ALL.150901.W100um.OptD2.DD28.TH10mm";
	//TString runName = "ALL.150901.W100um.OptD2.DD28.TH10mm.Lead";
	TString runName = "ALL.150919.W500um.OptD2.1mmCFRP.DD35.1cmLead";
	//TString runName = "ALL.150901.W100um.OptD2.1mmCFRP.1cmAl.DD35";
	//TString runName = "ALL.150901.W100um.OptD2.5mmCFRP.1cmAl.DD35";
	//TString runName = "ALL.150901.W100um.OptD2.2cmSUS.2cmAl.DD35";
	//TString runName = "ALL.150901.W100um.OptD2.2cmSUS.2cmSUS.DD35";
	if (argc>1) runName = argv[1];
	c->Add(runName+".root");
	std::vector<double> * in_x = 0;
	std::vector<double> * in_y = 0;
	std::vector<double> * in_z = 0;
	std::vector<double> * in_px = 0;
	std::vector<double> * in_py = 0;
	std::vector<double> * in_pz = 0;
	std::vector<double> * in_t = 0;
	std::vector<int> * in_pid = 0;
	std::vector<int> * in_dep = 0;
	std::vector<std::string> * in_process = 0;
	std::vector<std::string> * in_particle = 0;
	std::vector<std::string> * in_volume = 0;
	int in_type;
	int in_topo;
	int in_nHits;
	int in_cpid;
	int evt_num;
	int run_num;
	std::vector<int> * in_triType = 0;
	std::vector<int> * in_triID = 0;
	std::vector<int> * in_tripos = 0;
	std::vector<double> * in_triz = 0;
	std::vector<double> * in_trit = 0;
	std::vector<double> * in_triedep = 0;
	std::vector<double> * in_tripx = 0;
	std::vector<double> * in_tripy = 0;
	std::vector<double> * in_tripz = 0;
	c->SetBranchAddress("evt_num",&evt_num);
	c->SetBranchAddress("run_num",&run_num);
	c->SetBranchAddress("x",&in_x);
	c->SetBranchAddress("y",&in_y);
	c->SetBranchAddress("z",&in_z);
	c->SetBranchAddress("px",&in_px);
	c->SetBranchAddress("py",&in_py);
	c->SetBranchAddress("pz",&in_pz);
	c->SetBranchAddress("t",&in_t);
	c->SetBranchAddress("dep",&in_dep);
	c->SetBranchAddress("process",&in_process);
	c->SetBranchAddress("particle",&in_particle);
	c->SetBranchAddress("volume",&in_volume);
	c->SetBranchAddress("pid",&in_pid);
	c->SetBranchAddress("type",&in_type);
	c->SetBranchAddress("topo",&in_topo);
	c->SetBranchAddress("nHits",&in_nHits);
	c->SetBranchAddress("cpid",&in_cpid);
	c->SetBranchAddress("O_t",&in_trit);
	c->SetBranchAddress("O_z",&in_triz);
	c->SetBranchAddress("O_edep",&in_triedep);
	c->SetBranchAddress("O_px",&in_tripx);
	c->SetBranchAddress("O_py",&in_tripy);
	c->SetBranchAddress("O_pz",&in_tripz);
	c->SetBranchAddress("O_triType",&in_triType);
	c->SetBranchAddress("O_triID",&in_triID);
	c->SetBranchAddress("O_triPos",&in_tripos);
	double out_x;
	double out_y;
	double out_z;
	double out_px;
	double out_py;
	double out_pz;
	double out_t;
	int out_pid;
	TFile * ofile = new TFile(runName+".output.root","RECREATE");
	TTree * otree = new TTree("t","t");
	int type = 0;
	int tritype = 0;
	int tripos = 0;
	double trit = 0;
	int cu,cd,su,sd;
	otree->Branch("evt_num",&evt_num);
	otree->Branch("run_num",&run_num);
	otree->Branch("tritype",&tritype);
	otree->Branch("tripos",&tripos);
	otree->Branch("trit",&trit);
	otree->Branch("x",&in_x);
	otree->Branch("y",&in_y);
	otree->Branch("z",&in_z);
	otree->Branch("px",&in_px);
	otree->Branch("py",&in_py);
	otree->Branch("pz",&in_pz);
	otree->Branch("t",&in_t);
	otree->Branch("pid",&in_pid);
	otree->Branch("type",&type);
	otree->Branch("topo",&in_topo);
	otree->Branch("dep",&in_dep);
	otree->Branch("process",&in_process);
	otree->Branch("particle",&in_particle);
	otree->Branch("volume",&in_volume);
	otree->Branch("cu",&cu);
	otree->Branch("cd",&cd);
	otree->Branch("su",&su);
	otree->Branch("sd",&sd);
	double sci_time[128];
	double che_time[128];
	int totalfound[9] = {0};
	int nHitsSU[9] = {0};
	int nHitsCU[9] = {0};
	int nHitsSD[9] = {0};
	int nHitsCD[9] = {0};
	int nHitsSUl[9] = {0};
	int nHitsCUl[9] = {0};
	int nHitsSDl[9] = {0};
	int nHitsCDl[9] = {0};
	double edepSU[9] = {0};
	double edepCU[9] = {0};
	double edepSD[9] = {0};
	double edepCD[9] = {0};
	double edepSUl[9] = {0};
	double edepCUl[9] = {0};
	double edepSDl[9] = {0};
	double edepCDl[9] = {0};

	TH1D* hSU[9];
	TH1D* hCU[9];
	TH1D* hSD[9];
	TH1D* hCD[9];
	for ( int i = 0; i<9; i++){
		hSU[i] = new TH1D(Form("hSU%d",i),Form("hSU%d",i),1170,0,1170);
		hCU[i] = new TH1D(Form("hCU%d",i),Form("hCU%d",i),1170,0,1170);
		hSD[i] = new TH1D(Form("hSD%d",i),Form("hSD%d",i),1170,0,1170);
		hCD[i] = new TH1D(Form("hCD%d",i),Form("hCD%d",i),1170,0,1170);
	}

	std::vector<int> vtype;
	std::vector<int> vpos;
	std::vector<double> vtime;
	Long64_t nEntries = c->GetEntries();
	for (Long64_t iEntry = 0; iEntry<nEntries; iEntry++){
		if (iEntry%100000==0) std::cout<<(double)iEntry/nEntries*100<<"%..."<<std::endl;
		c->GetEntry(iEntry);
		//if (in_topo==-2) continue;
		if (in_type!=1) continue;
		if (in_nHits<1) continue;

		// define the type
		type = 8; // else
		bool foundmuon = false;
		bool foundgamtgt = false;
		bool foundgamtail = false;
		bool foundmuoncon = false;
		bool foundmuontgt = false;
		int inipid = 0;
		for (int ip = 0; ip<in_pid->size(); ip++){
			if (ip<in_pid->size()-1&&(*in_pid)[ip+1]==13&&((*in_volume)[ip]).substr(0,3)=="Tri") foundmuon=true;
			if (ip<in_pid->size()-1&&(*in_pid)[ip+1]==13&&((*in_volume)[ip])=="A9Container") foundmuoncon=true;
			if (ip<in_pid->size()-1&&(*in_pid)[ip+1]==13&&(*in_pid)[ip]==11&&((*in_volume)[ip])=="Target") foundmuontgt=true;
			if ((*in_pid)[ip]==22&&((*in_volume)[ip]).substr(0,4)=="Tail") foundgamtail=true;
			if ((*in_pid)[ip]==22&&(*in_volume)[ip]=="Target") foundgamtgt=true;
			inipid = (*in_pid)[ip];
		}
		if (inipid==-13) type=6;
		else if (fabs(inipid)==211) type=7;
		else if (foundmuon) type=2;
		else if (foundmuoncon) type=3;
		else if (foundgamtgt) type=1;
		else if (foundgamtail) type=0;
		else if (foundmuontgt) type=4;

		cu = 0; cd = 0; su = 0; sd = 0;
		for (int itri = 0; itri<128; itri++){
			che_time[itri] = -1;
			sci_time[itri] = -1;
		}
		for ( int ihit = 0; ihit<in_nHits; ihit++){
			double time = fmod((*in_trit)[ihit],1170);
			if ((*in_triType)[ihit]==0&&fabs(in_cpid)==11){
				double px = (*in_tripx)[ihit];
				double py = (*in_tripy)[ihit];
				double pz = (*in_tripz)[ihit];
				double pa = sqrt(px*px+py*py+pz*pz);
				double beta = pa/sqrt(pa*pa+me*me);
				if (beta>1/1.5){
					che_time[(*in_triID)[ihit]+((*in_tripos)[ihit]+1)*32] = time;
					if ((*in_tripos)[ihit]>0){
						nHitsCD[type]++;
						cd=1;
						edepCD[type]+=(*in_triedep)[ihit];
						if (time>200){
							nHitsCDl[type]++;
							edepCDl[type]+=(*in_triedep)[ihit];
						}
						hCD[type]->Fill(time);
					}
					else {
						nHitsCU[type]++;
						cu=1;
						edepCU[type]+=(*in_triedep)[ihit];
						if (time>200){
							nHitsCUl[type]++;
							edepCUl[type]+=(*in_triedep)[ihit];
						}
						hCU[type]->Fill(time);
					}
				}
			}
			else if ((*in_triType)[ihit]==1&&(*in_triedep)[ihit]>630e-6){
//			else if ((*in_triType)[ihit]==1){
				sci_time[(*in_triID)[ihit]+((*in_tripos)[ihit]+1)*32] = time;
				if ((*in_tripos)[ihit]>0){
					sd = 1;
					nHitsSD[type]++;
					edepSD[type]+=(*in_triedep)[ihit];
					if (time>200){
						nHitsSDl[type]++;
						edepSDl[type]+=(*in_triedep)[ihit];
					}
					hSD[type]->Fill(time);
				}
				else{
					su = 1;
					nHitsSU[type]++;
					edepSU[type]+=(*in_triedep)[ihit];
					if (time>200){
						nHitsSUl[type]++;
						edepSUl[type]+=(*in_triedep)[ihit];
					}
					hSU[type]->Fill(time);
				}
			}
		}
		// option D
		vtype.clear();
		vtime.clear();
		vpos.clear();
		for (int itri = 0; itri<128; itri++){
			int time = sci_time[itri];
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
				trit = vtime[it];
				tripos = vpos[it];
			}
		}
		tritype = nhitmax*10+ndeltamin;
		otree->Fill();
	}
	int nHitsSUt = 0;
	int nHitsSDt = 0;
	int nHitsCUt = 0;
	int nHitsCDt = 0;
	int nHitsSUtl = 0;
	int nHitsSDtl = 0;
	int nHitsCUtl = 0;
	int nHitsCDtl = 0;
	int totalfoundt = 0;
	for (int type = 0; type<9; type++){
		std::cout<<"type "<<type<<":"<<std::endl;
		std::cout<<"   totalfound = "<<totalfound[type]<<", "<<(double)totalfound[type]/1e9*2.5e12*2.6/1e3<<" kHz"<<std::endl;
		std::cout<<"   nHitsSU = "<<nHitsSU[type]<<", "<<(double)nHitsSU[type]/1e9*2.5e12*2.6/1e3/64<<" kHz"<<";   nHitsSUl = "<<nHitsSUl[type]<<", "<<(double)nHitsSUl[type]/1e9*2.5e12*2.6/1e3/64<<" kHz"<<std::endl;
		std::cout<<"   nHitsSD = "<<nHitsSD[type]<<", "<<(double)nHitsSD[type]/1e9*2.5e12*2.6/1e3/64<<" kHz"<<";   nHitsSDl = "<<nHitsSDl[type]<<", "<<(double)nHitsSDl[type]/1e9*2.5e12*2.6/1e3/64<<" kHz"<<std::endl;
		std::cout<<"   nHitsCU = "<<nHitsCU[type]<<", "<<(double)nHitsCU[type]/1e9*2.5e12*2.6/1e3/64<<" kHz"<<";   nHitsCUl = "<<nHitsCUl[type]<<", "<<(double)nHitsCUl[type]/1e9*2.5e12*2.6/1e3/64<<" kHz"<<std::endl;
		std::cout<<"   nHitsCD = "<<nHitsCD[type]<<", "<<(double)nHitsCD[type]/1e9*2.5e12*2.6/1e3/64<<" kHz"<<";   nHitsCDl = "<<nHitsCDl[type]<<", "<<(double)nHitsCDl[type]/1e9*2.5e12*2.6/1e3/64<<" kHz"<<std::endl;
		totalfoundt += totalfound[type];
		nHitsSUt+=nHitsSU[type];
		nHitsSDt+=nHitsSD[type];
		nHitsCUt+=nHitsCU[type];
		nHitsCDt+=nHitsCD[type];
		nHitsSUtl+=nHitsSUl[type];
		nHitsSDtl+=nHitsSDl[type];
		nHitsCUtl+=nHitsCUl[type];
		nHitsCDtl+=nHitsCDl[type];
	}
	std::cout<<"totally:"<<std::endl;
	std::cout<<"   totalfound = "<<totalfoundt<<", "<<(double)totalfoundt/1e9*2.5e12*2.6/1e3<<" kHz"<<std::endl;
	std::cout<<"   nHitsSU = "<<nHitsSUt<<", "<<(double)nHitsSUt/1e9*2.5e12*2.6/1e3/64<<" kHz"<<";   nHitsSUl = "<<nHitsSUtl<<", "<<(double)nHitsSUtl/1e9*2.5e12*2.6/1e3/64<<" kHz"<<std::endl;
	std::cout<<"   nHitsSD = "<<nHitsSDt<<", "<<(double)nHitsSDt/1e9*2.5e12*2.6/1e3/64<<" kHz"<<";   nHitsSDl = "<<nHitsSDtl<<", "<<(double)nHitsSDtl/1e9*2.5e12*2.6/1e3/64<<" kHz"<<std::endl;
	std::cout<<"   nHitsCU = "<<nHitsCUt<<", "<<(double)nHitsCUt/1e9*2.5e12*2.6/1e3/64<<" kHz"<<";   nHitsCUl = "<<nHitsCUtl<<", "<<(double)nHitsCUtl/1e9*2.5e12*2.6/1e3/64<<" kHz"<<std::endl;
	std::cout<<"   nHitsCD = "<<nHitsCDt<<", "<<(double)nHitsCDt/1e9*2.5e12*2.6/1e3/64<<" kHz"<<";   nHitsCDl = "<<nHitsCDtl<<", "<<(double)nHitsCDtl/1e9*2.5e12*2.6/1e3/64<<" kHz"<<std::endl;
	double edepSUt = 0;
	double edepSDt = 0;
	double edepCUt = 0;
	double edepCDt = 0;
	double edepSUtl = 0;
	double edepSDtl = 0;
	double edepCUtl = 0;
	double edepCDtl = 0;
	for (int type = 0; type<9; type++){
		std::cout<<"type "<<type<<":"<<std::endl;
		std::cout<<"   edepSU = "<<edepSU[type]<<", "<<(double)edepSU[type]/1e9*2.5e12*2.6*1.17e-6/64<<" GeV"<<";   edepSUl = "<<edepSUl[type]<<", "<<(double)edepSUl[type]/1e9*2.5e12*2.6*1.17e-6/64<<" GeV"<<std::endl;
		std::cout<<"   edepSD = "<<edepSD[type]<<", "<<(double)edepSD[type]/1e9*2.5e12*2.6*1.17e-6/64<<" GeV"<<";   edepSDl = "<<edepSDl[type]<<", "<<(double)edepSDl[type]/1e9*2.5e12*2.6*1.17e-6/64<<" GeV"<<std::endl;
		std::cout<<"   edepCU = "<<edepCU[type]<<", "<<(double)edepCU[type]/1e9*2.5e12*2.6*1.17e-6/64<<" GeV"<<";   edepCUl = "<<edepCUl[type]<<", "<<(double)edepCUl[type]/1e9*2.5e12*2.6*1.17e-6/64<<" GeV"<<std::endl;
		std::cout<<"   edepCD = "<<edepCD[type]<<", "<<(double)edepCD[type]/1e9*2.5e12*2.6*1.17e-6/64<<" GeV"<<";   edepCDl = "<<edepCDl[type]<<", "<<(double)edepCDl[type]/1e9*2.5e12*2.6*1.17e-6/64<<" GeV"<<std::endl;
		edepSUt+=edepSU[type];
		edepSDt+=edepSD[type];
		edepCUt+=edepCU[type];
		edepCDt+=edepCD[type];
		edepSUtl+=edepSUl[type];
		edepSDtl+=edepSDl[type];
		edepCUtl+=edepCUl[type];
		edepCDtl+=edepCDl[type];
	}
	std::cout<<"totally:"<<std::endl;
	std::cout<<"   edepSU = "<<edepSUt<<", "<<(double)edepSUt/1e9*2.5e12*2.6*1.17e-6/64<<" GeV"<<";   edepSUl = "<<edepSUtl<<", "<<(double)edepSUtl/1e9*2.5e12*2.6*1.17e-6/64<<" GeV"<<std::endl;
	std::cout<<"   edepSD = "<<edepSDt<<", "<<(double)edepSDt/1e9*2.5e12*2.6*1.17e-6/64<<" GeV"<<";   edepSDl = "<<edepSDtl<<", "<<(double)edepSDtl/1e9*2.5e12*2.6*1.17e-6/64<<" GeV"<<std::endl;
	std::cout<<"   edepCU = "<<edepCUt<<", "<<(double)edepCUt/1e9*2.5e12*2.6*1.17e-6/64<<" GeV"<<";   edepCUl = "<<edepCUtl<<", "<<(double)edepCUtl/1e9*2.5e12*2.6*1.17e-6/64<<" GeV"<<std::endl;
	std::cout<<"   edepCD = "<<edepCDt<<", "<<(double)edepCDt/1e9*2.5e12*2.6*1.17e-6/64<<" GeV"<<";   edepCDl = "<<edepCDtl<<", "<<(double)edepCDtl/1e9*2.5e12*2.6*1.17e-6/64<<" GeV"<<std::endl;
	otree->Write();
	for ( int i = 0; i<9; i++){
		hSU[i]->Write(); 
		hCU[i]->Write();
		hSD[i]->Write();
		hCD[i]->Write();
	}
	ofile->Close();
	return 0;
}
