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
#include "TApplication.h"

#include "TMVA/Factory.h"
#include "TMVA/Tools.h"
#include "TMVA/TMVAGui.h"
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
   TString outfileName( "data/TMVAClassification_SplitBlock_XiCutEPOSSD1.root" );
   TFile* outputFile = TFile::Open( outfileName, "RECREATE" );
   TMVA::Factory *factory = new TMVA::Factory( "TMVAClassification", outputFile,
                                               "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification" );
   

   //////////////////////////////the Final variables/////////////////////////////////////////////////

   factory->AddVariable( "DeltaEta","DeltaEta", "units", 'F' );
   factory->AddVariable( "DeltaEtaZero","DeltaEtaZero", "units", 'F' );
   factory->AddVariable( "EtaMax","EtaMax", "units", 'F' );
   factory->AddVariable( "EtaMin","EtaMin", "units", 'F' );
   factory->AddVariable( "NbrTowersHFminus","NbrTowersHFminus", "units", 'I' );
   factory->AddVariable( "NbrTowersHFplus","NbrTowersHFplus", "units", 'I' );//Log->logdegistirdin
   factory->AddVariable( "NbrTowersCastor","NbrTowersCastor", "units", 'I' );
   factory->AddVariable( "NbrTracks","NbrTracks", "units", 'I' );
   factory->AddVariable( "NbrAllTowers","NbrAllTowers", "units", 'F' );
   factory->AddVariable( "HottestTowerCastor","HottestTowerCastor", "units", 'F' );
   factory->AddVariable( "CastorSumEnergy","CastorSumEnergy", "units", 'F' );

   factory->AddVariable( "SumEnergyHFPlus","SumEnergyHFPlus", "units", 'F' );
   factory->AddVariable( "SumEnergyHFMinus","SumEnergyHFMinus", "units", 'F' );
   // factory->AddVariable( "log10XixReco","log10XixReco", "units", 'F' );
   // factory->AddVariable( "log10XiyReco","log10XiyReco", "units", 'F' );//Log->logdegistirdin
   
  
   TString sample_name = "XiCutEPOSSD1";
   TFile *input = mSample[sample_name].file;
   std::cout << "--- TMVAClassification       : Using input file: " << input->GetName() << std::endl;

   // --- Register the training and test trees
   // TTree *signal = (TTree*)input->Get( mSample[sample_name].tree_name + "/sigTreeDD");
   // TTree *background = (TTree*)input->Get( mSample[sample_name].tree_name + "/bkgTreeDD");   
   // Double_t signalWeight     = 1.0;
   // Double_t backgroundWeight = 1.0;
   // // // You can add an arbitrary number of signal or background trees
   // factory->AddSignalTree(signal, signalWeight);
   // factory->AddBackgroundTree(background, backgroundWeight);

   ////////////////////////////////////////////////////////////////////////////
   // Signal and Background events located in the same tree
   TTree* inputTree = (TTree*)input->Get( mSample[sample_name].tree_name + "/AllTree" );
   // 
   // how to identify signal events
   // TCut signalCut = "Pythia8processid==105";
   // how to identify background events
   // TCut backgrCut = "Pythia8processid!=105";
   // use Xi-Selection-Cut
   // 
   TCut signalCut = mSample[sample_name].signalCut;
   TCut backgrCut = mSample[sample_name].backgrCut;
   // 
   factory->SetInputTrees( inputTree, signalCut, backgrCut );

   // change weight file extension
   (TMVA::gConfig().GetIONames()).fWeightFileExtension = mSample[sample_name].weight_name;


   
   // Apply additional cuts on the signal and background samples (can be different)
   TCut mycuts = "DeltaEtaZero>=0" ; // for example: TCut mycuts = "abs(var1)<0.5 && abs(var2-0.5)<1";
   TCut mycutb = "DeltaEtaZero>=0"; // for example: TCut mycutb = "abs(var1)<0.5";
   // 
   factory->PrepareTrainingAndTestTree( mycuts, mycutb,
                                       // "nTrain_Signal= 4000:nTest_Signal=2000:nTrain_Background=4000:nTest_Background=2000:SplitMode=Random:NormMode=NumEvents:!V" );
                                     //Random  // "nTrain_Signal=200000:nTest_Signal=90000:nTrain_Background=300000:nTest_Background=90000:SplitMode=Random:NormMode=NumEvents:!V" );
                                   //Alternate "nTrain_Signal=100000:nTrain_Background=100000::nTest_Signal=100000:nTest_Background=100000:SplitMode=Alternate:NormMode=NumEvents:!V" );
                                 
                                    // DD mbr 210000  0.14 (0.138) , 280000: 0.286 (0.419)
                                    // "nTrain_Signal= 280000:nTest_Signal=280000:nTrain_Background=280000:nTest_Background=280000:SplitMode=Block:NormMode=NumEvents:!V" );
                                    //epos 0.044 (0.452)
                                    // "nTrain_Signal= 156000:nTest_Signal=156000:nTrain_Background=156000:nTest_Background=156000:SplitMode=Block:NormMode=NumEvents:!V" );
                                    // SD1 kolmogorov mbr: 0.395 (0.009) 
                                    // "nTrain_Signal= 20000:nTest_Signal=20000:nTrain_Background=20000:nTest_Background=20000:SplitMode=Block:NormMode=NumEvents:!V" );
                                    // epos:  0.186 (0.376)
                                   "nTrain_Signal= 40500:nTest_Signal=40500:nTrain_Background=40500:nTest_Background=40500:SplitMode=Block:NormMode=NumEvents:!V" );
                                   //SD2 mbr kolmogorov 0.287 (0.109)
                                    // "nTrain_Signal= 100000:nTest_Signal=100000:nTrain_Background=100000:nTest_Background=100000:SplitMode=Block:NormMode=NumEvents:!V" );
   


   // Please lookup the various method configuration options in the corresponding cxx files, eg:
   // src/MethoCuts.cxx, etc, or here: http://tmva.sourceforge.net/optionRef.html
   // it is possible to preset ranges in the option string in which the cut optimisation should be done:
   // "...:CutRangeMin[2]=-1:CutRangeMax[2]=1"...", where [2] is the third input variable

   // Cut optimisation
   
   // Boosted Decision Trees

   
    factory->BookMethod( TMVA::Types::kBDT, "BDTG","!H:!V:NTrees=300:MinNodeSize=2.5%:BoostType=Grad:Shrinkage=0.3:UseBaggedBoost:BaggedSampleFraction=0.9:nCuts=20:MaxDepth=2" );
   
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // factory->BookMethod( TMVA::Types::kBDT, "BDTG", "!H:!V:NTrees=1000:MinNodeSize=2.5%:BoostType=Grad:Shrinkage=0.3:UseBaggedBoost:BaggedSampleFraction=0.9:nCuts=20:MaxDepth=3" );
   // factory->BookMethod( TMVA::Types::kBDT, "BDTG", "!H:!V:NTrees=1000:MinNodeSize=2.5%:BoostType=Grad:Shrinkage=0.3:UseBaggedBoost:BaggedSampleFraction=0.9:nCuts=20:MaxDepth=3" );
   // factory->BookMethod( TMVA::Types::kFisher, "Fisher", "H:!V:Fisher:VarTransform=None:CreateMVAPdfs:PDFInterpolMVAPdf=Spline2:NbinsMVAPdf=50:NsmoothMVAPdf=10" );
   // factory->BookMethod( TMVA::Types::kLD, "LD", "H:!V:VarTransform=None:CreateMVAPdfs:PDFInterpolMVAPdf=Spline2:NbinsMVAPdf=50:NsmoothMVAPdf=10" );
   // factory->BookMethod( TMVA::Types::kLikelihood, "Likelihood","H:!V:TransformOutput:PDFInterpol=Spline2:NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmoothBkg[1]=10:NSmooth=1:NAvEvtPerBin=50" );
   // factory->BookMethod( TMVA::Types::kMLP, "MLP", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5:!UseRegulator" );






   // // Train MVAs using the set of training events
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
   TMVA::TMVAGui( outfileName );

   return 0;
}

// !!! to start TMVAGui start Gui inside root
// !!! > root -l
// !!! > TMVA::TMVAGui("data/TMVA.root") // <- check outfileName Melike!!!
int main( int argc, char** argv )
{
   UNUSED(argc);
   UNUSED(argv);


   //////////////////////////////////////////////////////////////////////////
   // later say app->Run() to freeze the program before execution
   TApplication * app = new TApplication("app",0,0);

   // Select methods (don't look at this code - not of interest)
   // TString methodList = ""; 
   // for (int i=1; i<argc; i++) {
   //    TString regMethod(argv[i]);
   //    if(regMethod=="-b" || regMethod=="--batch") continue;
   //    if (!methodList.IsNull()) methodList += TString(","); 
   //    methodList += regMethod;
   // }

   Diffractive_TMVAClassification(); 

   /////////////////////////////////////////////////
   // freeze program
   app->Run();
   /////////////////////////////////////////////////

   return 0;
}
