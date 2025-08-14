#include"/home/shihc/workarea/headfile/bes3plotstyle.c"
#include"ApertSet.C"
#include <TCanvas.h>
#include "TTree.h"
#include "TLegend.h"
#include <iostream>
#include <fstream>
using namespace std;
using namespace TMath;

void DrawIRZ(){
  //SetPrelimStyle();
  //SetStyle();
  gStyle->SetOptStat(0);
  gStyle->SetFrameLineWidth(3);

  TString filename = "rootfiles/all.root";
  TFile *fileTracking = new TFile(filename);
  TTree *Toustree = (TTree*) fileTracking->Get("Tous");
  TTree *Coultree = (TTree*) fileTracking->Get("Coul");
  TTree *Bremtree = (TTree*) fileTracking->Get("Brem");

  double xmin=-10,xmax=10,ymax=1.5e7;
  int nbin=100;
  TH1F *fIRZ_tous = new TH1F("fIRZ_tous","",nbin,xmin,xmax);
  TH1F *fIRZ_coul = new TH1F("fIRZ_coul","",nbin,xmin,xmax);
  TH1F *fIRZ_brem = new TH1F("fIRZ_brem","",nbin,xmin,xmax); 
  fIRZ_tous->SetFillColor(kRed);
  fIRZ_coul->SetFillColor(kGreen);
  fIRZ_brem->SetFillColor(kMagenta);
  fIRZ_tous->SetXTitle("Z position /m");
  fIRZ_tous->SetYTitle("Lost rate / Hz"); 
  fIRZ_tous->GetYaxis()->SetRangeUser(0,ymax);

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
    fIRZ_tous->Fill(Z_tous,dN_tous);
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
    fIRZ_coul->Fill(Z_coul,dN_coul);
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
    fIRZ_brem->Fill(Z_brem,dN_brem);
  }
   
  TCanvas *c1 = new TCanvas("c1");
  THStack *hs_all = new THStack("hs_all","");
  hs_all->Add(fIRZ_tous);
  hs_all->Add(fIRZ_coul);
  hs_all->Add(fIRZ_brem);

  TLegend *leg1 =new TLegend(0.2,0.65,0.55,0.85);
  leg1->SetFillColor(0);
  leg1->SetBorderSize(0);
  leg1->SetTextSize(0.03);
  leg1->AddEntry(fIRZ_tous,"Touschek","f");
  leg1->AddEntry(fIRZ_coul,"Coulomb","f");
  leg1->AddEntry(fIRZ_brem,"Bremsstrahlung","f");

  fIRZ_tous->Draw("HIST"); 
  hs_all->Draw("sameHIST");
  leg1->Draw();

  //draw apert
  double Vs[1001],Vrx[1001],Vry[1001],Vdx[1001];
  double rx,ry,dx;
  int i=0;
  double yApertmax=0.1;
  double scale = ymax/yApertmax;
  for(double s1=xmin;s1<=xmax;s1=s1+(xmax-xmin)/500){
    ApertSet(s1,rx,ry,dx);
    Vs[i]=s1;
    Vrx[i]=rx*scale;
    Vry[i]=ry*scale;
    Vdx[i]=dx*scale;
    i++;
  }
  TGraph* g1 = new TGraph(i,Vs,Vrx);
  TGraph* g2 = new TGraph(i,Vs,Vry);
  TGraph* g3 = new TGraph(i,Vs,Vdx);
  g1->GetYaxis()->SetRangeUser(0,yApertmax);
  g1->SetLineColor(kBlue);
  g2->SetLineColor(kCyan);
  g3->SetLineColor(kYellow);
  int linesize=2;
  g1->SetLineWidth(linesize);
  g2->SetLineWidth(linesize);
  g3->SetLineWidth(linesize);
  g1->Draw("same");
  g2->Draw("same");
  g3->Draw("same");
  TGaxis *axis = new TGaxis(xmax,0,xmax,ymax,0,yApertmax,510,"+L");
  axis->SetTitle("Aperture Size/m");
  axis->Draw();
  TLegend *leg2 =new TLegend(0.6,0.65,0.9,0.85);
  leg2->SetFillColor(0);
  leg2->SetBorderSize(0);
  leg2->SetTextSize(0.04);
  leg2->AddEntry(g1,"r_{x}","l");
  leg2->AddEntry(g2,"r_{y}","l");
  leg2->AddEntry(g3,"d_{x}","l");
  leg2->Draw();

  c1->Print("Drawings/Zloss.eps");
}
