int getRootList(){
	TFile *file = 0;
	TTree *t = 0;
	int N = 0;
	std::stringstream buf;
	for (int i = 0; i < 50; i++ ){
		buf.str("");
		buf.clear();
		buf<<"BP2."<<i<<".root";
		file = TFile::Open(buf.str().c_str());
		t=(TTree*)file->Get("t");
		N = t->GetEntries();
		std::cout<<buf.str()<<"\t\t"<<N<<std::endl;
	}
}
