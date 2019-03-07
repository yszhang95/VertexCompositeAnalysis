import os
import sys

import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

from Configuration.StandardSequences.Eras import eras

process = cms.Process('MTDAnalysis',eras.Phase2C4_timing_layer_bar)

#process = cms.Process("ANASKIM")

# setup 'standard'  options
#paras = VarParsing.VarParsing ('test')
#paras.pTMin = 1.95
#paras.pTMax = 3.0

# get and parse the command line arguments
#options.parseArguments()

process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('HeavyIonsAnalysis.Configuration.collisionEventSelection_cff')
process.load('Configuration.EventContent.EventContent_cff')
#process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.Geometry.GeometryExtended2023D35Reco_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.MessageLogger.cerr.FwkReport.reportEvery = 2

process.source = cms.Source("PoolSource",
   fileNames = cms.untracked.vstring(
'root://xrootd-cms.infn.it//store/user/anstahll/MTD/MC/NonEmbedded/Hydjet_5p02TeV_TuneCP5_MTD_RECO_20190127/Hydjet_5p02TeV_TuneCP5_MTD/Hydjet_5p02TeV_TuneCP5_MTD_RECO_20190127/190127_085926/0000/Hydjet_RECO_9.root'
),
                        skipEvents=cms.untracked.uint32(24)
)

# =============== Other Statements =====================
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1))
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))
process.GlobalTag.globaltag = '103X_upgrade2023_realistic_v2'

# =============== Import Sequences =====================

process.PAprimaryVertexFilter = cms.EDFilter("VertexSelector",
    src = cms.InputTag("offlinePrimaryVertices4D"),
    cut = cms.string("!isFake && abs(z) <= 50 && position.Rho <= 5 && tracksSize >= 2"),
#    cut = cms.string("!isFake && abs(z) <= 1 && position.Rho <= 2 && tracksSize >= 5"),
    filter = cms.bool(True),   # otherwise it won't filter the events
)

#Reject beam scraping events standard pp configuration
#process.NoScraping = cms.EDFilter("FilterOutScraping",
#    applyfilter = cms.untracked.bool(True),
#    debugOn = cms.untracked.bool(False),
#    numtrack = cms.untracked.uint32(10),
#    thresh = cms.untracked.double(0.25)
#)

process.PAcollisionEventSelection = cms.Sequence(
                                         #process.hfCoincFilter * 
                                         #process.hfCoincFilter3 *
                                         process.PAprimaryVertexFilter #*
#                                         process.NoScraping
                                         )

process.eventFilter_HM = cms.Sequence( 
    process.PAcollisionEventSelection
)

process.eventFilter_HM_step = cms.Path( process.eventFilter_HM )

#process.dEdx_step = cms.Path( process.eventFilter_HM * process.produceEnergyLoss )

########## D0 candidate rereco ###############################################################
process.load("VertexCompositeAnalysis.VertexCompositeProducer.generalLamC3PCandidates_cff")
process.generalLamC3PCandidatesNew = process.generalLamC3PCandidates.clone()
#process.generalLamC3PCandidatesNew.tkPtSumCut = cms.double(2.1)
#process.generalLamC3PCandidatesNew.tkEtaDiffCut = cms.double(1.0)
process.generalLamC3PCandidatesNew.tkNhitsCut = cms.int32(11)
process.generalLamC3PCandidatesNew.tkPtErrCut = cms.double(0.1)
process.generalLamC3PCandidatesNew.tkPCut = cms.double(0.7)
process.generalLamC3PCandidatesNew.tkEtaCut = cms.double(3.0)
process.generalLamC3PCandidatesNew.tkPtMidCut = cms.double(0.7)
process.generalLamC3PCandidatesNew.tkPFwdCut = cms.double(0.7)
process.generalLamC3PCandidatesNew.tkPtFwdCut = cms.double(0.)
#process.generalLamC3PCandidatesNew.alphaCut = cms.double(1.0)
#process.generalLamC3PCandidatesNew.alpha2DCut = cms.double(1.0)
process.generalLamC3PCandidatesNew.VtxChiProbCut = cms.double(0.15)

process.generalLamC3PCandidatesNew.tkDCACut = cms.double(0.5)
process.generalLamC3PCandidatesNew.dPt3CutMin = cms.double(1.95)
#process.generalLamC3PCandidatesNew.dPt3CutMax = cms.double(1000)
process.generalLamC3PCandidatesNew.dY3CutMin = cms.double(-3.1)
process.generalLamC3PCandidatesNew.dY3CutMax = cms.double(3.1)

process.load('RecoMTD.TrackExtender.trackExtenderWithMTD_cfi')
process.load('RecoLocalFastTime.FTLRecProducers.mtdTrackingRecHits_cfi')
process.load('RecoLocalFastTime.FTLClusterizer.mtdClusters_cfi')

process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",
    ignoreTotal = cms.untracked.int32(1)
)

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
process.hiCentrality.srcVertex = cms.InputTag("offlinePrimaryVertices") 
process.load("RecoHI.HiCentralityAlgos.CentralityBin_cfi") 
process.centralityBin.Centrality = cms.InputTag("hiCentrality") 
process.centralityBin.centralityVariable = cms.string("HFtowers") 
process.centralityBin.nonDefaultGlauberModel = cms.string("") 
process.hiCentrality.srcEBhits = cms.InputTag("HGCalRecHit","HGCHEBRecHits")
process.hiCentrality.srcEEhits = cms.InputTag("HGCalRecHit","HGCEERecHits")

process.cent_seq = cms.Sequence(process.hiCentrality * process.centralityBin)
process.cent_step = cms.Path( process.eventFilter_HM * process.cent_seq )

process.lamc3prereco_step = cms.Path( process.eventFilter_HM * process.generalLamC3PCandidatesNew )

###############################################################################################


process.load("VertexCompositeAnalysis.VertexCompositeAnalyzer.lamc3pselector_cff")
process.load("VertexCompositeAnalysis.VertexCompositeAnalyzer.lamc3panalyzer_ntp_cff")

process.TFileService = cms.Service("TFileService",
                                       fileName =
cms.string('hyjets_mc_lamc3p_mtd_tree.root')
                                   )

process.lamc3pana_mc.isUseMtd = cms.untracked.bool(True)
process.lamc3pana_mc.doRecoNtuple = cms.untracked.bool(True)
process.lamc3pana_mc.doGenNtuple = cms.untracked.bool(True)
process.lamc3pana_mc.doGenMatching = cms.untracked.bool(False)
process.lamc3pana_mc.VertexCollection = cms.untracked.InputTag("offlinePrimaryVertices4D")
process.lamc3pana_mc.VertexCompositeCollection = cms.untracked.InputTag("lamc3pselectorMC:LamC3P")
process.lamc3pana_mc.MVACollection = cms.InputTag("lamc3pselectorMC:MVAValuesNewLamC3P")
process.lamc3pana_mc.isCentrality = cms.bool(True)
process.lamc3pselectorMC.VertexCollection = cms.untracked.InputTag("offlinePrimaryVertices4D")

process.lamc3pana_seq = cms.Sequence(process.lamc3pselectorMC * process.lamc3pana_mc)
process.lamc3pana_step = cms.Path( process.eventFilter_HM * process.lamc3pana_seq )

process.load("VertexCompositeAnalysis.VertexCompositeProducer.mtdanalysisSkimContentD0_cff")
process.output_HM = cms.OutputModule("PoolOutputModule",
    outputCommands = process.analysisSkimContent.outputCommands,
#    fileName = cms.untracked.string('/eos/cms/store/group/phys_heavyions/MTD/LamC3P_test/hyjets_lambdac_cent_pt2to2p5_pca2mm_b3.root'),
    fileName = cms.untracked.string('hyjets_lambdac.root'),
    SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('eventFilter_HM_step')),
    dataset = cms.untracked.PSet(
      dataTier = cms.untracked.string('AOD')
    )
)

process.output_HM_step = cms.EndPath(process.output_HM)

process.schedule = cms.Schedule(
    process.eventFilter_HM_step,
    process.cent_step,
    process.lamc3prereco_step,
    process.lamc3pana_step,
    process.output_HM_step
)
