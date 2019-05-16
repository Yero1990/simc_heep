void getYield()
{

 
  gROOT->SetBatch(kTRUE);                                                                                                                     
  gStyle->SetOptStat(1001111);                                                                                       


  ofstream ofile;
  ofile.open("yield.data");

  ofile <<"#! Run[i,0]/    dataY[f,1]/     dataY_err[f,2]/     simcY[f,3]/     simcY_err[f,4]/" << endl;
  
  
  int bin_min;
  int bin_max;

  double xmin[4] = {0.85,  0.85, 0.85, 0.85};
  double xmax[4] = {1.05, 1.05, 1.05, 1.05};

  //double xmin[4] = {0.8,  0.8, 0.8, 0.8};
  //double xmax[4] = {1.1, 1.1, 1.1, 1.1};

  Double_t simcY, simcY_err;
  Double_t dataY, dataY_err;
  
  int run[4] = {3288, 3371, 3374, 3377};

  //Pre-defined SIMC/data root file names containing histogram object to comapare                                                                     
  //  TString simc_filename =  Form("../data_analysis/POST_BPM_Calibrations/FINAL_CORRECTIONS/Wcheck_simc_histos_%d.root", run);                                                                                                                                                                                                                                                                                   
  //TString data_filename = Form("../data_analysis/POST_BPM_Calibrations/FINAL_CORRECTIONS/Wcheck_data_histos_%d.root",run);     


  TCanvas *c0 = new TCanvas("c0", "", 1500, 1500);
  c0->Divide(2,2);

  TCanvas *Em_canv = new TCanvas("Em_canv", "", 1500, 1500);
  Em_canv->Divide(2,2);
  TCanvas *hdelta_canv = new TCanvas("hdelta_canv", "", 1500, 1500);
  hdelta_canv->Divide(2,2);
  TCanvas *edelta_canv = new TCanvas("edelta_canv", "", 1500, 1500);
  edelta_canv->Divide(2,2);

  for (int irun = 0; irun <4; irun++){
    //TString simc_filename =  Form("../simc_analysis/D2_heep/Wcheck_simc_histos_%d.root", run[irun]);                                                                                                                                                             
    //TString data_filename = Form("../data_analysis/Wcheck_data_histos_%d.root",run[irun]);     
   
    TString simc_filename =  Form("../simc_analysis/D2_heep/Wcheck_simc_histos_%d.root", run[irun]);                                                                                                                                                             
    TString data_filename = Form("../data_analysis/Wcheck_data_histos_%d.root",run[irun]);     
  
   
                                                                                                                        
  //Open SIMC/data ROOT files;                                                                                                                        
  TFile *simc_file = new TFile(simc_filename);                                                                                        
  TFile *data_file = new TFile(data_filename);           
 
  //Create Histogram Objects  to store Histograms from ROOTfiles
  TH1F *dataW = 0;
  TH1F *simcW = 0; 

  TH1F *dataW_unweight = 0;
  TH1F *simcW_unweight = 0; 

  TH1F *dataEm = 0;
  TH1F *simcEm = 0;

  TH1F *data_hdelta = 0;
  TH1F *simc_hdelta = 0;
  
  TH1F *data_edelta = 0;
  TH1F *simc_edelta = 0;  


  //Change to simc file
  simc_file->cd();
  simc_file->GetObject("cut_W_inv", simcW);
  simc_file->GetObject("cut_W_inv_UnWeight", simcW_unweight);

  //simc_file->GetObject("W_inv", simcW);

  simc_file->GetObject("Emiss", simcEm);
  simc_file->GetObject("hdelta", simc_hdelta);
  simc_file->GetObject("edelta", simc_edelta);

  bin_min = simcW->GetXaxis()->FindBin(xmin[irun]);                                                                                                       
  bin_max = simcW->GetXaxis()->FindBin(xmax[irun]);

  simcY = simcW->IntegralAndError(bin_min, bin_max, simcY_err);
  //simcY_err = sqrt( simcW_unweight->Integral(bin_min, bin_max) );
  

  cout << "SIMC Y = " << simcY << endl;
  cout << "SIMC Y_err = " << simcY_err << endl;


  //Change to data file                                                                                                                   
  data_file->cd();                                                                                                                         
  data_file->GetObject("cut_W_inv", dataW);  
  data_file->GetObject("cut_W_inv_UnWeight", dataW_unweight);

  data_file->GetObject("Emiss", dataEm);
  data_file->GetObject("hdelta", data_hdelta);
  data_file->GetObject("edelta", data_edelta);

  bin_min = dataW->GetXaxis()->FindBin(xmin[irun]);         
  bin_max = dataW->GetXaxis()->FindBin(xmax[irun]);   

  dataY = dataW->IntegralAndError(bin_min, bin_max, dataY_err);
  //dataY_err = sqrt( dataW_unweight->Integral(bin_min, bin_max) );

  
  cout << "DATA Y = " << dataY << endl;
  cout << "DATA Y_err = " << dataY_err << endl;



  ofile << Form("%i   %f    %f    %f    %f", run[irun],  dataY,  dataY_err,  simcY,   simcY_err)  << endl;



  //Draw to Canvas
  c0->cd(irun+1);
  simcW->Draw();
  dataW->Draw("sames");
  
  Em_canv->cd(irun+1);
  simcEm->Draw();
  dataEm->Draw("sames");
  
  hdelta_canv->cd(irun+1);
  simc_hdelta->Draw();
  data_hdelta->Draw("sames");
  
  edelta_canv->cd(irun+1);
  simc_edelta->Draw();
  data_edelta->Draw("sames");
  

    } //end run loop
  
  


}
