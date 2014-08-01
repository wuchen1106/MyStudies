{
double weight;
int pid;
double count;
double px;
double py;
double pz;
int n;

TFile *_file0 = TFile::Open("CDC.mum_o75.em.g40cm10mm.005T.Vac0731_200_200_39.g4s.QBH.root");
TTree *t = (TTree*) _file0->Get("t");
t->SetBranchAddress("weight",&weight);
t->SetBranchAddress("pid",&pid);
//t->SetBranchAddress("px",&px);
//t->SetBranchAddress("py",&py);
//t->SetBranchAddress("pz",&pz);
n = t->GetEntries();
count = 0;
for ( int i = 0; i<n; i++){
	t->GetEntry(i);
//	double pa = sqrt(px*px+py*py+pz*pz);
//	if (pa<100) continue;
	if (pid==11) count+=weight;
}
printf("count for pim: %lf\n",count);

//TFile *_file0 = TFile::Open("CDC.EP_pim.em.g40cm10mm.005T.Vac0731_200_200_39.g4s.QBH.root");
//TTree *t = (TTree*) _file0->Get("t");
//t->SetBranchAddress("weight",&weight);
//t->SetBranchAddress("pid",&pid);
//t->SetBranchAddress("px",&px);
//t->SetBranchAddress("py",&py);
//t->SetBranchAddress("pz",&pz);
//n = t->GetEntries();
//count = 0;
//for ( int i = 0; i<n; i++){
//	t->GetEntry(i);
//	double pa = sqrt(px*px+py*py+pz*pz);
//	if (pa<100) continue;
//	if (pid==11) count+=weight;
//}
//printf("count for pim: %lf\n",count);
//
//_file0 = TFile::Open("CDC.EP_mum.em.g40cm10mm.005T.Vac0731_200_200_39.g4s.QBH.root");
//t = (TTree*) _file0->Get("t");
//t->SetBranchAddress("weight",&weight);
//t->SetBranchAddress("pid",&pid);
//t->SetBranchAddress("px",&px);
//t->SetBranchAddress("py",&py);
//t->SetBranchAddress("pz",&pz);
//n = t->GetEntries();
//count = 0;
//for ( int i = 0; i<n; i++){
//	t->GetEntry(i);
//	double pa = sqrt(px*px+py*py+pz*pz);
//	if (pa<100) continue;
//	if (pid==11) count+=weight;
//}
//printf("count for mum: %lf\n",count);
//
//_file0 = TFile::Open("CDC.EP_em.em.g40cm10mm.005T.Vac0731_200_200_39.g4s.QBH.root");
//t = (TTree*) _file0->Get("t");
//t->SetBranchAddress("weight",&weight);
//t->SetBranchAddress("pid",&pid);
//t->SetBranchAddress("px",&px);
//t->SetBranchAddress("py",&py);
//t->SetBranchAddress("pz",&pz);
//n = t->GetEntries();
//count = 0;
//for ( int i = 0; i<n; i++){
//	t->GetEntry(i);
//	double pa = sqrt(px*px+py*py+pz*pz);
//	if (pa<100) continue;
//	if (pid==11) count+=weight;
//}
//printf("count for em: %lf\n",count);
//
//_file0 = TFile::Open("CDC.EP_OT.em.g40cm10mm.005T.Vac0731_200_200_39.g4s.QBH.root");
//t = (TTree*) _file0->Get("t");
//t->SetBranchAddress("weight",&weight);
//t->SetBranchAddress("pid",&pid);
//t->SetBranchAddress("px",&px);
//t->SetBranchAddress("py",&py);
//t->SetBranchAddress("pz",&pz);
//n = t->GetEntries();
//count = 0;
//for ( int i = 0; i<n; i++){
//	t->GetEntry(i);
//	double pa = sqrt(px*px+py*py+pz*pz);
//	if (pa<100) continue;
//	if (pid==11) count+=weight;
//}
//printf("count for OT: %lf\n",count);
}
