{
	// MeV, mm

	double MPI = 139.570;
	double MMU = 105.6584;
	double ME  = 0.510999;

	TH1D *hmu2e = new TH1D("hmu2e","hmu2e",100,0,100);
	for (int ibin = 1; ibin<=100; ibin++){
		std::cout<<"ibin = "<<ibin<<std::endl;
		TLorentzVector tlmu(0,0,ibin,sqrt(ibin*ibin+MMU*MMU));
		double p = (MMU*MMU-ME*ME)/(2*MMU);
		TLorentzVector tle(0,0,p,sqrt(p*p+ME*ME));
//		std::cout<<"("<<tle.Px()
//			     <<","<<tle.Py()
//			     <<","<<tle.Pz()
//			     <<","<<tle.E()
//			     <<")"
//			     <<std::endl;
		tle.Boost(tlmu.BoostVector());
//		std::cout<<"("<<tle.Px()
//			     <<","<<tle.Py()
//			     <<","<<tle.Pz()
//			     <<","<<tle.E()
//			     <<")"
//			     <<std::endl;
		hmu2e->SetBinContent(ibin,tle.P());
	}

	TH1D *hpi2mu = new TH1D("hpi2mu","hpi2mu",100,0,100);
	for (int ibin = 1; ibin<=100; ibin++){
		std::cout<<"ibin = "<<ibin<<std::endl;
		TLorentzVector tlpi(0,0,ibin,sqrt(ibin*ibin+MPI*MPI));
		double p = (MPI*MPI-MMU*MMU)/(2*MPI);
		TLorentzVector tlmu(0,0,p,sqrt(p*p+MMU*MMU));
//		std::cout<<"("<<tlmu.Px()
//			     <<","<<tlmu.Py()
//			     <<","<<tlmu.Pz()
//			     <<","<<tlmu.E()
//			     <<")"
//			     <<std::endl;
		tlmu.Boost(tlpi.BoostVector());
//		std::cout<<"("<<tlmu.Px()
//			     <<","<<tlmu.Py()
//			     <<","<<tlmu.Pz()
//			     <<","<<tlmu.E()
//			     <<")"
//			     <<std::endl;
		hpi2mu->SetBinContent(ibin,tlmu.P());
	}
}
