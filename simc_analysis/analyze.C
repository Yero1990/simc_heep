#include "TROOT.h"
#include <TChain.h>
#include "heep.h"
#include <fstream>

// run an analysis based on SNT.C the analyysis script for the simc n-tuple
//
// this script is setup for the proposed commissioning runs. the steps in pm are 0.15 GeV/c

void analyze()
{


  //TFile *f = new TFile("weighted_simc_ep_coin_simc_1929.root");

  //TTree *SNT = (TTree*)f->Get("SNT");
      
  gROOT->Reset();

  // gROOT->ProcessLine("heep.C");

  gROOT->LoadMacro("heep.C+");


  TChain *chain = new TChain("SNT");

  heep* simc = new heep(chain);
  

  //chain->Add("ep_coin_Eb6.4_earm_27.5_rad.root");

  /*

  cout << chain << endl;
  gROOT->ProcessLine("heep simc(chain)");

  
  
  //heep simc;
  //simc.Loop("ep_coin_Eb6.4_earm_27.5_rad.root");
  */
  // create a chain of output files
  /*
  TString datadir = "../worksim/";
  TString filename = "ep_coin_Eb6.4_earm_27.5_rad.root";
  // from *.hist file
  TChain *chain = new TChain("SNT");
  
  TString file = datadir + filename;
  cout << "Analyzing : " << filename  << endl;
  
  chain->Add(file);
  
  heep *simc = new heep(chain);
  simc->Loop(filename); // do all e-p events
  chain->Reset();
  */
  /*
  type = type_offset + 65;
  pm = "pm_0.65";
  file = datadir + pm + Q2 + xbj + einc + calc + rad + ext;
  cout << "Analyzing : " << file << endl;
  chain->Add(file);
  simc->Init(&chain);
  simc->Loop(qmin, qmax, type, res_head); // do all e-p events
  chain->Reset();

  type = type_offset + 80;
  pm = "pm_0.80";
  file = datadir + pm + Q2 + xbj + einc + calc + rad + ext;
  cout << "Analyzing : " << file << endl;
  chain->Add(file);
  simc->Init(&chain);
  simc->Loop(qmin, qmax, type, res_head); // do all e-p events
  chain->Reset();

  */




}
