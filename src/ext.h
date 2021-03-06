#ifndef ext_h
#define ext_h
#include "TFile.h"
#include "TTree.h"
#include <iostream>
#include <fstream>
#include "stdio.h"
#include <string>
#include "TCanvas.h"
#include "TH2F.h"
#include "TCut.h"
#include "TChain.h"
#include "TF1.h"
#include <map>
#include "common.h"
#include "TSystem.h"
#include <thread>
#include <chrono>
#include "TStyle.h"
#include "TMinuit.h"
#include "TString.h"

const string DataDir=DATA+"/raw_root/";

using namespace std;
void ext(int);
void ext(vector<int>);
void ext(vector<int>,int,int,int);
void ext_i(vector<int>,int,int);
bool init(int,int);
bool init(vector<int>);
TString Get_Cut(int,int);
TString Get_Cut(int,int,int);
TString Get_Cut(int,int,int,int);
TString Get_Command(int,TString);
TString Get_Command(int,TString,int);
void extract_noise_and_thr(int,int);
int count_line(int);
#endif
