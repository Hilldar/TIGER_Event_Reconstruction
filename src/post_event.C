#include "post_event.h"

const int n_chip = 100;
int trigg_channel = 20;
//if(run>=118) trigg_channel=62;

double s_TriggerTime;
unsigned int s_size;
vector<float>  * v_evttime = 0, * v_time_fine = 0;
vector<float> * v_chargeQDC = 0, * v_ttrigg = 0;
vector<int>    * v_channel = 0, * v_gemchannel = 0, * v_gemchannel_phi = 0, * v_gemchannel_v = 0, * v_header = 0, * v_tiger = 0, * v_FPGAid = 0, * v_chip = 0;


void post_event(int run, int subrun){
  std::string iname=ANADIR;
  iname=iname+std::to_string(run)+"/Sub_RUN_event_"  +std::to_string(subrun)+".root";
  std::string oname=ANADIR;
  oname=oname+std::to_string(run)+"/Sub_RUN_post_event_"+std::to_string(subrun)+".root";
  if(run>=118) trigg_channel = 62;

  auto file = new TFile(iname.c_str());
  auto fChain = (TTree*)file->Get("tree");

  fChain->SetBranchAddress("nhits"       ,&s_size          );
  fChain->SetBranchAddress("t_min_ttrigg",&v_evttime       );
  fChain->SetBranchAddress("charge_SH"   ,&v_chargeQDC     );
  fChain->SetBranchAddress("channel"     ,&v_channel       );
  fChain->SetBranchAddress("strip_x"     ,&v_gemchannel_phi);
  fChain->SetBranchAddress("strip_v"     ,&v_gemchannel_v  );
  fChain->SetBranchAddress("FEB_label"   ,&v_header        ); // <-- FEB ID con FEB=0 sulla strpi=0   
  fChain->SetBranchAddress("FEB_SW"      ,&v_tiger         ); // <-- TIGER ID from 0 to 7  
  fChain->SetBranchAddress("gemroc"      ,&v_FPGAid        );
  fChain->SetBranchAddress("tfine"       ,&v_time_fine     );
  fChain->SetBranchAddress("chip"        ,&v_chip          );
  fChain->SetBranchAddress("tfine"       ,&v_time_fine     );
  fChain->SetBranchAddress("tcoarse"     ,&v_ttrigg        );

  auto ofile = new TFile((TString)oname,"RECREATE");
  TTree *otree = new TTree("tree","tree");

  otree->Branch("nhits"             , &s_size        , "nhits/i"          ); // the number of hits in each event
  otree->Branch("time"              , "double"       , &s_TriggerTime     );
  otree->Branch("channel"           , "vector<int>"  , &v_channel         ); // channel ID for each hit
  otree->Branch("gemroc"            , "vector<int>"  , &v_FPGAid          ); // GEMROC ID for each hit
  otree->Branch("strip_v"           , "vector<int>"  , &v_gemchannel_v    ); // strip V number of each hit
  otree->Branch("strip_x"           , "vector<int>"  , &v_gemchannel_phi  ); // strip X number of each hit
  otree->Branch("chip"              , "vector<int>"  , &v_chip            ); // FEB chip ID for each hit
  otree->Branch("FEB_label"         , "vector<int>"  , &v_header          ); // FEB label number for each hit
  otree->Branch("charge_SH"         , "vector<float>", &v_chargeQDC       ); // charge in S&H mode, with QDC calibration, for each hit
  otree->Branch("tfine"             , "vector<float>", &v_time_fine       ); // tcoarse of each trigger channel 
  otree->Branch("t_min_ttrigg"      , "vector<float>", &v_evttime         ); // tcoarse of each hit - tcoarse from trigger channel in each event
  otree->Branch("FEB_SW"            , "vector<int>"  , &v_tiger           );

  int n_event = fChain->GetEntries();
  cout<<n_event<<endl;
  for(int i=0;i<n_event;i++){
    fChain->GetEntry(i);
    
    //Fill the TP
    float TP[n_chip] = {-1};
    for(int j=0;j<s_size;j++){
      if(v_channel->at(j) == trigg_channel && v_header->at(j) > -1) {
	TP[v_header->at(j)] = v_ttrigg->at(j);
	//cout<<"TP{"<<v_header->at(j)<<"] "<<TP[v_header->at(j)] <<endl;
      }
    }

    //Measure the tcoarse - TP of the same chip
    for(int j=0;j<s_size;j++){
      if(v_channel->at(j) != trigg_channel && v_header->at(j) > -1) {
	bool TP_is_good = abs( v_evttime->at(j) - ( v_ttrigg->at(j) - TP[v_header->at(j)] ) )<30;
	bool TP_is_the_same = abs( v_evttime->at(j) - ( v_ttrigg->at(j) - TP[v_header->at(j)] ) );
	if(TP[v_header->at(j)] != -1 && TP_is_good ){
	  //if(TP_is_the_same) cout<<"Prima: "<<v_evttime->at(j)<<endl;
	  v_evttime->at(j) = v_ttrigg->at(j) - TP[v_header->at(j)];
	  //if(TP_is_the_same) cout<<"Dopo: "<<v_evttime->at(j)<<endl;
	}
      }
    }

    //Fill the new TTree
    otree->Fill();
    //cout<<"Prima --> "<<fChain->GetEntries("nhits")<<endl;
    //cout<<"Dopo  --> "<<otree->GetEntries("nhits")<<endl;
    
  }
  ofile->Write();
  ofile->Close();
  return;
}
