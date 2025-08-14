#include <iostream>
#include <fstream>
#include "TFile.h"
#include "TTree.h"
using namespace std;
using namespace TMath;

void CountZ(){
  void countNum(TString MODE, double zmin, double zmax);
  TString mode;
  double zmin,zmax;

  mode="T";
  zmin=-10; zmax=-4;
  countNum(mode, zmin, zmax);
  zmin=-4; zmax=0;
  countNum(mode, zmin, zmax);
  zmin=0; zmax=10;
  countNum(mode, zmin, zmax);
  //zmin=4; zmax=10;
  //countNum(mode, zmin, zmax);

  mode="C";
  zmin=-10; zmax=0;
  countNum(mode, zmin, zmax);
  zmin=0; zmax=10;
  countNum(mode, zmin, zmax);

  mode="B";
  zmin=-10; zmax=10;
  countNum(mode, zmin, zmax);
}

void countNum(TString mode, double zmin, double zmax){
  TString fileName="rootfiles/all.root";
  TString treeName;
  if(mode=="T") treeName="Tous";
  else if(mode=="C") treeName="Coul";
  else if(mode=="B") treeName="Brem";
  else return;

  TFile* f1=new TFile(fileName,"read");
  TTree* t1=(TTree*)f1->Get(treeName);
  Double_t x,y,z,dN,dX;
  t1->SetBranchAddress("x",&x);
  t1->SetBranchAddress("y",&y);
  t1->SetBranchAddress("Z",&z);
  t1->SetBranchAddress("dN",&dN);    

  double N1=0;
  int ntotal=t1->GetEntries();
  for(int i=0;i<ntotal;i++){
    t1->GetEntry(i);
    if(z>zmin&&z<zmax){
      N1+=dN;
    }
  }
  N1=N1/1e6;
  cout<<"mode:"<<mode<<"; z range(:"<<zmin<<","<<zmax<<"); N:"<<Form("%.4lf",N1)<<"MHz"<<endl;
  delete f1;
}
