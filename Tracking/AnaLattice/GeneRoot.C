#include <iostream>
#include <fstream>
#include "TTree.h"
using namespace std;

void GeneRoot(){
  TFile *newfile = new TFile("Twiss.root","recreate");
  TTree *newtree = new TTree("t1","recreate");
  double s,betaX, betaY, DisIP;
  TBranch* Brs     = newtree->Branch("s",&s,"s/D");
  TBranch* BrBetaX = newtree->Branch("betaX",&betaX,"betaX/D");
  TBranch* BrBetaY = newtree->Branch("betaY",&betaY,"betaY/D");  
  TBranch* BrDisIP = newtree->Branch("DisIP",&DisIP,"DisIP/D");

  TString infilename="Twiss.txt";
  ifstream infile(infilename);
 
  int Nid=0;
  while(infile&&!infile.eof()){
    infile>>s>>betaX>>betaY>>DisIP;
    newtree->GetEntry(Nid);
    newtree->Fill();
    Nid++;
  }
  newtree->Write();
  delete newtree;
}
