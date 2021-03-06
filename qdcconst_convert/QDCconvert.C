#include "TF1.h"
#include "TFile.h"
#include "TTree.h"
#include "TDirectory.h"
#include "Riostream.h"
#include <iostream>
#include <fstream>

void QDCconvert(){
    auto file = new TFile("QDCcalib.root","RECREATE");
    auto tree = new TTree("tree","tree");

    for(int layer_id = 0; layer_id <4; layer_id++){
	for(int tig_id=1; tig_id<65; tig_id++){
	    for(int chip_id=1; chip_id<3; chip_id++){
		std::string data_folder = Form("L%i_QDC_calib/",layer_id);
		std::string FILENAME = Form("L%dFEB%i_c%i_Efine_calib.txt",layer_id,tig_id,chip_id);

		std::ifstream data_file(data_folder + FILENAME);

		if(!data_file){
		    std::cout << "could not open the file " << FILENAME << std::endl;
		    continue;
		}
		int ch_id,gemroc_id,SW_FEB_id,FEB_label;
		float slope, constant, qmax;

		tree->Branch("layer_id",&layer_id,"layer_id/I");
		tree->Branch("channel_id",&ch_id,"channel_id/I");
		tree->Branch("gemroc_id",&gemroc_id,"gemroc_id/I");
		tree->Branch("SW_FEB_id",&SW_FEB_id,"SW_FEB_id/I");
		tree->Branch("FEB_label",&FEB_label,"FEB_label/I");
		tree->Branch("HW_FEB_id",&tig_id,"HW_FEB_id/I");
		tree->Branch("chip_id",&chip_id,"chip_id/I");
		tree->Branch("slope",&slope,"slope/F");
		tree->Branch("constant",&constant,"constant/F");
		tree->Branch("qmax",&qmax,"qmax/F");

		while(data_file >> ch_id >> constant >> slope){
		  if(layer_id==2){
		    if     (tig_id==4)  FEB_label = 16;
		    else if(tig_id==17)  FEB_label = 17;
		    else if(tig_id==18) FEB_label = 18;
		    else if(tig_id==6)  FEB_label = 19;
		    else if(tig_id==9)  FEB_label = 20;
		    else if(tig_id==33) FEB_label = 21;
		    else if(tig_id==38) FEB_label = 22;
		    //else if(tig_id==34) FEB_label = 23;
		    else if(tig_id==53) FEB_label = 23;
		    else if(tig_id==37) FEB_label = 24;
		    else if(tig_id==36) FEB_label = 25;
		    else if(tig_id==50) FEB_label = 26;
		    else if(tig_id==1)  FEB_label = 27;
		    else if(tig_id==2)  FEB_label = 28;
		    else if(tig_id==19) FEB_label = 29;
		    else if(tig_id==52) FEB_label = 30;
		    else if(tig_id==32) FEB_label = 31;
		    else if(tig_id==30) FEB_label = 32;
		    else if(tig_id==26) FEB_label = 33;
		    else if(tig_id==27) FEB_label = 34;
		    else if(tig_id==56) FEB_label = 35;
		    else if(tig_id==25) FEB_label = 36;
		    //else if(tig_id==57) FEB_label = 37;
		    else if(tig_id==49) FEB_label = 37;
		    else if(tig_id==12) FEB_label = 38;
		    else if(tig_id==7)  FEB_label = 39;
		    else if(tig_id==20) FEB_label = 40;
		    else if(tig_id==21) FEB_label = 41;
		    else if(tig_id==24) FEB_label = 42;
		    else if(tig_id==3)  FEB_label = 43;
		    else continue;
		  }
		  else if(layer_id==0){
		    if     (tig_id==32) FEB_label = 48;
		    else if(tig_id==10) FEB_label = 49;
		    else if(tig_id==9)  FEB_label = 52;
		    else if(tig_id==28) FEB_label = 53;
		    else if(tig_id==26) FEB_label = 54;
                    else if(tig_id==29) FEB_label = 55;
		    else continue;
		  }
		  else if(layer_id==1){
		    if     (tig_id==61) FEB_label = 0;
		    else if(tig_id==5) FEB_label = 1;
		    else if(tig_id==2) FEB_label = 2;
		    else if(tig_id==21) FEB_label = 3;
		    else if(tig_id==25) FEB_label = 4;
		    else if(tig_id==10) FEB_label = 5;
		    else if(tig_id==29) FEB_label = 6;
		    else if(tig_id==34) FEB_label = 7;
		    else if(tig_id==32) FEB_label = 8;
		    else if(tig_id==26) FEB_label = 9;
		    else if(tig_id==27) FEB_label = 10;
		    else if(tig_id==22) FEB_label = 11;
		    else if(tig_id==24) FEB_label = 12;
		    else if(tig_id==4) FEB_label = 13;
		    else if(tig_id==19) FEB_label = 14;
		    else if(tig_id==30) FEB_label = 15;
		    else continue;
		  }
		  else continue;

		  
		  gemroc_id = FEB_label/4;
		  SW_FEB_id = (FEB_label%4)*2 + chip_id-1;

		  auto f1 = new TF1("f1","[0]*x + [1]",-1000,1000);
		  f1->SetParameter(0,slope);
		  f1->SetParameter(1,constant);
		  qmax = f1->GetX(0);
		  
		  tree->Fill();
		}
	    }
	}
    }
    file->Write();
    file->Close();
    
}


