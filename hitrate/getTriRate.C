{
	TRandom rand;

	int nsci = 41;
	int nche = 252;

	double t1[128];
	double t2[128];
	int good = 0;
	for ( int i = 0; i<10000; i++){
		for ( int it = 0; it<128; it++){
			t1[it] = 0;
			t2[it] = 0;
		}
		for ( int isci = 0; isci<nsci; isci++){
			int seg = rand.Uniform(128);
			int time = rand.Uniform(100);
			t1[seg] = time;
		}
		for ( int iche = 0; iche<nche; iche++){
			int seg = rand.Uniform(128);
			int time = rand.Uniform(100);
			t2[seg] = time;
		}
		for ( int iseg = 0; iseg<128; iseg++){
			if(t1[seg]==t2[seg]) good++;
		}
	}
	printf("%d\n",good);
}
