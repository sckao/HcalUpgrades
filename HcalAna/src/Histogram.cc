#include "Histogram.h"

IsoHisto::IsoHisto() {

          nbin_abs = 300 ;
	  nbin_rel = 100 ;
	  nbin_hit = 30  ;
	    
	  ini_abs = 0 ;
	  ini_rel = 0 ;
	  ini_hit = 0 ;

	  end_abs = 30 ;
	  end_rel = 5 ;
	  end_hit = 30 ;
	   
	  for ( int j=0; j < 4; j++ ) {

	      r_absIso[0][j]  = new TH1D("r_absIso1", "absIso dR < 0.1 ", nbin_abs, ini_abs, end_abs);
	      r_absIso[1][j]  = new TH1D("r_absIso2", "absIso dR < 0.2 ", nbin_abs, ini_abs, end_abs);
	      r_absIso[2][j]  = new TH1D("r_absIso3", "absIso dR < 0.3 ", nbin_abs, ini_abs, end_abs);
	      r_absIso[3][j]  = new TH1D("r_absIso4", "absIso dR < 0.4 ", nbin_abs, ini_abs, end_abs);
	      r_absIso[4][j]  = new TH1D("r_absIso5", "absIso dR < 0.5 ", nbin_abs, ini_abs, end_abs);

	      r_relIso[0][j]  = new TH1D("r_relIso1", "relIso dR < 0.1 ", nbin_rel, ini_rel, end_rel);
	      r_relIso[1][j]  = new TH1D("r_relIso2", "relIso dR < 0.2 ", nbin_rel, ini_rel, end_rel);
	      r_relIso[2][j]  = new TH1D("r_relIso3", "relIso dR < 0.3 ", nbin_rel, ini_rel, end_rel);
	      r_relIso[3][j]  = new TH1D("r_relIso4", "relIso dR < 0.4 ", nbin_rel, ini_rel, end_rel);
	      r_relIso[4][j]  = new TH1D("r_relIso5", "relIso dR < 0.5 ", nbin_rel, ini_rel, end_rel);

	      r_Ihits[0][j]  = new TH1D("r_Ihits1", "nhits in dR < 0.1 ", nbin_hit, ini_hit, end_hit);
	      r_Ihits[1][j]  = new TH1D("r_Ihits2", "nhits in dR < 0.2 ", nbin_hit, ini_hit, end_hit);
	      r_Ihits[2][j]  = new TH1D("r_Ihits3", "nhits in dR < 0.3 ", nbin_hit, ini_hit, end_hit);
	      r_Ihits[3][j]  = new TH1D("r_Ihits4", "nhits in dR < 0.4 ", nbin_hit, ini_hit, end_hit);
	      r_Ihits[4][j]  = new TH1D("r_Ihits5", "nhits in dR < 0.5 ", nbin_hit, ini_hit, end_hit);

	      g_absIso[0][j]  = new TH1D("g_absIso1", "absIso dR < 0.1 ", nbin_abs, ini_abs, end_abs);
	      g_absIso[1][j]  = new TH1D("g_absIso2", "absIso dR < 0.2 ", nbin_abs, ini_abs, end_abs);
	      g_absIso[2][j]  = new TH1D("g_absIso3", "absIso dR < 0.3 ", nbin_abs, ini_abs, end_abs);
	      g_absIso[3][j]  = new TH1D("g_absIso4", "absIso dR < 0.4 ", nbin_abs, ini_abs, end_abs);
	      g_absIso[4][j]  = new TH1D("g_absIso5", "absIso dR < 0.5 ", nbin_abs, ini_abs, end_abs);

	      g_relIso[0][j]  = new TH1D("g_relIso1", "relIso dR < 0.1 ", nbin_rel, ini_rel, end_rel);
	      g_relIso[1][j]  = new TH1D("g_relIso2", "relIso dR < 0.2 ", nbin_rel, ini_rel, end_rel);
	      g_relIso[2][j]  = new TH1D("g_relIso3", "relIso dR < 0.3 ", nbin_rel, ini_rel, end_rel);
	      g_relIso[3][j]  = new TH1D("g_relIso4", "relIso dR < 0.4 ", nbin_rel, ini_rel, end_rel);
	      g_relIso[4][j]  = new TH1D("g_relIso5", "relIso dR < 0.5 ", nbin_rel, ini_rel, end_rel);

	      g_Ihits[0][j]  = new TH1D("g_Ihits1", "nhits in dR < 0.1 ", nbin_hit, ini_hit, end_hit);
	      g_Ihits[1][j]  = new TH1D("g_Ihits2", "nhits in dR < 0.2 ", nbin_hit, ini_hit, end_hit);
	      g_Ihits[2][j]  = new TH1D("g_Ihits3", "nhits in dR < 0.3 ", nbin_hit, ini_hit, end_hit);
	      g_Ihits[3][j]  = new TH1D("g_Ihits4", "nhits in dR < 0.4 ", nbin_hit, ini_hit, end_hit);
	      g_Ihits[4][j]  = new TH1D("g_Ihits5", "nhits in dR < 0.5 ", nbin_hit, ini_hit, end_hit);

	      w_absIso[0][j]  = new TH1D("w_absIso1", "absIso dR < 0.1 ", nbin_abs, ini_abs, end_abs);
	      w_absIso[1][j]  = new TH1D("w_absIso2", "absIso dR < 0.2 ", nbin_abs, ini_abs, end_abs);
	      w_absIso[2][j]  = new TH1D("w_absIso3", "absIso dR < 0.3 ", nbin_abs, ini_abs, end_abs);
	      w_absIso[3][j]  = new TH1D("w_absIso4", "absIso dR < 0.4 ", nbin_abs, ini_abs, end_abs);
	      w_absIso[4][j]  = new TH1D("w_absIso5", "absIso dR < 0.5 ", nbin_abs, ini_abs, end_abs);

	      w_relIso[0][j]  = new TH1D("w_relIso1", "relIso dR < 0.1 ", nbin_rel, ini_rel, end_rel);
	      w_relIso[1][j]  = new TH1D("w_relIso2", "relIso dR < 0.2 ", nbin_rel, ini_rel, end_rel);
	      w_relIso[2][j]  = new TH1D("w_relIso3", "relIso dR < 0.3 ", nbin_rel, ini_rel, end_rel);
	      w_relIso[3][j]  = new TH1D("w_relIso4", "relIso dR < 0.4 ", nbin_rel, ini_rel, end_rel);
	      w_relIso[4][j]  = new TH1D("w_relIso5", "relIso dR < 0.5 ", nbin_rel, ini_rel, end_rel);

	      w_Ihits[0][j]  = new TH1D("w_Ihits1", "nhits in dR < 0.1 ", nbin_hit, ini_hit, end_hit);
	      w_Ihits[1][j]  = new TH1D("w_Ihits2", "nhits in dR < 0.2 ", nbin_hit, ini_hit, end_hit);
	      w_Ihits[2][j]  = new TH1D("w_Ihits3", "nhits in dR < 0.3 ", nbin_hit, ini_hit, end_hit);
	      w_Ihits[3][j]  = new TH1D("w_Ihits4", "nhits in dR < 0.4 ", nbin_hit, ini_hit, end_hit);
	      w_Ihits[4][j]  = new TH1D("w_Ihits5", "nhits in dR < 0.5 ", nbin_hit, ini_hit, end_hit);

	  } 

}

IsoHisto::~IsoHisto() {

      for (int i=0; i < 5 ; i++) {
          for (int j=0; j < 4 ; j++) {
              delete r_absIso[i][j] ;
	      delete r_relIso[i][j] ;
	      delete r_Ihits[i][j] ;
	      delete g_absIso[i][j] ;
	      delete g_relIso[i][j] ;
	      delete g_Ihits[i][j] ;
	      delete w_absIso[i][j] ;
	      delete w_relIso[i][j] ;
	      delete w_Ihits[i][j] ;
	  }
      } 

}

void IsoHisto::Fill_RecoIso( int dRi, int depth, double muPt, double Iso, int nhits ) {

     for ( int i=0; i< 5; i++) {
         if ( dRi == i ) {
	    double absIso = ( Iso < end_abs ) ? Iso : end_abs ;
	    double relIso = ( Iso/muPt < end_rel ) ? Iso/muPt : end_rel ;
	    double Ihits  = ( nhits < end_hit ) ? nhits : end_hit ;
            r_absIso[i][depth]->Fill( absIso ) ;
            r_relIso[i][depth]->Fill( relIso ) ;
            r_Ihits[i][depth]->Fill( Ihits ) ;
	 } 
     } 
      
}

void IsoHisto::Fill_GenIso( int dRi, int depth, double muPt, double Iso, int nhits ) {

     for ( int i=0; i< 5; i++) {
         if ( dRi == i ) {
	    double absIso = ( Iso < end_abs ) ? Iso : end_abs ;
	    double relIso = ( Iso/muPt < end_rel ) ? Iso/muPt : end_rel ;
	    double Ihits  = ( nhits < end_hit ) ? nhits : end_hit ;
            g_absIso[i][depth]->Fill( absIso ) ;
            g_relIso[i][depth]->Fill( relIso ) ;
            g_Ihits[i][depth]->Fill( Ihits ) ;
	 }   
     } 

}

void IsoHisto::Fill_GenWIso( int dRi, int depth, double muPt, double Iso, int nhits ) {

     for ( int i=0; i< 5; i++) {
         if ( dRi == i ) {
	    double absIso = ( Iso < end_abs ) ? Iso : end_abs ;
	    double relIso = ( Iso/muPt < end_rel ) ? Iso/muPt : end_rel ;
	    double Ihits  = ( nhits < end_hit ) ? nhits : end_hit ;
            w_absIso[i][depth]->Fill( absIso ) ;
            w_relIso[i][depth]->Fill( relIso ) ;
            w_Ihits[i][depth]->Fill( Ihits ) ;
	 }   
     } 

}

void IsoHisto::Write( string theFolder , TFile* file  ) {

    file->cd( theFolder.c_str() );

    for (int i=0; i < 5 ; i++) {
        for (int j=0; j < 4 ; j++) {
            r_absIso[i][j]->Write() ;
	    r_relIso[i][j]->Write() ;
	    r_Ihits[i][j]->Write() ;
	    g_absIso[i][j]->Write() ;
	    g_relIso[i][j]->Write() ;
	    g_Ihits[i][j]->Write() ;
	    w_absIso[i][j]->Write() ;
	    w_relIso[i][j]->Write() ;
	    w_Ihits[i][j]->Write() ;
	} 
    } 

}

