#include "TROOT.h"
#include <TChain.h>
#include "heep.C"
#include <fstream>


//Define prototype function to get integrated bcm charge
Double_t getCharge(string spec, string bcm, TString filename);



// run an analysis based on SNT.C the analyysis script for the simc n-tuple
// this script is setup for the proposed commissioning runs. the steps in pm are 0.15 GeV/c

void analyze()
{


  gROOT->Reset();

  //Create a TChain
  TChain chain("SNT");

  //Create an instance of the heep class
  heep* simc = new heep(&chain);

  
  TString electron_arm;
  Double_t Ib;       //beam current in uA
  Double_t time;     //TIME in hrs
  Double_t charge;
  Double_t Q1, Q2;
  Int_t runNUM, evtNUM;
  //Chain each file

  TString simc_file;
  TString data_file;

  /*
  //--Estimate Spectrometer Resolution
  Ib = 40.;
  time = 1.;
  simc_file = "heep_simc_rad.root";
  cout << "Analyzing: " << simc_file << endl;
  chain.Add("../worksim/"+simc_file);
  simc->Init(&chain);
  simc->Loop(simc_file, Ib, time);
  chain.Reset();
  */
  
  //E12-10-003 H(e,e'p) Check!

  //------Get TRUE accumulated charge from actual data to use in SIMC--------
  runNUM = 2283;
  evtNUM = -1;
  data_file = Form("../ROOTfiles/coin_replay_production_%d_%d.root", runNUM, evtNUM);
  Q1 = getCharge("SHMS", "BCM4A", data_file);
  Q2 = getCharge("SHMS", "BCM4B", data_file);
  charge = (Q1 + Q2)/2.;
  //-------------------------------------------------------------------------
    
  simc_file = "heep_simc_rad.root";
  cout << "Analyzing: " << simc_file << endl;
  chain.Add("../worksim/"+simc_file);
  simc->Init(&chain);
  simc->Loop(simc_file, 1, 1, charge);
  chain.Reset();
  
  /*
  
  //Coin Run 1929
  //electron_arm = "HMS";
  Ib = 40.;
  time = 1.;
  simc_file = "ep_coin_simc_1929.root";
  cout << "Analyzing: " << simc_file << endl;
  chain.Add("../worksim/"+simc_file);
  simc->Init(&chain);
  simc->Loop(simc_file, Ib, time);
  chain.Reset();

  
  
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


Double_t getCharge(string spec, string bcm, TString filename)
{
   /*Brief: Get the accumulated charge if beam current was above threhsold (typically > 5 uA)
   */

  /*PARAMETERS: 
    spec --> "HMS" or "SHMS"
    bcm --> "BCM1", "BCM2", "BCM4A", "BCM4B", "BCM17"
    filename --> "/path/to/ROOTfile/file.root"
  */
  
  TFile *data_file = new TFile(filename, "READ");
  Double_t charge;    //in uC

  if (spec=="HMS")
    {

      TTree *TSH = (TTree*)data_file->Get("TSH");		
      
      if (bcm=="BCM1") { charge = TSH->GetMaximum("H.BCM1.scalerChargeCut"); }
      else if (bcm=="BCM2") { charge = TSH->GetMaximum("H.BCM2.scalerChargeCut"); }                                     
      else if (bcm=="BCM4A") { charge = TSH->GetMaximum("H.BCM4A.scalerChargeCut"); }                                       
      else if (bcm=="BCM4B") { charge = TSH->GetMaximum("H.BCM4B.scalerChargeCut"); }    
      else if (bcm=="BCM17") { charge = TSH->GetMaximum("H.BCM17.scalerChargeCut"); }                        
      return charge;
      
    }
	  
  else if (spec=="SHMS")
    {
      
      TTree *TSP = (TTree*)data_file->Get("TSP");		
      
      if (bcm=="BCM1") { charge = TSP->GetMaximum("P.BCM1.scalerChargeCut"); }
      else if (bcm=="BCM2") { charge = TSP->GetMaximum("P.BCM2.scalerChargeCut"); }                                     
      else if (bcm=="BCM4A") { charge = TSP->GetMaximum("P.BCM4A.scalerChargeCut"); }                                       
      else if (bcm=="BCM4B") { charge = TSP->GetMaximum("P.BCM4B.scalerChargeCut"); }    
      else if (bcm=="BCM17") { charge = TSP->GetMaximum("P.BCM17.scalerChargeCut"); }                        
      return charge;
      
    }
}
