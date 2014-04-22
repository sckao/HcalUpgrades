#ifndef txt2ntuple_H
#define txt2ntuple_H

#include "TObject.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <TMath.h>
#include <TF1.h>
#include <TH1.h>
#include <TH2.h>
#include <TH2F.h>
#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <TSystem.h>
#include <TChain.h>
#include "TLorentzVector.h"

using namespace std;

struct Ntuple {

   int event, fiber, badCap, badData, bx, capId ;
   float adc[6] ;
   float fc[6] ;
   float tdc[6] ;

};

class txt2ntuple : public TObject {

public:

   txt2ntuple();     
   ~txt2ntuple();     
 
   bool getValue( int& theValue, string keyword, string line, int N, int shift = 0) ;  
   bool getVector( vector<int>& theVector, string keyword, string line, int shift = 0) ;

   void decode( string fileName, int nBx_ = -1 );
   void produce( string fileName, int nBx_ );
   void analysis( string fileName, int nBx_ = -1 );
   void linkTree( string fileName ) ;

private:
 
   TFile* theFile ;
   TTree* theTree ;
   Ntuple treeVars ;
   int nBx ; 
   //ClassDef(txt2ntuple, 1);
};

//#if !defined(__CINT__)
//    ClassImp(txt2ntuple);
#endif

