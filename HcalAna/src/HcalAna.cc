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
// $Id: HcalAna.cc,v 1.1 2012/06/14 21:38:34 sckao Exp $
//
//


#include "HcalAna.h"
//
// constructors and destructor
//
using namespace edm;
using namespace std;

HcalAna::HcalAna(const edm::ParameterSet& iConfig) {

   //now do what ever initialization is needed
   debug_        = iConfig.getParameter<bool>("debug") ;
   rootFileName  = iConfig.getUntrackedParameter<string> ("rootFileName");
   HBHERecHits   = iConfig.getParameter<edm::InputTag> ("HBHERecHits") ;
   zsThreshold   = iConfig.getParameter<double>("ZSThrehold") ;
   muonSource    = iConfig.getParameter<edm::InputTag> ("MuonSource") ;
   muonCuts      = iConfig.getParameter< vector<double> >("MuonCuts") ;
   vtxSource     = iConfig.getParameter<edm::InputTag> ("VertexSource") ;
   vtxCuts       = iConfig.getParameter< vector<double> >("VertexCuts") ;
   jetSource     = iConfig.getParameter<edm::InputTag> ("jetSource");
   jetCuts              = iConfig.getParameter<std::vector<double> >("jetCuts");


   // set the root file and ntuple tree
   theFile  = new TFile( rootFileName.c_str(), "RECREATE") ;
   theFile->cd () ;
   theTree  = new TTree ( "HcalUpgrade","HcalUpgrade" ) ;
   setBranches( theTree, theLeaves ) ;

}


HcalAna::~HcalAna() {
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
   theFile->cd () ; 
   theTree->Write() ;
   theFile->Close() ;
}


//
// member functions
//

// ------------ method called for each event  ------------
void HcalAna::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {


   initializeBranches( theTree, theLeaves );

   Handle<reco::PFJetCollection>      jets;
   iEvent.getByLabel( jetSource,      jets  );

   theLeaves.runId       = iEvent.id().run() ;
   theLeaves.eventId     = iEvent.id().event() ;

   GetGenEvent( iEvent, theLeaves, debug_ ) ;

   vector<MuonSummary> mlist ;
   GetRecoMuons( iEvent, theLeaves, mlist ) ;

   CheckVertex( iEvent, theLeaves ) ;
   JetSelection( jets, theLeaves ) ;

   theTree->Fill();

}

void HcalAna::CheckVertex( const edm::Event& iEvent, Ntuple& leaves ) {

   edm::Handle<reco::VertexCollection> vtx;
   iEvent.getByLabel( vtxSource, vtx);

   int nVtx = 0;
   for(reco::VertexCollection::const_iterator v=vtx->begin();  v!=vtx->end() ; v++){

       if ( ! v->isValid() ||  v->isFake() ) continue ;
       if ( fabs(v->z()) >= vtxCuts[0] ) continue ; 
       if (   v->ndof()   < vtxCuts[1] ) continue ;
       double d0 = sqrt( ( v->x()*v->x() ) + ( v->y()*v->y() ) );
       if ( d0 >= vtxCuts[2] ) continue ;
       /*
       if ( nVtx >= MAXVTX ) break ;

       leaves.vtxNTracks[nVtx]= v->tracksSize();
       leaves.vtxChi2[nVtx] =   v->chi2();
       leaves.vtxNdof[nVtx] =   v->ndof();
       leaves.vtxX[nVtx] =      v->x();
       leaves.vtxY[nVtx] =      v->y();
       leaves.vtxZ[nVtx] =      v->z();
       leaves.vtxDx[nVtx] =     v->xError();
       leaves.vtxDy[nVtx] =     v->yError();
       leaves.vtxDz[nVtx] =     v->zError();
       */ 
       nVtx++ ;
   }
   leaves.nVertices = nVtx ;
}

bool HcalAna::JetSelection( Handle<reco::PFJetCollection> jets, Ntuple& leaves ) {

   int k = 0 ;
   for(reco::PFJetCollection::const_iterator it = jets->begin(); it != jets->end(); it++) {
       // fiducial cuts
       if ( it->pt() < jetCuts[0] || fabs( it->eta() ) > jetCuts[1] ) continue ;

       // Loose JetID cuts
       if ( it->numberOfDaughters() < 2 )                   continue ;
       if ( it->neutralHadronEnergyFraction() >= 0.99 )     continue ;
       if ( it->neutralEmEnergyFraction()     >= 0.99 )     continue ;
       if ( fabs( it->eta() ) < 2.4 && it->chargedEmEnergyFraction()     >= 0.99 ) continue ;
       if ( fabs( it->eta() ) < 2.4 && it->chargedHadronEnergyFraction() <= 0    ) continue ;
       if ( fabs( it->eta() ) < 2.4 && it->chargedMultiplicity()         <= 0    ) continue ;
       

       if ( k >= MAXJET ) break ;
       leaves.jetPx[k] = it->p4().Px() ;
       leaves.jetPy[k] = it->p4().Py() ;
       leaves.jetPz[k] = it->p4().Pz() ;
       leaves.jetE[k]  = it->p4().E()  ;
       leaves.jetNDau[k] = it->numberOfDaughters() ;

       leaves.jetCM[k]   = it->chargedMultiplicity() ;
       leaves.jetNM[k]   = it->neutralMultiplicity() ;
       leaves.jetMM[k]   = it->muonMultiplicity() ;

       leaves.jetCEF[k]  = it->chargedEmEnergyFraction() ;
       leaves.jetCHF[k]  = it->chargedHadronEnergyFraction() ;
       leaves.jetNHF[k]  = it->neutralHadronEnergyFraction() ;
       leaves.jetNEF[k]  = it->neutralEmEnergyFraction() ;
       leaves.jetMEF[k]  = it->muonEnergyFraction() ;
       k++ ;
   }
   leaves.nJets = k ;

   if ( k > jetCuts[2] ) return true  ;
   else                  return false ;

}


void HcalAna::LoopHCAL( const edm::Event& iEvent, int muId, TLorentzVector muP4, Ntuple& leaves, bool isReco ) {

   edm::Handle<HBHERecHitCollection> hbhe;
   iEvent.getByLabel( HBHERecHits, hbhe);

   //int jj = 0 ;
   for (HBHERecHitCollection::const_iterator ihit=hbhe->begin(); ihit!=hbhe->end(); ihit++) {

      // first, zero suppression!
      if ( ihit->energy() < zsThreshold ) continue;

      // get those info from hcalRechits from hit_id
      int ieta=ihit->id().ieta();
      double eta_hit=2.0*M_PI/72.0*ieta+((ieta>0)?(-M_PI/72.0):(M_PI/72.0));
      int iphi=ihit->id().iphi();
      double phi_hit=( iphi <= 36 ) ?   (iphi/72.0)*2.0*M_PI : ( (iphi - 73) /72.0)*2.0*M_PI ; 
      //double phi_hit= (iphi/72.0)*2.0*M_PI  ; 
      double hit_et=ihit->energy()/cosh(eta_hit);
 
      // construct the 4-vec for hcal hits
      ROOT::Math::RhoEtaPhiVector v1(1.0,eta_hit,phi_hit);
      TLorentzVector hP4( v1.X(), v1.Y(), v1.Z(), 1.0 );
      hP4 = hP4* ihit->energy() ; 

      double dR = muP4.DeltaR( hP4 )  ;

      /* 
      if ( jj == 0 ) { 
         for ( int kk=1 ; kk <= 36 ; kk++ ) {
             double phi_i = kk*1.0 ;
	     double phi_j = kk*(-1.0) ; 
	     double phi1 = ( phi_i / 72.)*2.*M_PI ;
	     double phi2 = (phi_j / 72.)*2.*M_PI ;
	     ROOT::Math::RhoEtaPhiVector v_360(1.0, 1., phi1 );
	     ROOT::Math::RhoEtaPhiVector v_180(1.0, 1., phi2 );
	     TLorentzVector testV1( v_360.X(), v_360.Y(), v_360.Z(), 1.0 );
	     TLorentzVector testV2( v_180.X(), v_180.Y(), v_180.Z(), 1.0 );

	     double dR1 = muP4.DeltaR( testV1 )  ;
	     double dR2 = muP4.DeltaR( testV2 )  ;
	     double dF1 = muP4.DeltaPhi( testV1 )  ;
	     double dF2 = muP4.DeltaPhi( testV2 )  ;

	     cout<<" v360_phi: "<< v_360.Phi() <<"  v180_phi: "<< v_180.Phi() <<endl ;
	     cout<<" f1 : "<< testV1.Phi() <<"  f2 :"<< testV2.Phi() << endl ;
	     cout<<" muP4 phi : "<< muP4.Phi() <<endl ;
	     cout<<" dF1 : "<< dF1 <<"   dF2 : "<< dF2 <<"  ddF ="<< dF1 - dF2 <<endl ;
	     cout<<" dR1 : "<< dR1 <<"   dR2 : "<< dR2 <<"  ddR ="<< dR1 - dR2 <<endl ;
	     cout<<" ------------------------ "<<endl;
	 }
     }
     jj++ ;
     */

      //printf(" h: %f, depth: %d \n", hP4.Eta(), ihit->id().depth()  ) ;

      for (int idep=0; idep<5; idep++) {
          //if ( idep ==0 ) cout<<" L0 T : "<< ihit->time() <<endl ;  
          //if ( abs(ieta) <= 16 && idep > 4  )  continue ;
          //int hit_depth = (ihit->id().depth() < 4) ? ihit->id().depth() : 3  ;
          int hit_depth = ihit->id().depth() - 1 ;

          if ( isReco ) {  
             if ( hit_depth == idep && dR < 0.5 ) leaves.muIso5[muId][idep] += hit_et ;
	     if ( hit_depth == idep && dR < 0.4 ) leaves.muIso4[muId][idep] += hit_et ;
	     if ( hit_depth == idep && dR < 0.3 ) leaves.muIso3[muId][idep] += hit_et ;
	     if ( hit_depth == idep && dR < 0.2 ) leaves.muIso2[muId][idep] += hit_et ;
	     if ( hit_depth == idep && dR < 0.1 ) leaves.muIso1[muId][idep] += hit_et ;

             if ( hit_depth == idep && dR < 0.5 ) leaves.muIhit5[muId][idep] += 1 ;
	     if ( hit_depth == idep && dR < 0.4 ) leaves.muIhit4[muId][idep] += 1 ;
	     if ( hit_depth == idep && dR < 0.3 ) leaves.muIhit3[muId][idep] += 1 ;
	     if ( hit_depth == idep && dR < 0.2 ) leaves.muIhit2[muId][idep] += 1 ;
	     if ( hit_depth == idep && dR < 0.1 ) leaves.muIhit1[muId][idep] += 1 ;
	  } else {
             if ( hit_depth == idep && dR < 0.5 ) leaves.genIso5[muId][idep] += hit_et ;
	     if ( hit_depth == idep && dR < 0.4 ) leaves.genIso4[muId][idep] += hit_et ;
	     if ( hit_depth == idep && dR < 0.3 ) leaves.genIso3[muId][idep] += hit_et ;
	     if ( hit_depth == idep && dR < 0.2 ) leaves.genIso2[muId][idep] += hit_et ;
	     if ( hit_depth == idep && dR < 0.1 ) leaves.genIso1[muId][idep] += hit_et ;

             if ( hit_depth == idep && dR < 0.5 ) leaves.genIhit5[muId][idep] += 1 ;
	     if ( hit_depth == idep && dR < 0.4 ) leaves.genIhit4[muId][idep] += 1 ;
	     if ( hit_depth == idep && dR < 0.3 ) leaves.genIhit3[muId][idep] += 1 ;
	     if ( hit_depth == idep && dR < 0.2 ) leaves.genIhit2[muId][idep] += 1 ;
	     if ( hit_depth == idep && dR < 0.1 ) leaves.genIhit1[muId][idep] += 1 ;
	  }
      } 
   }

}

void HcalAna::GetRecoMuons( const edm::Event& iEvent, Ntuple& leaves, vector<MuonSummary>& mlist ) {

   edm::Handle<reco::MuonCollection> muons; 
   iEvent.getByLabel( muonSource, muons);

   int nMu = 0 ; 
   for(reco::MuonCollection::const_iterator it = muons->begin(); it != muons->end(); it++) {
      if ( it->pt() < muonCuts[0] || fabs( it->eta() ) > muonCuts[1] ) continue ;
      if ( nMu >= 10 ) continue ;
      //if ( ! it->isCaloMuon() ) continue ;

      // keep selected muons for other functions
      MuonSummary theMu ;
      theMu.id = nMu ;
      theMu.p4 = TLorentzVector( it->p4().px(), it->p4().py(), it->p4().pz(), it->energy() );
      mlist.push_back( theMu ) ; 

      // record in ntuples
      leaves.muPx[nMu] = it->p4().px() ;  
      leaves.muPy[nMu] = it->p4().py() ;  
      leaves.muPz[nMu] = it->p4().pz() ;  
      leaves.muE[nMu]  = it->energy() ;  

      // get HCAL iso-deposit  
      LoopHCAL( iEvent, nMu, theMu.p4, leaves, true ) ;

      nMu++;
   }
   leaves.nMuons = nMu ; 

}

int HcalAna::TraceMom( HepMC::GenVertex* &vtx, bool debug ) {

     int theMomId =  0 ; 

     HepMC::GenVertex::particles_in_const_iterator  vbegin = vtx->particles_in_const_begin() ;
     HepMC::GenVertex::particles_in_const_iterator    vend = vtx->particles_in_const_end() ;
     for ( HepMC::GenVertex::particles_in_const_iterator i1 = vbegin ; i1 != vend ; ++i1 ) {

	 if ( debug ) printf(" --> Come in vtx * pid: %d, status : %d \n", (*i1)->pdg_id(), (*i1)->status()  ) ;     
         if ( abs((*i1)->pdg_id()) == 13  ) {
            HepMC::GenVertex* momVert = (*i1)->production_vertex();
	    HepMC::GenVertex::particles_in_const_iterator  mbegin = momVert->particles_in_const_begin() ;
	    HepMC::GenVertex::particles_in_const_iterator    mend = momVert->particles_in_const_end() ;
	    for ( HepMC::GenVertex::particles_in_const_iterator j1 = mbegin ; j1 != mend ; ++j1 ) {
		theMomId = (*j1)->pdg_id() ; 
                if ( debug ) printf(" --> The Mom vtx * pid: %d, status : %d \n", (*j1)->pdg_id(), (*j1)->status()  ) ;     
	    }
            vtx = (*i1)->production_vertex() ;
         } else {
		theMomId = (*i1)->pdg_id() ; 
                if ( debug ) printf(" --> The Mom vtx * pid: %d, status : %d \n", (*i1)->pdg_id(), (*i1)->status()  ) ;     
	 }
     }
     return theMomId ;

}

void HcalAna::GetGenEvent( const edm::Event& iEvent, Ntuple& leaves, bool debug ) {

   edm::Handle<edm::HepMCProduct> HepMCEvt;
   iEvent.getByLabel("generator",HepMCEvt);

   const HepMC::GenEvent* genevent = HepMCEvt->GetEvent();
   HepMC::GenEvent::particle_const_iterator begin = genevent->particles_begin() ;
   HepMC::GenEvent::particle_const_iterator   end = genevent->particles_end() ;

   //   hepMCEvt->GetEvent()->print();
   int mId = 0 ;
   for ( HepMC::GenEvent::particle_const_iterator it = begin; it != end; ++it ) {
         
	 // Identify final state Muon 
         HepMC::GenParticle* p = *it;
         if ( abs( p->pdg_id() ) != 13 || p->status() != 1  ) continue ;

         // Get muon's production vertex and momentum
         HepMC::GenVertex* genVert  = p->production_vertex();
         HepMC::GenVertex* genVert_ = p->production_vertex();
         HepMC::FourVector p4 = p->momentum();
         TLorentzVector theGenP4 = TLorentzVector( p4.px(), p4.py(), p4.pz(), p4.e() );
         //if ( theGenP4->Pt() < muonCuts[0] || fabs( theGenP4->Eta() ) > muonCuts[1] ) continue ;

         if ( debug ) cout<<" ------------------- "<<endl;
         // check muon's parents, parent particle is the come-in particle of the vertex
	 // keep searching upward until find the real mom   
	 int momId = 13 ;
	 while ( abs(momId) == 13 ) {
	       momId = TraceMom( genVert_, debug ) ;
	 }

         leaves.pdgId[mId] = p->pdg_id() ;
         leaves.genPx[mId] = p4.px();
         leaves.genPy[mId] = p4.py();
         leaves.genPz[mId] = p4.pz();
         leaves.genE[mId]  = p4.e();
         leaves.genVx[mId] = genVert->position().x();
         leaves.genVy[mId] = genVert->position().y();
         leaves.genVz[mId] = genVert->position().z();
         leaves.momId[mId] = momId ;

         // get HCAL iso-deposit  
         LoopHCAL( iEvent, mId, theGenP4, leaves, false ) ;

         // kids from this vetex 
	 if ( debug ) {  
            HepMC::GenVertex::particles_out_const_iterator  ubegin = genVert->particles_out_const_begin() ;
	    HepMC::GenVertex::particles_out_const_iterator    uend = genVert->particles_out_const_end() ;
	    for ( HepMC::GenVertex::particles_in_const_iterator i2 = ubegin ; i2 != uend ; ++i2 ) {
	        printf(" Out going vtx * ==> pid: %d, status : %d \n", (*i2)->pdg_id(), (*i2)->status()  ) ;     
  	    } 
	 }

         mId++ ;
   }
   leaves.nGen = mId  ;
}

// ------------ method called once each job just before starting event loop  ------------
/* 
void 
HcalAna::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
HcalAna::endJob() 
{
}

// ------------ method called when starting to processes a run  ------------
void 
HcalAna::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
HcalAna::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
HcalAna::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
HcalAna::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
HcalAna::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
*/

//define this as a plug-in
DEFINE_FWK_MODULE(HcalAna);
