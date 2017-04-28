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
    
    TString legend_name;

    TFile* file;
    TString tree_name;

    std::vector<TString> vMethode;
    TString weight_name;
    TString signal;

    TString app_output_file_name;
  };

  //////////////////////////////////////////////////////////////////////////
  std::map<TString, sSample> read_data_mc_files() {
    TH1F * htmp;

    std::map<TString, sSample> mSample;

    mSample["Pythia8"].isData = false;
    mSample["Pythia8"].procesesID_pythia8 = true;
    mSample["Pythia8"].xs = 71390.000000; // ub
    mSample["Pythia8"].file = TFile::Open("data/trackanddiffractive_sigDD_pythia8.root");
    mSample["Pythia8"].legend_name = "Pythia8 MBR";
    mSample["Pythia8"].tree_name = "MinBias_TuneMBR_13TeV-pythia8_MagnetOff_CASTORmeasured_newNoise";
    mSample["Pythia8"].weight_name = "Pythia8_DD_weights";
    mSample["Pythia8"].signal = "DD";
    mSample["Pythia8"].vMethode.push_back("BDTG");
    mSample["Pythia8"].vMethode.push_back("Fisher");
    mSample["Pythia8"].vMethode.push_back("Likelihood");
    mSample["Pythia8"].vMethode.push_back("MLP");
    mSample["Pythia8"].app_output_file_name = "TMVApp_Pythia8.root";
    htmp = (TH1F*)mSample["Pythia8"].file->Get(mSample["Pythia8"].tree_name + "/hNentries");
    mSample["Pythia8"].lumi = htmp->GetBinContent(1)/mSample["Pythia8"].xs;


   //  mSample["XiCutPythia8"].isData = false;
   //  mSample["XiCutPythia8"].procesesID_pythia8 = false;
   //  mSample["XiCutPythia8"].lumi = 0;
   //  mSample["XiCutPythia8"].xs = 79948.200000; // ub
   //  mSample["XiCutPythia8"].file = TFile::Open("data/trackanddiffractive_sigDD_XiCutpythia8.root");
   //  mSample["XiCutPythia8"].legend_name = "XiCutPythia8";
   //  mSample["XiCutPythia8"].tree_name = "MinBias_TuneMBR_13TeV-pythia8_MagnetOff_CASTORmeasured_newNoise";
   //  mSample["XiCutPythia8"].weight_name = "Pythia8_XiEvtSel_DD_weights";
   //  mSample["XiCutPythia8"].signal = "DD";
   //  mSample["XiCutPythia8"].vMethode.push_back("BDTG");
   //  mSample["XiCutPythia8"].app_output_file_name = "TMVApp_XiCutPythia8.root";
   //  htmp = (TH1F*)mSample["XiCutPythia8"].file->Get(mSample["XiCutPythia8"].tree_name + "/hNentries");
   //  mSample["XiCutPythia8"].lumi = htmp->GetBinContent(1)/mSample["XiCutPythia8"].xs;

   //  mSample["EPOS"].isData = false;
   //  mSample["EPOS"].procesesID_pythia8 = true;
   //  mSample["EPOS"].lumi = 0;
   //  mSample["EPOS"].xs = 79948.200000; // ub
   //  mSample["EPOS"].file = TFile::Open("data/trackanddiffractive_sigDD_epos.root");
   //  mSample["EPOS"].legend_name = "EPOS";
   //  mSample["EPOS"].tree_name = "MinBias_EPOS_13TeV_MagnetOff_CASTORmeasured_newNoise";
   //  mSample["EPOS"].weight_name = "EPOS_DD_weights";
   //  mSample["EPOS"].signal = "DD";
   //  mSample["EPOS"].vMethode.push_back("BDTG");
   //  mSample["EPOS"].app_output_file_name = "TMVApp_EPOS.root";
   //  htmp = (TH1F*)mSample["EPOS"].file->Get(mSample["EPOS"].tree_name + "/hNentries");
   //  mSample["EPOS"].lumi = htmp->GetBinContent(1)/mSample["EPOS"].xs;

   //  mSample["XiCutEPOS"].isData = false;
   //  mSample["XiCutEPOS"].procesesID_pythia8 = false;
   //  mSample["XiCutEPOS"].lumi = 0;
   //  mSample["XiCutEPOS"].xs = 79948.200000; // ub
   //  mSample["XiCutEPOS"].file = TFile::Open("data/trackanddiffractive_sigDD_XiCutepos.root");
   //  mSample["XiCutEPOS"].legend_name = "EPOS";
   //  mSample["XiCutEPOS"].tree_name = "MinBias_EPOS_13TeV_MagnetOff_CASTORmeasured_newNoise";
   //  mSample["XiCutEPOS"].weight_name = "XiCutEPOS_DD_weights";
   //  mSample["XiCutEPOS"].signal = "DD";
   //  mSample["XiCutEPOS"].vMethode.push_back("BDTG");
   //  mSample["XiCutEPOS"].app_output_file_name = "TMVApp_XiCutEPOS.root";
   //  htmp = (TH1F*)mSample["XiCutEPOS"].file->Get(mSample["XiCutEPOS"].tree_name + "/hNentries");
   //  mSample["XiCutEPOS"].lumi = htmp->GetBinContent(1)/mSample["XiCutEPOS"].xs;
    
   //  mSample["Data"].isData = true;
   //  // effective lumi; already divided by prescale
   //  mSample["Data"].lumi = 0.35; // 1/ub
   //  mSample["Data"].xs = 0;
   //  mSample["Data"].legend_name = "DATA";
   //  mSample["Data"].file = TFile::Open("data/trackanddiffractive_sigDD_data.root");
   //  mSample["Data"].tree_name = "data_ZeroBias1_CASTOR";
   //  mSample["Data"].weight_name = "";
   //  mSample["Data"].signal = "";
   //  mSample["Data"].app_output_file_name = "TMVApp_Data.root";
    
   //  // mSample["Data"].isData = true;
   //  // // effective lumi; already divided by prescale
   //  // mSample["Data"].lumi = 0.35; // 1/ub
   //  // mSample["Data"].xs = 0;
   //  // mSample["Data"].legend_name = "DATA";
   //  // mSample["Data"].file = TFile::Open("data/trackanddiffractive_sigDD_DataHF.root");
   //  // mSample["Data"].tree_name = "data_ZeroBias1_CASTOR";
   //  // mSample["Data"].weight_name = "";
   //  // mSample["Data"].signal = "";
   //  // mSample["Data"].app_output_file_name = "TMVApp_DataHF.root";
    

   //  // effective lumi; already divided by prescale
   //  // mSample["Data"].isData = true;
   //  // mSample["Data"].lumi = 3.73; // 1/ub
   //  // mSample["Data"].xs = 0;
   //  // mSample["Data"].legend_name = "DATA";
   //  // mSample["Data"].file = TFile::Open("data/trackanddiffractive_sigDD_data_247934.root");
   //  // mSample["Data"].tree_name = "data_ZeroBias_27Jan2016_LHCf";
   //  // mSample["Data"].weight_name = "";
   //  // mSample["Data"].signal = "";
   //  // mSample["Data"].app_output_file_name = "TMVApp_Data_247934.root";
   



   // // // //////////////////////////SD1//////////////////////////////////

   //  mSample["Pythia8SD1"].isData = false;
   //  mSample["Pythia8SD1"].procesesID_pythia8 = true;
   //  mSample["Pythia8SD1"].xs = 71390.000000; // ub
   //  mSample["Pythia8SD1"].file = TFile::Open("data/trackanddiffractive_sigSD1_pythia8.root");
   //  mSample["Pythia8SD1"].legend_name = "Pythia8 MBR";
   //  mSample["Pythia8SD1"].tree_name = "MinBias_TuneMBR_13TeV-pythia8_MagnetOff_CASTORmeasured_newNoise";
   //  mSample["Pythia8SD1"].weight_name = "Pythia8_SD1_weights";
   //  mSample["Pythia8SD1"].signal = "SD1";
   //  mSample["Pythia8SD1"].vMethode.push_back("BDTG");
   //  mSample["Pythia8SD1"].app_output_file_name = "TMVApp_Pythia8SD1.root";
   //  htmp = (TH1F*)mSample["Pythia8SD1"].file->Get(mSample["Pythia8SD1"].tree_name + "/hNentries");
   //  mSample["Pythia8SD1"].lumi = htmp->GetBinContent(1)/mSample["Pythia8SD1"].xs;


   //  mSample["XiCutPythia8SD1"].isData = false;
   //  mSample["XiCutPythia8SD1"].procesesID_pythia8 = false;
   //  mSample["XiCutPythia8SD1"].lumi = 0;
   //  mSample["XiCutPythia8SD1"].xs = 79948.200000; // ub
   //  mSample["XiCutPythia8SD1"].file = TFile::Open("data/trackanddiffractive_sigSD1_XiCutpythia8.root");
   //  mSample["XiCutPythia8SD1"].legend_name = "Pythia8 MBR";
   //  mSample["XiCutPythia8SD1"].tree_name = "MinBias_TuneMBR_13TeV-pythia8_MagnetOff_CASTORmeasured_newNoise";
   //  mSample["XiCutPythia8SD1"].weight_name = "Pythia8_XiEvtSel_SD1_weights";
   //  mSample["XiCutPythia8SD1"].signal = "SD1";
   //  mSample["XiCutPythia8SD1"].vMethode.push_back("BDTG");
   //  mSample["XiCutPythia8SD1"].app_output_file_name = "TMVApp_XiCutPythia8SD1.root";
   //  htmp = (TH1F*)mSample["XiCutPythia8SD1"].file->Get(mSample["XiCutPythia8SD1"].tree_name + "/hNentries");
   //  mSample["XiCutPythia8SD1"].lumi = htmp->GetBinContent(1)/mSample["XiCutPythia8SD1"].xs;

   //  mSample["EPOSSD1"].isData = false;
   //  mSample["EPOSSD1"].procesesID_pythia8 = true;
   //  mSample["EPOSSD1"].lumi = 0;
   //  mSample["EPOSSD1"].xs = 79948.200000; // ub
   //  mSample["EPOSSD1"].file = TFile::Open("data/trackanddiffractive_sigSD1_epos.root");
   //  mSample["EPOSSD1"].legend_name = "EPOS";
   //  mSample["EPOSSD1"].tree_name = "MinBias_EPOS_13TeV_MagnetOff_CASTORmeasured_newNoise";
   //  mSample["EPOSSD1"].weight_name = "EPOS_SD1_weights";
   //  mSample["EPOSSD1"].signal = "SD1";
   //  mSample["EPOSSD1"].vMethode.push_back("BDTG");
   //  mSample["EPOSSD1"].app_output_file_name = "TMVApp_EPOSSD1.root";
   //  htmp = (TH1F*)mSample["EPOSSD1"].file->Get(mSample["EPOSSD1"].tree_name + "/hNentries");
   //  mSample["EPOSSD1"].lumi = htmp->GetBinContent(1)/mSample["EPOSSD1"].xs;

   //  mSample["XiCutEPOSSD1"].isData = false;
   //  mSample["XiCutEPOSSD1"].procesesID_pythia8 = false;
   //  mSample["XiCutEPOSSD1"].lumi = 0;
   //  mSample["XiCutEPOSSD1"].xs = 79948.200000; // ub
   //  mSample["XiCutEPOSSD1"].file = TFile::Open("data/trackanddiffractive_sigSD1_XiCutepos.root");
   //  mSample["XiCutEPOSSD1"].legend_name = "EPOS";
   //  mSample["XiCutEPOSSD1"].tree_name = "MinBias_EPOS_13TeV_MagnetOff_CASTORmeasured_newNoise";
   //  mSample["XiCutEPOSSD1"].weight_name = "XiCutEPOS_SD1_weights";
   //  mSample["XiCutEPOSSD1"].signal = "SD1";
   //  mSample["XiCutEPOSSD1"].vMethode.push_back("BDTG");
   //  mSample["XiCutEPOSSD1"].app_output_file_name = "TMVApp_XiCutEPOSSD1.root";
   //  htmp = (TH1F*)mSample["XiCutEPOSSD1"].file->Get(mSample["XiCutEPOSSD1"].tree_name + "/hNentries");
   //  mSample["XiCutEPOSSD1"].lumi = htmp->GetBinContent(1)/mSample["XiCutEPOSSD1"].xs;
    
   
   // //  //////////////////////////SD2//////////////////////////////////

   //  mSample["Pythia8SD2"].isData = false;
   //  mSample["Pythia8SD2"].procesesID_pythia8 = true;
   //  mSample["Pythia8SD2"].xs = 71390.000000; // ub
   //  mSample["Pythia8SD2"].file = TFile::Open("data/trackanddiffractive_sigSD2_pythia8.root");
   //  mSample["Pythia8SD2"].legend_name = "Pythia8 MBR";
   //  mSample["Pythia8SD2"].tree_name = "MinBias_TuneMBR_13TeV-pythia8_MagnetOff_CASTORmeasured_newNoise";
   //  mSample["Pythia8SD2"].weight_name = "Pythia8_SD2_weights";
   //  mSample["Pythia8SD2"].signal = "SD2";
   //  mSample["Pythia8SD2"].vMethode.push_back("BDTG");
   //  mSample["Pythia8SD2"].app_output_file_name = "TMVApp_Pythia8SD2.root";
   //  htmp = (TH1F*)mSample["Pythia8SD2"].file->Get(mSample["Pythia8SD2"].tree_name + "/hNentries");
   //  mSample["Pythia8SD2"].lumi = htmp->GetBinContent(1)/mSample["Pythia8SD2"].xs;


   //  mSample["XiCutPythia8SD2"].isData = false;
   //  mSample["XiCutPythia8SD2"].procesesID_pythia8 = false;
   //  mSample["XiCutPythia8SD2"].lumi = 0;
   //  mSample["XiCutPythia8SD2"].xs = 79948.200000; // ub
   //  mSample["XiCutPythia8SD2"].file = TFile::Open("data/trackanddiffractive_sigSD2_XiCutpythia8.root");
   //  mSample["XiCutPythia8SD2"].legend_name = "Pythia8 MBR";
   //  mSample["XiCutPythia8SD2"].tree_name = "MinBias_TuneMBR_13TeV-pythia8_MagnetOff_CASTORmeasured_newNoise";
   //  mSample["XiCutPythia8SD2"].weight_name = "Pythia8_XiEvtSel_SD2_weights";
   //  mSample["XiCutPythia8SD2"].signal = "SD2";
   //  mSample["XiCutPythia8SD2"].vMethode.push_back("BDTG");
   //  mSample["XiCutPythia8SD2"].app_output_file_name = "TMVApp_XiCutPythia8SD2.root";
   //  htmp = (TH1F*)mSample["XiCutPythia8SD2"].file->Get(mSample["XiCutPythia8SD2"].tree_name + "/hNentries");
   //  mSample["XiCutPythia8SD2"].lumi = htmp->GetBinContent(1)/mSample["XiCutPythia8SD2"].xs;

   //  mSample["EPOSSD2"].isData = false;
   //  mSample["EPOSSD2"].procesesID_pythia8 = true;
   //  mSample["EPOSSD2"].lumi = 0;
   //  mSample["EPOSSD2"].xs = 79948.200000; // ub
   //  mSample["EPOSSD2"].file = TFile::Open("data/trackanddiffractive_sigSD2_epos.root");
   //  mSample["EPOSSD2"].legend_name = "EPOS";
   //  mSample["EPOSSD2"].tree_name = "MinBias_EPOS_13TeV_MagnetOff_CASTORmeasured_newNoise";
   //  mSample["EPOSSD2"].weight_name = "EPOS_SD2_weights";
   //  mSample["EPOSSD2"].signal = "SD2";
   //  mSample["EPOSSD2"].vMethode.push_back("BDTG");
   //  mSample["EPOSSD2"].app_output_file_name = "TMVApp_EPOSSD2.root";
   //  htmp = (TH1F*)mSample["EPOSSD2"].file->Get(mSample["EPOSSD2"].tree_name + "/hNentries");
   //  mSample["EPOSSD2"].lumi = htmp->GetBinContent(1)/mSample["EPOSSD2"].xs;

   //  mSample["XiCutEPOSSD2"].isData = false;
   //  mSample["XiCutEPOSSD2"].procesesID_pythia8 = false;
   //  mSample["XiCutEPOSSD2"].lumi = 0;
   //  mSample["XiCutEPOSSD2"].xs = 79948.200000; // ub
   //  mSample["XiCutEPOSSD2"].file = TFile::Open("data/trackanddiffractive_sigSD2_XiCutepos.root");
   //  mSample["XiCutEPOSSD2"].legend_name = "EPOS";
   //  mSample["XiCutEPOSSD2"].tree_name = "MinBias_EPOS_13TeV_MagnetOff_CASTORmeasured_newNoise";
   //  mSample["XiCutEPOSSD2"].weight_name = "XiCutEPOS_SD2_weights";
   //  mSample["XiCutEPOSSD2"].signal = "SD2";
   //  mSample["XiCutEPOSSD2"].vMethode.push_back("BDTG");
   //  mSample["XiCutEPOSSD2"].app_output_file_name = "TMVApp_XiCutEPOSSD2.root";
   //  htmp = (TH1F*)mSample["XiCutEPOSSD2"].file->Get(mSample["XiCutEPOSSD2"].tree_name + "/hNentries");
   //  mSample["XiCutEPOSSD2"].lumi = htmp->GetBinContent(1)/mSample["XiCutEPOSSD2"].xs;
    
  
   // //  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

   // //   //////////////////////////REST//////////////////////////////////

   //  mSample["Pythia8Rest"].isData = false;
   //  mSample["Pythia8Rest"].procesesID_pythia8 = true;
   //  mSample["Pythia8Rest"].xs = 71390.000000; // ub
   //  mSample["Pythia8Rest"].file = TFile::Open("data/trackanddiffractive_sigRest_pythia8.root");
   //  mSample["Pythia8Rest"].legend_name = "Pythia8 MBR";
   //  mSample["Pythia8Rest"].tree_name = "MinBias_TuneMBR_13TeV-pythia8_MagnetOff_CASTORmeasured_newNoise";
   //  mSample["Pythia8Rest"].weight_name = "Pythia8_Rest_weights";
   //  mSample["Pythia8Rest"].signal = "Rest";
   //  mSample["Pythia8Rest"].vMethode.push_back("BDTG");
   //  mSample["Pythia8Rest"].app_output_file_name = "TMVApp_Pythia8Rest.root";
   //  htmp = (TH1F*)mSample["Pythia8Rest"].file->Get(mSample["Pythia8Rest"].tree_name + "/hNentries");
   //  mSample["Pythia8Rest"].lumi = htmp->GetBinContent(1)/mSample["Pythia8Rest"].xs;


   //  mSample["XiCutPythia8Rest"].isData = false;
   //  mSample["XiCutPythia8Rest"].procesesID_pythia8 = false;
   //  mSample["XiCutPythia8Rest"].lumi = 0;
   //  mSample["XiCutPythia8Rest"].xs = 79948.200000; // ub
   //  mSample["XiCutPythia8Rest"].file = TFile::Open("data/trackanddiffractive_sigRest_XiCutpythia8.root");
   //  mSample["XiCutPythia8Rest"].legend_name = "Pythia8 MBR";
   //  mSample["XiCutPythia8Rest"].tree_name = "MinBias_TuneMBR_13TeV-pythia8_MagnetOff_CASTORmeasured_newNoise";
   //  mSample["XiCutPythia8Rest"].weight_name = "Pythia8_XiEvtSel_Rest_weights";
   //  mSample["XiCutPythia8Rest"].signal = "Rest";
   //  mSample["XiCutPythia8Rest"].vMethode.push_back("BDTG");
   //  mSample["XiCutPythia8Rest"].app_output_file_name = "TMVApp_XiCutPythia8Rest.root";
   //  htmp = (TH1F*)mSample["XiCutPythia8Rest"].file->Get(mSample["XiCutPythia8Rest"].tree_name + "/hNentries");
   //  mSample["XiCutPythia8Rest"].lumi = htmp->GetBinContent(1)/mSample["XiCutPythia8Rest"].xs;

   //  mSample["EPOSRest"].isData = false;
   //  mSample["EPOSRest"].procesesID_pythia8 = true;
   //  mSample["EPOSRest"].lumi = 0;
   //  mSample["EPOSRest"].xs = 79948.200000; // ub
   //  mSample["EPOSRest"].file = TFile::Open("data/trackanddiffractive_sigRest_epos.root");
   //  mSample["EPOSRest"].legend_name = "EPOS";
   //  mSample["EPOSRest"].tree_name = "MinBias_EPOS_13TeV_MagnetOff_CASTORmeasured_newNoise";
   //  mSample["EPOSRest"].weight_name = "EPOS_Rest_weights";
   //  mSample["EPOSRest"].signal = "Rest";
   //  mSample["EPOSRest"].vMethode.push_back("BDTG");
   //  mSample["EPOSRest"].app_output_file_name = "TMVApp_EPOSRest.root";
   //  htmp = (TH1F*)mSample["EPOSRest"].file->Get(mSample["EPOSRest"].tree_name + "/hNentries");
   //  mSample["EPOSRest"].lumi = htmp->GetBinContent(1)/mSample["EPOSRest"].xs;

   //  mSample["XiCutEPOSRest"].isData = false;
   //  mSample["XiCutEPOSRest"].procesesID_pythia8 = false;
   //  mSample["XiCutEPOSRest"].lumi = 0;
   //  mSample["XiCutEPOSRest"].xs = 79948.200000; // ub
   //  mSample["XiCutEPOSRest"].file = TFile::Open("data/trackanddiffractive_sigRest_XiCutepos.root");
   //  mSample["XiCutEPOSRest"].legend_name = "EPOS";
   //  mSample["XiCutEPOSRest"].tree_name = "MinBias_EPOS_13TeV_MagnetOff_CASTORmeasured_newNoise";
   //  mSample["XiCutEPOSRest"].weight_name = "XiCutEPOS_Rest_weights";
   //  mSample["XiCutEPOSRest"].signal = "Rest";
   //  mSample["XiCutEPOSRest"].vMethode.push_back("BDTG");
   //  mSample["XiCutEPOSRest"].app_output_file_name = "TMVApp_XiCutEPOSRest.root";
   //  htmp = (TH1F*)mSample["XiCutEPOSRest"].file->Get(mSample["XiCutEPOSRest"].tree_name + "/hNentries");
   //  mSample["XiCutEPOSRest"].lumi = htmp->GetBinContent(1)/mSample["XiCutEPOSRest"].xs;
    
    return mSample;
    
  }
  //////////////////////////////////////////////////////////////////////////

} // end of namespace

#endif