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

  TH1* hsetup1;
  TH1* hsetup2;

  bool bRatioCanvas;
  bool b2DHistCanvas;

  struct sHistInfo
  {
    TH1* hist;
    TString draw_option;
  };

  std::vector<sHistInfo> vHist;
  std::vector<sHistInfo> vRatio;

  // internal parameter for fine tuning of the pad's margin
  double relpadhigh;
  double topmargin;
  double middlemargin;
  double bottommargin;
  double rightmargin;

  //////////////////////////////////////////////////////////////////////////
  // privat function to copy configuration of hsetup1 to TH2
  // is needed to draw 2D hist ohterwise DrawHist is not working correctly
  void copyHsetupConfigTo2DHist(TH2* h);

public:
  CanvasHelper(TString _chName)
    : chName(_chName), c(NULL), hsetup1(NULL), hsetup2(NULL),
      bRatioCanvas(false), b2DHistCanvas(false),
      relpadhigh(0.34), topmargin(0.06), middlemargin(0.05), 
      bottommargin(0.13), rightmargin(0.05)
  {
    c = new TCanvas(TString("c") + chName,chName);
  }

  //////////////////////////////////////////////////////////////////////////
  // init funcitons for canvas to create normal or with ratio plots
  // both returns pointer to the canvas for adjustments
  TCanvas* initNormalCanvas(double xlow, double xup,
                            double ylow, double yup,
                            TString xTitle, TString yTitle,
                            int xAxisNdiv=505, int yAxisNdiv=505);

  TCanvas* initRatioCanvas(double xlow, double xup,
                           double ylow, double yup,
                           double ylow_RatioPad, double yup_RatioPad,
                           TString xTitle, TString yTitle, TString yTitle_RatioPad,
                           int xAxisNdiv=505, int yAxisNdiv=505);
  TCanvas* initTH2Canvas(double xlow, double xup,
                         double ylow, double yup,
                         double zlow, double zup,
                         TString xTitle, TString yTitle, TString zTitle,
                         int xAxisNdiv=505, int yAxisNdiv=505, int zAxisNdiv=505); 


  //////////////////////////////////////////////////////////////////////////
  // add histogram for normal pad
  // the option 'same' will automatically added to the draw option
  void addHist(TH1* h, TString draw_option, 
               Color_t col=kBlack, Style_t stl=kSolid, 
               int mkstyle=20, int fillstyle=0);
  // will add the ratio hist
  // hist are only plotted when 'initRatioCanvas()' was used
  // the option 'same' will automatically added to the draw option
  void addRatioHist(TH1* h, TString draw_option, 
                    Color_t col=kBlack, Style_t stl=kSolid, 
                    int mkstyle=20, int fillstyle=0);


  //////////////////////////////////////////////////////////////////////////
  // the difference in the add... functions between MC and DATA 
  // is only the automatic set of draw options

  //////////////////////////////////////////////////////////////////////////
  // add histograms to draw
  // use addHist
  void addDataHist(TH1* h, Color_t col=kBlack, Style_t stl=kSolid, int mkstyle=20) { addHist(h,"EP",col,stl,mkstyle); }
  void addMCHist(TH1* h, Color_t col=kBlue, Style_t stl=kSolid, int mkstyle=20) { addHist(h,"HIST",col,stl,mkstyle); }
  // use addRatioHist
  void addRatioDataHist(TH1* h, Color_t col=kBlack, Style_t stl=kSolid, int mkstyle=20) { addRatioHist(h,"EP",col,stl,mkstyle); }
  void addRatioMCHist(TH1* h, Color_t col=kBlue, Style_t stl=kSolid, int mkstyle=20) { addRatioHist(h,"HIST",col,stl,mkstyle); }


  //////////////////////////////////////////////////////////////////////////
  // do basic style adjustments to the hist
  // like thicker lines and markers
  // col is set line, marker and fill color 
  static void SetUpHist(TH1* h, Color_t col=kBlue, Style_t stl=kSolid, int mkstyle=20, int fillstyle=0);  
  void SetUpLegend(TLegend* leg);

  //////////////////////////////////////////////////////////////////////////
  // plot the histograms add before by addHist or addRatioHist
  // loop over vector vHist and vRatio
  void DrawHist(bool logScale=true);

  //////////////////////////////////////////////////////////////////////////
  // draw CMS always on top
  //   + Preliminary when first bool is set
  //   + Simulation
  // 
  // options posCMSlogo:
  //   alignment * 10 + x-position
  //   true for alignment and x-position
  //   1: left
  //   2: center
  //   3: right
  //   e.g. 13 will plot CMS and Prelimimary on the right 
  //        but align the text to the left
  //
  //  only
  //   0: the text will be plotted on the left side 
  //      above the pad similar like the lumi text
  // 
  // lumiText:
  //   defines luminosity text written on the top of the plot
  void DrawCMSPreliminary(bool writePreliminary=true, int posCMSlogo=11, TString lumiText = "XXX nb^{-1} (XX TeV)");
  void DrawCMSSimulation(bool writePreliminary=true, int posCMSlogo=11, TString lumiText = "");
  void DrawCMSOwnWork(bool writePreliminary=true, int posCMSlogo=11, TString lumiText = "");

  // to fine tune the drawings
  TH1* getSetupHist1() { return hsetup1; }
  TH1* getSetupHist2() { return hsetup2; }

  TCanvas* getCanvas() { return c; }
};
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////





//////////////////////////////////////////////////////////////////////////
// function definition

//////////////////////////////////////////////////////////////////////////
// private function
void 
CanvasHelper::copyHsetupConfigTo2DHist(TH2* h)
{
  h->GetXaxis()->SetRangeUser( hsetup1->GetXaxis()->GetBinLowEdge(1), hsetup1->GetXaxis()->GetBinUpEdge(100) );
  h->GetYaxis()->SetRangeUser( hsetup1->GetYaxis()->GetBinLowEdge(1), hsetup1->GetYaxis()->GetBinUpEdge(100) );
  h->SetMinimum( hsetup1->GetMinimum() ); h->SetMaximum( hsetup1->GetMaximum() );

  h->GetXaxis()->SetTitle( hsetup1->GetXaxis()->GetTitle() );
  h->GetYaxis()->SetTitle( hsetup1->GetYaxis()->GetTitle() );
  h->GetZaxis()->SetTitle( hsetup1->GetZaxis()->GetTitle() );

  h->GetXaxis()->SetNdivisions( hsetup1->GetXaxis()->GetNdivisions() );
  h->GetYaxis()->SetNdivisions( hsetup1->GetYaxis()->GetNdivisions() );
  h->GetZaxis()->SetNdivisions( hsetup1->GetZaxis()->GetNdivisions() );
}
//////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////
TCanvas *
CanvasHelper::initNormalCanvas(double xlow, double xup,
                               double ylow, double yup,
                               TString xTitle, TString yTitle,
                               int xAxisNdiv, int yAxisNdiv)
{

  // setup pad size in canvas
  c->cd(1)->SetTopMargin(topmargin);
  c->cd(1)->SetRightMargin(rightmargin);
  c->cd(1)->SetBottomMargin(bottommargin);

  TString hname1 = chName + "_h1";

  // fake hist for draw
  hsetup1 = new TH1F(hname1,"",100,xlow,xup);

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

  // setup pad size in canvas
  c->cd(1)->SetPad(0,relpadhigh,1,1);
  c->cd(1)->SetTopMargin(topmargin * 1/(1-relpadhigh));
  c->cd(1)->SetBottomMargin(middlemargin);
  c->cd(1)->SetRightMargin(rightmargin);

  c->cd(2)->SetPad(0,0,1,relpadhigh);
  c->cd(2)->SetTopMargin(middlemargin);
  c->cd(2)->SetBottomMargin(bottommargin * 1/relpadhigh);
  c->cd(2)->SetRightMargin(rightmargin);
  // c->cd(2)->SetTitle("");

  TString hname1 = chName + "_h1";
  TString hname2 = chName + "_h2";

  // fake hist for draw
  hsetup1 = new TH1F(hname1,"",100,xlow,xup);
  hsetup2 = new TH1F(hname2,"",100,xlow,xup);


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
TCanvas *
CanvasHelper::initTH2Canvas(double xlow, double xup,
                            double ylow, double yup,
                            double zlow, double zup,
                            TString xTitle, TString yTitle, TString zTitle,
                            int xAxisNdiv, int yAxisNdiv, int zAxisNdiv)
{
  b2DHistCanvas = true;

  // setup pad size in canvas
  c->cd(1)->SetTopMargin(topmargin);
  c->cd(1)->SetRightMargin(0.21); // need to be adjust for Z palette
  c->cd(1)->SetBottomMargin(bottommargin);

  TString hname1 = chName + "_h1";

  // fake hist for draw
  hsetup1 = new TH2F(hname1,"",100,xlow,xup,100,ylow,yup);

  c->cd(1)->SetTickx();
  c->cd(1)->SetTicky();

  hsetup1->SetMinimum(zlow); hsetup1->SetMaximum(zup);
  hsetup1->GetXaxis()->SetTitle(xTitle);
  hsetup1->GetYaxis()->SetTitle(yTitle);
  hsetup1->GetZaxis()->SetTitle(zTitle);

  hsetup1->GetXaxis()->SetNdivisions(xAxisNdiv);
  hsetup1->GetYaxis()->SetNdivisions(yAxisNdiv);
  hsetup1->GetZaxis()->SetNdivisions(zAxisNdiv);

  hsetup1->Draw();

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
void
CanvasHelper::SetUpLegend(TLegend* leg)
{
  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->SetTextFont(42);
  leg->SetTextSize(0.05);
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
  if(logScale) {
    if(b2DHistCanvas) c->cd(1)->SetLogz();
    else              c->cd(1)->SetLogy();
  }

  for(unsigned int iHist=0; iHist<vHist.size(); iHist++) {
    // need to do this again; root do not take the Z range & title from hsetup
    if(b2DHistCanvas) {
      copyHsetupConfigTo2DHist((TH2*)vHist[iHist].hist);
    }

    vHist[iHist].hist->Draw( vHist[iHist].draw_option + " same" );
  }

  // Draw axis again
  hsetup1->Draw("same axis");

  if(bRatioCanvas) {
    c->cd(2);

    for(unsigned int iRatioHist=0; iRatioHist<vRatio.size(); iRatioHist++) {
      vRatio[iRatioHist].hist->Draw( vRatio[iRatioHist].draw_option + " same" );
    }

    // Draw axis again
    hsetup2->Draw("same axis");
  }


}

//////////////////////////////////////////////////////////////////////////
void 
CanvasHelper::DrawCMSPreliminary(bool writePreliminary, int posCMSlogo, TString lumiText)
{
  cms_lumi::writeExtraText = writePreliminary;

  cms_lumi::CMS_lumi((TPad*)c->cd(1),lumiText,posCMSlogo);
}
//////////////////////////////////////////////////////////////////////////
void 
CanvasHelper::DrawCMSSimulation(bool writePreliminary, int posCMSlogo, TString lumiText)
{
  cms_lumi::writeExtraText = writePreliminary;
  cms_lumi::extraText = "Simulation";

  cms_lumi::CMS_lumi((TPad*)c->cd(1),lumiText,posCMSlogo);
}
//////////////////////////////////////////////////////////////////////////
void 
CanvasHelper::DrawCMSOwnWork(bool writePreliminary, int posCMSlogo, TString lumiText)
{
  cms_lumi::writeExtraText = writePreliminary;
  cms_lumi::extraText = "Own Work";

  cms_lumi::CMS_lumi((TPad*)c->cd(1),lumiText,posCMSlogo);
}

#endif