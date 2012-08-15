#ifndef hDraws_H
#define hDraws_H

#include <TObject.h>
#include <TLorentzVector.h>
#include <TMath.h>
#include <TString.h>
#include <TStyle.h>
#include <TF1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TGraphAsymmErrors.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <TSystem.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "AnaInput.h"

using namespace std;

class hDraw {

  public:

  hDraw( string datacardfile = "DataCard.txt" ) ;
  ~hDraw() ;

  void Draw ( TH1D* h1, string plotName, string xTitle, string yTitle, string logY, float statY, int color = 1, TLegend* leg = NULL ) ;
  void DrawAppend ( TH1D* h1, string plotName, float statY, int color = 4, TLegend* leg = NULL ) ;


  void CreateNxM( string plotName , int N, int M ) ;
  void DrawNxM( int id, TH1D* h1, string xTitle, string yTitle, string logY, int color = 1, float titleFontSize =0.05, float statFontSize = 0.05, bool close = true ) ;
  void DrawNxM( int id, TH1D* h1, int color, TLegend* leg = NULL, bool close = true ) ;
  void DrawNxM( int id, TH2D* h2, string xTitle, string yTitle, string logZ, int nColor = 5, float titleFontSize =0.05, float statFontSize = 0.05, bool close = true ) ;

  void FitNDraw( TH1D* h1, string plotName, string xTitle, string yTitle, string logY, float statY, int color = 1, TLegend* leg=NULL ) ;
  void FitNDrawAppend ( TH1D* h1, string plotName, float statY, int color = 4, TLegend* leg = NULL ) ;
  void SetFitParameter( string fitFunc_ , double fitMin_, double fitMax_ , int nFitPara_,  double* initVals, int color = 2 ) ;
  void SetFitParameter( string fitFunc_ , TH1D* h1,  double fitMin_, double fitMax_ , int nFitPara_, int color = 2 ) ;

  void EffPlot( TH1D* hCut, TH1D* hAll, string xlable, double minBinContent, int beginBin = 1, int endBin = -1 , string graphName = "Efficiency" ) ;
  pair<double, double> EffError( double N_all, double N_pass ) ;

  static Double_t BinomialErr( Double_t* x, Double_t* par ) ;
  static Double_t fExp(Double_t *v, Double_t *par) ;
  static Double_t fitGS(Double_t *v, Double_t *par) ;

  void EffProbPlot( double N_all, string graphName = "EffErrFunction" ) ;

  void SetHistoAtt( string axis, float labelSize, float tickLength, float titleSize, float titleOffset ) ;
  void SetHistoAtt( TH1D* h1 ) ;
  void SetHistoAtt( TH2D* h2 ) ;
  void SetPlotStyle( bool StatBoxON_, float tMargin=0.1 , float bMargin=0.1 , float lMargin=0.1 , float rMargin=0.1 ) ;

  private:

  string fitFunc ;
  double fitMin, fitMax  ;
  int nFitPara ;

  AnaInput*     Input;

  string hfolder  ;
  string plotType ;

  TF1* func1 ;

  TCanvas*  c1 ;
  TCanvas*  c2 ;
  TCanvas*  c3 ;

  TString plotname2 ;

  // histogram attributions
  float labelSize_x ;
  float tickLength_x ;
  float titleSize_x ;
  float titleOffset_x ;
  float labelSize_y ;
  float tickLength_y ;
  float titleSize_y ;
  float titleOffset_y ;
  //string axis_ ;

  bool StatBoxOn ;

};

#endif
