#include <iostream>
#include <vector>
#include <TChain.h>
#include <TFile.h>
#include <TTree.h>
#include <TTreeReader.h>
#include <TTreeReaderArray.h>
#include <TString.h>
#include <TROOT.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <fstream>
#include <TMath.h>
#include <TH1.h>
#include <TH1D.h>
#include <TH2.h>
#include <TF1.h>
#include <TF2.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TSystem.h>
#include <TLatex.h>
using namespace std;

const TString REPLAYED_DIR = "/adaqfs/home/a-onl/sbs/Rootfiles";

/* Define constants for CDet geometry*/
static const int TDCmult_cut = 100;
static const double xcut = 998.0;
//static const int nhitcutlow = 2;
//static const int nhitcuthigh = 20;
static const double TDC_calib_to_ns = 0.01;
static const double HotChannelRatio = .01;

static const int NumPaddles = 16;
static const int NumBars = 14;
static const int NumLayers = 2;
static const int NumSides = 2;
static const int NumModules = 3;
static const int NumHalfModules = NumModules*NumSides*NumLayers;

static const int NumCDetPaddles = NumHalfModules*NumBars*NumPaddles; //2688
static const int nRef = 4;
static const int NumRefPaddles = 4;
static const int nTdc = NumCDetPaddles+NumRefPaddles; //2704

static const int NumSidesTotal = NumSides*NumLayers;
static const int NumCDetPaddlesPerSide = NumCDetPaddles/NumSidesTotal; //672
static const int NumLogicalPaddlesPerSide = NumCDetPaddlesPerSide+nRef; //676

static const int nBarsADC = 0;
static const double ADCCUT = 150.;   //100.0

static const double ecal_dist = 6.6;
static const double cdet_dist_offset = 2.0;
static const double cdet_y_half_length = 0.30;

/* Define vectors as globals */
std::vector<std::vector<double>> *vcdet_good_le(nTDC);
std::vector<double> *vcdet_pmtnum;

vector<vector<double>> readDataFromFiles(const vector<string>& filenames) {
    const int NUM_VALUES = 42;
    vector<vector<double>> allData;

    for (const auto& filename : filenames) {
        ifstream infile(filename);
        if (!infile) {
            cerr << "Error opening file: " << filename << endl;
            continue; // Skip this file and move on
        }

        vector<double> fileData;
        double value;
        for (int i = 0; i < NUM_VALUES; ++i) {
            infile >> value;
            if (!infile) {
                cerr << "Error reading value " << i << " from file: " << filename << endl;
                break; // Stop reading this file
            }
            fileData.push_back(value);
        }

        if (fileData.size() == NUM_VALUES) {
            allData.push_back(fileData);
        } else {
            cerr << "Incomplete data in file: " << filename << endl;
        }

        infile.close();
    }

    return allData;
}

// Main function
void earmAnalysis(int run_num, int kevents, int seg) {
    
    //Initialize Histograms
    //vcdet_good_le = new TH1D("hcdet_good_le","cdet good le; LE time [ns]; ",60,0,200.);
    

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
    TTreeReaderArray<Double_t> earm_cdet_pmtnum(tree,"Ndata.earm.cdet.hit.pmtnum");

    int eventCounter = 0;

    cout << "Starting event loop..." << endl;

    while (tree.Next()) {
        ++eventCounter;

        if (eventCounter % 1000 == 0) {
            cout << eventCounter << " events processed." << endl;
        }

        for(int ihit = 0 ; ihit < earm_cdet_tdc_le.GetSize(); ++ihit)
        //start filling histograms here

    }

    cout << "Finished processing " << eventCounter << " events." << endl;
} //End Main Macro