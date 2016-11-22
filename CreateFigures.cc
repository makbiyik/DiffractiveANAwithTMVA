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

#include "SampleList.h"

#define UNUSED(x) (void)(x) // to avoid unused compiler warning



// temporary struct for training variables directly compared to data
struct sSingleVar
{
  TString hist_name;
  TString xaxis_title;
  TString yaxis_title;
  TString ratio_title;
  TString canvas_title;

  double xmin, xmax;
  double ymin, ymax;
  double rmin, rmax;

  int cms_alignment;
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
std::map<TString, sSingleVar> build_hist_parameters();


//////////////////////////////////////////////////////////////////////////
// as example and for testing
void exampleCode();


//////////////////////////////////////////////////////////////////////////
// plot the variables for tmva training and compare this with data
void training_variables_compare_mc_data(std::map<TString, SampleList::sSample>& mSample);
void single_sample_compare_mc_data(std::map<TString, SampleList::sSample>& mSample,
                                   std::vector<TString>& vSuffix,
                                   std::map<TString, sSingleVar>& mSingleTrainingVar,
                                   TString sample_name,
                                   bool scale_data = true);
void single_figure_compare_mc_data(CanvasHelper& ch,
                                   std::map<TString, SampleList::sSample>& mSample,
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
  std::map<TString, SampleList::sSample> mSample = SampleList::read_data_mc_files();
  std::map<TString, SampleList::sTMVAOutput> mTMVAOutput = SampleList::read_TMVAOutput();
  

  //////////////////////////////////////////////////////////////////////////
  // compare training variables MC with DATA
  training_variables_compare_mc_data(mSample);

  // training_discriminatevalue_compare(mTMVAOutput);
  /////////////////////////////////////////////////s
  // freeze program
  app->Run();
  /////////////////////////////////////////////////

  return 0; 
}
//////////////////////////////////////////////////////////////////////////






//////////////////////////////////////////////////////////////////////////
std::map<TString, sSingleVar> build_hist_parameters() {

  std::map<TString, sSingleVar> mSingleTrainingVar;

  mSingleTrainingVar["EtaDeltaZero"].hist_name = "Hist_Eta_DeltaZero";
  mSingleTrainingVar["EtaDeltaZero"].xaxis_title = "#Delta#eta_{0}";
  mSingleTrainingVar["EtaDeltaZero"].yaxis_title = "1/L dN/d#eta [nb^{1}]";
  mSingleTrainingVar["EtaDeltaZero"].ratio_title = "MC / Data";
  mSingleTrainingVar["EtaDeltaZero"].canvas_title = "cDeltaEtaZero_";
  mSingleTrainingVar["EtaDeltaZero"].xmin = 0;
  mSingleTrainingVar["EtaDeltaZero"].xmax = 6;
  mSingleTrainingVar["EtaDeltaZero"].ymin = 1;
  mSingleTrainingVar["EtaDeltaZero"].ymax = 1e7;
  mSingleTrainingVar["EtaDeltaZero"].rmin = 0;
  mSingleTrainingVar["EtaDeltaZero"].rmax = 3;
  mSingleTrainingVar["EtaDeltaZero"].cms_alignment = 33;


  mSingleTrainingVar["EtaMin"].hist_name = "Hist_Eta_Min";
  mSingleTrainingVar["EtaMin"].xaxis_title = "#eta_{min}";
  mSingleTrainingVar["EtaMin"].yaxis_title = "1/L dN/d#eta [nb^{1}]";
  mSingleTrainingVar["EtaMin"].ratio_title = "MC / Data";
  mSingleTrainingVar["EtaMin"].canvas_title = "cEtaMin_";
  mSingleTrainingVar["EtaMin"].xmin = -4;
  mSingleTrainingVar["EtaMin"].xmax = 4;
  mSingleTrainingVar["EtaMin"].ymin = 1;
  mSingleTrainingVar["EtaMin"].ymax = 1e6;
  mSingleTrainingVar["EtaMin"].rmin = 0;
  mSingleTrainingVar["EtaMin"].rmax = 3;
  mSingleTrainingVar["EtaMin"].cms_alignment = 33;

  mSingleTrainingVar["EtaMax"].hist_name = "Hist_Eta_Max";
  mSingleTrainingVar["EtaMax"].xaxis_title = "#eta_{min}";
  mSingleTrainingVar["EtaMax"].yaxis_title = "1/L dN/d#eta [nb^{1}]";
  mSingleTrainingVar["EtaMax"].ratio_title = "MC / Data";
  mSingleTrainingVar["EtaMax"].canvas_title = "cEtaMax_";
  mSingleTrainingVar["EtaMax"].xmin = -4;
  mSingleTrainingVar["EtaMax"].xmax = 4;
  mSingleTrainingVar["EtaMax"].ymin = 1;
  mSingleTrainingVar["EtaMax"].ymax = 1e6;
  mSingleTrainingVar["EtaMax"].rmin = 0;
  mSingleTrainingVar["EtaMax"].rmax = 3;
  mSingleTrainingVar["EtaMax"].cms_alignment = 33;
  
  mSingleTrainingVar["NTowHF_plus"].hist_name = "Hist_numberoftowerebovenoise_forwardplus";
  mSingleTrainingVar["NTowHF_plus"].xaxis_title = "N_{tow}";
  mSingleTrainingVar["NTowHF_plus"].yaxis_title = "1/L dN/dN_{tow} [nb^{1}]";
  mSingleTrainingVar["NTowHF_plus"].ratio_title = "MC / Data";
  mSingleTrainingVar["NTowHF_plus"].canvas_title = "cNTowHF_plus_";
  mSingleTrainingVar["NTowHF_plus"].xmin = 0;
  mSingleTrainingVar["NTowHF_plus"].xmax = 200;
  mSingleTrainingVar["NTowHF_plus"].ymin = 1;
  mSingleTrainingVar["NTowHF_plus"].ymax = 1e6;
  mSingleTrainingVar["NTowHF_plus"].rmin = 0;
  mSingleTrainingVar["NTowHF_plus"].rmax = 3;
  mSingleTrainingVar["NTowHF_plus"].cms_alignment = 33;


  mSingleTrainingVar["NTowHF_minus"].hist_name = "Hist_numberoftowerebovenoise_forwardminus";
  mSingleTrainingVar["NTowHF_minus"].xaxis_title = "N_{tow}";
  mSingleTrainingVar["NTowHF_minus"].yaxis_title = "1/L dN/dN_{tow} [nb^{1}]";
  mSingleTrainingVar["NTowHF_minus"].ratio_title = "MC / Data";
  mSingleTrainingVar["NTowHF_minus"].canvas_title = "cNTowHF_minus_";
  mSingleTrainingVar["NTowHF_minus"].xmin = 0;
  mSingleTrainingVar["NTowHF_minus"].xmax = 200;
  mSingleTrainingVar["NTowHF_minus"].ymin = 1;
  mSingleTrainingVar["NTowHF_minus"].ymax = 1e6;
  mSingleTrainingVar["NTowHF_minus"].rmin = 0;
  mSingleTrainingVar["NTowHF_minus"].rmax = 3;
  mSingleTrainingVar["NTowHF_minus"].cms_alignment = 33;

  mSingleTrainingVar["NTowCastor"].hist_name = "Hist_numberoftowerebovenoise_castor";
  mSingleTrainingVar["NTowCastor"].xaxis_title = "N_{tow}";
  mSingleTrainingVar["NTowCastor"].yaxis_title = "1/L dN/dN_{tow} [nb^{1}]";
  mSingleTrainingVar["NTowCastor"].ratio_title = "MC / Data";
  mSingleTrainingVar["NTowCastor"].canvas_title = "cNTowCastor_";
  mSingleTrainingVar["NTowCastor"].xmin = 0;
  mSingleTrainingVar["NTowCastor"].xmax = 16;
  mSingleTrainingVar["NTowCastor"].ymin = 1;
  mSingleTrainingVar["NTowCastor"].ymax = 1e6;
  mSingleTrainingVar["NTowCastor"].rmin = 0;
  mSingleTrainingVar["NTowCastor"].rmax = 3;
  mSingleTrainingVar["NTowCastor"].cms_alignment = 33;


  mSingleTrainingVar["NTracks"].hist_name = "Hist_NbrTracks";
  mSingleTrainingVar["NTracks"].xaxis_title = "N_{tow}";
  mSingleTrainingVar["NTracks"].yaxis_title = "1/L dN/dN_{trk} [nb^{1}]";
  mSingleTrainingVar["NTracks"].ratio_title = "MC / Data";
  mSingleTrainingVar["NTracks"].canvas_title = "cNTracks_";
  mSingleTrainingVar["NTracks"].xmin = 0;
  mSingleTrainingVar["NTracks"].xmax = 50;
  mSingleTrainingVar["NTracks"].ymin = 1;
  mSingleTrainingVar["NTracks"].ymax = 1e6;
  mSingleTrainingVar["NTracks"].rmin = 0;
  mSingleTrainingVar["NTracks"].rmax = 3;
  mSingleTrainingVar["NTracks"].cms_alignment = 33;


  mSingleTrainingVar["recoXix"].hist_name = "Hist_log10XiX";
  mSingleTrainingVar["recoXix"].xaxis_title = "log_{10}#xi_{x}";
  mSingleTrainingVar["recoXix"].yaxis_title = "1/L dN/d#xi [nb^{1}]";
  mSingleTrainingVar["recoXix"].ratio_title = "MC / Data";
  mSingleTrainingVar["recoXix"].canvas_title = "crecoXix_";
  mSingleTrainingVar["recoXix"].xmin = -11.5;
  mSingleTrainingVar["recoXix"].xmax = 0.5;
  mSingleTrainingVar["recoXix"].ymin = 1;
  mSingleTrainingVar["recoXix"].ymax = 1e6;
  mSingleTrainingVar["recoXix"].rmin = 0;
  mSingleTrainingVar["recoXix"].rmax = 3;
  mSingleTrainingVar["recoXix"].cms_alignment = 33;

  mSingleTrainingVar["recoXiy"].hist_name = "Hist_log10XiY";
  mSingleTrainingVar["recoXiy"].xaxis_title = "log_{10}#xi_{y}";
  mSingleTrainingVar["recoXiy"].yaxis_title = "1/L dN/d#xi [nb^{1}]";
  mSingleTrainingVar["recoXiy"].ratio_title = "MC / Data";
  mSingleTrainingVar["recoXiy"].canvas_title = "crecoXiy_";
  mSingleTrainingVar["recoXiy"].xmin = -11.5;
  mSingleTrainingVar["recoXiy"].xmax = 0.5;
  mSingleTrainingVar["recoXiy"].ymin = 1;
  mSingleTrainingVar["recoXiy"].ymax = 1e6;
  mSingleTrainingVar["recoXiy"].rmin = 0;
  mSingleTrainingVar["recoXiy"].rmax = 3;
  mSingleTrainingVar["recoXiy"].cms_alignment = 33;


  // mSingleTrainingVar["XiDD"].hist_name = "Hist_Reco_log10XiDD";
  // mSingleTrainingVar["XiDD"].xaxis_title = "log_{10}#xi_{DD}";
  // mSingleTrainingVar["XiDD"].yaxis_title = "1/L dN/d#xi [nb^{1}]";
  // mSingleTrainingVar["XiDD"].ratio_title = "MC / Data";
  // mSingleTrainingVar["XiDD"].canvas_title = "cXiDD_";
  // mSingleTrainingVar["XiDD"].xmin = -11.5;
  // mSingleTrainingVar["XiDD"].xmax = 0.5;
  // mSingleTrainingVar["XiDD"].ymin = 1;
  // mSingleTrainingVar["XiDD"].ymax = 1e6;
  // mSingleTrainingVar["XiDD"].rmin = 0;
  // mSingleTrainingVar["XiDD"].rmax = 3;
  // mSingleTrainingVar["XiDD"].cms_alignment = 33;



  return mSingleTrainingVar;
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
void training_variables_compare_mc_data(std::map<TString, SampleList::sSample>& mSample) {
  
  std::vector<TString> vSuffix;
  vSuffix.push_back("_NONE");
  vSuffix.push_back("_SD1");
  vSuffix.push_back("_SD2");
  vSuffix.push_back("_DD");
  vSuffix.push_back("_Rest");


  std::map<TString, sSingleVar> mSingleTrainingVar = build_hist_parameters();

  single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"Pythia8");
  single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"EPOS",false);


  std::vector<TString> vSuff_XiEventSel;
  vSuff_XiEventSel.push_back("_SD1");
  vSuff_XiEventSel.push_back("_SD2");
  vSuff_XiEventSel.push_back("_DD");
  vSuff_XiEventSel.push_back("_Rest");
  mSingleTrainingVar["EtaDeltaZero"].hist_name = "Hist_eventEtaID_DeltaZero"; 
  single_sample_compare_mc_data(mSample,vSuff_XiEventSel,mSingleTrainingVar,"Pythia8XiEventselectioncut",false);

  // vSuffix.clear(); burasi detector icin
  // vSuffix.push_back("_Barrel");
}

void single_sample_compare_mc_data(std::map<TString, SampleList::sSample>& mSample,
                                   std::vector<TString>& vSuffix,
                                   std::map<TString, sSingleVar>& mSingleTrainingVar,
                                   TString sample_name,
                                   bool scale_data) {

  if( mSample.find(sample_name) == mSample.end() ) {
    std::cout << "In single_sample_compare_mc_data(): map mSample has no Sample with name: "
              << sample_name << " !!!" << std::endl;
    throw;
  }

  for(std::map<TString, sSingleVar>::iterator it = mSingleTrainingVar.begin(); it != mSingleTrainingVar.end(); it++) {
    CanvasHelper ch(it->second.canvas_title + sample_name);
    ch.initRatioCanvas(it->second.xmin,it->second.xmax,
                       it->second.ymin,it->second.ymax,
                       it->second.rmin,it->second.rmax,
                       it->second.xaxis_title,
                       it->second.yaxis_title,
                       it->second.ratio_title);
    single_figure_compare_mc_data(ch,mSample,vSuffix,sample_name,it->second.hist_name,scale_data);
    ch.DrawCMSPreliminary(true,it->second.cms_alignment,"0.34 nb^{-1} (13 TeV)");
  }


}


//////////////////////////////////////////////////////////////////////////
void single_figure_compare_mc_data(CanvasHelper& ch,
                                   std::map<TString, SampleList::sSample>& mSample,
                                   std::vector<TString>& vSuffix,
                                   TString sample_name,
                                   TString hist_var_name,
                                   bool scale_data) {

  //////////////////////////////////////////////////////////////////////////
  // stack the hists together
  StackHistHelper shh;
  try {
    shh.addHistFromFileWithSuffix(mSample[sample_name].file,
                                  mSample[sample_name].tree_name + "/" + hist_var_name,
                                  vSuffix,
                                  sample_name + "_Stack");
  } catch(TString err) {
    std::cout << "In function single_figure_compare_mc_data:" << std::endl;
    std::cout << "!!! Sample: " << sample_name 
              << " has no hist: " << err << std::endl;
  }

  //////////////////////////////////////////////////////////////////////////
  // color code for the different hists
  Color_t col[5] = {29, kGreen -1, kGreen, kYellow, 24};

  /////////////////////////////////////////////////////////////i/////////////
  // access the stacked hists and add it to the canvas helper
  for(unsigned int iHist=0; iHist<shh.getHistSize(); iHist++) {
    shh.getHist(iHist)->Scale( 1/mSample[sample_name].lumi, "width" );
    ch.addHist( shh.getHist(iHist), "HIST", col[iHist], kSolid, 20, 1001 );
  }

  TH1F* hMC = (TH1F*)mSample[sample_name].file->Get(mSample[sample_name].tree_name + "/" + hist_var_name);
  hMC->Scale( 1/mSample[sample_name].lumi, "width" );
  ch.addHist( hMC, "HIST", kBlue+2 );

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