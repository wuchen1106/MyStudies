{
  TTree * t = new TTree("t", "t");
  t->ReadFile("sobottka_all", "E:count:exm:exp:eym:eyp");
  //t->Print();
  //t->Draw("count:E");

  float et, ct;
  t->SetBranchAddress("E", &et);
  t->SetBranchAddress("count", &ct);

  const int nentries = t->GetEntries();
  double e[nentries], c[nentries], ce[nentries];
  double area = 0.;
  double err = 0.;
  double area14 = 0.;
  double area25_80 = 0;
  double area45 = 0.;
  for (unsigned int i = 0; i < nentries; ++i)
  {
    t->GetEntry(i);
    e[i] = et;
    c[i] = ct;
    ce[i] = sqrt(ct);
    area += ct;
    if (et>1.4)
    {
      area14 + = ct;
    }
    if (et>2.5 && et<8)
    {
      area25_80 + = ct;
    }
    if (et>=8.0 && et<=10.0)
    {
      //err += sqrt(ct);
      //cout<<"e: "<<et<<", c: "<<ct<<", tot: "<<area<<", err: "<<err<<endl;
    }
    if (et>=4.5)
    {
      area45 += ct;
      //cout<<"e: "<<et<<", c: "<<ct<<", tot: "<<area45<<", err: "<<err<<endl;
    }
  }
      cout<<"area14: "<<area14<<", area25-80: "<<area25_80<<endl;
      cout<<"area45: "<<area45<<"frac: "<<area45/area<<endl;
  TGraphErrors *gr = new TGraphErrors(nentries, e, c,0,ce);
  gr->Draw("AP");
}
