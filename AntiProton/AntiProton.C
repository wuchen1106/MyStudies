#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>
#include <vector>
#include <algorithm>

#include <cstdlib>

#include "TChain.h"
#include "TTree.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TGraph.h"
#include "Math/DistFunc.h"
#include "TVector3.h"

std::vector<TString> nameForH2D;
std::vector<TString> titleForH2D;
std::vector<TString> xNameForH2D;
std::vector<TString> yNameForH2D;
std::vector<int>     bin1ForH2D;
std::vector<double>  left1ForH2D;
std::vector<double>  right1ForH2D;
std::vector<int>     bin2ForH2D;
std::vector<double>  left2ForH2D;
std::vector<double>  right2ForH2D;
std::vector<TH2D*>   vecH2D;

std::vector<TString> nameForH1D;
std::vector<TString> titleForH1D;
std::vector<int> compareForH1D;
std::vector<int> xlogForH1D;
std::vector<int> ylogForH1D;
std::vector<int> colorForH1D;
std::vector<int> markerForH1D;
std::vector<TString> drawOptForH1D;
std::vector<TString> xNameForH1D;
std::vector<TString> yNameForH1D;
std::vector<int>     bin1ForH1D;
std::vector<double>  left1ForH1D;
std::vector<double>  right1ForH1D;
std::vector<TH1D*>   vecH1D;

std::vector<TString> nameForGraph;
std::vector<TString> titleForGraph;
std::vector<int> compareForGraph;
std::vector<int> xlogForGraph;
std::vector<int> ylogForGraph;
std::vector<int> colorForGraph;
std::vector<int> markerForGraph;
std::vector<TString> drawOptForGraph;
std::vector<TString> xNameForGraph;
std::vector<TString> yNameForGraph;
std::vector<std::vector<double> > xForGraph;
std::vector<std::vector<double> > yForGraph;

TH2D* get_TH2D(std::string name);
TH1D* get_TH1D(std::string name);
int get_TGraph(std::string name);
bool ISEMPTY(std::string content);

int main(int argc, char* argv[]){
	std::stringstream buff;

	//=======================================
	//*************read parameter**********
	//=> About verbose
	int verbose = 0;
	//=> About Event limit
	int nEventsLimit = 0;
	if ( argc >= 2 ){
		buff.str("");
		buff.clear();
		buff<<argv[1];
		buff>>verbose;
		if ( argc >= 3 ){
			buff.str("");
			buff.clear();
			buff<<argv[2];
			buff>>nEventsLimit;
		}
	}

	//=======================================
	//************Verbose Control***********
	int Verbose_SectorInfo = 5; //大概的流程情况
	std::string prefix_SectorInfo = "### ";
	int Verbose_HistInfo = 10; //有哪些hist,什么时候输出了，参数如何
	std::string prefix_HistInfo= "  [Histograms] ";
	int Verbose_Statistics = 10; //跟统计相关的(效率，分辨，粒子鉴别的情况)
	std::string prefix_Statistics="  [Statistics] ";
	int Verbose_FileInfo = 10; //有哪些FileList,都有多少file
	std::string prefix_FileInfo="  [FileInfo] ";
	int Verbose_EffInfo = 15; //Efficiency info
	std::string prefix_EffInfo="  [EffInfo] ";
	int Verbose_EventInfo = 20; //每个event的基本流程
	std::string prefix_EventInfoStart="    =>[EventInfo] ";
	std::string prefix_EventInfo="      [EventInfo] ";
	int Verbose_ParticleInfo=25; //每个particle的基本信息
	std::string prefix_ParticleInfoStart="    ->[ParticleInfo]";
	std::string prefix_ParticleInfo="      [ParticleInfo]";

	//=======================================================================================================
	//************PRESET********************
	if (verbose >= Verbose_SectorInfo ) std::cout<<prefix_SectorInfo<<"In PRESET###"<<std::endl;

	//=> About Histogram
	std::string histList = "histList";
	TH1D* TH1D_temp;
	TH2D* TH2D_temp;
	int index_temp;

	//=>About Constant
	double PI = 3.141592653589793238;
	double FSC = 1/137.03599911; //fine structure constant
	double M_MUON = 105.6584; //mass of muon in MeV
	double M_ELE = 0.510999; //mass of electron in MeV
	double M_U = 931.494061; //atomic mass unit in MeV

	//=>About output
	std::string OutputDir = "result/";


	//=======================================================================================================
	//************SET HISTOGRAMS********************
	if (verbose >= Verbose_SectorInfo ) std::cout<<prefix_SectorInfo<<"In SET HISTOGRAMS###"<<std::endl;
 
	//=>Read histList
	std::ifstream fin_card(histList.c_str());
	if(!fin_card){
		std::cout<<"Cannot find "<<histList<<std::endl;
	}
	std::string s_card;
	std::string histtype, histname, histtitle, dirname, runname, histdrawOpt, histxName, histyName;
	double histleft1, histright1, histleft2, histright2;
	int histbin1, histbin2, histcolor, histcompare, histlogx, histlogy, nCPU, njob, histmarker;
	std::vector<std::string> DirNames; 
	std::vector<std::string> RunNames; 
	std::vector<int> NCPU;
	std::vector<int> NJob;
	// read histList
	while(getline(fin_card,s_card)){
		if ( ISEMPTY(s_card) ) continue;
		buff.str("");
		buff.clear();
		buff<<s_card;
		buff>>histtype;
		if ( histtype == "TH1D" ){
			buff>>histname>>histtitle>>histxName>>histyName>>histbin1>>histleft1>>histright1>>histcolor>>histcompare>>histlogx>>histlogy>>histmarker>>histdrawOpt;
			nameForH1D.push_back(histname);
			titleForH1D.push_back(histtitle);
			xNameForH1D.push_back(histxName);
			yNameForH1D.push_back(histyName);
			bin1ForH1D.push_back(histbin1);
			left1ForH1D.push_back(histleft1);
			right1ForH1D.push_back(histright1);
			colorForH1D.push_back(histcolor);
			compareForH1D.push_back(histcompare);
			xlogForH1D.push_back(histlogx);
			ylogForH1D.push_back(histlogy);
			markerForH1D.push_back(histmarker);
			drawOptForH1D.push_back(histdrawOpt);
		}
		else if ( histtype == "TH2D" ){
			buff>>histname>>histtitle>>histxName>>histyName>>histbin1>>histleft1>>histright1>>histbin2>>histleft2>>histright2;
			nameForH2D.push_back(histname);
			titleForH2D.push_back(histtitle);
			xNameForH2D.push_back(histxName);
			yNameForH2D.push_back(histyName);
			bin1ForH2D.push_back(histbin1);
			left1ForH2D.push_back(histleft1);
			right1ForH2D.push_back(histright1);
			bin2ForH2D.push_back(histbin2);
			left2ForH2D.push_back(histleft2);
			right2ForH2D.push_back(histright2);
		}
		else if ( histtype == "FILE" ){
			buff>>dirname>>runname>>nCPU>>njob;
			DirNames.push_back(dirname);
			RunNames.push_back(runname);
			NCPU.push_back(nCPU);
			NJob.push_back(njob);
		}
		else if ( histtype == "TGraph" ){
			buff>>histname>>histtitle>>histxName>>histyName>>histcolor>>histcompare>>histlogx>>histlogy>>histmarker>>histdrawOpt;
			nameForGraph.push_back(histname);
			titleForGraph.push_back(histtitle);
			xNameForGraph.push_back(histxName);
			yNameForGraph.push_back(histyName);
			std::vector<double> avec;
			xForGraph.push_back(avec);
			std::vector<double> bvec;
			yForGraph.push_back(bvec);
			colorForGraph.push_back(histcolor);
			compareForGraph.push_back(histcompare);
			xlogForGraph.push_back(histlogx);
			ylogForGraph.push_back(histlogy);
			markerForGraph.push_back(histmarker);
			drawOptForGraph.push_back(histdrawOpt);
			int i = nameForGraph.size() - 1;
			if (verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<"Input vecGraph["<<i<<"]: "<<nameForGraph[i]<<", "<<titleForGraph[i]<<", "<<xNameForGraph[i]<<", "<<yNameForGraph[i]<<", Color="<<colorForGraph[i]<<", xlogSyle="<<xlogForGraph[i]<<", ylogSyle="<<ylogForGraph[i]<<", nCompare="<<compareForGraph[i]<<", markerStyle="<<markerForGraph[i]<<", drawOpt=\""<<drawOptForGraph[i]<<"\""<<std::endl;
		}
		else{
			std::cout<<"Cannot recogonize this line: "<<buff.str()<<std::endl;
			continue;
		}
	}

	//=> Get histograms in
	for ( int i = 0; i < nameForH2D.size(); i++ ){
		vecH2D.push_back(new TH2D(nameForH2D[i],titleForH2D[i],bin1ForH2D[i],left1ForH2D[i],right1ForH2D[i],bin2ForH2D[i],left2ForH2D[i],right2ForH2D[i]) );
	}
	for ( int i = 0; i < nameForH1D.size(); i++ ){
		vecH1D.push_back(new TH1D(nameForH1D[i],titleForH1D[i],bin1ForH1D[i],left1ForH1D[i],right1ForH1D[i]) );
	}

	//=======================================================================================================
	//************SET Statistics********************
	if (verbose >= Verbose_SectorInfo ) std::cout<<prefix_SectorInfo<<"In SET Statistics###"<<std::endl;

	//=>About Statistical
	int N0 = 0;
	int N1 = 0;
	int N2 = 0;
	int N3 = 0;

	//=> for efficiency
	int nbin_eff = 6;
	double min_eff = 94;
	double max_eff = 99;

	double halfbin_eff = (max_eff-min_eff)/((double)nbin_eff-1)/2;
	std::vector<int> mu_num;
	std::vector<int> mu_num_pass;
	std::vector<int> ap_num;
	std::vector<int> ap_num_pass;
	std::vector<double> mu_eff;
	std::vector<double> ap_eff;
	mu_eff.resize(nbin_eff);
	mu_num.resize(nbin_eff);
	mu_num_pass.resize(nbin_eff);
	ap_eff.resize(nbin_eff);
	ap_num.resize(nbin_eff);
	ap_num_pass.resize(nbin_eff);
	for ( int i = 0; i < nbin_eff; i++ ){
		mu_eff[i] = 0;
		mu_num[i] = 0;
		mu_num_pass[i] = 0;
		ap_eff[i] = 0;
		ap_num[i] = 0;
		ap_num_pass[i] = 0;
	}

	//=> threshold
	double mu_thre = 0;
	double ap_thre = 0;

	//=======================================================================================================
	//************READ THE FILES********************
	if (verbose >= Verbose_SectorInfo ) std::cout<<prefix_SectorInfo<<"In READ THE FILES###"<<std::endl;
	std::string TreeName = "tree";
	TChain *m_TChain = new TChain(TreeName.c_str());

	int iStart = 0;
	int nBit = 2;
	for ( int iFile = 0; iFile < DirNames.size(); iFile++ ){
		int nCPU = NCPU[iFile];
		int njob = NJob[iFile];
		if ( verbose >= Verbose_FileInfo) std::cout<<prefix_FileInfo<<"FileList \""<<DirNames[iFile]<<"\" with runname = \""<<RunNames[iFile]<<"\" has "<<NJob[iFile]<<" jobs on "<<NCPU[iFile]<<" CPUs"<<std::endl;
		for (int i = iStart; i < iStart + nCPU; i ++){
			for (int j = iStart; j < iStart + njob; j ++){
				buff.str("");
				buff.clear();
				buff<<DirNames[iFile]<<"/"<<i<<"_job"<<j<<"_"<<RunNames[iFile]<<".raw";
				m_TChain->Add(buff.str().c_str());
			}
		}
	}

	//=======================================================================================================
	//************SET BRANCHES********************
	if (verbose >= Verbose_SectorInfo ) std::cout<<prefix_SectorInfo<<"In SET BRANCHES###"<<std::endl;
	//=> Get all the stuffs in input file
	int run_num;
	int evt_num;

	int McTruth_nTracks;
	std::vector<int> *McTruth_pid = 0;
	std::vector<int> *McTruth_tid = 0;
	std::vector<int> *McTruth_ptid = 0;
	std::vector<double> *McTruth_time = 0;
	std::vector<double> *McTruth_px = 0;
	std::vector<double> *McTruth_py = 0;
	std::vector<double> *McTruth_pz = 0;
	std::vector<double> *McTruth_e = 0;
	std::vector<double> *McTruth_x = 0;
	std::vector<double> *McTruth_y = 0;
	std::vector<double> *McTruth_z = 0;
	std::vector<int> *McTruth_charge = 0;
	std::vector<std::string> *McTruth_particleName = 0;
	std::vector<std::string> *McTruth_process = 0;
	std::vector<std::string> *McTruth_volume = 0;

	int Monitor_nHits;
	std::vector<double> *Monitor_x = 0;
	std::vector<double> *Monitor_y = 0;
	std::vector<double> *Monitor_z = 0;
	std::vector<double> *Monitor_t = 0;
	std::vector<double> *Monitor_px = 0;
	std::vector<double> *Monitor_py = 0;
	std::vector<double> *Monitor_pz = 0;
	std::vector<double> *Monitor_e = 0;
	std::vector<double> *Monitor_edep = 0;
	std::vector<double> *Monitor_stepL = 0;
	std::vector<int> *Monitor_volID = 0;
	std::vector<std::string> *Monitor_volName = 0;
	std::vector<int> *Monitor_tid = 0;
	std::vector<int> *Monitor_pid = 0;
	std::vector<int> *Monitor_charge = 0;

	TBranch *bMcTruth_pid = 0;
	TBranch *bMcTruth_tid = 0;
	TBranch *bMcTruth_ptid = 0;
	TBranch *bMcTruth_time = 0;
	TBranch *bMcTruth_px = 0;
	TBranch *bMcTruth_py = 0;
	TBranch *bMcTruth_pz = 0;
	TBranch *bMcTruth_e = 0;
	TBranch *bMcTruth_x = 0;
	TBranch *bMcTruth_y = 0;
	TBranch *bMcTruth_z = 0;
	TBranch *bMcTruth_charge = 0;
	TBranch *bMcTruth_particleName = 0;
	TBranch *bMcTruth_process = 0;
	TBranch *bMcTruth_volume = 0;

	TBranch *bMonitor_x = 0;
	TBranch *bMonitor_y = 0;
	TBranch *bMonitor_z = 0;
	TBranch *bMonitor_t = 0;
	TBranch *bMonitor_px = 0;
	TBranch *bMonitor_py = 0;
	TBranch *bMonitor_pz = 0;
	TBranch *bMonitor_e = 0;
	TBranch *bMonitor_edep = 0;
	TBranch *bMonitor_stepL = 0;
	TBranch *bMonitor_volID = 0;
	TBranch *bMonitor_volName = 0;
	TBranch *bMonitor_tid = 0;
	TBranch *bMonitor_pid = 0;
	TBranch *bMonitor_charge = 0;

	m_TChain->SetBranchAddress("run_num", &run_num);
	m_TChain->SetBranchAddress("evt_num", &evt_num);

	m_TChain->SetBranchAddress("McTruth_nTracks", &McTruth_nTracks);
	m_TChain->SetBranchAddress("McTruth_pid", &McTruth_pid, &bMcTruth_pid);
	m_TChain->SetBranchAddress("McTruth_tid", &McTruth_tid, &bMcTruth_tid);
	m_TChain->SetBranchAddress("McTruth_ptid", &McTruth_ptid, &bMcTruth_ptid);
	m_TChain->SetBranchAddress("McTruth_time", &McTruth_time, &bMcTruth_time);
	m_TChain->SetBranchAddress("McTruth_px", &McTruth_px, &bMcTruth_px);
	m_TChain->SetBranchAddress("McTruth_py", &McTruth_py, &bMcTruth_py);
	m_TChain->SetBranchAddress("McTruth_pz", &McTruth_pz, &bMcTruth_pz);
	m_TChain->SetBranchAddress("McTruth_e", &McTruth_e, &bMcTruth_e);
	m_TChain->SetBranchAddress("McTruth_x", &McTruth_x, &bMcTruth_x);
	m_TChain->SetBranchAddress("McTruth_y", &McTruth_y, &bMcTruth_y);
	m_TChain->SetBranchAddress("McTruth_z", &McTruth_z, &bMcTruth_z);
	m_TChain->SetBranchAddress("McTruth_charge", &McTruth_charge, &bMcTruth_charge);
	m_TChain->SetBranchAddress("McTruth_particleName", &McTruth_particleName, &bMcTruth_particleName);
	m_TChain->SetBranchAddress("McTruth_process", &McTruth_process, &bMcTruth_process);
	m_TChain->SetBranchAddress("McTruth_volume", &McTruth_volume, &bMcTruth_volume);

	m_TChain->SetBranchAddress("Monitor_nHits", &Monitor_nHits);
	m_TChain->SetBranchAddress("Monitor_x", &Monitor_x, &bMonitor_x);
	m_TChain->SetBranchAddress("Monitor_y", &Monitor_y, &bMonitor_y);
	m_TChain->SetBranchAddress("Monitor_z", &Monitor_z, &bMonitor_z);
	m_TChain->SetBranchAddress("Monitor_t", &Monitor_t, &bMonitor_t);
	m_TChain->SetBranchAddress("Monitor_px", &Monitor_px, &bMonitor_px);
	m_TChain->SetBranchAddress("Monitor_py", &Monitor_py, &bMonitor_py);
	m_TChain->SetBranchAddress("Monitor_pz", &Monitor_pz, &bMonitor_pz);
	m_TChain->SetBranchAddress("Monitor_e", &Monitor_e, &bMonitor_e);
	m_TChain->SetBranchAddress("Monitor_edep", &Monitor_edep, &bMonitor_edep);
	m_TChain->SetBranchAddress("Monitor_stepL", &Monitor_stepL, &bMonitor_stepL);
	m_TChain->SetBranchAddress("Monitor_volID", &Monitor_volID, &bMonitor_volID);
	m_TChain->SetBranchAddress("Monitor_volName", &Monitor_volName, &bMonitor_volName);
	m_TChain->SetBranchAddress("Monitor_tid", &Monitor_tid, &bMonitor_tid);
	m_TChain->SetBranchAddress("Monitor_pid", &Monitor_pid, &bMonitor_pid);
	m_TChain->SetBranchAddress("Monitor_charge", &Monitor_charge, &bMonitor_charge);

	//=======================================================================================================
	//************DO THE DIRTY WORK*******************
	if (verbose >= Verbose_SectorInfo) std::cout<<prefix_SectorInfo<<"In DO THE DIRTY WORK ###"<<std::endl;
	Long64_t nEvent = m_TChain->GetEntries();
	//loop in events
	for( Long64_t iEvent = 0; iEvent < (nEventsLimit?nEventsLimit:nEvent); iEvent++ ){
		N0++;
		if (verbose >= Verbose_EventInfo ) std::cout<<prefix_EventInfoStart<<"In Event "<<iEvent<<std::endl;

		Long64_t tentry = m_TChain->LoadTree(iEvent);

		if(bMcTruth_pid) bMcTruth_pid->GetEntry(tentry);
		if(bMcTruth_tid) bMcTruth_tid->GetEntry(tentry);
		if(bMcTruth_ptid) bMcTruth_ptid->GetEntry(tentry);
		if(bMcTruth_time) bMcTruth_time->GetEntry(tentry);
		if(bMcTruth_px) bMcTruth_px->GetEntry(tentry);
		if(bMcTruth_py) bMcTruth_py->GetEntry(tentry);
		if(bMcTruth_pz) bMcTruth_pz->GetEntry(tentry);
		if(bMcTruth_e) bMcTruth_e->GetEntry(tentry);
		if(bMcTruth_x) bMcTruth_x->GetEntry(tentry);
		if(bMcTruth_y) bMcTruth_y->GetEntry(tentry);
		if(bMcTruth_z) bMcTruth_z->GetEntry(tentry);
		if(bMcTruth_charge) bMcTruth_charge->GetEntry(tentry);
		if(bMcTruth_particleName) bMcTruth_particleName->GetEntry(tentry);
		if(bMcTruth_process) bMcTruth_process->GetEntry(tentry);
		if(bMcTruth_volume) bMcTruth_volume->GetEntry(tentry);

		if(bMonitor_x) bMonitor_x->GetEntry(tentry);
		if(bMonitor_y) bMonitor_y->GetEntry(tentry);
		if(bMonitor_z) bMonitor_z->GetEntry(tentry);
		if(bMonitor_t) bMonitor_t->GetEntry(tentry);
		if(bMonitor_px) bMonitor_px->GetEntry(tentry);
		if(bMonitor_py) bMonitor_py->GetEntry(tentry);
		if(bMonitor_pz) bMonitor_pz->GetEntry(tentry);
		if(bMonitor_e) bMonitor_e->GetEntry(tentry);
		if(bMonitor_edep) bMonitor_edep->GetEntry(tentry);
		if(bMonitor_stepL) bMonitor_stepL->GetEntry(tentry);
		if(bMonitor_volID) bMonitor_volID->GetEntry(tentry);
		if(bMonitor_volName) bMonitor_volName->GetEntry(tentry);
		if(bMonitor_tid) bMonitor_tid->GetEntry(tentry);
		if(bMonitor_pid) bMonitor_pid->GetEntry(tentry);
		if(bMonitor_charge) bMonitor_charge->GetEntry(tentry);

		m_TChain->GetEntry(iEvent);
		if (verbose >= Verbose_EventInfo ) std::cout<<prefix_EventInfo<<"Got Entry!"<<std::endl;

		//=> Start the dirty work
		// muon or anti_proton
		int event_type = 0; // (1:anti_proton, 2: muon, 0: others)
		if ( (*McTruth_pid)[0] == -2212 )
			event_type = 1;
		else if ( (*McTruth_pid)[0] == 13 )
			event_type = 2;

		// passed or not
		bool passed = false;
		for ( int ihit = 0; ihit < Monitor_nHits; ihit++ ){
			if ( (*Monitor_volID)[ihit] == 1 && (*Monitor_tid)[ihit] == 1 ){
				passed = true;
				break;
			}
		}

		// which bin
		double pa = sqrt((*McTruth_px)[0]*(*McTruth_px)[0]+(*McTruth_py)[0]*(*McTruth_py)[0]+(*McTruth_pz)[0]*(*McTruth_pz)[0]);
		int ibin = (int)((pa-min_eff)/halfbin_eff/2 + 0.5);
		if (verbose >= Verbose_EventInfo ) std::cout<<prefix_EventInfo<<"pz = "<<(*McTruth_pz)[0]<<", pa = "<<pa<<", ibin = "<<ibin<<std::endl;

		// count
		if ( event_type == 1 ){
			ap_num[ibin]++;
			if (passed) ap_num_pass[ibin]++;
		}
		else if ( event_type == 2 ){
			mu_num[ibin]++;
			if (passed) mu_num_pass[ibin]++;
		}

		if (verbose >= Verbose_EventInfo ) std::cout<<prefix_EventInfo<<"Finished!"<<std::endl;
	}/* end of loop in events*/
	//=======================================================================================================
	//************FOR EFFICIENCY**********************
	for (int i = 0; i < nbin_eff; i++ ){
		double pa = min_eff + i*halfbin_eff*2;
		double mu_eff = mu_num[i]?(double)(mu_num_pass[i])/mu_num[i]:0;
		double ap_eff = ap_num[i]?(double)(ap_num_pass[i])/ap_num[i]:0;
		if ( mu_eff && !mu_thre ){
			mu_thre = pa;
			if (verbose >= Verbose_EffInfo) std::cout<<prefix_EffInfo<<"mu_thre = "<<mu_thre<<"MeV, mu_eff = "<<mu_eff<<std::endl;
		}
		if ( ap_eff && !ap_thre ){
			ap_thre = pa;
			if (verbose >= Verbose_EffInfo) std::cout<<prefix_EffInfo<<"ap_thre = "<<ap_thre<<"MeV, ap_eff = "<<ap_eff<<std::endl;
		}
		if ( (index_temp = get_TGraph("muon_rate_vs_pa")) != -1 ){
			xForGraph[index_temp].push_back(pa);
			yForGraph[index_temp].push_back(mu_eff);
		}
		if ( (index_temp = get_TGraph("rate_vs_pa")) != -1 ){
			xForGraph[index_temp].push_back(pa);
			yForGraph[index_temp].push_back(ap_eff);
		}
		if (verbose >= Verbose_EffInfo) std::cout<<prefix_EffInfo<<"pa = "<<pa
		                                                         <<", mu_num: "<<mu_num[i]
		                                                         <<", mu_num_pass: "<<mu_num_pass[i]
		                                                         <<", ap_num: "<<ap_num[i]
		                                                         <<", ap_num_pass: "<<ap_num_pass[i]
		                                                         <<std::endl;
	}

	//=======================================================================================================
	//************WRITE AND OUTPUT********************
	if (verbose >= Verbose_SectorInfo) std::cout<<prefix_SectorInfo<<"In WRITE AND OUTPUT ###"<<std::endl;
	std::string outputFileName = OutputDir + "output.root";
	TFile *file = new TFile(outputFileName.c_str(),"RECREATE");
	std::cout<<"N0 = "<<N0<<std::endl;
	std::cout<<"N1 = "<<N1<<std::endl;
	std::cout<<"N2 = "<<N2<<std::endl;
	std::cout<<"N3 = "<<N3<<std::endl;

	gStyle->SetPalette(1);
	gStyle->SetOptStat(0);
	gStyle->SetPadTickX(1);
	gStyle->SetPadTickY(1);
	//  gStyle->SetTitleW(0.99);
	//  gStyle->SetTitleH(0.08);
	//Output these histograms
	for ( int i = 0; i < vecH1D.size(); i++ ){
		if (verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<"Output vecH1D["<<i<<"]: "<<nameForH1D[i]<<", "<<titleForH1D[i]<<", "<<xNameForH1D[i]<<", "<<yNameForH1D[i]<<", "<<bin1ForH1D[i]<<", "<<left1ForH1D[i]<<", "<<right1ForH1D[i]<<", Color="<<colorForH1D[i]<<", xlogSyle="<<xlogForH1D[i]<<", ylogSyle="<<ylogForH1D[i]<<", nCompare="<<compareForH1D[i]<<", markerStyle="<<markerForH1D[i]<<", drawOpt=\""<<drawOptForH1D[i]<<"\""<<std::endl;
		vecH1D[i]->SetLineColor(colorForH1D[i]);
		TString name = vecH1D[i]->GetName();
		TCanvas* c = new TCanvas(name);
		int nCompare = compareForH1D[i];
		if ( nCompare ) if (verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<nCompare<<" histograms to be compared"<<std::endl;
		double currentMaximum = vecH1D[i]->GetMaximum();
		for ( int j = 1; j <= nCompare; j++ ){
			double maximum = vecH1D[i+j]->GetMaximum();
			if ( maximum > currentMaximum ){
				currentMaximum = maximum;
			}
		}
		double xmax = vecH1D[i]->GetXaxis()->GetXmax();
		if ( nCompare ) if (verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<"  maximum y value is ("<<currentMaximum<<")"<<std::endl;
		if ( xlogForH1D[i] ) gPad->SetLogx(1);
		else gPad->SetLogx(0);
		if ( ylogForH1D[i] ) gPad->SetLogy(1);
		else gPad->SetLogy(0);
		if ( xlogForH1D[i] ) vecH1D[i]->GetXaxis()->SetLimits(1e-6,2*xmax);
		else vecH1D[i]->GetXaxis()->SetLimits(0.,1.05*xmax);
		if ( ylogForH1D[i] ) vecH1D[i]->GetYaxis()->SetRangeUser(1e-6,2*currentMaximum);
		else vecH1D[i]->GetYaxis()->SetRangeUser(0.,1.05*currentMaximum);
		vecH1D[i]->SetMarkerStyle(markerForH1D[i]);
		vecH1D[i]->SetMarkerColor(colorForH1D[i]);
		vecH1D[i]->SetLineColor(colorForH1D[i]);
		vecH1D[i]->GetXaxis()->SetTitle(xNameForH1D[i]);
		vecH1D[i]->GetYaxis()->SetTitle(yNameForH1D[i]);
		vecH1D[i]->Draw(drawOptForH1D[i]);
		vecH1D[i]->Write();
		for ( int j = 0; j < nCompare; j++ ){
			i++;
			if (verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<" ->"<<j<<", vecH1D["<<i<<"]: "<<nameForH1D[i]<<", "<<titleForH1D[i]<<", "<<xNameForH1D[i]<<", "<<yNameForH1D[i]<<", "<<bin1ForH1D[i]<<", "<<left1ForH1D[i]<<", "<<right1ForH1D[i]<<", Color="<<colorForH1D[i]<<", xlogSyle="<<xlogForH1D[i]<<", ylogSyle="<<ylogForH1D[i]<<", nCompare="<<compareForH1D[i]<<", markerStyle="<<markerForH1D[i]<<", drawOpt=\""<<drawOptForH1D[i]<<"\""<<std::endl;
			vecH1D[i]->SetLineColor(colorForH1D[i]);
			vecH1D[i]->SetMarkerStyle(markerForH1D[i]);
			vecH1D[i]->SetMarkerColor(colorForH1D[i]);
			vecH1D[i]->SetLineColor(colorForH1D[i]);
			TString drawOpt = drawOptForH1D[i]+"SAME";
			vecH1D[i]->Draw(drawOpt);
		}
		TString fileName = OutputDir + name + ".pdf";
		c->Print(fileName);
	}
	gStyle->SetOptStat(0);
	for ( int i = 0; i < vecH2D.size(); i++ ){
		if (verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<"Output vecH2D["<<i<<"]: "<<nameForH2D[i]<<", "<<titleForH2D[i]<<", "<<xNameForH2D[i]<<", "<<yNameForH2D[i]<<", "<<bin1ForH2D[i]<<", "<<left1ForH2D[i]<<", "<<right1ForH2D[i]<<", "<<bin2ForH2D[i]<<", "<<left2ForH2D[i]<<", "<<right2ForH2D[i]<<std::endl;
		TString name = vecH2D[i]->GetName();
		TCanvas* c = new TCanvas(name);
		vecH2D[i]->GetXaxis()->SetTitle(xNameForH2D[i]);
		vecH2D[i]->GetYaxis()->SetTitle(yNameForH2D[i]);
		vecH2D[i]->Draw("COLZ");
		vecH2D[i]->Write();
		TString fileName = OutputDir + name + ".pdf";
		c->Print(fileName);
	}
	for ( int i = 0; i < nameForGraph.size(); i++ ){
		int sizeOfThisGraph = xForGraph[i].size();
		if (verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<"Output vecGraph["<<i<<"]: "<<nameForGraph[i]<<", "<<titleForGraph[i]<<", "<<xNameForGraph[i]<<", "<<yNameForGraph[i]<<", Color="<<colorForGraph[i]<<", xlogSyle="<<xlogForGraph[i]<<", ylogSyle="<<ylogForGraph[i]<<", nCompare="<<compareForGraph[i]<<", markerStyle="<<markerForGraph[i]<<", drawOpt=\""<<drawOptForGraph[i]<<"\", size = "<<sizeOfThisGraph<<std::endl;
		if ( sizeOfThisGraph <= 0 ) continue;
		if (verbose >= Verbose_HistInfo){
			for ( int j = 0; j < sizeOfThisGraph; j++ ){
				std::cout<<prefix_HistInfo<<"  ["<<j<<"]: ("<<xForGraph[i][j]<<","<<yForGraph[i][j]<<")"<<std::endl;
			}
		}
		TString name = nameForGraph[i];
		TCanvas* c = new TCanvas(nameForGraph[i]);
		TGraph *aTGraph = new TGraph(sizeOfThisGraph,&xForGraph[i][0],&yForGraph[i][0]);
		aTGraph->SetTitle(titleForGraph[i]);
		int nCompare = compareForGraph[i];
		if ( nCompare ) if (verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<nCompare<<" graphs to be compared"<<std::endl;
		std::vector<double> yforgraph = yForGraph[i];
		std::vector<double> xforgraph = xForGraph[i];
		double xmin = min_eff;
		double xmax = max_eff;
		double currentMaximum = *std::max_element(yforgraph.begin(),yforgraph.end());
		for ( int j = 1; j <= nCompare; j++ ){
			double maximum = *std::max_element(yForGraph[i+j].begin(),yForGraph[i+j].end());
			if ( maximum > currentMaximum ){
				currentMaximum = maximum;
			}
		}
		if ( nCompare ) if (verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<"  maximum y value is ("<<currentMaximum<<")"<<std::endl;
		if ( xlogForGraph[i] ) gPad->SetLogx(1);
		else gPad->SetLogx(0);
		if ( ylogForGraph[i] ) gPad->SetLogy(1);
		else gPad->SetLogy(0);
		if ( xlogForGraph[i] ) aTGraph->GetXaxis()->SetLimits(1e-6,2*xmax);
		else aTGraph->GetXaxis()->SetLimits(0.95*xmin,1.05*xmax);
		if ( ylogForGraph[i] ) aTGraph->GetYaxis()->SetRangeUser(1e-6,2*currentMaximum);
		else aTGraph->GetYaxis()->SetRangeUser(0.,1.05*currentMaximum);
		aTGraph->GetXaxis()->SetTitle(xNameForGraph[i]);
		aTGraph->GetYaxis()->SetTitle(yNameForGraph[i]);
		aTGraph->SetMarkerStyle(markerForGraph[i]);
		aTGraph->SetMarkerColor(colorForGraph[i]);
		aTGraph->SetLineColor(colorForGraph[i]);
		TString drawOpt = "A"+drawOptForGraph[i];
		aTGraph->Draw(drawOpt);
		aTGraph->Write();
		for ( int j = 0; j < nCompare; j++ ){
			i++;
			int sizeOfThisGraph = xForGraph[i].size();
			if (verbose >= Verbose_HistInfo) std::cout<<prefix_HistInfo<<" ->"<<j<<", vecGraph["<<i<<"]: "<<nameForGraph[i]<<", "<<titleForGraph[i]<<", "<<xNameForGraph[i]<<", "<<yNameForGraph[i]<<", Color="<<colorForGraph[i]<<", xlogSyle="<<xlogForGraph[i]<<", ylogSyle="<<ylogForGraph[i]<<", nCompare="<<compareForGraph[i]<<", markerStyle="<<markerForGraph[i]<<", drawOpt=\""<<drawOptForGraph[i]<<"\", size = "<<sizeOfThisGraph<<std::endl;
			if ( sizeOfThisGraph <= 0 ) continue;
			if (verbose >= Verbose_HistInfo){
				for ( int k = 0; k < sizeOfThisGraph; k++ ){
					std::cout<<prefix_HistInfo<<"  ["<<k<<"]: ("<<xForGraph[i][k]<<","<<yForGraph[i][k]<<")"<<std::endl;
				}
			}
			TGraph *bTGraph = new TGraph(sizeOfThisGraph,&xForGraph[i][0],&yForGraph[i][0]);
			bTGraph->SetTitle(titleForGraph[i]);
			bTGraph->GetXaxis()->SetTitle(xNameForGraph[i]);
			bTGraph->GetYaxis()->SetTitle(yNameForGraph[i]);
			bTGraph->SetLineColor(colorForGraph[i]);
			bTGraph->SetMarkerStyle(markerForGraph[i]);
			bTGraph->SetMarkerColor(colorForGraph[i]);
			bTGraph->SetLineColor(colorForGraph[i]);
			bTGraph->Draw(drawOptForGraph[i]);
			bTGraph->Write();
		}
		TString fileName = OutputDir + name + ".pdf";
		c->Print(fileName);
	}

	//TTree *m_TTree = m_TChain->CloneTree();
	//m_TTree->Write();
	//file->Write();
	file->Close();

	delete file;
	return 0;
}

TH2D* get_TH2D(std::string name){
	for ( int i = 0; i < vecH2D.size(); i++ ){
		if ( nameForH2D[i] == name ) return vecH2D[i];
	}
	std::cout<<"###!!!In get_TH2D: CAN NOT FIND "<<name<<"!!!"<<std::endl;
	return 0;
}

TH1D* get_TH1D(std::string name){
	for ( int i = 0; i < vecH1D.size(); i++ ){
		if ( nameForH1D[i] == name ) return vecH1D[i];
	}
	std::cout<<"###!!!In get_TH1D: CAN NOT FIND "<<name<<"!!!"<<std::endl;
	return 0;
}

int get_TGraph(std::string name){
	for ( int i = 0; i < nameForGraph.size(); i++ ){
		if ( nameForGraph[i] == name ) return i;
	}
	std::cout<<"###!!!In get_TGraph: CAN NOT FIND "<<name<<"!!!"<<std::endl;
	return -1;
}

bool ISEMPTY(std::string content){
	bool flag = false;
	const char* c_card = content.c_str();
	int length = strlen(c_card);
	int offset = 0;
	for ( ; offset < length; offset++ ){
		if ( c_card[offset] != ' ' ) break;
	}
	if ( c_card[offset] == '#' || (c_card[offset] == '/' && c_card[offset+1] == '/') || length - offset == 0 ){
		flag = true;
	}
	return flag;
}
