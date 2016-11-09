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

void Diffractive_TMVApplication( TString myMethodList = "" ) 
{   

   //---------------------------------------------------------------

   // This loads the library
   TMVA::Tools::Instance();

   // Default MVA methods to be trained + tested
   std::map<std::string,int> Use;
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
      for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) it->second = 0;

      std::vector<TString> mlist = gTools().SplitString( myMethodList, ',' );
      for (UInt_t i=0; i<mlist.size(); i++) {
         std::string regMethod(mlist[i]);

         if (Use.find(regMethod) == Use.end()) {
            std::cout << "Method \"" << regMethod 
                      << "\" not known in TMVA under this name. Choose among the following:" << std::endl;
            for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) {
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
   Float_t HFminusNtowers ,HFplusNtowers ,CastorNtowers,Ntracks,Pythia8processid ;
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
   // reader->AddVariable("log10XiDD",&log10XiDD);
 

   // --- Book the MVA methods
   TString dir    = "weights/";
   TString prefix = "TMVAClassification";


   // Book method(s)
   for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) {
      if (it->second) {
         TString methodName = TString(it->first) + TString(" method");
         TString weightfile = dir + prefix + TString("_") + TString(it->first) + TString(".weights.xml");
         reader->BookMVA(methodName, weightfile ); 
      }
   }
   
   // Book output histograms
   Int_t nbin = 100;
   Int_t NbrEtaBins,NbrDetaBins, BinDetaMin, BinDetaMax,NbrLogXiBins;
   Float_t BinEtaMin,BinEtaMax,BinLogXiMin,BinLogXiMax;
   NbrEtaBins = 50;
   BinEtaMin = -5.5;
   BinEtaMax = 4.5;
   NbrDetaBins = 50;
   BinDetaMin = 0;
   BinDetaMax = 8;
   
   NbrLogXiBins = 100;
   BinLogXiMin = -11.5;
   BinLogXiMax =0.5;


   TH1F   *histFi(0), *histFiG(0), *histFiB(0);
   TH1F   *histBdt(0), *histBdtG(0);
   // TH1F   *histSVMP(0), *histSVML(0), *histFDAMT(0), *histFDAGA(0), *histCat(0), *histPBdt(0);
   TH1F *hSignal_deltazero(0),*hSignal_etamax(0),*hSignal_etamin(0) ; 
   TH1F *hSignal_CastorNtowers(0),*hSignal_HFminusNtowers(0),*hSignal_HFplusNtowers(0), *hsignal_NTracks(0); 
   TH1F *hSignal_XixReco(0),*hSignal_XiyReco(0);//*hsignal_XiDD(0);
   
   TH1F *hSignal_deltazero_nodiscriminantcut(0),*hSignal_etamax_nodiscriminantcut(0),*hSignal_etamin_nodiscriminantcut(0) ; 
   TH1F *hSignal_CastorNtowers_nodiscriminantcut(0),*hSignal_HFminusNtowers_nodiscriminantcut(0),*hSignal_HFplusNtowers_nodiscriminantcut(0), *hsignal_NTracks_nodiscriminantcut(0); 
   TH1F *hSignal_XixReco_nodiscriminantcut(0),*hSignal_XiyReco_nodiscriminantcut(0);//*hsignal_XiDD(0);


   
   hSignal_etamin =  new TH1F("hSignal_etamin", "hSignal_etamin", NbrEtaBins, BinEtaMin, BinEtaMax);
   hSignal_etamax =  new TH1F("hSignal_etamax", "hSignal_etamax", NbrEtaBins, BinEtaMin, BinEtaMax);
   hSignal_deltazero = new TH1F("hSignal_deltazero", "hSignal_deltazero", NbrDetaBins, BinDetaMin, BinDetaMax);
   hSignal_XixReco = new TH1F("hSignal_XixReco","hSignal_XixReco",NbrLogXiBins,BinEtaMin,BinLogXiMax);
   hSignal_XiyReco = new TH1F("hSignal_XiyReco","hSignal_XiyReco",NbrLogXiBins,BinLogXiMin,BinLogXiMax);
   // hsignal_XiDD = new TH1F("hsignal_XiDD","hsignal_XiDD",nbin,-11.5,0.5);
   hSignal_HFplusNtowers = new TH1F("hSignal_HFplusNtowers","hSignal_HFplusNtowers",30, 0,300);
   hSignal_HFminusNtowers = new TH1F("hSignal_HFminusNtowers","hSignal_HFminusNtowers",30, 0,300) ;
   hSignal_CastorNtowers = new TH1F("hSignal_CastorNtowers","hSignal_CastorNtowers",16, 0,16) ;
   hsignal_NTracks = new TH1F("hsignal_NTracks","hsignal_NTracks",16, 0,16) ;
   

   hSignal_etamin_nodiscriminantcut =  new TH1F("hSignal_etamin_nodiscriminantcut", "hSignal_etamin_nodiscriminantcut", NbrEtaBins, BinEtaMin, BinEtaMax);
   hSignal_etamax_nodiscriminantcut =  new TH1F("hSignal_etamax_nodiscriminantcut", "hSignal_etamax_nodiscriminantcut", NbrEtaBins, BinEtaMin, BinEtaMax);
   hSignal_deltazero_nodiscriminantcut = new TH1F("hSignal_deltazero_nodiscriminantcut", "hSignal_deltazero_nodiscriminantcut", NbrDetaBins, BinDetaMin, BinDetaMax);
   hSignal_XixReco_nodiscriminantcut = new TH1F("hSignal_XixReco_nodiscriminantcut","hSignal_XixReco_nodiscriminantcut",NbrLogXiBins,BinEtaMin,BinLogXiMax);
   hSignal_XiyReco_nodiscriminantcut = new TH1F("hSignal_XiyReco_nodiscriminantcut","hSignal_XiyReco_nodiscriminantcut",NbrLogXiBins,BinLogXiMin,BinLogXiMax);
   // hsignal_XiDD = new TH1F("hsignal_XiDD","hsignal_XiDD",nbin,-11.5,0.5);
   hSignal_HFplusNtowers_nodiscriminantcut = new TH1F("hSignal_HFplusNtowers_nodiscriminantcut","hSignal_HFplusNtowers_nodiscriminantcut",30, 0,300);
   hSignal_HFminusNtowers_nodiscriminantcut = new TH1F("hSignal_HFminusNtowers_nodiscriminantcut","hSignal_HFminusNtowers_nodiscriminantcut",30, 0,300) ;
   hSignal_CastorNtowers_nodiscriminantcut = new TH1F("hSignal_CastorNtowers_nodiscriminantcut","hSignal_CastorNtowers_nodiscriminantcut",16, 0,16) ;
   hsignal_NTracks_nodiscriminantcut = new TH1F("hsignal_NTracks_nodiscriminantcut","hsignal_NTracks_nodiscriminantcut",16, 0,16) ;
       




   
   if (Use["Fisher"])        histFi      = new TH1F( "MVA_Fisher",        "MVA_Fisher",        nbin, -4, 4 );
   if (Use["FisherG"])       histFiG     = new TH1F( "MVA_FisherG",       "MVA_FisherG",       nbin, -1, 1 );
   if (Use["BoostedFisher"]) histFiB     = new TH1F( "MVA_BoostedFisher", "MVA_BoostedFisher", nbin, -2, 2 );
   
   if (Use["BDT"])           histBdt     = new TH1F( "MVA_BDT",           "MVA_BDT",           nbin, -0.8, 0.8 );
   if (Use["BDTG"])          histBdtG    = new TH1F( "MVA_BDTG",          "MVA_BDTG",          nbin, -1.0, 1.0 );


   // Book example histogram for probability (the other methods are done similarly)
   TH1F *probHistFi(0), *rarityHistFi(0);
   if (Use["Fisher"]) {
      probHistFi   = new TH1F( "MVA_Fisher_Proba",  "MVA_Fisher_Proba",  nbin, 0, 1 );
      rarityHistFi = new TH1F( "MVA_Fisher_Rarity", "MVA_Fisher_Rarity", nbin, 0, 1 );
   }

   // Prepare input tree (this must be replaced by your data source)
   // in this example, there is a toy tree with signal and one with background events
   // we'll later on use only the "signal" events for the test in this example.
   //   
   TString fname = "/home/lxadmin/MyRoot/root/tutorials/tmva/DiffractiveANAwithTMVA/data/trackanddiffractive_sigDD_Epos.root";
   TFile *input = TFile::Open( fname );
   std::cout << "--- TMVAClassification       : Using input file: " << input->GetName() << std::endl;
 
   // --- Register the training and test trees
   
   std::cout << "--- Select signal sample" << std::endl;
   // TTree* theTree = (TTree*)input->Get("MinBias_TuneMBR_13TeV-pythia8_MagnetOff_CASTORmeasured_newNoise/sigTree");
   
   TTree* theTree = (TTree*)input->Get("MinBias_EPOS_13TeV_MagnetOff_CASTORmeasured_newNoise/AllTree");
   Int_t HFminusNtowers_tree ,HFplusNtowers_tree ,CastorNtowers_tree,Ntracks_tree,Pythia8processid_tree;
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
 

   // Efficiency calculator for cut method
   Int_t    nSelCutsGA = 0;
   Double_t effS       = 0.7;

   std::vector<Float_t> vecVar(8); // vector for EvaluateMVA tests

   std::cout << "--- Processing: " << theTree->GetEntries() << " events" << std::endl;
   TStopwatch sw;
   sw.Start();
   for (Long64_t ievt=0; ievt<theTree->GetEntries() && ievt<10000; ievt++) {

      if (ievt%1000 == 0) std::cout << "--- ... Processing event: " << ievt << std::endl;

      theTree->GetEntry(ievt);

      HFplusNtowers = (Float_t)HFplusNtowers_tree;
      HFminusNtowers = (Float_t)HFminusNtowers_tree;
      CastorNtowers = (Float_t)CastorNtowers_tree;
      Ntracks =(Float_t)Ntracks_tree;
      Pythia8processid = (Float_t)Pythia8processid_tree;
      //var1 = userVar1 + userVar2;
      //var2 = userVar1 - userVar2;
      hSignal_deltazero_nodiscriminantcut->Fill(deltazero);
      hSignal_etamax_nodiscriminantcut->Fill(etamax);
      hSignal_etamin_nodiscriminantcut->Fill(etamin);
      hSignal_XixReco_nodiscriminantcut->Fill(log10XixReco); // Only filled for with TMVA selected DD events 
      hSignal_XiyReco_nodiscriminantcut->Fill(log10XiyReco);
      // hsignal_XiDD->Fill(log10XiDD);
      hSignal_CastorNtowers_nodiscriminantcut->Fill(CastorNtowers);
      hSignal_HFminusNtowers_nodiscriminantcut->Fill(HFminusNtowers);
      hSignal_HFplusNtowers_nodiscriminantcut->Fill(HFplusNtowers);
      hsignal_NTracks_nodiscriminantcut->Fill(Ntracks);

      /////////////////////////////////////////////////////////////////////////
      // Start Own Code Example
      // I used Fisser method e.g.
      double discriminant_value = reader->EvaluateMVA("BDTG method");
      double discriminant_cut = 0.;
   

      



      if( discriminant_value < discriminant_cut) {
         // Reject event
         continue;
      }

      // Start to fill my histograms with events selected as DoubleDiffractive
      
      hSignal_deltazero->Fill(deltazero);
      hSignal_etamax->Fill(etamax);
      hSignal_etamin->Fill(etamin);
      hSignal_XixReco->Fill(log10XixReco); // Only filled for with TMVA selected DD events 
      hSignal_XiyReco->Fill(log10XiyReco);
      // hsignal_XiDD->Fill(log10XiDD);
      hSignal_CastorNtowers->Fill(CastorNtowers);
      hSignal_HFminusNtowers->Fill(HFminusNtowers);
      hSignal_HFplusNtowers->Fill(HFplusNtowers);
      hsignal_NTracks->Fill(Ntracks);

   
   

      /////////////////////////////////////////////////////////////////////////

     

      // --- Return the MVA outputs and fill into histograms

      if (Use["CutsGA"]) {
         // Cuts is a special case: give the desired signal efficienciy
         Bool_t passed = reader->EvaluateMVA( "CutsGA method", effS );
         if (passed) nSelCutsGA++;
      }

      if (Use["Fisher"       ])   histFi     ->Fill( reader->EvaluateMVA( "Fisher method"        ) );
      if (Use["FisherG"      ])   histFiG    ->Fill( reader->EvaluateMVA( "FisherG method"       ) );
      if (Use["BoostedFisher"])   histFiB    ->Fill( reader->EvaluateMVA( "BoostedFisher method" ) );
      if (Use["BDT"          ])   histBdt    ->Fill( reader->EvaluateMVA( "BDT method"           ) );
      if (Use["BDTG"         ])   histBdtG   ->Fill( reader->EvaluateMVA( "BDTG method"          ) );

      // Retrieve probability instead of MVA output
      if (Use["Fisher"])   {
         probHistFi  ->Fill( reader->GetProba ( "Fisher method" ) );
         rarityHistFi->Fill( reader->GetRarity( "Fisher method" ) );
      }
   }

   // Get elapsed time
   sw.Stop();
   std::cout << "--- End of event loop: "; sw.Print();

   // Get efficiency for cuts classifier
   if (Use["CutsGA"]) std::cout << "--- Efficiency for CutsGA method: " << double(nSelCutsGA)/theTree->GetEntries()
                                << " (for a required signal efficiency of " << effS << ")" << std::endl;

   if (Use["CutsGA"]) {

      // test: retrieve cuts for particular signal efficiency
      // CINT ignores dynamic_casts so we have to use a cuts-secific Reader function to acces the pointer  
      TMVA::MethodCuts* mcuts = reader->FindCutsMVA( "CutsGA method" ) ;

      if (mcuts) {      
         std::vector<Double_t> cutsMin;
         std::vector<Double_t> cutsMax;
         mcuts->GetCuts( 0.7, cutsMin, cutsMax );
         std::cout << "--- -------------------------------------------------------------" << std::endl;
         std::cout << "--- Retrieve cut values for signal efficiency of 0.7 from Reader" << std::endl;
         for (UInt_t ivar=0; ivar<cutsMin.size(); ivar++) {
            std::cout << "... Cut: " 
                      << cutsMin[ivar] 
                      << " < \"" 
                      << mcuts->GetInputVar(ivar)
                      << "\" <= " 
                      << cutsMax[ivar] << std::endl;
         }
         std::cout << "--- -------------------------------------------------------------" << std::endl;
      }
   }

   // --- Write histograms

   TFile *target  = new TFile( "data/TMVApp_DD.root","RECREATE" );
  
   if (Use["Fisher"       ])   histFi     ->Write();
   if (Use["FisherG"      ])   histFiG    ->Write();
   if (Use["BoostedFisher"])   histFiB    ->Write();
  
   if (Use["BDT"          ])   histBdt    ->Write();
   if (Use["BDTG"         ])   histBdtG   ->Write(); 
    
  
   hSignal_deltazero->Write();
   hSignal_etamax->Write();
   hSignal_etamin->Write();
   hSignal_XixReco->Write(); // Only filled for with TMVA selected DD events 
   hSignal_XiyReco->Write();
   // hsignal_XiDD->Write();
   hSignal_CastorNtowers->Write();
   hSignal_HFminusNtowers->Write();
   hSignal_HFplusNtowers->Write();
   hsignal_NTracks->Write();

   
   hSignal_deltazero_nodiscriminantcut->Write();
   hSignal_etamax_nodiscriminantcut->Write();
   hSignal_etamin_nodiscriminantcut->Write();
   hSignal_XixReco_nodiscriminantcut->Write(); // Only filled for with TMVA selected DD events 
   hSignal_XiyReco_nodiscriminantcut->Write();
   // hsignal_XiDD->Write();
   hSignal_CastorNtowers_nodiscriminantcut->Write();
   hSignal_HFminusNtowers_nodiscriminantcut->Write();
   hSignal_HFplusNtowers_nodiscriminantcut->Write();
   hsignal_NTracks_nodiscriminantcut->Write();


   // Write also probability hists
   if (Use["Fisher"]) { if (probHistFi != 0) probHistFi->Write(); if (rarityHistFi != 0) rarityHistFi->Write(); }
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
  

  
