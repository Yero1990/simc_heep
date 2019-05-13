void getYield()
{

 
  gROOT->SetBatch(kTRUE);                                                                                                                     
  gStyle->SetOptStat(1001111);                                                                                       


  ofstream ofile;
  ofile.open("yield.data");

  ofile <<"#! Run[i,0]/   W_cut_range[s,1]/    dataY[f,2]/     dataY_err[f,3]/     simcY[f,4]/     simcY_err[f,5]/" << endl;
  
  double x_min[6] = {0.85,  0.85,  0.85,  0.85, 0.85, 0.85};
  double x_max[6] = {1.05,  1.0,  0.99,  0.98, 0.97, 0.96};

  
  int bin_min;
  int bin_max;


  //double xmin[4] = {0.8,  0.8, 0.8, 0.8};
  //double xmax[4] = {1.1, 1.1, 1.1, 1.1};

  Double_t simcY[6], simcY_err[6];
  Double_t dataY[6], dataY_err[6];
    
  //Create Histogram Objects  to store Histograms from ROOTfiles
  TH1F *dataW[6] = {0};  //4 histograms.  one for each Em setting
  TH1F *simcW[6] = {0}; 


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

  

  //Change to simc file
  simc_file->cd();
  simc_file->GetObject("W_cut_r1", simcW[0]);
  simc_file->GetObject("W_cut_r2", simcW[1]);
  simc_file->GetObject("W_cut_r3", simcW[2]);
  simc_file->GetObject("W_cut_r4", simcW[3]);
  simc_file->GetObject("W_cut_r5", simcW[4]);
  simc_file->GetObject("W_cut_r6", simcW[5]);


  simcY[0] = simcW[0]->IntegralAndError(simcW[0]->FindBin(x_min[0]),simcW[0]->FindBin(x_max[0]), simcY_err[0]);
  simcY[1] = simcW[1]->IntegralAndError(simcW[1]->FindBin(x_min[1]),simcW[1]->FindBin(x_max[1]), simcY_err[1]);
  simcY[2] = simcW[2]->IntegralAndError(simcW[2]->FindBin(x_min[2]),simcW[2]->FindBin(x_max[2]), simcY_err[2]);
  simcY[3] = simcW[3]->IntegralAndError(simcW[3]->FindBin(x_min[3]),simcW[3]->FindBin(x_max[3]), simcY_err[3]);
  simcY[4] = simcW[4]->IntegralAndError(simcW[4]->FindBin(x_min[4]),simcW[4]->FindBin(x_max[4]), simcY_err[4]);
  simcY[5] = simcW[5]->IntegralAndError(simcW[5]->FindBin(x_min[5]),simcW[5]->FindBin(x_max[5]), simcY_err[5]);


  
  //Change to data file                                                                                                                   
  data_file->cd();
  data_file->GetObject("W_cut_r1", dataW[0]);
  data_file->GetObject("W_cut_r2", dataW[1]);
  data_file->GetObject("W_cut_r3", dataW[2]);
  data_file->GetObject("W_cut_r4", dataW[3]);
  data_file->GetObject("W_cut_r5", dataW[4]);
  data_file->GetObject("W_cut_r6", dataW[5]);


  dataY[0] = dataW[0]->IntegralAndError(dataW[0]->FindBin(x_min[0]),dataW[0]->FindBin(x_max[0]), dataY_err[0]);
  dataY[1] = dataW[1]->IntegralAndError(dataW[1]->FindBin(x_min[1]),dataW[1]->FindBin(x_max[1]), dataY_err[1]);
  dataY[2] = dataW[2]->IntegralAndError(dataW[2]->FindBin(x_min[2]),dataW[2]->FindBin(x_max[2]), dataY_err[2]);
  dataY[3] = dataW[3]->IntegralAndError(dataW[3]->FindBin(x_min[3]),dataW[3]->FindBin(x_max[3]), dataY_err[3]);
  dataY[4] = dataW[4]->IntegralAndError(dataW[4]->FindBin(x_min[4]),dataW[4]->FindBin(x_max[4]), dataY_err[4]);
  dataY[5] = dataW[5]->IntegralAndError(dataW[5]->FindBin(x_min[5]),dataW[5]->FindBin(x_max[5]), dataY_err[5]);

  
  ofile << Form("%i   %f_%f    %f    %f    %f    %f", run[irun],  x_min[0],  x_max[0],   dataY[0],  dataY_err[0],  simcY[0],   simcY_err[0])  << endl;
  ofile << Form("%i   %f_%f    %f    %f    %f    %f", run[irun],  x_min[1],  x_max[1],   dataY[1],  dataY_err[1],  simcY[1],   simcY_err[1])  << endl;
  ofile << Form("%i   %f_%f    %f    %f    %f    %f", run[irun],  x_min[2],  x_max[2],   dataY[2],  dataY_err[2],  simcY[2],   simcY_err[2])  << endl;
  ofile << Form("%i   %f_%f    %f    %f    %f    %f", run[irun],  x_min[3],  x_max[3],   dataY[3],  dataY_err[3],  simcY[3],   simcY_err[3])  << endl;
  ofile << Form("%i   %f_%f    %f    %f    %f    %f", run[irun],  x_min[4],  x_max[4],   dataY[4],  dataY_err[4],  simcY[4],   simcY_err[4])  << endl;
  ofile << Form("%i   %f_%f    %f    %f    %f    %f", run[irun],  x_min[5],  x_max[5],   dataY[5],  dataY_err[5],  simcY[5],   simcY_err[5])  << endl;

  
  } //end run loop
  
  


}
