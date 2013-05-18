# A simple example of makefile.
  # Type "make" to compile and link your program.
  # Type "make clean" to delete objects.
  # Type "make [PACKAGE]" to compile and link the specified package only.
  # Type "make all" to compile and link all your packages and programs.

#--------------------------PART 1: about the compiler-------------------------------------------
# Specify the compiler. Usually it is g++, if you are using gcc.
CXX = g++
# Here you can pass different options to the compiler
CXXFLAGS = -g -O2 -W -Wall

#--------------------------PART 2: about packages you would use-------------------------------------------
# If you want to use external files, or third party packages, you need to specify two kinds of paths and a list of shared library files.
  # INCDIR tells the compiler where are thoses headers contained.
  # LIBDIR shows directories which contain library files coresponding to those headers. 
  # LIBS is the list of shared library files you would use.They can be found in LIBDIR. ( Names changing rule: libXXX.so to -lXXX )

# Take root as an example. ( Root is a widely used package developed by CERN for analysing. )
INCDIR = -I$(shell $(ROOTSYS)/bin/root-config --incdir) -I/home/chen/Programs/geant4.9.5.p02-install/include/Geant4 -I/home/chen/Programs/CLHEP-install/include
LIBDIR = $(shell $(ROOTSYS)/bin/root-config --glibs) -L/home/chen/Programs/geant4.9.5.p02-install/lib64 -L/home/chen/Programs/CLHEP-install/lib
LIBS = -pthread -lm -ldl -rdynamic\
       -lASImageGui -lASImage -lBonjour -lCintex -lCint -lcomplexDict -lCore -ldequeDict -lEG -lEve -lFFTW\
       -lFitPanel -lFoam -lFTGL -lFumili -lGed -lGenetic -lGenVector -lGeomBuilder -lGeomPainter -lGeom -lGLEW -lGpad -lGraf3d\
       -lGraf -lGuiBld -lGuiHtml -lGui -lGviz3d -lGX11 -lGX11TTF -lHistFactory -lHistPainter -lHist -lHtml -lKrb5Auth -llistDict\
       -lmap2Dict -lmapDict -lMathCore -lMatrix -lMemStat -lMinuit -lMLP -lmultimap2Dict -lmultimapDict -lmultisetDict -lNet\
       -lNew -lPhysics -lPostscript -lProofBench -lProofDraw -lProofPlayer -lProof -lQuadp -lRecorder -lReflexDict -lReflex\
       -lRGL -lRint -lRIO -lRLDAP -lRooFitCore -lRooFit -lRooStats -lRootAuth -lSessionViewer -lsetDict -lSmatrix -lSpectrumPainter\
       -lSpectrum -lSPlot -lSQLIO -lSrvAuth -lThread -lTMVA -lTreePlayer -lTree -lTreeViewer -lvalarrayDict\
       -lvectorDict -lVMC -lX3d -lXMLIO -lXMLParser\
       -lCLHEP-2.1.2.4 -lCLHEP-Cast-2.1.2.4 -lCLHEP-Evaluator-2.1.2.4 -lCLHEP-Exceptions-2.1.2.4\
       -lCLHEP-GenericFunctions-2.1.2.4 -lCLHEP-Geometry-2.1.2.4 -lCLHEP-Matrix-2.1.2.4\
       -lCLHEP-Random-2.1.2.4 -lCLHEP-RandomObjects-2.1.2.4 -lCLHEP-RefCount-2.1.2.4\
       -lCLHEP-Vector-2.1.2.4 -lG4analysis -lG4digits_hits -lG4error_propagation -lG4event\
       -lG4FR -lG4geometry -lG4gl2ps -lG4global -lG4GMocren -lG4graphics_reps -lG4intercoms -lG4interfaces\
       -lG4materials -lG4modeling -lG4OpenGL -lG4parmodels -lG4particles -lG4persistency -lG4physicslists\
       -lG4processes -lG4RayTracer -lG4readout -lG4run -lG4tracking -lG4track -lG4Tree -lG4visHepRep\
       -lG4vis_management -lG4visXXX -lG4VRML -lG4zlib 

#--------------------------PART 3: about your own packages and the main program-------------------------------------------
# Names of user defined packages

# Change this name to your program which contains the main function
PROGRAM = eIoni

# Object list for user defined packages
OBJ = 

$(PROGRAM): $(OBJ) $(PROGRAM).cc
	$(CXX) $(CXXFLAGS) $(INCDIR) $(LIBDIR) $(LIBS) $(PROGRAM).cc $(OBJ) -o $(PROGRAM)

#--------------------------PART 4: about make options-------------------------------------------
# You can create new options to realise special functions
# Here is an example showing how to make clean and make a specified user defined package
.PHONY: clean
clean:
	$(RM) $(OBJ)
	$(RM) $(PROGRAM)

.PHONY: all
all: $(PROGRAM)
