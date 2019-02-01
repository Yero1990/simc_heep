#include "../simc_analysis/D2_heep/set_heep_histos.h"

void getYield_v2()
{

 
  gROOT->SetBatch(kTRUE);                                                                                                                     
  gStyle->SetOptStat(1001111);                                                                                       

  //Loop over elastic runs
  for (int irun = 0; irun <4; irun++)
    {
      TString simc_filename =  Form("../simc_analysis/D2_heep/Wcheck_simc_histos_%d.root", run[irun]);                                                                                                                                                             
      TString data_filename = Form("../data_analysis/Wcheck_data_histos_%d.root",run[irun]);     
      
                                                                                                                        
     



    } //end run loop
  



  /*
  
  ofstream ofile;
  ofile.open("yield.data");
  ofile <<"#! Run[i,0]/    dataY[f,1]/     dataY_err[f,2]/     simcY[f,3]/     simcY_err[f,4]/" << endl;
  
  
  int bin_min;
  int bin_max;

  double xmin[4] = {0.85,  0.85, 0.85, 0.85};
  double xmax[4] = {1.05, 1.05, 1.05, 1.05};

  Double_t simcY, simcY_err;
  Double_t dataY, dataY_err;
  
  int run[4] = {3288, 3371, 3374, 3377};



  TCanvas *c0 = new TCanvas("c0", "", 1500, 1500);
  c0->Divide(2,2);

  TCanvas *Em_canv = new TCanvas("Em_canv", "", 1500, 1500);
  Em_canv->Divide(2,2);
  TCanvas *hdelta_canv = new TCanvas("hdelta_canv", "", 1500, 1500);
  hdelta_canv->Divide(2,2);
  TCanvas *edelta_canv = new TCanvas("edelta_canv", "", 1500, 1500);
  edelta_canv->Divide(2,2);

  */

}
