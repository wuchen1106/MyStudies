#include <iostream>
#include <fstream>
#include <stdio.h>
#include <math.h>
#include <string>
#include <vector>
using namespace std;

int main() {
	float x, y, z, Bx, By, Bz, BT;

	/*
	   string inFile1 = "/unix/lfv/rootfiles/fieldmaps/toshibaMaps/01-seg-1.table";
	   string outFile1 = "/unix/lfv/rootfiles/fieldmaps/toshibaMaps/seg_1.map";

	   string inFile2 = "/unix/lfv/rootfiles/fieldmaps/toshibaMaps/02-seg-2.table";
	   string outFile2 = "/unix/lfv/rootfiles/fieldmaps/toshibaMaps/seg_2.map";
	 */

	string inFile3 = "/unix/lfv/rootfiles/fieldmaps/toshibaMaps/03-arc-1.table";
	string outFile3 = "/unix/lfv/edmonds/fieldmap/arc_1.map";

	/*
	   string inFile4 = "/unix/lfv/rootfiles/fieldmaps/toshibaMaps/04-seg-3.table";
	   string outFile4 = "/unix/lfv/rootfiles/fieldmaps/toshibaMaps/seg_3.map";

	   string inFile5 = "/unix/lfv/rootfiles/fieldmaps/toshibaMaps/05-arc-2.table";
	   string outFile5 = "/unix/lfv/rootfiles/fieldmaps/toshibaMaps/arc_2.map";

	   string inFile6 = "/unix/lfv/rootfiles/fieldmaps/toshibaMaps/06-seg-4.table";
	   string outFile6 = "/unix/lfv/rootfiles/fieldmaps/toshibaMaps/seg_4.map";

	   ifstream input1;
	   input1.open (inFile1.c_str(),fstream::in);
	   ofstream output1;
	   output1.open (outFile1.c_str(),fstream::out);

	   ifstream input2;
	   input2.open (inFile2.c_str(),fstream::in);
	   ofstream output2;
	   output2.open (outFile2.c_str(),fstream::out);
	 */

	ifstream input3;
	input3.open (inFile3.c_str(),fstream::in);
	ofstream output3;
	output3.open (outFile3.c_str(),fstream::out);

	/*
	   ifstream input4;
	   input4.open (inFile4.c_str(),fstream::in);
	   ofstream output4;
	   output4.open (outFile4.c_str(),fstream::out);

	   ifstream input5;
	   input5.open (inFile5.c_str(),fstream::in);
	   ofstream output5;
	   output5.open (outFile5.c_str(),fstream::out);

	   ifstream input6;
	   input6.open (inFile6.c_str(),fstream::in);
	   ofstream output6;
	   output6.open (outFile6.c_str(),fstream::out);

	   output1 << "#This is the BLFieldMap input file for SEG1 in cartesian co-ords \ngrid X0=-600 Y0=0 Z0=-1000 nX=121 nY=61 nZ=401 dX=10 dY=10 dZ=10 \nextendY flip=Bx,By \ndata" << endl;

	   if (input1.is_open()) {
	   while ( input1.good() ){
	   input1 >> z >> x >> y >> Bz >> Bx >> By >> BT;
	   output1 << x << " " << y << " " << -z << " " << Bx << " " << By << " " << Bz << endl;
	   }
	   }

	   input1.close();
	   output1.close();

	   output2 << "#This is the BLFieldMap input file for SEG2 in cartesian co-ords \ngrid X0=-240 Y0=0 Z0=3000 nX=49 nY=25 nZ=136 dX=10 dY=10 dZ=10 \nextendY flip=Bx,By \ndata" << endl;

	   if (input2.is_open()) {
	   while ( input2.good() ){
	   input2 >> z >> x >> y >> Bz >> Bx >> By >> BT;
	   output2 << x << " " << y << " " << -z << " " << Bx << " " << By << " " << Bz << endl;
	   }
	   }

	   input2.close();
	   output2.close();
	 */

	output3 << "#This is the BLFieldMap input file for ARC1 in cartesian co-ords \ngrid X0=-240 Y0=0 Z0=4250 nX=730 nY=85 nZ=396 dX=10 dY=10 dZ=10 \nextendY flip=Bx,By \ndata" << endl;

	for (int z = 8250; z > 7600; z-=10) {
		for (int x = 7050; x > 3000; x-=10) {
			for (int y = 0; y <= 850; y+=10) {
				if(x > 4500){
					output3 << x << " " << y << " " << z << " " << -1 << " " << 0 << " " << 0 << endl;
				}
				if(x < 4510 && x > 3000){
					output3 << x << " " << y << " " << z << " " << -3.25+(2.25*((float)(x-3000)/1500)) << " " << 0 << " " << 0 << endl;
				}
			}
		}
	}

	if (input3.is_open()) {
		while ( input3.good() ){      
			input3 >> z >> x >> y >> Bz >> Bx >> By >> BT;
			if(x > -3010){
				output3 << -x << " " << y << " " << -z << " " << Bx << " " << By << " " << Bz << endl;
			}
			if(x < -4500){
				output3 << -x << " " << y << " " << -z << " " << -1 << " " << 0 << " " << 0 << endl;
				if (y == 240) {
					for (int y = 250; y <= 850; y+=10) {
						output3 << -x << " " << y << " " << -z << " " << -1 << " " << 0 << " " << 0 << endl;
					}
				}
			}
			if(x > -4510 && x < -3000){
				output3 << -x << " " << y << " " << -z << " " << -3.25+(2.25*((-x-3000)/1500)) << " " << 0 << " " << 0 << endl;
				if (y == 240) {
					for (int y = 250; y <= 850; y+=10) {
						output3 << -x << " " << y << " " << -z << " " << -3.25+(2.25*((-x-3000)/1500)) << " " << 0 << " " << 0 << endl;
					}
				}
			}
			/*
			   else{
			   for(int i=1; i<151; i++){
			   double Bx_grad = -3.25 + (2.25*i)/150 ;
			   for(int j=0; j<24; j++){
			   for(int k=1; k<337; k++){
			   output3 << (i*10)+3000 << " " << j*10 << " " << (k*10)+4350 << " " << Bx_grad << " " << 0 << " " << 0 << endl;
			   }
			   }
			   }
			   }
			 */
		}
	}

	input3.close();
	output3.close();

	/*
	   output4 << "#This is the BLFieldMap input file for SEG3 in cartesian co-ords \ngrid X0=6100 Y0=0 Z0=-700 nX=141 nY=71 nZ=506 dX=10 dY=10 dZ=10 \nextendY flip=Bx,By \ndata" << endl;

	   if (input4.is_open()) {
	   while ( input4.good() ){
	   input4 >> z >> x >> y >> Bz >> Bx >> By >> BT;
	   output4 << -x << " " << y << " " << -z << " " << Bx << " " << By << " " << Bz << endl;
	   }
	   }

	   input4.close();
	   output4.close();

	   output5 << "#This is the BLFieldMap input file for ARC2 in cartesian co-ords \ngrid X0=6100 Y0=0 Z0=-4000 nX=541 nY=71 nZ=331 dX=10 dY=10 dZ=10 \nextendY flip=Bx,By \ndata" << endl;

	   if (input5.is_open()) {
	   while ( input5.good() ){
	   input5 >> z >> x >> y >> Bz >> Bx >> By >> BT;
	   output5 << -x << " " << y << " " << -z << " " << Bx << " " << By << " " << Bz << endl;
	   }
	   }

	   input5.close();
	   output5.close();

	   output6 << "#This is the BLFieldMap input file for SEG4 in cartesian co-ords \ngrid X0=10100 Y0=0 Z0=-700 nX=141 nY=71 nZ=445 dX=10 dY=10 dZ=10 \nextendY flip=Bx,By \ndata" << endl;

	   if (input6.is_open()) {
	   while ( input6.good() ){
	   input6 >> z >> x >> y >> Bz >> Bx >> By >> BT;
	   output6 << -x << " " << y << " " << -z << " " << Bx << " " << By << " " << Bz << endl;
	   }
	   }

	   input6.close();
	   output6.close();
	 */

}
