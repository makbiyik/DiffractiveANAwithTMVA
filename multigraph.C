#include "TString.h"
#include <string>
#include <vector>
#include <iostream> //or iostream.h
#include <cmath> // or math.h
#include <sstream>
#include <TH2D.h>
#include "TNtuple.h"
#include <TH1D.h>
#include <TNtuple.h>
#include <TFile.h>
#include <TSystem.h>
#include <TF1.h>
#include <TMath.h>
#include <TGaxis.h>
#include <TCanvas.h>
#include <TProfile.h>
#include <TLegend.h>
#include <TCut.h>
#include <TLine.h>
#include <TChain.h>
#include <TDirectory.h>
#include <TStyle.h>
// #include <tdrstyle.C>
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TMultiGraph.h"
#include "TGraphAsymmErrors.h"


void SetLegendStyle(TLegend * leg) {
  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  // leg->SetTextFont(23);
  // leg->SetTextSize(22);

  // leg->SetTextSize(22);
  leg->SetTextFont(42);
}


void multigraph(){



TGaxis::SetMaxDigits(3);
TH1::SetDefaultSumw2();
using namespace std;
gStyle->SetOptFit();



TFile* file_Model = TFile::Open("Model_PYTHIAMBR_01/Model_PYTHIAMBR_CrossectionSqrts_V2_visiblenewpythiauppercut_1.root"); //
TFile* file_Model_Epos = TFile::Open("Model_EPOS/Model_EPOS_CrossectionSqrts_V2_visiblenewpythiauppercut_1.root");
TFile* file_Model_Epos_199 = TFile::Open("Model_EPOS_1.99/Model_EPOS_1.99_CrossectionSqrts.root");
TFile* file_Model_QGSJETII03 = TFile::Open("Model_QGSJETII-03/model_QGSJETII03_CrossectionSqrts.root");
TFile* file_Model_QGSJETII04 = TFile::Open("Model_QGSJETII-04/Model_QGSJETII04_CrossectionSqrts.root");
TFile* file_Model_PythiE08 = TFile::Open("Model_PYTHIAMBR_Epsilom008/model_pythiambr_epsilom008_CrossectionSqrts.root");
TFile* file_Model_Model_Sibyll_23 = TFile::Open("Model_Sibyll_2.3/model_Sibyll_2.3_CrossectionSqrts.root");


TGraphErrors* Gen_XC_SD = (TGraphErrors*)file_Model->Get("crosssection_SD");
TGraphErrors* Gen_XC_SD_epos = (TGraphErrors*)file_Model_Epos->Get("crosssection_SD");                  
TGraphErrors* Gen_XC_SD_epos_199 = (TGraphErrors*)file_Model_Epos_199->Get("crosssection_SD");        
TGraphErrors* Gen_XC_SD_PythiE08 = (TGraphErrors*)file_Model_PythiE08->Get("crosssection_SD");
TGraphErrors* Gen_XC_SD_QGSJETII04 = (TGraphErrors*)file_Model_QGSJETII04->Get("crosssection_SD");
TGraphErrors* Gen_XC_SD_QGSJETII03 = (TGraphErrors*)file_Model_QGSJETII03->Get("crosssection_SD");
TGraphErrors* Gen_XC_SD_Sibyll_23 = (TGraphErrors*)file_Model_Model_Sibyll_23->Get("crosssection_SD");


TGraphErrors* Gen_XC_DD = (TGraphErrors*)file_Model->Get("crosssection_DD");
TGraphErrors* Gen_XC_DD_epos = (TGraphErrors*)file_Model_Epos->Get("crosssection_DD"); 

TGraphErrors* Gen_XC_DD_epos_199 = (TGraphErrors*)file_Model_Epos_199->Get("crosssection_DD"); 
TGraphErrors* Gen_XC_DD_PythiE08 = (TGraphErrors*)file_Model_PythiE08->Get("crosssection_DD");
TGraphErrors* Gen_XC_DD_QGSJETII04 = (TGraphErrors*)file_Model_QGSJETII04->Get("crosssection_DD");
TGraphErrors* Gen_XC_DD_QGSJETII03 = (TGraphErrors*)file_Model_QGSJETII03->Get("crosssection_DD");
TGraphErrors* Gen_XC_DD_Sibyll_23 = (TGraphErrors*)file_Model_Model_Sibyll_23->Get("crosssection_DD");


TCanvas *c1 = new TCanvas("c1","multigraphSD",600,400);
// c1->SetGrid();
TCanvas *c2 = new TCanvas("c2","multigraphDD",600,400);
// c2->SetGrid()
TMultiGraph *mg_dd = new TMultiGraph();
TMultiGraph *mg_sd = new TMultiGraph();

mg_sd->SetTitle("SD;#sqrt{s} (GeV); #sigma^{SD}(mb)");
mg_dd->SetTitle("DD;#sqrt{s} (GeV); #sigma^{DD}(mb)");
mg_sd->SetMaximum(25);
mg_dd->SetMaximum(25);
   

Gen_XC_SD->SetTitle("PYTHIA");
Gen_XC_SD->SetLineStyle(1);
Gen_XC_SD->SetLineColor(1);
Gen_XC_SD->SetLineWidth(3);

Gen_XC_SD_PythiE08->SetLineStyle(2);
Gen_XC_SD_PythiE08->SetLineColor(1);
Gen_XC_SD_PythiE08->SetLineWidth(3);

Gen_XC_SD_QGSJETII04->SetLineStyle(4);
Gen_XC_SD_QGSJETII04->SetLineColor(kGreen+2);
Gen_XC_SD_QGSJETII04->SetLineWidth(3);

Gen_XC_SD_QGSJETII03->SetLineStyle(5);
Gen_XC_SD_QGSJETII03->SetLineColor(kGreen+2);
Gen_XC_SD_QGSJETII03->SetLineWidth(3);

// for (Int_t i = 0 ; i < Gen_XC_SD_epos->GetN(); i++) {
//       // Int_t ipoint = GetN();
//       y[25] = Gen_XC_SD->GetY()[i];
//     for Int_t j = 0 ; j < Gen_XC_SD_epos->GetN(); j++) 
//       x[0] = Gen_XC_SD->GetX()[j];

//  Gen_XC_SD_epos->SetPoint(0,x[j], y[i])

//   } 

Gen_XC_SD_epos->SetLineStyle(6);
Gen_XC_SD_epos->SetLineColor(kOrange+2);
Gen_XC_SD_epos->SetLineWidth(3);

Gen_XC_SD_epos_199->SetLineStyle(7);
Gen_XC_SD_epos_199->SetLineColor(kOrange+2);
Gen_XC_SD_epos_199->SetLineWidth(3);

Gen_XC_SD_Sibyll_23->SetLineStyle(8);
Gen_XC_SD_Sibyll_23->SetLineColor(kYellow+1);
Gen_XC_SD_Sibyll_23->SetLineWidth(3);

Gen_XC_DD_Sibyll_23->SetLineStyle(8);
Gen_XC_DD_Sibyll_23->SetLineColor(kYellow+1);
Gen_XC_DD_Sibyll_23->SetLineWidth(3);

Gen_XC_DD->SetTitle("PYTHIA");
Gen_XC_DD->SetLineStyle(1);
Gen_XC_DD->SetLineColor(1);
Gen_XC_DD->SetLineWidth(3);

Gen_XC_DD_PythiE08->SetLineStyle(2);
Gen_XC_DD_PythiE08->SetLineColor(1);
Gen_XC_DD_PythiE08->SetLineWidth(3);

Gen_XC_DD_QGSJETII04->SetLineStyle(4);
Gen_XC_DD_QGSJETII04->SetLineColor(kGreen+2);
Gen_XC_DD_QGSJETII04->SetLineWidth(3);

Gen_XC_DD_QGSJETII03->SetLineStyle(5);
Gen_XC_DD_QGSJETII03->SetLineColor(kGreen+2);
Gen_XC_DD_QGSJETII03->SetLineWidth(3);




Gen_XC_DD_epos->SetTitle("EPOS");
Gen_XC_DD_epos->SetLineStyle(6);
Gen_XC_DD_epos->SetLineColor(kOrange+2);
Gen_XC_DD_epos->SetLineWidth(3);

Gen_XC_DD_epos_199->SetLineStyle(7);
Gen_XC_DD_epos_199->SetLineColor(kOrange+2);
Gen_XC_DD_epos_199->SetLineWidth(3);


//SD
//CMS
const Int_t ncms = 1;
Double_t CMcms[ncms] = {7000};
Double_t x_sd_cms[ncms]   = {8.84 };
Double_t exl_cms[ncms] = {sqrt(0.08*0.08 + 1.49*1.49 + 1.17*1.17) };
//Double_t exl_cms[ncms] = {sqrt(0.08*0.08 + 1.49*1.49 + 1.17+1.17) };
// Double_t exl_dd[ndd] = {2.0,sqrt(3.2*3.2+.2*0.2),sqrt(2.6*2.6+.3*.0.3)};
TGraphErrors *gcms = new TGraphErrors(ncms,CMcms,x_sd_cms,0,exl_cms);
 // new TGraphAsymmErrors(n,x,y,exl,exh,eyl,eyh);

// gcms->SetTitle("ALICE");
// gcms->SetMarkerColor(4);
gcms->SetLineColor(2);
gcms->SetLineWidth(2);
gcms->SetMarkerStyle(24);
gcms->SetMarkerSize(1);
gcms->SetMarkerColor(2);

//CMS
const Int_t Nbrsd_cms = 1;
Double_t CM_cms_M[Nbrsd_cms] = {13000};
Double_t x_sd_cms_M[Nbrsd_cms] = {6.9520673198};
Double_t x_sd_error_model_min[Nbrsd_cms] = {0.3622769364};
Double_t x_sd_error_model_max[Nbrsd_cms] = {0.3622769364};
Double_t x_sd_extrpol_max[Nbrsd_cms]= {1.3395589462};
Double_t x_sd_extrpol_min[Nbrsd_cms]= {1.516025725};
Double_t exl_cms_M[Nbrsd_cms] = {sqrt(0.0767049655)}; //allsyssqr
// cout<<"x_sd_extrpol_min: " << x_sd_extrpol_min[0] << endl;
// cout<<"x_sd_extrpol_max: " << x_sd_extrpol_max[0] << endl;
cout<<"systematic_SD: " << sqrt(0.0767049655) << endl;

 for (int incms_sd = 0; incms_sd<Nbrsd_cms; ++incms_sd) {
     x_sd_sys_max[incms_sd] = sqrt(pow(x_sd_extrpol_max[incms_sd],2) + pow(exl_cms_M[incms_sd],2) + pow(x_sd_cms_M[incms_sd]*0.023,2) +pow(x_sd_error_model_max[incms_sd],2) );
     x_sd_sys_min[incms_sd] = sqrt(pow(x_sd_extrpol_min[incms_sd],2) + pow(exl_cms_M[incms_sd],2)+ pow(x_sd_cms_M[incms_sd]*0.023,2)+pow(x_sd_error_model_min[incms_sd],2));
  
  }

cout<<"x_sd_sys_max: "<<x_sd_sys_max[0]<< endl;
cout<<"x_sd_sys_min: "<< x_sd_sys_min[0]<< endl; 

TGraphAsymmErrors *gcms_M = new TGraphAsymmErrors(Nbrsd_cms, CM_cms_M, x_sd_cms_M, 0, 0, x_sd_sys_min, x_sd_sys_max);
gcms_M->SetLineColor(2);
gcms_M->SetMarkerStyle(20);
gcms_M->SetMarkerSize(1.5);
gcms_M->SetMarkerColor(2);
gcms_M->SetLineWidth(2);


//ALICE

const Int_t n = 3;
Double_t CM[n] = {900, 2760, 7000};
Double_t x_sd[n]   = {11.2,12.2 ,14.9 };
Double_t exl[n] = {sqrt(1.6*1.6),sqrt(3.9*3.9+0.2*0.2) ,sqrt(3.4*3.4+0.5*0.5)};
// Double_t exl_dd[ndd] = {2.0,sqrt(3.2*3.2+.2*0.2),sqrt(2.6*2.6+.3*.0.3)};
TGraphErrors *gr = new TGraphErrors(n, CM, x_sd, 0, exl);
 // new TGraphAsymmErrors(n,x,y,exl,exh,eyl,eyh);

gr->SetTitle("ALICE");
// gr->SetMarkerColor(4);
gr->SetMarkerStyle(24);
gr->SetMarkerSize(1);
gr->SetMarkerColor(4);
gr->SetLineColor(4);
gr->SetLineWidth(2);


 // create second graph CHLM
const Int_t n1 = 6;
Double_t S_chlm[n1] = {549, 725, 934,1047,1239,1464};
Double_t CM_1[n1];
for (int ichlm=0; ichlm<n1; ++ichlm) CM_1[ichlm] = sqrt(S_chlm[ichlm]); 

Double_t X_sd_1[n1] = {6.07, 6.05, 6.37, 6.32, 7.01,6.80};
Double_t exl_sd_1[n1] = {.17,.22,.15,.22,.28,.29};

TGraphErrors *gr1 = new TGraphErrors(n1,CM_1, X_sd_1, 0, exl_sd_1 );
gr1->SetName("gr1");
gr1->SetTitle("CHLM (ISR)");
// gr1->SetMarkerColor(1);
gr1->SetMarkerStyle(30);
gr1->SetMarkerSize(1);
gr1->SetMarkerColor(1);
gr1->SetLineWidth(2);

//UA4 collaboration
const Int_t nUA4 = 1;
Double_t CM_UA4[nUA4]= {546};
Double_t XS_UA4[nUA4]= {9.4};
Double_t exl_UA4[nUA4]= {.7};
// Double_t exh_UA4[nUA4]= {.-7};
TGraphErrors* grUA4 = new TGraphErrors(nUA4, CM_UA4, XS_UA4, 0, exl_UA4);
grUA4->SetName("grUA4");
grUA4->SetTitle("UA4");
// grUA4->SetMarkerColor(6);
grUA4->SetMarkerStyle(25);
grUA4->SetMarkerSize(1.2);
// grUA4->SetMarkerColor(2);

//ISR
const Int_t nISR = 8;
Double_t S_ISR[nISR]= {550, 750, 1050, 1260, 1480, 2000, 2880, 3880}; // squareroot
Double_t CM_ISR[nISR];
for (int iisr=0; iisr<nISR; ++iisr) CM_ISR[iisr] = sqrt(S_ISR[iisr]); 
Double_t XS_ISR[nISR]= {6.5,6.3, 6.5, 7.5, 7.3, 7.3, 7.0, 7.5};
Double_t exl_ISR[nISR]= {.2,.2,.2,.5,.4, .3,.3,.3};
// Double_t exh_ISR[nISR]= {.2,.2,.2,.5,.4, .3,.3,.3};

TGraphErrors*grISR = new TGraphErrors(nISR, CM_ISR, XS_ISR,0, exl_ISR);
grISR->SetName("grISR");
grISR->SetTitle("Armitage et al.(ISR)");
grISR->SetMarkerColor(1);
grISR->SetMarkerStyle(22);
grISR->SetMarkerSize(1);
grISR->SetLineWidth(2);
// grUA4->SetMarkerColor(2


//GLM model


const Int_t nGLM = 5;
Double_t CM_GLM[nGLM]= {1800, 7000, 8000, 14000, 57000};
Double_t XS_GLM[nGLM]= {10.27, 14.88, 15.2, 17.31, 21.68 };
// Double_t eX_ISR[nISR]= {.,.2,.2,.5,.4, .3,.3,.3};

TGraphErrors*grGLM = new TGraphErrors(nGLM, CM_GLM, XS_GLM, 0, 0); //new TGraphErrors(n,x,y,ex,ey);
grGLM->SetName("grGLM");
grGLM->SetTitle("GLM");
grGLM->SetLineWidth(3);
grGLM->SetLineStyle(4);
// grISR->SetMarkerSize(2);
// grUA4->SetMarkerColor(2

///KP
const Int_t nkp = 5;
Double_t CM_kp[nkp] = {900, 2760, 7000, 10000, 14000 };
Double_t X_sd_kp[nkp] = {9.3, 11.2, 12.9, 13.6, 14.3};
Double_t exl_sd_kp[nkp] = {};
TGraphErrors *grkp = new TGraphErrors(nkp,CM_kp, X_sd_kp, 0,0 );
grkp->SetName("grkp");
grkp->SetTitle("KP");
// grkp->SetMarkerColor(1);
grkp->SetLineStyle(3);
grkp->SetLineWidth(3);
grkp->SetLineColor(1);
 



//CDF

const Int_t nCDF = 2;
Double_t CM_CDF[nCDF]= {546, 1800};
Double_t XS_CDF[nCDF]= {7.89, 9.46};
Double_t exl_CDF[nCDF]= {.33, .44};
// Double_t exh_CDF[nCDF] = {.33,.44};
// TGraphAsymmErrors *grCDF = new TGraphAsymmErrors(nCDF,CM_CDF, X_CDF,exl_CDF,exh_CDF,0,0);
TGraphErrors *grCDF = new TGraphErrors(nCDF,CM_CDF, XS_CDF,0,exl_CDF);
grCDF->SetName("grCDF");
grCDF->SetTitle("CDF");
grCDF->SetMarkerColor(1);
grCDF->SetMarkerStyle(20);
grCDF->SetMarkerSize(1);
grCDF->SetLineWidth(2);
// grUA4->SetMarkerColor(2






// DD
//CMS
const Int_t ncms_DD = 1;
Double_t CMcms_DD[ncms] = {7000};
Double_t xcms_DD[ncms_DD]   = {5.17};
Double_t exlcms_DD[ncms_DD] = {sqrt(0.08*0.08 + 0.55*0.55 + 1.62*1.62)};
// Double_t exh_dd[ncms_DD] = {.0,.2,.3};
TGraphErrors *gcms_DD = new TGraphErrors(ncms_DD, CMcms_DD, xcms_DD, 0,exlcms_DD);
gcms_DD->SetMarkerStyle(24);
gcms_DD->SetMarkerSize(1.2);
gcms_DD->SetMarkerColor(2);
gcms_DD->SetLineColor(2);
gcms_DD->SetLineWidth(2);

const Int_t ncms_DD_M = 1;
Double_t CMcms_DD_M[ncms_DD_M] = {13000};
// Double_t xcms_DD_M[ncms_DD_M] = {6.1281*1.679704829};
// double x_dd_error_model_min[ncms_DD_M] = {6.1281-(6.97678)}; 
// double x_dd_error_model_max[ncms_DD_M] = {(8.0312) - 6.1281};
// double x_dd_errorextrpol_max[ncms_DD_M]= {6.1281 * (2.762486-1.679704829)};
// double x_dd_sys_min[ncms_DD_M]= {6.1281 * (1.679704829 - 1.343341)};
// Double_t exlcms_DD_M[ncms_DD_M] = {sqrt( (0.0023459*0.0023459)+ (0.0090559*0.0090559)+ (-0.0163701*-0.0163701)+ (-0.0625601*-0.0625601)+ (0.03046815*0.03046815))};

Double_t xcms_DD_M[ncms_DD_M] = {10.3573291884};
Double_t x_dd_error_model_min[ncms_DD_M] = {0.6883250437};
Double_t x_dd_error_model_max[ncms_DD_M] = {0.6883250437};
Double_t x_dd_extrpol_max[ncms_DD_M]= {3.4955639833};
Double_t x_dd_extrpol_min[ncms_DD_M]= {1.7415609148};
Double_t exlcms_DD_M[ncms_DD_M] = {sqrt(0.0314930551)};//sys all sys
cout<<"systematic_DD:" << sqrt(0.0314930551) << endl;

for (int incms=0; incms<ncms_DD_M; ++incms) {
   x_dd_sys_max[incms] = sqrt(pow(x_dd_extrpol_max[incms],2) + pow(exlcms_DD_M[incms],2) + pow(xcms_DD_M[incms]*0.023,2) +pow(x_dd_error_model_max[incms],2));
   x_dd_sys_min[incms] = sqrt(pow(x_dd_extrpol_min[incms],2) + pow(exlcms_DD_M[incms],2)+ pow(xcms_DD_M[incms]*0.023,2)+pow(x_dd_error_model_min[incms],2));
}

cout<<"x_dd_sys_max: "<< x_dd_sys_max[0]<< endl;
cout<<"x_dd_sys_min: "<< x_dd_sys_min[0] << endl;



// Double_t exh_dd_M[ncms_DD_M] = {.0,.2,.3};
TGraphAsymmErrors *gcms_DD_M = new TGraphAsymmErrors(ncms_DD_M, CMcms_DD_M, xcms_DD_M, 0,0, x_dd_sys_min, x_dd_sys_max);

gcms_DD_M->SetMarkerStyle(20);
gcms_DD_M->SetMarkerSize(1.5);
gcms_DD_M->SetMarkerColor(2);
gcms_DD_M->SetLineColor(2);
gcms_DD_M->SetLineWidth(2);



//AlICE


const Int_t ndd = 3;
Double_t CM_alice[ndd] = {900, 2760, 7000};
Double_t x_dd[ndd]   = {5.6, 7.8 , 9.0 };
Double_t exl_dd[ndd] = {2.0,sqrt(3.2*3.2+.2*0.2),sqrt(2.6*2.6+.3*0.3)};
// Double_t exh_dd[ndd] = {.0,.2,.3};
TGraphErrors *gr_dd = new TGraphErrors(ndd, CM_alice, x_dd, 0,exl_dd);

gr_dd->SetMarkerStyle(24);
gr_dd->SetMarkerSize(1);
gr_dd->SetMarkerColor(4);
gr_dd->SetLineColor(4);
gr_dd->SetLineWidth(2);

const Int_t nGLM_dd = 5;
Double_t CM_GLM_dd[nGLM_dd]= {1800, 7000, 8000, 14000, 57000};
Double_t X_GLM_dd[nGLM_dd]= {5.5, 7.44, 7.61, 8.37, 13.14};
// Double_t eX_ISR[nISR]= {.,.2,.2,.5,.4, .3,.3,.3};

TGraphErrors*grGLM_dd = new TGraphErrors(nGLM_dd, CM_GLM_dd,X_GLM_dd, 0, 0);
grGLM_dd->SetName("grGLM_dd");
grGLM_dd->SetTitle("GLM");
grGLM_dd->SetLineWidth(3);
grGLM_dd->SetLineStyle(4);

const Int_t nkp_DD = 5;
Double_t CM_kp_DD[nkp_DD] = {900, 2760, 7000, 10000, 14000 };
Double_t X_sd_kp_DD[nkp_DD] = {5.7, 5.9, 6.1, 6.2, 6.4};
// Double_t exl_sd_kp_DD[nkp_DD] = {};
TGraphErrors *grkp_DD = new TGraphErrors(nkp_DD, CM_kp_DD, X_sd_kp_DD, 0, 0);
grkp_DD->SetName("grkp_DD");
grkp_DD->SetTitle("CHLM (ISR)");
// grkp_DD->SetMarkerColor(1);
grkp_DD->SetLineStyle(3);
grkp_DD->SetLineWidth(3);
grkp_DD->SetLineColor(1);
 


// gStyle->SetLegendTextSize(0.05); //nosuffix
// gStyle->SetLegendFont(42);

TLegend* leg_SD = new TLegend(0.09,0.36,0.47,0.86);
TLegend* leg_cut = new TLegend(0.44,0.4,0.7,0.92);
TLegend* leg_cut_DD = new TLegend(0.44,0.4,0.7,0.92);
SetLegendStyle(leg_SD);
SetLegendStyle(leg_cut);
SetLegendStyle(leg_cut_DD);
leg_cut->SetTextSize(0.05);
leg_cut_DD->SetTextSize(0.05);

// leg_SD->SetHeader("The Legend Title","C"); // option "C" allows to center the header
leg_SD->AddEntry(gr,"ALICE ","ep");
leg_SD->AddEntry(gr1,"CHLM (ISR)","ep");
leg_SD->AddEntry(grUA4,"UA4","ep");
leg_SD->AddEntry(gcms,"CMS 7 TeV","ep");
leg_SD->AddEntry(gcms_M,"CMS 13 TeV","ep");
leg_SD->AddEntry(grISR,"Armitage et al.(ISR)","ep");
leg_SD->AddEntry(grGLM,"GLM","l");
leg_SD->AddEntry(grCDF,"CDF","ep");

leg_SD->AddEntry(Gen_XC_SD,"PYTHIA-MBR(#xi = 0.08)","l");
leg_SD->AddEntry(Gen_XC_SD_PythiE08,"PYTHIA-MBR(#xi = 0.104)","l");
leg_SD->AddEntry(Gen_XC_SD_QGSJETII04,"QGSJETII-04 ","l");
leg_SD->AddEntry(Gen_XC_SD_QGSJETII03,"QGSJETII-03 ","l");
leg_SD->AddEntry(Gen_XC_SD_Sibyll_23,"Sibyll_2.3 ","l");
leg_SD->AddEntry(Gen_XC_SD_epos,"EPOS LHC","l");
leg_SD->AddEntry(Gen_XC_SD_epos_199,"EPOS_1.99 ","l");
leg_SD->AddEntry(grkp,"KP","l");
leg_cut->AddEntry(gcms,"#xi < 0.05","");
TLegend* leg_DD = new TLegend(0.10,0.42,0.48,0.86);
SetLegendStyle(leg_DD);
// leg_DD->SetHeader("The Legend Title","C"); // option "C" allows to center the header
leg_DD->AddEntry(gcms_DD,"CMS 7 TeV ","ep");
leg_DD->AddEntry(gcms_DD_M,"CMS 13 TeV","ep");
leg_DD->AddEntry(gr_dd,"ALICE ","ep");

leg_DD->AddEntry(Gen_XC_DD,"PYTHIA-MBR(#xi = 0.08)","l");
leg_DD->AddEntry(Gen_XC_DD_PythiE08,"PYTHIA-MBR(#xi = 0.104) ","l");
leg_DD->AddEntry(Gen_XC_DD_QGSJETII04,"QGSJETII-04 ","l");
leg_DD->AddEntry(Gen_XC_DD_QGSJETII03,"QGSJETII-03 ","l");
leg_DD->AddEntry(Gen_XC_DD_Sibyll_23,"Sibyll_2.3 ","l");
leg_DD->AddEntry(Gen_XC_DD_epos,"EPOS LHC ","l");
leg_DD->AddEntry(Gen_XC_DD_epos_199,"EPOS_1.99 ","l");
leg_DD->AddEntry(grGLM_dd,"GLM","l");
leg_DD->AddEntry(grkp_DD,"KP","l");
leg_cut_DD->AddEntry(gcms_DD,"#Delta#eta > 3","");


c1->cd();
c1->SetLogx();
mg_sd->Add(gcms,"p");
mg_sd->Add(gcms_M,"p");
mg_sd->Add(gr, "p");
mg_sd->Add(gr1, "p");
mg_sd->Add(grUA4, "p");
mg_sd->Add(grISR, "p");
mg_sd->Add(grCDF, "p");
mg_sd->Add(grkp, "l");
mg_sd->Add(grGLM, "l");
mg_sd->Add(Gen_XC_SD, "l");
mg_sd->Add(Gen_XC_SD_PythiE08, "l");
mg_sd->Add(Gen_XC_SD_QGSJETII04, "l");
mg_sd->Add(Gen_XC_SD_QGSJETII03, "l");
mg_sd->Add(Gen_XC_SD_epos, "l");
mg_sd->Add(Gen_XC_SD_epos_199, "l");
mg_sd->Add(Gen_XC_SD_Sibyll_23, "l");
mg_sd->Draw("a");
// c1->BuildLegend();
leg_SD->Draw();
leg_cut->Draw();


c2->cd();
c2->SetLogx();

mg_dd->Add(gcms_DD, "p");
mg_dd->Add(gcms_DD_M, "p");
mg_dd->Add(gr_dd, "p");
mg_dd->Add(grGLM_dd, "l");
mg_dd->Add(grkp_DD, "l");
mg_dd->Add(Gen_XC_DD_PythiE08, "l");
mg_dd->Add(Gen_XC_DD_QGSJETII04, "l");
mg_dd->Add(Gen_XC_DD_QGSJETII03, "l");
mg_dd->Add(Gen_XC_DD_epos, "l");
mg_dd->Add(Gen_XC_DD, "l");
mg_dd->Add(Gen_XC_DD_epos_199, "l");
mg_dd->Add(Gen_XC_DD_Sibyll_23, "l");
mg_dd->Draw("a");
leg_DD->Draw();
leg_cut_DD->Draw();




}
