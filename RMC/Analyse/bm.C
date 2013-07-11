#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <math.h>
#include <string>
#include <vector>
#include <algorithm>
//#include <cstdlib>

#include "globals.hh"
#include "Randomize.hh"

#include "TH1D.h"
#include "TH2D.h"

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
		int nSteps = 0;
		std::vector<double> postX;
		std::vector<double> postY;
		std::vector<double> postZ;
		std::vector<double> time;
		std::vector<std::string> process;
		index_temp = fMyRootInterface->get_TBranch_index("evt_num");
		if (index_temp!=-1) evt_num = fMyRootInterface->get_vec_int(index_temp);
		index_temp = fMyRootInterface->get_TBranch_index("run_num");
		if (index_temp!=-1) run_num = fMyRootInterface->get_vec_int(index_temp);
		index_temp = fMyRootInterface->get_TBranch_index("ProcessCounting_nSteps");
		if (index_temp!=-1) nSteps = fMyRootInterface->get_vec_int(index_temp);
		index_temp = fMyRootInterface->get_TBranch_index("ProcessCounting_postX");
		if (index_temp!=-1) postX = *(fMyRootInterface->get_vec_vecdouble(index_temp));
		for (int i = 0; i<postX.size();i++) postX[i] *= cm;
		index_temp = fMyRootInterface->get_TBranch_index("ProcessCounting_postY");
		if (index_temp!=-1) postY = *(fMyRootInterface->get_vec_vecdouble(index_temp));
		for (int i = 0; i<postY.size();i++) postY[i] *= cm;
		index_temp = fMyRootInterface->get_TBranch_index("ProcessCounting_postZ");
		if (index_temp!=-1) postZ = *(fMyRootInterface->get_vec_vecdouble(index_temp));
		for (int i = 0; i<postZ.size();i++) postZ[i] *= cm;
		index_temp = fMyRootInterface->get_TBranch_index("ProcessCounting_time");
		if (index_temp!=-1) time = *(fMyRootInterface->get_vec_vecdouble(index_temp));
		for (int i = 0; i<time.size();i++) time[i] *= ns;
		index_temp = fMyRootInterface->get_TBranch_index("ProcessCounting_process");
		if (index_temp!=-1) process = *(fMyRootInterface->get_vec_vecstring(index_temp));

		// Got stopped?
		int nPlates = 17;
		double thickness = 0.2*mm;
		double space = 5*cm;
		double right_most_end = (nPlates+1)/2.*space+thickness/2.;
		int index_OB = -1;
		for ( int i_step = 0; i_step < nSteps; i_step++ ){
			double z = postZ[i_step];
			if (z>=right_most_end){
				index_OB = i_step;
				break;
			}
		}

		// Got captured? which step?
		int index_cap = -1;
		for ( int i_step = 0; i_step < nSteps; i_step++ ){
			std::string processName = process[i_step];
			if (processName == "muMinusCaptureAtRest"){
				index_cap = i_step;
				break;
			}
		}

		// Position and which plate?
		double hit_x = index_cap==-1?0:postX[index_cap];
		double hit_y = index_cap==-1?0:postY[index_cap];
		double hit_z = index_cap==-1?0:postZ[index_cap];
		double hit_t = index_cap==-1?0:time[index_cap];
		double hit_r = sqrt(hit_x*hit_x+hit_y*hit_y);
		double hit_relZ = 0;
		int hit_iPlate = -1;
		for ( int i_plate = 0; i_plate < nPlates; i_plate++ ){
			double plateZ = (i_plate*2 - nPlates +1)/2*space;
			double leftEnd = plateZ - thickness/2;
			double rightEnd = plateZ + thickness/2;
			if ( hit_z >= leftEnd && hit_z <= rightEnd){
				hit_iPlate = i_plate;
				hit_relZ = hit_z - plateZ;
				break;
			}
		}

		// Fill the tree
		// for g4sim input, should use mm, MeV, ns
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("evt_num")) != -1 )
			fMyRootInterface->set_ovec_double(index_temp,evt_num); 
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("run_num")) != -1 )
			fMyRootInterface->set_ovec_double(index_temp,run_num); 
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("x")) != -1 )
			fMyRootInterface->set_ovec_double(index_temp,hit_x/mm); 
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("y")) != -1 )
			fMyRootInterface->set_ovec_double(index_temp,hit_y/mm); 
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("z")) != -1 )
			fMyRootInterface->set_ovec_double(index_temp,hit_z/mm); 
		double decayTime = -880*log(G4UniformRand());
		if ( (index_temp = fMyRootInterface->get_oTBranch_index("t")) != -1 )
			fMyRootInterface->set_ovec_double(index_temp,decayTime+hit_t); 

		if (index_OB!=-1)
			continue;
		N1++;
		if ( index_cap == -1 )
			continue;
		N2++;
		if ( hit_iPlate == -1 )
			continue;
		N3++;
		fMyRootInterface->Fill();

		if ( (index_temp = fMyRootInterface->get_TH1D_index("relz")) != -1 ){
			fMyRootInterface->get_TH1D(index_temp)->Fill(hit_relZ/mm);
		}
		if ( (index_temp = fMyRootInterface->get_TH1D_index("iplate")) != -1 ){
			fMyRootInterface->get_TH1D(index_temp)->Fill(hit_iPlate);
		}
		if ( (index_temp = fMyRootInterface->get_TH1D_index("r")) != -1 ){
			fMyRootInterface->get_TH1D(index_temp)->Fill(hit_r/cm);
		}
		buff.str("");
		buff.clear();
		buff<<"relz_"<<hit_iPlate;
		if ( (index_temp = fMyRootInterface->get_TH1D_index(buff.str())) != -1 ){
			fMyRootInterface->get_TH1D(index_temp)->Fill(hit_relZ/mm);
		}
		buff.str("");
		buff.clear();
		buff<<"r_"<<hit_iPlate;
		if ( (index_temp = fMyRootInterface->get_TH1D_index(buff.str())) != -1 ){
			fMyRootInterface->get_TH1D(index_temp)->Fill(hit_r/cm);
		}

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
