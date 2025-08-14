#include "TTree.h"
#include <iostream>
#include <fstream>
using namespace std;
const double zIP=59.05419;

void Gene(TString InfileName, TString OutfileName){
  cout<<"remember to set IP z position!!"<<endl;
  TFile *newfile = new TFile(OutfileName,"recreate");
  
  TString treeName="t1";
  TTree *newtree=new TTree(treeName,"new one");

  Int_t nstep, flag[2000];
  Double_t x[2000],y[2000],z[2000];
  TBranch *Br1 = newtree->Branch("nstep",&nstep,"nstep/I");
  TBranch *Br2 = newtree->Branch("x",x,"x[nstep]/D");
  TBranch *Br3 = newtree->Branch("y",y,"y[nstep]/D");
  TBranch *Br4 = newtree->Branch("z",z,"z[nstep]/D");
  TBranch *Br5 = newtree->Branch("flag",flag,"flag[nstep]/I");

  ifstream infile(InfileName);
  int i_new=0;
  while(!infile.eof()){
    infile>>nstep;
    for(int i=0;i<nstep;i++){
      infile>>x[i]>>y[i]>>z[i]>>flag[i];
      z[i]=z[i]-zIP;
    }
    newtree->GetEntry(i_new);
    i_new++;
    newtree->Fill();
  }
  newfile->Write();
  delete newfile;
}

void GeneRoot(){
//  TString InfileName="../outTrack/trackTous.out";
//  TString OutfileName="rootfiles/trackTous.root";
  TString InfileName="../outTrack/trackCoul.out";
  TString OutfileName="rootfiles/trackCoul.root";
//  TString InfileName="../outTrack/trackBrem.out";
//  TString OutfileName="rootfiles/tracBrem.root";

  Gene(InfileName,OutfileName);
}
