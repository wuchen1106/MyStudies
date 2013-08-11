int a(){
	std::stringstream buff;

	TFile * fp_ref = new TFile("");
	TTree * tree = (TTree*)fp_ref->Get("t");
	TH2D* h2 = (TH2D*)fp_ref->Get("rz");

	for (int iPar = 0; iPar<5; iPar++){
		if (iPar==0)
		TCanvas *c = new TCanvas("c","c",1024,768);
		int nx = 1;
		int ny = 2;
		TPad padList[10];
		int ipad=0;
		for (int ix = 0; ix < nx; ix++ ){
			for (int iy = ny-1; iy >= 0; iy--){
				buff.str("");
				buff.clear();
				buff<<"pad("<<ix<<":"<<iy<<")"<<std::endl;
				double x1 = ix/((double)nx);
				double y1 = iy/((double)ny);
				double x2 = (1+ix)/((double)nx);
				double y2 = (1+iy)/((double)ny);
				TPad *apad = new TPad(buff.str().c_str(),buff.str().c_str(),x1,y1,x2,y2);
				apad->Draw();
				padList[ipad] = apad;
				ipad++;
			}
		}
		padList[0]->cd();
		tree->Draw("process","pid==11");

		c->Print("c.pdf");
		c->Print("c.jpg");
	}

	return 0;
}
