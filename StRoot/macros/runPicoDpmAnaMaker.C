
/* **************************************************
 *   Run StPicoHFMyAnaMaker in different modes
 * --------------------------------------------------
 * run as :
 *  root -l -b -q StRoot/macros/loadSharedHFLibraries.C StRoot/macros/runPicoHFMyAnaMaker.C++
 *   or
 *  root -l -b -q StRoot/macros/runPicoHFMyAnaMaker.C
 *
 * -------------------------------------------------- 
 *  - Different modes to use the  class
 *    - StPicoHFMaker::kAnalyze - don't write candidate trees, just fill histograms
 *        inputFile : fileList of PicoDst files or single picoDst file
 *        outputFile: baseName for outfile 
 *    - StPicoHFMaker::kWrite   - write candidate trees
 *        inputFile : path to single picoDist file
 *        outputFile: baseName for outfile 
 *    - StPicoHFMaker::kRead    - read candidate trees and fill histograms
 *        inputFile : fileList of PicoDst files
 *        outputFile: baseName for outfile 
 *
 * --------------------------------------------------
 *  Authors:  Xin Dong        (xdong@lbl.gov)
 *            Michael Lomnitz (mrlomnitz@lbl.gov)
 *            Mustafa Mustafa (mmustafa@lbl.gov)
 *            Jochen Thaeder  (jmthader@lbl.gov)
 *
 * **************************************************
 */

#ifndef __CINT__
#include "TROOT.h"
#include "TSystem.h"
#include "TChain.h"

#include "StMaker.h"
#include "StChain.h"

#include "StPicoDstMaker/StPicoDstMaker.h"
#include "StPicoHFMaker/StPicoHFEvent.h"
#include "StPicoHFMaker/StHFCuts.h"

#include "StPicoHFMyAnaMaker/StPicoHFMyAnaMaker.h"

#include "macros/loadSharedHFLibraries.C"

#include <iostream>
#include <ctime>
#include <cstdio>

#include "StPicoDpmAnaMaker/StPicoDpmAnaMaker.h" //kvapil

#include "StRefMultCorr/StRefMultCorr.h"
#include "StRefMultCorr/CentralityMaker.h"

using namespace std;

#else
class StChain;
#endif

StChain *chain;

void runPicoDpmAnaMaker(const Char_t *inputFile="test.list", const Char_t *outputFile="outputBaseName",  
    const unsigned int makerMode = 0 /*kAnalyze*/,
    const Char_t *badRunListFileName = "picoList_bad_MB.list", const Char_t *treeName = "picoHFtree",
    const Char_t *productionBasePath = "/star/data100/reco/AuAu_200_production_2016/ReversedFullField/P16ij/2016",
    const unsigned int decayChannel = 0 /* kChannel0 */) { 
  // -- Check STAR Library. Please set SL_version to the original star library used in the production 
  //    from http://www.star.bnl.gov/devcgi/dbProdOptionRetrv.pl
  string SL_version = "SL17d"; //originally SL16j, not available at RCF any more
  string env_SL = getenv ("STAR");
  if (env_SL.find(SL_version)==string::npos) {
    cout<<"Environment Star Library does not match the requested library in runPicoHFMyAnaMaker.C. Exiting..."<<endl;
    exit(1);
  }

#ifdef __CINT__
  gROOT->LoadMacro("loadSharedHFLibraries.C");
  loadSharedHFLibraries();
#endif


  chain = new StChain();

  // ========================================================================================
  //makerMode    = StPicoHFMaker::kAnalyze;
  // ========================================================================================

  cout << "Maker Mode    " << makerMode << endl;
  cout << "TreeName      " << treeName << endl; 
  cout << "Decay Channel " << decayChannel << endl; 

  TString sInputFile(inputFile);
  TString sInputListHF("");  
  TString sProductionBasePath(productionBasePath);
  TString sTreeName(treeName);

  if (makerMode == StPicoHFMaker::kAnalyze) {
    if (!sInputFile.Contains(".list") && !sInputFile.Contains("picoDst.root")) {
      cout << "No input list or picoDst root file provided! Exiting..." << endl;
      exit(1);
    }
  }
  else if (makerMode == StPicoHFMaker::kWrite) {
    if (!sInputFile.Contains("picoDst.root")) {
      cout << "No input picoDst root file provided! Exiting..." << endl;
      exit(1);
    }
  }
  else if (makerMode == StPicoHFMaker::kRead) {
    if (!sInputFile.Contains(".list")) {
      cout << "No input list provided! Exiting..." << endl;
      exit(1);
    }

    // -- prepare filelist for picoDst from trees
    sInputListHF = sInputFile;
    sInputFile = "tmpPico.list";

    TString command = "sed 's|" + sTreeName + ".root|picoDst.root|g' " + sInputListHF + " > " + sInputFile;
    cout << "COMMAND : " << command << endl; 
    gSystem->Exec(command.Data());

    command = "sed -i 's|^.*" + sTreeName + "|" + sProductionBasePath + "|g' " + sInputFile; // + " > " + sInputFile;
    cout << "COMMAND : " << command << endl; 
    gSystem->Exec(command.Data());
  }
  else {
    cout << "Unknown makerMode! Exiting..." << endl;
    exit(1);
  }
  StPicoDstMaker* picoDstMaker = new StPicoDstMaker(StPicoDstMaker::IoRead, sInputFile, "picoDstMaker"); //for local testing only
  StPicoDpmAnaMaker* picoDpmAnaMaker = new StPicoDpmAnaMaker("picoDpmAnaMaker", picoDstMaker, outputFile, sInputListHF);
  picoDpmAnaMaker->setMakerMode(makerMode);
  picoDpmAnaMaker->setDecayChannel(StPicoDpmAnaMaker::kChannel1);//kvapil
  picoDpmAnaMaker->setTreeName(treeName);


  StHFCuts* hfCuts = new StHFCuts("hfBaseCuts");
  picoDpmAnaMaker->setHFBaseCuts(hfCuts);

  // ---------------------------------------------------
  // -- Set Base cuts for HF analysis

  // -- File name of bad run list
  hfCuts->setBadRunListFileName(badRunListFileName);

  // -- ADD USER CUTS HERE ----------------------------
  hfCuts->setCutVzMax(6.);
  hfCuts->setCutVzVpdVzMax(3.);

  //Run16 SL16j triggers
  hfCuts->addTriggerId(520001);    // VPDMB-5-p-sst (production 1, physics stream)
  hfCuts->addTriggerId(520011);    // VPDMB-5-p-sst 
  hfCuts->addTriggerId(520021);    // VPDMB-5-p-sst
  hfCuts->addTriggerId(520031);    // VPDMB-5-p-sst
  hfCuts->addTriggerId(520041);    // VPDMB-5-p-sst
  hfCuts->addTriggerId(520051);    // VPDMB-5-p-sst

  hfCuts->addTriggerId(570002);    // VPDMB-5-nosst (production 2, nosst stream)
  hfCuts->addTriggerId(570001);    // VPDMB-5-sst (production 2, sst stream )

  hfCuts->setStream(0); //0 - physics stream, 1 - sst+nosst streams (for proper setup of StRefMultCorr)

  hfCuts->setCutNHitsFitMin(20); //for analysis (TTree)
  hfCuts->setCutNHitsFitMinHist(20); //for histograms
  hfCuts->setCutRequireHFT(true);

  hfCuts->setCutNHitsFitnHitsMax(0.52); 

  // ---------------------------------------------------

  picoDpmAnaMaker->setDecayMode(StPicoHFEvent::kThreeParticleDecay); //kvapil


  hfCuts->setCutEta(1.);
  /*
  //---------------------------------FOR FIXED RECTANGULAR CUTS------------------------------------------------ 
  //used for texting and before TMVA cuts optimization
  hfCuts->setCutTripletdV0Max(0.022);

  hfCuts->setCutDcaMin(0.009,StHFCuts::kPion); //federic 1aug2016
  //hfCuts->setCutDcaMin(0.01,StHFCuts::kKaon); //federic 1aug2016
  hfCuts->setCutDcaMin(0.007,StHFCuts::kKaon); //federic 3aug2016

  //-----------SECONDARY TRIPLET CUTS----------------------------
  float dcaDaughters12Max, dcaDaughters23Max, dcaDaughters31Max;
  float decayLengthMin, decayLengthMax;
  float cosThetaMin, massMin, massMax;

  dcaDaughters12Max = 0.009;
  dcaDaughters23Max = 0.009;
  dcaDaughters31Max = 0.009;

  decayLengthMin = 0.003;
  decayLengthMax = 0.2;

  cosThetaMin = 0.997;
  massMin = 1.7;
  massMax = 2.1;

  hfCuts->setCutSecondaryTriplet(dcaDaughters12Max, dcaDaughters23Max, dcaDaughters31Max,
  decayLengthMin, decayLengthMax, 				 
  cosThetaMin, massMin, massMax);
  // --- Lomnitz cuts to remove noise from ghosting
  //------------------------------------------------------------

  //Single track pt
  hfCuts->setCutPtRange(0.5,50.0,StHFCuts::kPion);
  hfCuts->setCutPtRange(0.5,50.0,StHFCuts::kKaon);
  //___________________________________________________________________________________________________________
  */
  //---------------------------------FOR TMVA RECTANGULAR CUTS------------------------------------------------
  //pre-cuts fror TMVA training and for D+- candidates TTree

  //daugter DCA to PV (default value used for creating arrays of good identified particle candidates)
  hfCuts->setCutDcaMin(0.006,StHFCuts::kPion);
  hfCuts->setCutDcaMin(0.006,StHFCuts::kKaon);

  //-----------SECONDARY TRIPLET CUTS - LOW pT----------------------------
  hfCuts->setCutSecondaryDaughtersDCAtoPVmin(0.006); //more tight cut for low-pT to reduce output file size

  hfCuts->setCutTripletdV0Max(100);

  float dcaDaughters12Max, dcaDaughters23Max, dcaDaughters31Max;
  float decayLengthMin, decayLengthMax;
  float cosThetaMin, massMin, massMax;

  dcaDaughters12Max = 0.011;
  dcaDaughters23Max = 0.011;
  dcaDaughters31Max = 0.011;

  decayLengthMin = 0.011;
  decayLengthMax = 100;   

  cosThetaMin = 0.995;
  massMin = 1.7;
  massMax = 2.1;

  hfCuts->setCutSecondaryTriplet(dcaDaughters12Max, dcaDaughters23Max, dcaDaughters31Max,
      decayLengthMin, decayLengthMax, 				 
      cosThetaMin, massMin, massMax);

  //-----------SECONDARY TRIPLET CUTS - HIGH pT----------------------------
  hfCuts->setCutTripletdV0MaxHighPt(100); 

  float dcaDaughters12Max_02, dcaDaughters23Max_02, dcaDaughters31Max_02;
  float decayLengthMin_02, decayLengthMax_02;
  float cosThetaMin_02, massMin_02, massMax_02;
  float ptThreshold;

  dcaDaughters12Max_02 = 0.011; 
  dcaDaughters23Max_02 = 0.011;
  dcaDaughters31Max_02 = 0.011;

  decayLengthMin_02 = 0.011; 
  decayLengthMax_02 = 100;

  cosThetaMin_02 = 0.995;
  massMin_02 = 1.7;
  massMax_02 = 2.1;

  ptThreshold = 4.;

  hfCuts->setCutSecondaryTripletHighPt(dcaDaughters12Max_02, dcaDaughters23Max_02, dcaDaughters31Max_02,
      decayLengthMin_02, decayLengthMax_02, 				 
      cosThetaMin_02, massMin_02, massMax_02,
      ptThreshold);


  //Single track pt
  hfCuts->setCutPtRange(0.3,50.0,StHFCuts::kPion);
  hfCuts->setCutPtRange(0.3,50.0,StHFCuts::kKaon);
  //___________________________________________________________________________________________________________


  //TPC setters
  hfCuts->setCutTPCNSigmaPion(3.0);
  hfCuts->setCutTPCNSigmaKaon(2.0);

  //for histograms
  hfCuts->setCutTPCNSigmaHadronHist(1.0, 1); //1 = pion
  hfCuts->setCutTPCNSigmaHadronHist(1.0, 2); //2 = kaon

  //TOF setters, need to set pt range as well
  hfCuts->setCutTOFDeltaOneOverBeta(0.03, StHFCuts::kKaon);
  //hfCuts->setCutPtotRangeHybridTOF(0.5,50.0,StHFCuts::kKaon); //for standard rectangular cuts
  hfCuts->setCutPtotRangeHybridTOF(0.3,50.0,StHFCuts::kKaon); //for TMVA

  hfCuts->setCutTOFDeltaOneOverBeta(0.03, StHFCuts::kPion);
  //hfCuts->setCutPtotRangeHybridTOF(0.5,50.0,StHFCuts::kPion); //for standard rectangular cuts
  hfCuts->setCutPtotRangeHybridTOF(0.3,50.0,StHFCuts::kPion); //for TMVA

  // set refmultCorr
  StRefMultCorr* grefmultCorrUtil = CentralityMaker::instance()->getgRefMultCorr_P16id(); //new StRefMultCorr, info about Run16, SL16d in the same file as for Run14, SL16d
  picoDpmAnaMaker->setRefMutCorr(grefmultCorrUtil);
  // ========================================================================================

  // ========================================================================================

  chain->Init();
  cout << "chain->Init();" << endl;
  int nEvents = picoDstMaker->chain()->GetEntries();
  cout << " Total entries = " << nEvents << endl;

  for (Int_t i=0; i<nEvents; i++) {
    if(i%10000==0)
      cout << "Working on eventNumber " << i << endl;

    chain->Clear();
    int iret = chain->Make(i);

    if (iret) { cout << "Bad return code!" << iret << endl; break;}

    //total++;
  }

  cout << "****************************************** " << endl;
  cout << "Work done... now its time to close up shop!"<< endl;
  cout << "****************************************** " << endl;
  chain->Finish();

  cout << "****************************************** " << endl;
  cout << "total number of events  " << nEvents << endl;
  cout << "****************************************** " << endl;

  cout << "****************************************** " << endl;

  delete chain;

  // -- clean up if in read mode
  if (makerMode == StPicoHFMaker::kRead)
    gSystem->Exec(Form("rm -f %s", sInputFile.Data()));
}

