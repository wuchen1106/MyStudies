#include <iostream>
#include "MyRoot.h"

MyRoot* MyRoot::fMyRoot = 0;

MyRoot* MyRoot::GetMyRoot(){
  if ( !fMyRoot ){
    fMyRoot = new MyRoot;
  }
  return fMyRoot;
}

MyRoot::MyRoot()
{

  //about root file
  file_name = "./raw_CometPhaseI.root";
  tree_name = "t";
  std::cout<<"file_name: "<<file_name<<std::endl;
  std::cout<<"tree_name: "<<tree_name<<std::endl;

}

MyRoot::~MyRoot()
 {
   printf("~MyRoot\n");
 }

void MyRoot::OpenFile()
{
  m_file = new TFile (file_name.c_str(), "RECREATE");
}

void MyRoot::Fill(){
  m_tree->Fill();
}

void MyRoot::Write(){
  m_tree->Write();
}

void MyRoot::Close(){
  m_file->Close();
}

void MyRoot::SetBranch(){
  m_tree = new TTree(tree_name.c_str(),tree_name.c_str());

  //to indicate a single event
  m_tree->Branch("evtNum", &evtNum, "evtNum/I");
  m_tree->Branch("runNum", &runNum, "runNum/I");
}

