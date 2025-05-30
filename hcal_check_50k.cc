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

void hcal_check_50k(Int_t run){
//void hcal_analysis(char const *input_rootfile){ //alternate to tchain
    gROOT->Reset();
    using namespace std;

    //####### Reading Map File #####
    //##############################

    cout<<"### Reading HCAL Map File ###"<<endl;
    
    ifstream fin1("hcal_mapAll.rtf");
  
  typedef pair<pair<int,int>,pair<int,int>>pair1;

  map<int,pair1> ChMap;

  int ch,ampS,amp_ch,fADC,fADC_ch,discS,disc_ch,hvS,hv_ch;
  int line_num=0;
  if(fin1.is_open()){
    string line;
    while(getline(fin1,line)){
      //cout<<line<<endl;
      ++line_num;
      sscanf(line.c_str(),"%d %*c%d-%d %*s %*s %*c%d-%d %*s Disc%d-%d %*s %*s %*s %*s %*s L%d.%d %*s",&ch,&ampS,&amp_ch,&fADC,&fADC_ch,&discS,&disc_ch,&hvS,&hv_ch);
      //sscanf(line.c_str(),"%d %*c%d-%d %*32c Disc%d-%d  PP%*46c L%d.%d %*c",&ch,&ampS,&amp_ch,&discS,&disc_ch,&hvS,&hv_ch);
      
      ChMap[ch]=make_pair(make_pair(ampS*100+amp_ch,discS*100+disc_ch),make_pair(fADC*100+fADC_ch,hvS*100+hv_ch));
      //cout<<ch<<"\t"<<amp<<"\t"<<amp_ch<<endl;
    }
  }else{
    cout<<"hcal map file cannot be opened"<<endl;
    exit(-1);
  }

  fin1.close();
  cout<<"### Finished Reading HCAL Map File ###"<<endl;
  //###Finished Reading Map File ###  
    
  cout<<endl;
  cout<<"### Starting to read ROOT File ###"<<endl;
    //####### tchain adding #####
    TChain *tchnT = new TChain("T");
    //tchnT->Add(Form("$HOME/sbs/Rootfiles/hcal_gep_cosmic_%d_*.root/T",run));
    tchnT->Add(Form("$HOME/sbs/Rootfiles/gep5_replayed_nogems_%d_50k_*.root/T",run));
    //tchnT->Add(Form("$HOME/sbs/Rootfiles/hcal_gen_r%d_*.root/T",run));
    //tchnT->Add(Form("/chafs2/work1/sbs/HCAL/hcal_gen_replay_%d_nev_10000.root/T",run));
    //tchnT->Add(Form("/chafs2/work1/sbs/HCAL/hcal_gen_replay_%d_nev_100001.root/T",run));
    TTreeReader tree(tchnT);
    
    //TChain *tchnTS = new TChain("TSsbs");
    //tchnTS->Add("$HOME/sbs/Rootfiles/gen_replayed_2174_50k_events.root/TSsbs");
    //TTreeReader tree1(tchnTS);
    
    //Creating new root file with histograms
    TFile *fout = new TFile(Form("/chafs2/work1/sbs/HCAL/gep_hcal_%d_50k.root",run),"recreate");
    //TFile *fout = new TFile(Form("gen_hcal_%d.root",run),"recreate");
    
    //###### Reading tree with TTreeReader (instead of TTreeBranch) #########
    //ifstream *file(input_rootfile);
    //TTree* tree= (TTree*) file.Get("T");
    //TFile *file = new TFile(input_rootfile);
    //TTreeReader tree("T",file);
    
     //tree leaves of hcal expert replay
    //TTreeReaderArray<double> sbs_hcal_tdc(tree,"sbs.hcal.tdc");
    //TTreeReaderArray<double> sbs_hcal_tdc_mult(tree,"sbs.hcal.tdc_mult");
    //TTreeReaderArray<double> sbs_hcal_tdcelemID(tree,"sbs.hcal.tdcelemID");
    //TTreeReaderArray<double> sbs_hcal_hits_t(tree,"sbs.hcal.hits.t");
    //TTreeReaderArray<double> sbs_hcal_hits_TDCelemID(tree,"sbs.hcal.hits.TDCelemID");
    
    //TTreeReaderArray<double> sbs_hcal_a(tree,"sbs.hcal.a");
    TTreeReaderArray<double> sbs_hcal_a_p(tree,"sbs.hcal.a_p");
    TTreeReaderArray<double> sbs_hcal_a_time(tree,"sbs.hcal.a_time");
    //TTreeReaderArray<double> sbs_hcal_a_amp(tree,"sbs.hcal.a_amp");
    TTreeReaderArray<double> sbs_hcal_a_amp_p(tree,"sbs.hcal.a_amp_p");
    //TTreeReaderArray<double> sbs_hcal_ped(tree,"sbs.hcal.ped");
    //TTreeReaderArray<double> sbs_hcal_hits_a(tree,"sbs.hcal.hits.a");
    TTreeReaderArray<double> sbs_hcal_adcelemID(tree,"sbs.hcal.adcelemID");

    TTreeReaderArray<double> sbs_hcal_vtp_clus_e(tree,"sbs.hcal.vtp.clus.e");
    TTreeReaderArray<double> sbs_hcal_vtp_clus_time(tree,"sbs.hcal.vtp.clus.time");
    TTreeReaderArray<double> sbs_hcal_vtp_clus_size(tree,"sbs.hcal.vtp.clus.size");
    TTreeReaderArray<double> sbs_hcal_vtp_clus_x(tree,"sbs.hcal.vtp.clus.x");
    TTreeReaderArray<double> sbs_hcal_vtp_clus_y(tree,"sbs.hcal.vtp.clus.y");
    TTreeReaderValue<double> sbs_hcal_vtp_detid(tree,"sbs.hcal.vtp.detid");
    TTreeReaderValue<double> sbs_hcal_vtp_errflag(tree,"sbs.hcal.vtp.errflag");
    
    //tree leaves of gen_replay (hcal side)
    TTreeReaderValue<double> sbs_hcal_e(tree,"sbs.hcal.e");
    TTreeReaderValue<double> sbs_hcal_eblk(tree,"sbs.hcal.eblk");
    TTreeReaderValue<double> sbs_hcal_againblk(tree,"sbs.hcal.againblk");
    TTreeReaderValue<double> sbs_hcal_rowblk(tree,"sbs.hcal.rowblk");
    TTreeReaderValue<double> sbs_hcal_colblk(tree,"sbs.hcal.colblk");
    TTreeReaderValue<double> sbs_hcal_x(tree,"sbs.hcal.x");
    TTreeReaderValue<double> sbs_hcal_y(tree,"sbs.hcal.y");
    TTreeReaderValue<double> sbs_hcal_nblk(tree,"sbs.hcal.nblk");
    TTreeReaderValue<double> sbs_hcal_idblk(tree,"sbs.hcal.idblk");
    TTreeReaderValue<double> sbs_hcal_atimeblk(tree,"sbs.hcal.atimeblk");
    TTreeReaderValue<double> sbs_hcal_tdctimeblk(tree,"sbs.hcal.tdctimeblk");
    TTreeReaderValue<double> sbs_hcal_index(tree,"sbs.hcal.index");
    TTreeReaderValue<double> sbs_hcal_nclus(tree,"sbs.hcal.nclus");
    TTreeReaderValue<double> sbs_hcal_ngoodADChits(tree,"sbs.hcal.ngoodADChits");
    
    TTreeReaderArray<double> sbs_hcal_clus_again(tree,"sbs.hcal.clus.again");
    TTreeReaderArray<double> sbs_hcal_clus_adctime(tree,"sbs.hcal.clus.adctime");
    TTreeReaderArray<double> sbs_hcal_clus_atimeblk(tree,"sbs.hcal.clus.atimeblk");
    TTreeReaderArray<double> sbs_hcal_clus_e(tree,"sbs.hcal.clus.e");
    TTreeReaderArray<double> sbs_hcal_clus_eblk(tree,"sbs.hcal.clus.eblk");
    TTreeReaderArray<double> sbs_hcal_clus_col(tree,"sbs.hcal.clus.col");
    TTreeReaderArray<double> sbs_hcal_clus_row(tree,"sbs.hcal.clus.row");
    TTreeReaderArray<double> sbs_hcal_clus_id(tree,"sbs.hcal.clus.id");
    TTreeReaderArray<double> sbs_hcal_clus_nblk(tree,"sbs.hcal.clus.nblk");
    TTreeReaderArray<double> sbs_hcal_clus_tdctime(tree,"sbs.hcal.clus.tdctime");
    TTreeReaderArray<double> sbs_hcal_clus_x(tree,"sbs.hcal.clus.x");
    TTreeReaderArray<double> sbs_hcal_clus_y(tree,"sbs.hcal.clus.y");
    
    TTreeReaderArray<double> sbs_hcal_clus_blk_again(tree,"sbs.hcal.clus_blk.again");
    TTreeReaderArray<double> sbs_hcal_clus_blk_atime(tree,"sbs.hcal.clus_blk.atime");
    TTreeReaderArray<double> sbs_hcal_clus_blk_e(tree,"sbs.hcal.clus_blk.e");
    TTreeReaderArray<double> sbs_hcal_clus_blk_col(tree,"sbs.hcal.clus_blk.col");
    TTreeReaderArray<double> sbs_hcal_clus_blk_row(tree,"sbs.hcal.clus_blk.row");
    TTreeReaderArray<double> sbs_hcal_clus_blk_id(tree,"sbs.hcal.clus_blk.id");
    TTreeReaderArray<double> sbs_hcal_clus_blk_tdctime(tree,"sbs.hcal.clus_blk.tdctime");
    TTreeReaderArray<double> sbs_hcal_clus_blk_x(tree,"sbs.hcal.clus_blk.x");
    TTreeReaderArray<double> sbs_hcal_clus_blk_y(tree,"sbs.hcal.clus_blk.y");
    
    //sbs.trig (10 sums)
    TTreeReaderArray<double> sbs_trig_a_p(tree,"sbs.trig.a_p");
    TTreeReaderArray<double> sbs_trig_a_time(tree,"sbs.trig.a_time");
    TTreeReaderArray<double> sbs_trig_a_amp_p(tree,"sbs.trig.a_amp_p");
    TTreeReaderArray<double> sbs_trig_adcelemID(tree,"sbs.trig.adcelemID");
    
    //bbcal tdc trig
    //TTreeReaderArray<double> bb_tdctrig_tdc(tree,"bb.tdctrig.tdc");
    //TTreeReaderArray<double> bb_tdctrig_tdcelemID(tree,"bb.tdctrig.tdcelemID");
    //trig bits
    TTreeReaderValue<double> g_trigbits(tree,"g.trigbits");
    TTreeReaderValue<u_int> fEvtHdr_TrigBits(tree,"fEvtHdr.fTrigBits");
   /* 
    //e_kinematics
    TTreeReaderValue<double> e_kine_Q2(tree,"e.kine.Q2");
    TTreeReaderValue<double> e_kine_W2(tree,"e.kine.Q2");
    TTreeReaderValue<double> e_kine_angle(tree,"e.kine.angle");
    TTreeReaderValue<double> e_kine_nu(tree,"e.kine.nu");
    TTreeReaderValue<double> e_kine_q_x(tree,"e.kine.q_x");
    TTreeReaderValue<double> e_kine_q_y(tree,"e.kine.q_y");
    TTreeReaderValue<double> e_kine_q_z(tree,"e.kine.q_z");
    TTreeReaderValue<double> e_kine_x_bj(tree,"e.kine.x_bj");
    */
    
    //tree leaves of TSsbs tree
    //TTreeReaderValue<double> sbs_TS1_BB_scalerRate(tree1,"sbs.TS1_BB.scalerRate");
    //TTreeReaderValue<double> sbs_TS2_Hcal_scalerRate(tree1,"sbs.TS1_Hcal.scalerRate");
    //TTreeReaderValue<double> sbs_TS1_Coin_scalerRate(tree1,"sbs.TS3_Coin.scalerRate");
    //TTreeReaderValue<double> sbs_L1A_scalerRate(tree1,"sbs.L1A.scalerRate");
    
    //###### Define histrograms ########
    
     //histogram for hcal expert replay
    //TH1D *h1TDC = new TH1D("h1TDC","TDC timing; hcal.tdc [ns]; ",1000,0.,0.);
    //TH2D *h2TDCvsCh = new TH2D("h2TDCvsCh","TDC timing; channel; hcal.tdc [ns]",100,0.,0.,1000,0.,0.);

    //TH1D *h1ADC = new TH1D("h1ADC","signal integral; hcal.a [pC]; ",1000,0.,500.);
    TH1D *h1ADC_p = new TH1D("h1ADC_p","signal integral; hcal.a_p [pC]; ",1200,-200.,400.);
    //TH1D *h1ADCAmp = new TH1D("h1ADCAmp","pulse amplitude; hcal.a_amp [mV]; ",1025,0.,500.);
    TH1D *h1ADCAmp_p = new TH1D("h1ADCAmp_p","pulse amplitude; hcal.a_amp_p [mV]; ",1025,0.,500.);
    TH1D *h1Atime = new TH1D("h1Atime","pulse adc time; hcal.a_time [ns]; ",60,-40.,200.);
    //TH1D *h1ped = new TH1D("h1ped","pedestal; hcal.ped [mV]; ",1025,0.,500.);
    //TH2D *h2ADCvsCh = new TH2D("h2ADCvsCh","signal integral; channel; hcal.a [pC]",290,0.,290.,2000,1.,1001.);
    TH2D *h2ADC_pvsCh = new TH2D("h2ADC_pvsCh","signal integral; channel; hcal.a_p [pC]",290,0.,290.,2000,-200.,800.);
    //TH2D *h2ADCAmpvsCh = new TH2D("h2ADCAmpvsCh","pulse amplitude; channel; hcal.a_amp [mV]",290,0.,290.,2050,1.,1001.);
    TH2D *h2ADCAmp_pvsCh = new TH2D("h2ADCAmp_pvsCh","pulse amplitude; channel; hcal.a_amp_p [mV]",290,0.,290.,2050,-50.,950.);
    //TH2D *h2pedvsCh = new TH2D("h2pedvsCh","pedestal; channel; hcal.ped [mV]",290,0.,290.,1025,0.,500.);
    TH2D *h2AtimevsCh = new TH2D("h2AtimevsCh","ADC peak timing; channel; hcal.a_time [ns]",290,0.,290.,60,-40.,200.);
    

    //hcal Vtp information
    TH1D *h1hcal_vtp_clus_e = new TH1D("h1hcal_vtp_clus_e","HCAL VTP clus e; vpt.clus.e [MeV]; counts",5000,0.,5000.);
    TH1D *h1hcal_vtp_clus_size = new TH1D("h1hcal_vtp_clus_size","HCAL VTP clus size; vpt.clus.size; counts",50,0.,50.);
    TH1D *h1hcal_vtp_clus_time = new TH1D("h1hcal_vtp_clus_time","HCAL VTP clus time; vpt.clus.time; counts",75,0.,75.);


    TH2D *h2hcal_vtp_clus_eVstime = new TH2D("h2hcal_vtp_clus_eVstime","HCAL VTP clus e vs time; vpt.clus.time; vtp.clus.e",75,0.,75.,10000,0,10000.);
    TH2D *h2hcal_vtp_clus_yVsx = new TH2D("h2hcal_vtp_clus_yVsx","HCAL VTP clus y vs x; vpt.clus.x; vtp.clus.y",12,1.,13.,24,1,25.);
    TH2D *h2hcal_vtp_clus_eVsblock = new TH2D("h2hcal_vtp_clus_eVsblock","HCAL VTP clus e vs block; block id; vtp.clus.e",288,1.,289.,10000,0,10000.);

    TH2D *h2hcal_vtp_Maxclus_eVstime = new TH2D("h2hcal_vtp_Maxclus_eVstime","HCAL VTP Max clus e vs time; vpt.clus.time; vtp.clus.e",75,0.,75.,10000,0,10000.);
    TH2D *h2hcal_vtp_Maxclus_yVsx = new TH2D("h2hcal_vtp_Maxclus_yVsx","HCAL VTP Max clus y vs x; vpt.clus.x; vtp.clus.y",12,1.,13.,24,1,25.);
    TH2D *h2hcal_vtp_Maxclus_eVsblock = new TH2D("h2hcal_vtp_Maxclus_eVsblock","HCAL VTP Max clus e vs block; block id; vtp.clus.e",288,1.,289.,10000,0,10000.);

    TH2D *h2hcal_vtp_MaxclusIn_eVstime = new TH2D("h2hcal_vtp_MaxclusIn_eVstime","HCAL VTP Max clus e vs time; vpt.clus.time; vtp.clus.e",75,0.,75.,10000,0,10000.);
    TH2D *h2hcal_vtp_MaxclusIn_yVsx = new TH2D("h2hcal_vtp_MaxclusIn_yVsx","HCAL VTP Max clus y vs x; vpt.clus.x; vtp.clus.y",12,1.,13.,24,1,25.);
    TH2D *h2hcal_vtp_MaxclusIn_eVsblock = new TH2D("h2hcal_vtp_MaxclusIn_eVsblock","HCAL VTP Max clus e vs block; block id; vtp.clus.e",288,1.,289.,10000,0,10000.);


    //histrogram for best cluster (hcal blocks and energy)    
    TH1D *h1hcal_clMax_e = new TH1D("h1hcal_clMAx_e","Energy deposition of block; hcal.e [GeV]; counts",10000,0.,10000.);
    TH1D *h1hcal_clMax_eblk = new TH1D("h1hcal_clMAx_eblk","Energy deposition of Max block; hcal.eblk [GeV]; counts",10000,0.,10000.);
    TH1D *h1hcal_clMax_again = new TH1D("h1hcal_clMax_again","clus gain; hcal.again; counts",100,0.,0.01);
    TH1D *h1hcal_clMax_adc = new TH1D("h1hcal_clMax_adc","clus adc (e/gain); hcal adc; counts",2000,-50.,950.);
    TH1D *h1hcal_clMax_atime = new TH1D("h1hcal_clMax_atime","atime of block; hcal.atime [ns]; counts",500,-25.,200.);
    TH1D *h1hcal_clMax_tdctime = new TH1D("h1hcal_clMax_tdctime","tdctime of block; hcal.tdctimeblk [ns];counts ",17200,-1100.,7500.);
    TH2D *h2hcal_clMax_row_vscol = new TH2D("h2hcal_clMax_row_vscol","Clus row-col; hcal colblk;hcal rowblk",12,0,12,24,0,24);
    TH2D *h2hcal_clMax_e_vsId = new TH2D("h2hcal_clMax_e_vsId","Clus e vs ID;block ID;hcal.e [GeV]",290,0,290,10000,0.,10000.);
    TH2D *h2hcal_clMax_eblk_vsId = new TH2D("h2hcal_clMax_eblk_vsId","Clus eblk vs ID;block ID;hcal.eblk [GeV]",290,0,290,10000,0.,10000.);
    TH2D *h2hcal_clMax_again_vsId = new TH2D("h2hcal_clMax_again_vsId","Clus gain vs ID;block ID;hcal.again",290,0,290,100,0.,0.01);
    TH2D *h2hcal_clMax_adc_vsId = new TH2D("h2hcal_clMax_adc_vsId","Clus adc vs ID;block ID;hcal adc",290,0,290,1000,-50.,1000.);
    TH2D *h2hcal_clMax_tdc_vsCh = new TH2D("h2hcal_clMax_tdc_vsCh","Clus TDC vs Ch; channel; hcal.tdctimeblk [ns]",290,0.,290.,17200,-1100.,7500.);
    TH2D *h2hcal_clMax_atime_vsCh = new TH2D("h2hcal_clMax_atime_vsCh","Clus atimeblk vs Ch; channel; hcal.atimeblk [ns]",290,0.,290.,500,-50.,200.);
    TH2D *h2hcal_clMax_atime_vstdctime = new TH2D("h2hcal_clMax_atime_vstdctime","Clus atimeblk vs tdctimeblk; hcal.tdctimeblk [ns]; hcal.atimeblk [ns]",17200,-1100.,7500.,500,-50.,200.);
    TH2D *h2hcal_clMax_adc_vstdctime = new TH2D("h2hcal_clMax_adc_vstdctime","Clus adc vs tdctimeblk;hcal.tdctimeblk [ns];hcal adc",17200,-1100,7500,1000,-50.,1000.);
    //TH2D *h2hcal_clMax_e_vstdctime = new TH2D("h2hcal_clMax_e_vstdctime","Clus e vs tdctimeblk;hcal.tdctimeblk [ns];hcal.e [GeV]",17200,-1100,7500,10000,0.,10000.);


    //histrogram for all cluster (hcal blocks and energy)    
    TH1D *h1hcal_clus_e = new TH1D("h1hcal_clus_e","Energy deposition of block; clus.e [GeV]; counts",10000,0.,10000.);
    TH1D *h1hcal_clus_again = new TH1D("h1hcal_clus_again","clus gain; clus.again; counts",100,0.,0.01);
    TH1D *h1hcal_clus_adc = new TH1D("h1hcal_clus_adc","clus adc (e/gain); clus adc; counts",2000,-50.,950.);
    TH1D *h1hcal_clus_atime = new TH1D("h1hcal_clus_atime","atime of block; clus.atime [ns]; counts",500,-25.,200.);
    TH1D *h1hcal_clus_tdctime = new TH1D("h1hcal_clus_tdctime","tdctime of block; clus.tdctime [ns];counts ",17200,-1100.,7500.);
    TH2D *h2hcal_clus_row_vscol = new TH2D("h2hcal_clus_row_vscol","Clus row-col; clus col;clus row",12,0,12,24,0,24);
    TH2D *h2hcal_clus_e_vsId = new TH2D("h2hcal_clus_e_vsId","Clus e vs ID;block ID;clus.e [GeV]",290,0,290,10000,0.,10000.);
    TH2D *h2hcal_clus_again_vsId = new TH2D("h2hcal_clus_again_vsId","Clus gain vs ID;block ID;clus.again",290,0,290,100,0.,0.01);
    TH2D *h2hcal_clus_adc_vsId = new TH2D("h2hcal_clus_adc_vsId","Clus adc vs ID;block ID;clus adc",290,0,290,1000,-50.,1000.);
    TH2D *h2hcal_clus_tdc_vsCh = new TH2D("h2hcal_clus_tdc_vsCh","Clus TDC vs Ch; channel; clus.tdctime [ns]",290,0.,290.,17200,-1100.,7500.);
    TH2D *h2hcal_clus_atime_vsCh = new TH2D("h2hcal_clus_atime_vsCh","Clus atime vs Ch; channel; clus.atime [ns]",290,0.,290.,500,-50.,200.);
    TH2D *h2hcal_clus_atime_vstdctime = new TH2D("h2hcal_clus_atime_vstdctime","Clus atime vs tdctime; clus.tdctime [ns]; clus.atime [ns]",17200,-1100.,7500.,500,-50.,200.);
    TH2D *h2hcal_clus_adc_vstdctime = new TH2D("h2hcal_clus_adc_vstdctime","Clus adc vs tdctime;clus tdctime [ns];clus adc",17200,-1100,7500,1000,-50.,1000.);
    //TH2D *h2hcal_clus_e_vstdctime = new TH2D("h2hcal_clus_e_vstdctime","Clus e vs tdctime;clus tdctime [ns];clus.e [GeV]",17200,-1100,7500,10000,0.,10000.);

    //histrogram for cluster blocks for primary cluster (hcal blocks and energy)    
    TH1D *h1hcal_clusblk_e = new TH1D("h1hcal_clusblk_e","Energy deposition of block; clus_blk.e [GeV]; counts",10000,0.,10000.);
    TH1D *h1hcal_clusblk_again = new TH1D("h1hcal_clusblk_again","clusblk gain; clus_blk.again; counts",100,0.,0.01);
    TH1D *h1hcal_clusblk_adc = new TH1D("h1hcal_clusblk_adc","clusblk adc (e/gain); clus_blk adc; counts",2000,-50.,950.);
    TH1D *h1hcal_clusblk_atime = new TH1D("h1hcal_clusblk_atime","atime of block; clus_blk.atime [ns]; counts",500,-25.,200.);
    TH1D *h1hcal_clusblk_tdctime = new TH1D("h1hcal_clusblk_tdctime","tdctime of block; clus_blk.tdctime [ns];counts ",17200,-1100.,7500.);
    TH2D *h2hcal_clusblk_row_vscol = new TH2D("h2hcal_clusblk_row_vscol","Clus_blk row-col; col;row",12,0,12,24,0,24);
    TH2D *h2hcal_clusblk_e_vsId = new TH2D("h2hcal_clusblk_e_vsId","Clus_blk e vs ID;block ID;clus_blk.e [GeV]",290,0,290,10000,0.,10000.);
    TH2D *h2hcal_clusblk_again_vsId = new TH2D("h2hcal_clusblk_again_vsId","Clus_blk gain vs ID;block ID;clus_blk.again",290,0,290,100,0.,0.01);
    TH2D *h2hcal_clusblk_adc_vsId = new TH2D("h2hcal_clusblk_adc_vsId","Clus_blk adc vs ID;block ID;clus_blk adc",290,0,290,1000,-50.,1000.);
    TH2D *h2hcal_clusblk_tdc_vsCh = new TH2D("h2hcal_clusblk_tdc_vsCh","Clus_blk TDC vs Ch; channel; clus_blk.tdctime [ns]",290,0.,290.,17200,-1100.,7500.);
    TH2D *h2hcal_clusblk_atime_vsCh = new TH2D("h2hcal_clusblk_atime_vsCh","Clus_blk atime vs Ch; channel; clus_blk.atime [ns]",290,0.,290.,500,-50.,200.);
    TH2D *h2hcal_clusblk_atime_vstdctime = new TH2D("h2hcal_clusblk_atime_vstdctime","Clus_blk atime vs tdctime; clus_blk.tdctime [ns]; clus_blk.atime [ns]",17200,-1100.,7500.,500,-50.,200.);
    TH2D *h2hcal_clusblk_adc_vstdctime = new TH2D("h2hcal_clusblk_adc_vstdctime","Clus_blk adc vs tdctime;clus_blk tdctime [ns];clus_blk adc",17200,-1100,7500,1000,-50.,1000.);
    //TH2D *h2hcal_clusblk_e_vstdctime = new TH2D("h2hcal_clusblk_e_vstdctime","Clus_blk e vs tdctime;clus_blk tdctime [ns];clus_blk.e [GeV]",17200,-1100,7500,10000,0.,10000.);
       
    //trigbits
    TH1D *h1trigbits = new TH1D("h1trigbits","run trigger bits; trigbits; ",50,0.,25.);

    TH2D *h2hcal_clMax_atime_vsfADCCh = new TH2D("h2hcal_clMax_atime_vsfADCCh","Clus atimeblk vs fADC Ch; fADC channel; hcal.atimeblk [ns]",290,0.,290.,500,-50.,200.);
    TH2D *h2hcal_clMax_atime_vsDiscCh = new TH2D("h2hcal_clMax_atime_vsDiscCh","Clus atimeblk vs Disc Ch; Disc channel; hcal.atimeblk [ns]",290,0.,290.,500,-50.,200.);
    TH2D *h2hcal_clMax_atime_vsAmpCh = new TH2D("h2hcal_clMax_atime_vsAmpCh","Clus atimeblk vs Amplifier Ch; Amplifier channel; hcal.atimeblk [ns]",290,0.,290.,500,-50.,200.);
    TH2D *h2hcal_clMax_atime_vsHVCh = new TH2D("h2hcal_clMax_atime_vsHVCh","Clus atimeblk vs HV Ch; HV channel; hcal.atimeblk [ns]",305,0.,305.,500,-50.,200.);

    TH2D *h2hcal_clus_atime_vsfADCCh = new TH2D("h2hcal_clus_atime_vsfADCCh","Clus atime vs fADC Ch; fADC channel; clus.atime [ns]",290,0.,290.,500,-50.,200.);
    TH2D *h2hcal_clus_atime_vsDiscCh = new TH2D("h2hcal_clus_atime_vsDiscCh","Clus atime vs Disc Ch; Disc channel; clus.atime [ns]",290,0.,290.,500,-50.,200.);
    TH2D *h2hcal_clus_atime_vsAmpCh = new TH2D("h2hcal_clus_atime_vsAmpCh","Clus atime vs Amplifier Ch; Amplifier channel; clus.atime [ns]",290,0.,290.,500,-50.,200.);
    TH2D *h2hcal_clus_atime_vsHVCh = new TH2D("h2hcal_clus_atime_vsHVCh","Clus atime vs HV Ch; HV channel; clus.atime [ns]",305,0.,305.,500,-50.,200.);

    /*
    TH2D *h2_DiscChAtime[19];
    TH2D *h2_AmpChAtime[19];
    TH2D *h2_fADCChAtime[19];
    
    char h_nameDis[75];
    char hh_nameDis[75];
    char h_nameAmp[75];
    char hh_nameAmp[75];
    char h_namefADC[75];
    char hh_namefADC[75];
    for(int kk=1;kk<19;++kk){
        sprintf(h_nameDis,"Disc. %d",kk);
	sprintf(hh_nameDis,"; Channel; a_time [ns]");
        h2_DiscChAtime[kk] = new TH2D(h_nameDis,hh_nameDis,16,1.,17.,400,0.,200.);
	sprintf(h_nameAmp,"Amp. %d",kk);
	sprintf(hh_nameAmp,"; Channel; a_time [ns]");
        h2_AmpChAtime[kk] = new TH2D(h_nameAmp,hh_nameAmp,16,1.,17.,400,0.,200.);
	sprintf(h_namefADC,"fADC %d",kk);
	sprintf(hh_namefADC,"; Channel; a_time [ns]");
        h2_fADCChAtime[kk] = new TH2D(h_namefADC,hh_namefADC,16,1.,17.,400,0.,200.);
    }

    TH2D *h2_HVChAtime[25];
    
    char h_nameHV[75];
    char hh_nameHV[75];
    for(int kk=1;kk<25;++kk){
        sprintf(h_nameHV,"HV. %d",kk);
	sprintf(hh_nameHV,"; Channel; a_time [ns]");
        h2_HVChAtime[kk] = new TH2D(h_nameHV,hh_nameHV,12,1.,13.,400,0.,200.);
    }
    */

   
    
    //###### gPad settings ######
    //gStyle->SetOptFit(0000);
    gStyle->SetOptStat(1110);
    gStyle->SetOptFit(0001);
    gStyle->SetTitleAlign(23);
    //gStyle->SetLineWidth(2.0);
    //gStyle->SetHistLineWidth(2.0);
    gStyle->SetTitleSize(0.045,"TXYZ");
    gStyle->SetTitleSize(0.045,"T");
    gStyle->SetTitleXOffset(1.1);
    gStyle->SetTitleYOffset(1.3);
    gStyle->SetTitleFont(62,"TXYZ");
    gStyle->SetLabelSize(0.045,"XYZ");
    //gStyle->SetLabelFont(62,"XY");
    //gStyle->SetLineColorAlpha(kBlue,0);
    gStyle->SetStatY(0.90);
    gStyle->SetStatX(0.90);
    gStyle->SetStatW(0.22);
    gStyle->SetStatH(0.23);
    gStyle->SetStatFont(62);
    //gStyle->SetStatFontSize(0.5);
    //gStyle->SetStatTextColor(kBlack);
    //gStyle->SetPadLeftMargin(0.13); //best setting previously for (3,2)
    //gStyle->SetPadRightMargin(0.12); ////best setting previously for (3,2)
    //gStyle->SetPadBottomMargin(0.11);
    gStyle->SetPadLeftMargin(0.05); //best setting previously for (3,2)
    gStyle->SetPadRightMargin(0.05); ////best setting previously for (3,2)
    gStyle->SetPadBottomMargin(0.10);
    
    gStyle->SetStatStyle(0); //set statbox transparent
    gStyle->SetTitleStyle(0);
    TGaxis::SetMaxDigits(4);
    gROOT->ForceStyle(true);
    
    //###### Being ready to read each events with while loop ######
    int Nentries = tree.GetEntries(1);
    int entry_num=0;
    //int nbytes = 0, nb = 0;
    while(tree.Next()){
    //for (int jentry=0; jentry<tree.GetEntries();jentry++) {
        //Long64_t ientry = tree.LoadTree(jentry);
        //if (ientry < 0) break;
        //nb = tree->GetEntry(jentry);   nbytes += nb;
        // if (Cut(ientry) < 0) continue;
        //if(entry_num%1000==0) cout<<entry_num<<endl;
        if(entry_num%1000==0) cout<<entry_num<<"/"<<Nentries<<"\r"<<flush;
	/*
        //looking for tdctrig data
        int N_bbtrig = bb_tdctrig_tdcelemID.GetSize();
        double bb_trigtime=0, hcal_trigtime=0, coin_trigtime=0, bb_fast=0;
        for(int nElem=0; nElem<N_bbtrig; ++nElem){
            double trig_tdcelemID= bb_tdctrig_tdcelemID[nElem];
            //hist fill for gen_replay bb_trigtdc
            if(trig_tdcelemID==0){
                hcal_trigtime=bb_tdctrig_tdc[nElem];
            }else if(trig_tdcelemID==1){
                coin_trigtime=bb_tdctrig_tdc[nElem];
            }else if(trig_tdcelemID==2){
                bb_trigtime=bb_tdctrig_tdc[nElem];
            }else if(trig_tdcelemID==5){
                bb_fast=bb_tdctrig_tdc[nElem];
            }
        } //end of for() loop with nElem
	*/
        
        // for ttreeReadervalue (not required for ReaderArray)
        
       	//if((bb_trigtime-hcal_trigtime)>40 && (bb_trigtime-hcal_trigtime)<80 && *fEvtHdr_TrigBits==4){

        int N_adcElemId = sbs_hcal_adcelemID.GetSize();
        //cout<< "Hcal tdc size " << NtdcArray <<"  "<<NtdchitsArray<<" adc (NadcArray) size "<<NadcArray<<"  "<<NadchitsArray<<endl;
        for(int nElem=0; nElem<N_adcElemId; ++nElem){
            double adc_time= sbs_hcal_a_time[nElem];
            h1Atime->Fill(sbs_hcal_a_time[nElem]);
            h2AtimevsCh->Fill(sbs_hcal_adcelemID[nElem],sbs_hcal_a_time[nElem]);
            if(adc_time>0){    
                 //hist fill for hcal expert replay
                //h1TDC->Fill(sbs_hcal_tdc[nElem]);
                //h2TDCvsCh->Fill(sbs_hcal_tdcelemID[nElem],sbs_hcal_tdc[nElem]);
               
                //h1ADC->Fill(sbs_hcal_a[nElem]);
                h1ADC_p->Fill(sbs_hcal_a_p[nElem]);
                //h1ADCAmp->Fill(sbs_hcal_a_amp[nElem]);
                h1ADCAmp_p->Fill(sbs_hcal_a_amp_p[nElem]);
                //h1Atime->Fill(sbs_hcal_a_time[nElem]);
                //h1ped->Fill(sbs_hcal_ped[nElem]);
                //h2ADCvsCh->Fill(sbs_hcal_adcelemID[nElem],sbs_hcal_a[nElem]);
                h2ADC_pvsCh->Fill(sbs_hcal_adcelemID[nElem],sbs_hcal_a_p[nElem]);
                //h2ADCAmpvsCh->Fill(sbs_hcal_adcelemID[nElem],sbs_hcal_a_amp[nElem]);
                h2ADCAmp_pvsCh->Fill(sbs_hcal_adcelemID[nElem],sbs_hcal_a_amp_p[nElem]);
                //h2pedvsCh->Fill(sbs_hcal_adcelemID[nElem],sbs_hcal_ped[nElem]);
                //h2AtimevsCh->Fill(sbs_hcal_adcelemID[nElem],sbs_hcal_a_time[nElem]);
                
            }
        } //end of for() loop with nElem

	
        int N_clusblk = sbs_hcal_clus_blk_id.GetSize();
        for(int nElem=0;nElem<N_clusblk;++nElem){
            h1hcal_clusblk_e->Fill(sbs_hcal_clus_blk_e[nElem]);
            h1hcal_clusblk_again->Fill(sbs_hcal_clus_blk_again[nElem]);
            h1hcal_clusblk_adc->Fill(sbs_hcal_clus_blk_e[nElem]/sbs_hcal_clus_blk_again[nElem]);
            h1hcal_clusblk_atime->Fill(sbs_hcal_clus_blk_atime[nElem]);
            h1hcal_clusblk_tdctime->Fill(sbs_hcal_clus_blk_tdctime[nElem]);
            h2hcal_clusblk_row_vscol->Fill(sbs_hcal_clus_blk_col[nElem],sbs_hcal_clus_blk_row[nElem]);
            h2hcal_clusblk_e_vsId->Fill(sbs_hcal_clus_blk_id[nElem],sbs_hcal_clus_blk_e[nElem]);
            h2hcal_clusblk_again_vsId->Fill(sbs_hcal_clus_blk_id[nElem],sbs_hcal_clus_blk_again[nElem]);
            h2hcal_clusblk_adc_vsId->Fill(sbs_hcal_clus_blk_id[nElem],sbs_hcal_clus_blk_e[nElem]/sbs_hcal_clus_blk_again[nElem]);
            h2hcal_clusblk_tdc_vsCh->Fill(sbs_hcal_clus_blk_id[nElem],sbs_hcal_clus_blk_tdctime[nElem]);
            h2hcal_clusblk_atime_vsCh->Fill(sbs_hcal_clus_blk_id[nElem],sbs_hcal_clus_blk_atime[nElem]);
	    h2hcal_clusblk_atime_vstdctime->Fill(sbs_hcal_clus_blk_tdctime[nElem],sbs_hcal_clus_blk_atime[nElem]);
	    h2hcal_clusblk_adc_vstdctime->Fill(sbs_hcal_clus_blk_tdctime[nElem],sbs_hcal_clus_blk_e[nElem]/sbs_hcal_clus_blk_again[nElem]);
	    //h2hcal_clusblk_e_vstdctime->Fill(sbs_hcal_clus_blk_tdctime[nElem],sbs_hcal_clus_blk_e[nElem]);
        }



	int N_clus = sbs_hcal_clus_id.GetSize();
        for(int nElem=0;nElem<N_clusblk;++nElem){
            h1hcal_clus_e->Fill(sbs_hcal_clus_e[nElem]);
            h1hcal_clus_again->Fill(sbs_hcal_clus_again[nElem]);
            h1hcal_clus_adc->Fill(sbs_hcal_clus_e[nElem]/sbs_hcal_clus_again[nElem]);
            h1hcal_clus_atime->Fill(sbs_hcal_clus_adctime[nElem]);
            h1hcal_clus_tdctime->Fill(sbs_hcal_clus_tdctime[nElem]);
            h2hcal_clus_row_vscol->Fill(sbs_hcal_clus_col[nElem],sbs_hcal_clus_row[nElem]);
            h2hcal_clus_e_vsId->Fill(sbs_hcal_clus_id[nElem],sbs_hcal_clus_e[nElem]);
            h2hcal_clus_again_vsId->Fill(sbs_hcal_clus_id[nElem],sbs_hcal_clus_again[nElem]);
            h2hcal_clus_adc_vsId->Fill(sbs_hcal_clus_id[nElem],sbs_hcal_clus_e[nElem]/sbs_hcal_clus_again[nElem]);
            h2hcal_clus_tdc_vsCh->Fill(sbs_hcal_clus_id[nElem],sbs_hcal_clus_tdctime[nElem]);
            h2hcal_clus_atime_vsCh->Fill(sbs_hcal_clus_id[nElem],sbs_hcal_clus_adctime[nElem]);
	    h2hcal_clus_atime_vstdctime->Fill(sbs_hcal_clus_tdctime[nElem],sbs_hcal_clus_adctime[nElem]);
	    h2hcal_clus_adc_vstdctime->Fill(sbs_hcal_clus_tdctime[nElem],sbs_hcal_clus_e[nElem]/sbs_hcal_clus_again[nElem]);
	    //h2hcal_clus_e_vstdctime->Fill(sbs_hcal_clus_tdctime[nElem],sbs_hcal_clus_e[nElem]);
	}
	

	h1hcal_clMax_e->Fill(*sbs_hcal_e);
	h1hcal_clMax_eblk->Fill(*sbs_hcal_eblk);
        h1hcal_clMax_again->Fill(*sbs_hcal_againblk);
        h1hcal_clMax_adc->Fill(*sbs_hcal_eblk/(*sbs_hcal_againblk));
        h1hcal_clMax_atime->Fill(*sbs_hcal_atimeblk);
        h1hcal_clMax_tdctime->Fill(*sbs_hcal_tdctimeblk);
        h2hcal_clMax_row_vscol->Fill(*sbs_hcal_colblk,*sbs_hcal_rowblk);
        h2hcal_clMax_e_vsId->Fill(*sbs_hcal_idblk,*sbs_hcal_e);
	h2hcal_clMax_eblk_vsId->Fill(*sbs_hcal_idblk,*sbs_hcal_eblk);
        h2hcal_clMax_again_vsId->Fill(*sbs_hcal_idblk,*sbs_hcal_againblk);
        h2hcal_clMax_adc_vsId->Fill(*sbs_hcal_idblk,*sbs_hcal_e/(*sbs_hcal_againblk));
        h2hcal_clMax_tdc_vsCh->Fill(*sbs_hcal_idblk,*sbs_hcal_tdctimeblk);
        h2hcal_clMax_atime_vsCh->Fill(*sbs_hcal_idblk,*sbs_hcal_atimeblk);
	h2hcal_clMax_atime_vstdctime->Fill(*sbs_hcal_tdctimeblk,*sbs_hcal_atimeblk);
	h2hcal_clMax_adc_vstdctime->Fill(*sbs_hcal_tdctimeblk,*sbs_hcal_e/(*sbs_hcal_againblk));
	//h2hcal_clMax_e_vstdctime->Fill(*sbs_hcal_tdctimeblk,*sbs_hcal_e);
	
	
	auto ClusIDblk = *sbs_hcal_idblk;
	auto it = ChMap.find(ClusIDblk);
	auto AMPs=it->second.first.first;
	auto AMPslt=AMPs/100;
	auto AMPch=AMPs%100;
	auto DISs=it->second.first.second;
	auto DISslt=DISs/100;
	auto DISch=DISs%100;
	auto ADCs=it->second.second.first;
	auto ADCslt=ADCs/100;
	auto ADCch=ADCs%100;
	auto HVs=it->second.second.second;
	auto HVslt=HVs/100;
	auto HVch=HVs%100;

	h2hcal_clMax_atime_vsDiscCh->Fill((DISslt-1)*16+DISch,*sbs_hcal_atimeblk);
	h2hcal_clMax_atime_vsfADCCh->Fill((ADCslt-1)*16+ADCch,*sbs_hcal_atimeblk);
	
	if(ClusIDblk<=144){
	  h2hcal_clMax_atime_vsAmpCh->Fill((AMPslt-1)*16+AMPch,*sbs_hcal_atimeblk);
	  h2hcal_clMax_atime_vsHVCh->Fill(HVslt*12+HVch,*sbs_hcal_atimeblk);
	}else{
	  h2hcal_clMax_atime_vsAmpCh->Fill((AMPslt-1)*16+AMPch+144,*sbs_hcal_atimeblk);
	  h2hcal_clMax_atime_vsHVCh->Fill(HVslt*12+HVch+144,*sbs_hcal_atimeblk);
	}
	
	
	
	
	//}//end of if for trigger cut

	
	double vtp_maxElem=-1.; double vtp_maxE=-1.; double vtp_maxX=-1.; double vtp_maxY=-1.; double vtp_maxTime=-1.;
	double vtp_maxElemIn=-1.; double vtp_maxEIn=-1.; double vtp_maxXIn=-1.; double vtp_maxYIn=-1.; double vtp_maxTimeIn=-1.;
	int N_vtpClus = sbs_hcal_vtp_clus_e.GetSize();
	for(int nElem=0; nElem<N_vtpClus;++nElem){
	   h1hcal_vtp_clus_e->Fill(sbs_hcal_vtp_clus_e[nElem]);
	   h1hcal_vtp_clus_size->Fill(sbs_hcal_vtp_clus_size[nElem]);
	   h1hcal_vtp_clus_time->Fill(sbs_hcal_vtp_clus_time[nElem]);

	   h2hcal_vtp_clus_eVstime->Fill(sbs_hcal_vtp_clus_time[nElem],sbs_hcal_vtp_clus_e[nElem]);
	   h2hcal_vtp_clus_yVsx->Fill(sbs_hcal_vtp_clus_x[nElem],sbs_hcal_vtp_clus_y[nElem]);
	   h2hcal_vtp_clus_eVsblock->Fill(sbs_hcal_vtp_clus_x[nElem]+12*(sbs_hcal_vtp_clus_y[nElem]-1),sbs_hcal_vtp_clus_e[nElem]);
	   if(vtp_maxE < sbs_hcal_vtp_clus_e[nElem]){
	     vtp_maxElem=nElem;
	     vtp_maxE=sbs_hcal_vtp_clus_e[nElem];
	     vtp_maxX=sbs_hcal_vtp_clus_x[nElem];
	     vtp_maxY=sbs_hcal_vtp_clus_y[nElem];
	     vtp_maxTime=sbs_hcal_vtp_clus_time[nElem];
	   }
	   if(vtp_maxE < sbs_hcal_vtp_clus_e[nElem]){
	     vtp_maxElem=nElem;
	     vtp_maxE=sbs_hcal_vtp_clus_e[nElem];
	     vtp_maxX=sbs_hcal_vtp_clus_x[nElem];
	     vtp_maxY=sbs_hcal_vtp_clus_y[nElem];
	     vtp_maxTime=sbs_hcal_vtp_clus_time[nElem];
	   }
	}

	
	h2hcal_vtp_Maxclus_eVstime->Fill(vtp_maxTime,vtp_maxE);
	h2hcal_vtp_Maxclus_yVsx->Fill(vtp_maxX,vtp_maxY);
	h2hcal_vtp_Maxclus_eVsblock->Fill(vtp_maxX+12*(vtp_maxY-1),vtp_maxE);

	

        ++entry_num;
    }//end of while() loop over all events of tree

    tree.end();
    delete tchnT;
    
    //gROOT->SetBatch(kTRUE); // no flashing of canvas in the screen (like -b)
    //TCanvas *c1=new TCanvas("c1","c1",4000,3000);
    
    fout->Write();

    fout->Close();
    
    cout<<"Total number of entries processed: "<<entry_num<<" of "<<Nentries<<endl;
    cout<<"Output root file with histograms: /chafs2/work1/sbs/HCAL/gep_hcal_runN_50k.root"<<endl;
}
