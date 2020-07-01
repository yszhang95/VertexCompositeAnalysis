from CRABAPI.RawCommand import crabCommand
from CRABClient.ClientExceptions import ClientException
from httplib import HTTPException
#from CRABClient import getUsername


# We want to put all the CRAB project directories from the tasks we submit here into one common directory.
# That's why we need to set this parameter (here or above in the configuration file, it does not matter, we will not overwrite it).
from CRABClient.UserUtilities import config
config = config()

config.section_("General")
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True
config.General.transferLogs = False

config.section_('JobType')
config.JobType.pluginName = 'Analysis'
#config.JobType.psetName = 'LamCKsP.py'
config.JobType.psetName = 'LamCKsP_old.py'

config.section_('Data')
config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.publication = False
config.JobType.allowUndistributedCMSSW = True

config.section_('Site')
config.Data.ignoreLocality = True
config.Site.whitelist = ['T1_US_*','T2_US_*','T1_FR_*','T2_FR_*','T2_CH_CERN']
config.Site.storageSite = 'T3_US_Rice'

def submit(config):
    try:
        crabCommand('submit', config = config, dryrun=False)
    except HTTPException as hte:
        print "Failed submitting task: %s" % (hte.headers)
    except ClientException as cle:
        print "Failed submitting task: %s" % (cle)

#############################################################################################
## From now on that's what users should modify: this is the a-la-CRAB2 configuration part. ##
#############################################################################################

dataMap = {
            #"lamc2ksppt0p9": { "PD": "/LambdaC-KsPr_LCpT-0p9_PbP-EmbEPOS_8p16_Pythia8/pPb816Summer16DR-PbPEmb_80X_mcRun2_pA_v4-v1/AODSIM", "Units": 1, "Memory": 1600, "RunTime": 820 },
            "lamc2ksppt5p9-Bst": { "PD": "/LambdaC-KsPr_LCpT-5p9_PbP-EmbEPOS_8p16_Pythia8/pPb816Summer16DR-PbPEmb_80X_mcRun2_pA_v4-v1/AODSIM", "Units": 1, "Memory": 1600, "RunTime": 820 }
            }

# Submit the MC PDs
for key, val in dataMap.items():
    config.General.requestName = 'VertexCompositeTree_'+key+'_lamCTest_v3'
    config.Data.inputDataset = val["PD"]
    config.Data.unitsPerJob = val["Units"]
    config.JobType.maxMemoryMB = val["Memory"]
    config.JobType.maxJobRuntimeMin = val["RunTime"]
    config.Data.outputDatasetTag = config.General.requestName
    #config.Data.outLFNDirBase = '/store/user/%s/RiceHIN/pPb2016/LamCTree/%s' % (getUsername(), config.General.requestName)
    config.Data.outLFNDirBase = '/store/user/yousen/RiceHIN/pPb2016/LamCTree/%s' % (config.General.requestName)
    print("Submitting CRAB job for: "+val["PD"])
    submit(config)
