#ifndef StPicoDpmAnaMaker_h
#define StPicoDpmAnaMaker_h

#include "StPicoHFMaker/StPicoHFMaker.h"
#include "TNtuple.h"
#include "StRefMultCorr/StRefMultCorr.h"
#include "TH2F.h"
//#include "StPicoDpmAnaHists.h"
#include <vector>

#include "TClonesArray.h"

#include "StThreeVectorF.hh"
#include "StLorentzVectorF.hh"

#include "StPicoDstMaker/StPicoDst.h"
#include "StPicoDstMaker/StPicoDstMaker.h"
#include "StPicoEvent/StPicoEvent.h"
#include "StPicoEvent/StPicoTrack.h"
#include "StPicoEvent/StPicoBTofPidTraits.h"

#include "StPicoHFMaker/StPicoHFEvent.h"
#include "StPicoHFMaker/StHFCuts.h"
#include "StPicoHFMaker/StHFPair.h"
#include "StPicoHFMaker/StHFTriplet.h"
#include "StBTofUtil/tofPathLength.hh"

#include "phys_constants.h"

#include "TH1F.h"
#include "TH3F.h"


#include <ctime>

/* **************************************************
 *  Sample class fo HF picoDST analysis
 * --------------------------------------------------
 * 
 *  For more info look also in the .h files in StPicoHFMaker/
 *     StPicoHFMaker/StPicoHFMaker.h      <-- Base Class for analysis
 *     StPicoHFMaker/StPicoHFEvent.h      <-- Holds candidates for one event (written to Tree)
 *     StPicoHFMaker/StHFCuts.h           <-- Cuts, can be set in run macro
 *     StPicoHFMaker/StHFPair.h           <-- Holds a pair candidate of a two body decay
 *     StPicoHFMaker/StHFTriplet.h        <-- Holds a triplet of a three body decay
 *
 *  Usage:
 *   - Implement
 *        InitHF()
 *        MakeHF()
 *        ClearHF()
 *        FinishHF()
 *
 *  - Do not ovewrite Init, Make, Clear, Finish which are inhertited from StPicoHFMaker via StMaker 

 *  - Set StHFCuts class via setHFBaseCuts(...) in run macro
 *
 *  - Set use mode of StPicoHFMaker class  via setMakerMode(...)
 *     use enum of StPicoHFMaker::eMakerMode
 *      StPicoHFMaker::kAnalyze - don't write candidate trees, just fill histograms
 *      StPicoHFMaker::kWrite   - write candidate trees
 *      StPicoHFMaker::kRead    - read candidate trees and fill histograms
 *
 *  - Set decay mode of analysis via setDecayMode(...)
 *     use enum of StPicoHFEvent::eHFEventMode (see there for more info)
 *      StPicoHFEvent::kTwoParticleDecay,
 *      StPicoHFEvent::kThreeParticleDecay
 *      StPicoHFEvent::kTwoAndTwoParticleDecay
 *
 *  - Implement these track selection methods used to fill vectors for 'good' identified particles
 *      (methods from StHFCuts utility class can/should be used)
 *       isPion
 *       isKaon
 *       isProton
 *
 *  --------------------------------------------------
 *  
 *  Initial Authors:  
 *            Xin Dong        (xdong@lbl.gov)
 *            Mustafa Mustafa (mmustafa@lbl.gov)
 *          **Jochen Thaeder  (jmthader@lbl.gov) 
 * 
 *  ** Code Maintainer
 *
 * **************************************************
 */

class StPicoDst;
class StPicoDstMaker;
class StPicoEvent;
class StPicoTrack;
class StPicoHFEvent;

class StHFPair;
class StHFTriplet;
class StHFCuts;

class StRefMultCorr;

class StPicoDpmAnaMaker : public StPicoHFMaker 
{
 public:
  StPicoDpmAnaMaker(char const* name, StPicoDstMaker* picoMaker, char const* outputBaseFileName,  
		       char const* inputHFListHFtree);
  virtual ~StPicoDpmAnaMaker();
  
  virtual Int_t InitHF();
  virtual Int_t MakeHF();
  virtual void  ClearHF(Option_t *opt);
  virtual Int_t FinishHF();
  // -- Lomnitz: Added this cut funtions to to filter iwthout having to make pairs
  virtual bool isCloseTracks(StPicoTrack const*, StPicoTrack const*,StThreeVectorF const & , float) const;
  virtual double DCA(StPicoTrack const*, StThreeVectorF const &) const;
  int createQA();
  
  // -- ADOPT DECAY CHANNELS, if wished ------------------- 
  void setDecayChannel(unsigned int u) { mDecayChannel = u; }

  enum eDecayChannel {kChannel1, kChannel2, kChannel3};

  void setRefMutCorr(StRefMultCorr* gRefMultCorr) { mRefmultCorrUtil = gRefMultCorr; }
  StRefMultCorr* getRefMultCorr() { return mRefmultCorrUtil; }


 protected:
  virtual bool isHadron(StPicoTrack const*, int pidFlag) const;
  virtual bool isPion(StPicoTrack const*) const;
  virtual bool isKaon(StPicoTrack const*) const;
  virtual bool isProton(StPicoTrack const*) const;

private:
  int createCandidates();
  int analyzeCandidates();



  // -- private members --------------------------

  unsigned int mDecayChannel;


  // -- ADD USER MEMBERS HERE ------------------- 
   //TNtuple *ntp_DMeson; //orig. Kvapil
   TTree *ntp_Dmeson; //Vanek

   StRefMultCorr* mRefmultCorrUtil;
   int mRunNumber;
       
  TString mOutFileBaseName;


   //---Variables for TTree---------------------------
	//Pion1
	Int_t pi1_runId, pi1_eventId;
	Float_t pi1_phi, pi1_eta, pi1_pt, pi1_dca, pi1_dedx, pi1_nSigma;
	Int_t pi1_nHitFit, pi1_nHitsMax, pi1_nHitdedx;
	Float_t pi1_TOFinvbeta, pi1_betaBase;

	//Pion2
	Int_t pi2_runId, pi2_eventId;
	Float_t pi2_phi, pi2_eta, pi2_pt, pi2_dca, pi2_dedx, pi2_nSigma;
	Int_t pi2_nHitFit, pi2_nHitsMax, pi2_nHitdedx;
	Float_t pi2_TOFinvbeta, pi2_betaBase;

	//Kaon
	Int_t k_runId, k_eventId;
	Float_t k_phi, k_eta, k_pt, k_dca, k_dedx, k_nSigma;
	Int_t k_nHitFit, k_nHitsMax, k_nHitdedx;
	Float_t k_TOFinvbeta, k_betaBase;

	//dca, flag, prim. vertex
	Float_t mdcaMax;
	Int_t flag;
  Float_t primVx;
  Float_t primVy;
	Float_t primVz;

  Float_t primVx_err;
  Float_t primVy_err;
	Float_t primVz_err;

	//D meson
	Float_t D_theta, D_decayL, D_phi, D_eta, D_y, D_pt, D_mass, D_dV0Max;

	//centrality, refMult
	Float_t mcentrality, refmult, mrefmultcorr, mreweight;

	//ZDCx, BBCx
//	Float_t mBBCx, mZDCx;

	

//-------------------------------------------------
  // -- ADD USER MEMBERS HERE -------------------

  ClassDef(StPicoDpmAnaMaker, 1) //set to 1
};

#endif
