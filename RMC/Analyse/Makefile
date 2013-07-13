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

# Special packages:
INCDIR += -I$(MYCOMMONTOOLSROOT)/MyRootInterface

# Take root as an example. ( Root is a widely used package developed by CERN for analysing. )
INCDIR += -I$(shell $(ROOTSYS)/bin/root-config --incdir) -I$(MYGEANT4ROOT)/include/Geant4 -I$(MYCLHEPROOT)/include
LIBDIR += $(shell $(ROOTSYS)/bin/root-config --glibs) -L$(MYGEANT4ROOT)/lib64 $(shell $(MYCLHEPROOT)/bin/clhep-config --libs)
LIBS += -pthread -lm -ldl -rdynamic
LIBDIR += -L$(MYCOMMONTOOLSROOT)/libs
LIBS += -lMyRootInterface

#--------------------------PART 3: about your own packages and the main program-------------------------------------------
# Names of user defined packages

# Change this name to your program which contains the main function
RMCE = RMCE
PH2E = ph2e
BM = bm

# Object list for user defined packages
OBJ = 

$(RMCE): $(OBJ) $(RMCE).cxx
	$(CXX) $(CXXFLAGS) $(INCDIR) $(LIBDIR) $(LIBS) $(RMCE).cxx $(OBJ) -o $(RMCE)

$(PH2E): $(OBJ) $(PH2E).cxx
	$(CXX) $(CXXFLAGS) $(INCDIR) $(LIBDIR) $(LIBS) $(PH2E).cxx $(OBJ) -o $(PH2E)

$(BM): $(OBJ) $(BM).cxx
	$(CXX) $(CXXFLAGS) $(INCDIR) $(LIBDIR) $(LIBS) $(BM).cxx $(OBJ) -o $(BM)

#--------------------------PART 4: about make options-------------------------------------------
# You can create new options to realise special functions
# Here is an example showing how to make clean and make a specified user defined package
.PHONY: clean
clean:
	$(RM) $(OBJ)
	$(RM) $(RMCE)
	$(RM) $(PH2E)
	$(RM) $(BM)

.PHONY: all
all: $(RMCE)
