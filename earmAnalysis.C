#include <iostream>
#include <vector>
#include <TChain.h>
#include <TFile.h>
#include <TTree.h>
#include <TTreeReader.h>
#include <TTreeReaderArray.h>
#include <TH1D.h>
#include <TString.h>

using namespace std;

const TString REPLAYED_DIR = "/adaqfs/home/a-onl/sbs/Rootfiles";

/* Set pointers for histograms */
//1D hist
TH1D *hcdet_good_le;

// Main function
void earmAnalysis(int run_num, int kevents, int seg) {
    
    //Initialize Histograms
    hcdet_good_le = new TH1D("hcdet_good_le","cdet good le; LE time [ns]; ",60,0,200.);
    

    // Set up the TChain
    TChain chain("T");

    TString subfile = TString::Format("gep5_replayed_%d_%dk_events_firstseg_%d", run_num, kevents, seg);
    TString sInFile = REPLAYED_DIR + "/" + subfile + ".root";

    cout << "Adding file: " << sInFile << endl;
    chain.Add(sInFile);

    // TTreeReader setup, named "tree"
    TTreeReader tree(&chain);

    // Declare readers for the arrays (vectors)
    TTreeReaderArray<Double_t> earm_cdet_tdc_le(tree,"earm.cdet.hit.tdc_le");

    int eventCounter = 0;

    cout << "Starting event loop..." << endl;

    while (tree.Next()) {
        ++eventCounter;

        if (eventCounter % 1000 == 0) {
            cout << eventCounter << " events processed." << endl;
        }
        //start filling histograms here

    }

    cout << "Finished processing " << eventCounter << " events." << endl;
} //End Main Macro
