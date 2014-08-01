#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <math.h>
#include <string>
#include <vector>
#include <algorithm>

#include "globals.hh"
#include "Randomize.hh"

#include "TH1D.h"
#include "TH2D.h"
#include "TRandom.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"

#include "MyRootInterface.hxx"

std::string m_workMode;
std::string m_prefix;
std::string m_suffix;
std::string m_MonitorPlane;
std::string m_runName;
std::string m_input;
std::string m_OutputDir;
std::string m_InputDir;
std::string m_OriginalFile;
std::string m_oFileName;
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
std::vector<int> Ncut;
std::vector<std::string> Ncut_message;

void init_Ncut();
void inc_Ncut(std::string);
void dump_Ncut();
void init_args();
void print_usage(char* prog_name);

int main(int argc, char* argv[]){

	clock_t t_START = clock();

	//=======================================
	//*************read parameter**********
	init_args();
	int result;
	while((result=getopt(argc,argv,"hb:t:v:n:N:m:M:r:D:O:o:d:p:P:w:W:x:y:i:"))!=-1){
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
//				printf("runName: %s\n",m_runName.c_str());
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
			case 'o':
				m_oFileName=optarg;
//				printf("oFileName: %s\n",m_oFileName.c_str());
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

	//##########################PRESET############################
	if (verbose >= Verbose_SectorInfo ) std::cout<<prefix_SectorInfo<<"In Preset###"<<std::endl;
	MyRootInterface *fMyRootInterface = new MyRootInterface(verbose,backup);
	fMyRootInterface->set_OutputDir(m_OutputDir);
	int index_temp = 0;
	TH1D *h1d_temp=0;
	TH1D *h1d_temp2=0;
	int bin_temp = 0;
	std::string name_temp = "";
	std::stringstream buff;

	//=>About Constant
	double PI = 3.141592653589793238;
	double FSC = 1/137.03599911; //fine structure constant
	double M_PION = 0.139570*GeV;
	double M_MUON = 105.6584*MeV; //mass of muon in MeV
	double M_ELE = 0.510999*MeV; //mass of electron in MeV
	double M_U = 931.494061*MeV; //atomic mass unit in MeV
	double M_p = 0.9382723*GeV;  // mass of proton// proton mass unit in GeV
	double tau = 26*ns; // mean life time of pion

	//##########################Prepare histograms############################
	if (verbose >= Verbose_SectorInfo ) std::cout<<prefix_SectorInfo<<"In SET HISTOGRAMS###"<<std::endl;
	fMyRootInterface->read(m_input);
	if (m_beginNo!=-1&&m_totalNo!=-1){
		fMyRootInterface->set_beginCPU(0,m_beginNo);
		fMyRootInterface->set_NCPU(0,m_totalNo);
	}
	if (m_oFileName!="Dummy.root"&&m_oFileName!="NULL"){
		fMyRootInterface->add_oFileName(m_oFileName.c_str());
	}
	fMyRootInterface->set_OutputName(m_runName);
	fMyRootInterface->init();

	//************SET Statistics********************
	if (verbose >= Verbose_SectorInfo ) std::cout<<prefix_SectorInfo<<"In SET Statistics###"<<std::endl;
	//=>About Statistical
	init_Ncut();
	double N_INI = 0;
	double N_CDC = 0;

	// General
	double weight;
	int R1;
	int R0;

	// MonitorSD
	std::vector<int> C_tid;
	std::vector<int> C_pid;
	std::vector<int> McTruth_pid;
	std::vector<double> McTruth_x;
	std::vector<double> McTruth_y;
	std::vector<double> McTruth_z;
	std::vector<double> McTruth_px;
	std::vector<double> McTruth_py;
	std::vector<double> McTruth_pz;
	std::vector<double> McTruth_time;
	std::vector<double> C_t;
	std::vector<double> C_x;
	std::vector<double> C_y;
	std::vector<double> C_z;
	std::vector<double> C_px;
	std::vector<double> C_py;
	std::vector<double> C_pz;
	std::vector<double> C_ox;
	std::vector<double> C_oy;
	std::vector<double> C_oz;
	std::vector<double> C_opx;
	std::vector<double> C_opy;
	std::vector<double> C_opz;
	std::vector<std::string> C_ovolName;
	std::vector<std::string> C_oprocess;

	//=======================================================================================================
	//************DO THE DIRTY WORK*******************
	if (verbose >= Verbose_SectorInfo ) std::cout<<prefix_SectorInfo<<"In DO THE DIRTY WORK###"<<std::endl;
	Long64_t nEvent = fMyRootInterface->get_Entries();
	for( Long64_t iEvent = 0; iEvent < (nEvents&&nEvents<nEvent?nEvents:nEvent); iEvent++ ){
		if (verbose >= Verbose_EventInfo || (printModule&&iEvent%printModule == 0)) std::cout<<prefix_EventInfoStart<<"In Event "<<iEvent<<std::endl;
		fMyRootInterface->GetEntry(iEvent);
		if (verbose >= Verbose_EventInfo || (printModule&&iEvent%printModule == 0)) std::cout<<prefix_EventInfoStart<<"Got entries"<<std::endl;

		// Get info
		fMyRootInterface->get_value("R0",R0);
		fMyRootInterface->get_value("R1",R1);
		fMyRootInterface->get_value("weight",weight);
		fMyRootInterface->get_value("C_tid",C_tid);
		fMyRootInterface->get_value("C_pid",C_pid);
		fMyRootInterface->get_value("McTruth_pid",McTruth_pid);
		fMyRootInterface->get_value("McTruth_time",McTruth_time,ns);
		fMyRootInterface->get_value("McTruth_x",McTruth_x,cm);
		fMyRootInterface->get_value("McTruth_y",McTruth_y,cm);
		fMyRootInterface->get_value("McTruth_z",McTruth_z,cm);
		fMyRootInterface->get_value("McTruth_px",McTruth_px,GeV);
		fMyRootInterface->get_value("McTruth_py",McTruth_py,GeV);
		fMyRootInterface->get_value("McTruth_pz",McTruth_pz,GeV);
		fMyRootInterface->get_value("C_t",C_t,ns);
		fMyRootInterface->get_value("C_x",C_x,cm);
		fMyRootInterface->get_value("C_y",C_y,cm);
		fMyRootInterface->get_value("C_z",C_z,cm);
		fMyRootInterface->get_value("C_px",C_px,GeV);
		fMyRootInterface->get_value("C_py",C_py,GeV);
		fMyRootInterface->get_value("C_pz",C_pz,GeV);
		fMyRootInterface->get_value("C_ox",C_ox,cm);
		fMyRootInterface->get_value("C_oy",C_oy,cm);
		fMyRootInterface->get_value("C_oz",C_oz,cm);
		fMyRootInterface->get_value("C_opx",C_opx,GeV);
		fMyRootInterface->get_value("C_opy",C_opy,GeV);
		fMyRootInterface->get_value("C_opz",C_opz,GeV);
		fMyRootInterface->get_value("C_ovolName",C_ovolName);
		fMyRootInterface->get_value("C_oprocess",C_oprocess);

		// find particles
		if (verbose >= Verbose_EventInfo || (printModule&&iEvent%printModule == 0)) std::cout<<prefix_EventInfoStart<<"###Getting information"<<std::endl;
		std::vector<int> gamtid;
		for (int iHit = 0; iHit < C_tid.size(); iHit++){
			if (C_pid[iHit]!=22) continue;
			bool found = false;
			for (int igam = 0; igam<gamtid.size(); igam++){
				if (C_tid[iHit]==gamtid[igam]){
					found = true;
					break;
				}
			}
			if (!found){
				gamtid.push_back(C_tid[iHit]);
				if (m_workMode=="BP"){
					fMyRootInterface->set_ovalue("pid",McTruth_pid[0]);
					fMyRootInterface->set_ovalue("t",McTruth_time[0]/ns);
					fMyRootInterface->set_ovalue("x",McTruth_x[0]/mm);
					fMyRootInterface->set_ovalue("y",McTruth_y[0]/mm);
					fMyRootInterface->set_ovalue("z",McTruth_z[0]/mm);
					fMyRootInterface->set_ovalue("r",sqrt(McTruth_x[0]*McTruth_x[0]+McTruth_y[0]*McTruth_y[0])/mm);
					fMyRootInterface->set_ovalue("px",McTruth_px[0]/MeV);
					fMyRootInterface->set_ovalue("py",McTruth_py[0]/MeV);
					fMyRootInterface->set_ovalue("pz",McTruth_pz[0]/MeV);
				}
				else if (m_workMode=="GAM"){
					fMyRootInterface->set_ovalue("pid",C_pid[iHit]);
					fMyRootInterface->set_ovalue("t",C_t[iHit]/ns);
					fMyRootInterface->set_ovalue("x",C_x[iHit]/mm);
					fMyRootInterface->set_ovalue("y",C_y[iHit]/mm);
					fMyRootInterface->set_ovalue("z",C_z[iHit]/mm);
					fMyRootInterface->set_ovalue("r",sqrt(C_x[iHit]*C_x[iHit]+C_y[iHit]*C_y[iHit])/mm);
					fMyRootInterface->set_ovalue("px",C_px[iHit]/MeV);
					fMyRootInterface->set_ovalue("py",C_py[iHit]/MeV);
					fMyRootInterface->set_ovalue("pz",C_pz[iHit]/MeV);
				}
				fMyRootInterface->set_ovalue("ce",sqrt(McTruth_px[0]*McTruth_px[0]+McTruth_py[0]*McTruth_py[0]+McTruth_pz[0]*McTruth_pz[0])/MeV);
				fMyRootInterface->set_ovalue("cx",C_x[iHit]/mm);
				fMyRootInterface->set_ovalue("cy",C_y[iHit]/mm);
				fMyRootInterface->set_ovalue("cz",C_z[iHit]/mm);
				fMyRootInterface->set_ovalue("cr",sqrt(C_x[iHit]*C_x[iHit]+C_y[iHit]*C_y[iHit])/mm);
				fMyRootInterface->set_ovalue("cpx",C_px[iHit]/MeV);
				fMyRootInterface->set_ovalue("cpy",C_py[iHit]/MeV);
				fMyRootInterface->set_ovalue("cpz",C_pz[iHit]/MeV);
				fMyRootInterface->set_ovalue("ce",sqrt(C_px[iHit]*C_px[iHit]+C_py[iHit]*C_py[iHit]+C_pz[iHit]*C_pz[iHit])/MeV); fMyRootInterface->set_ovalue("ox",C_ox[iHit]/mm);
				fMyRootInterface->set_ovalue("cox",C_ox[iHit]/mm);
				fMyRootInterface->set_ovalue("coy",C_oy[iHit]/mm);
				fMyRootInterface->set_ovalue("coz",C_oz[iHit]/mm);
				fMyRootInterface->set_ovalue("cor",sqrt(C_ox[iHit]*C_ox[iHit]+C_oy[iHit]*C_oy[iHit])/mm);
				fMyRootInterface->set_ovalue("copx",C_opx[iHit]/MeV);
				fMyRootInterface->set_ovalue("copy",C_opy[iHit]/MeV);
				fMyRootInterface->set_ovalue("copz",C_opz[iHit]/MeV);
				fMyRootInterface->set_ovalue("coe",sqrt(C_opx[iHit]*C_opx[iHit]+C_opy[iHit]*C_opy[iHit]+C_opz[iHit]*C_opz[iHit])/MeV);
				fMyRootInterface->set_ovalue("volume",C_ovolName[iHit]);
				fMyRootInterface->set_ovalue("process",C_oprocess[iHit]);
				fMyRootInterface->set_ovalue("weight",weight);
				fMyRootInterface->set_ovalue("R0",R0);
				fMyRootInterface->set_ovalue("R1",R1);
				fMyRootInterface->Fill();
			}
		}
		N_CDC+=gamtid.size()*weight;

		if (verbose >= Verbose_EventInfo || (printModule&&iEvent%printModule == 0)) std::cout<<prefix_EventInfo<<"Finished!"<<std::endl;
		if (writeModule&&iEvent%writeModule==0){
			fMyRootInterface->Write();
			if (verbose >= Verbose_EventInfo || (writeModule&&iEvent%writeModule == 0)) std::cout<<prefix_EventInfoStart<<"Written file"<<std::endl;
		}

	}/* end of loop in events*/
	fMyRootInterface->Write();

	//=>For output
	clock_t t_END = clock();
	//=======================================================================================================
	//************WRITE AND OUTPUT********************
	if (verbose >= Verbose_SectorInfo) std::cout<<prefix_SectorInfo<<"In WRITE AND OUTPUT ###"<<std::endl;
//	std::cout<<"\n################BENTCH MARK###################"<<std::endl;
//	std::cout<<"TOTAL TIME COST IS:  "<<(double)(t_END-t_START)/CLOCKS_PER_SEC*1000<<"ms"<<std::endl;
//	//std::cout<<"PRESET:              "<<(double)(t_PRESET-t_START)/CLOCKS_PER_SEC*1000<<"ms"<<std::endl;
//	//std::cout<<"INITIALIZE:          "<<(double)(t_INITIALIZE-t_PRESET)/CLOCKS_PER_SEC*1000<<"ms"<<std::endl;
//	//std::cout<<"GENERATEEVENTS:      "<<(double)(t_GENERATEEVENTS-t_BUILDHISTOGRAMS)/CLOCKS_PER_SEC*1000<<"ms"<<std::endl;
//	//std::cout<<"TIME COST PER EVENT: "<<(double)(t_GENERATEEVENTS-t_BUILDHISTOGRAMS)/CLOCKS_PER_SEC/num_evt*1000<<"ms"<<std::endl;
//	std::cout<<"##############################################\n"<<std::endl;
	dump_Ncut();

//	fMyRootInterface->dump();

	std::cout<<N_CDC<<std::endl;

	return 0;
}

void init_args()
{
	m_prefix="";
	m_suffix="";
	m_InputDir="";
	m_OriginalFile="NONE";
	m_workMode="BP";
	m_MonitorPlane="PTACS";
	m_OutputDir="result";
	m_input="input";
	m_oFileName="NULL";
	m_runName="";
	verbose = 0;
	nEvents = 0;
	printModule = 0;
	writeModule = 0;
	UseWeight = 0;
	PDGEncoding = 13;
	backup = false;
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

void init_Ncut(){
	Ncut.clear();
	Ncut_message.clear();
}

void inc_Ncut(std::string mess){
	int iCut = -1;
	for (int i = 0; i < Ncut_message.size(); i++){
		if (Ncut_message[i]==mess){
			iCut = i;
			break;
		}
	}
	if (iCut == -1){
		iCut = Ncut_message.size();
		Ncut_message.push_back(mess);
		Ncut.push_back(0);
	}
	Ncut[iCut]++;
}

void dump_Ncut(){
	for (int i = 0; i < Ncut.size(); i++ ){
		std::cout<<"  "<<Ncut_message[i]<<": "<<Ncut[i]<<std::endl;
	}
}
