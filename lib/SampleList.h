#ifndef SAMPLELIST_H
#define SAMPLELIST_H

#include <map>

#include "TString.h"
#include "TH1.h"
#include "TFile.h"

namespace SampleList {

  struct sSample
  {
    bool isData;
    bool procesesID_pythia8;
    double lumi;
    double xs; // in ub
    

    TFile* file;
    TString tree_name;

    TString weight_name;
    TString signal;
  };

  //////////////////////////////////////////////////////////////////////////
  std::map<TString, sSample> read_data_mc_files() {
    TH1F * htmp;

    sSample sPythia8DD;
    sPythia8DD.isData = false;
    sPythia8DD.procesesID_pythia8 = true;
    sPythia8DD.xs = 71390.000000; // ub
    sPythia8DD.file = TFile::Open("data/trackanddiffractive_sigDD_pythia8.root");
    sPythia8DD.tree_name = "MinBias_TuneMBR_13TeV-pythia8_MagnetOff_CASTORmeasured_newNoise";
    sPythia8DD.weight_name = "Pythia8_DD_weights";
    sPythia8DD.signal = "DD";
    htmp = (TH1F*)sPythia8DD.file->Get(sPythia8DD.tree_name + "/hNentries");
    sPythia8DD.lumi = htmp->GetBinContent(1)/sPythia8DD.xs;


    sSample sXiEventSelectioncutDD;
    sXiEventSelectioncutDD.isData = false;
    sXiEventSelectioncutDD.procesesID_pythia8 = false;
    sXiEventSelectioncutDD.lumi = 0;
    sXiEventSelectioncutDD.xs = 79948.200000; // ub
    sXiEventSelectioncutDD.file = TFile::Open("data/trackanddiffractive_sigDD_XiCutpythia8.root");
    sXiEventSelectioncutDD.tree_name = "MinBias_TuneMBR_13TeV-pythia8_MagnetOff_CASTORmeasured_newNoise";
    sXiEventSelectioncutDD.weight_name = "Pythia8_XiEvtSel_DD_weights";
    sXiEventSelectioncutDD.signal = "DD";
    htmp = (TH1F*)sXiEventSelectioncutDD.file->Get(sXiEventSelectioncutDD.tree_name + "/hNentries");
    sXiEventSelectioncutDD.lumi = htmp->GetBinContent(1)/sXiEventSelectioncutDD.xs;

    sSample sEPOSDD;
    sEPOSDD.isData = false;
    sEPOSDD.procesesID_pythia8 = true;
    sEPOSDD.lumi = 0;
    sEPOSDD.xs = 79948.200000; // ub
    sEPOSDD.file = TFile::Open("data/trackanddiffractive_sigDD_epos.root");
    sEPOSDD.tree_name = "MinBias_EPOS_13TeV_MagnetOff_CASTORmeasured_newNoise";
    sEPOSDD.weight_name = "EPOS_DD_weights";
    sEPOSDD.signal = "DD";
    htmp = (TH1F*)sEPOSDD.file->Get(sEPOSDD.tree_name + "/hNentries");
    sEPOSDD.lumi = htmp->GetBinContent(1)/sEPOSDD.xs;

    sSample sXicutEPOsDD;
    sXicutEPOsDD.isData = false;
    sXicutEPOsDD.procesesID_pythia8 = false;
    sXicutEPOsDD.lumi = 0;
    sXicutEPOsDD.xs = 79948.200000; // ub
    sXicutEPOsDD.file = TFile::Open("data/trackanddiffractive_sigDD_XiCutepos.root");
    sXicutEPOsDD.tree_name = "MinBias_EPOS_13TeV_MagnetOff_CASTORmeasured_newNoise";
    sXicutEPOsDD.weight_name = "XiCutEPOS_DD_weights";
    sXicutEPOsDD.signal = "DD";
    htmp = (TH1F*)sXicutEPOsDD.file->Get(sXicutEPOsDD.tree_name + "/hNentries");
    sXicutEPOsDD.lumi = htmp->GetBinContent(1)/sXicutEPOsDD.xs;
    
    sSample sData;
    sData.isData = true;
    sData.lumi = 0.34; // 1/nb
    sData.xs = 0;
    sData.file = TFile::Open("data/trackanddiffractive_sigDD_data.root");
    sData.tree_name = "data_ZeroBias1_CASTOR";
    sData.weight_name = "";
    sData.signal = "";



   //////////////////////////SD1//////////////////////////////////

    sSample sPythia8SD1;
    sPythia8SD1.isData = false;
    sPythia8SD1.procesesID_pythia8 = true;
    sPythia8SD1.xs = 71390.000000; // ub
    sPythia8SD1.file = TFile::Open("data/trackanddiffractive_sigSD1_pythia8.root");
    sPythia8SD1.tree_name = "MinBias_TuneMBR_13TeV-pythia8_MagnetOff_CASTORmeasured_newNoise";
    sPythia8SD1.weight_name = "Pythia8_SD1_weights";
    sPythia8SD1.signal = "SD1";
    htmp = (TH1F*)sPythia8SD1.file->Get(sPythia8SD1.tree_name + "/hNentries");
    sPythia8SD1.lumi = htmp->GetBinContent(1)/sPythia8SD1.xs;


    sSample sXiEventSelectioncutSD1;
    sXiEventSelectioncutSD1.isData = false;
    sXiEventSelectioncutSD1.procesesID_pythia8 = false;
    sXiEventSelectioncutSD1.lumi = 0;
    sXiEventSelectioncutSD1.xs = 79948.200000; // ub
    sXiEventSelectioncutSD1.file = TFile::Open("data/trackanddiffractive_sigSD1_XiCutpythia8.root");
    sXiEventSelectioncutSD1.tree_name = "MinBias_TuneMBR_13TeV-pythia8_MagnetOff_CASTORmeasured_newNoise";
    sXiEventSelectioncutSD1.weight_name = "Pythia8_XiEvtSel_SD1_weights";
    sXiEventSelectioncutSD1.signal = "SD1";
    htmp = (TH1F*)sXiEventSelectioncutSD1.file->Get(sXiEventSelectioncutSD1.tree_name + "/hNentries");
    sXiEventSelectioncutSD1.lumi = htmp->GetBinContent(1)/sXiEventSelectioncutSD1.xs;

    sSample sEPOSSD1;
    sEPOSSD1.isData = false;
    sEPOSSD1.procesesID_pythia8 = true;
    sEPOSSD1.lumi = 0;
    sEPOSSD1.xs = 79948.200000; // ub
    sEPOSSD1.file = TFile::Open("data/trackanddiffractive_sigSD1_epos.root");
    sEPOSSD1.tree_name = "MinBias_EPOS_13TeV_MagnetOff_CASTORmeasured_newNoise";
    sEPOSSD1.weight_name = "EPOS_SD1_weights";
    sEPOSSD1.signal = "SD1";
    htmp = (TH1F*)sEPOSSD1.file->Get(sEPOSSD1.tree_name + "/hNentries");
    sEPOSSD1.lumi = htmp->GetBinContent(1)/sEPOSSD1.xs;

    sSample sXicutEPOsSD1;
    sXicutEPOsSD1.isData = false;
    sXicutEPOsSD1.procesesID_pythia8 = false;
    sXicutEPOsSD1.lumi = 0;
    sXicutEPOsSD1.xs = 79948.200000; // ub
    sXicutEPOsSD1.file = TFile::Open("data/trackanddiffractive_sigSD1_XiCutepos.root");
    sXicutEPOsSD1.tree_name = "MinBias_EPOS_13TeV_MagnetOff_CASTORmeasured_newNoise";
    sXicutEPOsSD1.weight_name = "XiCutEPOS_SD1_weights";
    sXicutEPOsSD1.signal = "SD1";
    htmp = (TH1F*)sXicutEPOsSD1.file->Get(sXicutEPOsSD1.tree_name + "/hNentries");
    sXicutEPOsSD1.lumi = htmp->GetBinContent(1)/sXicutEPOsSD1.xs;
    
   
    //////////////////////////SD2//////////////////////////////////

    sSample sPythia8SD2;
    sPythia8SD2.isData = false;
    sPythia8SD2.procesesID_pythia8 = true;
    sPythia8SD2.xs = 71390.000000; // ub
    sPythia8SD2.file = TFile::Open("data/trackanddiffractive_sigSD2_pythia8.root");
    sPythia8SD2.tree_name = "MinBias_TuneMBR_13TeV-pythia8_MagnetOff_CASTORmeasured_newNoise";
    sPythia8SD2.weight_name = "Pythia8_SD2_weights";
    sPythia8SD2.signal = "SD2";
    htmp = (TH1F*)sPythia8SD2.file->Get(sPythia8SD2.tree_name + "/hNentries");
    sPythia8SD2.lumi = htmp->GetBinContent(1)/sPythia8SD2.xs;


    sSample sXiEventSelectioncutSD2;
    sXiEventSelectioncutSD2.isData = false;
    sXiEventSelectioncutSD2.procesesID_pythia8 = false;
    sXiEventSelectioncutSD2.lumi = 0;
    sXiEventSelectioncutSD2.xs = 79948.200000; // ub
    sXiEventSelectioncutSD2.file = TFile::Open("data/trackanddiffractive_sigSD2_XiCutpythia8.root");
    sXiEventSelectioncutSD2.tree_name = "MinBias_TuneMBR_13TeV-pythia8_MagnetOff_CASTORmeasured_newNoise";
    sXiEventSelectioncutSD2.weight_name = "Pythia8_XiEvtSel_SD2_weights";
    sXiEventSelectioncutSD2.signal = "SD2";
    htmp = (TH1F*)sXiEventSelectioncutSD2.file->Get(sXiEventSelectioncutSD2.tree_name + "/hNentries");
    sXiEventSelectioncutSD2.lumi = htmp->GetBinContent(1)/sXiEventSelectioncutSD2.xs;

    sSample sEPOSSD2;
    sEPOSSD2.isData = false;
    sEPOSSD2.procesesID_pythia8 = true;
    sEPOSSD2.lumi = 0;
    sEPOSSD2.xs = 79948.200000; // ub
    sEPOSSD2.file = TFile::Open("data/trackanddiffractive_sigSD2_epos.root");
    sEPOSSD2.tree_name = "MinBias_EPOS_13TeV_MagnetOff_CASTORmeasured_newNoise";
    sEPOSSD2.weight_name = "EPOS_SD2_weights";
    sEPOSSD2.signal = "SD2";
    htmp = (TH1F*)sEPOSSD2.file->Get(sEPOSSD2.tree_name + "/hNentries");
    sEPOSSD2.lumi = htmp->GetBinContent(1)/sEPOSSD2.xs;

    sSample sXicutEPOsSD2;
    sXicutEPOsSD2.isData = false;
    sXicutEPOsSD2.procesesID_pythia8 = false;
    sXicutEPOsSD2.lumi = 0;
    sXicutEPOsSD2.xs = 79948.200000; // ub
    sXicutEPOsSD2.file = TFile::Open("data/trackanddiffractive_sigSD2_XiCutepos.root");
    sXicutEPOsSD2.tree_name = "MinBias_EPOS_13TeV_MagnetOff_CASTORmeasured_newNoise";
    sXicutEPOsSD2.weight_name = "XiCutEPOS_SD2_weights";
    sXicutEPOsSD2.signal = "SD2";
    htmp = (TH1F*)sXicutEPOsSD2.file->Get(sXicutEPOsSD2.tree_name + "/hNentries");
    sXicutEPOsSD2.lumi = htmp->GetBinContent(1)/sXicutEPOsSD2.xs;
    
  
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

     //////////////////////////REST//////////////////////////////////

    sSample sPythia8Rest;
    sPythia8Rest.isData = false;
    sPythia8Rest.procesesID_pythia8 = true;
    sPythia8Rest.xs = 71390.000000; // ub
    sPythia8Rest.file = TFile::Open("data/trackanddiffractive_sigRest_pythia8.root");
    sPythia8Rest.tree_name = "MinBias_TuneMBR_13TeV-pythia8_MagnetOff_CASTORmeasured_newNoise";
    sPythia8Rest.weight_name = "Pythia8_Rest_weights";
    sPythia8Rest.signal = "Rest";
    htmp = (TH1F*)sPythia8Rest.file->Get(sPythia8Rest.tree_name + "/hNentries");
    sPythia8Rest.lumi = htmp->GetBinContent(1)/sPythia8Rest.xs;


    sSample sXiEventSelectioncutRest;
    sXiEventSelectioncutRest.isData = false;
    sXiEventSelectioncutRest.procesesID_pythia8 = false;
    sXiEventSelectioncutRest.lumi = 0;
    sXiEventSelectioncutRest.xs = 79948.200000; // ub
    sXiEventSelectioncutRest.file = TFile::Open("data/trackanddiffractive_sigRest_XiCutpythia8.root");
    sXiEventSelectioncutRest.tree_name = "MinBias_TuneMBR_13TeV-pythia8_MagnetOff_CASTORmeasured_newNoise";
    sXiEventSelectioncutRest.weight_name = "Pythia8_XiEvtSel_Rest_weights";
    sXiEventSelectioncutRest.signal = "Rest";
    htmp = (TH1F*)sXiEventSelectioncutRest.file->Get(sXiEventSelectioncutRest.tree_name + "/hNentries");
    sXiEventSelectioncutRest.lumi = htmp->GetBinContent(1)/sXiEventSelectioncutRest.xs;

    sSample sEPOSRest;
    sEPOSRest.isData = false;
    sEPOSRest.procesesID_pythia8 = true;
    sEPOSRest.lumi = 0;
    sEPOSRest.xs = 79948.200000; // ub
    sEPOSRest.file = TFile::Open("data/trackanddiffractive_sigRest_epos.root");
    sEPOSRest.tree_name = "MinBias_EPOS_13TeV_MagnetOff_CASTORmeasured_newNoise";
    sEPOSRest.weight_name = "EPOS_Rest_weights";
    sEPOSRest.signal = "Rest";
    htmp = (TH1F*)sEPOSRest.file->Get(sEPOSRest.tree_name + "/hNentries");
    sEPOSRest.lumi = htmp->GetBinContent(1)/sEPOSRest.xs;

    sSample sXicutEPOsRest;
    sXicutEPOsRest.isData = false;
    sXicutEPOsRest.procesesID_pythia8 = false;
    sXicutEPOsRest.lumi = 0;
    sXicutEPOsRest.xs = 79948.200000; // ub
    sXicutEPOsRest.file = TFile::Open("data/trackanddiffractive_sigRest_XiCutepos.root");
    sXicutEPOsRest.tree_name = "MinBias_EPOS_13TeV_MagnetOff_CASTORmeasured_newNoise";
    sXicutEPOsRest.weight_name = "XiCutEPOS_Rest_weights";
    sXicutEPOsRest.signal = "Rest";
    htmp = (TH1F*)sXicutEPOsRest.file->Get(sXicutEPOsRest.tree_name + "/hNentries");
    sXicutEPOsRest.lumi = htmp->GetBinContent(1)/sXicutEPOsRest.xs;
    

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::map<TString, sSample> mSample;

    mSample["Pythia8"] =  sPythia8DD;
    mSample["XiCutPythia8"] =  sXiEventSelectioncutDD;
    mSample["XiCutEPOS"] =  sXicutEPOsDD;
    mSample["EPOS"] = sEPOSDD;
    mSample["Data"] = sData;
    mSample["Pythia8SD1"] =  sPythia8SD1;
    mSample["XiCutPythia8SD1"] =  sXiEventSelectioncutSD1;
    mSample["XiCutEPOSSD1"] =  sXicutEPOsSD1;
    mSample["EPOSSD1"] = sEPOSSD1;
    mSample["Pythia8SD2"] =  sPythia8SD2;
    mSample["XiCutPythia8SD2"] =  sXiEventSelectioncutSD2;
    mSample["XiCutEPOSSD2"] =  sXicutEPOsSD2;
    mSample["EPOSSD2"] = sEPOSSD2;
    

    mSample["Pythia8Rest"] =  sPythia8Rest;
    mSample["XiCutPythia8Rest"] =  sXiEventSelectioncutRest;
    mSample["XiCutEPOSRest"] =  sXicutEPOsRest;
    mSample["EPOSRest"] = sEPOSRest;






    return mSample;
  }
  //////////////////////////////////////////////////////////////////////////



  struct sTMVAOutput
  {
    TString method;

    TString training_sample;
    TString app_input_sample;
    TString app_output_file_name;
  };


  //////////////////////////////////////////////////////////////////////////
  std::map<TString, sTMVAOutput> read_TMVAOutput() {
    std::map<TString, sTMVAOutput> mTMVAOutput;

    //////////////////////////////////////////////////////////////////////////
    
   // Monte Carlo
 
    ////////////////////// SD1////////////////////////////////////////////////////

    mTMVAOutput["Pythia8SD1_BDTG_Pythia8TrainedSD1"].method = "BDTG";
    mTMVAOutput["Pythia8SD1_BDTG_Pythia8TrainedSD1"].training_sample = "Pythia8SD1"; // correspond to sSample
    mTMVAOutput["Pythia8SD1_BDTG_Pythia8TrainedSD1"].app_input_sample = "Pythia8SD1"; // correspond to sSample
    mTMVAOutput["Pythia8SD1_BDTG_Pythia8TrainedSD1"].app_output_file_name = "TMVApp_Pythia8SD1_BDTG_training_Pythia8SD1.root";

    mTMVAOutput["EPOSSD1_BDTG_EPOSSD1Trained"].method = "BDTG";
    mTMVAOutput["EPOSSD1_BDTG_EPOSSD1Trained"].training_sample = "EPOSSD1"; // correspond to sSample
    mTMVAOutput["EPOSSD1_BDTG_EPOSSD1Trained"].app_input_sample = "EPOSSD1"; // correspond to sSample
    mTMVAOutput["EPOSSD1_BDTG_EPOSSD1Trained"].app_output_file_name = "TMVApp_EPOSSD1_BDTG_training_EPOSSD1.root";


    mTMVAOutput["XiCutEPOSSD1_BDTG_XiCutEPOSSD1Trained"].method = "BDTG";
    mTMVAOutput["XiCutEPOSSD1_BDTG_XiCutEPOSSD1Trained"].training_sample = "XiCutEPOSSD1"; // correspond to sSample
    mTMVAOutput["XiCutEPOSSD1_BDTG_XiCutEPOSSD1Trained"].app_input_sample = "XiCutEPOSSD1"; // correspond to sSample
    mTMVAOutput["XiCutEPOSSD1_BDTG_XiCutEPOSSD1Trained"].app_output_file_name = "TMVApp_XiCutEPOSSD1_BDTG_training_XiCutEPOSSD1.root";

    mTMVAOutput["XiCutPythia8SD1_BDTG_XiCutPythia8SD1Trained"].method = "BDTG";
    mTMVAOutput["XiCutPythia8SD1_BDTG_XiCutPythia8SD1Trained"].training_sample = "XiCutPythia8SD1"; // correspond to sSample
    mTMVAOutput["XiCutPythia8SD1_BDTG_XiCutPythia8SD1Trained"].app_input_sample = "XiCutPythia8SD1"; // correspond to sSample
    mTMVAOutput["XiCutPythia8SD1_BDTG_XiCutPythia8SD1Trained"].app_output_file_name = "TMVApp_XiCutPythia8SD1_BDTG_training_XiCutPythia8SD1.root";


    mTMVAOutput["DataSD1_BDTG_Pythia8SD1Trained"].method = "BDTG";
    mTMVAOutput["DataSD1_BDTG_Pythia8SD1Trained"].training_sample = "Pythia8SD1"; // correspond to sSample
    mTMVAOutput["DataSD1_BDTG_Pythia8SD1Trained"].app_input_sample = "Data"; // correspond to sSample
    mTMVAOutput["DataSD1_BDTG_Pythia8SD1Trained"].app_output_file_name = "TMVApp_DataSD1_BDTG_Pythia8SD1Trained.root";

    mTMVAOutput["DataSD1_BDTG_EPOSSD1Trained"].method = "BDTG";
    mTMVAOutput["DataSD1_BDTG_EPOSSD1Trained"].training_sample = "EPOSSD1"; // correspond to sSample
    mTMVAOutput["DataSD1_BDTG_EPOSSD1Trained"].app_input_sample = "Data"; // correspond to sSample
    mTMVAOutput["DataSD1_BDTG_EPOSSD1Trained"].app_output_file_name = "TMVApp_DataSD1_BDTG_EPOSSD1Trained.root";

    mTMVAOutput["DataSD1_BDTG_XiCutEPOSSD1Trained"].method = "BDTG";
    mTMVAOutput["DataSD1_BDTG_XiCutEPOSSD1Trained"].training_sample = "XiCutEPOSSD1"; // correspond to sSample
    mTMVAOutput["DataSD1_BDTG_XiCutEPOSSD1Trained"].app_input_sample = "Data"; // correspond to sSample
    mTMVAOutput["DataSD1_BDTG_XiCutEPOSSD1Trained"].app_output_file_name = "TMVApp_DataSD1_BDTG_XiCutEPOSSD1Trained.root";
  
    mTMVAOutput["DataSD1_BDTG_XiCutPythia8SD1Trained"].method = "BDTG";
    mTMVAOutput["DataSD1_BDTG_XiCutPythia8SD1Trained"].training_sample = "XiCutPythia8SD1"; // correspond to sSample
    mTMVAOutput["DataSD1_BDTG_XiCutPythia8SD1Trained"].app_input_sample = "Data"; // correspond to sSample
    mTMVAOutput["DataSD1_BDTG_XiCutPythia8SD1Trained"].app_output_file_name = "TMVApp_DataSD1_BDTG_XiCutPythia8SD1Trained.root";



     ////////////////////// SD2////////////////////////////////////////////////////

    mTMVAOutput["Pythia8SD2_BDTG_Pythia8TrainedSD2"].method = "BDTG";
    mTMVAOutput["Pythia8SD2_BDTG_Pythia8TrainedSD2"].training_sample = "Pythia8SD2"; // correspond to sSample
    mTMVAOutput["Pythia8SD2_BDTG_Pythia8TrainedSD2"].app_input_sample = "Pythia8SD2"; // correspond to sSample
    mTMVAOutput["Pythia8SD2_BDTG_Pythia8TrainedSD2"].app_output_file_name = "TMVApp_Pythia8SD2_BDTG_training_Pythia8SD2.root";

    mTMVAOutput["EPOSSD2_BDTG_EPOSSD2Trained"].method = "BDTG";
    mTMVAOutput["EPOSSD2_BDTG_EPOSSD2Trained"].training_sample = "EPOSSD2"; // correspond to sSample
    mTMVAOutput["EPOSSD2_BDTG_EPOSSD2Trained"].app_input_sample = "EPOSSD2"; // correspond to sSample
    mTMVAOutput["EPOSSD2_BDTG_EPOSSD2Trained"].app_output_file_name = "TMVApp_EPOSSD2_BDTG_training_EPOSSD1.root";


    mTMVAOutput["XiCutEPOSSD2_BDTG_XiCutEPOSSD2Trained"].method = "BDTG";
    mTMVAOutput["XiCutEPOSSD2_BDTG_XiCutEPOSSD2Trained"].training_sample = "XiCutEPOSSD2"; // correspond to sSample
    mTMVAOutput["XiCutEPOSSD2_BDTG_XiCutEPOSSD2Trained"].app_input_sample = "XiCutEPOSSD2"; // correspond to sSample
    mTMVAOutput["XiCutEPOSSD2_BDTG_XiCutEPOSSD2Trained"].app_output_file_name = "TMVApp_XiCutEPOSSD2_BDTG_training_XiCutEPOSSD2.root";

    mTMVAOutput["XiCutPythia8SD2_BDTG_XiCutPythia8SD2Trained"].method = "BDTG";
    mTMVAOutput["XiCutPythia8SD2_BDTG_XiCutPythia8SD2Trained"].training_sample = "XiCutPythia8SD2"; // correspond to sSample
    mTMVAOutput["XiCutPythia8SD2_BDTG_XiCutPythia8SD2Trained"].app_input_sample = "XiCutPythia8SD2"; // correspond to sSample
    mTMVAOutput["XiCutPythia8SD2_BDTG_XiCutPythia8SD2Trained"].app_output_file_name = "TMVApp_XiCutPythia8SD2_BDTG_training_XiCutPythia8SD2.root";


    mTMVAOutput["DataSD2_BDTG_Pythia8SD2Trained"].method = "BDTG";
    mTMVAOutput["DataSD2_BDTG_Pythia8SD2Trained"].training_sample = "Pythia8SD2"; // correspond to sSample
    mTMVAOutput["DataSD2_BDTG_Pythia8SD2Trained"].app_input_sample = "Data"; // correspond to sSample
    mTMVAOutput["DataSD2_BDTG_Pythia8SD2Trained"].app_output_file_name = "TMVApp_DataSD2_BDTG_Pythia8SD2Trained.root";

    mTMVAOutput["DataSD2_BDTG_EPOSSD2Trained"].method = "BDTG";
    mTMVAOutput["DataSD2_BDTG_EPOSSD2Trained"].training_sample = "EPOSSD2"; // correspond to sSample
    mTMVAOutput["DataSD2_BDTG_EPOSSD2Trained"].app_input_sample = "Data"; // correspond to sSample
    mTMVAOutput["DataSD2_BDTG_EPOSSD2Trained"].app_output_file_name = "TMVApp_DataSD2_BDTG_EPOSSD2Trained.root";

    mTMVAOutput["DataSD2_BDTG_XiCutEPOSSD2Trained"].method = "BDTG";
    mTMVAOutput["DataSD2_BDTG_XiCutEPOSSD2Trained"].training_sample = "XiCutEPOSSD2"; // correspond to sSample
    mTMVAOutput["DataSD2_BDTG_XiCutEPOSSD2Trained"].app_input_sample = "Data"; // correspond to sSample
    mTMVAOutput["DataSD2_BDTG_XiCutEPOSSD2Trained"].app_output_file_name = "TMVApp_DataSD2_BDTG_XiCutEPOSSD2Trained.root";
  

    mTMVAOutput["DataSD2_BDTG_XiCutPythia8SD2Trained"].method = "BDTG";
    mTMVAOutput["DataSD2_BDTG_XiCutPythia8SD2Trained"].training_sample = "XiCutPythia8SD2"; // correspond to sSample
    mTMVAOutput["DataSD2_BDTG_XiCutPythia8SD2Trained"].app_input_sample = "Data"; // correspond to sSample
    mTMVAOutput["DataSD2_BDTG_XiCutPythia8SD2Trained"].app_output_file_name = "TMVApp_DataSD2_BDTG_XiCutPythia8SD1Trained.root";

      ////////////////////// REST////////////////////////////////////////////////////

    mTMVAOutput["Pythia8Rest_BDTG_Pythia8TrainedRest"].method = "BDTG";
    mTMVAOutput["Pythia8Rest_BDTG_Pythia8TrainedRest"].training_sample = "Pythia8Rest"; // correspond to sSample
    mTMVAOutput["Pythia8Rest_BDTG_Pythia8TrainedRest"].app_input_sample = "Pythia8Rest"; // correspond to sSample
    mTMVAOutput["Pythia8Rest_BDTG_Pythia8TrainedRest"].app_output_file_name = "TMVApp_Pythia8Rest_BDTG_training_Pythia8Rest.root";

    mTMVAOutput["EPOSRest_BDTG_EPOSRestTrained"].method = "BDTG";
    mTMVAOutput["EPOSRest_BDTG_EPOSRestTrained"].training_sample = "EPOSRest"; // correspond to sSample
    mTMVAOutput["EPOSRest_BDTG_EPOSRestTrained"].app_input_sample = "EPOSRest"; // correspond to sSample
    mTMVAOutput["EPOSRest_BDTG_EPOSRestTrained"].app_output_file_name = "TMVApp_EPOSRest_BDTG_training_EPOSRest.root";


    mTMVAOutput["XiCutEPOSRest_BDTG_XiCutEPOSRestTrained"].method = "BDTG";
    mTMVAOutput["XiCutEPOSRest_BDTG_XiCutEPOSRestTrained"].training_sample = "XiCutEPOSRest"; // correspond to sSample
    mTMVAOutput["XiCutEPOSRest_BDTG_XiCutEPOSRestTrained"].app_input_sample = "XiCutEPOSRest"; // correspond to sSample
    mTMVAOutput["XiCutEPOSRest_BDTG_XiCutEPOSRestTrained"].app_output_file_name = "TMVApp_XiCutEPOSRest_BDTG_training_XiCutEPOSRest.root";

    mTMVAOutput["XiCutPythia8Rest_BDTG_XiCutPythia8RestTrained"].method = "BDTG";
    mTMVAOutput["XiCutPythia8Rest_BDTG_XiCutPythia8RestTrained"].training_sample = "XiCutPythia8Rest"; // correspond to sSample
    mTMVAOutput["XiCutPythia8Rest_BDTG_XiCutPythia8RestTrained"].app_input_sample = "XiCutPythia8Rest"; // correspond to sSample
    mTMVAOutput["XiCutPythia8Rest_BDTG_XiCutPythia8RestTrained"].app_output_file_name = "TMVApp_XiCutPythia8Rest_BDTG_training_XiCutPythia8Rest.root";


    mTMVAOutput["DataRest_BDTG_Pythia8RestTrained"].method = "BDTG";
    mTMVAOutput["DataRest_BDTG_Pythia8RestTrained"].training_sample = "Pythia8Rest"; // correspond to sSample
    mTMVAOutput["DataRest_BDTG_Pythia8RestTrained"].app_input_sample = "Data"; // correspond to sSample
    mTMVAOutput["DataRest_BDTG_Pythia8RestTrained"].app_output_file_name = "TMVApp_DataRest_BDTG_Pythia8RestTrained.root";

    mTMVAOutput["DataRest_BDTG_EPOSRestTrained"].method = "BDTG";
    mTMVAOutput["DataRest_BDTG_EPOSRestTrained"].training_sample = "EPOSRest"; // correspond to sSample
    mTMVAOutput["DataRest_BDTG_EPOSRestTrained"].app_input_sample = "Data"; // correspond to sSample
    mTMVAOutput["DataRest_BDTG_EPOSRestTrained"].app_output_file_name = "TMVApp_DataRest_BDTG_EPOSRestTrained.root";

    mTMVAOutput["DataRest_BDTG_XiCutEPOSRestTrained"].method = "BDTG";
    mTMVAOutput["DataRest_BDTG_XiCutEPOSRestTrained"].training_sample = "XiCutEPOSRest"; // correspond to sSample
    mTMVAOutput["DataRest_BDTG_XiCutEPOSRestTrained"].app_input_sample = "Data"; // correspond to sSample
    mTMVAOutput["DataRest_BDTG_XiCutEPOSRestTrained"].app_output_file_name = "TMVApp_DataRest_BDTG_XiCutEPOSRestTrained.root";
  

    mTMVAOutput["DataRest_BDTG_XiCutPythia8RestTrained"].method = "BDTG";
    mTMVAOutput["DataRest_BDTG_XiCutPythia8RestTrained"].training_sample = "XiCutPythia8Rest"; // correspond to sSample
    mTMVAOutput["DataRest_BDTG_XiCutPythia8RestTrained"].app_input_sample = "Data"; // correspond to sSample
    mTMVAOutput["DataRest_BDTG_XiCutPythia8RestTrained"].app_output_file_name = "TMVApp_DataRest_BDTG_XiCutPythia8SD1Trained.root";





    //////////////////////////DD////////////////////////////////////////////////
    mTMVAOutput["Pythia8_BDTG_Pythia8Trained"].method = "BDTG";
    mTMVAOutput["Pythia8_BDTG_Pythia8Trained"].training_sample = "Pythia8"; // correspond to sSample
    mTMVAOutput["Pythia8_BDTG_Pythia8Trained"].app_input_sample = "Pythia8"; // correspond to sSample
    mTMVAOutput["Pythia8_BDTG_Pythia8Trained"].app_output_file_name = "TMVApp_Pythia8_BDTG_training_Pythia8.root";
   
    mTMVAOutput["EPOS_BDTG_Pythia8Trained"].method = "BDTG";
    mTMVAOutput["EPOS_BDTG_Pythia8Trained"].training_sample = "Pythia8"; // correspond to sSample
    mTMVAOutput["EPOS_BDTG_Pythia8Trained"].app_input_sample = "EPOS"; // correspond to sSample
    mTMVAOutput["EPOS_BDTG_Pythia8Trained"].app_output_file_name = "TMVApp_EPOS_BDTG_Pythia8Trained.root";

    mTMVAOutput["XiCutPythia8_BDTG_Pythia8Trained"].method = "BDTG";
    mTMVAOutput["XiCutPythia8_BDTG_Pythia8Trained"].training_sample = "Pythia8"; // correspond to sSample
    mTMVAOutput["XiCutPythia8_BDTG_Pythia8Trained"].app_input_sample = "XiCutPythia8"; // correspond to sSample
    mTMVAOutput["XiCutPythia8_BDTG_Pythia8Trained"].app_output_file_name = "TMVApp_XiCutPythia8Trained_BDTG_Pythia8Trained.root";

    mTMVAOutput["EPOS_BDTG_EPOSTrained"].method = "BDTG";
    mTMVAOutput["EPOS_BDTG_EPOSTrained"].training_sample = "EPOS"; // correspond to sSample
    mTMVAOutput["EPOS_BDTG_EPOSTrained"].app_input_sample = "EPOS"; // correspond to sSample
    mTMVAOutput["EPOS_BDTG_EPOSTrained"].app_output_file_name = "TMVApp_EPOS_BDTG_training_EPOS.root";

    mTMVAOutput["XiCutEPOS_BDTG_EPOSTrained"].method = "BDTG";
    mTMVAOutput["XiCutEPOS_BDTG_EPOSTrained"].training_sample = "EPOS"; // correspond to sSample
    mTMVAOutput["XiCutEPOS_BDTG_EPOSTrained"].app_input_sample = "XiCutEPOS"; // correspond to sSample
    mTMVAOutput["XiCutEPOS_BDTG_EPOSTrained"].app_output_file_name = "TMVApp_XiCutEPOS_BDTG_training_EPOS.root";



    mTMVAOutput["Pythia8_BDTG_EPOSTrained"].method = "BDTG";
    mTMVAOutput["Pythia8_BDTG_EPOSTrained"].training_sample = "EPOS"; // correspond to sSample
    mTMVAOutput["Pythia8_BDTG_EPOSTrained"].app_input_sample = "Pythia8"; // correspond to sSample
    mTMVAOutput["Pythia8_BDTG_EPOSTrained"].app_output_file_name = "TMVApp_Pythia8_BDTG_training_EPOS.root";
    
    mTMVAOutput["XiCutPythia8_BDTG_EPOSTrained"].method = "BDTG";
    mTMVAOutput["XiCutPythia8_BDTG_EPOSTrained"].training_sample = "EPOS"; // correspond to sSample
    mTMVAOutput["XiCutPythia8_BDTG_EPOSTrained"].app_input_sample = "XiCutPythia8"; // correspond to sSample
    mTMVAOutput["XiCutPythia8_BDTG_EPOSTrained"].app_output_file_name = "TMVApp_XiCutPythia8_BDTG_training_EPOS.root";

   /////////////////////XiCutEPOS//////////////////////////////////////////////////////////////////////////
    mTMVAOutput["XiCutEPOS_BDTG_XiCutEPOSTrained"].method = "BDTG";
    mTMVAOutput["XiCutEPOS_BDTG_XiCutEPOSTrained"].training_sample = "XiCutEPOS"; // correspond to sSample
    mTMVAOutput["XiCutEPOS_BDTG_XiCutEPOSTrained"].app_input_sample = "XiCutEPOS"; // correspond to sSample
    mTMVAOutput["XiCutEPOS_BDTG_XiCutEPOSTrained"].app_output_file_name = "TMVApp_XiCutEPOS_BDTG_training_XiCutEPOS.root";

    mTMVAOutput["EPOS_BDTG_XiCutEPOSTrained"].method = "BDTG";
    mTMVAOutput["EPOS_BDTG_XiCutEPOSTrained"].training_sample = "XiCutEPOS"; // correspond to sSample
    mTMVAOutput["EPOS_BDTG_XiCutEPOSTrained"].app_input_sample = "EPOS"; // correspond to sSample
    mTMVAOutput["EPOS_BDTG_XiCutEPOSTrained"].app_output_file_name = "TMVApp_EPOS_BDTG_training_XiCutEPOS.root";

    mTMVAOutput["Pythia8_BDTG_XiCutEPOSTrained"].method = "BDTG";
    mTMVAOutput["Pythia8_BDTG_XiCutEPOSTrained"].training_sample = "XiCutEPOS"; // correspond to sSample
    mTMVAOutput["Pythia8_BDTG_XiCutEPOSTrained"].app_input_sample = "Pythia8"; // correspond to sSample
    mTMVAOutput["Pythia8_BDTG_XiCutEPOSTrained"].app_output_file_name = "TMVApp_Pythia8_BDTG_training_XiCutEPOS.root";
    
    mTMVAOutput["XiCutPythia8_BDTG_XiCutEPOSTrained"].method = "BDTG";
    mTMVAOutput["XiCutPythia8_BDTG_XiCutEPOSTrained"].training_sample = "XiCutEPOS"; // correspond to sSample
    mTMVAOutput["XiCutPythia8_BDTG_XiCutEPOSTrained"].app_input_sample = "XiCutPythia8"; // correspond to sSample
    mTMVAOutput["XiCutPythia8_BDTG_XiCutEPOSTrained"].app_output_file_name = "TMVApp_XiCutPythia8_BDTG_training_XiCutEPOS.root";




   
///////////////////////////////////////////////////////////////////////////////////////////////////////
    mTMVAOutput["EPOS_BDTG_XiCutPythia8Trained"].method = "BDTG";
    mTMVAOutput["EPOS_BDTG_XiCutPythia8Trained"].training_sample = "XiCutPythia8"; // correspond to sSample
    mTMVAOutput["EPOS_BDTG_XiCutPythia8Trained"].app_input_sample = "EPOS"; // correspond to sSample
    mTMVAOutput["EPOS_BDTG_XiCutPythia8Trained"].app_output_file_name = "TMVApp_EPOS_BDTG_training_XiCutPythia8.root";

    mTMVAOutput["Pythia8_BDTG_XiCutPythia8Trained"].method = "BDTG";
    mTMVAOutput["Pythia8_BDTG_XiCutPythia8Trained"].training_sample = "XiCutPythia8"; // correspond to sSample
    mTMVAOutput["Pythia8_BDTG_XiCutPythia8Trained"].app_input_sample = "Pythia8"; // correspond to sSample
    mTMVAOutput["Pythia8_BDTG_XiCutPythia8Trained"].app_output_file_name = "TMVApp_Pythia8_BDTG_training_XiCutPythia8.root";
    
    mTMVAOutput["XiCutPythia8_BDTG_XiCutPythia8Trained"].method = "BDTG";
    mTMVAOutput["XiCutPythia8_BDTG_XiCutPythia8Trained"].training_sample = "XiCutPythia8"; // correspond to sSample
    mTMVAOutput["XiCutPythia8_BDTG_XiCutPythia8Trained"].app_input_sample = "XiCutPythia8"; // correspond to sSample
    mTMVAOutput["XiCutPythia8_BDTG_XiCutPythia8Trained"].app_output_file_name = "TMVApp_XiCutPythia8_BDTG_training_XiCutPythia8.root";





    //////////////////////////////////////////////////////////////////////////
    // Data
    mTMVAOutput["Data_BDTG_Pythia8Trained"].method = "BDTG";
    mTMVAOutput["Data_BDTG_Pythia8Trained"].training_sample = "Pythia8"; // correspond to sSample
    mTMVAOutput["Data_BDTG_Pythia8Trained"].app_input_sample = "Data"; // correspond to sSample
    mTMVAOutput["Data_BDTG_Pythia8Trained"].app_output_file_name = "TMVApp_Data_BDTG_Pythia8Trained.root";

    mTMVAOutput["Data_BDTG_EPOSTrained"].method = "BDTG";
    mTMVAOutput["Data_BDTG_EPOSTrained"].training_sample = "EPOS"; // correspond to sSample
    mTMVAOutput["Data_BDTG_EPOSTrained"].app_input_sample = "Data"; // correspond to sSample
    mTMVAOutput["Data_BDTG_EPOSTrained"].app_output_file_name = "TMVApp_Data_BDTG_EPOSTrained.root";

    mTMVAOutput["Data_BDTG_XiCutEPOSTrained"].method = "BDTG";
    mTMVAOutput["Data_BDTG_XiCutEPOSTrained"].training_sample = "XiCutEPOS"; // correspond to sSample
    mTMVAOutput["Data_BDTG_XiCutEPOSTrained"].app_input_sample = "Data"; // correspond to sSample
    mTMVAOutput["Data_BDTG_XiCutEPOSTrained"].app_output_file_name = "TMVApp_Data_BDTG_XiCutEPOSTrained.root";
  


    mTMVAOutput["Data_BDTG_XiCutPythia8Trained"].method = "BDTG";
    mTMVAOutput["Data_BDTG_XiCutPythia8Trained"].training_sample = "XiCutPythia8"; // correspond to sSample
    mTMVAOutput["Data_BDTG_XiCutPythia8Trained"].app_input_sample = "Data"; // correspond to sSample
    mTMVAOutput["Data_BDTG_XiCutPythia8Trained"].app_output_file_name = "TMVApp_Data_BDTG_XiCutPythia8Trained.root";




    return mTMVAOutput;
  }
  //////////////////////////////////////////////////////////////////////////


} // end of namespace

#endif