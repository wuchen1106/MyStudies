//---------------------------------------------------------------------------//
//Description: Generate histogram according to given formula
//Author: Wu Chen(wuchen@mail.ihep.ac.cn)
//Created: 9 Jan, 2012
//Modified:
//Comment: Defalt units: "mm", "rad", "MeV"
//---------------------------------------------------------------------------//

#include <iostream>
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
	double M_NEU = 939.5656; //neutron mass in MeV

	//=>About Spectrum
	double maxEk = 10; //maximum kinetic energy in MeV
	int num_bin = 500; //How many bins do you want
	double LEN_left  = 0.; //Left end. in MeV/c
	double LEN_right = sqrt( (maxEk+M_NEU)*(maxEk+M_NEU) - M_NEU*M_NEU ); //Right end. in MeV/c
	if (argc >= 2){
		buff.str("");
		buff.clear();
		buff<<argv[1];
		buff>>num_bin;
	}


	//=>Prepare histograms
	nameForH1D.push_back("LEN_hist");
	titleForH1D.push_back("Prob (p_{Neutron})");
	bin1ForH1D.push_back(num_bin);
	left1ForH1D.push_back(LEN_left);
	right1ForH1D.push_back(LEN_right);

	for ( int i = 0; i < nameForH2D.size(); i++ ){
		vecH2D.push_back(new TH2D(nameForH2D[i],titleForH2D[i],bin1ForH2D[i],left1ForH2D[i],right1ForH2D[i],bin2ForH2D[i],left2ForH2D[i],right2ForH2D[i]) );
	}

	for ( int i = 0; i < nameForH1D.size(); i++ ){
		vecH1D.push_back(new TH1D(nameForH1D[i],titleForH1D[i],bin1ForH1D[i],left1ForH1D[i],right1ForH1D[i]) );
		std::cout<<"vecH1D["<<i<<"]: "<<nameForH1D[i]<<", "<<titleForH1D[i]<<", "<<bin1ForH1D[i]<<", "<<left1ForH1D[i]<<", "<<right1ForH1D[i]<<std::endl;
	}

	//=>About Model
	//P(Ek) = exp(-Ek/E1)*sqrt(Ek/E2)
	double E1 = 1.42; //in MeV
	double E2 = 1.5*1.5; //in MeV

	//=>About Statistical
	int N0 = 0;
	int N1 = 0;
	int N2 = 0;
	int N3 = 0;

	//=>Generate the histogram
	for ( int i_bin = 0; i_bin < num_bin; i_bin++ ){
		double pa = LEN_left + (i_bin + 0.5)*( LEN_right - LEN_left )/num_bin;
		double E = sqrt( pa*pa + M_NEU*M_NEU );
		double Ek = E - M_NEU;
		double Prob = exp(-Ek/E1)*sqrt(Ek/E2);
		vecH1D[0]->Fill(pa,Prob);
	}

	//=>For output
	std::string output_filename = "output.root";
	TFile output_file(output_filename.c_str(),"recreate");

	clock_t t_END = clock();

	//#########################THEEND###############################
	std::cout<<"\n################BENTCH MARK###################"<<std::endl;
	std::cout<<"TOTAL TIME COST IS:  "<<(double)(t_END-t_START)/CLOCKS_PER_SEC*1000<<"ms"<<std::endl;
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
		//if ( i == 1 ) gPad->SetLogy(1);
		//else gPad->SetLogy(0);
		vecH1D[i]->Draw();
  	vecH1D[i]->Write();
		TString fileName = name + ".pdf";
		c->Print(fileName);
	}

	output_file.Close();
	return 0;
}
