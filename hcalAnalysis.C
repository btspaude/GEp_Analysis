#include <iostream>
#include <fstream>
#include <stdio.h>
#include <sstream>
#include <string>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <math.h>
#include <TH1D.h>
#include <TH1F.h>
#include <TH2D.h>
#include <TF1.h>
#include <TH2F.h>
#include <TFile.h>
#include <iomanip>
#include <time.h>
#include <TVector3.h>
#include <TMath.h>
#include "TCutG.h"
#include <TLine.h>
#include <TLorentzVector.h>
#include <stdlib.h>
#include <vector>
#include <cmath>
#include <TGraphErrors.h>
#include "TLegend.h"
#include "TVirtualPad.h"
#include "TObjString.h"
#include "TVirtualHistPainter.h"
#include "THLimitsFinder.h"
#include "TFitResult.h"
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>

using namespace std;

const TString REPLAYED_DIR = "/adaqfs/home/a-onl/sbs/Rootfiles";

namespace THCal {
    Double_t sbs_hcal_clus_adctime;
    Double_t sbs_hcal_clus_id; 
    int_t nhits;
}

TChain *T=0;

//Define histograms as global pointers
TH1D *hHcal_clus_adctime;


//sbs.hcal.clus.adctime

//main loop
void hcalAnalysis(int run_num, int kevents,int seg){
    //Define histograms
    hHcal_clus_adctime = new TH1D("hHcal_clus_adctime","adctime of block; clus.atime [ns]; counts",500,-25.,200.);
    std::cout << "test line" << std::endl;

    if(!T) { 
        // TString sInFile = REPLAYED_DIR + "/" + InFile + ".root";
        T = new TChain("T");
    
        TString subfile, sInFile;
    
        subfile = TString::Format("gep5_replayed_%d_%dk_events_firstseg_%d",run_num,kevents,seg);
        sInFile = REPLAYED_DIR + "/" + subfile + ".root";
        cout << "Input ROOT file = " << sInFile << endl;
        cout << "Adding " << sInFile << endl;
        T->Add(sInFile);
        cout << "Adding " << nruns << " files ... " << endl;
        /*for (Int_t i=1; i<=nruns; i++) {
            subfile = TString::Format("cdet_%d_%d_%d",RunNumber1,kevents,i);
            //subfile = TString::Format("_%d_1000000_%d",RunNumber1,i);
            sInFile = REPLAYED_DIR + "/" + subfile + ".root";
            cout << "Input ROOT file = " << sInFile << endl;
            cout << "Adding " << sInFile << endl;
            T->Add(sInFile);
        }*/
     
        
        // disable all branches
        T->SetBranchStatus("*",0);
        // enable branches
        T->SetBranchStatus("sbs.hcal.*",1);
    
        // set branches
        T->SetBranchAddress("sbs.hcal.clus_adctime",THCal::sbs_hcal_clus_adctime);
        T->SetBranchAddress("sbs.hcal.clus.id",THCal::sbs_hcal_clus_id);
    }

    Int_t nevents = T->GetEntries();
    cout << "N entries in tree is " << nevents << endl;

    Int_t EventCounter = 0;
    cout << "Starting Event Loop" << endl;
    // loop through entries
    for(Int_t event=0; event < nevents; event++){
    
        T->GetEntry(event);
        EventCounter++;
        if (EventCounter % 1000 == 0) {
        cout << EventCounter << "/" << nevents << endl;
   
        }

        int N_clus = sbs_hcal_clus_id.GetSize();
        for(int nElem=0;nElem<N_clus;++nElem){
            hHcal_clus_adctime->Fill(sbs_hcal_clus_adctime[nElem]);
        }

    } //End loop through tree entries
    hHcal_clus_adctime->Draw();

}