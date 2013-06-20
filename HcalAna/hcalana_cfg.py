import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring( 'file:/local/cms/phedex/store/mc/Summer12/PYTHIA6_Tauola_TTbar_TuneZ2star_14TeV/GEN-SIM-DIGI-RECO/UpgradeHCAL_PixelPhase1_DR428-PU50-DESIGN42_V17S-v2/0000/file.root'

    )
)

process.demo = cms.EDAnalyzer('HcalAna',
    rootFileName = cms.untracked.string('shcal_tt_test.root'),
    debug        = cms.bool(False),
    HBHERecHits  = cms.InputTag('hbhereco'),
    ZSThrehold   = cms.double(0.25),
    VertexSource = cms.InputTag('offlinePrimaryVerticesWithBS'),
    MuonSource   = cms.InputTag('muons'),
    ElectronSource = cms.InputTag('gsfElectrons'),
    JetSource    = cms.InputTag("ak5PFJets"),
    Target_pdgId = cms.int32(11),

    # vertex cuts                z   ndof   d0
    VertexCuts   = cms.vdouble( 99.,   0,   99 ),
    # muon cuts                 pt  eta   
    MuonCuts     = cms.vdouble( 10, 2.5 ),
    # electron cuts             pt  eta   
    ElectronCuts = cms.vdouble( 10, 2.5 ),
    # jet cuts                  pt    eta  nJets
    JetCuts      = cms.vdouble( 30. , 2.4,    0 )

)


process.p = cms.Path(process.demo)
