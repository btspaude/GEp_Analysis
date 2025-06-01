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

// Global histograms
//1D hist
TH1D *hHcal_clus_adctime;
TH1D *hHcal_clus_x;
TH1D *hHcal_clus_y;

//2D hist
TH2D *h2Hcal_clus_x_vs_y;

// Main function
void hcalAnalysis(int run_num, int kevents, int seg) {
    hHcal_clus_adctime = new TH1D("hHcal_clus_adctime","adctime of block; clus.atime [ns]; counts",500, 0, 200.);
    hHcal_clus_x = new TH1D("hHcal_clus_x","cluster x position ; xpos (m); counts",)
    
    h2Hcal_clus_x = new TH2D

    // Set up the TChain
    TChain chain("T");

    TString subfile = TString::Format("gep5_replayed_%d_%dk_events_firstseg_%d", run_num, kevents, seg);
    TString sInFile = REPLAYED_DIR + "/" + subfile + ".root";

    cout << "Adding file: " << sInFile << endl;
    chain.Add(sInFile);

    // TTreeReader setup, named "tree"
    TTreeReader tree(&chain);

    // Declare readers for the arrays (vectors)
    TTreeReaderArray<Double_t> sbs_hcal_clus_adctime(tree, "sbs.hcal.clus.adctime");
    TTreeReaderArray<Double_t> sbs_hcal_clus_id(tree, "sbs.hcal.clus.id");
    TTreeReaderArray<Double_t> sbs_hcal_clus_blk_id(tree, "sbs.hcal.clus_blk.id");

    int eventCounter = 0;

    cout << "Starting event loop..." << endl;

    while (tree.Next()) {
        ++eventCounter;

        if (eventCounter % 1000 == 0) {
            cout << eventCounter << " events processed." << endl;
        }

        // Fill histogram
        Int_t N_clus = sbs_hcal_clus_id.GetSize();
        for (int nElem = 0; nElem < N_clus; ++nElem) {
            hHcal_clus_adctime->Fill(sbs_hcal_clus_adctime[nElem]);
        }
    }

    cout << "Finished processing " << eventCounter << " events." << endl;
    //Eventually move Draw to its own plot Macro, can one make cuts in the plot macros rather then main macro?
    hHcal_clus_adctime->Draw();
} //End Main Macro
