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

// Global vectors to hold raw data from the tree
std::vector<double> vHcal_atime;
std::vector<double> vHcal_clus_adctime;
std::vector<double> vHcal_clus_id;
std::vector<double> vHcal_atimeblk;

const TString REPLAYED_DIR = "/adaqfs/home/a-onl/sbs/Rootfiles";

// Main function to extract data from tree and fill vectors
void hcalAnalysis(int run_num, int kevents, int seg) {

    // Set up the TChain
    TChain chain("T");
    TString subfile = TString::Format("gep5_replayed_%d_%dk_events_firstseg_%d", run_num, kevents, seg);
    TString sInFile = REPLAYED_DIR + "/" + subfile + ".root";

    cout << "Adding file: " << sInFile << endl;
    chain.Add(sInFile);

    // TTreeReader setup
    TTreeReader tree(&chain);

    // Declare readers for the arrays (vectors)
    TTreeReaderArray<Double_t> sbs_hcal_adcelemID(tree, "sbs.hcal.adcelemID");
    TTreeReaderArray<Double_t> sbs_hcal_a_time(tree, "sbs.hcal.a_time");

    TTreeReaderArray<Double_t> sbs_hcal_clus_adctime(tree, "sbs.hcal.clus.adctime");
    TTreeReaderArray<Double_t> sbs_hcal_clus_id(tree, "sbs.hcal.clus.id");

    TTreeReaderArray<Double_t> sbs_hcal_idblk(tree, "sbs.hcal.idblk");
    TTreeReaderArray<Double_t> sbs_hcal_atimeblk(tree, "sbs.hcal.atimeblk");

    int eventCounter = 0;

    cout << "Starting event loop..." << endl;

    while (tree.Next()) {
        ++eventCounter;

        if (eventCounter % 1000 == 0) {
            cout << eventCounter << " events processed." << endl;
        }

        int N_adcElemId = sbs_hcal_adcelemID.GetSize();
        for (int nElem = 0; nElem < N_adcElemId; ++nElem) {
            vHcal_atime.push_back(sbs_hcal_a_time[nElem]);
        }

        int N_clus = sbs_hcal_clus_id.GetSize();
        for (int nElem = 0; nElem < N_clus; ++nElem) {
            vHcal_clus_adctime.push_back(sbs_hcal_clus_adctime[nElem]);
            vHcal_clus_id.push_back(sbs_hcal_clus_id[nElem]);
        }

        int N_idblk = sbs_hcal_idblk.GetSize();
        for (int nElem = 0; nElem < N_idblk; ++nElem) {
            vHcal_atimeblk.push_back(sbs_hcal_atimeblk[nElem]);
        }
    }

    cout << "Finished processing " << eventCounter << " events." << endl;
}

// Plotting macro with temporary cuts
void plotHcalClusTime(int minCut = 0, int maxCut = 200) {

    auto c = new TCanvas("cHcal_clus_adctime", "Cluster ADC Time with Cut", 800, 600);
    auto hHcal_clus_adctime = new TH1D("hHcal_clus_adctime", "Cluster ADC Time with cut;adctime [ns];counts", 204, 0, 200);

    for (size_t nElem = 0; nElem < vHcal_clus_adctime.size(); ++nElem) {
        if (vHcal_clus_adctime[nElem] > minCut && vHcal_clus_adctime[nElem] < maxCut) {
            hHcal_clus_adctime->Fill(vHcal_clus_adctime[nElem]);
            
        }
    }
    hHcal_clus_adctime->Draw();
}

void plotHcalBlks(double min_time = 0, double max_time = 200) {
    auto c = new TCanvas("cHcal_atimeblk", "Block ADC Time", 800, 600);
    auto hHcal_atimeblk = new TH1D("hHcal_atimeblk", "Block ADC Time with cut;atimeblk [ns];counts", 204, 0, 200);

    for (size_t nElem = 0; nElem < vHcal_atimeblk.size(); ++nElem) {
        if (vHcal_atimeblk[nElem] > min_time && vHcal_atimeblk[nElem] < max_time) {
            hHcal_atimeblk->Fill(vHcal_atimeblk[nElem]);
        }
    }
    hHcal_atimeblk->Draw();
}

void plotHcalReg(int minCut = 0, int maxCut = 200) {

    auto c = new TCanvas("cHcal_atime", "HCal a_time with Cut", 800, 600);
    auto hHcal_atime = new TH1D("hHcal_atime", "HCal a_time ;a_time [ns];counts", 204, 0, 200);

    for (size_t nElem = 0; nElem < vHcal_atime.size(); ++nElem) {
        if (vHcal_atime[nElem] > minCut && vHcal_atime[nElem] < maxCut) {
            hHcal_atime->Fill(vHcal_atime[nElem]);
        }
    }
    hHcal_atime->Draw();
}