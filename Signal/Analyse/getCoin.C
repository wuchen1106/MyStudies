{
	TRandom rand;

	//// A
	//double d1 = 0.5;
	//double d2 = 1;
	//double d3 = 0;
	// B
	double d1 = 0.5/45*55;
	double d2 = 1./45*55;
	double d3 = 0;
	//// BL
	//double d1 = 1;
	//double d2 = 3.5;
	//double d3 = 0;
	//// HS,HST
	//double d1 = 0.5*1.14;
	//double d2 = 1;
	//double d3 = 2;
	//// C
	//double d1 = 0.5*1.14;
	//double d2 = 1;
	//double d3 = 0;
	//// D
	//double d1 = 0.5/45*90*1.14;
	//double d2 = 1./45*90;
	//double d3 = 0;

	int N1hits = 19*d1/0.5;
	int N2hits = 192*d2/1;
	int N3hits = N1hits/d1*d3;
	int Npulse = 1e4;
	double twin = 1140-200;
	double tcoin = 10;
	double t1[64];
	double t2[64];
	double t3[64];
	TH1I * h = new TH1I("h","h",6,0,3);
	for ( int i = 0; i<Npulse; i++ ){
		if (i%100==0) std::cout<<"i = "<<i<<std::endl;
		for (int j = 0; j<64; j++){
			t1[j] = -1;
			t2[j] = -1;
			t3[j] = -1;
		}
		for ( int j = 0; j<N1hits; j++ ){
			int index = rand.Uniform(64);
			t1[index] = rand.Uniform(twin);
		}
		for ( int j = 0; j<N2hits; j++ ){
			int index = rand.Uniform(64);
			t2[index] = rand.Uniform(twin);
		}
		for ( int j = 0; j<N3hits; j++ ){
			int index = rand.Uniform(64);
			t3[index] = rand.Uniform(twin);
		}
		int ntri = 0;
		for (int j = 0; j<64; j++){
			bool found = false;
			for (int k = j; k<j+1; k++){
			//for (int k = j-1; k<j+2; k++){
				//for (int l = j; l<j+1; l++){
				//for (int l = j-1; l<j+2; l++){
					//int il=l%64;
					//if (il<0) il+=64;
					int ik=k%64;
					if (ik<0) ik+=64;
					int jj = (j+1)%64;
					int kk = (ik+1)%64;
					if (t1[j]!=-1&&t2[ik]!=-1&&fabs(t1[j]-t2[ik])<tcoin){
					//if (t1[j]!=-1&&t2[ik]!=-1&&t1[jj]!=-1&&t2[kk]!=-1&&fabs(t1[j]-t2[ik])<tcoin&&fabs(t1[j]-t2[kk])<tcoin&&fabs(t1[j]-t2[jj])<tcoin){
					//if (t1[j]!=-1&&t2[ik]!=-1&&t3[il]!=-1&&fabs(t1[j]-t2[ik])<tcoin&&fabs(t1[j]-t3[il])<tcoin){
						found = true;
					}
				//}
			}
			if (found) ntri++;
		}
		h->Fill(ntri);
	}
	std::cout<<"Rate = "<<1/1.140*1000*(Npulse-h->GetBinContent(1))/Npulse<<" kHz"<<std::endl;
}
