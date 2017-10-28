#ifndef __include__myhistograms_h__
#define __include__myhistograms_h__


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
 ////////////////////////////////////////////////////////////////// 
 
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

 


/// SYSTematic//////////////////////
 
  // mSingleTrainingVar["NTowHF_plus_sys"].hist_name = "Hist_numberoftowerebovenoise_forwardplus";
  // mSingleTrainingVar["NTowHF_plus_sys"].xaxis_title = "N_{tow}";
  // mSingleTrainingVar["NTowHF_plus_sys"].yaxis_title = "1/L dN/dN_{tow} [#mub^{-1}]";
  // mSingleTrainingVar["NTowHF_plus_sys"].ratio_title = "MC / Data";
  // mSingleTrainingVar["NTowHF_plus_sys"].canvas_title = "cNTowHF_plus_sys_";
  // mSingleTrainingVar["NTowHF_plus_sys"].xmin = -0.5;
  // mSingleTrainingVar["NTowHF_plus_sys"].xmax = 100.5;
  // mSingleTrainingVar["NTowHF_plus_sys"].ymin = 1;
  // mSingleTrainingVar["NTowHF_plus_sys"].ymax = 1e5;
  // mSingleTrainingVar["NTowHF_plus_sys"].rmin = 0;
  // mSingleTrainingVar["NTowHF_plus_sys"].rmax = 3;
  // mSingleTrainingVar["NTowHF_plus_sys"].cms_alignment = 33;


  // mSingleTrainingVar["NTowHF_minus_sys"].hist_name = "Hist_numberoftowerebovenoise_forwardminus";
  // mSingleTrainingVar["NTowHF_minus_sys"].xaxis_title = "N_{tow}";
  // mSingleTrainingVar["NTowHF_minus_sys"].yaxis_title = "1/L dN/dN_{tow} [#mub^{-1}]";
  // mSingleTrainingVar["NTowHF_minus_sys"].ratio_title = "MC / Data";
  // mSingleTrainingVar["NTowHF_minus_sys"].canvas_title = "cNTowHF_minus_sys_";
  // mSingleTrainingVar["NTowHF_minus_sys"].xmin = -0.5;
  // mSingleTrainingVar["NTowHF_minus_sys"].xmax = 100.5;
  // mSingleTrainingVar["NTowHF_minus_sys"].ymin = 1;
  // mSingleTrainingVar["NTowHF_minus_sys"].ymax = 1e5;
  // mSingleTrainingVar["NTowHF_minus_sys"].rmin = 0;
  // mSingleTrainingVar["NTowHF_minus_sys"].rmax = 3;
  // mSingleTrainingVar["NTowHF_minus_sys"].cms_alignment = 33;

 
  // mSingleTrainingVar["NTowMaxCastor_sys"].hist_name = "Hist_MaxCastorEnergy";
  // mSingleTrainingVar["NTowMaxCastor_sys"].xaxis_title = "Max_{energytow}";
  // mSingleTrainingVar["NTowMaxCastor_sys"].yaxis_title = "1/L dN/dN_{tow} [#mub^{-1}]";
  // mSingleTrainingVar["NTowMaxCastor_sys"].ratio_title = "MC / Data";
  // mSingleTrainingVar["NTowMaxCastor_sys"].canvas_title = "cEnergyMaxTowCastor_";
  // mSingleTrainingVar["NTowMaxCastor_sys"].xmin = 0;
  // mSingleTrainingVar["NTowMaxCastor_sys"].xmax = 600;
  // mSingleTrainingVar["NTowMaxCastor_sys"].ymin = 1;
  // mSingleTrainingVar["NTowMaxCastor_sys"].ymax = 1e5;
  // mSingleTrainingVar["NTowMaxCastor_sys"].rmin = 0;
  // mSingleTrainingVar["NTowMaxCastor_sys"].rmax = 3;
  // mSingleTrainingVar["NTowMaxCastor_sys"].cms_alignment = 33;
 

  // mSingleTrainingVar["NTowCastor_sys"].hist_name = "Hist_numberoftowerebovenoise_castor";
  // mSingleTrainingVar["NTowCastor_sys"].xaxis_title = "N_{tow}";
  // mSingleTrainingVar["NTowCastor_sys"].yaxis_title = "1/L dN/dN_{tow} [#mub^{-1}]";
  // mSingleTrainingVar["NTowCastor_sys"].ratio_title = "MC / Data";
  // mSingleTrainingVar["NTowCastor_sys"].canvas_title = "cNTowCastor_";
  // mSingleTrainingVar["NTowCastor_sys"].xmin = 0;
  // mSingleTrainingVar["NTowCastor_sys"].xmax = 16;
  // mSingleTrainingVar["NTowCastor_sys"].ymin = 500;
  // mSingleTrainingVar["NTowCastor_sys"].ymax = 1e5;
  // mSingleTrainingVar["NTowCastor_sys"].rmin = 0;
  // mSingleTrainingVar["NTowCastor_sys"].rmax = 2.2;
  // mSingleTrainingVar["NTowCastor_sys"].cms_alignment = 33;

 
  // mSingleTrainingVar["NTracks_sys"].hist_name = "Hist_NbrTracks";
  // mSingleTrainingVar["NTracks_sys"].xaxis_title = "N_{trk}";
  // mSingleTrainingVar["NTracks_sys"].yaxis_title = "1/L dN/dN_{trk} [#mub^{-1}]";
  // mSingleTrainingVar["NTracks_sys"].ratio_title = "MC / Data";
  // mSingleTrainingVar["NTracks_sys"].canvas_title = "cNTracks_sys_";
  // mSingleTrainingVar["NTracks_sys"].xmin = 0;
  // mSingleTrainingVar["NTracks_sys"].xmax = 50;
  // mSingleTrainingVar["NTracks_sys"].ymin = 700;
  // mSingleTrainingVar["NTracks_sys"].ymax = 1e4;
  // mSingleTrainingVar["NTracks_sys"].rmin = 0;
  // mSingleTrainingVar["NTracks_sys"].rmax = 1.5;
  // mSingleTrainingVar["NTracks_sys"].cms_alignment = 33;

  
  ////////////////////////////////////////////////////////////////////////



 
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

#endif