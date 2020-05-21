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
#include "TPaveText.h"
#include "TF1.h"
#include <map>
#include "common.h"

const string DataDir=DATA+"/raw_root/";

using namespace std;
void daq(int);
bool Check_File(int);
void Print_Variable(TString var, int);
#endif
