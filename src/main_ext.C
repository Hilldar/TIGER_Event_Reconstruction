#include <iostream>
#include <fstream>
#include <vector>
#include "ext.h"
using namespace std;

int main(int argc, const char* argv[]){
  if(argc==2){
    int run = atoi(argv[1]);
    ext(run);
    return 0;
  }
  else if(argc==5 && atoi(argv[1])>4){
    vector<int> runs;
    runs.push_back(atoi(argv[1]));
    int feb     = atoi(argv[2]);
    int chip    = atoi(argv[3]);
    int channel = atoi(argv[4]);
    ext(runs,feb,chip,channel);
    return 0;
  }
  else if(argc>2 && atoi(argv[1])<5){
    const int nruns = atoi(argv[1]);
    vector<int> runs;
    for(int i=0;i<nruns;i++) runs.push_back(atoi(argv[2+i]));
    cout<<nruns<<" ";
    for(int i=0;i<nruns;i++) cout<<runs.at(i)<<" ";
    cout<<endl;
    if(argc == nruns+2){
      ext(runs);
    }
    if(argc == nruns+5){
      int feb     = atoi(argv[nruns+2]);
      int chip    = atoi(argv[nruns+3]);
      int channel = atoi(argv[nruns+4]);
      ext(runs,feb,chip,channel);
    }
    return 0;
  }
  else cout<<"BAD ARGUMENT"<<endl;
  return 0;
};
