import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('ANASKIM',eras.Phase2C4_timing_layer_bar)

process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('HeavyIonsAnalysis.Configuration.collisionEventSelection_cff')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.Geometry.GeometryExtended2023D35Reco_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.MessageLogger.cerr.FwkReport.reportEvery = 2

process.source = cms.Source("PoolSource",
   fileNames = cms.untracked.vstring('/store/mc/PhaseIIMTDTDRAutumn18DR/MinBias_Hydjet_Drume5_5p5TeV_TuneCP5_Pythia8/FEVT/NoPU_103X_upgrade2023_realistic_v2-v2/30000/C14FE87B-FFB6-A043-BFE4-953DB6D2210C.root')
)

# =============== Other Statements =====================
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))
process.GlobalTag.globaltag = '103X_upgrade2023_realistic_v2'

# =============== Import Sequences =====================

process.PAprimaryVertexFilter = cms.EDFilter("VertexSelector",
    src = cms.InputTag("offlinePrimaryVertices4D"),
    cut = cms.string("!isFake && abs(z) <= 50 && position.Rho <= 5 && tracksSize >= 2"),
    filter = cms.bool(True),   # otherwise it won't filter the events
)

process.PAcollisionEventSelection = cms.Sequence(
                                         process.PAprimaryVertexFilter
                                         )

process.eventFilter_HM = cms.Sequence(
    process.PAcollisionEventSelection
)

process.eventFilter_HM_step = cms.Path( process.eventFilter_HM )

########## D0 candidate rereco ###############################################################
process.load("VertexCompositeAnalysis.VertexCompositeProducer.generalD0Candidates_cff")
process.generalD0CandidatesNew = process.generalD0Candidates.clone()
process.generalD0CandidatesNew.tkNhitsCut = cms.int32(11)
process.generalD0CandidatesNew.tkPtErrCut = cms.double(0.1)
process.generalD0CandidatesNew.tkPtMidCut = cms.double(0.7)
process.generalD0CandidatesNew.tkPFwdCut = cms.double(0.7)
process.generalD0CandidatesNew.tkPtFwdCut = cms.double(0.0)

# centrality setup
process.GlobalTag.snapshotTime = cms.string("9999-12-31 23:59:59.000")
process.GlobalTag.toGet.extend([
    cms.PSet(record = cms.string("HeavyIonRcd"),
        tag = cms.string("CentralityTable_HFtowers200_HydjetTuneCP5MTD_v1040mtd4x1_mc"),
        connect = cms.string("frontier://FrontierProd/CMS_CONDITIONS"),
        label = cms.untracked.string("HFtowers")
        ),
    ])
process.load('RecoHI.HiCentralityAlgos.HiCentrality_cfi')
process.hiCentrality.produceHFhits = False
process.hiCentrality.produceHFtowers = True
process.hiCentrality.produceEcalhits = False
process.hiCentrality.produceZDChits = False
process.hiCentrality.produceETmidRapidity = False
process.hiCentrality.producePixelhits = False
process.hiCentrality.produceTracks = False
process.hiCentrality.producePixelTracks = False
process.hiCentrality.reUseCentrality = False
process.hiCentrality.srcReUse = cms.InputTag("hiCentrality","","RECO")
process.hiCentrality.srcTracks = cms.InputTag("generalTracks")
process.hiCentrality.srcVertex = cms.InputTag("offlinePrimaryVertices4D")
process.load("RecoHI.HiCentralityAlgos.CentralityBin_cfi")
process.centralityBin.Centrality = cms.InputTag("hiCentrality")
process.centralityBin.centralityVariable = cms.string("HFtowers")
process.centralityBin.nonDefaultGlauberModel = cms.string("")
process.hiCentrality.srcEBhits = cms.InputTag("HGCalRecHit","HGCHEBRecHits")
process.hiCentrality.srcEEhits = cms.InputTag("HGCalRecHit","HGCEERecHits")

process.cent_seq = cms.Sequence(process.hiCentrality * process.centralityBin)
process.cent_step = cms.Path( process.eventFilter_HM * process.cent_seq )

process.d0rereco_step = cms.Path( process.eventFilter_HM * process.generalD0CandidatesNew )
###############################################################################################

# MTD RE-RECO
process.reconstruction_step = cms.Path(process.eventFilter_HM)
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.pfPileUpIso.PFCandidates = cms.InputTag("particleFlowPtrs")
process.pfNoPileUpIso.bottomCollection = cms.InputTag("particleFlowPtrs")
process.reconstruction_step += process.mtdClusters
process.reconstruction_step += process.mtdTrackingRecHits
process.trackExtenderWithMTD.UseVertex = cms.bool(True) #run trackExtender using vertex constrain
process.trackExtenderWithMTD.DZCut = 0.3
process.reconstruction_step += process.trackExtenderWithMTD
process.tofPID.vtxsSrc = cms.InputTag('offlinePrimaryVertices4D')
process.tofPID.fixedT0Error = cms.double(0.035) #put a constant 0.035 [ns] error for each track (cannot
process.reconstruction_step += process.tofPID

process.generalD0CandidatesNew.trackBeta = cms.InputTag("trackExtenderWithMTD:generalTrackBeta:ANASKIM")
process.generalD0CandidatesNew.trackt0 = cms.InputTag("tofPID:t0:ANASKIM")
process.generalD0CandidatesNew.trackSigmat0 = cms.InputTag("tofPID:sigmat0:ANASKIM")
process.generalD0CandidatesNew.tracktmtd = cms.InputTag("trackExtenderWithMTD:generalTracktmtd:ANASKIM")
process.generalD0CandidatesNew.trackSigmatmtd = cms.InputTag("trackExtenderWithMTD:generalTracksigmatmtd:ANASKIM")
process.generalD0CandidatesNew.trackp = cms.InputTag("trackExtenderWithMTD:generalTrackp:ANASKIM")
process.generalD0CandidatesNew.trackPathLength = cms.InputTag("trackExtenderWithMTD:generalTrackPathLength:ANASKIM")
###############################################################################################

process.load("VertexCompositeAnalysis.VertexCompositeProducer.mtdanalysisSkimContentD0_cff")
process.output_HM = cms.OutputModule("PoolOutputModule",
    outputCommands = process.analysisSkimContent.outputCommands,
    fileName = cms.untracked.string('hyjets_d0.root'),
    SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('eventFilter_HM_step')),
    dataset = cms.untracked.PSet(
      dataTier = cms.untracked.string('AOD')
    )
)

process.output_HM_step = cms.EndPath(process.output_HM)

process.schedule = cms.Schedule(
    process.eventFilter_HM_step,
    process.reconstruction_step,
    process.cent_step,
    process.d0rereco_step,
    process.output_HM_step
)