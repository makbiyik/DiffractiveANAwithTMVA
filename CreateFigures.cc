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

using namespace std;

#define UNUSED(x) (void)(x) // to avoid unused compiler warning

const TString figure_dir = "AN_figures";
const TString figure_type = "png";
// const TString figure_type = "gif";
const bool draw_figure = true;


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
                                   TString data_sample_name,
                                   bool scale_data = true);
void single_sample_compare_syst(std::map<TString, SampleList::sSample>& mSample,
                                std::map<TString, sSingleVar>& mSingleTrainingVar,
                                TString sample_name,
                                TString data_sample_name);
std::map<TString, TH1*> single_figure_compare_mc_data(CanvasHelper& ch,
                                                      std::map<TString, SampleList::sSample>& mSample,
                                                      std::vector<TString>& vSuffix,
                                                      TString sample_name,
                                                      TString data_sample_name,
                                                      TString hist_var_name,
                                                      // TLegend* leg,
                                                      bool scale_data = true);
void draw_legend_for_diff_canvas(std::map<TString, TCanvas*>& mCanvas,
                                 std::map<TString, std::map<TString, TH1*> >& mDrawHists,
                                 std::map<TString, TLegend*>& mLegend,
                                 std::vector<TString>& vSuffix,
                                 TString sample_name,
                                 TString data_sample_name);
void draw_legend_for_diff_canvas_diffMC(std::map<TString, TCanvas*>& mCanvas,
                                        std::map<TString, std::map<TString, TH1*> >& mDrawHists,
                                        std::map<TString, TLegend*>& mLegend,
                                        std::vector<TString>& vSuffix,
                                        TString sample_name,
                                        TString data_sample_name);

//////////////////////////////////////////////////////////////////////////
// plot discriminant value for different proccess
void discriminant_compare_mc_data(std::map<TString, SampleList::sSample>& mSample);


//////////////////////////////////////////////////////////////////////////
// find optimal cut value on discriminant distribution
void discriminant_results(std::map<TString, SampleList::sSample>& mSample);


void calc_signal_cross_section(double& result_cross_section, double& standartError,
                               std::map<TString, SampleList::sSample>& mSample,
                               const TString& sSignal,
                               double classifier_cut);

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
  

  //////////////////////////////////////////////////////////////////////////
  // compare training variables MC with DATA
  training_variables_compare_mc_data(mSample);


  discriminant_compare_mc_data(mSample);
  discriminant_results(mSample);





  //////////////////////////////////////////////////////////////////////////
  double cross_section_result = 0;
  double standartError = 0;

  TCanvas* ctest_clcut = new TCanvas("ctest_clcut");
  ctest_clcut->cd();
  TH1F* htest_clcut = new TH1F("htest_clcut","",21,-1.05,1.05);

  for(double clcut = -1.0; clcut < 0.99; clcut += 0.1) {
    calc_signal_cross_section(cross_section_result, standartError,
                              mSample, "SD1", clcut);
    htest_clcut->GetXaxis()->SetTitle("Cuts are at Discriminant Value");
    htest_clcut->GetYaxis()->SetTitle("#sigma");

    htest_clcut->Fill(clcut, cross_section_result);
    htest_clcut->SetBinError( htest_clcut->FindBin(clcut), standartError );
  }

  htest_clcut->Draw();
  //////////////////////////////////////////////////////////////////////////



  /////////////////////////////////////////////////
  // freeze program
  app->Run();
  /////////////////////////////////////////////////

  return 0; 
}
//////////////////////////////////////////////////////////////////////////






//////////////////////////////////////////////////////////////////////////
std::map<TString, sSingleVar> build_hist_parameters() 
{

  std::map<TString, sSingleVar> mSingleTrainingVar;

  mSingleTrainingVar["DeltaEta"].hist_name = "Hist_Eta_DeltaMax";
  mSingleTrainingVar["DeltaEta"].xaxis_title = "#Delta#eta";
  mSingleTrainingVar["DeltaEta"].yaxis_title = "1/L dN/d#eta [#mub^{-1}]";
  mSingleTrainingVar["DeltaEta"].ratio_title = "MC / Data";
  mSingleTrainingVar["DeltaEta"].canvas_title = "cDeltaEta_";
  mSingleTrainingVar["DeltaEta"].xmin = 0;
  mSingleTrainingVar["DeltaEta"].xmax = 11;
  mSingleTrainingVar["DeltaEta"].ymin = 1;
  mSingleTrainingVar["DeltaEta"].ymax = 1e10;
  mSingleTrainingVar["DeltaEta"].rmin = 0;
  mSingleTrainingVar["DeltaEta"].rmax = 3;
  mSingleTrainingVar["DeltaEta"].cms_alignment = 33;


  mSingleTrainingVar["ForwardEtaDelta"].hist_name = "Hist_forwarddelta";
  mSingleTrainingVar["ForwardEtaDelta"].xaxis_title = "#Delta#eta^{f}";
  mSingleTrainingVar["ForwardEtaDelta"].yaxis_title = "1/L dN/d#eta^{f} [#mub^{-1}]";
  mSingleTrainingVar["ForwardEtaDelta"].ratio_title = "MC / Data";
  mSingleTrainingVar["ForwardEtaDelta"].canvas_title = "cForwardDeltaEta_";
  mSingleTrainingVar["ForwardEtaDelta"].xmin = 0;
  mSingleTrainingVar["ForwardEtaDelta"].xmax = 9;
  mSingleTrainingVar["ForwardEtaDelta"].ymin = 1;
  mSingleTrainingVar["ForwardEtaDelta"].ymax = 1e10;
  mSingleTrainingVar["ForwardEtaDelta"].rmin = 0;
  mSingleTrainingVar["ForwardEtaDelta"].rmax = 3;
  mSingleTrainingVar["ForwardEtaDelta"].cms_alignment = 33;

  mSingleTrainingVar["EtaDeltaZero"].hist_name = "Hist_Eta_DeltaZero";
  mSingleTrainingVar["EtaDeltaZero"].xaxis_title = "#Delta#eta_{0}";
  mSingleTrainingVar["EtaDeltaZero"].yaxis_title = "1/L dN/d#eta [#mub^{-1}]";
  mSingleTrainingVar["EtaDeltaZero"].ratio_title = "MC / Data";
  mSingleTrainingVar["EtaDeltaZero"].canvas_title = "cDeltaEtaZero_";
  mSingleTrainingVar["EtaDeltaZero"].xmin = 0;
  mSingleTrainingVar["EtaDeltaZero"].xmax = 11;
  mSingleTrainingVar["EtaDeltaZero"].ymin = 1;
  mSingleTrainingVar["EtaDeltaZero"].ymax = 1e10;
  mSingleTrainingVar["EtaDeltaZero"].rmin = 0;
  mSingleTrainingVar["EtaDeltaZero"].rmax = 3;
  mSingleTrainingVar["EtaDeltaZero"].cms_alignment = 33;

  mSingleTrainingVar["EtaMin"].hist_name = "Hist_Eta_Min";
  mSingleTrainingVar["EtaMin"].xaxis_title = "#eta_{min}";
  mSingleTrainingVar["EtaMin"].yaxis_title = "1/L dN/d#eta [#mub^{-1}]";
  mSingleTrainingVar["EtaMin"].ratio_title = "MC / Data";
  mSingleTrainingVar["EtaMin"].canvas_title = "cEtaMin_";
  mSingleTrainingVar["EtaMin"].xmin = -6.;
  mSingleTrainingVar["EtaMin"].xmax = 5.5;
  mSingleTrainingVar["EtaMin"].ymin = 1;
  mSingleTrainingVar["EtaMin"].ymax = 1e11;
  mSingleTrainingVar["EtaMin"].rmin = 0;
  mSingleTrainingVar["EtaMin"].rmax = 3;
  mSingleTrainingVar["EtaMin"].cms_alignment = 33;

  



  mSingleTrainingVar["EtaMax"].hist_name = "Hist_Eta_Max";
  mSingleTrainingVar["EtaMax"].xaxis_title = "#eta_{max}";
  mSingleTrainingVar["EtaMax"].yaxis_title = "1/L dN/d#eta [#mub^{-1}]";
  mSingleTrainingVar["EtaMax"].ratio_title = "MC / Data";
  mSingleTrainingVar["EtaMax"].canvas_title = "cEtaMax_";
  mSingleTrainingVar["EtaMax"].xmin = -6.0;
  mSingleTrainingVar["EtaMax"].xmax = 5.5;
  mSingleTrainingVar["EtaMax"].ymin = 1;
  mSingleTrainingVar["EtaMax"].ymax = 1e11;
  mSingleTrainingVar["EtaMax"].rmin = 0;
  mSingleTrainingVar["EtaMax"].rmax = 3;
  mSingleTrainingVar["EtaMax"].cms_alignment = 33;

  
  mSingleTrainingVar["NTowHF_plus"].hist_name = "Hist_numberoftowerebovenoise_forwardplus";
  mSingleTrainingVar["NTowHF_plus"].xaxis_title = "N_{tow}";
  mSingleTrainingVar["NTowHF_plus"].yaxis_title = "1/L dN/dN_{tow} [#mub^{-1}]";
  mSingleTrainingVar["NTowHF_plus"].ratio_title = "MC / Data";
  mSingleTrainingVar["NTowHF_plus"].canvas_title = "cNTowHF_plus_";
  mSingleTrainingVar["NTowHF_plus"].xmin = -0.5;
  mSingleTrainingVar["NTowHF_plus"].xmax = 100.5;
  mSingleTrainingVar["NTowHF_plus"].ymin = 1;
  mSingleTrainingVar["NTowHF_plus"].ymax = 1e8;
  mSingleTrainingVar["NTowHF_plus"].rmin = 0;
  mSingleTrainingVar["NTowHF_plus"].rmax = 3;
  mSingleTrainingVar["NTowHF_plus"].cms_alignment = 33;


 
  mSingleTrainingVar["NTowHF_minus"].hist_name = "Hist_numberoftowerebovenoise_forwardminus";
  mSingleTrainingVar["NTowHF_minus"].xaxis_title = "N_{tow}";
  mSingleTrainingVar["NTowHF_minus"].yaxis_title = "1/L dN/dN_{tow} [#mub^{-1}]";
  mSingleTrainingVar["NTowHF_minus"].ratio_title = "MC / Data";
  mSingleTrainingVar["NTowHF_minus"].canvas_title = "cNTowHF_minus_";
  mSingleTrainingVar["NTowHF_minus"].xmin = -0.5;
  mSingleTrainingVar["NTowHF_minus"].xmax = 100.5;
  mSingleTrainingVar["NTowHF_minus"].ymin = 1;
  mSingleTrainingVar["NTowHF_minus"].ymax = 1e8;
  mSingleTrainingVar["NTowHF_minus"].rmin = 0;
  mSingleTrainingVar["NTowHF_minus"].rmax = 3;
  mSingleTrainingVar["NTowHF_minus"].cms_alignment = 33;


  mSingleTrainingVar["NTowCastor"].hist_name = "Hist_numberoftowerebovenoise_castor";
  mSingleTrainingVar["NTowCastor"].xaxis_title = "N_{tow}";
  mSingleTrainingVar["NTowCastor"].yaxis_title = "1/L dN/dN_{tow} [#mub^{-1}]";
  mSingleTrainingVar["NTowCastor"].ratio_title = "MC / Data";
  mSingleTrainingVar["NTowCastor"].canvas_title = "cNTowCastor_";
  mSingleTrainingVar["NTowCastor"].xmin = 0;
  mSingleTrainingVar["NTowCastor"].xmax = 16;
  mSingleTrainingVar["NTowCastor"].ymin = 1;
  mSingleTrainingVar["NTowCastor"].ymax = 1e8;
  mSingleTrainingVar["NTowCastor"].rmin = 0;
  mSingleTrainingVar["NTowCastor"].rmax = 3;
  mSingleTrainingVar["NTowCastor"].cms_alignment = 33;

 
  mSingleTrainingVar["NTow"].hist_name = "Hist_CaloReducedenergyClass";
  mSingleTrainingVar["NTow"].xaxis_title = "N_{tow}";
  mSingleTrainingVar["NTow"].yaxis_title = "1/L dN/dN_{tow} [#mub^{-1}]";
  mSingleTrainingVar["NTow"].ratio_title = "MC / Data";
  mSingleTrainingVar["NTow"].canvas_title = "cNTow_";
  mSingleTrainingVar["NTow"].xmin = 0;
  mSingleTrainingVar["NTow"].xmax = 50;
  mSingleTrainingVar["NTow"].ymin = 1;
  mSingleTrainingVar["NTow"].ymax = 1e8;
  mSingleTrainingVar["NTow"].rmin = 0;
  mSingleTrainingVar["NTow"].rmax = 3;
  mSingleTrainingVar["NTow"].cms_alignment = 33;

  
  mSingleTrainingVar["NTowMaxHF"].hist_name = "Hist_MaxHFEnergy";
  mSingleTrainingVar["NTowMaxHF"].xaxis_title = "Max_{energytow}";
  mSingleTrainingVar["NTowMaxHF"].yaxis_title = "1/L dN/dN_{tow} [#mub^{-1}]";
  mSingleTrainingVar["NTowMaxHF"].ratio_title = "MC / Data";
  mSingleTrainingVar["NTowMaxHF"].canvas_title = "cEnergyMaxTowHF_";
  mSingleTrainingVar["NTowMaxHF"].xmin = 0;
  mSingleTrainingVar["NTowMaxHF"].xmax = 600;
  mSingleTrainingVar["NTowMaxHF"].ymin = 1;
  mSingleTrainingVar["NTowMaxHF"].ymax = 1e8;
  mSingleTrainingVar["NTowMaxHF"].rmin = 0;
  mSingleTrainingVar["NTowMaxHF"].rmax = 3;
  mSingleTrainingVar["NTowMaxHF"].cms_alignment = 33;

  
  mSingleTrainingVar["SumEnergyHF"].hist_name = "Hist_HFSumEnergy";
  mSingleTrainingVar["SumEnergyHF"].xaxis_title = "Sum_{E}";
  mSingleTrainingVar["SumEnergyHF"].yaxis_title = "1/L dN/dN_{tow} [#mub^{-1}]";
  mSingleTrainingVar["SumEnergyHF"].ratio_title = "MC / Data";
  mSingleTrainingVar["SumEnergyHF"].canvas_title = "cSumEnergyHF_";
  mSingleTrainingVar["SumEnergyHF"].xmin = 0;
  mSingleTrainingVar["SumEnergyHF"].xmax = 3000;
  mSingleTrainingVar["SumEnergyHF"].ymin = 1;
  mSingleTrainingVar["SumEnergyHF"].ymax = 1e8;
  mSingleTrainingVar["SumEnergyHF"].rmin = 0;
  mSingleTrainingVar["SumEnergyHF"].rmax = 3;
  mSingleTrainingVar["SumEnergyHF"].cms_alignment = 33;


  mSingleTrainingVar["SumEnergyCastor"].hist_name = "Hist_CastorSumEnergy";
  mSingleTrainingVar["SumEnergyCastor"].xaxis_title = "Sum_{E}";
  mSingleTrainingVar["SumEnergyCastor"].yaxis_title = "1/L dN/dN_{tow} [#mub^{-1}]";
  mSingleTrainingVar["SumEnergyCastor"].ratio_title = "MC / Data";
  mSingleTrainingVar["SumEnergyCastor"].canvas_title = "cSumEnergyCastor_";
  mSingleTrainingVar["SumEnergyCastor"].xmin = 0;
  mSingleTrainingVar["SumEnergyCastor"].xmax = 3000;
  mSingleTrainingVar["SumEnergyCastor"].ymin = 1;
  mSingleTrainingVar["SumEnergyCastor"].ymax = 1e8;
  mSingleTrainingVar["SumEnergyCastor"].rmin = 0;
  mSingleTrainingVar["SumEnergyCastor"].rmax = 3;
  mSingleTrainingVar["SumEnergyCastor"].cms_alignment = 33;






  mSingleTrainingVar["NTowMaxCastor"].hist_name = "Hist_MaxCastorEnergy";
  mSingleTrainingVar["NTowMaxCastor"].xaxis_title = "Max_{energytow}";
  mSingleTrainingVar["NTowMaxCastor"].yaxis_title = "1/L dN/dN_{tow} [#mub^{-1}]";
  mSingleTrainingVar["NTowMaxCastor"].ratio_title = "MC / Data";
  mSingleTrainingVar["NTowMaxCastor"].canvas_title = "cEnergyMaxTowCastor_";
  mSingleTrainingVar["NTowMaxCastor"].xmin = 0;
  mSingleTrainingVar["NTowMaxCastor"].xmax = 600;
  mSingleTrainingVar["NTowMaxCastor"].ymin = 1;
  mSingleTrainingVar["NTowMaxCastor"].ymax = 1e8;
  mSingleTrainingVar["NTowMaxCastor"].rmin = 0;
  mSingleTrainingVar["NTowMaxCastor"].rmax = 3;
  mSingleTrainingVar["NTowMaxCastor"].cms_alignment = 33;

 


  mSingleTrainingVar["NTracks"].hist_name = "Hist_NbrTracks";
  mSingleTrainingVar["NTracks"].xaxis_title = "N_{trk}";
  mSingleTrainingVar["NTracks"].yaxis_title = "1/L dN/dN_{trk} [#mub^{-1}]";
  mSingleTrainingVar["NTracks"].ratio_title = "MC / Data";
  mSingleTrainingVar["NTracks"].canvas_title = "cNTracks_";
  mSingleTrainingVar["NTracks"].xmin = 0;
  mSingleTrainingVar["NTracks"].xmax = 50;
  mSingleTrainingVar["NTracks"].ymin = 1;
  mSingleTrainingVar["NTracks"].ymax = 1e8;
  mSingleTrainingVar["NTracks"].rmin = 0;
  mSingleTrainingVar["NTracks"].rmax = 3;
  mSingleTrainingVar["NTracks"].cms_alignment = 33;

  mSingleTrainingVar["recoXix"].hist_name = "Hist_log10XiX";
  mSingleTrainingVar["recoXix"].xaxis_title = "log_{10}#xi_{x}";
  mSingleTrainingVar["recoXix"].yaxis_title = "1/L dN/d(log_{10}#xi_{x}) [#mub^{-1}]";
  mSingleTrainingVar["recoXix"].ratio_title = "MC / Data";
  mSingleTrainingVar["recoXix"].canvas_title = "crecoXix_";
  mSingleTrainingVar["recoXix"].xmin = -11.5;
  mSingleTrainingVar["recoXix"].xmax = 0.5;
  mSingleTrainingVar["recoXix"].ymin = 1;
  mSingleTrainingVar["recoXix"].ymax = 1e10;
  mSingleTrainingVar["recoXix"].rmin = 0;
  mSingleTrainingVar["recoXix"].rmax = 3;
  mSingleTrainingVar["recoXix"].cms_alignment = 33;



  mSingleTrainingVar["recoXiy"].hist_name = "Hist_log10XiY";
  mSingleTrainingVar["recoXiy"].xaxis_title = "log_{10}#xi_{y}";
  mSingleTrainingVar["recoXiy"].yaxis_title = "1/L dN/d(log_{10}#xi_{y}) [#mub^{-1}]";
  mSingleTrainingVar["recoXiy"].ratio_title = "MC / Data";
  mSingleTrainingVar["recoXiy"].canvas_title = "crecoXiy_";
  mSingleTrainingVar["recoXiy"].xmin = -11.5;
  mSingleTrainingVar["recoXiy"].xmax = 0.5;
  mSingleTrainingVar["recoXiy"].ymin = 1;
  mSingleTrainingVar["recoXiy"].ymax = 1e10;
  mSingleTrainingVar["recoXiy"].rmin = 0;
  mSingleTrainingVar["recoXiy"].rmax = 3;
  mSingleTrainingVar["recoXiy"].cms_alignment = 33;



  // mSingleTrainingVar["XiDD"].hist_name = "Hist_Reco_log10XiDD";
  // mSingleTrainingVar["XiDD"].xaxis_title = "log_{10}#xi_{DD}";
  // mSingleTrainingVar["XiDD"].yaxis_title = "1/L dN/d#xi [#mub^{-1}]";
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
void exampleCode() 
{

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
  // vSuffix.push_back("_NONE"); //=> NONE
  vSuffix.push_back("_SD1");  //=> NONE + SD1
  vSuffix.push_back("_SD2");  //=> NONE + SD1 + SD2
  vSuffix.push_back("_DD");   //=> NONE + SD1 + SD2 + DD
  vSuffix.push_back("_Rest");
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
  ch2.DrawCMSPreliminary(true,33,"0.34 #mub^{-1} (13 TeV)");
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
void training_variables_compare_mc_data(std::map<TString, SampleList::sSample>& mSample) 
{
  
  std::vector<TString> vSuffix;
  // vSuffix.push_back("_NONE");
  const bool inverted = true;
  if (inverted) {
    vSuffix.push_back("_SD2");
    vSuffix.push_back("_SD1");
    vSuffix.push_back("_DD");
    vSuffix.push_back("_Rest");


    // vSuffix.push_back("_Rest");
    // vSuffix.push_back("_SD1");
    // vSuffix.push_back("_SD2");
    // vSuffix.push_back("_DD");




  } else {  
    vSuffix.push_back("_Rest");
    vSuffix.push_back("_SD2");
    vSuffix.push_back("_SD1");
    vSuffix.push_back("_DD");
  } 
/*
    vSuffix.push_back("_SD1");
    vSuffix.push_back("_SD2");
    vSuffix.push_back("_DD");
    vSuffix.push_back("_Rest");
*/
  std::map<TString, sSingleVar> mSingleTrainingVar = build_hist_parameters();


  single_sample_compare_syst(mSample,mSingleTrainingVar,"Pythia8","Data");

  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"Pythia8","Data");


  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"CUETP8M1","Data",false);


  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"EPOS","Data",false);


  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"Pythia8SD1","Data",false);
  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"EPOSSD1","Data",false);
  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"Pythia8SD2","Data",false);
  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"EPOSSD2","Data",false); 
  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"Pythia8Rest","Data",false);
  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"CUETP8M1","Data",false); 
  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"CUETP8M1_sysplus","Data",false); 
 
  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"Pythia8","Data_sysHFPlus");
  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"CUETP8M1","Data_sysHFPlus",false);
  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"EPOS","Data_sysHFPlus",false);
  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"Pythia8SD1","Data_sysHFPlus",true);
  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"EPOSSD1","Data_sysHFPlus",true);
  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"Pythia8SD2","Data_sysHFPlus",false);
  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"EPOSSD2","Data_sysHFPlus",false); 
  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"Pythia8Rest","Data_sysHFPlus",false);
  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"CUETP8M1","Data_sysHFPlus",false); 
  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"CUETP8M1_sysplus","Data_sysHFPlus",false); 
 

  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"Pythia8","DATA_sysCastorPlus");
  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"CUETP8M1","DATA_sysCastorPlus",false);
  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"EPOS","DATA_sysCastorPlus",false);
  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"Pythia8SD1","DATA_sysCastorPlus",false);
  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"EPOSSD1","DATA_sysCastorPlus",false);
  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"Pythia8SD2","DATA_sysCastorPlus",false);
  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"EPOSSD2","DATA_sysCastorPlus",false); 
  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"Pythia8Rest","DATA_sysCastorPlus",false);
  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"CUETP8M1","DATA_sysCastorPlus",false); 
  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"CUETP8M1_sysplus","DATA_sysCastorPlus",false); 

  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"Pythia8","Data_sysCastorMinus");
  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"CUETP8M1","Data_sysCastorMinus",false);
  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"EPOS","Data_sysCastorMinus",false);
  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"Pythia8SD1","Data_sysCastorMinus",false);
  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"EPOSSD1","Data_sysCastorMinus",false);
  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"Pythia8SD2","Data_sysCastorMinus",false);
  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"EPOSSD2","Data_sysCastorMinus",false); 
  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"Pythia8Rest","Data_sysCastorMinus",false);
  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"CUETP8M1","Data_sysCastorMinus",false); 
  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"CUETP8M1_sysplus","Data_sysCastorMinus",false); 

  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"Pythia8","Data_sysTrackPlus");
  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"CUETP8M1","Data_sysTrackPlus",false);
  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"EPOS","Data_sysTrackPlus",false);
  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"Pythia8SD1","Data_sysTrackPlus",false);
  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"EPOSSD1","Data_sysTrackPlus",false);
  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"Pythia8SD2","Data_sysTrackPlus",false);
  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"EPOSSD2","Data_sysTrackPlus",false); 
  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"Pythia8Rest","Data_sysTrackPlus",false);
  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"CUETP8M1","Data_sysTrackPlus",false); 
  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"CUETP8M1_sysplus","Data_sysTrackPlus",false); 



 // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"Pythia8","Data_sysTrackMinus");
  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"CUETP8M1","Data_sysTrackMinus",false);
  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"EPOS","Data_sysTrackMinus",false);
  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"Pythia8SD1","Data_sysTrackMinus",false);
  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"EPOSSD1","Data_sysTrackMinus",false);
  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"Pythia8SD2","Data_sysTrackMinus",false);
  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"EPOSSD2","Data_sysTrackMinus",false); 
  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"Pythia8Rest","Data_sysTrackMinus",false);
  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"CUETP8M1","Data_sysTrackMinus",false); 
  // single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,"CUETP8M1_sysplus","Data_sysTrackMinus",false); 




  std::vector<TString> vSuff_XiEventSel;
  vSuff_XiEventSel.push_back("_SD2");
  vSuff_XiEventSel.push_back("_SD1");
  vSuff_XiEventSel.push_back("_DD");
  vSuff_XiEventSel.push_back("_Rest");

  // vSuff_XiEventSel.push_back("_Rest");
  // vSuff_XiEventSel.push_back("_SD2");
  // vSuff_XiEventSel.push_back("_SD1");
  // vSuff_XiEventSel.push_back("_DD");
  mSingleTrainingVar["DeltaEta"].hist_name = "Hist_eventXiID_DeltaMax";
  mSingleTrainingVar["ForwardEtaDelta"].hist_name = "Hist_eventXiID_forwarddelta";
  mSingleTrainingVar["EtaDeltaZero"].hist_name = "Hist_eventXiID_DeltaZero";
  mSingleTrainingVar["EtaMin"].hist_name = "Hist_eventXiID_Min";
  mSingleTrainingVar["EtaMax"].hist_name = "Hist_eventXiID_Max";
  mSingleTrainingVar["NTowHF_plus"].hist_name = "Hist_eventXiID_numberoftowerebovenoise_forwardplus";
  mSingleTrainingVar["NTowHF_minus"].hist_name = "Hist_eventXiID_numberoftowerebovenoise_forwardminus";
  mSingleTrainingVar["NTowCastor"].hist_name = "Hist_eventXiID_numberoftowerebovenoise_castor";
  mSingleTrainingVar["NTow"].hist_name = "Hist_eventXiID_CaloReducedenergyClass";
  mSingleTrainingVar["NTowMaxHF"].hist_name = "Hist_eventXiID_MaxHFEnergy"; 
  mSingleTrainingVar["NTowMaxCastor"].hist_name = "Hist_eventXiID_MaxCastorEnergy"; 
  mSingleTrainingVar["SumEnergyHF"].hist_name = "Hist_eventXiID_HFSumEnergy"; 
  mSingleTrainingVar["SumEnergyCastor"].hist_name = "Hist_eventXiID_CastorSumEnergy"; 

  mSingleTrainingVar["NTracks"].hist_name = "Hist_eventXiID_NbrTracks";
  mSingleTrainingVar["recoXix"].hist_name = "Hist_eventXiID_log10XiX";
  mSingleTrainingVar["recoXiy"].hist_name = "Hist_eventXiID_log10XiY";
  // single_sample_compare_mc_data(mSample,vSuff_XiEventSel,mSingleTrainingVar,"XiCutPythia8","Data",true);
  // single_sample_compare_mc_data(mSample,vSuff_XiEventSel,mSingleTrainingVar,"XiCutEPOS","Data",false);
  // single_sample_compare_mc_data(mSample,vSuff_XiEventSel,mSingleTrainingVar,"XiCutPythia8SD1","Data",false);
  // single_sample_compare_mc_data(mSample,vSuff_XiEventSel,mSingleTrainingVar,"XiCutEPOSSD1","Data",false);
  // single_sample_compare_mc_data(mSample,vSuff_XiEventSel,mSingleTrainingVar,"XiCutPythia8SD2","Data",false);
  // single_sample_compare_mc_data(mSample,vSuff_XiEventSel,mSingleTrainingVar,"XiCutEPOSSD2","Data",false); 
  // single_sample_compare_mc_data(mSample,vSuff_XiEventSel,mSingleTrainingVar,"XiCutPythia8Rest","Data",false);
  // single_sample_compare_mc_data(mSample,vSuff_XiEventSel,mSingleTrainingVar,"CUETP8M1","Data",false); 
  
  // single_sample_compare_mc_data(mSample,vSuff_XiEventSel,mSingleTrainingVar,"XiCutPythia8","Data_sysHFPlus",true);
  // single_sample_compare_mc_data(mSample,vSuff_XiEventSel,mSingleTrainingVar,"XiCutEPOS","Data_sysHFPlus",false);
  // single_sample_compare_mc_data(mSample,vSuff_XiEventSel,mSingleTrainingVar,"XiCutPythia8SD1","Data_sysHFPlus",false);
  // single_sample_compare_mc_data(mSample,vSuff_XiEventSel,mSingleTrainingVar,"XiCutEPOSSD1","Data_sysHFPlus",false);
  // single_sample_compare_mc_data(mSample,vSuff_XiEventSel,mSingleTrainingVar,"XiCutPythia8SD2","Data_sysHFPlus",false);
  // single_sample_compare_mc_data(mSample,vSuff_XiEventSel,mSingleTrainingVar,"XiCutEPOSSD2","Data_sysHFPlus",false); 
  // single_sample_compare_mc_data(mSample,vSuff_XiEventSel,mSingleTrainingVar,"XiCutPythia8Rest","Data_sysHFPlus",false);
  // single_sample_compare_mc_data(mSample,vSuff_XiEventSel,mSingleTrainingVar,"CUETP8M1","Data_sysHFPlus",false); 

  // single_sample_compare_mc_data(mSample,vSuff_XiEventSel,mSingleTrainingVar,"XiCutPythia8","DATA_sysCastorPlus",true);
  // single_sample_compare_mc_data(mSample,vSuff_XiEventSel,mSingleTrainingVar,"XiCutEPOS","DATA_sysCastorPlus",false);
  // single_sample_compare_mc_data(mSample,vSuff_XiEventSel,mSingleTrainingVar,"XiCutPythia8SD1","DATA_sysCastorPlus",false);
  // single_sample_compare_mc_data(mSample,vSuff_XiEventSel,mSingleTrainingVar,"XiCutEPOSSD1","DATA_sysCastorPlus",false);
  // single_sample_compare_mc_data(mSample,vSuff_XiEventSel,mSingleTrainingVar,"XiCutPythia8SD2","DATA_sysCastorPlus",false);
  // single_sample_compare_mc_data(mSample,vSuff_XiEventSel,mSingleTrainingVar,"XiCutEPOSSD2","DATA_sysCastorPlus",false); 
  // single_sample_compare_mc_data(mSample,vSuff_XiEventSel,mSingleTrainingVar,"XiCutPythia8Rest","DATA_sysCastorPlus",false);
  // single_sample_compare_mc_data(mSample,vSuff_XiEventSel,mSingleTrainingVar,"CUETP8M1","DATA_sysCastorPlus",false); 

  // single_sample_compare_mc_data(mSample,vSuff_XiEventSel,mSingleTrainingVar,"XiCutPythia8","Data_sysCastorMinus",true);
  // single_sample_compare_mc_data(mSample,vSuff_XiEventSel,mSingleTrainingVar,"XiCutEPOS","Data_sysCastorMinus",false);
  // single_sample_compare_mc_data(mSample,vSuff_XiEventSel,mSingleTrainingVar,"XiCutPythia8SD1","Data_sysCastorMinus",false);
  // single_sample_compare_mc_data(mSample,vSuff_XiEventSel,mSingleTrainingVar,"XiCutEPOSSD1","Data_sysCastorMinus",false);
  // single_sample_compare_mc_data(mSample,vSuff_XiEventSel,mSingleTrainingVar,"XiCutPythia8SD2","Data_sysCastorMinus",false);
  // single_sample_compare_mc_data(mSample,vSuff_XiEventSel,mSingleTrainingVar,"XiCutEPOSSD2","Data_sysCastorMinus",false); 
  // single_sample_compare_mc_data(mSample,vSuff_XiEventSel,mSingleTrainingVar,"XiCutPythia8Rest","Data_sysCastorMinus",false);
  // single_sample_compare_mc_data(mSample,vSuff_XiEventSel,mSingleTrainingVar,"CUETP8M1","Data_sysCastorMinus",false); 

   // single_sample_compare_mc_data(mSample,vSuff_XiEventSel,mSingleTrainingVar,"XiCutPythia8","Data_sysTrackPlus",true);
  // single_sample_compare_mc_data(mSample,vSuff_XiEventSel,mSingleTrainingVar,"XiCutEPOS","Data_sysTrackPlus",false);
  // single_sample_compare_mc_data(mSample,vSuff_XiEventSel,mSingleTrainingVar,"XiCutPythia8SD1","Data_sysTrackPlus",false);
  // single_sample_compare_mc_data(mSample,vSuff_XiEventSel,mSingleTrainingVar,"XiCutEPOSSD1","Data_sysTrackPlus",false);
  // single_sample_compare_mc_data(mSample,vSuff_XiEventSel,mSingleTrainingVar,"XiCutPythia8SD2","Data_sysTrackPlus",false);
  // single_sample_compare_mc_data(mSample,vSuff_XiEventSel,mSingleTrainingVar,"XiCutEPOSSD2","Data_sysTrackPlus",false); 
  // single_sample_compare_mc_data(mSample,vSuff_XiEventSel,mSingleTrainingVar,"XiCutPythia8Rest","Data_sysTrackPlus",false);
  // single_sample_compare_mc_data(mSample,vSuff_XiEventSel,mSingleTrainingVar,"CUETP8M1","Data_sysTrackPlus",false); 



  // single_sample_compare_mc_data(mSample,vSuff_XiEventSel,mSingleTrainingVar,"XiCutPythia8","Data_sysTrackMinus",true);
  // single_sample_compare_mc_data(mSample,vSuff_XiEventSel,mSingleTrainingVar,"XiCutEPOS","Data_sysTrackMinus",false);
  // single_sample_compare_mc_data(mSample,vSuff_XiEventSel,mSingleTrainingVar,"XiCutPythia8SD1","Data_sysTrackMinus",false);
  // single_sample_compare_mc_data(mSample,vSuff_XiEventSel,mSingleTrainingVar,"XiCutEPOSSD1","Data_sysTrackMinus",false);
  // single_sample_compare_mc_data(mSample,vSuff_XiEventSel,mSingleTrainingVar,"XiCutPythia8SD2","Data_sysTrackMinus",false);
  // single_sample_compare_mc_data(mSample,vSuff_XiEventSel,mSingleTrainingVar,"XiCutEPOSSD2","Data_sysTrackMinus",false); 
  // single_sample_compare_mc_data(mSample,vSuff_XiEventSel,mSingleTrainingVar,"XiCutPythia8Rest","Data_sysTrackMinus",false);
  // single_sample_compare_mc_data(mSample,vSuff_XiEventSel,mSingleTrainingVar,"CUETP8M1","Data_sysTrackMinus",false); 





  // vSuffix.clear(); burasi detector icin
  // vSuffix.push_back("_Barrel");
}




void draw_legend_for_diff_canvas(std::map<TString, TCanvas*>& mCanvas,
                                 std::map<TString, std::map<TString, TH1*> >& mDrawHists,
                                 std::map<TString, TLegend*>& mLegend,
                                 std::vector<TString>& vSuffix,
                                 TString sample_name,
                                 TString data_sample_name)
{
  mCanvas["DeltaEta"]->cd(1);
  mLegend["DeltaEta"] = new TLegend(0.37,0.50,0.72,0.87);
  for(unsigned int iSuffix=0; iSuffix<vSuffix.size(); iSuffix++) {
      // define legend text for stacked hist
      TString leg_text = vSuffix[iSuffix];
      leg_text.Remove(0,1);
      // add legend entry
      mLegend["DeltaEta"]->AddEntry( mDrawHists["DeltaEta"][ vSuffix[iSuffix] ], leg_text, "f" );
  }
  mLegend["DeltaEta"]->AddEntry( mDrawHists["DeltaEta"][sample_name], sample_name, "l" );
  mLegend["DeltaEta"]->AddEntry( mDrawHists["DeltaEta"][data_sample_name], "Data", "lep" );
  mLegend["DeltaEta"]->Draw("same");
  if(draw_figure) mCanvas["DeltaEta"]->Print( figure_dir + "/DeltaEta_" + sample_name + "." + figure_type );



  ///
  mCanvas["ForwardEtaDelta"]->cd(1);
  mLegend["ForwardEtaDelta"] = new TLegend(0.37,0.50,0.72,0.87);
  for(unsigned int iSuffix=0; iSuffix<vSuffix.size(); iSuffix++) {
    // define legend text for stacked hist
    TString leg_text = vSuffix[iSuffix];
    leg_text.Remove(0,1);
    // add legend entry
    mLegend["ForwardEtaDelta"]->AddEntry( mDrawHists["ForwardEtaDelta"][ vSuffix[iSuffix] ], leg_text, "f" );
  }
  mLegend["ForwardEtaDelta"]->AddEntry( mDrawHists["ForwardEtaDelta"][sample_name], sample_name, "l" );
  mLegend["ForwardEtaDelta"]->AddEntry( mDrawHists["ForwardEtaDelta"][data_sample_name], "Data", "lep" );
  mLegend["ForwardEtaDelta"]->Draw("same");
  if(draw_figure) mCanvas["ForwardEtaDelta"]->Print( figure_dir + "/ForwardEtaDelta_" + sample_name + "." + figure_type );




  mCanvas["EtaDeltaZero"]->cd(1);
  mLegend["EtaDeltaZero"] = new TLegend(0.37,0.50,0.72,0.87);
  for(unsigned int iSuffix=0; iSuffix<vSuffix.size(); iSuffix++) {
    // define legend text for stacked hist
    TString leg_text = vSuffix[iSuffix];
    leg_text.Remove(0,1);
    // add legend entry
    mLegend["EtaDeltaZero"]->AddEntry( mDrawHists["EtaDeltaZero"][ vSuffix[iSuffix] ], leg_text, "f" );
  }
  mLegend["EtaDeltaZero"]->AddEntry( mDrawHists["EtaDeltaZero"][sample_name], sample_name, "l" );
  mLegend["EtaDeltaZero"]->AddEntry( mDrawHists["EtaDeltaZero"][data_sample_name], "Data", "lep" );
  mLegend["EtaDeltaZero"]->Draw("same");
  if(draw_figure) mCanvas["EtaDeltaZero"]->Print( figure_dir + "/EtaDeltaZero_" + sample_name + "." + figure_type );


  mCanvas["EtaMin"]->cd(1);
  mLegend["EtaMin"] = new TLegend(0.19,0.47,0.59,0.87);
  for(unsigned int iSuffix=0; iSuffix<vSuffix.size(); iSuffix++) {
    // define legend text for stacked hist
    TString leg_text = vSuffix[iSuffix];
    leg_text.Remove(0,1);
    // add legend entry
    mLegend["EtaMin"]->AddEntry( mDrawHists["EtaMin"][ vSuffix[iSuffix] ], leg_text, "f" );
  }
  mLegend["EtaMin"]->AddEntry( mDrawHists["EtaMin"][sample_name], sample_name, "l" );
  mLegend["EtaMin"]->AddEntry( mDrawHists["EtaMin"][data_sample_name], "Data", "lep" );
  mLegend["EtaMin"]->Draw("same");
  if(draw_figure) mCanvas["EtaMin"]->Print( figure_dir + "/EtaMin_" + sample_name + "." + figure_type );
  
  mCanvas["EtaMax"]->cd(1);
  mLegend["EtaMax"] = new TLegend(0.19,0.47,0.59,0.87);
  for(unsigned int iSuffix=0; iSuffix<vSuffix.size(); iSuffix++) {
    // define legend text for stacked hist
    TString leg_text = vSuffix[iSuffix];
    leg_text.Remove(0,1);
    // add legend entry
    mLegend["EtaMax"]->AddEntry( mDrawHists["EtaMax"][ vSuffix[iSuffix] ], leg_text, "f" );
  }
  mLegend["EtaMax"]->AddEntry( mDrawHists["EtaMax"][sample_name], sample_name, "l" );
  mLegend["EtaMax"]->AddEntry( mDrawHists["EtaMax"][data_sample_name], "Data", "lep" );
  mLegend["EtaMax"]->Draw("same");
  if(draw_figure) mCanvas["EtaMax"]->Print( figure_dir + "/EtaMax_" + sample_name + "." + figure_type );

  mCanvas["NTowHF_plus"]->cd(1);
  mLegend["NTowHF_plus"] = new TLegend(0.19,0.47,0.59,0.87);
  for(unsigned int iSuffix=0; iSuffix<vSuffix.size(); iSuffix++) {
    // define legend text for stacked hist
    TString leg_text = vSuffix[iSuffix];
    leg_text.Remove(0,1);
    // add legend entry
    mLegend["NTowHF_plus"]->AddEntry( mDrawHists["NTowHF_plus"][ vSuffix[iSuffix] ], leg_text, "f" );
  }
  mLegend["NTowHF_plus"]->AddEntry( mDrawHists["NTowHF_plus"][sample_name], sample_name, "l" );
  mLegend["NTowHF_plus"]->AddEntry( mDrawHists["NTowHF_plus"][data_sample_name], "Data", "lep" );
  mLegend["NTowHF_plus"]->Draw("same");
  if(draw_figure) mCanvas["NTowHF_plus"]->Print( figure_dir + "/NTowHF_plus_" + sample_name + "." + figure_type );
  
  mCanvas["NTowHF_minus"]->cd(1);
  mLegend["NTowHF_minus"] = new TLegend(0.19,0.47,0.59,0.87);
  for(unsigned int iSuffix=0; iSuffix<vSuffix.size(); iSuffix++) {
    // define legend text for stacked hist
    TString leg_text = vSuffix[iSuffix];
    leg_text.Remove(0,1);
    // add legend entry
    mLegend["NTowHF_minus"]->AddEntry( mDrawHists["NTowHF_minus"][ vSuffix[iSuffix] ], leg_text, "f" );
  }
  mLegend["NTowHF_minus"]->AddEntry( mDrawHists["NTowHF_minus"][sample_name], sample_name, "l" );
  mLegend["NTowHF_minus"]->AddEntry( mDrawHists["NTowHF_minus"][data_sample_name], "Data", "lep" );
  mLegend["NTowHF_minus"]->Draw("same");
  if(draw_figure) mCanvas["NTowHF_minus"]->Print( figure_dir + "/NTowHF_minus_" + sample_name + "." + figure_type );

  mCanvas["NTowCastor"]->cd(1);
  mLegend["NTowCastor"] = new TLegend(0.19,0.47,0.59,0.87);
  for(unsigned int iSuffix=0; iSuffix<vSuffix.size(); iSuffix++) {
    // define legend text for stacked hist
    TString leg_text = vSuffix[iSuffix];
    leg_text.Remove(0,1);
    // add legend entry
    mLegend["NTowCastor"]->AddEntry( mDrawHists["NTowCastor"][ vSuffix[iSuffix] ], leg_text, "f" );
  }
  mLegend["NTowCastor"]->AddEntry( mDrawHists["NTowCastor"][sample_name], sample_name, "l" );
  mLegend["NTowCastor"]->AddEntry( mDrawHists["NTowCastor"][data_sample_name], "Data", "lep" );
  mLegend["NTowCastor"]->Draw("same");
  if(draw_figure) mCanvas["NTowCastor"]->Print( figure_dir + "/NTowCastor_" + sample_name + "." + figure_type );

 
  mCanvas["NTow"]->cd(1);
  mLegend["NTow"] = new TLegend(0.19,0.47,0.59,0.87);
  for(unsigned int iSuffix=0; iSuffix<vSuffix.size(); iSuffix++) {
    // define legend text for stacked hist
    TString leg_text = vSuffix[iSuffix];
    leg_text.Remove(0,1);
    // add legend entry
    mLegend["NTow"]->AddEntry( mDrawHists["NTow"][ vSuffix[iSuffix] ], leg_text, "f" );
  }
  mLegend["NTow"]->AddEntry( mDrawHists["NTow"][sample_name], sample_name, "l" );
  mLegend["NTow"]->AddEntry( mDrawHists["NTow"][data_sample_name], "Data", "lep" );
  mLegend["NTow"]->Draw("same");
  if(draw_figure) mCanvas["NTow"]->Print( figure_dir + "/NTow_" + sample_name + "." + figure_type );


  mCanvas["NTowMaxHF"]->cd(1);
  mLegend["NTowMaxHF"] = new TLegend(0.19,0.47,0.59,0.87);
  for(unsigned int iSuffix=0; iSuffix<vSuffix.size(); iSuffix++) {
    // define legend text for stacked hist
    TString leg_text = vSuffix[iSuffix];
    leg_text.Remove(0,1);
    // add legend entry
    mLegend["NTowMaxHF"]->AddEntry( mDrawHists["NTowMaxHF"][ vSuffix[iSuffix] ], leg_text, "f" );
  }
  mLegend["NTowMaxHF"]->AddEntry( mDrawHists["NTowMaxHF"][sample_name], sample_name, "l" );
  mLegend["NTowMaxHF"]->AddEntry( mDrawHists["NTowMaxHF"][data_sample_name], "Data", "lep" );
  mLegend["NTowMaxHF"]->Draw("same");
  if(draw_figure) mCanvas["NTowMaxHF"]->Print( figure_dir + "/NTowMaxHF_" + sample_name + "." + figure_type );

  mCanvas["SumEnergyHF"]->cd(1);
  mLegend["SumEnergyHF"] = new TLegend(0.19,0.47,0.59,0.87);
  for(unsigned int iSuffix=0; iSuffix<vSuffix.size(); iSuffix++) {
    // define legend text for stacked hist
    TString leg_text = vSuffix[iSuffix];
    leg_text.Remove(0,1);
    // add legend entry
    mLegend["SumEnergyHF"]->AddEntry( mDrawHists["SumEnergyHF"][ vSuffix[iSuffix] ], leg_text, "f" );
  }
  mLegend["SumEnergyHF"]->AddEntry( mDrawHists["SumEnergyHF"][sample_name], sample_name, "l" );
  mLegend["SumEnergyHF"]->AddEntry( mDrawHists["SumEnergyHF"][data_sample_name], "Data", "lep" );
  mLegend["SumEnergyHF"]->Draw("same");
  if(draw_figure) mCanvas["SumEnergyHF"]->Print( figure_dir + "/SumEnergyHF_" + sample_name + "." + figure_type );

  mCanvas["SumEnergyCastor"]->cd(1);
  mLegend["SumEnergyCastor"] = new TLegend(0.19,0.47,0.59,0.87);
  for(unsigned int iSuffix=0; iSuffix<vSuffix.size(); iSuffix++) {
    // define legend text for stacked hist
    TString leg_text = vSuffix[iSuffix];
    leg_text.Remove(0,1);
    // add legend entry
    mLegend["SumEnergyCastor"]->AddEntry( mDrawHists["SumEnergyCastor"][ vSuffix[iSuffix] ], leg_text, "f" );
  }
  mLegend["SumEnergyCastor"]->AddEntry( mDrawHists["SumEnergyCastor"][sample_name], sample_name, "l" );
  mLegend["SumEnergyCastor"]->AddEntry( mDrawHists["SumEnergyCastor"][data_sample_name], "Data", "lep" );
  mLegend["SumEnergyCastor"]->Draw("same");
  if(draw_figure) mCanvas["SumEnergyCastor"]->Print( figure_dir + "/SumEnergyCastor_" + sample_name + "." + figure_type );




  mCanvas["NTowMaxCastor"]->cd(1);
  mLegend["NTowMaxCastor"] = new TLegend(0.19,0.47,0.59,0.87);
  for(unsigned int iSuffix=0; iSuffix<vSuffix.size(); iSuffix++) {
    // define legend text for stacked hist
    TString leg_text = vSuffix[iSuffix];
    leg_text.Remove(0,1);
    // add legend entry
    mLegend["NTowMaxCastor"]->AddEntry( mDrawHists["NTowMaxCastor"][ vSuffix[iSuffix] ], leg_text, "f" );
  }
  mLegend["NTowMaxCastor"]->AddEntry( mDrawHists["NTowMaxCastor"][sample_name], sample_name, "l" );
  mLegend["NTowMaxCastor"]->AddEntry( mDrawHists["NTowMaxCastor"][data_sample_name], "Data", "lep" );
  mLegend["NTowMaxCastor"]->Draw("same");
  if(draw_figure) mCanvas["NTowMaxCastor"]->Print( figure_dir + "/NTowMaxCastor_" + sample_name + "." + figure_type );





  mCanvas["NTracks"]->cd(1);
  mLegend["NTracks"] = new TLegend(0.19,0.47,0.59,0.87);
  for(unsigned int iSuffix=0; iSuffix<vSuffix.size(); iSuffix++) {
    // define legend text for stacked hist
    TString leg_text = vSuffix[iSuffix];
    leg_text.Remove(0,1);
    // add legend entry
    mLegend["NTracks"]->AddEntry( mDrawHists["NTracks"][ vSuffix[iSuffix] ], leg_text, "f" );
  }
  mLegend["NTracks"]->AddEntry( mDrawHists["NTracks"][sample_name], sample_name, "l" );
  mLegend["NTracks"]->AddEntry( mDrawHists["NTracks"][data_sample_name], "Data", "lep" );
  mLegend["NTracks"]->Draw("same");
  if(draw_figure) mCanvas["NTracks"]->Print( figure_dir + "/NTracks_" + sample_name + "." + figure_type );
  
  mCanvas["recoXix"]->cd(1);
  mLegend["recoXix"] = new TLegend(0.19,0.47,0.59,0.87);
  for(unsigned int iSuffix=0; iSuffix<vSuffix.size(); iSuffix++) {
    // define legend text for stacked hist
    TString leg_text = vSuffix[iSuffix];
    leg_text.Remove(0,1);
    // add legend entry
    mLegend["recoXix"]->AddEntry( mDrawHists["recoXix"][ vSuffix[iSuffix] ], leg_text, "f" );
  }
  mLegend["recoXix"]->AddEntry( mDrawHists["recoXix"][sample_name], sample_name, "l" );
  mLegend["recoXix"]->AddEntry( mDrawHists["recoXix"][data_sample_name], "Data", "lep" );
  mLegend["recoXix"]->Draw("same");
  if(draw_figure) mCanvas["recoXix"]->Print( figure_dir + "/recoXix_" + sample_name + "." + figure_type );

  mCanvas["recoXiy"]->cd(1);
  mLegend["recoXiy"] = new TLegend(0.19,0.47,0.59,0.87);
  for(unsigned int iSuffix=0; iSuffix<vSuffix.size(); iSuffix++) {
    // define legend text for stacked hist
    TString leg_text = vSuffix[iSuffix];
    leg_text.Remove(0,1);
    // add legend entry
    mLegend["recoXiy"]->AddEntry( mDrawHists["recoXiy"][ vSuffix[iSuffix] ], leg_text, "f" );
  }
  mLegend["recoXiy"]->AddEntry( mDrawHists["recoXiy"][sample_name], sample_name, "l" );
  mLegend["recoXiy"]->AddEntry( mDrawHists["recoXiy"][data_sample_name], "Data", "lep" );
  mLegend["recoXiy"]->Draw("same");
  if(draw_figure) mCanvas["recoXiy"]->Print( figure_dir + "/recoXiy_" + sample_name + "." + figure_type );
}




void draw_legend_for_diff_canvas_diffMC(std::map<TString, TCanvas*>& mCanvas,
                                        std::map<TString, std::map<TString, TH1*> >& mDrawHists,
                                        std::map<TString, TLegend*>& mLegend,
                                        std::vector<TString>& vSuffix,
                                        TString sample_name,
                                        TString data_sample_name)
{


  mCanvas["DeltaEta"]->cd(1);
  mLegend["DeltaEta"] = new TLegend(0.37,0.50,0.72,0.87);
  for(unsigned int iSuffix=0; iSuffix<vSuffix.size(); iSuffix++) {
      // define legend text for stacked hist
      TString leg_text = vSuffix[iSuffix];
      // leg_text.Remove(0,1);
      // add legend entry
      mLegend["DeltaEta"]->AddEntry( mDrawHists["DeltaEta"][ vSuffix[iSuffix] ], leg_text, "l" );
  }
  mLegend["DeltaEta"]->AddEntry( mDrawHists["DeltaEta"][sample_name], sample_name, "l" );
  mLegend["DeltaEta"]->AddEntry( mDrawHists["DeltaEta"][data_sample_name], "Data", "lep" );
  mLegend["DeltaEta"]->Draw("same");
  if(draw_figure) mCanvas["DeltaEta"]->Print( figure_dir + "/DeltaEta_" + sample_name + "." + figure_type );



  ///
  mCanvas["ForwardEtaDelta"]->cd(1);
  mLegend["ForwardEtaDelta"] = new TLegend(0.37,0.50,0.72,0.87);
  for(unsigned int iSuffix=0; iSuffix<vSuffix.size(); iSuffix++) {
    // define legend text for stacked hist
    TString leg_text = vSuffix[iSuffix];
    leg_text.Remove(0,1);
    // add legend entry
    mLegend["ForwardEtaDelta"]->AddEntry( mDrawHists["ForwardEtaDelta"][ vSuffix[iSuffix] ], leg_text, "f" );
  }
  mLegend["ForwardEtaDelta"]->AddEntry( mDrawHists["ForwardEtaDelta"][sample_name], sample_name, "l" );
  mLegend["ForwardEtaDelta"]->AddEntry( mDrawHists["ForwardEtaDelta"][data_sample_name], "Data", "lep" );
  mLegend["ForwardEtaDelta"]->Draw("same");
  if(draw_figure) mCanvas["ForwardEtaDelta"]->Print( figure_dir + "/ForwardEtaDelta_" + sample_name + "." + figure_type );




  mCanvas["EtaDeltaZero"]->cd(1);
  mLegend["EtaDeltaZero"] = new TLegend(0.37,0.50,0.72,0.87);
  for(unsigned int iSuffix=0; iSuffix<vSuffix.size(); iSuffix++) {
    // define legend text for stacked hist
    TString leg_text = vSuffix[iSuffix];
    leg_text.Remove(0,1);
    // add legend entry
    mLegend["EtaDeltaZero"]->AddEntry( mDrawHists["EtaDeltaZero"][ vSuffix[iSuffix] ], leg_text, "f" );
  }
  mLegend["EtaDeltaZero"]->AddEntry( mDrawHists["EtaDeltaZero"][sample_name], sample_name, "l" );
  mLegend["EtaDeltaZero"]->AddEntry( mDrawHists["EtaDeltaZero"][data_sample_name], "Data", "lep" );
  mLegend["EtaDeltaZero"]->Draw("same");
  if(draw_figure) mCanvas["EtaDeltaZero"]->Print( figure_dir + "/EtaDeltaZero_" + sample_name + "." + figure_type );


  mCanvas["EtaMin"]->cd(1);
  mLegend["EtaMin"] = new TLegend(0.19,0.47,0.59,0.87);
  for(unsigned int iSuffix=0; iSuffix<vSuffix.size(); iSuffix++) {
    // define legend text for stacked hist
    TString leg_text = vSuffix[iSuffix];
    leg_text.Remove(0,1);
    // add legend entry
    mLegend["EtaMin"]->AddEntry( mDrawHists["EtaMin"][ vSuffix[iSuffix] ], leg_text, "f" );
  }
  mLegend["EtaMin"]->AddEntry( mDrawHists["EtaMin"][sample_name], sample_name, "l" );
  mLegend["EtaMin"]->AddEntry( mDrawHists["EtaMin"][data_sample_name], "Data", "lep" );
  mLegend["EtaMin"]->Draw("same");
  if(draw_figure) mCanvas["EtaMin"]->Print( figure_dir + "/EtaMin_" + sample_name + "." + figure_type );
  
  mCanvas["EtaMax"]->cd(1);
  mLegend["EtaMax"] = new TLegend(0.19,0.47,0.59,0.87);
  for(unsigned int iSuffix=0; iSuffix<vSuffix.size(); iSuffix++) {
    // define legend text for stacked hist
    TString leg_text = vSuffix[iSuffix];
    leg_text.Remove(0,1);
    // add legend entry
    mLegend["EtaMax"]->AddEntry( mDrawHists["EtaMax"][ vSuffix[iSuffix] ], leg_text, "f" );
  }
  mLegend["EtaMax"]->AddEntry( mDrawHists["EtaMax"][sample_name], sample_name, "l" );
  mLegend["EtaMax"]->AddEntry( mDrawHists["EtaMax"][data_sample_name], "Data", "lep" );
  mLegend["EtaMax"]->Draw("same");
  if(draw_figure) mCanvas["EtaMax"]->Print( figure_dir + "/EtaMax_" + sample_name + "." + figure_type );

  mCanvas["NTowHF_plus"]->cd(1);
  mLegend["NTowHF_plus"] = new TLegend(0.19,0.47,0.59,0.87);
  for(unsigned int iSuffix=0; iSuffix<vSuffix.size(); iSuffix++) {
    // define legend text for stacked hist
    TString leg_text = vSuffix[iSuffix];
    leg_text.Remove(0,1);
    // add legend entry
    mLegend["NTowHF_plus"]->AddEntry( mDrawHists["NTowHF_plus"][ vSuffix[iSuffix] ], leg_text, "f" );
  }
  mLegend["NTowHF_plus"]->AddEntry( mDrawHists["NTowHF_plus"][sample_name], sample_name, "l" );
  mLegend["NTowHF_plus"]->AddEntry( mDrawHists["NTowHF_plus"][data_sample_name], "Data", "lep" );
  mLegend["NTowHF_plus"]->Draw("same");
  if(draw_figure) mCanvas["NTowHF_plus"]->Print( figure_dir + "/NTowHF_plus_" + sample_name + "." + figure_type );
  
  mCanvas["NTowHF_minus"]->cd(1);
  mLegend["NTowHF_minus"] = new TLegend(0.19,0.47,0.59,0.87);
  for(unsigned int iSuffix=0; iSuffix<vSuffix.size(); iSuffix++) {
    // define legend text for stacked hist
    TString leg_text = vSuffix[iSuffix];
    leg_text.Remove(0,1);
    // add legend entry
    mLegend["NTowHF_minus"]->AddEntry( mDrawHists["NTowHF_minus"][ vSuffix[iSuffix] ], leg_text, "f" );
  }
  mLegend["NTowHF_minus"]->AddEntry( mDrawHists["NTowHF_minus"][sample_name], sample_name, "l" );
  mLegend["NTowHF_minus"]->AddEntry( mDrawHists["NTowHF_minus"][data_sample_name], "Data", "lep" );
  mLegend["NTowHF_minus"]->Draw("same");
  if(draw_figure) mCanvas["NTowHF_minus"]->Print( figure_dir + "/NTowHF_minus_" + sample_name + "." + figure_type );

  mCanvas["NTowCastor"]->cd(1);
  mLegend["NTowCastor"] = new TLegend(0.19,0.47,0.59,0.87);
  for(unsigned int iSuffix=0; iSuffix<vSuffix.size(); iSuffix++) {
    // define legend text for stacked hist
    TString leg_text = vSuffix[iSuffix];
    leg_text.Remove(0,1);
    // add legend entry
    mLegend["NTowCastor"]->AddEntry( mDrawHists["NTowCastor"][ vSuffix[iSuffix] ], leg_text, "f" );
  }
  mLegend["NTowCastor"]->AddEntry( mDrawHists["NTowCastor"][sample_name], sample_name, "l" );
  mLegend["NTowCastor"]->AddEntry( mDrawHists["NTowCastor"][data_sample_name], "Data", "lep" );
  mLegend["NTowCastor"]->Draw("same");
  if(draw_figure) mCanvas["NTowCastor"]->Print( figure_dir + "/NTowCastor_" + sample_name + "." + figure_type );

 
  mCanvas["NTow"]->cd(1);
  mLegend["NTow"] = new TLegend(0.19,0.47,0.59,0.87);
  for(unsigned int iSuffix=0; iSuffix<vSuffix.size(); iSuffix++) {
    // define legend text for stacked hist
    TString leg_text = vSuffix[iSuffix];
    leg_text.Remove(0,1);
    // add legend entry
    mLegend["NTow"]->AddEntry( mDrawHists["NTow"][ vSuffix[iSuffix] ], leg_text, "f" );
  }
  mLegend["NTow"]->AddEntry( mDrawHists["NTow"][sample_name], sample_name, "l" );
  mLegend["NTow"]->AddEntry( mDrawHists["NTow"][data_sample_name], "Data", "lep" );
  mLegend["NTow"]->Draw("same");
  if(draw_figure) mCanvas["NTow"]->Print( figure_dir + "/NTow_" + sample_name + "." + figure_type );


  mCanvas["NTowMaxHF"]->cd(1);
  mLegend["NTowMaxHF"] = new TLegend(0.19,0.47,0.59,0.87);
  for(unsigned int iSuffix=0; iSuffix<vSuffix.size(); iSuffix++) {
    // define legend text for stacked hist
    TString leg_text = vSuffix[iSuffix];
    leg_text.Remove(0,1);
    // add legend entry
    mLegend["NTowMaxHF"]->AddEntry( mDrawHists["NTowMaxHF"][ vSuffix[iSuffix] ], leg_text, "f" );
  }
  mLegend["NTowMaxHF"]->AddEntry( mDrawHists["NTowMaxHF"][sample_name], sample_name, "l" );
  mLegend["NTowMaxHF"]->AddEntry( mDrawHists["NTowMaxHF"][data_sample_name], "Data", "lep" );
  mLegend["NTowMaxHF"]->Draw("same");
  if(draw_figure) mCanvas["NTowMaxHF"]->Print( figure_dir + "/NTowMaxHF_" + sample_name + "." + figure_type );

  mCanvas["SumEnergyHF"]->cd(1);
  mLegend["SumEnergyHF"] = new TLegend(0.19,0.47,0.59,0.87);
  for(unsigned int iSuffix=0; iSuffix<vSuffix.size(); iSuffix++) {
    // define legend text for stacked hist
    TString leg_text = vSuffix[iSuffix];
    leg_text.Remove(0,1);
    // add legend entry
    mLegend["SumEnergyHF"]->AddEntry( mDrawHists["SumEnergyHF"][ vSuffix[iSuffix] ], leg_text, "f" );
  }
  mLegend["SumEnergyHF"]->AddEntry( mDrawHists["SumEnergyHF"][sample_name], sample_name, "l" );
  mLegend["SumEnergyHF"]->AddEntry( mDrawHists["SumEnergyHF"][data_sample_name], "Data", "lep" );
  mLegend["SumEnergyHF"]->Draw("same");
  if(draw_figure) mCanvas["SumEnergyHF"]->Print( figure_dir + "/SumEnergyHF_" + sample_name + "." + figure_type );

  mCanvas["SumEnergyCastor"]->cd(1);
  mLegend["SumEnergyCastor"] = new TLegend(0.19,0.47,0.59,0.87);
  for(unsigned int iSuffix=0; iSuffix<vSuffix.size(); iSuffix++) {
    // define legend text for stacked hist
    TString leg_text = vSuffix[iSuffix];
    leg_text.Remove(0,1);
    // add legend entry
    mLegend["SumEnergyCastor"]->AddEntry( mDrawHists["SumEnergyCastor"][ vSuffix[iSuffix] ], leg_text, "f" );
  }
  mLegend["SumEnergyCastor"]->AddEntry( mDrawHists["SumEnergyCastor"][sample_name], sample_name, "l" );
  mLegend["SumEnergyCastor"]->AddEntry( mDrawHists["SumEnergyCastor"][data_sample_name], "Data", "lep" );
  mLegend["SumEnergyCastor"]->Draw("same");
  if(draw_figure) mCanvas["SumEnergyCastor"]->Print( figure_dir + "/SumEnergyCastor_" + sample_name + "." + figure_type );




  mCanvas["NTowMaxCastor"]->cd(1);
  mLegend["NTowMaxCastor"] = new TLegend(0.19,0.47,0.59,0.87);
  for(unsigned int iSuffix=0; iSuffix<vSuffix.size(); iSuffix++) {
    // define legend text for stacked hist
    TString leg_text = vSuffix[iSuffix];
    leg_text.Remove(0,1);
    // add legend entry
    mLegend["NTowMaxCastor"]->AddEntry( mDrawHists["NTowMaxCastor"][ vSuffix[iSuffix] ], leg_text, "f" );
  }
  mLegend["NTowMaxCastor"]->AddEntry( mDrawHists["NTowMaxCastor"][sample_name], sample_name, "l" );
  mLegend["NTowMaxCastor"]->AddEntry( mDrawHists["NTowMaxCastor"][data_sample_name], "Data", "lep" );
  mLegend["NTowMaxCastor"]->Draw("same");
  if(draw_figure) mCanvas["NTowMaxCastor"]->Print( figure_dir + "/NTowMaxCastor_" + sample_name + "." + figure_type );





  mCanvas["NTracks"]->cd(1);
  mLegend["NTracks"] = new TLegend(0.19,0.47,0.59,0.87);
  for(unsigned int iSuffix=0; iSuffix<vSuffix.size(); iSuffix++) {
    // define legend text for stacked hist
    TString leg_text = vSuffix[iSuffix];
    leg_text.Remove(0,1);
    // add legend entry
    mLegend["NTracks"]->AddEntry( mDrawHists["NTracks"][ vSuffix[iSuffix] ], leg_text, "f" );
  }
  mLegend["NTracks"]->AddEntry( mDrawHists["NTracks"][sample_name], sample_name, "l" );
  mLegend["NTracks"]->AddEntry( mDrawHists["NTracks"][data_sample_name], "Data", "lep" );
  mLegend["NTracks"]->Draw("same");
  if(draw_figure) mCanvas["NTracks"]->Print( figure_dir + "/NTracks_" + sample_name + "." + figure_type );
  
  mCanvas["recoXix"]->cd(1);
  mLegend["recoXix"] = new TLegend(0.19,0.47,0.59,0.87);
  for(unsigned int iSuffix=0; iSuffix<vSuffix.size(); iSuffix++) {
    // define legend text for stacked hist
    TString leg_text = vSuffix[iSuffix];
    leg_text.Remove(0,1);
    // add legend entry
    mLegend["recoXix"]->AddEntry( mDrawHists["recoXix"][ vSuffix[iSuffix] ], leg_text, "f" );
  }
  mLegend["recoXix"]->AddEntry( mDrawHists["recoXix"][sample_name], sample_name, "l" );
  mLegend["recoXix"]->AddEntry( mDrawHists["recoXix"][data_sample_name], "Data", "lep" );
  mLegend["recoXix"]->Draw("same");
  if(draw_figure) mCanvas["recoXix"]->Print( figure_dir + "/recoXix_" + sample_name + "." + figure_type );

  mCanvas["recoXiy"]->cd(1);
  mLegend["recoXiy"] = new TLegend(0.19,0.47,0.59,0.87);
  for(unsigned int iSuffix=0; iSuffix<vSuffix.size(); iSuffix++) {
    // define legend text for stacked hist
    TString leg_text = vSuffix[iSuffix];
    leg_text.Remove(0,1);
    // add legend entry
    mLegend["recoXiy"]->AddEntry( mDrawHists["recoXiy"][ vSuffix[iSuffix] ], leg_text, "f" );
  }
  mLegend["recoXiy"]->AddEntry( mDrawHists["recoXiy"][sample_name], sample_name, "l" );
  mLegend["recoXiy"]->AddEntry( mDrawHists["recoXiy"][data_sample_name], "Data", "lep" );
  mLegend["recoXiy"]->Draw("same");
  if(draw_figure) mCanvas["recoXiy"]->Print( figure_dir + "/recoXiy_" + sample_name + "." + figure_type );
}




void single_sample_compare_mc_data(std::map<TString, SampleList::sSample>& mSample,
                                   std::vector<TString>& vSuffix,
                                   std::map<TString, sSingleVar>& mSingleTrainingVar,
                                   TString sample_name,
                                   TString data_sample_name,
                                   bool scale_data) 
{

  if( mSample.find(sample_name) == mSample.end() ) {
    std::cout << "In single_sample_compare_mc_data(): map mSample has no Sample with name: "
              << sample_name << " !!!" << std::endl;
    throw;
  }

  
  std::map<TString, TCanvas*> mCanvas;
  std::map<TString, std::map<TString, TH1*> > mDrawHists;
  std::map<TString, TLegend*> mLegend;

  for(std::map<TString, sSingleVar>::iterator it = mSingleTrainingVar.begin(); it != mSingleTrainingVar.end(); it++) {
    CanvasHelper ch(it->second.canvas_title + sample_name);
    mCanvas[it->first] = ch.initRatioCanvas(it->second.xmin,it->second.xmax,
                                            it->second.ymin,it->second.ymax,
                                            it->second.rmin,it->second.rmax,
                                            it->second.xaxis_title,
                                            it->second.yaxis_title,
                                            it->second.ratio_title);

    mDrawHists[it->first] = single_figure_compare_mc_data(ch,mSample,vSuffix,sample_name,data_sample_name,it->second.hist_name,scale_data);
    ch.DrawCMSPreliminary(true,it->second.cms_alignment,"3.86 #mub^{-1} (13 TeV)");
  }

  //////////////////////////////////////////////////////////////////////////LEGENDStack0
  // Draw Legends for different Hists
  draw_legend_for_diff_canvas(mCanvas,mDrawHists,mLegend,vSuffix,sample_name,data_sample_name);
/////////////////////////////////////////////////////////////////////////////LEGENDStack0
  // add different MC samples to compare
  vector<TString> sample_name_diffMC;
  sample_name_diffMC.push_back("Pythia8");
  // sample_name_diffMC.push_back("EPOS");
  // draw_legend_for_diff_canvas_diffMC(mCanvas,mDrawHists,mLegend,sample_name_diffMC,sample_name,data_sample_name); // add different MC samples0
}





//////////////////////////////////////////////////////////////////////////
std::map<TString, TH1*>
single_figure_compare_mc_data(CanvasHelper& ch,
                              std::map<TString, SampleList::sSample>& mSample,
                              std::vector<TString>& vSuffix,
                              TString sample_name,
                              TString data_sample_name,
                              TString hist_var_name,
                              // TLegend* leg,
                              bool scale_data) 
{
  //////////////////////////////////////////////////////////////////////////
  // hist pointer container to return pointer to drawn hists
  std::map<TString, TH1*> mDrawHists;

  double lumi_mc   = mSample[sample_name].lumi;
  double lumi_data = mSample[data_sample_name].lumi;

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

  TH1F* hMC = (TH1F*)mSample[sample_name].file->Get(mSample[sample_name].tree_name + "/" + hist_var_name);
  TH1F* hData = (TH1F*)mSample[data_sample_name].file->Get(mSample[data_sample_name].tree_name + "/" + hist_var_name);

  // lumi_mc = hMC->GetEntries();
  // lumi_data = hData->GetEntries();
  

  //////////////////////////////////////////////////////////////////////////
  // color code for the different hists
  Color_t col[5] = {29, kGreen, kGreen+1, kYellow+1, 24}; //stacked hists1
  // Color_t col_diffMC[5] = {29, kGreen, kGreen+1, kYellow+1, 24};comparision MCs1

  /////////////////////////////////////////////////////////////i/////////////stacked hists2
  // access the stacked hists and add it to the canvas helper
  for(unsigned int iHist=0; iHist<shh.getHistSize(); iHist++) {
    shh.getHist(iHist)->Scale( 1/lumi_mc, "width" );
    ch.addHist( shh.getHist(iHist), "HIST", col[iHist], kSolid, 20, 1001 );

    TString hist_text = vSuffix[shh.getHistSize()-iHist-1];
    mDrawHists[hist_text] = shh.getHist(iHist);
  }

  //////////////////////////////////////////////////////////////////////stacked hists2
 
  hMC->Scale( 1/lumi_mc, "width" );
  ch.addHist( hMC, "HIST", kBlue+2);
  // mDrawHists[sample_name] = hMC;



  // ///////////////////////////////////////////////////////////// comparision MCs2
  // // add different MC samples to compare
  // vector<TString> sample_name_diffMC;
  // sample_name_diffMC.push_back("Pythia8");
  // // sample_name_diffMC.push_back("EPOS");
  // for(unsigned int iHist=0; iHist<sample_name_diffMC.size(); iHist++) {
  //   double lumi_data_diffMC = mSample[sample_name_diffMC[iHist]].lumi;
  //   mDrawHists[sample_name_diffMC[iHist]] = (TH1F*)mSample[sample_name_diffMC[iHist]].file->Get(mSample[sample_name_diffMC[iHist]].tree_name + "/" + hist_var_name);
  //   // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  //   #warning LUMI
  //   // mDrawHists[sample_name_diffMC[iHist]]->Scale( 1./lumi_data_diffMC, "width");
  //   // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  //   ch.addHist( mDrawHists[sample_name_diffMC[iHist]], "HIST", col_diffMC[iHist] );

  //   ch.addRatioHist( get_Ratio(mDrawHists[sample_name_diffMC[iHist]],hData), "HIST", col_diffMC[iHist] );
  // }
  // //////////////////////////////////////////////////////////////////////// comparision MCs2


  //////////////////////////////////////////////////////////////////////////
  // access data hist
  if(scale_data) hData->Scale( 1/lumi_data , "width" );
  ch.addDataHist( hData );
  mDrawHists[data_sample_name] = hData;

  ch.addRatioHist( get_Ratio(hMC,hData), "HIST", kBlue+2 );
  ch.addRatioHist( get_Ratio(hData,hData), "EP", kBlack );

  // draw stacked hists
  ch.DrawHist();

  // draw legend
  // ch.getCanvas()->cd(1);
  // leg->Draw("same");

  return mDrawHists;
}





//////////////////////////////////////////////////////////////////////////
void discriminant_compare_mc_data(std::map<TString, SampleList::sSample>& mSample)
{

  std::vector<TString> vSuffix;
  // vSuffix.push_back("_NONE");
  vSuffix.push_back("_DD");
  vSuffix.push_back("_Rest");
  vSuffix.push_back("_SD2"); 
  
  vSuffix.push_back("_SD1");//Signal:DD,SD you need to switch
  
  TString mc_sample_name = "XiCutEPOSSD1";
  TString data_sample_name = "Data";
  TString sSignal ="SD1";


  TString training_sample_name = "XiCutEPOSSD1";
  TString training_method = "BDTG";

  TString hist_name = TString("hDisciminant_") + training_sample_name + "_" + training_method;

  // TString mc_sample_name = "EPOSSD1_BDTG_EPOSSD1Trained";
  // TString data_sample_name = "DataSD1_BDTG_EPOSSD1Trained";

  TString data_dir = "data";
  TFile* mc_file = TFile::Open(data_dir + "/" + mSample[mc_sample_name].app_output_file_name);
  TFile* data_file = TFile::Open(data_dir + "/" + mSample[data_sample_name].app_output_file_name);


  TLegend * leg = new TLegend(0.5,0.7,0.9,0.9);
  //////////////////////////////////////////////////////////////////////////
  // stack the hists together
  StackHistHelper shh;
  try {
    shh.addHistFromFileWithSuffix(mc_file,hist_name,vSuffix,
                                  mc_sample_name + "_Stack");
  } catch(TString err) {
    std::cout << "In function discriminant_compare_mc_data:" << std::endl;
    std::cout << "!!! Sample: " << mc_sample_name 
              << " has no hist: " << err << std::endl;
  }


  /////////////////////////////////////////////////////////////
  // init canvas helper
  CanvasHelper ch("cDiscriminant" + mc_sample_name + training_method);
  ch.initRatioCanvas(-1,1,
                     // 0,3e5,
                     1e3,1e6,
                     0,3,
                     "Classifier X",
                     // "1/N_{evt} dN/dX",
                     "1/L dN/dX [#mub]",
                     "MC / Data");

  /////////////////////////////////////////////////////////////
  // get lumi for mc sample
  double lumi_mc = mSample[mc_sample_name].lumi;
  // double events_mc = ((TH1F*)mc_file->Get("hNentries"))->GetBinContent(1);

  //////////////////////////////////////////////////////////////////////////
  // color code for the different hists
  // Color_t col[5] = {29, kGreen-1, kGreen, kYellow, 24};
  Color_t BkgColor = kGreen-1;
  Color_t col[5] = {kYellow, BkgColor, BkgColor, BkgColor, BkgColor};

  ////////////////////////
  /////////////////////////////////////////////////////////////i/////////////
  // access the stacked hists and add it to the canvas helper
  for(unsigned int iHist=0; iHist<shh.getHistSize(); iHist++) {
    // shh.getHist(iHist)->Scale( 1/events_mc, "width" );
    shh.getHist(iHist)->Scale( 1/lumi_mc, "width" );
    ch.addHist( shh.getHist(iHist), "HIST", col[iHist], kSolid, 20, 1001 );

    // define legend text for stacked hist
    TString leg_text = vSuffix[shh.getHistSize()-iHist-1];
    leg_text.Remove(0,1);
    // add legend entry
    // leg->AddEntry( shh.getHist(iHist), leg_text, "f" );
    if( leg_text == "Rest" ) {
      leg_text = "Background";
      leg->AddEntry( shh.getHist(iHist), leg_text, "f" );
    } else if( leg_text== sSignal ) {
      leg->AddEntry( shh.getHist(iHist), leg_text, "f" );
    }
  }

  TH1F* hMC = (TH1F*)mc_file->Get(hist_name);
  if (!hMC) {
    cout << "cannot read " << hist_name << " from file " << mc_file->GetName() << endl;
    exit(1);
  }
  // hMC->Scale( 1./events_mc, "width" );
  hMC->Scale( 1./lumi_mc, "width" );
  ch.addHist( hMC, "HIST", kBlue+2 );
  leg->AddEntry( hMC, mSample[mc_sample_name].legend_name , "l");

  //////////////////////////////////////////////////////////////////////////
  // access data hist
  TH1F* hData = (TH1F*)data_file->Get(hist_name);
  if (!hData) {
    cout << "cannot read " << hist_name << " from file " << data_file->GetName() << endl;
    exit(1);
  }

  //////////////////////////////////////////////////////////////////////////
  // get data lumi
  double lumi_data = mSample[data_sample_name].lumi;
  // double events_data = ((TH1F*)data_file->Get("hNentries"))->GetBinContent(1);
  // as a hack
  // hData->Sumw2();
  // hData->Scale( 1./events_data , "width" );
  hData->Scale( 1./lumi_data , "width" );



  ch.addDataHist( hData );
  // add legend entry
  leg->AddEntry( hData, mSample[data_sample_name].legend_name , "lep" );

  ch.addRatioHist( get_Ratio(hMC,hData), "HIST", kBlue );
  ch.addRatioHist( get_Ratio(hData,hData), "EP", kBlack );

  // draw stacked hists
  ch.DrawHist();

  // draw legend
  ch.getCanvas()->cd(1);
  leg->Draw("same");
  if(draw_figure)ch.getCanvas()->Print( figure_dir + "/hDisciminant_" + training_sample_name + "." + figure_type );
  ch.DrawCMSPreliminary(true,11,"0.34 #mub^{-1} (13 TeV)");
}


void discriminant_results(std::map<TString, SampleList::sSample>& mSample)
{

  std::vector<TString> vSuffix;
  // vSuffix.push_back("_NONE");

  vSuffix.push_back("_SD1");
  vSuffix.push_back("_SD2");
  vSuffix.push_back("_DD");
  vSuffix.push_back("_Rest");

  TString mc_sample_name = "XiCutEPOSSD1";
  TString data_sample_name = "Data";

  TString training_sample_name = "XiCutEPOSSD1";
  TString training_method = "BDTG";

  TString hist_name = TString("hDisciminant_") + training_sample_name + "_" + training_method;


  TString data_dir = "data";
  TFile* mc_file = TFile::Open(data_dir + "/" + mSample[mc_sample_name].app_output_file_name);

  TH1F* hMC   = (TH1F*)mc_file->Get(hist_name);

  TString sSignal ="SD1";
  TH1F* hsig = (TH1F*)mc_file->Get(hist_name+"_"+sSignal);

  std::map<TString,TH1F*> hbkg;
  for(unsigned int idx=0; idx<vSuffix.size(); idx++) {
    if( vSuffix[idx].Copy().Remove(0,1) == sSignal ) continue;
    hbkg[ vSuffix[idx].Copy().Remove(0,1) ] = (TH1F*)mc_file->Get(hist_name+vSuffix[idx]);
  }
  
  // std::cout << "file name = " << data_dir + "/" + mSample[mc_sample_name].app_output_file_name << std::endl;
  // std::cout << "hist_name = " << hist_name+"_"+sSignal << std::endl;

  CanvasHelper chROCurve("ROC");
  TCanvas * cROC = chROCurve.initNormalCanvas(0,1.2,0,1.4,"signal efficiency","background rejection",510,510);
  cROC->SetGrid();

  const Int_t n = 40;
  TGraph *gr = new TGraph(n);
  TGraph *gr_mark_point = new TGraph(1);
  TGraph *gr_dist_aim_point = new TGraph(n);
  TGraph *gr_prob_sig = new TGraph(n);
  TGraph *gr_prob_bkg = new TGraph(n);
  TGraph *gr_prob_sig_bkg = new TGraph(n);
  TGraph *gr_SdivSqrtSplusB = new TGraph(n);
  TGraph *gr_sig_purity_tims_eff = new TGraph(n);



  // TEfficiency* pEff = new TEfficiency("eff",";efficiency;purityXout",40,0,2);
  TCanvas* c2 = new TCanvas("example","",600,400);
  // c2->SetFillStyle(1001);
  // c2->SetFillColor(kWhite);


  CanvasHelper chSdivSqrtSplusB("SdivSqrtSplusB");
  TCanvas * cSdivSqrtSplusB = chSdivSqrtSplusB.initNormalCanvas(-1,1,0,1,"classifier X","sig. purtiy times sig. eff.");
  // cSdivSqrtSplusB->SetLogy();
  cSdivSqrtSplusB->SetGrid();

  // define my ''point closest to'' in the ROC curve
  double aim_eff = 1;
  double aim_bkg = 1;
  double dist_aim_minimum = 1e99;
  int iCutVal_minimum = -1;

  double value_to_maximize = -1000;
  int iCutVal_value_to_maximize = -1;

  // loop over discrimant cut values
  for( int iCutVal = 40; iCutVal>=0; iCutVal-- ) {
  
    double discriminant_cut_value = hMC->GetBinLowEdge(iCutVal);

    double Entry_sig= 0; 
    double Entry_sig_all = 0;
    double Entry_bkg = 0;
    double Entry_bkg_all = 0;
    double efficiencyXout=0; 
    double purityXout=0;
    double prob_sig=0;
    double prob_bkg=0;
    double SdivSqrtSplusB=0;

    double value_of_interest=-1000;


    for ( int iBin = 41; iBin>=0; iBin-- ) {
      
      Entry_sig_all += hsig->GetBinContent(iBin);
      for(std::map<TString,TH1F*>::iterator it=hbkg.begin(); it!=hbkg.end(); it++)
        Entry_bkg_all += it->second->GetBinContent(iBin);

      if (hsig->GetBinCenter(iBin) > discriminant_cut_value) {
        Entry_sig += hsig->GetBinContent(iBin);
        for(std::map<TString,TH1F*>::iterator it=hbkg.begin(); it!=hbkg.end(); it++)
          Entry_bkg += it->second->GetBinContent(iBin);        
      }
    }

    if (Entry_sig == 0 || Entry_sig_all == 0 || Entry_bkg ==0 || Entry_bkg_all == 0 ) continue;
    efficiencyXout = Entry_sig / Entry_sig_all;
    purityXout = 1 - (Entry_bkg / Entry_bkg_all);
    if( Entry_sig + Entry_bkg <= 0 ) {
      prob_sig = 0;
      prob_bkg = 0;
      SdivSqrtSplusB = 0;
    } else {
      prob_sig = Entry_sig / (Entry_bkg + Entry_sig);
      prob_bkg = Entry_bkg / (Entry_bkg + Entry_sig);
      SdivSqrtSplusB = Entry_sig / sqrt( Entry_sig + Entry_bkg );
    }

    // std::cout << "Discriminat Cut Value = " << discriminant_cut_value << std::endl;
    // std::cout << "--- Signal Eff = " << efficiencyXout << std::endl;
    // std::cout << "--- Bkg Eff    = " << purityXout << std::endl;

    gr->SetPoint(iCutVal,efficiencyXout,purityXout);
    gr_prob_sig_bkg->SetPoint(iCutVal,prob_sig,prob_bkg);

    // distance to my ''point closest to'' in the ROC curve
    double dist_aim_point = sqrt( 
                                  (efficiencyXout-aim_eff)*(efficiencyXout-aim_eff) +
                                  (purityXout-aim_bkg)*(purityXout-aim_bkg)
                                );

    if( dist_aim_point < dist_aim_minimum ) {
      dist_aim_minimum = dist_aim_point;
      iCutVal_minimum = iCutVal;
    }

    gr_dist_aim_point->SetPoint(iCutVal,discriminant_cut_value,dist_aim_point);
    gr_prob_sig->SetPoint(iCutVal,discriminant_cut_value,prob_sig);
    gr_prob_bkg->SetPoint(iCutVal,discriminant_cut_value,prob_bkg);

    gr_mark_point->SetPoint(0,gr->GetX()[iCutVal_minimum],gr->GetY()[iCutVal_minimum]);

    gr_SdivSqrtSplusB->SetPoint(iCutVal,discriminant_cut_value,SdivSqrtSplusB);
    gr_sig_purity_tims_eff->SetPoint(iCutVal,discriminant_cut_value,prob_sig/efficiencyXout);



    value_of_interest = prob_sig*efficiencyXout;

    if(value_of_interest >= value_to_maximize) {
      value_to_maximize = value_of_interest;
      iCutVal_value_to_maximize = iCutVal;
    }

  } // loop over discrimant cut values

  std::cout << "!!! Best Cut is at Discriminant Value = " << gr_dist_aim_point->GetX()[iCutVal_minimum] << std::endl;
  std::cout << "!!!                          hist bin = " << iCutVal_minimum << std::endl;
  std::cout << "!!! --- with an efficiency of       = " << gr->GetX()[iCutVal_minimum] << std::endl;
  std::cout << "!!! --- and background rejection of = " << gr->GetY()[iCutVal_minimum] << std::endl;
  std::cout << "!!! --- signal probability in selected reagion = " << gr_prob_sig->GetY()[iCutVal_minimum] << std::endl;

  std::cout << "=================================================================" << std::endl;
  std::cout << "!!! Best Cut by \"Sig_purity x Sig_eff\" at Discriminant Value = " << gr_sig_purity_tims_eff->GetX()[iCutVal_value_to_maximize] << std::endl;
  std::cout << "!!! --- with an efficiency of       = " << gr->GetX()[iCutVal_value_to_maximize] << std::endl;
  std::cout << "!!! --- and background rejection of = " << gr->GetY()[iCutVal_value_to_maximize] << std::endl;
  std::cout << "!!! --- signal probability in selected reagion = " << gr_prob_sig->GetY()[iCutVal_value_to_maximize] << std::endl;

  chROCurve.SetUpHist((TH1*)gr,kRed);
  chROCurve.SetUpHist((TH1*)gr_mark_point,kBlack);
  gr_mark_point->SetMarkerSize(1.7);
  cROC->cd();
  // gr_mark_point->Draw("P same");
  gr->Draw("PL same");
  chROCurve.DrawCMSSimulation();

  TLatex tl;
  char buf[128];
  sprintf(buf,"sig. eff = %3.1f",gr->GetX()[iCutVal_minimum]*100);
  tl.DrawLatex(0.5,0.5,buf);
  sprintf(buf,"bckg rejection = %3.1f",gr->GetY()[iCutVal_minimum]*100);
  tl.DrawLatex(0.5,0.45,buf);


  CanvasHelper::SetUpHist((TH1*)gr_prob_sig_bkg,kBlue);
  // gr_prob_sig_bkg->Draw("P same");

  c2->cd();
  gr_dist_aim_point->SetLineColor(4);
  gr_dist_aim_point->SetLineWidth(4);
  gr_dist_aim_point->SetMarkerColor(4);
  gr_dist_aim_point->SetMarkerStyle(21);
  gr_dist_aim_point->GetXaxis()->SetTitle("discriminant_cut_value");
  gr_dist_aim_point->GetYaxis()->SetTitle("distance to eff=1;bkg=0");
  TLegend * leg = new TLegend(0.3,0.7,0.7,0.9);
  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->SetTextFont(23);
  leg->SetTextSize(20);
  
  leg->AddEntry(gr_dist_aim_point,"distance to eff=1;bkg=0","lep");
 
  gr_dist_aim_point->Draw("ALP");
  
  leg->Draw("same");

  


  

  cSdivSqrtSplusB->cd();
  chSdivSqrtSplusB.SetUpHist((TH1*)gr_SdivSqrtSplusB,kGreen+2);
  chSdivSqrtSplusB.SetUpHist((TH1*)gr_sig_purity_tims_eff,kBlue);
  CanvasHelper::SetUpHist((TH1*)gr_prob_sig,kRed);
  CanvasHelper::SetUpHist((TH1*)gr_prob_bkg,kMagenta);
  TLegend * legSdivSqrtSplusB = new TLegend(0.48,0.7,0.88,0.9);
  legSdivSqrtSplusB->SetFillColor(0);
  legSdivSqrtSplusB->SetBorderSize(0);
  legSdivSqrtSplusB->SetFillStyle(0);
  legSdivSqrtSplusB->SetTextFont(23);
  legSdivSqrtSplusB->SetTextSize(20);



  legSdivSqrtSplusB->AddEntry(gr_prob_sig,"Sig probability","lep");
  legSdivSqrtSplusB->AddEntry(gr_prob_bkg,"background probability","lep");
  legSdivSqrtSplusB->AddEntry(gr_sig_purity_tims_eff,"purity_tims_eff","lep");
 
 
//  gr_SdivSqrtSplusB->Draw("APL");
  gr_sig_purity_tims_eff->Draw("APL");
  gr_prob_sig->Draw("P");
  gr_prob_bkg->Draw("P");
  // gr_mark_point->Draw("P same");
  chSdivSqrtSplusB.DrawCMSSimulation();
  legSdivSqrtSplusB->Draw("");

}  
  

void calc_signal_cross_section(double& result_cross_section, double& standartError,
                               std::map<TString, SampleList::sSample>& mSample,
                               const TString& sSignal,
                               double classifier_cut)
{
  std::vector<TString> vSuffix;
  // vSuffix.push_back("_NONE");

  vSuffix.push_back("_SD1");
  vSuffix.push_back("_SD2");
  vSuffix.push_back("_DD");
  vSuffix.push_back("_Rest");

  TString mc_sample_name = "XiCutEPOSSD1";
  TString data_sample_name = "Data";

  TString training_sample_name = "XiCutEPOSSD1";
  TString training_method = "BDTG";

  TString hist_name = TString("hDisciminant_") + training_sample_name + "_" + training_method;

  //////////////////////////////////////////////////////////////////////////
  // get data lumi
  double lumi_data = mSample[data_sample_name].lumi;

  TString data_dir = "data";
  TFile* mc_file = TFile::Open(data_dir + "/" + mSample[mc_sample_name].app_output_file_name);
  TFile* data_file = TFile::Open(data_dir + "/" + mSample[data_sample_name].app_output_file_name);

  // TH1F* hMC   = (TH1F*)mc_file->Get(hist_name);
  TH1F* hData = (TH1F*)data_file->Get(hist_name);
  TH1F* hsig = (TH1F*)mc_file->Get(hist_name+"_"+sSignal);

  std::map<TString,TH1F*> hbkg;
  for(unsigned int idx=0; idx<vSuffix.size(); idx++) {
    TString testName = vSuffix[idx].Copy().Remove(0,1); 
    if( testName == sSignal ) continue;
    hbkg[ testName ] = (TH1F*)mc_file->Get(hist_name+vSuffix[idx]);
    std::cout << "Background selected: " << testName  << std::endl;
  }

  // std::cout << "file name = " << data_dir + "/" + mSample[mc_sample_name].app_output_file_name << std::endl;
  // std::cout << "hist_name = " << hist_name+"_"+sSignal << std::endl;

  //////////////////////////////////////////////////////////////////////////
  // value to cut in classifier/discriminant output distribution
  double Entry_sig= 0; 
  double Entry_sig_all = 0;
  double Entry_bkg = 0;
  double Entry_bkg_all = 0;
  double Entry_data = 0;
  
  // loop over discrimant cut values
  for( int iBin = 41; iBin>=0; iBin-- ) {
    double classifier_value = hData->GetBinCenter(iBin);

    Entry_sig_all += hsig->GetBinContent(iBin);
    for(std::map<TString,TH1F*>::iterator it=hbkg.begin(); it!=hbkg.end(); it++)
      Entry_bkg_all += it->second->GetBinContent(iBin);

    if(classifier_value > classifier_cut) {
      Entry_sig += hsig->GetBinContent(iBin);
      for(std::map<TString,TH1F*>::iterator it=hbkg.begin(); it!=hbkg.end(); it++)
        Entry_bkg += it->second->GetBinContent(iBin);

      Entry_data += hData->GetBinContent(iBin);
    }

    // std::cout << "classifier_value = " << classifier_value << std::endl;
    // std::cout << "Entry_sig_all = " << Entry_sig_all << std::endl;
    // std::cout << "Entry_sig = " << Entry_sig << std::endl;
    // std::cout << "Entry_bkg = " << Entry_bkg << std::endl;
    // std::cout << "Entry_data = " << Entry_data << std::endl;
  }
  
  
  


  TString histoname = "NentriesWithEventSelectionCuts";
  TH1F* hentry_mc = (TH1F*)mSample[mc_sample_name].file->Get(mSample[mc_sample_name].tree_name + "/" + histoname);
  TH1F* hentry_data = (TH1F*)mSample[data_sample_name].file->Get(mSample[data_sample_name].tree_name + "/" + histoname);

  std::cout << hentry_mc << std::endl;

  double Allevents_mc = hentry_mc->GetBinContent(hentry_mc->GetXaxis()->FindBin("all"));
  
  double Allevents_data = hentry_data->GetBinContent(hentry_data->GetXaxis()->FindBin("run"));
 
  double eventswithvrtzcut_data = hentry_data->GetBinContent(hentry_data->GetXaxis()->FindBin("vtxcut") );
  
  double eventswithvrtzcut_mc = hentry_mc->GetBinContent(hentry_mc->GetXaxis()->FindBin("mc_vtxcut") );
  
  double Fsplit = 1 - (eventswithvrtzcut_mc / Allevents_mc);
  
  std::cout << "Fsplit: " << Fsplit << std::endl;
  
  double eff_PU_data = Allevents_data / eventswithvrtzcut_data;
  std::cout << "eff_PU_data: " << eff_PU_data << std::endl;



  double prob_sig = Entry_sig / (Entry_bkg + Entry_sig);
  
  double eff_sig = (Entry_sig * Fsplit )/ Entry_sig_all; //MC signal efficiency

  double eff_number_of_signal_events_in_data = (Entry_data * prob_sig)/eff_sig; //effictive lumi data

  result_cross_section = eff_number_of_signal_events_in_data/(lumi_data/eff_PU_data);
  standartError = std::sqrt(Entry_data) * prob_sig / (lumi_data/eff_PU_data) / eff_sig ;

  std::cout << std::endl;
  std::cout << std::endl;
  std::cout << "======================================================================" << std::endl;
  std::cout << "======================================================================" << std::endl;
  std::cout << "Took data sampe: " << data_sample_name << std::endl; 
  std::cout << " with   LUMI = " << lumi_data << " 1/#mub" <<std::endl;
  std::cout << " with SIGNAL = " << sSignal << std::endl;
  std::cout << "======================================================================" << std::endl;
  std::cout << "cut at classifier value = " << classifier_cut << std::endl;
  std::cout << "--- signal efficiency = " << eff_sig << std::endl;
  std::cout << "--- signal probability = " << prob_sig << std::endl;
  std::cout << "--- number of selected data events = " << Entry_data << std::endl;
  std::cout << "--- !!! Effective Number of Signal Events in Data = " << eff_number_of_signal_events_in_data << " !!! ---" << std::endl;
  std::cout << "--- !!! Standart Error = " << standartError << " !!! ---" << std::endl;
  std::cout << "--- !!! Effective Signal CROSS SECTION = " << result_cross_section << " ub !!! ---" << std::endl;
  std::cout << "======================================================================" << std::endl;
  std::cout << std::endl;
  std::cout << std::endl;
}


void single_sample_compare_syst(std::map<TString, SampleList::sSample>& mSample,
                                std::map<TString, sSingleVar>& mSingleTrainingVar,
                                TString sample_name,
                                TString data_sample_name)
{
  CanvasHelper ch("dEta_Syst_HFescale_" + sample_name);
  ch.initRatioCanvas(mSingleTrainingVar["DeltaEta"].xmin, mSingleTrainingVar["DeltaEta"].xmax,
                     mSingleTrainingVar["DeltaEta"].ymin, mSingleTrainingVar["DeltaEta"].ymax,
                     mSingleTrainingVar["DeltaEta"].rmin, mSingleTrainingVar["DeltaEta"].rmax,
                     mSingleTrainingVar["DeltaEta"].xaxis_title,
                     mSingleTrainingVar["DeltaEta"].yaxis_title,
                     mSingleTrainingVar["DeltaEta"].ratio_title);

  TString hist_var_name = mSingleTrainingVar["DeltaEta"].hist_name;

  TH1F* hMC = (TH1F*)mSample[sample_name].file->Get(mSample[sample_name].tree_name + "/" + hist_var_name);
  TH1F* hData = (TH1F*)mSample[data_sample_name].file->Get(mSample[data_sample_name].tree_name + "/" + hist_var_name);
  

  double lumi_mc   = mSample[sample_name].lumi;
  double lumi_data = mSample[data_sample_name].lumi;

  //////////////////////////////////////////////////////////////////////////
  // acces hist with systematics
  TString HFPlus_sample_name = "Data_sysHFPlus";
  TH1F* hData_HFPlus = (TH1F*)mSample[HFPlus_sample_name].file->Get(mSample[HFPlus_sample_name].tree_name + "/" + hist_var_name);
  hData_HFPlus->Scale( 1/lumi_data , "width" );

  //////////////////////////////////////////////////////////////////////////
  // create TGraphError
  hData->Scale( 1/lumi_data , "width" );
  TGraphAsymmErrors* grData_HF = new TGraphAsymmErrors(hData);
  grData_HF->SetFillColor(kYellow+2);

  for(int iBin=1; iBin<=hData->GetXaxis()->GetNbins(); iBin++) {
    double ydiff = hData_HFPlus->GetBinContent(iBin) - hData->GetBinContent(iBin);
    double yhigh_err =  ydiff > 0 ? ydiff : 0;
    double ylow_err  =  ydiff > 0 ? 0 : -ydiff;

    double xhigh_err = hData->GetBinLowEdge(iBin+1) - hData->GetBinCenter(iBin);
    double xlow_err = hData->GetBinCenter(iBin) - hData->GetBinLowEdge(iBin);

    grData_HF->SetPointEYhigh(iBin-1, yhigh_err);
    grData_HF->SetPointEYlow(iBin-1, ylow_err);
    grData_HF->SetPointEXhigh(iBin-1, xhigh_err);
    grData_HF->SetPointEXlow(iBin-1, xlow_err);
  }


  TH1F* hData_HFPlus_Ratio = get_Ratio(hData_HFPlus,hData);
  TGraphAsymmErrors* grData_HF_Ratio = new TGraphAsymmErrors(hData_HFPlus_Ratio);
  grData_HF_Ratio->SetFillColor(kYellow+2);

  for(int iBin=1; iBin<=hData->GetXaxis()->GetNbins(); iBin++) {
    cout << "Bin Center: " << hData->GetBinCenter(iBin) << endl;
    cout << "hData: " << hData->GetBinContent(iBin) << endl;
    cout << "hData_HFPlus: " << hData_HFPlus->GetBinContent(iBin) << endl;
    cout << "hData_HFPlus_Ratio: " << hData_HFPlus_Ratio->GetBinContent(iBin) << endl;

    double yvalue = 1;
    double xvalue = hData_HFPlus_Ratio->GetBinCenter(iBin);

    double ydiff = hData->GetBinContent(iBin) > 0 ? hData_HFPlus_Ratio->GetBinContent(iBin)-1 : 0;
    double yhigh_err =  ydiff > 0 ? ydiff : 0;
    double ylow_err  =  ydiff > 0 ? 0 : -ydiff;

    cout << "yhigh_err: " << yhigh_err << endl;
    cout << "ylow_err: " << ylow_err << endl;

    double xhigh_err = hData->GetBinLowEdge(iBin+1) - hData->GetBinCenter(iBin);
    double xlow_err = hData->GetBinCenter(iBin) - hData->GetBinLowEdge(iBin);

    grData_HF_Ratio->SetPoint(iBin-1, xvalue, yvalue);

    grData_HF_Ratio->SetPointEYhigh(iBin-1, yhigh_err);
    grData_HF_Ratio->SetPointEYlow(iBin-1, ylow_err);
    grData_HF_Ratio->SetPointEXhigh(iBin-1, xhigh_err);
    grData_HF_Ratio->SetPointEXlow(iBin-1, xlow_err);
  }  

  ch.getCanvas()->cd(1);
  grData_HF->Draw("same 2");

  ch.getCanvas()->cd(2);
  grData_HF_Ratio->Draw("same 2");



  hMC->Scale( 1/lumi_mc, "width" );
  ch.addHist( hMC, "HIST", kBlue+2);
  // mDrawHists[sample_name] = hMC;

  //////////////////////////////////////////////////////////////////////////
  // access data hist
  ch.addDataHist( hData );

  ch.addRatioHist( get_Ratio(hMC,hData), "HIST", kBlue+2 );
  ch.addRatioHist( get_Ratio(hData,hData), "EP", kBlack );

  // draw stacked hists
  ch.DrawHist();

  




  // draw legend
  // ch.getCanvas()->cd(1);
  // leg->Draw("same");
}