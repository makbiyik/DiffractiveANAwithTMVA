/**********************************************************************************
 * Project   : TMVA - a Root-integrated toolkit for multivariate data analysis    *
 * Package   : TMVA                                                               *
 * Exectuable: Diffractive_TMVApplication                                      *
 *                                                                                *
 * This macro provides a simple example on how to use the trained classifiers     *
 * within an analysis module                                                      *
 **********************************************************************************/

// g++ -Wall -O2 `root-config --cflags` Diffractive_TMVApplication.cc `root-config --libs` -lMLP -lMinuit -lTreePlayer -lTMVA -lTMVAGui -lXMLIO  -lMLP -o Diffractive_TMVApplication

#include <cstdlib>
#include <vector>
#include <iostream>
#include <map>
#include <string>

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TStopwatch.h"

#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include "TMVA/MethodCuts.h"

#include "SampleList.h"

#define UNUSED(x) (void)(x) // to avoid unused compiler warning

using namespace TMVA;


//////////////////////////////////////////////////////////////////////////////////////////////////////
// global struct for ordering data info
struct sSample
{
  bool isData;
  double lumi;
  double xs; // in nb

  TFile* file;
  TString tree_name;
};



//////////////////////////////////////////////////////////////////////////////////////////////////////
// get string from integer value Pythia8processid
TString get_Pythia_Process_ID(int Pythia8processid) {

   if(Pythia8processid == -1) {
      return "_Rest";
   } else if (Pythia8processid == 103) {
      return "_SD1";
   } else if (Pythia8processid == 104) {
      return "_SD2";
   } else if (Pythia8processid == 105) {
      return "_DD";
   }
   // } else if (Pythia8processid == 106) {
   //    return "_CD";
   // }

   return "_Rest";
}

TString get_Pythia_Process_ID(TString& sampleSignal,
                              int EventselectionXiprocessDD_tree,
                              int EventselectionXiprocessSD1_tree,
                              int EventselectionXiprocessSD2_tree,
                              int EventselectionXiprocessRest_tree) {


   if ( sampleSignal == "DD" ) {
      if (EventselectionXiprocessDD_tree == 1) 
         return "_DD";
   } else if ( sampleSignal == "SD1" ) {
      if (EventselectionXiprocessSD1_tree == 1) 
         return "_SD1";
   } else if ( sampleSignal == "SD2" ) {
      if (EventselectionXiprocessSD2_tree == 1) 
         return "_SD2";
   } else if ( sampleSignal == "Rest" ) {
      if (EventselectionXiprocessRest_tree == 1) 
         return "_Rest";
   }


   // if (EventselectionXiprocessDD_tree == 1) {
   //    return "_DD";
   // } else if (EventselectionXiprocessSD1_tree == 1) {
   //    return "_SD1";
   // } else if (EventselectionXiprocessSD2_tree == 1) {
   //    return "_SD2";
   // } else if (EventselectionXiprocessRest_tree == 1) {
   //    return "_Rest";
   // }

   return "_Rest";
}


////////////////////////////////////////////////////////////////////////
void Diffractive_TMVApplication() 
{   

   //---------------------------------------------------------------
   //Pythia8_BDTG_Pythia8Trained, EPOS_BDTG_Pythia8Trained, Pythia8XiEventselectioncut_BDTG_Pythia8Trained,Data_BDTG_Pythia8Trained 
   ////////////////////////////////////////////////////////////////////////////
   // !!!
   TString sampleName = "Pythia8";
   ////////////////////////////////////////////////////////////////////////////


   // This loads the library
   TMVA::Tools::Instance();

   std::cout << std::endl;
   std::cout << "==> Start Diffractive_TMVApplication" << std::endl;

   ////////////////////////////////////////////////////////////////////////////
   std::map<TString, SampleList::sSample> mSample = SampleList::read_data_mc_files();
   // std::map<TString, SampleList::sWeightFiles> mWeightFiles = SampleList::read_WeightFiles();
   ////////////////////////////////////////////////////////////////////////////

   bool bFoundSample = false;

   do {
      std::cout << "Which Sample you want to use?" << std::endl;
      std::cout << "Possible Samples are: " << std::endl;
      for(std::map<TString, SampleList::sSample>::iterator it=mSample.begin(); it!=mSample.end(); it++)
         std::cout << "--- " << it->first << std::endl;
      std::cin >> sampleName;
      if(mSample.find(sampleName) == mSample.end()) {
         std::cout << "Sorry sampel " << sampleName << " could not be found" << std::endl;
         std::cout << "Try again" << std::endl;
         bFoundSample = false;
      } else {
         bFoundSample = true;
      }
   } while(!bFoundSample);


   // --------------------------------------------------------------------------------------------------
   // --- Create the Reader object



   TMVA::Reader *reader = new TMVA::Reader( "!Color:!Silent" );

   // Create a set of variables and declare them to the reader
   // - the variable names MUST corresponds in name and type to those given in the weight file(s) used
   
   ///////////////////////////the final varibles/////////////////////////////////
   
   Float_t deltaeta, deltazero, etamax, etamin;
   // Float_t  log10XixReco, log10XiyReco;//log10XiDD;
   Float_t HFminusNtowers, HFplusNtowers ,CastorNtowers,CaloReducedenergyClass, Ntracks;//RGmean;// ,Pythia8processid,EventselectionXiprocessid;
   Float_t MaxCastorEnergy; //CastorSumEnergy,CaloReducedenergyClass, HFMinusSumEnergy;//;//MaxHFEnergy;
   Float_t CastorSumEnergy;
   Float_t HFPlusSumEnergy,HFMinusSumEnergy ;
   // ,HFMinusSumEnergy;
   
   
   reader->AddVariable("DeltaEta", &deltaeta );
   reader->AddVariable("DeltaEtaZero", &deltazero );
   reader->AddVariable("EtaMax" , &etamax );
   reader->AddVariable("EtaMin" , &etamin );
   reader->AddVariable("NbrTowersHFminus" , &HFminusNtowers);
   reader->AddVariable("NbrTowersHFplus" , &HFplusNtowers);
   reader->AddVariable("NbrTowersCastor" , &CastorNtowers);
   reader->AddVariable("NbrTracks" ,&Ntracks );
   reader->AddVariable("NbrAllTowers",&CaloReducedenergyClass);
   reader->AddVariable("HottestTowerCastor",&MaxCastorEnergy);
   reader->AddVariable("CastorSumEnergy", &CastorSumEnergy);
   reader->AddVariable("SumEnergyHFPlus", &HFPlusSumEnergy);
   reader->AddVariable("SumEnergyHFMinus", &HFMinusSumEnergy);

  
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // reader->AddVariable( "log10XixReco" , &log10XixReco );
  //  reader->AddVariable( "log10XiyReco" , &log10XiyReco );
   
   
  

   // --- Book the MVA methods
   std::map<TString, TString> mMethodeHistSuffix;
   std::string sUseWeightFile;

   std::cout << "--- Using weight files:" << std::endl;
   for(std::map<TString, SampleList::sSample>::iterator it=mSample.begin(); it!=mSample.end(); it++) {
      for(std::vector<TString>::iterator vit=it->second.vMethode.begin(); vit!=it->second.vMethode.end(); vit++) {
         TString weightfile = "TMVAClassification_" + *vit + "." + it->second.weight_name + ".xml";
         TString methodName = *vit + " method " + it->first;

         std::ifstream fweight_file_tmp("weights/" + weightfile);
         if(!fweight_file_tmp.good()) continue;

         if(sUseWeightFile!="rest") {
            sUseWeightFile = "";
            std::cout << "!!! Should I use this weight file: " << weightfile << " (y/n/rest) ?" << std::endl;
            std::cin >> sUseWeightFile;
            if(sUseWeightFile == "n") continue;
         }
         mMethodeHistSuffix[methodName] = it->first + "_" + *vit;

         reader->BookMVA(methodName, "weights/" + weightfile );
      }
   }
   

   ////////////////////////////////////////////////////////////////////////////
   // Book output histograms
   ////////////////////////////////////////////////////////////////////////////
   std::map<TString, TH1*> mHist;

   mHist["hNentries"] = new TH1F("hNentries","hNentries",10,0,10);

   
   ////////////////////////////////////////////////////////////////////////////
   // loop over proccess ID
   ////////////////////////////////////////////////////////////////////////////
   const int Nproc = 7;
   TString proccesses[Nproc] = {"","_CD","_DD","_SD1","_SD2","_Rest","_NONE"};
   TString histname;
   for(int iproc=0; iproc<Nproc; iproc++) {

      for(std::map<TString, TString>::iterator it=mMethodeHistSuffix.begin(); it!=mMethodeHistSuffix.end(); it++) {
            histname = TString("hDisciminant_") + it->second + proccesses[iproc];
            mHist[histname] = new TH1F(histname,histname,40,-1,1);
      }

   }
   // end loop over proccess ID
   ////////////////////////////////////////////////////////////////////////////


   ////////////////////////////////////////////////////////////////////////////
   for( std::map<TString, TH1*>::iterator it = mHist.begin(); it != mHist.end(); it++)
      it->second->Sumw2();
   
   ////////////////////////////////////////////////////////////////////////////
   // end of hist booking
   ////////////////////////////////////////////////////////////////////////////



   ////////////////////////////////////////////////////////////////////////////
   std::cout << "--- TMVAClassification       : Using input file: " << mSample[sampleName].file->GetName() << std::endl;
 
   // --- Get trained signal for this sampleName
   TString sampleSignal = mSample[sampleName].signal;

   // --- Register the training and test trees   
   std::cout << "--- Select signal sample" << std::endl;
   TTree* theTree = (TTree*)mSample[sampleName].file->Get(mSample[sampleName].tree_name + "/AllTree");

/////////////////////////// the final variables///////////////////////////////////////////
   Int_t HFminusNtowers_tree ,HFplusNtowers_tree ,CastorNtowers_tree,CaloReducedenergyClass_tree,Ntracks_tree;
   Int_t Pythia8processid_tree;
   Int_t EventselectionXiprocessDD_tree, EventselectionXiprocessSD1_tree, EventselectionXiprocessSD2_tree, EventselectionXiprocessRest_tree;
   
   theTree->SetBranchAddress("DeltaEta", &deltaeta); 
   theTree->SetBranchAddress("DeltaEtaZero", &deltazero);
   theTree->SetBranchAddress("EtaMax", &etamax);
   theTree->SetBranchAddress("EtaMin", &etamin);
   theTree->SetBranchAddress("NbrTowersHFminus", &HFminusNtowers_tree);
   theTree->SetBranchAddress("NbrTowersHFplus", &HFplusNtowers_tree);
   theTree->SetBranchAddress("NbrTowersCastor", &CastorNtowers_tree);
   theTree->SetBranchAddress("NbrTracks",&Ntracks_tree);
   theTree->SetBranchAddress("NbrAllTowers", &CaloReducedenergyClass_tree);
   theTree->SetBranchAddress("HottestTowerCastor",&MaxCastorEnergy);
   theTree->SetBranchAddress("CastorSumEnergy",&CastorSumEnergy);
   theTree->SetBranchAddress("SumEnergyHFPlus",&HFPlusSumEnergy);
   // theTree->SetBranchAddress("SumEnergyHFMinus",&HFMinusSumEnergy);
    
   // theTree->SetBranchAddress("log10XixReco", &log10XixReco);
   // theTree->SetBranchAddress("log10XiyReco", &log10XiyReco);
//////////////////////////////////////////////////////////////////////////////////////////
   
   theTree->SetBranchAddress("Pythia8processid",&Pythia8processid_tree);
   theTree->SetBranchAddress("EventselectionXiprocessDD",&EventselectionXiprocessDD_tree);
   theTree->SetBranchAddress("EventselectionXiprocessSD1",&EventselectionXiprocessSD1_tree);
   theTree->SetBranchAddress("EventselectionXiprocessSD2",&EventselectionXiprocessSD2_tree);
   theTree->SetBranchAddress("EventselectionXiprocessRest",&EventselectionXiprocessRest_tree);

   
   


   std::map<TString, double> mClassifier_Value;

   ////////////////////////////////////////////////////////////////////////////
   // loop over entries in InputTree
   ////////////////////////////////////////////////////////////////////////////
   std::cout << "--- Processing: " << theTree->GetEntries() << " events" << std::endl;
   TStopwatch sw;
   sw.Start();
   for (Long64_t ievt=0; ievt<theTree->GetEntries() /*&& ievt<50000*/; ievt++) {

      if (ievt%1000 == 0) std::cout << "--- ... Processing event: " << ievt << std::endl;

      theTree->GetEntry(ievt);

      mHist["hNentries"]->Fill("all",1);

      HFplusNtowers = (Float_t)HFplusNtowers_tree;
      HFminusNtowers = (Float_t)HFminusNtowers_tree;
      CastorNtowers = (Float_t)CastorNtowers_tree;
      Ntracks =(Float_t)Ntracks_tree;
      CaloReducedenergyClass = (Float_t)CaloReducedenergyClass_tree;
     
      // Pythia8processid=(Float_t)Pythia8processid_tree;
      // EventselectionXiprocessid=(Float_t)EventselectionXiprocessid_tree;


      for(std::map<TString, TString>::iterator it=mMethodeHistSuffix.begin(); it!=mMethodeHistSuffix.end(); it++)
            mClassifier_Value[it->second] = reader->EvaluateMVA(it->first);


      // Fill hist depending on proccess ID string
      TString proccess = "" ;
      if (mSample[sampleName].procesesID_pythia8) proccess = get_Pythia_Process_ID(Pythia8processid_tree);
      else proccess = get_Pythia_Process_ID(sampleSignal, EventselectionXiprocessDD_tree, EventselectionXiprocessSD1_tree, EventselectionXiprocessSD2_tree, EventselectionXiprocessRest_tree);  


      for(std::map<TString, TString>::iterator it=mMethodeHistSuffix.begin(); it!=mMethodeHistSuffix.end(); it++) {
            // Fill general hists
            histname = TString("hDisciminant_") + it->second;
            mHist[histname]->Fill(mClassifier_Value[it->second]);

            histname = TString("hDisciminant_") + it->second + proccess;
            mHist[histname]->Fill(mClassifier_Value[it->second]);
      }


      // /////////////////////////////////////////////////////////////////////////
      // // Start Own Code Example
      // // I used Fisser method e.g.
      // double discriminant_cut = 0.15;
      // if( discriminant_value < discriminant_cut) {
      //    // Reject event
      //    continue;
      // }


      // mHist["hNentries"]->Fill("signal",1);
   }
   ////////////////////////////////////////////////////////////////////////////
   // end of loop over tree entries
   ////////////////////////////////////////////////////////////////////////////
   // Get elapsed time
   sw.Stop();
   std::cout << "--- End of event loop: "; sw.Print();

   // --- Write histograms
   std::cout << "--- Write output file: " << "data/" + mSample[sampleName].app_output_file_name << std::endl;
   TFile *target  = new TFile( "data/" + mSample[sampleName].app_output_file_name, "RECREATE" );
   for( std::map<TString, TH1*>::iterator it = mHist.begin(); it != mHist.end(); it++)
      it->second->Write();
   // close file
   target->Close();

   // std::cout << "--- Created root file: signal DD \"" << target->GetName() << "\" containing the MVA output histograms" << std::endl;
  
   delete reader;
    
   std::cout << "==> Diffractive_TMVApplication is done!" << std::endl << std::endl;
} 

int main( int argc, char** argv )
{
   UNUSED(argc);
   UNUSED(argv);

   Diffractive_TMVApplication(); 
   return 0; 
}
  

  
