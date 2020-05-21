#include "clean.h"
#include "TSystem.h"

const bool TP_on = 0;

void clean(int run){
  std::string iname=ANADIR;
  iname=iname+std::to_string(run)+"/TP_event.root";

  std::ifstream inStream(iname, std::ios::binary);
  if (!inStream) {
    std::cerr << "File " << iname << " not found" << std::endl;
    return;
  }
  TString recreate = Form("$exe_ter -C %i",run);
  auto file = new TFile(iname.c_str());
  TTree *tree = (TTree*)file->Get("t1");
  TCanvas *c1 = new TCanvas("c1","c1",800,600);
  const int max_subrun = 2000;
  const int max_event = 1000;
  TH1I *h_subrun = new TH1I("h_subrun","h_subrun",max_subrun,0,max_subrun);
  TH1I *h_event = new TH1I("h_event","h_event",100,0,max_event);
  TF1 *g_event = new TF1("g_event","gaus",0,max_event);
  //TString bash_cut="rm -f " + ANADIR + Form("%i/",run);
  TString bash_cut="mv -f " + ANADIR + Form("%i/",run); 
  TString mkdir="mkdir -p " + ANADIR + Form("%i/",run) + "badSubRUN";
  TString mkdir1=mkdir+"/lowevent";
  TString mkdir2=mkdir+"/nofireFEB";
  TString mkdir3=mkdir+"/tool1ts";
  cout<<mkdir<<endl;
  cout<<mkdir1<<endl;
  cout<<mkdir2<<endl;
  cout<<mkdir3<<endl;
  gSystem->Exec(mkdir);
  gSystem->Exec(mkdir1);
  gSystem->Exec(mkdir2);
  gSystem->Exec(mkdir3);
  //Remove subRUN with at least 1 FEB w/o TP
  if(TP_on){
    auto file = new TFile(iname.c_str());
    TTree *tree = (TTree*)file->Get("t1");
    tree->Draw("subRUN>>h_subrun","FEB<44 && TP_eff<0.1","");
    for(int i=0;i<max_subrun;i++){
      if(h_subrun->GetBinContent(i+1)) {
	TString bash_cut_i=bash_cut + Form("Sub_RUN_event_%i.root",i);
	bash_cut_i += " " + ANADIR + Form("%i/",run) + "/badSubRUN/.";
	//cout<<bash_cut_i<<endl;
	gSystem->Exec(bash_cut_i);
	TString bash_cut_ii=bash_cut + Form("Sub_RUN_TP_event_%i.root",i);
	bash_cut_ii += " " + ANADIR + Form("%i/",run) + "/badSubRUN/.";
	gSystem->Exec(bash_cut_ii);
      }
    }
    //gSystem->Exec(recreate);
  }
  //Remove subRUN with low number of trigger in at least on FEB                                                                                                                                                           
  if(1){
    if(run<371) tree->Draw("subRUN>>h_subrun","FEB<44 && nhit==0","");
    else if(run<400) tree->Draw("subRUN>>h_subrun","FEB<44 && nhit==0 && FEB*2+chip-1!=68 && FEB*2+chip-1!=69","");
    else tree->Draw("subRUN>>h_subrun","FEB<44 && nhit==0 && FEB*2+chip-1!=68 && FEB*2+chip-1!=6\
9 && FEB*2+chip-1!=72","");
    for(int i=0;i<max_subrun;i++){
      //cout<<i<<" "<<h_subrun->GetBinContent(i+1)<<endl;                                                                                                                                                                 
      if(h_subrun->GetBinContent(i+1)){
        TString bash_cut_i=bash_cut + Form("Sub_RUN_event_%i.root",i);
        bash_cut_i += " " + ANADIR + Form("%i/",run) + "/badSubRUN/nofireFEB/.";
        //cout<<bash_cut_i<<endl;                                                                                                                                                                                         
        gSystem->Exec(bash_cut_i);
        TString bash_cut_ii=bash_cut + Form("Sub_RUN_TP_event_%i.root",i);
        bash_cut_ii += " " + ANADIR + Form("%i/",run) + "/badSubRUN/nofireFEB/.";
        gSystem->Exec(bash_cut_ii);
      }
    }
    //gSystem->Exec(recreate);
  }
  //Remove subRUN with a FEB with too much l1ts_min_tcoarse outside the good region                                                                                                                                       
  if(1){
    tree->Draw("subRUN>>h_subrun","FEB<44 && ratiol1ts>0.01 && nhit>0","");
    for(int i=0;i<max_subrun;i++){
      //cout<<i<<" "<<h_subrun->GetBinContent(i+1)<<endl;                                                                                                                                                                 
      if(h_subrun->GetBinContent(i+1)){
        TString bash_cut_i=bash_cut + Form("Sub_RUN_event_%i.root",i);
        bash_cut_i += " " + ANADIR + Form("%i/",run) + "/badSubRUN/tool1ts/.";
        gSystem->Exec(bash_cut_i);
        TString bash_cut_ii=bash_cut + Form("Sub_RUN_TP_event_%i.root",i);
        bash_cut_ii += " " + ANADIR + Form("%i/",run) + "/badSubRUN/tool1ts/.";
        gSystem->Exec(bash_cut_ii);
      }
    }
    //gSystem->Exec(recreate);
  }
  //Remove subRUN with low number of trigger
  if(1){
    int nsigma = 5;
    cout<<"ciao"<<endl;
    tree->Draw("event>>h_event","FEB<44 && nhit>0 && ratiol1ts<0.01","");
    g_event->SetParameters(h_event->GetMaximum(),h_event->GetBinCenter(h_event->GetMaximumBin()),50);
    g_event->SetParLimits(0,h_event->GetMaximum()*0.8,h_event->GetMaximum()*1.2);
    g_event->SetParLimits(1,h_event->GetBinCenter(h_event->GetMaximumBin())-10, h_event->GetBinCenter(h_event->GetMaximumBin())+10);
    h_event->Fit(g_event,"W");
    c1->SaveAs("~/ciao.pdf");
    cout<<g_event->GetParameter(1)<<" "<<g_event->GetParameter(2)<<" "<<3*g_event->GetParameter(2)<<" "<<g_event->GetParameter(1)-nsigma*g_event->GetParameter(2)<<endl;
    TString cut = Form("FEB<44 && event<%f && nhit>0",g_event->GetParameter(1)-5*g_event->GetParameter(2));
    cout<<cut<<endl;
    cout<<tree->GetEntries()<<endl;
    cout<<tree->GetEntries(cut)<<endl;;
    tree->Draw("subRUN>>h_subrun",cut,"");
    for(int i=0;i<max_subrun;i++){
      //cout<<i<<" "<<h_subrun->GetBinContent(i+1)<<endl;
      if(h_subrun->GetBinContent(i+1)) {
	TString bash_cut_i=bash_cut + Form("Sub_RUN_event_%i.root",i);
	bash_cut_i += " " + ANADIR + Form("%i/",run) + "/badSubRUN/lowevent/.";
	//cout<<bash_cut_i<<endl;
	gSystem->Exec(bash_cut_i);
	TString bash_cut_ii=bash_cut + Form("Sub_RUN_TP_event_%i.root",i);
	bash_cut_ii += " " + ANADIR + Form("%i/",run) + "/badSubRUN/lowevent/.";
	gSystem->Exec(bash_cut_ii);      
      }
    }
    //gSystem->Exec(recreate);
  }
  return;
}
