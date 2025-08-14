#include"/home/shihc/workarea/headfile/bes3plotstyle.c"
#include <TCanvas.h>
#include "TTree.h"
#include "TLegend.h"
#include <iostream>
#include <fstream>
#include "ApertSet.C"
using namespace std;
const double sint=sin(0.011);
const double cost=cos(0.011);
const double zmax=5;
const double xmax=0.12;
const int mode=1; //0: all;  1:lost near IP; 2:not lost
const int nlimit_Draw=100;

void DrawTrack_yz(){
  SetPrelimStyle();
  SetStyle();

//  TString filename = "rootfiles/trackBrem.root";
//  TString filename = "rootfiles/trackTous.root";
//  TString filename = "rootfiles/trackCoul.root";
  
  TFile *fileTracking = new TFile(filename);

  TTree *tree = (TTree*) fileTracking->Get("t1");
  Int_t n1=tree->GetEntries();

  Int_t nstep,flag[2000];
  Double_t x[2000],y[2000],z[2000];
  tree->SetBranchAddress("nstep",&nstep); 
  tree->SetBranchAddress("x",x);
  tree->SetBranchAddress("y",y);
  tree->SetBranchAddress("z",z);
  tree->SetBranchAddress("flag",flag);

  vector <TGraph*> gr;
  gr.clear();
  int nDraw=0;
  for(int i=0;i<n1;i++){
    tree->GetEntry(i);
    if(flag[0]==0) continue;    

    //draw particles lost at IP range
    bool ifLost=false;
    int nstop=0;
    for(int j=0;j<nstep;j++){
      if(flag[j]==0) ifLost=true;
      else nstop++;
    }
     
    if(mode==1) {if(!ifLost) continue;}
    else if(mode==2) {if(ifLost) continue;}

//    cout<<"nstep:"<<nstep<<endl;
    for(int j=0;j<nstep;j++){
//      cout<<"z:"<<z[j]<<"; y:"<<y[j]<<endl;
    }

    TGraph* gr1 = new TGraph(nstop,z,y);
    gr.push_back(gr1);
    nDraw++;
  } 
  cout<<"ntotal:"<<n1<<"; nDraw:"<<nDraw<<endl;
  
  //Aperture
  double Apx1[1000],Apx2[1000],Apy[1000],Apz[1000];
  double Apx;
  double dx=0;
  for(int m=0;m<1000;m++){
    Apz[m]=-1*zmax+2*zmax/1000*m;
    ApertSet(fabs(Apz[m]),Apx,Apy[m],dx);
    Apx1[m]=Apx;
    Apx2[m]=-Apx;
  }
  TGraph *gr_ap = new TGraph(1000,Apz,Apx1);
  gr_ap->SetLineWidth(3);
  gr_ap->SetLineColor(kRed);
  TGraph *gr_ap2 = new TGraph(1000,Apz,Apx2);
  gr_ap2->SetLineWidth(3);
  gr_ap2->SetLineColor(kRed);

  //Draw
  TMultiGraph *mg = new TMultiGraph();
  for(int i=0;i<nDraw;i++){
    if(i>nlimit_Draw) continue;
    mg->Add(gr[i]);
  }
  mg->Add(gr_ap);
  mg->Add(gr_ap2);
  mg->GetXaxis()->SetRangeUser(-1*zmax,zmax);
  mg->GetYaxis()->SetRangeUser(-1*xmax,xmax);
  mg->GetXaxis()->SetTitle("Z/m");
  mg->GetYaxis()->SetTitle("x/m");
  mg->Draw("AL");
}

