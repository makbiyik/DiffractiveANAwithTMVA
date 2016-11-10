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
#include "TF2.h"
#include "TEfficiency.h"

#include "tdrstyle.C"

#include "CanvasHelper.h"
#include "StackHistHelper.h"
#include "SystErrHelper.h"

#define UNUSED(x) (void)(x) // to avoid unused compiler warning


//////////////////////////////////////////////////////////////////////////
// global struct for ordering data info
struct sSample
{
  bool isData;
  double lumi;
  double xs; // in nb

  TFile* file;
  TString tree_name;
};



//////////////////////////////////////////////////////////////////////////
// do ratio without error propagation
TH1F* get_Ratio(TH1* hMC, TH1* hData) {
  TH1F* hRatio = (TH1F*)hMC->Clone(TString(hMC->GetName()) + "_Ratio");
  hRatio->Divide(hData);

  int nBins = hRatio->GetXaxis()->GetNbins();
  for(int ibin=1; ibin<=nBins; ibin++) {
    hRatio->SetBinError( ibin, hMC->GetBinError(ibin)/hMC->GetBinContent(ibin) );
    if( hMC->GetBinContent(ibin) == 0 ) hRatio->SetBinError( ibin, 0 );
  }

  return hRatio;
}




//////////////////////////////////////////////////////////////////////////
// read in data and mc files with important informations
std::map<TString, sSample> read_data_mc_files();


//////////////////////////////////////////////////////////////////////////
// as example and for testing
void exampleCode();


//////////////////////////////////////////////////////////////////////////
// plot the variables for tmva training and compare this with data
void training_variables_compare_mc_data(std::map<TString, sSample>& mSample);
void single_sample_compare_mc_data(std::map<TString, sSample>& mSample,
                                   std::vector<TString>& vSuffix,
                                   TString sample_name,
                                   bool scale_data = true);
void single_figure_compare_mc_data(CanvasHelper& ch,
                                   std::map<TString, sSample>& mSample,
                                   std::vector<TString>& vSuffix,
                                   TString sample_name,
                                   TString hist_var_name,
                                   bool scale_data = true);



//////////////////////////////////////////////////////////////////////////
// main function
//////////////////////////////////////////////////////////////////////////
int main( int argc, char** argv )
{
  UNUSED(argc);
  UNUSED(argv);

  //////////////////////////////////////////////////////////////////////////
  // later say app->Run() to freeze the program before execution
  // needed to see figures online
  TApplication * app = new TApplication("app",0,0);

  //////////////////////////////////////////////////////////////////////////
  // setup plot style
  // gStyle->SetOptStat("");
  TStyle* tdrStyle = setTDRStyle();
  // own adjustments => !!! mixing up with CanvasHelper
  tdrStyle->SetOptFit(0);
  tdrStyle->SetPalette(1);
  tdrStyle->SetPadTopMargin(0.06);
  tdrStyle->SetPadRightMargin(0.2);
  tdrStyle->SetTitleOffset(1.25, "Z");

  //////////////////////////////////////////////////////////////////////////
  // for testing
  // exampleCode();

  //////////////////////////////////////////////////////////////////////////
  // run code to creat figures
  //////////////////////////////////////////////////////////////////////////
  // read data files
  std::map<TString, sSample> mSample = read_data_mc_files();
  //////////////////////////////////////////////////////////////////////////
  // compare training variables MC with DATA
  training_variables_compare_mc_data(mSample);


  /////////////////////////////////////////////////
  // freeze program
  app->Run();
  /////////////////////////////////////////////////

  return 0; 
}
//////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////
std::map<TString, sSample> read_data_mc_files() {
  TH1F * htmp;

  sSample sPythia8;
  sPythia8.isData = false;
  sPythia8.xs = 71390.000000; // nb
  sPythia8.file = TFile::Open("data/trackanddiffractive_sigDD_Pythia8.root");
  sPythia8.tree_name = "MinBias_TuneMBR_13TeV-pythia8_MagnetOff_CASTORmeasured_newNoise";
  htmp = (TH1F*)sPythia8.file->Get(sPythia8.tree_name + "/hNentries");
  sPythia8.lumi = htmp->GetBinContent(1)/sPythia8.xs;


  sSample sEPOS;
  sEPOS.isData = false;
  sEPOS.lumi = 0;
  sEPOS.xs = 79948.200000; // nb
  sEPOS.file = TFile::Open("data/trackanddiffractive_sigDD_Epos.root");
  sEPOS.tree_name = "MinBias_EPOS_13TeV_MagnetOff_CASTORmeasured_newNoise";
  htmp = (TH1F*)sEPOS.file->Get(sEPOS.tree_name + "/hNentries");
  sEPOS.lumi = htmp->GetBinContent(1)/sEPOS.xs;

  sSample sData;
  sData.isData = true;
  sData.lumi = 0.34; // 1/nb
  sData.xs = 0;
  sData.file = TFile::Open("data/trackanddiffractive_sigDD_Data.root");
  sData.tree_name = "data_ZeroBias1_CASTOR";

  std::map<TString, sSample> mSample;

  mSample["Pythia8"] =  sPythia8;
  mSample["EPOS"] = sEPOS;
  mSample["Data"] = sData;

  return mSample;
}




//////////////////////////////////////////////////////////////////////////
void exampleCode() {

  TH1* htest = new TH1F("htest","",100,-3,3);
  htest->FillRandom("gaus",10000);

  TH1* htest_2 = new TH1F("htest_2","",100,-3,3);
  htest_2->FillRandom("gaus",10000);

  TH1* htest_3 = new TH1F("htest_3","",100,-3,3);
  htest_3->FillRandom("gaus",9000);
  
  TH1* hRatio = (TH1*)htest_3->Clone("hRatio");
  hRatio->Divide(htest);

  //////////////////////////////////////////////////////////////////////////
  // init canvas with ratio pad in the bottom
  CanvasHelper chelpertest("test");
  TCanvas * ctest = chelpertest.initRatioCanvas(-3,3,1,1e5,0,2,"A.U.","N","Ratio");

  //////////////////////////////////////////////////////////////////////////
  // add hists in different ways
  chelpertest.addDataHist(htest);
  chelpertest.addMCHist(htest_2);

  chelpertest.addHist(htest_3,"HIST",kYellow,kSolid,20,1001);

  chelpertest.addRatioHist(hRatio,"HIST",kBlue);
  // change color of hist afterwords
  hRatio->SetLineColor(kRed);

  //////////////////////////////////////////////////////////////////////////
  // draw hist add before
  chelpertest.DrawHist();
  chelpertest.DrawCMSOwnWork();

  //////////////////////////////////////////////////////////////////////////
  // acces the top pad for more adjustments or plots
  ctest->cd(1);
  // plot other hist e.g.




  //////////////////////////////////////////////////////////////////////////
  // open example file for test stacking hists
  // TFile* file = TFile::Open("data/trackanddiffractive_sigDD_epos.root");
  TFile* file = TFile::Open("data/trackanddiffractive_sigDD_mc.root");
  std::vector<TString> vSuffix;
  // in this order order the hists are stacked as follows
  vSuffix.push_back("_NONE"); //=> NONE
  vSuffix.push_back("_SD1");  //=> NONE + SD1
  vSuffix.push_back("_SD2");  //=> NONE + SD1 + SD2
  vSuffix.push_back("_DD");   //=> NONE + SD1 + SD2 + DD
  //////////////////////////////////////////////////////////////////////////
  // stack the hists together
  StackHistHelper shh;
  shh.addHistFromFileWithSuffix(file,"MinBias_TuneMBR_13TeV-pythia8_MagnetOff_CASTORmeasured_newNoise/Hist_Eta_DeltaZero",vSuffix);
  //////////////////////////////////////////////////////////////////////////
  // init canvas just with one pad
  CanvasHelper ch2("ch2");
  ch2.initNormalCanvas(0,8,1,1e6,"A.U.","N");

  //////////////////////////////////////////////////////////////////////////
  // color code for the different hists
  Color_t col[4] = {kRed+2, kGreen -1, kGreen,kYellow};

  //////////////////////////////////////////////////////////////////////////
  // access the stacked hists and add it to the canvas helper
  for(unsigned int iHist=0; iHist<shh.getHistSize()&&iHist<4; iHist++) {
    ch2.addHist( shh.getHist(iHist), "HIST", col[iHist], kSolid, 20, 1001 );
  }


  // draw stacked hists
  ch2.DrawHist();
  // plot CMS Preliminary
  ch2.DrawCMSPreliminary(true,33,"0.34 nb^{-1} (13 TeV)");
  // or CMS Simulation 
  // ch2.DrawCMSSimulation(true,11);
  // or CMS Own Work
  // ch2.DrawCMSOwnWork(true,11);
  //////////////////////////////////////////////////////////////////////////




  //////////////////////////////////////////////////////////////////////////
  // create 2D example hist
  TH2F* h2_test = new TH2F("h2_test","h2_test",30,-3,3,30,-3,3);
  // create 2D function
  TF2* f2d = new TF2("f2d","exp( -(x**2+y**2) ) * sin(x)**2 * cos(y)**2",-3,3,-3,3);
  UNUSED(f2d);
  // fill histogram with 2D function
  h2_test->FillRandom("f2d",10000);


  //////////////////////////////////////////////////////////////////////////
  // init canvas just with one pad
  CanvasHelper ch3("ch3");
  ch3.initTH2Canvas(-3,3, -3,3, 1e-3,1, "X","Y","1/N_{evt} d^{2}N/dXdY");

  h2_test->Scale( 1./10000, "width" ); // 1/N | width -> 1/dx dy
  ch3.addHist( h2_test, "colz" );

  //////////////////////////////////////////////////////////////////////////
  // draw 2D hist
  ch3.DrawHist();
  ch3.DrawCMSSimulation(true,0);
}





//////////////////////////////////////////////////////////////////////////
void training_variables_compare_mc_data(std::map<TString, sSample>& mSample) {
  
  std::vector<TString> vSuffix;
  vSuffix.push_back("_NONE");
  vSuffix.push_back("_SD1");
  vSuffix.push_back("_SD2");
  vSuffix.push_back("_DD");
  vSuffix.push_back("_Rest");


  single_sample_compare_mc_data(mSample,vSuffix,"Pythia8");
  single_sample_compare_mc_data(mSample,vSuffix,"EPOS",false);
}

void single_sample_compare_mc_data(std::map<TString, sSample>& mSample,
                                   std::vector<TString>& vSuffix,
                                   TString sample_name,
                                   bool scale_data) {

  //////////////////////////////////////////////////////////////////////////
  // plot delta eta zero
  CanvasHelper cDeltaEtaZero("cDeltaEtaZero_" + sample_name);
  cDeltaEtaZero.initRatioCanvas(0,6,1,1e7,0,3,"#Delta#eta_{0}","1/L dN/d#eta [nb^{1}]","MC / Data");
  single_figure_compare_mc_data(cDeltaEtaZero,mSample,vSuffix,sample_name,"Hist_Eta_DeltaZero",scale_data);
  // plot CMS Preliminary
  cDeltaEtaZero.DrawCMSPreliminary(true,33,"0.34 nb^{-1} (13 TeV)");


  // plot eta min
  CanvasHelper cEtaMin("cEtaMin_" + sample_name);
  cEtaMin.initRatioCanvas(-4,4,1,1e6,0,3,"#eta_{min}","1/L dN/d#eta [nb^{1}]","MC / Data");
  single_figure_compare_mc_data(cEtaMin,mSample,vSuffix,sample_name,"Hist_Eta_Min",scale_data);
  // plot CMS Preliminary
  cEtaMin.DrawCMSPreliminary(true,33,"0.34 nb^{-1} (13 TeV)");


  // plot eta max
  CanvasHelper cEtaMax("cEtaMax_" + sample_name);
  cEtaMax.initRatioCanvas(-4,4,1,1e6,0,3,"#eta_{max}","1/L dN/d#eta [nb^{1}]","MC / Data");
  single_figure_compare_mc_data(cEtaMax,mSample,vSuffix,sample_name,"Hist_Eta_Max",scale_data);
  // plot CMS Preliminary
  cEtaMax.DrawCMSPreliminary(true,11,"0.34 nb^{-1} (13 TeV)");


  // plot number of towers above noise in HF plus side
  CanvasHelper cNTowHF_plus("cNTowHF_plus_" + sample_name);
  cNTowHF_plus.initRatioCanvas(0,200,1,1e6,0,3,"N_{tow}","1/L dN/dN_{tow} [nb^{1}]","MC / Data");
  single_figure_compare_mc_data(cNTowHF_plus,mSample,vSuffix,sample_name,"Hist_numberoftowerebovenoise_forwardplus",scale_data);
  // plot CMS Preliminary
  cNTowHF_plus.DrawCMSPreliminary(true,11,"0.34 nb^{-1} (13 TeV)");


  // plot number of towers above noise in HF plus side
  CanvasHelper cNTowHF_minus("cNTowHF_minus_" + sample_name);
  cNTowHF_minus.initRatioCanvas(0,200,1,1e6,0,3,"N_{tow}","1/L dN/dN_{tow} [nb^{1}]","MC / Data");
  single_figure_compare_mc_data(cNTowHF_minus,mSample,vSuffix,sample_name,"Hist_numberoftowerebovenoise_forwardminus",scale_data);
  // plot CMS Preliminary
  cNTowHF_minus.DrawCMSPreliminary(true,11,"0.34 nb^{-1} (13 TeV)");
}


//////////////////////////////////////////////////////////////////////////
void single_figure_compare_mc_data(CanvasHelper& ch,
                                   std::map<TString, sSample>& mSample,
                                   std::vector<TString>& vSuffix,
                                   TString sample_name,
                                   TString hist_var_name,
                                   bool scale_data) {

  //////////////////////////////////////////////////////////////////////////
  // stack the hists together
  StackHistHelper shh;
  shh.addHistFromFileWithSuffix(mSample[sample_name].file,
                                mSample[sample_name].tree_name + "/" + hist_var_name,
                                vSuffix,
                                sample_name + "_Stack");

  //////////////////////////////////////////////////////////////////////////
  // color code for the different hists
  Color_t col[5] = {kRed+2, kGreen -1, kGreen, kYellow, kMagenta};

  //////////////////////////////////////////////////////////////////////////
  // access the stacked hists and add it to the canvas helper
  for(unsigned int iHist=0; iHist<shh.getHistSize(); iHist++) {
    shh.getHist(iHist)->Scale( 1/mSample[sample_name].lumi, "width" );
    ch.addHist( shh.getHist(iHist), "HIST", col[iHist], kSolid, 20, 1001 );
  }

  TH1F* hMC = (TH1F*)mSample[sample_name].file->Get(mSample[sample_name].tree_name + "/" + hist_var_name);
  hMC->Scale( 1/mSample[sample_name].lumi, "width" );
  ch.addHist( hMC, "HIST", kBlue );

  //////////////////////////////////////////////////////////////////////////
  // access data hist
  TH1F* hData = (TH1F*)mSample["Data"].file->Get(mSample["Data"].tree_name + "/" + hist_var_name);
  if(scale_data) hData->Scale( 1/mSample["Data"].lumi , "width" );
  ch.addDataHist( hData );

  ch.addRatioHist( get_Ratio(hMC,hData), "HIST", kBlue );
  ch.addRatioHist( get_Ratio(hData,hData), "EP", kBlack );

  // draw stacked hists
  ch.DrawHist();
}