#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <vector>

#include "TChain.h"
#include "TCut.h"
#include "TROOT.h"

#ifdef __MAKECINT__
#pragma link C++ class vector<TCut>+;
#endif

std::string m_treeName;
std::string m_fileDir;
std::string m_file;
std::string m_monitor;
int nJobs = 0;
int m_pid = 0;
int verbose = 0;
int nEvents = 0;
int printModule = 1;

void init_args();
void print_usage(char* prog_name);

int main(int argc, char* argv[]){
	std::stringstream buff;

	gROOT->ProcessLine(".L vecTCutDict.C+");
	//=======================================
	//*************read parameter**********
	init_args();
	int result;
	while((result=getopt(argc,argv,"hv:n:m:d:f:j:i:p:t:"))!=-1){
		switch(result){
			/* INPUTS */
			case 't':
				m_treeName = optarg;
				printf("Tree Name: %s\n",m_treeName.c_str());
				break;
			case 'm':
				m_monitor = optarg;
				printf("monitor plane: %s\n",m_monitor.c_str());
				break;
			case 'd':
				m_fileDir=optarg;
				printf("JobFile directory: %s\n",m_fileDir.c_str());
				break;
			case 'f':
				m_file=optarg;
				printf("Single file: %s\n",m_file.c_str());
				break;
			case 'j':
				nJobs=atoi(optarg);
				printf("number of jobs: %d\n",nJobs);
				break;
			case 'i':
				m_pid=atoi(optarg);
				printf("PDGEncoding: %d\n",m_pid);
				break;
			case 'v':
				verbose = atoi(optarg);
				printf("verbose level: %d\n",verbose);
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

	TChain *c = new TChain("tree");
	for ( int i = 0; i < nJobs; i++ ){
		buff.str("");
		buff.clear();
		buff<<m_fileDir<<"/"<<i<<"_job0.raw";
		c->AddFile(buff.str().c_str());
		std::cout<<"Added "<<buff.str()<<std::endl;
	}

	if (m_file!=""){
		c->AddFile(m_file.c_str());
		std::cout<<"Added "<<m_file<<std::endl;
	}

	c->SetScanField(0);
	std::string content = m_monitor + ".trackID";
	if (m_monitor == "ts2_0"){
		content += ":" + m_monitor + ".position.fX*10";
		content += ":" + m_monitor + ".position.fY*10";
		content += ":" + m_monitor + ".position.fZ*10-1389.5"; // to CdcWithBLT
	}
	else if (m_monitor == "blt1"){
		content += ":" + m_monitor + ".position.fX*10";
		content += ":" + m_monitor + ".position.fY*10";
		content += ":" + m_monitor + ".position.fZ*10-2390.5"; // to CdcWithBLT
	}
	else if (m_monitor == "blt0"){
		content += ":" + m_monitor + ".position.fX*10";
		content += ":" + m_monitor + ".position.fY*10";
		content += ":" + m_monitor + ".position.fZ*10-2790.5"; // to CdcWithBLT
	}
	else if (m_monitor == "ptacs_beampipe"){
		content += ":-" + m_monitor + ".position.fZ*10+4505.5";
		content += ":" + m_monitor + ".position.fY*10";
		content += ":" + m_monitor + ".position.fX*10-5790.5"; // to CometPhase1
	}
	else if (m_monitor == "ptacs_shielding"){
		content += ":-" + m_monitor + ".position.fZ*10+6600";
		content += ":" + m_monitor + ".position.fY*10";
		content += ":" + m_monitor + ".position.fX*10-5790.5"; // to CometPhase1
	}
	else {
		printf("Cannot recogonize this  monitor plane \"%s\"\n",m_monitor.c_str());
		return -1;
	}
	if (m_monitor == "ts2_0"||m_monitor=="blt1"||m_monitor=="blt0"){
		content += ":" + m_monitor + ".direction.fX*" + m_monitor + ".momentum";
		content += ":" + m_monitor + ".direction.fY*" + m_monitor + ".momentum";
		content += ":" + m_monitor + ".direction.fZ*" + m_monitor + ".momentum";
	}
	else if (m_monitor == "ptacs_beampipe" || m_monitor == "ptacs_shielding"){
		content += ":-" + m_monitor + ".direction.fZ*" + m_monitor + ".momentum";
		content += ":" + m_monitor + ".direction.fY*" + m_monitor + ".momentum";
		content += ":" + m_monitor + ".direction.fX*" + m_monitor + ".momentum";
	}
	content += ":" + m_monitor + ".globalTime";
	content += ":-" + m_monitor + ".origin.fZ*10+7350";
	content += ":" + m_monitor + ".origin.fY*10";
	content += ":" + m_monitor + ".origin.fX*10-5790.5";

	std::string cutcont = m_monitor + ".direction.fZ>0";
	if (m_pid){
		buff.str("");
		buff.clear();
		buff<<m_monitor<<".PDGEncoding=="<<m_pid;
		cutcont += "&&" + buff.str();
	}
	if (m_monitor == "ts2_0"){
		cutcont += "&&" + m_monitor + ".origin.fX<450.15";
	}
	else if (m_monitor == "blt1"){
		cutcont += "&&" + m_monitor + ".origin.fX<340.05";
	}
	else if (m_monitor == "blt0"){
		cutcont += "&&" + m_monitor + ".origin.fX<300.05";
	}
	else if (m_monitor == "ptacs_beampipe"){
		cutcont += "&&" + m_monitor + ".origin.fZ<2844.5";
	}
	else if (m_monitor == "ptacs_shielding"){
		cutcont += "&&" + m_monitor + ".origin.fZ<75.05";
	}
	else {
		printf("Cannot recogonize this  monitor plane \"%s\"\n",m_monitor.c_str());
		return -1;
	}

	std::string optioncont = "colsize=16 precision=16";

	printf("Doing the scanning...\n");
	c->Scan(content.c_str(),cutcont.c_str(),optioncont.c_str());

	return 0;
}

void init_args()
{
	m_treeName = "tree";
	m_fileDir = "";
	m_file = "";
	m_monitor = "";
	m_pid = 0;
	nJobs = 0;
	verbose = 0;
	nEvents = 0;
	printModule = 10000;
}

void print_usage(char* prog_name)
{
	fprintf(stderr,"Usage %s [options (args)] [input files]\n",prog_name);
	fprintf(stderr,"[options]\n");
	fprintf(stderr,"\t -t\n");
	fprintf(stderr,"\t\t set tree name \n");
	fprintf(stderr,"\t -d\n");
	fprintf(stderr,"\t\t set directory fof job files\n");
	fprintf(stderr,"\t -j\n");
	fprintf(stderr,"\t\t set job number\n");
	fprintf(stderr,"\t -f\n");
	fprintf(stderr,"\t\t set full file name with directory for single file\n");
	fprintf(stderr,"\t -m\n");
	fprintf(stderr,"\t\t set monitor name\n");
	fprintf(stderr,"\t -i\n");
	fprintf(stderr,"\t\t set pid\n");
	fprintf(stderr,"\t -v\n");
	fprintf(stderr,"\t\t verbose level\n");
	fprintf(stderr,"\t -n\n");
	fprintf(stderr,"\t\t nEvent\n");
	fprintf(stderr,"\t -p\n");
	fprintf(stderr,"\t\t printModule\n");
	fprintf(stderr,"\t -h\n");
	fprintf(stderr,"\t\t Usage message.\n");
	fprintf(stderr,"[example]\n");
	fprintf(stderr,"\t\t%s -m ab -v 20 -n 100\n",prog_name);
}
