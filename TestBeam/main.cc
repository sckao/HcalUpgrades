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

#include "txt2ntuple.h"

using namespace std; 

int main( int argc, const char* argv[] ) { 

  if ( argc < 2 ) {
     printf("Usage: ./txt2ntp.exe [action] -f [fileName.txt] -w [nBx] \n");
     printf(" action list : all, pro, ana                  \n") ;
     printf(" all : ntuple production + histogram analysis \n") ; 
     printf(" dec : ntuple production                      \n") ; 
     printf(" ana : histogram analysis                     \n") ; 
     printf(" -w is optional, self-determined if not specified \n") ;
     printf(" Example: > ./txt2ntp.exe all -f positrons_140415_1107_FBPMT.txt  \n") ;
     printf(" Example: > ./txt2ntp.exe dec -f positrons_140415_1107_FBPMT.txt -w 20 \n") ;
     printf(" Example: > ./txt2ntp.exe pro -f positrons_140415_1107_FBPMT -w 20 \n") ;
  }

  string fileName ;
  int width  = -1 ;
  int type = 0 ;
  for ( int i=0; i< argc ; i++ ) {

      if ( strcasecmp(argv[i] , "all" )==0 )
         type = 0 ;
      if ( strcasecmp(argv[i] , "dec" )==0 )
         type = 1 ;
      if ( strcasecmp(argv[i] , "ana" )==0 )
         type = 2 ;
      if ( strcasecmp(argv[i] , "pro" )==0 )
         type = 3 ;
      if ( strcasecmp(argv[i] , "-f" )==0 )
         fileName = argv[i+1] ;
      if ( strcasecmp(argv[i] , "-w" )==0 )
         width = atoi(argv[i+1]) ; 
  }


  txt2ntuple   *t2n = new txt2ntuple();
  if ( type == 0 ) {
                   t2n->decode(fileName , width) ;
                   t2n->analysis(fileName , width) ;
  } 
  if ( type == 1 ) t2n->decode(fileName , width) ;
  if ( type == 2 ) t2n->analysis(fileName , width) ;
  if ( type == 3 ) t2n->produce(fileName , width) ;
  

  delete t2n ;

  return 0;

}

