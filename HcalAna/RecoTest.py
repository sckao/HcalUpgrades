# Auto generated configuration file
# using: 
# Revision: 1.172.2.5 
# Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: step2 -s RECO -n 100 --conditions DESIGN_36_V10::All --datatier GEN-SIM-RECO --eventcontent RECOSIM --beamspot Gauss --fileout file:reco.root --filein file:raw.root --python_filename RecoMuon_Fullsim_cfg.py --no_exec
import FWCore.ParameterSet.Config as cms

process = cms.Process('RECO')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load("SLHCUpgradeSimulations.Geometry.mixLowLumPU_Phase1_R30F12_HCal_cff")
process.load("SLHCUpgradeSimulations.Geometry.Phase1_R30F12_HCal_cmsSimIdealGeometryXML_cff")
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('SLHCUpgradeSimulations.Geometry.Digi_Phase1_R30F12_HCal_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('Configuration.StandardSequences.DigiToRaw_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.EventContent.EventContent_cff')

#process.configurationMetadata = cms.untracked.PSet(
#    version = cms.untracked.string('$Revision: 1.6 $'),
#    annotation = cms.untracked.string('step2 nevts:100'),
#    name = cms.untracked.string('PyReleaseValidation')
#)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)
process.options = cms.untracked.PSet(
  wantSummary = cms.untracked.bool(True)

)
# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
	#'file:FourPiPt_1_50_cfi_GEN_SIM.root'
	#'file:/local/cms/phedex/store/mc/Summer12/PYTHIA6_Tauola_TTbar_TuneZ2star_14TeV/GEN-SIM/UpgradeHCAL_PixelPhase1-DESIGN42_V17-v1/0000/000313A6-4098-E111-AED6-80000048FE80.root', 
	#'file:/local/cms/phedex/store/mc/Summer12/PYTHIA6_Tauola_TTbar_TuneZ2star_14TeV/GEN-SIM/UpgradeHCAL_PixelPhase1-DESIGN42_V17-v1/0000/00800DC1-BB94-E111-BB26-0026189438FA.root',
	#'file:/local/cms/phedex/store/mc/Summer12/PYTHIA6_Tauola_TTbar_TuneZ2star_14TeV/GEN-SIM/UpgradeHCAL_PixelPhase1-DESIGN42_V17-v1/0000/008A69DB-BD94-E111-99EF-00261894388B.root'
        #'file:/local/cms/phedex/store/mc/Summer12/PYTHIA6_Tauola_TTbar_TuneZ2star_14TeV/GEN-SIM/UpgradeHCAL_PixelPhase1-DESIGN42_V17-v1/0000/000313A6-4098-E111-AED6-80000048FE80.root'
	'file:/local/cms/phedex/store/mc/Summer12/PYTHIA6_Tauola_TTbar_TuneZ2star_14TeV/GEN-SIM/UpgradeHCAL_PixelPhase1-DESIGN42_V17-v1/0000/00800DC1-BB94-E111-BB26-0026189438FA.root' 
    )
)
# Output definition
HCalUpgradeRECOSIMEventContent = cms.untracked.vstring()
HCalUpgradeRECOSIMEventContent.extend(process.RECOSIMEventContent.outputCommands)
HCalUpgradeRECOSIMEventContent.extend([
    #'keep *_hcalDigis_*_*',
    #'keep *_simHcalDigis_*_*',
    #'keep *_simEcalDigis_*_*',
    #'keep *_calotowermaker_*_*',
    #'keep *_simEcalUnsuppressedDigis_*_*',
    #'keep *_hbheprereco_*_*',
    #'keep *_*_*_*'
    'keep *_generator_*_*', 
    'keep *_simHcalUnsuppressedDigis_*_*',
    'keep *_hcalupgradereco_*_*',
    'keep *_hbhereco_*_*',
    ])

process.output = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = HCalUpgradeRECOSIMEventContent,
    fileName = cms.untracked.string('file:/local/cms/user/sckao/tt_mixtest2.root'),
    #dataset = cms.untracked.PSet(
    #    dataTier = cms.untracked.string('GEN-SIM-RECO'),
    #    filterName = cms.untracked.string('')
    #)
     SelectEvents = cms.untracked.PSet(
                     SelectEvents = cms.vstring('skim_step')
     )   
)

#I'm only interested in the validation stuff
#process.output.outputCommands = cms.untracked.vstring('drop *','keep *_MEtoEDMConverter_*_*')

#process.output = cms.OutputModule("PoolOutputModule",
#         outputCommands = process.AODSIMEventContent.outputCommands,
#         fileName = cms.untracked.string(
#		'file:/uscms_data/d2/brownson/slhc/quadMuon_RECO.root')
#)


# Additional output definition

# Other statements
process.GlobalTag.globaltag = 'DESIGN42_V17::All'

### PhaseI Geometry and modifications ###############################################
#process.Timing =  cms.Service("Timing")
## no playback when doing digis
#process.mix.playback = True
#process.MessageLogger.destinations = cms.untracked.vstring("detailedInfo_fullph1geom")

### if pileup we need to set the number
#process.mix.input.nbPileupEvents = cms.PSet(
#  averageNumber = cms.double(50.0)
#)


process.mix.input = cms.SecSource("PoolSource",
  nbPileupEvents = cms.PSet(
                   averageNumber = cms.double(50.0)
  ),
  type = cms.string('poisson'),
  sequential = cms.untracked.bool(False),
  fileNames = cms.untracked.vstring(

'file:/local/cms/phedex/store/mc/Summer12/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/UpgradeHCAL_PixelPhase1-DESIGN42_V17-v2/0000/1ACAF486-5FA3-E111-BA2A-18A905705B70.root',
'file:/local/cms/phedex/store/mc/Summer12/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/UpgradeHCAL_PixelPhase1-DESIGN42_V17-v2/0000/20755E6A-B1A3-E111-B3DF-18A90570BB52.root',
'file:/local/cms/phedex/store/mc/Summer12/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/UpgradeHCAL_PixelPhase1-DESIGN42_V17-v2/0000/307E9292-5FA3-E111-B42E-18A905702BD2.root',
'file:/local/cms/phedex/store/mc/Summer12/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/UpgradeHCAL_PixelPhase1-DESIGN42_V17-v2/0000/5A8DFB50-95A3-E111-B471-18A905705A8C.root',
'file:/local/cms/phedex/store/mc/Summer12/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/UpgradeHCAL_PixelPhase1-DESIGN42_V17-v2/0000/6873D550-95A3-E111-8BF9-18A905705B70.root',
'file:/local/cms/phedex/store/mc/Summer12/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/UpgradeHCAL_PixelPhase1-DESIGN42_V17-v2/0000/70C46D55-DBA3-E111-9626-18A905705B70.root',
'file:/local/cms/phedex/store/mc/Summer12/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/UpgradeHCAL_PixelPhase1-DESIGN42_V17-v2/0000/7AE0360B-C8A3-E111-BB3C-18A90570BB1C.root',
'file:/local/cms/phedex/store/mc/Summer12/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/UpgradeHCAL_PixelPhase1-DESIGN42_V17-v2/0000/82E3035F-97A3-E111-83D2-18A905705B70.root',
'file:/local/cms/phedex/store/mc/Summer12/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/UpgradeHCAL_PixelPhase1-DESIGN42_V17-v2/0000/8A32AD02-99A3-E111-BF84-18A90570394C.root',
'file:/local/cms/phedex/store/mc/Summer12/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/UpgradeHCAL_PixelPhase1-DESIGN42_V17-v2/0000/92B94B51-95A3-E111-94CB-18A905706BA8.root',
'file:/local/cms/phedex/store/mc/Summer12/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/UpgradeHCAL_PixelPhase1-DESIGN42_V17-v2/0000/9466F494-AEA3-E111-B558-18A90570BB1C.root',
'file:/local/cms/phedex/store/mc/Summer12/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/UpgradeHCAL_PixelPhase1-DESIGN42_V17-v2/0000/B4DE1D91-5FA3-E111-A792-18A905702BD2.root',
'file:/local/cms/phedex/store/mc/Summer12/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/UpgradeHCAL_PixelPhase1-DESIGN42_V17-v2/0000/CCCC71A6-95A3-E111-9E0C-18A90570A7EC.root',
'file:/local/cms/phedex/store/mc/Summer12/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/UpgradeHCAL_PixelPhase1-DESIGN42_V17-v2/0000/D006DF55-95A3-E111-B84A-18A90570497E.root',
'file:/local/cms/phedex/store/mc/Summer12/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/UpgradeHCAL_PixelPhase1-DESIGN42_V17-v2/0000/EAF214C2-C9A3-E111-B324-18A90570BB1C.root'

	      	)
)

### if doing inefficiency at <PU>=50
#process.simSiPixelDigis.AddPixelInefficiency = 20
## also for strips TIB inefficiency if we want
## TIB1,2 inefficiency at 20%
#process.simSiStripDigis.Inefficiency = 20
## TIB1,2 inefficiency at 50%
#process.simSiStripDigis.Inefficiency = 30
## TIB1,2 inefficiency at 99% (i.e. dead)
#process.simSiStripDigis.Inefficiency = 40

process.load("SLHCUpgradeSimulations.Geometry.fakeConditions_Phase1_R30F12_HCal_cff")
process.load("SLHCUpgradeSimulations.Geometry.recoFromSimDigis_cff")
process.load("SLHCUpgradeSimulations.Geometry.upgradeTracking_phase1_cff")

process.ctfWithMaterialTracks.TTRHBuilder = 'WithTrackAngle'
process.PixelCPEGenericESProducer.UseErrorsFromTemplates = cms.bool(False)
process.PixelCPEGenericESProducer.TruncatePixelCharge = cms.bool(False)
process.PixelCPEGenericESProducer.LoadTemplatesFromDB = cms.bool(False)
process.PixelCPEGenericESProducer.Upgrade = cms.bool(True)
process.PixelCPEGenericESProducer.SmallPitch = False
process.PixelCPEGenericESProducer.IrradiationBiasCorrection = False
process.PixelCPEGenericESProducer.DoCosmics = False

## CPE for other steps
process.siPixelRecHits.CPE = cms.string('PixelCPEGeneric')
process.initialStepTracks.TTRHBuilder = cms.string('WithTrackAngle')
process.lowPtTripletStepTracks.TTRHBuilder = cms.string('WithTrackAngle')
process.pixelPairStepTracks.TTRHBuilder = cms.string('WithTrackAngle')
process.detachedTripletStepTracks.TTRHBuilder = cms.string('WithTrackAngle')
process.mixedTripletStepTracks.TTRHBuilder = cms.string('WithTrackAngle')
process.pixelLessStepTracks.TTRHBuilder = cms.string('WithTrackAngle')
process.tobTecStepTracks.TTRHBuilder = cms.string('WithTrackAngle')
process.highPtTripletStepTracks.TTRHBuilder = cms.string('WithTrackAngle')

# Need these lines to stop some errors about missing siStripDigis collections.
# should add them to fakeConditions_Phase1_cff
process.MeasurementTracker.inactiveStripDetectorLabels = cms.VInputTag()
process.MeasurementTracker.UseStripModuleQualityDB     = cms.bool(False)
process.MeasurementTracker.UseStripAPVFiberQualityDB   = cms.bool(False)
process.MeasurementTracker.UseStripStripQualityDB      = cms.bool(False)
process.MeasurementTracker.UsePixelModuleQualityDB     = cms.bool(False)
process.MeasurementTracker.UsePixelROCQualityDB        = cms.bool(False)
#process.lowPtTripletStepMeasurementTracker.inactiveStripDetectorLabels = cms.VInputTag()
#process.lowPtTripletStepMeasurementTracker.UseStripModuleQualityDB     = cms.bool(False)
#process.lowPtTripletStepMeasurementTracker.UseStripAPVFiberQualityDB   = cms.bool(False)
#process.lowPtTripletStepMeasurementTracker.UseStripStripQualityDB      = cms.bool(False)
#process.lowPtTripletStepMeasurementTracker.UsePixelModuleQualityDB     = cms.bool(False)
#process.lowPtTripletStepMeasurementTracker.UsePixelROCQualityDB        = cms.bool(False)
#process.pixelPairStepMeasurementTracker.inactiveStripDetectorLabels = cms.VInputTag()
#process.pixelPairStepMeasurementTracker.UseStripModuleQualityDB     = cms.bool(False)
#process.pixelPairStepMeasurementTracker.UseStripAPVFiberQualityDB   = cms.bool(False)
#process.pixelPairStepMeasurementTracker.UseStripStripQualityDB      = cms.bool(False)
#process.pixelPairStepMeasurementTracker.UsePixelModuleQualityDB     = cms.bool(False)
#process.pixelPairStepMeasurementTracker.UsePixelROCQualityDB        = cms.bool(False)
process.detachedTripletStepMeasurementTracker.inactiveStripDetectorLabels = cms.VInputTag()
process.detachedTripletStepMeasurementTracker.UseStripModuleQualityDB     = cms.bool(False)
process.detachedTripletStepMeasurementTracker.UseStripAPVFiberQualityDB   = cms.bool(False)
process.detachedTripletStepMeasurementTracker.UseStripStripQualityDB      = cms.bool(False)
process.detachedTripletStepMeasurementTracker.UsePixelModuleQualityDB     = cms.bool(False)
process.detachedTripletStepMeasurementTracker.UsePixelROCQualityDB        = cms.bool(False)
process.mixedTripletStepMeasurementTracker.inactiveStripDetectorLabels = cms.VInputTag()
process.mixedTripletStepMeasurementTracker.UseStripModuleQualityDB     = cms.bool(False)
process.mixedTripletStepMeasurementTracker.UseStripAPVFiberQualityDB   = cms.bool(False)
process.mixedTripletStepMeasurementTracker.UseStripStripQualityDB      = cms.bool(False)
process.mixedTripletStepMeasurementTracker.UsePixelModuleQualityDB     = cms.bool(False)
process.mixedTripletStepMeasurementTracker.UsePixelROCQualityDB        = cms.bool(False)
process.pixelLessStepMeasurementTracker.inactiveStripDetectorLabels = cms.VInputTag()
process.tobTecStepMeasurementTracker.inactiveStripDetectorLabels = cms.VInputTag()

process.muons.TrackerKinkFinderParameters.TrackerRecHitBuilder = cms.string('WithTrackAngle')
# The SeedMergerPSet should be added to the following file for Phase 1
# RecoTracker/SpecialSeedGenerators/python/CombinatorialSeedGeneratorForCosmicsRegionalReconstruction_cfi.py
# but pixel layers are not used here for cosmic TODO: need Maria and Jan to do appropriate thing here
process.regionalCosmicTrackerSeeds.SeedMergerPSet = cms.PSet(
	mergeTriplets = cms.bool(False),
	ttrhBuilderLabel = cms.string( "PixelTTRHBuilderWithoutAngle" ),
	addRemainingTriplets = cms.bool(False),
	layerListName = cms.string( "PixelSeedMergerQuadruplets" )
	)
process.regionalCosmicTracks.TTRHBuilder = cms.string('WithTrackAngle')

#################################

process.ReadLocalMeasurement = cms.EDAnalyzer("StdHitNtuplizer",
   src = cms.InputTag("siPixelRecHits"),
   stereoRecHits = cms.InputTag("siStripMatchedRecHits","stereoRecHit"),
   rphiRecHits = cms.InputTag("siStripMatchedRecHits","rphiRecHit"),
   matchedRecHits = cms.InputTag("siStripMatchedRecHits","matchedRecHit"),
   ### if using simple (non-iterative) or old (as in 1_8_4) tracking
   trackProducer = cms.InputTag("generalTracks"),
   OutputFile = cms.string("stdgrechitfullph1g_ntuple.root"),
   ### for using track hit association
   associatePixel = cms.bool(True),
   associateStrip = cms.bool(False),
   associateRecoTracks = cms.bool(False),
   ROUList = cms.vstring('g4SimHitsTrackerHitsPixelBarrelLowTof',
                         'g4SimHitsTrackerHitsPixelBarrelHighTof',
                         'g4SimHitsTrackerHitsPixelEndcapLowTof',
                         'g4SimHitsTrackerHitsPixelEndcapHighTof')
)
process.anal = cms.EDAnalyzer("EventContentAnalyzer")

## need this at the end as the validation config redefines random seed with just mix
#process.load("IOMC.RandomEngine.IOMC_cff")

### Now add the HCal upgrade Reco ##################################################
# Path and EndPath definitions
process.load("RecoLocalCalo.HcalRecProducers.HcalUpgradeReconstructor_cff")
process.load("RecoJets.Configuration.CaloTowersRec_cff")
process.load("RecoLocalCalo.HcalRecAlgos.hcalRecAlgoESProd_cfi")
process.load("RecoLocalCalo.Configuration.RecoLocalCalo_cff")
#process.load("RecoLocalCalo.CaloTowersCreator.calotowermaker_cfi")

process.ecalGlobalUncalibRecHit.EBdigiCollection = cms.InputTag("simEcalDigis","ebDigis")
process.ecalGlobalUncalibRecHit.EEdigiCollection = cms.InputTag("simEcalDigis","eeDigis")
process.ecalRecHit.ebDetIdToBeRecovered = cms.InputTag("","")
process.ecalRecHit.eeDetIdToBeRecovered = cms.InputTag("","")
process.ecalRecHit.eeFEToBeRecovered = cms.InputTag("","")
process.ecalRecHit.ebFEToBeRecovered = cms.InputTag("","")
process.ecalRecHit.recoverEBFE = cms.bool(False)
process.ecalRecHit.recoverEEFE = cms.bool(False)

process.load("RecoLocalCalo.HcalRecProducers.HcalSimpleReconstructor_hbhe_cfi")
process.load("RecoLocalCalo.HcalRecProducers.HcalSimpleReconstructor_ho_cfi")
process.load("RecoLocalCalo.HcalRecProducers.HcalSimpleReconstructor_hf_cfi")

process.hbheprereco.digiLabel = "simHcalUnsuppressedDigis"
process.horeco.digiLabel = "simHcalUnsuppressedDigis"
process.hfreco.digiLabel = "simHcalUnsuppressedDigis"
process.hcalupgradereco.digiLabel = "simHcalUnsuppressedDigis"

### Known alterations for Reco #####################################################
delattr(process,"hbhereco")
process.hbhereco = process.hbheprereco.clone()
#process.hcalLocalRecoSequence.replace(process.hbheprereco,process.hbhereco)

process.metrecoPlusHCALNoise.remove(process.BeamHaloSummary)
process.metrecoPlusHCALNoise.remove(process.GlobalHaloData)
process.metrecoPlusHCALNoise.remove(process.HcalHaloData)

process.metrecoPlusHCALNoise.remove(process.hcalnoise)
process.jetGlobalReco = cms.Sequence(process.recoJets*process.recoTrackJets)
process.jetHighLevelReco = cms.Sequence(process.recoJetAssociations*process.recoPFJets*process.recoJPTJets)

### Add MuonFilter ######################
process.muonFilter = cms.EDFilter("MuonFilter",

    muonSource = cms.InputTag('muons'),
    ptCut      = cms.double( 5.0 ),
    etaCut     = cms.double( 2.5 ),
    minNumber  = cms.int32( 1 )

)


### Place to add in the reco steps one by one ######################################
process.calolocalreco = cms.Sequence(process.ecalGlobalUncalibRecHit+
				process.ecalDetIdToBeRecovered+
				process.ecalRecHit+
				process.ecalCompactTrigPrim+
				process.ecalTPSkim+
				process.ecalPreshowerRecHit+
				#process.hbheprereco+
				process.hbhereco+
				process.horeco+process.hfreco+process.hcalupgradereco+process.towerMaker
				#+process.zdcreco
				)
process.localreco  = cms.Sequence(process.trackerlocalreco+
				process.muonlocalreco+
				process.calolocalreco+
				process.castorreco+
				process.lumiProducer
				)
process.globalreco = cms.Sequence(process.offlineBeamSpot
                          *process.recopixelvertexing
                          *process.trackingGlobalReco
                          #*process.hcalGlobalRecoSequence 
                          *process.particleFlowCluster
                          *process.ecalClusters
                          *process.caloTowersRec
                          *process.vertexreco
                          *process.egammaGlobalReco
                          *process.pfTrackingGlobalReco
                          *process.jetGlobalReco
                          *process.muonrecoComplete
                          *process.muoncosmicreco
                          *process.CastorFullReco
			  )
process.highlevelreco = cms.Sequence(process.egammaHighLevelRecoPrePF
                             *process.particleFlowReco
                             *process.egammaHighLevelRecoPostPF
                             *process.jetHighLevelReco
                             *process.tautagging
                             *process.metrecoPlusHCALNoise 
                             *process.btagging
                             *process.recoPFMET
                             *process.PFTau
                             *process.regionalCosmicTracksSeq
                             *process.muoncosmichighlevelreco
                             *process.reducedRecHits
                             )
process.reconstruction = cms.Sequence(	process.localreco       *
					process.globalreco      *
					process.highlevelreco   *
					process.logErrorHarvester 
					)
### back to standard job commands ##################################################
process.DigiToRaw.remove(process.castorRawData)

process.DigiToRaw.remove(process.siPixelRawData)
process.RawToDigi.remove(process.siPixelDigis)

## removing large memory usage module if we don't need it
process.pdigi.remove(process.mergedtruth)

process.digitisation_step = cms.Path(process.pdigi)
process.L1simulation_step = cms.Path(process.SimL1Emulator)

# Path and EndPath definitions
process.digi2raw_step = cms.Path(process.DigiToRaw)
process.raw2digi_step = cms.Path(process.RawToDigi)
process.L1Reco_step = cms.Path(process.L1Reco)

#process.reconstruction_step 	= cms.Path(process.reconstruction + process.muonFilter )
process.skim_step 	        = cms.Path(process.muonFilter )
process.reconstruction_step 	= cms.Path(process.reconstruction )
process.mix_step 		= cms.Path(process.mix)
process.debug_step 		= cms.Path(process.anal)
process.user_step 		= cms.Path(process.ReadLocalMeasurement)
process.endjob_step 		= cms.Path(process.endOfProcess )
process.out_step 		= cms.EndPath(process.output )

# Schedule definition
#process.schedule = cms.Schedule(process.digitisation_step,process.L1simulation_step,process.digi2raw_step,process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.skim_step,process.endjob_step,process.out_step)
process.schedule = cms.Schedule(process.digitisation_step,process.L1simulation_step,process.digi2raw_step,process.raw2digi_step,process.mix_step,process.reconstruction_step,process.skim_step,process.endjob_step,process.out_step)

