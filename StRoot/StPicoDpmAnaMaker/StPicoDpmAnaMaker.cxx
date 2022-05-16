#include "StPicoDpmAnaMaker.h"
//#include "StPicoHFMaker/StHFCuts.h"
#include <iostream>
#include <cmath>

ClassImp(StPicoDpmAnaMaker)

using namespace std;

// _________________________________________________________
StPicoDpmAnaMaker::StPicoDpmAnaMaker(char const* name, StPicoDstMaker* picoMaker, char const* outputBaseFileName,
               char const* inputHFListHFtree = "") :
  StPicoHFMaker(name, picoMaker, outputBaseFileName, inputHFListHFtree),
  mDecayChannel(kChannel1), mRefmultCorrUtil(NULL),mOutFileBaseName(outputBaseFileName){


  // constructor
}

// _________________________________________________________
StPicoDpmAnaMaker::~StPicoDpmAnaMaker() {
  // destructor
}

// _________________________________________________________
int StPicoDpmAnaMaker::InitHF() {
  // -- INITIALIZE USER HISTOGRAMS ETC HERE -------------------
  //    add them to the output list mOutList which is automatically written

  // EXAMPLE //  mOutList->Add(new TH1F(...));
  // EXAMPLE //  TH1F* hist = static_cast<TH1F*>(mOutList->Last());

  //QA histograms and TOF matching histograms
  mOutList->Add(new TH1F("h_time_per_event","h_time_per_event", 2000., 0., 20.));
  mOutList->Add(new TH2F("h_piTOF","h_piTOF",100,0,10, 2, 0, 2));
  mOutList->Add(new TH2F("h_kTOF","h_kTOF",100,0,10, 2, 0, 2));
  mOutList->Add(new TH2F("h_pTOF","h_pTOF",100,0,10, 2, 0, 2));

  mOutList->Add(new TH2F("h_piTOF_20","h_piTOF_20",100,0,10, 2, 0, 2));
  mOutList->Add(new TH2F("h_kTOF_20","h_kTOF_20",100,0,10, 2, 0, 2));
  mOutList->Add(new TH2F("h_pTOF_20","h_pTOF_20",100,0,10, 2, 0, 2));

  mOutList->Add(new TH2F("h_piTOF_1sig","h_piTOF_1sig",100,0,10, 2, 0, 2));
  mOutList->Add(new TH2F("h_kTOF_1sig","h_kTOF_1sig",100,0,10, 2, 0, 2));
  mOutList->Add(new TH2F("h_pTOF_1sig","h_pTOF_1sig",100,0,10, 2, 0, 2));

  mOutList->Add(new TH2F("h_piTOF_HFT","h_piTOF_HFT",100,0,10, 2, 0, 2));
  mOutList->Add(new TH2F("h_kTOF_HFT","h_kTOF_HFT",100,0,10, 2, 0, 2));
  mOutList->Add(new TH2F("h_pTOF_HFT","h_pTOF_HFT",100,0,10, 2, 0, 2));

  mOutList->Add(new TH2F("h_piTOF_HFT_1sig","h_piTOF_HFT_1sig",100,0,10, 2, 0, 2));
  mOutList->Add(new TH2F("h_kTOF_HFT_1sig","h_kTOF_HFT_1sig",100,0,10, 2, 0, 2));
  mOutList->Add(new TH2F("h_pTOF_HFT_1sig","h_pTOF_HFT_1sig",100,0,10, 2, 0, 2));

  mOutList->Add(new TH2F("h_piTOF_HFT_20","h_piTOF_HFT_20",100,0,10, 2, 0, 2));
  mOutList->Add(new TH2F("h_kTOF_HFT_20","h_kTOF_HFT_20",100,0,10, 2, 0, 2));
  mOutList->Add(new TH2F("h_pTOF_HFT_20","h_pTOF_HFT_20",100,0,10, 2, 0, 2));

  mOutList->Add(new TH2F("h_piTOF_HFT_1sig_20","h_piTOF_HFT_1sig_20",100,0,10, 2, 0, 2));
  mOutList->Add(new TH2F("h_kTOF_HFT_1sig_20","h_kTOF_HFT_1sig_20",100,0,10, 2, 0, 2));
  mOutList->Add(new TH2F("h_pTOF_HFT_1sig_20","h_pTOF_HFT_1sig_20",100,0,10, 2, 0, 2));

  mOutList->Add(new TH2F("h_piTOFbeta","h_piTOFbeta",500,0,10, 500, 0, 5));
  mOutList->Add(new TH2F("h_kTOFbeta","h_kTOFbeta",500,0,10, 500, 0, 5));
  mOutList->Add(new TH2F("h_pTOFbeta","h_pTOFbeta",500,0,10, 500, 0, 5));

  mOutList->Add(new TH2F("h_pinsigma","h_pinsigma",1000,0,10, 99, -5, 5));
  mOutList->Add(new TH2F("h_knsigma","h_knsigma",1000,0,10, 99, -5, 5));
  mOutList->Add(new TH2F("h_pnsigma","h_pnsigma",1000,0,10, 99, -5, 5));

  mOutList->Add(new TH2F("h_dedx","h_dedx", 1000, 0, 10, 1000, 0, 10));
  mOutList->Add(new TH2F("h_OneOverBeta", "h_OneOverBeta", 1000, 0, 10, 1000, 0, 4));

  mOutFileBaseName = mOutFileBaseName.ReplaceAll(".root", "");

   mOutList->Add(new TH1F("h_mh1Cent", "EventsVsCentrality;cent;Counts", 10, -1.5, 8.5));
   mOutList->Add(new TH1F("h_mh1CentWg", "EventsVsCentrality;cent;Counts", 10, -1.5, 8.5));
   mOutList->Add(new TH1F("h_mh1gRefmultCor", "gRefmultCor;gRefmult;Counts", 700, 0, 700));
   mOutList->Add(new TH1F("h_mh1gRefmultCorWg", "gRefmultCorWg;gRefmultCorWg;Counts", 700, 0, 700));
   mOutList->Add(new TH2F("h_mh2CentVz", "CentralityVsVz;cent;Vz", 10, -1.5, 8.5, 200, -10, 10));
   mOutList->Add(new TH2F("h_mh2CentVzWg", "CentralityVsVzWg;cent;Vz", 10, -1.5, 8.5, 200, -10, 10));

   mOutList->Add(new TH3F("h_mh3CentVxVyWg", "CentralityVsVxVyWg", 10, -1.5, 8.5, 200, -10, 10, 200, -10, 10));


  //load refmult table
  mRefmultCorrUtil->setVzForWeight(6, -6.0, 6.0);
  //comment what is not needed
  if(mHFCuts->getStream() == 0 )//physics stream
  {
    mRefmultCorrUtil->readScaleForWeight("StRoot/StRefMultCorr/macros/weight_grefmult_VpdnoVtx_Vpd5_Run16.txt"); //for new StRefMultCorr, Run16, SL16j, production 1, physics stream
  }

  if(mHFCuts->getStream() == 1 )//sst+nosst streams
  {
    mRefmultCorrUtil->readScaleForWeight("StRoot/StRefMultCorr/macros/weight_grefmult_VpdnoVtx_Vpd5_Run16_prod2.txt"); //for new StRefMultCorr, Run16, SL16j, production 2, sst stream
  }

  //create TTree to store D+- candidates
  ntp_Dmeson = new TTree("ntp", "DMeson TTree"); //create TTree


//---Set branches------------------------------------------------------------------------------
  //commented branches were used for QA and in older versions of the analysis
  //commented in order to reduce output file size as much as possible

  //Pion1
  //ntp_Dmeson->Branch("pi1_runId", &pi1_runId, "pi1_runId/I");            //Int_t pi1_runId
  //ntp_Dmeson->Branch("pi1_eventId", &pi1_eventId, "pi1_eventId/I");       //Int_t pi1_eventId
  //ntp_Dmeson->Branch("pi1_phi", &pi1_phi, "pi1_phi/F");             //Float_t pi1_phi
  //ntp_Dmeson->Branch("pi1_eta", &pi1_eta, "pi1_eta/F");             //Float_t pi1_eta
  ntp_Dmeson->Branch("pi1_pt", &pi1_pt, "pi1_pt/F");               //Float_t pi1_pt
  ntp_Dmeson->Branch("pi1_dca", &pi1_dca, "pi1_dca/F");             //Float_t pi1_dca
  //ntp_Dmeson->Branch("pi1_dedx", &pi1_dedx, "pi1_dedx/F");             //Float_t pi1_dedx
  //ntp_Dmeson->Branch("pi1_nSigma", &pi1_nSigma, "pi1_nSigma/F");         //Float_t pi1_nSigma
  //ntp_Dmeson->Branch("pi1_nHitFit", &pi1_nHitFit, "pi1_nHitFit/I");       //Int_t pi1_nHitFit
  //ntp_Dmeson->Branch("pi1_nHitsMax", &pi1_nHitsMax, "pi1_nHitsMax/I");      //Int_t pi1_nHitsMax
  //ntp_Dmeson->Branch("pi1_nHitdedx", &pi1_nHitdedx, "pi1_nHitdedx/I");      //Int_t pi1_nHitdedx
  //ntp_Dmeson->Branch("pi1_TOFinvbeta", &pi1_TOFinvbeta, "pi1_TOFinvbeta/F");  //Float_t pi1_TOFinvbeta
  //ntp_Dmeson->Branch("pi1_betaBase", &pi1_betaBase, "pi1_betaBase/F");      //Float_t pi1_betaBase

  //Pion2
  //ntp_Dmeson->Branch("pi2_runId", &pi2_runId, "pi2_runId/I");            //Int_t pi2_runId
  //ntp_Dmeson->Branch("pi2_eventId", &pi2_eventId, "pi2_eventId/I");       //Int_t pi2_eventId
  //ntp_Dmeson->Branch("pi2_phi", &pi2_phi, "pi2_phi/F");             //Float_t pi2_phi
  //ntp_Dmeson->Branch("pi2_eta", &pi2_eta, "pi2_eta/F");             //Float_t pi2_eta
  ntp_Dmeson->Branch("pi2_pt", &pi2_pt, "pi2_pt/F");               //Float_t pi2_pt
  ntp_Dmeson->Branch("pi2_dca", &pi2_dca, "pi2_dca/F");             //Float_t pi2_dca
  //ntp_Dmeson->Branch("pi2_dedx", &pi2_dedx, "pi2_dedx/F");             //Float_t pi2_dedx
  //ntp_Dmeson->Branch("pi2_nSigma", &pi2_nSigma, "pi2_nSigma/F");         //Float_t pi2_nSigma
  //ntp_Dmeson->Branch("pi2_nHitFit", &pi2_nHitFit, "pi2_nHitFit/I");       //Int_t pi2_nHitFit
  //ntp_Dmeson->Branch("pi2_nHitsMax", &pi2_nHitsMax, "pi2_nHitsMax/I");       //Int_t pi2_nHitsMax
  //ntp_Dmeson->Branch("pi2_nHitdedx", &pi2_nHitdedx, "pi2_nHitdedx/I");      //Int_t pi2_nHitdedx
  //ntp_Dmeson->Branch("pi2_TOFinvbeta", &pi2_TOFinvbeta, "pi2_TOFinvbeta/F");  //Float_t pi2_TOFinvbeta
  //ntp_Dmeson->Branch("pi2_betaBase", &pi2_betaBase, "pi2_betaBase/F");      //Float_t pi2_betaBase

  //Kaon
  //ntp_Dmeson->Branch("k_runId", &k_runId, "k_runId/I");          //Int_t k_runId
  //ntp_Dmeson->Branch("k_eventId", &k_eventId, "k_eventId/I");       //Int_t k_eventId
  //ntp_Dmeson->Branch("k_phi", &k_phi, "k_phi/F");             //Float_t k_phi
  //ntp_Dmeson->Branch("k_eta", &k_eta, "k_eta/F");             //Float_t k_eta
  ntp_Dmeson->Branch("k_pt", &k_pt, "k_pt/F");               //Float_t k_pt
  ntp_Dmeson->Branch("k_dca", &k_dca, "k_dca/F");             //Float_t k_dca
  //ntp_Dmeson->Branch("k_dedx", &k_dedx, "k_dedx/F");           //Float_t k_dedx
  //ntp_Dmeson->Branch("k_nSigma", &k_nSigma, "k_nSigma/F");         //Float_t k_nSigma
  //ntp_Dmeson->Branch("k_nHitFit", &k_nHitFit, "k_nHitFit/I");       //Int_t k_nHitFit
  //ntp_Dmeson->Branch("k_nHitsMax", &k_nHitsMax, "k_nHitsMax/I");       //Int_t k_nHitsMax
  //ntp_Dmeson->Branch("k_nHitdedx", &k_nHitdedx, "k_nHitdedx/I");    //Int_t k_nHitdedx
  //ntp_Dmeson->Branch("k_TOFinvbeta", &k_TOFinvbeta, "k_TOFinvbeta/F");  //Float_t k_TOFinvbeta
  //ntp_Dmeson->Branch("k_betaBase", &k_betaBase, "k_betaBase/F");    //Float_t k_betaBase

  //dca, flag, prim. vertex
  ntp_Dmeson->Branch("mdcaMax", &mdcaMax, "mdcaMax/F"); //Float_t mdcaMax - max. pair DCA
  ntp_Dmeson->Branch("flag", &flag, "flag/I");      //Int_t flag
  ntp_Dmeson->Branch("primVx", &primVx, "primVx/F"); //Float_t primVx
  ntp_Dmeson->Branch("primVy", &primVy, "primVy/F"); //Float_t primVy
  ntp_Dmeson->Branch("primVz", &primVz, "primVz/F"); //Float_t primVz

  ntp_Dmeson->Branch("primVx_err", &primVx_err, "primVx_err/F"); //Float_t primVx
  ntp_Dmeson->Branch("primVy_err", &primVy_err, "primVy_err/F"); //Float_t primVy
  ntp_Dmeson->Branch("primVz_err", &primVz_err, "primVz_err/F"); //Float_t primVz

  //D meson
  ntp_Dmeson->Branch("D_theta", &D_theta, "D_theta/F");   //Float_t D_theta
  ntp_Dmeson->Branch("D_decayL", &D_decayL, "D_decayL/F");  //Float_t D_decayL
  ntp_Dmeson->Branch("D_phi", &D_phi, "D_phi/F");      //Float_t D_phi
  ntp_Dmeson->Branch("D_eta", &D_eta, "D_eta/F");      //Float_t D_eta
  //ntp_Dmeson->Branch("D_y", &D_y, "D_y/F");      //Float_t D_y
  ntp_Dmeson->Branch("D_pt", &D_pt, "D_pt/F");        //Float_t D_pt
  ntp_Dmeson->Branch("D_mass", &D_mass, "D_mass/F");    //Float_t D_mass
  ntp_Dmeson->Branch("D_dV0Max", &D_dV0Max, "D_dV0Max/F");  //Float_t D_dV0Max

  //centrality, refmult
  ntp_Dmeson->Branch("mcentrality", &mcentrality, "mcentrality/F");    //Float_t cmentrality
  //ntp_Dmeson->Branch("refmult", &refmult, "refmult/F");        //Float_t refmult
  //ntp_Dmeson->Branch("mrefmultcorr", &mrefmultcorr, "mrefmultcorr/F");  //Float_t mrefmultcorr
  ntp_Dmeson->Branch("mreweight", &mreweight, "mreweight/F");        //Float_t mreweight

  //BBCx, ZDCx
//  ntp_Dmeson->Branch("mZDCx", &mZDCx, "mZDCx/F"); //Float_t mZDCx
//  ntp_Dmeson->Branch("mBBCx", &mBBCx, "mBBCx/F"); //Float_t mBBCx
//-----------------------Set branches END------------------------------------------------------------------------------

  mRunNumber = 0;
  return kStOK;
}

// _________________________________________________________
void StPicoDpmAnaMaker::ClearHF(Option_t *opt="") {
  return;
}

// _________________________________________________________
int StPicoDpmAnaMaker::FinishHF() {
   if( isMakerMode() != StPicoHFMaker::kWrite )
    ntp_Dmeson->Write(); //for candidates

  return kStOK;
}

// _________________________________________________________
int StPicoDpmAnaMaker::MakeHF() {

  //create and analyze D+- candidates

  //createCandidates() makes triplets of K and pi and checks that they pass cuts
  //analyzeCandidates() saves relevant information about the triplets to TTree

  std::clock_t start1 = std::clock();//kvapil

  if (isMakerMode() == StPicoHFMaker::kWrite) {
    createCandidates();
  }
  else if (isMakerMode() == StPicoHFMaker::kRead) {
    // -- the reading back of the perviously written trees happens in the background
    analyzeCandidates();
  }
  else if (isMakerMode() == StPicoHFMaker::kAnalyze) {
    createCandidates();
    analyzeCandidates();
  }


  //get all QA and TOF matching histograms from mOutList and create them
  TH2F *h_piTOF = static_cast<TH2F*>(mOutList->FindObject("h_piTOF"));
  TH2F *h_kTOF = static_cast<TH2F*>(mOutList->FindObject("h_kTOF"));
  TH2F *h_pTOF = static_cast<TH2F*>(mOutList->FindObject("h_pTOF"));

  TH2F *h_piTOF_20 = static_cast<TH2F*>(mOutList->FindObject("h_piTOF_20"));
  TH2F *h_kTOF_20 = static_cast<TH2F*>(mOutList->FindObject("h_kTOF_20"));
  TH2F *h_pTOF_20 = static_cast<TH2F*>(mOutList->FindObject("h_pTOF_20"));

  TH2F *h_piTOF_1sig = static_cast<TH2F*>(mOutList->FindObject("h_piTOF_1sig"));
  TH2F *h_kTOF_1sig = static_cast<TH2F*>(mOutList->FindObject("h_kTOF_1sig"));
  TH2F *h_pTOF_1sig = static_cast<TH2F*>(mOutList->FindObject("h_pTOF_1sig"));

  TH2F *h_piTOF_HFT = static_cast<TH2F*>(mOutList->FindObject("h_piTOF_HFT"));
  TH2F *h_kTOF_HFT = static_cast<TH2F*>(mOutList->FindObject("h_kTOF_HFT"));
  TH2F *h_pTOF_HFT = static_cast<TH2F*>(mOutList->FindObject("h_pTOF_HFT"));

  TH2F *h_piTOF_HFT_1sig = static_cast<TH2F*>(mOutList->FindObject("h_piTOF_HFT_1sig"));
  TH2F *h_kTOF_HFT_1sig = static_cast<TH2F*>(mOutList->FindObject("h_kTOF_HFT_1sig"));
  TH2F *h_pTOF_HFT_1sig = static_cast<TH2F*>(mOutList->FindObject("h_pTOF_HFT_1sig"));

  TH2F *h_piTOF_HFT_20 = static_cast<TH2F*>(mOutList->FindObject("h_piTOF_HFT_20"));
  TH2F *h_kTOF_HFT_20 = static_cast<TH2F*>(mOutList->FindObject("h_kTOF_HFT_20"));
  TH2F *h_pTOF_HFT_20 = static_cast<TH2F*>(mOutList->FindObject("h_pTOF_HFT_20"));

  TH2F *h_piTOF_HFT_1sig_20 = static_cast<TH2F*>(mOutList->FindObject("h_piTOF_HFT_1sig_20"));
  TH2F *h_kTOF_HFT_1sig_20 = static_cast<TH2F*>(mOutList->FindObject("h_kTOF_HFT_1sig_20"));
  TH2F *h_pTOF_HFT_1sig_20 = static_cast<TH2F*>(mOutList->FindObject("h_pTOF_HFT_1sig_20"));

  TH2F *h_piTOFbeta = static_cast<TH2F*>(mOutList->FindObject("h_piTOFbeta"));
  TH2F *h_kTOFbeta = static_cast<TH2F*>(mOutList->FindObject("h_kTOFbeta"));
  TH2F *h_pTOFbeta = static_cast<TH2F*>(mOutList->FindObject("h_pTOFbeta"));

  TH2F *h_pinsigma = static_cast<TH2F*>(mOutList->FindObject("h_pinsigma"));
  TH2F *h_knsigma = static_cast<TH2F*>(mOutList->FindObject("h_knsigma"));
  TH2F *h_pnsigma = static_cast<TH2F*>(mOutList->FindObject("h_pnsigma"));

  TH2F *h_dedx = static_cast<TH2F*>(mOutList->FindObject("h_dedx"));
  TH2F *h_OneOverBeta = static_cast<TH2F*>(mOutList->FindObject("h_OneOverBeta"));

  TH1F *h_mh1Cent = static_cast<TH1F*>(mOutList->FindObject("h_mh1Cent"));
  TH1F *h_mh1CentWg = static_cast<TH1F*>(mOutList->FindObject("h_mh1CentWg"));
  TH1F *h_mh1gRefmultCor = static_cast<TH1F*>(mOutList->FindObject("h_mh1gRefmultCor"));
  TH1F *h_mh1gRefmultCorWg = static_cast<TH1F*>(mOutList->FindObject("h_mh1gRefmultCorWg"));
  TH2F *h_mh2CentVz = static_cast<TH2F*>(mOutList->FindObject("h_mh2CentVz"));
  TH2F *h_mh2CentVzWg = static_cast<TH2F*>(mOutList->FindObject("h_mh2CentVzWg"));

  TH3F *h_mh3CentVxVyWg = static_cast<TH3F*>(mOutList->FindObject("h_mh3CentVxVyWg"));

  //primary vertex
  StThreeVectorF pVtx = mPicoDst->event()->primaryVertex();

  //create refmultCor to get centrality
  mRefmultCorrUtil->init(mPicoDst->event()->runId());

  if (!mRefmultCorrUtil)
  {
    LOG_WARN << " No mGRefMultCorrUtil! Skip! " << endl;
    return kStWarn;
  }

  if (mRefmultCorrUtil->isBadRun(mPicoDst->event()->runId())) return kStOK;

  mRefmultCorrUtil->initEvent(mPicoDst->event()->grefMult(), mPrimVtx.z(), mPicoDst->event()->ZDCx()) ;
  int centrality = mRefmultCorrUtil->getCentralityBin9(); //deleted const
  const double reweight = mRefmultCorrUtil->getWeight();
  const double refmultCor = mRefmultCorrUtil->getRefMultCorr();

  h_mh1gRefmultCor->Fill(refmultCor);
  h_mh1gRefmultCorWg->Fill(refmultCor, reweight);
  h_mh1Cent->Fill(centrality);
  h_mh1CentWg->Fill(centrality, reweight);

  h_mh2CentVz->Fill(centrality, pVtx.z());
  h_mh2CentVzWg->Fill(centrality, pVtx.z(), reweight);

  h_mh3CentVxVyWg->Fill(centrality, pVtx.x(), pVtx.y(), reweight);


  //loop over PicoDst tracks
  UInt_t nTracks = mPicoDst->numberOfTracks();
  for (unsigned short iTrack = 0; iTrack < nTracks; ++iTrack)
  {
      StPicoTrack const* trk = mPicoDst->track(iTrack);
      if (!trk) continue;
      
      StPhysicalHelixD helix = trk->helix(mPicoDst->event()->bField()); //SL16j, Vanek
      StThreeVectorF momentum = trk->gMom(pVtx, mPicoDst->event()->bField()); //bFiled NOT in kilogauss - properly computed inside gMom(...) function in StPicoTrack

      //check nHits and eta of track
      if(!(mHFCuts->hasGoodNHitsFitMinHist(trk))) continue;
      if (!(mHFCuts->hasGoodEta(momentum))) continue;

      //PID of pions for QA histograms and 
      if (mHFCuts->hasGoodTPCnSigmaPion(trk))
      { 
          //calculate beta of track from its momentum
          float piBeta = mHFCuts->getTofBetaBase(trk, mPicoDst->event()->bField()); //SL16j, Vanek

          //get beta of track from TOF, if TOF information is available          
          Int_t piTofAvailable = 0;
          if(!isnan(piBeta) && piBeta > 0)
          {
             piTofAvailable = 1;
             float tofPion = fabs(1. / piBeta - sqrt(1+M_PION_PLUS*M_PION_PLUS/(momentum.mag()*momentum.mag())));
             h_piTOFbeta->Fill(trk->gPt(),tofPion);
          }

          //fill QA and TOF matching histograms
          h_piTOF->Fill(trk->gPt(),piTofAvailable);
          if (mHFCuts->hasGoodNHitsFitMinHist(trk)) h_piTOF_20->Fill(trk->gPt(),piTofAvailable);   //see line 303
          if (trk->isHFTTrack()) h_piTOF_HFT->Fill(trk->gPt(),piTofAvailable);
          if ((trk->isHFTTrack()) && (mHFCuts->hasGoodNSigmaHist(trk, 1))) h_piTOF_HFT_1sig->Fill(trk->gPt(),piTofAvailable); //hasGoodNSigmaHist(trk, 1) => check nSigma fo pion
          if ((trk->isHFTTrack()) && (mHFCuts->hasGoodNHitsFitMinHist(trk))) h_piTOF_HFT_20->Fill(trk->gPt(),piTofAvailable);
          if ((trk->isHFTTrack()) && (mHFCuts->hasGoodNSigmaHist(trk, 1)) && (mHFCuts->hasGoodNHitsFitMinHist(trk))) h_piTOF_HFT_1sig_20->Fill(trk->gPt(),piTofAvailable); //hasGoodNSigmaHist(trk, 1) => check nSigma of pion
          if (fabs(mHFCuts->hasGoodNSigmaHist(trk, 1))) h_piTOF_1sig->Fill(trk->gPt(),piTofAvailable); //hasGoodNSigmaHist(trk, 1) => check nSigma of pion
        }

        if (mHFCuts->hasGoodTPCnSigmaKaon(trk))
        { //new, hasGoodTPCnSigmaKaon defined in StPicoCutsBase
            //float kBeta = mHFCuts->getTofBetaBase(trk);  //SL16d
            float kBeta = mHFCuts->getTofBetaBase(trk, mPicoDst->event()->bField()); //SL16j, Vanek
            Int_t kTofAvailable = 0;
            if(!isnan(kBeta) && kBeta > 0)
            {
                 kTofAvailable = 1;
                 float tofKaon = fabs(1. / kBeta - sqrt(1+M_KAON_PLUS*M_KAON_PLUS/(momentum.mag()*momentum.mag())));
                 h_kTOFbeta->Fill(trk->gPt(),tofKaon);
            }

            h_kTOF->Fill(trk->gPt(),kTofAvailable);
            if (mHFCuts->hasGoodNHitsFitMinHist(trk)) h_kTOF_20->Fill(trk->gPt(),kTofAvailable);   //see line 303
            if (trk->isHFTTrack()) h_kTOF_HFT->Fill(trk->gPt(),kTofAvailable);
            if ((trk->isHFTTrack()) && (mHFCuts->hasGoodNSigmaHist(trk, 2))) h_kTOF_HFT_1sig->Fill(trk->gPt(),kTofAvailable); //hasGoodNSigmaHist(trk, 2) => check nSigma of kaon
            if ((trk->isHFTTrack()) && (mHFCuts->hasGoodNHitsFitMinHist(trk))) h_kTOF_HFT_20->Fill(trk->gPt(),kTofAvailable);
            if ((trk->isHFTTrack()) && (mHFCuts->hasGoodNSigmaHist(trk, 2)) && (mHFCuts->hasGoodNHitsFitMinHist(trk))) h_kTOF_HFT_1sig_20->Fill(trk->gPt(),kTofAvailable); //hasGoodNSigmaHist(trk, 2) => check nSigma for kaon of pion
            if (fabs(mHFCuts->hasGoodNSigmaHist(trk, 2))) h_kTOF_1sig->Fill(trk->gPt(),kTofAvailable); //hasGoodNSigmaHist(trk, 2) => check nSigma of kaon
        }

        //
        if (mHFCuts->hasGoodTPCnSigmaProton(trk))
        { //new, hasGoodTPCnSigmaProton defined in StPicoCutsBase
            //float pBeta = mHFCuts->getTofBetaBase(trk); //SL16d
            float pBeta = mHFCuts->getTofBetaBase(trk, mPicoDst->event()->bField()); //SL16j, Vanek
            Int_t pTofAvailable = 0;
            if(!isnan(pBeta) && pBeta > 0)
            {
               pTofAvailable = 1;
               float tofProton = fabs(1. / pBeta - sqrt(1+M_PROTON*M_PROTON/(momentum.mag()*momentum.mag())));
               h_pTOFbeta->Fill(trk->gPt(),tofProton);
            }

            h_pTOF->Fill(trk->gPt(),pTofAvailable);
            if (mHFCuts->hasGoodNHitsFitMinHist(trk)) h_pTOF_20->Fill(trk->gPt(),pTofAvailable);   //see line 303
            if (trk->isHFTTrack()) h_pTOF_HFT->Fill(trk->gPt(),pTofAvailable);
            if ((trk->isHFTTrack()) && (mHFCuts->hasGoodNSigmaHist(trk, 3))) h_pTOF_HFT_1sig->Fill(trk->gPt(),pTofAvailable); //hasGoodNSigmaHist(trk, 3) => check nSigma of proton
            if ((trk->isHFTTrack()) && (mHFCuts->hasGoodNHitsFitMinHist(trk))) h_pTOF_HFT_20->Fill(trk->gPt(),pTofAvailable);
            if ((trk->isHFTTrack()) && (mHFCuts->hasGoodNSigmaHist(trk, 3)) && (mHFCuts->hasGoodNHitsFitMinHist(trk))) h_pTOF_HFT_1sig_20->Fill(trk->gPt(),pTofAvailable); //hasGoodNSigmaHist(trk, 3) => check nSigma of proton
            if (fabs(mHFCuts->hasGoodNSigmaHist(trk, 3))) h_pTOF_1sig->Fill(trk->gPt(),pTofAvailable); //hasGoodNSigmaHist(trk, 3) => check nSigma of proton


        }

        h_pinsigma->Fill(momentum.mag(),trk->nSigmaPion());
        h_knsigma->Fill(momentum.mag(),trk->nSigmaKaon());
        h_pnsigma->Fill(momentum.mag(),trk->nSigmaProton());

        h_dedx->Fill(momentum.mag(),trk->dEdx());

        float globBeta = mHFCuts->getTofBetaBase(trk, mPicoDst->event()->bField()); //SL16j, Vanek
        
        if(!isnan(globBeta) && globBeta > 0)
        {
          h_OneOverBeta->Fill(momentum.mag(),1./globBeta);
        }


  } // .. end tracks loop



    double duration = (double) (std::clock() - start1) / (double) CLOCKS_PER_SEC;
  TH1F *h_time_per_event = static_cast<TH1F*>(mOutList->FindObject("h_time_per_event"));
  h_time_per_event->Fill(duration);

  return kStOK;
}


int StPicoDpmAnaMaker::createQA(){
       
   return 0;
}

// _________________________________________________________
int StPicoDpmAnaMaker::createCandidates() {
  // Creating candidates for D+- 3 body decay
  // D- -> K+2Pi decay

  for (unsigned short idxPion1 = 0; idxPion1 < mIdxPicoPions.size(); ++idxPion1) {
    StPicoTrack const *pion1 = mPicoDst->track(mIdxPicoPions[idxPion1]);
    // -- Pion selection

    for (unsigned short idxPion2 = idxPion1+1; idxPion2 < mIdxPicoPions.size(); ++idxPion2) {
      StPicoTrack const *pion2 = mPicoDst->track(mIdxPicoPions[idxPion2]);
      // -- Pion selection
      if ( !isCloseTracks(pion1,pion2,mPrimVtx, mBField)) continue;

      for (unsigned short idxKaon = 0; idxKaon < mIdxPicoKaons.size(); ++idxKaon) {
        StPicoTrack const *kaon = mPicoDst->track(mIdxPicoKaons[idxKaon]);
        // -- Kaon selection
        // -- TOF
        if( !mHFCuts->isHybridTOFKaon(kaon, mHFCuts->getTofBetaBase(kaon, mPicoDst->event()->bField())) ) continue; //SL16j, Vanek, hybrid TOF kaons, probably not needed here
        if (mIdxPicoKaons[idxKaon] == mIdxPicoPions[idxPion1]|| mIdxPicoKaons[idxKaon] == mIdxPicoPions[idxPion2] || mIdxPicoPions[idxPion1] == mIdxPicoPions[idxPion2]) continue;
        if ( !isCloseTracks(pion1,kaon,mPrimVtx, mBField)) continue;
        if ( !isCloseTracks(kaon,pion2,mPrimVtx, mBField)) continue;

        // -- Making triplet
        StHFTriplet triplet(pion1,pion2,kaon,mHFCuts->getHypotheticalMass(StHFCuts::kPion),mHFCuts->getHypotheticalMass(StHFCuts::kPion),mHFCuts->getHypotheticalMass(StHFCuts::kKaon), mIdxPicoPions[idxPion1],mIdxPicoPions[idxPion2],mIdxPicoKaons[idxKaon], mPrimVtx, mBField);

        if(!mHFCuts->isGoodSecondaryVertexTriplet(triplet)) continue;

        //apply tighter pre-cuts on low pT triplets
        if(triplet.pt() < mHFCuts->cutHighPtThreshold())
        {
          if( !mHFCuts->hasGoodTripletDaughtersDCAtoPV(triplet) ) continue;
        }

        mPicoHFEvent->addHFSecondaryVertexTriplet(&triplet);

      }  //end for (unsigned short idxKaon = 0; idxKaon < mIdxPicoKaons.size(); ++idxKaon)
    } //end for (unsigned short idxPion2 = idxPion1+1; idxPion2 < mIdxPicoPions.size(); ++idxPion2)
  } //end for (unsigned short idxPion1 = 0; idxPion1 < mIdxPicoPions.size(); ++idxPion1)

  return kStOK;
}

// _________________________________________________________
int StPicoDpmAnaMaker::analyzeCandidates() {

  // --- Analyze previously constructed triplets and save to ntuple
  // -- Decay channel1
  TClonesArray const * aCandidates= mPicoHFEvent->aHFSecondaryVertices();

   mRefmultCorrUtil->init(mPicoDst->event()->runId());

   if (!mRefmultCorrUtil)
   {
      LOG_WARN << " No mGRefMultCorrUtil! Skip! " << endl;
      return kStWarn;
   }


   if (mRefmultCorrUtil->isBadRun(mPicoDst->event()->runId())) return kStOK;
   mRefmultCorrUtil->initEvent(mPicoDst->event()->grefMult(), mPrimVtx.z(), mPicoDst->event()->ZDCx()) ;

   int const centrality = mRefmultCorrUtil->getCentralityBin9();
   const double reweight = mRefmultCorrUtil->getWeight();
   const double refmultCor = mRefmultCorrUtil->getRefMultCorr();

   float ZDCx = mPicoDst->event()->ZDCx();
   float BBCx = mPicoDst->event()->BBCx();


  if( mPicoHFEvent->nHFSecondaryVertices() >0 )
  {
    for (unsigned int idx = 0; idx <  mPicoHFEvent->nHFSecondaryVertices(); ++idx)
    {

      StHFTriplet const* triplet = static_cast<StHFTriplet*>(aCandidates->At(idx));
      StPicoTrack const* pion1 = mPicoDst->track(triplet->particle1Idx());
      StPicoTrack const* pion2 = mPicoDst->track(triplet->particle2Idx());
      StPicoTrack const* kaon = mPicoDst->track(triplet->particle3Idx());

      // Greatest distance between tracks
      float const dcaDaughters_12 = triplet->dcaDaughters12();
      float const dcaDaughters_23 = triplet->dcaDaughters23();
      float const dcaDaughters_13 = triplet->dcaDaughters31();
      float dcaMax = dcaDaughters_12 > dcaDaughters_13 ? dcaDaughters_12 : dcaDaughters_13;
      dcaMax = dcaMax > dcaDaughters_23 ? dcaMax : dcaDaughters_23;

      //TOF ---
      float kaonBetaBase = -1;
      float pion1BetaBase = -1;
      float pion2BetaBase = -1;

      kaonBetaBase = mHFCuts->getTofBetaBase(kaon, mPicoDst->event()->bField()); 
      pion1BetaBase = mHFCuts->getTofBetaBase(pion1, mPicoDst->event()->bField());
      pion2BetaBase = mHFCuts->getTofBetaBase(pion2, mPicoDst->event()->bField());

      float kaonTOFinvbeta = fabs(1. / kaonBetaBase - sqrt(1+M_KAON_PLUS*M_KAON_PLUS/(kaon->gMom(mPrimVtx,mBField).mag()*kaon->gMom(mPrimVtx,mBField).mag())));
      float pion1TOFinvbeta = fabs(1. / pion1BetaBase - sqrt(1+M_PION_PLUS*M_PION_PLUS/(pion1->gMom(mPrimVtx,mBField).mag()*pion1->gMom(mPrimVtx,mBField).mag())));
      float pion2TOFinvbeta = fabs(1. / pion2BetaBase - sqrt(1+M_PION_PLUS*M_PION_PLUS/(pion2->gMom(mPrimVtx,mBField).mag()*pion2->gMom(mPrimVtx,mBField).mag())));

      // -- Flag D plus and Dminus
      flag = -99;
      if( kaon->charge()<0 && pion1->charge()>0 && pion2->charge()>0 ) flag=0; // -- D+ -> K- + 2pi+
      if( kaon->charge()>0 && pion1->charge()<0 && pion2->charge()<0 ) flag=1; // -- D- -> K+ + 2pi-

      if( kaon->charge()<0 && pion1->charge()>0 && pion2->charge()<0 ) flag=2; // -+-
      if( kaon->charge()<0 && pion1->charge()<0 && pion2->charge()>0 ) flag=2; // --+
      if( kaon->charge()>0 && pion1->charge()>0 && pion2->charge()<0 ) flag=3; // ++-
      if( kaon->charge()>0 && pion1->charge()<0 && pion2->charge()>0 ) flag=3; // +-+

      if( kaon->charge()<0 && pion1->charge()<0 && pion2->charge()<0 ) flag=4; // ---
      if( kaon->charge()>0 && pion1->charge()>0 && pion2->charge()>0 ) flag=5; // +++

      // ---
      // Saving to TTree
      pi1_runId = mPicoHFEvent->runId();
      pi1_eventId = mPicoHFEvent->eventId();
      pi1_eta = pion1->gMom(mPrimVtx,mBField).pseudoRapidity();
      pi1_pt  = pion1->gPt();
      pi1_dca = triplet->particle1Dca();
      pi1_nSigma = pion1->nSigmaPion();
      pi1_nHitFit = pion1->nHitsFit();
      pi1_nHitsMax = pion1->nHitsMax();
      pi1_TOFinvbeta = pion1TOFinvbeta;
      pi1_betaBase = pion1BetaBase;

      
      //eventId and runId common for all particles in the triplet
      pi2_eta = pion2->gMom(mPrimVtx,mBField).pseudoRapidity();
      pi2_pt = pion2->gPt();
      pi2_dca = triplet->particle2Dca();
      pi2_nSigma = pion2->nSigmaPion();
      pi2_nHitFit = pion2->nHitsFit();
      pi2_nHitsMax = pion2->nHitsMax();
      pi2_TOFinvbeta = pion2TOFinvbeta;
      pi2_betaBase = pion2BetaBase;

      k_eta = kaon->gMom(mPrimVtx,mBField).pseudoRapidity();
      k_pt = kaon->gPt();
      k_dca = triplet->particle3Dca();
      k_nSigma = kaon->nSigmaKaon();
      k_nHitFit = kaon->nHitsFit();
      k_nHitsMax = kaon->nHitsMax();
      k_TOFinvbeta = kaonTOFinvbeta;
      k_betaBase = kaonBetaBase;

      mdcaMax = dcaMax;
      primVx = mPrimVtx.x();
      primVy = mPrimVtx.y();      
      primVz = mPrimVtx.z();

      primVx_err = mPicoDst->event()->primaryVertexError().x();
      primVy_err = mPicoDst->event()->primaryVertexError().y();
      primVz_err = mPicoDst->event()->primaryVertexError().z();

      D_theta = triplet->pointingAngle();
      D_decayL = triplet->decayLength();
      D_phi = triplet->phi();
      D_eta = triplet->eta();
      D_pt = triplet->pt();
      D_mass = triplet->m();
      D_dV0Max = triplet->dV0Max();
      mcentrality = centrality;
      mreweight = reweight;

      ntp_Dmeson->Fill();


    } // for (unsigned int idx = 0; idx <  mPicoHFEvent->nHFSecondaryVertices(); ++idx) {
  }

  return kStOK;
}

// _________________________________________________________
bool StPicoDpmAnaMaker::isHadron(StPicoTrack const * const trk, int pidFlag) const {
  // -- good hadron
  return (mHFCuts->isGoodTrack(trk) && mHFCuts->isTPCHadron(trk, pidFlag));
}

// _________________________________________________________
bool StPicoDpmAnaMaker::isPion(StPicoTrack const * const trk) const {
  // -- good pion

   StThreeVectorF t = trk->gMom();
   if (fabs(t.pseudoRapidity()) > 1.) return false; //pridano fabs 1212
   if (!mHFCuts->isHybridTOFHadron(trk, mHFCuts->getTofBetaBase(trk, mPicoDst->event()->bField()), StHFCuts::kPion) ) return false;
   if (!mHFCuts->cutMinDcaToPrimVertex(trk, StPicoCutsBase::kPion)) return false;
   return (mHFCuts->isGoodTrack(trk) && mHFCuts->isTPCHadron(trk, StPicoCutsBase::kPion));
}

// _________________________________________________________
bool StPicoDpmAnaMaker::isKaon(StPicoTrack const * const trk) const {
  // -- good kaon

  StThreeVectorF t = trk->gMom();
  if (fabs(t.pseudoRapidity()) > 1.) return false;//pridano fabs 1212
  if (!mHFCuts->isHybridTOFHadron(trk, mHFCuts->getTofBetaBase(trk, mPicoDst->event()->bField()), StHFCuts::kKaon) ) return false;
  if (!mHFCuts->cutMinDcaToPrimVertex(trk, StPicoCutsBase::kKaon)) return false;
  return (mHFCuts->isGoodTrack(trk) && mHFCuts->isTPCHadron(trk, StPicoCutsBase::kKaon));
}

// _________________________________________________________
bool StPicoDpmAnaMaker::isProton(StPicoTrack const * const trk) const {
  // -- good proton
  return (mHFCuts->isGoodTrack(trk) && mHFCuts->isTPCHadron(trk, StPicoCutsBase::kProton));
}

double StPicoDpmAnaMaker::DCA(StPicoTrack const * const trk, StThreeVectorF const & vtx) const {
  // -- particle DCA

  return ((trk->origin() - vtx).mag());
}


bool StPicoDpmAnaMaker::isCloseTracks(StPicoTrack const * const trk1, StPicoTrack const * const trk2, StThreeVectorF const & vtx, float bField) const {
  // -- good pair

  if( ( trk1->origin()-vtx ).mag()>1.5 || ( trk2->origin()-vtx ).mag()>1.5 ) return false;

  StThreeVectorF const p1Mom = trk1->gMom();
  StThreeVectorF const p2Mom = trk2->gMom();
  StPhysicalHelixD const p1StraightLine(p1Mom, trk1->origin(), 0, trk1->charge());
  StPhysicalHelixD const p2StraightLine(p2Mom, trk2->origin(), 0, trk2->charge());

  pair<double, double> const ss = p1StraightLine.pathLengths(p2StraightLine);
  StThreeVectorF const p1AtDcaToP2 = p1StraightLine.at(ss.first);
  StThreeVectorF const p2AtDcaToP1 = p2StraightLine.at(ss.second);
  float const dca = (p1AtDcaToP2-p2AtDcaToP1).mag();

  if(dca > mHFCuts->cutSecondaryTripletDcaDaughters12Max()) return false; 

  return true;
}

//-----------------------------------------------------------------------------
