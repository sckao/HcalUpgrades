#ifndef HcalAna_H
#define HcalAna_H

#include "TObject.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <TMath.h>
#include <TF1.h>
#include <TH1.h>
#include <TH2.h>
#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <TSystem.h>
#include "TLorentzVector.h"

#include "AnaInput.h"
#include "hDraw.h"
#include "Ntuple.h"

//#define MAXGEN 20
//#define MAXMU  10
//#define MAXJET 20

//typedef pair<int, TLorentzVector> objID ;

struct objID {

     int type ; //  muon(+/- 13) , jet(0) ,
     int it   ; //  0 ~ 
     TLorentzVector p4 ;

     objID( int type_ , int it_, TLorentzVector p4_ ) {
            type = type_ ;
            it   = it_ ;
            p4   = p4_ ;
     }  
} ;

struct iMatch {

   int    gid ; // the matching id for gen
   int    rid ; // the matching id for reco
   int    ig ; // position in gen collection
   int    ir ; // position in reco collection
   double dr ;
   double dPt ;
   int    type ;  // which object , muon(13) , jet(0)

} ;

// 4 depths, 5 dR
struct isoHisto {

   TH1D* h_abs[4][5] ;
   TH1D* h_rel[4][5] ;
   TH1D* h_hit[4][5] ;

} ;

class HcalAna : public TObject {

public:

   HcalAna( string datacardfile = "DataCard.txt");     
   ~HcalAna();     
   
   void ReadTree( string dataName, bool reScale = false );

   /*
   double IsoDeposit( string Obj, int mu_id, int depth, int dR_i, double offset = 0. , double scale = 1. );
   double IsoDeposit1( int type, string Obj, int mu_id, int dR_i, double offset = 0., double scale = 1. ) ;
   double InclusiveIsoDeposit( string Obj, int mu_id, int depth,  int dR_i, double offset = 0., double scale = 1.) ;
   */

   double IsoDeposit( string type, int mu_id, int depth, int dR_i, double offset = 0 , double scale = 1 ) ;
   int    IsoHits( string type, int mu_id, int depth, int dR_i, int offset = 0 , int scale = 1 ) ;
   iEff BgRatio( TH1D* hS, TH1D* hB, double sgEff = 0.95  );
   iEff SignalEff( TH1D* hS, TH1D* hB, double bgRate = 0.1  );
   double SBRatio( TH1D* hS, TH1D* hB ) ; 

   vector<iMatch> GlobalDRMatch( vector<objID> vr, vector<objID> vg ) ;

   void WriteMuonPtReWeighting( string dataname ) ;
   void ReadMuonPtReWeighting() ;
   double GetMuonPtReWeighting( double muPt ) ;

   void OpenHistograms() ;
   void DrawHistograms() ;
   void HistoWrite( string theFolder , TFile* file ) ;

private:

   TFile *theFile ;
   string hfName ;
   string scaleFile ;

   AnaInput*     Input ;
   hDraw*        h_draw ;

   string hfolder ;
   string plotType ;
   int isoMethod ;
   int ProcessEvents ;
   int SkipEvents ;
   vector<double> muonCuts ;
   vector<double> jetCuts ;
   vector<int> vtxCuts ;

   double muPt_l[25] ;
   double muPt_h[25] ;
   double scaleF[25] ;

   // histograms 
   TH1D* h_nVtx  ;
   TH1D* h_nMu   ;
   TH1D* h_nJet   ;
   TH1D* h_bmuE   ;
   TH1D* h_wmuE   ;
   TH1D* h_bmuPt  ;
   TH1D* h_wmuPt  ;
   TH1D* h_JetPt ;

   isoHisto wH ;
   isoHisto bH ;
   isoHisto rH ;
   isoHisto jH ;

   Ntuple leaves ;

   //ClassDef(HcalAna, 1);
};

//#if !defined(__CINT__)
//    ClassImp(HcalAna);
#endif

