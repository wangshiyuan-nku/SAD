#include <iostream>
#include <fstream>
#include "TTree.h"
#include "TGraph.h"
using namespace std;
const int nArr=10000;

void DrawTwiss(){
  TFile *f1 = new TFile("Twiss.root","read");
  TTree* t1 = (TTree*)f1->Get("t1");
  double s,betaX,betaY,DisIP;
  t1->SetBranchAddress("s",&s);
  t1->SetBranchAddress("betaX",&betaX);
  t1->SetBranchAddress("betaY",&betaY);
  t1->SetBranchAddress("DisIP",&DisIP);
  
  double ntotal=t1->GetEntries();
  double Vx[nArr],Vy1[nArr],Vy2[nArr];
  double xmin=-30;
  double xmax=30;
  void OrderVecs(double* Vx,double* Vy,double* Vz,int ntotal);
  int nFill=0;
  for(int i=0;i<ntotal;i++){
    t1->GetEntry(i);
    if( !(DisIP>xmin&&DisIP<xmax) ) continue;
    Vx[nFill]=DisIP;
    Vy1[nFill]=betaX;
    Vy2[nFill]=betaY;
    nFill++;
  }
  OrderVecs(Vx,Vy1,Vy2,nFill);
  //for(int i=0;i<nFill;i++) cout<<"x:"<<Vx[i]<<"; y1:"<<Vy1[i]<<"; y2:"<<Vy2[i]<<endl;

  TGraph* gr1 = new TGraph(nFill,Vx,Vy1);
  gr1->SetLineWidth(2);
  gr1->SetLineColor(kRed);
  TGraph* gr2 = new TGraph(nFill,Vx,Vy2);
  gr2->SetLineWidth(2);
  gr2->SetLineColor(kBlue);
  TMultiGraph *mg = new TMultiGraph();
  mg->Add(gr1);
  mg->Add(gr2);
  mg->Draw("AC");
}

void OrderVecs(double* Vx,double* Vy1,double* Vy2,int ntotal){
  int idmin=-1;
  double Xmin=Vx[0];
  for(int i=0;i<ntotal;i++){
    if(Xmin>Vx[i]){
      Xmin=Vx[i];
      idmin=i;
    }
  }
  if(idmin==-1){
    cout<<"not changed"<<endl;
    return;
  }
  double SVx[nArr],SVy1[nArr],SVy2[nArr];
  int i2=0;
  for(int i=idmin;i<ntotal;i++){
    SVx[i2]=Vx[i];
    SVy1[i2]=Vy1[i];
    SVy2[i2]=Vy2[i];
    i2++;
  }
  for(int i=0;i<idmin;i++){
    SVx[i2]=Vx[i];
    SVy1[i2]=Vy1[i];
    SVy2[i2]=Vy2[i];
    i2++;
  }
  for(int i=0;i<ntotal;i++){
    Vx[i]=SVx[i];
    Vy1[i]=SVy1[i];
    Vy2[i]=SVy2[i];
  }
  return;
}

void OrderVecs(double* Vx,double* Vy1,double* Vy2,int start,int end){ // quick sort
  if(start>=end) return;
  double Xmid=Vx[end];
  int left=start,right=end-1;
  while(left<right){
    while(Vx[left]<Xmid&&left<right) left++;
    while(Vx[left]>=Xmid&&left<right) right--;
    swap(Vx[left],Vx[right]);
    swap(Vy1[left],Vy1[right]);
    swap(Vy2[left],Vy2[right]);
  }
  if(Vx[left]>=Vx[end]){
    swap(Vx[left],Vx[end]);
    swap(Vy1[left],Vy1[end]);
    swap(Vy2[left],Vy2[end]);
  }
  else left++;
  OrderVecs(Vx,Vy1,Vy2,start,left-1);
  OrderVecs(Vx,Vy1,Vy2,left+1,end);
}
