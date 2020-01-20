#ifndef daq_h
#define daq_h
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

const string DataDir="/home/ihep_data/data/raw_root/";

using namespace std;
void daq(int);
void daq(int,int);
void daq_i(int,int);
bool init(int,int);
TString Get_Cut(int,int);
TString Get_Cut(int,int,int);
TString Get_Cut(int,int,int,int);
TString Get_Command(int,TString);
TString Get_Command(int,TString,int);
void noise_check(int,int);
void charge_check(int,int);
void comunication_efficiency(int,int);
void extract_noise_and_thr(int,int);
void print_error(int,int);
void count_error(int,int);
string error_type(int);
#endif
