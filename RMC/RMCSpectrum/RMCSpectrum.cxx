#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <math.h>

#include "globals.hh"

#include "TH1D.h"
#include "TH2D.h"

#include "MyRootInterface.hxx"

using namespace std;

int main(int argc, char** argv){

	clock_t t_START = clock();
	//##########################PRESET############################
	stringstream buff;
	MyRootInterface *fMyRootInterface = new MyRootInterface(10);
	fMyRootInterface->set_OutputDir("result");
	TH1D *h1d_temp=0;
	int bin_temp = 0;

	//=>About Constant
	double PI = 3.141592653589793238;
	double FSC = 1/137.03599911; //fine structure constant
	double M_MUON = 105.6584*MeV; //mass of muon in MeV
	double M_ELE = 0.510999*MeV; //mass of electron in MeV
	double M_U = 931.494061*MeV; //atomic mass unit in MeV
	double E_BIND = 460.4*keV; //Binding energy of muon in Aluminum atom

	//=>About Atom
	int AtomIndex = 13; //default value is Al;
	if (argc >= 2){
		buff.str("");
		buff.clear();
		buff<<argv[1];
		buff>>AtomIndex;
	}

	//##########################Material############################
	//Read Abundance info
	std::vector<int> vec_num_p,vec_num_n;
	std::vector<double> vec_Abundance;
	buff.str("");
	buff.clear();
	buff<<"config/Abundance_"<<AtomIndex<<".txt";
	std::string Abundance_fileName = buff.str();
	FILE* fp_temp = fopen(Abundance_fileName.c_str(),"r");
	if (fp_temp==NULL) {
		std::cout<<"ERROR: Can not find file: "<<Abundance_fileName<<"!!!"<<std::endl;
		return -1;
	}
	char line[128];
	int num_p,num_n;
	double Abundance;
	fgets(line,sizeof(line),fp_temp); //comment line
	while (fgets(line,sizeof(line),fp_temp)) {
		sscanf(line,"%i %i %lf",&num_p,&num_n,&Abundance);
		vec_num_p.push_back(num_p);
		vec_num_n.push_back(num_n);
		vec_Abundance.push_back(Abundance);
	}
	fclose(fp_temp);
	//check
	double total_abundance = 0;
	for ( int i = 0; i < vec_Abundance.size(); i++ ){
		total_abundance += vec_Abundance[i];
	}
	if ( total_abundance == 0 ){
		std::cout<<"total_abundance is 0!!!"<<std::endl;
		return -1;
	}
	if ( total_abundance != 1 ){
		std::cout<<"total_abundance is not 1!!!\nWill be normalized to 1!!!"<<std::endl;
		for ( int i = 0; i < vec_Abundance.size(); i++ ){
			vec_Abundance[i] /= total_abundance;
		}
	}
	for ( int i = 0; i < vec_Abundance.size(); i++ ){
		std::cout<<"num_p = "<<vec_num_p[i]<<", num_n = "<<vec_num_n[i]<<", Abundance = "<<vec_Abundance[i]<<std::endl;
	}
	//Read Mass Info
	std::vector<int> dict_p;
	std::vector<int> dict_n;
	std::vector<double> dict_mass;
	std::string Mass_FileName = "config/Isotope_Mass.txt";
	fp_temp = fopen(Mass_FileName.c_str(),"r");
	if (fp_temp==NULL) {
		std::cout<<"ERROR: Can not find file: "<<Abundance_fileName<<"!!!"<<std::endl;
		return -1;
	}
	line[128];
	double mass_isotope;
	fgets(line,sizeof(line),fp_temp); //comment line
	while (fgets(line,sizeof(line),fp_temp)) {
		sscanf(line,"%i %i %lf",&num_p,&num_n,&mass_isotope);
		mass_isotope *= M_U; // data in this file is in unit mass
		dict_p.push_back(num_p);
		dict_n.push_back(num_n);
		dict_mass.push_back(mass_isotope);
	}
	fclose(fp_temp);
	//check
	for ( int i = 0; i < dict_mass.size(); i++ ){
		std::cout<<"Np("<<dict_p[i]<<"), Nn("<<dict_n[i]<<"), Atom Mass = "<<std::setprecision(14)<<dict_mass[i]/M_U<<" u"<<std::endl;
	}
	//Get Kmax
	std::vector<double> vec_kmax;
	double maxK = 0;
	for ( int i = 0; i < vec_Abundance.size(); i++ ){
		double Mass_initial = -1;
		double Mass_final = -1;
		for ( int j = 0; j < dict_mass.size(); j++ ){
			if ( dict_p[j] == vec_num_p[i] && dict_n[j] == vec_num_n[i] ){
				Mass_initial = dict_mass[j];
			}
			else if ( dict_p[j] == vec_num_p[i] - 1  && dict_n[j] == vec_num_n[i] + 1 ){
				Mass_final = dict_mass[j];
			}
		}
		if ( Mass_initial == -1 || Mass_final == -1 ){
			std::cout<<"Cannot find mass of this atom in "<<Mass_FileName<<std::endl;
			return -1;
		}
		double kmi = M_MUON+(Mass_initial-Mass_final)-M_ELE - E_BIND;
		double km = kmi*(1-kmi/(Mass_final-(vec_num_p[i]-1)*M_ELE+kmi)/2);
		std::cout<<"km = "<<kmi<<"*(1-"<<kmi<<"/("<<Mass_final<<"-"<<(vec_num_p[i]-1)<<"*"<<M_ELE<<"+"<<kmi<<")/2) = "<<km<<std::endl;
		if (km>maxK) maxK = km;
		vec_kmax.push_back(km);
	}
	//check
	for ( int i = 0; i < vec_Abundance.size(); i++ ){
		std::cout<<"num_p = "<<vec_num_p[i]<<", num_n = "<<vec_num_n[i]<<", Abundance = "<<vec_Abundance[i]<<", Kmax = "<<std::setprecision(14)<<vec_kmax[i]/MeV<<" MeV"<<std::endl;
	}

	//##########################Prepare histograms############################
	fMyRootInterface->read("input");
	fMyRootInterface->init();

	//=>Get histograms parameters
	// RMC
	int ihist_RMC = fMyRootInterface->get_TH1D_index("RMC");
	if (ihist_RMC==-1){
		std::cout<<"Cannnot find RMC! Please add it in MyRootInterface"<<std::endl;
		return -1;
	}
	// get useful info
	std::string  xName = fMyRootInterface->get_xNameForH1D(ihist_RMC);
	std::string  yName = fMyRootInterface->get_yNameForH1D(ihist_RMC);
	int  bin1 = fMyRootInterface->get_bin1ForH1D(ihist_RMC);
	double  left1 = fMyRootInterface->get_left1ForH1D(ihist_RMC);
	double  right1 = fMyRootInterface->get_right1ForH1D(ihist_RMC);
	double  minx = fMyRootInterface->get_minxForH1D(ihist_RMC);
	double  miny = fMyRootInterface->get_minyForH1D(ihist_RMC);
	int  color = fMyRootInterface->get_colorForH1D(ihist_RMC);
	int  compare = fMyRootInterface->get_compareForH1D(ihist_RMC);
	int  xlog = fMyRootInterface->get_xlogForH1D(ihist_RMC);
	int  ylog = fMyRootInterface->get_ylogForH1D(ihist_RMC);
	int  marker = fMyRootInterface->get_markerForH1D(ihist_RMC);
	double  norm = fMyRootInterface->get_normForH1D(ihist_RMC);
	std::string  drawOpt = fMyRootInterface->get_drawOptForH1D(ihist_RMC);
	// integrated RMC, should be the same with RMC
	int ihist_RMC_integral = fMyRootInterface->get_TH1D_index("RMC_integral");
	if (ihist_RMC_integral==-1){
		std::cout<<"Cannnot find RMC_integral! Please add it in MyRootInterface"<<std::endl;
		return -1;
	}
	// hRMC: tail component
	int ihist_hRMC = fMyRootInterface->get_TH1D_index("hRMC");
	if (ihist_hRMC==-1){
		std::cout<<"Cannnot find hRMC! Please add it in MyRootInterface"<<std::endl;
		return -1;
	}
	int bin1_hRMC = fMyRootInterface->get_bin1ForH1D(ihist_hRMC);
	int left1_hRMC = fMyRootInterface->get_left1ForH1D(ihist_hRMC);
	int right1_hRMC = fMyRootInterface->get_right1ForH1D(ihist_hRMC);

	//=>Add more histograms
	std::vector<int> ihist_iso;
	for ( int i = 0; i < vec_Abundance.size(); i++ ){
		buff.str("");
		buff.clear();
		buff<<"RMC_"<<vec_num_p[i]<<"_"<<vec_num_n[i];
		std::string name = buff.str();
		buff.str("");
		buff.clear();
		buff<<"Radiative Muon Capture Spectrum for N_{p} = "<<vec_num_p[i]<<" N_{n} = "<<vec_num_n[i];
		std::string title = buff.str();
		fMyRootInterface->add_TH1D(name,title,xName,yName,bin1,left1,right1,minx,miny,color,compare,xlog,ylog,marker,norm,drawOpt);
		ihist_iso.push_back(fMyRootInterface->get_TH1D_size()-1);
	}

	//##########################Calculation############################
	//=>About Model
	double Kmax;
	double Vav;
	double b1;
	double b0;
	double gv;
	double ga;
	//assumptions
	gv = 1;
	ga = 8.56/6.78;
	b0 = gv*gv+3*ga;
	b1 = gv*gv-ga*ga;

	//=>About Statistical
	int N0 = 0;
	int N1 = 0;
	int N2 = 0;
	int N3 = 0;

	//=>Generate the histogram
	// histogram for each isotope
	for ( int i_iso = 0; i_iso < vec_kmax.size(); i_iso++ ){
		Kmax = vec_kmax[i_iso];
		h1d_temp = fMyRootInterface->get_TH1D(ihist_iso[i_iso]);
		bin_temp = fMyRootInterface->get_bin1ForH1D(ihist_iso[i_iso]);
		for ( int i_bin = 1; i_bin <= bin_temp; i_bin++ ){
			double k = (h1d_temp->GetBinCenter(i_bin))*MeV; // this plot is in MeV
			double Rx;
			if ( k > Kmax ){
				Rx = 0;
			}
			else{
				double x = k/Kmax;
				Rx = b0*(1-2*x+2*x*x+b1/b0/3*x*(1-x))*x*(1-x)*(1-x);
			}
			//std::cout<<"ibin = "<<i_bin<<", k = "<<k/MeV<<"MeV, Rx = "<<Rx<<std::endl;
			h1d_temp->SetBinContent(i_bin,Rx);
		}
		h1d_temp->Scale(1/h1d_temp->Integral());
	}
	// histogram for RMC and RMC_integral
	h1d_temp = fMyRootInterface->get_TH1D(ihist_RMC);
	bin_temp = fMyRootInterface->get_bin1ForH1D(ihist_RMC);
	for ( int i_bin = bin1; i_bin > 0; i_bin-- ){
		double Rx_tot = 0;
		double Rx = 0;
		double k = (h1d_temp->GetBinCenter(i_bin))*MeV; // this plot is in MeV
		for ( int i_iso = 0; i_iso < vec_kmax.size(); i_iso++ ){
			Rx = fMyRootInterface->get_TH1D(ihist_iso[i_iso])->GetBinContent(i_bin);
			Rx_tot += Rx*vec_Abundance[i_iso];
			fMyRootInterface->get_TH1D(ihist_iso[i_iso])->SetBinContent(i_bin,Rx);
		}
		//std::cout<<"ibin = "<<i_bin<<", k = "<<k/MeV<<", Rx = "<<Rx<<", Rx_tot = "<<Rx_tot<<std::endl;
		fMyRootInterface->get_TH1D(ihist_RMC)->SetBinContent(i_bin,Rx_tot);
		double Rx_integral = fMyRootInterface->get_TH1D(ihist_RMC)->Integral();
		fMyRootInterface->get_TH1D(ihist_RMC_integral)->SetBinContent(i_bin,Rx_integral);
	}
	// histogram for hRMC
	h1d_temp = fMyRootInterface->get_TH1D(ihist_hRMC);
	bin_temp = fMyRootInterface->get_bin1ForH1D(ihist_hRMC);
	for ( int i_bin = bin_temp; i_bin > 0; i_bin-- ){
		double k = (h1d_temp->GetBinCenter(i_bin))*MeV; // this plot is in MeV
		double Rx_tot = 0;
		for ( int i_iso = 0; i_iso < vec_kmax.size(); i_iso++ ){
			int i_bin_iso = fMyRootInterface->get_TH1D(ihist_iso[i_iso])->FindBin(k/MeV);
			double Rx = fMyRootInterface->get_TH1D(ihist_iso[i_iso])->GetBinContent(i_bin_iso);
			Rx_tot += Rx*vec_Abundance[i_iso];
		}
		fMyRootInterface->get_TH1D(ihist_hRMC)->SetBinContent(i_bin,Rx_tot);
	}

	//=>For output
	clock_t t_END = clock();
	//#########################THEEND###############################
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

	fMyRootInterface->dump();
	return 0;
}
