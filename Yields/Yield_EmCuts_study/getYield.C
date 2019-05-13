void getYield()
{

 
  gROOT->SetBatch(kTRUE);                                                                                                                     
  gStyle->SetOptStat(1001111);                                                                                       


  ofstream ofile;
  ofile.open("yield.data");

  ofile <<"#! Run[i,0]/   Em_cut[f,1]/    dataY[f,2]/     dataY_err[f,3]/     simcY[f,4]/     simcY_err[f,5]/" << endl;
  
  double em_cut[4] = {0.0,  0.03,  0.06,  0.09};
  
  
  int bin_min;
  int bin_max;

  double xmin[4] = {0.85,  0.85, 0.85, 0.85};
  double xmax[4] = {1.05, 1.05, 1.05, 1.05};

  //double xmin[4] = {0.8,  0.8, 0.8, 0.8};
  //double xmax[4] = {1.1, 1.1, 1.1, 1.1};

  Double_t simcY[4], simcY_err[4];
  Double_t dataY[4], dataY_err[4];
  
  int run[4] = {3288, 3371, 3374, 3377};

  //Pre-defined SIMC/data root file names containing histogram object to comapare                                                                     
  //  TString simc_filename =  Form("../data_analysis/POST_BPM_Calibrations/FINAL_CORRECTIONS/Wcheck_simc_histos_%d.root", run);                                                                                                                                                                                                                                                                                   
  //TString data_filename = Form("../data_analysis/POST_BPM_Calibrations/FINAL_CORRECTIONS/Wcheck_data_histos_%d.root",run);     


  //Loop over each Em setting for each run
  for (int irun = 0; irun <4; irun++){
   
    //TString simc_filename =  Form("../simc_analysis/D2_heep/Wcheck_simc_histos_%d.root", run[irun]);                                              
    //TString data_filename = Form("../data_analysis/Wcheck_data_histos_%d.root",run[irun)
    TString simc_filename =  Form("../../simc_analysis/D2_heep/Wcheck_simc_histos_%d.root", run[irun]);                             
    TString data_filename = Form("../../data_analysis/Wcheck_data_histos_%d.root",run[irun]);     
  
   
                                                                                                                        
  //Open SIMC/data ROOT files;                                                                                                                        
  TFile *simc_file = new TFile(simc_filename);                                                                                        
  TFile *data_file = new TFile(data_filename);           

  
  //Create Histogram Objects  to store Histograms from ROOTfiles
  TH1F *dataW[4] = {0};  //4 histograms.  one for each Em setting
  TH1F *simcW[4] = {0}; 

  TH1F *dataW_err[4] = {0};  //4 histograms.  one for each Em setting
  TH1F *simcW_err[4] = {0}; 



  //Change to simc file
  simc_file->cd();
  simc_file->GetObject("W_NOemcut", simcW[0]);
  simc_file->GetObject("W_NOemcut_err", simcW_err[0]);
  simc_file->GetObject("W_emcut_30", simcW[1]);
  simc_file->GetObject("W_emcut_30_err", simcW_err[1]);
  simc_file->GetObject("W_emcut_60", simcW[2]);
  simc_file->GetObject("W_emcut_60_err", simcW_err[2]);
  simc_file->GetObject("W_emcut_90", simcW[3]);
  simc_file->GetObject("W_emcut_90_err", simcW_err[3]);


  bin_min = simcW[0]->GetXaxis()->FindBin(xmin[irun]);                                                                                                       
  bin_max = simcW[0]->GetXaxis()->FindBin(xmax[irun]);

  simcY[0] = simcW[0]->Integral(bin_min, bin_max);
  simcY_err[0] = sqrt( simcW_err[0]->Integral(bin_min, bin_max) );
  simcY[1] = simcW[1]->Integral(bin_min, bin_max);
  simcY_err[1] = sqrt( simcW_err[1]->Integral(bin_min, bin_max) );
  simcY[2] = simcW[2]->Integral(bin_min, bin_max);
  simcY_err[2] = sqrt( simcW_err[2]->Integral(bin_min, bin_max) );
  simcY[3] = simcW[3]->Integral(bin_min, bin_max);
  simcY_err[3] = sqrt( simcW_err[3]->Integral(bin_min, bin_max) );
  

  //Change to data file                                                                                                                   
  data_file->cd();                                                                                                                         
  data_file->GetObject("W_NOemcut", dataW[0]);
  data_file->GetObject("W_NOemcut_err", dataW_err[0]);
  data_file->GetObject("W_emcut_30", dataW[1]);
  data_file->GetObject("W_emcut_30_err", dataW_err[1]);
  data_file->GetObject("W_emcut_60", dataW[2]);
  data_file->GetObject("W_emcut_60_err", dataW_err[2]);
  data_file->GetObject("W_emcut_90", dataW[3]);
  data_file->GetObject("W_emcut_90_err", dataW_err[3]);

  bin_min = dataW[0]->GetXaxis()->FindBin(xmin[irun]);         
  bin_max = dataW[0]->GetXaxis()->FindBin(xmax[irun]);   

  dataY[0] = dataW[0]->Integral(bin_min, bin_max);
  dataY_err[0] = sqrt( dataW_err[0]->Integral(bin_min, bin_max) );
  dataY[1] = dataW[1]->Integral(bin_min, bin_max);
  dataY_err[1] = sqrt( dataW_err[1]->Integral(bin_min, bin_max) );
  dataY[2] = dataW[2]->Integral(bin_min, bin_max);
  dataY_err[2] = sqrt( dataW_err[2]->Integral(bin_min, bin_max) );
  dataY[3] = dataW[3]->Integral(bin_min, bin_max);
  dataY_err[3] = sqrt( dataW_err[3]->Integral(bin_min, bin_max) );
  

  ofile << Form("%i   %f    %f    %f    %f    %f", run[irun],  em_cut[0],   dataY[0],  dataY_err[0],  simcY[0],   simcY_err[0])  << endl;
  ofile << Form("%i   %f    %f    %f    %f    %f", run[irun],  em_cut[1],   dataY[1],  dataY_err[1],  simcY[1],   simcY_err[1])  << endl;
  ofile << Form("%i   %f    %f    %f    %f    %f", run[irun],  em_cut[2],   dataY[2],  dataY_err[2],  simcY[2],   simcY_err[2])  << endl;
  ofile << Form("%i   %f    %f    %f    %f    %f", run[irun],  em_cut[3],   dataY[3],  dataY_err[3],  simcY[3],   simcY_err[3])  << endl;

    } //end run loop
  
  


}
