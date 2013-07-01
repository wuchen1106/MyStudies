#include <iostream>
#include <sstream>

//#include "MyRoot.h"
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

	//=>About Simulation
	int num_evt = 1000; //How many events do you want
	double ini_theta = 0;
	if (argc < 3){
		return -1;
	}
	buff.str("");
	buff.clear();
	buff<<argv[1];
	buff>>num_evt;
	buff.str("");
	buff.clear();
	buff<<argv[2];
	buff>>ini_theta;
	int nsteps = 100;
	std::cout<<"theta = "<<ini_theta<<std::endl;

	//=>About Geometry
	double thickness = 3.4; // mm
	double r_disk = 10;// mm
	double Rmin = 55.8;
	double pa = 105;
	double space = 50;// mm

	//=>About Particle
	//TString Lambda_fileName = "Lambda_Al_Ele.root";
	//TString Lambda_histName = "p_Lambda";
	//TFile Lambda_file(Lambda_fileName);
	//if (!Lambda_file){
	//	std::cout<<"Cannot find file \""<<Lambda_fileName<<"\""<<std::endl;
	//	return 1;
	//}
	//TH1D* Lambda_hist = (TH1D*)Lambda_file.Get(Lambda_histName);
	//Lambda_file.Close();
	double mass = 0.510999; //MeV Mass
	double Ek0 = 104.49; //MeV Kinetic Energy
	double Lambda = 5011.24; //mm ...
	double RL = 88.9304; //mm Radiative Length
	double deltaE = 0; //MeV Total Energy loss through total length //CURRENTLY NOT SUPPORTED!!!
	double charge = -1; //e

	//=>About Material
	int Zeff = 13; //Al

	//=>About Model
	double HL = 13.6; //MeV
	double tauSmall = 1e-16;
	double theta0Max = PI/6; //rad
	double b = 1.;

	//=>About Statistical
	int N0 = 0;
	int N1 = 0;
	int N2 = 0;
	int N3 = 0;

	//=>For output
	std::string output_filename = "output.root";
	TFile output_file(output_filename.c_str(),"recreate");

	clock_t t_PRESET = clock();

	//###########################INITIALIZING#######################
	double trackL = thickness/cos(ini_theta)/nsteps;
	//=>Useful variables
	double stepL = trackL/nsteps;
	//std::cout<<"stepL = "<<stepL<<std::endl;
	double tau = stepL/Lambda;
	//std::cout<<"tau = "<<tau<<std::endl;
	double y = log(tau); //IMPORTANT VALUE!!!
	//std::cout<<"y = "<<y<<std::endl;

	//=>Independent parameters: p,q,a,b,d,u0
	//==>a:
	double Ek1 = Ek0 - deltaE/nsteps;
	double BetaP0 = Ek0*(Ek0+2*mass)/(Ek0+mass); //beta*p = p^2/E = (E^2-m^2)/E = ((Ek0+m)^2-m^2)/(Ek0+m) = Ek0(Ek0+2*m)/(Ek0+m)
	//std::cout<<"BetaP0 = "<<BetaP0<<std::endl;
	double BetaP1 = Ek1*(Ek1+2*mass)/(Ek1+mass); //beta*p = p^2/E = (E^2-m^2)/E = ((EK1+m)^2-m^2)/(EK1+m) = EK1(EK1+2*m)/(EK1+m)
	//std::cout<<"BetaP1 = "<<BetaP1<<std::endl;
	double BetaP = sqrt(BetaP0*BetaP1);
	//std::cout<<"BetaP = "<<BetaP<<std::endl;
	double lnZ = log(Zeff);
	//std::cout<<"lnZ = "<<lnZ<<std::endl;
	double CT1 = (1. - 8.7780e-2/Zeff)*(0.87 + 0.03*lnZ);                   
	//std::cout<<"CT1 = "<<CT1<<std::endl;
	double CT2 = (4.0780e-2 + 1.7315e-4*Zeff)*(0.87 + 0.03*lnZ);              
	//std::cout<<"CT2 = "<<CT2<<std::endl;
	double theta0 = HL*fabs(charge)*sqrt(stepL/RL)/BetaP*(CT1+CT2*log(stepL/RL));
	//std::cout<<"theta0 = "<<theta0<<std::endl;
	double a = 1/(2*sin(theta0/2)); a *= a;
	//std::cout<<"a = "<<a<<std::endl;

	//==>b:
	//Independent! Should be set in PRESET section;

	//==>d:
	double Z13 = exp(lnZ/3.);
	double tau16 = exp(log(tau)/6);
	double x = log(stepL/tau/RL);
	double coeff1  = 2.3785    - Z13*(4.1981e-1 - Z13*6.3100e-2);
	double coeff2  = 4.7526e-1 + Z13*(1.7694    - Z13*3.3885e-1);
	double coeff3  = 2.3683e-1 - Z13*(1.8111    - Z13*3.2774e-1);
	double coeff4  = 1.7888e-2 + Z13*(1.9659e-2 - Z13*2.6664e-3);
	double d = coeff1+tau16*(coeff2+coeff3*tau16)+coeff4*x;

	//==>u0:
	double u0 = b - d/a;
	double acu0 = acos(u0);

	//==>p:
	double C1 = a/(1-exp(a*(u0-1)));
	//std::cout<<"C1 = "<<C1<<std::endl;
	double b1 = b + 1;
	//std::cout<<"b1 = "<<b1<<std::endl;
	double bu = b - u0;
	//std::cout<<"bu = "<<bu<<std::endl;
	double d1 = d - 1;
	//std::cout<<"d1 = "<<d1<<std::endl;
	double b1d1 = exp(log(b1)*d1);
	//std::cout<<"b1d1 = "<<b1d1<<std::endl;
	double bud1 = exp(log(bu)*d1);
	//std::cout<<"bud1 = "<<bud1<<std::endl;
	double b1d = exp(log(b1)*d);
	//std::cout<<"b1d = "<<b1d<<std::endl;
	double bud = exp(log(bu)*d);
	//std::cout<<"bud = "<<bud<<std::endl;
	double expau0 = exp(a*(u0-1));
	//std::cout<<"expau0 = "<<expau0<<std::endl;
	double C2 = d1*b1d1/(b1d1/bud1-1);
	//std::cout<<"C2 = "<<C2<<std::endl;
	double p = C2/bud/(C1*expau0+C2/bud);
	//std::cout<<"p = "<<p<<std::endl;

	//==>q:
	double xmean = exp(log(1-FSC*stepL)/FSC/Lambda);
	//std::cout<<"xmean = "<<xmean<<std::endl;
	double u1mean = u0 + (1-u0)/(1-expau0) - 1/a;
	//std::cout<<"u1mean = "<<u1mean<<std::endl;
	double u2mean = (1-b-d+d1*(u0+1)/(1-bud1/b1d1))/(d-2);
	//std::cout<<"u2mean = "<<u2mean<<std::endl;
	double q = xmean/(p*u1mean+(1-p)*u2mean);
	//std::cout<<"q = "<<q<<std::endl;

	//=>Distribution Function:
	double C3 = 0.5;

	std::cout<<"=========================================="<<std::endl;
	std::cout<<"Parameters:"<<std::endl;
	std::cout<<"y = "<<y<<std::endl;
	std::cout<<"q = "<<q<<std::endl;
	std::cout<<"p = "<<p<<std::endl;
	std::cout<<"a = "<<a<<std::endl;
	std::cout<<"b = "<<b<<std::endl;
	std::cout<<"d = "<<d<<std::endl;
	std::cout<<"u0 = "<<u0<<std::endl;
	std::cout<<"=========================================="<<std::endl;

	//=>histogram
	buff.str("");
	buff.clear();
	buff<<" (E_{k}="<<Ek0<<"MeV, L_{track}="<<trackL<<"mm, n_{stpes}="<<nsteps<<")";
	TString suffexT = buff.str();
	TString suffexN = buff.str();
	TString TitleOfP("P(#theta)");
	TString TitleOfP1("P1(#theta)");
	TString TitleOfP2("P2(#theta)");
	TString TitleOfP3("P3(#theta)");
	TString NameOfP("P_theta");
	TString NameOfP1("P1_theta");
	TString NameOfP2("P2_theta");
	TString NameOfP3("P3_theta");
	Int_t bin0 = 1000;
	Double_t left0 = 0;
	Double_t right0 = PI;

	TString TitleOfViriable1D2("#theta/rad");
	TString NameOfViriable1D2("theta");
	Int_t bin2 = 200;
	Double_t left2 = 0;
	Double_t right2 = PI;

	TH1D* h1P = new TH1D(NameOfP+suffexN,TitleOfP+suffexT,bin0,left0,right0);
	TH1D* h1P1 = new TH1D(NameOfP1+suffexN,TitleOfP1+suffexT,bin0,left0,acu0);
	TH1D* h1P2 = new TH1D(NameOfP2+suffexN,TitleOfP2+suffexT,bin0,acu0,right0);
	TH1D* h1P3 = new TH1D(NameOfP3+suffexN,TitleOfP3+suffexT,bin0,left0,right0);

	TH1D* h1D2 = new TH1D(NameOfViriable1D2+suffexN,TitleOfViriable1D2+suffexT,bin2,left2,right2);

	vecH1D.push_back(h1P);
	vecH1D.push_back(h1P1);
	vecH1D.push_back(h1P2);
	vecH1D.push_back(h1P3);
	vecH1D.push_back(h1D2);

	for ( int i = 0; i < bin0; i++ ){
		double theta = left0 + (right0 - left0)*(i+0.5)/bin0;
		double u = cos(theta);
		double g1 = (u<u0?0:sin(theta)*C1*exp(-a*(1-u)));
		double g2 = (u>u0?0:sin(theta)*C2/exp(log(b-u)*d));
		double g3 = sin(theta)*C3;
		h1P3->Fill(theta,g3);
		g1 *= q*p;
		g2 *= q*(1-p);
		g3 *= 1-q;
		double g = g1+g2+g3;
		h1P->Fill(theta,g);
	}

	for ( int i = 0; i < bin0; i++ ){
		double theta = left0 + (acu0 - left0)*(i+0.5)/bin0;
		double u = cos(theta);
		double g1 = sin(theta)*C1*exp(-a*(1-u));
		h1P1->Fill(theta,g1);
	}

	for ( int i = 0; i < bin0; i++ ){
		double theta = acu0 + (right0 - acu0)*(i+0.5)/bin0;
		double u = cos(theta);
		double g2 = sin(theta)*C2/exp(log(b-u)*d);
		h1P2->Fill(theta,g2);
	}

	//###########################GENERATEEVENTS#######################
	double u[2], phi, theta;
	double z, cp, st1, st2, sp, rp;
	for ( int i = 0; i < num_evt; i++ ){
		u[0] = 1; //Initial Angle is 0
		for ( int iStep = 0; iStep < nsteps; iStep++ ){
			if(G4UniformRand() < q)
			{
				if(G4UniformRand() < p){
					double R = G4UniformRand();
					u[1] = 1.+log(expau0+R*(1-expau0))/a;
				}
				else{
					double R = G4UniformRand();
					u[1] = b-b1/exp(log(1+(b1d1/bud1-1)*R)/d1);
				}
			}
			else{
				u[1] = 2.*G4UniformRand()-1.;
			}
			phi = 2*PI*G4UniformRand();
			cp = cos(phi);
			st1 = sqrt(1-u[0]*u[0]);
			st2 = sqrt(1-u[1]*u[1]);
			sp = sin(phi);
			z = u[0]*u[1] - st1*st2*cp;
			u[0] = z;
		}
		theta = acos(u[0]);
		h1D2->Fill(theta);
		double xp = sin(ini_theta)+sin(theta)*cos(phi)*cos(ini_theta);
		double yp = sin(theta)*sin(phi);
		double zp = cos(ini_theta)-sin(theta)*cos(phi)*sin(ini_theta);
		double thetap = acos(zp);
		double phip = atan(yp/xp);
		double pos_x = 0., pos_y = 0., pos_z = 0.;
		bool pos_OK = false;
		while( !pos_OK ){
			pos_x=(G4UniformRand()-0.5)*2;
			pos_y=(G4UniformRand()-0.5)*2;
			if ( pos_x*pos_x + pos_y*pos_y <= 1 ) pos_OK = true;
		}
		pos_x *= r_disk;
		pos_y *= r_disk;
		double r = 35*sin(thetap);
		double l = Rmin - r;
		double cosphimax = (r_disk*r_disk+l*l-r*r)/2/r_disk/l;
		double prob;
		int idisk = G4UniformRand()*17;
		z = (idisk - 9)*space;
		if (cos(phip)>cosphimax){
			N1++;
			std::cout<<" "<<pos_x<<" "<<pos_y<<" "<<pos_z
				<<" "<<xp*pa<<" "<<yp*pa<<" "<<zp*pa
				<<std::endl;
		}
		N0++;
	}

	clock_t t_GENERATEEVENTS = clock();

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
		gPad->SetLogy(1);
		vecH1D[i]->Draw();
		vecH1D[i]->Write();
		TString fileName = name + ".pdf";
		c->Print(fileName);
	}

	output_file.Close();
	return 0;
}
