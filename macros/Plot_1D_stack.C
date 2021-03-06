#include <iostream>
#include <string>

#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TAxis.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TColor.h>
#include <TColorWheel.h>
#include <TH1D.h>
#include <TStyle.h>

#include "../include/ReducedBase_new.hh"
#include "../include/SampleSet.hh"
#include "RestFrames/RestFrames.hh"

using namespace RestFrames;
using namespace std;

vector<SampleSet*> g_Samples;

double g_Lumi;
string g_PlotTitle;
string g_Xname;
string g_units;
double g_Xmin;
double g_Xmax;
double g_NX;
double units_per_bin;

class SampleSet;

void Plot_1D_stack(){
  RestFrames::SetStyle();

  string StopNtuplePath = "/home/t3-ku/crogan/NTUPLES/StopNtuple_new/";
  int BKG_SKIP = 10; //takes 1 in every BKG_SKIP events
  bool logy = true;


  
  SampleSet DYjets;
  DYjets.SetBkg(true);
  DYjets.SetTitle("DY + jets");
  DYjets.SetColor(kGreen-7);
  DYjets.AddFile(StopNtuplePath+"All_Bkg_2017/DYJetsToLL_M-5to50_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17.root");
  DYjets.AddFile(StopNtuplePath+"All_Bkg_2017/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8_Fall17.root");
  DYjets.SetSkip(BKG_SKIP);
  g_Samples.push_back(&DYjets);

  SampleSet Wjets;
  Wjets.SetBkg(true);
  Wjets.SetTitle("W + jets");
  Wjets.SetColor(kRed-7);
  Wjets.AddFile(StopNtuplePath+"All_Bkg_2017/WJetsToLNu_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17.root");
  Wjets.AddFile(StopNtuplePath+"All_Bkg_2017/WJetsToLNu_HT-1200To2500_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17.root");
  Wjets.AddFile(StopNtuplePath+"All_Bkg_2017/WJetsToLNu_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17.root");
  Wjets.AddFile(StopNtuplePath+"All_Bkg_2017/WJetsToLNu_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17.root");
  Wjets.AddFile(StopNtuplePath+"All_Bkg_2017/WJetsToLNu_HT-600To800_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17.root");
  Wjets.AddFile(StopNtuplePath+"All_Bkg_2017/WJetsToLNu_HT-800To1200_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17.root");
  Wjets.SetSkip(BKG_SKIP);
  g_Samples.push_back(&Wjets);

  SampleSet ttX;
   ttX.SetBkg(true);
   ttX.SetTitle("t#bar{t} + X");
   ttX.SetColor(kAzure-7);
   ttX.AddFile(StopNtuplePath+"All_Bkg_2017/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8_TuneCP5.root");
   // ttX.AddFile(StopNtuplePath+"All_Bkg_2017/TTJets_DiLept_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17.root");
  // // ttX.AddFile(StopNtuplePath+"All_Bkg_2017/TTJets_SingleLeptFromT_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17.root");
  // // ttX.AddFile(StopNtuplePath+"All_Bkg_2017/TTJets_SingleLeptFromTbar_TuneCP5_13TeV-madgraphMLM-pythia8_Fall17.root");
  // // ttX.AddFile(StopNtuplePath+"All_Bkg_2017/ttWJets_TuneCP5_13TeV_madgraphMLM_pythia8_Fall17.root");
  // // ttX.AddFile(StopNtuplePath+"All_Bkg_2017/ttZJets_TuneCP5_13TeV_madgraphMLM_pythia8_Fall17.root");
  // // ttX.AddFile(StopNtuplePath+"All_Bkg_2017/TTGJets_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8_Fall17.root");
   ttX.SetSkip(BKG_SKIP);
   g_Samples.push_back(&ttX);
 
  SampleSet SIG1;
  SIG1.SetBkg(false);
  SIG1.SetTitle("m_{#chi^{#pm}_{1}/#chi^{0}_{2}} = 200, m_{#chi^{0}_{1}} = 170");
  SIG1.SetTreeName("SMS_200_170");
  SIG1.SetColor(kMagenta-7);
  SIG1.AddFile(StopNtuplePath+"All_Sig/SMS-TChiWZ_ZToLL_mZMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17.root");
  SIG1.SetSkip(1);
  g_Samples.push_back(&SIG1);

  SampleSet SIG2;
  SIG2.SetBkg(false);
  SIG2.SetTitle("m_{#chi^{#pm}_{1}/#chi^{0}_{2}} = 200, m_{#chi^{0}_{1}} = 197");
  SIG2.SetTreeName("SMS_200_197");
  SIG2.SetColor(kBlue-7);
  SIG2.AddFile(StopNtuplePath+"All_Sig/SMS-TChiWZ_ZToLL_mZMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17.root");
  SIG2.SetSkip(1);
  g_Samples.push_back(&SIG2);


  SampleSet SIG3;
  SIG3.SetBkg(false);
  SIG3.SetTitle("m_{#chi^{#pm}_{1}/#chi^{0}_{2}} = 200, m_{#chi^{0}_{1}} = 1");
  SIG3.SetTreeName("SMS_200_1");
  SIG3.SetColor(kViolet-7);
  SIG3.AddFile(StopNtuplePath+"All_Sig/SMS-TChiWZ_ZToLL_mZMin-0p1_TuneCP2_13TeV-madgraphMLM-pythia8_Fall17.root");
  SIG3.SetSkip(1);
  g_Samples.push_back(&SIG3);

 
  //////////////////

  int Nsample = g_Samples.size();
  cout << "Nsample: " << Nsample << endl;

  g_PlotTitle = "#eta_{lep}";
  g_Lumi = 100;

  g_Xname = "Eta";

  //Njets_X
  // g_Xmin = 0.0;
  // g_Xmax = 300.;
  // units_per_bin = 1.;

  //Eta
  g_Xmin = -3.3;
  g_Xmax = 3.3;
  units_per_bin = 0.1;
  g_units = " (rad)";

  //dPhiCMI
  // g_Xmin = 0.0;
  // g_Xmax = 3.3;
  // units_per_bin = 0.1;

  //M_lep
  // g_Xmin = 0.0;
  // g_Xmax = 0.5;
  // units_per_bin = 0.01;

  //MET
  // g_Xmin = 0.0;
  // g_Xmax = 1500.;
  // units_per_bin = 50.;
  g_Xname = "MET";
  g_Xmin = 0;
  g_Xmax = 4.;
  g_NX = 5;

  //pTlep
  // g_Xmin = 0.0;
  // g_Xmax = 200.;
  // units_per_bin = 5.;
  // g_units = " (GeV)";


  g_NX = (int)((g_Xmax - g_Xmin)/units_per_bin);

  TH1D* hist[Nsample];
  for(int i = 0; i < Nsample; i++)
    hist[i] = new TH1D(("h"+to_string(i)).c_str(),
		       ("h"+to_string(i)).c_str(),
		       g_NX,g_Xmin,g_Xmax);

  for(int s = 0; s < Nsample; s++){
    int Nfile = g_Samples[s]->GetNFile();
    cout << "Processing " << Nfile << " files for sample " << g_Samples[s]->GetTitle() << endl;
    for(int f = 0; f < Nfile; f++){
      cout << "   Processing file " << g_Samples[s]->GetFile(f) << " w/ tree " << g_Samples[s]->GetTreeName() << endl;
    
      TChain* chain = new TChain(g_Samples[s]->GetTreeName().c_str());
      chain->Add(g_Samples[s]->GetFile(f).c_str());

      ReducedBase* base = new ReducedBase(chain);

      int Nentry = base->fChain->GetEntries();

      int SKIP = g_Samples[s]->GetSkip();
      for(int e = 0; e < Nentry; e += SKIP){
	base->GetEntry(e);
	if((e/SKIP)%(std::max(1, int(Nentry/SKIP/10))) == 0)
	  cout << "      event " << e << " | " << Nentry << endl;



 if(base->ID_lep->at(0) != 3) //medium(?) ID selection
    continue;

	if(base->Nlep != 1)
    continue;

  if(base->MiniIso_lep->at(0) > 0.1)
    continue;

	




	hist[s]->Fill(base->Eta_lep->at(0), base->weight*g_Lumi*double(SKIP));
      }

      delete base;
      delete chain;
    }
  }

  TH1D* h_BKG = nullptr;
  bool isBKG = false;
  for(int i = 0; i < Nsample; i++){
    cout << "Sample " << g_Samples[i]->GetTitle() << " has " << hist[i]->Integral() << " events" << endl;
    if(g_Samples[i]->GetBkg()){
      if(!isBKG){
	h_BKG = (TH1D*) hist[i]->Clone("TOT_BKG");
	isBKG = true;
      } else {
	for(int k = 0; k < i; k++){
	  hist[k]->Add(hist[i]);
	}
	h_BKG->Add(hist[i]);
      }
    }
  }
  if(h_BKG)
    cout << "Total Background is " << h_BKG->Integral() << " events" << endl;
  
  double fmax = -1.;
  int imax = -1;
  for(int i = 0; i < Nsample; i++){
    if(hist[i]->GetMaximum() > fmax){
      fmax = hist[i]->GetMaximum();
      imax = i;
    }
  }
  float width = hist[0]->GetBinWidth(1);
  char *yaxis = new char[100];
  //sprintf(yaxis,"Events / %f", width);
  sprintf(yaxis,"Events / bin", width);


  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(11111111);
  TCanvas* can = (TCanvas*) new TCanvas("can","can",600.,500);


  can->SetLeftMargin(0.13);
  can->SetRightMargin(0.04);
  can->SetBottomMargin(0.15);
  can->SetTopMargin(0.085);
  can->SetGridx();
  can->SetGridy();
  can->Draw();
  can->cd();
  hist[imax]->Draw("hist");
  hist[imax]->SetTitle("");
  hist[imax]->GetXaxis()->CenterTitle();
  hist[imax]->GetXaxis()->SetTitleFont(132);
  hist[imax]->GetXaxis()->SetTitleSize(0.06);
  hist[imax]->GetXaxis()->SetTitleOffset(1.06);
  hist[imax]->GetXaxis()->SetLabelFont(132);
  hist[imax]->GetXaxis()->SetLabelSize(0.05);
  TString xaxis = g_Xname + g_units;
  hist[imax]->GetXaxis()->SetTitle(xaxis);
  hist[imax]->GetYaxis()->CenterTitle();
  hist[imax]->GetYaxis()->SetTitleFont(132);
  hist[imax]->GetYaxis()->SetTitleSize(0.06);
  hist[imax]->GetYaxis()->SetTitleOffset(1.);
  hist[imax]->GetYaxis()->SetLabelFont(132);
  hist[imax]->GetYaxis()->SetLabelSize(0.05);
  hist[imax]->GetYaxis()->SetTitle("a. u.");
  hist[imax]->GetYaxis()->SetTitle(yaxis);
  //hist[imax]->GetYaxis()->SetTitle("N_{evt} / fb^{-1}");

  for(int i = 0; i < Nsample; i++){
    if(g_Samples[i]->GetBkg()){
      hist[i]->SetLineColor(kBlack);
      hist[i]->SetLineWidth(1.0);
      hist[i]->SetFillColor(g_Samples[i]->GetColor());
      hist[i]->SetFillStyle(1001);
      hist[i]->Draw("SAME HIST");
    }
  }

  if(isBKG){
    h_BKG->SetLineWidth(3.0);
    h_BKG->SetLineColor(kRed);
    h_BKG->SetMarkerSize(0);
    h_BKG->Draw("SAME HIST");
  }
  
  for(int i = 0; i < Nsample; i++){
    if(!g_Samples[i]->GetBkg()){
      hist[i]->SetLineWidth(3.0);
      hist[i]->SetMarkerSize(0.);
      hist[i]->SetMarkerColor(kBlack);
      hist[i]->SetLineStyle(7);
      hist[i]->SetLineColor(g_Samples[i]->GetColor());
      hist[i]->Draw("SAME HIST");
    }
  }

  TLegend* leg = new TLegend(0.688,0.22,0.93,0.42);
  leg->SetTextFont(132);
  leg->SetTextSize(0.045);
  leg->SetFillColor(kWhite);
  leg->SetLineColor(kWhite);
  leg->SetShadowColor(kWhite);
  if(isBKG) leg->AddEntry(h_BKG, "SM total");
  for(int i = 0; i < Nsample; i++)
    if(g_Samples[i]->GetBkg())
      leg->AddEntry(hist[i],g_Samples[i]->GetTitle().c_str(),"F");
    else
      leg->AddEntry(hist[i],g_Samples[i]->GetTitle().c_str());
  leg->SetLineColor(kWhite);
  leg->SetFillColor(kWhite);
  leg->SetShadowColor(kWhite);
  leg->Draw("SAME");

  TLatex l;
  l.SetTextFont(132);
  l.SetNDC();
  l.SetTextSize(0.05);
  l.SetTextFont(132);
  l.DrawLatex(0.42,0.94,g_PlotTitle.c_str());
  l.SetTextSize(0.03);
  l.SetTextFont(42);
  l.DrawLatex(0.145,0.92,"#bf{#it{CMS}} Internal 13 TeV Simulation");
  l.SetTextSize(0.05);
  l.SetTextFont(132);
  string s_lumi = "#scale[0.6]{#int} #it{L dt} = "+to_string(int(g_Lumi))+" fb^{-1}";
  l.DrawLatex(0.43,0.79,s_lumi.c_str());	

  
  TString file_name = "output/1D_stacked_plots/"+g_Xname+"_stacked_plot.root";
  TFile* file = new TFile(file_name,"RECREATE");
  file->cd();
  can->Write();

  // can->Close();

}



