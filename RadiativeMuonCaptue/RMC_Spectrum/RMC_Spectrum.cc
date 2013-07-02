#include <iostream>
#include <iomanip>
#include <sstream>

#include <cstdlib>
#include <math.h>

#include "Randomize.hh"

#include "TStyle.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "Math/DistFunc.h"

using namespace std;

int main(int argc, char** argv){

	clock_t t_START = clock();
	//##########################PRESET############################
	stringstream buff;

	//histogram
	vector<TString> nameForH2D;
	vector<TString> titleForH2D;
	vector<int>     bin1ForH2D;
	vector<double>  left1ForH2D;
	vector<double>  right1ForH2D;
	vector<int>     bin2ForH2D;
	vector<double>  left2ForH2D;
	vector<double>  right2ForH2D;
	vector<TH2D*>   vecH2D;

	vector<TString> nameForH1D;
	vector<TString> titleForH1D;
	vector<int>     bin1ForH1D;
	vector<double>  left1ForH1D;
	vector<double>  right1ForH1D;
	vector<TH1D*>   vecH1D;

	//=>About Constant
	double PI = 3.141592653589793238;
	double FSC = 1/137.03599911; //fine structure constant
	double M_MUON = 105.6584; //mass of muon in MeV
	double M_ELE = 0.510999; //mass of electron in MeV
	double M_U = 931.494061; //atomic mass unit in MeV

	//=>About Spectrum
	int num_bin = 10000; //How many bins do you want
	if (argc >= 3){
		buff.str("");
		buff.clear();
		buff<<argv[2];
		buff>>num_bin;
	}

	//=>About Atom
	int AtomIndex = 14; //default value is Al;
	if (argc >= 2){
		buff.str("");
		buff.clear();
		buff<<argv[1];
		buff>>AtomIndex;
	}
	//Read Abundance info
	std::vector<int> vec_num_p,vec_num_n;
	std::vector<double> vec_Abundance, vec_Norm;
	buff.str("");
	buff.clear();
	buff<<"Abundance_"<<AtomIndex<<".txt";
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
	std::string Mass_FileName = "Isotope_Mass.txt";
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
		dict_p.push_back(num_p);
		dict_n.push_back(num_n);
		dict_mass.push_back(mass_isotope);
	}
	fclose(fp_temp);
	//check
	for ( int i = 0; i < dict_mass.size(); i++ ){
		std::cout<<dict_p[i]<<", "<<dict_n[i]<<", "<<std::setprecision(14)<<dict_mass[i]<<std::endl;
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
		double km = M_MUON+(Mass_initial-Mass_final)*M_U-M_ELE;
		if (km>maxK) maxK = km;
		vec_kmax.push_back(km);
	}
	//check
	for ( int i = 0; i < vec_Abundance.size(); i++ ){
		std::cout<<"num_p = "<<vec_num_p[i]<<", num_n = "<<vec_num_n[i]<<", Abundance = "<<vec_Abundance[i]<<", Kmax = "<<std::setprecision(14)<<vec_kmax[i]<<std::endl;
	}

	//=>Prepare histograms
	double RMC_left = 100.;
	nameForH1D.push_back("RMC_hist");
	buff.str("");
	buff.clear();
	buff<<"R_{x} (N_{p} = "<<vec_num_p[0]<<")";
	titleForH1D.push_back(buff.str());
	bin1ForH1D.push_back(num_bin);
	left1ForH1D.push_back(0);
	right1ForH1D.push_back(maxK);
	nameForH1D.push_back("RMC_Integ_hist");
	buff.str("");
	buff.clear();
	buff<<"Integral of R_{x} (N_{p} = "<<vec_num_p[0]<<")";
	titleForH1D.push_back(buff.str());
	bin1ForH1D.push_back(num_bin);
	left1ForH1D.push_back(0);
	right1ForH1D.push_back(maxK);
	nameForH1D.push_back("hRMC");
	buff.str("");
	buff.clear();
	buff<<"R_{x} (N_{p} = "<<vec_num_p[0]<<")";
	titleForH1D.push_back(buff.str());
	bin1ForH1D.push_back(100);
	left1ForH1D.push_back(RMC_left);
	right1ForH1D.push_back(maxK);
	for ( int i = 0; i < vec_Abundance.size(); i++ ){
		buff.str("");
		buff.clear();
		buff<<"Rx_"<<vec_num_p[i]<<"_"<<vec_num_n[i];
		nameForH1D.push_back(buff.str());
		buff.str("");
		buff.clear();
		buff<<"R_{x} (N_{p} = "<<vec_num_p[i]<<" N_{n} = "<<vec_num_n[i]<<")";
		titleForH1D.push_back(buff.str());
		bin1ForH1D.push_back(num_bin);
		left1ForH1D.push_back(0);
		right1ForH1D.push_back(maxK);
	}

	for ( int i = 0; i < nameForH2D.size(); i++ ){
		vecH2D.push_back(new TH2D(nameForH2D[i],titleForH2D[i],bin1ForH2D[i],left1ForH2D[i],right1ForH2D[i],bin2ForH2D[i],left2ForH2D[i],right2ForH2D[i]) );
	}

	for ( int i = 0; i < nameForH1D.size(); i++ ){
		vecH1D.push_back(new TH1D(nameForH1D[i],titleForH1D[i],bin1ForH1D[i],left1ForH1D[i],right1ForH1D[i]) );
		std::cout<<"vecH1D["<<i<<"]: "<<nameForH1D[i]<<", "<<titleForH1D[i]<<", "<<bin1ForH1D[i]<<", "<<left1ForH1D[i]<<", "<<right1ForH1D[i]<<std::endl;
	}

	//=>About Model
	double Kmax;
	double Vav;
	double b1;
	double b0;
	double gv;
	double ga;
	double R;
	//assumptions
	gv = 1;
	ga = 8.56/6.78;
	b0 = gv*gv+3*ga;
	b1 = gv*gv-ga*ga;
	R = 1;

	//=>About Statistical
	int N0 = 0;
	int N1 = 0;
	int N2 = 0;
	int N3 = 0;

	//=>Generate the histogram
	for ( int i_iso = 0; i_iso < vec_kmax.size(); i_iso++ ){
		Kmax = vec_kmax[i_iso];
		TH1D* h1d = new TH1D("temp","temp",num_bin,0,maxK);
		for ( int i_bin = 1; i_bin <= num_bin; i_bin++ ){
			double k = (i_bin - 0.5)*maxK/num_bin;
			double Rx;
			if ( k > Kmax ){
				Rx = 0;
			}
			else{
				double x = k/Kmax;
				Rx = b0*(1-2*x+2*x*x+b1/b0/3*x*(1-x))*x*(1-x)*(1-x);
			}
			//std::cout<<"x = "<<x<<", Rx = "<<Rx<<std::endl;
			h1d->AddBinContent(i_bin,Rx);
		}
		vec_Norm.push_back(R/h1d->Integral());
	}
	for ( int i_bin = num_bin; i_bin > 0; i_bin-- ){
		double k = (i_bin - 0.5)*maxK/num_bin;
		double Rx_tot = 0;
		for ( int i_iso = 0; i_iso < vec_kmax.size(); i_iso++ ){
			Kmax = vec_kmax[i_iso];
			double Rx;
			if ( k > Kmax ){
				Rx = 0;
			}
			else{
				double x = k/Kmax;
				Rx = b0*(1-2*x+2*x*x+b1/b0/3*x*(1-x))*x*(1-x)*(1-x)*vec_Norm[i_iso];
			}
			Rx_tot += Rx*vec_Abundance[i_iso];
			//std::cout<<"x = "<<x<<", Rx = "<<Rx<<std::endl;
			vecH1D[i_iso+3]->AddBinContent(i_bin,Rx);
		}
		vecH1D[0]->AddBinContent(i_bin,Rx_tot);
		double Rx_integral = vecH1D[0]->Integral();
		vecH1D[1]->AddBinContent(i_bin,Rx_integral);
	}
	for ( int i_bin = 100; i_bin > 0; i_bin-- ){
		double k = RMC_left + (i_bin - 0.5)*(maxK-RMC_left)/100;
		double Rx_tot = 0;
		for ( int i_iso = 0; i_iso < vec_kmax.size(); i_iso++ ){
			Kmax = vec_kmax[i_iso];
			double Rx;
			if ( k > Kmax ){
				Rx = 0;
			}
			else{
				double x = k/Kmax;
				Rx = b0*(1-2*x+2*x*x+b1/b0/3*x*(1-x))*x*(1-x)*(1-x)*vec_Norm[i_iso];
			}
			Rx_tot += Rx*vec_Abundance[i_iso];
		}
		//std::cout<<"k = "<<k<<", Rx_tot = "<<Rx_tot<<std::endl;
		vecH1D[2]->AddBinContent(i_bin,Rx_tot);
	}

	//=>For output
	std::string output_filename = "output.root";
	TFile output_file(output_filename.c_str(),"recreate");

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

	gStyle->SetPalette(1);
	gStyle->SetOptStat(0);
	//  gStyle->SetTitleW(0.99);
	//  gStyle->SetTitleH(0.08);

	for ( int i = 0; i < vecH2D.size(); i++ ){
		TString name = vecH2D[i]->GetName();
		TCanvas* c = new TCanvas(name);
		//gPad->SetLogy(1);
		vecH2D[i]->Draw("COLZ");
		vecH2D[i]->Write();
		TString fileName = name + ".pdf";
		c->Print(fileName);
	}

	for ( int i = 0; i < vecH1D.size(); i++ ){
		TString name = vecH1D[i]->GetName();
		TCanvas* c = new TCanvas(name);
		if ( i == 1 ) gPad->SetLogy(1);
		else gPad->SetLogy(0);
		vecH1D[i]->Draw();
		vecH1D[i]->Write();
		TString fileName = name + ".pdf";
		c->Print(fileName);
	}

	output_file.Close();
	return 0;
}
