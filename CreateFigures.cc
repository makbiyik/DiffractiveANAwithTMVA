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


class StackHistHelper
{
private:
  std::vector<TH1*> vHist;

public:
  StackHistHelper() {};

  void addHist(TH1* h)
  {
    TH1* hStack = (TH1*)h->Clone( TString(h->GetName()) + "_Stack" );

    if( vHist.size() != 0 ) hStack->Add( vHist[vHist.size()-1] );
    vHist.push_back(hStack);
  }

  void addHistFromFileWithPrefix(TFile* file, TString histname, std::vector<TString> vPrefix)
  {
    for(unsigned int iPrefix=0; iPrefix<vPrefix.size(); iPrefix++) {
      TH1* h = (TH1*)file->Get(histname + vPrefix[iPrefix]);
      addHist(h);
    }
  }

  unsigned int getHistSize() { return vHist.size(); }

  TH1* getHist(unsigned int nbr, bool reverse=true)
  {
    if( nbr >= vHist.size() ) return NULL;

    if( reverse ) {
      return vHist[ vHist.size()-1-nbr ];
    }

    return vHist[nbr];
  }

  



};







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
  // as example and for testing
  TH1* htest = new TH1F("htest","",100,-3,3);
  htest->FillRandom("gaus",10000);

  TH1* htest_2 = new TH1F("htest_2","",100,-3,3);
  htest_2->FillRandom("gaus",10000);

  TH1* htest_3 = new TH1F("htest_3","",100,-3,3);
  htest_3->FillRandom("gaus",9000);
  
  TH1* hRatio = (TH1*)htest_3->Clone("hRatio");
  hRatio->Divide(htest);



  CanvasHelper chelpertest("test");
  TCanvas * ctest = chelpertest.initRatioCanvas(-3,3,1,1e5,0,2,"A.U.","N","Ratio");

  chelpertest.addDataHist(htest);
  chelpertest.addMCHist(htest_2);

  chelpertest.addHist(htest_3,"HIST",kYellow,kSolid,20,1001);

  chelpertest.addRatioHist(hRatio,"HIST",kBlue);
  hRatio->SetLineColor(kRed);

  chelpertest.DrawHist();

  ctest->cd(1);
  // plot other hist e.g.





  // test stacking hists
  TFile* file = TFile::Open("data/trackanddiffractive_sigDD_epos.root");
  std::vector<TString> vPrefix;
  vPrefix.push_back("_NONE");
  vPrefix.push_back("_SD1");
  vPrefix.push_back("_SD2");
  vPrefix.push_back("_DD");


  StackHistHelper shh;
  shh.addHistFromFileWithPrefix(file,"MinBias_EPOS_13TeV_MagnetOff_CASTORmeasured_newNoise/Hist_GP_DeltaZero",vPrefix);


  CanvasHelper ch2("ch2");
  ch2.initNormalCanvas(0,8,1,1e5,"A.U.","N");

  Color_t col[4] = {kBlue,kRed,kGreen,kMagenta};

  for(unsigned int iHist=0; iHist<shh.getHistSize()&&iHist<4; iHist++) {
    ch2.addHist( shh.getHist(iHist), "HIST", col[iHist], kSolid, 20, 1001 );
  }
  ch2.DrawHist();
  //////////////////////////////////////////////////////////////////////////





  /////////////////////////////////////////////////
  // freeze program
  app->Run();
  /////////////////////////////////////////////////

  return 0; 
}