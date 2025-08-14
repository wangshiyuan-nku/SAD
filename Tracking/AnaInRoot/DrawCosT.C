#include"/home/shihc/workarea/headfile/bes3plotstyle.c"
#include <TCanvas.h>
#include "TTree.h"
#include "TLegend.h"
#include <iostream>
#include <fstream>
using namespace std;

void DrawCosT(){
  SetPrelimStyle();
  SetStyle();
  gStyle->SetOptStat(0);
  gStyle->SetFrameLineWidth(3);

  TString filename = "rootfiles/all.root";
  TFile *fileTracking = new TFile(filename);
  TTree *Toustree = (TTree*) fileTracking->Get("Tous");
  TTree *Coultree = (TTree*) fileTracking->Get("Coul");
  TTree *Bremtree = (TTree*) fileTracking->Get("Brem");

  double xmin=0.9,xmax=1,ymax=5e7;
  int nbin=100;
  TH1F *fCosT_tous = new TH1F("fCosT_tous","",nbin,xmin,xmax);
  TH1F *fCosT_coul = new TH1F("fCosT_coul","",nbin,xmin,xmax);
  TH1F *fCosT_brem = new TH1F("fCosT_brem","",nbin,xmin,xmax); 
  fCosT_tous->SetMarkerColor(kRed);
  fCosT_coul->SetMarkerColor(kGreen);
  fCosT_brem->SetMarkerColor(kBlue);
  fCosT_tous->SetMarkerStyle(5);
  fCosT_coul->SetMarkerStyle(26);
  fCosT_brem->SetMarkerStyle(4);
  fCosT_tous->SetMarkerSize(1.5);
  fCosT_coul->SetMarkerSize(1.5);
  fCosT_brem->SetMarkerSize(1.5);
  fCosT_tous->SetXTitle("Cos(theta)");
  fCosT_tous->SetYTitle("Lost rate / Hz"); 
  fCosT_tous->GetYaxis()->SetRangeUser(0,ymax);

  //---fill touschek----
  Int_t ntous=Toustree->GetEntries();
  Double_t dN_tous,dX_tous,Z0_tous,x_tous,px_tous,y_tous,py_tous,Z_tous,dE_tous,nturn_tous;
  Toustree->SetBranchAddress("dN",&dN_tous);
  Toustree->SetBranchAddress("dX",&dX_tous);
  Toustree->SetBranchAddress("Z0",&Z0_tous);
  Toustree->SetBranchAddress("x",&x_tous);
  Toustree->SetBranchAddress("px",&px_tous);
  Toustree->SetBranchAddress("y",&y_tous);
  Toustree->SetBranchAddress("py",&py_tous);
  Toustree->SetBranchAddress("Z",&Z_tous);
  Toustree->SetBranchAddress("dE",&dE_tous);
  Toustree->SetBranchAddress("nturn",&nturn_tous);
  for(Int_t itous=0;itous<ntous;itous++){
    Toustree->GetEntry(itous);   
    double cost_tous=sqrt(1-(px_tous*px_tous+py_tous*py_tous)/((1+dE_tous)*(1+dE_tous)));
    fCosT_tous->Fill(cost_tous,dN_tous);
  }

  //----fill coulomb-----
  Int_t ncoul=Coultree->GetEntries();
  Double_t dN_coul,dX_coul,Z0_coul,x_coul,px_coul,y_coul,py_coul,Z_coul,dE_coul,nturn_coul;
  Coultree->SetBranchAddress("dN",&dN_coul);
  Coultree->SetBranchAddress("dX",&dX_coul);
  Coultree->SetBranchAddress("Z0",&Z0_coul);
  Coultree->SetBranchAddress("x",&x_coul);
  Coultree->SetBranchAddress("px",&px_coul);
  Coultree->SetBranchAddress("y",&y_coul);
  Coultree->SetBranchAddress("py",&py_coul);
  Coultree->SetBranchAddress("Z",&Z_coul);
  Coultree->SetBranchAddress("dE",&dE_coul);
  Coultree->SetBranchAddress("nturn",&nturn_coul);
  for(Int_t icoul=0;icoul<ncoul;icoul++){
    Coultree->GetEntry(icoul);
    double cost_coul=sqrt(1-(px_coul*px_coul+py_coul*py_coul)/((1+dE_coul)*(1+dE_coul)));
    fCosT_coul->Fill(cost_coul,dN_coul);
  }
 
  //---fill bremsstrahlung----
  Int_t nbrem=Bremtree->GetEntries();
  Double_t dN_brem,dX_brem,Z0_brem,x_brem,px_brem,y_brem,py_brem,Z_brem,dE_brem,nturn_brem;
  Bremtree->SetBranchAddress("dN",&dN_brem);
  Bremtree->SetBranchAddress("dX",&dX_brem);
  Bremtree->SetBranchAddress("Z0",&Z0_brem);
  Bremtree->SetBranchAddress("x",&x_brem);
  Bremtree->SetBranchAddress("px",&px_brem);
  Bremtree->SetBranchAddress("y",&y_brem);
  Bremtree->SetBranchAddress("py",&py_brem);
  Bremtree->SetBranchAddress("Z",&Z_brem);
  Bremtree->SetBranchAddress("dE",&dE_brem);
  Bremtree->SetBranchAddress("nturn",&nturn_brem);

  for(Int_t ibrem=0;ibrem<nbrem;ibrem++){
    Bremtree->GetEntry(ibrem);
    double cost_brem=sqrt(1-(px_brem*px_brem+py_brem*py_brem)/((1+dE_brem)*(1+dE_brem)));
    fCosT_brem->Fill(cost_brem,dN_brem);
  }

  TCanvas *c1 = new TCanvas("c1");
  double step=(xmax-xmin)/nbin;
  for(double m=xmin+step/2;m<xmax;m=m+step){
    fCosT_tous->Fill(m,1);
    fCosT_coul->Fill(m,1);
    fCosT_brem->Fill(m,1);
  }
  fCosT_tous->GetYaxis()->SetRangeUser(1,ymax);
  gPad->SetLogy();  

  TLegend *leg =new TLegend(0.2,0.65,0.55,0.9);
  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.04);
  leg->AddEntry(fCosT_tous,"Touschek","p");
  leg->AddEntry(fCosT_coul,"Coulomb","p");
  leg->AddEntry(fCosT_brem,"Bremsstrahlung","p");
  
  fCosT_tous->Draw("E");
  fCosT_coul->Draw("Esame");
  fCosT_brem->Draw("Esame");
  leg->Draw();

  c1->Print("Drawings/CosT.eps");
}
