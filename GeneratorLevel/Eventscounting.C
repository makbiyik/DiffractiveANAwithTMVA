
 //or iostream.h
#include <cmath> // or math.h
#include "TNtuple.h"
#include <TH1D.h>
#include <TCanvas.h>
#include <TChain.h>
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
using namespace std;


int Eventscounting(int sample = 8){

  


 string sampleNames[] = {"EPOS_LHC","EPOS_1.99","Pythia_6.115","Sibyll_2.3",
                        "QGSJETII-04","QGSJETII-03","model_PythiaTune_MBR_Eps008",
                        "PythiaTune_MBR_Eps0104","PythiaTune_MBR_Eps007","PythiaTune_MBR_Eps0104_alpha0125"};
 
    // # model [0=EPOS_LHC, 1=EPOS_1.99, 
    // # 2=QGSJET01 (it has error analysis code cant read the files), 3=Gheisha, 4=Pythia_6.115, 
    // # 5=Hijing_1.38, 6=Sibyll_2.3, 
    // # 7=QGSJETII-04, 8=Phojet, 11=QGSJETII-03, 
    // # 12=DPMJet 3.0-6]

 bool data = 0;
  bool mix = 0;

// for (Int_t j = 0 ; j < sampleNames[sample].size(); j++) {


 TH1::SetDefaultSumw2();
  using namespace std;
  
  TFile* inf, *outf;

  cout<<"Sample : "<<sample<<endl;
  cout<<"Sample name is : "<<sampleNames[sample].data()<<endl;
  cout<<"Output file name : "<<Form("Eventselection_%s",sampleNames[sample].data() )<<endl;
  outf = new TFile(Form("Eventselection_%s",sampleNames[sample].data() ),"recreate");

  if(sample ==0){
  inf = TFile::Open("CM1300/model_0_1300_CrossectionSqrts.root");  
  }else if (sample == 1){
  inf = TFile::Open("CM1300/model_1_1300_CrossectionSqrts.root");
  }else if (sample == 2){
  inf = TFile::Open("CM1300/model_4_1300_CrossectionSqrts.root");
  
  }else if (sample == 3){
  inf = TFile::Open("CM1300/model_6_1300_CrossectionSqrts.root");
  
  }else if (sample == 4){
  inf = TFile::Open("CM1300/model_7_1300_CrossectionSqrts.root");
  
  }else if (sample == 5){
  inf = TFile::Open("CM1300/model_11_1300_CrossectionSqrts.root");
  
  }else if (sample == 6){
  inf = TFile::Open("CM1300/model_PythiaTune_MBR_Sqrts_CM_1300_eps_008.root");
  
  }else if (sample == 7){
  inf = TFile::Open("CM1300/model_PythiaTune_MBR_Sqrts_CM_1300_eps_0104.root");
  
  }else if (sample == 8){
  inf = TFile::Open("CM1300/model_pythiaTune_MBR_Sqrts_CM_1300_eps_0.07_CrossectionSqrts.root");
  }else if (sample == 9){
  inf = TFile::Open("CM1300/model_PythiaTune_MBR_Sqrts_CM_1300_eps_0104_lalpha0125.root");
  }


double crosssection_SD = 0;
double crosssection_visible_SD = 0;
double crosssection_DD = 0;
double crosssection_visible_DD = 0;


TGraphErrors* Gen_XC_SD = (TGraphErrors*)inf->Get("crosssection_SD");
TGraphErrors* Gen_XC_visibleSD = (TGraphErrors*)inf->Get("crosssection_visible_SD");

TGraphErrors* Gen_XC_DD = (TGraphErrors*)inf->Get("crosssection_DD");
TGraphErrors* Gen_XC_visibleDD = (TGraphErrors*)inf->Get("crosssection_visible_DD");
// Double_t * exl = Gen_XC_visibleDD->GetErrorX();


  
outf->cd();
Double_t n[10];
     
  for (Int_t i = 0 ; i < Gen_XC_SD->GetN(); i++) {
      // Int_t ipoint = GetN();
      n[0] = Gen_XC_SD->GetY()[i];
      cout<<"SD is: "<<n[0]<< endl;

  } 

  for (Int_t i = 0 ; i < Gen_XC_visibleSD->GetN(); i++) {
      // Int_t ipoint = GetN();
      n[1] = Gen_XC_visibleSD->GetY()[i];
      cout<<"visible SD is: "<<n[1]<< endl;
  } 

  for (Int_t i = 0 ; i < Gen_XC_DD->GetN(); i++) {
      // Int_t ipoint = GetN();
      n[2] = Gen_XC_DD->GetY()[i];
      cout<<"DD is: "<<n[2]<< endl;
  } 

  for (Int_t i = 0 ; i < Gen_XC_visibleDD->GetN(); i++) {
      // Int_t ipoint = GetN();
      n[3] = Gen_XC_visibleDD->GetY()[i];
      cout<<"visible DD is: "<<n[3]<< endl;

  } 


cout<<"SD is: "<<n[0]<< endl;
cout<<"SD visible is: "<<n[1]<< endl;
cout <<"n[0]/n[1] is: "<< ((double)n[0])/n[1]<< endl;

cout<<"DD is: "<<n[2]<< endl;
cout<<"DD visible is: "<<n[3]<< endl;
cout <<"DD : n[2]/n[3] is: "<< ((double)n[2])/n[3]<< endl;

// }


  
return 0;
}

 int main() { // for  repeating

  for(int i = 1; i < 9; ++i){ 
  string selections[]= {"EPOS_LHC","EPOS_1.99","Pythia_6.115","Sibyll_2.3"};

  cout <<"|"<<selections[i].data()<<"|"<< ((double)n[i-1])/n[i]<< "|"<<((double)n[i])/n[0]<<"|"<< endl; 
  }
  
}