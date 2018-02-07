#include "TROOT.h"
#include <TChain.h>
#include "heep.C"
#include <fstream>

// run an analysis based on SNT.C the analyysis script for the simc n-tuple
//
// this script is setup for the proposed commissioning runs. the steps in pm are 0.15 GeV/c

void analyze()
{



  gROOT->Reset();

  //Create a TChain
  TChain chain("SNT");

  //Create an instance of the heep class
  heep* simc = new heep(&chain);

  TString electron_arm;


  //Chain each file

  TString simc_file;
  string report_file;

  //Coin Run 1929
  electron_arm = "HMS";
  simc_file = "ep_coin_simc_1929.root";
  cout << "Analyzing: " << simc_file << endl;
  chain.Add("../worksim/"+simc_file);
  simc->Init(&chain);
  simc->Loop(simc_file, electron_arm);
  chain.Reset();

  /*
  //Coin Run 2279
  electron_arm = "SHMS";
  simc_file = "ep_coin_simc_1929.root";
  cout << "Analyzing: " << simc_file << endl;
  chain.Add("../worksim/"+simc_file);
  simc->Init(&chain);
  simc->Loop(simc_file, electron_arm);
  chain.Reset();
  */

}
