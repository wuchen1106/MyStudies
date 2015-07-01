#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <math.h>
#include <string>
#include <vector>
#include <algorithm>

#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TMath.h"

std::string m_workMode;
std::string m_prefix;
std::string m_suffix;
std::string m_MonitorPlane;
std::string m_runName;
std::string m_input;
std::string m_OutputDir;
std::string m_InputDir;
std::string m_OriginalFile;
int m_beginNo = -1;
int m_totalNo = -1;
int verbose = 0;
int nEvents = 0;
int printModule = 1;
int writeModule = 10000;
int UseWeight = 0;
int PDGEncoding = 13;
double m_norm = 0;
bool backup = false;
std::string CdcFile = "";
std::vector<int> Ncut;
std::vector<std::string> Ncut_message;

void init_args();
void print_usage(char* prog_name);

int main(int argc, char* argv[]){

	clock_t t_START = clock();

	//=======================================
	//*************read parameter**********
	init_args();
	int result;
	while((result=getopt(argc,argv,"hb:t:v:n:N:m:M:r:D:O:d:p:P:w:W:x:y:i:C:"))!=-1){
		switch(result){
			/* INPUTS */
			case 'm':
				m_workMode = optarg;
				printf("Work Mode: %s\n",m_workMode.c_str());
				break;
			case 'x':
				m_prefix = optarg;
				printf("m_prefix: %s\n",m_prefix.c_str());
				break;
			case 'y':
				m_suffix = optarg;
				printf("m_suffix: %s\n",m_suffix.c_str());
				break;
			case 'M':
				m_MonitorPlane = optarg;
				printf("Monitor plane: %s\n",m_MonitorPlane.c_str());
				break;
			case 'r':
				m_runName=optarg;
				printf("runName: %s\n",m_runName.c_str());
				break;
			case 'i':
				m_input=optarg;
				printf("input configuration file: %s\n",m_input.c_str());
				break;
			case 'D':
				m_InputDir=optarg;
				printf("Input Directroy: %s\n",m_InputDir.c_str());
				break;
			case 'O':
				m_OriginalFile=optarg;
				printf("OriginalFile: %s\n",m_OriginalFile.c_str());
				break;
			case 'd':
				m_OutputDir=optarg;
				printf("Output Directroy: %s\n",m_OutputDir.c_str());
				break;
			case 'v':
				verbose = atoi(optarg);
				printf("verbose level: %d\n",verbose);
				break;
			case 'b':
				m_beginNo = atoi(optarg);
				printf("CPU index starts from%d\n",m_beginNo);
				break;
			case 't':
				m_totalNo = atoi(optarg);
				printf("Total CPU numbers%d\n",m_totalNo);
				break;
			case 'N':
				m_norm = atof(optarg);
				printf("norm: %lf\n",m_norm);
				break;
			case 'n':
				nEvents = atoi(optarg);
				printf("nEvent: %d\n",nEvents);
				break;
			case 'p':
				printModule = atoi(optarg);
				printf("printModule: %d\n",printModule);
				break;
			case 'P':
				PDGEncoding = atoi(optarg);
				printf("PDGEncoding: %d\n",PDGEncoding);
				break;
			case 'w':
				writeModule = atoi(optarg);
				printf("writeModule: %d\n",writeModule);
				break;
			case 'W':
				UseWeight = atoi(optarg);
				printf("UseWeight : %d\n",UseWeight);
				break;
			case 'C':
				CdcFile = optarg;
				printf("CdcFile: %s\n",CdcFile.c_str());
				break;
			case '?':
				printf("Wrong option! optopt=%c, optarg=%s\n", optopt, optarg);
				break;
			case 'h':
			default:
				print_usage(argv[0]);
				return 1;
		}
	}

	//=======================================
	//************Verbose Control***********
	int Verbose_SectorInfo = 5; //å¤§æ¦çæµç¨æåµ
	std::string prefix_SectorInfo = "### ";
	int Verbose_HistInfo = 10; //æåªäºhist,ä»ä¹æ¶åè¾åºäºï¼åæ°å¦ä½
	std::string prefix_HistInfo= "  [Histograms] ";
	int Verbose_Statistics = 10; //è·ç»è®¡ç¸å³ç(æçï¼åè¾¨ï¼ç²å­é´å«çæåµ)
	std::string prefix_Statistics="  [Statistics] ";
	int Verbose_FileInfo = 10; //æåªäºFileList,é½æå¤å°file
	std::string prefix_FileInfo="  [FileInfo] ";
	int Verbose_EffInfo = 15; //Efficiency info
	std::string prefix_EffInfo="  [EffInfo] ";
	int Verbose_EventInfo = 20; //æ¯ä¸ªeventçåºæ¬æµç¨
	std::string prefix_EventInfoStart="    =>[EventInfo] ";
	std::string prefix_EventInfo="      [EventInfo] ";
	int Verbose_ParticleInfo=25; //æ¯ä¸ªparticleçåºæ¬ä¿¡æ¯
	std::string prefix_ParticleInfoStart="    ->[ParticleInfo]";
	std::string prefix_ParticleInfo="      [ParticleInfo]";

	//=>About Constant
	double PI = 3.141592653589793238;
	double FSC = 1/137.03599911; //fine structure constant
	double M_PION = 0.139570;
	double M_MUON = 105.6584; //mass of muon in MeV
	double M_ELE = 0.510999; //mass of electron in MeV
	double M_U = 931.494061; //atomic mass unit in MeV
	double M_p = 0.9382723;  // mass of proton// proton mass unit in GeV
	double tau = 26; // mean life time of pion

	//************SET Statistics********************
	double TriTimeSci[128];
	double TriTimeGap[128];
	double TriTimeChe[128];

	// MonitorSD
	int Mc_nTracks = 0;
	std::vector<int> *Mc_tid = 0;
	std::vector<int> *Mc_pid = 0;
	std::vector<std::string> *Mc_process = 0;
	std::vector<std::string> *Mc_volume = 0;
	std::vector<double> *Mc_px = 0;
	std::vector<double> *Mc_py = 0;
	std::vector<double> *Mc_pz = 0;
	int C_nHits = 0;
	std::vector<int> *C_tid = 0;
	std::vector<double> *C_t = 0;
	std::vector<double> *C_edep = 0;
	std::vector<int>    *C_layerID = 0;
	int M_nHits = 0;
	std::vector<int> *M_tid = 0;
	std::vector<double> *M_t = 0;
	std::vector<double> *M_edep = 0;
	std::vector<double> *M_px = 0;
	std::vector<double> *M_py = 0;
	std::vector<double> *M_pz = 0;
	std::vector<int>    *M_volID = 0;
	std::vector<std::string>    *M_volName = 0;

	//***********************************If We Need Original File***********************************
	TChain* m_TChain = new TChain("tree");
	m_TChain->Add(m_OriginalFile.c_str());
	m_TChain->SetBranchAddress("CdcCell_nHits",&C_nHits);
	m_TChain->SetBranchAddress("CdcCell_t",&C_t);
	m_TChain->SetBranchAddress("CdcCell_edep",&C_edep);
	m_TChain->SetBranchAddress("CdcCell_layerID",&C_layerID);
	m_TChain->SetBranchAddress("CdcCell_tid",&C_tid);
	m_TChain->SetBranchAddress("M_nHits",&M_nHits);
	m_TChain->SetBranchAddress("M_t",&M_t);
	m_TChain->SetBranchAddress("M_px",&M_px);
	m_TChain->SetBranchAddress("M_py",&M_py);
	m_TChain->SetBranchAddress("M_pz",&M_pz);
	m_TChain->SetBranchAddress("M_edep",&M_edep);
	m_TChain->SetBranchAddress("M_volID",&M_volID);
	m_TChain->SetBranchAddress("M_volName",&M_volName);
	m_TChain->SetBranchAddress("M_tid",&M_tid);
	m_TChain->SetBranchAddress("Mc_nTracks",&Mc_nTracks);
	m_TChain->SetBranchAddress("Mc_pid",&Mc_pid);
	m_TChain->SetBranchAddress("Mc_tid",&Mc_tid);
	m_TChain->SetBranchAddress("Mc_process",&Mc_process);
	m_TChain->SetBranchAddress("Mc_volume",&Mc_volume);
	m_TChain->SetBranchAddress("Mc_px",&Mc_px);
	m_TChain->SetBranchAddress("Mc_py",&Mc_py);
	m_TChain->SetBranchAddress("Mc_pz",&Mc_pz);

	TFile * ofile = new TFile("output.root","RECREATE");
	TTree * otree = new TTree("t","t");
	otree->Branch("process",&Mc_process);
	otree->Branch("volume",&Mc_volume);
	otree->Branch("pid",&Mc_pid);
	otree->Branch("px",&Mc_px);
	otree->Branch("py",&Mc_py);
	otree->Branch("pz",&Mc_pz);
	otree->Branch("tid",&Mc_tid);
	otree->Branch("n",&Mc_nTracks);

	//=======================================================================================================
	Long64_t nEvent = m_TChain->GetEntries();
	int nPass = 0;
	for( Long64_t iEvent = 0; iEvent < (nEvents&&nEvents<nEvent?nEvents:nEvent); iEvent++ ){
		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"In Event "<<iEvent<<std::endl;
		m_TChain->GetEntry(iEvent);
		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Got entries"<<std::endl;

		// CDC cut
		double tmin = -1;
		// FIXME: signal or noise
		//int nHits = 0;
		//int maxlayer = 0;
		//for ( int i = 0; i<C_nHits; i++ ){
		//	if ((*C_edep)[i]<=5e-6&&(*C_tid)[i]==1){
		//		nHits++;
		//		if (maxlayer<(*C_layerID)[i]) maxlayer = (*C_layerID)[i];
		//		if (tmin==-1) tmin = (*C_t)[i];
		//	}
		//}
		//if (maxlayer<6) continue;
		//if (nHits<34) continue;

		// Trigger cut
		for ( int i = 0; i<128; i++){
			TriTimeSci[i] = -1;
			TriTimeGap[i] = -1;
			TriTimeChe[i] = -1;
		}
		for ( int i = 0; i<M_nHits; i++ ){
			// FIXME: signal or noise
			//if ((*M_tid)[i]!=1) continue;
			int type = -1;
			int pos = 0;
			if ((*M_volName)[i]=="TriSciU"||(*M_volName)[i]=="TriSciD") type = 0;
			else if ((*M_volName)[i]=="TriCheU"||(*M_volName)[i]=="TriCheD") type = 1;
			else if ((*M_volName)[i]=="gapU"||(*M_volName)[i]=="gapD") type = 2;
			if ((*M_volName)[i]=="TriCheD"||(*M_volName)[i]=="TriSciD"||(*M_volName)[i]=="gapD") pos = 1;
			if (type<0) continue;
			if (type==0&&(*M_edep)[i]<630e-6) continue;
			double pa = sqrt((*M_px)[i]*(*M_px)[i]+(*M_py)[i]*(*M_py)[i]+(*M_pz)[i]*(*M_pz)[i])*1000;
			double beta = pa/(sqrt(M_ELE*M_ELE+pa*pa));
			if (type==1&&beta<0.5) continue;
			if (type==0) TriTimeSci[64*pos+(*M_volID)[i]] = (*M_t)[i];
			if (type==1) TriTimeChe[64*pos+(*M_volID)[i]] = (*M_t)[i];
			if (type==2) TriTimeGap[64*pos+(*M_volID)[i]] = (*M_t)[i];
		}
		bool pass = false;
		for (int i = 0; i<128; i++){
			//for ( int j = i-1; j<i+2; j++ ){
			for ( int j = i; j<i+1; j++ ){
				for ( int k = j; k<j+1; k++ ){
				//for ( int k = i-1; k<i+2; k++ ){
				//for ( int k = i+1; k<i+2; k++ ){
				//for ( int l = j; l<j+1; l++ ){
				//for ( int l = i-1; l<i+2; l++ ){
				//for ( int l = j+1; l<j+2; l++ ){
					//int ll = l%128;
					//if (ll<0) ll+=128;
					int kk = k%128;
					if (kk<0) kk+=128;
					int ii = i%128;
					if (ii<0) ii+=128;
					int jj = j%128;
					if (jj<0) jj+=128;
					//if (TriTimeChe[jj]!=-1&&TriTimeSci[ii]!=-1&&fabs(TriTimeChe[jj]-TriTimeSci[ii])<10&&TriTimeSci[ii]>tmin){
					//if (TriTimeChe[jj]!=-1&&TriTimeSci[ii]!=-1&&TriTimeChe[ll]!=-1&&TriTimeSci[kk]!=-1&&fabs(TriTimeChe[jj]-TriTimeSci[ii])<10&&fabs(TriTimeChe[ll]-TriTimeSci[ii])<10&&fabs(TriTimeChe[kk]-TriTimeSci[ii])<10&&TriTimeSci[ii]>tmin){
					if (TriTimeGap[kk]!=-1&&TriTimeChe[jj]!=-1&&TriTimeSci[ii]!=-1&&fabs(TriTimeChe[jj]-TriTimeSci[ii])<10&&fabs(TriTimeGap[kk]-TriTimeSci[ii])<10&&TriTimeSci[ii]>tmin){
						pass = true;
					}
				//}
				}
			}
			if (pass) break;
		}
		if (pass) {
			nPass++;
			otree->Fill();
		}
	}/* end of loop in events*/
	otree->Write();
	std::cout<<"nPass = "<<nPass<<std::endl;

	//=>For output
	clock_t t_END = clock();
	//=======================================================================================================
	//************WRITE AND OUTPUT********************
	if (verbose >= Verbose_SectorInfo) std::cout<<prefix_SectorInfo<<"In WRITE AND OUTPUT ###"<<std::endl;
	std::cout<<"\n################BENTCH MARK###################"<<std::endl;
	std::cout<<"TOTAL TIME COST IS:  "<<(double)(t_END-t_START)/CLOCKS_PER_SEC*1000<<"ms"<<std::endl;
	//std::cout<<"PRESET:              "<<(double)(t_PRESET-t_START)/CLOCKS_PER_SEC*1000<<"ms"<<std::endl;
	//std::cout<<"INITIALIZE:          "<<(double)(t_INITIALIZE-t_PRESET)/CLOCKS_PER_SEC*1000<<"ms"<<std::endl;
	//std::cout<<"GENERATEEVENTS:      "<<(double)(t_GENERATEEVENTS-t_BUILDHISTOGRAMS)/CLOCKS_PER_SEC*1000<<"ms"<<std::endl;
	//std::cout<<"TIME COST PER EVENT: "<<(double)(t_GENERATEEVENTS-t_BUILDHISTOGRAMS)/CLOCKS_PER_SEC/num_evt*1000<<"ms"<<std::endl;
	std::cout<<"##############################################\n"<<std::endl;

	return 0;
}

void init_args()
{
	m_prefix="";
	m_suffix="";
	m_InputDir="";
	m_OriginalFile="NONE";
	m_workMode="monitor";
	m_MonitorPlane="PTACS";
	m_OutputDir="result";
	m_input="input";
	verbose = 0;
	nEvents = 0;
	printModule = 10000;
	writeModule = 10000;
	UseWeight = 0;
	PDGEncoding = 13;
	backup = false;
	CdcFile = "CdcFile";
}

void print_usage(char* prog_name)
{
	fprintf(stderr,"Usage %s [options (args)] [input files]\n",prog_name);
	fprintf(stderr,"[options]\n");
	fprintf(stderr,"\t -m\n");
	fprintf(stderr,"\t\t choose work mode: [gen(default), com]\n");
	fprintf(stderr,"\t -r\n");
	fprintf(stderr,"\t\t set run name\n");
	fprintf(stderr,"\t -v\n");
	fprintf(stderr,"\t\t verbose level\n");
	fprintf(stderr,"\t -n\n");
	fprintf(stderr,"\t\t nEvent\n");
	fprintf(stderr,"\t -p\n");
	fprintf(stderr,"\t\t printModule\n");
	fprintf(stderr,"\t -P\n");
	fprintf(stderr,"\t\t PDGEncoding\n");
	fprintf(stderr,"\t -h\n");
	fprintf(stderr,"\t\t Usage message.\n");
	fprintf(stderr,"\t -b\n");
	fprintf(stderr,"\t\tCPU index starts from\n");
	fprintf(stderr,"\t -t\n");
	fprintf(stderr,"\t\tTotal CPU numbers\n");
	fprintf(stderr,"[example]\n");
	fprintf(stderr,"\t\t%s -m ab -v 20 -n 100\n",prog_name);
}
