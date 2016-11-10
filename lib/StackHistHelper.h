#ifndef STACKHISTHELPER_H
#define STACKHISTHELPER_H

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
class StackHistHelper
{
private:
  std::vector<TH1*> vHist;

public:
  StackHistHelper() {};

  //////////////////////////////////////////////////////////////////////////
  // stack histogram to the previous one in the vector vHist
  void addHist(TH1* h, TString hist_suffix = "_Stack" )
  {
    TH1* hStack = (TH1*)h->Clone( TString(h->GetName()) + hist_suffix );

    if( vHist.size() != 0 ) hStack->Add( vHist[vHist.size()-1] );
    vHist.push_back(hStack);
  }

  //////////////////////////////////////////////////////////////////////////
  // loop over the hist with the histname + vSuffix and 
  // add them to the previous hist
  // the suffix and the order is given by the vector vSuffix 
  //   (first hist will be untoched)
  void addHistFromFileWithSuffix(TFile* file, TString histname, std::vector<TString> vSuffix, TString hist_suffix = "_Stack")
  {
    vHist.clear();

    for(unsigned int iSuffix=0; iSuffix<vSuffix.size(); iSuffix++) {
      TH1* h = (TH1*)file->Get(histname + vSuffix[iSuffix]);
      addHist(h,hist_suffix);
    }
  }

  //////////////////////////////////////////////////////////////////////////
  unsigned int getHistSize() { return vHist.size(); }

  //////////////////////////////////////////////////////////////////////////
  // get hist pointer beginning from 
  // the last (most stacked) when reverse = true
  TH1* getHist(unsigned int nbr, bool reverse=true)
  {
    if( nbr >= vHist.size() ) return NULL;

    if( reverse ) {
      return vHist[ vHist.size()-1-nbr ];
    }

    return vHist[nbr];
  }

  
};
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#endif