#include"/home/shihc/workarea/headfile/bes3plotstyle.c"
#include <TCanvas.h>
#include "TTree.h"
#include "TLegend.h"
#include <iostream>
#include <fstream>
#include "../AnaLattice/DrawTwiss.C"
#include "ApertSet.C"
using namespace std;

void DrawIRZ_Apt_Beta(){
   gROOT->Reset();
   gStyle->SetOptStat(0);
   gStyle->SetPadBorderMode(0);
   gStyle->SetFrameBorderMode(0);
   gStyle->SetOptTitle(0);

  TCanvas* c1 = new TCanvas();
  c1->Divide(1,3);

  void SetgPad(int i);
  for(int i=0;i<3;i++){
    c1->cd(i+1); SetgPad(i);
  }

  double xmin=-10, xmax=10;
  void DrawIRbetaXY(TCanvas*, int,double,double);
  DrawIRbetaXY(c1,1,xmin,xmax);
  void DrawIRApt(TCanvas*, int,double,double);
  DrawIRApt(c1,2,xmin,xmax);
  void DrawIRZ_logy(TCanvas*, int, double,double);
  DrawIRZ_logy(c1,3,xmin,xmax);
}

void SetgPad(int i){
  Float_t small = 1e-5;
  int setB[3]={1,1,0};
  int setT[3]={0,1,1};
  int setR[3]={0,0,0};
  int setL[3]={0,0,0};
  if(i>=3){
    cout<<"ipad out of range!"<<endl;
    return;
  }
  if(setB[i]==1){
     gPad->SetBottomMargin(small);
  }
  if(setT[i]==1){
    gPad->SetTopMargin(small);
  }
  if(setR[i]==1){
    gPad->SetRightMargin(small);
  }
  if(setL[i]==1){
    gPad->SetLeftMargin(small);
  }
//  gPad->SetTickx();
//  gPad->SetTicky();
}

void DrawIRbetaXY(TCanvas* c1, int iPad, double xmin,double xmax){
  TFile *f1 = new TFile("../AnaLattice/Twiss.root","read");
  TTree* t1 = (TTree*)f1->Get("t1");
  double s,betaX,betaY,DisIP;
  t1->SetBranchAddress("s",&s);
  t1->SetBranchAddress("betaX",&betaX);
  t1->SetBranchAddress("betaY",&betaY);
  t1->SetBranchAddress("DisIP",&DisIP);

  double ntotal=t1->GetEntries();
  double Vx[nArr],Vy1[nArr],Vy2[nArr];
  double ymax=0;
  void OrderVecs(double* Vx,double* Vy,double* Vz,int ntotal);
  int nFill=0;
  for(int i=0;i<ntotal;i++){
    t1->GetEntry(i);
    if( !(DisIP>xmin&&DisIP<xmax) ) continue;
    Vx[nFill]=DisIP;
    Vy1[nFill]=betaX;
    Vy2[nFill]=betaY;
    double y1=betaX>betaY?betaX:betaY;
    ymax=ymax>y1?ymax:y1;
    nFill++;
  }
  ymax=ymax*1.1;
  OrderVecs(Vx,Vy1,Vy2,nFill);

  TGraph* gr1 = new TGraph(nFill,Vx,Vy1);
  gr1->SetLineWidth(2);
  gr1->SetLineColor(kRed);
  TGraph* gr2 = new TGraph(nFill,Vx,Vy2);
  gr2->SetLineWidth(2);
  gr2->SetLineColor(kBlue);
  TMultiGraph *mg = new TMultiGraph();
  mg->Add(gr1);
  mg->Add(gr2);
  mg->GetXaxis()->SetRangeUser(xmin,xmax);
  mg->GetYaxis()->SetRangeUser(0,ymax);
  mg->GetXaxis()->SetNdivisions(10,0,0);
  mg->GetXaxis()->SetTickLength(0.0);
  mg->GetYaxis()->SetLabelSize(0.1);
  mg->GetXaxis()->SetLabelSize(0.0);
  mg->GetYaxis()->SetTitleSize(0.1);
  mg->GetYaxis()->SetTitleOffset(0.3);
  mg->GetYaxis()->SetTitle("#beta function");
  c1->cd(iPad);
  mg->Draw("AC");

  TLegend *leg1 =new TLegend(0.65,0.6,0.9,0.85);
  leg1->SetFillColor(0);
  leg1->SetBorderSize(0);
  leg1->SetTextSize(0.1);
  leg1->AddEntry(gr1,"#beta_{x}","l");
  leg1->AddEntry(gr2,"#beta_{y}","l");
  leg1->Draw();
}

void DrawIRApt(TCanvas* c1, int iPad,double xmin,double xmax){
  double Vs[1001],Vrx[1001],Vry[1001],Vdx[1001];
  double rx,ry,dx;
  int i=0;
  double yApertmax=0.11;
  for(double s1=xmin;s1<=xmax;s1=s1+(xmax-xmin)/500){
    ApertSet(s1,rx,ry,dx);
    Vs[i]=s1;
    Vrx[i]=rx;
    Vry[i]=ry;
    Vdx[i]=dx;
    i++;
  }
  TGraph* g1 = new TGraph(i,Vs,Vrx);
  TGraph* g2 = new TGraph(i,Vs,Vry);
  TGraph* g3 = new TGraph(i,Vs,Vdx);
  g1->GetYaxis()->SetRangeUser(0,yApertmax);
  g1->GetXaxis()->SetRangeUser(xmin,xmax);
  g1->SetLineColor(kBlue);
  g2->SetLineColor(kCyan);
  g3->SetLineColor(kYellow);
  int linesize=2;
  g1->SetLineWidth(linesize);
  g2->SetLineWidth(linesize);
  g3->SetLineWidth(linesize);

  g1->GetXaxis()->SetNdivisions(10,0,0);
  g1->GetYaxis()->SetNdivisions(5,2,0);
  g1->GetYaxis()->SetLabelSize(0.1);
  g1->GetXaxis()->SetLabelSize(0.0);
  g1->GetXaxis()->SetTickLength(0.0);
  g1->GetYaxis()->SetTitle("Apert/m");
  g1->GetYaxis()->SetTitleSize(0.1);
  g1->GetYaxis()->SetTitleOffset(0.3);

  c1->cd(iPad);
  g1->Draw();
  g2->Draw("same");
  g3->Draw("same");

  TLegend *leg2 =new TLegend(0.65,0.6,0.9,0.85);
  leg2->SetFillColor(0);
  leg2->SetBorderSize(0);
  leg2->SetTextSize(0.1);
  leg2->AddEntry(g1,"r_{x}","l");
  leg2->AddEntry(g2,"r_{y}","l");
  leg2->AddEntry(g3,"d_{x}","l");
  leg2->Draw();
    
}

void DrawIRZ_logy(TCanvas* c1, int iPad,double xmin,double xmax){
  TString filename = "rootfiles/all.root";
  TFile *fileTracking = new TFile(filename);
  TTree *Toustree = (TTree*) fileTracking->Get("Tous");
  TTree *Coultree = (TTree*) fileTracking->Get("Coul");
  TTree *Bremtree = (TTree*) fileTracking->Get("Brem");

  double ymax=1e10-5e9;
  int nbin=100;
  TH1F *fIRZ_tous = new TH1F("fIRZ_tous","",nbin,xmin,xmax);
  TH1F *fIRZ_coul = new TH1F("fIRZ_coul","",nbin,xmin,xmax);
  TH1F *fIRZ_brem = new TH1F("fIRZ_brem","",nbin,xmin,xmax);

  fIRZ_tous->SetFillColor(kRed);
  fIRZ_coul->SetFillColor(kGreen);
  fIRZ_brem->SetFillColor(kBlue);
  fIRZ_tous->SetFillStyle(3744);
  fIRZ_coul->SetFillStyle(3335);
  fIRZ_brem->SetFillStyle(3353);

  fIRZ_tous->SetXTitle("Z/m");
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

  for(Int_t ibrem=0;ibrem<nbrem;ibrem++){
    Bremtree->GetEntry(ibrem);
    fIRZ_brem->Fill(Z_brem,dN_brem);
  }

  double step=(xmax-xmin)/nbin;
  for(double m=xmin+step/2;m<xmax;m=m+step){
    fIRZ_tous->Fill(m,1);
    fIRZ_coul->Fill(m,1);
    fIRZ_brem->Fill(m,1);
  }
  fIRZ_tous->GetYaxis()->SetRangeUser(1,ymax);

//  fIRZ_tous->GetXaxis()->SetNdivisions(4,5,0);
  fIRZ_tous->GetYaxis()->SetNdivisions(3,2,0);
  fIRZ_tous->GetYaxis()->SetLabelSize(0.1); 
  fIRZ_tous->GetXaxis()->SetLabelSize(0.1);
  fIRZ_tous->GetXaxis()->SetTitleSize(0.1);
  fIRZ_tous->GetYaxis()->SetTitleSize(0.1);
  fIRZ_tous->GetXaxis()->SetTitleOffset(0.35);
  fIRZ_tous->GetYaxis()->SetTitleOffset(0.3);

  c1->cd(iPad);
  gPad->SetLogy();
  TLegend *leg =new TLegend(0.65,0.6,0.9,0.85);
  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.1);
  leg->AddEntry(fIRZ_tous,"Touschek","f");
  leg->AddEntry(fIRZ_coul,"Coulomb","f");
  leg->AddEntry(fIRZ_brem,"Bremsstrahlung","f");

  fIRZ_tous->Draw("H");
  fIRZ_coul->Draw("Hsame");
  fIRZ_brem->Draw("Hsame");
  leg->Draw(); 
}
