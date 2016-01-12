{
	TString thegas = "isobutane";
//	TString thegas = "ethane";
	double ind,su1,sd1,cu1,cd1,sd2,su2,cu2,cd2,st,mt,t1,t2,ch,n1,n2;
	char type[128];
	char gas[128];
	TChain * chain = new TChain("t");
	chain->Add("sum.root");
	chain->SetBranchAddress("ind",&ind);
	chain->SetBranchAddress("eu1",&su1);
	chain->SetBranchAddress("ed1",&sd1);
	chain->SetBranchAddress("cu1",&cu1);
	chain->SetBranchAddress("cd1",&cd1);
	chain->SetBranchAddress("su2",&su2);
	chain->SetBranchAddress("sd2",&sd2);
	chain->SetBranchAddress("cu2",&cu2);
	chain->SetBranchAddress("cd2",&cd2);
	chain->SetBranchAddress("t1",&t1);
	chain->SetBranchAddress("t2",&t2);
	chain->SetBranchAddress("n1",&n1);
	chain->SetBranchAddress("n2",&n2);
	chain->SetBranchAddress("st",&st);
	chain->SetBranchAddress("mt",&mt);
	chain->SetBranchAddress("ch",&ch);
	chain->SetBranchAddress("type",&type);
	chain->SetBranchAddress("gas",&gas);
	std::vector<double> vind;
	std::vector<double> vsu1;
	std::vector<double> vsd1;
	std::vector<double> vcu1;
	std::vector<double> vcd1;
	std::vector<double> vsu2;
	std::vector<double> vsd2;
	std::vector<double> vcu2;
	std::vector<double> vcd2;
	std::vector<double> vt1;
	std::vector<double> vt2;
	std::vector<double> vn1;
	std::vector<double> vn2;
	std::vector<double> vst;
	std::vector<double> vmt;
	std::vector<double> vct;
	std::vector<double> vch;
	std::vector<std::string> vtype;
	double mxcd1 = 0;
	double mxsu2 = 0;
	double mxt1 = 0;
	double mxn1 = 0;
	double mxst = 0;
	double mxmt = 0;
	double mxct = 0;
	double mxch = 0;
	double mxind = 0;
	for (int i = 0; i<chain->GetEntries(); i++){
		chain->GetEntry(i);
		if (gas!=thegas) continue;
		t1/=2.48*0.9;
		t2/=2.48*0.9;
		n1/=44.82;
		n2/=44.82;
		su1*=1.17;
		sd1*=1.17;
		cu1*=1.17/1000;
		cd1*=1.17/1000;
		su2*=1170./970/1000;
		sd2*=1170./970/1000;
		cu2*=1170./970/1000;
		cd2*=1170./970/1000;
		vind.push_back(ind);
		vsu1.push_back(su1);
		vsd1.push_back(sd1);
		vcu1.push_back(cu1);
		vcd1.push_back(cd1);
		vsu2.push_back(su2);
		vsd2.push_back(sd2);
		vcu2.push_back(cu2);
		vcd2.push_back(cd2);
		vt1.push_back(t1);
		vt2.push_back(t2);
		vn1.push_back(n1);
		vn2.push_back(n2);
		vst.push_back(st);
		vmt.push_back(mt);
		vct.push_back(mt+st);
		vch.push_back(ch);
		vtype.push_back(type);
		if(ind>mxind) mxind=ind;
		if(cd1>mxcd1) mxcd1=cd1;
		if(su2>mxsu2) mxsu2=su2;
		if(t1>mxt1) mxt1=t1;
		if(n1>mxn1) mxn1=n1;
		if(st>mxst) mxst=st;
		if(mt>mxmt) mxmt=mt;
		if(mt+st>mxct) mxct=mt+st;
		if(ch>mxch) mxch=ch;
	}
	for (int i = 0; i<vind.size(); i++){
		vt1[i]*=0.9*mxct/mxt1;
		vt2[i]*=0.9*mxct/mxt1;
		vn1[i]*=0.9*mxct/mxn1;
		vn2[i]*=0.9*mxct/mxn1;
		vch[i]*=0.9*mxct/mxch;
		vsu2[i]*=0.9*mxct/mxsu2;
		vsd2[i]*=0.9*mxct/mxsu2;
		vcu2[i]*=0.9*mxct/mxsu2;
		vcd2[i]*=0.9*mxct/mxsu2;
		vsu1[i]*=0.9*mxct/mxcd1;
		vsd1[i]*=0.9*mxct/mxcd1;
		vcu1[i]*=0.9*mxct/mxcd1;
		vcd1[i]*=0.9*mxct/mxcd1;
	}
	TGraph * gsu1 = new TGraph(vind.size(),&(vind[0]),&(vsu1)[0]);
	TGraph * gsd1 = new TGraph(vind.size(),&(vind[0]),&(vsd1)[0]);
	TGraph * gcu1 = new TGraph(vind.size(),&(vind[0]),&(vcu1)[0]);
	TGraph * gcd1 = new TGraph(vind.size(),&(vind[0]),&(vcd1)[0]);
	TGraph * gsu2 = new TGraph(vind.size(),&(vind[0]),&(vsu2)[0]);
	TGraph * gsd2 = new TGraph(vind.size(),&(vind[0]),&(vsd2)[0]);
	TGraph * gcu2 = new TGraph(vind.size(),&(vind[0]),&(vcu2)[0]);
	TGraph * gcd2 = new TGraph(vind.size(),&(vind[0]),&(vcd2)[0]);
	TGraph * gt1 = new TGraph(vind.size(),&(vind[0]),&(vt1)[0]);
	TGraph * gt2 = new TGraph(vind.size(),&(vind[0]),&(vt2)[0]);
	TGraph * gn1 = new TGraph(vind.size(),&(vind[0]),&(vn1)[0]);
	TGraph * gn2 = new TGraph(vind.size(),&(vind[0]),&(vn2)[0]);
	TGraph * gst = new TGraph(vind.size(),&(vind[0]),&(vst)[0]);
	TGraph * gmt = new TGraph(vind.size(),&(vind[0]),&(vmt)[0]);
	TGraph * gct = new TGraph(vind.size(),&(vind[0]),&(vct)[0]);
	TGraph * gch = new TGraph(vind.size(),&(vind[0]),&(vch)[0]);
	gct->GetYaxis()->SetRangeUser(0,1.1*mxct);
	gct->GetXaxis()->SetRangeUser(0,mxind);
	gct->GetYaxis()->SetTitle("Acceptance [%]");
	gct->SetTitle("Optimization for "+thegas);
	gct->SetMarkerColor(kBlack);
	gct->SetLineColor(kBlack);
	gct->SetMarkerStyle(20);
	gsu1->SetMarkerStyle(4);
	gsu1->SetMarkerColor(kGreen);
	gsu1->SetLineColor(kGreen);
	gsd1->SetMarkerStyle(4);
	gsd1->SetMarkerColor(kCyan);
	gsd1->SetLineColor(kCyan);
	gcu1->SetMarkerStyle(4);
	gcu1->SetMarkerColor(kMagenta);
	gcu1->SetLineColor(kMagenta);
	gcd1->SetMarkerStyle(4);
	gcd1->SetMarkerColor(kOrange);
	gcd1->SetLineColor(kOrange);
	gsu2->SetMarkerStyle(4);
	gsu2->SetMarkerColor(kGreen);
	gsu2->SetLineColor(kGreen);
	gsd2->SetMarkerStyle(4);
	gsd2->SetMarkerColor(kCyan);
	gsd2->SetLineColor(kCyan);
	gcu2->SetMarkerStyle(4);
	gcu2->SetMarkerColor(kMagenta);
	gcu2->SetLineColor(kMagenta);
	gcd2->SetMarkerStyle(4);
	gcd2->SetMarkerColor(kOrange);
	gcd2->SetLineColor(kOrange);
	gt1->SetMarkerStyle(4);
	gt1->SetMarkerColor(kMagenta);
	gt1->SetLineColor(kMagenta);
	gt2->SetMarkerStyle(4);
	gt2->SetMarkerColor(kGreen);
	gt2->SetLineColor(kGreen);
	gn1->SetMarkerStyle(4);
	gn1->SetMarkerColor(kMagenta);
	gn1->SetLineColor(kMagenta);
	gn2->SetMarkerStyle(4);
	gn2->SetMarkerColor(kGreen);
	gn2->SetLineColor(kGreen);
	gst->SetMarkerStyle(20);
	gst->SetMarkerColor(kRed);
	gst->SetLineColor(kRed);
	gmt->SetMarkerStyle(20);
	gmt->SetMarkerColor(kBlue);
	gmt->SetLineColor(kBlue);
	gch->SetMarkerStyle(4);
	gch->SetMarkerColor(kGreen);
	gch->SetLineColor(kGreen);

	gStyle->SetPalette(1);
	gStyle->SetOptStat(0);
	gStyle->SetPadTickX(1);
//	gStyle->SetPadTickY(1);
	TGaxis * axis = 0;

	TCanvas * canv1 = new TCanvas("c1","c1",1440,768);
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	gct->Draw("LAP");
	axis = new TGaxis(mxind,0,mxind,1.1*mxct,0,1.1*mxt1,510,"+L");
	axis->SetTitleSize(0.035);
	axis->SetTitleFont(42);
	axis->SetLabelSize(0.035);
	axis->SetLabelFont(42);
	axis->SetTitle("Trigger Rate [kHz]");
	axis->SetLineColor(kRed);
	axis->SetLabelColor(kRed);
	axis->Draw();
	gst->Draw("LPSAME");
	gmt->Draw("LPSAME");
	gt1->Draw("LPSAME");
	gt2->Draw("LPSAME");

	std::string pretype = vtype[0];
	double left  =0;
	for (int i = 0; i<vind.size(); i++){
		if (vtype[i]!=pretype){// this is a new type and not the first one
			TLine * l = new TLine(vind[i]-0.5,0,vind[i]-0.5,1.1*mxct);
			l->Draw("SAME");
		}
		if (vtype[i]!=pretype||i==vind.size()-1){// this is a new type and not the first one
			double theleft = left-0.4;
			if (left==0) theleft = 0;
			TPaveText *lable = new TPaveText(theleft,1.02*mxct,vind[i]-0.6,1.08*mxct,"NB");
			lable->AddText(pretype.c_str());
			lable->SetTextColor(kBlack);
			lable->Draw();
			pretype = vtype[i];
			left = vind[i];
		}
	}

	TCanvas * canv2 = new TCanvas("c2","c2",1440,768);
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	gct->Draw("LAP");
	axis = new TGaxis(mxind,0,mxind,1.1*mxct,0,1.1*mxsu2,510,"+L");
	axis->SetTitleSize(0.035);
	axis->SetTitleFont(42);
	axis->SetLabelSize(0.035);
	axis->SetLabelFont(42);
	axis->SetTitle("Instantaneous Hit Rate after 200 ns [MHz]");
	axis->SetLineColor(kRed);
	axis->SetLabelColor(kRed);
	axis->Draw();
	gst->Draw("LPSAME");
	gmt->Draw("LPSAME");
	gsu2->Draw("LPSAME");
	gsd2->Draw("LPSAME");
	gcu2->Draw("LPSAME");
	gcd2->Draw("LPSAME");

	std::string pretype = vtype[0];
	double left  =0;
	for (int i = 0; i<vind.size(); i++){
		if (vtype[i]!=pretype){// this is a new type and not the first one
			TLine * l = new TLine(vind[i]-0.5,0,vind[i]-0.5,1.1*mxct);
			l->Draw("SAME");
		}
		if (vtype[i]!=pretype||i==vind.size()-1){// this is a new type and not the first one
			double theleft = left-0.4;
			if (left==0) theleft = 0;
			TPaveText *lable = new TPaveText(theleft,1.02*mxct,vind[i]-0.6,1.08*mxct,"NB");
			lable->AddText(pretype.c_str());
			lable->SetTextColor(kBlack);
			lable->Draw();
			pretype = vtype[i];
			left = vind[i];
		}
	}

	TCanvas * canv3 = new TCanvas("c3","c3",1440,768);
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	gct->Draw("LAP");
	axis = new TGaxis(mxind,0,mxind,1.1*mxct,0,1.1*mxcd1,510,"+L");
	axis->SetTitleSize(0.035);
	axis->SetTitleFont(42);
	axis->SetLabelSize(0.035);
	axis->SetLabelFont(42);
	axis->SetTitle("Prompt Size in first 200ns [signal size as 1]");
	axis->SetLineColor(kRed);
	axis->SetLabelColor(kRed);
	axis->Draw();
	gst->Draw("LPSAME");
	gmt->Draw("LPSAME");
	gsu1->Draw("LPSAME");
	gsd1->Draw("LPSAME");
	gcu1->Draw("LPSAME");
	gcd1->Draw("LPSAME");

	std::string pretype = vtype[0];
	double left  =0;
	for (int i = 0; i<vind.size(); i++){
		if (vtype[i]!=pretype){// this is a new type and not the first one
			TLine * l = new TLine(vind[i]-0.5,0,vind[i]-0.5,1.1*mxct);
			l->Draw("SAME");
		}
		if (vtype[i]!=pretype||i==vind.size()-1){// this is a new type and not the first one
			double theleft = left-0.4;
			if (left==0) theleft = 0;
			TPaveText *lable = new TPaveText(theleft,1.02*mxct,vind[i]-0.6,1.08*mxct,"NB");
			lable->AddText(pretype.c_str());
			lable->SetTextColor(kBlack);
			lable->Draw();
			pretype = vtype[i];
			left = vind[i];
		}
	}

	TCanvas * canv4 = new TCanvas("c4","c4",1440,768);
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	gct->Draw("LAP");
	axis = new TGaxis(mxind,0,mxind,1.1*mxct,0,1.1*mxch,510,"+L");
	axis->SetTitleSize(0.035);
	axis->SetTitleFont(42);
	axis->SetLabelSize(0.035);
	axis->SetLabelFont(42);
	axis->SetTitle("Aging in CDC [mC/cm/day]");
	axis->SetLineColor(kRed);
	axis->SetLabelColor(kRed);
	axis->Draw();
	gst->Draw("LPSAME");
	gmt->Draw("LPSAME");
	gch->Draw("LPSAME");

	std::string pretype = vtype[0];
	double left  =0;
	for (int i = 0; i<vind.size(); i++){
		if (vtype[i]!=pretype){// this is a new type and not the first one
			TLine * l = new TLine(vind[i]-0.5,0,vind[i]-0.5,1.1*mxct);
			l->Draw("SAME");
		}
		if (vtype[i]!=pretype||i==vind.size()-1){// this is a new type and not the first one
			double theleft = left-0.4;
			if (left==0) theleft = 0;
			TPaveText *lable = new TPaveText(theleft,1.02*mxct,vind[i]-0.6,1.08*mxct,"NB");
			lable->AddText(pretype.c_str());
			lable->SetTextColor(kBlack);
			lable->Draw();
			pretype = vtype[i];
			left = vind[i];
		}
	}

	TCanvas * canv5 = new TCanvas("c5","c5",1440,768);
	gPad->SetGridx(1);
	gPad->SetGridy(1);
	gct->Draw("LAP");
	axis = new TGaxis(mxind,0,mxind,1.1*mxct,0,1.1*mxn1,510,"+L");
	axis->SetTitleSize(0.035);
	axis->SetTitleFont(42);
	axis->SetLabelSize(0.035);
	axis->SetLabelFont(42);
	axis->SetTitle("CDC Occupancy [%]");
	axis->SetLineColor(kRed);
	axis->SetLabelColor(kRed);
	axis->Draw();
	gst->Draw("LPSAME");
	gmt->Draw("LPSAME");
	gn1->Draw("LPSAME");
	gn2->Draw("LPSAME");

	std::string pretype = vtype[0];
	double left  =0;
	for (int i = 0; i<vind.size(); i++){
		if (vtype[i]!=pretype){// this is a new type and not the first one
			TLine * l = new TLine(vind[i]-0.5,0,vind[i]-0.5,1.1*mxct);
			l->Draw("SAME");
		}
		if (vtype[i]!=pretype||i==vind.size()-1){// this is a new type and not the first one
			double theleft = left-0.4;
			if (left==0) theleft = 0;
			TPaveText *lable = new TPaveText(theleft,1.02*mxct,vind[i]-0.6,1.08*mxct,"NB");
			lable->AddText(pretype.c_str());
			lable->SetTextColor(kBlack);
			lable->Draw();
			pretype = vtype[i];
			left = vind[i];
		}
	}

	canv1->SaveAs("triggerrate."+thegas+".png");
	canv2->SaveAs("hitrate."+thegas+".png");
	canv3->SaveAs("prompt."+thegas+".png");
	canv4->SaveAs("charge."+thegas+".png");
	canv5->SaveAs("occupancy."+thegas+".png");
}
