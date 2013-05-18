#include "TFile.h"
#include "TTree.h"

class MyRoot
{
public:
  MyRoot();
   ~MyRoot();

  static MyRoot* GetMyRoot();

  void OpenFile();

  void Fill();

  void SetBranch();

  void Write();

  void Close();

  void set_file_name( std::string val ){ file_name = val; };
  void set_tree_name( std::string val ){ tree_name = val; };
  void set_evtNum( int val ){ evtNum = val; };
  void set_runNum( int val ){ runNum = val; };

private:

  static MyRoot* fMyRoot;

  //about root file
  std::string file_name;
  std::string tree_name;
  TFile* m_file;
  TTree* m_tree;

  //to indicate a single event
  int evtNum;
  int runNum;

};

