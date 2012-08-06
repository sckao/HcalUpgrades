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

#define MAXGEN 20
#define MAXMU  10
#define MAXJET 20

class HcalAna : public TObject {

public:

   HcalAna( string datacardfile = "DataCard.txt");     
   ~HcalAna();     
   
   void ReadTree( string dataName );
   double IsoDeposit( string type, int mu_id, int depth, int dR_i, double offset = 0. , double scale = 1. );
   int    IsoHits( string type, int mu_id, int depth, int dR_i, int offset = 0 , int scale = 1 ) ;

private:

   AnaInput*     Input ;
   hDraw*        h_draw ;

   string hfolder ;
   string plotType ;
   int ProcessEvents ;
   vector<double> muonCuts ;

   float muPx[MAXMU], muPy[MAXMU], muPz[MAXMU], muE[MAXMU] ;
   float muIso1[MAXMU][5], muIso2[MAXMU][5], muIso3[MAXMU][5], muIso4[MAXMU][5], muIso5[MAXMU][5];   
   int   muIhit1[MAXMU][5], muIhit2[MAXMU][5], muIhit3[MAXMU][5], muIhit4[MAXMU][5], muIhit5[MAXMU][5];   

   int momId[MAXGEN] ;
   float genPx[MAXGEN], genPy[MAXGEN], genPz[MAXGEN], genE[MAXGEN] ;
   float genIso1[MAXGEN][5], genIso2[MAXGEN][5], genIso3[MAXGEN][5], genIso4[MAXGEN][5], genIso5[MAXGEN][5];
   int   genIhit1[MAXGEN][5], genIhit2[MAXGEN][5], genIhit3[MAXGEN][5], genIhit4[MAXGEN][5], genIhit5[MAXGEN][5];
   //float genVx[MAXGEN], genVy[MAXGEN], genVz[MAXGEN] ;

   float jetPx[MAXJET],   jetPy[MAXJET], jetPz[MAXJET],  jetE[MAXJET] ;

   int   nMuons, nJets, nGen ;

   TH1D* r_absIso[5] ;
   TH1D* r_relIso[5] ;
   TH1D* r_Ihits[5] ;
   TH1D* g_absIso[5] ;
   TH1D* g_relIso[5] ;
   TH1D* g_Ihits[5] ;
   TH1D* w_absIso[5] ;
   TH1D* w_relIso[5] ;
   TH1D* w_Ihits[5] ;

   double absiso_max ;
   double absiso_bound ;
   double reliso_max ;
   double reliso_bound ;
   int    isohit_max ;
   int    isohit_bound ;

   //ClassDef(HcalAna, 1);
};

//#if !defined(__CINT__)
//    ClassImp(HcalAna);
#endif

