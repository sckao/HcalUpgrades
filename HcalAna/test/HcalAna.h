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
#include <THStack.h>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TRandom.h>
#include <TLeaf.h>
#include <TString.h>
#include <TSystem.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TFrame.h>
#include <TLegend.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include "TLorentzVector.h"

#include "AnaInput.h"
#include "hDraw.h"
#include "Ntuple.h"

//#define MAXGEN 20
//#define MAXMU  10
//#define MAXJET 20

class HcalAna : public TObject {

public:

   HcalAna( string datacardfile = "DataCard.txt");     
   ~HcalAna();     
   
   void ReadTree( string dataName );

   /*
   double IsoDeposit( string Obj, int mu_id, int depth, int dR_i, double offset = 0. , double scale = 1. );
   double IsoDeposit1( int type, string Obj, int mu_id, int dR_i, double offset = 0., double scale = 1. ) ;
   double InclusiveIsoDeposit( string Obj, int mu_id, int depth,  int dR_i, double offset = 0., double scale = 1.) ;
   */

   double IsoDeposit( string type, int mu_id, int depth, int dR_i, double offset = 0 , double scale = 1 ) ;
   int    IsoHits( string type, int mu_id, int depth, int dR_i, int offset = 0 , int scale = 1 ) ;
   double BgRatio( TH1D* hS, TH1D* hB, int nbin, int depth  );

private:

   AnaInput*     Input ;
   hDraw*        h_draw ;

   string hfolder ;
   string plotType ;
   int isoMethod ;
   int ProcessEvents ;
   vector<double> muonCuts ;

   TH1D* r_absIso[5] ;
   TH1D* r_relIso[5] ;
   TH1D* r_Ihits[5] ;
   TH1D* g_absIso[5] ;
   TH1D* g_relIso[5] ;
   TH1D* g_Ihits[5] ;
   TH1D* w_absIso[5] ;
   TH1D* w_relIso[5] ;
   TH1D* w_Ihits[5] ;
   TH1D* j_absIso[5] ;
   TH1D* j_relIso[5] ;
   TH1D* j_Ihits[5] ;

   TH1D* AbsBgRate[4]  ;
   TH1D* RelBgRate[4]  ;
   TH1D* HitBgRate[4]  ;

   double absiso_max ;
   double absiso_bound ;
   double absiso_end ;
   int    absiso_nbin ;

   double reliso_max ;
   double reliso_bound ;
   double reliso_end ;
   int    reliso_nbin ;

   int    isohit_max ;
   int    isohit_bound ;
   double isohit_end ;
   int    isohit_nbin ;

   Ntuple leaves ;

   //ClassDef(HcalAna, 1);
};

//#if !defined(__CINT__)
//    ClassImp(HcalAna);
#endif

