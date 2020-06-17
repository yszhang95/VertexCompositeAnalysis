// system include files
#include <memory>
#include <string>
#include <vector>
#include <array>
#include <iostream>
#include <math.h>
#include <algorithm>

#include <TH1.h>
#include <TH2.h>
#include <TTree.h>
#include <TFile.h>
#include <TVector3.h>
#include <TMath.h>

#include <Math/Functions.h>
#include <Math/SVector.h>
#include <Math/SMatrix.h>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Common/interface/TriggerNames.h"

#include "DataFormats/TrackReco/interface/DeDxData.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/GenericParticle.h"
#include "DataFormats/HeavyIonEvent/interface/CentralityBins.h"
#include "DataFormats/HeavyIonEvent/interface/Centrality.h"
#include "DataFormats/HeavyIonEvent/interface/EvtPlane.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "HLTrigger/HLTcore/interface/HLTPrescaleProvider.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/PatternTools/interface/ClosestApproachInRPhi.h"
#include "TrackingTools/PatternTools/interface/TSCBLBuilderNoMaterial.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

//
// constants, enums and typedefs
//

#define PI 3.1416
#define MAXCAN 50000
#define MAXDAU 3
#define MAXGDAU 2
#define MAXTRG 1024
#define MAXSEL 100

typedef ROOT::Math::SMatrix<double, 3, 3, ROOT::Math::MatRepSym<double, 3> > SMatrixSym3D;
typedef ROOT::Math::SVector<double, 3> SVector3;
typedef ROOT::Math::SVector<double, 6> SVector6;

//
// class decleration
//

class PATGenericParticleTreeProducer : public edm::EDAnalyzer {
public:
  explicit PATGenericParticleTreeProducer(const edm::ParameterSet&);
  ~PATGenericParticleTreeProducer();

  //using MVACollection = std::vector<float>;

private:
  virtual void beginJob();
  virtual void beginRun(const edm::Run&, const edm::EventSetup&);
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void fillRECO(const edm::Event&, const edm::EventSetup&);
  virtual void fillGEN(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  //virtual void initHistogram();
  virtual void initTree();
  reco::GenParticleRef findLastPar(const reco::GenParticleRef&);
  reco::GenParticleRef findMother(const reco::GenParticleRef&);
  bool hasDaughters(const reco::GenParticleRef&, const std::vector<int>&);
  void genDecayLength(const uint&, const reco::GenParticle&);

  // ----------member data ---------------------------

  edm::Service<TFileService> fs;

  TTree* PATGenericParticleNtuple;
  /*
  TH2F*  hMassVsMVA[6][10];
  TH2F*  hpTVsMVA[6][10];
  TH2F*  hetaVsMVA[6][10];
  TH2F*  hyVsMVA[6][10];
  TH2F*  hVtxProbVsMVA[6][10];
  TH2F*  h3DCosPointingAngleVsMVA[6][10];
  TH2F*  h3DPointingAngleVsMVA[6][10];
  TH2F*  h2DCosPointingAngleVsMVA[6][10];
  TH2F*  h2DPointingAngleVsMVA[6][10];
  TH2F*  h3DDecayLengthSignificanceVsMVA[6][10];
  TH2F*  h3DDecayLengthVsMVA[6][10];
  TH2F*  h2DDecayLengthSignificanceVsMVA[6][10];
  TH2F*  h2DDecayLengthVsMVA[6][10];
  TH2F*  h3DDCAVsMVA[6][10];
  TH2F*  h2DDCAVsMVA[6][10];
  TH2F*  hzDCASignificanceDaughterVsMVA[MAXDAU][6][10];
  TH2F*  hxyDCASignificanceDaughterVsMVA[MAXDAU][6][10];
  TH2F*  hNHitDVsMVA[MAXDAU][6][10];
  TH2F*  hpTDVsMVA[MAXDAU][6][10];
  TH2F*  hpTerrDVsMVA[MAXDAU][6][10];
  TH2F*  hEtaDVsMVA[MAXDAU][6][10];
  TH2F*  hdedxHarmonic2DVsMVA[MAXDAU][6][10];
  TH2F*  hdedxHarmonic2DVsP[MAXDAU][6][10];
  */

  bool   saveTree_;
  //bool   saveHistogram_;
  //bool   saveAllHistogram_;
  double massHistPeak_;
  double massHistWidth_;
  int    massHistBins_;

  //options
  bool doRecoNtuple_;
  bool doGenNtuple_;   
  bool doGenMatching_;
  bool doGenMatchingTOF_;
  bool decayInGen_;
  bool twoLayerDecay_;
  bool threeProngDecay_;
  bool doMuon_;
  bool doMuonFull_;
  const std::vector<int> PID_dau_;
  const ushort NDAU_;
  const ushort NGDAU_ = MAXGDAU;

  //cut variables
  double multMax_;
  double multMin_;
  double deltaR_; //deltaR for Gen matching
  double deltaPt_; //deltaPt for Gen matching

  std::vector<double> pTBins_;
  std::vector<double> yBins_;

  //tree branches
  //event info
  uint  runNb;
  uint  eventNb;
  uint  lsNb;
  short trigPrescale[MAXTRG];
  short centrality;
  int   Ntrkoffline;
  int   Npixel;
  short nPV;
  uint candSize;
  bool  trigHLT[MAXTRG];
  bool  evtSel[MAXSEL];
  float HFsumETPlus;
  float HFsumETMinus;
  float ZDCPlus;
  float ZDCMinus;
  float bestvx;
  float bestvy;
  float bestvz;
  float ephfpAngle[3];
  float ephfmAngle[3];
  float eptrackmidAngle[3];
  float ephfpQ[3];
  float ephfmQ[3];
  float eptrackmidQ[3];
  float ephfpSumW;
  float ephfmSumW;
  float eptrackmidSumW;

  //Composite candidate info
  float mva[MAXCAN];
  float pt[MAXCAN];
  float eta[MAXCAN];
  float phi[MAXCAN];
  float flavor[MAXCAN];
  float y[MAXCAN];
  float mass[MAXCAN];
  float VtxProb[MAXCAN];
  float dlos[MAXCAN];
  float dl[MAXCAN];
  float dlerror[MAXCAN];
  float dlerror2[MAXCAN];
  float agl[MAXCAN];
  float vtxChi2[MAXCAN];
  //float ndf[MAXCAN]; // yousen
  float agl_abs[MAXCAN];
  float agl2D[MAXCAN];
  float agl2D_abs[MAXCAN];
  float dlos2D[MAXCAN];
  float dl2D[MAXCAN];
  bool isSwap[MAXCAN];
  bool matchGEN[MAXCAN];
  int idmom_reco[MAXCAN];
    
  //dau candidate info
  // yousen
  /*
  float grand_mass[MAXCAN];
  float grand_VtxProb[MAXCAN];
  float grand_dlos[MAXCAN];
  float grand_dl[MAXCAN];
  float grand_dlerror[MAXCAN];
  float grand_agl[MAXCAN];
  float grand_vtxChi2[MAXCAN];
  float grand_ndf[MAXCAN];
  float grand_agl_abs[MAXCAN];
  float grand_agl2D[MAXCAN];
  float grand_agl2D_abs[MAXCAN];
  float grand_dlos2D[MAXCAN];
  */
  std::vector<int>  dauIsIntermediate_; //yousen
  float grand_mass[MAXDAU][MAXCAN]; //yousen
  float grand_VtxProb[MAXDAU][MAXCAN]; //yousen
  float grand_dlos[MAXDAU][MAXCAN]; //yousen
  float grand_dl[MAXDAU][MAXCAN]; //yousen
  //float grand_dlerror[MAXDAU][MAXCAN]; //yousen
  float grand_agl[MAXDAU][MAXCAN]; //yousen
  float grand_vtxChi2[MAXDAU][MAXCAN]; //yousen
  //float grand_ndf[MAXDAU][MAXCAN]; //yousen
  float grand_agl_abs[MAXDAU][MAXCAN]; //yousen
  float grand_agl2D[MAXDAU][MAXCAN]; //yousen
  float grand_agl2D_abs[MAXDAU][MAXCAN]; //yousen
  float grand_dlos2D[MAXDAU][MAXCAN]; //yousen
  float grand_dl2D[MAXDAU][MAXCAN]; //yousen

  //dau info
  float dzos[MAXDAU][MAXCAN];
  float dxyos[MAXDAU][MAXCAN];
  float nhit[MAXDAU][MAXCAN];
  bool  trkquality[MAXDAU][MAXCAN];
  float ptDau[MAXDAU][MAXCAN];
  float ptErr[MAXDAU][MAXCAN];
  float pDau[MAXDAU][MAXCAN];
  float etaDau[MAXDAU][MAXCAN];
  float phiDau[MAXDAU][MAXCAN];
  short chargeDau[MAXDAU][MAXCAN];
  int   pid[MAXDAU][MAXCAN];
  float tof[MAXDAU][MAXCAN];
  float H2dedx[MAXDAU][MAXCAN];
  float T4dedx[MAXDAU][MAXCAN];
  float trkChi[MAXDAU][MAXCAN];
   
  //grand-dau info
  // yousen
  /*
  float grand_dzos[MAXGDAU][MAXCAN];
  float grand_dxyos[MAXGDAU][MAXCAN];
  float grand_nhit[MAXGDAU][MAXCAN];
  bool  grand_trkquality[MAXGDAU][MAXCAN];
  float grand_pt[MAXGDAU][MAXCAN];
  float grand_ptErr[MAXGDAU][MAXCAN];
  float grand_p[MAXGDAU][MAXCAN];
  float grand_eta[MAXGDAU][MAXCAN];
  short grand_charge[MAXGDAU][MAXCAN];
  float grand_H2dedx[MAXGDAU][MAXCAN];
  float grand_T4dedx[MAXGDAU][MAXCAN];
  float grand_trkChi[MAXGDAU][MAXCAN];
  */
  float grand_dzos[MAXGDAU][MAXDAU][MAXCAN];
  float grand_dxyos[MAXGDAU][MAXDAU][MAXCAN];
  float grand_nhit[MAXGDAU][MAXDAU][MAXCAN];
  bool  grand_trkquality[MAXGDAU][MAXDAU][MAXCAN];
  float grand_pt[MAXGDAU][MAXDAU][MAXCAN];
  float grand_ptErr[MAXGDAU][MAXDAU][MAXCAN];
  float grand_p[MAXGDAU][MAXDAU][MAXCAN];
  float grand_eta[MAXGDAU][MAXDAU][MAXCAN];
  short grand_charge[MAXGDAU][MAXDAU][MAXCAN];
  float grand_H2dedx[MAXGDAU][MAXDAU][MAXCAN];
  float grand_T4dedx[MAXGDAU][MAXDAU][MAXCAN];
  float grand_trkChi[MAXGDAU][MAXDAU][MAXCAN];
    
  //dau muon info
  bool  onestmuon[MAXDAU][MAXCAN];
  bool  pfmuon[MAXDAU][MAXCAN];
  bool  glbmuon[MAXDAU][MAXCAN];
  bool  trkmuon[MAXDAU][MAXCAN];
  bool  tightmuon[MAXDAU][MAXCAN];
  bool  softmuon[MAXDAU][MAXCAN];
  bool  hybridmuon[MAXDAU][MAXCAN];
  bool  hpmuon[MAXDAU][MAXCAN];
  std::vector<std::vector<UChar_t> > trgmuon[MAXDAU];
  short nmatchedst[MAXDAU][MAXCAN];
  short ntrackerlayer[MAXDAU][MAXCAN];
  short npixellayer[MAXDAU][MAXCAN];
  short npixelhit[MAXDAU][MAXCAN];
  short nmuonhit[MAXDAU][MAXCAN];
  float glbtrkchi[MAXDAU][MAXCAN];
  float muonbestdxy[MAXDAU][MAXCAN];
  float muonbestdz[MAXDAU][MAXCAN];
  float muondxy[MAXDAU][MAXCAN];
  float muondz[MAXDAU][MAXCAN];
  short nmatchedch[MAXDAU][MAXCAN];
  float matchedenergy[MAXDAU][MAXCAN];
  float dx_seg[MAXDAU][MAXCAN];
  float dy_seg[MAXDAU][MAXCAN];
  float dxSig_seg[MAXDAU][MAXCAN];
  float dySig_seg[MAXDAU][MAXCAN];
  float ddxdz_seg[MAXDAU][MAXCAN];
  float ddydz_seg[MAXDAU][MAXCAN];
  float ddxdzSig_seg[MAXDAU][MAXCAN];
  float ddydzSig_seg[MAXDAU][MAXCAN];

  // gen info
  std::vector<reco::GenParticleRef> genVec_;
  float weight_gen;
  uint candSize_gen;
  float pt_gen[MAXCAN];
  float eta_gen[MAXCAN];
  float y_gen[MAXCAN];
  short status_gen[MAXCAN];
  int pid_gen[MAXCAN];
  int idmom_gen[MAXCAN];
  short idxrec_gen[MAXCAN];
  float agl_abs_gen[MAXCAN];
  float agl2D_abs_gen[MAXCAN];
  float dl_gen[MAXCAN];
  float dl2D_gen[MAXCAN];
  int idDau_gen[MAXDAU][MAXCAN];
  short chargeDau_gen[MAXDAU][MAXCAN];
  float ptDau_gen[MAXDAU][MAXCAN];
  float etaDau_gen[MAXDAU][MAXCAN];
  float phiDau_gen[MAXDAU][MAXCAN];

  bool useAnyMVA_;
  bool isSkimMVA_;
  bool isCentrality_;
  bool isEventPlane_;
  bool useDeDxData_;

  //token
  edm::EDGetTokenT<reco::BeamSpot> tok_offlineBS_;
  edm::EDGetTokenT<reco::VertexCollection> tok_offlinePV_;
  edm::EDGetTokenT<pat::GenericParticleCollection> patGenericParticleCollection_Token_;
  //edm::EDGetTokenT<MVACollection> MVAValues_Token_;

  edm::EDGetTokenT<edm::ValueMap<reco::DeDxData> > Dedx_Token1_;
  edm::EDGetTokenT<edm::ValueMap<reco::DeDxData> > Dedx_Token2_;
  edm::EDGetTokenT<reco::GenParticleCollection> tok_genParticle_;
  edm::EDGetTokenT<GenEventInfoProduct> tok_genInfo_;

  edm::EDGetTokenT<int> tok_centBinLabel_;
  edm::EDGetTokenT<reco::Centrality> tok_centSrc_;

  edm::EDGetTokenT<reco::EvtPlaneCollection> tok_eventplaneSrc_;

  //trigger
  const std::vector<std::string> triggerNames_;
  const std::vector<std::string> filterNames_;
  edm::EDGetTokenT<edm::TriggerResults> tok_triggerResults_;
  const ushort NTRG_;

  //event selection
  const std::vector<std::string> eventFilters_;
  edm::EDGetTokenT<edm::TriggerResults> tok_filterResults_;
  const ushort NSEL_;
  const std::string selectEvents_;

  //prescale provider
  HLTPrescaleProvider hltPrescaleProvider_;
};

//
// static data member definitions
//

//
// constructors and destructor
//

PATGenericParticleTreeProducer::PATGenericParticleTreeProducer(const edm::ParameterSet& iConfig) :
  PID_dau_(iConfig.getUntrackedParameter<std::vector<int> >("PID_dau")),
  NDAU_(PID_dau_.size()>MAXDAU ? MAXDAU : PID_dau_.size()),
  patGenericParticleCollection_Token_(consumes<pat::GenericParticleCollection>(iConfig.getUntrackedParameter<edm::InputTag>("VertexCompositeCollection"))),
  triggerNames_(iConfig.getUntrackedParameter<std::vector<std::string> >("triggerPathNames")),
  filterNames_(iConfig.getUntrackedParameter<std::vector<std::string> >("triggerFilterNames")),
  NTRG_(triggerNames_.size()>MAXTRG ? MAXTRG : triggerNames_.size()),
  eventFilters_(iConfig.getUntrackedParameter<std::vector<std::string> >("eventFilterNames")),
  NSEL_(eventFilters_.size()>MAXSEL ? MAXSEL : eventFilters_.size()),
  selectEvents_(iConfig.getUntrackedParameter<std::string>("selectEvents")),
  hltPrescaleProvider_(iConfig, consumesCollector(), *this)
{
  //options
  doRecoNtuple_ = iConfig.getUntrackedParameter<bool>("doRecoNtuple");
  doGenNtuple_ = iConfig.getUntrackedParameter<bool>("doGenNtuple");
  twoLayerDecay_ = iConfig.getUntrackedParameter<bool>("twoLayerDecay");
  threeProngDecay_ = iConfig.getUntrackedParameter<bool>("threeProngDecay");
  doGenMatching_ = iConfig.getUntrackedParameter<bool>("doGenMatching");
  doGenMatchingTOF_ = iConfig.getUntrackedParameter<bool>("doGenMatchingTOF");
  decayInGen_ = iConfig.getUntrackedParameter<bool>("decayInGen");
  doMuon_ = iConfig.getUntrackedParameter<bool>("doMuon");
  doMuonFull_ = iConfig.getUntrackedParameter<bool>("doMuonFull");

  saveTree_ = iConfig.getUntrackedParameter<bool>("saveTree");
  //saveHistogram_ = iConfig.getUntrackedParameter<bool>("saveHistogram");
  //saveAllHistogram_ = iConfig.getUntrackedParameter<bool>("saveAllHistogram");
  massHistPeak_ = iConfig.getUntrackedParameter<double>("massHistPeak");
  massHistWidth_ = iConfig.getUntrackedParameter<double>("massHistWidth");
  massHistBins_ = iConfig.getUntrackedParameter<int>("massHistBins");

  //cut variables
  multMax_ = iConfig.getUntrackedParameter<double>("multMax", -1);
  multMin_ = iConfig.getUntrackedParameter<double>("multMin", -1);
  deltaR_ = iConfig.getUntrackedParameter<double>("deltaR", 0.03);
  deltaPt_ = iConfig.getUntrackedParameter<double>("deltaPt", 0.5);

  pTBins_ = iConfig.getUntrackedParameter<std::vector<double> >("pTBins");
  yBins_  = iConfig.getUntrackedParameter<std::vector<double> >("yBins");

  dauIsIntermediate_ = iConfig.getUntrackedParameter<std::vector<int> >("dauIsIntermediate");

  //input tokens
  tok_offlineBS_ = consumes<reco::BeamSpot>(iConfig.getUntrackedParameter<edm::InputTag>("beamSpotSrc"));
  tok_offlinePV_ = consumes<reco::VertexCollection>(iConfig.getUntrackedParameter<edm::InputTag>("VertexCollection"));
  //MVAValues_Token_ = consumes<MVACollection>(iConfig.getParameter<edm::InputTag>("MVACollection"));
  tok_genParticle_ = consumes<reco::GenParticleCollection>(edm::InputTag(iConfig.getUntrackedParameter<edm::InputTag>("GenParticleCollection")));
  tok_genInfo_ = consumes<GenEventInfoProduct>(edm::InputTag("generator"));

  useDeDxData_ = (iConfig.exists("useDeDxData") ? iConfig.getParameter<bool>("useDeDxData") : false);
  if(useDeDxData_)
  {
    Dedx_Token1_ = consumes<edm::ValueMap<reco::DeDxData> >(edm::InputTag("dedxHarmonic2"));
    Dedx_Token2_ = consumes<edm::ValueMap<reco::DeDxData> >(edm::InputTag("dedxTruncated40"));
  }

  isCentrality_ = (iConfig.exists("isCentrality") ? iConfig.getParameter<bool>("isCentrality") : false);
  if(isCentrality_)
  {
    tok_centBinLabel_ = consumes<int>(iConfig.getParameter<edm::InputTag>("centralityBinLabel"));
    tok_centSrc_ = consumes<reco::Centrality>(iConfig.getParameter<edm::InputTag>("centralitySrc"));
  }

  isEventPlane_ = (iConfig.exists("isEventPlane") ? iConfig.getParameter<bool>("isEventPlane") : false);
  if(isEventPlane_) tok_eventplaneSrc_ = consumes<reco::EvtPlaneCollection>(iConfig.getParameter<edm::InputTag>("eventplaneSrc"));

  useAnyMVA_ = (iConfig.exists("useAnyMVA") ? iConfig.getParameter<bool>("useAnyMVA") : false);
  //if(useAnyMVA_) MVAValues_Token_ = consumes<MVACollection>(iConfig.getParameter<edm::InputTag>("MVACollection"));
  isSkimMVA_ = iConfig.getUntrackedParameter<bool>("isSkimMVA");
  
  tok_triggerResults_ = consumes<edm::TriggerResults>(iConfig.getUntrackedParameter<edm::InputTag>("TriggerResultCollection"));
  tok_filterResults_ = consumes<edm::TriggerResults>(iConfig.getUntrackedParameter<edm::InputTag>("FilterResultCollection"));
}


PATGenericParticleTreeProducer::~PATGenericParticleTreeProducer()
{

  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
PATGenericParticleTreeProducer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  //check event
  if(selectEvents_!="")
  {
    edm::Handle<edm::TriggerResults> filterResults;
    iEvent.getByToken(tok_filterResults_, filterResults);
    const auto& filterNames = iEvent.triggerNames(*filterResults);
    const auto& index = filterNames.triggerIndex(selectEvents_);
    if(index<filterNames.size() && filterResults->wasrun(index) && !filterResults->accept(index)) return;
  }
  genVec_.clear();
  if(doRecoNtuple_) fillRECO(iEvent,iSetup);
  if(doGenNtuple_) fillGEN(iEvent,iSetup);
  if(saveTree_) PATGenericParticleNtuple->Fill();
}


void
PATGenericParticleTreeProducer::fillRECO(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  //get collection
  edm::Handle<reco::BeamSpot> beamspot;
  iEvent.getByToken(tok_offlineBS_, beamspot);
  edm::Handle<reco::VertexCollection> vertices;
  iEvent.getByToken(tok_offlinePV_, vertices);
  if(!vertices.isValid()) throw cms::Exception("PATGenericParticleAnalyzer") << "Primary vertices  collection not found!" << std::endl;

  edm::Handle<pat::GenericParticleCollection> generalParticles;
  iEvent.getByToken(patGenericParticleCollection_Token_, generalParticles);
  if(!generalParticles.isValid()) throw cms::Exception("PATGenericParticleAnalyzer") << "V0 candidate collection not found!" << std::endl;

  //edm::Handle<MVACollection> mvavalues;
  //if(useAnyMVA_)
  //{
    //iEvent.getByToken(MVAValues_Token_, mvavalues);
    //if(!mvavalues.isValid()) throw cms::Exception("PATGenericParticleAnalyzer") << "MVA collection not found!" << std::endl;
    //assert( (*mvavalues).size() == generalParticles->size() );
  //}

  // dedx information is in pat
  //edm::Handle<edm::ValueMap<reco::DeDxData> > dEdxHandle1 , dEdxHandle2;
  //if(useDeDxData_)
  //{
    //iEvent.getByToken(Dedx_Token1_, dEdxHandle1);
    //iEvent.getByToken(Dedx_Token2_, dEdxHandle2);
  //}

  runNb = iEvent.id().run();
  eventNb = iEvent.id().event();
  lsNb = iEvent.luminosityBlock();

  //Trigger Information
  edm::Handle<edm::TriggerResults> triggerResults;
  iEvent.getByToken(tok_triggerResults_, triggerResults);
  if(triggerNames_.size()>0)
  {
    const edm::TriggerNames& triggerNames = iEvent.triggerNames(*triggerResults);
    for(ushort iTr=0; iTr<NTRG_; iTr++)
    {
      //Initiliaze the arrays
      trigHLT[iTr] = false;
      trigPrescale[iTr] = -9;
      //Find the trigger index
      const auto& trigName = triggerNames_.at(iTr);
      std::vector<ushort> trgIdxFound;
      for(ushort trgIdx=0; trgIdx<triggerNames.size(); trgIdx++)
      {
        if(triggerNames.triggerName(trgIdx).find(trigName)!=std::string::npos && triggerResults->wasrun(trgIdx)) { trgIdxFound.push_back(trgIdx); }
      }
      short triggerIndex = -1;
      if(trgIdxFound.size()>1)
      {
        for(const auto& trgIdx : trgIdxFound) { if(triggerResults->accept(trgIdx)) { triggerIndex = trgIdx; break; } }
        if(triggerIndex<0) triggerIndex = trgIdxFound[0];
      }
      else if(trgIdxFound.size()==1) triggerIndex = trgIdxFound[0];
      else continue;
      //Check if trigger fired
      bool isTriggerFired = false;
      if(triggerResults->accept(triggerIndex)) isTriggerFired = true;
      //Get the trigger prescale
      int prescaleValue = -1;
      if(hltPrescaleProvider_.hltConfigProvider().inited() && hltPrescaleProvider_.prescaleSet(iEvent,iSetup)>=0)
      {
        const auto& presInfo = hltPrescaleProvider_.prescaleValuesInDetail(iEvent, iSetup, triggerNames.triggerName(triggerIndex));
        const auto& hltPres = presInfo.second;
        const short& l1Pres = ((presInfo.first.size()==1) ? presInfo.first.at(0).second : ((presInfo.first.size()>1) ? 1 : -1));
        prescaleValue = hltPres*l1Pres;
      }
      trigPrescale[iTr] = prescaleValue;
      if(isTriggerFired) trigHLT[iTr] = true;
    }
  }

  //Event selection information
  edm::Handle<edm::TriggerResults> filterResults;
  iEvent.getByToken(tok_filterResults_, filterResults);
  if(eventFilters_.size()>0)
  {
    const edm::TriggerNames& filterNames = iEvent.triggerNames(*filterResults);
    for(ushort iFr=0; iFr<eventFilters_.size(); ++iFr)
    {
      evtSel[iFr] = false;
      const auto& index = filterNames.triggerIndex(eventFilters_.at(iFr));
      if(index < filterNames.size()) evtSel[iFr] = (filterResults->wasrun(index) && filterResults->accept(index));
    }
  }

  centrality = -1;
  if(isCentrality_)
  {
    edm::Handle<reco::Centrality> cent;
    iEvent.getByToken(tok_centSrc_, cent);
    HFsumETPlus = (cent.isValid() ? cent->EtHFtowerSumPlus() : -1.);
    HFsumETMinus = (cent.isValid() ? cent->EtHFtowerSumMinus() : -1.);
    Npixel = (cent.isValid() ? cent->multiplicityPixel() : -1);
    ZDCPlus = (cent.isValid() ? cent->zdcSumPlus() : -1.);
    ZDCMinus = (cent.isValid() ? cent->zdcSumMinus() : -1.);
    Ntrkoffline = (cent.isValid() ? cent->Ntracks() : -1);
      
    edm::Handle<int> cbin;
    iEvent.getByToken(tok_centBinLabel_, cbin);
    centrality = (cbin.isValid() ? *cbin : -1);
  }

  if(isEventPlane_)
  {
    edm::Handle<reco::EvtPlaneCollection> eventplanes;
    iEvent.getByToken(tok_eventplaneSrc_, eventplanes);

    ephfmAngle[0] = (eventplanes.isValid() ? (*eventplanes)[0].angle(2) : -99.);
    ephfmAngle[1] = (eventplanes.isValid() ? (*eventplanes)[6].angle(2) : -99.);
    ephfmAngle[2] = (eventplanes.isValid() ? (*eventplanes)[13].angle(2) : -99.);

    ephfpAngle[0] = (eventplanes.isValid() ? (*eventplanes)[1].angle(2) : -99.);
    ephfpAngle[1] = (eventplanes.isValid() ? (*eventplanes)[7].angle(2) : -99.);
    ephfpAngle[2] = (eventplanes.isValid() ? (*eventplanes)[14].angle(2) : -99.);
    
    eptrackmidAngle[0] = -99.9;
    eptrackmidAngle[1] = (eventplanes.isValid() ? (*eventplanes)[9].angle(2) : -99.);
    eptrackmidAngle[2] = (eventplanes.isValid() ? (*eventplanes)[16].angle(2) : -99.);

    ephfmQ[0] = (eventplanes.isValid() ? (*eventplanes)[0].q(2) : -99.);
    ephfmQ[1] = (eventplanes.isValid() ? (*eventplanes)[6].q(2) : -99.);
    ephfmQ[2] = (eventplanes.isValid() ? (*eventplanes)[13].q(2) : -99.);

    ephfpQ[0] = (eventplanes.isValid() ? (*eventplanes)[1].q(2) : -99.);
    ephfpQ[1] = (eventplanes.isValid() ? (*eventplanes)[7].q(2) : -99.);
    ephfpQ[2] = (eventplanes.isValid() ? (*eventplanes)[14].q(2) : -99.);

    eptrackmidQ[0] = -99.9;
    eptrackmidQ[1] = (eventplanes.isValid() ? (*eventplanes)[9].q(2) : -99.);
    eptrackmidQ[2] = (eventplanes.isValid() ? (*eventplanes)[16].q(2) : -99.);
    
    ephfmSumW = (eventplanes.isValid() ? (*eventplanes)[6].sumw() : -99.);
    ephfpSumW = (eventplanes.isValid() ? (*eventplanes)[7].sumw() : -99.);
    eptrackmidSumW = (eventplanes.isValid() ? (*eventplanes)[9].sumw() : -99.);
  }

  nPV = vertices->size();
  //best vertex
  const auto& vtxPrimary = (vertices->size()>0 ? (*vertices)[0] : reco::Vertex());
  const bool& isPV = (!vtxPrimary.isFake() && vtxPrimary.tracksSize()>=2);
  const auto& bs = (!isPV ? reco::Vertex(beamspot->position(), beamspot->covariance3D()) : reco::Vertex());
  const reco::Vertex& vtx = (isPV ? vtxPrimary : bs);
  bestvz = vtx.z(); bestvx = vtx.x(); bestvy = vtx.y();
  const math::XYZPoint bestvtx(bestvx, bestvy, bestvz);
  //const double& bestvzError = vtx.zError(), bestvxError = vtx.xError(), bestvyError = vtx.yError(); // yousen

  //RECO Candidate info
  candSize = generalParticles->size();
  if(candSize>MAXCAN) throw cms::Exception("PATGenericParticleAnalyzer") << "Number of candidates (" << candSize << ") exceeds limit!" << std::endl; 
  for(uint it=0; it<candSize; ++it)
  { 
    const auto& cand = (*generalParticles)[it];

    pt[it] = cand.pt();
    eta[it] = cand.eta();
    phi[it] = cand.phi();
    mass[it] = cand.mass();
    y[it] = cand.rapidity();
    //flavor[it] = (cand.pdgId()!=0 ? cand.pdgId()/fabs(cand.pdgId()) : 0.);
    //flavor is useless

    //mva[it] = (useAnyMVA_ ? (*mvavalues)[it] : 0.0);

    //vtxChi2
    //vtxChi2[it] = cand.userFloat("vertexChi2");
    vtxChi2[it] = cand.userFloat("normChi2"); // yousen
    
    //ndf[it] = cand.userFloat("vertexNdof");
    //VtxProb[it] = TMath::Prob(vtxChi2[it],ndf[it]);
    VtxProb[it] = cand.userFloat("vertexProb"); // yousen

    //const double& secvz = cand.vz(), secvx = cand.vx(), secvy = cand.vy();
    //const double& px = cand.px(), py = cand.py(), pz = cand.pz();

    //PAngle
    /* // yousen
    const TVector3 ptosvec(secvx-bestvx, secvy-bestvy, secvz-bestvz);
    const TVector3 secvec(px, py, pz);
    const TVector3 ptosvec2D(secvx-bestvx, secvy-bestvy, 0);
    const TVector3 secvec2D(px,py,0);
    */

    /* // yousen
    //agl[it] = std::cos(secvec.Angle(ptosvec));
    //agl_abs[it] = secvec.Angle(ptosvec);
    //agl2D[it] = std::cos(secvec2D.Angle(ptosvec2D));
    //agl2D_abs[it] = secvec2D.Angle(ptosvec2D);
    */ 
    agl[it] = std::cos(cand.userFloat("angle3D"));
    agl_abs[it] = cand.userFloat("angle3D");
    agl2D[it] = std::cos(cand.userFloat("angle2D"));
    agl2D_abs[it] = cand.userFloat("angle2D");
        
    //Decay length 3D
    /* // yousen
    const SMatrixSym3D& trkCovMat = *cand.userData<reco::Vertex::CovarianceMatrix>("vertexCovariance");
    const SMatrixSym3D& totalCov = vtx.covariance() + trkCovMat;
    const SVector3 distanceVector(secvx-bestvx, secvy-bestvy, secvz-bestvz);
    const SVector3 pvec(px, py, pz);

    dl[it] = ROOT::Math::Mag(distanceVector);
    dlerror[it] = std::sqrt(ROOT::Math::Similarity(totalCov, distanceVector))/dl[it];
    dlerror2[it] = std::sqrt(ROOT::Math::Similarity(totalCov, pvec))/ROOT::Math::Mag(pvec); 
    dlos[it] = dl[it]/dlerror[it];
    */
    dl[it] = cand.userFloat("lVtxMag");
    dlos[it] = cand.userFloat("lVtxSig");
    dlerror[it] = dl[it]/dlos[it];
    // dlerror2[it]; // what is it for? // yousen

    //Decay length 2D
    /* // yousen
    const SVector6 v1(vtx.covariance(0,0), vtx.covariance(0,1), vtx.covariance(1,1), 0, 0, 0);
    const SVector6 v2(trkCovMat(0,0), trkCovMat(0,1), trkCovMat(1,1), 0, 0, 0);
    const SMatrixSym3D& totalCov2D = SMatrixSym3D(v1) + SMatrixSym3D(v2);
    const SVector3 distanceVector2D(secvx-bestvx, secvy-bestvy, 0);

    dl2D[it] = ROOT::Math::Mag(distanceVector2D);
    const double& dl2Derror = std::sqrt(ROOT::Math::Similarity(totalCov2D, distanceVector2D))/dl2D[it];
    dlos2D[it] = dl2D[it]/dl2Derror;
    */
    dl2D[it] = cand.userFloat("rVtxMag");
    dlos2D[it] = cand.userFloat("rVtxSig");

    const auto& dauColl = *cand.userData<pat::GenericParticleCollection>("daughters");
    //const ushort& nDau = cand.numberOfDaughters(); // yousen
    const auto& nDau = dauColl.size();
    if(nDau!=NDAU_) throw cms::Exception("PATGenericParticleAnalyzer") << "Expected " << NDAU_ << " daughters but V0 candidate has " << nDau << " daughters!" << std::endl;

    //Gen match
    // need to be done
    // yousen
    if(doGenMatching_)
    {
      isSwap[it] = false;
      matchGEN[it] = false;

      std::vector<const reco::Candidate*> finalParticles; // no matter it is dau or grand dau

      for(ushort iDau=0; iDau<nDau; iDau++)
      {
        if(dauColl[iDau].hasUserData("daughters")) {
          if(!twoLayerDecay_)
            throw cms::Exception("PATGenericParticleAnalyzer") << "Expected twoLayerDecay to be true, check the config" << std::endl;
          const auto& grandDauColl = *dauColl[iDau].userData<pat::GenericParticleCollection>("daughters");
          for(const auto& cand : grandDauColl) 
            finalParticles.push_back(&cand);
        } else
        {
          finalParticles.push_back(&dauColl[iDau]);
        }
      }

      edm::Handle<reco::GenParticleCollection> genpars;
      iEvent.getByToken(tok_genParticle_,genpars);
      if(!genpars.isValid())
      {
        cout<<"Gen matching cannot be done without Gen collection!!"<<endl;
        return;
      }
      auto id_tmp = std::labs(cand.pdgId());
      //std::cout << "id_tmp" << std::endl;
      std::vector<const reco::Candidate*> finalParticles_GEN;
      for(uint idx=0; idx<genpars->size(); ++idx)
      {
        const auto& genCand = reco::GenParticleRef(genpars, idx);
        finalParticles_GEN.clear();
        if( std::labs(genCand->pdgId()) != id_tmp) continue;
        //std::cout << "found a gen cand" << std::endl;
        if( !twoLayerDecay_ && genCand->numberOfDaughters() != nDau)
        {
          std::cout << "number of daughters in gen different from that in reco" << std::endl;
          continue;
        }
        if( !twoLayerDecay_ && genCand->numberOfDaughters() == nDau)
        {
          for(size_t iDau=0; iDau<nDau; iDau++){
            finalParticles_GEN.push_back( genCand->daughter(iDau) );
          }
        }
        if(twoLayerDecay_) 
        {
          for(size_t iDau=0; iDau<genCand->numberOfDaughters() ;++iDau)
          {
            const auto& dau_gen = *genCand->daughter(iDau);
            auto nGrandDau = dau_gen.numberOfDaughters();
            //std::cout << Form("dau_gen_%zu has %zu daughters", iDau, nGrandDau) << std::endl;
            if(nGrandDau>0)
            {
              for(size_t iGrandDau=0; iGrandDau<nGrandDau; iGrandDau++)
              {
                if(dau_gen.daughter(iGrandDau)->status()!=1) continue;
                finalParticles_GEN.push_back(dau_gen.daughter(iGrandDau));
              }
            } else {
              finalParticles_GEN.push_back(&dau_gen);
            }
          }
        }
        if(finalParticles_GEN.size()!=finalParticles.size()) continue;
        //std::cout << genCand->pdgId() << "has " << finalParticles_GEN.size()  << std::endl;
        const size_t nFinalParticles = finalParticles.size();
        std::vector<size_t> particlePermutations;
        for(size_t iDauGEN=0; iDauGEN<nFinalParticles; iDauGEN++)
        {
          particlePermutations.push_back(iDauGEN);
        }
        bool found = false;
        bool swap_tmp = false;
        do {
          std::vector<bool>  matchFianlParticlesP(nFinalParticles);
          std::vector<bool>  matchFianlParticlesM(nFinalParticles);
          //for(const auto e : finalParticles_GEN) std::cout << e << "\t";
          //std::cout << std::endl;
          for(size_t iDau=0; iDau<nFinalParticles; iDau++) {
            matchFianlParticlesP[iDau] = false;
            matchFianlParticlesM[iDau] = false;
            const auto& recoFinal = *finalParticles[iDau];
            const auto& genFinal = *finalParticles_GEN[particlePermutations[iDau]];
            if(recoFinal.charge() != genFinal.charge()) continue;
            //std::cout << "match charge" << std::endl;
            if( std::fabs(genFinal.mass() - recoFinal.mass()) < 0.001 ) matchFianlParticlesM[iDau] = true;
            //if(matchFianlParticlesM[iDau]) std::cout << "match m" << std::endl;
            double dPtRel = std::fabs(recoFinal.pt()-genFinal.pt())/sqrt(recoFinal.pt()*genFinal.pt());
            double dR = reco::deltaR(recoFinal.p4().Eta(), recoFinal.p4().Phi(), genFinal.p4().Eta(), genFinal.p4().Phi());
            //double dR = reco::deltaR(recoFinal.eta(), recoFinal.phi(), genFinal.eta(), genFinal.phi());
            if(dPtRel < deltaPt_ && dR < deltaR_) {
              matchFianlParticlesP[iDau] = true;
              //break;
            }
            //if(matchFianlParticlesP[iDau]) std::cout << "match P" << std::endl;
          }
          found = !(std::count(matchFianlParticlesP.begin(), matchFianlParticlesP.end(), false) > 0);
          swap_tmp = std::count(matchFianlParticlesM.begin(), matchFianlParticlesM.end(), false) > 0;
          if(found) break;
        } while ( std::next_permutation(particlePermutations.begin(), particlePermutations.end()) );

        matchGEN[it] = found;
        isSwap[it] = swap_tmp;
        idmom_reco[it] = (matchGEN[it] ? genCand->pdgId() : -77);
        genVec_.push_back(matchGEN[it] ? genCand : reco::GenParticleRef());
      }
    }

    for(ushort iDau=0; iDau<nDau; iDau++)
    {
      //const auto& dau = *(trk.daughter(iDau));
      const auto& dau = dauColl[iDau];

      ptDau[iDau][it] = dau.pt();
      pDau[iDau][it] = dau.p();
      etaDau[iDau][it] = dau.eta();
      phiDau[iDau][it] = dau.phi();
      chargeDau[iDau][it] = dau.charge();

      /* // yousen need to be done
      pid[iDau][it] = -77;
      if(doGenMatchingTOF_)
      {
        const auto& recDau = dynamic_cast<const pat::Muon*>(trk.daughter(iDau));
        const auto& genDau = (recDau ? recDau->genParticleRef() : reco::GenParticleRef());
        if(genDau.isNonnull()) { pid[iDau][it] = genDau->pdgId(); }
      }
      */

      //trk info
      if(!twoLayerDecay_)
      {
        //const auto& dtrk = dau.get<reco::TrackRef>();
        const auto& dtrk = dau.track(); // yousen

        //trk quality
        trkquality[iDau][it] = (dtrk.isNonnull() ? dtrk->quality(reco::TrackBase::highPurity) : false);

        //trk dEdx
        H2dedx[iDau][it] = -999.9;
        T4dedx[iDau][it] = -999.9; // not available, yousen
        if(dau.hasUserFloat("dEdx")) H2dedx[iDau][it] = dau.userFloat("dEdx");

        //track Chi2
        trkChi[iDau][it] = (dtrk.isNonnull() ? dtrk->normalizedChi2() : 99.);

        //track pT error
        ptErr[iDau][it] = (dtrk.isNonnull() ? dtrk->ptError() : -1.);

        //trkNHits
        nhit[iDau][it] = (dtrk.isNonnull() ? dtrk->numberOfValidHits() : -1);

        //DCA
        dzos[iDau][it] = 99.;
        dxyos[iDau][it] = 99.;
        if (dtrk.isNonnull()) {
          //dzos[iDau][it] = dau.userFloat("dzSig");
          //dxyos[iDau][it] = dau.userFloat("dxySig"); // yousen
          if(dau.hasUserFloat("dzSig")) dzos[iDau][it] = dau.userFloat("dzSig");
          if(dau.hasUserFloat("dxySig")) dxyos[iDau][it] = dau.userFloat("dxySig");
        }
      }
 
      if(doMuon_)
      {
        //const auto& muon = (dau.isMuon() ? *dynamic_cast<const pat::Muon*>(trk.daughter(iDau)) : pat::Muon());
        // yousen
        const auto& muon = ( std::labs(dau.pdgId()) == 13 ? *dau.userData<pat::Muon>("src") : pat::Muon());


        // following need to be verified by andre, yousen
        // Tight ID Muon POG Run 2
        //glbmuon[iDau][it] = (dau.isMuon() ? muon.isGlobalMuon() : false); // yousen
        //pfmuon[iDau][it]  = (dau.isMuon() ? muon.isPFMuon() : false); // yousen
        glbmuon[iDau][it] = ( std::labs(dau.pdgId()) == 13 ? muon.isGlobalMuon() : false); // yousen
        pfmuon[iDau][it]  = ( std::labs(dau.pdgId()) == 13 ? muon.isPFMuon() : false); // yousen
        
        glbtrkchi[iDau][it] = (muon.globalTrack().isNonnull() ? muon.globalTrack()->normalizedChi2() : 99.);
        nmuonhit[iDau][it] = (muon.globalTrack().isNonnull() ? muon.globalTrack()->hitPattern().numberOfValidMuonHits() : -1);
        //nmatchedst[iDau][it] = (dau.isMuon() ? muon.numberOfMatchedStations() : -1); // yousen
        nmatchedst[iDau][it] = ( std::labs(dau.pdgId()) == 13 ? muon.numberOfMatchedStations() : -1); // yousen
        npixelhit[iDau][it] = (muon.innerTrack().isNonnull() ? muon.innerTrack()->hitPattern().numberOfValidPixelHits() : -1);
        ntrackerlayer[iDau][it] = (muon.innerTrack().isNonnull() ? muon.innerTrack()->hitPattern().trackerLayersWithMeasurement() : -1);
        muonbestdxy[iDau][it] = (muon.muonBestTrack().isNonnull() ? muon.muonBestTrack()->dxy(bestvtx) : 99.);
        muonbestdz[iDau][it] = (muon.muonBestTrack().isNonnull() ? muon.muonBestTrack()->dz(bestvtx) : 99.);
        tightmuon[iDau][it] = (
                               glbmuon[iDau][it] &&
                               pfmuon[iDau][it] &&
                               (glbtrkchi[iDau][it] < 10.) &&
                               (nmuonhit[iDau][it] > 0) &&
                               (nmatchedst[iDau][it] > 1) &&
                               (npixelhit[iDau][it] > 0) &&
                               (ntrackerlayer[iDau][it] > 5) &&
                               (fabs(muonbestdxy[iDau][it]) < 0.2) &&
                               (fabs(muonbestdz[iDau][it]) < 0.5)
                               );

        // Soft ID Muon POG Run 2
        //onestmuon[iDau][it] = (dau.isMuon() ? muon::isGoodMuon(muon, muon::SelectionType::TMOneStationTight) : false); // yousen
        onestmuon[iDau][it] = ( std::labs(dau.pdgId()) == 13 ? muon::isGoodMuon(muon, muon::SelectionType::TMOneStationTight) : false); // yousen
        npixellayer[iDau][it] = (muon.innerTrack().isNonnull() ? muon.innerTrack()->hitPattern().pixelLayersWithMeasurement() : -1);
        hpmuon[iDau][it] = (muon.innerTrack().isNonnull() ? muon.innerTrack()->quality(reco::TrackBase::highPurity) : false);
        muondxy[iDau][it] = (muon.innerTrack().isNonnull() ? muon.innerTrack()->dxy(bestvtx) : 99.);
        muondz[iDau][it] = (muon.innerTrack().isNonnull() ? muon.innerTrack()->dz(bestvtx) : 99.);
        softmuon[iDau][it] = (
                              onestmuon[iDau][it] &&
                              (ntrackerlayer[iDau][it] > 5) &&
                              (npixellayer[iDau][it] > 0) &&
                              hpmuon[iDau][it] &&
                              (fabs(muondxy[iDau][it]) < 0.3) &&
                              (fabs(muondz[iDau][it]) < 20.)
                              );

        // Hybrid Soft ID HIN PAG Run 2 PbPb
        //trkmuon[iDau][it] = (dau.isMuon() ? muon.isTrackerMuon() : false); // yousen
        trkmuon[iDau][it] = ( std::labs(dau.pdgId()) == 13  ? muon.isTrackerMuon() : false); // yousen
        hybridmuon[iDau][it] = (
                                glbmuon[iDau][it] &&
                                (ntrackerlayer[iDau][it] > 5) &&
                                (npixellayer[iDau][it] > 0) &&
                                (fabs(muondxy[iDau][it]) < 0.3) &&
                                (fabs(muondz[iDau][it]) < 20.)
                                );

        // Muon Trigger Matching
        if(it==0)
        {
          trgmuon[iDau].clear();
          trgmuon[iDau] = std::vector<std::vector<UChar_t>>(filterNames_.size(), std::vector<UChar_t>(candSize, 0));
        }
        //if(dau.isMuon()) // yousen
        if( std::labs(dau.pdgId() == 13) ) // yousen
        {
          for(ushort iTr=0; iTr<filterNames_.size(); iTr++)
          {
            const auto& muHLTMatchesFilter = muon.triggerObjectMatchesByFilter(filterNames_.at(iTr));
            if(muHLTMatchesFilter.size()>0) trgmuon[iDau][iTr][it] = 1;
          }
        }

        if(doMuonFull_)
        {
          //nmatchedch[iDau][it] = (dau.isMuon() ? muon.numberOfMatches() : -1); // yousen
          //matchedenergy[iDau][it] = (dau.isMuon() ? muon.calEnergy().hadMax : -99.); // yousen
          nmatchedch[iDau][it] = ( std::labs(dau.pdgId() == 13) ? muon.numberOfMatches() : -1);
          matchedenergy[iDau][it] = ( std::labs(dau.pdgId() == 13) ? muon.calEnergy().hadMax : -99.);

          dx_seg[iDau][it] = 999.9;
          dy_seg[iDau][it] = 999.9;
          dxSig_seg[iDau][it] = 999.9;
          dySig_seg[iDau][it] = 999.9;
          ddxdz_seg[iDau][it] = 999.9;
          ddydz_seg[iDau][it] = 999.9;
          ddxdzSig_seg[iDau][it] = 999.9;
          ddydzSig_seg[iDau][it] = 999.9;
          const std::vector<reco::MuonChamberMatch>& muchmatches = muon.matches();
          for(ushort ich=0; ich<muchmatches.size(); ich++)
          {
            const double& x_exp = muchmatches[ich].x;
            const double& y_exp = muchmatches[ich].y;
            const double& xerr_exp = muchmatches[ich].xErr;
            const double& yerr_exp = muchmatches[ich].yErr;
            const double& dxdz_exp = muchmatches[ich].dXdZ;
            const double& dydz_exp = muchmatches[ich].dYdZ;
            const double& dxdzerr_exp = muchmatches[ich].dXdZErr;
            const double& dydzerr_exp = muchmatches[ich].dYdZErr;

            const std::vector<reco::MuonSegmentMatch>& musegmatches = muchmatches[ich].segmentMatches;
            for(ushort jseg=0; jseg<musegmatches.size(); jseg++)
            {
              const double& x_seg = musegmatches[jseg].x;
              const double& y_seg = musegmatches[jseg].y;
              const double& xerr_seg = musegmatches[jseg].xErr;
              const double& yerr_seg = musegmatches[jseg].yErr;
              const double& dxdz_seg = musegmatches[jseg].dXdZ;
              const double& dydz_seg = musegmatches[jseg].dYdZ;
              const double& dxdzerr_seg = musegmatches[jseg].dXdZErr;
              const double& dydzerr_seg = musegmatches[jseg].dYdZErr;

              const double& dseg = std::sqrt((x_seg-x_exp)*(x_seg-x_exp) + (y_seg-y_exp)*(y_seg-y_exp));
              const double& dxerr_seg = std::sqrt(xerr_seg*xerr_seg + xerr_exp*xerr_exp);
              const double& dyerr_seg = std::sqrt(yerr_seg*yerr_seg + yerr_exp*yerr_exp);
              const double& ddxdzerr_seg = std::sqrt(dxdzerr_seg*dxdzerr_seg + dxdzerr_exp*dxdzerr_exp);
              const double& ddydzerr_seg = std::sqrt(dydzerr_seg*dydzerr_seg + dydzerr_exp*dydzerr_exp);

              if(dseg < std::sqrt(dx_seg[iDau][it]*dx_seg[iDau][it] + dy_seg[iDau][it]*dy_seg[iDau][it]))
              {
                dx_seg[iDau][it] = x_seg - x_exp;
                dy_seg[iDau][it] = y_seg - y_exp;
                dxSig_seg[iDau][it] = dx_seg[iDau][it] / dxerr_seg;
                dySig_seg[iDau][it] = dy_seg[iDau][it] / dyerr_seg;
                ddxdz_seg[iDau][it] = dxdz_seg - dxdz_exp;
                ddydz_seg[iDau][it] = dydz_seg - dydz_exp;
                ddxdzSig_seg[iDau][it] = ddxdz_seg[iDau][it] / ddxdzerr_seg;
                ddydzSig_seg[iDau][it] = ddydz_seg[iDau][it] / ddydzerr_seg;
              }
            }
          }
        }
      }
    }
 
    if(twoLayerDecay_)
    {
      // yousen
      /*
      const auto& d = *(trk.daughter(0));
      grand_mass[it] = d.mass();
      */

      // yousen -- begin
      for(ushort iDau=0; iDau<NDAU_; iDau++)
      {
        const auto& d = dauColl[iDau];
        if(!d.hasUserData("daughters")) {
          //std::cout << d.pdgId() << "has no daughters" << std::endl;
          continue;
        }
        grand_mass[iDau][it] = d.mass();
        const auto& daughters = *d.userData<pat::GenericParticleCollection>("daughters");
        for(ushort iGDau=0; iGDau<NGDAU_; iGDau++)
        {
          const auto& gd = daughters.at(iGDau);
          const auto& gdau = gd.track();
  
          //trk quality
          grand_trkquality[iGDau][iDau][it] = (gdau.isNonnull() ? gdau->quality(reco::TrackBase::highPurity) : false);
  
          //trk dEdx
          grand_H2dedx[iGDau][iDau][it] = -999.9;
          grand_T4dedx[iGDau][iDau][it] = -999.9;
          if(gd.hasUserFloat("dEdx")) grand_H2dedx[iGDau][iDau][it] = gd.userFloat("dEdx");
  
          //track pt
          grand_pt[iGDau][iDau][it] = gd.pt();
  
          //track momentum
          grand_p[iGDau][iDau][it] = gd.p(); // yousen
  
          //track eta
          grand_eta[iGDau][iDau][it] = gd.eta(); // yousen
  
          //track charge
          grand_charge[iGDau][iDau][it] = gd.charge(); // yousen
  
          //track Chi2
          grand_trkChi[iGDau][iDau][it] = (gdau.isNonnull() ? gdau->normalizedChi2() : 99.); // yousen
  
          //track pT error
          grand_ptErr[iGDau][iDau][it] = (gdau.isNonnull() ? gdau->ptError() : -1.); // yousen
  
          //trkNHits
          grand_nhit[iGDau][iDau][it] = (gdau.isNonnull() ? gdau->numberOfValidHits() : -1); // yousen
  
          //DCA
          grand_dzos[iGDau][iDau][it] = (d.hasUserFloat("dzSig") ? d.userFloat("dzSig") : 99.);
          grand_dxyos[iGDau][iDau][it] = (d.hasUserFloat("dxySig") ? d.userFloat("dxySig") : 99.);
        }
     
        //vtxChi2
        grand_vtxChi2[iDau][it] = d.userFloat("normChi2");
        grand_VtxProb[iDau][it] = d.userFloat("vertexProb");
  
        //PAngle
        grand_agl[iDau][it] = std::cos(d.userFloat("angle3D"));
        grand_agl_abs[iDau][it] = d.userFloat("angle3D");
        grand_agl2D[iDau][it] = std::cos(d.userFloat("angle2D"));
        grand_agl2D_abs[iDau][it] = d.userFloat("angle2D");
  
        //Decay length 3D
        grand_dl[iDau][it] = d.userFloat("lVtxMag");
        grand_dlos[iDau][it] = d.userFloat("lVtxSig");
  
        //Decay length 2D
        grand_dl2D[iDau][it] = d.userFloat("rVtxMag");
        grand_dlos2D[iDau][it] = d.userFloat("rVtxSig");
      }
      // yousen -- end
      // yousen
      /*
      for(ushort iGDau=0; iGDau<NGDAU_; iGDau++)
      {
        if(!d.daughter(iGDau)) continue;
        const auto& gd = *(d.daughter(iGDau));
        const auto& gdau = gd.get<reco::TrackRef>();

        //trk quality
        grand_trkquality[iGDau][it] = (gdau.isNonnull() ? gdau->quality(reco::TrackBase::highPurity) : false);

        //trk dEdx
        grand_H2dedx[iGDau][it] = -999.9;
        if(gdau.isNonnull() && dEdxHandle1.isValid())
        {
          const edm::ValueMap<reco::DeDxData>& dEdxTrack = *dEdxHandle1.product();
          grand_H2dedx[iGDau][it] = dEdxTrack[gdau].dEdx();
        }   
        grand_T4dedx[iGDau][it] = -999.9;
        if(gdau.isNonnull() && dEdxHandle2.isValid())
        {
          const edm::ValueMap<reco::DeDxData>& dEdxTrack = *dEdxHandle2.product();
          grand_T4dedx[iGDau][it] = dEdxTrack[gdau].dEdx();
        }

        //track pt
        grand_pt[iGDau][it] = gd.pt();

        //track momentum
        grand_p[iGDau][it] = gd.p();

        //track eta
        grand_eta[iGDau][it] = gd.eta();

        //track charge
        grand_charge[iGDau][it] = gd.charge();

        //track Chi2
        grand_trkChi[iGDau][it] = (gdau.isNonnull() ? gdau->normalizedChi2() : 99.);

        //track pT error
        grand_ptErr[iGDau][it] = (gdau.isNonnull() ? gdau->ptError() : -1.);

        //trkNHits
        grand_nhit[iGDau][it] = (gdau.isNonnull() ? gdau->numberOfValidHits() : -1);

        //DCA
        grand_dzos[iGDau][it] = 99.;
        grand_dxyos[iGDau][it] = 99.;
        if(gdau.isNonnull())
        {
          const double& gdzbest = gdau->dz(bestvtx);
          const double& gdxybest = gdau->dxy(bestvtx);
          const double& gdzerror = std::sqrt(gdau->dzError()*gdau->dzError() + bestvzError*bestvzError);
          const double& gdxyerror = std::sqrt(gdau->d0Error()*gdau->d0Error() + bestvxError*bestvyError);
          grand_dzos[iGDau][it] = gdzbest/gdzerror;
          grand_dxyos[iGDau][it] = gdxybest/gdxyerror;
        }
      }
   
      //vtxChi2
      grand_vtxChi2[it] = d.vertexChi2();
      grand_ndf[it] = d.vertexNdof();
      grand_VtxProb[it] = TMath::Prob(grand_vtxChi2[it], grand_ndf[it]);

      //PAngle
      const double& secvz = d.vz(), secvx = d.vx(), secvy = d.vy();
      const TVector3 ptosvec(secvx-bestvx, secvy-bestvy, secvz-bestvz);
      const TVector3 secvec(d.px(), d.py(), d.pz());            
      const TVector3 ptosvec2D(secvx-bestvx, secvy-bestvy, 0);
      const TVector3 secvec2D(d.px(), d.py(), 0);

      grand_agl[it] = std::cos(secvec.Angle(ptosvec));
      grand_agl_abs[it] = secvec.Angle(ptosvec);
      grand_agl2D[it] = std::cos(secvec2D.Angle(ptosvec2D));
      grand_agl2D_abs[it] = secvec2D.Angle(ptosvec2D);

      //Decay length 3D
      const SMatrixSym3D& totalCov = vtx.covariance() + d.vertexCovariance();
      const SVector3 distanceVector(secvx-bestvx, secvy-bestvy, secvz-bestvz);

      grand_dl[it] = ROOT::Math::Mag(distanceVector);
      grand_dlerror[it] = std::sqrt(ROOT::Math::Similarity(totalCov, distanceVector))/grand_dl[it];
      grand_dlos[it] = grand_dl[it]/grand_dlerror[it];

      //Decay length 2D
      const SVector6 v1(vtx.covariance(0,0), vtx.covariance(0,1), vtx.covariance(1,1), 0, 0, 0);
      const SVector6 v2(d.vertexCovariance(0,0), d.vertexCovariance(0,1), d.vertexCovariance(1,1), 0, 0, 0);
      const SMatrixSym3D totalCov2D = SMatrixSym3D(v1) + SMatrixSym3D(v2);
      const SVector3 distanceVector2D(secvx-bestvx, secvy-bestvy, 0);

      const double& gdl2D = ROOT::Math::Mag(distanceVector2D);
      const double& gdl2Derror = std::sqrt(ROOT::Math::Similarity(totalCov2D, distanceVector2D))/gdl2D;
      grand_dlos2D[it] = gdl2D/gdl2Derror;
      */
    }

    /*
    if(saveHistogram_)
    {
      for(unsigned int ipt=0;ipt<pTBins_.size()-1;ipt++)
      {
        for(unsigned int iy=0;iy<yBins_.size()-1;iy++)
        {
          if(pt[it]<pTBins_[ipt+1] && pt[it]>pTBins_[ipt] && y[it]<yBins_[iy+1] && y[it]>yBins_[iy])
          {
            hMassVsMVA[iy][ipt]->Fill(mva[it], mass[it]);

            if(saveAllHistogram_)
            {
              hpTVsMVA[iy][ipt]->Fill(mva[it], pt[it]);
              hetaVsMVA[iy][ipt]->Fill(mva[it], eta[it]);
              hyVsMVA[iy][ipt]->Fill(mva[it], y[it]);
              hVtxProbVsMVA[iy][ipt]->Fill(mva[it], VtxProb[it]);
              h3DCosPointingAngleVsMVA[iy][ipt]->Fill(mva[it], agl[it]);
              h3DPointingAngleVsMVA[iy][ipt]->Fill(mva[it], agl_abs[it]);
              h2DCosPointingAngleVsMVA[iy][ipt]->Fill(mva[it], agl2D[it]);
              h2DPointingAngleVsMVA[iy][ipt]->Fill(mva[it], agl2D_abs[it]);
              h3DDecayLengthSignificanceVsMVA[iy][ipt]->Fill(mva[it], dlos[it]);
              h3DDecayLengthVsMVA[iy][ipt]->Fill(mva[it], dl[it]);
              h2DDecayLengthSignificanceVsMVA[iy][ipt]->Fill(mva[it], dlos2D[it]);
              h2DDecayLengthVsMVA[iy][ipt]->Fill(mva[it], dl2D[it]);
              for (ushort iDau=0; iDau<NDAU_; iDau++)
              {
                hzDCASignificanceDaughterVsMVA[iDau][iy][ipt]->Fill(mva[it], dzos[iDau][it]);
                hxyDCASignificanceDaughterVsMVA[iDau][iy][ipt]->Fill(mva[it], dxyos[iDau][it]);
                hNHitDVsMVA[iDau][iy][ipt]->Fill(mva[it], nhit[iDau][it]);
                hpTDVsMVA[iDau][iy][ipt]->Fill(mva[it], ptDau[iDau][it]);
                hpTerrDVsMVA[iDau][iy][ipt]->Fill(mva[it], ptErr[iDau][it]/ptDau[iDau][it]);
                hEtaDVsMVA[iDau][iy][ipt]->Fill(mva[it], etaDau[iDau][it]);
                hdedxHarmonic2DVsMVA[iDau][iy][ipt]->Fill(mva[it], H2dedx[iDau][it]);
                hdedxHarmonic2DVsP[iDau][iy][ipt]->Fill(pDau[iDau][it], H2dedx[iDau][it]);
              }
            }
          }
        }
      }
    }
    */
  }
}


void
PATGenericParticleTreeProducer::fillGEN(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  edm::Handle<GenEventInfoProduct> geninfo;
  iEvent.getByToken(tok_genInfo_, geninfo);
  weight_gen = (geninfo.isValid() ? geninfo->weight() : -1.0);

  edm::Handle<reco::GenParticleCollection> genpars;
  iEvent.getByToken(tok_genParticle_, genpars);
  if(!genpars.isValid()) throw cms::Exception("PATGenericParticleAnalyzer") << "Gen matching cannot be done without Gen collection!" << std::endl;

  candSize_gen = 0;
  for(uint idx=0; idx<genpars->size(); ++idx)
  {
    const auto& trk = reco::GenParticleRef(genpars, idx);

    if (trk.isNull()) continue; //check gen particle ref
    if(!hasDaughters(trk, PID_dau_)) continue; //check if has the daughters

    pt_gen[candSize_gen] = trk->pt();
    eta_gen[candSize_gen] = trk->eta();
    y_gen[candSize_gen] = trk->rapidity();
    pid_gen[candSize_gen] = trk->pdgId();
    status_gen[candSize_gen] = trk->status();

    const auto& recIt = std::find(genVec_.begin(), genVec_.end(), trk);
    idxrec_gen[candSize_gen] = (recIt!=genVec_.end() ? std::distance(genVec_.begin(), recIt) : -1);

    const auto& mom = findMother(trk);
    idmom_gen[candSize_gen] = (mom.isNonnull() ? mom->pdgId() : -77);

    genDecayLength(candSize_gen, genpars->at(idx));

    if(decayInGen_)
    {
      for(ushort iDau=0; iDau<NDAU_; iDau++)
      {
        const auto& Dd = findLastPar(trk->daughterRef(iDau));
        idDau_gen[iDau][candSize_gen] = (Dd.isNonnull() ? Dd->pdgId() : 99999);
        chargeDau_gen[iDau][candSize_gen] = (Dd.isNonnull() ? Dd->charge() : 9);
        ptDau_gen[iDau][candSize_gen] = (Dd.isNonnull() ? Dd->pt() : -1.);
        etaDau_gen[iDau][candSize_gen] = (Dd.isNonnull() ? Dd->eta() : 9.);
        phiDau_gen[iDau][candSize_gen] = (Dd.isNonnull() ? Dd->phi() : 9.);
      }
    }

    candSize_gen++;
  }
}


// ------------ method called once each job just before starting event
//loop  ------------
void
PATGenericParticleTreeProducer::beginJob()
{
  TH1D::SetDefaultSumw2();

  // Check inputs
  if((!threeProngDecay_ && NDAU_!=2) || (threeProngDecay_ && NDAU_!=3))
  {
    throw cms::Exception("PATGenericParticleAnalyzer") << "Want threeProngDecay but PID daughter vector size is: " << NDAU_ << " !" << std::endl;
  }
  if(!doRecoNtuple_ && !doGenNtuple_) throw cms::Exception("PATGenericParticleAnalyzer") << "No output for either RECO or GEN!! Fix config!!" << std::endl;
  if(twoLayerDecay_ && doMuon_) throw cms::Exception("PATGenericParticleAnalyzer") << "Muons cannot be coming from two layer decay!! Fix config!!" << std::endl;

    

  //if(saveHistogram_) initHistogram();
  if(saveTree_) initTree();
}


/*
void
PATGenericParticleTreeProducer::initHistogram()
{
  for(unsigned int ipt=0;ipt<pTBins_.size()-1;ipt++)
  {
    for(unsigned int iy=0;iy<yBins_.size()-1;iy++)
    {
      hMassVsMVA[iy][ipt] = fs->make<TH2F>(Form("hMassVsMVA_y%d_pt%d",iy,ipt),";mva;mass(GeV)",100,-1.,1.,massHistBins_,massHistPeak_-massHistWidth_,massHistPeak_+massHistWidth_);
      if(saveAllHistogram_)
      {
        hpTVsMVA[iy][ipt] = fs->make<TH2F>(Form("hpTVsMVA_y%d_pt%d",iy,ipt),";mva;pT;",100,-1,1,100,0,10);
        hetaVsMVA[iy][ipt] = fs->make<TH2F>(Form("hetaVsMVA_y%d_pt%d",iy,ipt),";mva;eta;",100,-1.,1.,40,-4,4);
        hyVsMVA[iy][ipt] = fs->make<TH2F>(Form("hyVsMVA_y%d_pt%d",iy,ipt),";mva;y;",100,-1.,1.,40,-4,4);
        hVtxProbVsMVA[iy][ipt] = fs->make<TH2F>(Form("hVtxProbVsMVA_y%d_pt%d",iy,ipt),";mva;VtxProb;",100,-1.,1.,100,0,1);
        h3DCosPointingAngleVsMVA[iy][ipt] = fs->make<TH2F>(Form("h3DCosPointingAngleVsMVA_y%d_pt%d",iy,ipt),";mva;3DCosPointingAngle;",100,-1.,1.,100,-1,1);
        h3DPointingAngleVsMVA[iy][ipt] = fs->make<TH2F>(Form("h3DPointingAngleVsMVA_y%d_pt%d",iy,ipt),";mva;3DPointingAngle;",100,-1.,1.,50,-3.14,3.14);
        h2DCosPointingAngleVsMVA[iy][ipt] = fs->make<TH2F>(Form("h2DCosPointingAngleVsMVA_y%d_pt%d",iy,ipt),";mva;2DCosPointingAngle;",100,-1.,1.,100,-1,1);
        h2DPointingAngleVsMVA[iy][ipt] = fs->make<TH2F>(Form("h2DPointingAngleVsMVA_y%d_pt%d",iy,ipt),";mva;2DPointingAngle;",100,-1.,1.,50,-3.14,3.14);
        h3DDecayLengthSignificanceVsMVA[iy][ipt] = fs->make<TH2F>(Form("h3DDecayLengthSignificanceVsMVA_y%d_pt%d",iy,ipt),";mva;3DDecayLengthSignificance;",100,-1.,1.,300,0,30);
        h2DDecayLengthSignificanceVsMVA[iy][ipt] = fs->make<TH2F>(Form("h2DDecayLengthSignificanceVsMVA_y%d_pt%d",iy,ipt),";mva;2DDecayLengthSignificance;",100,-1.,1.,300,0,30);
        h3DDecayLengthVsMVA[iy][ipt] = fs->make<TH2F>(Form("h3DDecayLengthVsMVA_y%d_pt%d",iy,ipt),";mva;3DDecayLength;",100,-1.,1.,300,0,30);
        h2DDecayLengthVsMVA[iy][ipt] = fs->make<TH2F>(Form("h2DDecayLengthVsMVA_y%d_pt%d",iy,ipt),";mva;2DDecayLength;",100,-1.,1.,300,0,30);
        for(ushort d=1; d<=NDAU_; d++)
        {
          hzDCASignificanceDaughterVsMVA[d-1][iy][ipt] = fs->make<TH2F>(Form("hzDCASignificanceDaughter%dVsMVA_y%d_pt%d",d,iy,ipt),Form(";mva;zDCASignificanceDaughter%d;",d),100,-1.,1.,100,-10,10);
          hxyDCASignificanceDaughterVsMVA[d-1][iy][ipt] = fs->make<TH2F>(Form("hxyDCASignificanceDaughter%dVsMVA_y%d_pt%d",d,iy,ipt),Form(";mva;xyDCASignificanceDaughter%d;",d),100,-1.,1.,100,-10,10);
          hNHitDVsMVA[d-1][iy][ipt] = fs->make<TH2F>(Form("hNHitD%dVsMVA_y%d_pt%d",d,iy,ipt),Form(";mva;NHitD%d;",d),100,-1.,1.,100,0,100);
          hpTDVsMVA[d-1][iy][ipt] = fs->make<TH2F>(Form("hpTD%dVsMVA_y%d_pt%d",d,iy,ipt),Form(";mva;pTD%d;",d),100,-1.,1.,100,0,10);
          hpTerrDVsMVA[d-1][iy][ipt] = fs->make<TH2F>(Form("hpTerrD%dVsMVA_y%d_pt%d",d,iy,ipt),Form(";mva;pTerrD%d;",d),100,-1.,1.,50,0,0.5);
          hEtaDVsMVA[d-1][iy][ipt] = fs->make<TH2F>(Form("hEtaD%dVsMVA_y%d_pt%d",d,iy,ipt),Form(";mva;EtaD%d;",d),100,-1.,1.,40,-4,4);
          if(useDeDxData_)
          {
            hdedxHarmonic2DVsMVA[d-1][iy][ipt] = fs->make<TH2F>(Form("hdedxHarmonic2D%dVsMVA_y%d_pt%d",d,iy,ipt),Form(";mva;dedxHarmonic2D%d;",d),100,-1.,1.,100,0,10);
            hdedxHarmonic2DVsP[d-1][iy][ipt] = fs->make<TH2F>(Form("hdedxHarmonic2D%dVsP_y%d_pt%d",d,iy,ipt),Form(";p (GeV);dedxHarmonic2D%d",d),100,0,10,100,0,10);
          }
        }
      }
    }
  }
}
*/


void 
PATGenericParticleTreeProducer::initTree()
{ 
  PATGenericParticleNtuple = fs->make< TTree>("VertexCompositeNtuple","VertexCompositeNtuple");

  if(doRecoNtuple_)
  {
    // Event info
    
    PATGenericParticleNtuple->Branch("RunNb",&runNb,"RunNb/i");
    PATGenericParticleNtuple->Branch("LSNb",&lsNb,"LSNb/i");
    PATGenericParticleNtuple->Branch("EventNb",&eventNb,"EventNb/i");
    PATGenericParticleNtuple->Branch("nPV",&nPV,"nPV/S");
    PATGenericParticleNtuple->Branch("bestvtxX",&bestvx,"bestvtxX/F");
    PATGenericParticleNtuple->Branch("bestvtxY",&bestvy,"bestvtxY/F");
    PATGenericParticleNtuple->Branch("bestvtxZ",&bestvz,"bestvtxZ/F");
    PATGenericParticleNtuple->Branch("candSize",&candSize,"candSize/i");
    if(isCentrality_) 
    {
      PATGenericParticleNtuple->Branch("centrality",&centrality,"centrality/S");
      PATGenericParticleNtuple->Branch("Npixel",&Npixel,"Npixel/I");
      PATGenericParticleNtuple->Branch("HFsumETPlus",&HFsumETPlus,"HFsumETPlus/F");
      PATGenericParticleNtuple->Branch("HFsumETMinus",&HFsumETMinus,"HFsumETMinus/F");
      PATGenericParticleNtuple->Branch("ZDCPlus",&ZDCPlus,"ZDCPlus/F");
      PATGenericParticleNtuple->Branch("ZDCMinus",&ZDCMinus,"ZDCMinus/F");
      PATGenericParticleNtuple->Branch("Ntrkoffline",&Ntrkoffline,"Ntrkoffline/I");
    }
    if(isEventPlane_) {
      PATGenericParticleNtuple->Branch("ephfpAngle",ephfpAngle,"ephfpAngle[3]/F");
      PATGenericParticleNtuple->Branch("ephfmAngle",ephfmAngle,"ephfmAngle[3]/F");
      PATGenericParticleNtuple->Branch("eptrackmidAngle",eptrackmidAngle,"eptrackmidAngle[3]/F");
      PATGenericParticleNtuple->Branch("ephfpQ",ephfpQ,"ephfpQ[3]/F");
      PATGenericParticleNtuple->Branch("ephfmQ",ephfmQ,"ephfmQ[3]/F");
      PATGenericParticleNtuple->Branch("eptrackmidQ",eptrackmidQ,"eptrackmidQ[3]/F");
      PATGenericParticleNtuple->Branch("ephfpSumW",&ephfpSumW,"ephfpSumW/F");
      PATGenericParticleNtuple->Branch("ephfmSumW",&ephfmSumW,"ephfmSumW/F");
      PATGenericParticleNtuple->Branch("eptrackmidSumW",&eptrackmidSumW,"eptrackmidSumW/F");
    }
    PATGenericParticleNtuple->Branch("trigPrescale",trigPrescale,Form("trigPrescale[%d]/S",NTRG_));
    PATGenericParticleNtuple->Branch("trigHLT",trigHLT,Form("trigHLT[%d]/O",NTRG_));
    PATGenericParticleNtuple->Branch("evtSel",evtSel,Form("evtSel[%d]/O",NSEL_));

    // particle info
    PATGenericParticleNtuple->Branch("pT",pt,"pT[candSize]/F");
    PATGenericParticleNtuple->Branch("eta",eta,"eta[candSize]/F");
    PATGenericParticleNtuple->Branch("phi",phi,"phi[candSize]/F");
    PATGenericParticleNtuple->Branch("mass",mass,"mass[candSize]/F");
    PATGenericParticleNtuple->Branch("y",y,"y[candSize]/F");
    //if(useAnyMVA_) PATGenericParticleNtuple->Branch("mva",mva,"mva[candSize]/F");

    if(!isSkimMVA_)
    {
      //Composite candidate info RECO
      PATGenericParticleNtuple->Branch("flavor",flavor,"flavor[candSize]/F");
      PATGenericParticleNtuple->Branch("VtxProb",VtxProb,"VtxProb[candSize]/F");
      PATGenericParticleNtuple->Branch("3DCosPointingAngle",agl,"3DCosPointingAngle[candSize]/F");
      PATGenericParticleNtuple->Branch("3DPointingAngle",agl_abs,"3DPointingAngle[candSize]/F");
      PATGenericParticleNtuple->Branch("2DCosPointingAngle",agl2D,"2DCosPointingAngle[candSize]/F");
      PATGenericParticleNtuple->Branch("2DPointingAngle",agl2D_abs,"2DPointingAngle[candSize]/F");
      PATGenericParticleNtuple->Branch("3DDecayLengthSignificance",dlos,"3DDecayLengthSignificance[candSize]/F");
      PATGenericParticleNtuple->Branch("3DDecayLength",dl,"3DDecayLength[candSize]/F");
      PATGenericParticleNtuple->Branch("3DDecayLengthError",dlerror,"3DDecayLengthError[candSize]/F");
      PATGenericParticleNtuple->Branch("3DDecayLengthError2",dlerror2,"3DDecayLengthError2[candSize]/F");
      PATGenericParticleNtuple->Branch("2DDecayLengthSignificance",dlos2D,"2DDecayLengthSignificance[candSize]/F");
      PATGenericParticleNtuple->Branch("2DDecayLength",dl2D,"2DDecayLength[candSize]/F");

      if(doGenMatching_)
      {
        PATGenericParticleNtuple->Branch("isSwap",isSwap,"isSwap[candSize]/O");
        PATGenericParticleNtuple->Branch("idmom_reco",idmom_reco,"idmom_reco[candSize]/I");
        PATGenericParticleNtuple->Branch("matchGEN",matchGEN,"matchGEN[candSize]/O");
      }
 
      if(doGenMatchingTOF_)
      {
        for(ushort iDau=1; iDau<=NDAU_; iDau++)
        {
          PATGenericParticleNtuple->Branch(Form("PIDD%d",iDau),pid[iDau-1],Form("PIDD%d[candSize]/I",iDau));
        }
      }

      //daughter & grand daughter info
      if(twoLayerDecay_)
      {
        // yousen
        /*
        PATGenericParticleNtuple->Branch("massDaughter1",grand_mass,"massDaughter1[candSize]/F");
        PATGenericParticleNtuple->Branch("VtxProbDaughter1",grand_VtxProb,"VtxProbDaughter1[candSize]/F");
        PATGenericParticleNtuple->Branch("3DCosPointingAngleDaughter1",grand_agl,"3DCosPointingAngleDaughter1[candSize]/F");
        PATGenericParticleNtuple->Branch("3DPointingAngleDaughter1",grand_agl_abs,"3DPointingAngleDaughter1[candSize]/F");
        PATGenericParticleNtuple->Branch("2DCosPointingAngleDaughter1",grand_agl2D,"2DCosPointingAngleDaughter1[candSize]/F");
        PATGenericParticleNtuple->Branch("2DPointingAngleDaughter1",grand_agl2D_abs,"2DPointingAngleDaughter1[candSize]/F");
        PATGenericParticleNtuple->Branch("3DDecayLengthSignificanceDaughter1",grand_dlos,"3DDecayLengthSignificanceDaughter1[candSize]/F");
        PATGenericParticleNtuple->Branch("3DDecayLengthDaughter1",grand_dl,"3DDecayLengthDaughter1[candSize]/F");
        //PATGenericParticleNtuple->Branch("3DDecayLengthErrorDaughter1",grand_dlerror,"3DDecayLengthErrorDaughter1[candSize]/F"); // yousen
        PATGenericParticleNtuple->Branch("2DDecayLengthSignificanceDaughter1",grand_dlos2D,"2DDecayLengthSignificanceDaughter1[candSize]/F");
        PATGenericParticleNtuple->Branch("2DDecayLengthSignificanceDaughter1",grand_dlos2D,"2DDecayLengthSignificanceDaughter1[candSize]/F");
        for(ushort iGDau=1; iGDau<=NGDAU_; iGDau++)
        {
          PATGenericParticleNtuple->Branch(Form("zDCASignificanceGrandDaughter%d",iGDau),grand_dzos[iGDau-1],Form("zDCASignificanceGrandDaughter%d[candSize]/F",iGDau));
          PATGenericParticleNtuple->Branch(Form("xyDCASignificanceGrandDaughter%d",iGDau),grand_dxyos[iGDau-1],Form("xyDCASignificanceGrandDaughter%d[candSize]/F",iGDau));
          PATGenericParticleNtuple->Branch(Form("NHitGrandD%d",iGDau),grand_nhit[iGDau-1],Form("NHitGrandD%d[candSize]/F",iGDau));
          PATGenericParticleNtuple->Branch(Form("HighPurityGrandDaughter%d",iGDau),grand_trkquality[iGDau-1],Form("HighPurityGrandDaughter%d[candSize]/O",iGDau));
          PATGenericParticleNtuple->Branch(Form("pTGrandD%d",iGDau),grand_pt[iGDau-1],Form("pTGrandD%d[candSize]/F",iGDau));
          PATGenericParticleNtuple->Branch(Form("pTerrGrandD%d",iGDau),grand_ptErr[iGDau-1],Form("pTerrGrandD%d[candSize]/F",iGDau));
          PATGenericParticleNtuple->Branch(Form("EtaGrandD%d",iGDau),grand_eta[iGDau-1],Form("EtaGrandD%d[candSize]/F",iGDau));
          if(useDeDxData_)
          {
            //PATGenericParticleNtuple->Branch(Form("dedxPixelHarmonic2GrandD%d",iGDau),grand_T4dedx[iGDau-1],Form("dedxPixelHarmonic2GrandD%d[candSize]/F",iGDau)); // yousen
            PATGenericParticleNtuple->Branch(Form("dedxHarmonic2GrandD%d",iGDau),grand_H2dedx[iGDau-1],Form("dedxHarmonic2GrandD%d[candSize]/F",iGDau));
          }
        }
        */
        for(ushort iDau=1; iDau<=NDAU_; iDau++){
          if(!dauIsIntermediate_[iDau-1]) continue;
          PATGenericParticleNtuple->Branch(Form("massDaughter%d", iDau), grand_mass[iDau-1], Form("massDaughter%d[candSize]/F", iDau));
          PATGenericParticleNtuple->Branch(Form("VtxProbDaughter%d", iDau), grand_VtxProb[iDau-1], Form("VtxProbDaughter%d[candSize]/F", iDau));
          PATGenericParticleNtuple->Branch(Form("3DCosPointingAngleDaughter%d", iDau), grand_agl[iDau-1], Form("3DCosPointingAngleDaughter%d[candSize]/F", iDau));
          PATGenericParticleNtuple->Branch(Form("3DPointingAngleDaughter%d", iDau), grand_agl_abs[iDau-1], Form("3DPointingAngleDaughter%d[candSize]/F", iDau));
          PATGenericParticleNtuple->Branch(Form("2DCosPointingAngleDaughter%d", iDau), grand_agl2D[iDau-1], Form("2DCosPointingAngleDaughter%d[candSize]/F", iDau));
          PATGenericParticleNtuple->Branch(Form("2DPointingAngleDaughter%d", iDau), grand_agl2D_abs[iDau-1], Form("2DPointingAngleDaughter%d[candSize]/F", iDau));
          PATGenericParticleNtuple->Branch(Form("3DDecayLengthSignificanceDaughter%d", iDau), grand_dlos[iDau-1], Form("3DDecayLengthSignificanceDaughter%d[candSize]/F", iDau));
          PATGenericParticleNtuple->Branch(Form("3DDecayLengthDaughter%d", iDau), grand_dl[iDau-1], Form("3DDecayLengthDaughter%d[candSize]/F", iDau));
          PATGenericParticleNtuple->Branch(Form("2DDecayLengthSignificanceDaughter%d", iDau), grand_dlos2D[iDau-1], Form("2DDecayLengthSignificanceDaughter%d[candSize]/F", iDau));
          PATGenericParticleNtuple->Branch(Form("2DDecayLengthDaughter%d", iDau), grand_dl2D[iDau-1], Form("2DDecayLengthDaughter%d[candSize]/F", iDau));
          for(ushort iGDau=1; iGDau<=NGDAU_; iGDau++)
          {
            PATGenericParticleNtuple->Branch(Form("zDCASignificanceGrandDaughter%dDau%d", iGDau, iDau), grand_dzos[iGDau-1][iDau-1], Form("zDCASignificanceGrandDaughter%dDau%d[candSize]/F", iGDau, iDau));
            PATGenericParticleNtuple->Branch(Form("xyDCASignificanceGrandDaughter%dDau%d", iGDau, iDau), grand_dxyos[iGDau-1][iDau-1], Form("xyDCASignificanceGrandDaughter%dDau%d[candSize]/F", iGDau, iDau));
            PATGenericParticleNtuple->Branch(Form("NHitGrandD%dDau%d", iGDau, iDau), grand_nhit[iGDau-1][iDau-1], Form("NHitGrandD%dDau%d[candSize]/F", iGDau, iDau));
            PATGenericParticleNtuple->Branch(Form("HighPurityGrandDaughter%dDau%d", iGDau, iDau), grand_trkquality[iGDau-1][iDau-1], Form("HighPurityGrandDaughter%dDau%d[candSize]/O", iGDau, iDau));
            PATGenericParticleNtuple->Branch(Form("pTGrandD%dDau%d", iGDau, iDau), grand_pt[iGDau-1][iDau-1], Form("pTGrandD%dDau%d[candSize]/F", iGDau, iDau));
            PATGenericParticleNtuple->Branch(Form("pTerrGrandD%dDau%d", iGDau, iDau), grand_ptErr[iGDau-1][iDau-1], Form("pTerrGrandD%dDau%d[candSize]/F", iGDau, iDau));
            PATGenericParticleNtuple->Branch(Form("EtaGrandD%dDau%d", iGDau, iDau), grand_eta[iGDau-1][iDau-1], Form("EtaGrandD%dDau%d[candSize]/F", iGDau, iDau));
            if(useDeDxData_)
            {
              //PATGenericParticleNtuple->Branch(Form("dedxPixelHarmonic2GrandD%d",iGDau),grand_T4dedx[iGDau-1],Form("dedxPixelHarmonic2GrandD%d[candSize]/F",iGDau)); // yousen
              PATGenericParticleNtuple->Branch(Form("dedxHarmonic2GrandD%dDau%d", iGDau, iDau), grand_H2dedx[iGDau-1][iDau-1], Form("dedxHarmonic2GrandD%dDau%d[candSize]/F", iGDau, iDau));
            }
          }
        }
      }

      for(ushort iDau=1; iDau<=NDAU_; iDau++)
      {
        PATGenericParticleNtuple->Branch(Form("zDCASignificanceDaughter%d",iDau),dzos[iDau-1],Form("zDCASignificanceDaughter%d[candSize]/F",iDau));
        PATGenericParticleNtuple->Branch(Form("xyDCASignificanceDaughter%d",iDau),dxyos[iDau-1],Form("xyDCASignificanceDaughter%d[candSize]/F",iDau));
        PATGenericParticleNtuple->Branch(Form("NHitD%d",iDau),nhit[iDau-1],Form("NHitD%d[candSize]/F",iDau));
        PATGenericParticleNtuple->Branch(Form("HighPurityDaughter%d",iDau),trkquality[iDau-1],Form("HighPurityDaughter%d[candSize]/O",iDau));
        PATGenericParticleNtuple->Branch(Form("pTD%d",iDau),ptDau[iDau-1],Form("pTD%d[candSize]/F",iDau));
        PATGenericParticleNtuple->Branch(Form("pTerrD%d",iDau),ptErr[iDau-1],Form("pTerrD%d[candSize]/F",iDau));
        PATGenericParticleNtuple->Branch(Form("EtaD%d",iDau),etaDau[iDau-1],Form("EtaD%d[candSize]/F",iDau));
        PATGenericParticleNtuple->Branch(Form("PhiD%d",iDau),phiDau[iDau-1],Form("PhiD%d[candSize]/F",iDau));
        PATGenericParticleNtuple->Branch(Form("chargeD%d",iDau),chargeDau[iDau-1],Form("chargeD%d[candSize]/S",iDau));
        if(useDeDxData_)
        {
          //PATGenericParticleNtuple->Branch(Form("dedxPixelHarmonic2D%d",iDau),T4dedx[iDau-1],Form("dedxPixelHarmonic2D%d[candSize]/F",iDau)); // yousen
          PATGenericParticleNtuple->Branch(Form("dedxHarmonic2D%d",iDau),H2dedx[iDau-1],Form("dedxHarmonic2D%d[candSize]/F",iDau));
        }
      }
 
      if(doMuon_)
      {
        for(ushort iDau=1; iDau<=NDAU_; iDau++)
        {
          if(fabs(PID_dau_[iDau-1])!=13) continue;
          PATGenericParticleNtuple->Branch(Form("OneStMuon%d",iDau),onestmuon[iDau-1],Form("OneStMuon%d[candSize]/O",iDau));
          PATGenericParticleNtuple->Branch(Form("PFMuon%d",iDau),pfmuon[iDau-1],Form("PFMuon%d[candSize]/O",iDau));
          PATGenericParticleNtuple->Branch(Form("GlbMuon%d",iDau),glbmuon[iDau-1],Form("GlbMuon%d[candSize]/O",iDau));
          PATGenericParticleNtuple->Branch(Form("trkMuon%d",iDau),trkmuon[iDau-1],Form("trkMuon%d[candSize]/O",iDau));
          PATGenericParticleNtuple->Branch(Form("tightMuon%d",iDau),tightmuon[iDau-1],Form("tightMuon%d[candSize]/O",iDau));
          PATGenericParticleNtuple->Branch(Form("softMuon%d",iDau),softmuon[iDau-1],Form("softMuon%d[candSize]/O",iDau));
          PATGenericParticleNtuple->Branch(Form("hybridMuon%d",iDau),hybridmuon[iDau-1],Form("hybridMuon%d[candSize]/O",iDau));
          PATGenericParticleNtuple->Branch(Form("HPMuon%d",iDau),hpmuon[iDau-1],Form("hybridMuon%d[candSize]/O",iDau));
          PATGenericParticleNtuple->Branch(Form("trigMuon%d",iDau),&(trgmuon[iDau-1]));
          PATGenericParticleNtuple->Branch(Form("nMatchedStationD%d",iDau),nmatchedst[iDau-1],Form("nMatchedStationD%d[candSize]/S",iDau));
          PATGenericParticleNtuple->Branch(Form("nTrackerLayerD%d",iDau),ntrackerlayer[iDau-1],Form("nTrackerLayerD%d[candSize]/S",iDau));
          PATGenericParticleNtuple->Branch(Form("nPixelLayerD%d",iDau),npixellayer[iDau-1],Form("nPixelLayerD%d[candSize]/S",iDau));
          PATGenericParticleNtuple->Branch(Form("nPixelHitD%d",iDau),npixelhit[iDau-1],Form("nPixelHitD%d[candSize]/S",iDau));
          PATGenericParticleNtuple->Branch(Form("nMuonHitD%d",iDau),nmuonhit[iDau-1],Form("nMuonHitD%d[candSize]/S",iDau));
          PATGenericParticleNtuple->Branch(Form("GlbTrkChiD%d",iDau),glbtrkchi[iDau-1],Form("GlbTrkChiD%d[candSize]/F",iDau));
          PATGenericParticleNtuple->Branch(Form("muondXYD%d",iDau),muonbestdxy[iDau-1],Form("muondXYD%d[candSize]/F",iDau));
          PATGenericParticleNtuple->Branch(Form("muondZD%d",iDau),muonbestdz[iDau-1],Form("muondZD%d[candSize]/F",iDau));
          PATGenericParticleNtuple->Branch(Form("dXYD%d",iDau),muondxy[iDau-1],Form("dXYD%d[candSize]/F",iDau));
          PATGenericParticleNtuple->Branch(Form("dZD%d",iDau),muondz[iDau-1],Form("dZD%d[candSize]/F",iDau));
          if(doMuonFull_)
          {
            PATGenericParticleNtuple->Branch(Form("nMatchedChamberD%d",iDau),nmatchedch[iDau-1],Form("nMatchedChamberD%d[candSize]/S",iDau));
            PATGenericParticleNtuple->Branch(Form("EnergyDepositionD%d",iDau),matchedenergy[iDau-1],Form("EnergyDepositionD%d[candSize]/F",iDau));
            PATGenericParticleNtuple->Branch(Form("dx%d_seg",iDau),        dx_seg[iDau-1], Form("dx%d_seg[candSize]/F",iDau));
            PATGenericParticleNtuple->Branch(Form("dy%d_seg",iDau),        dy_seg[iDau-1], Form("dy%d_seg[candSize]/F",iDau));
            PATGenericParticleNtuple->Branch(Form("dxSig%d_seg",iDau),     dxSig_seg[iDau-1], Form("dxSig%d_seg[candSize]/F",iDau));
            PATGenericParticleNtuple->Branch(Form("dySig%d_seg",iDau),     dySig_seg[iDau-1], Form("dySig%d_seg[candSize]/F",iDau));
            PATGenericParticleNtuple->Branch(Form("ddxdz%d_seg",iDau),     ddxdz_seg[iDau-1], Form("ddxdz%d_seg[candSize]/F",iDau));
            PATGenericParticleNtuple->Branch(Form("ddydz%d_seg",iDau),     ddydz_seg[iDau-1], Form("ddydz%d_seg[candSize]/F",iDau));
            PATGenericParticleNtuple->Branch(Form("ddxdzSig%d_seg",iDau),  ddxdzSig_seg[iDau-1], Form("ddxdzSig%d_seg[candSize]/F",iDau));
            PATGenericParticleNtuple->Branch(Form("ddydzSig%d_seg",iDau),  ddydzSig_seg[iDau-1], Form("ddydzSig%d_seg[candSize]/F",iDau));
          }
        }
      }
    }
  } // doRecoNtuple_

  if(doGenNtuple_)
  {
    PATGenericParticleNtuple->Branch("weight_gen",&weight_gen,"weight_gen/F");
    PATGenericParticleNtuple->Branch("candSize_gen",&candSize_gen,"candSize_gen/i");
    PATGenericParticleNtuple->Branch("pT_gen",pt_gen,"pT_gen[candSize_gen]/F");
    PATGenericParticleNtuple->Branch("eta_gen",eta_gen,"eta_gen[candSize_gen]/F");
    PATGenericParticleNtuple->Branch("y_gen",y_gen,"y_gen[candSize_gen]/F");
    PATGenericParticleNtuple->Branch("status_gen",status_gen,"status_gen[candSize_gen]/S");
    PATGenericParticleNtuple->Branch("PID_gen",pid_gen,"PID_gen[candSize_gen]/I");
    PATGenericParticleNtuple->Branch("MotherID_gen",idmom_gen,"MotherID_gen[candSize_gen]/I");
    PATGenericParticleNtuple->Branch("RecIdx_gen",idxrec_gen,"RecIdx_gen[candSize_gen]/S");
    PATGenericParticleNtuple->Branch("3DPointingAngle_gen",&agl_abs_gen,"3DPointingAngle_gen[candSize_gen]/F");
    PATGenericParticleNtuple->Branch("2DPointingAngle_gen",&agl2D_abs_gen,"2DPointingAngle_gen[candSize_gen]/F");
    PATGenericParticleNtuple->Branch("3DDecayLength_gen",&dl_gen,"3DDecayLength_gen[candSize_gen]/F");
    PATGenericParticleNtuple->Branch("2DDecayLength_gen",&dl2D_gen,"2DDecayLength_gen[candSize_gen]/F");

    if(decayInGen_)
    {
      for(ushort iDau=1; iDau<=NDAU_; iDau++)
      {
        PATGenericParticleNtuple->Branch(Form("PIDD%d_gen",iDau),idDau_gen[iDau-1],Form("PIDD%d_gen[candSize_gen]/I",iDau));
        PATGenericParticleNtuple->Branch(Form("chargeD%d_gen",iDau),chargeDau_gen[iDau-1],Form("chargeD%d_gen[candSize_gen]/S",iDau));
        PATGenericParticleNtuple->Branch(Form("pTD%d_gen",iDau),ptDau_gen[iDau-1],Form("pTD%d_gen[candSize_gen]/F",iDau));
        PATGenericParticleNtuple->Branch(Form("EtaD%d_gen",iDau),etaDau_gen[iDau-1],Form("EtaD%d_gen[candSize_gen]/F",iDau));
        PATGenericParticleNtuple->Branch(Form("PhiD%d_gen",iDau),phiDau_gen[iDau-1],Form("PhiD%d_gen[candSize_gen]/F",iDau));
      }
    }
  }
}


//--------------------------------------------------------------------------------------------------
void 
PATGenericParticleTreeProducer::beginRun(const edm::Run& iRun, const edm::EventSetup& iSetup)
{
  bool changed = true;
  EDConsumerBase::Labels triggerResultsLabel;
  EDConsumerBase::labelsForToken(tok_triggerResults_, triggerResultsLabel);
  hltPrescaleProvider_.init(iRun, iSetup, triggerResultsLabel.process, changed);
}


// ------------ method called once each job just after ending the event
//loop  ------------
void 
PATGenericParticleTreeProducer::endJob()
{
}


reco::GenParticleRef
PATGenericParticleTreeProducer::findLastPar(const reco::GenParticleRef& genParRef)
{
  if(genParRef.isNull()) return genParRef;
  reco::GenParticleRef genLastParRef = genParRef;
  const int& pdg_OLD = genParRef->pdgId();
  while(genLastParRef->numberOfDaughters()>0 && genLastParRef->daughterRef(0)->pdgId()==pdg_OLD)
  {
    genLastParRef = genLastParRef->daughterRef(0);
  }
  return genLastParRef;
}


reco::GenParticleRef
PATGenericParticleTreeProducer::findMother(const reco::GenParticleRef& genParRef)
{
  if(genParRef.isNull()) return genParRef;
  reco::GenParticleRef genMomRef = genParRef;
  int pdg = genParRef->pdgId(); const int pdg_OLD = pdg;
  while(pdg==pdg_OLD && genMomRef->numberOfMothers()>0)
  {
    genMomRef = genMomRef->motherRef(0);
    pdg = genMomRef->pdgId();
  }
  if(pdg==pdg_OLD) genMomRef = reco::GenParticleRef();
  return genMomRef;
}


bool
PATGenericParticleTreeProducer::hasDaughters(const reco::GenParticleRef& genParRef, const std::vector<int>& PID_dau)
{
  bool hasDau = false;
  if(genParRef->numberOfDaughters()==PID_dau.size())
  {
    std::vector<int> PIDvec = PID_dau;
    for(ushort iDau=0; iDau<genParRef->numberOfDaughters(); iDau++)
    {
      const auto& dau = *(genParRef->daughter(iDau));
      bool found = false;
      for(ushort iPID=0; iPID<PIDvec.size(); iPID++)
      {
        if(fabs(dau.pdgId())==PIDvec[iPID])
        {
          PIDvec.erase(PIDvec.begin()+iPID);
          found=true;
          break;
        }
      }
      if(!found) break;
    }
    hasDau = (PIDvec.size()==0);
  }
  return hasDau;
}


void
PATGenericParticleTreeProducer::genDecayLength(const uint& it, const reco::GenParticle& gCand)
{
  dl_gen[it] = -99.; agl_abs_gen[it] = -99.; dl2D_gen[it] = -99.; agl2D_abs_gen[it] = -99.;
  if(gCand.numberOfDaughters()==0 || !gCand.daughter(0)) return;
  const auto& dauVtx = gCand.daughter(0)->vertex();
  TVector3 ptosvec(dauVtx.X(), dauVtx.Y(), dauVtx.Z());
  TVector3 secvec(gCand.px(), gCand.py(), gCand.pz());
  agl_abs_gen[it] = secvec.Angle(ptosvec);
  dl_gen[it] = ptosvec.Mag();
  TVector3 ptosvec2D(dauVtx.X(), dauVtx.Y(), 0.0);
  TVector3 secvec2D(gCand.px(), gCand.py(), 0.0);
  agl2D_abs_gen[it] = secvec2D.Angle(ptosvec2D);
  dl2D_gen[it] = ptosvec2D.Mag();
}


//define this as a plug-in
DEFINE_FWK_MODULE(PATGenericParticleTreeProducer);
