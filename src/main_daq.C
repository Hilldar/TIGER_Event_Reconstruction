#include <iostream>
#include <fstream>
#include "daq.h"
using namespace std;

int main(int argc, const char* argv[]){
  //string folder = argv[1];
  int run    = atoi(argv[1]);
  int subrun;
  if(argc==2) daq(run);
  return 0;
};
