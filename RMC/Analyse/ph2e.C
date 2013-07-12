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

#include "MyRootInterface.hxx"

char m_workMode[128];
std::string m_runName;
int verbose = 0;
int nEvents = 0;
int printModule = 1;
bool backup = false;

void init_args();
void print_usage(char* prog_name);

int main(int argc, char* argv[]){

	clock_t t_START = clock();

	//=======================================
	//*************read parameter**********
	init_args();
	int result;
	while((result=getopt(argc,argv,"hbv:n:m:r:p:"))!=-1){
		switch(result){
			/* INPUTS */
			case 'm':
				strcpy(m_workMode,optarg);
				printf("work mode: %s\n",m_workMode);
				break;
			case 'r':
				m_runName=optarg;
				printf("run name: %s\n",m_runName.c_str());
				break;
			case 'v':
				verbose = atoi(optarg);
				printf("verbose level: %d\n",verbose);
				break;
			case 'b':
				backup = true;
				printf("restore backup file!\n");
				break;
			case 'n':
				nEvents = atoi(optarg);
				printf("nEvent: %d\n",nEvents);
				break;
			case 'p':
				printModule = atoi(optarg);
				printf("printModule: %d\n",printModule);
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

	//##########################PRESET############################
	if (verbose >= Verbose_SectorInfo ) std::cout<<prefix_SectorInfo<<"In Preset###"<<std::endl;
	MyRootInterface *fMyRootInterface = new MyRootInterface(verbose);
	fMyRootInterface->set_OutputDir("result");
	int index_temp = 0;
	TH1D *h1d_temp=0;
	int bin_temp = 0;
	std::string name_temp = "";
	std::stringstream buff;

	//=>About Constant
	double PI = 3.141592653589793238;
	double FSC = 1/137.03599911; //fine structure constant
	double M_MUON = 105.6584*MeV; //mass of muon in MeV
	double M_ELE = 0.510999*MeV; //mass of electron in MeV
	double M_U = 931.494061*MeV; //atomic mass unit in MeV
	double M_p = 0.9382723*GeV;  // mass of proton// proton mass unit in GeV

	//##########################Prepare histograms############################
	if (verbose >= Verbose_SectorInfo ) std::cout<<prefix_SectorInfo<<"In SET HISTOGRAMS###"<<std::endl;
	fMyRootInterface->read("input");
	fMyRootInterface->set_OutputName(m_runName);
	fMyRootInterface->init();

	//************SET Statistics********************
	if (verbose >= Verbose_SectorInfo ) std::cout<<prefix_SectorInfo<<"In SET Statistics###"<<std::endl;
	//=>About Statistical
	int N0 = 0;
	int N1 = 0;
	int N2 = 0;
	int N3 = 0;
	int N4 = 0;
	int N5 = 0;
	int N6 = 0;
	int N7 = 0;

	//=======================================================================================================
	//************DO THE DIRTY WORK*******************
	if (verbose >= Verbose_SectorInfo) std::cout<<prefix_SectorInfo<<"In DO THE DIRTY WORK ###"<<std::endl;
	Long64_t nEvent = fMyRootInterface->get_Entries();
	for( Long64_t iEvent = 0; iEvent < nEvent; iEvent++ ){
		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"In Event "<<iEvent<<std::endl;
		N0++;
		fMyRootInterface->GetEntry(iEvent);
		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Got entries"<<std::endl;

		// Get info
		int evt_num;
		int run_num;
		int McTruth_nTracks = 0;
		std::vector<int> McTruth_tid;
		std::vector<int> McTruth_pid;
		std::vector<int> McTruth_ptid;
		std::vector<double> McTruth_x;
		std::vector<double> McTruth_y;
		std::vector<double> McTruth_z;
		std::vector<double> McTruth_px;
		std::vector<double> McTruth_py;
		std::vector<double> McTruth_pz;
		std::vector<double> McTruth_e;
		std::vector<double> McTruth_time;
		std::vector<std::string> McTruth_process;
		std::vector<std::string> McTruth_volume;
		int CdcCell_nHits = 0;
		std::vector<int> CdcCell_tid;
		std::vector<double> CdcCell_t;
		std::vector<double> CdcCell_e;
		int Trigger_nHits = 0;
		std::vector<int> Trigger_tid;
		std::vector<double> Trigger_t;

		index_temp = fMyRootInterface->get_TBranch_index("evt_num");
		if (index_temp!=-1) evt_num = fMyRootInterface->get_vec_int(index_temp);
		index_temp = fMyRootInterface->get_TBranch_index("run_num");
		if (index_temp!=-1) run_num = fMyRootInterface->get_vec_int(index_temp);
		index_temp = fMyRootInterface->get_TBranch_index("McTruth_nTracks");
		if (index_temp!=-1) McTruth_nTracks = fMyRootInterface->get_vec_int(index_temp);
		index_temp = fMyRootInterface->get_TBranch_index("McTruth_pid");
		if (index_temp!=-1) McTruth_pid = *(fMyRootInterface->get_vec_vecint(index_temp));
		index_temp = fMyRootInterface->get_TBranch_index("McTruth_tid");
		if (index_temp!=-1) McTruth_tid = *(fMyRootInterface->get_vec_vecint(index_temp));
		index_temp = fMyRootInterface->get_TBranch_index("McTruth_ptid");
		if (index_temp!=-1) McTruth_ptid = *(fMyRootInterface->get_vec_vecint(index_temp));
		index_temp = fMyRootInterface->get_TBranch_index("McTruth_x");
		if (index_temp!=-1) McTruth_x = *(fMyRootInterface->get_vec_vecdouble(index_temp));
		for (int i = 0; i<McTruth_x.size();i++) McTruth_x[i] *= cm;
		index_temp = fMyRootInterface->get_TBranch_index("McTruth_y");
		if (index_temp!=-1) McTruth_y = *(fMyRootInterface->get_vec_vecdouble(index_temp));
		for (int i = 0; i<McTruth_y.size();i++) McTruth_y[i] *= cm;
		index_temp = fMyRootInterface->get_TBranch_index("McTruth_z");
		if (index_temp!=-1) McTruth_z = *(fMyRootInterface->get_vec_vecdouble(index_temp));
		for (int i = 0; i<McTruth_z.size();i++) McTruth_z[i] *= cm;
		index_temp = fMyRootInterface->get_TBranch_index("McTruth_px");
		if (index_temp!=-1) McTruth_px = *(fMyRootInterface->get_vec_vecdouble(index_temp));
		for (int i = 0; i<McTruth_px.size();i++) McTruth_px[i] *= GeV;
		index_temp = fMyRootInterface->get_TBranch_index("McTruth_py");
		if (index_temp!=-1) McTruth_py = *(fMyRootInterface->get_vec_vecdouble(index_temp));
		for (int i = 0; i<McTruth_py.size();i++) McTruth_py[i] *= GeV;
		index_temp = fMyRootInterface->get_TBranch_index("McTruth_pz");
		if (index_temp!=-1) McTruth_pz = *(fMyRootInterface->get_vec_vecdouble(index_temp));
		for (int i = 0; i<McTruth_pz.size();i++) McTruth_pz[i] *= GeV;
		index_temp = fMyRootInterface->get_TBranch_index("McTruth_time");
		if (index_temp!=-1) McTruth_time = *(fMyRootInterface->get_vec_vecdouble(index_temp));
		for (int i = 0; i<McTruth_time.size();i++) McTruth_time[i] *= ns;
		index_temp = fMyRootInterface->get_TBranch_index("McTruth_e");
		if (index_temp!=-1) McTruth_e = *(fMyRootInterface->get_vec_vecdouble(index_temp));
		for (int i = 0; i<McTruth_e.size();i++) McTruth_e[i] *= GeV;
		index_temp = fMyRootInterface->get_TBranch_index("McTruth_process");
		if (index_temp!=-1) McTruth_process = *(fMyRootInterface->get_vec_vecstring(index_temp));
		index_temp = fMyRootInterface->get_TBranch_index("McTruth_volume");
		if (index_temp!=-1) McTruth_volume = *(fMyRootInterface->get_vec_vecstring(index_temp));
		index_temp = fMyRootInterface->get_TBranch_index("Trigger_nHits");
		if (index_temp!=-1) Trigger_nHits = fMyRootInterface->get_vec_int(index_temp);
		index_temp = fMyRootInterface->get_TBranch_index("Trigger_t");
		if (index_temp!=-1) Trigger_t = *(fMyRootInterface->get_vec_vecdouble(index_temp));
		for (int i = 0; i<Trigger_t.size();i++) Trigger_t[i] *= ns;
		index_temp = fMyRootInterface->get_TBranch_index("Trigger_tid");
		if (index_temp!=-1) Trigger_tid = *(fMyRootInterface->get_vec_vecint(index_temp));
		index_temp = fMyRootInterface->get_TBranch_index("CdcCell_nHits");
		if (index_temp!=-1) CdcCell_nHits = fMyRootInterface->get_vec_int(index_temp);
		index_temp = fMyRootInterface->get_TBranch_index("CdcCell_t");
		if (index_temp!=-1) CdcCell_t = *(fMyRootInterface->get_vec_vecdouble(index_temp));
		for (int i = 0; i<CdcCell_t.size();i++) CdcCell_t[i] *= ns;
		index_temp = fMyRootInterface->get_TBranch_index("CdcCell_e");
		if (index_temp!=-1) CdcCell_e = *(fMyRootInterface->get_vec_vecdouble(index_temp));
		for (int i = 0; i<CdcCell_e.size();i++) CdcCell_e[i] *= GeV;
		index_temp = fMyRootInterface->get_TBranch_index("CdcCell_tid");
		if (index_temp!=-1) CdcCell_tid = *(fMyRootInterface->get_vec_vecint(index_temp));
		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Got info"<<std::endl;

		// find electron
		int index = -1;
		double maxe = 0;
		for ( int i_par = 0; i_par < McTruth_nTracks; i_par++ ){
			int pid = McTruth_pid[i_par];
			int ptid = McTruth_ptid[i_par];
			double e = McTruth_e[i_par];
			if ( pid == 11 && e>maxe ){
				index = i_par;
				maxe = e;
			}
		}
		if (index == -1 ) continue;
		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Found electron"<<std::endl;
		N1++;

		// get information of this electron
		double x = McTruth_x[index];
		double y = McTruth_y[index];
		double z = McTruth_z[index];
		double px = McTruth_px[index];
		double py = McTruth_py[index];
		double pz = McTruth_pz[index];
		double e = McTruth_e[index];
		double pa = sqrt(px*px+py*py+pz*pz);
		double theta = (pa==0?2*PI:acos(pz/pa));
		double t = McTruth_time[index];
		std::string process = McTruth_process[index];
		std::string volume = McTruth_volume[index];

		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0)
			std::cout<<prefix_EventInfoStart
				<<" pa = "<<pa/MeV
				<<"MeV, process = \""<<process
				<<"\", volume = \""<<volume
				<<"\""
				<<std::endl;

		if (CdcCell_nHits<=0) // not hit the Cdc
			continue;
		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Found CDC hits"<<std::endl;
		N2++;

		int tid = McTruth_tid[index];
		int i_CdcHit = -1;
		double CdcCell_firstHitTime = -1;
		for ( int i_hit = 0; i_hit < CdcCell_nHits; i_hit++ ){
			int i_tid = CdcCell_tid[i_hit];
			if (i_tid == tid){
				CdcCell_firstHitTime = CdcCell_t[i_hit];
				i_CdcHit = i_hit;
				break;
			}
		}
		if ( CdcCell_firstHitTime == -1) // this electron not hit the Cdc
			continue;
		N3++;
		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Found electron hits CDC"<<std::endl;

		if (Trigger_nHits<=0) // not hit the trigger
			continue;
		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Found Trigger hits"<<std::endl;
		N4++;

		double Trigger_firstHitTime = -1;
		for ( int i_hit = 0; i_hit < Trigger_nHits; i_hit++ ){
			int i_tid = Trigger_tid[i_hit];
			if (i_tid == tid){
				Trigger_firstHitTime = Trigger_t[i_hit];
				break;
			}
		}
		if ( Trigger_firstHitTime == -1) // this electron not hit the trigger
			continue;
		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Found electron hits Trigger"<<std::endl;
		N5++;

		if ( Trigger_firstHitTime <= CdcCell_firstHitTime ) // hit trigger first
			continue;
		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Found electron hits CDC first"<<std::endl;
		N6++;

		double deltat = gRandom->Gaus()*100*ns;
		double smeared_time = CdcCell_firstHitTime + deltat;
		double smeared_ini_time = t + deltat;
		// Fill the histogram
		double e_CdcHit = CdcCell_e[i_CdcHit];
		double depE = e - e_CdcHit;
		if ( (index_temp = fMyRootInterface->get_TH1D_index(m_runName+"pa")) != -1 ){
			fMyRootInterface->get_TH1D(index_temp)->Fill(pa);
		}
		if ( (index_temp = fMyRootInterface->get_TH1D_index(m_runName+"theta")) != -1 ){
			fMyRootInterface->get_TH1D(index_temp)->Fill(theta);
		}
		if ( (index_temp = fMyRootInterface->get_TH1D_index(m_runName+"depE")) != -1 ){
			fMyRootInterface->get_TH1D(index_temp)->Fill(depE/MeV);
		}

		// Fill the tree
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("evt_num")) != -1 )
			fMyRootInterface->set_ovec_double(index_temp,evt_num); 
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("run_num")) != -1 )
			fMyRootInterface->set_ovec_double(index_temp,run_num); 
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("x")) != -1 )
			fMyRootInterface->set_ovec_double(index_temp,x/mm); 
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("y")) != -1 )
			fMyRootInterface->set_ovec_double(index_temp,y/mm); 
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("z")) != -1 )
			fMyRootInterface->set_ovec_double(index_temp,z/mm); 
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("px")) != -1 )
			fMyRootInterface->set_ovec_double(index_temp,px/MeV); 
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("py")) != -1 )
			fMyRootInterface->set_ovec_double(index_temp,py/MeV); 
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("pz")) != -1 )
			fMyRootInterface->set_ovec_double(index_temp,pz/MeV); 
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("t")) != -1 )
			fMyRootInterface->set_ovec_double(index_temp,t); 
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("dt")) != -1 )
			fMyRootInterface->set_ovec_double(index_temp,deltat); 
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("st")) != -1 )
			fMyRootInterface->set_ovec_double(index_temp,smeared_ini_time); 
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("sht")) != -1 )
			fMyRootInterface->set_ovec_double(index_temp,smeared_time); 
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("prid")) != -1 )
			fMyRootInterface->set_ovec_string(index_temp,process); 
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("vid")) != -1 )
			fMyRootInterface->set_ovec_string(index_temp,volume); 
		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Set oTrees"<<std::endl;

		fMyRootInterface->Fill();
		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Filled"<<std::endl;

		if ( smeared_time < 700*ns || smeared_time > 1314*ns ) // hit trigger first
			continue;
		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfoStart<<"Found electron hits CDC first"<<std::endl;
		N7++;


		if (verbose >= Verbose_EventInfo || iEvent%printModule == 0) std::cout<<prefix_EventInfo<<"Finished!"<<std::endl;
	}/* end of loop in events*/

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
	std::cout<<"N0 = "<<N0<<std::endl;
	std::cout<<"N1 = "<<N1<<std::endl;
	std::cout<<"N2 = "<<N2<<std::endl;
	std::cout<<"N3 = "<<N3<<std::endl;
	std::cout<<"N4 = "<<N4<<std::endl;
	std::cout<<"N5 = "<<N5<<std::endl;
	std::cout<<"N6 = "<<N6<<std::endl;
	std::cout<<"N7 = "<<N7<<std::endl;

	fMyRootInterface->dump();
	return 0;
}

void init_args()
{
	strcpy(m_workMode,"gen");
	verbose = 0;
	nEvents = 0;
	printModule = 10000;
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
	fprintf(stderr,"\t -h\n");
	fprintf(stderr,"\t\t Usage message.\n");
	fprintf(stderr,"\t -b\n");
	fprintf(stderr,"\t\t restore backup file.\n");
	fprintf(stderr,"[example]\n");
	fprintf(stderr,"\t\t%s -m ab -v 20 -n 100\n",prog_name);
}
