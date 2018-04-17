#!/usr/bin/env python



#from rootpy.math.physics.vector import LorentzVector
import sys, os, time
sys.path.append(os.path.dirname(__file__))
sys.path.append('/cr/data01/castor/') 


import ROOT
ROOT.gROOT.SetBatch(True)
import ParticleDataTool as pd
from collections import Counter
from bad_calo_channel_list import bad_channels_eta_phi_Run247934_20

# from ROOT import edm

from array import *
import math
import numpy as np
from math import cosh
from math import sinh
from math import sin
from math import cos
from math import tan
from math import pi
from math import sqrt
from math import log10
from math import atan
from math import exp
import random

import CommonFSQFramework.Core.ExampleProofReader

EventSelection_with_Xi = True
Training_Signal = "DD"
# self.HF_energy_scale =1.00#0.9 #1.1

# ParameterSet = 'Seb_dNdEta_LHCf'
# ParameterSet = 'Melike_dNdEta'
# ParameterSet = 'Seb_LHCf_Run247934'
# ParameterSet = 'MC'




def compareTracketa(first,second):
    if first[0] > second[0]: return 1
    if first[0] == second[0]: return 0
    # if first[0] < second[0]: return -1
    return -1


def compareGeneta(first,second):
    if first[0].eta() > second[0].eta(): return 1
    if first[0].eta() == second[0].eta(): return 0
    # if first[0].eta() < second[0].eta(): return -1
    return -1
def compareGenrapidity(first,second):
    if first[0].Rapidity() > second[0].Rapidity(): return 1
    if first[0].Rapidity() == second[0].Rapidity(): return 0
    # if first[0].Rapidity() < second[0].Rapidity(): return -1
    return -1
class DiffractiveAndTrack(CommonFSQFramework.Core.ExampleProofReader.ExampleProofReader):

    def getIEtaIPhifromTower():
        pass



    def CreateTree(self,tree):
        tree.Branch('EventselectionXiprocessDD', self.OUTEventselectionXiprocessDD,'EventselectionXiprocessDD/I')
        tree.Branch('EventselectionXiprocessSD1', self.OUTEventselectionXiprocessSD1,'EventselectionXiprocessSD1/I')
        tree.Branch('EventselectionXiprocessSD2', self.OUTEventselectionXiprocessSD2,'EventselectionXiprocessSD2/I')
        tree.Branch('EventselectionXiprocessRest', self.OUTEventselectionXiprocessRest,'EventselectionXiprocessRest/I')
        tree.Branch('Pythia8processid', self.OUTPythia8processid,'Pythia8processid/I')
        tree.Branch('log10XixGen', self.OUTlog10XixGen,'log10XixGen/F')
        tree.Branch('log10XiyGen', self.OUTlog10XiyGen,'log10XiyGen/F')
        tree.Branch('log10XixReco', self.OUTlog10XixReco,'log10XixReco/F')
        tree.Branch('log10XiyReco', self.OUTlog10XiyReco,'log10XiyReco/F')
        tree.Branch('DeltaEtaZero', self.OUTdeltazero,'deltazero/F')
        tree.Branch('ForwardDeltaEta', self.OUTforwardgendelta, 'forwardgendelta/F')
        tree.Branch('DeltaEta', self.OUTdeltaeta,'deltaeta/F')
        tree.Branch('EtaMin', self.OUTetamin, 'etamin/F')
        tree.Branch('RGmean', self.OUTrapditygapmean,'RGmean/F')
        tree.Branch('EtaMax', self.OUTetamax, 'etamax/F')
        tree.Branch('NbrTowersCastor', self.OUTCastorNtowers, 'CastorNtowers/I')
        tree.Branch('NbrTowersHFminus', self.OUTHFminusNtowers,'HFminusNtowers/I')
        tree.Branch('NbrTowersHFplus', self.OUTHFplusNtowers,'HFplusNtowers/I')
        tree.Branch('NbrTracks', self.OUTNtracks,'Ntracks/I')
        tree.Branch('Etarange', self.OUTEtarange, 'Etarange/I')
        tree.Branch('log10XiDD', self.OUTlog10XiDD,'log10XiDD/F')
        tree.Branch('NbrAllTowers', self.OUTCaloReducedenergyClass,'CaloReducedenergyClass/I')
        tree.Branch('SumEnergyCastor', self.OUTCastorSumEnergy, 'CastorSumEnergy/F')
        tree.Branch('SumEnergyHFPlus', self.OUTHFPlusSumEnergy, 'HFPlusSumEnergy/F')  
        tree.Branch('HottestTowerMaxHFPlus', self.OUTMaxHFPlusEnergy, 'MaxHFPlusEnergy/F')
        tree.Branch('SumEnergyHFMinus', self.OUTHFMinusSumEnergy, 'HFMinusSumEnergy/F')  
        tree.Branch('HottestTowerHFMinus', self.OUTMaxHFMinusEnergy, 'MaxHFMinusEnergy/F')
        tree.Branch('HottestTowerCastor', self.OUTMaxCastorEnergy, 'MaxCastorEnergy/F')


        # tree.Branch('EvtWeight', self.OUTEvtWeight, 'EvtWeight/F')

        
    

#    def init(self, ParameterSet="MC", HF_energy_scale=1.0, Track_efficiency_scale=1.0, maxEvents = None):
    def init(self, maxEvents = None):
        
        self.maxEvents = maxEvents

        self.hist = {}
        self.hist["hNentries"] = ROOT.TH1F("hNentries","hNentries",10, 0, 20)
        self.hist["hNentriesWithEventSelectionCuts"] = ROOT.TH1F("NentriesWithEventSelectionCuts","NentriesWithEventSelectionCuts",10, 0, 20)

        self.hist["hProcessesIdPythia"] = ROOT.TH1F("hProcessesIdPythia","hProcessesIdPythia",10, 0, 20)
        self.hist["hProcessescut"] = ROOT.TH1F("hProcessescut","hProcessescut",10, 0, 20)
        self.hist["hProcessescuteta"] = ROOT.TH1F("hProcessescuteta","hProcessescuteta",10, 0, 20)
        self.hist["BunchCrossing"] = ROOT.TH1F("BunchCrossing", "BunchCrossing",  3600, 0-0.5, 3600-0.5)
        self.hist["Runs"] =  ROOT.TH1F("Runs", "Runs",  2000, 246000-0.5, 282000-0.5)
        self.hist["HFEnergy"] = ROOT.TH1F("HFEnergy","HFEnergy",100, 0, 200)
        
        
        nBinSumE =250; 
        MinSumE = 0 ; 
        MaxSumE = 2500;
       

        ntow= 101
        Mintow=-0.5
        Maxtow= 100.5
        self.hist["Hist_sum_CAS_E"] = ROOT.TH1F("Hist_sum_CAS_E", "Hist_sum_CAS_E" ,nBinSumE, MinSumE, MaxSumE)
        self.hist["Hist_HFPlusSumEnergy"] = ROOT.TH1F("Hist_HFPlusSumEnergy", "Hist_HFPlusSumEnergy" ,nBinSumE,MinSumE, MaxSumE)
        self.hist["Hist_HFMinusSumEnergy"] = ROOT.TH1F("Hist_HFMinusSumEnergy", "Hist_HFMinusSumEnergy" ,nBinSumE,MinSumE, MaxSumE)
        self.hist["Hist_CastorSumEnergy"] = ROOT.TH1F("Hist_CastorSumEnergy", "Hist_CastorSumEnergy" ,nBinSumE,MinSumE, MaxSumE)  
        self.hist["Hist_eventXiID_HFPlusSumEnergy"] = ROOT.TH1F("Hist_eventXiID_HFPlusSumEnergy", "Hist_eventXiID_HFPlusSumEnergy" ,nBinSumE,MinSumE, MaxSumE)
        self.hist["Hist_eventXiID_HFMinusSumEnergy"] = ROOT.TH1F("Hist_eventXiID_HFMinusSumEnergy", "Hist_eventXiID_HFMinusSumEnergy" ,nBinSumE,MinSumE, MaxSumE)
        self.hist["Hist_eventXiID_CastorSumEnergy"] = ROOT.TH1F("Hist_eventXiID_CastorSumEnergy", "Hist_eventXiID_CastorSumEnergy" ,nBinSumE,MinSumE, MaxSumE)  
       
        self.hist["Hist_MaxHFPlusEnergy"] = ROOT.TH1F("Hist_MaxHFPlusEnergy","Hist_MaxHFPlusEnergy",nBinSumE,MinSumE, MaxSumE) 
        self.hist["Hist_MaxHFMinusEnergy"] = ROOT.TH1F("Hist_MaxHFMinusEnergy","Hist_MaxHFMinusEnergy",nBinSumE,MinSumE, MaxSumE) 
        self.hist["Hist_MaxCastorEnergy"] = ROOT.TH1F("Hist_MaxCastorEnergy","Hist_MaxCastorEnergy",nBinSumE,MinSumE, MaxSumE) 
        self.hist["Hist_eventXiID_MaxHFPlusEnergy"] = ROOT.TH1F("Hist_eventXiID_MaxHFPlusEnergy","Hist_eventXiID_MaxHFPlusEnergy",nBinSumE,MinSumE, MaxSumE)
        self.hist["Hist_eventXiID_MaxHFMinusEnergy"] = ROOT.TH1F("Hist_eventXiID_MaxHFMinusEnergy","Hist_eventXiID_MaxHFMinusEnergy",nBinSumE,MinSumE, MaxSumE)    
        self.hist["Hist_eventXiID_MaxCastorEnergy"] = ROOT.TH1F("Hist_eventXiID_MaxCastorEnergy","Hist_eventXiID_MaxCastorEnergy",nBinSumE,MinSumE, MaxSumE)  
     

        self.hist["hParticleCounts"] = ROOT.TH1F("hParticleCounts","hParticleCounts",10, 0, 20) 
        self.hist["Hist_2Drecogen_EnergyX"] = ROOT.TH2D("Hist_2Drecogen_EnergyX", "Hist_2Drecogen_EnergyX", 100, 0, 8000,100, 0, 8000)    
        self.hist["Hist_2Drecogen_EnergyY"] = ROOT.TH2D("Hist_2Drecogen_EnergyY", "Hist_2Drecogen_EnergyY", 100, 0, 8000,100, 0, 8000)
        self.hist["Hist_numberoftowerebovenoise_endcap"] = ROOT.TH1F("Hist_numberoftowerebovenoise_endcap","Hist_numberoftowerebovenoise_endcap",ntow,Mintow,Maxtow) 
        self.hist["Hist_numberoftowerebovenoise_barrel"] = ROOT.TH1F("Hist_numberoftowerebovenoise_barrel","Hist_numberoftowerebovenoise_barrel",ntow,Mintow,Maxtow) 
        self.hist["Hist_numberoftowerebovenoise_endcapforwardtransition"] = ROOT.TH1F("Hist_numberoftowerebovenoise_endcapforwardtransition","Hist_numberoftowerebovenoise_endcapforwardtransition",ntow,Mintow,Maxtow) 

        self.hist["Hist_numberoftowerebovenoise_forwardplus"] = ROOT.TH1F("Hist_numberoftowerebovenoise_forwardplus","Hist_numberoftowerebovenoise_forwardplus",ntow,Mintow,Maxtow) 
        self.hist["Hist_numberoftowerebovenoise_forwardminus"] = ROOT.TH1F("Hist_numberoftowerebovenoise_forwardminus","Hist_numberoftowerebovenoise_forwardminus",ntow,Mintow,Maxtow) 
        self.hist["Hist_numberoftowerebovenoise_castor"] = ROOT.TH1F("Hist_numberoftowerebovenoise_castor","Hist_numberoftowerebovenoise_castor",18,0,18) 
        self.hist["Hist_eventXiID_numberoftowerebovenoise_forwardplus"] = ROOT.TH1F("Hist_eventXiID_numberoftowerebovenoise_forwardplus","Hist_eventXiID_numberoftowerebovenoise_forwardplus",ntow,Mintow,Maxtow) 
        self.hist["Hist_eventXiID_numberoftowerebovenoise_forwardminus"] = ROOT.TH1F("Hist_eventXiID_numberoftowerebovenoise_forwardminus","Hist_eventXiID_numberoftowerebovenoise_forwardminus",ntow,Mintow,Maxtow) 
        self.hist["Hist_eventXiID_numberoftowerebovenoise_castor"] = ROOT.TH1F("Hist_eventXiID_numberoftowerebovenoise_castor","Hist_eventXiID_numberoftowerebovenoise_castor",18, 0,18) 
        
      

        nEtaBins = 8
        EtaBins = array('d',[-6.6, -5.2, -3.2, -2.8, -1.4, 1.4, 2.8, 3.2, 5.2])
    

        NbrVtxBins = 20
        BinVtxMin = 0
        BinVtxMax = 20


        self.hist["Hist_NrVtx"] = ROOT.TH1F("Hist_NrVtx_","Hist_NrVtx",NbrVtxBins,BinVtxMin, BinVtxMax)  
        self.hist["Hist_NrVtxdelta"] = ROOT.TH1F("Hist_NrVtxdelta","Hist_NrVtxdelta",1000,BinVtxMin,50)   
        self.hist["Hist_NrVtxdeltacut"] = ROOT.TH1F("Hist_NrVtxdeltacut","Hist_NrVtxdeltacut",1000,BinVtxMin,50)   

        NbrEtaBins = 50
        BinEtaMin = -6.5
        BinEtaMax = 5.5
        NbrDetaBins = 50
        DetaMin = 0
        DetaMax = 11
        
        self.hist["Hist_GP_forwardgendelta"] = ROOT.TH1F("Hist_GP_forwardgendelta", "Hist_GP_forwardgendelta", NbrDetaBins, 0,12)
        self.hist["Hist_GP_eventXiID_forwardgendelta"] = ROOT.TH1F("Hist_GP_eventXiID_forwardgendelta", "Hist_GP_eventXiID_forwardgendelta", NbrDetaBins, 0,12)
        self.hist["Hist_GP_eventXiID_Min"] = ROOT.TH1F("Hist_GP_eventXiID_Min", "Hist_GP_eventXiID_Min", NbrEtaBins, BinEtaMin, BinEtaMax)
        self.hist["Hist_GP_eventXiID_Max"] = ROOT.TH1F("Hist_GP_eventXiID_Max", "Hist_GP_eventXiID_Max", NbrEtaBins, BinEtaMin, BinEtaMax)
        self.hist["Hist_GP_eventXiID_DeltaMax"] = ROOT.TH1F("Hist_GP_eventXiID_DeltaMax", "Hist_GP_eventXiID_DeltaMax", NbrDetaBins, DetaMin, DetaMax)
        self.hist["Hist_GP_eventXiID_DeltaZero"] = ROOT.TH1F("Hist_GP_eventXiID_DeltaZero", "Hist_GP_eventXiID_DeltaZero", NbrDetaBins, DetaMin, DetaMax)
        self.hist["Hist_eventXiID_Min"] = ROOT.TH1F("Hist_eventXiID_Min", "Hist_eventXiID_Min", NbrEtaBins, BinEtaMin, BinEtaMax)
        self.hist["Hist_eventXiID_Max"] = ROOT.TH1F("Hist_eventXiID_Max", "Hist_eventXiID_Max", NbrEtaBins, BinEtaMin, BinEtaMax)
        self.hist["Hist_eventXiID_DeltaMax"] = ROOT.TH1F("Hist_eventXiID_DeltaMax", "Hist_eventXiID_DeltaMax", NbrDetaBins, DetaMin, DetaMax)
        self.hist["Hist_eventXiID_DeltaZero"] = ROOT.TH1F("Hist_eventXiID_DeltaZero", "Hist_eventXiID_DeltaZero", NbrDetaBins, DetaMin, DetaMax)
        self.hist["Hist_eventEtaID_Min"] = ROOT.TH1F("Hist_eventEtaID_Min", "Hist_eventEtaID_Min", NbrEtaBins, BinEtaMin, BinEtaMax)
        self.hist["Hist_eventEtaID_Max"] = ROOT.TH1F("Hist_eventEtaID_Max", "Hist_eventEtaID_Max", NbrEtaBins, BinEtaMin, BinEtaMax)
        self.hist["Hist_eventEtaID_DeltaMax"] = ROOT.TH1F("Hist_eventEtaID_DeltaMax", "Hist_eventEtaID_DeltaMax", NbrDetaBins, DetaMin, DetaMax)
        
        self.hist["Hist_Eta_Min"] = ROOT.TH1F("Hist_Eta_Min", "Hist_Eta_Min", NbrEtaBins, BinEtaMin, BinEtaMax)
        self.hist["Hist_Eta_Max"] = ROOT.TH1F("Hist_Eta_Max", "Hist_Eta_Max", NbrEtaBins, BinEtaMin, BinEtaMax)
        self.hist["Hist_Eta_Delta"] = ROOT.TH1F("Hist_Eta_Delta", "Hist_Eta_Delta", NbrDetaBins, DetaMin, DetaMax)
        self.hist["Hist_forwarddelta"] = ROOT.TH1F("Hist_forwarddelta", "Hist_forwarddelta", NbrDetaBins, 0,12)
        self.hist["Hist_eventXiID_forwarddelta"] = ROOT.TH1F("Hist_eventXiID_forwarddelta", "Hist_eventXiID_forwarddelta", NbrDetaBins, 0,12)
        self.hist["Hist_Eta_DeltaZero"] = ROOT.TH1F("Hist_Eta_DeltaZero", "Hist_Eta_DeltaZero", NbrDetaBins, DetaMin, DetaMax)
        self.hist["Hist_Eta_DeltaMax"] = ROOT.TH1F("Hist_Eta_DeltaMax", "Hist_Eta_DeltaMax", NbrDetaBins, DetaMin, DetaMax)
        self.hist["Hist_2D_genreco_Deltapos"] = ROOT.TH2D("Hist_2D_genreco_Deltapos", "Hist_2D_genreco_Deltapos", NbrEtaBins, BinEtaMin, BinEtaMax,NbrEtaBins, BinEtaMin, BinEtaMax)
        # Gen PArticle 
        self.hist["Hist_2D_recogen_Mean"] = ROOT.TH2D("Hist_2D_recogen_Mean","Hist_2D_recogen_Mean",NbrEtaBins, BinEtaMin, BinEtaMax,NbrEtaBins, BinEtaMin, BinEtaMax)
        self.hist["Hist_GP_Min"] = ROOT.TH1F("Hist_GP_Min", "Hist_GP_Min", NbrEtaBins, BinEtaMin, BinEtaMax)
        self.hist["Hist_GP_Max"] = ROOT.TH1F("Hist_GP_Max", "Hist_GP_Max", NbrEtaBins, BinEtaMin, BinEtaMax)
        self.hist["Hist_GP_Delta"] = ROOT.TH1F("Hist_GP_Delta","Hist_GP_Delta", NbrDetaBins, DetaMin, DetaMax)
        self.hist["Hist_GP_DeltaZero"] = ROOT.TH1F("Hist_GP_DeltaZero", "Hist_GP_DeltaZero", NbrDetaBins, DetaMin, DetaMax)
        self.hist["Hist_GP_DeltaMax"] = ROOT.TH1F("Hist_GP_DeltaMax", "Hist_GP_DeltaMax", NbrDetaBins, DetaMin, DetaMax)
        self.hist["Hist_GP_deltagenreco"] = ROOT.TH1F("Hist_GP_deltagenreco", "Hist_GP_deltagenreco", NbrDetaBins, -10, DetaMax)
        self.hist["Hist_2D_recogen_DeltaZero"] = ROOT.TH2D("Hist_2D_recogen_DeltaZero","Hist_2D_recogen_DeltaZero", NbrDetaBins, DetaMin, DetaMax,NbrDetaBins, DetaMin, DetaMax);
        self.hist["Hist_2D_recogen_DeltaMax"] = ROOT.TH2D("Hist_2D_recogen_DeltaMax","Hist_2D_recogen_DeltaMax", NbrDetaBins, DetaMin, DetaMax,NbrDetaBins, DetaMin, DetaMax);
        self.hist["Hist_2D_recogen_EtaMiniumum"] = ROOT.TH2D("Hist_2D_recogen_EtaMiniumum","Hist_2D_recogen_EtaMiniumum",NbrEtaBins, BinEtaMin, BinEtaMax,NbrEtaBins, BinEtaMin, BinEtaMax)
        
        self.hist["Hist_2D_recogen_forwarddelta"] = ROOT.TH2D("Hist_2D_recogen_forwarddelta", "Hist_2D_recogen_forwarddelta", NbrDetaBins, 0,12,NbrDetaBins, 0,12)
        self.hist["Hist_2D_recogen_EtaMax"]= ROOT.TH2D("Hist_2D_recogen_EtaMax","Hist_2D_recogen_EtaMax",NbrEtaBins, BinEtaMin, BinEtaMax,NbrEtaBins, BinEtaMin, BinEtaMax)
        

        NbrSizeBins = 50
        SizeEMin = 0
        BinSizeEMax = 50
        
        self.hist["Hist_NbrTracks"] = ROOT.TH1F("Hist_NbrTracks","Hist_NbrTracks",NbrSizeBins, SizeEMin,50)
        self.hist["Hist_eventXiID_NbrTracks"] = ROOT.TH1F("Hist_eventXiID_NbrTracks","Hist_eventXiID_NbrTracks",NbrSizeBins, SizeEMin,50)
       
        PhiMin = -5
        PhiMax = 5
        NbrPhiBins = 50
        
        self.hist["Hist_trkPhi"] = ROOT.TH1F("Hist_trkPhi","Hist_trkPhi",NbrPhiBins,  PhiMin, PhiMax)  
        self.hist["Hist_Eta"] = ROOT.TH1F("Hist_Eta","Hist_Eta",NbrEtaBins, BinEtaMin, BinEtaMax) 

        self.hist["Hist_Calotower_eta"]= ROOT.TH1F("Hist_Calotower_eta","Hist_Calotower_eta",100,-6.6,6.6)
        
        self.hist["Hist_reducedEta"] = ROOT.TH1F("Hist_reducedEta","Hist_reducedEta",NbrEtaBins, BinEtaMin, BinEtaMax)  
        self.hist["Hist_2D_Calotower_ieta_eta"] =  ROOT.TH2D("Hist_2D_Calotower_ieta_eta","Hist_2D_Calotower_ieta_eta",101,-50.5, 50.5, 101, -50.5, 50.5)
        self.hist["Hist_2D_Calotower_eta_phi_Alltowers"] =  ROOT.TH2D("Hist_2D_Calotower_eta_phi_Alltowers","Hist_2D_Calotower_eta_phi_Alltowers",101, -50.5, 50.5, 81, -0.5, 80.5)
        self.hist["Hist_2D_Calotower_iphi_phi"] =  ROOT.TH2D("Hist_2D_Calotower_iphi_phi","Hist_2D_Calotower_iphi_phi",81, -0.5, 80.5,81, -0.5, 80.5)
        self.hist["Castortower2D_Energy_sec"] = ROOT.TH2D("Castortower2D_Energy_sec","Castortower2D_Energy_sec", 100, 0, 100, 81, -0.5, 80.5)
        self.hist["Hist_2D_Calotower_Energy_eta_ieta"] =  ROOT.TProfile2D("Hist_2D_Calotower_Energy_eta_ieta","Hist_2D_Calotower_Energy_eta_ieta",101,-50.5, 50.5, 101, -50.5, 50.5)
        self.hist["Hist_2D_Calotower_Energy_phi_iphi"] =  ROOT.TProfile2D("Hist_2D_Calotower_Energy_phi_iphi","Hist_2D_Calotower_Energy_phi_iphi",81, -0.5, 80.5,81, -0.5, 80.5)
        
        self.hist["Hist_2D_Calotower_withoutnoisecut_eta_phi"] =  ROOT.TH2D("Hist_2D_Calotower_withoutnoisecut_eta_phi","Hist_2D_Calotower_withoutnoisecut_eta_phi",101,-50.5,50.5,81, -0.5, 80.5)
        self.hist["Hist_2D_Calotower_withnoisecut_eta_phi"] =  ROOT.TH2D("Hist_2D_Calotower_withnoisecut_eta_phi","Hist_2D_Calotower_withnoisecut_eta_phi",101,-50.5,50.5,81, -0.5, 80.5)
        self.hist["Hist_trkEta"] = ROOT.TH1F("Hist_trkEta","Hist_trkEta",NbrEtaBins, BinEtaMin, BinEtaMax)  
        self.hist["Hist_trkplusEta"] = ROOT.TH1F("Hist_trkplusEta","Hist_trkplusEta",NbrEtaBins, BinEtaMin, BinEtaMax) 


        NbrLogMBins = 100
        LogMin = -3.5
        LogMax = 5.5
        
        NbrEBins = 50
        BinEMin = 0
        BinEMax = 100

       
        self.hist["Hist_log10Mx"] = ROOT.TH1F("Hist_log10Mx", "Hist_log10Mx", NbrLogMBins, LogMin, LogMax)
        self.hist["Hist_log10My"] = ROOT.TH1F("Hist_log10My", "Hist_log10My", NbrLogMBins, LogMin, LogMax)  
        self.hist["Hist_2D_log10MxMy"] = ROOT.TH2D("Hist_2D_log10MxMy", "Hist_2D_log10MxMy", NbrLogMBins, LogMin, LogMax, NbrLogMBins, LogMin, LogMax)
        self.hist["Hist_2D_eventXiID_log10MxMy"] =  ROOT.TH2D("Hist_2D_eventXiID_log10MxMy", "Hist_2D_eventXiID_log10MxMy", NbrLogMBins, LogMin, LogMax, NbrLogMBins, LogMin, LogMax)
        Process_ID = ["_NONE","_Rest","_SD1","_SD2","_DD", "_CD"]
        
        self.hist["Hist_GP_Mx"] =  ROOT.TH1F("Hist_GP_Mx", "Hist_GP_Mx", NbrLogMBins, LogMin, LogMax)
        self.hist["Hist_GP_My"] =  ROOT.TH1F("Hist_GP_My", "Hist_GP_My", NbrLogMBins, LogMin, LogMax)  
        self.hist["Hist_GP_log10Mx"] =  ROOT.TH1F("Hist_GP_log10Mx", "Hist_GP_log10Mx", NbrLogMBins, LogMin, LogMax)
        self.hist["Hist_GP_log10My"] =  ROOT.TH1F("Hist_GP_log10My", "Hist_GP_log10My", NbrLogMBins, LogMin, LogMax)  
        self.hist["Hist_2D_GP_log10MxMy"] =  ROOT.TH2D("Hist_2D_GP_log10MxMy", "Hist_2D_GP_log10MxMy", NbrLogMBins, LogMin, LogMax, NbrLogMBins, LogMin, LogMax)
        self.hist["Hist_Energy"] =  ROOT.TH1F("Hist_Energy", "Hist_Energy" , NbrEBins, BinEMin, BinEMax)
        self.hist["Hist_Energy_barrel"] =  ROOT.TH1F("Hist_Energy_barrel", "Hist_Energy_barrel" , NbrEBins, BinEMin, BinEMax)
        self.hist["Hist_Energy_endcap"] =  ROOT.TH1F("Hist_Energy_endcap", "Hist_Energy_endcap" , NbrEBins, BinEMin, BinEMax)
        self.hist["Hist_Energy_endcap_forwardtransition"] =  ROOT.TH1F("Hist_Energy_endcap_forwardtransition", "Hist_Energy_endcap_forwardtransition" , NbrEBins, BinEMin, BinEMax)
        self.hist["Hist_Energy_forward"] =  ROOT.TH1F("Hist_Energy_forward", "Hist_Energy_forward" , NbrEBins, BinEMin, BinEMax)
        self.hist["Hist_Energy_forwardplus"] =  ROOT.TH1F("Hist_Energy_forwardplus", "Hist_Energy_forwardplus" , NbrEBins, BinEMin, BinEMax)
        self.hist["Hist_Energy_forwardminus"] =  ROOT.TH1F("Hist_Energy_forwardminus", "Hist_Energy_forwardminus" , NbrEBins, BinEMin, BinEMax)
        self.hist["Hist_Energy_Castor"] =  ROOT.TH1F("Hist_Energy_Castor", "Hist_Energy_Castor" , NbrEBins, BinEMin, BinEMax)
        self.hist["Hist_eventXiID_Energy"] =  ROOT.TH1F("Hist_eventXiID_Energy", "Hist_eventXiID_Energy" , NbrEBins, BinEMin, BinEMax)
        self.hist["Hist_eventXiID_Energy_barrel"] =  ROOT.TH1F("Hist_eventXiID_Energy_barrel", "Hist_eventXiID_Energy_barrel" , NbrEBins, BinEMin, BinEMax)
        self.hist["Hist_eventXiID_Energy_endcap"] =  ROOT.TH1F("Hist_eventXiID_Energy_endcap", "Hist_eventXiID_Energy_endcap" , NbrEBins, BinEMin, BinEMax)
        self.hist["Hist_eventXiID_Energy_endcap_forwardtransition"] =  ROOT.TH1F("Hist_eventXiID_Energy_endcap_forwardtransition", "Hist_eventXiID_Energy_endcap_forwardtransition" , NbrEBins, BinEMin, BinEMax)
        self.hist["Hist_eventXiID_Energy_forward"] =  ROOT.TH1F("Hist_eventXiID_Energy_forward", "Hist_eventXiID_Energy_forward" , NbrEBins, BinEMin, BinEMax)
        self.hist["Hist_eventXiID_Energy_forwardplus"] =  ROOT.TH1F("Hist_eventXiID_Energy_forwardplus", "Hist_eventXiID_Energy_forwardplus" , NbrEBins, BinEMin, BinEMax)
        self.hist["Hist_eventXiID_Energy_forwardminus"] =  ROOT.TH1F("Hist_eventXiID_Energy_forwardminus", "Hist_eventXiID_Energy_forwardminus" , NbrEBins, BinEMin, BinEMax)
        self.hist["Hist_eventXiID_Energy_Castor"] =  ROOT.TH1F("Hist_eventXiID_Energy_Castor", "Hist_eventXiID_Energy_Castor" , NbrEBins, BinEMin, BinEMax)
        
        self.hist["Hist_eventXiID_reducedEnergy"] =  ROOT.TH1F("Hist_eventXiID_reducedEnergy", "Hist_eventXiID_reducedEnergy" , NbrEBins, BinEMin, BinEMax)
        self.hist["Hist_eventXiID_reducedEnergy_barrel"] =  ROOT.TH1F("Hist_eventXiID_reducedEnergy_barrel", "Hist_eventXiID_reducedEnergy_barrel" , NbrEBins, BinEMin, BinEMax)
        self.hist["Hist_eventXiID_reducedEnergy_endcap"] =  ROOT.TH1F("Hist_eventXiID_reducedEnergy_endcap", "Hist_eventXiID_reducedEnergy_endcap" , NbrEBins, BinEMin, BinEMax)
        self.hist["Hist_eventXiID_reducedEnergy_endcap_forwardtransition"] =  ROOT.TH1F("Hist_eventXiID_reducedEnergy_endcap_forwardtransition", "Hist_eventXiID_reducedEnergy_endcap_forwardtransition" , NbrEBins, BinEMin, BinEMax)
        self.hist["Hist_eventXiID_reducedEnergy_forward"] =  ROOT.TH1F("Hist_eventXiID_reducedEnergy_forward", "Hist_eventXiID_reducedEnergy_forward" , NbrEBins, BinEMin, BinEMax)
        self.hist["Hist_eventXiID_reducedEnergy_forwardplus"] =  ROOT.TH1F("Hist_eventXiID_reducedEnergy_forwardplus", "Hist_eventXiID_reducedEnergy_forwardplus" , NbrEBins, BinEMin, BinEMax)
        self.hist["Hist_eventXiID_reducedEnergy_forwardminus"] =  ROOT.TH1F("Hist_eventXiID_reducedEnergy_forwardminus", "Hist_eventXiID_reducedEnergy_forwardminus" , NbrEBins, BinEMin, BinEMax)
        self.hist["Hist_eventXiID_reducedEnergy_Castor"] =  ROOT.TH1F("Hist_eventXiID_reducedEnergy_Castor", "Hist_eventXiID_reducedEnergy_Castor" , NbrEBins, BinEMin, BinEMax)
        self.hist["Hist_reducedEnergy"] =  ROOT.TH1F("Hist_reducedEnergy", "Hist_reducedEnergy" , NbrEBins, BinEMin, BinEMax)
        self.hist["Hist_reducedEnergy_barrel"] =  ROOT.TH1F("Hist_reducedEnergy_barrel", "Hist_reducedEnergy_barrel" , NbrEBins, BinEMin, BinEMax)
        self.hist["Hist_reducedEnergy_endcap"] =  ROOT.TH1F("Hist_reducedEnergy_endcap", "Hist_reducedEnergy_endcap" , NbrEBins, BinEMin, BinEMax)
        self.hist["Hist_reducedEnergy_endcap_forwardtransition"] =  ROOT.TH1F("Hist_reducedEnergy_endcap_forwardtransition", "Hist_reducedEnergy_endcap_forwardtransition" , NbrEBins, BinEMin, BinEMax)
        self.hist["Hist_reducedEnergy_forward"] =  ROOT.TH1F("Hist_reducedEnergy_forward", "Hist_reducedEnergy_forward" , NbrEBins, BinEMin, BinEMax)
        self.hist["Hist_reducedEnergy_forwardplus"] =  ROOT.TH1F("Hist_reducedEnergy_forwardplus", "Hist_reducedEnergy_forwardplus" , NbrEBins, BinEMin, BinEMax)
        self.hist["Hist_reducedEnergy_forwardminus"] =  ROOT.TH1F("Hist_reducedEnergy_forwardminus", "Hist_reducedEnergy_forwardminus" , NbrEBins, BinEMin, BinEMax)
        self.hist["Hist_reducedEnergy_Castor"] =  ROOT.TH1F("Hist_reducedEnergy_Castor", "Hist_reducedEnergy_Castor" , NbrEBins, BinEMin, BinEMax)
        self.hist["Hist_CaloReducedenergyClass"]= ROOT.TH1F("Hist_CaloReducedenergyClass","Hist_CaloReducedenergyClass",NbrEBins, BinEMin, BinEMax)
        self.hist["Hist_CaloReducedenergyClass_nocastor"]= ROOT.TH1F("Hist_CaloReducedenergyClass_nocastor","Hist_CaloReducedenergyClass_nocastor",NbrEBins, BinEMin, BinEMax)
        self.hist["Hist_CaloReducedenergyClass_noHFm"]= ROOT.TH1F("Hist_CaloReducedenergyClass_noHFm","Hist_CaloReducedenergyClass_noHFm",NbrEBins, BinEMin, BinEMax)
        self.hist["Hist_CaloReducedenergyClass_noHFp"]= ROOT.TH1F("Hist_CaloReducedenergyClass_noHFp","Hist_CaloReducedenergyClass_noHFp",NbrEBins, BinEMin, BinEMax)

        self.hist["Hist_eventXiID_CaloReducedenergyClass"]= ROOT.TH1F("Hist_eventXiID_CaloReducedenergyClass","Hist_eventXiID_CaloReducedenergyClass",NbrEBins, BinEMin, BinEMax)

        NbrLogXiBins = 100
        LogXiMin = -11.5
        LogXiMax = 0.5
        NbrXiBins = 50
        XiMin = 0
        XiMax = 5
        NbrLogXiBins = 100
        LogXiMin = -11.5
        
        
        self.hist["Hist_XiX"] =  ROOT.TH1F("Hist_XiX", "Hist_XiX", NbrXiBins, XiMin, XiMax)
        self.hist["Hist_XiY"] =  ROOT.TH1F("Hist_XiY", "Hist_XiY", NbrXiBins, XiMin, XiMax)
        self.hist["Hist_GP_log10XiDD"] =  ROOT.TH1F("Hist_GP_log10XiDD", "Hist_GP_log10XiDD", NbrLogXiBins, LogXiMin,LogXiMax)
        self.hist["Hist_GP_Aftercutlog10XiDD"] =  ROOT.TH1F("Hist_GP_Aftercutlog10XiDD", "Hist_GP_Aftercutlog10XiDD", NbrLogXiBins, LogXiMin,LogXiMax)
        self.hist["Hist_2D_log10XiDD_Deltamax"]= ROOT.TH2D("Hist_2D_log10XiDD_Deltamax","Hist_2D_log10XiDD_Deltamax",NbrLogXiBins, LogXiMin, LogXiMax,NbrDetaBins, DetaMin, DetaMax)
        self.hist["Hist_Reco_log10XiDD"] =  ROOT.TH1F("Hist_Reco_log10XiDD", "Hist_Reco_log10XiDD", NbrLogXiBins, LogXiMin,LogXiMax)
        self.hist["Hist_eventXiID_Reco_log10XiDD"] =  ROOT.TH1F("Hist_eventXiID_Reco_log10XiDD", "Hist_eventXiID_Reco_log10XiDD", NbrLogXiBins, LogXiMin,LogXiMax)
        self.hist["Hist_2D_RecoLogXi_DDGenLogXi_DD"] = ROOT.TH2D("Hist_2D_RecoLogXi_DDGenLogXi_DD", "Hist_2D_RecoLogXi_DDGenLogXi_DD", NbrLogXiBins, LogXiMin,LogXiMax, NbrLogXiBins, LogXiMin,LogXiMax)
        self.hist["Hist_log10XiX"] =  ROOT.TH1F("Hist_log10XiX", "Hist_log10XiX", NbrLogXiBins, LogXiMin,LogXiMax)
        self.hist["Hist_log10XiY"] =  ROOT.TH1F("Hist_log10XiY", "Hist_log10XiY", NbrLogXiBins, LogXiMin,LogXiMax)
        self.hist["Hist_GP_log10XiX"] =  ROOT.TH1F("Hist_GP_log10XiX", "Hist_GP_log10XiX", NbrLogXiBins, LogXiMin,LogXiMax)
        self.hist["Hist_GP_log10XiY"] =  ROOT.TH1F("Hist_GP_log10XiY", "Hist_GP_log10XiY", NbrLogXiBins, LogXiMin,LogXiMax)
        self.hist["Hist_GP_log10XiX_protonplus"] =  ROOT.TH1F("Hist_GP_log10XiX_protonplus", "Hist_GP_log10XiX_protonplus", NbrLogXiBins, LogXiMin,LogXiMax)
        self.hist["Hist_GP_log10XiY_protonminus"] =  ROOT.TH1F("Hist_GP_log10XiY_protonminus", "Hist_GP_log10XiY_protonminus", NbrLogXiBins, LogXiMin,LogXiMax)
        self.hist["Hist_GP_log10MaxXi"] =  ROOT.TH1F("Hist_GP_log10MaxXi", "Hist_GP_log10MaxXi", NbrLogXiBins, LogXiMin,LogXiMax)
        self.hist["Hist_GP_Aftercutlog10XiX"] =  ROOT.TH1F("Hist_GP_Aftercutlog10XiX", "Hist_GP_Aftercutlog10XiX", NbrLogXiBins, LogXiMin,LogXiMax)
        self.hist["Hist_GP_Aftercutlog10XiY"] =  ROOT.TH1F("Hist_GP_Aftercutlog10XiY", "Hist_GP_Aftercutlog10XiY", NbrLogXiBins, LogXiMin,LogXiMax)
        self.hist["Hist_GP_Aftercutlog10MaxXi"] =  ROOT.TH1F("Hist_GP_Aftercutlog10MaxXi", "Hist_GP_Aftercutlog10MaxXi", NbrLogXiBins, LogXiMin,LogXiMax)

        self.hist["Hist_2Dlog10XiXXiY"] = ROOT.TH2D("Hist_2Dlog10XiXXiY", "Hist_2Dlog10XiXXiY", NbrLogXiBins, LogXiMin,LogXiMax, NbrLogXiBins, LogXiMin,LogXiMax)
        self.hist["Hist_2DLogRecoXiX_logGenXiX"] = ROOT.TH2D("Hist_2DLogRecoXiX_logGenXiX", "Hist_2DLogRecoXiX_logGenXiX", NbrLogXiBins, LogXiMin,LogXiMax, NbrLogXiBins, LogXiMin,LogXiMax)
        self.hist["Hist_2DLogRecoXiY_logGenLogXiY"] = ROOT.TH2D("Hist_2DLogRecoXiY_logGenLogXiY", "Hist_2DLogRecoXiY_logGenLogXiY", NbrLogXiBins, LogXiMin,LogXiMax, NbrLogXiBins, LogXiMin,LogXiMax)
        self.hist["Hist_2DLogGenXiX_LogGenXiY"] = ROOT.TH2D("Hist_2DLogGenXiX_LogGenXiY", "Hist_2DLogGenXiX_LogGenXiY", NbrLogXiBins, LogXiMin,LogXiMax, NbrLogXiBins, LogXiMin,LogXiMax)
        self.hist["Hist_Aftercut2DLogGenXiX_LogGenXiY"] = ROOT.TH2D("Hist_Aftercut2DLogGenXiX_LogGenXiY", "Hist_Aftercut2DLogGenXiX_LogGenXiY", NbrLogXiBins, LogXiMin,LogXiMax, NbrLogXiBins, LogXiMin,LogXiMax)

        self.hist["Hist_2D_FG2_RecoLogXiXGenLogXiX"] = ROOT.TH2D("Hist_2D_FG2_RecoLogXiXGenLogXiX", "Hist_2D_FG2_RecoLogXiXGenLogXiX", NbrLogXiBins, LogXiMin,LogXiMax, NbrLogXiBins, LogXiMin,LogXiMax)
        self.hist["Hist_2D_FG2_RecoLogXiYGenLogXiY"] = ROOT.TH2D("Hist_2D_FG2_RecoLogXiYGenLogXiY", "Hist_2D_FG2_RecoLogXiYGenLogXiY", NbrLogXiBins, LogXiMin,LogXiMax, NbrLogXiBins, LogXiMin,LogXiMax)
        self.hist["Hist_2D_FG1_RecoLogXiXGenLogXiX"] = ROOT.TH2D("Hist_2D_FG1_RecoLogXiXGenLogXiX", "Hist_2D_FG1_RecoLogXiXGenLogXiX", NbrLogXiBins, LogXiMin,LogXiMax, NbrLogXiBins, LogXiMin,LogXiMax)
        self.hist["Hist_2D_FG1_RecoLogXiYGenLogXiY"] = ROOT.TH2D("Hist_2D_FG1_RecoLogXiYGenLogXiY", "Hist_2D_FG1_RecoLogXiYGenLogXiY", NbrLogXiBins, LogXiMin,LogXiMax, NbrLogXiBins, LogXiMin,LogXiMax)
        self.hist["Hist_2D_CG_RecoLogXiXGenLogXiX"] = ROOT.TH2D("Hist_2D_CG_RecoLogXiXGenLogXiX", "Hist_2D_CG_RecoLogXiXGenLogXiX", NbrLogXiBins, LogXiMin,LogXiMax, NbrLogXiBins, LogXiMin,LogXiMax)
        self.hist["Hist_2D_CG_RecoLogXiYGenLogXiY"] = ROOT.TH2D("Hist_2D_CG_RecoLogXiYGenLogXiY", "Hist_2D_CG_RecoLogXiYGenLogXiY", NbrLogXiBins, LogXiMin,LogXiMax, NbrLogXiBins, LogXiMin,LogXiMax)
        self.hist["Hist_2D_FG2_2DLogGenXiX_LogGenXiY"] = ROOT.TH2D("Hist_2D_FG2_2DLogGenXiX_LogGenXiY", "Hist_2D_FG2_2DLogGenXiX_LogGenXiY", NbrLogXiBins, LogXiMin,LogXiMax, NbrLogXiBins, LogXiMin,LogXiMax)
        self.hist["Hist_2D_FG1_2DLogGenXiX_LogGenXiY"] = ROOT.TH2D("Hist_2D_FG1_2DLogGenXiX_LogGenXiY", "Hist_2D_FG1_2DLogGenXiX_LogGenXiY", NbrLogXiBins, LogXiMin,LogXiMax, NbrLogXiBins, LogXiMin,LogXiMax)
        self.hist["Hist_2D_CG_2DLogGenXiX_LogGenXiY"] = ROOT.TH2D("Hist_2D_CG_2DLogGenXiX_LogGenXiY", "Hist_2D_CG_2DLogGenXiX_LogGenXiY", NbrLogXiBins, LogXiMin,LogXiMax, NbrLogXiBins, LogXiMin,LogXiMax)
        self.hist["Hist_eventXiID_log10XiX"] =  ROOT.TH1F("Hist_eventXiID_log10XiX", "Hist_eventXiID_log10XiX", NbrLogXiBins, LogXiMin,LogXiMax)
        self.hist["Hist_eventXiID_log10XiY"] =  ROOT.TH1F("Hist_eventXiID_log10XiY", "Hist_eventXiID_log10XiY", NbrLogXiBins, LogXiMin,LogXiMax)
        self.hist["Hist_GP_eventXiID_log10XiX"] =  ROOT.TH1F("Hist_GP_eventXiID_log10XiX", "Hist_GP_eventXiID_log10XiX", NbrLogXiBins, LogXiMin,LogXiMax)
        self.hist["Hist_GP_eventXiID_log10XiY"] =  ROOT.TH1F("Hist_GP_eventXiID_log10XiY", "Hist_GP_eventXiID_log10XiY", NbrLogXiBins, LogXiMin,LogXiMax)
        
        
       
        for ip in Process_ID:
            self.hist["Hist_Eta_Min"+str(ip)] = ROOT.TH1D("Hist_Eta_Min"+str(ip),"Hist_Eta_Min ", NbrEtaBins, BinEtaMin, BinEtaMax)
            self.hist["Hist_Eta_Max"+str(ip)] = ROOT.TH1D("Hist_Eta_Max"+str(ip),"Hist_Eta_Max ", NbrEtaBins, BinEtaMin, BinEtaMax) 
            self.hist["Hist_Eta_Delta"+str(ip)] = ROOT.TH1D("Hist_Eta_Delta"+str(ip),"Hist_Eta_Delta ", NbrDetaBins, DetaMin, DetaMax)
            self.hist["Hist_Eta_DeltaZero"+str(ip)] = ROOT.TH1D("Hist_Eta_DeltaZero"+str(ip),"Hist_Eta_DeltaZero ", NbrDetaBins, DetaMin, DetaMax)
            self.hist["Hist_Eta_DeltaMax"+str(ip)] =  ROOT.TH1F("Hist_Eta_DeltaMax"+str(ip), "Hist_Eta_DeltaMax"+str(ip), NbrDetaBins, DetaMin, DetaMax)               
            self.hist["Hist_forwarddelta"+str(ip)] = ROOT.TH1F("Hist_forwarddelta"+str(ip), "Hist_forwarddelta"+str(ip), NbrDetaBins, 0,12)

            self.hist["Hist_Eta"+str(ip)] =  ROOT.TH1F("Hist_Eta"+str(ip), "Hist_Eta"+str(ip), nEtaBins, EtaBins)
            self.hist["Hist_reducedEta"+str(ip)] =  ROOT.TH1F("Hist_reducedEta"+str(ip), "Hist_reducedEta"+str(ip), nEtaBins, EtaBins)
            self.hist["Hist_GP_DeltaMax"+str(ip)] =  ROOT.TH1F("Hist_GP_DeltaMax"+str(ip), "Hist_GP_DeltaMax"+str(ip), NbrDetaBins, DetaMin, DetaMax)               
            self.hist["Hist_GP_Min"+str(ip)] = ROOT.TH1D("Hist_GP_Min"+str(ip),"Hist_GP_Min ", NbrEtaBins, BinEtaMin, BinEtaMax)
            self.hist["Hist_GP_Max"+str(ip)] = ROOT.TH1D("Hist_GP_Max"+str(ip),"Hist_GP_Max ", NbrEtaBins, BinEtaMin, BinEtaMax) 
            self.hist["Hist_GP_Delta"+str(ip)] = ROOT.TH1D("Hist_GP_Delta"+str(ip),"Hist_GP_Delta ", NbrDetaBins, DetaMin, DetaMax)
            self.hist["Hist_GP_DeltaZero"+str(ip)] = ROOT.TH1D("Hist_GP_DeltaZero"+str(ip),"Hist_GP_DeltaZero ", NbrDetaBins, DetaMin, DetaMax)
            self.hist["Hist_Energy"+str(ip)] =  ROOT.TH1F("Hist_Energy"+str(ip), "Hist_Energy"+str(ip) , NbrEBins, BinEMin, BinEMax)
            self.hist["Hist_Energy_barrel"+str(ip)] =  ROOT.TH1F("Hist_Energy_barrel"+str(ip), "Hist_Energy_barrel"+str(ip) , NbrEBins, BinEMin, BinEMax)
            self.hist["Hist_Energy_endcap"+str(ip)] =  ROOT.TH1F("Hist_Energy_endcap"+str(ip), "Hist_Energy_endcap"+str(ip) , NbrEBins, BinEMin, BinEMax)
            self.hist["Hist_Energy_endcap_forwardtransition"+str(ip)] =  ROOT.TH1F("Hist_Energy_endcap_forwardtransition"+str(ip), "Hist_Energy_endcap_forwardtransition"+str(ip) , NbrEBins, BinEMin, BinEMax)
            self.hist["Hist_Energy_forward"+str(ip)] =  ROOT.TH1F("Hist_Energy_forward"+str(ip), "Hist_Energy_forward"+str(ip) , NbrEBins, BinEMin, BinEMax)
            self.hist["Hist_Energy_forwardplus"+str(ip)] =  ROOT.TH1F("Hist_Energy_forwardplus"+str(ip), "Hist_Energy_forwardplus"+str(ip) , NbrEBins, BinEMin, BinEMax)
            self.hist["Hist_Energy_forwardminus"+str(ip)] =  ROOT.TH1F("Hist_Energy_forwardminus"+str(ip), "Hist_Energy_forwardminus"+str(ip) , NbrEBins, BinEMin, BinEMax)
            self.hist["Hist_Energy_Castor"+str(ip)] =  ROOT.TH1F("Hist_Energy_Castor"+str(ip), "Hist_Energy_Castor"+str(ip) , NbrEBins, BinEMin, BinEMax)
           
            self.hist["Hist_CaloReducedenergyClass"+str(ip)]= ROOT.TH1F("Hist_CaloReducedenergyClass"+str(ip),"Hist_CaloReducedenergyClass"+str(ip),NbrEBins, BinEMin, BinEMax)
            self.hist["Hist_CaloReducedenergyClass_nocastor"+str(ip)]= ROOT.TH1F("Hist_CaloReducedenergyClass_nocastor"+str(ip),"Hist_CaloReducedenergyClass_nocastor"+str(ip),NbrEBins, BinEMin, BinEMax)
            self.hist["Hist_CaloReducedenergyClass_noHFm"+str(ip)]= ROOT.TH1F("Hist_CaloReducedenergyClass_noHFm"+str(ip),"Hist_CaloReducedenergyClass_noHFm"+str(ip),NbrEBins, BinEMin, BinEMax)
            self.hist["Hist_CaloReducedenergyClass_noHFp"+str(ip)]= ROOT.TH1F("Hist_CaloReducedenergyClass_noHFp"+str(ip),"Hist_CaloReducedenergyClass_noHFp"+str(ip),NbrEBins, BinEMin, BinEMax)
           
            

            self.hist["Hist_reducedEnergy"+str(ip)] =  ROOT.TH1F("Hist_reducedEnergy"+str(ip), "Hist_reducedEnergy"+str(ip) , NbrEBins, BinEMin, BinEMax)
            self.hist["Hist_reducedEnergy_barrel"+str(ip)] =  ROOT.TH1F("Hist_reducedEnergy_barrel"+str(ip), "Hist_reducedEnergy_barrel"+str(ip) , NbrEBins, BinEMin, BinEMax)
            self.hist["Hist_reducedEnergy_endcap"+str(ip)] =  ROOT.TH1F("Hist_reducedEnergy_endcap"+str(ip), "Hist_reducedEnergy_endcap"+str(ip) , NbrEBins, BinEMin, BinEMax)
            self.hist["Hist_reducedEnergy_endcap_forwardtransition"+str(ip)] =  ROOT.TH1F("Hist_reducedEnergy_endcap_forwardtransition"+str(ip), "Hist_reducedEnergy_endcap_forwardtransition"+str(ip) , NbrEBins, BinEMin, BinEMax)
            self.hist["Hist_reducedEnergy_forward"+str(ip)] =  ROOT.TH1F("Hist_reducedEnergy_forward"+str(ip), "Hist_reducedEnergy_forward"+str(ip) , NbrEBins, BinEMin, BinEMax)
            self.hist["Hist_reducedEnergy_forwardplus"+str(ip)] =  ROOT.TH1F("Hist_reducedEnergy_forwardplus"+str(ip), "Hist_reducedEnergy_forwardplus"+str(ip) , NbrEBins, BinEMin, BinEMax)
            self.hist["Hist_reducedEnergy_forwardminus"+str(ip)] =  ROOT.TH1F("Hist_reducedEnergy_forwardminus"+str(ip), "Hist_reducedEnergy_forwardminus"+str(ip) , NbrEBins, BinEMin, BinEMax)
            self.hist["Hist_reducedEnergy_Castor"+str(ip)] =  ROOT.TH1F("Hist_reducedEnergy_Castor"+str(ip), "Hist_reducedEnergy_Castor"+str(ip) , NbrEBins, BinEMin, BinEMax)
            
            self.hist["Hist_2D_log10MxMy"+str(ip)] =  ROOT.TH2D("Hist_2D_log10MxMy"+str(ip), "Hist_2D_log10MxMy"+str(ip), NbrLogMBins, LogMin, LogMax, NbrLogMBins, LogMin, LogMax)
            self.hist["Hist_2D_eventXiID_log10MxMy"+str(ip)] =  ROOT.TH2D("Hist_2D_eventXiID_log10MxMy"+str(ip), "Hist_2D_eventXiID_log10MxMy"+str(ip), NbrLogMBins, LogMin, LogMax, NbrLogMBins, LogMin, LogMax)
            self.hist["Hist_log10Mx"+str(ip)] =  ROOT.TH1D("Hist_log10Mx"+str(ip), "Hist_log10Mx"+str(ip), NbrLogMBins, LogMin, LogMax)
            self.hist["Hist_log10My"+str(ip)] =  ROOT.TH1D("Hist_log10My"+str(ip), "Hist_log10My"+str(ip), NbrLogMBins, LogMin, LogMax)
            self.hist["Hist_GP_log10Mx"+str(ip)] =  ROOT.TH1D("Hist_GP_log10Mx"+str(ip), "Hist_GP_log10Mx"+str(ip), NbrLogMBins, LogMin, LogMax)
            self.hist["Hist_GP_log10My"+str(ip)] =  ROOT.TH1D("Hist_GP_log10My"+str(ip), "Hist_GP_log10My"+str(ip), NbrLogMBins, LogMin, LogMax)
            self.hist["Hist_2D_GP_log10MxMy"+str(ip)] =  ROOT.TH2D("Hist_2D_GP_log10MxMy"+str(ip), "Hist_2D_GP_log10MxMy"+str(ip), NbrLogMBins, LogMin, LogMax, NbrLogMBins, LogMin, LogMax)
            self.hist["Hist_GP_log10XiX"+str(ip)] =  ROOT.TH1F("Hist_GP_log10XiX"+str(ip), "Hist_GP_log10XiX"+str(ip), NbrLogXiBins, LogXiMin,LogXiMax)
            self.hist["Hist_GP_log10XiX_protonplus"+str(ip)] =  ROOT.TH1F("Hist_GP_log10XiX_protonplus"+str(ip), "Hist_GP_log10XiX_protonplus"+str(ip), NbrLogXiBins, LogXiMin,LogXiMax)
            self.hist["Hist_GP_log10XiY_protonminus"+str(ip)] =  ROOT.TH1F("Hist_GP_log10XiY_protonminus"+str(ip), "Hist_GP_log10XiY_protonminus"+str(ip), NbrLogXiBins, LogXiMin,LogXiMax)
            self.hist["Hist_GP_log10XiY"+str(ip)] =  ROOT.TH1F("Hist_GP_log10XiY"+str(ip), "Hist_GP_log10XiY"+str(ip), NbrLogXiBins, LogXiMin,LogXiMax)
            self.hist["Hist_GP_log10MaxXi"+str(ip)] =  ROOT.TH1F("Hist_GP_log10MaxXi"+str(ip), "Hist_GP_log10MaxXi"+str(ip), NbrLogXiBins, LogXiMin,LogXiMax)
            self.hist["Hist_GP_Aftercutlog10XiX"+str(ip)] =  ROOT.TH1F("Hist_GP_Aftercutlog10XiX"+str(ip), "Hist_GP_Aftercutlog10XiX"+str(ip), NbrLogXiBins, LogXiMin,LogXiMax)
            self.hist["Hist_GP_Aftercutlog10XiY"+str(ip)] =  ROOT.TH1F("Hist_GP_Aftercutlog10XiY"+str(ip), "Hist_GP_Aftercutlog10XiY"+str(ip), NbrLogXiBins, LogXiMin,LogXiMax)
            self.hist["Hist_GP_Aftercutlog10MaxXi"+str(ip)] =  ROOT.TH1F("Hist_GP_Aftercutlog10MaxXi"+str(ip), "Hist_GP_Aftercutlog10MaxXi"+str(ip), NbrLogXiBins, LogXiMin,LogXiMax) 

            self.hist["Hist_2D_log10XiDD_Deltamax"+str(ip)]= ROOT.TH2D("Hist_2D_log10XiDD_Deltamax"+str(ip),"Hist_2D_log10XiDD_Deltamax"+str(ip),NbrLogXiBins, LogXiMin, LogXiMax,NbrDetaBins, DetaMin, DetaMax)
            self.hist["Hist_Xi"+str(ip)] =  ROOT.TH1F("Hist_Xi"+str(ip), "Hist_Xi"+str(ip), NbrXiBins, XiMin, XiMax)
            self.hist["Hist_Xi"+str(ip)] =  ROOT.TH1F("Hist_Xi"+str(ip), "Hist_Xi"+str(ip), NbrXiBins, XiMin, XiMax)
            self.hist["Hist_XiX"+str(ip)] =  ROOT.TH1F("Hist_XiX"+str(ip), "Hist_XiX"+str(ip), NbrXiBins, XiMin, XiMax)
            self.hist["Hist_XiY"+str(ip)] =  ROOT.TH1F("Hist_XiY"+str(ip), "Hist_XiY"+str(ip), NbrXiBins, XiMin, XiMax)
            self.hist["Hist_log10Xi"+str(ip)] =  ROOT.TH1F("Hist_log10Xi"+str(ip), "Hist_log10Xi"+str(ip), NbrLogXiBins, LogXiMin,LogXiMax)
            self.hist["Hist_log10Xi"+str(ip)] =  ROOT.TH1F("Hist_log10Xi"+str(ip), "Hist_log10Xi"+str(ip), NbrLogXiBins, LogXiMin,LogXiMax)
            self.hist["Hist_log10XiX"+str(ip)] =  ROOT.TH1F("Hist_log10XiX"+str(ip), "Hist_log10XiX"+str(ip), NbrLogXiBins, LogXiMin,LogXiMax)
            self.hist["Hist_log10XiY"+str(ip)] =  ROOT.TH1F("Hist_log10XiY"+str(ip), "Hist_log10XiY"+str(ip), NbrLogXiBins, LogXiMin,LogXiMax)
            self.hist["Hist_2Dlog10XiXXiY"+str(ip)] = ROOT.TH2D("Hist_2Dlog10XiXXiY"+str(ip), "Hist_2Dlog10XiXXiY"+str(ip), NbrLogXiBins, LogXiMin,LogXiMax, NbrLogXiBins, LogXiMin,LogXiMax)
            self.hist["Hist_Reco_log10XiDD"+str(ip)] =  ROOT.TH1F("Hist_Reco_log10XiDD"+str(ip), "Hist_Reco_log10XiDD"+str(ip), NbrLogXiBins, LogXiMin,LogXiMax)
            self.hist["Hist_2DLogRecoXiX_logGenXiX"+str(ip)] = ROOT.TH2D("Hist_2DLogRecoXiX_logGenXiX"+str(ip), "Hist_2DLogRecoXiX_logGenXiX"+str(ip), NbrLogXiBins, LogXiMin,LogXiMax, NbrLogXiBins, LogXiMin,LogXiMax)
            self.hist["Hist_2DLogRecoXiY_logGenLogXiY"+str(ip)] = ROOT.TH2D("Hist_2DLogRecoXiY_logGenLogXiY"+str(ip), "Hist_2DLogRecoXiY_logGenLogXiY"+str(ip), NbrLogXiBins, LogXiMin,LogXiMax, NbrLogXiBins, LogXiMin,LogXiMax)
            self.hist["Hist_2DLogGenXiX_LogGenXiY"+str(ip)] = ROOT.TH2D("Hist_2DLogGenXiX_LogGenXiY"+str(ip), "Hist_2DLogGenXiX_LogGenXiY"+str(ip), NbrLogXiBins,LogXiMin,LogXiMax, NbrLogXiBins, LogXiMin,LogXiMax)
            self.hist["Hist_Aftercut2DLogGenXiX_LogGenXiY"+str(ip)] = ROOT.TH2D("Hist_Aftercut2DLogGenXiX_LogGenXiY"+str(ip), "Hist_Aftercut2DLogGenXiX_LogGenXiY"+str(ip), NbrLogXiBins,LogXiMin,LogXiMax, NbrLogXiBins, LogXiMin,LogXiMax)

            self.hist["Hist_GP_log10XiDD"+str(ip)] =  ROOT.TH1F("Hist_GP_log10XiDD"+str(ip), "Hist_GP_log10XiDD"+str(ip), NbrLogXiBins, LogXiMin,LogXiMax)
            self.hist["Hist_GP_Aftercutlog10XiDD"+str(ip)] =  ROOT.TH1F("Hist_GP_Aftercutlog10XiDD"+str(ip), "Hist_GP_Aftercutlog10XiDD"+str(ip), NbrLogXiBins, LogXiMin,LogXiMax)
            self.hist["Hist_numberoftowerebovenoise_forwardplus"+str(ip)] = ROOT.TH1F("Hist_numberoftowerebovenoise_forwardplus"+str(ip),"Hist_numberoftowerebovenoise_forwardplus"+str(ip),ntow,Mintow,Maxtow) 
            self.hist["Hist_numberoftowerebovenoise_forwardminus"+str(ip)] = ROOT.TH1F("Hist_numberoftowerebovenoise_forwardminus"+str(ip),"Hist_numberoftowerebovenoise_forwardminus"+str(ip),ntow,Mintow,Maxtow) 
            self.hist["Hist_numberoftowerebovenoise_castor"+str(ip)] = ROOT.TH1F("Hist_numberoftowerebovenoise_castor"+str(ip),"Hist_numberoftowerebovenoise_castor"+str(ip),18, 0,18) 
            self.hist["Hist_NbrTracks"+str(ip)] = ROOT.TH1F("Hist_NbrTracks"+str(ip),"Hist_NbrTracks"+str(ip),NbrSizeBins, SizeEMin,50)
            self.hist["Hist_eventXiID_NbrTracks"+str(ip)] = ROOT.TH1F("Hist_eventXiID_NbrTracks"+str(ip),"Hist_eventXiID_NbrTracks"+str(ip),NbrSizeBins, SizeEMin,50)
            
            self.hist["Hist_numberoftowerebovenoise_endcap"+str(ip)] = ROOT.TH1F("Hist_numberoftowerebovenoise_endcap"+str(ip),"Hist_numberoftowerebovenoise_endcap"+str(ip),ntow,Mintow,Maxtow) 
            self.hist["Hist_numberoftowerebovenoise_barrel"+str(ip)] = ROOT.TH1F("Hist_numberoftowerebovenoise_barrel"+str(ip),"Hist_numberoftowerebovenoise_barrel"+str(ip),ntow,Mintow,Maxtow) 
            self.hist["Hist_numberoftowerebovenoise_endcapforwardtransition"+str(ip)] = ROOT.TH1F("Hist_numberoftowerebovenoise_endcapforwardtransition"+str(ip),"Hist_numberoftowerebovenoise_endcapforwardtransition"+str(ip),ntow,Mintow,Maxtow) 
            self.hist["Hist_HFPlusSumEnergy"+str(ip)] = ROOT.TH1F("Hist_HFPlusSumEnergy"+str(ip), "Hist_HFPlusSumEnergy"+str(ip) ,nBinSumE, MinSumE, MaxSumE)
            self.hist["Hist_HFMinusSumEnergy"+str(ip)] = ROOT.TH1F("Hist_HFMinusSumEnergy"+str(ip), "Hist_HFMinusSumEnergy"+str(ip) ,nBinSumE, MinSumE, MaxSumE)

            self.hist["Hist_CastorSumEnergy"+str(ip)] = ROOT.TH1F("Hist_CastorSumEnergy"+str(ip), "Hist_CastorSumEnergy"+str(ip) ,nBinSumE, MinSumE, MaxSumE)  
            self.hist["Hist_MaxHFPlusEnergy"+str(ip)] = ROOT.TH1F("Hist_MaxHFPlusEnergy"+str(ip),"Hist_MaxHFPlusEnergy"+str(ip),nBinSumE,MinSumE, MaxSumE) 
            self.hist["Hist_MaxHFMinusEnergy"+str(ip)] = ROOT.TH1F("Hist_MaxHFMinusEnergy"+str(ip),"Hist_MaxHFMinusEnergy"+str(ip),nBinSumE,MinSumE, MaxSumE) 
            self.hist["Hist_MaxCastorEnergy"+str(ip)] = ROOT.TH1F("Hist_MaxCastorEnergy"+str(ip),"Hist_MaxCastorEnergy"+str(ip),nBinSumE,MinSumE, MaxSumE) 
            
            self.hist["Hist_2D_recogen_forwarddelta"+str(ip)] = ROOT.TH2D("Hist_2D_recogen_forwarddelta"+str(ip), "Hist_2D_recogen_forwarddelta"+str(ip), NbrDetaBins, 0,12,NbrDetaBins, 0,12) 
            
            self.hist["Hist_2D_recogen_forwarddelta_eventselection"+str(ip)] = ROOT.TH2D("Hist_2D_recogen_forwarddelta_eventselection"+str(ip), "Hist_2D_recogen_forwarddelta_eventselection"+str(ip), NbrDetaBins, 0,12,NbrDetaBins, 0,12) 
            

             # eventID is process Id which is defined by the cuts that are applied on Xi , no using process ID from Pythi8
            

            self.hist["Hist_GP_eventXiID_Min"+str(ip)] = ROOT.TH1D("Hist_GP_eventXiID_Min"+str(ip),"Hist_GP_eventXiID_Min ", NbrEtaBins, BinEtaMin, BinEtaMax)
            self.hist["Hist_GP_eventXiID_Max"+str(ip)] = ROOT.TH1D("Hist_GP_eventXiID_Max"+str(ip),"Hist_GP_eventXiID_Max ", NbrEtaBins, BinEtaMin, BinEtaMax) 
            self.hist["Hist_GP_eventXiID_DeltaMax"+str(ip)] =  ROOT.TH1F("Hist_GP_eventXiID_DeltaMax"+str(ip), "Hist_GP_eventXiID_DeltaMax"+str(ip), NbrDetaBins, DetaMin, DetaMax)
            self.hist["Hist_GP_eventXiID_DeltaZero"+str(ip)] =  ROOT.TH1F("Hist_GP_eventXiID_DeltaZero"+str(ip), "Hist_GP_eventXiID_DeltaZero"+str(ip), NbrDetaBins, DetaMin, DetaMax)
            
            self.hist["Hist_eventXiID_forwarddelta"+str(ip)] = ROOT.TH1F("Hist_eventXiID_forwarddelta"+str(ip), "Hist_eventXiID_forwarddelta"+str(ip), NbrDetaBins, 0,12)
            self.hist["Hist_eventXiID_Min"+str(ip)] = ROOT.TH1D("Hist_eventXiID_Min"+str(ip),"Hist_eventXiID_Min ", NbrEtaBins, BinEtaMin, BinEtaMax)
            self.hist["Hist_eventXiID_Max"+str(ip)] = ROOT.TH1D("Hist_eventXiID_Max"+str(ip),"Hist_eventXiID_Max ", NbrEtaBins, BinEtaMin, BinEtaMax) 
            self.hist["Hist_eventXiID_DeltaMax"+str(ip)] = ROOT.TH1F("Hist_eventXiID_DeltaMax"+str(ip), "Hist_eventXiID_DeltaMax"+str(ip), NbrDetaBins, DetaMin, DetaMax)
            self.hist["Hist_eventXiID_DeltaZero"+str(ip)] = ROOT.TH1F("Hist_eventXiID_DeltaZero"+str(ip), "Hist_eventXiID_DeltaZero"+str(ip), NbrDetaBins, DetaMin, DetaMax)
            self.hist["Hist_eventXiID_log10Mx"+str(ip)] = ROOT.TH1D("Hist_eventXiID_log10Mx"+str(ip), "Hist_eventXiID_log10Mx"+str(ip), NbrLogMBins, LogMin, LogMax)
            self.hist["Hist_eventXiID_log10My"+str(ip)] = ROOT.TH1D("Hist_eventXiID_log10My"+str(ip), "Hist_eventXiID_log10My"+str(ip), NbrLogMBins, LogMin, LogMax)
            self.hist["Hist_eventXiID_GP_2Dlog10MxMy"+str(ip)] =  ROOT.TH2D("Hist_eventXiID_GP_2Dlog10MxMy"+str(ip), "Hist_eventXiID_GP_2Dlog10MxMy"+str(ip), NbrLogMBins, LogMin, LogMax, NbrLogMBins, LogMin, LogMax)
            self.hist["Hist_eventXiID_log10XiX"+str(ip)] = ROOT.TH1F("Hist_eventXiID_log10XiX"+str(ip), "Hist_eventXiID_log10XiX"+str(ip), NbrLogXiBins, LogXiMin,LogXiMax)
            self.hist["Hist_eventXiID_log10XiY"+str(ip)] = ROOT.TH1F("Hist_eventXiID_log10XiY"+str(ip), "Hist_eventXiID_log10XiY"+str(ip), NbrLogXiBins, LogXiMin,LogXiMax)
            
          
            self.hist["Hist_GP_eventXiID_log10XiX"+str(ip)] = ROOT.TH1F("Hist_GP_eventXiID_log10XiX"+str(ip), "Hist_GP_eventXiID_log10XiX"+str(ip), NbrLogXiBins, LogXiMin,LogXiMax)
            self.hist["Hist_GP_eventXiID_log10XiY"+str(ip)] = ROOT.TH1F("Hist_GP_eventXiID_log10XiY"+str(ip), "Hist_GP_eventXiID_log10XiY"+str(ip), NbrLogXiBins, LogXiMin,LogXiMax)
            self.hist["Hist_eventXiID_2DLogGenXiX_LogGenXiY"+str(ip)] = ROOT.TH2D("Hist_eventXiID_2DLogGenXiX_LogGenXiY"+str(ip), "Hist_eventXiID_2DLogGenXiX_LogGenXiY"+str(ip), NbrLogXiBins,LogXiMin,LogXiMax, NbrLogXiBins, LogXiMin,LogXiMax)
            self.hist["Hist_eventXiID_Reco_log10XiDD"+str(ip)] = ROOT.TH1F("Hist_eventXiID_Reco_log10XiDD"+str(ip), "Hist_eventXiID_Reco_log10XiDD"+str(ip), NbrLogXiBins, LogXiMin,LogXiMax)
            self.hist["Hist_eventXiID_Energy"+str(ip)] =  ROOT.TH1F("Hist_eventXiID_Energy"+str(ip), "Hist_eventXiID_Energy"+str(ip) , NbrEBins, BinEMin, BinEMax)
            self.hist["Hist_eventXiID_Energy_barrel"+str(ip)] = ROOT.TH1F("Hist_eventXiID_Energy_barrel"+str(ip), "Hist_eventXiID_Energy_barrel"+str(ip) , NbrEBins, BinEMin, BinEMax)
            self.hist["Hist_eventXiID_Energy_endcap"+str(ip)] = ROOT.TH1F("Hist_eventXiID_Energy_endcap"+str(ip), "Hist_eventXiID_Energy_endcap"+str(ip) , NbrEBins, BinEMin, BinEMax)
            self.hist["Hist_eventXiID_Energy_endcap_forwardtransition"+str(ip)] =  ROOT.TH1F("Hist_eventXiID_Energy_endcap_forwardtransition"+str(ip), "Hist_eventXiID_Energy_endcap_forwardtransition"+str(ip) , NbrEBins, BinEMin, BinEMax)
            self.hist["Hist_eventXiID_Energy_forward"+str(ip)] =  ROOT.TH1F("Hist_eventXiID_Energy_forward"+str(ip), "Hist_eventXiID_Energy_forward"+str(ip) , NbrEBins, BinEMin, BinEMax)
            self.hist["Hist_eventXiID_Energy_forwardplus"+str(ip)] =  ROOT.TH1F("Hist_eventXiID_Energy_forwardplus"+str(ip), "Hist_eventXiID_Energy_forwardplus"+str(ip) , NbrEBins, BinEMin, BinEMax)
            self.hist["Hist_eventXiID_Energy_forwardminus"+str(ip)] =  ROOT.TH1F("Hist_eventXiID_Energy_forwardminus"+str(ip), "Hist_eventXiID_Energy_forwardminus"+str(ip) , NbrEBins, BinEMin, BinEMax)
            self.hist["Hist_eventXiID_Energy_Castor"+str(ip)] =  ROOT.TH1F("Hist_eventXiID_Energy_Castor"+str(ip), "Hist_eventXiID_Energy_Castor"+str(ip) , NbrEBins, BinEMin, BinEMax)
            self.hist["Hist_eventXiID_reducedEnergy"+str(ip)] =  ROOT.TH1F("Hist_eventXiID_reducedEnergy"+str(ip), "Hist_eventXiID_reducedEnergy"+str(ip) , NbrEBins, BinEMin, BinEMax)
            self.hist["Hist_eventXiID_reducedEnergy_barrel"+str(ip)] =  ROOT.TH1F("Hist_eventXiID_reducedEnergy_barrel"+str(ip), "Hist_eventXiID_reducedEnergy_barrel"+str(ip) , NbrEBins, BinEMin, BinEMax)
            self.hist["Hist_eventXiID_reducedEnergy_endcap"+str(ip)] =  ROOT.TH1F("Hist_eventXiID_reducedEnergy_endcap"+str(ip), "Hist_eventXiID_reducedEnergy_endcap"+str(ip) , NbrEBins, BinEMin, BinEMax)
            self.hist["Hist_eventXiID_reducedEnergy_endcap_forwardtransition"+str(ip)] =  ROOT.TH1F("Hist_eventXiID_reducedEnergy_endcap_forwardtransition"+str(ip), "Hist_eventXiID_reducedEnergy_endcap_forwardtransition"+str(ip) , NbrEBins, BinEMin, BinEMax)
            self.hist["Hist_eventXiID_reducedEnergy_forward"+str(ip)] =  ROOT.TH1F("Hist_eventXiID_reducedEnergy_forward"+str(ip), "Hist_eventXiID_reducedEnergy_forward"+str(ip) , NbrEBins, BinEMin, BinEMax)
            self.hist["Hist_eventXiID_reducedEnergy_forwardplus"+str(ip)] =  ROOT.TH1F("Hist_eventXiID_reducedEnergy_forwardplus"+str(ip), "Hist_eventXiID_reducedEnergy_forwardplus"+str(ip) , NbrEBins, BinEMin, BinEMax)
            self.hist["Hist_eventXiID_reducedEnergy_forwardminus"+str(ip)] =  ROOT.TH1F("Hist_eventXiID_reducedEnergy_forwardminus"+str(ip), "Hist_eventXiID_reducedEnergy_forwardminus"+str(ip) , NbrEBins, BinEMin, BinEMax)
            self.hist["Hist_eventXiID_reducedEnergy_Castor"+str(ip)] =  ROOT.TH1F("Hist_eventXiID_reducedEnergy_Castor"+str(ip), "Hist_eventXiID_reducedEnergy_Castor"+str(ip) , NbrEBins, BinEMin, BinEMax)
            self.hist["Hist_eventXiID_numberoftowerebovenoise_forwardplus"+str(ip)] = ROOT.TH1F("Hist_eventXiID_numberoftowerebovenoise_forwardplus"+str(ip),"Hist_eventXiID_numberoftowerebovenoise_forwardplus"+str(ip),ntow,Mintow,Maxtow) 
            self.hist["Hist_eventXiID_numberoftowerebovenoise_forwardminus"+str(ip)] = ROOT.TH1F("Hist_eventXiID_numberoftowerebovenoise_forwardminus"+str(ip),"Hist_eventXiID_numberoftowerebovenoise_forwardminus"+str(ip),ntow,Mintow,Maxtow) 
            self.hist["Hist_eventXiID_numberoftowerebovenoise_castor"+str(ip)] = ROOT.TH1F("Hist_eventXiID_numberoftowerebovenoise_castor"+str(ip),"Hist_eventXiID_numberoftowerebovenoise_castor"+str(ip),16, 0,16) 
            self.hist["Hist_eventXiID_CaloReducedenergyClass"+str(ip)]= ROOT.TH1F("Hist_eventXiID_CaloReducedenergyClass"+str(ip),"Hist_eventXiID_CaloReducedenergyClass"+str(ip),NbrEBins, BinEMin, BinEMax)
           
            self.hist["Hist_eventXiID_HFPlusSumEnergy"+str(ip)] = ROOT.TH1F("Hist_eventXiID_HFPlusSumEnergy"+str(ip), "Hist_eventXiID_HFPlusSumEnergy"+str(ip) ,nBinSumE, MinSumE, MaxSumE)
            self.hist["Hist_eventXiID_HFMinusSumEnergy"+str(ip)] = ROOT.TH1F("Hist_eventXiID_HFMinusSumEnergy"+str(ip), "Hist_eventXiID_HFMinusSumEnergy"+str(ip) ,nBinSumE, MinSumE, MaxSumE)
            self.hist["Hist_eventXiID_CastorSumEnergy"+str(ip)] = ROOT.TH1F("Hist_eventXiID_CastorSumEnergy"+str(ip), "Hist_eventXiID_CastorSumEnergy"+str(ip) ,nBinSumE, MinSumE, MaxSumE)  
            self.hist["Hist_eventXiID_MaxHFPlusEnergy"+str(ip)] = ROOT.TH1F("Hist_eventXiID_MaxHFPlusEnergy"+str(ip),"Hist_eventXiID_MaxHFPlusEnergy"+str(ip),nBinSumE,MinSumE, MaxSumE)
            self.hist["Hist_eventXiID_MaxHFMinusEnergy"+str(ip)] = ROOT.TH1F("Hist_eventXiID_MaxHFMinusEnergy"+str(ip),"Hist_eventXiID_MaxHFMinusEnergy"+str(ip),nBinSumE,MinSumE, MaxSumE)  
            self.hist["Hist_eventXiID_MaxCastorEnergy"+str(ip)] = ROOT.TH1F("Hist_eventXiID_MaxCastorEnergy"+str(ip),"Hist_eventXiID_MaxCastorEnergy"+str(ip),nBinSumE,MinSumE, MaxSumE) 
     
      



        for h in self.hist:
            self.hist[h].Sumw2()
            self.GetOutputList().Add(self.hist[h])

        self.pdg_helper = pd.PYTHIAParticleData()
        self.castor_tower_p4 = []
        self.CMenergy = 13000 # GeV
        self.protonmass = 0.938 # GeV
        self.logXi_p = -8.28 # Mp*2/CMenergylog10(1/13e3/13e3)
        for isec in xrange(0,16):
            self.castor_tower_p4.append( ROOT.ROOT.Math.LorentzVector('ROOT::Math::PxPyPzE4D<double>')(0,0,0,0) )

        self.OUTlog10XixGen = array( 'f', 1 * [0] )
        self.OUTlog10XiyGen = array( 'f', 1 * [0] )
        self.OUTlog10XixReco = array( 'f', 1 * [0] )
        self.OUTlog10XiyReco = array( 'f', 1 * [0] )
        self.OUTlog10MxGen = array( 'f', 1 * [0] )
        self.OUTlog10MyGen = array( 'f', 1 * [0] )
        self.OUTlog10MxReco = array( 'f', 1 * [0] )
        self.OUTlog10MyReco = array( 'f', 1 * [0] )
        self.OUTdeltazero = array( 'f', 1 * [0] )
        self.OUTforwardgendelta = array('f', 1 * [0])
        self.OUTdeltaeta = array( 'f', 1 * [0] )
        self.OUTetamin = array( 'f', 1 * [0] )
        self.OUTrapditygapmean = array( 'f', 1 * [0] )
        self.OUTetamax = array( 'f', 1 * [0] )
        self.OUTCastorNtowers = array( 'i', 1 * [0] )
        self.OUTHFminusNtowers = array( 'i', 1 * [0] )
        self.OUTNtracks = array( 'i', 1 * [0] )
        self.OUTEtarange = array( 'f', 1 * [0] ) 
        self.OUTHFplusNtowers = array( 'i', 1 * [0] )
        self.OUTlog10XiDD = array( 'f', 1 * [0] )
        self.OUTPythia8processid = array('i', 1 * [0] )
        self.OUTEventselectionXiprocessDD = array( 'i', 1 * [0] )
        self.OUTEventselectionXiprocessSD1 = array( 'i', 1 * [0] )
        self.OUTEventselectionXiprocessSD2 = array( 'i', 1 * [0] )
        self.OUTEventselectionXiprocessRest = array( 'i', 1 * [0] )
        self.OUTCaloReducedenergyClass = array( 'i', 1 * [0] )
        self.OUTCastorSumEnergy= array( 'f', 1 * [0] )
        self.OUTHFMinusSumEnergy= array('f', 1 * [0] )
        self.OUTMaxHFMinusEnergy = array('f', 1 * [0])
        self.OUTHFPlusSumEnergy= array('f', 1 * [0] )
        self.OUTMaxHFPlusEnergy = array('f', 1 * [0])
        self.OUTMaxCastorEnergy = array('f', 1 * [0])
        # self.Ou

        
        

        AllTree = ROOT.TTree("AllTree", "all events")
        self.CreateTree(AllTree)
        setattr(self, "AllTree",AllTree)
        self.addToOutput(self.AllTree)
        

    def get_Pythia_Process_ID(self):
        # if self.fChain.processID == 101:
        #     return '_Rest', self.fChain.processID
        if self.fChain.processID == 103:
            return '_SD1', self.fChain.processID
        if self.fChain.processID == 104:
            return '_SD2', self.fChain.processID
        if self.fChain.processID == 105:
            return '_DD', self.fChain.processID
        # if self.fChain.processID == 106:
        #     return '_CD', self.fChain.processID

        return '_Rest', -1



    def get_MultiEventSelectionXiProcess_ID(self,Genlog10XiX,Genlog10XiY,deltaeta,protonMinus,protonPlus):
        Classes = []

        if deltaeta > 4 and (Genlog10XiY> -6 or Genlog10XiX >-7) and not protonMinus and not protonPlus : #delta>3
        # if (log10(GenXi_DD) < -5) :
            Classes.append('_DD')
        
        if (Genlog10XiY> -6 and Genlog10XiY <-1 and protonMinus):    
        # if (Genlog10XiY> -6 and abs(Genlog10XiX - self.logXi_p)< 0.04):    
            Classes.append('_SD1')

        if  (Genlog10XiX >-7 and Genlog10XiX <-1 and protonPlus):
        # if     (Genlog10XiX >-7 and abs(Genlog10XiY - self.logXi_p)< 0.04):
            Classes.append('_SD2')
        
        if (len(Classes)==0):
            Classes.append('_Rest')

        return Classes


    def get_Process_ID_from_String(self,process):
        if "Rest" in process:
            return -1
        if "SD1" in process:
            return 103
        if "SD2" in process:
            return 104
        if "DD" in process:
            return 105
        if "CD" in process:
            return -1

        sys.exit(1)

    

    def analyze(self):
        self.hist["BunchCrossing"].Fill(self.fChain.bx)
        self.hist["Runs"].Fill(self.fChain.run)
        if self.isData:
            if self.ParameterSet == 'Melike_dNdEta'.lower() or self.ParameterSet == 'Seb_dNdEta_LHCf'.lower():
                if not self.fChain.run == 247324: return 1
                if self.fChain.lumi<97 or self.fChain.lumi>311 or not self.fChain.bx == 208: return 1  
           
            elif self.ParameterSet == 'Seb_LHCf_Run247920'.lower(): 
                if not self.fChain.run == 247920: return 1

            elif self.ParameterSet == 'Seb_LHCf_Run247934'.lower() or self.ParameterSet== 'data_247934'.lower():
                if not self.fChain.run == 247934 : return 1
                if self.fChain.lumi<26 or self.fChain.lumi>570: return 1   
            
            else:
                if not self.fChain.run == 247934: return 1 #247934   
                # if self.fChain.lumi<26 or self.fChain.lumi>570: return 1    
            


        Nbrvertex = 1 

        self.hist["hNentries"].Fill("all",1)
        self.hist["hNentriesWithEventSelectionCuts"].Fill("all",1)  

        vVertexZ = 0
        if self.ParameterSet == 'cuet'.lower() or self.ParameterSet == 'cuetplus'.lower() or self.ParameterSet == 'mbr'.lower()or self.ParameterSet == 'epos'.lower() or self.ParameterSet == 'Seb_LHCf_Run247934'.lower() or self.ParameterSet == 'Seb_LHCf_Run247920'.lower():    
            vVertexZ = self.fChain.ZeroTeslaTracking_PixelnoPreSplitting_VtxZ
        else:
            vVertexZ = self.fChain.ZeroTeslaPixelnoPreSplittingVtx_vrtxZ

        Nbrvertex = vVertexZ.size()

        deltavertex = 0
        if Nbrvertex == 2:
            deltavertex = (abs(vVertexZ[0] - vVertexZ[1]))
            self.hist["Hist_NrVtxdelta"].Fill(deltavertex)

        self.hist["Hist_NrVtx"].Fill(Nbrvertex)

        if self.isData:           
            if Nbrvertex > 2:  return 0

        self.hist["hNentries"].Fill("Nvrtxcut",1)
        self.hist["hNentriesWithEventSelectionCuts"].Fill("Nvrtxcut",1)  

        if Nbrvertex == 2:
            self.hist["Hist_NrVtxdeltacut"].Fill(deltavertex)
            if self.isData and abs(deltavertex)> 0.5 : return 0
            if not self.isData and abs(deltavertex)> 0.5 :
                self.hist["hNentries"].Fill("mc_vtxcut",1)
                self.hist["hNentriesWithEventSelectionCuts"].Fill("mc_vtxcut",1)
            
        
        self.hist["hNentries"].Fill("vtxcut",1)
        self.hist["hNentriesWithEventSelectionCuts"].Fill("vtxcut",1)          

        weight = 1
        num = 0
        # genTracks
        #num = self.fChain.genTracks.size()
        Eta = 0
        theta= 0
        Ntrack = 0
        phi = 0
        #print self.maxEta # see slaveParams below
        #self.hist["numGenTracks"].Fill(1)
       

        #############################################################
        # also write a ttree          #
        #               #
        # create the branches and assign the fill-variables to them #
        #############################################################
        
        self.hist["hProcessescut"].Fill("all",1)
        self.hist["hProcessesIdPythia"].Fill("all",1)

        Pythia_Process_ID = '_NONE'
        int_Pythia_Process_ID = -11
        EventSelectionXiProcess_IDs = []
        
        if not self.isData:
            Pythia_Process_ID, int_Pythia_Process_ID = self.get_Pythia_Process_ID()
            self.hist["hProcessesIdPythia"].Fill(Pythia_Process_ID[1:],1)
   
        self.hist["hNentriesWithEventSelectionCuts"].Fill("Vrtx Cut",1)  
        
   
        #######################HFCUT##################################    
        HFCut = False
        CaloTower = self.fChain.CaloTowersp4.size()
        for icalo in xrange(0,CaloTower):
            calop4 = self.fChain.CaloTowersp4[icalo]
            caloem = self.fChain.CaloTowersemEnergy[icalo]
            calohad = self.fChain.CaloTowershadEnergy[icalo]
           
            if abs(calop4.eta()) > 3.2 and abs(calop4.eta()) < 5.2 :
                if (calop4.e()) > 5:
                    HFCut = True
                    

        
        # if HFCut == False : return 0
        
        Eta = 0
        GenParticleClass = []
        ChargedGenParticleClass = []
        theta = 0
        mingeneta = -1
        maxgeneta = -1
       
        forwardgendelta= -1
        deltaeta=-1
        delta_zero_gen = -1
        delta_max_gen = -1
        delta_Gen_zero_pos = -1
        deltaetamax_Gen_pos = -1
        # deltagenreco = -1
        Etarange = -1
        CLOSE = 0.004
        mass=0
        # self.hist["hNentriess"].Fill("hf cut",1)         
       
        if  not self.isData:
           
            ngenParticle = self.fChain.genParticlesp4.size()
            # final state particles from the genParticles collection
            for igenP in xrange(ngenParticle):
                genp4 = self.fChain.genParticlesp4[igenP]
                genid = self.fChain.genParticlespdg[igenP]
                genst = self.fChain.genParticlesstatus[igenP]
              
                if genst != 1: continue
                GenParticleClass.append([genp4,genid])
                if self.pdg_helper.charge(genid) == 0 : continue ######### only use charged particles
                ChargedGenParticleClass.append([genp4,genid])
       
       
            # if len(ChargedGenParticleClass) == 0:
            #     return 0

            # ChargedGenParticleClass.sort(cmp=compareGeneta)
            ChargedGenParticleClass.sort(cmp=compareGenrapidity)
            GenParticleClass.sort(cmp=compareGenrapidity)
            mingeneta = ChargedGenParticleClass[0][0].eta()
            maxgeneta = ChargedGenParticleClass[len(ChargedGenParticleClass)-1][0].eta()
            self.hist["Hist_GP_Min"].Fill(mingeneta)
            self.hist["Hist_GP_Max"].Fill(maxgeneta)
            Etarange = maxgeneta - mingeneta
            forwardgendelta = max(abs(mingeneta -(-6.0)), abs(maxgeneta-5.2))
            
           
            self.hist["Hist_GP_Delta"].Fill(Etarange)
            self.hist["Hist_GP_forwardgendelta"].Fill(forwardgendelta)
            self.hist["Hist_GP_eventXiID_forwardgendelta"].Fill(forwardgendelta)
            




            for igp in xrange(len(GenParticleClass)-1):
                genp4  = GenParticleClass[igp][0]
                genid  = GenParticleClass[igp][1]
                deltaeta = GenParticleClass[igp+1][0].eta() - GenParticleClass[igp][0].eta() 
               
                if  (deltaeta > delta_max_gen):
                    delta_max_gen = deltaeta
                    deltaetamax_Gen_pos = igp

                if GenParticleClass[igp+1][0].eta() > 0 and GenParticleClass[igp][0].eta() < 0:
                    delta_zero_gen = deltaeta
                    delta_Gen_zero_pos = igp

            

            protonMinus = False
            if (deltaetamax_Gen_pos == 0):
                mass = GenParticleClass[0][0].M()
                if (abs(mass - self.protonmass) < CLOSE):
                    protonMinus = True
     
            
           
            protonPlus = False
            if (deltaetamax_Gen_pos == len(GenParticleClass)-2):
                mass = GenParticleClass[len(GenParticleClass)-1][0].M()
                if (abs(mass - self.protonmass) < CLOSE):
                    protonPlus = True 
           
        


            self.hist["Hist_GP_DeltaMax"].Fill(delta_max_gen)
            self.hist["Hist_GP_DeltaZero"].Fill(delta_zero_gen)
            self.hist["Hist_GP_Min" + Pythia_Process_ID].Fill(mingeneta)
            self.hist["Hist_GP_Max" + Pythia_Process_ID].Fill(maxgeneta)  
            self.hist["Hist_GP_Delta" + Pythia_Process_ID].Fill(Etarange)
            self.hist["Hist_GP_DeltaMax" + Pythia_Process_ID].Fill(delta_max_gen)
            self.hist["Hist_GP_DeltaZero" + Pythia_Process_ID].Fill(delta_zero_gen)
            
           

            # if len(GenParticleClass) == 0:
            #     return 0

            # calculate Mx2 and My2 for all genetrated particles
            XGenEtot = 0
            XPxtot = 0
            XPytot = 0
            XPztot = 0
            YGenEtot = 0
            YPxtot = 0
            YPytot = 0
            YPztot = 0
            Mx2 = 0
            My2 = 0
            MxGen = 0 
            MyGen = 0 
            MeanGen =0
            Px = 0
            Py= 0
            Px2 = 0
            Py2= 0
            GenXi_DD = 0


            for igp in xrange(0,len(GenParticleClass)): 
#                if GenParticleClass[igp][0].eta() <= ChargedGenParticleClass[deltaetamax_Gen_pos][0].eta():
                if GenParticleClass[igp][0].eta() <= GenParticleClass[deltaetamax_Gen_pos][0].eta():
                    XGenEtot += GenParticleClass[igp][0].E()
                    XPxtot += GenParticleClass[igp][0].Px()
                    XPytot += GenParticleClass[igp][0].Py()
                    XPztot += GenParticleClass[igp][0].Pz()
                    
#                if GenParticleClass[igp][0].eta() >= ChargedGenParticleClass[deltaetamax_Gen_pos+1][0].eta():
                if GenParticleClass[igp][0].eta() >= GenParticleClass[deltaetamax_Gen_pos+1][0].eta():
                    YGenEtot += GenParticleClass[igp][0].E()
                    YPxtot += GenParticleClass[igp][0].Px()
                    YPytot += GenParticleClass[igp][0].Py()
                    YPztot += GenParticleClass[igp][0].Pz()

#                MeanGen = (ChargedGenParticleClass[deltaetamax_Gen_pos+1][0].eta() + ChargedGenParticleClass[deltaetamax_Gen_pos][0].eta())/2 
                MeanGen = (GenParticleClass[deltaetamax_Gen_pos+1][0].eta() + GenParticleClass[deltaetamax_Gen_pos][0].eta())/2 
               

            Px2 =  XPxtot**2 + XPytot**2 +XPztot**2
            Py2 =  YPxtot**2 + YPytot**2 +YPztot**2
            Px = sqrt(Px2)
            Py = sqrt(Py2)

            Mx2 = (XGenEtot+Px)*(XGenEtot-Px)
            My2 = (YGenEtot+Py)*(YGenEtot-Py)  
            if Mx2<=0: Mx2=1.01e-6
            if My2<=0: My2=1.01e-6
          
           
            MyGen = sqrt(My2)
            MxGen = sqrt(Mx2)
            self.hist["Hist_GP_Mx"].Fill(MxGen)
            self.hist["Hist_GP_log10Mx"].Fill(log10(MxGen))
            self.hist["Hist_GP_My"].Fill(MyGen)
            self.hist["Hist_GP_log10My"].Fill(log10(MyGen))
            self.hist["Hist_2D_GP_log10MxMy"].Fill(log10(MxGen),log10(MyGen))
            
            self.hist["Hist_GP_log10Mx"+ Pythia_Process_ID].Fill(log10(MxGen))
            self.hist["Hist_GP_log10My"+ Pythia_Process_ID].Fill(log10(MyGen))
            self.hist["Hist_2D_GP_log10MxMy"+ Pythia_Process_ID].Fill(log10(MxGen),log10(MyGen))

            
           

          
            GenXiX = Mx2/self.CMenergy/self.CMenergy
            GenXiY = My2/self.CMenergy/self.CMenergy
            Genlog10XiX = log10(GenXiX)
            Genlog10XiY = log10(GenXiY)

            GenXi_DD = Mx2*My2/(self.protonmass**2 * self.CMenergy**2)
            GenMaxXi = max(GenXiX, GenXiY)
           
            self.hist["Hist_GP_log10XiX"].Fill(log10(GenXiX))
            self.hist["Hist_GP_log10XiY"].Fill(log10(GenXiY))
            self.hist["Hist_GP_log10MaxXi"].Fill(log10(GenMaxXi))
            self.hist["Hist_GP_log10XiX"+Pythia_Process_ID].Fill(log10(GenXiX))
            self.hist["Hist_GP_log10XiY"+Pythia_Process_ID].Fill(log10(GenXiY))
            self.hist["Hist_GP_log10MaxXi"+Pythia_Process_ID].Fill(log10(GenMaxXi))
            self.hist["Hist_2DLogGenXiX_LogGenXiY"].Fill(log10(GenXiX),log10(GenXiY))
            self.hist["Hist_2DLogGenXiX_LogGenXiY"+Pythia_Process_ID].Fill(log10(GenXiX),log10(GenXiY))
            self.hist["Hist_GP_log10XiDD"].Fill(log10(GenXi_DD))  
            self.hist["Hist_GP_log10XiDD"+Pythia_Process_ID].Fill(log10(GenXi_DD)) 
            self.hist["Hist_2D_log10XiDD_Deltamax"].Fill(log10(GenXi_DD), delta_max_gen)
            self.hist["Hist_2D_log10XiDD_Deltamax"+Pythia_Process_ID].Fill(log10(GenXi_DD), delta_max_gen)   
            if protonPlus:
                self.hist["Hist_GP_log10XiX_protonplus"].Fill(log10(GenXiX))
                self.hist["Hist_GP_log10XiX_protonplus" + Pythia_Process_ID].Fill(log10(GenXiX))
            if protonMinus:
                self.hist["Hist_GP_log10XiY_protonminus"].Fill(log10(GenXiY))
                self.hist["Hist_GP_log10XiY_protonminus" + Pythia_Process_ID].Fill(log10(GenXiY))

            # EventSelectionXiProcess_ID, int_EventSelectionXiProcess_ID = self.get_EventSelectionXiProcess_ID(GenXi_DD,GenXiX,GenXiY)
            EventSelectionXiProcess_IDs = self.get_MultiEventSelectionXiProcess_ID(Genlog10XiX,Genlog10XiY,delta_max_gen,protonMinus,protonPlus)
           

            for EventSelectionXiProcess_ID in  EventSelectionXiProcess_IDs:
                self.hist["hProcessescut"].Fill(EventSelectionXiProcess_ID[1:],1)
                self.hist["Hist_GP_eventXiID_DeltaMax"+ EventSelectionXiProcess_ID].Fill(delta_max_gen)
                self.hist["Hist_GP_eventXiID_DeltaZero"+ EventSelectionXiProcess_ID].Fill(delta_zero_gen)
                self.hist["Hist_GP_eventXiID_Min"+ EventSelectionXiProcess_ID].Fill(mingeneta)
                self.hist["Hist_GP_eventXiID_Max"+ EventSelectionXiProcess_ID].Fill(maxgeneta)  
                self.hist["Hist_eventXiID_GP_2Dlog10MxMy"+ EventSelectionXiProcess_ID].Fill(log10(MxGen),log10(MyGen))
                self.hist["Hist_eventXiID_log10Mx"+ EventSelectionXiProcess_ID].Fill(log10(MxGen))
                self.hist["Hist_eventXiID_log10My"+ EventSelectionXiProcess_ID].Fill(log10(MyGen))
                self.hist["Hist_GP_eventXiID_log10XiX"+EventSelectionXiProcess_ID].Fill(log10(GenXiX))
                self.hist["Hist_GP_eventXiID_log10XiY"+EventSelectionXiProcess_ID].Fill(log10(GenXiY))
                self.hist["Hist_eventXiID_2DLogGenXiX_LogGenXiY"+EventSelectionXiProcess_ID].Fill(log10(GenXiX),log10(GenXiY))
               

                    
            self.hist["Hist_GP_eventXiID_DeltaMax"].Fill(delta_max_gen)
            self.hist["Hist_GP_eventXiID_DeltaZero"].Fill(delta_zero_gen)
            self.hist["Hist_GP_eventXiID_Min"].Fill(mingeneta)
            self.hist["Hist_GP_eventXiID_Max"].Fill(maxgeneta) 
            self.hist["Hist_GP_eventXiID_log10XiX"].Fill(log10(GenXiX))
            self.hist["Hist_GP_eventXiID_log10XiY"].Fill(log10(GenXiY)) 
            

            
        ##################Track##############################################################3
        TrackCandClass = []
        Nbrtracks = 0


        vTrackTheta = 0
        vTrackPhi   = 0
        if self.ParameterSet == 'mbr'.lower() or self.ParameterSet == 'cuet'.lower() or self.ParameterSet == 'epos'.lower() or self.ParameterSet == 'cuetplus'.lower() or self.ParameterSet == 'Seb_LHCf_Run247934'.lower() or self.ParameterSet == 'Seb_LHCf_Run247920'.lower(): 
            vTrackTheta = self.fChain.ZeroTeslaTracking_PixelnoPreSplitting_TrackTheta
            vTrackPhi   = self.fChain.ZeroTeslaTracking_PixelnoPreSplitting_TrackPhi
        else:
            vTrackTheta = self.fChain.ZeroTeslaPixelnoPreSplittingVtx_trktheta
            vTrackPhi   = self.fChain.ZeroTeslaPixelnoPreSplittingVtx_trkphi

        nTrackCand = vTrackPhi.size()
        # nTrackCand = self.fChain.ZeroTeslaPixelnoPreSplittingVtx_trktheta.size()
       
        for itrk in xrange(nTrackCand):

            if self.Track_efficiency_scale<1.0:
                if (random.random() > self.Track_efficiency_scale):
                    continue

            theta  = vTrackTheta[itrk]
            trkphi = vTrackPhi[itrk]
            
            # theta= self.fChain.ZeroTeslaPixelnoPreSplittingVtx_trktheta[itrk]
            # trkphi = self.fChain.ZeroTeslaPixelnoPreSplittingVtx_trkphi[itrk]


            if (theta<=0):
                print ("Negative theta particle theta="+ str(theta*180/3.141))
                continue
            Eta = -np.log(math.tan(theta/2))
            if (abs(Eta)>2.2): continue
            TrackCandClass.append([Eta,trkphi])
            
        Nbrtracks = len(TrackCandClass)

       


         ####################################Calotower######################################


        CaloCandClass = []
        CaloReducedenergyClass = []
        CaloTower = self.fChain.CaloTowersp4.size()
       
        #noise cut i am using the vrtx cut ==0 and vrtx cut ==1
         
      
        CASTOR_Numberoftowerebovenoise = 0
        HFplus_Numberoftowerebovenoise = 0 
        HFminus_Numberoftowerebovenoise = 0 
        Barrel_Numberoftowerebovenoise = 0 
        Endcap_Numberoftowerebovenoise = 0 
        endcapforwardtransition_Numberoftowerebovenoise =0
        

        bad_channels_eta_phi = []
        # if self.ParameterSet == 'Seb_LHCf_Run247934'.lower() or self.ParameterSet == 'Seb_LHCf_Run247920'.lower() or not self.isData :
        # if self.ParameterSet == self.ParameterSet == 'epos'.lower():   
        bad_channels_eta_phi = bad_channels_eta_phi_Run247934_20 
        
         

        for icalo in xrange(0,CaloTower):
            calop4 = self.fChain.CaloTowersp4[icalo]
            caloem = self.fChain.CaloTowersemEnergy[icalo]
            calohad = self.fChain.CaloTowershadEnergy[icalo]
            caloieta = self.fChain.CaloTowersieta[icalo]  # Sebastian version has ieta and iphi
            caloiphi = self.fChain.CaloTowersiphi[icalo] #Sebastian version has ieta and iphi

            self.hist["Hist_2D_Calotower_eta_phi_Alltowers"].Fill(caloieta, caloiphi)
            self.hist["Hist_2D_Calotower_withoutnoisecut_eta_phi"].Fill(caloieta, caloiphi)


            # if [caloieta,caloiphi] in bad_channels_eta_phi: continue!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            if [caloieta,caloiphi] in bad_channels_eta_phi: continue

            self.hist["Hist_2D_Calotower_withnoisecut_eta_phi"].Fill(caloieta, caloiphi)

            CaloCandClass.append([calop4,caloem,calohad])
            
           
            if abs(calop4.eta()) < 1.4:
               
                # if calop4.e() < 2.00: continue
                if calop4.e() < 2.2: continue
                Barrel_Numberoftowerebovenoise += 1 


            elif abs(calop4.eta()) < 2.8:
                if calop4.e() < 3.8 : continue 
                # if calop4.e() < 3.0: continue //20% =3.8to  5
                Endcap_Numberoftowerebovenoise += 1

            elif abs(calop4.eta()) < 3.152: 
                if calop4.e() < 4.5: continue
                # if calop4.e() < 3.8: continue 4.5to 6
                endcapforwardtransition_Numberoftowerebovenoise += 1  
                 
            # if abs(calop4.eta()) > 3.2 and abs(calop4.eta()) < 5.2 :
            #     if (calop4.e()) < 2.2: continue
            #     endcapforwardtransition_Numberoftowerebovenoise += 1  

            #elif abs(calop4.eta())< 5.2:
            else: # only HF left here 
                calop4 *= self.HF_energy_scale
                if not self.isData:
                    calop4 *= 1/1.117   
                if calop4.e() <5: continue
                # if calop4.e() <5: continue
                if (calop4.eta()>0):
                    HFplus_Numberoftowerebovenoise += 1
                else:
                    HFminus_Numberoftowerebovenoise += 1
                # self.hist["Hist_Energy_forwardplus"]
           
            # CaloReducedenergyClass.append([calop4,caloem,calohad])  #Sebastian cuet
            CaloReducedenergyClass.append([calop4,caloem,calohad,caloieta,caloiphi])

     
        ####################################Castortower######################################
       
        self.sum_CAS_E_em = [0.0] *16
        self.sum_CAS_E_had = [0.0] * 16
        self.sum_CAS_E = [0.0] * 16
        self.castor_id = [0] * 16
        self.castor_tower_E = [0.0] * 16
        


        if self.fChain.CastorRecHitEnergy.size() != 224:
            return 0


        for i in xrange(0, 224):
            isec = i//14
            imod = i%14
            if imod< 2:
               self.sum_CAS_E_em[isec] += self.fChain.CastorRecHitEnergy[i]
               # print "sum_CAS_E_em[isec][imod]", self.sum_CAS_E_em[isec]
            elif imod< 5:
                self.sum_CAS_E_had[isec] += self.fChain.CastorRecHitEnergy[i]
                # print "sum_CAS_E_had[isec][imod]", self.sum_CAS_E_had[isec]

            self.castor_tower_E[isec] += self.fChain.CastorRecHitEnergy[i]
          

      
        for isec in xrange(0,16): 
            self.sum_CAS_E[isec] = self.sum_CAS_E_em[isec] + self.sum_CAS_E_had[isec]
            self.hist["Hist_sum_CAS_E"].Fill(self.sum_CAS_E[isec])

            if  self.sum_CAS_E_em[isec] > self.sum_CAS_E_had[isec]:
                self.castor_id[isec] = 9 #3 is_gamma id
            else : self.castor_id[isec] = 8 # charged  hadron

            energy = self.sum_CAS_E[isec]
            energy *= self.Castor_energy_scale

            caseta = -6.0
            castheta = 2*atan(exp(-caseta))

            px = energy*sin(castheta) * cos(isec*pi/16. + pi/32.)
            py = energy*sin(castheta) * sin(isec*pi/16. + pi/32.)
            pz = energy*cos(castheta) 
            self.castor_tower_p4[isec].SetPxPyPzE(px,py,pz,energy)

            self.hist["Hist_2D_Calotower_eta_phi_Alltowers"].Fill(-45,4*isec+1)

            #add castoreta in calocandclass
            CaloCandClass.append([self.castor_tower_p4[isec],
                            self.sum_CAS_E_em[isec],
                            self.sum_CAS_E_had[isec],
                            -45,4*isec+1])
            # print ("calop4: " ,self.castor_tower_p4[isec].eta() )

            # print ("calop4: " ,calop4 )
            
            # if (isec==0) or (isec==14) : continue

            if (energy) < 5.0:continue 
            # if (energy) < 1.5:continue   #Threshold for castor: 5.600000 ############# Castor noise cut

            CASTOR_Numberoftowerebovenoise += 1
            
            CaloReducedenergyClass.append([self.castor_tower_p4[isec], 
                                # self.castor_id[isec],
                                self.sum_CAS_E_em[isec],
                                self.sum_CAS_E_had[isec],
                                -45,4*isec+1]) # something strange for ieta, iphi
    
            
              
        for icalo in CaloReducedenergyClass:
            
            CaloTwriEta = icalo[3]
            CaloTwrsiPhi = icalo[4]

            self.hist["Hist_Calotower_eta"].Fill(icalo[0].eta())
            self.hist["Hist_2D_Calotower_ieta_eta"].Fill(icalo[0].eta(),CaloTwriEta)
            self.hist["Hist_2D_Calotower_iphi_phi"].Fill(icalo[0].phi(),CaloTwrsiPhi )
            self.hist["Hist_2D_Calotower_Energy_eta_ieta"].Fill(icalo[0].eta(),CaloTwriEta, icalo[0].e() )
            self.hist["Hist_2D_Calotower_Energy_phi_iphi"].Fill(icalo[0].eta(),CaloTwriEta, icalo[0].e() )

          

        ####### Vrtx cut######  
        # if self.isData:
        # if (Nbrtracks)== 0 and len(CaloReducedenergyClass) < 4: return 0
        # if (Nbrtracks)== 0 and len(CaloReducedenergyClass) == 0: return 0
        if (Nbrtracks== 0) and(CASTOR_Numberoftowerebovenoise == 0 and HFplus_Numberoftowerebovenoise == 0 and HFminus_Numberoftowerebovenoise == 0 ):return 0
        
        if len(CaloReducedenergyClass) == 1:
            for icalo in CaloReducedenergyClass:
              
                CaloTwriEta = icalo[3]
                CaloTwrsiPhi = icalo[4]

                

                if(CaloTwriEta == -45): self.hist["Castortower2D_Energy_sec"].Fill(icalo[0].e(),CaloTwrsiPhi)

        
        self.hist["hNentriesWithEventSelectionCuts"].Fill("Nbrtracks&Towers Cut",1)

        for EventSelectionXiProcess_ID in  EventSelectionXiProcess_IDs:

            self.hist["Hist_eventXiID_numberoftowerebovenoise_castor"+ EventSelectionXiProcess_ID].Fill(CASTOR_Numberoftowerebovenoise)
            self.hist["Hist_eventXiID_numberoftowerebovenoise_forwardplus"+ EventSelectionXiProcess_ID].Fill(HFplus_Numberoftowerebovenoise)
            self.hist["Hist_eventXiID_numberoftowerebovenoise_forwardminus"+ EventSelectionXiProcess_ID].Fill(HFminus_Numberoftowerebovenoise)
            self.hist ["Hist_eventXiID_CaloReducedenergyClass"+ EventSelectionXiProcess_ID ].Fill(len(CaloReducedenergyClass))



        self.hist["Hist_numberoftowerebovenoise_castor"].Fill(CASTOR_Numberoftowerebovenoise)
        self.hist["Hist_eventXiID_numberoftowerebovenoise_castor"].Fill(CASTOR_Numberoftowerebovenoise)
        self.hist["Hist_numberoftowerebovenoise_castor"+ Pythia_Process_ID].Fill(CASTOR_Numberoftowerebovenoise)
        self.hist["Hist_numberoftowerebovenoise_forwardplus"].Fill(HFplus_Numberoftowerebovenoise)
        self.hist["Hist_eventXiID_numberoftowerebovenoise_forwardplus"].Fill(HFplus_Numberoftowerebovenoise)
        self.hist["Hist_numberoftowerebovenoise_forwardplus"+ Pythia_Process_ID].Fill(HFplus_Numberoftowerebovenoise)
        
        self.hist["Hist_numberoftowerebovenoise_forwardminus"].Fill(HFminus_Numberoftowerebovenoise)
        self.hist["Hist_eventXiID_numberoftowerebovenoise_forwardminus"].Fill(HFminus_Numberoftowerebovenoise)
        self.hist["Hist_numberoftowerebovenoise_forwardminus"+ Pythia_Process_ID].Fill(HFminus_Numberoftowerebovenoise)
        self.hist["Hist_numberoftowerebovenoise_barrel"].Fill(Barrel_Numberoftowerebovenoise)
        self.hist["Hist_numberoftowerebovenoise_barrel"+Pythia_Process_ID].Fill(Barrel_Numberoftowerebovenoise)
        self.hist["Hist_numberoftowerebovenoise_endcap"].Fill(Endcap_Numberoftowerebovenoise)
        self.hist["Hist_numberoftowerebovenoise_endcap"+Pythia_Process_ID].Fill(Endcap_Numberoftowerebovenoise)
        self.hist["Hist_numberoftowerebovenoise_endcapforwardtransition"].Fill(endcapforwardtransition_Numberoftowerebovenoise)
        self.hist["Hist_numberoftowerebovenoise_endcapforwardtransition"+Pythia_Process_ID].Fill(endcapforwardtransition_Numberoftowerebovenoise)
        self.hist ["Hist_CaloReducedenergyClass"] .Fill(len(CaloReducedenergyClass))
        self.hist ["Hist_CaloReducedenergyClass_nocastor"].Fill(len(CaloReducedenergyClass)-CASTOR_Numberoftowerebovenoise)
        self.hist ["Hist_CaloReducedenergyClass_nocastor"+ Pythia_Process_ID].Fill(len(CaloReducedenergyClass)-CASTOR_Numberoftowerebovenoise)
       
        self.hist ["Hist_CaloReducedenergyClass_noHFm"].Fill(len(CaloReducedenergyClass)-HFplus_Numberoftowerebovenoise ) 
        self.hist ["Hist_CaloReducedenergyClass_noHFm"+Pythia_Process_ID].Fill(len(CaloReducedenergyClass)-HFplus_Numberoftowerebovenoise) 
        self.hist ["Hist_CaloReducedenergyClass_noHFp"].Fill(len(CaloReducedenergyClass)-HFminus_Numberoftowerebovenoise)  
        self.hist ["Hist_CaloReducedenergyClass_noHFp"+ Pythia_Process_ID].Fill(len(CaloReducedenergyClass)-HFminus_Numberoftowerebovenoise)  
        self.hist ["Hist_CaloReducedenergyClass"+ Pythia_Process_ID].Fill(len(CaloReducedenergyClass))
        self.hist ["Hist_eventXiID_CaloReducedenergyClass"].Fill(len(CaloReducedenergyClass))
      
        


        self.hist["hParticleCounts"].Fill("all",1)

        
        if not self.isData:
            self.hist["Hist_GP_Aftercutlog10XiX"].Fill(log10(GenXiX))
            self.hist["Hist_GP_Aftercutlog10XiY"].Fill(log10(GenXiY))
            self.hist["Hist_GP_Aftercutlog10XiX"+Pythia_Process_ID].Fill(log10(GenXiX))
            self.hist["Hist_GP_Aftercutlog10XiY"+Pythia_Process_ID].Fill(log10(GenXiY))
            self.hist["Hist_Aftercut2DLogGenXiX_LogGenXiY"].Fill(log10(GenXiX),log10(GenXiY))
            self.hist["Hist_Aftercut2DLogGenXiX_LogGenXiY"+Pythia_Process_ID].Fill(log10(GenXiX),log10(GenXiY))
            self.hist["Hist_GP_Aftercutlog10MaxXi"].Fill(log10(GenMaxXi))  
            self.hist["Hist_GP_Aftercutlog10MaxXi"+Pythia_Process_ID].Fill(log10(GenMaxXi))  
            self.hist["Hist_GP_Aftercutlog10XiDD"].Fill(log10(GenXi_DD))  
            
            self.hist["Hist_GP_Aftercutlog10XiDD"+Pythia_Process_ID].Fill(log10(GenXi_DD))

        mineta = 5.2
        maxeta = -6.2
        Etarange = -1
        MaxCastorEnergy = 0
        MaxHFPlusEnergy = 0
        HFPlusSumEnergy = 0
        CastorSumEnergy = 0
        forwarddelta = -1
        MaxHFMinusEnergy = 0
        HFMinusSumEnergy = 0
        for icalo in xrange(0,len(CaloReducedenergyClass)):
            calop4  = CaloReducedenergyClass[icalo][0]
            caloem  = CaloReducedenergyClass[icalo][1]
            calohad  = CaloReducedenergyClass[icalo][2]

            # self.hist["ReducedCalo_P4_EtaValueLog"].Fill( np.log10(abs(calop4.eta())))

            if (calop4.eta()) < mineta:
                mineta = calop4.eta()
            if (calop4.eta()) > maxeta:   
                maxeta = calop4.eta()
           
            

            for EventSelectionXiProcess_ID in  EventSelectionXiProcess_IDs:
                self.hist["Hist_reducedEnergy" + EventSelectionXiProcess_ID].Fill(calop4.e())     
               
                if  (calop4.eta()) > -6.6 and (calop4.eta()) < -5.2 :
                    self.hist["Hist_eventXiID_reducedEnergy_Castor"].Fill(calop4.e())
                    self.hist["Hist_eventXiID_reducedEnergy_Castor" + EventSelectionXiProcess_ID].Fill(calop4.e()) 
                    
                
                elif abs( calop4.eta() ) < 1.4:
                    self.hist["Hist_eventXiID_reducedEnergy_barrel"].Fill(calop4.e())
                    self.hist["Hist_eventXiID_reducedEnergy_barrel" + EventSelectionXiProcess_ID].Fill(calop4.e()) 
               
                elif abs(calop4.eta()) < 2.8:
                    self.hist["Hist_eventXiID_reducedEnergy_endcap"].Fill(calop4.e()) 
                    self.hist["Hist_eventXiID_reducedEnergy_endcap" + EventSelectionXiProcess_ID].Fill(calop4.e()) 
                
                
                elif abs(calop4.eta()) < 3.152:
                    self.hist["Hist_eventXiID_reducedEnergy_endcap_forwardtransition"].Fill(calop4.e()) 
                    self.hist["Hist_eventXiID_reducedEnergy_endcap_forwardtransition" + EventSelectionXiProcess_ID].Fill(calop4.e()) 
               
                
                else: # only HF left here 
                
                # if calop4.e() <5: continue
                    if (calop4.eta()>0):
                        self.hist["Hist_eventXiID_reducedEnergy_forwardplus"].Fill(calop4.e())
                        self.hist["Hist_eventXiID_reducedEnergy_forwardplus" + EventSelectionXiProcess_ID].Fill(calop4.e()) 
                        
                    else:
                        self.hist["Hist_eventXiID_reducedEnergy_forwardminus"].Fill(calop4.e())
                        self.hist["Hist_eventXiID_reducedEnergy_forwardminus" + EventSelectionXiProcess_ID].Fill(calop4.e())
                    
     
                   
            self.hist["Hist_reducedEta"].Fill(calop4.eta()) 
            self.hist["Hist_reducedEnergy"].Fill(calop4.e())
            self.hist["Hist_eventXiID_reducedEnergy"].Fill(calop4.e())         

            self.hist["Hist_reducedEta" + Pythia_Process_ID].Fill(calop4.eta())     
            self.hist["Hist_reducedEnergy" + Pythia_Process_ID].Fill(calop4.e()) 
               
            if  (calop4.eta()) > -6.6 and (calop4.eta()) < -5.2 :
                self.hist["Hist_reducedEnergy_Castor"].Fill(calop4.e())
                # CASTOR_Numberoftowerebovenoise += calop4.e()
                self.hist["Hist_reducedEnergy_Castor" + Pythia_Process_ID].Fill(calop4.e()) 
                
                CastorSumEnergy += calop4.e()
                if calop4.e()> MaxCastorEnergy:
                    MaxCastorEnergy = calop4.e()  
           
            elif abs( calop4.eta() ) < 1.4:
                self.hist["Hist_reducedEnergy_barrel"].Fill(calop4.e()) 
                self.hist["Hist_reducedEnergy_barrel" + Pythia_Process_ID].Fill(calop4.e()) 
                 
            elif abs( calop4.eta() ) < 2.8:
                self.hist["Hist_reducedEnergy_endcap"].Fill(calop4.e()) 
                self.hist["Hist_reducedEnergy_endcap" + Pythia_Process_ID].Fill(calop4.e()) 
               
            elif abs( calop4.eta() ) < 3.152:
                self.hist["Hist_reducedEnergy_endcap_forwardtransition"].Fill(calop4.e()) 
                self.hist["Hist_reducedEnergy_endcap_forwardtransition" + Pythia_Process_ID].Fill(calop4.e()) 

               
            else:
                if (calop4.eta()>0): 
                    # self.hist["Hist_reducedEnergy_forward"].Fill(calop4.e())
                    # self.hist["Hist_reducedEnergy_forward" + Pythia_Process_ID].Fill(calop4.e()) 
                    self.hist["Hist_reducedEnergy_forwardplus"].Fill(calop4.e())
                    self.hist["Hist_reducedEnergy_forwardplus" + Pythia_Process_ID].Fill(calop4.e()) 
                    HFPlusSumEnergy += calop4.e() 
                    if calop4.e() > MaxHFPlusEnergy :
                        MaxHFPlusEnergy = calop4.e()
                                        

                else:
                    self.hist["Hist_reducedEnergy_forwardminus"].Fill(calop4.e())
                    self.hist["Hist_reducedEnergy_forwardminus" + Pythia_Process_ID].Fill(calop4.e()) 
                    HFMinusSumEnergy += calop4.e() 
                    if calop4.e() > MaxHFMinusEnergy :
                        MaxHFMinusEnergy = calop4.e()   
                                


                

               
           
                
        
        for EventSelectionXiProcess_ID in  EventSelectionXiProcess_IDs:     
            self.hist["Hist_eventXiID_CastorSumEnergy" + EventSelectionXiProcess_ID].Fill(CastorSumEnergy) 
            self.hist["Hist_eventXiID_MaxCastorEnergy" + EventSelectionXiProcess_ID].Fill(MaxCastorEnergy) 
            self.hist["Hist_eventXiID_HFPlusSumEnergy" + EventSelectionXiProcess_ID].Fill(HFPlusSumEnergy) 
            self.hist["Hist_eventXiID_MaxHFPlusEnergy" + EventSelectionXiProcess_ID].Fill(MaxHFPlusEnergy) 
            self.hist["Hist_eventXiID_HFMinusSumEnergy" + EventSelectionXiProcess_ID].Fill(HFMinusSumEnergy) 
            self.hist["Hist_eventXiID_MaxHFMinusEnergy" + EventSelectionXiProcess_ID].Fill(MaxHFMinusEnergy)  
            
        


        self.hist["Hist_CastorSumEnergy"].Fill(CastorSumEnergy)       
        self.hist["Hist_CastorSumEnergy" + Pythia_Process_ID].Fill(CastorSumEnergy) 
        self.hist["Hist_eventXiID_CastorSumEnergy"].Fill(CastorSumEnergy)
        self.hist["Hist_MaxCastorEnergy"].Fill(MaxCastorEnergy) 
        self.hist["Hist_MaxCastorEnergy" + Pythia_Process_ID].Fill(MaxCastorEnergy) 
        self.hist["Hist_eventXiID_MaxCastorEnergy"].Fill(MaxCastorEnergy)
       
        self.hist["Hist_HFPlusSumEnergy"].Fill(HFPlusSumEnergy) 
        self.hist["Hist_HFPlusSumEnergy" + Pythia_Process_ID].Fill(HFPlusSumEnergy) 
        self.hist["Hist_eventXiID_HFPlusSumEnergy"].Fill(HFPlusSumEnergy)
        self.hist["Hist_MaxHFPlusEnergy"].Fill(MaxHFPlusEnergy) 
        self.hist["Hist_MaxHFPlusEnergy" + Pythia_Process_ID].Fill(MaxHFPlusEnergy) 
        self.hist["Hist_eventXiID_MaxHFPlusEnergy"].Fill(MaxHFPlusEnergy)
                    
        self.hist["Hist_HFMinusSumEnergy"].Fill(HFMinusSumEnergy) 
        self.hist["Hist_HFMinusSumEnergy" + Pythia_Process_ID].Fill(HFMinusSumEnergy) 
        self.hist["Hist_eventXiID_HFMinusSumEnergy"].Fill(HFMinusSumEnergy)
        self.hist["Hist_MaxHFMinusEnergy"].Fill(MaxHFMinusEnergy) 
        self.hist["Hist_MaxHFMinusEnergy" + Pythia_Process_ID].Fill(MaxHFMinusEnergy) 
        self.hist["Hist_eventXiID_MaxHFMinusEnergy"].Fill(MaxHFMinusEnergy)           
                       
      


        if len(TrackCandClass) > 0:
            if len(TrackCandClass) > 1:    
                TrackCandClass.sort(cmp=compareTracketa)
            mineta =  min(mineta, TrackCandClass[0][0])
            maxeta =  max(maxeta, TrackCandClass[len(TrackCandClass)-1][0])#eta()pseduorapidty
        
            

        #self.hist["Hist_TrackCandClass"].Fill(len(TrackCandClass))  
        self.hist["Hist_NbrTracks"].Fill(len(TrackCandClass))  
        self.hist["Hist_NbrTracks"+ Pythia_Process_ID].Fill(len(TrackCandClass))
        self.hist["Hist_eventXiID_NbrTracks"].Fill(len(TrackCandClass))  
        
        for EventSelectionXiProcess_ID in EventSelectionXiProcess_IDs:
            self.hist["Hist_eventXiID_NbrTracks"+EventSelectionXiProcess_ID].Fill(len(TrackCandClass))

        
    

        deltaeta = -1
        delta_zero = -1
        deltaetamax = -1
       
     
        rapidityGapeta= 0
        ListofEta = []
       
        for jtrk in xrange(0,len(TrackCandClass)): 
            ListofEta.append(TrackCandClass[jtrk][0])
            
        for icalo in xrange(0,len(CaloReducedenergyClass)):
            calop4  = CaloReducedenergyClass[icalo][0]
            ListofEta.append(calop4.eta())
        
        ListofEta.append(-6.6)
        ListofEta.append(5.2)
        ListofEta.sort()
       

        if len(ListofEta) > 1:
            for jtrk in xrange(0,len(ListofEta)-1): 
                
                self.hist["Hist_trkEta"].Fill(ListofEta[jtrk])
                deltaeta = ListofEta[jtrk+1] - ListofEta[jtrk] #eta()pseduorapidty
                self.hist["Hist_trkplusEta"].Fill(ListofEta[jtrk+1])
                
                if  (deltaeta > deltaetamax):
                    deltaetamax = deltaeta
                    rapidityGapeta = (ListofEta[jtrk+1] + ListofEta[jtrk])/2 
                    
                if  ListofEta[jtrk+1] > 0 and ListofEta[jtrk]< 0:
                    delta_zero = deltaeta
                    
        forwarddelta = max(abs(mineta -(-6.6)), abs(maxeta-5.2))
        Etarange = maxeta - mineta
        self.hist["Hist_forwarddelta"].Fill(forwarddelta)
        self.hist["Hist_Eta_DeltaZero"].Fill(delta_zero)
        self.hist["Hist_Eta_DeltaMax"].Fill(deltaetamax)
        self.hist["Hist_Eta_DeltaMax"+ Pythia_Process_ID].Fill(deltaetamax)
        self.hist["Hist_Eta_DeltaZero"+ Pythia_Process_ID].Fill(delta_zero)
        self.hist["Hist_forwarddelta"+ Pythia_Process_ID].Fill(forwarddelta)
        self.hist["Hist_eventXiID_DeltaMax"].Fill(deltaetamax)
        self.hist["Hist_eventXiID_DeltaZero"].Fill(delta_zero)
        self.hist["Hist_eventXiID_forwarddelta"].Fill(forwarddelta)

        deltagenreco = (delta_zero_gen - delta_zero)
        self.hist["Hist_GP_deltagenreco"].Fill(deltagenreco)
        self.hist["Hist_2D_recogen_DeltaZero"].Fill(delta_zero, delta_zero_gen)
        self.hist["Hist_2D_recogen_DeltaMax"].Fill(deltaetamax, delta_max_gen) 
        self.hist["Hist_2D_recogen_EtaMiniumum"].Fill(mineta,mingeneta)
        self.hist["Hist_2D_recogen_forwarddelta"].Fill(forwarddelta,forwardgendelta)
        self.hist["Hist_2D_recogen_forwarddelta"+ Pythia_Process_ID].Fill(forwarddelta,forwardgendelta)
        self.hist["Hist_2D_recogen_EtaMax"].Fill(maxeta,maxgeneta)
            # self.hist["hNentries"].Fill("CaloReducedenergyClass2",1) 

        self.hist["Hist_Eta_Min"].Fill(mineta)
        self.hist["Hist_Eta_Max"].Fill(maxeta)
        self.hist["Hist_Eta_Delta"].Fill(Etarange)  
        self.hist["Hist_Eta_Min" + Pythia_Process_ID].Fill(mineta)
        self.hist["Hist_Eta_Max" + Pythia_Process_ID].Fill(maxeta)  
        self.hist["Hist_Eta_Delta" + Pythia_Process_ID].Fill(Etarange) 
        self.hist["Hist_eventXiID_Min"].Fill(mineta)
        self.hist["Hist_eventXiID_Max"].Fill(maxeta) 

        for EventSelectionXiProcess_ID in  EventSelectionXiProcess_IDs:
            self.hist["Hist_eventXiID_DeltaMax"+ EventSelectionXiProcess_ID].Fill(deltaetamax)
            self.hist["Hist_eventXiID_DeltaZero"+ EventSelectionXiProcess_ID].Fill(delta_zero)
            self.hist["Hist_eventXiID_Min"+ EventSelectionXiProcess_ID].Fill(mineta)
            self.hist["Hist_eventXiID_Max"+ EventSelectionXiProcess_ID].Fill(maxeta)  
            self.hist["Hist_eventXiID_forwarddelta"+ EventSelectionXiProcess_ID].Fill(forwarddelta)
            self.hist["Hist_2D_recogen_forwarddelta_eventselection"+ EventSelectionXiProcess_ID].Fill(forwarddelta,forwardgendelta)          
        XEtot =  0
        XPxtot = 0
        XPytot = 0
        XPztot = 0
        YEtot =  0
        YPxtot = 0
        YPytot = 0
        YPztot = 0
        xix = 0 
        xiy = 0 
        Mx2 = 0
        My2 = 0
        Xi_DD = 0
        Px= 0
        Py= 0

          

        # if len(CaloCandClass) == 0:
        #     return 0
        
       
        self.hist["hNentries"].Fill("CaloCandClass1",1)    
        for icalo in xrange(0,len(CaloCandClass)):
            calop4  = CaloCandClass[icalo][0]
            caloem  = CaloCandClass[icalo][1]
            calohad  = CaloCandClass[icalo][2]
           
            
            for EventSelectionXiProcess_ID in  EventSelectionXiProcess_IDs:
                self.hist["Hist_eventXiID_Energy" + EventSelectionXiProcess_ID].Fill(calop4.e())  
                if abs(calop4.eta()) < 1.4:
                    self.hist["Hist_eventXiID_Energy_barrel"].Fill(calop4.e()) 
                    self.hist["Hist_eventXiID_Energy_barrel" + EventSelectionXiProcess_ID].Fill(calop4.e()) 
               
                if  abs( calop4.eta()) > 1.4 and abs(calop4.eta()) <2.8:
                    self.hist["Hist_eventXiID_Energy_endcap" + EventSelectionXiProcess_ID].Fill(calop4.e()) 
                    self.hist["Hist_eventXiID_Energy_endcap"].Fill(calop4.e()) 
               
                if  abs(calop4.eta()) > 2.8 and abs(calop4.eta()) < 3.2:
                    self.hist["Hist_eventXiID_Energy_endcap_forwardtransition"].Fill(calop4.e()) 
                    self.hist["Hist_eventXiID_Energy_endcap_forwardtransition" + EventSelectionXiProcess_ID].Fill(calop4.e()) 
               
                if  abs (calop4.eta()) > 3.2 and abs(calop4.eta()) < 5.2:
                    self.hist["Hist_eventXiID_Energy_forward"].Fill(calop4.e())
                    self.hist["Hist_eventXiID_Energy_forward" + EventSelectionXiProcess_ID].Fill(calop4.e()) 
                
                if (calop4.eta()) > 3.2 and (calop4.eta()) < 5.2:
                    self.hist["Hist_eventXiID_Energy_forwardplus"].Fill(calop4.e())
                    self.hist["Hist_eventXiID_Energy_forwardplus" + EventSelectionXiProcess_ID].Fill(calop4.e())
                
                if (calop4.eta()) > -5.2 and (calop4.eta()) < -3.2:
                    self.hist["Hist_eventXiID_Energy_forwardminus" + EventSelectionXiProcess_ID].Fill(calop4.e()) 
                    self.hist["Hist_eventXiID_Energy_forwardminus"].Fill(calop4.e())
               
                if  (calop4.eta()) > -6.6 and (calop4.eta()) < -5.2 :
                    self.hist["Hist_eventXiID_Energy_Castor" + EventSelectionXiProcess_ID].Fill(calop4.e()) 
                    self.hist["Hist_eventXiID_Energy_Castor"].Fill(calop4.e())
       

           
            self.hist["Hist_Eta"].Fill(calop4.eta()) 
            self.hist["Hist_Energy"].Fill(calop4.e())     
            self.hist["Hist_Eta" + Pythia_Process_ID].Fill(calop4.eta())     
            self.hist["Hist_Energy" + Pythia_Process_ID].Fill(calop4.e())     
            self.hist["hParticleCounts"].Fill("all",1)
            if abs(calop4.eta()) < 1.4:
                self.hist["Hist_Energy_barrel"].Fill(calop4.e()) 
                self.hist["Hist_Energy_barrel" + Pythia_Process_ID].Fill(calop4.e()) 
                
            if  abs( calop4.eta()) > 1.4 and abs(calop4.eta()) <2.8:
                self.hist["Hist_Energy_endcap"].Fill(calop4.e()) 
                self.hist["Hist_Energy_endcap" + Pythia_Process_ID].Fill(calop4.e()) 
               
            if  abs(calop4.eta()) > 2.8 and abs(calop4.eta()) < 3.2:
                self.hist["Hist_Energy_endcap_forwardtransition"].Fill(calop4.e()) 
                self.hist["Hist_Energy_endcap_forwardtransition" + Pythia_Process_ID].Fill(calop4.e()) 

            if  abs (calop4.eta()) > 3.2 and abs(calop4.eta()) < 5.2:
                self.hist["Hist_Energy_forward"].Fill(calop4.e())
                self.hist["Hist_Energy_forward" + Pythia_Process_ID].Fill(calop4.e()) 
                
                    
            if (calop4.eta()) > 3.2 and (calop4.eta()) < 5.2:
                self.hist["Hist_Energy_forwardplus"].Fill(calop4.e())
                self.hist["Hist_Energy_forwardplus" + Pythia_Process_ID].Fill(calop4.e()) 
               
         
            if (calop4.eta()) > -5.2 and (calop4.eta()) < -3.2:
                self.hist["Hist_Energy_forwardminus"].Fill(calop4.e())
                self.hist["Hist_Energy_forwardminus" + Pythia_Process_ID].Fill(calop4.e()) 
               
            if  (calop4.eta()) > -6.6 and (calop4.eta()) < -5.2 :
                self.hist["Hist_Energy_Castor"].Fill(calop4.e())
                self.hist["Hist_Energy_Castor" + Pythia_Process_ID].Fill(calop4.e()) 
     

      
            # self.hist["hNentries"].Fill("CaloCandClass2",1)    
            
            if calop4.eta() <= rapidityGapeta:
               
                XEtot += calop4.E()
                XPxtot += calop4.Px()
                XPytot += calop4.Py()
                XPztot += calop4.Pz()
                

       
            if calop4.eta() >= rapidityGapeta:
                  
                YEtot += calop4.E()
                YPxtot += calop4.Px()
                YPytot += calop4.Py()
                YPztot += calop4.Pz()

            
        
        Px2 =  XPxtot**2 + XPytot**2 +XPztot**2
        Py2 =  YPxtot**2 + YPytot**2 +YPztot**2
        Px = sqrt(Px2)
        Py = sqrt(Py2)
        # Mx2 = XEtot*XEtot - XPxtot*XPxtot - XPytot*XPytot - XPztot*XPztot
        # My2 = YEtot*YEtot - YPxtot*YPxtot - YPytot*YPytot - YPztot*YPztot
        Mx2 = (XEtot+Px)*(XEtot-Px)
        My2 = (YEtot+Py)*(YEtot-Py) 

         # set Mx2, My2 to zero if negative (i.e. the X,Y system is a photon)            
        if Mx2<=0: Mx2=1.01e-6
        if My2<=0: My2=1.01e-6

        Mx = sqrt(Mx2)
        My = sqrt(My2)
            
                

        
        if self.CMenergy > 0 :    
        # // calculate xix and xiy
            # xix = Mx*Mx/self.CMenergy/self.CMenergy
            # xiy = My*My/self.CMenergy/self.CMenergy
           
            xix = Mx2/self.CMenergy/self.CMenergy
            xiy = My2/self.CMenergy/self.CMenergy
            Xi_DD = Mx2*My2/(self.protonmass**2 * self.CMenergy**2)
            
            self.hist["Hist_XiX"].Fill(xix)
            self.hist["Hist_XiY"].Fill(xiy)
            self.hist["Hist_log10XiX"].Fill(log10(xix))
            self.hist["Hist_log10XiY"].Fill(log10(xiy))
            self.hist["Hist_2Dlog10XiXXiY"].Fill(log10(xix),log10(xiy))
            self.hist["Hist_XiX"+Pythia_Process_ID].Fill(xix)
            self.hist["Hist_XiY"+Pythia_Process_ID].Fill(xiy)
            self.hist["Hist_log10XiX"+Pythia_Process_ID].Fill(log10(xix))
            self.hist["Hist_log10XiY"+Pythia_Process_ID].Fill(log10(xiy))
            self.hist["Hist_2Dlog10XiXXiY"+Pythia_Process_ID].Fill(log10(xix),log10(xiy))
            self.hist["Hist_log10Mx"].Fill(log10(Mx))
            self.hist["Hist_log10My"].Fill(log10(My))
            self.hist["Hist_2D_log10MxMy"].Fill(log10(Mx),log10(My))
            self.hist["Hist_log10Mx"+ Pythia_Process_ID].Fill(log10(Mx))
            self.hist["Hist_log10My"+ Pythia_Process_ID].Fill(log10(My))
            self.hist["Hist_2D_log10MxMy"+ Pythia_Process_ID].Fill(log10(Mx),log10(My))
            self.hist["Hist_2D_eventXiID_log10MxMy"].Fill(log10(Mx),log10(My))
            self.hist["Hist_eventXiID_log10XiY"].Fill(log10(xiy))
            self.hist["Hist_eventXiID_log10XiX"].Fill(log10(xix))
            
            for EventSelectionXiProcess_ID in  EventSelectionXiProcess_IDs:
                self.hist["Hist_2D_eventXiID_log10MxMy"+ EventSelectionXiProcess_ID].Fill(log10(Mx),log10(My))
                self.hist["Hist_eventXiID_log10XiX"+EventSelectionXiProcess_ID].Fill(log10(xix))
                self.hist["Hist_eventXiID_log10XiY"+EventSelectionXiProcess_ID].Fill(log10(xiy))
            # self.hist["hNentries"].Fill("Xi",1)
               
            
            
            self.OUTlog10XiDD[0] =log10(Xi_DD) 
            
            self.OUTlog10XixReco[0] = log10(xix)
            self.OUTlog10XiyReco[0] = log10(xiy)
            self.OUTforwardgendelta = forwardgendelta
            self.OUTdeltazero[0] = delta_zero
            self.OUTdeltaeta[0] = deltaetamax
            self.OUTetamin[0] = mineta
            self.OUTetamax[0] = maxeta
            self.OUTrapditygapmean [0] = rapidityGapeta
            self.OUTCastorNtowers[0] = CASTOR_Numberoftowerebovenoise
            self.OUTHFminusNtowers[0] = HFminus_Numberoftowerebovenoise
            self.OUTHFplusNtowers[0] = HFplus_Numberoftowerebovenoise
            self.OUTNtracks[0] = Nbrtracks
            self.OUTEtarange[0]= Etarange
            self.OUTPythia8processid[0] = int_Pythia_Process_ID
            self.OUTEventselectionXiprocessDD[0] = "_DD" in EventSelectionXiProcess_IDs
            self.OUTEventselectionXiprocessSD1[0] = "_SD1" in EventSelectionXiProcess_IDs
            self.OUTEventselectionXiprocessSD2[0] = "_SD2" in EventSelectionXiProcess_IDs
            self.OUTEventselectionXiprocessRest[0] = "_Rest" in EventSelectionXiProcess_IDs
            self.OUTCaloReducedenergyClass[0] = (len(CaloReducedenergyClass))
            self.OUTCastorSumEnergy[0]= CastorSumEnergy
            self.OUTHFPlusSumEnergy[0]= HFPlusSumEnergy
            self.OUTMaxHFPlusEnergy[0] = MaxHFPlusEnergy
            self.OUTHFMinusSumEnergy[0]= HFMinusSumEnergy
            self.OUTMaxHFMinusEnergy[0] = MaxHFMinusEnergy
            self.OUTMaxCastorEnergy[0] = MaxCastorEnergy

            if not self.isData: 
                self.OUTlog10XiyGen[0] = log10(GenXiY)
                self.OUTlog10XiyGen[0] = log10(GenXiY)
          
            self.AllTree.Fill()
            
            
                


            if not self.isData:
                self.hist["Hist_2DLogRecoXiX_logGenXiX"].Fill(log10(xix),log10(GenXiX))
                self.hist["Hist_2DLogRecoXiY_logGenLogXiY"].Fill(log10(xiy),log10(GenXiY))
                # self.hist["Hist_2D_genreco_Deltapos"].Fill(ChargedGenParticleClass[deltaetamax_Gen_pos][0].eta(),TrackCandClass[deltaetamax_Reco_pos][0]) 
                self.hist["Hist_2DLogRecoXiX_logGenXiX"+Pythia_Process_ID].Fill(log10(xix),log10(GenXiX))
                self.hist["Hist_2DLogRecoXiY_logGenLogXiY"+Pythia_Process_ID].Fill(log10(xiy),log10(GenXiY))
                self.hist["Hist_2Drecogen_EnergyX"].Fill(XEtot,XGenEtot)
                self.hist["Hist_2Drecogen_EnergyY"].Fill(YEtot,YGenEtot)
                self.hist["Hist_2D_recogen_Mean"].Fill(rapidityGapeta, MeanGen)
                self.hist["Hist_Reco_log10XiDD"].Fill(log10(Xi_DD)) 
                self.hist["Hist_eventXiID_Reco_log10XiDD"].Fill(log10(Xi_DD)) 
                self.hist["Hist_Reco_log10XiDD"+ Pythia_Process_ID].Fill(log10(Xi_DD)) 
                self.hist["Hist_eventXiID_Reco_log10XiDD"+ EventSelectionXiProcess_ID].Fill(log10(Xi_DD)) 
                self.hist["Hist_2D_RecoLogXi_DDGenLogXi_DD"].Fill(log10(Xi_DD),log10(GenXi_DD))
                

                   
              
               
    

        return 1

    def finalize(self):
        print "Finalize:"
        if hasattr(self, 'AllTree'):
            self.AllTree.AutoSave()    
        # if hasattr(self, 'sigTreeDD'):
        #     self.sigTreeDD.AutoSave()
        # if hasattr(self, 'sigTreeSD1'):
        #     self.sigTreeSD1.AutoSave()   
        # if hasattr(self, 'sigTreeSD2'):
        #     self.sigTreeSD2.AutoSave()    
        
        # if hasattr(self, 'bkgTreeDD'):
        #     self.bkgTreeDD.AutoSave()       
        # if hasattr(self, 'bkgTreeSD1'):
        #     self.bkgTreeSD1.AutoSave() 
        # if hasattr(self, 'bkgTreeSD2'):
        #     self.bkgTreeSD2.AutoSave() 


        

    
if __name__ == "__main__":
       
    if len(sys.argv) != 3 :
        print ("please use: ./DiffractiveAndTrack_cuetseb ParameterSet Std|HFsysPlus|HFsysMinus|CastorsysPlus|CastorsysMinus|TrackPlus|TrackMinus.... ")
        print ("Try again!!")
        sys.exit(1)

    Castor_energy_scale = 1.0
    HF_energy_scale = 1.0
    Track_efficiency_scale = 1.0
    HF_bad_channel_list = 'Run_247934'
    # HF_bad_channel_list = 'Run_dNdEta'
    
    ParameterSet=str(sys.argv[1]).lower()
    Outputname="trackanddiffractive"

    sampleList = []

    if ParameterSet == 'Seb_LHCf_Run247934'.lower():
        sampleList.append("Run2015A_ReReco_ZeroBias") #247934 #sebastians tree for HF towers            
        Outputname =  Outputname + "_data_Seb_LHCf_Run247934" 
    
    elif ParameterSet == 'Seb_LHCf_Run247920'.lower():
        sampleList.append("Run2015A_ReReco_ZeroBias") #247934 #sebastians tree for HF towers 
        Outputname =  Outputname + "_data_Seb_LHCf_Run247920"  
   
    elif ParameterSet == 'Melike_dNdEta'.lower():
        sampleList.append("data_ZeroBias1_CASTOR")  
        Outputname = Outputname + "_data"
   
    elif ParameterSet== 'data_247934'.lower():
        sampleList.append("data_ZeroBias1_CASTOR247934")
        Outputname =  Outputname + "_data_247934"  
    elif ParameterSet == 'epos'.lower():
        sampleList.append("MinBias_EPOS_13TeV_MagnetOff_CASTORmeasured_newNoise_newVertexReconstruction")    
        Outputname = Outputname + "_epos"

    # elif ParameterSet == 'Sebastian_epos'.lower():
    #     sampleList.append("MinBias_EPOS_13TeV_MagnetOff_CASTORmeasured_newCASTORnoise_ZeroTeslaTracking_sBaur_Okt2016")    
    #     Outputname = Outputname + "_sebastian_epos"

    elif ParameterSet == 'mbr'.lower():
        sampleList.append("MinBias_TuneMBR_13TeV-pythia8_MagnetOff_CASTORmeasured_newNoise_newVertexReconstruction")
        Outputname = Outputname + "_mbr"
        

    elif ParameterSet == 'cuet'.lower():
        sampleList.append("MinBias_CUETP8M1_13TeV-pythia8_MagnetOff_CASTORmeasured_newNoise_newVertexReconstruction")
        Outputname = Outputname + "_cuet"

    elif ParameterSet == 'cuetplus'.lower():
        sampleList.append("MinBias_CUETP8M1_13TeV-pythia8_MagnetOff_CASTORmeasured")
        Outputname = Outputname + "_cuetplus" 

    else:
        print("ParameterSet unkonwn")
        sys.exit(1)

    # Outputname += '_TEST'


    mode=str(sys.argv[2]).lower()
    if (mode=="hfsysplus"):
        HF_energy_scale = 1.2
        Outputname = Outputname + "_sysHFPlus"
    
    elif (mode=="hfsysminus"):
        HF_energy_scale = 0.8
        Outputname = Outputname + "_sysHFMinus"

    elif(mode=="castorsysplus"):
        Castor_energy_scale = 1.15
        Outputname = Outputname + "_sysCastorPlus"    
    
    elif(mode=="castorsysminus"):
        Castor_energy_scale = 0.85
        Outputname = Outputname + "_sysCastorMinus"   

    elif (mode=="trackplus"):
        Track_efficiency_scale = 1.05
        Outputname = Outputname + "_sysTrackPlus"
    elif (mode=="trackminus"):
        Track_efficiency_scale = 0.95
        Outputname = Outputname + "_sysTrackMinus"
    elif (mode=="std"):
        pass
    else:
        print ("Please specify parameters!")
        sys.exit(1)

    Outputname = Outputname + ".root"

    sys.stdout = os.fdopen(sys.stdout.fileno(), 'w', 0)
    # ROOT.gSystem.Load("libFWCoreFWLite.so")
    # ROOT.AutoLibraryLoader.enable()


    # sampleList.append("MinBias_TuneMBR_13TeV-pythia8_MagnetOff_CASTORmeasured_newNoise")
    # sampleList.append("MinBias_EPOS_13TeV_MagnetOff_CASTORmeasured_newNoise")
    # sampleList.append("data_ZeroBias_27Jan2016_LHCf") #247934 #sebastians tree for HF towers
    # sampleList.append("data_ZeroBias1_CASTOR247934")
    # sampleList.append("data_ZeroBias1_CASTOR")
    maxFilesMC = None# run through all ffiles found
    maxFilesData =None # same
    nWorkers = 15# Use all cpu cores
   
   
    slaveParams = {}
    # slaveParams["maxEta"] = 2.
    slaveParams['ParameterSet'] = ParameterSet
    slaveParams['HF_energy_scale'] = HF_energy_scale
    slaveParams['Castor_energy_scale'] = Castor_energy_scale
    slaveParams['Track_efficiency_scale'] = Track_efficiency_scale
    slaveParams['HF_bad_channel_list'] = HF_bad_channel_list

    print ("Outputname=" + Outputname)

    # use printTTree.py <sampleName> to see what trees are avaliable inside the skim file
    #cff = DiffractiveAndTrack(  ParameterSet=ParameterSet,
    #                            HF_energy_scale=HF_energy_scale,
    #                            Track_efficiency_scale=Track_efficiency_scale )
 
    if ParameterSet=='mbr'.lower() or ParameterSet=='epos'.lower() or ParameterSet=='cuet'.lower()or ParameterSet == 'Seb_LHCf_Run247934'.lower() or ParameterSet == 'Seb_LHCf_Run247920'.lower():
        DiffractiveAndTrack.runAll(treeName="CFFTree", # if you use the sebastians tree    
           slaveParameters=slaveParams,
           sampleList=sampleList,
           maxFilesMC = maxFilesMC,
           maxFilesData = maxFilesData,
           nWorkers= nWorkers,
           # maxNevents = 10000,
           verbosity = 2,
           outFile = Outputname) 


    else:
        DiffractiveAndTrack.runAll(treeName="EflowTree",
    
    # DiffractiveAndTrack.runAll(treeName="CFFTree", # if you use the sebastians tree    
           slaveParameters=slaveParams,
           sampleList=sampleList,
           maxFilesMC = maxFilesMC,
           maxFilesData = maxFilesData,
           nWorkers= nWorkers,
           # maxNevents =2000,
           verbosity = 2,
           outFile = Outputname) 
