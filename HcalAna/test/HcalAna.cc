#include "HcalAna.h"

HcalAna::HcalAna( string datacardfile ){

  Input  = new AnaInput( datacardfile );
  h_draw = new hDraw( datacardfile ); 
 
  Input->GetParameters("PlotType",      &plotType ) ; 
  Input->GetParameters("Path",          &hfolder ) ; 
  Input->GetParameters("ProcessEvents", &ProcessEvents ) ; 
  Input->GetParameters("MuonCuts",      &muonCuts ) ; 

}

HcalAna::~HcalAna(){

  delete h_draw ;
  delete Input ;
  cout<<" done ! "<<endl ;

}

// analysis template
void HcalAna::ReadTree( string dataName ) { 

   TTree* tr = Input->TreeMap( dataName );

   tr->SetBranchAddress("nJets",        &nJets);
   tr->SetBranchAddress("nMuons",       &nMuons);
   tr->SetBranchAddress("nGen",         &nGen);

   tr->SetBranchAddress("muPx",       muPx );
   tr->SetBranchAddress("muPy",       muPy );
   tr->SetBranchAddress("muPz",       muPz );
   tr->SetBranchAddress("muE",        muE );
   tr->SetBranchAddress("muIso5",     muIso5 );
   tr->SetBranchAddress("muIso4",     muIso4 );
   tr->SetBranchAddress("muIso3",     muIso3 );
   tr->SetBranchAddress("muIso2",     muIso2 );
   tr->SetBranchAddress("muIso1",     muIso1 );
   tr->SetBranchAddress("muIhit5",    muIhit5 );
   tr->SetBranchAddress("muIhit4",    muIhit4 );
   tr->SetBranchAddress("muIhit3",    muIhit3 );
   tr->SetBranchAddress("muIhit2",    muIhit2 );
   tr->SetBranchAddress("muIhit1",    muIhit1 );

   tr->SetBranchAddress("jetPx",       jetPx );
   tr->SetBranchAddress("jetPy",       jetPy );
   tr->SetBranchAddress("jetPz",       jetPz );
   tr->SetBranchAddress("jetE",        jetE );

   tr->SetBranchAddress("momId",       momId);
   tr->SetBranchAddress("genPx",       genPx );
   tr->SetBranchAddress("genPy",       genPy );
   tr->SetBranchAddress("genPz",       genPz );
   tr->SetBranchAddress("genE",        genE );
   tr->SetBranchAddress("genIso5",     genIso5 );
   tr->SetBranchAddress("genIso4",     genIso4 );
   tr->SetBranchAddress("genIso3",     genIso3 );
   tr->SetBranchAddress("genIso2",     genIso2 );
   tr->SetBranchAddress("genIso1",     genIso1 );
   tr->SetBranchAddress("genIhit5",    genIhit5 );
   tr->SetBranchAddress("genIhit4",    genIhit4 );
   tr->SetBranchAddress("genIhit3",    genIhit3 );
   tr->SetBranchAddress("genIhit2",    genIhit2 );
   tr->SetBranchAddress("genIhit1",    genIhit1 );

   int totalN = tr->GetEntries();
   cout<<" from  "<< dataName <<" total entries = "<< totalN <<" Process "<< ProcessEvents <<endl;

   TH1D* h_nMu   = new TH1D("h_nMu",  " number of reco Muons ", 15, 0, 15 );
   TH1D* h_muPt  = new TH1D("h_muPt", " Muon Pt distribution ", 50, 0, 250 );
   TH1D* h_muE   = new TH1D("h_muE",  " Muon E distribution  ", 50, 0, 500 );

   TH2D* g_Pt_relIso5 = new TH2D("g_Pt_relIso5", "gen muon Pt vs RelIso dR 0.5", 60, 0,300, 60, 0., 6. ) ;
   TH2D* g_E_relIso5  = new TH2D("g_E_relIso5",  "gen muon E vs RelIso dR 0.5", 60, 0,300, 60, 0., 6. ) ;
   TH2D* w_Pt_relIso5 = new TH2D("w_Pt_relIso5", "gen W muon Pt vs RelIso dR 0.5", 60, 0,300, 60, 0., 6. ) ;
   TH2D* w_E_relIso5  = new TH2D("w_E_relIso5",  "gen W muon E vs RelIso dR 0.5", 60, 0,300, 60, 0., 6. ) ;
   TH2D* h_Pt_relIso5 = new TH2D("h_Pt_relIso5", "reco muon Pt vs RelIso dR 0.5", 60, 0,300, 60, 0., 6. ) ;
   TH2D* h_E_relIso5  = new TH2D("h_E_relIso5",  "reco muon E vs RelIso dR 0.5", 60, 0,300, 60, 0., 6. ) ;

   TH2D* g_Pt_absIso5 = new TH2D("g_Pt_absIso5", "gen muon Pt vs AbsIso dR 0.5", 60, 0,300, 60, 0., 30 ) ;
   TH2D* g_E_absIso5  = new TH2D("g_E_absIso5",  "gen muon E vs AbsIso dR 0.5", 60, 0,300, 60, 0., 30 ) ;
   TH2D* w_Pt_absIso5 = new TH2D("w_Pt_absIso5", "gen W muon Pt vs AbsIso dR 0.5", 60, 0,300, 60, 0., 30 ) ;
   TH2D* w_E_absIso5  = new TH2D("w_E_absIso5",  "gen W muon E vs AbsIso dR 0.5", 60, 0,300, 60, 0., 30 ) ;
   TH2D* h_Pt_absIso5 = new TH2D("h_Pt_absIso5", "reco muon Pt vs AbsIso dR 0.5", 60, 0,300, 60, 0., 30 ) ;
   TH2D* h_E_absIso5  = new TH2D("h_E_absIso5",  "reco muon E vs AbsIso dR 0.5", 60, 0,300, 60, 0., 30 ) ;

   TH2D* h_abs_relIso  = new TH2D("h_abs_relIso",  "reco muon abs vs rel Iso dR 0.3",  120, 0,30, 60, 0., 6 ) ;
   TH2D* w_abs_relIso  = new TH2D("w_abs_relIso",  "gen W muon abs vs rel Iso dR 0.3", 120, 0,30, 60, 0., 6 ) ;
   TH2D* g_abs_relIso  = new TH2D("g_abs_relIso",  "gen muon abs vs rel Iso dR 0.3",   120, 0,30, 60, 0., 6 ) ;

   r_Ihits[0]  = new TH1D("r_Ihits1", "nhits in dR < 0.1 ", 100,  0, 100.);
   r_Ihits[1]  = new TH1D("r_Ihits2", "nhits in dR < 0.2 ", 100,  0, 100.);
   r_Ihits[2]  = new TH1D("r_Ihits3", "nhits in dR < 0.3 ", 100,  0, 100.);
   r_Ihits[3]  = new TH1D("r_Ihits4", "nhits in dR < 0.4 ", 100,  0, 100.);
   r_Ihits[4]  = new TH1D("r_Ihits5", "nhits in dR < 0.5 ", 100,  0, 100.);

   r_absIso[0]  = new TH1D("r_absIso1", "absIso dR < 0.1 ", 500,  0, 100.);
   r_absIso[1]  = new TH1D("r_absIso2", "absIso dR < 0.2 ", 500,  0, 100.);
   r_absIso[2]  = new TH1D("r_absIso3", "absIso dR < 0.3 ", 500,  0, 100.);
   r_absIso[3]  = new TH1D("r_absIso4", "absIso dR < 0.4 ", 500,  0, 100.);
   r_absIso[4]  = new TH1D("r_absIso5", "absIso dR < 0.5 ", 500,  0, 100.);

   r_relIso[0]  = new TH1D("r_relIso1", "relIso dR < 0.1 ", 250,  0, 25.);
   r_relIso[1]  = new TH1D("r_relIso2", "relIso dR < 0.2 ", 250,  0, 25.);
   r_relIso[2]  = new TH1D("r_relIso3", "relIso dR < 0.3 ", 250,  0, 25.);
   r_relIso[3]  = new TH1D("r_relIso4", "relIso dR < 0.4 ", 250,  0, 25.);
   r_relIso[4]  = new TH1D("r_relIso5", "relIso dR < 0.5 ", 250,  0, 25.);

   g_Ihits[0]  = new TH1D("g_Ihits1", "nhits in dR < 0.1 ", 100,  0, 100.);
   g_Ihits[1]  = new TH1D("g_Ihits2", "nhits in dR < 0.2 ", 100,  0, 100.);
   g_Ihits[2]  = new TH1D("g_Ihits3", "nhits in dR < 0.3 ", 100,  0, 100.);
   g_Ihits[3]  = new TH1D("g_Ihits4", "nhits in dR < 0.4 ", 100,  0, 100.);
   g_Ihits[4]  = new TH1D("g_Ihits5", "nhits in dR < 0.5 ", 100,  0, 100.);

   g_absIso[0]  = new TH1D("g_absIso1", "absIso dR < 0.1 ", 500,  0, 100.);
   g_absIso[1]  = new TH1D("g_absIso2", "absIso dR < 0.2 ", 500,  0, 100.);
   g_absIso[2]  = new TH1D("g_absIso3", "absIso dR < 0.3 ", 500,  0, 100.);
   g_absIso[3]  = new TH1D("g_absIso4", "absIso dR < 0.4 ", 500,  0, 100.);
   g_absIso[4]  = new TH1D("g_absIso5", "absIso dR < 0.5 ", 500,  0, 100.);

   g_relIso[0]  = new TH1D("g_relIso1", "relIso dR < 0.1 ", 250,  0, 25.);
   g_relIso[1]  = new TH1D("g_relIso2", "relIso dR < 0.2 ", 250,  0, 25.);
   g_relIso[2]  = new TH1D("g_relIso3", "relIso dR < 0.3 ", 250,  0, 25.);
   g_relIso[3]  = new TH1D("g_relIso4", "relIso dR < 0.4 ", 250,  0, 25.);
   g_relIso[4]  = new TH1D("g_relIso5", "relIso dR < 0.5 ", 250,  0, 25.);

   w_Ihits[0]  = new TH1D("w_Ihits1", "w->mu nhits in dR < 0.1 ", 100,  0, 100.);
   w_Ihits[1]  = new TH1D("w_Ihits2", "w->mu nhits in dR < 0.2 ", 100,  0, 100.);
   w_Ihits[2]  = new TH1D("w_Ihits3", "w->mu nhits in dR < 0.3 ", 100,  0, 100.);
   w_Ihits[3]  = new TH1D("w_Ihits4", "w->mu nhits in dR < 0.4 ", 100,  0, 100.);
   w_Ihits[4]  = new TH1D("w_Ihits5", "w->mu nhits in dR < 0.5 ", 100,  0, 100.);

   w_absIso[0]  = new TH1D("w_absIso1", "w->mu absIso dR < 0.1 ", 500,  0, 100.);
   w_absIso[1]  = new TH1D("w_absIso2", "w->mu absIso dR < 0.2 ", 500,  0, 100.);
   w_absIso[2]  = new TH1D("w_absIso3", "w->mu absIso dR < 0.3 ", 500,  0, 100.);
   w_absIso[3]  = new TH1D("w_absIso4", "w->mu absIso dR < 0.4 ", 500,  0, 100.);
   w_absIso[4]  = new TH1D("w_absIso5", "w->mu absIso dR < 0.5 ", 500,  0, 100.);

   w_relIso[0]  = new TH1D("w_relIso1", "w->mu relIso dR < 0.1 ", 250,  0, 25.);
   w_relIso[1]  = new TH1D("w_relIso2", "w->mu relIso dR < 0.2 ", 250,  0, 25.);
   w_relIso[2]  = new TH1D("w_relIso3", "w->mu relIso dR < 0.3 ", 250,  0, 25.);
   w_relIso[3]  = new TH1D("w_relIso4", "w->mu relIso dR < 0.4 ", 250,  0, 25.);
   w_relIso[4]  = new TH1D("w_relIso5", "w->mu relIso dR < 0.5 ", 250,  0, 25.);

   reliso_max   = 5.5 ;
   reliso_bound = 6.0 ;
   absiso_max   = 24.5 ;
   absiso_bound = 25.0 ;
   isohit_max   = 19 ;
   isohit_bound = 20 ;

   int nEvt = 0 ;
   for ( int i=0; i< totalN ; i++ ) {
       if ( ProcessEvents > 0 && i > ( ProcessEvents - 1 ) ) break;
       tr->GetEntry( i );

       nEvt++; 
       h_nMu->Fill( nMuons ) ;

       for ( int k=0; k< nMuons; k++){
           TLorentzVector mP4( muPx[k], muPy[k], muPz[k], muE[k] )  ;
           
           if ( mP4.Pt() < muonCuts[0] || mP4.Pt() > muonCuts[1] ) continue ;
           if ( fabs( mP4.Eta() ) > muonCuts[2] ) continue ;
           h_muPt->Fill( mP4.Pt() ) ;
           h_muE->Fill( mP4.E() ) ;

           //if ( mP4.E() <= muonSel[0] || mP4.E() > muonSel[1] ) continue ;

           // looping thruogh 4 depths
           double theAbs = -1 ;
           double theRel = -1 ;
           for ( int j=0; j<4 ; j++ ) {
               // looping 5 different dR 
               //cout<<" Depth : "<< j+1 <<endl ;
               for ( int r=1; r < 6; ++r ) {
                   double theAbsIso_ = IsoDeposit( "reco", k, j, r , 0., 1. ) ;
                   double theAbsIso  = ( theAbsIso_ > absiso_max ) ? absiso_max : theAbsIso_ ;
                   double theRelIso_ = theAbsIso_ / mP4.Pt()   ;
                   double theRelIso  = ( theRelIso_ > reliso_max ) ? reliso_max : theRelIso_ ;
                   int    theIsohit_ = IsoHits( "reco", k, j, r ) ;
                   int    theIsohit  = ( theIsohit_ > isohit_max ) ? isohit_max : theIsohit_ ;

                   r_absIso[r-1]->Fill( theAbsIso + (j*absiso_bound) ) ;
                   r_relIso[r-1]->Fill( theRelIso + (j*reliso_bound) ) ;
                   r_Ihits[r-1]->Fill( theIsohit + (j*isohit_bound) ) ;
		   if ( j==0 && r == 5 )  h_Pt_relIso5->Fill( mP4.Pt(), theRelIso_ ) ;
		   if ( j==0 && r == 5 )  h_E_relIso5->Fill(  mP4.E(),  theRelIso_ ) ;
		   if ( j==0 && r == 5 )  h_Pt_absIso5->Fill( mP4.Pt(), theAbsIso_ ) ;
		   if ( j==0 && r == 5 )  h_E_absIso5->Fill(  mP4.E(),  theAbsIso_ ) ;
		   if ( j==2 && r == 1 )  theAbs = theAbsIso_  ;
		   if ( j==0 && r == 3 )  theRel = theRelIso_ ;
               }
           }
           h_abs_relIso->Fill( theAbs, theRel ) ;
       }

        
       for ( int k=0; k< nGen; k++) {
           TLorentzVector gP4 = TLorentzVector( genPx[k], genPy[k], genPz[k], genE[k] ) ;
           if ( gP4.Pt() < muonCuts[0] || gP4.Pt() > muonCuts[1] ) continue ;
           if ( fabs( gP4.Eta() ) > muonCuts[2] ) continue ;

           double theAbs = -1 ;
           double theRel = -1 ;
           if ( abs( momId[k]) == 24 ) {

              for ( int j=0; j<4 ; j++ ) {
                  for ( int r=1; r < 6; r++ ) {
                      double scale = ( j == 3 ) ? 1. : 1. ;
                      double theAbsIso_ = IsoDeposit( "gen", k, j, r , 0., scale ) ;
                      double theAbsIso  = ( theAbsIso_ > absiso_max ) ? absiso_max : theAbsIso_ ;
                      double theRelIso_ = theAbsIso_ / gP4.Pt()   ;
                      double theRelIso  = ( theRelIso_ > reliso_max ) ? reliso_max : theRelIso_ ;
                      int    theIsohit_ = IsoHits( "gen", k, j, r ) ;
                      int    theIsohit  = ( theIsohit_ > isohit_max ) ? isohit_max : theIsohit_ ;

                      //if (r == 3) printf(" (%d), layer_%d , iso = %f from %d hits\n", k, j, theAbsIso_, theIsohit_ ) ;

                      w_absIso[r-1]->Fill( theAbsIso + (j*absiso_bound) ) ;
                      w_relIso[r-1]->Fill( theRelIso + (j*reliso_bound) ) ;
                      w_Ihits[r-1]->Fill( theIsohit + (j*isohit_bound) ) ;
		      if ( j==0 && r == 5 )  w_Pt_relIso5->Fill( gP4.Pt(), theRelIso_ ) ;
		      if ( j==0 && r == 5 )  w_E_relIso5->Fill(  gP4.E(),  theRelIso_ ) ;
		      if ( j==0 && r == 5 )  w_Pt_absIso5->Fill( gP4.Pt(), theAbsIso_ ) ;
		      if ( j==0 && r == 5 )  w_E_absIso5->Fill(  gP4.E(),  theAbsIso_ ) ;
		      if ( j==2 && r == 1 )  theAbs = theAbsIso_  ;
		      if ( j==0 && r == 3 )  theRel = theRelIso_  ;
                  }
              }   
              w_abs_relIso->Fill( theAbs, theRel ) ;
              //cout<<" --------------- "<<endl ;
           } else {

              for ( int j=0; j<4 ; j++ ) {
                  for ( int r=1; r < 6; r++ ) {
                      double theAbsIso_ = IsoDeposit( "gen", k, j, r , 0., 1. ) ;
                      double theAbsIso  = ( theAbsIso_ > absiso_max ) ? absiso_max : theAbsIso_ ;
                      double theRelIso_ = theAbsIso_ / gP4.Pt()   ;
                      double theRelIso  = ( theRelIso_ > reliso_max ) ? reliso_max : theRelIso_ ;
                      int    theIsohit_ = IsoHits( "gen", k, j, r ) ;
                      int    theIsohit  = ( theIsohit_ > isohit_max ) ? isohit_max : theIsohit_ ;

                      g_absIso[r-1]->Fill( theAbsIso + (j*absiso_bound) ) ;
                      g_relIso[r-1]->Fill( theRelIso + (j*reliso_bound) ) ;
                      g_Ihits[r-1]->Fill( theIsohit + (j*isohit_bound) ) ;
		      if ( j==0 && r == 5 )  g_Pt_relIso5->Fill( gP4.Pt(), theRelIso_ ) ;
		      if ( j==0 && r == 5 )  g_E_relIso5->Fill(  gP4.E(),  theRelIso_ ) ;
		      if ( j==0 && r == 5 )  g_Pt_absIso5->Fill( gP4.Pt(), theAbsIso_ ) ;
		      if ( j==0 && r == 5 )  g_E_absIso5->Fill(  gP4.E(),  theAbsIso_ ) ;
		      if ( j==2 && r == 1 )  theAbs = theAbsIso_  ;
		      if ( j==0 && r == 3 )  theRel = theRelIso_  ;
                  }
              }   
              g_abs_relIso->Fill( theAbs, theRel ) ;
           }
       }
       

   } // end of event looping

   cout<<" drawing histograms "<<endl;
   h_draw->Draw( h_nMu,  "nMuons", "N of Muons",      "", "logY", 0.95, 1 );
   h_draw->Draw( h_muPt, "muonPt", "muon pt (GeV/c)", "", "logY", 0.95, 1 );
   h_draw->Draw( h_muE,  "muonE",  "muon E (GeV)",    "", "logY", 0.95, 1 );

   // set histogram's attributions : axis, labelsize, tickLength, titleSize, titleOffset   
   h_draw->SetHistoAtt("X", 0.1, 0.07, 0.1, 1. ) ;
   h_draw->SetHistoAtt("Y", 0.1, 0.02, 0.1, 0 ) ;
   h_draw->CreateNxM( "RecoAbsIsolations", 1,5 );
   h_draw->DrawNxM( 1, r_absIso[0], "abs. Isolation for reco muon", "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 2, r_absIso[1], "abs. Isolation for reco muon", "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 3, r_absIso[2], "abs. Isolation for reco muon", "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 4, r_absIso[3], "abs. Isolation for reco muon", "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 5, r_absIso[4], "abs. Isolation for reco muon", "", "logY", 1, 0.1, 0.1, true );

   h_draw->CreateNxM( "RecoRelIsolations", 1,5 );
   h_draw->DrawNxM( 1, r_relIso[0],   "rel. Isolation for reco muon", "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 2, r_relIso[1],   "rel. Isolation for reco muon", "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 3, r_relIso[2],   "rel. Isolation for reco muon", "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 4, r_relIso[3],   "rel. Isolation for reco muon", "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 5, r_relIso[4],   "rel. Isolation for reco muon", "", "logY", 1, 0.1, 0.1, true );

   h_draw->CreateNxM( "RecoIsoHits", 1,5 );
   h_draw->DrawNxM( 1, r_Ihits[0],   " N of Isohits for reco muon", "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 2, r_Ihits[1],   " N of Isohits for reco muon", "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 3, r_Ihits[2],   " N of Isohits for reco muon", "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 4, r_Ihits[3],   " N of Isohits for reco muon", "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 5, r_Ihits[4],   " N of Isohits for reco muon", "", "logY", 1, 0.1, 0.1, true );

   h_draw->CreateNxM( "GenAbsIsolations", 1,5 );
   h_draw->DrawNxM( 1, g_absIso[0],   "abs. Isolation for gen muon", "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 2, g_absIso[1],   "abs. Isolation for gen muon", "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 3, g_absIso[2],   "abs. Isolation for gen muon", "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 4, g_absIso[3],   "abs. Isolation for gen muon", "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 5, g_absIso[4],   "abs. Isolation for gen muon", "", "logY", 1, 0.1, 0.1, true );

   h_draw->CreateNxM( "GenRelIsolations", 1,5 );
   h_draw->DrawNxM( 1, g_relIso[0],   "rel. Isolation for gen muon", "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 2, g_relIso[1],   "rel. Isolation for gen muon", "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 3, g_relIso[2],   "rel. Isolation for gen muon", "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 4, g_relIso[3],   "rel. Isolation for gen muon", "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 5, g_relIso[4],   "rel. Isolation for gen muon", "", "logY", 1, 0.1, 0.1, true );

   h_draw->CreateNxM( "GenIsoHits", 1,5 );
   h_draw->DrawNxM( 1, g_Ihits[0],   " N of Isohits for gen muon", "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 2, g_Ihits[1],   " N of Isohits for gen muon", "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 3, g_Ihits[2],   " N of Isohits for gen muon", "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 4, g_Ihits[3],   " N of Isohits for gen muon", "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 5, g_Ihits[4],   " N of Isohits for gen muon", "", "logY", 1, 0.1, 0.1, true );

   h_draw->CreateNxM( "GenWAbsIsolations", 1,5 );
   h_draw->DrawNxM( 1, w_absIso[0],   "abs. Isolation for gen W muon", "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 2, w_absIso[1],   "abs. Isolation for gen W muon", "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 3, w_absIso[2],   "abs. Isolation for gen W muon", "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 4, w_absIso[3],   "abs. Isolation for gen W muon", "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 5, w_absIso[4],   "abs. Isolation for gen W muon", "", "logY", 1, 0.1, 0.1, true );

   h_draw->CreateNxM( "GenWRelIsolations", 1,5 );
   h_draw->DrawNxM( 1, w_relIso[0],   "rel. Isolation for gen W muon", "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 2, w_relIso[1],   "rel. Isolation for gen W muon", "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 3, w_relIso[2],   "rel. Isolation for gen W muon", "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 4, w_relIso[3],   "rel. Isolation for gen W muon", "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 5, w_relIso[4],   "rel. Isolation for gen W muon", "", "logY", 1, 0.1, 0.1, true );

   h_draw->CreateNxM( "GenWIsoHits", 1,5 );
   h_draw->DrawNxM( 1, w_Ihits[0],   " N of Isohits for gen W muon", "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 2, w_Ihits[1],   " N of Isohits for gen W muon", "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 3, w_Ihits[2],   " N of Isohits for gen W muon", "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 4, w_Ihits[3],   " N of Isohits for gen W muon", "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 5, w_Ihits[4],   " N of Isohits for gen W muon", "", "logY", 1, 0.1, 0.1, true );

   h_draw->SetHistoAtt("X", 0, 0, 0, 0 ) ; // reset to histogram attributions to default 
   h_draw->SetHistoAtt("Y", 0, 0, 0, 0 ) ; // reset to histogram attributions to default 

   // set histogram's attributions : axis, labelsize, tickLength, titleSize, titleOffset   
   h_draw->SetHistoAtt("X", 0.1, 0.07, 0.08, 1. ) ;
   h_draw->SetHistoAtt("Y", 0.1, 0.02, 0.08, 0.5 ) ;

   gStyle->SetStatX( 0.9 ) ;
   h_draw->CreateNxM( "PtRelIso", 1,3 );
   h_draw->DrawNxM( 1, w_Pt_relIso5,  "Pt (GeV)", "RelIso ", "", 5, 0.1, 0.06, false );
   h_draw->DrawNxM( 2, g_Pt_relIso5,  "Pt (GeV)", "RelIso ", "", 5, 0.1, 0.06, false );
   h_draw->DrawNxM( 3, h_Pt_relIso5,  "Pt (GeV)", "RelIso ", "", 5, 0.1, 0.06, true );

   h_draw->CreateNxM( "ERelIso", 1,3 );
   h_draw->DrawNxM( 1, w_E_relIso5,   "E (GeV)", "RelIso ",  "", 5, 0.1, 0.06, false );
   h_draw->DrawNxM( 2, g_E_relIso5,   "E (GeV)", "RelIso ",  "", 5, 0.1, 0.06, false );
   h_draw->DrawNxM( 3, h_E_relIso5,   "E (GeV)", "RelIso ",  "", 5, 0.1, 0.05, true );

   h_draw->CreateNxM( "PtAbsIso", 1,3 );
   h_draw->DrawNxM( 1, w_Pt_absIso5,  "Pt (GeV)", "AbsIso ", "", 5, 0.1, 0.06, false );
   h_draw->DrawNxM( 2, g_Pt_absIso5,  "Pt (GeV)", "AbsIso ", "", 5, 0.1, 0.06, false );
   h_draw->DrawNxM( 3, h_Pt_absIso5,  "Pt (GeV)", "AbsIso ", "", 5, 0.1, 0.06, true );

   h_draw->CreateNxM( "EAbsIso", 1,3 );
   h_draw->DrawNxM( 1, w_E_absIso5,   "E (GeV)", "AbsIso ",  "", 5, 0.1, 0.06, false );
   h_draw->DrawNxM( 2, g_E_absIso5,   "E (GeV)", "AbsIso ",  "", 5, 0.1, 0.06, false );
   h_draw->DrawNxM( 3, h_E_absIso5,   "E (GeV)", "AbsIso ",  "", 5, 0.1, 0.05, true );

   h_draw->CreateNxM( "Abs1_Rel3", 1,3 );
   h_draw->DrawNxM( 1, w_abs_relIso,   "AbsIso", "RelIso ",  "", 5, 0.1, 0.06, false );
   h_draw->DrawNxM( 2, g_abs_relIso,   "AbsIso", "RelIso ",  "", 5, 0.1, 0.06, false );
   h_draw->DrawNxM( 3, h_abs_relIso,   "AbsIso", "RelIso ",  "", 5, 0.1, 0.05, true );

   h_draw->SetHistoAtt("X", 0, 0, 0, 0 ) ; // reset to histogram attributions to default 
   h_draw->SetHistoAtt("Y", 0, 0, 0, 0 ) ; // reset to histogram attributions to default 

}  

double HcalAna::IsoDeposit( string type, int mu_id, int depth, int dR_i, double offset, double scale ) { 

     //absolute isolation
     double muIso = -1 ;
     if ( strncasecmp( "reco", type.c_str(), type.size() ) ==0 && type.size() > 0 ) {
        if ( dR_i == 1 )  muIso = muIso1[mu_id][depth] ;
	if ( dR_i == 2 )  muIso = muIso2[mu_id][depth] ;
	if ( dR_i == 3 )  muIso = muIso3[mu_id][depth] ;
	if ( dR_i == 4 )  muIso = muIso4[mu_id][depth] ;
	if ( dR_i == 5 )  muIso = muIso5[mu_id][depth] ;
     }
     if ( strncasecmp( "gen", type.c_str(), type.size() ) ==0 && type.size() > 0 ) {
        if ( dR_i == 1 )  muIso = genIso1[mu_id][depth] ;
	if ( dR_i == 2 )  muIso = genIso2[mu_id][depth] ;
	if ( dR_i == 3 )  muIso = genIso3[mu_id][depth] ;
	if ( dR_i == 4 )  muIso = genIso4[mu_id][depth] ;
	if ( dR_i == 5 )  muIso = genIso5[mu_id][depth] ;
     }

     //double theAbsIso_ = ( muIso > absiso_max ) ? absiso_max :  muIso ;
     double theAbsIso  = ( muIso*scale ) +  offset  ;
     //if ( scale != 1. ) cout <<" muIso: "<< muIso << " scaled by "<< scale <<endl ;
     //return theAbsIso ;
     return theAbsIso ;
}

int HcalAna::IsoHits( string type, int mu_id, int depth, int dR_i, int offset, int scale ) { 

     //absolute isolation
     double muIhit = -1 ;
     if ( strncasecmp( "reco", type.c_str(), type.size() ) ==0 && type.size() > 0 ) {
        if ( dR_i == 1 )  muIhit = muIhit1[mu_id][depth] ;
	if ( dR_i == 2 )  muIhit = muIhit2[mu_id][depth] ;
	if ( dR_i == 3 )  muIhit = muIhit3[mu_id][depth] ;
	if ( dR_i == 4 )  muIhit = muIhit4[mu_id][depth] ;
	if ( dR_i == 5 )  muIhit = muIhit5[mu_id][depth] ;
     }
     if ( strncasecmp( "gen", type.c_str(), type.size() ) ==0 && type.size() > 0 ) {
        if ( dR_i == 1 )  muIhit = genIhit1[mu_id][depth] ;
	if ( dR_i == 2 )  muIhit = genIhit2[mu_id][depth] ;
	if ( dR_i == 3 )  muIhit = genIhit3[mu_id][depth] ;
	if ( dR_i == 4 )  muIhit = genIhit4[mu_id][depth] ;
	if ( dR_i == 5 )  muIhit = genIhit5[mu_id][depth] ;
     }

     //double theAbsIso_ = ( muIso > absiso_max ) ? absiso_max :  muIso ;
     int theIsoHits  = ( muIhit*scale ) +  offset  ;
     //if ( scale != 1. ) cout <<" muIso: "<< muIso << " scaled by "<< scale <<endl ;
     //return theAbsIso ;
     return theIsoHits ;
}

