#include "Ntuple.h"

void setBranchAddresses(TTree* chain, Ntuple& treeVars)
{
  chain -> SetBranchAddress("runId",       &treeVars.runId);
  chain -> SetBranchAddress("eventId",     &treeVars.eventId);
  chain -> SetBranchAddress("isSignal",    &treeVars.isSignal);
  
  chain -> SetBranchAddress("nLeptons",    &treeVars.nLeptons     );
  chain -> SetBranchAddress("nJets",       &treeVars.nJets      );
  chain -> SetBranchAddress("nVertices",   &treeVars.nVertices  );
  chain -> SetBranchAddress("nGen",        &treeVars.nGen       );

  chain -> SetBranchAddress("vtxX",         treeVars.vtxX      );
  chain -> SetBranchAddress("vtxY",         treeVars.vtxY      );
  chain -> SetBranchAddress("vtxZ",         treeVars.vtxZ      );
  chain -> SetBranchAddress("vtxNdof",      treeVars.vtxNdof   );
  chain -> SetBranchAddress("vtxChi2",      treeVars.vtxChi2   );
  //chain -> SetBranchAddress("vtxNTracks",   treeVars.vtxNTracks );

  chain -> SetBranchAddress("lepPx",        treeVars.lepPx       );
  chain -> SetBranchAddress("lepPy",        treeVars.lepPy       );
  chain -> SetBranchAddress("lepPz",        treeVars.lepPz       );
  chain -> SetBranchAddress("lepE",         treeVars.lepE        );
  chain -> SetBranchAddress("lepE",         treeVars.lepE        );
  chain -> SetBranchAddress("lepIso5",      treeVars.lepIso5     );
  chain -> SetBranchAddress("lepIso4",      treeVars.lepIso4     );
  chain -> SetBranchAddress("lepIso3",      treeVars.lepIso3     );
  chain -> SetBranchAddress("lepIso2",      treeVars.lepIso2     );
  chain -> SetBranchAddress("lepIso1",      treeVars.lepIso1     );
  chain -> SetBranchAddress("lepIhit5",     treeVars.lepIhit5     );
  chain -> SetBranchAddress("lepIhit4",     treeVars.lepIhit4     );
  chain -> SetBranchAddress("lepIhit3",     treeVars.lepIhit3     );
  chain -> SetBranchAddress("lepIhit2",     treeVars.lepIhit2     );
  chain -> SetBranchAddress("lepIhit1",     treeVars.lepIhit1     );
  
  chain -> SetBranchAddress("jetPx",        treeVars.jetPx     );
  chain -> SetBranchAddress("jetPy",        treeVars.jetPy     );
  chain -> SetBranchAddress("jetPz",        treeVars.jetPz     );
  chain -> SetBranchAddress("jetE",         treeVars.jetE      );
  chain -> SetBranchAddress("jetNDau",      treeVars.jetNDau   );
  chain -> SetBranchAddress("jetCM",        treeVars.jetCM     );
  chain -> SetBranchAddress("jetNM",        treeVars.jetNM     );
  chain -> SetBranchAddress("jetMM",        treeVars.jetMM     );
  chain -> SetBranchAddress("jetCEF",       treeVars.jetCEF    );
  chain -> SetBranchAddress("jetCHF",       treeVars.jetCHF    );
  chain -> SetBranchAddress("jetNHF",       treeVars.jetNHF    );
  chain -> SetBranchAddress("jetNEF",       treeVars.jetNEF    );
  chain -> SetBranchAddress("jetMEF",        treeVars.jetMEF    );


  chain -> SetBranchAddress("pdgId",        treeVars.pdgId       );
  chain -> SetBranchAddress("momId",        treeVars.momId       );
  chain -> SetBranchAddress("genPx",        treeVars.genPx       );
  chain -> SetBranchAddress("genPy",        treeVars.genPy       );
  chain -> SetBranchAddress("genPz",        treeVars.genPz       );
  chain -> SetBranchAddress("genE",         treeVars.genE        );
  chain -> SetBranchAddress("genVx",        treeVars.genVx       );
  chain -> SetBranchAddress("genVy",        treeVars.genVy       );
  chain -> SetBranchAddress("genVz",        treeVars.genVz       );
  chain -> SetBranchAddress("genIso5",      treeVars.genIso5     );
  chain -> SetBranchAddress("genIso4",      treeVars.genIso4     );
  chain -> SetBranchAddress("genIso3",      treeVars.genIso3     );
  chain -> SetBranchAddress("genIso2",      treeVars.genIso2     );
  chain -> SetBranchAddress("genIso1",      treeVars.genIso1     );
  chain -> SetBranchAddress("genIhit5",     treeVars.genIhit5     );
  chain -> SetBranchAddress("genIhit4",     treeVars.genIhit4     );
  chain -> SetBranchAddress("genIhit3",     treeVars.genIhit3     );
  chain -> SetBranchAddress("genIhit2",     treeVars.genIhit2     );
  chain -> SetBranchAddress("genIhit1",     treeVars.genIhit1     );
  
}


void setBranches(TTree* chain, Ntuple& treeVars)
{
  chain -> Branch("runId",         &treeVars.runId,                "runId/i");
  chain -> Branch("eventId",       &treeVars.eventId,            "eventId/i");
  chain -> Branch("isSignal",      &treeVars.isSignal,          "isSignal/I");
  
  // RECO VARIABLES
  chain -> Branch("nLeptons",     &treeVars.nLeptons,             "nLeptons/I");
  chain -> Branch("nJets",        &treeVars.nJets,                "nJets/I");
  chain -> Branch("nVertices",    &treeVars.nVertices,            "nVertices/I");
  chain -> Branch("nTotalVtx",    &treeVars.nTotalVtx,            "nTotalVtx/I");
  chain -> Branch("nGen",         &treeVars.nGen,                 "nGen/I");

  chain -> Branch("vtxX",         treeVars.vtxX,                  "vtxX[nVertices]/F");
  chain -> Branch("vtxY",         treeVars.vtxY,                  "vtxY[nVertices]/F");
  chain -> Branch("vtxZ",         treeVars.vtxZ,                  "vtxZ[nVertices]/F");
  chain -> Branch("vtxNdof",      treeVars.vtxNdof,               "vtxNdof[nVertices]/F");
  chain -> Branch("vtxChi2",      treeVars.vtxChi2,               "vtxChi2[nVertices]/F");
  //chain -> Branch("vtxNTracks",   treeVars.vtxNTracks,            "vtxNTracks[nVertices]/F");

  chain -> Branch("lepPx",        treeVars.lepPx,                 "lepPx[nLeptons]/F");
  chain -> Branch("lepPy",        treeVars.lepPy,                 "lepPy[nLeptons]/F");
  chain -> Branch("lepPz",        treeVars.lepPz,                 "lepPz[nLeptons]/F");
  chain -> Branch("lepE",         treeVars.lepE,                  "lepE[nLeptons]/F");
  chain -> Branch("lepIso5",      treeVars.lepIso5,               "lepIso5[nLeptons][5]/F");
  chain -> Branch("lepIso4",      treeVars.lepIso4,               "lepIso4[nLeptons][5]/F");
  chain -> Branch("lepIso3",      treeVars.lepIso3,               "lepIso3[nLeptons][5]/F");
  chain -> Branch("lepIso2",      treeVars.lepIso2,               "lepIso2[nLeptons][5]/F");
  chain -> Branch("lepIso1",      treeVars.lepIso1,               "lepIso1[nLeptons][5]/F");
  chain -> Branch("lepIhit5",     treeVars.lepIhit5,              "lepIhit5[nLeptons][5]/I");
  chain -> Branch("lepIhit4",     treeVars.lepIhit4,              "lepIhit4[nLeptons][5]/I");
  chain -> Branch("lepIhit3",     treeVars.lepIhit3,              "lepIhit3[nLeptons][5]/I");
  chain -> Branch("lepIhit2",     treeVars.lepIhit2,              "lepIhit2[nLeptons][5]/I");
  chain -> Branch("lepIhit1",     treeVars.lepIhit1,              "lepIhit1[nLeptons][5]/I");
  
  chain -> Branch("jetPx",        treeVars.jetPx,                 "jetPx[nJets]/F");
  chain -> Branch("jetPy",        treeVars.jetPy,                 "jetPy[nJets]/F");
  chain -> Branch("jetPz",        treeVars.jetPz,                 "jetPz[nJets]/F");
  chain -> Branch("jetE",         treeVars.jetE,                  "jetE[nJets]/F");
  chain -> Branch("jetNDau",      treeVars.jetNDau,               "jetNDau[nJets]/I");
  chain -> Branch("jetCM",        treeVars.jetCM,                 "jetCM[nJets]/I"  );
  chain -> Branch("jetNM",        treeVars.jetNM,                 "jetNM[nJets]/I"  );
  chain -> Branch("jetMM",        treeVars.jetMM,                 "jetMM[nJets]/I"  );
  chain -> Branch("jetCEF",       treeVars.jetCEF,                "jetCEF[nJets]/F" );
  chain -> Branch("jetCHF",       treeVars.jetCHF,                "jetCHF[nJets]/F" );
  chain -> Branch("jetNEF",       treeVars.jetNEF,                "jetNEF[nJets]/F" );
  chain -> Branch("jetNHF",       treeVars.jetNHF,                "jetNHF[nJets]/F" );
  chain -> Branch("jetMEF",       treeVars.jetMEF,                "jetMEF[nJets]/F" );

  chain -> Branch("pdgId",        treeVars.pdgId,                 "pdgId[nGen]/I");
  chain -> Branch("momId",        treeVars.momId,                 "momId[nGen]/I");
  chain -> Branch("genPx",        treeVars.genPx,                 "genPx[nGen]/F");
  chain -> Branch("genPy",        treeVars.genPy,                 "genPy[nGen]/F");
  chain -> Branch("genPz",        treeVars.genPz,                 "genPz[nGen]/F");
  chain -> Branch("genE",         treeVars.genE,                  "genE[nGen]/F");
  chain -> Branch("genVx",        treeVars.genVx,                 "genVx[nGen]/F");
  chain -> Branch("genVy",        treeVars.genVy,                 "genVy[nGen]/F");
  chain -> Branch("genVz",        treeVars.genVz,                 "genVz[nGen]/F");
  chain -> Branch("genIso5",      treeVars.genIso5,               "genIso5[nGen][5]/F");
  chain -> Branch("genIso4",      treeVars.genIso4,               "genIso4[nGen][5]/F");
  chain -> Branch("genIso3",      treeVars.genIso3,               "genIso3[nGen][5]/F");
  chain -> Branch("genIso2",      treeVars.genIso2,               "genIso2[nGen][5]/F");
  chain -> Branch("genIso1",      treeVars.genIso1,               "genIso1[nGen][5]/F");
  chain -> Branch("genIhit5",     treeVars.genIhit5,              "genIhit5[nGen][5]/I");
  chain -> Branch("genIhit4",     treeVars.genIhit4,              "genIhit4[nGen][5]/I");
  chain -> Branch("genIhit3",     treeVars.genIhit3,              "genIhit3[nGen][5]/I");
  chain -> Branch("genIhit2",     treeVars.genIhit2,              "genIhit2[nGen][5]/I");
  chain -> Branch("genIhit1",     treeVars.genIhit1,              "genIhit1[nGen][5]/I");
  
}


void initializeBranches(TTree* chain, Ntuple& treeVars)
{
  treeVars.runId       = 0;
  treeVars.eventId     = 0; 
  treeVars.isSignal    = 0; 
  
  treeVars.nJets      = 0 ; 
  treeVars.nLeptons   = 0 ; 
  treeVars.nGen       = 0 ; 
  treeVars.nVertices  = 0 ; 
  treeVars.nTotalVtx  = 0 ; 

  for ( int i=0; i< MAXVTX; i++) {
      treeVars.vtxX[i] = 0 ;
      treeVars.vtxY[i] = 0 ;
      treeVars.vtxZ[i] = 0 ;
      treeVars.vtxNdof[i] = 0 ;
      treeVars.vtxChi2[i] = 0 ;
      //treeVars.vtxNTracks[i] = 0 ;
  }

  for ( int i=0; i< MAXJET; i++) {
      treeVars.jetPx[i] = 0 ;
      treeVars.jetPy[i] = 0 ;
      treeVars.jetPz[i] = 0 ;
      treeVars.jetE[i]  = 0 ;
      treeVars.jetNDau[i] = 0 ;
      treeVars.jetCM[i] = 0 ;
      treeVars.jetNM[i] = 0 ;
      treeVars.jetMM[i] = 0 ;
      treeVars.jetCEF[i] = 0 ;
      treeVars.jetCHF[i] = 0 ;
      treeVars.jetNEF[i] = 0 ;
      treeVars.jetNHF[i] = 0 ;
      treeVars.jetMEF[i] = 0 ;

  }
  for ( int i=0; i< MAXLEP; i++) {
      treeVars.lepPx[i] = 0 ;
      treeVars.lepPy[i] = 0 ;
      treeVars.lepPz[i] = 0 ;
      treeVars.lepE[i] = 0 ;
      for ( int j=0; j< 5; j++ ) {
          treeVars.lepIso5[i][j] = 0 ;
          treeVars.lepIso4[i][j] = 0 ;
          treeVars.lepIso3[i][j] = 0 ;
          treeVars.lepIso2[i][j] = 0 ;
          treeVars.lepIso1[i][j] = 0 ;
          treeVars.lepIhit5[i][j] = 0 ;
          treeVars.lepIhit4[i][j] = 0 ;
          treeVars.lepIhit3[i][j] = 0 ;
          treeVars.lepIhit2[i][j] = 0 ;
          treeVars.lepIhit1[i][j] = 0 ;
      }	   
  } 
  for ( int i=0; i< MAXGEN; i++) {
      treeVars.pdgId[i] = 0 ;
      treeVars.momId[i] = 0 ;
      treeVars.genPx[i] = 0 ;
      treeVars.genPy[i] = 0 ;
      treeVars.genPz[i] = 0 ;
      treeVars.genE[i]  = 0 ;
      treeVars.genVx[i] = 0 ;
      treeVars.genVy[i] = 0 ;
      treeVars.genVz[i] = 0 ;
      for ( int j=0; j< 5; j++ ) {
          treeVars.genIso5[i][j] = 0 ;
          treeVars.genIso4[i][j] = 0 ;
          treeVars.genIso3[i][j] = 0 ;
          treeVars.genIso2[i][j] = 0 ;
          treeVars.genIso1[i][j] = 0 ;
          treeVars.genIhit5[i][j] = 0 ;
          treeVars.genIhit4[i][j] = 0 ;
          treeVars.genIhit3[i][j] = 0 ;
          treeVars.genIhit2[i][j] = 0 ;
          treeVars.genIhit1[i][j] = 0 ;
      }	   
  }

}
