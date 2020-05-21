#include "RooHist.h"
#include "TFile.h"
#include "TH1.h"
#include "TH1F.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TGraphAsymmErrors.h"
#include <TROOT.h>
#include <TH1F.h>
#include <TFile.h>
#include <TCanvas.h>

#include "daq.h"
TFile *file1;
TTree *tree1;

void daq(int run){
  gErrorIgnoreLevel = kWarning;
  TString file_name = DATA + Form("/raw_daq/extracted_noise_thr_%i.root",run);
  if(Check_File(run)){
    file1 = new TFile(file_name);
    tree1 = (TTree*)file1->Get("tree");
    TCanvas *c1 = new TCanvas("c1","c1",800.600);
    TString pdf_name = DATA + Form("/pdf_ter/%i.pdf",run);
    TPaveText *pt = new TPaveText(.05,.1,.95,.8);
    pt->AddText(Form("Begin RUN %i",run));
    pt->Draw();
    c1->Print(pdf_name+"(");
    pt->~TPaveText();
    cout<<"********************"<<endl;
    cout<<"********************"<<endl;
    cout<<"********************"<<endl;
    cout<<endl;
    Print_Variable("noise_rate_Hz",run);
    Print_Variable("noise_meanQ_fC",run);
    Print_Variable("threshold_fC",run);
    cout<<endl;
    cout<<"********************"<<endl;
    cout<<"********************"<<endl;
    cout<<"********************"<<endl;
    TPaveText *pt1 = new TPaveText(.05,.1,.95,.8);
    pt1->AddText(Form("End RUN %i",run));
    pt1->Draw();
    c1->Print(pdf_name+")");
  }
  return;
}


bool Check_File(int run){
  TString name = DATA + Form("/raw_daq/extracted_noise_thr_%i.root",run);
  TFile *file = new TFile(name);
  TTree *tree = (TTree*)file->Get("tree");
  bool b1(0),b2(0),b3(0);
  b1=file->IsOpen();
  b2=(bool)tree;
  if(b2) b3=tree->IsFolder();
  if(b1*b2*b3) {
    cout<<"Init file "<<name<<endl;
    return true;
  }
  else {
    cout<<"ERROR in file "<<name<<b1<<b2<<b3<<endl;
    return false;
  }
}

void Print_Variable(TString var, int run){
  TString pdf_name = DATA + Form("/pdf_ter/%i.pdf",run);
  TString a=var+">>a";
  TString b=var+">>b";
  TString d=var+">>d";
  TString e=var+">>e";
  TCanvas *c = new TCanvas("c","c",800,600);
  c->Divide(2,2);
  c->cd(1);
  tree1->Draw(a,"FEB<16 && strip_x>=0");
  cout<<var<<" L1top "<<((TH1D*)(c->FindObject("a")))->GetMean()<<endl;
  c->cd(2);
  tree1->Draw(b,"FEB<16 && strip_v>=0");
  cout<<var<<" L1bot "<<((TH1D*)(c->FindObject("b")))->GetMean()<<endl;  
  c->cd(3);
  tree1->Draw(d,"FEB>15 && FEB<44 && strip_x>=0");
  cout<<var<<" L2top "<<((TH1D*)(c->FindObject("d")))->GetMean()<<endl;
  c->cd(4);
  tree1->Draw(e,"FEB>15 && FEB<44 && strip_v>=0");
  cout<<var<<" L2bot "<<((TH1D*)(c->FindObject("e")))->GetMean()<<endl;
  cout<<endl;
  bool goodness = tree1->GetEntries("FEB<16 && strip_x>=0")*tree1->GetEntries("FEB<16 && strip_v>=0")*tree1->GetEntries("FEB>15 && FEB<44 && strip_x>=0")*tree1->GetEntries("FEB>15 && FEB<44 && strip_v>0");
  if(goodness>0) c->Print(pdf_name);
  else {
    cout<<"The command "<<var<<" has empty entries"<<endl;
  }
  c->~TCanvas();
  return;
}
