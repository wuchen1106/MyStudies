{

//	// 120822
//	TChain * c = new TChain("t","t");
//	c->Add("/home/chen/MyWorkArea/MyStudies/MagneticField/result/UK_120822/default_phase-1_fieldmap_0.018T.dat.root");
//	double bx,by,bz,x,y,z;
//	c->SetBranchAddress("bx",&bx);
//	c->SetBranchAddress("by",&by);
//	c->SetBranchAddress("bz",&bz);
//	c->SetBranchAddress("x",&x);
//	c->SetBranchAddress("y",&y);
//	c->SetBranchAddress("z",&z);
//	c->Draw("y:x>>h","z==0&&x<2000");
//	h->SetTitle("Magnetic Field Lines on the Virticle Plane at the Centre of the Production Target");
//	h->GetXaxis()->SetTitle("Horizental Position [mm] (Pointing to the Detector Side)");
//	h->GetYaxis()->SetTitle("Verticle Position [mm] (Pointing Up)");
//	gStyle->SetOptStat(0);
//	for(int i = 0; i<=h->GetNbinsX(); i++){
//		for(int j = 0; j<=h->GetNbinsX(); j++){
//			h->SetBinContent(i,j,0);
//		}
//	}
//	h->Draw();
//	for (int i = 0; i<c->GetEntries(); i++){
//		c->GetEntry(i);
//		if (z==0&&x<2000){
//			TArrow * a = new TArrow(); a->SetLineColor(kRed); a->SetFillColor(kRed);a->DrawArrow(x,y,x+5*bx/0.01,y+5*by/0.01,0.0018,"|>");
//		}
//	}

//	// 111216
//	TChain * c = new TChain("t","t");
//	c->Add("/home/chen/MyWorkArea/MyStudies/MagneticField/result/UK_111216/defaultFieldMap-0.0000T-0.0000T-0.17T.dat.root");
//	double bx,by,bz,x,y,z;
//	c->SetBranchAddress("bx",&bx);
//	c->SetBranchAddress("by",&by);
//	c->SetBranchAddress("bz",&bz);
//	c->SetBranchAddress("x",&x);
//	c->SetBranchAddress("y",&y);
//	c->SetBranchAddress("z",&z);
//	c->Draw("y:x>>h","z==0&&x<2000&&y>=0");
//	h->SetTitle("Magnetic Field Lines on the Virticle Plane at the Centre of the Production Target");
//	h->GetXaxis()->SetTitle("Horizental Position [mm] (Pointing to the Detector Side)");
//	h->GetYaxis()->SetTitle("Verticle Position [mm] (Pointing Up)");
//	gStyle->SetOptStat(0);
//	for(int i = 0; i<=h->GetNbinsX(); i++){
//		for(int j = 0; j<=h->GetNbinsX(); j++){
//			h->SetBinContent(i,j,0);
//		}
//	}
//	h->Draw();
//	for (int i = 0; i<c->GetEntries(); i++){
//		c->GetEntry(i);
//		if (z==0&&x<2000&&y>=0){
//			TArrow * a = new TArrow(); a->SetLineColor(kRed); a->SetFillColor(kRed);a->DrawArrow(x,y,x+5*bx/0.01,y+5*by/0.01,0.0018,"|>");
//		}
//	}

//	// 111026
//	TChain * c = new TChain("t","t");
//	c->Add("/home/chen/MyWorkArea/MyStudies/MagneticField/result/UK_111026/defaultFieldMap.dat.root");
//	double bx,by,bz,x,y,z;
//	c->SetBranchAddress("bx",&bx);
//	c->SetBranchAddress("by",&by);
//	c->SetBranchAddress("bz",&bz);
//	c->SetBranchAddress("x",&x);
//	c->SetBranchAddress("y",&y);
//	c->SetBranchAddress("z",&z);
//	c->Draw("y:x>>h","z==0&&x<2000");
//	h->SetTitle("Magnetic Field Lines on the Virticle Plane at the Centre of the Production Target");
//	h->GetXaxis()->SetTitle("Horizental Position [mm] (Pointing to the Detector Side)");
//	h->GetYaxis()->SetTitle("Verticle Position [mm] (Pointing Up)");
//	gStyle->SetOptStat(0);
//	for(int i = 0; i<=h->GetNbinsX(); i++){
//		for(int j = 0; j<=h->GetNbinsX(); j++){
//			h->SetBinContent(i,j,0);
//		}
//	}
//	h->Draw();
//	for (int i = 0; i<c->GetEntries(); i++){
//		c->GetEntry(i);
//		if (z==0&&x<2000){
//			TArrow * a = new TArrow(); a->SetLineColor(kRed); a->SetFillColor(kRed);a->DrawArrow(x,y,x+5*bx/0.01,y+5*by/0.01,0.0018,"|>");
//		}
//	}

//	// 111026_chen
//	TChain * c = new TChain("data","data");
//	c->Add("/home/chen/MyWorkArea/Simulate/comet/fieldMaps/TOSHIBA_20111026_B9.root");
//	float bx,by,bz,x,y,z;
//	c->SetBranchAddress("Bx",&bx);
//	c->SetBranchAddress("By",&by);
//	c->SetBranchAddress("Bz",&bz);
//	c->SetBranchAddress("X",&x);
//	c->SetBranchAddress("Y",&y);
//	c->SetBranchAddress("Z",&z);
//	TCanvas * c1 = new TCanvas("c1","c1");
//	c->Draw("Y:Z>>h","X==7350");
//	h->SetTitle("Magnetic Field Lines on the Virticle Plane at the Centre of the Production Target");
//	h->GetXaxis()->SetTitle("Horizental Position [mm] (Pointing to the Detector Side)");
//	h->GetYaxis()->SetTitle("Verticle Position [mm] (Pointing Up)");
//	gStyle->SetOptStat(0);
//	for(int i = 0; i<=h->GetNbinsX(); i++){
//		for(int j = 0; j<=h->GetNbinsX(); j++){
//			h->SetBinContent(i,j,0);
//		}
//	}
//	h->Draw();
//	for (int i = 0; i<c->GetEntries(); i++){
//		c->GetEntry(i);
//		if (x==7350){
//			TArrow * a = new TArrow(); a->SetLineColor(kRed); a->SetFillColor(kRed);a->DrawArrow(z,y,z+5*bz/0.01,y+5*by/0.01,0.0018,"|>");
//		}
//	}

//	// 120822_chen
//	TChain * c = new TChain("data","data");
//	c->Add("/home/chen/MyWorkArea/Simulate/comet/fieldMaps/TOSHIBA_20120822_B9.root");
//	float bx,by,bz,x,y,z;
//	c->SetBranchAddress("Bx",&bx);
//	c->SetBranchAddress("By",&by);
//	c->SetBranchAddress("Bz",&bz);
//	c->SetBranchAddress("X",&x);
//	c->SetBranchAddress("Y",&y);
//	c->SetBranchAddress("Z",&z);
//	TCanvas * c1 = new TCanvas("c1","c1");
//	c->Draw("Y:Z>>h","X==7350");
//	h->SetTitle("Magnetic Field Lines on the Virticle Plane at the Centre of the Production Target");
//	h->GetXaxis()->SetTitle("Horizental Position [mm] (Pointing to the Detector Side)");
//	h->GetYaxis()->SetTitle("Verticle Position [mm] (Pointing Up)");
//	gStyle->SetOptStat(0);
//	for(int i = 0; i<=h->GetNbinsX(); i++){
//		for(int j = 0; j<=h->GetNbinsX(); j++){
//			h->SetBinContent(i,j,0);
//		}
//	}
//	h->Draw();
//	for (int i = 0; i<c->GetEntries(); i++){
//		c->GetEntry(i);
//		if (x==7350){
//			TArrow * a = new TArrow(); a->SetLineColor(kRed); a->SetFillColor(kRed);a->DrawArrow(z,y,z+5*bz/0.01,y+5*by/0.01,0.0018,"|>");
//		}
//	}

//	// 130117_chen
//	TChain * c = new TChain("data","data");
//	c->Add("/home/chen/MyWorkArea/Simulate/comet/fieldMaps/TOSHIBA_20130117_B9.root");
//	float bx,by,bz,x,y,z;
//	c->SetBranchAddress("Bx",&bx);
//	c->SetBranchAddress("By",&by);
//	c->SetBranchAddress("Bz",&bz);
//	c->SetBranchAddress("X",&x);
//	c->SetBranchAddress("Y",&y);
//	c->SetBranchAddress("Z",&z);
//	TCanvas * c1 = new TCanvas("c1","c1");
//	c->Draw("Y:Z>>h","X==7350");
//	h->SetTitle("Magnetic Field Lines on the Virticle Plane at the Centre of the Production Target");
//	h->GetXaxis()->SetTitle("Horizental Position [mm] (Pointing to the Detector Side)");
//	h->GetYaxis()->SetTitle("Verticle Position [mm] (Pointing Up)");
//	gStyle->SetOptStat(0);
//	for(int i = 0; i<=h->GetNbinsX(); i++){
//		for(int j = 0; j<=h->GetNbinsX(); j++){
//			h->SetBinContent(i,j,0);
//		}
//	}
//	h->Draw();
//	for (int i = 0; i<c->GetEntries(); i++){
//		c->GetEntry(i);
//		if (x==7350){
//			TArrow * a = new TArrow(); a->SetLineColor(kRed); a->SetFillColor(kRed);a->DrawArrow(z,y,z+5*bz/0.01,y+5*by/0.01,0.0018,"|>");
//		}
//	}

//	// 111026
//	TChain * c = new TChain("t","t");
//	c->Add("/home/chen/MyWorkArea/MyStudies/MagneticField/result/UK_130117/Phase1Field.dat.root");
//	double bx,by,bz,x,y,z;
//	c->SetBranchAddress("bx",&bx);
//	c->SetBranchAddress("by",&by);
//	c->SetBranchAddress("bz",&bz);
//	c->SetBranchAddress("x",&x);
//	c->SetBranchAddress("y",&y);
//	c->SetBranchAddress("z",&z);
//	c->Draw("y:x>>h","z==0");
//	h->SetTitle("Magnetic Field Lines on the Virticle Plane at the Centre of the Production Target");
//	h->GetXaxis()->SetTitle("Horizental Position [mm] (Pointing to the Detector Side)");
//	h->GetYaxis()->SetTitle("Verticle Position [mm] (Pointing Up)");
//	gStyle->SetOptStat(0);
//	for(int i = 0; i<=h->GetNbinsX(); i++){
//		for(int j = 0; j<=h->GetNbinsX(); j++){
//			h->SetBinContent(i,j,0);
//		}
//	}
//	h->Draw();
//	for (int i = 0; i<c->GetEntries(); i++){
//		c->GetEntry(i);
//		if (z==0){
//			TArrow * a = new TArrow(); a->SetLineColor(kRed); a->SetFillColor(kRed);a->DrawArrow(x,y,x+5*bx/0.01,y+5*by/0.01,0.0018,"|>");
//		}
//	}

//	// 140625
//	TChain * c = new TChain("t","t");
//	c->Add("/home/chen/MyWorkArea/MyStudies/MagneticField/result/UK_140625/cap1_X-1000to+3000_Z-600to+600.table.root");
//	double bx,by,bz,x,y,z;
//	c->SetBranchAddress("bx",&bx);
//	c->SetBranchAddress("by",&by);
//	c->SetBranchAddress("bz",&bz);
//	c->SetBranchAddress("x",&x);
//	c->SetBranchAddress("y",&y);
//	c->SetBranchAddress("z",&z);
//	c->Draw("y:-z>>h","x==0");
//	h->SetTitle("Magnetic Field Lines on the Virticle Plane at the Centre of the Production Target");
//	h->GetXaxis()->SetTitle("Horizental Position [mm] (Pointing to the Detector Side)");
//	h->GetYaxis()->SetTitle("Verticle Position [mm] (Pointing Up)");
//	gStyle->SetOptStat(0);
//	for(int i = 0; i<=h->GetNbinsX(); i++){
//		for(int j = 0; j<=h->GetNbinsX(); j++){
//			h->SetBinContent(i,j,0);
//		}
//	}
//	h->Draw();
//	for (int i = 0; i<c->GetEntries(); i++){
//		c->GetEntry(i);
//		if (x==0){
//			TArrow * a = new TArrow(); a->SetLineColor(kRed); a->SetFillColor(kRed);a->DrawArrow(-z,y,-z-5*bz/0.05,y+5*by/0.05,0.0018,"|>");
//		}
//	}
//
//	// 140625_chen
//	TChain * c = new TChain("data","data");
//	c->Add("/home/chen/MyWorkArea/Simulate/comet/fieldMaps/TOSHIBA_20140625_CS.root");
//	float bx,by,bz,x,y,z;
//	c->SetBranchAddress("Bx",&bx);
//	c->SetBranchAddress("By",&by);
//	c->SetBranchAddress("Bz",&bz);
//	c->SetBranchAddress("X",&x);
//	c->SetBranchAddress("Y",&y);
//	c->SetBranchAddress("Z",&z);
//	TCanvas * c1 = new TCanvas("c1","c1");
//	c->Draw("Y:Z>>h","X==7650");
//	h->SetTitle("Magnetic Field Lines on the Virticle Plane at the Centre of the Production Target");
//	h->GetXaxis()->SetTitle("Horizental Position [mm] (Pointing to the Detector Side)");
//	h->GetYaxis()->SetTitle("Verticle Position [mm] (Pointing Up)");
//	gStyle->SetOptStat(0);
//	for(int i = 0; i<=h->GetNbinsX(); i++){
//		for(int j = 0; j<=h->GetNbinsX(); j++){
//			h->SetBinContent(i,j,0);
//		}
//	}
//	h->Draw();
////	TCanvas * c2 = new TCanvas("c2","c2");
////	c->Draw("Z:X","Y==0");
//	for (int i = 0; i<c->GetEntries(); i++){
//		c->GetEntry(i);
//		if (x==7650){
////			c1->cd();
//			TArrow * a = new TArrow(); a->SetLineColor(kRed); a->SetFillColor(kRed);a->DrawArrow(z,y,z+5*bz/0.05,y+5*by/0.05,0.0018,"|>");
//		}
////		if (y==0){
////			c2->cd();
////			TArrow * a = new TArrow(); a->DrawArrow(x,z,x+5*bx/3,z+5*bz/3,0.001,">");
////		}
//	}

	// TS3_IHEP141022_chen
	TChain * c = new TChain("data","data");
	c->Add("/home/chen/MyWorkArea/Simulate/comet/fieldMaps/IHEP_A9_141022_TS3.root");
	float br,bz,r,z;
	c->SetBranchAddress("Br",&br);
	c->SetBranchAddress("Bz",&bz);
	c->SetBranchAddress("R",&r);
	c->SetBranchAddress("Z",&z);
	TCanvas * c1 = new TCanvas("c1","c1");
	c->Draw("Y:X>>h","Z==3500");
	h->SetTitle("Magnetic Field Lines on the Virticle Plane at the Centre of the Production Target");
	h->GetXaxis()->SetTitle("Horizental Position [mm] (Pointing to the Detector Side)");
	h->GetYaxis()->SetTitle("Verticle Position [mm] (Pointing Up)");
	gStyle->SetOptStat(0);
	for(int i = 0; i<=h->GetNbinsX(); i++){
		for(int j = 0; j<=h->GetNbinsX(); j++){
			h->SetBinContent(i,j,0);
		}
	}
	h->Draw();
	for (int i = 0; i<c->GetEntries(); i++){
		c->GetEntry(i);
		if (z==3500){
			TArrow * a = new TArrow(); a->SetLineColor(kRed); a->SetFillColor(kRed);a->DrawArrow(x,y,x+5*bx/0.01,y+5*by/0.01,0.0018,"|>");
		}
	}

}
