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

int cellNoIntegral[18] = {198,402,612,828,1050,1278,1512,1752,1998,2250,2508,2772,3042,3318,3600,3888,4182,4482};

int get_cell_layer(int volID,int &cell,int &layer){
	int i = 0;
	for (; i< 18; i++){
		if (volID<cellNoIntegral[i]) break;
	}
	layer = i;
	cell = volID;
	if (i>0) cell -= cellNoIntegral[i-1];

	return 0;
}

void gettopo(int &topo,std::vector<std::string> * process,std::vector<int> * pid,std::vector<double> *o_px,std::vector<double> *o_py,std::vector<double> *o_pz){
	if (pid->size()<=1||(*pid)[pid->size()-1]!=13) topo = 0; // primary or not from muon
	else{ // from muon
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
	return;
}

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
//int tri(){
	std::stringstream buff;
	// About this run
//	TString parName = "n0";
	TString parName = "OTWC";
//	TString suffixName = "0508_100cm_1e7";
	TString suffixName = "0508_100cm_1e9";
	TString runName = parName+"."+suffixName;
	std::vector<TString> DirName;
	std::vector<int> nRuns;
	std::vector<TString> FileNames;
	 // ########Should Modify#########
//	FileNames.push_back(MyData+"/n0.root");
//	FileNames.push_back(runName+".root");
	//DirName.push_back("/scratchfs/bes/wuc/MyWorkArea/Data/raw/g4sim/BLTCDC.em.g60cm10mm.005T.1p5_0927_11_p5.g4s.QBH");
	//nRuns.push_back(100);
	//DirName.push_back("/scratchfs/bes/wuc/MyWorkArea/Data/raw/g4sim/BLTCDC.OT.g60cm10mm.005T.1p5_0927_11_p5.g4s.QBH");
	//nRuns.push_back(100);
	//DirName.push_back("/scratchfs/bes/wuc/MyWorkArea/Data/raw/g4sim/BLTCDC.mum.g60cm10mm.005T.1p5_0927_11_p5.g4s.QBH");
	//nRuns.push_back(100);
	DirName.push_back(MyData+"/CDCHit."+parName+".g60cm10mm.005T."+suffixName+".g4s.QBH");
	nRuns.push_back(100);
//	DirName.push_back(MyData+"/raw/g4sim/CDCHit.pim.g60cm10mm.005T.0508.g4s.QBH");
//	nRuns.push_back(50);
	double nProtons = 1e9;
	if (parName == "pim" || parName == "pimWC")
		nProtons *= 10;
	else if (parName == "n0")
		nProtons *= 0.9;
//	nProtons*=19./20;
	 // ########Should Modify#########

	// input
	int  C_nHits = 0;
	int  M_nHits = 0;

	int  evt_num;
	int  run_num;
	double weight;

	std::vector<int>     *C_volID = 0;
	std::vector<int>     *C_ptid = 0;
	std::vector<double>  *C_edep = 0;
	std::vector<double>  *C_stepL = 0;
	std::vector<double>  *C_t = 0;
	std::vector<double>  *C_px = 0;
	std::vector<double>  *C_py = 0;
	std::vector<double>  *C_pz = 0;
	std::vector<double>  *C_x = 0;
	std::vector<double>  *C_y = 0;
	std::vector<double>  *C_z = 0;
	std::vector<int>  *C_pid = 0;
	std::vector<int>  *C_ppid = 0;
	std::vector<std::string>  *C_oprocess = 0;
	std::vector<std::string>  *C_ovolName = 0;

	std::vector<int>     *M_volID = 0;
	std::vector<double>     *M_opx = 0;
	std::vector<double>  *M_edep = 0;
	std::vector<double>  *M_stepL = 0;
	std::vector<double>  *M_t = 0;
	std::vector<double>  *M_px = 0;
	std::vector<double>  *M_py = 0;
	std::vector<double>  *M_pz = 0;
	std::vector<double>  *M_x = 0;
	std::vector<double>  *M_y = 0;
	std::vector<double>  *M_z = 0;
	std::vector<std::string>  *M_volName = 0;
	std::vector<int>  *M_pid = 0;
	std::vector<int>  *M_ppid = 0;
	std::vector<std::string>  *M_oprocess = 0;
	std::vector<std::string>  *M_ovolName = 0;

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
	//double dR0, dR1;
	//bool doubleRandom = false;
	//	std::string title = c->GetBranch("R1")->GetTitle();
	//	if ((title).substr(0,4)=="R1/D"){
	//		doubleRandom = true;
	//	}
	//	if (doubleRandom){ c->SetBranchAddress("R0",&dR0);
	//		c->SetBranchAddress("R1",&dR1);
	//	}
	//	else{
	//		c->SetBranchAddress("R0",&R0);
	//		c->SetBranchAddress("R1",&R1);
	//	}

	int set_C_setpL = 1;
	int set_M_setpL = 1;
	c->SetBranchAddress("C_nHits",&C_nHits);
	c->SetBranchAddress("C_volID",&C_volID);
	c->SetBranchAddress("C_ptid",&C_ptid);
	c->SetBranchAddress("C_edep",&C_edep);
	set_C_setpL=c->SetBranchAddress("C_stepL",&C_stepL);
	c->SetBranchAddress("C_t",&C_t);
	c->SetBranchAddress("C_px",&C_px);
	c->SetBranchAddress("C_py",&C_py);
	c->SetBranchAddress("C_pz",&C_pz);
	c->SetBranchAddress("C_x",&C_x);
	c->SetBranchAddress("C_y",&C_y);
	c->SetBranchAddress("C_z",&C_z);
	c->SetBranchAddress("C_pid",&C_pid);
	c->SetBranchAddress("C_ppid",&C_ppid);
	c->SetBranchAddress("C_oprocess",&C_oprocess);
	c->SetBranchAddress("C_ovolName",&C_ovolName);

	c->SetBranchAddress("M_nHits",&M_nHits);
	c->SetBranchAddress("M_volName",&M_volName);
	c->SetBranchAddress("M_volID",&M_volID);
	c->SetBranchAddress("M_opx",&M_opx);
	c->SetBranchAddress("M_edep",&M_edep);
	set_M_setpL=c->SetBranchAddress("M_stepL",&M_stepL);
	c->SetBranchAddress("M_t",&M_t);
	c->SetBranchAddress("M_px",&M_px);
	c->SetBranchAddress("M_py",&M_py);
	c->SetBranchAddress("M_pz",&M_pz);
	c->SetBranchAddress("M_x",&M_x);
	c->SetBranchAddress("M_y",&M_y);
	c->SetBranchAddress("M_z",&M_z);
	c->SetBranchAddress("M_pid",&M_pid);
	c->SetBranchAddress("M_ppid",&M_ppid);
	c->SetBranchAddress("M_oprocess",&M_oprocess);
	c->SetBranchAddress("M_ovolName",&M_ovolName);

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
	c->SetBranchAddress("weight",&weight);

	// output
	TFile * f = new TFile(runName+".root","RECREATE");
	TTree *tree  = new TTree("tree","tree");

	int nHits;
	int type; 
	int cpid; 
	int topo;

	std::vector<int> *O_cellID = 0;
	std::vector<int> *O_layerID = 0;
	std::vector<int> *O_triType = 0;
	std::vector<int> *O_triID = 0;
	std::vector<int> *O_triPos = 0;

	std::vector<double> *O_edep = 0;
	std::vector<double> *O_stepL = 0;
	std::vector<double> *O_t = 0;
	std::vector<double> *O_px = 0;
	std::vector<double> *O_py = 0;
	std::vector<double> *O_pz = 0;
	std::vector<double> *O_x = 0;
	std::vector<double> *O_y = 0;
	std::vector<double> *O_z = 0;

	std::vector<double> *o_t = 0;
	std::vector<double> *o_px = 0;
	std::vector<double> *o_py = 0;
	std::vector<double> *o_pz = 0;
	std::vector<double> *o_x = 0;
	std::vector<double> *o_y = 0;
	std::vector<double> *o_z = 0;

	std::vector<std::string> *process = 0;
	std::vector<std::string> *volume = 0;
	std::vector<int> *pid = 0;
	std::vector<int> *ppid = 0;

	tree->Branch("evt_num",&evt_num);
	tree->Branch("run_num",&run_num);
	tree->Branch("nHits",&nHits);
	tree->Branch("type",&type);
	tree->Branch("topo",&topo);
	tree->Branch("cpid",&cpid);
	tree->Branch("weight",&weight);

	tree->Branch("O_cellID",&O_cellID);
	tree->Branch("O_layerID",&O_layerID);
	tree->Branch("O_triID",&O_triID);
	tree->Branch("O_triType",&O_triType);
	tree->Branch("O_triPos",&O_triPos);

	tree->Branch("O_edep",&O_edep);
	tree->Branch("O_stepL",&O_stepL);
	tree->Branch("O_t",&O_t);
	tree->Branch("O_px",&O_px);
	tree->Branch("O_py",&O_py);
	tree->Branch("O_pz",&O_pz);
	tree->Branch("O_x",&O_x);
	tree->Branch("O_y",&O_y);
	tree->Branch("O_z",&O_z);

	tree->Branch("t",&o_t);
	tree->Branch("px",&o_px);
	tree->Branch("py",&o_py);
	tree->Branch("pz",&o_pz);
	tree->Branch("x",&o_x);
	tree->Branch("y",&o_y);
	tree->Branch("z",&o_z);
	tree->Branch("process",&process);
	tree->Branch("volume",&volume);
	tree->Branch("pid",&pid);
	tree->Branch("ppid",&ppid);

	// Loop in events
	Long64_t nEvents = c->GetEntries();
	std::cout<<"nEvents = "<<nEvents<<std::endl;
	int printModulo = 1e4;
	int printModulo2 = printModulo;
	for (Long64_t iEvent = 0; iEvent < nEvents; iEvent++ ){
		if (iEvent%printModulo==0) std::cout<<(double)iEvent/nEvents*100<<" % ..."<<std::endl;
		c->GetEntry(iEvent);
		if (C_nHits>0){
			nHits = 0;
			type = 0; 

			if(O_cellID) delete O_cellID; O_cellID = new std::vector<int>;
			if(O_layerID) delete O_layerID; O_layerID = new std::vector<int>;
			if(O_triType) delete O_triType; O_triType = new std::vector<int>;
			if(O_triID) delete O_triID; O_triID = new std::vector<int>;
			if(O_triPos) delete O_triPos; O_triPos = new std::vector<int>;

			if(O_edep) delete O_edep; O_edep = new std::vector<double>;
			if(O_stepL) delete O_stepL; O_stepL = new std::vector<double>;
			if(O_t) delete O_t; O_t = new std::vector<double>;
			if(O_px) delete O_px; O_px = new std::vector<double>;
			if(O_py) delete O_py; O_py = new std::vector<double>;
			if(O_pz) delete O_pz; O_pz = new std::vector<double>;
			if(O_x) delete O_x; O_x = new std::vector<double>;
			if(O_y) delete O_y; O_y = new std::vector<double>;
			if(O_z) delete O_z; O_z = new std::vector<double>;

			if(o_t) delete o_t; o_t = new std::vector<double>;
			if(o_px) delete o_px; o_px = new std::vector<double>;
			if(o_py) delete o_py; o_py = new std::vector<double>;
			if(o_pz) delete o_pz; o_pz = new std::vector<double>;
			if(o_x) delete o_x; o_x = new std::vector<double>;
			if(o_y) delete o_y; o_y = new std::vector<double>;
			if(o_z) delete o_z; o_z = new std::vector<double>;

			if (process) delete process; process = new std::vector<std::string>;
			if (volume) delete volume; volume = new std::vector<std::string>;
			if (pid) delete pid; pid = new std::vector<int>;
			if (ppid) delete ppid; ppid = new std::vector<int>;

			int pretid = -1;
			int prepid = -1;
			std::string preprocess = "";
			std::string prevolume = "";
			if (iEvent%printModulo==0) std::cout<<"==> Start looping in CDC hits, nHits = "<<C_nHits<<std::endl;
			for ( int iHit = 0; iHit<C_nHits;iHit++){
				if (iEvent%printModulo2==0) std::cout<<"	# "<<iHit
					                                <<": "<<(*C_pid)[iHit]
					                                <<","<<(*C_ptid)[iHit]
					                                <<","<<(*C_px)[iHit]*1e3
					                                <<","<<(*C_oprocess)[iHit]
					                                <<","<<(*C_ovolName)[iHit]
					                                <<std::endl;
				int tptid = (*C_ptid)[iHit];
				if (tptid != pretid || (*C_oprocess)[iHit] != preprocess || (*C_ovolName)[iHit] != prevolume || (*C_pid)[iHit] != prepid ){ // new track
					if (iEvent%printModulo2==0) std::cout<<"		Found new track!"<<std::endl;
					if (iHit!=0){
						tree->Fill();
						if (iEvent%printModulo2==0) std::cout<<"			Fill previous one!"<<std::endl;
					}

					cpid = (*C_pid)[iHit];
					prepid = (*C_pid)[iHit];
					pretid = tptid;
					preprocess = (*C_oprocess)[iHit];
					prevolume = (*C_ovolName)[iHit];
					nHits = 0;

					if(O_cellID) delete O_cellID; O_cellID = new std::vector<int>;
					if(O_layerID) delete O_layerID; O_layerID = new std::vector<int>;
					if(O_triType) delete O_triType; O_triType = new std::vector<int>;
					if(O_triID) delete O_triID; O_triID = new std::vector<int>;
					if(O_triPos) delete O_triPos; O_triPos = new std::vector<int>;

					if(O_edep) delete O_edep; O_edep = new std::vector<double>;
					if(O_stepL) delete O_stepL; O_stepL = new std::vector<double>;
					if(O_t) delete O_t; O_t = new std::vector<double>;
					if(O_px) delete O_px; O_px = new std::vector<double>;
					if(O_py) delete O_py; O_py = new std::vector<double>;
					if(O_pz) delete O_pz; O_pz = new std::vector<double>;
					if(O_x) delete O_x; O_x = new std::vector<double>;
					if(O_y) delete O_y; O_y = new std::vector<double>;
					if(O_z) delete O_z; O_z = new std::vector<double>;

					if(o_t) delete o_t; o_t = new std::vector<double>;
					if(o_px) delete o_px; o_px = new std::vector<double>;
					if(o_py) delete o_py; o_py = new std::vector<double>;
					if(o_pz) delete o_pz; o_pz = new std::vector<double>;
					if(o_x) delete o_x; o_x = new std::vector<double>;
					if(o_y) delete o_y; o_y = new std::vector<double>;
					if(o_z) delete o_z; o_z = new std::vector<double>;

					if (process) delete process; process = new std::vector<std::string>;
					if (volume) delete volume; volume = new std::vector<std::string>;
					if (pid) delete pid; pid = new std::vector<int>;
					if (ppid) delete ppid; ppid = new std::vector<int>;
					if (iEvent%printModulo2==0) std::cout<<"			Vectors clear!"<<std::endl;

					int maxdepth = 1;
					int idepth = 0;
					if (iEvent%printModulo2==0) std::cout<<"			==> Start looping in McTruth, nMc = "<<McTruth_pid->size()<<std::endl;
					bool foundfirst = false;
					for(int iMc = McTruth_pid->size()-1; iMc>=0; iMc--){
						bool isfirst=false;
						if (!foundfirst){
							if (ppid->size()==0){
								double pa_temp1 = sqrt((*C_px)[iHit]*(*C_px)[iHit]+(*C_py)[iHit]*(*C_py)[iHit]+(*C_pz)[iHit]*(*C_pz)[iHit]);
								double pa_temp2 = sqrt((*McTruth_px)[iMc]*(*McTruth_px)[iMc]+(*McTruth_py)[iMc]*(*McTruth_py)[iMc]+(*McTruth_pz)[iMc]*(*McTruth_pz)[iMc]);
								if( tptid==(*McTruth_ptid)[iMc] // same mother
										&&(*C_pid)[iHit]==(*McTruth_pid)[iMc] // same pid
										&&(*C_ovolName)[iHit]==(*McTruth_volume)[iMc] // from same volume
										&&(*C_oprocess)[iHit]==(*McTruth_process)[iMc] // from same process
										&&pa_temp2>=pa_temp1){
									isfirst=true;
									foundfirst = true;
								}
							}
						}
						if(isfirst||(foundfirst&&tptid==(*McTruth_tid)[iMc])){
//							if (++idepth>maxdepth) break;
							ppid->push_back((*McTruth_ppid)[iMc]);
							pid->push_back((*McTruth_pid)[iMc]);
							process->push_back((*McTruth_process)[iMc]);
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
							tptid = (*McTruth_ptid)[iMc];
							if (iEvent%printModulo2==0) std::cout<<"				=>"<<iMc
								                                                       <<","<<idepth
								                                                       <<":"<<(*pid)[pid->size()-1]
								                                                       <<","<<tptid
								                                                       <<","<<(*o_px)[pid->size()-1]
								                                                       <<","<<(*volume)[pid->size()-1]
								                                                       <<","<<(*process)[pid->size()-1]
								                                                       <<std::endl;
						}
					}
					if (!foundfirst) topo = -1;
					else
						gettopo(topo,process,pid,o_px,o_py,o_pz);
					if (iEvent%printModulo2==0) std::cout<<"			<== End looping in McTruth, nMc = "<<McTruth_pid->size()<<std::endl;
				}
				nHits++;
				int layer, cell;
				get_cell_layer((*C_volID)[iHit],cell,layer);
				O_layerID->push_back(layer);
				O_cellID->push_back(cell);
				O_edep->push_back((*C_edep)[iHit]);
				if (!set_C_setpL) O_stepL->push_back((*C_stepL)[iHit]);
				else O_stepL->push_back(0);
				O_t->push_back((*C_t)[iHit]);
				O_px->push_back((*C_px)[iHit]*1000);
				O_py->push_back((*C_py)[iHit]*1000);
				O_pz->push_back((*C_pz)[iHit]*1000);
				O_x->push_back((*C_x)[iHit]*10);
				O_y->push_back((*C_y)[iHit]*10);
				O_z->push_back((*C_z)[iHit]*10);

				if (iHit==C_nHits-1){
					tree->Fill();
					if (iEvent%printModulo2==0) std::cout<<"			Last hit! Fill previous one!"<<std::endl;
				}
			}
			if (iEvent%printModulo==0) std::cout<<"<== End looping in CDC hits"<<std::endl;
		}
		if (M_nHits>0){
			nHits = 0;
			type = 1;

			if(O_cellID) delete O_cellID; O_cellID = new std::vector<int>;
			if(O_layerID) delete O_layerID; O_layerID = new std::vector<int>;
			if(O_triType) delete O_triType; O_triType = new std::vector<int>;
			if(O_triID) delete O_triID; O_triID = new std::vector<int>;
			if(O_triPos) delete O_triPos; O_triPos = new std::vector<int>;

			if(O_edep) delete O_edep; O_edep = new std::vector<double>;
			if(O_stepL) delete O_stepL; O_stepL = new std::vector<double>;
			if(O_t) delete O_t; O_t = new std::vector<double>;
			if(O_px) delete O_px; O_px = new std::vector<double>;
			if(O_py) delete O_py; O_py = new std::vector<double>;
			if(O_pz) delete O_pz; O_pz = new std::vector<double>;
			if(O_x) delete O_x; O_x = new std::vector<double>;
			if(O_y) delete O_y; O_y = new std::vector<double>;
			if(O_z) delete O_z; O_z = new std::vector<double>;

			if(o_t) delete o_t; o_t = new std::vector<double>;
			if(o_px) delete o_px; o_px = new std::vector<double>;
			if(o_py) delete o_py; o_py = new std::vector<double>;
			if(o_pz) delete o_pz; o_pz = new std::vector<double>;
			if(o_x) delete o_x; o_x = new std::vector<double>;
			if(o_y) delete o_y; o_y = new std::vector<double>;
			if(o_z) delete o_z; o_z = new std::vector<double>;

			if (process ) delete process ; process = new std::vector<std::string>;
			if (volume) delete volume; volume = new std::vector<std::string>;
			if (pid) delete pid; pid = new std::vector<int>;
			if (ppid) delete ppid; ppid = new std::vector<int>;

			int pretid = -1;
			double preopx = 1e9;
			if (iEvent%printModulo==0) std::cout<<"==> Start looping in M hits, nHits = "<<M_nHits<<std::endl;
			for ( int iHit = 0; iHit<M_nHits;iHit++){
				double topx = (*M_opx)[iHit];
				int tptid = -1;
				if (topx!=preopx){ // new track
					if (iEvent%printModulo==0) std::cout<<"		Found new track!"<<std::endl;
					if (iHit!=0) tree->Fill();

					cpid = (*M_pid)[iHit];
					pretid = tptid;
					nHits = 0;

					if(O_cellID) delete O_cellID; O_cellID = new std::vector<int>;
					if(O_layerID) delete O_layerID; O_layerID = new std::vector<int>;
					if(O_triType) delete O_triType; O_triType = new std::vector<int>;
					if(O_triID) delete O_triID; O_triID = new std::vector<int>;
					if(O_triPos) delete O_triPos; O_triPos = new std::vector<int>;

					if(O_edep) delete O_edep; O_edep = new std::vector<double>;
					if(O_stepL) delete O_stepL; O_stepL = new std::vector<double>;
					if(O_t) delete O_t; O_t = new std::vector<double>;
					if(O_px) delete O_px; O_px = new std::vector<double>;
					if(O_py) delete O_py; O_py = new std::vector<double>;
					if(O_pz) delete O_pz; O_pz = new std::vector<double>;
					if(O_x) delete O_x; O_x = new std::vector<double>;
					if(O_y) delete O_y; O_y = new std::vector<double>;
					if(O_z) delete O_z; O_z = new std::vector<double>;

					if(o_t) delete o_t; o_t = new std::vector<double>;
					if(o_px) delete o_px; o_px = new std::vector<double>;
					if(o_py) delete o_py; o_py = new std::vector<double>;
					if(o_pz) delete o_pz; o_pz = new std::vector<double>;
					if(o_x) delete o_x; o_x = new std::vector<double>;
					if(o_y) delete o_y; o_y = new std::vector<double>;
					if(o_z) delete o_z; o_z = new std::vector<double>;

					if (process) delete process; process = new std::vector<std::string>;
					if (volume) delete volume; volume = new std::vector<std::string>;
					if (pid) delete pid; pid = new std::vector<int>;
					if (ppid) delete ppid; ppid = new std::vector<int>;

					for(int iMc = McTruth_pid->size()-1; iMc>=0; iMc--){
						if (tptid == -1){
							if (fabs((topx-(*McTruth_px)[iMc])/(topx+(*McTruth_px)[iMc]))<1e6){
								tptid = (*McTruth_tid)[iMc];
							}
						}
						if (tptid==(*McTruth_tid)[iMc]){
							ppid->push_back((*McTruth_ppid)[iMc]);
							pid->push_back((*McTruth_pid)[iMc]);
							process->push_back((*McTruth_process)[iMc]);
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
							tptid = (*McTruth_ptid)[iMc];
						}
					}
					gettopo(topo,process,pid,o_px,o_py,o_pz);
				}
				nHits++;
				int triType,triPos;
				if ((*M_volName)[iHit]=="TriCheU"){
					triType = 0;
					triPos = 1;
				}
				else if ((*M_volName)[iHit]=="TriCheD"){
					triType = 0;
					triPos = -1;
				}
				else if ((*M_volName)[iHit]=="TriSciU"){
					triType = 1;
					triPos = 1;
				}
				else if ((*M_volName)[iHit]=="TriSciD"){
					triType = 1;
					triPos = -1;
				}
				O_triType->push_back(triType);
				O_triPos->push_back(triPos);
				O_triID->push_back((*M_volID)[iHit]);
				O_edep->push_back((*M_edep)[iHit]);
				if (iEvent%printModulo==0) std::cout<<"O_edep->push_back("<<(*M_edep)[iHit]<<")"<<std::endl;
				if (!set_C_setpL) O_stepL->push_back((*M_stepL)[iHit]);
				else O_stepL->push_back(0);
				O_t->push_back((*M_t)[iHit]);
				O_px->push_back((*M_px)[iHit]*1000);
				O_py->push_back((*M_py)[iHit]*1000);
				O_pz->push_back((*M_pz)[iHit]*1000);
				O_x->push_back((*M_x)[iHit]*10);
				O_y->push_back((*M_y)[iHit]*10);
				O_z->push_back((*M_z)[iHit]*10);

//				o_t->push_back((*M_t)[iHit]);
//				o_px->push_back((*M_px)[iHit]*1000);
//				o_py->push_back((*M_py)[iHit]*1000);
//				o_pz->push_back((*M_pz)[iHit]*1000);
//				o_x->push_back((*M_x)[iHit]*10);
//				o_y->push_back((*M_y)[iHit]*10);
//				o_z->push_back((*M_z)[iHit]*10);
				if (iHit==M_nHits-1) tree->Fill();
			}
			if (iEvent%printModulo==0) std::cout<<"<== End looping in M hits"<<std::endl;
		}
	}
	tree->Write();
	f->Close();
	return 0;
}
