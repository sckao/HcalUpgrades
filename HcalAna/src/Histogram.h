#ifndef Histogram_H
#define Histogram_H

#include <TObject.h>
#include <TMath.h>
#include <TString.h>
#include <TStyle.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TFile.h>
#include <TSystem.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

using namespace std;

class IsoHisto {

    public:

    IsoHisto() ;
    ~IsoHisto() ;

    void Fill_RecoIso( int dRi, int depth, double muPt, double Iso, int nhits ) ;
    void Fill_GenIso( int dRi, int depth, double muPt, double Iso, int nhits ) ;
    void Fill_GenWIso( int dRi, int depth, double muPt, double Iso, int nhits ) ;
    
    void Write( string theFolder , TFile* file  ) ;
    
    private:

    // 5 different dR and 4 different layers
    TH1D* r_absIso[5][4] ;
    TH1D* r_relIso[5][4] ;
    TH1D* r_Ihits[5][4] ;

    TH1D* g_absIso[5][4] ;
    TH1D* g_relIso[5][4] ;
    TH1D* g_Ihits[5][4] ;

    TH1D* w_absIso[5][4] ;
    TH1D* w_relIso[5][4] ;
    TH1D* w_Ihits[5][4] ;

    int nbin_rel ;
    int nbin_abs ;
    int nbin_hit ;

    double end_rel ;
    double end_abs ;
    double end_hit ;

    double ini_rel ;
    double ini_abs ;
    double ini_hit ;

};

#endif
