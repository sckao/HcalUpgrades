import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(50) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        #'dcache:/pnfs/cms/WAX/11/store/mc/Summer12/PYTHIA6_Tauola_TTbar_TuneZ2star_14TeV/GEN-SIM-DIGI-RECO/UpgradeHCAL_PixelPhase1_DR428-PU50-DESIGN42_V17S-v2/0000/'
        'dcache:/pnfs/cms/WAX/11/store/mc/Summer12/PYTHIA6_Tauola_TTbar_TuneZ2star_14TeV/GEN-SIM-DIGI-RECO/UpgradeHCAL_PixelPhase1_DR428-PU50-DESIGN42_V17S-v2/0000/000FE056-B5C6-E111-A3FE-003048678B76.root',
        'dcache:/pnfs/cms/WAX/11/store/mc/Summer12/PYTHIA6_Tauola_TTbar_TuneZ2star_14TeV/GEN-SIM-DIGI-RECO/UpgradeHCAL_PixelPhase1_DR428-PU50-DESIGN42_V17S-v2/0000/001B052D-A4C6-E111-8867-00261894393E.root'
    )
)

process.demo = cms.EDAnalyzer('HcalAna',
    rootFileName = cms.untracked.string('shcal_tt_test.root'),
    debug        = cms.bool(False),
    HBHERecHits  = cms.InputTag('hbhereco'),
    ZSThrehold   = cms.double(0.25),
    VertexSource = cms.InputTag('offlinePrimaryVerticesWithBS'),
    MuonSource   = cms.InputTag('muons'),
    jetSource    = cms.InputTag("ak5PFJets"),

    # vertex cuts                z   ndof   d0
    VertexCuts   = cms.vdouble( 99.,   0,   99 ),
    # muon cuts                 pt  eta   
    MuonCuts     = cms.vdouble( 10, 2.5 ),
    # jet cuts                  pt    eta  nJets
    jetCuts      = cms.vdouble( 30. , 2.4,    0 )

)


process.p = cms.Path(process.demo)
