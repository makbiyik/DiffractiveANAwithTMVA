#ifndef SYSTERRHELPER_H
#define SYSTERRHELPER_H

#include <iostream>
#include <cmath>
#include <cstdio>
#include <algorithm>
#include <map>

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TApplication.h"
#include "TPad.h"
#include "TLine.h"
#include "TString.h"
#include "TLegend.h"
#include "TMatrix.h"
#include "TMatrixDEigen.h"
#include "TDecompSVD.h"
#include "TStyle.h"
#include "TRandom3.h"
#include "TGraphAsymmErrors.h"
#include "TVectorD.h"
#include "TLatex.h"
#include "TF1.h"
#include "TEfficiency.h"


//////////////////////////////////////////////////////////////////////////
// class defenition
//////////////////////////////////////////////////////////////////////////
class SystErrHelper
{
private:
  std::map<TString, TGraphAsymmErrors*> mSyst;

  //////////////////////////////////////////////////////////////////////////
  // create empty TGraph with no Errors
  void initEmptySyst(const TString& syst_name, TH1* hData);

public:
  SystErrHelper() {};

  //////////////////////////////////////////////////////////////////////////
  // create new TGraph with the maximum deflection 
  // of the histograms as error
  // hData is unsed as anker point
  void addSyst(const TString& syst_name, TH1* hData, std::vector<TH1*>& vHist);

};
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////





//////////////////////////////////////////////////////////////////////////
void
SystErrHelper::initEmptySyst(const TString& syst_name, TH1* hData)
{
  unsigned int nBins = hData->GetXaxis()->GetNbins();

  std::vector<double> vXData(nBins);
  std::vector<double> vYData(nBins);
  
  std::vector<double> vXlowErr(nBins);
  std::vector<double> vXupErr(nBins);

  for(unsigned int ibin=1; ibin<=nBins; ibin++) {
    vXData[ibin-1] = hData->GetXaxis()->GetBinCenter(ibin);
    vYData[ibin-1] = hData->GetBinContent(ibin);

    vXlowErr[ibin-1] = vXData[ibin-1] - hData->GetXaxis()->GetBinLowEdge(ibin);
    vXupErr[ibin-1] = hData->GetXaxis()->GetBinLowEdge(ibin+1) - vXData[ibin-1];
  }

  mSyst[syst_name] = new TGraphAsymmErrors(nBins,&vXData[0],&vYData[0],&vXlowErr[0],&vXupErr[0]);
}



//////////////////////////////////////////////////////////////////////////
void
SystErrHelper::addSyst(const TString& syst_name, TH1* hData, std::vector<TH1*>& vHist)
{
  initEmptySyst(syst_name,hData);
}

#endif