#ifndef Ntuple_h
#define Ntuple_h

#include "TChain.h" 

#define MAXJET 20
#define MAXLEP 10
#define MAXGEN 20
#define MAXVTX 10

struct Ntuple
{
  
  unsigned int runId;
  unsigned int eventId;
  int isSignal ;

  // reco variables
  int   nTotalVtx ;
  int   nVertices ;
  float vtxX[MAXVTX] ;
  float vtxY[MAXVTX] ;
  float vtxZ[MAXVTX] ;
  float vtxNdof[MAXVTX] ;
  float vtxChi2[MAXVTX] ;
  //float vtxNTracks[MAXVTX] ;

  int   nJets ;
  float jetPx[MAXJET];
  float jetPy[MAXJET];
  float jetPz[MAXJET];
  float jetE[MAXJET];
  int   jetNDau[MAXJET];
  int   jetCM[MAXJET];
  int   jetNM[MAXJET];
  int   jetMM[MAXJET];
  float jetCEF[MAXJET];
  float jetCHF[MAXJET];
  float jetNEF[MAXJET];
  float jetNHF[MAXJET];
  float jetMEF[MAXJET];

  
  int   nLeptons ;
  float lepPx[MAXLEP];
  float lepPy[MAXLEP];
  float lepPz[MAXLEP];
  float lepE[MAXLEP];
  float lepIso5[MAXLEP][5];
  float lepIso4[MAXLEP][5];
  float lepIso3[MAXLEP][5];
  float lepIso2[MAXLEP][5];
  float lepIso1[MAXLEP][5];
  int   lepIhit5[MAXLEP][5];
  int   lepIhit4[MAXLEP][5];
  int   lepIhit3[MAXLEP][5];
  int   lepIhit2[MAXLEP][5];
  int   lepIhit1[MAXLEP][5];

  // Gen Particle information
  int nGen ; 
  int pdgId[MAXGEN] ;
  int momId[MAXGEN] ;
  float genPx[MAXGEN] ; 
  float genPy[MAXGEN] ; 
  float genPz[MAXGEN] ; 
  float genE[MAXGEN] ; 
  float genVx[MAXGEN] ; 
  float genVy[MAXGEN] ; 
  float genVz[MAXGEN] ; 
  float genIso5[MAXGEN][5];
  float genIso4[MAXGEN][5];
  float genIso3[MAXGEN][5];
  float genIso2[MAXGEN][5];
  float genIso1[MAXGEN][5];
  int   genIhit5[MAXGEN][5];
  int   genIhit4[MAXGEN][5];
  int   genIhit3[MAXGEN][5];
  int   genIhit2[MAXGEN][5];
  int   genIhit1[MAXGEN][5];

};

// ------------------------------------------------------------------------
//! branch addresses settings
void setBranchAddresses(TTree* chain, Ntuple& treeVars);

//! create branches for a tree
void setBranches(TTree* chain, Ntuple& treeVars);

//! initialize branches
void initializeBranches(TTree* chain, Ntuple& treeVars);


#endif
