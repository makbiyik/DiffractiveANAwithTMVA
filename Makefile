CXX         :=g++

# compile with debug info
#CXXFLAGS    :=-g
# bounds checking not used for C++ front-ends
#CXXFLAGS    +=-fbounds-check 
# Warnings
CXXFLAGS    +=-Wall -Wextra -Wshadow
# Iso standard
CXXFLAGS    +=-ansi

# Add root library
CXXFLAGS    +=$(shell root-config --cflags)
LDFLAGS     +=$(shell root-config --libs)

CXXFLAGS    +=-I./lib/

# Add RooUnfold package
# CXXFLAGS    +=-I/home/hauke/RooUnfold-1.1.1/src/
# LDFLAGS     +=-L/home/hauke/RooUnfold-1.1.1
# LDFLAGS     +=-lRooUnfold

# Add TMVA libraries WITHOUT -lTMVAGui (Hauke: not on my system older TMVA version)
LDFLAGS     += -lMLP -lMinuit -lTreePlayer -lTMVA -lXMLIO -lMLP

# Needed for ROOT TASImage in CMS_lumi.C
LDFLAGS     += -lASImage

.PHONY : all

all: classification application createfigures

classification:
		@$(CXX) $(CXXFLAGS) Diffractive_TMVAClassification.cc $(LDFLAGS) -o Diffractive_TMVAClassification

application:
		@$(CXX) $(CXXFLAGS) Diffractive_TMVApplication.cc $(LDFLAGS) -o Diffractive_TMVApplication

createfigures:
		@$(CXX) $(CXXFLAGS) CreateFigures.cc $(LDFLAGS) -o CreateFigures

clean:
		@rm -rfv Diffractive_TMVAClassification
		@rm -rfv Diffractive_TMVApplication
		@rm -rfv CreateFigures