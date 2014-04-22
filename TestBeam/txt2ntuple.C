#include <TMath.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <TString.h>
#include <TSystem.h>
#include <TFile.h>
#include <TH2.h>
#include <TH1.h>
#include <TH1D.h>
#include <TF1.h>
//#include <TStyle.h>
//#include <TCanvas.h>
//#include <TFrame.h>
//#include <TFormula.h>
//#include <TGraphErrors.h>
//#include <TGraph.h>
#include <TMath.h>
#include <TChain.h>

struct Ntuple {

   int event, bx, capId, badCap, badData ;
   float adc[6] ;
   float fc[6] ;
   float tdc[6] ;
   
};

Double_t edges10[249] = { 0, 
        1.58,   4.73,   7.88,   11.0,   14.2,   17.3,   20.5,   23.6,
        26.8,   29.9,   33.1,   36.2,   39.4,   42.5,   45.7,   48.8,
        53.6,   60.1,   66.6,   73.0,   79.5,   86.0,   92.5,   98.9,
        105,    112,    118,    125,    131,    138,    144,    151,
        157,    164,    170,    177,    186,    199,    212,    225,
        238,    251,    264,    277,    289,    302,    315,    328,
        341,    354,    367,    380,    393,    406,    418,    431,
        444,    464,    490,    516,    542,    568,    594,    620,
        645,    670,    695,    720,    745,
        771,    796,    821,    846,    871,    897,    922,    947,
        960,    1010,   1060,   1120,   1170,   1220,   1270,   1320,
        1370,   1430,   1480,   1530,   1580,   1630,   1690,   1740,
        1790,   1840,   1890,   1940,   2020,   2120,   2230,   2330,
        2430,   2540,   2640,   2740,   2850,   2950,   3050,   3150,
        3260,   3360,   3460,   3570,   3670,   3770,   3880,   3980,
        4080,   4240,   4450,   4650,   4860,   5070,   5280,   5490,
        5680,   5880,   6080,   6280,   6480,
        6680,   6890,   7090,   7290,   7490,   7690,   7890,   8090,
        8400,   8810,   9220,   9630,   10000,  10400,  10900,  11300,
        11700,  12100,  12500,  12900,  13300,  13700,  14100,  14500,
        15000,  15400,  15800,  16200,  16800,  17600,  18400,  19300,
        20100,  20900,  21700,  22500,  23400,  24200,  25000,  25800,
        26600,  27500,  28300,  29100,  29900,  30700,  31600,  32400,
        33200,  34400,  36100,  37700,  39400,  41000,  42700,  44300,
        45900,  47600,  49200,  50800,  52500,
        54100,  55700,  57400,  59000,  60600,  62200,  63900,  65500,
        68000,  71300,  74700,  78000,  81400,  84700,  88000,  91400,
        94700,  98100,  101000, 105000, 108000, 111000, 115000, 118000,
        121000, 125000, 128000, 131000, 137000, 145000, 152000, 160000,
        168000, 176000, 183000, 191000, 199000, 206000, 214000, 222000,
        230000, 237000, 245000, 253000, 261000, 268000, 276000, 284000,
        291000, 302000, 316000, 329000, 343000, 356000, 370000, 384000, 398000
};

Double_t qieMap[257] = {
        1.58,   4.73,   7.88,   11.0,   14.2,   17.3,   20.5,   23.6,
        26.8,   29.9,   33.1,   36.2,   39.4,   42.5,   45.7,   48.8,
        53.6,   60.1,   66.6,   73.0,   79.5,   86.0,   92.5,   98.9,
        105,    112,    118,    125,    131,    138,    144,    151,
        157,    164,    170,    177,    186,    199,    212,    225,
        238,    251,    264,    277,    289,    302,    315,    328,
        341,    354,    367,    380,    393,    406,    418,    431,
        444,    464,    490,    516,    542,    568,    594,    620,
        569,    594,    619,    645,    670,    695,    720,    745,
        771,    796,    821,    846,    871,    897,    922,    947,
        960,    1010,   1060,   1120,   1170,   1220,   1270,   1320,
        1370,   1430,   1480,   1530,   1580,   1630,   1690,   1740,
        1790,   1840,   1890,   1940,   2020,   2120,   2230,   2330,
        2430,   2540,   2640,   2740,   2850,   2950,   3050,   3150,
        3260,   3360,   3460,   3570,   3670,   3770,   3880,   3980,
        4080,   4240,   4450,   4650,   4860,   5070,   5280,   5490,
        5080,   5280,   5480,   5680,   5880,   6080,   6280,   6480,
        6680,   6890,   7090,   7290,   7490,   7690,   7890,   8090,
        8400,   8810,   9220,   9630,   10000,  10400,  10900,  11300,
        11700,  12100,  12500,  12900,  13300,  13700,  14100,  14500,
        15000,  15400,  15800,  16200,  16800,  17600,  18400,  19300,
        20100,  20900,  21700,  22500,  23400,  24200,  25000,  25800,
        26600,  27500,  28300,  29100,  29900,  30700,  31600,  32400,
        33200,  34400,  36100,  37700,  39400,  41000,  42700,  44300,
        41100,  42700,  44300,  45900,  47600,  49200,  50800,  52500,
        54100,  55700,  57400,  59000,  60600,  62200,  63900,  65500,
        68000,  71300,  74700,  78000,  81400,  84700,  88000,  91400,
        94700,  98100,  101000, 105000, 108000, 111000, 115000, 118000,
        121000, 125000, 128000, 131000, 137000, 145000, 152000, 160000,
        168000, 176000, 183000, 191000, 199000, 206000, 214000, 222000,
        230000, 237000, 245000, 253000, 261000, 268000, 276000, 284000,
        291000, 302000, 316000, 329000, 343000, 356000, 370000, 384000, 398000
};

//void txt2histo( TString fileName, int nBefore, int nAfter ) {
void txt2ntuple( string fileName, int nBx ) {

  // 1. Get txt file and set output root file
  fileName+="_output.txt" ;
  TString txtFile = fileName ;
  FILE* logfile = fopen( txtFile , "r");
  string hfileName = fileName.substr(0, fileName.size()-11 ) ;
  hfileName += ".root" ;
  TFile* hfile = new TFile( hfileName.c_str() ,"RECREATE");

  // 2. Set branches
  TTree* theTree  = new TTree ( "hcaltb","hcaltb" ) ;
  Ntuple treeVars ;
  theTree->Branch("event",    &treeVars.event,       "event/i");
  theTree->Branch("bx",       &treeVars.bx,          "bx/i");
  theTree->Branch("capId",    &treeVars.capId,       "capId/i");
  theTree->Branch("badCap",   &treeVars.badCap,      "badCap/i");
  theTree->Branch("badData",  &treeVars.badData,     "badData/i");
  theTree->Branch("adc",      treeVars.adc,          "adc[6]/F");
  theTree->Branch("fc",       treeVars.fc,           "fc[6]/F");
  theTree->Branch("tdc",      treeVars.tdc,          "tdc[6]/F");

  // 3. Initialize branches
  treeVars.event   = 0 ;
  treeVars.bx      = -1 ;
  treeVars.capId   = -1 ;
  treeVars.badCap  = 0 ;
  treeVars.badData = 0 ;
  for ( int i=0 ; i<6 ; i++) {
     treeVars.adc[i] = -1 ;
     treeVars.fc[i]  = -1 ;
     treeVars.tdc[i] = -1 ;
  }

  // 4. Fill up the tree
  // Data format -> 0:badData, 1: badcap, 2: capID, 3: bx, 4~9: ADC, 10~15: TDC  
  bool eof = false ;
  int  evtId = 0 ;
  int  badData(0),badCap(0),capId(-1),bx(-1) ;
  float a1(0.),t1(0.) ;
  float aV[6] , tV[6] ;
  while ( !eof ) {
     a1 = 0;
     t1 = 0;
     int fo = -1 ;
     for ( int j=0; j<16; j++ ) {
         if ( j ==0           ) fo = fscanf(logfile, "%d" , &badData );
         if ( j ==1           ) fo = fscanf(logfile, "%d" , &badCap );
         if ( j ==2           ) fo = fscanf(logfile, "%d" , &bx );
         if ( j ==3           ) fo = fscanf(logfile, "%d" , &capId );
         //if ( j ==3           ) printf(" [ %d %d %d %d] " , badData, badCap, bx, capId );
         if ( j >=4 && j < 10 ) {
            fo = fscanf(logfile, "%f" , &a1 );
            aV[j-4] = a1;
            //printf(" %.f,", a1 ) ;
         }
         if ( j >=10 && j < 16 ) {
            fo = fscanf(logfile, "%f" , &t1 );
            tV[j-10] = t1;
            //printf(" %.f,", t1 ) ;
         }
         if ( fo != 1 ) break;
     }
     eof = (feof( logfile) != 0) ? true : false ;
     if ( eof ) break ;
     if ( fo != 1 ) printf(" !!! fscan fail %d \n", fo ) ;

     if ( bx == 0 )   evtId++ ;
     treeVars.event   = evtId ;
     treeVars.bx      = bx ;
     treeVars.capId   = capId ;
     treeVars.badCap  = badCap ;
     treeVars.badData = badData ;
     for ( int k=0; k<6 ; k++) {
         treeVars.adc[k] = aV[k] ;
         treeVars.fc[k]  = qieMap[ (int)aV[k] ];
         treeVars.tdc[k] = tV[k] ;
     }
     theTree->Fill() ;
     printf(" event: %d , bx: %d \n", evtId,bx ) ;
     
     //printf("\n") ;
  }

  theTree->Write() ;
  hfile->Close();  

  fclose(logfile);
  printf("Ntuple file successfully produced ! \n") ;

}
