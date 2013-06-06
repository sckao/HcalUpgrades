#include <TLegend.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TGraphErrors.h>

#include "HcalAna.h"

HcalAna::HcalAna( string datacardfile ){

  Input  = new AnaInput( datacardfile );
  h_draw = new hDraw( datacardfile ); 
 
  SkipEvents = 0 ;
  Input->GetParameters("PlotType",      &plotType ) ; 
  Input->GetParameters("Path",          &hfolder ) ; 
  Input->GetParameters("ProcessEvents", &ProcessEvents ) ; 
  Input->GetParameters("SkipEvents",    &SkipEvents ) ;
  Input->GetParameters("MuonCuts",      &muonCuts ) ; 
  Input->GetParameters("JetCuts",       &jetCuts ) ; 
  Input->GetParameters("VtxCuts",       &vtxCuts ) ; 
  Input->GetParameters("IsoMethod",     &isoMethod ) ; 
  Input->GetParameters("HistoName",     &hfName ) ;
  Input->GetParameters("ScaleFile",     &scaleFile ) ;

}

HcalAna::~HcalAna(){

  delete h_draw ;
  delete Input ;
  cout<<" done ! "<<endl ;

}

// analysis template
void HcalAna::ReadTree( string dataName, bool reScale  ) { 

   // create histogram files 
   gSystem->mkdir( hfolder.c_str() );
   TString Path_fName = hfolder + hfName + ".root" ;
   theFile = new TFile( Path_fName, "RECREATE" );
   theFile->cd() ;

   // read scale Factor 
   if ( reScale ) ReadMuonPtReWeighting() ;

   // read the ntuple tree 
   string dataFileNames ;
   if ( dataName != "0" ) {
      dataFileNames = dataName ;
   } else {
      Input->GetParameters( "TheData", &dataFileNames );
   }
   TTree* tr   = Input->GetTree( dataFileNames, "HcalUpgrade" );

   // set addresses for variables
   setBranchAddresses( tr, leaves );

   int totalN = tr->GetEntries();
   cout<<" from  "<< dataName <<" total entries = "<< totalN <<" Process "<< ProcessEvents <<endl;
   if ( isoMethod == 0 ) cout <<" Iso Deposit from each layer "<<endl ;
   if ( isoMethod == 1 ) cout <<" Iso Deposit from min/max layer "<<endl ;
   if ( isoMethod == 2 ) cout <<" Iso Deposit from inclusive layers "<<endl ;

   // booking histograms 
   h_nVtx  = new TH1D("h_nVtx",  " number of vertices ",   100, 0, 100 );
   h_nMu   = new TH1D("h_nMu",   " number of reco Muons ", 15, 0, 15 );
   h_nJet  = new TH1D("h_nJet",  " number of jets ", 15, 0, 15 );
   h_wmuE  = new TH1D("h_wmuE",  " W Muon E distribution  ", 50, 0, 500 );
   h_bmuE  = new TH1D("h_bmuE",  " b Muon E distribution  ", 50, 0, 500 );
   h_wmuPt = new TH1D("h_wmuPt", " W Muon Pt distribution ", 50, 0, 250 );
   h_bmuPt = new TH1D("h_bmuPt", " b Muon Pt distribution ", 50, 0, 250 );
   h_JetPt = new TH1D("h_JetPt", " Jet Pt distribution  ", 50, 0, 500 );

   // histograms for various isolation deposit
   double absiso_max = 50 ;
   double reliso_max =  5 ;
   double isohit_max = 50 ;

   for ( int j=0; j<4 ; j++ ) {
       for ( int k=0; k<5 ; k++ ) {
           char hname_a[20], htitle_a[80], hname_r[20], htitle_r[80], hname_h[20], htitle_h[80]  ;
           sprintf( hname_a,   "w_abs_D%dR%d", j+1, k+1 ) ;
           sprintf( htitle_a,  "Abs Iso of muon from W at depth %d in dR < 0.%d", j+1, k+1 ) ;
           sprintf( hname_r,   "w_rel_D%dR%d", j+1, k+1 ) ;
           sprintf( htitle_r,  "Rel Iso of muon from W at depth %d in dR < 0.%d", j+1, k+1 ) ;
           sprintf( hname_h,   "w_hit_D%dR%d", j+1, k+1 ) ;
           sprintf( htitle_h,  "Iso hits of muon from W at depth %d in dR < 0.%d", j+1, k+1 ) ;

           wH.h_abs[j][k] = new TH1D( hname_a, htitle_a, 100, 0, absiso_max );
           wH.h_rel[j][k] = new TH1D( hname_r, htitle_r, 100, 0, reliso_max );
           wH.h_hit[j][k] = new TH1D( hname_h, htitle_h, 50, 0,  isohit_max );

           sprintf( hname_a,   "b_abs_D%dR%d", j+1, k+1 ) ;
           sprintf( htitle_a,  "Abs Iso of muon from b at depth %d in dR < 0.%d", j+1, k+1 ) ;
           sprintf( hname_r,   "b_rel_D%dR%d", j+1, k+1 ) ;
           sprintf( htitle_r,  "Rel Iso of muon from b at depth %d in dR < 0.%d", j+1, k+1 ) ;
           sprintf( hname_h,   "b_hit_D%dR%d", j+1, k+1 ) ;
           sprintf( htitle_h,  "Iso hits of muon from b at depth %d in dR < 0.%d", j+1, k+1 ) ;

           bH.h_abs[j][k] = new TH1D( hname_a, htitle_a, 100, 0, absiso_max );
           bH.h_rel[j][k] = new TH1D( hname_r, htitle_r, 100, 0, reliso_max );
           bH.h_hit[j][k] = new TH1D( hname_h, htitle_h, 50, 0,  isohit_max );

           sprintf( hname_a,   "r_abs_D%dR%d", j+1, k+1 ) ;
           sprintf( htitle_a,  "Abs Iso of reco muon at depth %d in dR < 0.%d", j+1, k+1 ) ;
           sprintf( hname_r,   "r_rel_D%dR%d", j+1, k+1 ) ;
           sprintf( htitle_r,  "Rel Iso of reco muon at depth %d in dR < 0.%d", j+1, k+1 ) ;
           sprintf( hname_h,   "r_hit_D%dR%d", j+1, k+1 ) ;
           sprintf( htitle_h,  "Iso hits of reco muon at depth %d in dR < 0.%d", j+1, k+1 ) ;

           rH.h_abs[j][k] = new TH1D( hname_a, htitle_a, 100, 0, absiso_max );
           rH.h_rel[j][k] = new TH1D( hname_r, htitle_r, 100, 0, reliso_max );
           rH.h_hit[j][k] = new TH1D( hname_h, htitle_h, 50, 0,  isohit_max );

           sprintf( hname_a,   "j_abs_D%dR%d", j+1, k+1 ) ;
           sprintf( htitle_a,  "Abs Iso of jets at depth %d in dR < 0.%d", j+1, k+1 ) ;
           sprintf( hname_r,   "j_rel_D%dR%d", j+1, k+1 ) ;
           sprintf( htitle_r,  "Rel Iso of jets at depth %d in dR < 0.%d", j+1, k+1 ) ;
           sprintf( hname_h,   "j_hit_D%dR%d", j+1, k+1 ) ;
           sprintf( htitle_h,  "Iso hits of jets at depth %d in dR < 0.%d", j+1, k+1 ) ;

           jH.h_abs[j][k] = new TH1D( hname_a, htitle_a, 100, 0, absiso_max );
           jH.h_rel[j][k] = new TH1D( hname_r, htitle_r, 100, 0, reliso_max );
           jH.h_hit[j][k] = new TH1D( hname_h, htitle_h, 50, 0,  isohit_max );

       }
   }
   cout<<" Created all histograms !! "<<endl ;

   // Collect reco and gen muons
   vector<objID> recoMuColl ;
   vector<objID> genMuColl ;
   // looping over the events
   int nEvt = 0 ;
   int beginEvent = SkipEvents + 1 ;
   cout<<" Event start from : "<< beginEvent <<" skip "<< SkipEvents <<" events "<< endl ;

   for ( int i= beginEvent ; i< totalN ; i++ ) {

       if ( ProcessEvents > 0 && i > ( ProcessEvents + beginEvent - 1 ) ) break;

       tr->GetEntry( i );

       if ( i % 10000 == 0 && i > 9999 ) printf(" ----- processed %8d Events \n", i ) ;
       //cout<<" Evt# : "<< nEvt <<endl; 

       nEvt++; 
       h_nVtx->Fill( leaves.nVertices ) ;
       // select event in different no. of vertices.
       if ( leaves.nVertices > vtxCuts[1] || leaves.nVertices <= vtxCuts[0]  ) continue ;
    
       h_nMu->Fill( leaves.nMuons ) ;

       // loop all reco muons in the event
       for ( int k=0; k< leaves.nMuons; k++){
           TLorentzVector mP4( leaves.muPx[k], leaves.muPy[k], leaves.muPz[k], leaves.muE[k] )  ;
           
           if ( mP4.Pt() < muonCuts[0] || mP4.Pt() > muonCuts[1] ) continue ;
           if ( fabs( mP4.Eta() ) > muonCuts[2] ) continue ;
           if ( ( leaves.muIhit5[k][0] + leaves.muIhit5[k][1] + leaves.muIhit5[k][2] ) < 1 ) continue ;

           // Collect Reco Muons
           objID i_mu(13,k, mP4 ) ;
           recoMuColl.push_back( i_mu ) ;
       }
       // get gen muon information 
       for ( int k=0; k< leaves.nGen; k++) {
           TLorentzVector gP4 = TLorentzVector( leaves.genPx[k], leaves.genPy[k], leaves.genPz[k], leaves.genE[k] ) ;
           if ( gP4.Pt() < muonCuts[0] || gP4.Pt() > muonCuts[1] ) continue ;
           if ( fabs( gP4.Eta() ) > muonCuts[2] ) continue ;
           if ( ( leaves.genIhit5[k][0] + leaves.genIhit5[k][1] + leaves.genIhit5[k][2] ) < 1 ) continue ;

           // Collect Gen Muons
           objID i_mu( leaves.pdgId[k] ,k, gP4 ) ;
           genMuColl.push_back( i_mu ) ;

           // Get Scaling factor for background muons -  normalize bg muon pt spectrum to W muons'
           double scaleFact = ( reScale ) ? GetMuonPtReWeighting( gP4.Pt() ) : 1. ;

           if ( abs( leaves.momId[k]) == 24 ) {

              // looping 4 different depths 
              for ( int j=0; j<3 ; j++ ) {
                  // looping 5 different dR 
                  for ( int r=1; r < 6; r++ ) {

                      double theAbsIso_ = IsoDeposit( "gen", k, j, r ) ;
                      double theAbsIso  = ( theAbsIso_ > absiso_max ) ? absiso_max - 0.1 : theAbsIso_ ;
                      double theRelIso_ = theAbsIso_ / gP4.Pt()   ;
                      double theRelIso  = ( theRelIso_ > reliso_max ) ? reliso_max - 0.01 : theRelIso_ ;
                      int    theIsohit_ = IsoHits( "gen", k, j, r ) ;
                      int    theIsohit  = ( theIsohit_ >= isohit_max ) ? isohit_max - 1 : theIsohit_ ;

                      wH.h_abs[j][r-1]->Fill( theAbsIso ) ;
		      wH.h_rel[j][r-1]->Fill( theRelIso ) ;
		      wH.h_hit[j][r-1]->Fill( theIsohit ) ;
                  }
              }   
              //cout<<" --------------- "<<endl ;
           } else {

              // Remove the kicked out muons - muon is away from jet
              double dR = 999. ;
              for ( int j =0; j< leaves.nJets ; j++) {
                   TLorentzVector jP4 = TLorentzVector( leaves.jetPx[j], leaves.jetPy[j], leaves.jetPz[j], leaves.jetE[j] ) ;
                   if ( jP4.Pt() < 30. ) continue ;
                   double dR_ = jP4.DeltaR( gP4 );  
                   dR = ( dR_ < dR ) ? dR_ : dR ;
              }
              if ( dR > 0.5 ) continue ;

              for ( int j=0; j<3 ; j++ ) {
                  for ( int r=1; r < 6; r++ ) {

                      double theAbsIso_ = IsoDeposit( "gen", k, j, r ) ;
                      double theAbsIso  = ( theAbsIso_ > absiso_max ) ? absiso_max - 0.1 : theAbsIso_ ;
                      double theRelIso_ = theAbsIso_ / gP4.Pt()   ;
                      double theRelIso  = ( theRelIso_ > reliso_max ) ? reliso_max - 0.01 : theRelIso_ ;
                      int    theIsohit_ = IsoHits( "gen", k, j, r ) ;
                      int    theIsohit  = ( theIsohit_ >= isohit_max ) ? isohit_max - 1 : theIsohit_ ;

                      bH.h_abs[j][r-1]->Fill( theAbsIso, scaleFact ) ;
		      bH.h_rel[j][r-1]->Fill( theRelIso, scaleFact ) ;
		      bH.h_hit[j][r-1]->Fill( theIsohit, scaleFact ) ;
                  }
              }   
           }
       }

       // Match reco and gen muons
       vector<iMatch> matchV = GlobalDRMatch( recoMuColl, genMuColl ) ;
       for ( size_t k=0; k < matchV.size(); k++ ) {
           if ( matchV[k].gid < 0  ) continue ; // exclude un-matched muon
 
           int genMomId =  leaves.momId[  matchV[k].ig ] ;
           //if ( abs(genMomId) != 24 ) continue ; // exclude those reco muon with "non-W boson" parents 
           int ir = matchV[k].ir ;
           if ( abs(genMomId) != 24 ) {
              h_bmuPt->Fill(  recoMuColl[ matchV[k].rid ].p4.Pt() ) ;
              h_bmuE->Fill(   recoMuColl[ matchV[k].rid ].p4.E() ) ;
           } else {
              h_wmuPt->Fill(  recoMuColl[ matchV[k].rid ].p4.Pt() ) ;
              h_wmuE->Fill(   recoMuColl[ matchV[k].rid ].p4.E() ) ;
           }
           // looping 4 different depths 
           for ( int j=0; j<3 ; j++ ) {
               // looping 5 different dR 
               for ( int r=1; r < 6; r++ ) {
                   double theAbsIso_ = IsoDeposit( "muon", ir, j, r ) ;
		   double theAbsIso  = ( theAbsIso_ > absiso_max ) ? absiso_max - 0.1 : theAbsIso_ ;
		   double theRelIso_ = theAbsIso_ / recoMuColl[ matchV[k].rid ].p4.Pt()   ;
		   double theRelIso  = ( theRelIso_ > reliso_max ) ? reliso_max - 0.01 : theRelIso_ ;
		   int    theIsohit_ = IsoHits( "muon", ir, j, r ) ;
		   int    theIsohit  = ( theIsohit_ >= isohit_max ) ? isohit_max - 1 : theIsohit_ ;

                   if (  abs(genMomId) == 24 ) {
                      rH.h_abs[j][r-1]->Fill( theAbsIso ) ;
		      rH.h_rel[j][r-1]->Fill( theRelIso ) ;
		      rH.h_hit[j][r-1]->Fill( theIsohit ) ;
                   } else {
                      jH.h_abs[j][r-1]->Fill( theAbsIso ) ;
		      jH.h_rel[j][r-1]->Fill( theRelIso ) ;
		      jH.h_hit[j][r-1]->Fill( theIsohit ) ;

                   }
               }
           }
       }
       recoMuColl.clear() ;
       genMuColl.clear() ;

       // get jet hcal deposit      
       int nGoodJet = 0 ;
       for ( int k =0; k< leaves.nJets ; k++) {
           //if ( nGoodJet > 2 ) break ;

           TLorentzVector jP4 = TLorentzVector( leaves.jetPx[k], leaves.jetPy[k], leaves.jetPz[k], leaves.jetE[k] ) ;

           // Jet ID cuts
           if ( jP4.Pt()        < jetCuts[0] ) continue ;
           if ( fabs(jP4.Eta()) > jetCuts[1] ) continue ;
	   if ( leaves.jetNDau[k] < jetCuts[2]  )  continue ;
	   if ( leaves.jetCEF[k]  > jetCuts[3]  )  continue ;
	   if ( leaves.jetNEF[k]  > jetCuts[4]  )  continue ;
	   if ( leaves.jetCHF[k]  > jetCuts[5]  )  continue ;
	   if ( leaves.jetNHF[k]  > jetCuts[6]  )  continue ;
	   if ( leaves.jetMEF[k]  > jetCuts[7]  )  continue ;
	   if ( fabs( jP4.Eta() ) < 2.4 && leaves.jetCM[k]  < 1 ) continue ;
           h_JetPt->Fill( jP4.Pt() ) ;
           nGoodJet++ ;

           // looping thruogh 4 depths or 3 different typiesa
           /*
           for ( int j=0; j<3 ; j++ ) {
               // looping 5 different dR 
               //cout<<" Depth : "<< j+1 <<endl ;
               for ( int r=1; r < 6; ++r ) {

		   double theAbsIso_ = IsoDeposit( "jet", k, j, r ) ;
                   double theAbsIso  = ( theAbsIso_ > absiso_max ) ? absiso_max - 0.1 : theAbsIso_ ;
                   double theRelIso_ = theAbsIso_ / jP4.Pt()   ;
                   double theRelIso  = ( theRelIso_ > reliso_max ) ? reliso_max - 0.01 : theRelIso_ ;
                   int    theIsohit_ = IsoHits( "jet", k, j, r ) ;
                   int    theIsohit  = ( theIsohit_ > isohit_max ) ? isohit_max - 0.1 : theIsohit_ ;

                   jH.h_abs[j][r-1]->Fill( theAbsIso ) ;
                   jH.h_rel[j][r-1]->Fill( theRelIso ) ;
                   jH.h_hit[j][r-1]->Fill( theIsohit ) ;

               }
           }
           */
       }
       h_nJet->Fill( nGoodJet );
   } // end of event looping

   DrawHistograms() ;

   // record file in histogram format
   theFile->cd() ;
   HistoWrite( "", theFile ) ;
   cout<<" historams written ! "<<endl ;
   theFile->Close() ;
}

void HcalAna::DrawHistograms( ) { 
   // **************************************** 
   // *           Draw Histograms            *
   // **************************************** 

   // Calculate Background Ratio
   //string hTitle_Rate[3] = { "Depth 1+2+3", "Depth 2+3", "Depth 3" };
   // set histogram's attributions : axis, labelsize(0.05), tickLength(0.03), titleSize(0.04), titleOffset(1)

   h_draw->SetHistoAtt("X", 0.05, 0.04, 0.08, 0.5 ) ;
   h_draw->SetHistoAtt("Y", 0.06, 0.04, 0.06, 0 ) ;
   h_draw->SetPlotStyle( false );
   TLegend* leg1  = new TLegend(.7, .75, .95, .95 );
   leg1->SetTextSize(0.04);
   TGraphAsymmErrors* gBGRateA[3] ;
   TGraphAsymmErrors* gBGRateR[3] ;
   TGraphAsymmErrors* gBGRateH[3] ;
   // 1. background rate when signal eff = 95% 
   for (int idep = 0 ; idep < 3; idep++) {
       vector<iEff> dRV  ;
       vector<iEff> A_bgRV, R_bgRV, H_bgRV ;
       double count = 0.1 ;
       for ( int idR = 0 ; idR < 5; idR++ ) {
           iEff bgR_a = BgRatio( wH.h_abs[idep][idR], bH.h_abs[idep][idR] ) ;
           iEff bgR_r = BgRatio( wH.h_rel[idep][idR], bH.h_rel[idep][idR] ) ;
           iEff bgR_h = BgRatio( wH.h_hit[idep][idR], bH.h_hit[idep][idR] ) ;
           iEff dRE( count, 0, 0) ;

           dRV.push_back( dRE ) ;    
           A_bgRV.push_back( bgR_a ) ;    
           R_bgRV.push_back( bgR_r ) ;    
           H_bgRV.push_back( bgR_h ) ;    

           count += 0.1 ;
       }

       char gName[15] ;
       sprintf( gName, "BgRate_%d", idep+1 ) ;
       h_draw->FillGraph( gBGRateH[idep], dRV, H_bgRV, "dR", "", 0.1, 1.1, 6 ) ;
       h_draw->FillGraph( gBGRateA[idep], dRV, A_bgRV, "dR", "", 0.1, 1.1, 2 ) ;
       h_draw->FillGraph( gBGRateR[idep], dRV, R_bgRV, "dR", "", 0.1, 1.1, 4 ) ;

       // drawing histograms for background rate
       leg1->Clear();
       leg1->AddEntry( gBGRateH[idep], " Hit  ", "P" ) ;
       leg1->AddEntry( gBGRateA[idep], "AbsIso", "P" ) ;
       leg1->AddEntry( gBGRateR[idep], "RelIso", "P" ) ;
 
       TCanvas* c_1  = new TCanvas("c_1","", 800, 600);
       TMultiGraph *mg = new TMultiGraph();
       mg->SetTitle( "Background Rate" );
       mg->Add( gBGRateH[idep] ) ;
       mg->Add( gBGRateA[idep] ) ;
       mg->Add( gBGRateR[idep] ) ;
       mg->SetMaximum(1.1) ;
       mg->SetMinimum(0.2) ;
       mg->Draw("ALP") ;
       c_1->Update() ;
       leg1->Draw("same") ;
       c_1->Update() ;
       TString graph_name = hfolder + gName + "." + plotType ;
       c_1->Print( graph_name );
       delete c_1 ;
   }

   // 2. signal efficiency while background rate = 10%
   TGraphAsymmErrors* gSGRateA[3] ;
   TGraphAsymmErrors* gSGRateR[3] ;
   TGraphAsymmErrors* gSGRateH[3] ;
   for (int idep = 0 ; idep < 3; idep++) {
       double count = 0.1 ;
       vector<iEff> dRV  ;
       vector<iEff> A_sgRV, R_sgRV, H_sgRV ;
       for ( int idR = 0 ; idR < 5; idR++ ) {
           iEff sgR_a = SignalEff( wH.h_abs[idep][idR], bH.h_abs[idep][idR], 0.30 ) ;
           iEff sgR_r = SignalEff( wH.h_rel[idep][idR], bH.h_rel[idep][idR], 0.30 ) ;
           iEff sgR_h = SignalEff( wH.h_hit[idep][idR], bH.h_hit[idep][idR], 0.30 ) ;
           iEff dRE( count, 0, 0) ;

           dRV.push_back( dRE ) ;    
           A_sgRV.push_back( sgR_a ) ;    
           R_sgRV.push_back( sgR_r ) ;    
           H_sgRV.push_back( sgR_h ) ;    

           count += 0.1 ;
       }

       char gName[15] ;
       sprintf( gName, "SgRate_%d", idep+1 ) ;
       h_draw->FillGraph( gSGRateH[idep], dRV, H_sgRV, "dR", "", 0.1, 1.1, 6  ) ;
       h_draw->FillGraph( gSGRateA[idep], dRV, A_sgRV, "dR", "", 0.1, 1.1, 2  ) ;
       h_draw->FillGraph( gSGRateR[idep], dRV, R_sgRV, "dR", "", 0.1, 1.1, 4  ) ;

       // Drawing histogram for signal efficiency
       leg1->Clear();
       leg1->AddEntry( gSGRateH[idep], " Hit  ", "P" ) ;
       leg1->AddEntry( gSGRateA[idep], "AbsIso", "P" ) ;
       leg1->AddEntry( gSGRateR[idep], "RelIso", "P" ) ;

       TCanvas* c_1  = new TCanvas("c_1","", 800, 600);
       TMultiGraph *mg = new TMultiGraph();
       mg->SetTitle( "Signal Efficiency" );
       mg->Add( gSGRateH[idep] ) ;
       mg->Add( gSGRateA[idep] ) ;
       mg->Add( gSGRateR[idep] ) ;
       mg->SetMaximum(1.1) ;
       mg->SetMinimum(0.0) ;
       mg->Draw("ALP") ;
       c_1->Update() ;
       leg1->Draw("same") ;
       c_1->Update() ;
       TString graph_name = hfolder + gName + "." + plotType ;
       c_1->Print( graph_name );
       delete c_1 ;
       delete mg ;
   }

   // 3.
   //TGraph* gSB_abs[3] , gSB_rel[3] , gSB_hit[3] ;
   TGraph* gSB_abs[3]  ;
   TGraph* gSB_rel[3]  ;
   TGraph* gSB_hit[3] ;
   double abs_a[5], rel_a[5], hit_a[5], dR_a[5] ; 
   for (int idep = 0 ; idep < 3; idep++) {
       for ( int idR = 0 ; idR < 5; idR++ ) {
           double abs_best = SBRatio( wH.h_abs[idep][idR], bH.h_abs[idep][idR] ) ;
           double rel_best = SBRatio( wH.h_rel[idep][idR], bH.h_rel[idep][idR] ) ;
           double hit_best = SBRatio( wH.h_hit[idep][idR], bH.h_hit[idep][idR] ) ;
           abs_a[idR] = abs_best ;
           rel_a[idR] = rel_best ;
           hit_a[idR] = hit_best ;
           dR_a[idR] = 0.1*(idR +1.) ;
       }
       gSB_abs[idep] = new TGraph( 5, dR_a , abs_a ) ;
       gSB_rel[idep] = new TGraph( 5, dR_a , rel_a ) ;
       gSB_hit[idep] = new TGraph( 5, dR_a , hit_a ) ;

       // Set styles
       gSB_abs[idep]->SetLineColor(2) ; 
       gSB_rel[idep]->SetLineColor(4) ; 
       gSB_hit[idep]->SetLineColor(6) ; 
       gSB_abs[idep]->SetMarkerStyle(20) ; 
       gSB_rel[idep]->SetMarkerStyle(20) ; 
       gSB_hit[idep]->SetMarkerStyle(20) ; 
       gSB_abs[idep]->SetMarkerColor(2) ; 
       gSB_rel[idep]->SetMarkerColor(4) ; 
       gSB_hit[idep]->SetMarkerColor(6) ; 
 
       // drawing histograms for background rate
       char gName[15] ;
       sprintf( gName, "SBRatio_%d", idep+1 ) ;

       leg1->Clear();
       leg1->AddEntry( gSB_hit[idep], " Hit  ", "P" ) ;
       leg1->AddEntry( gSB_abs[idep], "AbsIso", "P" ) ;
       leg1->AddEntry( gSB_rel[idep], "RelIso", "P" ) ;
 
       TCanvas* c_1  = new TCanvas("c_1","", 800, 600);
       TMultiGraph *mg = new TMultiGraph();
       mg->SetTitle( "S / #sqrt{B} Ratio" );
       mg->Add( gSB_abs[idep] ) ;
       mg->Add( gSB_rel[idep] ) ;
       mg->Add( gSB_hit[idep] ) ;
       mg->SetMaximum(400.) ;
       mg->SetMinimum(200.) ;
       mg->Draw("ALP") ;
       c_1->Update() ;
       leg1->Draw("same") ;
       c_1->Update() ;
       TString graph_name = hfolder + gName + "." + plotType ;
       c_1->Print( graph_name );
       delete c_1 ;
   }
   delete leg1 ;

   // some basic information
   cout<<" drawing histograms "<<endl;
   h_draw->SetPlotStyle( true );
   h_draw->Draw( h_nVtx,  "nVertices", "N of Vertices",  "", "logY", 0.95, 1 );
   h_draw->Draw( h_nMu,   "nMuons", "N of Muons",        "", "logY", 0.95, 1 );
   h_draw->Draw( h_nJet,  "nJets", "N of jets",        "", "logY", 0.95, 1 );
   h_draw->Draw( h_wmuPt, "wmuonPt", "muon P_{T} from W (GeV/c)", "", "logY", 0.95, 1 );
   h_draw->Draw( h_bmuPt, "bmuonPt", "muon P_{T} from b (GeV/c)", "", "logY", 0.95, 1 );
   h_draw->Draw( h_JetPt, "jetPt", "jet P_{T} (GeV/c)", "", "logY", 0.95, 1 );
   h_draw->Draw( h_wmuE,  "wmuonE",  "muon E from W (GeV)",    "", "logY", 0.95, 1 );
   h_draw->Draw( h_bmuE,  "bmuonE",  "muon E from b (GeV)",    "", "logY", 0.95, 1 );


   h_draw->SetPlotStyle(false) ;
   TLegend* leg2  = new TLegend(.5, .7, .9, .9 );
   char RStr0[50], RStr1[50], RStr2[50], RStr3[50] ;
   char fname[25] ;   

   for ( int i=0; i  < 5 ; i++) {
       for ( int j=0; j  < 3 ; j++) {

           sprintf( RStr0, "(W) Depth%d = %.2f ", j,  wH.h_abs[j][i]->Integral( 10, 100) / wH.h_abs[j][i]->Integral() ) ;
	   sprintf( RStr1, "(B) Depth%d = %.2f ", j,  bH.h_abs[j][i]->Integral( 10, 100) / bH.h_abs[j][i]->Integral() ) ;
	   sprintf( RStr2, "(J) Depth%d = %.2f ", j,  jH.h_abs[j][i]->Integral( 10, 100) / jH.h_abs[j][i]->Integral() ) ;
	   sprintf( RStr3, "(R) Depth%d = %.2f ", j,  rH.h_abs[j][i]->Integral( 10, 100) / rH.h_abs[j][i]->Integral() ) ;
	   leg2->AddEntry( wH.h_abs[j][i], RStr0 , "L" ) ;
	   leg2->AddEntry( bH.h_abs[j][i], RStr1 , "L" ) ;
	   leg2->AddEntry( jH.h_abs[j][i], RStr2 , "L" ) ;
	   leg2->AddEntry( rH.h_abs[j][i], RStr3 , "L" ) ;

	   sprintf( fname,   "AbsIsoDR%d_L%d", i+1, j ) ;
	   h_draw->Draw(       wH.h_abs[j][i],  "",  "Abs. Isolation",  "", "logY", 0.95, 1 );
	   h_draw->DrawAppend( bH.h_abs[j][i],  "",      0.75, 2      );
	   h_draw->DrawAppend( jH.h_abs[j][i],  "",      0.55, 6      );
	   h_draw->DrawAppend( rH.h_abs[j][i],  fname,   0.35, 4, leg2 );
	   leg2->Clear() ;

           sprintf( RStr0, "(W) Depth%d = %.2f ", j,  wH.h_rel[j][i]->Integral( 5, 100) / wH.h_rel[j][i]->Integral() ) ;
	   sprintf( RStr1, "(B) Depth%d = %.2f ", j,  bH.h_rel[j][i]->Integral( 5, 100) / bH.h_rel[j][i]->Integral() ) ;
	   sprintf( RStr2, "(J) Depth%d = %.2f ", j,  jH.h_rel[j][i]->Integral( 5, 100) / jH.h_rel[j][i]->Integral() ) ;
	   sprintf( RStr3, "(R) Depth%d = %.2f ", j,  rH.h_rel[j][i]->Integral( 5, 100) / rH.h_rel[j][i]->Integral() ) ;
	   leg2->AddEntry( wH.h_rel[j][i], RStr0 , "L" ) ;
	   leg2->AddEntry( bH.h_rel[j][i], RStr1 , "L" ) ;
	   leg2->AddEntry( jH.h_rel[j][i], RStr2 , "L" ) ;
	   leg2->AddEntry( rH.h_rel[j][i], RStr3 , "L" ) ;

	   sprintf( fname,   "RelIsoDR%d_L%d", i+1, j ) ;
	   h_draw->Draw(       wH.h_rel[j][i],  "",  "Rel. Isolation",  "", "logY", 0.95, 1 );
	   h_draw->DrawAppend( bH.h_rel[j][i],  "",      0.75, 2      );
	   h_draw->DrawAppend( jH.h_rel[j][i],  "",      0.55, 6      );
	   h_draw->DrawAppend( rH.h_rel[j][i],  fname,   0.35, 4, leg2 );
	   leg2->Clear() ;

           sprintf( RStr0, "(W) Depth%d = %.2f ", j,  wH.h_hit[j][i]->Integral( 5, 50) / wH.h_hit[j][i]->Integral() ) ;
	   sprintf( RStr1, "(B) Depth%d = %.2f ", j,  bH.h_hit[j][i]->Integral( 5, 50) / bH.h_hit[j][i]->Integral() ) ;
	   sprintf( RStr2, "(J) Depth%d = %.2f ", j,  jH.h_hit[j][i]->Integral( 5, 50) / jH.h_hit[j][i]->Integral() ) ;
	   sprintf( RStr3, "(R) Depth%d = %.2f ", j,  rH.h_hit[j][i]->Integral( 5, 50) / rH.h_hit[j][i]->Integral() ) ;
	   leg2->AddEntry( wH.h_hit[j][i], RStr0 , "L" ) ;
	   leg2->AddEntry( bH.h_hit[j][i], RStr1 , "L" ) ;
	   leg2->AddEntry( jH.h_hit[j][i], RStr2 , "L" ) ;
	   leg2->AddEntry( rH.h_hit[j][i], RStr3 , "L" ) ;

	   sprintf( fname,   "IsoHitDR%d_L%d", i+1, j ) ;
	   h_draw->Draw(       wH.h_hit[j][i],  "",  "Isolation Hits",  "", "logY", 0.95, 1 );
	   h_draw->DrawAppend( bH.h_hit[j][i],  "",      0.75, 2      );
	   h_draw->DrawAppend( jH.h_hit[j][i],  "",      0.55, 6      );
	   h_draw->DrawAppend( rH.h_hit[j][i],  fname,   0.35, 4, leg2 );
	   leg2->Clear() ;
       }
   }


} 

void HcalAna::OpenHistograms() {

     TString Path_fName = hfolder + hfName + ".root" ;
     cout<<" Opening : "<< Path_fName <<endl ;

     theFile = (TFile*) TFile::Open( Path_fName , "READ" );
     //hFile->cd() ;
     cout<<" file opened ! "<<endl ;

     h_nMu     = (TH1D*) theFile->Get("h_nMu")     ;
     h_nJet    = (TH1D*) theFile->Get("h_nJet")     ;
     h_nVtx    = (TH1D*) theFile->Get("h_nVtx")     ;
     h_bmuPt   = (TH1D*) theFile->Get("h_bmuPt")     ;
     h_wmuPt   = (TH1D*) theFile->Get("h_wmuPt")     ;
     h_bmuE    = (TH1D*) theFile->Get("h_bmuE")     ;
     h_wmuE    = (TH1D*) theFile->Get("h_wmuE")     ;
     h_JetPt   = (TH1D*) theFile->Get("h_JetPt")     ;

     char hname_a[20],  hname_r[20], hname_h[20]  ;
     for ( int j=0; j< 4 ; j++) {
         for ( int k=0; k< 5 ; k++) {
	     sprintf( hname_a,   "w_abs_D%dR%d", j+1, k+1 ) ;
	     sprintf( hname_r,   "w_rel_D%dR%d", j+1, k+1 ) ;
	     sprintf( hname_h,   "w_hit_D%dR%d", j+1, k+1 ) ;
             wH.h_abs[j][k] = (TH1D*) theFile->Get( hname_a ) ;
             wH.h_rel[j][k] = (TH1D*) theFile->Get( hname_r ) ;
             wH.h_hit[j][k] = (TH1D*) theFile->Get( hname_h ) ;

	     sprintf( hname_a,   "b_abs_D%dR%d", j+1, k+1 ) ;
	     sprintf( hname_r,   "b_rel_D%dR%d", j+1, k+1 ) ;
	     sprintf( hname_h,   "b_hit_D%dR%d", j+1, k+1 ) ;
             bH.h_abs[j][k] = (TH1D*) theFile->Get( hname_a ) ;
             bH.h_rel[j][k] = (TH1D*) theFile->Get( hname_r ) ;
             bH.h_hit[j][k] = (TH1D*) theFile->Get( hname_h ) ;

	     sprintf( hname_a,   "r_abs_D%dR%d", j+1, k+1 ) ;
	     sprintf( hname_r,   "r_rel_D%dR%d", j+1, k+1 ) ;
	     sprintf( hname_h,   "r_hit_D%dR%d", j+1, k+1 ) ;
             rH.h_abs[j][k] = (TH1D*) theFile->Get( hname_a ) ;
             rH.h_rel[j][k] = (TH1D*) theFile->Get( hname_r ) ;
             rH.h_hit[j][k] = (TH1D*) theFile->Get( hname_h ) ;

	     sprintf( hname_a,   "j_abs_D%dR%d", j+1, k+1 ) ;
	     sprintf( hname_r,   "j_rel_D%dR%d", j+1, k+1 ) ;
	     sprintf( hname_h,   "j_hit_D%dR%d", j+1, k+1 ) ;
             jH.h_abs[j][k] = (TH1D*) theFile->Get( hname_a ) ;
             jH.h_rel[j][k] = (TH1D*) theFile->Get( hname_r ) ;
             jH.h_hit[j][k] = (TH1D*) theFile->Get( hname_h ) ;

         }
     }
}
 

double HcalAna::IsoDeposit( string type, int mu_id, int depth, int dR_i, double offset, double scale ) { 

   //absolute isolation
   double IsoA   =  0 ;
   double Iso_   =  0 ;
   double minIso =  999 ;
   double maxIso =  0 ;
   for ( int j=3; j >= 0; j--) {
       if ( isoMethod == 0 && j != depth ) continue ;
       if ( isoMethod == 2 && j  < depth ) continue ;

       if ( strncasecmp( "muon", type.c_str(), type.size() ) ==0 && type.size() > 0 ) {
          if ( dR_i == 1 )  Iso_ = leaves.muIso1[mu_id][j] ;
	  if ( dR_i == 2 )  Iso_ = leaves.muIso2[mu_id][j] ;
	  if ( dR_i == 3 )  Iso_ = leaves.muIso3[mu_id][j] ;
	  if ( dR_i == 4 )  Iso_ = leaves.muIso4[mu_id][j] ;
	  if ( dR_i == 5 )  Iso_ = leaves.muIso5[mu_id][j] ;
       }
       if ( strncasecmp( "gen", type.c_str(), type.size() ) ==0 && type.size() > 0 ) {
          if ( dR_i == 1 )  Iso_ = leaves.genIso1[mu_id][j] ;
	  if ( dR_i == 2 )  Iso_ = leaves.genIso2[mu_id][j] ;
	  if ( dR_i == 3 )  Iso_ = leaves.genIso3[mu_id][j] ;
	  if ( dR_i == 4 )  Iso_ = leaves.genIso4[mu_id][j] ;
	  if ( dR_i == 5 )  Iso_ = leaves.genIso5[mu_id][j] ;
       }
       if ( strncasecmp( "jet", type.c_str(), type.size() ) ==0 && type.size() > 0 ) {
          if ( dR_i == 1 )  Iso_ = leaves.jetIso1[mu_id][j] ;
	  if ( dR_i == 2 )  Iso_ = leaves.jetIso2[mu_id][j] ;
	  if ( dR_i == 3 )  Iso_ = leaves.jetIso3[mu_id][j] ;
	  if ( dR_i == 4 )  Iso_ = leaves.jetIso4[mu_id][j] ;
	  if ( dR_i == 5 )  Iso_ = leaves.jetIso5[mu_id][j] ;
       }
       if ( isoMethod == 0 ) IsoA  = Iso_ ;
       if ( isoMethod  > 0 ) IsoA += Iso_ ;
       if ( isoMethod == 1 && Iso_ < minIso && Iso_ > 0. )  minIso = Iso_ ;
       if ( isoMethod == 1 && Iso_ > maxIso && Iso_ > 0. )  maxIso = Iso_ ;
   }
   if ( isoMethod == 1 && depth == 1 ) IsoA = minIso ;
   if ( isoMethod == 1 && depth == 2 ) IsoA = maxIso ;

   double theIso  = ( IsoA*scale ) +  offset  ;
   return theIso ;
}

int HcalAna::IsoHits( string type, int mu_id, int depth, int dR_i, int offset, int scale ) { 

   //absolute isolation
   int IhitA   =  0 ;
   int Ihit_   =  0 ;
   int minIhit =  999 ;
   for ( int j=3; j >= 0; j--) {
       if ( isoMethod == 0 && j != depth ) continue ;
       if ( isoMethod == 2 && j  < depth ) continue ;

       if ( strncasecmp( "muon", type.c_str(), type.size() ) ==0 && type.size() > 0 ) {
          if ( dR_i == 1 )  Ihit_ = leaves.muIhit1[mu_id][j] ;
	  if ( dR_i == 2 )  Ihit_ = leaves.muIhit2[mu_id][j] ;
	  if ( dR_i == 3 )  Ihit_ = leaves.muIhit3[mu_id][j] ;
	  if ( dR_i == 4 )  Ihit_ = leaves.muIhit4[mu_id][j] ;
	  if ( dR_i == 5 )  Ihit_ = leaves.muIhit5[mu_id][j] ;
       }
       if ( strncasecmp( "gen", type.c_str(), type.size() ) ==0 && type.size() > 0 ) {
          if ( dR_i == 1 )  Ihit_ = leaves.genIhit1[mu_id][j] ;
	  if ( dR_i == 2 )  Ihit_ = leaves.genIhit2[mu_id][j] ;
	  if ( dR_i == 3 )  Ihit_ = leaves.genIhit3[mu_id][j] ;
	  if ( dR_i == 4 )  Ihit_ = leaves.genIhit4[mu_id][j] ;
	  if ( dR_i == 5 )  Ihit_ = leaves.genIhit5[mu_id][j] ;
       }
       if ( strncasecmp( "jet", type.c_str(), type.size() ) ==0 && type.size() > 0 ) {
          if ( dR_i == 1 )  Ihit_ = leaves.jetIhit1[mu_id][j] ;
	  if ( dR_i == 2 )  Ihit_ = leaves.jetIhit2[mu_id][j] ;
	  if ( dR_i == 3 )  Ihit_ = leaves.jetIhit3[mu_id][j] ;
	  if ( dR_i == 4 )  Ihit_ = leaves.jetIhit4[mu_id][j] ;
	  if ( dR_i == 5 )  Ihit_ = leaves.jetIhit5[mu_id][j] ;
       }
       if ( isoMethod == 0 ) IhitA  = Ihit_ ;
       if ( isoMethod  > 0 ) IhitA += Ihit_ ;
       if ( isoMethod == 1 && Ihit_ < minIhit && Ihit_ > 0 )  minIhit = Ihit_ ;

   }
   if ( isoMethod == 1 && depth == 1 ) IhitA = minIhit ;
   if ( isoMethod == 1 && depth == 2 ) IhitA = IhitA - minIhit ;

     //double theAbsIso_ = ( muIso > absiso_max ) ? absiso_max :  muIso ;
     int theIsoHits  = ( IhitA*scale ) +  offset  ;
     //if ( scale != 1. ) cout <<" muIso: "<< muIso << " scaled by "<< scale <<endl ;
     //return theAbsIso ;
     return theIsoHits ;
}

// depth : 0 ~ 4, 
iEff HcalAna::BgRatio( TH1D* hS, TH1D* hB, double sgEff ) { 

    double totalS = hS->Integral() ;
    double totalB = hB->Integral() ;
    printf("*** BGRate under %f Intgral S: %.2f, B: %.2f \n", sgEff, totalS, totalB ) ;

    iEff bRs ;
    if ( totalS == (double)0 || totalB == (double)0 ) return bRs ;

    double sR = 0 ;
    double bR = 0 ;
    double subS = 0 ;
    double subB = 0 ;
    double last_subS = 0 ;
    double last_subB = 0 ;
    double last_sR   = 0 ;
    int binx = 1 ;
    do { 
        last_sR   = sR ;
        last_subB = subB ;
        last_subS = subS ;
        subS   = hS->Integral( 1, binx ) ;
        subB   = hB->Integral( 1, binx ) ;
        sR =  subS / totalS ;
        bR =  subB / totalB ;
        printf(" bin: %d, S: %.1f sR: %.2f , B: %.1f bR: %.2f \n", binx, subS, (subS/totalS) , subB , (subB/totalB) ) ;
        binx++ ;
    } while ( sR < sgEff ) ;
    
    double slope  = ( subB - last_subB ) / ( sR - last_sR ) ;
    double subB95 = slope*( sgEff - last_sR ) + last_subB ;
    printf("** b1: %.1f ( %.3f) < b: %1.f ( %.3f) < b2: %.1f ( %.3f) \n", 
             last_subB, last_subB/totalB, subB95, subB/totalB, subB, bR ) ;
    printf("** s1: %.1f ( %.3f) <         ( 0.95) < s2: %.1f ( %.3f) \n", 
             last_subS, last_subS/totalS,         subS, sR ) ;

    pair<double,double> bR_Err = h_draw->EffError( totalB, subB95 );
    
    bRs.eff = subB95 / totalB ;
    //bRs.eff = bR ;
    bRs.errUp = bR_Err.first   ;
    bRs.errDn = bR_Err.second  ;
    return bRs ;
}

// The signal efficiency under the assumption of bgRate ( background comtamination )
iEff HcalAna::SignalEff( TH1D* hS, TH1D* hB,  double bgRate ) { 

    double totalS = hS->Integral() ;
    double totalB = hB->Integral() ;
    printf("*** SgEff under bgRate %f : Intgral S: %.2f, B: %.2f \n", bgRate, totalS, totalB ) ;

    iEff sRs ;
    if ( totalS == (double)0 || totalB == (double)0 ) return sRs ;

    double sR = 0 ;
    double bR = 0 ;
    double subS = 0 ;
    double subB = 0 ;
    int binx = 1 ;
    double last_subS = 0 ;
    double last_subB = 0 ;
    double last_bR   = 0 ;
    do {
        last_bR   = bR ;
        last_subB = subB ;
        last_subS = subS ;
        subS   = hS->Integral( 1, binx ) ;
        subB   = hB->Integral( 1, binx ) ;
        sR =  subS / totalS ;
        bR =  subB / totalB ;
        printf(" bin: %d, S: %.1f sR: %.2f , B: %.1f bR: %.2f \n", binx, subS, (subS/totalS) , subB , (subB/totalB) ) ;
        binx++ ;
    } while ( bR < bgRate ) ;

    double slope  = ( subS - last_subS ) / ( bR - last_bR ) ;
    double subS10 = slope*( bgRate - last_bR ) + last_subS ;
    printf("** s1: %.1f ( %.3f) < s: %.1f ( %.3f) < s2: %.1f ( %.3f) \n", 
              last_subS, last_subS/totalS, subS10, subS10/totalS ,subS, sR ) ;
    printf("** b1: %.1f ( %.3f) <         ( 0.1 ) < b2: %.1f ( %.3f) \n", 
              last_subB, last_subB/totalB,         subB, bR ) ;


    pair<double,double> sR_Err = h_draw->EffError( totalS, subS10 );
    
    sRs.eff = subS10 / totalS  ;
    //sRs.eff = sR  ;
    sRs.errUp = sR_Err.first   ;
    sRs.errDn = sR_Err.second  ;

    return sRs ;
}

// Find the cut for highest SB Ratio
double HcalAna::SBRatio( TH1D* hS, TH1D* hB ) { 

    double totalS = hS->Integral() ;
    double totalB = hB->Integral() ;
    printf(" ***** Max S/sqrt(B) Ratio ***** \n" ) ;

    //iEff sRs ;
    //if ( totalS == (double)0 || totalB == (double)0 ) return sRs ;

    double sR = 0 ;
    double bR = 0 ;
    double subS = 0 ;
    double subB = 0 ;
    int binx = 1 ;
    double maxSB  = 0 ;
    double max_sR = 0 ;
    double max_bR = 0 ;
 
    for ( int i=1; i <= hS->GetNbinsX() ; i++ ) {
    
        subS   = hS->Integral( 1, i ) ;
        subB   = hB->Integral( 1, i ) ;
        sR =  subS / totalS ;
        bR =  subB / totalB ;
        if ( sR > 0.5 ) {
           double maxSB_ = subS / sqrt( subB ) ;
           if ( maxSB_ > maxSB ) {
              maxSB = maxSB_ ; 
              binx = i ;
              max_sR = sR ;
              max_bR = bR ;
           }
        }

    } 
    printf(" bin: %d, S/B: %.2f sR: %.2f , bR: %.2f \n", binx, maxSB, max_sR , max_bR ) ;

    //return binx ;
    return maxSB ;
    //return sRs ;
}


// matching reco and generated objects - search for the minimum total dR for all objects 
vector<iMatch> HcalAna::GlobalDRMatch( vector<objID> vr, vector<objID> vg ) {

    vector<int> pool ;
    for (size_t i=0; i < vg.size(); i++) pool.push_back(i) ;
    //printf(" size of vr : %d and vg : %d \n", (int)vr.size(), (int)vg.size() ) ;
    if ( vr.size() > vg.size() ) {
       for ( size_t i=0 ; i< vr.size() - vg.size() ; i++ ) pool.push_back( -1 ) ;
    }
    //cout<<" pool size = "<< pool.size() <<endl ;

    vector<iMatch> vMatch ;
    vector<iMatch> vMatch0 ;
    iMatch iM0 ;
    double minDR = 999 ;
    do {

        double dr2 = 0 ;
        vMatch0.clear() ;
        //cout<<" ( " ;
        for ( size_t j=0; j< vr.size() ; j++ ) {
            if ( pool[j] == -1 ) continue ;
            double dr_ = vr[j].p4.DeltaR( vg[ pool[j] ].p4 ) ;

            iM0.gid  = pool[j] ;
            iM0.rid  = j ;
            iM0.ig   = vg[ pool[j] ].it ;
            iM0.ir   = vr[ j ].it ;
            iM0.dr   = dr_ ;
            iM0.dPt  = vr[j].p4.Pt() - vg[ pool[j] ].p4.Pt()  ;
            iM0.type = vr[j].type ; 

            vMatch0.push_back( iM0 ) ;
            dr2 += (dr_*dr_) ;
            //cout<< j <<", " ;
        }
        double dr = sqrt( dr2 ) ;
        //cout<<" ) , dR = "<< dr << endl ;

        if ( dr < minDR ) {
             minDR = dr ;
             vMatch = vMatch0 ;
        }

    } while (  next_permutation( pool.begin() ,pool.end() ) ) ;

    return vMatch ;
}

// write the scaling factor for different muon pt
void HcalAna::WriteMuonPtReWeighting( string dataName ) {

   cout<<" Get Muon Pt ReWeighting "<< endl ;

   // Get tree
   string dataFileNames ;
   if ( dataName != "0" ) {
      dataFileNames = dataName ;
   } else {
      Input->GetParameters( "TheData", &dataFileNames );
   }
   TTree* tr   = Input->GetTree( dataFileNames, "HcalUpgrade" );

   // Set addresses
   setBranchAddresses( tr, leaves );

   int totalN = tr->GetEntries();
   cout<<" from  "<< dataName <<" total entries = "<< totalN <<" Process "<< ProcessEvents <<endl;

   TH1D* g_muPt  = new TH1D("g_muPt",  " Gen Muon Pt distribution ",   25, 0, 250 );
   TH1D* w_muPt  = new TH1D("w_muPt",  " Gen W Muon Pt distribution ", 25, 0, 250 );
   TH1D* h_jetPt = new TH1D("h_jetPt", " Jet Pt distribution ",        25, 0, 250 );

   
   // Get the Pt spectrum for muon and jet
   double pt_cut = 10. ;
   double eta_cut = 0.8 ;
   for ( int i=0; i< totalN ; i++ ) {
       if ( ProcessEvents > 0 && i > ( ProcessEvents - 1 ) ) break;
       tr->GetEntry( i );

       // Get signal and background muon pt spectrum
       for ( int k=0; k< leaves.nGen; k++) {
           TLorentzVector gP4 = TLorentzVector( leaves.genPx[k], leaves.genPy[k], leaves.genPz[k], leaves.genE[k] ) ;
           // Select muons in different pt and eta range
           if ( gP4.Pt() < pt_cut ) continue ;
           if ( fabs( gP4.Eta() ) > eta_cut ) continue ;

           double theP4 = ( gP4.Pt() > 249.9 ) ? 249.9 : gP4.E() ;
           if ( abs( leaves.momId[k]) == 24 ) w_muPt->Fill( theP4 ) ; // signal 
           else                               g_muPt->Fill( theP4 ) ; // background 
           
       }

       // Get jet pt spectrum
       for ( int k =0; k< leaves.nJets ; k++) {
           TLorentzVector jP4 = TLorentzVector( leaves.jetPx[k], leaves.jetPy[k], leaves.jetPz[k], leaves.jetE[k] ) ;
           // Jet ID cuts
           if ( jP4.Pt()          < pt_cut  ) continue ;
           if ( fabs(jP4.Eta())   > eta_cut ) continue ;
	   if ( leaves.jetNDau[k] < jetCuts[2]  )  continue ;
	   if ( leaves.jetCEF[k]  > jetCuts[3]  )  continue ;
	   if ( leaves.jetNEF[k]  > jetCuts[4]  )  continue ;
	   if ( leaves.jetCHF[k]  > jetCuts[5]  )  continue ;
	   if ( leaves.jetNHF[k]  > jetCuts[6]  )  continue ;
	   if ( leaves.jetMEF[k]  > jetCuts[7]  )  continue ;
	   if ( fabs( jP4.Eta() ) < 2.4 && leaves.jetCM[k]  < 1 ) continue ;
           double theP4 = ( jP4.Pt() > 249.9 ) ? 249.9 : jP4.E() ;
           h_jetPt->Fill( theP4 ) ;
       }
   }

   // 2. Calculate and record the scaling factor
   string scaleFileName = scaleFile ;
   scaleFileName += ".log" ;
   FILE* logfile = fopen( scaleFileName.c_str() ,"w"); 

   TH1D* h1_ = (TH1D*) g_muPt->Clone() ;
   TH1D* h2_ = (TH1D*) w_muPt->Clone() ;
   TH1D* h3_ = (TH1D*) h_jetPt->Clone() ;

   h1_->Scale( 1./ g_muPt->Integral() ) ;
   h2_->Scale( 1./ w_muPt->Integral() ) ;
   h3_->Scale( 1./ h_jetPt->Integral() ) ;

   h_draw->Draw(       h1_, "", "Muon P_{T} (GeV/c)", "", "logY", 0.95, 2 );
   h_draw->DrawAppend( h3_, "",          0.75, 4 );
   h_draw->DrawAppend( h2_, "genMuonPt", 0.55, 1 );

   // Get the turn-on bin and value
   int nbin = h1_->GetNbinsX() ;
   /*
   double thresholdVal = 0 ;
   int    turnOnBin    = 0 ; 
   for ( int i=1 ; i <= nbin ; i++ ) {
       double x1 = h1_->GetBinContent( i ) ;
       if ( x1 > 0. ) {
          thresholdVal = x1 ;
          turnOnBin = i ;
          break ;
       }
   }
   */

   // get the normalization values 
   for ( int i=1 ; i <= nbin ; i++ ) {
       double x1 = h1_->GetBinCenter( i ) ;
       double w1 = h1_->GetBinWidth( i ) ;
       double b1 = h1_->GetBinContent( i ) ;
       double b2 = h2_->GetBinContent( i ) ;
       double b3 = h3_->GetBinContent( i ) ;
       double sc1 = ( b1 < 0.000001 ) ? 0 :  b2/b1 ;  
       double sc3 = ( b3 < 0.000001 ) ? 0 :  b2/b3 ;  
       //if ( i < turnOnBin && b2 > 0 ) sc = thresholdVal / b2 ;
       fprintf(logfile," %1d %.1f %.1f  %.3f %.3f \n", i,  x1-(w1/2), x1+(w1/2),  sc1, sc3 ) ;
   }
 
   delete h_jetPt ;
   delete g_muPt ;
   delete w_muPt ;
   fclose( logfile ) ;
}


// Read muon pt reweighting from the log file
void HcalAna::ReadMuonPtReWeighting( ) {

   //FILE* logfile = fopen( "MuPtReWeight_jet.log" ,"r"); 
   string scaleFileName = scaleFile ;
   scaleFileName += ".log" ;
   FILE* logfile = fopen( scaleFileName.c_str() ,"r"); 
   int ibin, r1 ;
   float ptl , ptu , sf1, sf3 ;
   for ( int i = 0; i < 25 ; i++ ) {
       r1 = fscanf(logfile, "%d", &ibin );
       r1 = fscanf(logfile, "%f", &ptl  );
       r1 = fscanf(logfile, "%f", &ptu  );
       r1 = fscanf(logfile, "%f", &sf1   );
       r1 = fscanf(logfile, "%f", &sf3   );
       muPt_l[i] = ptl ;
       muPt_h[i] = ptu ;
       scaleF[i] = sf1  ;
       //scaleF[i] = sf3  ;
       if ( r1 != 1 ) cout<<" reading error "<<endl ;
   }

   fclose( logfile ) ;
}

// return the corresponding scale for different muon pt
double HcalAna::GetMuonPtReWeighting( double muPt ) {

       int i = muPt / 10  ;
       if ( i > 24 ) i = 24 ;
       double scaleFactor = scaleF[i] ;
 
       //cout<<" muPt = "<< muPt <<"  i = "<< i <<" scf: "<< scaleF[i] <<endl ;
       return scaleFactor ;
}

void HcalAna::HistoWrite(  string theFolder , TFile* file ) {

    if ( theFolder.size() > 0 ) file->cd( theFolder.c_str() ); 

    h_nVtx->Write()   ;
    h_nMu->Write()   ;
    h_nJet->Write()   ;
    h_wmuE->Write()   ;
    h_bmuE->Write()   ;
    h_wmuPt->Write()  ;
    h_bmuPt->Write()  ;
    h_JetPt->Write() ;

    for ( int i=0; i< 4 ; i++) {
        for ( int j=0; j< 5 ; j++) {
            wH.h_abs[i][j]->Write() ;
            wH.h_rel[i][j]->Write() ;
            wH.h_hit[i][j]->Write() ;

            bH.h_abs[i][j]->Write() ;
            bH.h_rel[i][j]->Write() ;
            bH.h_hit[i][j]->Write() ;

            rH.h_abs[i][j]->Write() ;
            rH.h_rel[i][j]->Write() ;
            rH.h_hit[i][j]->Write() ;

            jH.h_abs[i][j]->Write() ;
            jH.h_rel[i][j]->Write() ;
            jH.h_hit[i][j]->Write() ;

        }
    }


}
