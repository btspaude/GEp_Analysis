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
TH1D *hHcal_clus_adctime;
TH1D *hHcal_clus_x;
TH1D *hHcal_clus_y;
TH1D *hHcal_a_time;

//2D hist
TH2D *h2Hcal_clus_adctime_vsCh;
TH2D *h2Hcal_clus_x_vs_y;
TH2D *h2AtimevsCh;
TH2D *h2hcal_clus_atime_vsCh;

// Main function
void hcalAnalysis(int run_num, int kevents, int seg) {
    
    //Initialize Histograms
    hHcal_a_time = new TH1D("h1Atime","pulse adc time; hcal.a_time [ns]; ",60,0,200.);
    hHcal_clus_adctime = new TH1D("hHcal_clus_adctime","HCal ADC Cluster Time; adctime [ns]; counts",500, 0, 200.);

    //h2AtimevsCh = new TH2D("h2AtimevsCh","ADC peak timing; channel; hcal.a_time [ns]",290,0.,290.,60,-40.,200.); //hcal adc peak time
    h2Hcal_clus_adctime_vsCh = new TH2D("h2Hcal_clus_adctime_vsCh","HCal ADC Clus time vs ElemID; ElemID; adc time [ns]",290,0.,290.,500,0.,200.);
    //hHcal_clus_x = new TH1D("hHcal_clus_x","cluster x position ; xpos (m); counts",)
    
    //h2Hcal_clus_x_vs_y = new TH2D

    // Set up the TChain
    TChain chain("T");

    TString subfile = TString::Format("gep5_replayed_%d_%dk_events_firstseg_%d", run_num, kevents, seg);
    TString sInFile = REPLAYED_DIR + "/" + subfile + ".root";

    cout << "Adding file: " << sInFile << endl;
    chain.Add(sInFile);

    // TTreeReader setup, named "tree"
    TTreeReader tree(&chain);

    // Declare readers for the arrays (vectors)
    TTreeReaderArray<Double_t> sbs_hcal_adcelemID(tree,"sbs.hcal.adcelemID");
    TTreeReaderArray<Double_t> sbs_hcal_a_time(tree,"sbs.hcal.a_time");

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
        //ElemID loop
        int N_adcElemId = sbs_hcal_adcelemID.GetSize();
        for (int nElem = 0; nElem < N_adcElemId; ++nElem) {
           hHcal_a_time->Fill(sbs_hcal_a_time[nElem]);
        }

        // Fill HCal cluster histograms
        Int_t N_clus = sbs_hcal_clus_id.GetSize();
        for (int nElem = 0; nElem < N_clus; ++nElem) {
            hHcal_clus_adctime->Fill(sbs_hcal_clus_adctime[nElem]);
            h2Hcal_clus_adctime_vsCh->Fill(sbs_hcal_clus_id[nElem], sbs_hcal_clus_adctime[nElem]);
        }
    }

    cout << "Finished processing " << eventCounter << " events." << endl;
} //End Main Macro

auto plotHCalADCtime(){

    // Create a canvas
    auto c1 = new TCanvas("c1", "HCal ADC Cluster Time", 800, 800);
    c1->Divide(1, 2);  // 1 column, 2 rows
    //c1->SetGrid();

    c1->cd(1);
    gPad->SetLogy();
    // Style the histogram
    hHcal_clus_adctime->SetLineColor(kBlue);
    hHcal_clus_adctime->SetLineWidth(2);
    hHcal_clus_adctime->SetFillColor(kBlue);
    hHcal_clus_adctime->SetStats(true);

    // Draw it
    hHcal_clus_adctime->Draw();

    c1->cd(2);
    h2Hcal_clus_adctime_vsCh->SetLineColor(kBlue);
    h2Hcal_clus_adctime_vsCh->SetLineWidth(2);
    h2Hcal_clus_adctime_vsCh->SetFillColor(kBlue);
    h2Hcal_clus_adctime_vsCh->SetStats(true);

    h2Hcal_clus_adctime_vsCh->Draw("colz");
    // Optional: save output
    //c1->SaveAs("histogram.png");

    // Return the canvas (or anything else, depending on use)
    return c1;
}    
 

