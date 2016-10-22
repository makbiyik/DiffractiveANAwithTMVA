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

#include "tdrstyle.C"

#include "CanvasHelper.h"

int main( int argc, char** argv )
{

  //////////////////////////////////////////////////////////////////////////
  // later say app->Run() to freeze the program before execution
  // needed to see figures online
  TApplication * app = new TApplication("app",0,0);

  //////////////////////////////////////////////////////////////////////////
  // setup plot style
  // gStyle->SetOptStat("");
  TStyle* tdrStyle = setTDRStyle();
  tdrStyle->SetOptFit(0);
  tdrStyle->SetPalette(1);
  tdrStyle->SetPadTopMargin(0.06);
  tdrStyle->SetPadRightMargin(0.17);


  //////////////////////////////////////////////////////////////////////////
  // for test purpose
  TH1* htest = new TH1F("htest","",100,-3,3);
  htest->FillRandom("gaus",10000);

  TH1* htest_2 = new TH1F("htest_2","",100,-3,3);
  htest_2->FillRandom("gaus",10000);

  TH1* htest_3 = new TH1F("htest_3","",100,-3,3);
  htest_3->FillRandom("gaus",9000);
  htest_3->SetFillColor(kYellow);
  htest_3->SetLineColor(kYellow);
  htest_3->SetFillStyle(1001);
  
  CanvasHelper chelpertest("test");
  TCanvas * ctest = chelpertest.initRatioCanvas(-3,3,1,1e5,0,2,"A.U.","N","Ratio");
  chelpertest.addDataHist(htest);
  chelpertest.addMCHist(htest_2);
  chelpertest.DrawHist();

  ctest->cd(1);
  htest_3->Draw("same");


  /////////////////////////////////////////////////
  // freeze program
  app->Run();
  /////////////////////////////////////////////////

  return 0; 
}