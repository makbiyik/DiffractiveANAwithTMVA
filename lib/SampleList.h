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
    double lumi;
    double xs; // in nb

    TFile* file;
    TString tree_name;
  };

  //////////////////////////////////////////////////////////////////////////
  std::map<TString, sSample> read_data_mc_files() {
    TH1F * htmp;

    sSample sPythia8;
    sPythia8.isData = false;
    sPythia8.xs = 71390.000000; // nb
    sPythia8.file = TFile::Open("data/trackanddiffractive_sigDD_pythia8.root");
    sPythia8.tree_name = "MinBias_TuneMBR_13TeV-pythia8_MagnetOff_CASTORmeasured_newNoise";
    htmp = (TH1F*)sPythia8.file->Get(sPythia8.tree_name + "/hNentries");
    sPythia8.lumi = htmp->GetBinContent(1)/sPythia8.xs;


    sSample sEPOS;
    sEPOS.isData = false;
    sEPOS.lumi = 0;
    sEPOS.xs = 79948.200000; // nb
    sEPOS.file = TFile::Open("data/trackanddiffractive_sigDD_epos.root");
    sEPOS.tree_name = "MinBias_EPOS_13TeV_MagnetOff_CASTORmeasured_newNoise";
    htmp = (TH1F*)sEPOS.file->Get(sEPOS.tree_name + "/hNentries");
    sEPOS.lumi = htmp->GetBinContent(1)/sEPOS.xs;


    sSample sXiEventSelectioncut;
    sXiEventSelectioncut.isData = false;
    sXiEventSelectioncut.lumi = 0;
    sXiEventSelectioncut.xs = 79948.200000; // nb
    sXiEventSelectioncut.file = TFile::Open("data/trackanddiffractive_sigDD_pythia8.root");
    sXiEventSelectioncut.tree_name = "MinBias_TuneMBR_13TeV-pythia8_MagnetOff_CASTORmeasured_newNoise";
    htmp = (TH1F*)sXiEventSelectioncut.file->Get(sXiEventSelectioncut.tree_name + "/hNentries");
    sXiEventSelectioncut.lumi = htmp->GetBinContent(1)/sXiEventSelectioncut.xs;



    sSample sData;
    sData.isData = true;
    sData.lumi = 0.34; // 1/nb
    sData.xs = 0;
    sData.file = TFile::Open("data/trackanddiffractive_sigDD_data.root");
    sData.tree_name = "data_ZeroBias1_CASTOR";

    std::map<TString, sSample> mSample;

    mSample["Pythia8"] =  sPythia8;
    mSample["Pythia8XiEventselectioncut"] =  sXiEventSelectioncut;

    mSample["EPOS"] = sEPOS;
    mSample["Data"] = sData;

    return mSample;
  }

} // end of namespace

#endif