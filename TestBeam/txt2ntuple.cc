#include "txt2ntuple.h"

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

txt2ntuple::txt2ntuple() {

   theFile = 0 ;
   nBx = 20 ;
}

txt2ntuple::~txt2ntuple(){


}

bool txt2ntuple::getValue( int& theValue, string keyword, string line, int N , int shift ) {

    if ( line.find( keyword ) < line.npos ) {
       int pos = line.find( keyword ) ;
       string getName = line.substr( pos + keyword.size() + shift , N  ) ;
       int getValue = atoi( getName.c_str() ) ;
       //printf(" %s : %d \n", keyword.c_str(), getValue ) ;
       theValue = getValue ;
       return true ;
    } else {
       return false ;
    }
}

bool txt2ntuple::getVector( vector<int>& theVector, string keyword, string line, int shift ) {

    vector<int> vvec  ;
    if ( line.find( keyword ) < line.size() ) {
       //cout<<" ==> "<< line.size()<<" ,  "<< line.npos <<endl ;
       int pos  = line.find( keyword ) ;
       string arrVal = line.substr( pos + keyword.size() + shift ) ;
       //printf( "-> %s (%d), ask %d \n", arrVal.c_str(), arrVal.size(), pos + keyword.size() + shift  ) ;
       
       string vtemp ;
       for (string::iterator it = arrVal.begin(); it<= arrVal.end(); it++) {
           if ( (*it) != ',' && (*it) != ' ' && (*it) != '(' && (*it) != ')' && (*it) != '=') vtemp.push_back( *it );
	   if ( (*it) == ' ' || it == arrVal.end() ) {
              if ( vtemp.size() > 0 ) vvec.push_back( atoi( vtemp.c_str() ) ) ;
              vtemp.clear() ;
           }
       }
       theVector.clear() ;
       theVector = vvec ; 
       return true ;
    } else {
       return false ;
    }
    //return vvec ;
}


// Self decode and ntuple producer !
void txt2ntuple::decode( string fileName, int nBx_ ) {

  // Open data file
  printf(" File:%s , nBx is set to %d \n", fileName.c_str(), nBx_ ) ;
  fstream logfile( fileName.c_str() );
  // Create histogram file
  string rfileName = fileName.substr(0, fileName.size()-4 ) ;
  rfileName += ".root" ;
  TFile* rfile = new TFile( rfileName.c_str() ,"RECREATE");

  // 2. Set branches
  theTree  = new TTree ( "hcaltb","hcaltb" ) ;
  theTree->Branch("event",    &treeVars.event,       "event/I");
  theTree->Branch("fiber",    &treeVars.fiber,       "fiber/I");
  theTree->Branch("bx",       &treeVars.bx,          "bx/I");
  theTree->Branch("capId",    &treeVars.capId,       "capId/I");
  theTree->Branch("badCap",   &treeVars.badCap,      "badCap/I");
  theTree->Branch("badData",  &treeVars.badData,     "badData/I");
  theTree->Branch("adc",      treeVars.adc,          "adc[6]/F");
  theTree->Branch("fc",       treeVars.fc,           "fc[6]/F");
  theTree->Branch("tdc",      treeVars.tdc,          "tdc[6]/F");

  // 3. Initialize branches
  treeVars.event   = -1 ;
  treeVars.fiber   = -1 ;
  treeVars.bx      = -1 ;
  treeVars.capId   = -1 ;
  treeVars.badCap  = -1 ;
  treeVars.badData = -1 ;
  for ( int i=0 ; i<6 ; i++) {
     treeVars.adc[i] = -1 ;
     treeVars.fc[i]  = -1 ;
     treeVars.tdc[i] = -1 ;
  }

  // 4. Fill up the tree
  // Data format -> 0:badData, 1: badcap, 2: capID, 3: bx, 4~9: ADC, 10~15: TDC  
  int  ifiber = -1 ;
  int  evtId = 0 ;
  int  badData(0),badCap(0),capId(-1),bx(-1) ;
  float aV[6] , tV[6] ;

  string line ;
  int val = -1 ;
  bool pass = false ;
  vector<int> adcs ;
  vector<int> tdcs ;
  while ( getline( logfile, line) ) {
        
        if ( line[0] == '-' ) {
            pass = getValue( val, "START EVENT", line, 8, 1 ) ;
            if ( pass ) evtId = val ;
            if ( pass ) {
                        nBx = (bx > 0 ) ?  bx+1 : nBx ;
                        //printf(" nBx = %d \n", nBx ) ;
            }            
            if ( evtId < 0 ) continue ;
        } 
        pass  = getValue( val, "Fiber:", line, 2 ) ;
        if ( pass ) {
           ifiber = val ;
           bx = 0 ;
        }
        pass  = getValue( val, "CAP", line, 1 ) ;
        if ( pass ) capId = val ;
 

        if ( line.find( "Suspicious data format" ) < line.npos  )  badData = 1 ;
        if ( line.find( "CAPID-mismatch" ) < line.npos )           badCap  = 1 ;
        //pass = getValue( val, "ADC", line, 4, -20 ) ;
        //if ( pass ) bx = (val-4) / 6 ;

        pass = getVector( adcs, "ADC:", line, 1 ) ;
        for ( size_t j=0; j< adcs.size() && pass ; j++ ) {
            aV[j] = (float)adcs[j] ;
        }
        pass = getVector( tdcs, "TDC:", line, 1 ) ;
        for ( size_t j=0; j< tdcs.size() && pass ; j++ ) {
            tV[j] = (float)tdcs[j] ;
        }
       
        // All information are kept. Fill the tree
        if ( tdcs.size() == 6 ) {
           bx++ ;
           treeVars.event   = evtId ;
           treeVars.fiber   = ifiber ;
	   treeVars.bx      = bx  ;
	   treeVars.capId   = capId ;
	   treeVars.badCap  = badCap ;
	   treeVars.badData = badData ;
	   for ( int k=0; k<6 ; k++) {
               treeVars.adc[k] = aV[k] ;
	       treeVars.fc[k]  = qieMap[ (int)aV[k] ];
	       treeVars.tdc[k] = tV[k] ;
           }
           //printf(" %d (%d %d) %d %d %d -> adc: %.f %.f %.f %.f %.f %.f , tdc %.f %.f %.f %.f %.f %.f \n", 
           //      evtId, bx, capId, badCap, badData, ifiber, 
           //      aV[0], aV[1], aV[2], aV[3], aV[4], aV[5], tV[0], tV[1], tV[2], tV[3], tV[4], tV[5] ) ;

           adcs.clear() ;
           tdcs.clear() ;
           theTree->Fill() ;
        }
        if ( bx >= nBx_ ) continue ;
  }
  rfile->cd() ;
  theTree->Write() ;
  rfile->Close();  

  logfile.close();
  //theTree = 0 ;
  printf("Ntuple file successfully produced ! \n") ;

}

// This is the ntuple producer needed the input from SCDecode.py
void txt2ntuple::produce( string fileName, int nBx_ ) {

  printf(" file:%s , nBx:%d \n", fileName.c_str(), nBx_ ) ;
  // 1. Get txt file and set output root file
  fileName +="_output.txt" ;
  FILE* logfile = fopen( fileName.c_str() , "r");
  string hfileName = fileName.substr(0, fileName.size()-11 ) ;
  hfileName += ".root" ;
  TFile* hfile = new TFile( hfileName.c_str() ,"RECREATE");

  // 2. Set branches
  theTree  = new TTree ( "hcaltb","hcaltb" ) ;
  theTree->Branch("event",    &treeVars.event,       "event/I");
  theTree->Branch("bx",       &treeVars.bx,          "bx/I");
  theTree->Branch("capId",    &treeVars.capId,       "capId/I");
  theTree->Branch("badCap",   &treeVars.badCap,      "badCap/I");
  theTree->Branch("badData",  &treeVars.badData,     "badData/I");
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
  int  evtId = 0 ;
  int  badData(0),badCap(0),capId(-1),bx(-1) ;
  float a1(0.),t1(0.) ;
  float aV[6] , tV[6] ;
  bool eof = false ;
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
     //printf(" event: %d , bx: %d \n", evtId,bx ) ;
     
     //printf("\n") ;
  }

  theTree->Write() ;
  hfile->Close();  

  fclose(logfile);
  theTree = 0 ;
  printf("Ntuple file successfully produced ! \n") ;

}

// Don't need it now  
void txt2ntuple::linkTree( string fileName ) {

  TString theFileName ;
  TChain* theChain = new TChain( "hcaltb" ) ;
  theFileName = fileName.c_str() ;
  cout<<" * fileName = "<< theFileName <<endl;
  theChain->Add( theFileName );
  theTree = theChain ;
  

}

void txt2ntuple::analysis( string fileName, int nBx_ ) {

  if ( nBx_ < 0 ) nBx_ = nBx ;

  // 1. Get ntuple file
  string rfileName ; 

  if ( fileName.substr( fileName.size()-4, 4) ==".txt"  ) {
     rfileName = fileName.substr(0, fileName.size()-4 ) ;
     rfileName += ".root" ;
  } 
  else if ( fileName.substr( fileName.size()-4, 4) =="root" ) {
    rfileName = fileName ;
  } else {
    rfileName = fileName + ".root" ;
  }
  printf(" root file:%s , nBX = %d \n", rfileName.c_str(), nBx_ ) ;
  theFile = TFile::Open( rfileName.c_str(), "READ" );
  theTree = (TTree*) theFile->Get( "hcaltb" );
  theFile->cd();

  // 2. Define histograms
  // Define histograms ; ADC, fC and TDC
  const int nCh = 6 ;
  TH1F* hADC[ nCh ][4] ;
  TH1F* hTDC[ nCh ][4] ;
  TH1F* hfCQ[ nCh ][4] ;
  TH2F* hBX_ADC[ nCh ] ;
  TH2F* hBX_TDC[ nCh ] ;
  TH2F* hBX_fCQ[ nCh ] ;

  char hName1[16], hName2[16], hName3[16] ;
  char hName4[16], hName5[16], hName6[16] ;
  for ( int i = 0; i < nCh; i++) {
      sprintf(hName1,"hBX_ADC%d", i) ;
      sprintf(hName2,"hBX_TDC%d", i) ;
      sprintf(hName3,"hBX_fCQ%d", i) ;
      hBX_ADC[i] = new TH2F( hName1, "Bx vs ADC", nBx_+1, 1, nBx_+1, 256, 0, 256. );
      hBX_TDC[i] = new TH2F( hName2, "Bx vs TDC", nBx_+1, 1, nBx_+1, 64,  0,  64. );
      hBX_fCQ[i] = new TH2F( hName3, "Bx vs fC ", nBx_+1, 1, nBx_+1, 248, edges10 );
      for ( int j = 0; j < 4; j++) {  // 4 capIds 
          sprintf(hName4,"hADC%d_%d", i, j) ;
          sprintf(hName5,"hTDC%d_%d", i, j) ;
          sprintf(hName6,"hfCQ%d_%d", i, j) ;
	  hADC[i][j] = new TH1F( hName4, "ADC ",  256, 0, 256. );
	  hTDC[i][j] = new TH1F( hName5, "TDC (ADC > 50)",  64,  0,  64. );
	  hfCQ[i][j] = new TH1F( hName6, "fC ",  248, edges10 );
      }
  }
  
  // 3. Set variables
  int event(-1),badData(0),badCap(0),capId(-1),bx(-1), fiber(-1) ;
  float adc[6] , fc[6], tdc[6] ;

  theTree->SetBranchAddress("event",    &event );
  theTree->SetBranchAddress("badData",  &badData );
  theTree->SetBranchAddress("badCap",   &badCap );
  theTree->SetBranchAddress("capId",    &capId );
  theTree->SetBranchAddress("bx",       &bx );
  theTree->SetBranchAddress("fiber",    &fiber );

  theTree->SetBranchAddress("adc",    adc );
  theTree->SetBranchAddress("fc",     fc );
  theTree->SetBranchAddress("tdc",    tdc );

  // 5. Loop events/bx
  int totalN = theTree->GetEntries();
  int evtId = 0 ;
  for ( int i= 1 ; i< totalN ; i++ ) {
      if ( evtId % 1000 == 0 && evtId > 999 && bx == 0 ) printf(" ----- processed %8d Events \n", evtId ) ;
      theTree->GetEntry( i );

      if ( badData !=0 ) continue ;
      if ( fiber != 4 ) continue ;
      for ( int j=0; j<6; j++ ) {
          hBX_ADC[j]->Fill( bx, adc[j] ) ;
          hBX_TDC[j]->Fill( bx, tdc[j] ) ;
          hBX_fCQ[j]->Fill( bx, fc[j] ) ;
          for ( int k=0; k<4; k++ ) {
              if ( k != capId ) continue ;
              hADC[j][k]->Fill( adc[j] ) ;
	      hfCQ[j][k]->Fill( fc[j] ) ;
	      if ( adc[j] > 50 ) hTDC[j][k]->Fill( tdc[j] ) ;
          }
      }
      
      if ( bx == 0 ) evtId++ ;
  }

  // 2. Create histogram file
  string hfileName = rfileName.substr(0, rfileName.size()-5 ) ;
  hfileName += "_histo.root" ;
  printf(" histogram file:%s \n", hfileName.c_str()) ;
  TFile* hfile = new TFile( hfileName.c_str() ,"RECREATE");
  //hfile->cd() ;
  // 6. Write histograms
  for ( int j = 0; j < nCh; j++) {
      hBX_ADC[j]->Write();
      hBX_TDC[j]->Write();
      hBX_fCQ[j]->Write();
      for ( int k = 0; k < 4; k++) {
          hADC[j][k]->Write();
	  hTDC[j][k]->Write();
	  hfCQ[j][k]->Write();
      }
  }
  hfile->Close();  
  theFile->Close();

}

