#include <iostream>
#include <fstream>
#include "post_event.h"

using namespace std;
int main(int argc, const char* argv[]){
  //string folder = argv[1];
  int run    = atoi(argv[1]);
  int subrun = atoi(argv[2]);
  post_event(run,subrun);
  return 0;
};
