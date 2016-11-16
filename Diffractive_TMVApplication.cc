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




//Pythia8
TString get_Pythia_Process_ID(int Pythia8processid) {

   if(Pythia8processid == 101) {
      return "_Rest";
   } else if (Pythia8processid == 103) {
      return "_SD1";
   } else if (Pythia8processid == 104) {
      return "_SD2";
   } else if (Pythia8processid == 105) {
      return "_DD";
   } else if (Pythia8processid == 106) {
      return "_CD";
   }

   return "_NONE";
}



////////////////////////////////////////////////////////////////////////

void Diffractive_TMVApplication( TString myMethodList = "" ) 
{   

   //---------------------------------------------------------------

   // This loads the library
   TMVA::Tools::Instance();

   // Default MVA methods to be trained + tested
   std::map<TString,int> Use;
   Use["Fisher"]          = 0;
   Use["FisherG"]         = 0;
   Use["BoostedFisher"]   = 0; // uses generalised MVA method boosting
  

   // --- Boosted Decision Trees
   Use["BDT"]             = 0; // uses Adaptive Boost
   Use["BDTG"]            = 1; // uses Gradient Boost

  
   std::cout << std::endl;
   std::cout << "==> Start Diffractive_TMVApplication" << std::endl;

   // Select methods (don't look at this code - not of interest)
   if (myMethodList != "") {
      for (std::map<TString,int>::iterator it = Use.begin(); it != Use.end(); it++) it->second = 0;

      std::vector<TString> mlist = gTools().SplitString( myMethodList, ',' );
      for (UInt_t i=0; i<mlist.size(); i++) {
         TString regMethod(mlist[i]);

         if (Use.find(regMethod) == Use.end()) {
            std::cout << "Method \"" << regMethod 
                      << "\" not known in TMVA under this name. Choose among the following:" << std::endl;
            for (std::map<TString,int>::iterator it = Use.begin(); it != Use.end(); it++) {
               std::cout << it->first << " ";
            }
            std::cout << std::endl;
            return;
         }
         Use[regMethod] = 1;
      }
   }

   // --------------------------------------------------------------------------------------------------

   // --- Create the Reader object

   TMVA::Reader *reader = new TMVA::Reader( "!Color:!Silent" );    

   // Create a set of variables and declare them to the reader
   // - the variable names MUST corresponds in name and type to those given in the weight file(s) used
   
   Float_t deltazero, etamax;
   Float_t etamin, log10XixReco, log10XiyReco;//log10XiDD;
   Float_t HFminusNtowers ,HFplusNtowers ,CastorNtowers,Ntracks,Pythia8processid,EventselectionXiprocessid;

   reader->AddVariable( "deltazero", &deltazero );
   reader->AddVariable( "etamax", &etamax );
   reader->AddVariable( "etamin", &etamin );
   reader->AddVariable( "log10XixReco", &log10XixReco );
   reader->AddVariable("log10XiyReco", &log10XiyReco);
   reader->AddVariable("HFminusNtowers", &HFminusNtowers);
   reader->AddVariable("HFplusNtowers", &HFplusNtowers);
   reader->AddVariable("CastorNtowers", &CastorNtowers);
   reader->AddVariable("Ntracks",&Ntracks);

   reader->AddVariable("Pythia8processid",&Pythia8processid);
   reader->AddVariable("EventselectionXiprocessid",&EventselectionXiprocessid);
   // reader->AddVariable("log10XiDD",&log10XiDD);
 

   // --- Book the MVA methods
   TString dir    = "weights/";
   TString prefix = "TMVAClassification";


   // Book method(s)
   for (std::map<TString,int>::iterator it = Use.begin(); it != Use.end(); it++) {
      if (it->second) {
         TString methodName = TString(it->first) + TString(" method");
         TString weightfile = dir + prefix + TString("_") + TString(it->first) + TString(".weights.xml");
         reader->BookMVA(methodName, weightfile ); 
      }
   }
   
   // Book output histograms
   // Int_t nbin = 100;
   int NbrEtaBins = 50;
   double BinEtaMin = -5.5;
   double BinEtaMax = 4.5;
   int NbrDetaBins = 50;
   double BinDetaMin = 0;
   double BinDetaMax = 8;
   
   int NbrLogXiBins = 100;
   double BinLogXiMin = -11.5;
   double BinLogXiMax =0.5;

   std::map<TString, TH1*> mHist;
   
   // TH1F   *histFi(0), *histFiG(0), *histFiB(0);
   // TH1F   *histBdt(0), *histBdtG(0);
   
   ////////////////////////////////////////////////////////////////////////////
   // loop over proccess ID
   ////////////////////////////////////////////////////////////////////////////
   const int Nproc = 7;
   TString proccesses[Nproc] = {"","_CD","_DD","_SD1","_SD2","_Rest","_NONE"};
   TString histname;
   for(int iproc=0; iproc<Nproc; iproc++) {
      histname = TString("hSignal_etamin") + proccesses[iproc];
      mHist[histname] = new TH1F(histname,histname,NbrEtaBins, BinEtaMin, BinEtaMax);
      histname = TString("hSignal_etamax") + proccesses[iproc];
      mHist[histname] = new TH1F(histname,histname,NbrEtaBins, BinEtaMin, BinEtaMax);
      histname = TString("hSignal_deltazero") + proccesses[iproc];
      mHist[histname] = new TH1F(histname,histname,NbrDetaBins, BinDetaMin, BinDetaMax);
      histname = TString("hSignal_XixReco") + proccesses[iproc];
      mHist[histname] = new TH1F(histname,histname,NbrLogXiBins,BinLogXiMin,BinLogXiMax);
      histname = TString("hSignal_XiyReco") + proccesses[iproc];
      mHist[histname] = new TH1F(histname,histname,NbrLogXiBins,BinLogXiMin,BinLogXiMax);
      histname = TString("hSignal_HFplusNtowers") + proccesses[iproc];
      mHist[histname] = new TH1F(histname,histname,30, 0,300);
      histname = TString("hSignal_HFminusNtowers") + proccesses[iproc];
      mHist[histname] = new TH1F(histname,histname,30, 0,300);
      histname = TString("hSignal_CastorNtowers") + proccesses[iproc];
      mHist[histname] = new TH1F(histname,histname,16, 0,16) ;
      histname = TString("hsignal_NTracks") + proccesses[iproc];
      mHist[histname] = new TH1F(histname,histname,50, 0,50);
   
      histname = TString("hDisciminant") + proccesses[iproc];
      mHist[histname] = new TH1F(histname,histname,40,-1,1);

      histname = TString("hSignal_etamin_nodiscriminantcut") + proccesses[iproc];
      mHist[histname] = new TH1F(histname,histname,NbrEtaBins, BinEtaMin, BinEtaMax);
      histname = TString("hSignal_etamax_nodiscriminantcut") + proccesses[iproc];
      mHist[histname] = new TH1F(histname,histname,NbrEtaBins, BinEtaMin, BinEtaMax);
      histname = TString("hSignal_deltazero_nodiscriminantcut") + proccesses[iproc];
      mHist[histname] = new TH1F(histname,histname,NbrDetaBins, BinDetaMin, BinDetaMax);
      histname = TString("hSignal_XixReco_nodiscriminantcut") + proccesses[iproc];
      mHist[histname] = new TH1F(histname,histname,NbrLogXiBins,BinLogXiMin,BinLogXiMax);
      histname = TString("hSignal_XiyReco_nodiscriminantcut") + proccesses[iproc];
      mHist[histname] = new TH1F(histname,histname,NbrLogXiBins,BinLogXiMin,BinLogXiMax);
      histname = TString("hSignal_HFplusNtowers_nodiscriminantcut") + proccesses[iproc];
      mHist[histname] = new TH1F(histname,histname,30, 0,300);
      histname = TString("hSignal_HFminusNtowers_nodiscriminantcut") + proccesses[iproc];
      mHist[histname] = new TH1F(histname,histname,30, 0,300);
      histname = TString("hSignal_CastorNtowers_nodiscriminantcut") + proccesses[iproc];
      mHist[histname] = new TH1F(histname,histname,16, 0,16) ;
      histname = TString("hsignal_NTracks_nodiscriminantcut") + proccesses[iproc];
      mHist[histname] = new TH1F(histname,histname,50, 0,50);
   }
   // end loop over proccess ID
   ////////////////////////////////////////////////////////////////////////////
   
   
   // if (Use["Fisher"])        histFi      = new TH1F( "MVA_Fisher",        "MVA_Fisher",        nbin, -4, 4 );
   // if (Use["FisherG"])       histFiG     = new TH1F( "MVA_FisherG",       "MVA_FisherG",       nbin, -1, 1 );
   // if (Use["BoostedFisher"]) histFiB     = new TH1F( "MVA_BoostedFisher", "MVA_BoostedFisher", nbin, -2, 2 );
   
   // if (Use["BDT"])           histBdt     = new TH1F( "MVA_BDT",           "MVA_BDT",           nbin, -0.8, 0.8 );
   // if (Use["BDTG"])          histBdtG    = new TH1F( "MVA_BDTG",          "MVA_BDTG",          nbin, -1.0, 1.0 );


   // Book example histogram for probability (the other methods are done similarly)
   // TH1F *probHistFi(0), *rarityHistFi(0);
   // if (Use["Fisher"]) {
   //    probHistFi   = new TH1F( "MVA_Fisher_Proba",  "MVA_Fisher_Proba",  nbin, 0, 1 );
   //    rarityHistFi = new TH1F( "MVA_Fisher_Rarity", "MVA_Fisher_Rarity", nbin, 0, 1 );
   // }

   
   TString fname = "/home/lxadmin/MyRoot/root/tutorials/tmva/DiffractiveANAwithTMVA/data/trackanddiffractive_sigDD_epos.root";
   TFile *input = TFile::Open( fname );
   std::cout << "--- TMVAClassification       : Using input file: " << input->GetName() << std::endl;
 
   // --- Register the training and test trees
   
   std::cout << "--- Select signal sample" << std::endl;
   // TTree* theTree = (TTree*)input->Get("MinBias_TuneMBR_13TeV-pythia8_MagnetOff_CASTORmeasured_newNoise/sigTree");
   //Data
   // TTree* theTree = (TTree*)input->Get("data_ZeroBias1_CASTOR/AllTree");
   //pythia8
   // TTree* theTree = (TTree*)input->Get("MinBias_TuneMBR_13TeV-pythia8_MagnetOff_CASTORmeasured_newNoise/AllTree");
   // //EPOS
   TTree* theTree = (TTree*)input->Get("MinBias_EPOS_13TeV_MagnetOff_CASTORmeasured_newNoise/AllTree");
   
   



   Int_t HFminusNtowers_tree ,HFplusNtowers_tree ,CastorNtowers_tree,Ntracks_tree;
   Int_t Pythia8processid_tree, EventselectionXiprocessid_tree;
   
   theTree->SetBranchAddress("deltazero", &deltazero);
   theTree->SetBranchAddress("etamax", &etamax);
   theTree->SetBranchAddress("etamin", &etamin);
   theTree->SetBranchAddress("log10XixReco", &log10XixReco);
   theTree->SetBranchAddress("log10XiyReco", &log10XiyReco);
   // theTree->SetBranchAddress("log10XiDD",&log10XiDD);
   theTree->SetBranchAddress("HFminusNtowers", &HFminusNtowers_tree);
   theTree->SetBranchAddress("HFplusNtowers", &HFplusNtowers_tree);
   theTree->SetBranchAddress("CastorNtowers", &CastorNtowers_tree);
   theTree->SetBranchAddress("Ntracks",&Ntracks_tree);
   theTree->SetBranchAddress("Pythia8processid",&Pythia8processid_tree);
   theTree->SetBranchAddress("EventselectionXiprocessid",&EventselectionXiprocessid_tree);


   // Efficiency calculator for cut method
   // Int_t    nSelCutsGA = 0;
   // Double_t effS       = 0.7;

   // std::vector<Float_t> vecVar(8); // vector for EvaluateMVA tests

   std::cout << "--- Processing: " << theTree->GetEntries() << " events" << std::endl;
   TStopwatch sw;
   sw.Start();
   for (Long64_t ievt=0; ievt<theTree->GetEntries() && ievt<100000; ievt++) {

      if (ievt%1000 == 0) std::cout << "--- ... Processing event: " << ievt << std::endl;

      theTree->GetEntry(ievt);

      HFplusNtowers = (Float_t)HFplusNtowers_tree;
      HFminusNtowers = (Float_t)HFminusNtowers_tree;
      CastorNtowers = (Float_t)CastorNtowers_tree;
      Ntracks =(Float_t)Ntracks_tree;
      Pythia8processid=(Float_t)Pythia8processid_tree;
      EventselectionXiprocessid=(Float_t)EventselectionXiprocessid_tree;

      double discriminant_value = reader->EvaluateMVA("BDTG method");

      // Fill general hists
      mHist["hDisciminant"]->Fill(discriminant_value);
      mHist["hSignal_etamax_nodiscriminantcut"]->Fill(etamax); 
      mHist["hSignal_etamin_nodiscriminantcut"]->Fill(etamin); 
      mHist["hSignal_deltazero_nodiscriminantcut"]->Fill(deltazero); 
      mHist["hSignal_XixReco_nodiscriminantcut"]->Fill(log10XixReco); 
      mHist["hSignal_XiyReco_nodiscriminantcut"]->Fill(log10XiyReco); 
      mHist["hSignal_CastorNtowers_nodiscriminantcut"]->Fill(CastorNtowers); 
      mHist["hSignal_HFminusNtowers_nodiscriminantcut"]->Fill(HFminusNtowers);
      mHist["hSignal_HFplusNtowers_nodiscriminantcut"]->Fill(HFplusNtowers);
      mHist["hsignal_NTracks_nodiscriminantcut"]->Fill(Ntracks);

      // Fill hist depending on proccess ID string
      TString proccess = get_Pythia_Process_ID(Pythia8processid_tree);
      mHist[TString("hDisciminant") + proccess]->Fill(discriminant_value);
      mHist[TString("hSignal_etamax_nodiscriminantcut") + proccess]->Fill(etamax); 
      mHist[TString("hSignal_etamin_nodiscriminantcut") + proccess]->Fill(etamin); 
      mHist[TString("hSignal_deltazero_nodiscriminantcut") + proccess]->Fill(deltazero); 
      mHist[TString("hSignal_XixReco_nodiscriminantcut") + proccess]->Fill(log10XixReco); 
      mHist[TString("hSignal_XiyReco_nodiscriminantcut") + proccess]->Fill(log10XiyReco); 
      mHist[TString("hSignal_CastorNtowers_nodiscriminantcut") + proccess]->Fill(CastorNtowers); 
      mHist[TString("hSignal_HFminusNtowers_nodiscriminantcut") + proccess]->Fill(HFminusNtowers);
      mHist[TString("hSignal_HFplusNtowers_nodiscriminantcut") + proccess]->Fill(HFplusNtowers);
      mHist[TString("hsignal_NTracks_nodiscriminantcut") + proccess]->Fill(Ntracks);



      /////////////////////////////////////////////////////////////////////////
      // Start Own Code Example
      // I used Fisser method e.g.
      double discriminant_cut = 0.;
      if( discriminant_value < discriminant_cut) {
         // Reject event
         continue;
      }

      // Start to fill my histograms with events selected as DoubleDiffractive
      mHist[TString("hSignal_etamax") + proccess]->Fill(etamax); 
      mHist[TString("hSignal_etamin") + proccess]->Fill(etamin); 
      mHist[TString("hSignal_deltazero") + proccess]->Fill(deltazero); 
      mHist[TString("hSignal_XixReco") + proccess]->Fill(log10XixReco); 
      mHist[TString("hSignal_XiyReco") + proccess]->Fill(log10XiyReco); 
      mHist[TString("hSignal_CastorNtowers") + proccess]->Fill(CastorNtowers); 
      mHist[TString("hSignal_HFminusNtowers") + proccess]->Fill(HFminusNtowers);
      mHist[TString("hSignal_HFplusNtowers") + proccess]->Fill(HFplusNtowers);
      mHist[TString("hsignal_NTracks") + proccess]->Fill(Ntracks);
    
      mHist["hSignal_etamax"]->Fill(etamax); 
      mHist["hSignal_etamin"]->Fill(etamin); 
      mHist["hSignal_deltazero"]->Fill(deltazero); 
      mHist["hSignal_XixReco"]->Fill(log10XixReco); 
      mHist["hSignal_XiyReco"]->Fill(log10XiyReco); 
      mHist["hSignal_CastorNtowers"]->Fill(CastorNtowers); 
      mHist["hSignal_HFminusNtowers"]->Fill(HFminusNtowers);
      mHist["hSignal_HFplusNtowers"]->Fill(HFplusNtowers);
      mHist["hsignal_NTracks"]->Fill(Ntracks);
   
   

      /////////////////////////////////////////////////////////////////////////

     

      // // --- Return the MVA outputs and fill into histograms

      // if (Use["CutsGA"]) {
      //    // Cuts is a special case: give the desired signal efficienciy
      //    Bool_t passed = reader->EvaluateMVA( "CutsGA method", effS );
      //    if (passed) nSelCutsGA++;
      // }

      // if (Use["Fisher"       ])   histFi     ->Fill( reader->EvaluateMVA( "Fisher method"        ) );
      // if (Use["FisherG"      ])   histFiG    ->Fill( reader->EvaluateMVA( "FisherG method"       ) );
      // if (Use["BoostedFisher"])   histFiB    ->Fill( reader->EvaluateMVA( "BoostedFisher method" ) );
      // if (Use["BDT"          ])   histBdt    ->Fill( reader->EvaluateMVA( "BDT method"           ) );
      // if (Use["BDTG"         ])   histBdtG   ->Fill( reader->EvaluateMVA( "BDTG method"          ) );

      // // Retrieve probability instead of MVA output
      // if (Use["Fisher"])   {
      //    probHistFi  ->Fill( reader->GetProba ( "Fisher method" ) );
      //    rarityHistFi->Fill( reader->GetRarity( "Fisher method" ) );
      // }
   }

   // Get elapsed time
   sw.Stop();
   std::cout << "--- End of event loop: "; sw.Print();

   // Get efficiency for cuts classifier
   // if (Use["CutsGA"]) std::cout << "--- Efficiency for CutsGA method: " << double(nSelCutsGA)/theTree->GetEntries()
   //                              << " (for a required signal efficiency of " << effS << ")" << std::endl;

   // if (Use["CutsGA"]) {

   //    // test: retrieve cuts for particular signal efficiency
   //    // CINT ignores dynamic_casts so we have to use a cuts-secific Reader function to acces the pointer  
   //    TMVA::MethodCuts* mcuts = reader->FindCutsMVA( "CutsGA method" ) ;

   //    if (mcuts) {      
   //       std::vector<Double_t> cutsMin;
   //       std::vector<Double_t> cutsMax;
   //       mcuts->GetCuts( 0.7, cutsMin, cutsMax );
   //       std::cout << "--- -------------------------------------------------------------" << std::endl;
   //       std::cout << "--- Retrieve cut values for signal efficiency of 0.7 from Reader" << std::endl;
   //       for (UInt_t ivar=0; ivar<cutsMin.size(); ivar++) {
   //          std::cout << "... Cut: " 
   //                    << cutsMin[ivar] 
   //                    << " < \"" 
   //                    << mcuts->GetInputVar(ivar)
   //                    << "\" <= " 
   //                    << cutsMax[ivar] << std::endl;
   //       }
   //       std::cout << "--- -------------------------------------------------------------" << std::endl;
   //    }
   // }



   // --- Write histograms
   TFile *target  = new TFile( "data/TMVApp_treesig_DD_training_pythi8_app_EOS.root","RECREATE" );
   
   // if (Use["Fisher"       ])   histFi     ->Write();
   // if (Use["FisherG"      ])   histFiG    ->Write();
   // if (Use["BoostedFisher"])   histFiB    ->Write();
   // if (Use["BDT"          ])   histBdt    ->Write();
   // if (Use["BDTG"         ])   histBdtG   ->Write(); 
   
   for( std::map<TString, TH1*>::iterator it = mHist.begin(); it != mHist.end(); it++)
      it->second->Write(); 
  
   // Write also probability hists
   // if (Use["Fisher"]) { if (probHistFi != 0) probHistFi->Write(); if (rarityHistFi != 0) rarityHistFi->Write(); }
   target->Close();

   std::cout << "--- Created root file: signal DD \"" << target->GetName() << "\" containing the MVA output histograms" << std::endl;
  
   delete reader;
    
   std::cout << "==> Diffractive_TMVApplication is done!" << std::endl << std::endl;
} 

int main( int argc, char** argv )
{
   UNUSED(argc);
   UNUSED(argv);

   TString methodList = ""; 
   // for (int i=1; i<argc; i++) {
   //    TString regMethod(argv[i]);
   //    if(regMethod=="-b" || regMethod=="--batch") continue;
   //    if (!methodList.IsNull()) methodList += TString(","); 
   //    methodList += regMethod;
   // }
   Diffractive_TMVApplication(methodList); 
   return 0; 
}
  

  
