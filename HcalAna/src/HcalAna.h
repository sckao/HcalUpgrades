#ifndef HcalAna_H
#define HcalAna_H

// -*- C++ -*-
//
// Package:    HcalAna
// Class:      HcalAna
// 
/**\class HcalAna HcalAna.cc MyWorks/HcalAna/src/HcalAna.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Shih-Chuan Kao
//         Created:  Tue May 29 10:59:50 CDT 2012
// $Id: HcalAna.h,v 1.1 2012/06/14 21:38:34 sckao Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "HepMC/GenParticle.h"
#include "HepMC/SimpleVector.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"

#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"

#include "DataFormats/Math/interface/LorentzVector.h"
#include <TMath.h>
#include <Math/VectorUtil.h>
#include <Math/Vector3D.h>


#include "TFile.h"
#include "TTree.h"
#include "Ntuple.h"
#include "TLorentzVector.h"

//
// class declaration
//
using namespace std ;

struct MuonSummary {
    int id;
    TLorentzVector p4;
};

class HcalAna : public edm::EDAnalyzer {

   public:
      explicit HcalAna(const edm::ParameterSet&);
      ~HcalAna();

      //static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      void GetGenEvent( const edm::Event& , Ntuple& leaves, bool debug = false ) ;
      void LoopHCAL( const edm::Event& , int muId, TLorentzVector muP4, Ntuple& leaves, bool isReco ) ;
      int  TraceMom( HepMC::GenVertex* &vtx, bool debug = false ) ;
      void GetRecoMuons( const edm::Event& , Ntuple& leaves, vector<MuonSummary>& mlist ) ;
      void CheckVertex( const edm::Event& , Ntuple& leaves );
      bool JetSelection( edm::Handle<reco::PFJetCollection> jets, Ntuple& leaves );

      /*
      virtual void beginJob() ;
      virtual void endJob() ;

      virtual void beginRun(edm::Run const&, edm::EventSetup const&);
      virtual void endRun(edm::Run const&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
      */
      // ----------member data ---------------------------
      Ntuple theLeaves ;
      TTree *theTree;
      TFile *theFile;

      string rootFileName;
      bool   debug_ ;
      double zsThreshold ;
      edm::InputTag HBHERecHits ; 
      edm::InputTag muonSource ;
      edm::InputTag jetSource ;
      edm::InputTag vtxSource ;
      std::vector<double> muonCuts ;  
      std::vector<double> vtxCuts ;  
      std::vector<double> jetCuts ;  
};

#endif
