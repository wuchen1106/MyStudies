{
	TFile * f  = new TFile("/home/chen/MyWorkArea/Simulate/comet/fieldMaps/IHEP_A9_140905.root");
	TTree * t  = (TTree*)f->Get("data");
	float z,r,bz,br;
	t->SetBranchAddress("Z",&z);
	t->SetBranchAddress("R",&r);
	t->SetBranchAddress("Br",&br);
	t->SetBranchAddress("Bz",&bz);

	//=============================
	double z0 = 5952.5;

	double Lm = 2000; // mm
	double Rm = 2000; // mm
	double Xm = 900; // mm

	double iStep = 10; // mm

	double Step = 10; // mm
	//double Step = 50; // mm
	//=============================

	double iBr[201][201];
	double iBz[201][201];

	for(Long64_t iEntry = 0; iEntry<t->GetEntries(); iEntry++){
		t->GetEntry(iEntry);
		if (fabs(z-z0)>Lm/2.) continue;
		if (r<0||r>Rm) continue;
		iBr[(int)(z-z0+Lm/2.)/iStep][(int)(r/iStep)] = br;
		iBz[(int)(z-z0+Lm/2.)/iStep][(int)(r/iStep)] = bz;
	}

	for(int i = 0; i<201; i++){//z
		for(int k = 0; k<181; k++){//y
			for(int j = 0; j<181; j++){//x
	//for(int i = 0; i<41; i++){//z
	//	for(int k = 0; k<37; k++){//y
	//		for(int j = 0; j<37; j++){//x
				double xt = j*Step-Xm;
				double yt = k*Step-Xm;
				double zt = i*Step-Lm/2.;
				double bx,by,bzt,brt,phi;
				if (xt) phi = atan(yt/xt);
				else phi = 0;
				if (xt<0) phi+=3.1415926;
				r = sqrt(xt*xt+yt*yt);
				int ir,iz;
				for(ir = 0; ir<201; ir++){
					if(r<ir*iStep) break;
				}
				iz=(int)(zt+Lm/2.)/iStep;
				//std::cout<<zt<<"->"<<iz<<std::endl;
				double a = r-(ir-1)*iStep;
				double b = ir*iStep-r;
				//std::cout<<(ir-1)*iStep<<" "<<r<<" "<<ir*iStep<<", "<<a<<" "<<b<<std::endl;
				//std::cout<<"Br("<<zt<<","<<(ir-1)*iStep<<","<<(ir)*iStep<<") = ("<<iBr[iz][ir-1]<<" , "<<iBr[iz][ir]<<")"<<std::endl;
				brt=iBr[iz][ir-1]*b+iBr[iz][ir]*a;
				brt/=(a+b);
				bzt=iBz[iz][ir];
				bx=brt*cos(phi);
				by=brt*sin(phi);
				//std::cout<<zt+z0<<","<<r<<": brt = "<<iBz[iz][ir-1]<<"*"<<b<<"+"<<iBr[iz][ir]<<"*"<<a<<"/"<<a+b<<" = "<<brt<<std::endl;
				//printf("%lf %lf %lf %lf %lf %lf\n",xt/1000.,yt/1000.,zt/1000.,bx,by,bzt);
				printf("%lf %lf %lf %lf %lf %lf\n",xt/1.,yt/1.,(zt+z0)/1.,bx,by,bzt);
			}
		}
	}
}
