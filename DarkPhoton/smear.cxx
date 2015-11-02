#include <iostream>
#include <string>
#include <vector>
#include "TRandom.h"
#include "TFile.h"
#include "TChain.h"

int main(int argc, char** argv){
	double m_e = 0.511e-3;
	double Ebeam = 1.89+m_e; // MeV
	double E0 = Ebeam+m_e;
	double pbeam = sqrt(Ebeam*Ebeam-m_e*m_e);
	TChain* in_t = new TChain("tree");
	std::vector<Long64_t> vN;
	for (int i = 0; i<200; i++){
		in_t->Add(Form("/scratchfs/bes/wuc/MyWorkArea/Data/thickT/%d_job0.raw",i));
		vN.push_back(in_t->GetEntries());
	}
	std::vector<double> * ipx=0;
	std::vector<double> * ipy=0;
	std::vector<double> * ipz=0;
	std::vector<int> * iptid=0;
	std::vector<std::string> * iprocess=0;
	std::vector<std::string> * ioprocess=0;
	int ientry;
	int ifile;
	in_t->SetBranchAddress("M_px",&ipx);
	in_t->SetBranchAddress("M_py",&ipy);
	in_t->SetBranchAddress("M_pz",&ipz);
	in_t->SetBranchAddress("McTruth_process",&iprocess);
	in_t->SetBranchAddress("McTruth_ptid",&iptid);
	in_t->SetBranchAddress("M_oprocess",&ioprocess);

	TFile * out_f = new TFile("output.root","RECREATE");
	double Em;
	double theta;
	int Nann;
	TTree * out_t = new TTree("t","t");
	out_t->Branch("ientry",&ientry);
	out_t->Branch("ifile",&ifile);
	out_t->Branch("Em",&Em);
	out_t->Branch("theta",&theta);
	out_t->Branch("Nann",&Nann);

	TRandom random;
	double px,py,pz;
	Long64_t N = in_t->GetEntries();
	std::cout<<"Going to process "<<N<<" events"<<std::endl;
	for (Long64_t i = 0; i<N; i++){
		if (i%10000==0) std::cout<<((double)i)/N*100<<"%..."<<std::endl;
		in_t->GetEntry(i);
		ientry = i;
		for (ifile = 0; ifile<200; ifile++){
			if (i<vN[ifile]) break;
			else ientry=i-vN[ifile];
		}
		int NannTemp = 0;
		for (int j = 0; j<iprocess->size(); j++){
			if ((*iprocess)[j]=="annihil"&&(*iptid)[j]==1) NannTemp++;
		}
		for (int j = 0; j<ipx->size(); j++){
			if ((*ioprocess)[j]=="annihil") Nann = NannTemp;
			else if ((*ioprocess)[j]!="eBrem") continue;
			else Nann = 0;

			px = (*ipx)[j];
			py = (*ipy)[j];
			pz = (*ipz)[j];
			theta = atan(sqrt(px*px+py*py)/pz);
			Em = sqrt(px*px+py*py+pz*pz)*1000;

			Em=Em*(1+random.Gaus(0,((2000-Em)/1800.*3.7+0.2)/100));
			theta+=random.Gaus(0,((2000-Em)/1800.*1.3+1.2))/4000;

			out_t->Fill();
		}
	}
	out_t->Write();
	out_f->Close();
}
