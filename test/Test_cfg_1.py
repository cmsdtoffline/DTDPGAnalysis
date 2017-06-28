import FWCore.ParameterSet.Config as cms


process = cms.Process("Test")


process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.EventContent.EventContent_cff")
process.load("Configuration.StandardSequences.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("Configuration.StandardSequences.Services_cff")


process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.GlobalTag.globaltag = "90X_dataRun2_Express_v1"


# for HO-TP
process.load("EventFilter.HcalRawToDigi.HcalRawToDigi_cfi")
process.hcalDigis.InputLabel = cms.InputTag("source")

# The analyzer
process.test = cms.EDAnalyzer("Test_1",
    hoTPLabel = cms.InputTag("hcalDigis")
)

maxEvents = -1
#maxEvents = 200
#maxEvents = 10 * pow(10, 3)

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(maxEvents))

#outFileName = "test_pedestal.root"
outFileName = "test_LED.root"

sourceFileNames = cms.untracked.vstring(
    # Pedestal run
    #"root://cms-xrd-global.cern.ch//store/group/dpg_hcal/comm_hcal/USC/run296181/USC_296181.root",
    #"root://cms-xrd-global.cern.ch//store/group/dpg_hcal/comm_hcal/USC/run295673/USC_295673.root",
    
    # LED run
    # (file is corrupt) "root://cms-xrd-global.cern.ch//store/group/dpg_hcal/comm_hcal/USC/run296182/USC_296182.root",
    "root://cms-xrd-global.cern.ch//store/group/dpg_hcal/comm_hcal/USC/run295675/USC_295675.root",
    
    # Das name: /SingleMuon/Run2016H-ZMu-PromptReco-v2/RAW-RECO
    #"root://cms-xrd-global.cern.ch//store/data/Run2016H/SingleMuon/RAW-RECO/ZMu-PromptReco-v2/000/284/035/00000/DC000F0F-559F-E611-825A-02163E0143B9.root",
    #"root://cms-xrd-global.cern.ch//store/data/Run2016H/SingleMuon/RAW-RECO/ZMu-PromptReco-v2/000/284/035/00000/E0908FC9-539F-E611-BCE9-02163E012634.root",
    #"root://cms-xrd-global.cern.ch//store/data/Run2016H/SingleMuon/RAW-RECO/ZMu-PromptReco-v2/000/284/035/00000/E8BFA3F1-539F-E611-9F33-02163E0136CD.root",
    #"root://cms-xrd-global.cern.ch//store/data/Run2016H/SingleMuon/RAW-RECO/ZMu-PromptReco-v2/000/284/035/00000/EE0E75B2-539F-E611-B967-02163E01349D.root",
    #"root://cms-xrd-global.cern.ch//store/data/Run2016H/SingleMuon/RAW-RECO/ZMu-PromptReco-v2/000/284/035/00000/F055B7E5-539F-E611-8192-02163E0119A7.root",
    #"root://cms-xrd-global.cern.ch//store/data/Run2016H/SingleMuon/RAW-RECO/ZMu-PromptReco-v2/000/284/035/00000/F20D63F7-549F-E611-B25B-FA163E8B6264.root",
    #"root://cms-xrd-global.cern.ch//store/data/Run2016H/SingleMuon/RAW-RECO/ZMu-PromptReco-v2/000/284/035/00000/F4D3933A-559F-E611-9694-02163E014557.root",
    #"root://cms-xrd-global.cern.ch//store/data/Run2016H/SingleMuon/RAW-RECO/ZMu-PromptReco-v2/000/284/035/00000/F6FCD1FE-549F-E611-8047-02163E0146C3.root",
    #"root://cms-xrd-global.cern.ch//store/data/Run2016H/SingleMuon/RAW-RECO/ZMu-PromptReco-v2/000/284/035/00000/FA77BCF4-549F-E611-A9F2-FA163EBCA43B.root",
    #"root://cms-xrd-global.cern.ch//store/data/Run2016H/SingleMuon/RAW-RECO/ZMu-PromptReco-v2/000/284/035/00000/FE558300-559F-E611-8C24-02163E012172.root",
)


#processSource = "PoolSource"
processSource = "HcalTBSource"

process.source = cms.Source(processSource,
    fileNames = sourceFileNames,
    secondaryFileNames = cms.untracked.vstring()
)

# Output
process.TFileService = cms.Service("TFileService", fileName = cms.string(outFileName))

process.p = cms.Path( \
    #process.RawToDigi * \
    process.hcalDigis * \
    process.test)

