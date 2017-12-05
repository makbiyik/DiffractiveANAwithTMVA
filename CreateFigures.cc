#include <iostream>
#include <cmath>
#include <cstdio>
#include <algorithm>
#include <map>
#include <sstream>

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
#include "MyHistograms.h"
#include "SampleList.h"

using namespace std;

#define UNUSED(x) (void)(x) // to avoid unused compiler warning

const TString figure_dir = "AN_figures";
const TString figure_type = "png";
// const TString figure_type = "gif";
const bool draw_figure = true;


// temporary struct for training variables directly compared to data
// struct sSingleVar
// {
//   TString hist_name;
//   TString xaxis_title;
//   TString yaxis_title;
//   TString ratio_title;
//   TString canvas_title;

//   double xmin, xmax;
//   double ymin, ymax;
//   double rmin, rmax;

//   int cms_alignment;
// };



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

std::map<TString, TH1*> single_figure_addstack(CanvasHelper& ch,
                              std::map<TString, SampleList::sSample>& mSample,
                              std::vector<TString>& vSuffix,
                              TString sample_name,
                              TString hist_var_name) ;


void single_sample_compare_mc_data(std::map<TString, SampleList::sSample>& mSample,
                                   std::vector<TString>& vSuffix,
                                   std::map<TString, sSingleVar>& mSingleTrainingVar,
                                  vector<TString> sample_names,
                                   TString data_sample_name,
                                   bool scale_data = true);



void single_sample_compare_syst(std::map<TString, SampleList::sSample>& mSample,
                                std::map<TString, sSingleVar>& mSingleTrainingVar,
                                TString sample_name,
                                TString data_sample_name);
std::map<TString, TH1*> single_figure_compare_mc_data(CanvasHelper& ch,
                                                      std::map<TString, SampleList::sSample>& mSample,
                                                      std::vector<TString>& vSuffix,
                              vector<TString> sample_names,
                                                      TString data_sample_name,
                                                      TString hist_var_name,
                                                      // TLegend* leg,
                                                      bool scale_data = true);
void draw_legend_for_diff_canvas(std::map<TString, TCanvas*>& mCanvas,
                                 std::map<TString, std::map<TString, TH1*> >& mDrawHists,
                                 std::map<TString, TLegend*>& mLegend,
                                 std::vector<TString>& vSuffix,
                              vector<TString> sample_names,
                                 TString data_sample_name);





//////////////////////////////////////////////////////////////////////////
// plot discriminant value for different proccess
void discriminant_compare_mc_data(std::map<TString, SampleList::sSample>& mSample);

//////////////////////////////////////////////////////////////////////////
// plot discriminant value for different proccess
void discriminant_compare_data_syst(std::map<TString, SampleList::sSample>& mSample);


//////////////////////////////////////////////////////////////////////////
// find optimal cut value on discriminant distribution
void discriminant_results(std::map<TString, SampleList::sSample>& mSample);


void calc_signal_cross_section(double& result_cross_section, double& totalcrossectionError,
                               std::map<TString, SampleList::sSample>& mSample,
                               const TString& sSignal,
                               double classifier_cut);


//void calc_error_propagation(double x, double y);
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
  // // compare training variables MC with DATA
  training_variables_compare_mc_data(mSample);
  // discriminant_compare_mc_data(mSample);
  // discriminant_compare_data_syst(mSample);
  // discriminant_results(mSample);




  //////////////////////////////////////////////////////////////////////////
  double cross_section_result = 0;
  double totalcrossectionError = 0;

  TCanvas* ctest_clcut = new TCanvas("ctest_clcut");
  ctest_clcut->cd();
  TH1F* htest_clcut = new TH1F("htest_clcut","",21,-1.05,1.05);
  TLegend* leg = new TLegend(0.1,0.83,0.5,0.92);
  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->SetTextFont(42);
  leg->SetTextSize(0.05);
  


/////////////////////////////////////////////////////////////////////////////////////////
  for(double clcut = -1.0; clcut < 0.99; clcut += 0.1) {
    calc_signal_cross_section(cross_section_result, totalcrossectionError,
                              mSample, "SD2", clcut); ////Burasi cok onemli degistir !!
     #warning Signal:?
    htest_clcut->Fill(clcut, cross_section_result);
    htest_clcut->SetBinError( htest_clcut->FindBin(clcut), totalcrossectionError );
  }
   
  htest_clcut->GetXaxis()->SetTitle("Classifier Threshold");
  htest_clcut->GetYaxis()->SetTitle("#sigma [mb]");
  htest_clcut->GetYaxis()->SetTitleOffset(1.5);
  htest_clcut->SetMaximum(15000); // 600 single
  // htest_clcut->SetMaximum(6000); // 600 
  ctest_clcut->cd()->SetRightMargin(0.1);
  
  // htest_clcut->GetXaxis()->SetLabelOffset(1.5); // shift
  // htest_clcut->GetXaxis()->SetNdivisions(505); // divisions of labels
  // htest_clcut->GetXaxis()->SetRange(0,1);


  leg->AddEntry(htest_clcut,"CMS preliminery","");
  // leg->AddEntry(htest_clcut,"2015 pp; #sqrt{s} = 13 TeV","lep");
  htest_clcut->Draw();
  
  leg->Draw();
  // htest_clcut->Write(figure_dir + "/hThreshold" + "." + figure_type );  
  
  // freeze program
  app->Run();
  /////////////////////////////////////////////////

  return 0; 
}
//////////////////////////////////////////////////////////////////////////


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
  ch2.DrawCMSPreliminary(true,33,"3.86 #mub^{-1} (13 TeV)");
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

  vector<TString> sampleNames = {"XiCutPythia8"};
  // vector<TString> sampleNames = {"Pythia8","EPOS"};
  single_sample_compare_mc_data(mSample,vSuffix,mSingleTrainingVar,sampleNames,"Data_sebastian247934");
  // single_sample_compare_syst(mSample,mSingleTrainingVar,"EPOS","Data");
  // single_sample_compare_syst(mSample,mSingleTrainingVar,"EPOS","Data"); // !!!!!
   // !!!!! icant run with "single_sample_compare_mc_data" scale problemfor systematic
  




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
  mSingleTrainingVar["NTowMaxHFPlus"].hist_name = "Hist_eventXiID_MaxHFPlusEnergy"; 
  mSingleTrainingVar["NTowMaxHFMinus"].hist_name = "Hist_eventXiID_MaxHFMinusEnergy"; 
  mSingleTrainingVar["NTowMaxCastor"].hist_name = "Hist_eventXiID_MaxCastorEnergy"; 
  mSingleTrainingVar["SumEnergyHFPlus"].hist_name = "Hist_HFPlusSumEnergy"; 
  mSingleTrainingVar["SumEnergyHFMinus"].hist_name = "Hist_HFMinusSumEnergy"; 
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
  
  

  // vSuffix.clear(); burasi detector icin
  // vSuffix.push_back("_Barrel");
}




void draw_legend_for_diff_canvas(std::map<TString, TCanvas*>& mCanvas,
                                 std::map<TString, std::map<TString, TH1*> >& mDrawHists,
                                 std::map<TString, TLegend*>& mLegend,
                                 std::vector<TString>& vSuffix,
                                 vector<TString> sample_names,
                                 TString data_sample_name)
{
  gStyle->SetLegendTextSize(0.05); //nosuffix
  gStyle->SetLegendFont(42);

  {
    mCanvas["DeltaEta"]->cd(1);
    mLegend["DeltaEta"] = new TLegend( 0.19,0.47,0.59,0.87);
    for(unsigned int iSuffix=0; iSuffix<vSuffix.size(); iSuffix++) {
        // define legend text for stacked hist
        TString leg_text = vSuffix[iSuffix];
        leg_text.Remove(0,1);
        // add legend entry
       mLegend["DeltaEta"]->AddEntry( mDrawHists["DeltaEta"][ vSuffix[iSuffix] ], leg_text, "f" );
    }
    TString print = figure_dir + "/DeltaEta_";
    for (auto sample_name : sample_names) {
      mLegend["DeltaEta"]->AddEntry( mDrawHists["DeltaEta"][sample_name], sample_name, "l" );
      print += sample_name + "_";
    }
    mLegend["DeltaEta"]->AddEntry( mDrawHists["DeltaEta"][data_sample_name], "Data", "lep" );
    mLegend["DeltaEta"]->Draw("same");
    if(draw_figure) mCanvas["DeltaEta"]->Print(print +"." + figure_type );
  } 

   {
  ///
    mCanvas["ForwardEtaDelta"]->cd(1);
    mLegend["ForwardEtaDelta"] = new TLegend(0.19,0.47,0.59,0.87); // 0.19,0.47,0.59,0.87 fornosuffix , for suffix0.37,0.50,0.72,0.87
    for(unsigned int iSuffix=0; iSuffix<vSuffix.size(); iSuffix++) {
      // define legend text for stacked hist
      TString leg_text = vSuffix[iSuffix];
      leg_text.Remove(0,1);
      // add legend entry
      mLegend["ForwardEtaDelta"]->AddEntry( mDrawHists["ForwardEtaDelta"][ vSuffix[iSuffix] ], leg_text, "f" );
    }
    TString print = figure_dir + "/ForwardEtaDelta_";
    for (auto sample_name : sample_names) {
      mLegend["ForwardEtaDelta"]->AddEntry( mDrawHists["ForwardEtaDelta"][sample_name], sample_name, "l" );
      print += sample_name + "_";
    }  

    mLegend["ForwardEtaDelta"]->AddEntry( mDrawHists["ForwardEtaDelta"][data_sample_name], "Data", "lep" );
    mLegend["ForwardEtaDelta"]->Draw("same");
    if(draw_figure) mCanvas["ForwardEtaDelta"]->Print(print +"." + figure_type );

  }

  {  
    mCanvas["EtaDeltaZero"]->cd(1);
    mLegend["EtaDeltaZero"] = new TLegend(0.19,0.47,0.59,0.87);
    for(unsigned int iSuffix=0; iSuffix<vSuffix.size(); iSuffix++) {
      // define legend text for stacked hist
      TString leg_text = vSuffix[iSuffix];
      leg_text.Remove(0,1);
      // add legend entry
      mLegend["EtaDeltaZero"]->AddEntry( mDrawHists["EtaDeltaZero"][ vSuffix[iSuffix] ], leg_text, "f" );
    }
    
    TString print = figure_dir + "/EtaDeltaZero_";
    for (auto sample_name : sample_names) {
      mLegend["EtaDeltaZero"]->AddEntry( mDrawHists["EtaDeltaZero"][sample_name], sample_name, "l" );
      print += sample_name + "_"; 
    }

    mLegend["EtaDeltaZero"]->AddEntry( mDrawHists["EtaDeltaZero"][data_sample_name], "Data", "lep" );
    mLegend["EtaDeltaZero"]->Draw("same");
    if(draw_figure) mCanvas["EtaDeltaZero"]->Print(print +"." + figure_type );

  }

  {  
    mCanvas["EtaMin"]->cd(1);
    mLegend["EtaMin"] = new TLegend(0.19,0.47,0.59,0.87);
    for(unsigned int iSuffix=0; iSuffix<vSuffix.size(); iSuffix++) {
      // define legend text for stacked hist
      TString leg_text = vSuffix[iSuffix];
      leg_text.Remove(0,1);
      // add legend entry
      mLegend["EtaMin"]->AddEntry( mDrawHists["EtaMin"][ vSuffix[iSuffix] ], leg_text, "f" );
    }
    TString print = figure_dir + "/EtaMin";
    for (auto sample_name : sample_names){
      mLegend["EtaMin"]->AddEntry( mDrawHists["EtaMin"][sample_name], sample_name, "l" );
      print += sample_name + "_";
    }

    mLegend["EtaMin"]->AddEntry( mDrawHists["EtaMin"][data_sample_name], "Data", "lep" );
    mLegend["EtaMin"]->Draw("same");
    if(draw_figure) mCanvas["EtaMin"]->Print(print +"." + figure_type );

    
  }

  {  
    mCanvas["EtaMax"]->cd(1);
    mLegend["EtaMax"] = new TLegend(0.19,0.47,0.59,0.87);
    for(unsigned int iSuffix=0; iSuffix<vSuffix.size(); iSuffix++) {
      // define legend text for stacked hist
      TString leg_text = vSuffix[iSuffix];
      leg_text.Remove(0,1);
      // add legend entry
      mLegend["EtaMax"]->AddEntry( mDrawHists["EtaMax"][ vSuffix[iSuffix] ], leg_text, "f" );
    }
    TString print = figure_dir + "/EtaMax";
    for (auto sample_name : sample_names) {
      mLegend["EtaMax"]->AddEntry( mDrawHists["EtaMax"][sample_name], sample_name, "l" );
      print += sample_name + "_";
    } 

    mLegend["EtaMax"]->AddEntry( mDrawHists["EtaMax"][data_sample_name], "Data", "lep" );
    mLegend["EtaMax"]->Draw("same");
    if(draw_figure) mCanvas["EtaMax"]->Print(print +"." + figure_type );
  }
 
  {
    mCanvas["NTowHF_plus"]->cd(1);
    mLegend["NTowHF_plus"] = new TLegend(0.19,0.47,0.59,0.87);
    for(unsigned int iSuffix=0; iSuffix<vSuffix.size(); iSuffix++) {
      // define legend text for stacked hist
      TString leg_text = vSuffix[iSuffix];
      leg_text.Remove(0,1);
      // add legend entry
      mLegend["NTowHF_plus"]->AddEntry( mDrawHists["NTowHF_plus"][ vSuffix[iSuffix] ], leg_text, "f" );
    }
    
    TString print = figure_dir + "/NTowHF_plus";
    for (auto sample_name : sample_names){
      mLegend["NTowHF_plus"]->AddEntry( mDrawHists["NTowHF_plus"][sample_name], sample_name, "l" );
       print += sample_name + "_";
    } 

    mLegend["NTowHF_plus"]->AddEntry( mDrawHists["NTowHF_plus"][data_sample_name], "Data", "lep" );
    mLegend["NTowHF_plus"]->Draw("same");
    if(draw_figure) mCanvas["NTowHF_plus"]->Print(print +"." + figure_type );
    
  }
  {
    mCanvas["NTowHF_minus"]->cd(1);
    mLegend["NTowHF_minus"] = new TLegend(0.19,0.47,0.59,0.87);
    for(unsigned int iSuffix=0; iSuffix<vSuffix.size(); iSuffix++) {
      // define legend text for stacked hist
      TString leg_text = vSuffix[iSuffix];
      leg_text.Remove(0,1);
      // add legend entry
      mLegend["NTowHF_minus"]->AddEntry( mDrawHists["NTowHF_minus"][ vSuffix[iSuffix] ], leg_text, "f" );
    }
    TString print = figure_dir + "/NTowHF_minus";
    for (auto sample_name : sample_names){
      mLegend["NTowHF_minus"]->AddEntry( mDrawHists["NTowHF_minus"][sample_name], sample_name, "l" );
      print += sample_name + "_";
    }  

    mLegend["NTowHF_minus"]->AddEntry( mDrawHists["NTowHF_minus"][data_sample_name], "Data", "lep" );
    mLegend["NTowHF_minus"]->Draw("same");
    if(draw_figure) mCanvas["NTowHF_minus"]->Print(print +"." + figure_type );
  }
  {
    mCanvas["NTowCastor"]->cd(1);
    mLegend["NTowCastor"] = new TLegend(0.19,0.47,0.59,0.87);
    for(unsigned int iSuffix=0; iSuffix<vSuffix.size(); iSuffix++) {
      // define legend text for stacked hist
      TString leg_text = vSuffix[iSuffix];
      leg_text.Remove(0,1);
      // add legend entry
      mLegend["NTowCastor"]->AddEntry( mDrawHists["NTowCastor"][ vSuffix[iSuffix] ], leg_text, "f" );
    }
    
    TString print = figure_dir + "/NTowCastor";
    for (auto sample_name : sample_names){
      mLegend["NTowCastor"]->AddEntry( mDrawHists["NTowCastor"][sample_name], sample_name, "l" );
      print += sample_name + "_";
    } 

    mLegend["NTowCastor"]->AddEntry( mDrawHists["NTowCastor"][data_sample_name], "Data", "lep" );
    mLegend["NTowCastor"]->Draw("same");
    if(draw_figure) mCanvas["NTowCastor"]->Print(print +"." + figure_type );
    
    // mCanvas["NTowCastor_sys"]->cd(1);
    // mLegend["NTowCastor_sys"]->AddEntry( mDrawHists["NTowCastor_sys"][sample_name], sample_name, "l" );
    // mLegend["NTowCastor_sys"]->AddEntry( mDrawHists["NTowCastor_sys"][data_sample_name], "Data", "lep" );
    // mLegend["NTowCastor_sys"]->Draw("same");
    // if(draw_figure) mCanvas["NTowCastor_sys"]->Print( figure_dir + "/NTowCastor_sys" + sample_name + "." + figure_type );
  }

  { 
    mCanvas["NTow"]->cd(1);
    mLegend["NTow"] = new TLegend(0.19,0.47,0.59,0.87);
    for(unsigned int iSuffix=0; iSuffix<vSuffix.size(); iSuffix++) {
      // define legend text for stacked hist
      TString leg_text = vSuffix[iSuffix];
      leg_text.Remove(0,1);
      // add legend entry
      mLegend["NTow"]->AddEntry( mDrawHists["NTow"][ vSuffix[iSuffix] ], leg_text, "f" );
    }
    TString print = figure_dir + "/NTow";
    for (auto sample_name : sample_names){
      mLegend["NTow"]->AddEntry( mDrawHists["NTow"][sample_name], sample_name, "l" );
      print += sample_name + "_";
    }
    mLegend["NTow"]->AddEntry( mDrawHists["NTow"][data_sample_name], "Data", "lep" );
    mLegend["NTow"]->Draw("same");
   if(draw_figure) mCanvas["NTow"]->Print(print +"." + figure_type );

  }
 
  {  
    mCanvas["NTowMaxHFPlus"]->cd(1);
    mLegend["NTowMaxHFPlus"] = new TLegend(0.19,0.47,0.59,0.87);
    for(unsigned int iSuffix=0; iSuffix<vSuffix.size(); iSuffix++) {
      // define legend text for stacked hist
      TString leg_text = vSuffix[iSuffix];
      leg_text.Remove(0,1);
      // add legend entry
      mLegend["NTowMaxHFPlus"]->AddEntry( mDrawHists["NTowMaxHFPlus"][ vSuffix[iSuffix] ], leg_text, "f" );
    }
    TString print = figure_dir + "/NTowMaxHFPlus";
    for (auto sample_name : sample_names){
      mLegend["NTowMaxHFPlus"]->AddEntry( mDrawHists["NTowMaxHFPlus"][sample_name], sample_name, "l" );
      print += sample_name + "_";
    } 

    mLegend["NTowMaxHFPlus"]->AddEntry( mDrawHists["NTowMaxHFPlus"][data_sample_name], "Data", "lep" );
    mLegend["NTowMaxHFPlus"]->Draw("same");
    if(draw_figure) mCanvas["NTowMaxHFPlus"]->Print(print +"." + figure_type );
  }
  
  
  {  
    mCanvas["NTowMaxHFMinus"]->cd(1);
    mLegend["NTowMaxHFMinus"] = new TLegend(0.19,0.47,0.59,0.87);
    for(unsigned int iSuffix=0; iSuffix<vSuffix.size(); iSuffix++) {
      // define legend text for stacked hist
      TString leg_text = vSuffix[iSuffix];
      leg_text.Remove(0,1);
      // add legend entry
      mLegend["NTowMaxHFMinus"]->AddEntry( mDrawHists["NTowMaxHFMinus"][ vSuffix[iSuffix] ], leg_text, "f" );
    }
    TString print = figure_dir + "/NTowMaxHFMinus";
    for (auto sample_name : sample_names){
      mLegend["NTowMaxHFMinus"]->AddEntry( mDrawHists["NTowMaxHFMinus"][sample_name], sample_name, "l" );
      print += sample_name + "_";
    } 

    mLegend["NTowMaxHFMinus"]->AddEntry( mDrawHists["NTowMaxHFMinus"][data_sample_name], "Data", "lep" );
    mLegend["NTowMaxHFMinus"]->Draw("same");
    if(draw_figure) mCanvas["NTowMaxHFMinus"]->Print(print +"." + figure_type );
  }
  
  
  {
    mCanvas["SumEnergyHFPlus"]->cd(1);
    mLegend["SumEnergyHFPlus"] = new TLegend(0.19,0.47,0.59,0.87);
    for(unsigned int iSuffix=0; iSuffix<vSuffix.size(); iSuffix++) {
      // define legend text for stacked hist
      TString leg_text = vSuffix[iSuffix];
      leg_text.Remove(0,1);
      // add legend entry
      mLegend["SumEnergyHFPlus"]->AddEntry( mDrawHists["SumEnergyHFPlus"][ vSuffix[iSuffix] ], leg_text, "f" );
    }
    TString print = figure_dir + "/SumEnergyHFPlus";
    for (auto sample_name : sample_names){
      mLegend["SumEnergyHFPlus"]->AddEntry( mDrawHists["SumEnergyHFPlus"][sample_name], sample_name, "l" );
      print += sample_name + "_";
    }  
    mLegend["SumEnergyHFPlus"]->AddEntry( mDrawHists["SumEnergyHFPlus"][data_sample_name], "Data", "lep" );
    mLegend["SumEnergyHFPlus"]->Draw("same");
    if(draw_figure) mCanvas["SumEnergyHFPlus"]->Print(print +"." + figure_type );
  }



  {
    mCanvas["SumEnergyHFMinus"]->cd(1);
    mLegend["SumEnergyHFMinus"] = new TLegend(0.19,0.47,0.59,0.87);
    for(unsigned int iSuffix=0; iSuffix<vSuffix.size(); iSuffix++) {
      // define legend text for stacked hist
      TString leg_text = vSuffix[iSuffix];
      leg_text.Remove(0,1);
      // add legend entry
      mLegend["SumEnergyHFMinus"]->AddEntry( mDrawHists["SumEnergyHFMinus"][ vSuffix[iSuffix] ], leg_text, "f" );
    }
    TString print = figure_dir + "/SumEnergyHFMinus";
    for (auto sample_name : sample_names){
      mLegend["SumEnergyHFMinus"]->AddEntry( mDrawHists["SumEnergyHFMinus"][sample_name], sample_name, "l" );
      print += sample_name + "_";
    }  
    mLegend["SumEnergyHFMinus"]->AddEntry( mDrawHists["SumEnergyHFMinus"][data_sample_name], "Data", "lep" );
    mLegend["SumEnergyHFMinus"]->Draw("same");
    if(draw_figure) mCanvas["SumEnergyHFMinus"]->Print(print +"." + figure_type );
  }
  {
    mCanvas["SumEnergyCastor"]->cd(1);
    mLegend["SumEnergyCastor"] = new TLegend(0.19,0.47,0.59,0.87);
    for(unsigned int iSuffix=0; iSuffix<vSuffix.size(); iSuffix++) {
      // define legend text for stacked hist
      TString leg_text = vSuffix[iSuffix];
      leg_text.Remove(0,1);
      // add legend entry
      mLegend["SumEnergyCastor"]->AddEntry( mDrawHists["SumEnergyCastor"][ vSuffix[iSuffix] ], leg_text, "f" );
    }
    TString print = figure_dir + "/SumEnergyCastor";
    for (auto sample_name : sample_names){
      mLegend["SumEnergyCastor"]->AddEntry( mDrawHists["SumEnergyCastor"][sample_name], sample_name, "l" );
      print += sample_name + "_";
    }  
    mLegend["SumEnergyCastor"]->AddEntry( mDrawHists["SumEnergyCastor"][data_sample_name], "Data", "lep" );
    mLegend["SumEnergyCastor"]->Draw("same");
    if(draw_figure) mCanvas["SumEnergyCastor"]->Print(print +"." + figure_type );

  }
  
  { 
    mCanvas["NTowMaxCastor"]->cd(1);
    mLegend["NTowMaxCastor"] = new TLegend(0.19,0.47,0.59,0.87);
    for(unsigned int iSuffix=0; iSuffix<vSuffix.size(); iSuffix++) {
      // define legend text for stacked hist
      TString leg_text = vSuffix[iSuffix];
      leg_text.Remove(0,1);
      // add legend entry
      mLegend["NTowMaxCastor"]->AddEntry( mDrawHists["NTowMaxCastor"][ vSuffix[iSuffix] ], leg_text, "f" );
    }
    
    TString print = figure_dir + "/NTowMaxCastor";
    for (auto sample_name : sample_names){
      mLegend["NTowMaxCastor"]->AddEntry( mDrawHists["NTowMaxCastor"][sample_name], sample_name, "l" );
      print += sample_name + "_";
    }  
    mLegend["NTowMaxCastor"]->AddEntry( mDrawHists["NTowMaxCastor"][data_sample_name], "Data", "lep" );
    mLegend["NTowMaxCastor"]->Draw("same");
    if(draw_figure) mCanvas["NTowMaxCastor"]->Print(print +"." + figure_type );

  }
  {
    mCanvas["NTracks"]->cd(1);
    mLegend["NTracks"] = new TLegend(0.19,0.47,0.59,0.87);
    for(unsigned int iSuffix=0; iSuffix<vSuffix.size(); iSuffix++) {
      // define legend text for stacked hist
      TString leg_text = vSuffix[iSuffix];
      leg_text.Remove(0,1);
      // add legend entry
      mLegend["NTracks"]->AddEntry( mDrawHists["NTracks"][ vSuffix[iSuffix] ], leg_text, "f" );
    }
    TString print = figure_dir + "/NTracks";
    for (auto sample_name : sample_names){
      mLegend["NTracks"]->AddEntry( mDrawHists["NTracks"][sample_name], sample_name, "l" );
      print += sample_name + "_";
    }   
    mLegend["NTracks"]->AddEntry( mDrawHists["NTracks"][data_sample_name], "Data", "lep" );
    mLegend["NTracks"]->Draw("same");
    if(draw_figure) mCanvas["NTracks"]->Print(print +"." + figure_type );

  }
  
  {
    mCanvas["recoXix"]->cd(1);
    mLegend["recoXix"] = new TLegend(0.19,0.47,0.59,0.87);
    for(unsigned int iSuffix=0; iSuffix<vSuffix.size(); iSuffix++) {
      // define legend text for stacked hist
      TString leg_text = vSuffix[iSuffix];
      leg_text.Remove(0,1);
      // add legend entry
      mLegend["recoXix"]->AddEntry( mDrawHists["recoXix"][ vSuffix[iSuffix] ], leg_text, "f" );
    }
    
    TString print = figure_dir + "/recoXix";
    for (auto sample_name : sample_names){
      mLegend["recoXix"]->AddEntry( mDrawHists["recoXix"][sample_name], sample_name, "l" );
      print += sample_name + "_";
    } 

    mLegend["recoXix"]->AddEntry( mDrawHists["recoXix"][data_sample_name], "Data", "lep" );
    mLegend["recoXix"]->Draw("same");
    if(draw_figure) mCanvas["recoXix"]->Print(print +"." + figure_type );

  }
 
  {  

    mCanvas["recoXiy"]->cd(1);
    mLegend["recoXiy"] = new TLegend(0.19,0.47,0.59,0.87);
    for(unsigned int iSuffix=0; iSuffix<vSuffix.size(); iSuffix++) {
      // define legend text for stacked hist
      TString leg_text = vSuffix[iSuffix];
      leg_text.Remove(0,1);
      // add legend entry
      mLegend["recoXiy"]->AddEntry( mDrawHists["recoXiy"][ vSuffix[iSuffix] ], leg_text, "f" );
    }
    TString print = figure_dir + "/recoXiy";
    for (auto sample_name : sample_names){
      mLegend["recoXiy"]->AddEntry( mDrawHists["recoXiy"][sample_name], sample_name, "l" );
      print += sample_name + "_";
    }
      
    mLegend["recoXiy"]->AddEntry( mDrawHists["recoXiy"][data_sample_name], "Data", "lep" );
    mLegend["recoXiy"]->Draw("same");
    if(draw_figure) mCanvas["recoXiy"]->Print(print +"." + figure_type );
  }

 }

void single_sample_compare_mc_data(std::map<TString, SampleList::sSample>& mSample,
                                   std::vector<TString>& vSuffix,
                                   std::map<TString, sSingleVar>& mSingleTrainingVar,
                                   std::vector<TString> sample_names,
                                   TString data_sample_name,
                                   bool scale_data) 
{
  {
  for (auto sample_name : sample_names)
   if( mSample.find(sample_name) == mSample.end() ) {
     std::cout << "In single_sample_compare_mc_data(): map mSample has no Sample with name: "
               << sample_name << " !!!" << std::endl;
     throw;
    }
  }

  const bool showStack = true; // false to draw different MCs
//  vector<TString> models = {""};
  
  std::map<TString, TCanvas*> mCanvas;
  std::map<TString, std::map<TString, TH1*> > mDrawHists;
  std::map<TString, TLegend*> mLegend;

  for(std::map<TString, sSingleVar>::iterator it = mSingleTrainingVar.begin(); it != mSingleTrainingVar.end(); it++) {
    // if(it->first != "NTow") continue;
    CanvasHelper ch(it->second.canvas_title);
    mCanvas[it->first] = ch.initRatioCanvas(it->second.xmin,it->second.xmax,
                                            it->second.ymin,it->second.ymax,
                                            it->second.rmin,it->second.rmax,
                                            it->second.xaxis_title,
                                            it->second.yaxis_title,
                                            it->second.ratio_title);

    for (auto sample_name : sample_names) {
      if (showStack) {
        mDrawHists[it->first] = single_figure_addstack(ch,mSample,vSuffix,sample_name,it->second.hist_name);
      }
    }
    std::map<TString, TH1*> moreHists = single_figure_compare_mc_data(ch,mSample,vSuffix,sample_names,data_sample_name,it->second.hist_name,scale_data);
    mDrawHists[it->first].insert(moreHists.begin(), moreHists.end());
    // ch.DrawCMSPreliminary(true,it->second.cms_alignment,"4.58088 #mub^{-1} (13 TeV)");//mine
    // ch.DrawCMSPreliminary(true,it->second.cms_alignment,"3.86 #mub^{-1} (13 TeV)"); //dNDeta
    ch.DrawCMSPreliminary(true,it->second.cms_alignment,"24.99 #mub^{-1} (13 TeV)");//Sebastian 247934
    // ch.DrawCMSPreliminary(true,it->second.cms_alignment,"32.24 #mub^{-1} (13 TeV)"); //sebastina 247920
  }

 
  //////////////////////////////////////////////////////////////////////////LEGENDStack0
  // Draw Legends for different Hists
  if (showStack)
    draw_legend_for_diff_canvas(mCanvas,mDrawHists,mLegend,vSuffix,sample_names,data_sample_name);
  else {
    std::vector<TString> noSuffix;
    draw_legend_for_diff_canvas(mCanvas,mDrawHists,mLegend,noSuffix,sample_names,data_sample_name);
  }


  /////////////////////////////////////////////////////////////////////////////LEGENDStack0
  // add different MC samples to compare
  //vector<TString> sample_name_diffMC;
  //ssample_name_diffMC.push_back("EPOS" );
  // sample_name_diffMC.push_back("EPOS"); //if want to add
 
  /////////////////Diffrent MC
  // draw_legend_for_diff_canvas_diffMC(mCanvas,mDrawHists,mLegend,sample_name_diffMC,sample_name,data_sample_name); // add different MC samples0

 ///////////////////////////////

}


////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
std::map<TString, TH1*>
single_figure_addstack(CanvasHelper& ch,
                              std::map<TString, SampleList::sSample>& mSample,
                              std::vector<TString>& vSuffix,
                              TString sample_name,
                              TString hist_var_name) 
{
  //////////////////////////////////////////////////////////////////////////
  // hist pointer container to return pointer to drawn hists
  std::map<TString, TH1*> mDrawHists;

  double lumi_mc   = mSample[sample_name].lumi;

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

  //////////////////////////// color code for the different hists////////////////////////////////////////////////
  
  Color_t col[5] = {29, kYellow+1 ,kGreen, kGreen+2, 24}; //stacked hists1
 ////////////////////////// color code for the different hists
 
 
  ////////////////////////////// access the stacked hists and add it to the canvas helper/////////////////////////////i/////////////stacked hists2
 
  for(unsigned int iHist=0; iHist<shh.getHistSize(); iHist++) {
    shh.getHist(iHist)->Scale( 1/lumi_mc, "width" );
    ch.addHist( shh.getHist(iHist), "HIST", col[iHist], kSolid, 20, 1001 );

    TString hist_text = vSuffix[shh.getHistSize()-iHist-1];
    mDrawHists[hist_text] = shh.getHist(iHist);
  }

  return mDrawHists;
}





//////////////////////////////////////////////////////////////////////////
std::map<TString, TH1*>
single_figure_compare_mc_data(CanvasHelper& ch,
                              std::map<TString, SampleList::sSample>& mSample,
                              std::vector<TString>& vSuffix,
                              std::vector<TString> sample_names,
                              TString data_sample_name,
                              TString hist_var_name,
                              // TLegend* leg,
                              bool scale_data) 
{
  //////////////////////////////////////////////////////////////////////////
  // hist pointer container to return pointer to drawn hists
  std::map<TString, TH1*> mDrawHists;

  double lumi_data = mSample[data_sample_name].lumi;

  // //////////////////////////////////////////////////////////////////////// comparision MCs2
  // access data hist
  TH1F* hData = (TH1F*)mSample[data_sample_name].file->Get(mSample[data_sample_name].tree_name + "/" + hist_var_name);
  if(scale_data) hData->Scale( 1/lumi_data , "width" );
  mDrawHists[data_sample_name] = hData;
 
  ///comparision MCs1
  Color_t col_diffMC[3] = {kBlue+2,kOrange+2, 24};

  // ///////////////////////////////////////////////////////////// comparision MCs2
  // // add different MC samples to compare
 
  for(unsigned int iHist=0; iHist<sample_names.size(); iHist++) {
    double lumi_sample = mSample[sample_names[iHist]].lumi;
    mDrawHists[sample_names[iHist]] = (TH1F*)mSample[sample_names[iHist]].file->Get(mSample[sample_names[iHist]].tree_name + "/" + hist_var_name);
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    #warning LUMI
    mDrawHists[sample_names[iHist]]->Scale( 1./lumi_sample, "width");
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    ch.addHist( mDrawHists[sample_names[iHist]], "HIST", col_diffMC[iHist] );

    ch.addRatioHist( get_Ratio(mDrawHists[sample_names[iHist]],hData), "HIST", col_diffMC[iHist] );
  }
  
  /////////////////////////////////////////////////////////////
  // Do at last so that data points are on top of the plot
  ch.addDataHist( hData );
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
  vSuffix.push_back("_SD2");
  vSuffix.push_back("_Rest");
  vSuffix.push_back("_SD1"); 
  
  vSuffix.push_back("_DD");//Signal:DD,SD you need to switch
  
  TString mc_sample_name = "XiCutEPOS";
  TString data_sample_name = "Data_sebastian247920";
  TString sSignal ="DD";
  TString training_sample_name = "XiCutEPOS";
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
                     // 1e2,1e6,//dNdeta
                     1e3,1e8,
                     0,3,
                     "Classifier X",
                     "1/N_{evt} dN/dX",
                     // "1/L dN/dX [#mub]",
                     "MC / Data");

  /////////////////////////////////////////////////////////////
  // get lumi for mc sample
  double lumi_mc = mSample[mc_sample_name].lumi;
  // double events_mc = ((TH1F*)mc_file->Get("hNentries"))->GetBinContent(1);

  //////////////////////////////////////////////////////////////////////////
  // get data lumi
  double lumi_data = mSample[data_sample_name].lumi;
  TH1F* hMC = (TH1F*)mc_file->Get(hist_name);
  if (!hMC) {
    cout << "cannot read " << hist_name << " from file " << mc_file->GetName() << endl;
    exit(1);
  }
  // hMC->Scale( 1./events_mc, "width" );
//  hMC->Scale( 1./lumi_mc, "width" );
  // access data hist
  TH1F* hData = (TH1F*)data_file->Get(hist_name);
  if (!hData) {
    cout << "cannot read " << hist_name << " from file " << data_file->GetName() << endl;
    exit(1);
  }


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
    shh.getHist(iHist)->Scale(hData->GetEntries()/hMC->GetEntries(), "width" );
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


  hMC->Scale( hData->GetEntries()/hMC->GetEntries(), "width" );
  // hMC->Scale(1/hMC, "width" );
  ch.addHist( hMC, "HIST", kBlue+2 );
  leg->AddEntry( hMC, mSample[mc_sample_name].legend_name , "l");

  //////////////////////////////////////////////////////////////////////////

  
  // double events_data = ((TH1F*)data_file->Get("hNentries"))->GetBinContent(1);
  // as a hack
  // hData->Sumw2();
  // hData->Scale( 1./events_data , "width" );
  hData->Scale( 1. , "width" );
  // hData->Scale( 1./lumi_data , "width" );



  ch.addDataHist(hData);
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
  ch.DrawCMSPreliminary(true,11,"32.24 #mub^{-1} (13 TeV)");
  
   // "4.58088 #mub^{-1} (13 TeV)");//mine 247934
   // "3.86 #mub^{-1} (13 TeV)"); //dNDeta
   // "24.99 #mub^{-1} (13 TeV)");//Sebastian 247934
   // "32.24 #mub^{-1} (13 TeV)"); //sebastina 247920

}




//////////////////////////////////////////////////////////////////////////
void discriminant_compare_data_syst(std::map<TString, SampleList::sSample>& mSample)
{

  std::vector<TString> vSuffix;
  // vSuffix.push_back("_NONE");
  vSuffix.push_back("_DD");
  vSuffix.push_back("_Rest");
  vSuffix.push_back("_SD1"); 

  vSuffix.push_back("_SD2");//Signal:DD,SD you need to switch
  
  TString mc_sample_name = "XiCutPythia8SD2";
  TString data_sample_name = "Data";
  const std::vector<TString> vData_Syst = {"Data_sysCastorPlus", "Data_sysCastorMinus","Data_sysHFPlus","Data_sysHFMinus", "Data_sysTrackMinus"};

  TString sSignal ="SD2";
  TString training_method = "BDTG";

  TString hist_name = TString("hDisciminant_") + mc_sample_name + "_" + training_method;
  

  TString data_dir = "data";
  TFile* mc_file = TFile::Open(data_dir + "/" + mSample[mc_sample_name].app_output_file_name);
  TFile* data_file = TFile::Open(data_dir + "/" + mSample[data_sample_name].app_output_file_name);
  // TGraphAsymmErrors* grDis_Sys = new TGraphAsymmErrors(hData->GetNbinsX());
  // grDis_Sys->SetFillColor(kYellow+2);
  // TGraphAsymmErrors* grDis_Sys_Ratio = new TGraphAsymmErrors(hData->GetNbinsX());
  // grDis_Sys_Ratio->SetFillColor(kYellow+2);
 
    
 
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
                     0.8,1.4,
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
    // ch.addHist( shh.getHist(iHist), "HIST", col[iHist], kSolid, 20, 1001 );
     
    // define legend text for stacked hist
    TString leg_text = vSuffix[shh.getHistSize()-iHist-1];
    leg_text.Remove(0,1);
    // add legend entry
    // leg->AddEntry( shh.getHist(iHist), leg_text, "f" );
    if( leg_text == "Rest" ) {
      leg_text = "Background";
      // leg->AddEntry( shh.getHist(iHist), leg_text, "f" );
    } else if( leg_text== sSignal ) {
      // leg->AddEntry( shh.getHist(iHist), leg_text, "f" );
    }
  }

  TH1F* hMC = (TH1F*)mc_file->Get(hist_name);
  if (!hMC) {
    cout << "cannot read " << hist_name << " from file " << mc_file->GetName() << endl;
    exit(1);
  }
  // hMC->Scale( 1./events_mc, "width" );
  // hMC->Scale( 1./lumi_mc, "width" );
     hMC->Scale( 1./lumi_mc, "width" );                  

  // ch.addHist( hMC, "HIST", kBlue+2 );
  // leg->AddEntry( hMC, mSample[mc_sample_name].legend_name , "l");

  //////////////////////////////////////////////////////////////////////////
  // access data hist
  TH1F* hData = (TH1F*)data_file->Get(hist_name);
  if (!hData) {
    cout << "cannot read " << hist_name << " from file " << data_file->GetName() << endl;
    exit(1);
  }




  //////////////////////////////////////////////////////////////////////////
  // access data hist with systematics
  Color_t col_syst[5] = {kRed+1, kYellow+1, kBlue+1, kMagenta+1, kGreen+3};
  std::vector<TH1F*> vHist_Data_Syst;
  for(unsigned int iHist=0; iHist<vData_Syst.size(); iHist++ ) {
    const TString data_sys_name = vData_Syst[iHist];

    TFile* fData_Syst = TFile::Open(data_dir + "/" + mSample[data_sys_name].app_output_file_name);

    TH1F* hData_Syst_tmp = (TH1F*)fData_Syst->Get(hist_name);
    if(!hData_Syst_tmp) {
      cout << "cannot read " << hist_name << " from file " << fData_Syst->GetName() << endl;
      exit(1);
    }

    double lumi_data_syst = mSample[data_sys_name].lumi;
    hData_Syst_tmp->Scale( 1./lumi_data_syst , "width" );
    ch.addHist( hData_Syst_tmp, "HIST", col_syst[(iHist%5)] );
    vHist_Data_Syst.push_back( hData_Syst_tmp );

    leg->AddEntry( hData_Syst_tmp, mSample[data_sys_name].legend_name , "l" );
  }



  //////////////////////////////////////////////////////////////////////////
  // get data lumi
  double lumi_data = mSample[data_sample_name].lumi;
  hData->Scale( 1./lumi_data , "width" );
  
  ch.addDataHist( hData );
  // add legend entry
  leg->AddEntry( hData, mSample[data_sample_name].legend_name , "lep" );

  // ch.addRatioHist( get_Ratio(hMC,hData), "HIST", kBlue );
  for(unsigned int iHist=0; iHist<vHist_Data_Syst.size(); iHist++ ) {
    ch.addRatioHist( get_Ratio(vHist_Data_Syst[iHist],hData), "HIST", col_syst[(iHist%5)] );
  }
  ch.addRatioHist( get_Ratio(hData,hData), "EP", kBlack );

  // draw stacked hists
  ch.DrawHist();

  // draw legend
  ch.getCanvas()->cd(1);
  leg->Draw("same");
  if(draw_figure)ch.getCanvas()->Print( figure_dir + "/hDisciminantsys_" + mc_sample_name + "" + data_sample_name +""+ sSignal+"."+ figure_type );
  ch.DrawCMSPreliminary(true,11,"3.86 #mub^{-1} (13 TeV)");
  
  
   // "4.58088 #mub^{-1} (13 TeV)");//mine 247934
   // "3.86 #mub^{-1} (13 TeV)"); //dNDeta
   // "24.99 #mub^{-1} (13 TeV)");//Sebastian 247934
   // "32.24 #mub^{-1} (13 TeV)"); //sebastina 247920

}



void discriminant_results(std::map<TString, SampleList::sSample>& mSample)
{

  std::vector<TString> vSuffix;
  // vSuffix.push_back("_NONE");

  vSuffix.push_back("_SD1");
  vSuffix.push_back("_SD2");
  vSuffix.push_back("_DD");
  vSuffix.push_back("_Rest");

  TString mc_sample_name = "XiCutEPOS";
  TString data_sample_name = "Data_sebastian247920";

  TString training_sample_name = "XiCutEPOS";
  TString training_method = "BDTG";

  TString hist_name = TString("hDisciminant_") + training_sample_name + "_" + training_method;


  TString data_dir = "data";
  TFile* mc_file = TFile::Open(data_dir + "/" + mSample[mc_sample_name].app_output_file_name);

  TH1F* hMC   = (TH1F*)mc_file->Get(hist_name);

  TString sSignal ="DD";
  TH1F* hsig = (TH1F*)mc_file->Get(hist_name+"_"+sSignal);

  std::map<TString,TH1F*> hbkg;
  for(unsigned int idx=0; idx<vSuffix.size(); idx++) {
    if( vSuffix[idx].Copy().Remove(0,1) == sSignal ) continue;
    hbkg[ vSuffix[idx].Copy().Remove(0,1) ] = (TH1F*)mc_file->Get(hist_name+vSuffix[idx]);
  }
  
  std::cout << "file name = " << data_dir + "/" + mSample[mc_sample_name].app_output_file_name << std::endl;
  std::cout << "hist_name = " << hist_name+"_"+sSignal << std::endl;

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
  sprintf(buf,"Signal Efficiency = %3.1f",gr->GetX()[iCutVal_minimum]*100);
  tl.DrawLatex(0.05,0.5,buf);
  sprintf(buf,"Background rejection = %3.1f",gr->GetY()[iCutVal_minimum]*100);
  tl.DrawLatex(0.05,0.35,buf);


  CanvasHelper::SetUpHist((TH1*)gr_prob_sig_bkg,kBlue);
  // gr_prob_sig_bkg->Draw("P same");

  c2->cd();
  CanvasHelper::SetUpHist((TH1*)gr_dist_aim_point,kBlue);
  gr_dist_aim_point->GetXaxis()->SetTitle("Classifier X");
  gr_dist_aim_point->GetYaxis()->SetTitle("distance to Eff.=1; Bckg.=0");
  gr_dist_aim_point->SetMarkerSize(1);
  TLegend * leg = new TLegend(0.3,0.7,0.7,0.9);
  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->SetTextFont(42);
  leg->SetTextSize(0.05);
  
  leg->AddEntry(gr_dist_aim_point,"distance to Eff.=1; Bckg=0","lep");
  
  gr_dist_aim_point->Draw("ALP");
  chROCurve.DrawCMSSimulation();
  leg->Draw("same");
  if(draw_figure)chROCurve.getCanvas()->Print( figure_dir + "/chROCurve_" + mc_sample_name + "." + data_sample_name +"."+ figure_type );



  cSdivSqrtSplusB->cd();
  chSdivSqrtSplusB.SetUpHist((TH1*)gr_SdivSqrtSplusB,kGreen+2);
  chSdivSqrtSplusB.SetUpHist((TH1*)gr_sig_purity_tims_eff,kBlue);
  CanvasHelper::SetUpHist((TH1*)gr_prob_sig,kRed);
  CanvasHelper::SetUpHist((TH1*)gr_prob_bkg,kMagenta);
  TLegend * legSdivSqrtSplusB = new TLegend(0.48,0.7,0.88,0.9);
  legSdivSqrtSplusB->SetFillColor(0);
  legSdivSqrtSplusB->SetBorderSize(0);
  legSdivSqrtSplusB->SetFillStyle(0);
  legSdivSqrtSplusB->SetTextFont(42);
  legSdivSqrtSplusB->SetTextSize(0.05);



  legSdivSqrtSplusB->AddEntry(gr_prob_sig,"Sig probability","lep");
  legSdivSqrtSplusB->AddEntry(gr_prob_bkg,"Background probability","lep");
  legSdivSqrtSplusB->AddEntry(gr_sig_purity_tims_eff,"Purity X Eff.","lep");
 
 
//  gr_SdivSqrtSplusB->Draw("APL");
  gr_sig_purity_tims_eff->Draw("APL");
  gr_prob_sig->Draw("P");
  gr_prob_bkg->Draw("P");
  // gr_mark_point->Draw("P same");
  chSdivSqrtSplusB.DrawCMSSimulation();
  chSdivSqrtSplusB.DrawHist(true);//logscale
  gr_sig_purity_tims_eff->GetXaxis()->SetTitle("Classifier");
  gr_sig_purity_tims_eff->GetYaxis()->SetTitle("Values");
  legSdivSqrtSplusB->Draw();
  if(draw_figure)chSdivSqrtSplusB.getCanvas()->Print( figure_dir + "/hSdivSqrtSplusB_" + mc_sample_name + "." + data_sample_name +"."+ figure_type);
}  
  

double calc_error_propagation (double x,double y) {    
    return  std::sqrt(1/pow(y,2)*(x) + pow((x/ pow(y,2)),2)*y);

}




void calc_signal_cross_section(double& result_cross_section, double& totalcrossectionError,
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

  TString mc_sample_name = "XiCutEPOS";
  TString data_sample_name = "Data_sebastian247920";

  TString training_sample_name = "XiCutEPOS";
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
  // calc_error_propagation( eventswithvrtzcut_mc ,Allevents_mc));
  double Error_Fsplit =1- calc_error_propagation(eventswithvrtzcut_mc ,Allevents_mc);
  
  double eff_PU_data = Allevents_data / eventswithvrtzcut_data;
  double Error_eff_PU_data = std::sqrt(1/ pow(eventswithvrtzcut_data,2)*Allevents_data+ pow((Allevents_data/pow(eventswithvrtzcut_data,2)),2)*(eventswithvrtzcut_data)); 
  
  double prob_sig = Entry_sig / (Entry_bkg + Entry_sig);
  double Error_probsig = calc_error_propagation(Entry_sig ,(Entry_bkg + Entry_sig)); 
  
  
  double eff_sig = (Entry_sig * Fsplit )/ Entry_sig_all; //MC signal efficiency
  // double Error_eff_sig = std::sqrt(1/ pow(Entry_sig_all,2)*(Entry_sig * Fsplit)+ pow(((Entry_sig * Fsplit )/pow(Entry_sig_all,2)),2)*(Entry_sig_all));  
  double Error_eff_sig = calc_error_propagation((Entry_sig * Fsplit ) , Entry_sig_all); 
  double eff_number_of_signal_events_in_data = (Entry_data * prob_sig)/eff_sig; //effictive lumi data
  
  
  result_cross_section = eff_number_of_signal_events_in_data/(lumi_data/eff_PU_data);
  totalcrossectionError = std::sqrt(Entry_data) * prob_sig / (lumi_data/eff_PU_data) / eff_sig ;
  
  
  // double standartError = std::sqrt(totalcrossectionError) * Error_probsig / (Error_Lumi/Error_eff_PU_data) / Error_eff_sig ;
  
  
 //////////////////////////////////Error Propagation//////////////////////////////////// 


/////////////////////////////////////////////////////////////////////////////////////////////////////    


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
  std::cout << "--- !!! totalcrossectionError = " << totalcrossectionError << " !!! ---" << std::endl;
  std::cout << "--- !!! Effective Signal CROSS SECTION = " << result_cross_section << " ub !!! ---" << std::endl;
  std::cout << "Fsplit: " << Fsplit << std::endl;
  std::cout << "eff_PU_data: " << eff_PU_data << std::endl;
  std::cout << "======================================================================" << std::endl;

  std::cout << "==============================Standart Error========================================" << std::endl;
  std::cout << "--- Standart Error for signal efficiency = " << Error_eff_sig << std::endl;
  std::cout << "--- Standart Error for signal probability = " << Error_probsig << std::endl;
  std::cout << "Error_eff_PU_data: " << Error_eff_PU_data << std::endl; 
  std::cout << "Error_Fsplit: " << Error_Fsplit << std::endl; 
  
  
  
  // std::cout << "STANDART ERROR: " <<standartError << std::endl; 

  std::cout << std::endl;
  std::cout << std::endl;


}


void single_sample_compare_syst(std::map<TString, SampleList::sSample>& mSample,
                                std::map<TString, sSingleVar>& mSingleTrainingVar,
                                TString sample_name,
                                TString data_sample_name)
{
  double lumi_mc   = mSample[sample_name].lumi;
  double lumi_data = mSample[data_sample_name].lumi;

  //////////////////////////////////////////////////////////////////////////
  // acces hist with systematics
 
  //  /////////////HF Pluss//////////////////////////
  const vector<TString> SysList = {"Data_sysHFPlus"};
  const vector<TString> ParList = {"NTowHF_plus_sys", "NTowHF_minus_sys"};
 
  // ////////////////////////Castor//////////////////////////////
  // const vector<TString> SysList = {"Data_sysCastorPlus", "Data_sysCastorMinus"};
  // const vector<TString> ParList = {"NTowCastor_sys", "NTowMaxCastor_sys"};
  
  ///////////////////////Track////////////////////////////////////////////
  // const vector<TString> SysList = { "Data_sysTrackMinus"};
  // const vector<TString> ParList = {"NTracks_sys"};
 

  for (unsigned int ipar = 0; ipar<ParList.size(); ipar++){

    TString hist_var_name = mSingleTrainingVar[ParList[ipar]].hist_name;

    TH1F* hMC = (TH1F*)mSample[sample_name].file->Get(mSample[sample_name].tree_name + "/" + hist_var_name);
    TH1F* hData = (TH1F*)mSample[data_sample_name].file->Get(mSample[data_sample_name].tree_name + "/" + hist_var_name);
    
    hData->Scale( 1/lumi_data , "width" );
    
    TGraphAsymmErrors* grData_Sys = new TGraphAsymmErrors(hData->GetNbinsX());
    grData_Sys->SetFillColor(kYellow+2);
    TGraphAsymmErrors* grData_Sys_Ratio = new TGraphAsymmErrors(hData->GetNbinsX());
    grData_Sys_Ratio->SetFillColor(kYellow+2);
    
    for (unsigned int isys = 0; isys<SysList.size(); isys++) {

      TH1F* hData_Sys = (TH1F*)mSample[SysList[isys]].file->Get(mSample[SysList[isys]].tree_name + "/" + hist_var_name);
      hData_Sys->Scale( 1/lumi_data , "width" );

      //////////////////////////////////////////////////////////////////////////
      // create TGraphError

      for(int iBin=1; iBin<=hData->GetXaxis()->GetNbins(); iBin++) {
        double xvalue = hData->GetBinCenter(iBin+1);
        double yvalue = hData->GetBinContent(iBin+1);
        grData_Sys->SetPoint(iBin-1, xvalue, yvalue);
        double ydiff = hData_Sys->GetBinContent(iBin) - hData->GetBinContent(iBin);
        if (ydiff>0) {
          if (grData_Sys->GetEYhigh()[iBin-1]<ydiff) {
            grData_Sys->SetPointEYhigh(iBin-1, ydiff);
          }
        } else {
          if (grData_Sys->GetEYlow()[iBin-1]<-ydiff) {
            grData_Sys->SetPointEYlow(iBin-1, -ydiff);
          }
        }
        double xhigh_err = hData->GetBinLowEdge(iBin+1) - hData->GetBinCenter(iBin);
        double xlow_err = hData->GetBinCenter(iBin) - hData->GetBinLowEdge(iBin);
        grData_Sys->SetPointEXhigh(iBin-1, xhigh_err);
        grData_Sys->SetPointEXlow(iBin-1, xlow_err);
      }    

      TH1F* hData_Sys_Ratio = get_Ratio(hData_Sys,hData);

      for(int iBin=1; iBin<=hData->GetXaxis()->GetNbins(); iBin++) {
        cout << "Bin Center: " << hData->GetBinCenter(iBin) << endl;

        cout << "hData_Sys: " << hData_Sys->GetBinContent(iBin) << endl;
        cout << "hData_Sys_Ratio: " << hData_Sys_Ratio->GetBinContent(iBin) << endl;

        double yvalue = 1;
        double xvalue = hData_Sys_Ratio->GetBinCenter(iBin);

        grData_Sys_Ratio->SetPoint(iBin-1, xvalue, yvalue);

        double ydiff = hData->GetBinContent(iBin) > 0 ? hData_Sys_Ratio->GetBinContent(iBin)-1 : 0;
        //double yhigh_err =  ydiff > 0 ? ydiff : 0;
        //double ylow_err  =  ydiff > 0 ? 0 : -ydiff;
        if (ydiff>0) {
          if (grData_Sys_Ratio->GetEYhigh()[iBin-1]<ydiff) {
            grData_Sys_Ratio->SetPointEYhigh(iBin-1, ydiff);
          }
        } else {
          if (grData_Sys_Ratio->GetEYlow()[iBin-1]<-ydiff) {
            grData_Sys_Ratio->SetPointEYlow(iBin-1, -ydiff);
          }
        }

        // cout << "yhigh_err: " << yhigh_err << endl;
        // cout << "ylow_err: " << ylow_err << endl;

        double xhigh_err = hData->GetBinLowEdge(iBin+1) - hData->GetBinCenter(iBin);
        double xlow_err = hData->GetBinCenter(iBin) - hData->GetBinLowEdge(iBin);


        //grData_Sys_Ratio->SetPointEYhigh(iBin-1, yhigh_err);
        //grData_Sys_Ratio->SetPointEYlow(iBin-1, ylow_err);
        grData_Sys_Ratio->SetPointEXhigh(iBin-1, xhigh_err);
        grData_Sys_Ratio->SetPointEXlow(iBin-1, xlow_err);
      }  
      

    }
    ostringstream canvasname;
    canvasname << "canvas_" << ParList[ipar] << "_" << sample_name;
    CanvasHelper ch(canvasname.str().c_str());
    ch.initRatioCanvas(mSingleTrainingVar[ParList[ipar]].xmin, mSingleTrainingVar[ParList[ipar]].xmax,
                       mSingleTrainingVar[ParList[ipar]].ymin, mSingleTrainingVar[ParList[ipar]].ymax,
                       mSingleTrainingVar[ParList[ipar]].rmin, mSingleTrainingVar[ParList[ipar]].rmax,
                       mSingleTrainingVar[ParList[ipar]].xaxis_title,
                       mSingleTrainingVar[ParList[ipar]].yaxis_title,
                       mSingleTrainingVar[ParList[ipar]].ratio_title);

    ch.getCanvas()->cd(1);
    
    gStyle->SetLegendTextSize(0.05); //nosuffix
    gStyle->SetLegendFont(42);
    TLegend * leg = new TLegend(0.59,0.75,0.99,0.9);
    leg->AddEntry( hMC, mSample[sample_name].legend_name , "l");
    leg->AddEntry( hData, mSample[data_sample_name].legend_name , "p");
    //ch.DrawHist(); //systematic draw stacked hists
    grData_Sys->Draw("same 2");
    leg->Draw("same");
    ch.DrawCMSPreliminary(true,10,"3.86 #mub^{-1} (13 TeV)");
    ch.getCanvas()->cd(2);
    grData_Sys_Ratio->Draw("same 2");



    hMC->Scale( 1/lumi_mc, "width" );
    ch.addHist( hMC, "HIST", kBlue+2);
    // mDrawHists[sample_name] = hMC;

    //////////////////////////////////////////////////////////////////////////
    // access data hist
    ch.addDataHist( hData );
    ch.addRatioHist( get_Ratio(hMC,hData), "HIST", kBlue+2 );
    ch.addRatioHist( get_Ratio(hData,hData), "EP", kBlack );
    ch.DrawHist(); //systematic
   
    
  }



  // draw legend
  // leg->Draw("same");
}