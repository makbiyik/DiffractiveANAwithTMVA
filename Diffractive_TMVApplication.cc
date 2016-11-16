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
void Diffractive_TMVApplication() 
{   

   //---------------------------------------------------------------

   // This loads the library
   TMVA::Tools::Instance();

   std::cout << std::endl;
   std::cout << "==> Start Diffractive_TMVApplication" << std::endl;

   // --------------------------------------------------------------------------------------------------
   // --- Create the Reader object
   TMVA::Reader *reader = new TMVA::Reader( "!Color:!Silent" );    

   // Create a set of variables and declare them to the reader
   // - the variable names MUST corresponds in name and type to those given in the weight file(s) used
   
   Float_t deltazero, etamax;
   Float_t etamin, log10XixReco, log10XiyReco;//log10XiDD;
   Float_t HFminusNtowers ,HFplusNtowers ,CastorNtowers,Ntracks,Pythia8processid,EventselectionXiprocessid;

   reader->AddVariable( "deltazero", &deltazero );
   reader->AddVariable( "etamax" , &etamax );
   reader->AddVariable( "etamin" , &etamin );
   reader->AddVariable( "log10XixReco" , &log10XixReco );
   reader->AddVariable( "log10XiyReco" , &log10XiyReco );
   reader->AddVariable( "HFminusNtowers" , &HFminusNtowers );
   reader->AddVariable( "HFplusNtowers" , &HFplusNtowers );
   reader->AddVariable( "CastorNtowers" , &CastorNtowers );
   reader->AddVariable( "Ntracks" ,&Ntracks );

   reader->AddVariable( "Pythia8processid" , &Pythia8processid );
   reader->AddVariable( "EventselectionXiprocessid" , &EventselectionXiprocessid );
   // reader->AddVariable("log10XiDD",&log10XiDD);


   ////////////////////////////////////////////////////////////////////////////
   std::map<TString, SampleList::sSample> mSample = SampleList::read_data_mc_files();
   std::map<TString, SampleList::sTMAOutput> mTMVAOuput = SampleList::read_TMVAOutput();
   ////////////////////////////////////////////////////////////////////////////
   TString sampleName = "Pythia8_BDTG_Pythia8Trained";
   TString inputSample = mTMVAOuput[sampleName].app_input_sample; 

   // --- Book the MVA methods
   TString weightfile = mTMVAOuput[sampleName].weight_name;
   TString methodName = mTMVAOuput[sampleName].method + " method";
   reader->BookMVA(methodName, weightfile ); 

   
   // Book output histograms
   std::map<TString, TH1*> mHist;

   mHist["hNentries"] = new TH1F("hNentries","hNentries",10,0,10);

   int NbrEtaBins = 50;
   double BinEtaMin = -5.5;
   double BinEtaMax = 4.5;
   int NbrDetaBins = 50;
   double BinDetaMin = 0;
   double BinDetaMax = 8;
   
   int NbrLogXiBins = 100;
   double BinLogXiMin = -11.5;
   double BinLogXiMax =0.5;
   
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


   ////////////////////////////////////////////////////////////////////////////
   std::cout << "--- TMVAClassification       : Using input file: " << mSample[inputSample].file->GetName() << std::endl;
 
   // --- Register the training and test trees   
   std::cout << "--- Select signal sample" << std::endl;
   TTree* theTree = (TTree*)mSample[inputSample].file->Get(mSample[inputSample].tree_name + "/AllTree");


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

   ////////////////////////////////////////////////////////////////////////////
   // loop over entries in InputTree
   ////////////////////////////////////////////////////////////////////////////
   std::cout << "--- Processing: " << theTree->GetEntries() << " events" << std::endl;
   TStopwatch sw;
   sw.Start();
   for (Long64_t ievt=0; ievt<theTree->GetEntries() && ievt<100000; ievt++) {

      if (ievt%1000 == 0) std::cout << "--- ... Processing event: " << ievt << std::endl;

      theTree->GetEntry(ievt);

      mHist["hNentries"]->Fill("all",1);

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

      mHist["hNentries"]->Fill("signal",1);

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
   }
   ////////////////////////////////////////////////////////////////////////////
   // end of loop over tree entries
   ////////////////////////////////////////////////////////////////////////////
   // Get elapsed time
   sw.Stop();
   std::cout << "--- End of event loop: "; sw.Print();

   // --- Write histograms
   TFile *target  = new TFile( "data/TMVApp_treesig_DD_training_pythi8_app_EOS.root","RECREATE" );
   for( std::map<TString, TH1*>::iterator it = mHist.begin(); it != mHist.end(); it++)
      it->second->Write();
   // close file
   target->Close();

   std::cout << "--- Created root file: signal DD \"" << target->GetName() << "\" containing the MVA output histograms" << std::endl;
  
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
  

  
