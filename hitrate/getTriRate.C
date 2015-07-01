{
	TRandom rand;
	int ntimes = (1170-200)/20.;
	int npairs = 64;
	int Npulses = 100000;

//	int nsci = 41;
//	int nche = 252;

	// 141109D
	double scaleF = 2.5e12*2.4*1170e-9/1e9;
	//int nsci = (3628+4848)*scaleF;
	//int nche = (17388)*scaleF;
	int nsci = (3774+1421)*scaleF;
	int nche = (9054)*scaleF;
	//int nsci = (3774+1421+3628+4848)*scaleF;
	//int nche = (9054+17388)*scaleF;

	double t1[128];
	double t2[128];
	int good = 0;
	for ( int i = 0; i<Npulses; i++){
		if (i%10000==0) printf("%lf\%...\n",((double)i)/Npulses*100);
		for ( int it = 0; it<npairs; it++){
			t1[it] = -1;
			t2[it] = -1;
		}
		for ( int isci = 0; isci<nsci; isci++){
			int seg = rand.Uniform(npairs);
			int time = rand.Uniform(ntimes);
			t1[seg] = time;
		}
		for ( int iche = 0; iche<nche; iche++){
			int seg = rand.Uniform(npairs);
			int time = rand.Uniform(ntimes);
			t2[seg] = time;
		}
		for ( int iseg = 0; iseg<npairs; iseg++){
			int isegnext = iseg +1;
			if (isegnext == npairs) isegnext = 0;
			if(t1[iseg]!=-1&&t2[iseg]!=-1&&
			   t1[isegnext]!=-1&&t2[isegnext]!=-1&&
			   t1[iseg]==t2[iseg]&&t1[iseg]==t1[isegnext]&&t1[iseg]==t2[isegnext]) good++;
		}
	}
	printf ("N = %d\n",good);
}
