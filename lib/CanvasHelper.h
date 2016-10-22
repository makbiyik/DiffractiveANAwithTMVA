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


//////////////////////////////////////////////////////////////////////////
// class defenition
//////////////////////////////////////////////////////////////////////////
class CanvasHelper {
private:
  TString chName;
  TCanvas * c;

  // double xmin,xmax,ymin,ymax;
  // double ymin_rat, ymax_rat;
  // TString xTitle, yTitle, yTitle_rat;

  TLegend * leg;

  std::vector<TH1*> vData;
  std::vector<TH1*> vMC;
  // std::vector<TH1*> vRatio;

  double relpadhigh;

public:
  CanvasHelper(TString _chName)
    : chName(_chName), c(NULL), 
      // xmin(0), xmax(0), ymin(0), ymax(0), ymin_rat(0), ymax_rat(0),
      // xTitle(""), yTitle(""), yTitle_rat(""), 
      leg(NULL), relpadhigh(0.34)
  {
    c = new TCanvas(TString("c") + chName);
  }

  //////////////////////////////////////////////////////////////////////////
  // init funcitons for canvas to create normal or with ratio plots
  TCanvas* initNormalCanvas(double xlow, double xup,
                            double ylow, double yup,
                            TString xTitle_, TString yTitle,
                            int xAxisNdiv=505, int yAxisNdiv=505);

  //////////////////////////////////////////////////////////////////////////
  void SetUpHist(TH1* h, Color_t col=kBlue, Style_t stl=kSolid, int mkstyle=20);

  //////////////////////////////////////////////////////////////////////////
  // add histograms to draw
  void addNormalDataHist(TH1* h, Color_t col=kBlack, Style_t stl=kSolid, int mkstyle=20);
  void addNormalMCHist(TH1* h, Color_t col=kBlue, Style_t stl=kSolid, int mkstyle=20);


  //////////////////////////////////////////////////////////////////////////
  void DrawHist(bool logScale=true);

  // //////////////////////////////////////////////////////////////////////////
  // CanvasHelper(TCanvas * c_,
  //                   double xlow, double xup,
  //                   double uppad_ylow, double uppad_yup,
  //                   double lowpad_ylow, double lowpad_yup,
  //                   TString xTitle_,
  //                   TString uppad_yTitle, TString lowpad_yTitle)
  // : c(c_), xmin(xlow), xmax(xup), ymin(uppad_ylow), ymax(uppad_yup),
  //   ymin_rat(lowpad_ylow), ymax_rat(lowpad_yup),
  //   xTitle(xTitle_), yTitle(uppad_yTitle), yTitle_rat(lowpad_yTitle), 
  //   leg(NULL), relpadhigh(0.34)
  // {}

  // //////////////////////////////////////////////////////////////////////////
  // TH1F* DrawRatioHist(TH1* hData, TH1* hMc)
  // {
  //   TString title = "Ratio_"; 
  //   title += hData->GetTitle();

  //   TH1F* hRatio = (TH1F*)hData->Clone(title);
  //   hRatio->Divide(hMc);

  //   hRatio->Draw("same");
  //   return hRatio;
  // }

  

  // //////////////////////////////////////////////////////////////////////////
  // static Color_t GetCircColor(int idx)
  // {
  //   const int ncol = 6;
  //   Color_t col[ncol] = {kBlack,kRed,kAzure+10,kGreen+2,kCyan+2,kMagenta};

  //   return col[idx%ncol];
  // }

  // //////////////////////////////////////////////////////////////////////////
  // static Style_t GetCircLineStyle(int idx)
  // {
  //   const int nstl = 3;
  //   Style_t stl[nstl] = {kDashed,kDashDotted,kSolid};

  //   return stl[idx%nstl];
  // }

  // //////////////////////////////////////////////////////////////////////////
  // void SetupRatioCanvas()
  // {
  //   // c->Divide(1,2,0.0001,0.0001);
  //   c->Divide(1,2);

  //   double middlemargin = 0.05;
  //   double bottommargin = 0.35;
  //   double rightmargin = 0.05;

  //   c->cd(1)->SetPad(0,relpadhigh,1,1);
  //   c->cd(1)->SetBottomMargin(middlemargin);
  //   c->cd(1)->SetRightMargin(rightmargin);

  //   c->cd(2)->SetPad(0,0,1,relpadhigh);
  //   c->cd(2)->SetTopMargin(middlemargin);
  //   c->cd(2)->SetBottomMargin(bottommargin);
  //   c->cd(2)->SetRightMargin(rightmargin);
  //   c->cd(2)->SetTitle("");
  // }

  // //////////////////////////////////////////////////////////////////////////
  // void initCanvas(int xAxisNdiv=505, int yAxisNdiv=505)   
  // {
  //   TString hname1 = TString(c->GetName()) + "_h1";
  //   TString hname2 = TString(c->GetName()) + "_h2";

  //   this->SetupRatioCanvas();

  //   // fake hist for draw
  //   TH1F * hsetup1 = new TH1F(hname1,"",100,xmin,xmax);
  //   TH1F * hsetup2 = new TH1F(hname2,"",100,xmin,xmax);


  //   double lablesize = 0.05;
  //   double titlesize = 0.05;//0.04;
  //   double titleoffset = 1.02;

  //   c->cd(1)->SetTickx();
  //   c->cd(1)->SetTicky();
  //   hsetup1->SetMinimum(ymin); hsetup1->SetMaximum(ymax);
  //   hsetup1->Draw();
  //   hsetup1->GetYaxis()->SetLabelSize(lablesize/(1-relpadhigh));
  //   hsetup1->GetXaxis()->SetLabelSize(0);

  //   hsetup1->GetXaxis()->SetNdivisions(xAxisNdiv);

  //   hsetup1->GetYaxis()->SetTitleSize(titlesize/(1-relpadhigh));
  //   hsetup1->GetYaxis()->SetTitleOffset(titleoffset);
  //   hsetup1->GetYaxis()->SetTitle(yTitle);

  //   // hsetup1->GetYaxis()->CenterTitle();

  //   TLine * lh = new TLine(xmin,1.,xmax,1.0);
  //   lh->SetLineStyle(3);

  //   c->cd(2)->SetTickx();
  //   c->cd(2)->SetTicky();
  //   hsetup2->SetMinimum(ymin_rat); hsetup2->SetMaximum(ymax_rat);
  //   hsetup2->Draw();
  //   lh->Draw("same");

  //   hsetup2->GetYaxis()->SetLabelSize(lablesize/relpadhigh);
  //   hsetup2->GetXaxis()->SetLabelSize(lablesize/relpadhigh);

  //   hsetup2->GetXaxis()->SetTitle(xTitle);
  //   hsetup2->GetYaxis()->SetTitle(yTitle_rat);
  //   hsetup2->GetYaxis()->CenterTitle();

  //   hsetup2->GetXaxis()->SetNdivisions(xAxisNdiv);
  //   hsetup2->GetYaxis()->SetNdivisions(yAxisNdiv);

  //   hsetup2->GetYaxis()->SetTitleSize(titlesize/relpadhigh);
  //   hsetup2->GetXaxis()->SetTitleSize(titlesize/relpadhigh);

  //   double offset_correction = relpadhigh/(1-relpadhigh)*1.04;
  //   hsetup2->GetYaxis()->SetTitleOffset(titleoffset*offset_correction);
  //   hsetup2->GetXaxis()->SetTitleOffset(titleoffset);
  // }

  // //////////////////////////////////////////////////////////////////////////
  // void cleanHistToDraw() { vData.clear(); vMC.clear(); vRatio.clear(); }

  // //////////////////////////////////////////////////////////////////////////
  // void AddHistToDraw(TH1* hData, TH1* hMC)
  // {
  //   vData.push_back(hData);
  //   vMC.push_back(hMC);
  // }

  // //////////////////////////////////////////////////////////////////////////
  // void DrawHist(bool logScale=true, bool showMC=true, bool showData=true)
  // {
  //   this->initCanvas();

  //   vRatio.resize(vData.size());

  //   // c->cd(1)->SetLogx();
  //   // c->cd(2)->SetLogx();

  //   c->cd(1)->SetLogy(logScale);
  //   for(int ih=0; ih<vData.size(); ih++) {
  //     SetUpHist(vData[ih], GetCircColor(ih), 21+ih);
  //     SetUpHist(vMC[ih], GetCircColor(ih), 21+ih);

  //     c->cd(1);
  //     if(showData) vData[ih]->Draw("same");
  //     if(showMC)   vMC[ih]->Draw("same hist");

  //     c->cd(2);
  //     // vRatio[ih] = DrawRatioHist(vData[ih],vMC[ih]);
  //     vRatio[ih] = DrawRatioHist(vMC[ih],vData[ih]);
  //   }
  // }

  // //////////////////////////////////////////////////////////////////////////
  // void DrawLegend(double xmin, double xmax, double ymin, double ymax,
  //                 TString legTitle, std::vector<TString>& vDataName, bool useMC = false) {
  //   c->cd(1);

  //   if( vDataName.size() != vData.size() ) return;

  //   leg = new TLegend(xmin,ymin,xmax,ymax);
  //   leg->SetHeader(legTitle);

  //   leg->SetFillColor(0);
  //   leg->SetTextSize(0.035);

  //   for(int ih=0; ih<vData.size(); ih++) {
  //     if(!useMC) leg->AddEntry(vData[ih], vDataName[ih], "p");
  //     else leg->AddEntry(vMC[ih], vDataName[ih], "l");
  //   }

  //   leg->Draw("same");
  // }

  // //////////////////////////////////////////////////////////////////////////
  // TVirtualPad* getUpPad() { return c->cd(1); }
  // TVirtualPad* getLowPad() { return c->cd(2); }

  // //////////////////////////////////////////////////////////////////////////
  // TLegend* getLegend() { return leg; }

  // //////////////////////////////////////////////////////////////////////////
  // std::vector<TH1*>& getDataHists() { return vData; }
  // std::vector<TH1*>& getMCHists() { return vMC; }
  // std::vector<TH1*>& getRatioHists() { return vRatio; }

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

  c->cd(1);
  hsetup1->Draw();

  return c;
}



//////////////////////////////////////////////////////////////////////////
void 
CanvasHelper::SetUpHist(TH1* h, Color_t col, Style_t stl, int mkstyle)
{
    h->SetLineColor(col);
    h->SetMarkerColor(col);
    h->SetLineWidth(3);
    h->SetLineStyle(stl);
    h->SetMarkerStyle(mkstyle);
    h->SetMarkerSize(1.3);
  }


//////////////////////////////////////////////////////////////////////////
void 
CanvasHelper::addNormalDataHist(TH1* h, Color_t col, Style_t stl, int mkstyle)
{
  vData.push_back(h);
  SetUpHist(h,col,stl,mkstyle);
}

void 
CanvasHelper::addNormalMCHist(TH1* h, Color_t col, Style_t stl, int mkstyle)
{
  vMC.push_back(h);
  SetUpHist(h,col,stl,mkstyle);
}


//////////////////////////////////////////////////////////////////////////
void 
CanvasHelper::DrawHist(bool logScale)
{

  c->cd(1);
  if(logScale) c->cd(1)->SetLogy();

  for(unsigned int iMCHist=0; iMCHist<vMC.size(); iMCHist++) {
    vMC[iMCHist]->Draw("HIST same");
  }

  for(unsigned int iDataHist=0; iDataHist<vData.size(); iDataHist++) {
    vData[iDataHist]->Draw("EP same");
  }

}

#endif