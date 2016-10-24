# DiffractiveANAwithTMVA

## Analysis code for selecting diffractive events in [CMS](http://cms.web.cern.ch/)

## Install

* Checkout code into the [TMVA](tmva.sourceforge.net/) working directory 
  of your [ROOT](https://root.cern.ch/) installation.

  ```shell
  # for compilation just type
  $ make
  ```

## Data

* Input data taken from [CFF Analyzer](https://github.com/makbiyik/CommonFSQFramework/tree/CMSSW_76X_master/Core/test/DiffractiveCrossSection)

* Information about [CFF](https://github.com/hvanhaev/CommonFSQFramework) can be found: <br>
  https://twiki.cern.ch/twiki/bin/viewauth/CMS/FSQCommonFW

## Workflow

* Diffractive_TMVAClassification: <br>
  For training with MC data and
  create weight files for application

* Diffractive_TMVApplication: <br>
  To apply classifier cut on data or MC
  and create histograms

* CreateFigures: <br>
  Draw figures from previous created hists
