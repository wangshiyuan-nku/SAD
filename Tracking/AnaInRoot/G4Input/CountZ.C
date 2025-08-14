#include <iostream>
#include <fstream>
#include "TFile.h"
#include "TTree.h"
using namespace std;
using namespace TMath;

void CountZ(){
  void countNum(bool ifElec,TString MODE, double zmin, double zmax);
  TString mode;
  double zmin,zmax;
  bool ifElec=true;

  zmin=-999; zmax=999;

  ifElec=true; 
  mode="T";
  countNum(ifElec,mode, zmin, zmax);

//  ifElec=false;
//  mode="T";
//  countNum(ifElec,mode, zmin, zmax);

  ifElec=true;
  mode="C";
  countNum(ifElec,mode, zmin, zmax);

//  ifElec=false;
//  mode="C";
//  countNum(ifElec,mode, zmin, zmax);

  ifElec=true;
  mode="B";
  countNum(ifElec,mode, zmin, zmax);

//  ifElec=false;
//  mode="B";
//  countNum(ifElec,mode, zmin, zmax);
}

void countNum(bool ifElec,TString mode, double zmin, double zmax){
  TString fileName;
  if(ifElec){
    fileName="G4InputElec.root";
  } else{
    fileName="G4InputPosi.root";
  }

  TString treeName;
  if(mode=="T") treeName="tous";
  else if(mode=="C") treeName="coul";
  else if(mode=="B") treeName="brem";
  else return;

  TFile* f1=new TFile(fileName,"read");
  TTree* t1=(TTree*)f1->Get(treeName);
  Double_t x,y,z,dN,dX;
  t1->SetBranchAddress("x",&x);
  t1->SetBranchAddress("y",&y);
  t1->SetBranchAddress("z",&z);
  t1->SetBranchAddress("dN",&dN);    

  double N1=0;
  int ntotal=t1->GetEntries();
  for(int i=0;i<ntotal;i++){
    t1->GetEntry(i);
    if(z>zmin&&z<zmax){
      N1+=dN;
    }
  }
  double TimeWindow=2e-6;
  double NparInEvt=N1*TimeWindow;

  N1=N1/1e6;
  if(ifElec) cout<<"e-:"<<endl;
  else cout<<"e+:"<<endl;
  cout<<"mode:"<<mode<<"; z range(:"<<zmin<<","<<zmax<<"); N:"<<Form("%.4lf",N1)<<"MHz"<<endl;
  cout<<"Expected particles in a event:"<<Form("%.2lf",NparInEvt)<<endl;
  delete f1;
}
