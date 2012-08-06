#ifndef Ntuple_h
#define Ntuple_h

#include "TChain.h" 

#define MAXJET 20
#define MAXMU  10
#define MAXGEN 20

struct Ntuple
{
  
  unsigned int runId;
  unsigned int eventId;
  int isSignal ;

  // reco variables
  int   nVertices ;

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

  
  int   nMuons ;
  float muPx[MAXMU];
  float muPy[MAXMU];
  float muPz[MAXMU];
  float muE[MAXMU];
  float muIso5[MAXMU][5];
  float muIso4[MAXMU][5];
  float muIso3[MAXMU][5];
  float muIso2[MAXMU][5];
  float muIso1[MAXMU][5];
  int   muIhit5[MAXMU][5];
  int   muIhit4[MAXMU][5];
  int   muIhit3[MAXMU][5];
  int   muIhit2[MAXMU][5];
  int   muIhit1[MAXMU][5];

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
