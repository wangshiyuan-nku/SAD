#include <iostream>
#include <fstream>
#include "TFile.h"
#include "TTree.h"
#include "TRandom.h"
using namespace std;
TRandom* m_rand = new TRandom();

void GeneBes(TTree* t1, int ID){
  double scale=0.1;
 
  int ID_p,Nevt=1;
  TString OutFileName;
  TString OutFile_pre="BesInput/";
  switch(ID){ 
    case 1: 
      OutFileName=OutFile_pre+"touse.txt";
      ID_p=11;
      Nevt=47;
      break;
    case 2:
      OutFileName=OutFile_pre+"tousp.txt";
      ID_p=-11;
      Nevt=66;
      break;
    case 3:
      OutFileName=OutFile_pre+"coule.txt";
      ID_p=11;
      Nevt=8;
      break;
    case 4:
      OutFileName=OutFile_pre+"coulp.txt";
      ID_p=-11;
      Nevt=8;
      break;
    case 5:
      OutFileName=OutFile_pre+"breme.txt";
      ID_p=11;
      Nevt=2;
      break;
    case 6:
      OutFileName=OutFile_pre+"bremp.txt";
      ID_p=-11;
      Nevt=2;
      break;
  }
  ofstream Outfile(OutFileName);

  double IRlength=10;
  double dN0,x0,px0,y0,py0,z0,pz0,E0;
  t1->SetBranchAddress("dN",&dN0);
  t1->SetBranchAddress("x",&x0);
  t1->SetBranchAddress("px",&px0);
  t1->SetBranchAddress("y",&y0);
  t1->SetBranchAddress("py",&py0);
  t1->SetBranchAddress("z",&z0);
  t1->SetBranchAddress("pz",&pz0);
  t1->SetBranchAddress("E",&E0);
  int ntotal0=t1->GetEntries();
  int n0=0;
  int ievt=9999; // particle index in a event
  for(int i0=0;i0<ntotal0;i0++){
    t1->GetEntry(i0);
    if(fabs(z0)>IRlength) continue;
    dN0=dN0*scale;
    if(dN0>1){
      for(double m=0.5;m<dN0;m=m+1){
        if(ievt<Nevt) ievt++;
	else{ 
	  Outfile<<Nevt<<endl;
	  ievt=1;
          n0++;
	}
        Outfile<<ID_p<<"  "<<px0/1000<<"  "<<py0/1000<<"  "<<pz0/1000<<"  "<<E0/1000<<"  "<<x0<<"  "<<y0<<"  "<<z0<<"  "<<0<<endl;
      } 
    } 
    else{
      double rand0 = m_rand->Rndm();
      if(rand0<dN0){
        if(ievt<Nevt) ievt++;
        else{
          Outfile<<Nevt<<endl;
          ievt=1;
          n0++;
        }
        Outfile<<ID_p<<"  "<<px0/1000<<"  "<<py0/1000<<"  "<<pz0/1000<<"  "<<E0/1000<<"  "<<x0<<"  "<<y0<<"  "<<z0<<"  "<<0<<endl;
      }
    }
  }
  while(ievt<Nevt){
    Outfile<<12<<"  "<<0<<"  "<<0<<"  "<<1.0<<"  "<<1.0<<"  "<<0<<"  "<<0<<"  "<<0<<"  "<<0<<endl;
    ievt++;
  }
  cout<<"write type:"<<ID<<"; evt number: "<<n0<<endl;
}

void MakeBesInput(){
  TFile *InfileElec = new TFile("G4InputElec.root");
//  TFile *InfilePosi = new TFile("G4InputPosi.root");
  TTree *tElec_tous = (TTree *)InfileElec->Get("tous");
  TTree *tElec_coul = (TTree *)InfileElec->Get("coul");
  TTree *tElec_brem = (TTree *)InfileElec->Get("brem");
//  TTree *tPosi_tous = (TTree *)InfilePosi->Get("tous");
//  TTree *tPosi_coul = (TTree *)InfilePosi->Get("coul");
//  TTree *tPosi_brem = (TTree *)InfilePosi->Get("brem");

  GeneBes(tElec_tous,1);
//  GeneBes(tPosi_tous,2);
  GeneBes(tElec_coul,3);
//  GeneBes(tPosi_coul,4);
  GeneBes(tElec_brem,5);
//  GeneBes(tPosi_brem,6);
}
