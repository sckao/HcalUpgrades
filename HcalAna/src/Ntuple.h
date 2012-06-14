#ifndef Ntuple_h
#define Ntuple_h

#include "TChain.h" 

#define MAXJET 15
#define MAXMU 10
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

};

// ------------------------------------------------------------------------
//! branch addresses settings
void setBranchAddresses(TTree* chain, Ntuple& treeVars);

//! create branches for a tree
void setBranches(TTree* chain, Ntuple& treeVars);

//! initialize branches
void initializeBranches(TTree* chain, Ntuple& treeVars);



#endif
