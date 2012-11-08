#include "HcalAna.h"

HcalAna::HcalAna( string datacardfile ){

  Input  = new AnaInput( datacardfile );
  h_draw = new hDraw( datacardfile ); 
 
  Input->GetParameters("PlotType",      &plotType ) ; 
  Input->GetParameters("Path",          &hfolder ) ; 
  Input->GetParameters("ProcessEvents", &ProcessEvents ) ; 
  Input->GetParameters("MuonCuts",      &muonCuts ) ; 
  Input->GetParameters("IsoMethod",     &isoMethod ) ; 
  Input->GetParameters("HistoName",     &hfName ) ;

}

HcalAna::~HcalAna(){

  delete h_draw ;
  delete Input ;
  cout<<" done ! "<<endl ;

}

// analysis template
void HcalAna::ReadTree( string dataName, bool reScale  ) { 

   // create histogram files 
   TString Path_fName = hfolder + hfName + ".root" ;
   theFile = new TFile( Path_fName, "RECREATE" );
   theFile->cd() ;

   // read scale Factor 
   if ( reScale ) ReadMuonPtReWeighting() ;

   // read the ntuple tree 
   TTree* tr = Input->TreeMap( dataName );
   setBranchAddresses( tr, leaves );

   int totalN = tr->GetEntries();
   cout<<" from  "<< dataName <<" total entries = "<< totalN <<" Process "<< ProcessEvents <<endl;
   if ( isoMethod == 0 ) cout <<" Iso Deposit from each layer "<<endl ;
   if ( isoMethod == 1 ) cout <<" Iso Deposit from min/max layer "<<endl ;
   if ( isoMethod == 2 ) cout <<" Iso Deposit from inclusive layers "<<endl ;

   // booking histograms 
   h_nMu   = new TH1D("h_nMu",   " number of reco Muons ", 15, 0, 15 );
   h_muE   = new TH1D("h_muE",   " Muon E distribution  ", 50, 0, 500 );
   h_muPt  = new TH1D("h_muPt",  " Muon Pt distribution ", 50, 0, 250 );
   h_JetPt = new TH1D("h_JetPt", " Jet Pt distribution  ", 50, 0, 500 );

   g_Pt_relIso5 = new TH2D("g_Pt_relIso5", "gen muon Pt vs RelIso dR 0.5", 60, 0,300, 60, 0., 6. ) ;
   g_E_relIso5  = new TH2D("g_E_relIso5",  "gen muon E vs RelIso dR 0.5", 60, 0,300, 60, 0., 6. ) ;
   w_Pt_relIso5 = new TH2D("w_Pt_relIso5", "gen W muon Pt vs RelIso dR 0.5", 60, 0,300, 60, 0., 6. ) ;
   w_E_relIso5  = new TH2D("w_E_relIso5",  "gen W muon E vs RelIso dR 0.5", 60, 0,300, 60, 0., 6. ) ;
   h_Pt_relIso5 = new TH2D("h_Pt_relIso5", "reco muon Pt vs RelIso dR 0.5", 60, 0,300, 60, 0., 6. ) ;
   h_E_relIso5  = new TH2D("h_E_relIso5",  "reco muon E vs RelIso dR 0.5", 60, 0,300, 60, 0., 6. ) ;

   g_Pt_absIso5 = new TH2D("g_Pt_absIso5", "gen muon Pt vs AbsIso dR 0.5", 60, 0,300, 60, 0., 30 ) ;
   g_E_absIso5  = new TH2D("g_E_absIso5",  "gen muon E vs AbsIso dR 0.5", 60, 0,300, 60, 0., 30 ) ;
   w_Pt_absIso5 = new TH2D("w_Pt_absIso5", "gen W muon Pt vs AbsIso dR 0.5", 60, 0,300, 60, 0., 30 ) ;
   w_E_absIso5  = new TH2D("w_E_absIso5",  "gen W muon E vs AbsIso dR 0.5", 60, 0,300, 60, 0., 30 ) ;
   h_Pt_absIso5 = new TH2D("h_Pt_absIso5", "reco muon Pt vs AbsIso dR 0.5", 60, 0,300, 60, 0., 30 ) ;
   h_E_absIso5  = new TH2D("h_E_absIso5",  "reco muon E vs AbsIso dR 0.5", 60, 0,300, 60, 0., 30 ) ;

   h_abs_relIso  = new TH2D("h_abs_relIso",  "reco muon abs vs rel Iso dR 0.3",  120, 0,30, 60, 0., 6 ) ;
   w_abs_relIso  = new TH2D("w_abs_relIso",  "gen W muon abs vs rel Iso dR 0.3", 120, 0,30, 60, 0., 6 ) ;
   g_abs_relIso  = new TH2D("g_abs_relIso",  "gen muon abs vs rel Iso dR 0.3",   120, 0,30, 60, 0., 6 ) ;

   reliso_max   = 5.5 ;
   reliso_bound = 6.0 ;
   reliso_end   = 24 ;
   reliso_nbin  = 240 ;

   absiso_max   = 99. ;
   absiso_bound = 100. ;
   absiso_end   = 400 ;
   absiso_nbin  = 400 ;

   isohit_max   = 49 ;
   isohit_bound = 50 ;
   isohit_end   = 200 ;
   isohit_nbin  = 200 ;

   r_Ihits[0]  = new TH1D("r_Ihits1", "nhits in dR < 0.1 ", isohit_nbin, 0, isohit_end);
   r_Ihits[1]  = new TH1D("r_Ihits2", "nhits in dR < 0.2 ", isohit_nbin, 0, isohit_end);
   r_Ihits[2]  = new TH1D("r_Ihits3", "nhits in dR < 0.3 ", isohit_nbin, 0, isohit_end);
   r_Ihits[3]  = new TH1D("r_Ihits4", "nhits in dR < 0.4 ", isohit_nbin, 0, isohit_end);
   r_Ihits[4]  = new TH1D("r_Ihits5", "nhits in dR < 0.5 ", isohit_nbin, 0, isohit_end);

   r_absIso[0]  = new TH1D("r_absIso1", "absIso dR < 0.1 ", absiso_nbin, 0, absiso_end);
   r_absIso[1]  = new TH1D("r_absIso2", "absIso dR < 0.2 ", absiso_nbin, 0, absiso_end);
   r_absIso[2]  = new TH1D("r_absIso3", "absIso dR < 0.3 ", absiso_nbin, 0, absiso_end);
   r_absIso[3]  = new TH1D("r_absIso4", "absIso dR < 0.4 ", absiso_nbin, 0, absiso_end);
   r_absIso[4]  = new TH1D("r_absIso5", "absIso dR < 0.5 ", absiso_nbin, 0, absiso_end);

   r_relIso[0]  = new TH1D("r_relIso1", "relIso dR < 0.1 ", reliso_nbin, 0, reliso_end);
   r_relIso[1]  = new TH1D("r_relIso2", "relIso dR < 0.2 ", reliso_nbin, 0, reliso_end);
   r_relIso[2]  = new TH1D("r_relIso3", "relIso dR < 0.3 ", reliso_nbin, 0, reliso_end);
   r_relIso[3]  = new TH1D("r_relIso4", "relIso dR < 0.4 ", reliso_nbin, 0, reliso_end);
   r_relIso[4]  = new TH1D("r_relIso5", "relIso dR < 0.5 ", reliso_nbin, 0, reliso_end);

   g_Ihits[0]  = new TH1D("g_Ihits1", "nhits in dR < 0.1 ", isohit_nbin, 0, isohit_end);
   g_Ihits[1]  = new TH1D("g_Ihits2", "nhits in dR < 0.2 ", isohit_nbin, 0, isohit_end);
   g_Ihits[2]  = new TH1D("g_Ihits3", "nhits in dR < 0.3 ", isohit_nbin, 0, isohit_end);
   g_Ihits[3]  = new TH1D("g_Ihits4", "nhits in dR < 0.4 ", isohit_nbin, 0, isohit_end);
   g_Ihits[4]  = new TH1D("g_Ihits5", "nhits in dR < 0.5 ", isohit_nbin, 0, isohit_end);

   g_absIso[0]  = new TH1D("g_absIso1", "absIso dR < 0.1 ", absiso_nbin, 0, absiso_end);
   g_absIso[1]  = new TH1D("g_absIso2", "absIso dR < 0.2 ", absiso_nbin, 0, absiso_end);
   g_absIso[2]  = new TH1D("g_absIso3", "absIso dR < 0.3 ", absiso_nbin, 0, absiso_end);
   g_absIso[3]  = new TH1D("g_absIso4", "absIso dR < 0.4 ", absiso_nbin, 0, absiso_end);
   g_absIso[4]  = new TH1D("g_absIso5", "absIso dR < 0.5 ", absiso_nbin, 0, absiso_end);

   g_relIso[0]  = new TH1D("g_relIso1", "relIso dR < 0.1 ", reliso_nbin, 0, reliso_end);
   g_relIso[1]  = new TH1D("g_relIso2", "relIso dR < 0.2 ", reliso_nbin, 0, reliso_end);
   g_relIso[2]  = new TH1D("g_relIso3", "relIso dR < 0.3 ", reliso_nbin, 0, reliso_end);
   g_relIso[3]  = new TH1D("g_relIso4", "relIso dR < 0.4 ", reliso_nbin, 0, reliso_end);
   g_relIso[4]  = new TH1D("g_relIso5", "relIso dR < 0.5 ", reliso_nbin, 0, reliso_end);

   w_Ihits[0]  = new TH1D("w_Ihits1", "w->mu nhits in dR < 0.1 ", isohit_nbin, 0, isohit_end);
   w_Ihits[1]  = new TH1D("w_Ihits2", "w->mu nhits in dR < 0.2 ", isohit_nbin, 0, isohit_end);
   w_Ihits[2]  = new TH1D("w_Ihits3", "w->mu nhits in dR < 0.3 ", isohit_nbin, 0, isohit_end);
   w_Ihits[3]  = new TH1D("w_Ihits4", "w->mu nhits in dR < 0.4 ", isohit_nbin, 0, isohit_end);
   w_Ihits[4]  = new TH1D("w_Ihits5", "w->mu nhits in dR < 0.5 ", isohit_nbin, 0, isohit_end);

   w_absIso[0]  = new TH1D("w_absIso1", "w->mu absIso dR < 0.1 ", absiso_nbin, 0, absiso_end);
   w_absIso[1]  = new TH1D("w_absIso2", "w->mu absIso dR < 0.2 ", absiso_nbin, 0, absiso_end);
   w_absIso[2]  = new TH1D("w_absIso3", "w->mu absIso dR < 0.3 ", absiso_nbin, 0, absiso_end);
   w_absIso[3]  = new TH1D("w_absIso4", "w->mu absIso dR < 0.4 ", absiso_nbin, 0, absiso_end);
   w_absIso[4]  = new TH1D("w_absIso5", "w->mu absIso dR < 0.5 ", absiso_nbin, 0, absiso_end);

   w_relIso[0]  = new TH1D("w_relIso1", "w->mu relIso dR < 0.1 ", reliso_nbin, 0, reliso_end);
   w_relIso[1]  = new TH1D("w_relIso2", "w->mu relIso dR < 0.2 ", reliso_nbin, 0, reliso_end);
   w_relIso[2]  = new TH1D("w_relIso3", "w->mu relIso dR < 0.3 ", reliso_nbin, 0, reliso_end);
   w_relIso[3]  = new TH1D("w_relIso4", "w->mu relIso dR < 0.4 ", reliso_nbin, 0, reliso_end);
   w_relIso[4]  = new TH1D("w_relIso5", "w->mu relIso dR < 0.5 ", reliso_nbin, 0, reliso_end);

   j_Ihits[0]  = new TH1D("j_Ihits1", "nhits in dR < 0.1 ", isohit_nbin, 0, isohit_end);
   j_Ihits[1]  = new TH1D("j_Ihits2", "nhits in dR < 0.2 ", isohit_nbin, 0, isohit_end);
   j_Ihits[2]  = new TH1D("j_Ihits3", "nhits in dR < 0.3 ", isohit_nbin, 0, isohit_end);
   j_Ihits[3]  = new TH1D("j_Ihits4", "nhits in dR < 0.4 ", isohit_nbin, 0, isohit_end);
   j_Ihits[4]  = new TH1D("j_Ihits5", "nhits in dR < 0.5 ", isohit_nbin, 0, isohit_end);

   j_absIso[0]  = new TH1D("j_absIso1", "absIso dR < 0.1 ", absiso_nbin, 0, absiso_end);
   j_absIso[1]  = new TH1D("j_absIso2", "absIso dR < 0.2 ", absiso_nbin, 0, absiso_end);
   j_absIso[2]  = new TH1D("j_absIso3", "absIso dR < 0.3 ", absiso_nbin, 0, absiso_end);
   j_absIso[3]  = new TH1D("j_absIso4", "absIso dR < 0.4 ", absiso_nbin, 0, absiso_end);
   j_absIso[4]  = new TH1D("j_absIso5", "absIso dR < 0.5 ", absiso_nbin, 0, absiso_end);

   j_relIso[0]  = new TH1D("j_relIso1", "relIso dR < 0.1 ", reliso_nbin, 0, reliso_end);
   j_relIso[1]  = new TH1D("j_relIso2", "relIso dR < 0.2 ", reliso_nbin, 0, reliso_end);
   j_relIso[2]  = new TH1D("j_relIso3", "relIso dR < 0.3 ", reliso_nbin, 0, reliso_end);
   j_relIso[3]  = new TH1D("j_relIso4", "relIso dR < 0.4 ", reliso_nbin, 0, reliso_end);
   j_relIso[4]  = new TH1D("j_relIso5", "relIso dR < 0.5 ", reliso_nbin, 0, reliso_end);

   AbsBgRate[0]  = new TH1D("AbsBgRate1", "Background rate in 95% signal efficiency (Abs), Depth_1", 5, 0., 0.5);
   RelBgRate[0]  = new TH1D("RelBgRate1", "Background rate in 95% signal efficiency (Rel), Depth_1", 5, 0., 0.5);
   HitBgRate[0]  = new TH1D("HitBgRate1", "Background rate in 95% signal efficiency (Hit), Depth_1", 5, 0., 0.5);

   AbsBgRate[1]  = new TH1D("AbsBgRate2", "Background rate in 95% signal efficiency (Abs), Depth_2", 5, 0., 0.5);
   RelBgRate[1]  = new TH1D("RelBgRate2", "Background rate in 95% signal efficiency (Rel), Depth_2", 5, 0., 0.5);
   HitBgRate[1]  = new TH1D("HitBgRate2", "Background rate in 95% signal efficiency (Hit), Depth_2", 5, 0., 0.5);

   AbsBgRate[2]  = new TH1D("AbsBgRate3", "Background rate in 95% signal efficiency (Abs), Depth_3", 5, 0., 0.5);
   RelBgRate[2]  = new TH1D("RelBgRate3", "Background rate in 95% signal efficiency (Rel), Depth_3", 5, 0., 0.5);
   HitBgRate[2]  = new TH1D("HitBgRate3", "Background rate in 95% signal efficiency (Hit), Depth_3", 5, 0., 0.5);

   AbsBgRate[3]  = new TH1D("AbsBgRate4", "Background rate in 95% signal efficiency (Abs), Depth_4", 5, 0., 0.5);
   RelBgRate[3]  = new TH1D("RelBgRate4", "Background rate in 95% signal efficiency (Rel), Depth_4", 5, 0., 0.5);
   HitBgRate[3]  = new TH1D("HitBgRate4", "Background rate in 95% signal efficiency (Hit), Depth_4", 5, 0., 0.5);

   AbsSgRate[0]  = new TH1D("AbsSgRate1", "Signal Efficiency in 95% signal efficiency (Abs), Depth_1", 5, 0., 0.5);
   RelSgRate[0]  = new TH1D("RelSgRate1", "Signal Efficiency in 95% signal efficiency (Rel), Depth_1", 5, 0., 0.5);
   HitSgRate[0]  = new TH1D("HitSgRate1", "Signal Efficiency in 95% signal efficiency (Hit), Depth_1", 5, 0., 0.5);

   AbsSgRate[1]  = new TH1D("AbsSgRate2", "Signal Efficiency in 95% signal efficiency (Abs), Depth_2", 5, 0., 0.5);
   RelSgRate[1]  = new TH1D("RelSgRate2", "Signal Efficiency in 95% signal efficiency (Rel), Depth_2", 5, 0., 0.5);
   HitSgRate[1]  = new TH1D("HitSgRate2", "Signal Efficiency in 95% signal efficiency (Hit), Depth_2", 5, 0., 0.5);

   AbsSgRate[2]  = new TH1D("AbsSgRate3", "Signal Efficiency in 95% signal efficiency (Abs), Depth_3", 5, 0., 0.5);
   RelSgRate[2]  = new TH1D("RelSgRate3", "Signal Efficiency in 95% signal efficiency (Rel), Depth_3", 5, 0., 0.5);
   HitSgRate[2]  = new TH1D("HitSgRate3", "Signal Efficiency in 95% signal efficiency (Hit), Depth_3", 5, 0., 0.5);

   AbsSgRate[3]  = new TH1D("AbsSgRate4", "Signal Efficiency in 95% signal efficiency (Abs), Depth_4", 5, 0., 0.5);
   RelSgRate[3]  = new TH1D("RelSgRate4", "Signal Efficiency in 95% signal efficiency (Rel), Depth_4", 5, 0., 0.5);
   HitSgRate[3]  = new TH1D("HitSgRate4", "Signal Efficiency in 95% signal efficiency (Hit), Depth_4", 5, 0., 0.5);

   r_relIsoA  = new TH1D("r_relIsoA", "combined relIso - reco", reliso_nbin, 0, reliso_end);
   g_relIsoA  = new TH1D("g_relIsoA", "combined relIso - gen", reliso_nbin, 0, reliso_end);
   w_relIsoA  = new TH1D("w_relIsoA", "combined relIso - genW", reliso_nbin, 0, reliso_end);
   j_relIsoA  = new TH1D("j_relIsoA", "combined relIso - jet", reliso_nbin, 0, reliso_end);

   // looping through the events
   int nEvt = 0 ;
   for ( int i=0; i< totalN ; i++ ) {
       if ( ProcessEvents > 0 && i > ( ProcessEvents - 1 ) ) break;
       tr->GetEntry( i );

       nEvt++; 
       h_nMu->Fill( leaves.nMuons ) ;

       // loop all muons in the event
       for ( int k=0; k< leaves.nMuons; k++){
           TLorentzVector mP4( leaves.muPx[k], leaves.muPy[k], leaves.muPz[k], leaves.muE[k] )  ;
           
           if ( mP4.Pt() < muonCuts[0] || mP4.Pt() > muonCuts[1] ) continue ;
           if ( fabs( mP4.Eta() ) > muonCuts[2] ) continue ;
           h_muPt->Fill( mP4.Pt() ) ;
           h_muE->Fill( mP4.E() ) ;

           //if ( mP4.E() <= muonSel[0] || mP4.E() > muonSel[1] ) continue ;

           // looping thruogh 4 depths
           double theAbs = -1 ;
           double theRel = -1 ;
           for ( int j=0; j<3 ; j++ ) {
               // looping 5 different dR 
               //cout<<" Depth : "<< j+1 <<endl ;
               for ( int r=1; r < 6; ++r ) {
                   double theAbsIso_ = IsoDeposit( "muon", k, j, r ) ;
                   double theAbsIso  = ( theAbsIso_ > absiso_max ) ? absiso_max : theAbsIso_ ;
                   double theRelIso_ = theAbsIso_ / mP4.Pt()   ;
                   double theRelIso  = ( theRelIso_ > reliso_max ) ? reliso_max : theRelIso_ ;
                   int    theIsohit_ = IsoHits( "muon", k, j, r ) ;
                   int    theIsohit  = ( theIsohit_ > isohit_max ) ? isohit_max : theIsohit_ ;

                   r_absIso[r-1]->Fill( theAbsIso + (j*absiso_bound) ) ;
                   r_relIso[r-1]->Fill( theRelIso + (j*reliso_bound) ) ;
                   r_Ihits[r-1]->Fill( theIsohit + (j*isohit_bound) ) ;
		   if ( j==0 && r == 5 )  h_Pt_relIso5->Fill( mP4.Pt(), theRelIso_ ) ;
		   if ( j==0 && r == 5 )  h_E_relIso5->Fill(  mP4.E(),  theRelIso_ ) ;
		   if ( j==0 && r == 5 )  h_Pt_absIso5->Fill( mP4.Pt(), theAbsIso_ ) ;
		   if ( j==0 && r == 5 )  h_E_absIso5->Fill(  mP4.E(),  theAbsIso_ ) ;
		   if ( j==2 && r == 1 )  theAbs = theAbsIso_  ;

                   // combined reliso deposit in different depth and dR 
		   if ( j==0 && r == 2 )  theRel += theRelIso_ ;
		   if ( j==1 && r == 1 )  theRel += theRelIso_ ;
		   if ( j==2 && r == 3 )  theRel += theRelIso_ ;
               }
           }
           h_abs_relIso->Fill( theAbs, theRel ) ;
           r_relIsoA->Fill( theRel ) ;
       }

       // get gen muon information 
       for ( int k=0; k< leaves.nGen; k++) {
           TLorentzVector gP4 = TLorentzVector( leaves.genPx[k], leaves.genPy[k], leaves.genPz[k], leaves.genE[k] ) ;
           if ( gP4.Pt() < muonCuts[0] || gP4.Pt() > muonCuts[1] ) continue ;
           if ( fabs( gP4.Eta() ) > muonCuts[2] ) continue ;

           double scaleFact = ( reScale ) ? GetMuonPtReWeighting( gP4.Pt() ) : 1. ;
           double theAbs = -1 ;
           double theRel = -1 ;
           if ( abs( leaves.momId[k]) == 24 ) {

              // looping 4 different depths 
              for ( int j=0; j<3 ; j++ ) {
                  // looping 5 different dR 
                  for ( int r=1; r < 6; r++ ) {

                      double theAbsIso_ = IsoDeposit( "gen", k, j, r ) ;
                      double theAbsIso  = ( theAbsIso_ > absiso_max ) ? absiso_max : theAbsIso_ ;
                      double theRelIso_ = theAbsIso_ / gP4.Pt()   ;
                      double theRelIso  = ( theRelIso_ > reliso_max ) ? reliso_max : theRelIso_ ;
                      int    theIsohit_ = IsoHits( "gen", k, j, r ) ;
                      int    theIsohit  = ( theIsohit_ > isohit_max ) ? isohit_max : theIsohit_ ;

                      //if (r == 3) printf(" (%d), layer_%d , iso = %f from %d hits\n", k, j, theAbsIso_, theIsohit_ ) ;
                      w_absIso[r-1]->Fill( theAbsIso + (j*absiso_bound) , scaleFact ) ;
                      w_relIso[r-1]->Fill( theRelIso + (j*reliso_bound) , scaleFact ) ;
                      w_Ihits[r-1]->Fill( theIsohit + (j*isohit_bound)  , scaleFact ) ;
		      if ( j==0 && r == 5 )  w_Pt_relIso5->Fill( gP4.Pt(), theRelIso_ ) ;
		      if ( j==0 && r == 5 )  w_E_relIso5->Fill(  gP4.E(),  theRelIso_ ) ;
		      if ( j==0 && r == 5 )  w_Pt_absIso5->Fill( gP4.Pt(), theAbsIso_ ) ;
		      if ( j==0 && r == 5 )  w_E_absIso5->Fill(  gP4.E(),  theAbsIso_ ) ;
		      if ( j==2 && r == 1 )  theAbs = theAbsIso_  ;
		      // combined reliso deposit in different depth and dR 
		      if ( j==0 && r == 2 )  theRel += theRelIso_ ;
		      if ( j==1 && r == 1 )  theRel += theRelIso_ ;
		      if ( j==2 && r == 3 )  theRel += theRelIso_ ;
                  }
              }   
              w_abs_relIso->Fill( theAbs, theRel ) ;
              w_relIsoA->Fill( theRel ) ;
              //cout<<" --------------- "<<endl ;
           } else {

              for ( int j=0; j<3 ; j++ ) {
                  for ( int r=1; r < 6; r++ ) {

                      double theAbsIso_ = IsoDeposit( "gen", k, j, r ) ;
                      double theAbsIso  = ( theAbsIso_ > absiso_max ) ? absiso_max : theAbsIso_ ;
                      double theRelIso_ = theAbsIso_ / gP4.Pt()   ;
                      double theRelIso  = ( theRelIso_ > reliso_max ) ? reliso_max : theRelIso_ ;
                      int    theIsohit_ = IsoHits( "gen", k, j, r ) ;
                      int    theIsohit  = ( theIsohit_ > isohit_max ) ? isohit_max : theIsohit_ ;

                      g_absIso[r-1]->Fill( theAbsIso + (j*absiso_bound) , scaleFact ) ;
                      g_relIso[r-1]->Fill( theRelIso + (j*reliso_bound) , scaleFact ) ;
                      g_Ihits[r-1]->Fill( theIsohit + (j*isohit_bound) , scaleFact ) ;
		      if ( j==0 && r == 5 )  g_Pt_relIso5->Fill( gP4.Pt(), theRelIso_ ) ;
		      if ( j==0 && r == 5 )  g_E_relIso5->Fill(  gP4.E(),  theRelIso_ ) ;
		      if ( j==0 && r == 5 )  g_Pt_absIso5->Fill( gP4.Pt(), theAbsIso_ ) ;
		      if ( j==0 && r == 5 )  g_E_absIso5->Fill(  gP4.E(),  theAbsIso_ ) ;
		      if ( j==2 && r == 1 )  theAbs = theAbsIso_  ;
		      if ( j==0 && r == 3 )  theRel = theRelIso_  ;
		      // combined reliso deposit in different depth and dR 
		      if ( j==0 && r == 2 )  theRel += theRelIso_ ;
		      if ( j==1 && r == 1 )  theRel += theRelIso_ ;
		      if ( j==2 && r == 3 )  theRel += theRelIso_ ;
                  }
              }   
              g_abs_relIso->Fill( theAbs, theRel ) ;
              g_relIsoA->Fill( theRel ) ;
           }
       }
 
       // get jet hcal deposit      
       for ( int k =0; k< leaves.nJets ; k++) {
           TLorentzVector jP4 = TLorentzVector( leaves.jetPx[k], leaves.jetPy[k], leaves.jetPz[k], leaves.jetE[k] ) ;
           h_JetPt->Fill( jP4.Pt() ) ;

           // looping thruogh 4 depths or 3 different typies
           double theRel = -1 ;
           for ( int j=0; j<3 ; j++ ) {
               // looping 5 different dR 
               //cout<<" Depth : "<< j+1 <<endl ;
               for ( int r=1; r < 6; ++r ) {

		   double theAbsIso_ = IsoDeposit( "jet", k, j, r ) ;
                   double theAbsIso  = ( theAbsIso_ > absiso_max ) ? absiso_max : theAbsIso_ ;
                   double theRelIso_ = theAbsIso_ / jP4.Pt()   ;
                   double theRelIso  = ( theRelIso_ > reliso_max ) ? reliso_max : theRelIso_ ;
                   int    theIsohit_ = IsoHits( "jet", k, j, r ) ;
                   int    theIsohit  = ( theIsohit_ > isohit_max ) ? isohit_max : theIsohit_ ;

                   j_absIso[r-1]->Fill( theAbsIso + (j*absiso_bound) ) ;
                   j_relIso[r-1]->Fill( theRelIso + (j*reliso_bound) ) ;
                   j_Ihits[r-1]->Fill( theIsohit + (j*isohit_bound) ) ;
		   // combined reliso deposit in different depth and dR 
		   if ( j==0 && r == 2 )  theRel += theRelIso_ ;
		   if ( j==1 && r == 1 )  theRel += theRelIso_ ;
		   if ( j==2 && r == 3 )  theRel += theRelIso_ ;
               }
           }
           j_relIsoA->Fill( theRel ) ;
       }

   } // end of event looping

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
           iEff bgR_a = BgRatio( w_absIso[idR], g_absIso[idR], absiso_nbin, idep ) ;
           iEff bgR_r = BgRatio( w_relIso[idR], g_relIso[idR], reliso_nbin, idep ) ;
           iEff bgR_h = BgRatio( w_Ihits[idR],  g_Ihits[idR],  isohit_nbin, idep ) ;
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
           iEff sgR_a = SignalEff( w_absIso[idR], g_absIso[idR], absiso_nbin, idep ) ;
           iEff sgR_r = SignalEff( w_relIso[idR], g_relIso[idR], reliso_nbin, idep ) ;
           iEff sgR_h = SignalEff( w_Ihits[idR],  g_Ihits[idR],  isohit_nbin, idep ) ;
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
   delete leg1 ;

   // some basic information
   cout<<" drawing histograms "<<endl;
   h_draw->SetPlotStyle( true );
   h_draw->Draw( h_nMu,  "nMuons", "N of Muons",      "", "logY", 0.95, 1 );
   h_draw->Draw( h_muPt, "muonPt", "muon P_{T} (GeV/c)", "", "logY", 0.95, 1 );
   h_draw->Draw( h_JetPt, "jetPt", "jet P_{T} (GeV/c)", "", "logY", 0.95, 1 );
   h_draw->Draw( h_muE,  "muonE",  "muon E (GeV)",    "", "logY", 0.95, 1 );

   // set histogram's attributions : axis, labelsize(0.05), tickLength(0.03), titleSize(0.04), titleOffset(1)
   h_draw->SetHistoAtt("X", 0.1, 0.07, 0.08, 1. ) ;
   h_draw->SetHistoAtt("Y", 0.1, 0.02, 0.1, 0 ) ;
   h_draw->SetPlotStyle( true, 0.1, 0.12, 0.1, 0.1 ) ;
   h_draw->CreateNxM( "AbsIsoDR1", 1,4 );
   h_draw->DrawNxM( 1, w_absIso[0], "abs. Isolation for gen W muon", "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 2, g_absIso[0], "abs. Isolation for gen muon",   "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 3, r_absIso[0], "abs. Isolation for reco muon",  "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 4, j_absIso[0], "abs. Isolation for reco jet",   "", "logY", 1, 0.1, 0.1, true );

   h_draw->CreateNxM( "AbsIsoDR2", 1,4 );
   h_draw->DrawNxM( 1, w_absIso[1], "abs. Isolation for gen W muon", "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 2, g_absIso[1], "abs. Isolation for gen muon",   "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 3, r_absIso[1], "abs. Isolation for reco muon",  "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 4, j_absIso[1], "abs. Isolation for reco jet",   "", "logY", 1, 0.1, 0.1, true );

   h_draw->CreateNxM( "AbsIsoDR3", 1,4 );
   h_draw->DrawNxM( 1, w_absIso[2], "abs. Isolation for gen W muon", "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 2, g_absIso[2], "abs. Isolation for gen muon",   "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 3, r_absIso[2], "abs. Isolation for reco muon",  "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 4, j_absIso[2], "abs. Isolation for reco jet",   "", "logY", 1, 0.1, 0.1, true );

   h_draw->CreateNxM( "AbsIsoDR4", 1,4 );
   h_draw->DrawNxM( 1, w_absIso[3], "abs. Isolation for gen W muon", "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 2, g_absIso[3], "abs. Isolation for gen muon",   "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 3, r_absIso[3], "abs. Isolation for reco muon",  "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 4, j_absIso[3], "abs. Isolation for reco jet",   "", "logY", 1, 0.1, 0.1, true );

   h_draw->CreateNxM( "AbsIsoDR5", 1,4 );
   h_draw->DrawNxM( 1, w_absIso[4], "abs. Isolation for gen W muon", "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 2, g_absIso[4], "abs. Isolation for gen muon",   "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 3, r_absIso[4], "abs. Isolation for reco muon",  "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 4, j_absIso[4], "abs. Isolation for reco jet",   "", "logY", 1, 0.1, 0.1, true );

   // RelIso
   h_draw->CreateNxM( "RelIsoDR1", 1,4 );
   h_draw->DrawNxM( 1, w_relIso[0], "rel. Isolation for gen W muon", "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 2, g_relIso[0], "rel. Isolation for gen muon",   "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 3, r_relIso[0], "rel. Isolation for reco muon",  "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 4, j_relIso[0], "rel. Isolation for reco jet",   "", "logY", 1, 0.1, 0.1, true );

   h_draw->CreateNxM( "RelIsoDR2", 1,4 );
   h_draw->DrawNxM( 1, w_relIso[1], "rel. Isolation for gen W muon", "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 2, g_relIso[1], "rel. Isolation for gen muon",   "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 3, r_relIso[1], "rel. Isolation for reco muon",  "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 4, j_relIso[1], "rel. Isolation for reco jet",   "", "logY", 1, 0.1, 0.1, true );

   h_draw->CreateNxM( "RelIsoDR3", 1,4 );
   h_draw->DrawNxM( 1, w_relIso[2], "rel. Isolation for gen W muon", "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 2, g_relIso[2], "rel. Isolation for gen muon",   "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 3, r_relIso[2], "rel. Isolation for reco muon",  "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 4, j_relIso[2], "rel. Isolation for reco jet",   "", "logY", 1, 0.1, 0.1, true );

   h_draw->CreateNxM( "RelIsoDR4", 1,4 );
   h_draw->DrawNxM( 1, w_relIso[3], "rel. Isolation for gen W muon", "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 2, g_relIso[3], "rel. Isolation for gen muon",   "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 3, r_relIso[3], "rel. Isolation for reco muon",  "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 4, j_relIso[3], "rel. Isolation for reco jet",   "", "logY", 1, 0.1, 0.1, true );

   h_draw->CreateNxM( "RelIsoDR5", 1,4 );
   h_draw->DrawNxM( 1, w_relIso[4], "rel. Isolation for gen W muon", "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 2, g_relIso[4], "rel. Isolation for gen muon",   "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 3, r_relIso[4], "rel. Isolation for reco muon",  "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 4, j_relIso[4], "rel. Isolation for reco jet",   "", "logY", 1, 0.1, 0.1, true );

   // isohits
   h_draw->CreateNxM( "IsoHitsDR1", 1,4 );
   h_draw->DrawNxM( 1, w_Ihits[0], "N of Isohits for gen W muon", "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 2, g_Ihits[0], "N of Isohits for gen muon",   "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 3, r_Ihits[0], "N of Isohits for reco muon",  "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 4, j_Ihits[0], "N of Isohits for reco jet",   "", "logY", 1, 0.1, 0.1, true );

   h_draw->CreateNxM( "IsoHitsDR2", 1,4 );
   h_draw->DrawNxM( 1, w_Ihits[1], "N of Isohits for gen W muon", "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 2, g_Ihits[1], "N of Isohits for gen muon",   "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 3, r_Ihits[1], "N of Isohits for reco muon",  "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 4, j_Ihits[1], "N of Isohits for reco jet",   "", "logY", 1, 0.1, 0.1, true );

   h_draw->CreateNxM( "IsoHitsDR3", 1,4 );
   h_draw->DrawNxM( 1, w_Ihits[2], "N of Isohits for gen W muon", "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 2, g_Ihits[2], "N of Isohits for gen muon",   "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 3, r_Ihits[2], "N of Isohits for reco muon",  "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 4, j_Ihits[2], "N of Isohits for reco jet",   "", "logY", 1, 0.1, 0.1, true );

   h_draw->CreateNxM( "IsoHitsDR4", 1,4 );
   h_draw->DrawNxM( 1, w_Ihits[3], "N of Isohits for gen W muon", "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 2, g_Ihits[3], "N of Isohits for gen muon",   "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 3, r_Ihits[3], "N of Isohits for reco muon",  "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 4, j_Ihits[3], "N of Isohits for reco jet",   "", "logY", 1, 0.1, 0.1, true );

   h_draw->CreateNxM( "IsoHitsDR5", 1,4 );
   h_draw->DrawNxM( 1, w_Ihits[4], "N of Isohits for gen W muon", "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 2, g_Ihits[4], "N of Isohits for gen muon",   "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 3, r_Ihits[4], "N of Isohits for reco muon",  "", "logY", 1, 0.1, 0.1, false );
   h_draw->DrawNxM( 4, j_Ihits[4], "N of Isohits for reco jet",   "", "logY", 1, 0.1, 0.1, true );

   //h_draw->SetHistoAtt("X", 0, 0, 0, 0 ) ; // reset to histogram attributions to default 
   //h_draw->SetHistoAtt("Y", 0, 0, 0, 0 ) ; // reset to histogram attributions to default 

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

   // record file in histogram format
   theFile->cd() ;
   HistoWrite( "", theFile ) ;
   cout<<" historams written ! "<<endl ;
   theFile->Close() ;
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
       if ( isoMethod == 3 && j  > depth ) continue ;

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
       if ( isoMethod == 3 && j  > depth ) continue ;

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
iEff HcalAna::BgRatio( TH1D* hS, TH1D* hB, int nbin, int depth ) { 

    int bin1 = 1 + depth*( nbin/4 ) ;
    int bin2= (depth+1)*( nbin/4 ) ; 
    double totalS = hS->Integral( bin1, bin2 ) ;
    double totalB = hB->Integral( bin1, bin2 ) ;

    iEff bRs ;
    if ( totalS == (double)0 || totalB == (double)0 ) return bRs ;

    double sR = 0 ;
    double bR = 0 ;
    double subS = 0 ;
    double subB = 0 ;
    int binx = bin1 ;
    do {
        subS   = hS->Integral( bin1, binx ) ;
        subB   = hB->Integral( bin1, binx ) ;
        sR =  subS / totalS ;
        bR =  subB / totalB ;
        binx++ ;
    } while ( sR < 0.95 ) ;
    pair<double,double> bR_Err = h_draw->EffError( totalB, subB );
    //printf("depth: %d, b1: %d, b2: %d,   S: %.1f, B: %.1f, sR: %.2f, bR: %.2f \n", 
    //        depth,     bin1,   bin2,      totalS,  totalB, sR,       bR ) ;
    
    bRs.eff = bR ;
    bRs.errUp = bR_Err.first   ;
    bRs.errDn = bR_Err.second  ;
    return bRs ;
}

iEff HcalAna::SignalEff( TH1D* hS, TH1D* hB, int nbin, int depth ) { 

    int bin1 = 1 + depth*( nbin/4 ) ;
    int bin2= (depth+1)*( nbin/4 ) ; 
    double totalS = hS->Integral( bin1, bin2 ) ;
    double totalB = hB->Integral( bin1, bin2 ) ;

    iEff sRs ;
    if ( totalS == (double)0 || totalB == (double)0 ) return sRs ;

    double sR = 0 ;
    double bR = 0 ;
    double subS = 0 ;
    double subB = 0 ;
    int binx = bin1 ;
    do {
        subS   = hS->Integral( bin1, binx ) ;
        subB   = hB->Integral( bin1, binx ) ;
        sR =  subS / totalS ;
        bR =  subB / totalB ;
        binx++ ;
    } while ( bR < 0.10 ) ;
    pair<double,double> sR_Err = h_draw->EffError( totalS, subS );
    //printf("depth: %d, b1: %d, b2: %d,   S: %.1f, B: %.1f, sR: %.2f, bR: %.2f \n", 
    //        depth,     bin1,   bin2,      totalS,  totalB, sR,       bR ) ;
    
    sRs.eff = sR ;
    sRs.errUp = sR_Err.first   ;
    sRs.errDn = sR_Err.second  ;

    return sRs ;
}

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
            double dr_ = vr[j].second.DeltaR( vg[ pool[j] ].second ) ;
            iM0.idg  = pool[j] ;
            iM0.idr  = j ;
            iM0.ig  = vg[ pool[j] ].first ;
            iM0.ir  = vr[ j ].first ;
            iM0.dr  = dr_ ;
            iM0.dPt = ( vr[j].second.Pt() - vg[ pool[j] ].second.Pt() ) / vg[ pool[j] ].second.Pt()  ;
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

   // 1. Get Signal and background muon pt spectrum
   TTree* tr = Input->TreeMap( dataName );
   setBranchAddresses( tr, leaves );

   int totalN = tr->GetEntries();
   cout<<" from  "<< dataName <<" total entries = "<< totalN <<" Process "<< ProcessEvents <<endl;

   TH1D* g_muPt  = new TH1D("g_muPt",  " Gen Muon Pt distribution ", 25, 0, 250 );
   TH1D* w_muPt  = new TH1D("w_muPt",  " Gen W Muon Pt distribution ", 25, 0, 250 );

   for ( int i=0; i< totalN ; i++ ) {
       if ( ProcessEvents > 0 && i > ( ProcessEvents - 1 ) ) break;
       tr->GetEntry( i );
       for ( int k=0; k< leaves.nGen; k++) {
           TLorentzVector gP4 = TLorentzVector( leaves.genPx[k], leaves.genPy[k], leaves.genPz[k], leaves.genE[k] ) ;
           if ( gP4.Pt() < muonCuts[0] || gP4.Pt() > muonCuts[1] ) continue ;
           if ( fabs( gP4.Eta() ) > muonCuts[2] ) continue ;

           double theP4 = ( gP4.Pt() > 249 ) ? 249.9 : gP4.Pt() ;
           if ( abs( leaves.momId[k]) == 24 ) w_muPt->Fill( theP4 ) ; // signal 
           else                               g_muPt->Fill( theP4 ) ; // background 
           
       }
   }

   h_draw->Draw(       g_muPt, "", "muon P_{T} (GeV/c)", "", "logY", 0.95, 2 );
   h_draw->DrawAppend( w_muPt, "genMuonPt", 0.75, 4 );

   // 2. Calculate and record the scaling factor
   FILE* logfile = fopen( "MuPtReWeight.log" ,"w"); 
   TH1D* h1_ = (TH1D*) g_muPt->Clone() ;
   TH1D* h2_ = (TH1D*) w_muPt->Clone() ;

   h1_->Scale( 1./ g_muPt->Integral() ) ;
   h2_->Scale( 1./ w_muPt->Integral() ) ;

   int nbin = h1_->GetNbinsX() ;
   for ( int i=1 ; i <= nbin ; i++ ) {
       double x1 = h1_->GetBinCenter( i ) ;
       double w1 = h1_->GetBinWidth( i ) ;
       double b1 = h1_->GetBinContent( i ) ;
       double b2 = h2_->GetBinContent( i ) ;
       double sc = ( b2 < 0.000001 ) ? 0 :  b1/b2 ;  
       fprintf(logfile," %1d %.1f %.1f  %.3f \n", i,  x1-(w1/2), x1+(w1/2),  sc ) ;
   }
 
   delete g_muPt ;
   delete w_muPt ;
   fclose( logfile ) ;

}

void HcalAna::ReadMuonPtReWeighting( ) {

   FILE* logfile = fopen( "MuPtReWeight.log" ,"r"); 
   int ibin, r1 ;
   float ptl , ptu , sf ;
   for ( int i = 0; i < 25 ; i++ ) {
       r1 = fscanf(logfile, "%d", &ibin );
       r1 = fscanf(logfile, "%f", &ptl  );
       r1 = fscanf(logfile, "%f", &ptu  );
       r1 = fscanf(logfile, "%f", &sf   );
       muPt_l[i] = ptl ;
       muPt_h[i] = ptu ;
       scaleF[i] = sf  ;
       if ( r1 != 1 ) cout<<" reading error "<<endl ;
   }

   fclose( logfile ) ;
}

double HcalAna::GetMuonPtReWeighting( double muPt ) {

       int i = muPt / 10  ;
       if ( i > 24 ) i = 24 ;
       double scaleFactor = scaleF[i] ;
 
       //cout<<" muPt = "<< muPt <<"  i = "<< i <<" scf: "<< scaleF[i] <<endl ;
       return scaleFactor ;
}

// depth: 1~3 , nbin : total number of bins of the hIso 
// not well-developed yet ....
double HcalAna::HistPDF( double x, TH1D* hIso, int depth, int nbin ) {

    TH1D* hPDF = (TH1D*) hIso->Clone() ;
    int inib = depth + ( nbin/4 )*(depth - 1) ;
    int endb = ( nbin/4 )* depth  ;

    double totalN = hPDF->Integral( inib, endb ); 
    hPDF->Scale( 1. / totalN )  ; 

    double iprob = 0 ;
    int bin95 = 1;
    
    do {
       iprob += hPDF->GetBinContent( bin95 );
       bin95++ ;
    } while( iprob < 0.95 ) ;

    int theBin = hPDF->FindBin(x); 

    // average out the tail 
    double prob = 0 ;
    if ( theBin >= bin95 ) {
       int outb = (nbin/4) - bin95 ;
       prob =  (0.05 / outb) ;
    } else {
       prob = hPDF->GetBinContent( theBin ) ;
    }
    
    return prob*(nbin/4) ;
}

void HcalAna::HistoWrite(  string theFolder , TFile* file ) {

    if ( theFolder.size() > 0 ) file->cd( theFolder.c_str() ); 

    h_nMu->Write()   ;
    h_muE->Write()   ;
    h_muPt->Write()  ;
    h_JetPt->Write() ;

    g_Pt_relIso5->Write()  ;
    g_E_relIso5->Write()   ;
    w_Pt_relIso5->Write()  ;
    w_E_relIso5->Write()   ;
    h_Pt_relIso5->Write()  ;
    h_E_relIso5->Write()   ;

    g_Pt_absIso5->Write() ;
    g_E_absIso5->Write()  ;
    w_Pt_absIso5->Write() ;
    w_E_absIso5->Write()  ;
    h_Pt_absIso5->Write() ;
    h_E_absIso5->Write()  ;

    h_abs_relIso->Write() ;
    w_abs_relIso->Write() ;
    g_abs_relIso->Write() ;

    for ( int i=0; i< 5 ; i++) {
        r_absIso[i]->Write() ;
	r_relIso[i]->Write() ;
	r_Ihits[i]->Write() ;
	g_absIso[i]->Write() ;
	g_relIso[i]->Write() ;
	g_Ihits[i]->Write() ;
	w_absIso[i]->Write() ;
	w_relIso[i]->Write() ;
	w_Ihits[i]->Write() ;
	j_absIso[i]->Write() ;
	j_relIso[i]->Write() ;
	j_Ihits[i]->Write() ;
    }

    for ( int i=0; i< 4 ; i++) {
        AbsBgRate[i]->Write()  ;
	RelBgRate[i]->Write()  ;
	HitBgRate[i]->Write()  ;

        AbsSgRate[i]->Write()  ;
	RelSgRate[i]->Write()  ;
	HitSgRate[i]->Write()  ;
    }

}
