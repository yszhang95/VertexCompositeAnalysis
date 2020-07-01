import FWCore.ParameterSet.Config as cms
process = cms.Process("ANASKIM")

process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('HeavyIonsAnalysis.Configuration.collisionEventSelection_cff')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.MessageLogger.cerr.FwkReport.reportEvery = 200

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool( True ),
)

process.GlobalTag.globaltag = '80X_mcRun2_pA_v4'

process.source = cms.Source("PoolSource",
   fileNames = cms.untracked.vstring(
     'root://cmsxrootd.fnal.gov///store/himc/pPb816Summer16DR/LambdaC-KsPr_LCpT-0p9_PbP-EmbEPOS_8p16_Pythia8/AODSIM/PbPEmb_80X_mcRun2_pA_v4-v1/90000/FEE76ED5-FEA1-E711-9163-FA163E497357.root'
   ),
)
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(2000))
#process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(50))

import HLTrigger.HLTfilters.hltHighLevel_cfi
process.hltHM = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
#process.hltHM.HLTPaths = ['HLT_PAFullTracks_Multiplicity280_v*']
process.hltHM.HLTPaths = ['HLT_PAFullTracks_Multiplicity280_v*']
process.hltHM.andOr = cms.bool(True)
process.hltHM.throw = cms.bool(False)

# Ks candidate rereco
process.load("VertexCompositeAnalysis.VertexCompositeProducer.generalParticles_cff")
process.generalKsCandidatesNew = process.generalParticles.clone(
    pdgId = cms.int32(310),
    doSwap = cms.bool(False),
    width = cms.double(0.15),

    preSelection = cms.string(""
       "charge==0"
       ),
    pocaSelection = cms.string(""
       "mass >= 0.44 && mass <= 0.56 && " "pt >= 1.0"
       "&& userFloat('dca') < 0.5 " # assume dca is in cm, check later
       ),
    postSelection = cms.string(""
       ),
    finalSelection = cms.string(""
       "userFloat('lVtxMag') >= 0.0 && userFloat('lVtxSig') >= 5.0"
       "&& abs(rapidity) < 2.4"
       "&& cos(userFloat('angle3D')) > 0.999"
       ),
#
    # daughter information
    daughterInfo = cms.VPSet([
        cms.PSet(pdgId = cms.int32(211), charge = cms.int32(-1),
           selection = cms.string(
              "pt>1.0 && abs(eta)<2.4"
              "&& quality('loose')"" && ptError/pt<0.1"
              "&& normalizedChi2<7.0"
              "&& numberOfValidHits >=4"
              ),
           finalSelection = cms.string(''
             #"userFloat('dzSig') > 1.0"
             #"&& userFloat('dxySig') > 1.0"
              )
           ),
        cms.PSet(pdgId = cms.int32(211), charge = cms.int32(+1),
           selection = cms.string(
              "pt>1.0 && abs(eta)<2.4"
              "&& quality('loose') ""&& ptError/pt<0.1"
              "&& normalizedChi2<7.0"
              "&& numberOfValidHits >=4"
              ),
           finalSelection = cms.string(''
             #"userFloat('dzSig') > 1.0"
             #"&& userFloat('dxySig') > 1.0"
              )
           )
    ])
  )

# lambda C candidates
process.generalLamCCandidatesNew = process.generalParticles.clone(
    pdgId = cms.int32(4122),
    doSwap = cms.bool(False),
    width = cms.double(0.15),

    preSelection = cms.string(""
       ),
    pocaSelection = cms.string(""
       "mass >= (2.2849-0.6) && mass <= (2.2849+0.6) && pt >= 1.0"
       #"&& userFloat('dca') >= 0.5 && userFloat('dca') <= 9999." # assume dca is in cm, check later
       ),
    postSelection = cms.string(""
       #"userFloat('vertexProb') >= 0.02" # temporary use it
       ),
    finalSelection = cms.string(""
       "userFloat('lVtxMag') >= 0.0 && userFloat('lVtxSig') >= 1.0"
       "&&userFloat('rVtxMag') >= 0.0 && userFloat('rVtxSig') >= 1.0"
       "&& abs(userFloat('angle3D')) <= 1.0 && abs(userFloat('angle2D')) <= 1.0"
       "&& abs(rapidity) < 2.0"
       ),
#
    # daughter information
    daughterInfo = cms.VPSet([
        cms.PSet(pdgId = cms.int32(310), charge = cms.int32(0),
          source = cms.InputTag("generalKsCandidatesNew"),
          mass = cms.double(0.4976),
          width = cms.double(0.15),
          selection = cms.string(""),
          finalSelection = cms.string('')
           ),
        cms.PSet(pdgId = cms.int32(2212), #charge = cms.int32(+1),
           selection = cms.string(
              "pt>1.0 && abs(eta)<2.4"
              "&& quality('highPurity') && ptError/pt<0.1"
              "&& normalizedChi2<7.0"
              "&& numberOfValidHits >=11"
              ),
           finalSelection = cms.string(''
              'abs(userFloat("dzSig")) < 3.0 && abs(userFloat("dxySig")) < 3.0' # not sure
              )
           )
    ])
  )

# tree producer
process.load("VertexCompositeAnalysis.VertexCompositeAnalyzer.generalanalyzer_tree_cff")
process.generalksanaNew = process.generalana_mc.clone(
  VertexCompositeCollection = cms.untracked.InputTag("generalKsCandidatesNew"),
  isEventPlane = cms.bool(False),
  eventplaneSrc = cms.InputTag(""),
  centralityBinLabel = cms.InputTag("",""),
  FilterResultCollection = cms.untracked.InputTag("TriggerResults::ANASKIM"),
  selectEvents = cms.untracked.string("eventFilter_HM_step"),
  GenParticleCollection = cms.untracked.InputTag("genParticles"),
  twoLayerDecay = cms.untracked.bool(False),
  deltaPt = cms.untracked.double(0.5),
  deltaR = cms.untracked.double(0.03),
  dauIsIntermediate = cms.untracked.vint32(0, 0),
    )
process.generallamcanaNew = process.generalana_mc.clone(
  VertexCompositeCollection = cms.untracked.InputTag("generalLamCCandidatesNew"),
  #VertexCompositeCollection = cms.untracked.InputTag("generalKsCandidatesNew"),
  isEventPlane = cms.bool(False),
  eventplaneSrc = cms.InputTag(""),
  centralityBinLabel = cms.InputTag("",""),
  FilterResultCollection = cms.untracked.InputTag("TriggerResults::ANASKIM"),
  selectEvents = cms.untracked.string("eventFilter_HM_step"),
  GenParticleCollection = cms.untracked.InputTag("genParticles"),
  twoLayerDecay = cms.untracked.bool(True),
  deltaPt = cms.untracked.double(0.5),
  deltaR = cms.untracked.double(0.03),
  dauIsIntermediate = cms.untracked.vint32(1, 0),
    )

process.generalksanaNewSeq = cms.Sequence(process.generalksanaNew)
process.generallamcanaNewSeq = cms.Sequence(process.generallamcanaNew)

# Add PbPb collision event selection
process.PAprimaryVertexFilter = cms.EDFilter("VertexSelector",
    src = cms.InputTag("offlinePrimaryVertices"),
    cut = cms.string("!isFake && abs(z) <= 25 && position.Rho <= 2 && tracksSize >= 2"),
#    cut = cms.string("!isFake && abs(z) <= 1 && position.Rho <= 2 && tracksSize >= 5"),
    filter = cms.bool(True),   # otherwise it won't filter the events
)

#Reject beam scraping events standard pp configuration
process.NoScraping = cms.EDFilter("FilterOutScraping",
    applyfilter = cms.untracked.bool(True),
    debugOn = cms.untracked.bool(False),
    numtrack = cms.untracked.uint32(10),
    thresh = cms.untracked.double(0.25)
)

process.PAcollisionEventSelection = cms.Sequence(
                                         #process.hfCoincFilter *
                                         process.PAprimaryVertexFilter *
                                         process.NoScraping
                                         )

# Define the event selection sequence
process.eventFilter_HM = cms.Sequence(
    #process.hltHM *
    process.PAcollisionEventSelection
)
process.eventFilter_HM_step = cms.Path( process.eventFilter_HM )

# Define the analysis steps
process.ks_rereco_step = cms.Path(process.eventFilter_HM * process.generalKsCandidatesNew )
process.lamc_rereco_step = cms.Path(process.eventFilter_HM * process.generalLamCCandidatesNew)
#process.rereco_step = cms.Path(process.eventFilter_HM * process.generalKsCandidatesNew* process.generalLamCCandidatesNew)

# Define the output
process.TFileService = cms.Service("TFileService",
                                       fileName = cms.string('lamCKsPana_mc.root')
                                  )

process.load("VertexCompositeAnalysis.VertexCompositeProducer.ppanalysisSkimContentD0_cff")
process.output_HM = cms.OutputModule("PoolOutputModule",
    outputCommands = process.analysisSkimContent.outputCommands,
    fileName = cms.untracked.string('pPb2016MC_SKIM_AOD.root'),
    SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('eventFilter_HM_step')),
    dataset = cms.untracked.PSet(
      dataTier = cms.untracked.string('AOD')
    )
)

process.generalksana_step = cms.EndPath( process.generalksanaNewSeq )
process.generallamcana_step = cms.EndPath( process.generallamcanaNewSeq )


process.output_HM.outputCommands = cms.untracked.vstring('drop *',
      'keep *_generalKsCandidatesNew_*_*',
      'keep *_generalLamCCandidatesNew_*_*',
      'keep *_TriggerResults_*_*'
)
process.output_HM_step = cms.EndPath(process.output_HM)

# Define the process schedule
process.schedule = cms.Schedule(
    process.eventFilter_HM_step,
    process.ks_rereco_step,
    process.lamc_rereco_step,
    #process.rereco_step,
    #process.output_HM_step,
    process.generalksana_step,
    process.generallamcana_step
)
