int a(){
	std::stringstream buff;

	TFile * fp_ref = new TFile("result/g40cm10mm_McTruth_EP_QB_0.root");
	TTree * tree = (TTree*)fp_ref->Get("t");
	if (!tree){
		std::cout<<"Cannot find tree!"<<std::endl;
		return -1;
	}

	TPad padList[20];
	int ipad=0;
	int pid=0;
	std::string pname="";
	for (int iPar = 0; iPar<5; iPar++){
		if (iPar==0) {pid=11;pname="em";}
		else if (iPar==1) {pid=-11;pname="ep";}
		else if (iPar==2) {pid=13;pname="mum";}
		else if (iPar==3) {pid=-211;pname="pim";}
		else if (iPar==4) {pid=2212;pname="pp";}
		std::string h2Name = pname+"_zr";
		TH2D* h2 = (TH2D*)fp_ref->Get(h2Name.c_str());
		if (!h2){
			std::cout<<"Cannot find h2!"<<std::endl;
			return -1;
		}
		std::string cName = pname+"_c";
		TCanvas *c = new TCanvas(cName.c_str(),cName.c_str(),1024,768);
		int nx = 2;
		int ny = 1;
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
		padList[ipad-2]->cd();
		gPad->SetLogy(0);
		h2->Draw("COLZ");
		padList[ipad-1]->cd();
		gPad->SetLogy(1);
		gPad->SetGridx(1);
		gPad->SetGridy(1);
		buff.str("");
		buff.clear();
		buff<<"pid=="<<pid;
		tree->Draw("process",buff.str().c_str());

		pdfName = pname+".pdf";
		c->Print(pdfName.c_str());
		jpgName = pname+".jpg";
		c->Print(jpgName.c_str());
	}

	return 0;
}
