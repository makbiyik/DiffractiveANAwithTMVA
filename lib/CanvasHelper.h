#ifndef CANVASHELPER_H
#define CANVASHELPER_H

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

#include "CMS_lumi.C"


//////////////////////////////////////////////////////////////////////////
// class defenition
//////////////////////////////////////////////////////////////////////////
class CanvasHelper {
private:
  TString chName;
  TCanvas * c;

  double relpadhigh;
  bool bRatioCanvas;

  struct sHistInfo
  {
    TH1* hist;
    TString draw_option;
  };

  std::vector<sHistInfo> vHist;
  std::vector<sHistInfo> vRatio;

public:
  CanvasHelper(TString _chName)
    : chName(_chName), c(NULL), relpadhigh(0.34), bRatioCanvas(false)
  {
    c = new TCanvas(TString("c") + chName,chName);
  }

  //////////////////////////////////////////////////////////////////////////
  // init funcitons for canvas to create normal or with ratio plots
  TCanvas* initNormalCanvas(double xlow, double xup,
                            double ylow, double yup,
                            TString xTitle, TString yTitle,
                            int xAxisNdiv=505, int yAxisNdiv=505);

  TCanvas* initRatioCanvas(double xlow, double xup,
                           double ylow, double yup,
                           double ylow_RatioPad, double yup_RatioPad,
                           TString xTitle, TString yTitle, TString yTitle_RatioPad,
                           int xAxisNdiv=505, int yAxisNdiv=505);  


  //////////////////////////////////////////////////////////////////////////
  // add histogram for normal pad
  // the option 'same' will automatically added to the draw option
  void addHist(TH1* h, TString draw_option, 
               Color_t col=kBlack, Style_t stl=kSolid, 
               int mkstyle=20, int fillstyle=0);
  // will add the ratio hist
  // hist are only plotted when 'initRatioCanvas()' was used
  void addRatioHist(TH1* h, TString draw_option, 
                    Color_t col=kBlack, Style_t stl=kSolid, 
                    int mkstyle=20, int fillstyle=0);


  //////////////////////////////////////////////////////////////////////////
  // the difference in the add... functions between MC and DATA 
  // is only the automatic set of draw options

  //////////////////////////////////////////////////////////////////////////
  // add histograms to draw
  // use addHist
  void addDataHist(TH1* h, Color_t col=kBlack, Style_t stl=kSolid, int mkstyle=20) 
  { addHist(h,"EP",col,stl,mkstyle); }
  void addMCHist(TH1* h, Color_t col=kBlue, Style_t stl=kSolid, int mkstyle=20)
  { addHist(h,"HIST",col,stl,mkstyle); }
  // use addRatioHist
  void addRatioDataHist(TH1* h, Color_t col=kBlack, Style_t stl=kSolid, int mkstyle=20)
  { addRatioHist(h,"EP",col,stl,mkstyle); }
  void addRatioMCHist(TH1* h, Color_t col=kBlue, Style_t stl=kSolid, int mkstyle=20)
  { addRatioHist(h,"HIST",col,stl,mkstyle); }


  //////////////////////////////////////////////////////////////////////////
  void SetUpHist(TH1* h, Color_t col=kBlue, Style_t stl=kSolid, int mkstyle=20, int fillstyle=0);

  //////////////////////////////////////////////////////////////////////////
  void DrawHist(bool logScale=true);

  //////////////////////////////////////////////////////////////////////////
  // draw CMS always on top
  // options posCMSlogo:
  //   alignment * 10 + x-position
  //   true for alignment and x-position
  //   1: left
  //   2: center
  //   3: right
  //   e.g. 13 will plot CMS and Prelimimary on the right but alignt on the left
  // 
  // lumiPeriod:
  //   defines luminosity text written on the top of the plot
  //   for changing please look at CMS_lumi.C and the parameter iPeriod
  void DrawCMSPreliminary(bool writePreliminary=true, int posCMSlogo=11, int lumiPeriod=3);
};
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////





//////////////////////////////////////////////////////////////////////////
// function definition

//////////////////////////////////////////////////////////////////////////
TCanvas *
CanvasHelper::initNormalCanvas(double xlow, double xup,
                               double ylow, double yup,
                               TString xTitle, TString yTitle,
                               int xAxisNdiv, int yAxisNdiv)
{
  bRatioCanvas = false;

  TString hname1 = chName + "_h1";

  // fake hist for draw
  TH1F * hsetup1 = new TH1F(hname1,"",100,xlow,xup);

  c->cd(1)->SetTickx();
  c->cd(1)->SetTicky();

  hsetup1->SetMinimum(ylow); hsetup1->SetMaximum(yup);
  hsetup1->GetXaxis()->SetTitle(xTitle);
  hsetup1->GetYaxis()->SetTitle(yTitle);

  hsetup1->GetXaxis()->SetNdivisions(xAxisNdiv);
  hsetup1->GetYaxis()->SetNdivisions(yAxisNdiv);

  hsetup1->Draw();

  return c;
}
//////////////////////////////////////////////////////////////////////////





//////////////////////////////////////////////////////////////////////////
TCanvas* 
CanvasHelper::initRatioCanvas(double xlow, double xup,
                              double ylow, double yup,
                              double ylow_RatioPad, double yup_RatioPad,
                              TString xTitle, TString yTitle, TString yTitle_RatioPad,
                              int xAxisNdiv, int yAxisNdiv)
{
  bRatioCanvas = true;

  c->Divide(1,2);

  // internal parameter for fine tuning of the pad's margin
  double middlemargin = 0.05;
  double bottommargin = 0.35;
  double rightmargin = 0.05;

  // setup pad size in canvas
  c->cd(1)->SetPad(0,relpadhigh,1,1);
  c->cd(1)->SetBottomMargin(middlemargin);
  c->cd(1)->SetRightMargin(rightmargin);

  c->cd(2)->SetPad(0,0,1,relpadhigh);
  c->cd(2)->SetTopMargin(middlemargin);
  c->cd(2)->SetBottomMargin(bottommargin);
  c->cd(2)->SetRightMargin(rightmargin);
  // c->cd(2)->SetTitle("");

  TString hname1 = chName + "_h1";
  TString hname2 = chName + "_h2";

  // fake hist for draw
  TH1F * hsetup1 = new TH1F(hname1,"",100,xlow,xup);
  TH1F * hsetup2 = new TH1F(hname2,"",100,xlow,xup);


  double lablesize = 0.05;
  double titlesize = 0.05;//0.04;
  double titleoffset = 1.02;

  c->cd(1)->SetTickx();
  c->cd(1)->SetTicky();
  hsetup1->SetMinimum(ylow); hsetup1->SetMaximum(yup);
  hsetup1->Draw();
  hsetup1->GetYaxis()->SetLabelSize(lablesize/(1-relpadhigh));
  hsetup1->GetXaxis()->SetLabelSize(0);

  hsetup1->GetXaxis()->SetNdivisions(xAxisNdiv);

  hsetup1->GetYaxis()->SetTitleSize(titlesize/(1-relpadhigh));
  hsetup1->GetYaxis()->SetTitleOffset(titleoffset);
  hsetup1->GetYaxis()->SetTitle(yTitle);

  // hsetup1->GetYaxis()->CenterTitle();

  TLine * lh = new TLine(xlow,1.,xup,1.0);
  lh->SetLineStyle(3);

  c->cd(2)->SetTickx();
  c->cd(2)->SetTicky();
  hsetup2->SetMinimum(ylow_RatioPad); hsetup2->SetMaximum(yup_RatioPad);
  hsetup2->Draw();
  lh->Draw("same");

  hsetup2->GetYaxis()->SetLabelSize(lablesize/relpadhigh);
  hsetup2->GetXaxis()->SetLabelSize(lablesize/relpadhigh);

  hsetup2->GetXaxis()->SetTitle(xTitle);
  hsetup2->GetYaxis()->SetTitle(yTitle_RatioPad);
  hsetup2->GetYaxis()->CenterTitle();

  hsetup2->GetXaxis()->SetNdivisions(xAxisNdiv);
  hsetup2->GetYaxis()->SetNdivisions(yAxisNdiv);

  hsetup2->GetYaxis()->SetTitleSize(titlesize/relpadhigh);
  hsetup2->GetXaxis()->SetTitleSize(titlesize/relpadhigh);

  double offset_correction = relpadhigh/(1-relpadhigh)*1.04;
  hsetup2->GetYaxis()->SetTitleOffset(titleoffset*offset_correction);
  hsetup2->GetXaxis()->SetTitleOffset(titleoffset);

  return c;
}
//////////////////////////////////////////////////////////////////////////





//////////////////////////////////////////////////////////////////////////
void 
CanvasHelper::SetUpHist(TH1* h, Color_t col, Style_t stl, int mkstyle, int fillstyle)
{
  h->SetLineColor(col);
  h->SetMarkerColor(col);
  h->SetLineWidth(3);
  h->SetLineStyle(stl);
  h->SetMarkerStyle(mkstyle);
  h->SetMarkerSize(1.3);
  h->SetFillColor(col);
  h->SetFillStyle(fillstyle);
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
void 
CanvasHelper::addHist(TH1* h, TString draw_option, 
                      Color_t col, Style_t stl, int mkstyle, int fillstyle)
{
  SetUpHist(h,col,stl,mkstyle,fillstyle);

  sHistInfo shi;
  shi.hist = h;
  shi.draw_option = draw_option;

  vHist.push_back(shi);
}

//////////////////////////////////////////////////////////////////////////
void 
CanvasHelper::addRatioHist(TH1* h, TString draw_option, 
                           Color_t col, Style_t stl, int mkstyle, int fillstyle)
{
  SetUpHist(h,col,stl,mkstyle,fillstyle);

  sHistInfo shi;
  shi.hist = h;
  shi.draw_option = draw_option;

  vRatio.push_back(shi);
}






//////////////////////////////////////////////////////////////////////////
void 
CanvasHelper::DrawHist(bool logScale)
{
  c->cd(1);
  if(logScale) c->cd(1)->SetLogy();

  for(unsigned int iHist=0; iHist<vHist.size(); iHist++) {
    vHist[iHist].hist->Draw( vHist[iHist].draw_option + " same" );
  }


  if(bRatioCanvas) {
    c->cd(2);

    for(unsigned int iRatioHist=0; iRatioHist<vRatio.size(); iRatioHist++) {
      vRatio[iRatioHist].hist->Draw( vRatio[iRatioHist].draw_option + " same" );
    }
  }
}

//////////////////////////////////////////////////////////////////////////
void 
CanvasHelper::DrawCMSPreliminary(bool writePreliminary, int posCMSlogo, int lumiPeriod)
{
  cms_lumi::writeExtraText = writePreliminary;

  cms_lumi::CMS_lumi((TPad*)c->cd(1),lumiPeriod,posCMSlogo);
}

#endif