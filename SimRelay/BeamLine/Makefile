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
INCDIR += -I$(MYGEANT4ROOT)/include/Geant4 -I$(MYCLHEPROOT)/include
LIBDIR += -L$(MYGEANT4ROOT)/lib64 $(shell $(MYCLHEPROOT)/bin/clhep-config --libs)
INCDIR += -I$(shell $(ROOTSYS)/bin/root-config --incdir)
LIBDIR += $(shell $(ROOTSYS)/bin/root-config --glibs)
LIBDIR += -lMinuit
LIBS += -pthread -lm -ldl -rdynamic

LIBDIR += -L$(MYCOMMONTOOLSROOT)/libs
LIBS += -lMyRootInterface

#--------------------------PART 3: about your own packages and the main program-------------------------------------------
# Names of user defined packages

# Change this name to your program which contains the main function

PROJECT1 = beam
PROJECT2 = gethist
PROJECT3 = get
PROJECT4 = draw

.PHONY: all
all: $(PROJECT1) $(PROJECT2) $(PROJECT3) $(PROJECT4)

# Object list for user defined packages
OBJ =

$(PROJECT1): $(OBJ) $(PROJECT1).cxx
	$(CXX) $(CXXFLAGS) $(INCDIR) $(LIBDIR) $(LIBS) $(PROJECT1).cxx $(OBJ) -o $(PROJECT1)

$(PROJECT2): $(OBJ) $(PROJECT2).cxx
	$(CXX) $(CXXFLAGS) $(INCDIR) $(LIBDIR) $(LIBS) $(PROJECT2).cxx $(OBJ) -o $(PROJECT2)

$(PROJECT3): $(OBJ) $(PROJECT3).cxx
	$(CXX) $(CXXFLAGS) $(INCDIR) $(LIBDIR) $(LIBS) $(PROJECT3).cxx $(OBJ) -o $(PROJECT3)

$(PROJECT4): $(OBJ) $(PROJECT4).cxx
	$(CXX) $(CXXFLAGS) $(INCDIR) $(LIBDIR) $(LIBS) $(PROJECT4).cxx $(OBJ) -o $(PROJECT4)

#--------------------------PART 4: about make options-------------------------------------------
# You can create new options to realise special functions
# Here is an example showing how to make clean and make a specified user defined package
.PHONY: clean
clean:
	$(RM) $(PROJECT1) $(PROJECT2)
