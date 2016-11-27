// @(#)root/tmva $Id$
/**********************************************************************************
 * Project   : TMVA - a ROOT-integrated toolkit for multivariate data analysis    *
 * Package   : TMVA                                                               *
 * Root Macro: TMVAClassification                                                 *
 *                                                                                *
 * This macro provides examples for the training and testing of the               *
 * TMVA classifiers.                                                              *
 *                                                                                *
 * As input data is used a toy-MC sample consisting of four Gaussian-distributed  *
 * and linearly correlated input variables.                                       *
 *                                                                                *
 * The methods to be used can be switched on and off by means of booleans, or     *
 * via the prompt command, for example:                                           *
 *                                                                                *
 *    root -l ./TMVAClassification.C\(\"Fisher,Likelihood\"\)                     *
 *                                                                                *
 * (note that the backslashes are mandatory)                                      *
 * If no method given, a default set of classifiers is used.                      *
 *                                                                                *
 * The output file "TMVA.root" can be analysed with the use of dedicated          *
 * macros (simply say: root -l <macro.C>), which can be conveniently              *
 * invoked through a GUI that will appear at the end of the run of this macro.    *
 * Launch the GUI via the command:                                                *
 *                                                                                *
 *    root -l ./TMVAGui.C                                                         *
 *                                                                                *
 * You can also compile and run the example with the following commands           *
 *                                                                                *
 *    make                                                                        *
 *    ./TMVAClassification <Methods>                                              *
 *                                                                                *
 * where: <Methods> = "method1 method2"                                           *
 *        are the TMVA classifier names                                           *
 *                                                                                *
 * example:                                                                       *
 *    ./TMVAClassification Fisher LikelihoodPCA BDT                               *
 *                                                                                *
 * If no method given, a default set is of classifiers is used                    *
 **********************************************************************************/


// As hint how to do it
// g++ -Wall -O2 `root-config --cflags` Diffractive_TMVAClassification.cc `root-config --libs` -lMLP -lMinuit -lTreePlayer -lTMVA -lTMVAGui -lXMLIO  -lMLP -o Diffractive_TMVAClassification


#include <cstdlib>
#include <iostream>
#include <map>
#include <string>

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TMVA/Factory.h"
#include "TMVA/Tools.h"
// #include "TMVA/TMVAGui.h"
#include "TMVA/Config.h"
#include "SampleList.h"
#define UNUSED(x) (void)(x) // to avoid unused compiler warning




int Diffractive_TMVAClassification()
{
   
   TMVA::Tools::Instance();

   std::cout << std::endl;
   std::cout << "==> Start TMVAClassification" << std::endl;
   // --- Here the preparation phase begins
   ////////////////////////////////////////////////////////////////////////////
   std::map<TString, SampleList::sSample> mSample = SampleList::read_data_mc_files();
   ////////////////////////////////////////////////////////////////////////////
   // Create a ROOT output file where TMVA will store ntuples, histograms, etc.
   TString outfileName( "data/TMVA_Pythia8XiEventselectioncut.root" );
   TFile* outputFile = TFile::Open( outfileName, "RECREATE" );
   TMVA::Factory *factory = new TMVA::Factory( "TMVAClassification", outputFile,
                                               "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification" );
   factory->AddVariable( "deltazero","deltazero", "units", 'F' );
   factory->AddVariable( "etamax","etamax", "units", 'F' );
   factory->AddVariable( "etamin","etamin", "units", 'F' );
   factory->AddVariable( "log10XixReco","log10XixReco", "units", 'F' );
   factory->AddVariable( "log10XiyReco","log10XiyReco", "units", 'F' );//Log->logdegistirdin
   factory->AddVariable( "HFminusNtowers","HFminusNtowers", "units", 'I' );
   factory->AddVariable( "HFplusNtowers","HFplusNtowers", "units", 'I' );//Log->logdegistirdin
   factory->AddVariable( "CastorNtowers","CastorNtowers", "units", 'I' );
   factory->AddVariable( "Ntracks","Ntracks", "units", 'I' );
   
   
   TString sample_name = "Pythia8XiEventselectioncut";
   TFile *input = mSample[sample_name].file;
   std::cout << "--- TMVAClassification       : Using input file: " << input->GetName() << std::endl;

   // --- Register the training and test trees
   TTree *signal = (TTree*)input->Get( mSample[sample_name].tree_name + "/sigTree");
   TTree *background = (TTree*)input->Get( mSample[sample_name].tree_name + "/bkgTree");   
   // change weight file extension
   (TMVA::gConfig().GetIONames()).fWeightFileExtension = mSample[sample_name].weight_name;
   Double_t signalWeight     = 1.0;
   Double_t backgroundWeight = 1.0;
   // You can add an arbitrary number of signal or background trees
   factory->AddSignalTree(signal, signalWeight);
   factory->AddBackgroundTree(background, backgroundWeight);
   
   // Apply additional cuts on the signal and background samples (can be different)
   TCut mycuts = "deltazero>=0"; // for example: TCut mycuts = "abs(var1)<0.5 && abs(var2-0.5)<1";
   TCut mycutb = "deltazero>=0"; // for example: TCut mycutb = "abs(var1)<0.5";
   factory->PrepareTrainingAndTestTree( mycuts, mycutb,
                                        "nTrain_Signal=500000:nTest_Signal=500000:nTrain_Background=500000:nTest_Background=500000:SplitMode=Random:NormMode=NumEvents:!V" );



   
   

   // ---- Book MVA methods
   //
   // Please lookup the various method configuration options in the corresponding cxx files, eg:
   // src/MethoCuts.cxx, etc, or here: http://tmva.sourceforge.net/optionRef.html
   // it is possible to preset ranges in the option string in which the cut optimisation should be done:
   // "...:CutRangeMin[2]=-1:CutRangeMax[2]=1"...", where [2] is the third input variable

   // Cut optimisation
   
   // Boosted Decision Trees
   factory->BookMethod( TMVA::Types::kBDT, "BDTG", "!H:!V:NTrees=1000:MinNodeSize=2.5%:BoostType=Grad:Shrinkage=0.10:UseBaggedBoost:BaggedSampleFraction=0.9:nCuts=20:MaxDepth=2" );

  
   // Train MVAs using the set of training events
   factory->TrainAllMethods();

   // ---- Evaluate all MVAs using the set of test events
   factory->TestAllMethods();

   // ----- Evaluate and compare performance of all configured MVAs
   factory->EvaluateAllMethods();

   // --------------------------------------------------------------

   // Save the output
   outputFile->Close();

   std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl;
   std::cout << "==> TMVAClassification is done!" << std::endl;

   delete factory;

   // Launch the GUI for the root macros
   // if (!gROOT->IsBatch()) TMVA::TMVAGui( outfileName );

   return 0;
}

int main( int argc, char** argv )
{
   UNUSED(argc);
   UNUSED(argv);


   // Select methods (don't look at this code - not of interest)
   // TString methodList = ""; 
   // for (int i=1; i<argc; i++) {
   //    TString regMethod(argv[i]);
   //    if(regMethod=="-b" || regMethod=="--batch") continue;
   //    if (!methodList.IsNull()) methodList += TString(","); 
   //    methodList += regMethod;
   // }
   return Diffractive_TMVAClassification(); 
}
