#include <iostream>
#include <sstream>

#include <cstdlib>
#include <math.h>

#include "Randomize.hh"

#include "TStyle.h"
#include "TCanvas.h"
#include "TFolder.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "Math/DistFunc.h"
#include "TROOT.h"

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
	int num_bin = 100; //How many bins do you want
	int num_bin4inte = 1000; //For integration
	if (argc >= 3){
		buff.str("");
		buff.clear();
		buff<<argv[2];
		buff>>num_bin;
	}

	//=>About Signal window
	double sig_up = 106;
	double sig_low = 103.8;

	//=>Get resolution
	std::string FN_res = "config/mix2_400um_2.root";
	std::string HN_res = "hsig_res_sum";
	TFile* fp_res = new TFile(FN_res.c_str());
	if (fp_res==NULL) {
		std::cout<<"ERROR: Can not find file: "<<FN_res<<"!!!"<<std::endl;
		return -1;
	}
	TH1D* h_res = (TH1D*)fp_res->Get(HN_res.c_str());
	if(h_res==NULL){
		std::cout<<"ERROR: Can not find histogram \""<<HN_res<<"\"in file : "<<FN_res<<"!!!"<<std::endl;
		return -1;
	}
	int nbin_res = h_res->GetNbinsX();
	h_res->SetTitle("P(p_{true}-p_{rec})");
	double binw_res = h_res->GetBinWidth(0);
	double res_max = h_res->GetBinLowEdge(nbin_res)+binw_res;
	//std::cout<<"res_max = "<<res_max<<std::endl;
	double res_min = h_res->GetBinLowEdge(0);
	//std::cout<<"res_min = "<<res_min<<std::endl;
	h_res->Scale(1./h_res->Integral());

	//=>Get spectrum
	std::string FN_SPEC = "config/RPC.root";
	//std::string FN_SPEC = "config/BE_00.root";
	//std::string FN_SPEC = "config/BE_01.root";
	//std::string FN_SPEC = "config/BE_02.root";
	//std::string FN_SPEC = "config/BE_03.root";
	//std::string FN_SPEC = "config/BE_04.root";
	//std::string FN_SPEC = "config/BE_05.root";
	std::string HN_SPEC = "pa";
	TFile* fp_SPEC = new TFile(FN_SPEC.c_str());
	if (fp_SPEC==NULL) {
		std::cout<<"ERROR: Can not find file: "<<FN_SPEC<<"!!!"<<std::endl;
		return -1;
	}
	TH1D* h_SPEC = (TH1D*)fp_SPEC->Get(HN_SPEC.c_str());
	if(h_SPEC==NULL){
		std::cout<<"ERROR: Can not find histogram \""<<HN_SPEC<<"\"in file : "<<FN_SPEC<<"!!!"<<std::endl;
		return -1;
	}
	int nbin_SPEC = h_SPEC->GetNbinsX();
	h_SPEC->SetTitle("P_{#mu->#gamma}(E_{#gamma})");
	double binw_SPEC = h_SPEC->GetBinWidth(0);
	double SPEC_max = h_SPEC->GetBinLowEdge(nbin_SPEC)+binw_SPEC;
	//std::cout<<"SPEC_max = "<<SPEC_max<<std::endl;
	double SPEC_min = h_SPEC->GetBinLowEdge(0);
	//std::cout<<"SPEC_min = "<<SPEC_min<<std::endl;

	//=>Prepare histograms
	double SPEC_left = SPEC_min;
	double SPEC_right = SPEC_max;
	//std::cout<<"SPEC_right = "<<SPEC_right<<std::endl;
	nameForH1D.push_back("Spectrum");
	titleForH1D.push_back("p_{a}/MeV");
	bin1ForH1D.push_back(num_bin);
	left1ForH1D.push_back(SPEC_left);
	right1ForH1D.push_back(SPEC_right);

	nameForH1D.push_back("MisMeas_hist");
	titleForH1D.push_back("P_{sig/e}(p_{e}/MeV)");
	bin1ForH1D.push_back(num_bin);
	left1ForH1D.push_back(SPEC_left);
	right1ForH1D.push_back(SPEC_right);

	nameForH1D.push_back("contribution");
	titleForH1D.push_back("contribution per captured muon");
	bin1ForH1D.push_back(num_bin);
	left1ForH1D.push_back(SPEC_left);
	right1ForH1D.push_back(SPEC_right);

	nameForH1D.push_back("TotalContribution");
	titleForH1D.push_back("Total Contribution VS reslution limit");
	bin1ForH1D.push_back(10);
	left1ForH1D.push_back(-3);
	right1ForH1D.push_back(0);

	for ( int i = 0; i < nameForH2D.size(); i++ ){
		vecH2D.push_back(new TH2D(nameForH2D[i],titleForH2D[i],bin1ForH2D[i],left1ForH2D[i],right1ForH2D[i],bin2ForH2D[i],left2ForH2D[i],right2ForH2D[i]) );
	}

	for ( int i = 0; i < nameForH1D.size(); i++ ){
		vecH1D.push_back(new TH1D(nameForH1D[i],titleForH1D[i],bin1ForH1D[i],left1ForH1D[i],right1ForH1D[i]) );
		std::cout<<"vecH1D["<<i<<"]: "<<nameForH1D[i]<<", "<<titleForH1D[i]<<", "<<bin1ForH1D[i]<<", "<<left1ForH1D[i]<<", "<<right1ForH1D[i]<<std::endl;
	}
	vecH1D.push_back(h_res);
	vecH1D.push_back(h_SPEC);
	vecH1D[0] = h_SPEC;

	//=>About Statistical
	int N0 = 0;
	int N1 = 0;
	int N2 = 0;
	int N3 = 0;

	//=>Calculate mis-measurement probability
	for ( int i = 0; i < num_bin; i++ ){
		double E = SPEC_left + ( (double)i + 0.5 )*(SPEC_right - SPEC_left)/num_bin;
		double probabilityE = 0;
		double res_min = E - sig_up;
		double res_max = E - sig_low;
		int ibinmin_res = h_res->FindBin(res_min);
		int ibinmax_res = h_res->FindBin(res_max);
		probabilityE = h_res->Integral(ibinmin_res,ibinmax_res);
		vecH1D[1]->Fill(E,probabilityE);
		//std::cout<<vecH1D[0]->GetName()<<"->Fill("<<E<<", "<<probabilityE<<")"<<std::endl;
	}

	//=>Calculate total mis-measurement probability
	double prob_tot = 0;
	std::vector<double> probs;
	for ( int i = 0; i < 10; i++ ){
		probs.push_back(0.);
	}
	for ( int i = 0; i < num_bin; i++ ){
		double E = SPEC_left + ( (double)i + 0.5 )*(SPEC_right - SPEC_left)/num_bin;
		int ibin_SPECE = vecH1D[0]->FindBin(E);
		int ibin_mis = vecH1D[1]->FindBin(E);
		double prob_SPECE = vecH1D[0]->GetBinContent(ibin_SPECE);
		double prob_mis = vecH1D[1]->GetBinContent(ibin_mis);
		for (int i = 1; i <= 10; i++){
			double res_limit = vecH1D[3]->GetBinCenter(i);
			double res_min = E - sig_up;
			double res_max = E - sig_low;
			std::cout<<"    res_min = "<<res_min
				     <<", res_max = "<<res_max
				     <<", res_limit = "<<res_limit
				     <<std::endl;
			if (res_min<res_limit) res_min = res_limit;
			double probabilityE = 0;
			if (res_min>res_max){
				probabilityE=0;
			} 
			else{
				int ibinmin_res = h_res->FindBin(res_min);
				int ibinmax_res = h_res->FindBin(res_max);
				probabilityE = h_res->Integral(ibinmin_res,ibinmax_res);
			}
			std::cout<<"     probabilityE = "<<probabilityE
				     <<std::endl;
			probs[i-1]+=prob_SPECE*probabilityE;
		}
		std::cout<<"bin["<<i
			     <<"], E = "<<E
			     <<", prob_SPECE = "<<prob_SPECE
			     <<", prob_mis = "<<prob_mis
			     <<", prob_tot = "<<prob_tot
			     <<" + "<<prob_SPECE * prob_mis
			     <<std::endl;
		prob_tot += prob_SPECE * prob_mis; 
		int ibin = vecH1D[2]->FindBin(E);
		vecH1D[2]->SetBinContent(ibin,prob_SPECE * prob_mis);
	}
	for ( int i = 1; i <= 10; i++ ){
		vecH1D[3]->SetBinContent(i,probs[i-1]);
	}
	std::cout<<"prob_tot = "<<prob_tot<<std::endl;

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
		//gPad->SetLogy(1);
		vecH1D[i]->Draw();
		vecH1D[i]->Write();
		TString fileName = name + ".pdf";
		c->Print(fileName);
	}

	output_file.Close();
	return 0;
}
