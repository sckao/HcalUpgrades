import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        'file:/local/cms/user/sckao/tt_pu50_2.root',
        'file:/local/cms/user/sckao/tt_pu50_3.root',
        'file:/local/cms/user/sckao/tt_mixtest1.root',
        'file:/local/cms/user/sckao/tt_mixtest2.root'
    )
)

process.demo = cms.EDAnalyzer('HcalAna',
    rootFileName = cms.untracked.string('shcal_mixtest.root'),
    debug        = cms.bool(False),
    HBHERecHits  = cms.InputTag('hbhereco'),
    ZSThrehold   = cms.double(0.25),
    MuonSource   = cms.InputTag('muons'),
    # muon cuts                 pt  eta   
    MuonCuts     = cms.vdouble( 10, 2.5 ),
    VertexSource = cms.InputTag('offlinePrimaryVerticesWithBS'),
    # vertex cuts                z   ndof   d0
    VertexCuts   = cms.vdouble( 99.,   0,   99 )
)


process.p = cms.Path(process.demo)
