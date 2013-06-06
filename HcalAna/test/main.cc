#include <iostream> 
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <stdio.h>
#include <TString.h>
#include <TSystem.h>
#include <TFile.h>
#include <TTree.h>
#include <TMinuit.h>

#include "AnaInput.h"
#include "HcalAna.h"

using namespace std; 

int main( int argc, const char* argv[] ) { 

  string datacardfile = ( argc > 1 ) ? argv[1] : "DataCard.txt";
  AnaInput        *Input = new AnaInput( datacardfile );

  //Input->LinkForests("Gen");
  string dataFileNames ;
  Input->GetParameters( "TheData", &dataFileNames );

  int module = -1 ;
  Input->GetParameters( "Module", &module ) ;
  cout<<"  Run module "<< module <<endl ;

  HcalAna   *hAna = new HcalAna( datacardfile );

  if ( module == 0 )  hAna->WriteMuonPtReWeighting( dataFileNames );
  if ( module == 1 )  hAna->ReadTree( dataFileNames );
  if ( module == 2 )  hAna->ReadTree( dataFileNames, true );
  if ( module == 3 )  {
                      hAna->OpenHistograms() ;
                      hAna->DrawHistograms() ;
  }
  /*
  vector<string> dataFileNames ;
  Input->GetParameters( "TheData", &dataFileNames );

  for ( size_t i =0 ; i< dataFileNames.size(); i++ ) {
      hAna->ReadTree( dataFileNames[i] );
  } 
  */
  
  delete hAna ;

  return 0;

}

