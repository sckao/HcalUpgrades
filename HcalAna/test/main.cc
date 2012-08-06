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
  Input->LinkForests("HcalUpgrade");

  HcalAna   *hAna = new HcalAna( datacardfile );

  vector<string> dataFileNames ;
  Input->GetParameters( "TheData", &dataFileNames );

  for ( size_t i =0 ; i< dataFileNames.size(); i++ ) {
      hAna->ReadTree( dataFileNames[i] );
  } 

  delete hAna ;

  return 0;

}

