void getYield()
{

 
  // gROOT->SetBatch(kTRUE);                                                                                                                     
  gStyle->SetOptStat(1001111);                                                                                       

 
  int run[4] = {3288, 3371, 3374, 3377};
                                                                                                                                                                                                                                          
  //TString data_filename = Form("../data_analysis/POST_BPM_Calibrations/FINAL_CORRECTIONS/Wcheck_data_histos_%d.root",run);     

  //SIMC original (no offsets in angle)
  TH1F *simcY_exptar[4] = {0}; 
  TH1F *simcY_eyptar[4] = {0}; 
  TH1F *simcY_hxptar[4] = {0};
  TH1F *simcY_hyptar[4] = {0};
  TH1F *simcY_edelta[4] = {0};
  TH1F *simcY_hdelta[4] = {0};
  
  //SIMC original (no offsets in angle, NO Coll Cuts)
  TH1F *simcY_exptar_noColl[4] = {0}; 
  TH1F *simcY_eyptar_noColl[4] = {0}; 
  TH1F *simcY_hxptar_noColl[4] = {0};
  TH1F *simcY_hyptar_noColl[4] = {0};
  TH1F *simcY_edelta_noColl[4] = {0};
  TH1F *simcY_hdelta_noColl[4] = {0};
  

  //SIMC SHMS +1 mr
  TH1F *simcY_exptar_sp1mr[4] = {0}; 
  TH1F *simcY_eyptar_sp1mr[4] = {0}; 
  TH1F *simcY_hxptar_sp1mr[4] = {0};
  TH1F *simcY_hyptar_sp1mr[4] = {0};
  TH1F *simcY_edelta_sp1mr[4] = {0};
  TH1F *simcY_hdelta_sp1mr[4] = {0};

  //SIMC SHMS -1 mr
  TH1F *simcY_exptar_sm1mr[4] = {0}; 
  TH1F *simcY_eyptar_sm1mr[4] = {0}; 
  TH1F *simcY_hxptar_sm1mr[4] = {0};
  TH1F *simcY_hyptar_sm1mr[4] = {0};
  TH1F *simcY_edelta_sm1mr[4] = {0};
  TH1F *simcY_hdelta_sm1mr[4] = {0};

  //SIMC HMS +1 mr
  TH1F *simcY_exptar_hp1mr[4] = {0}; 
  TH1F *simcY_eyptar_hp1mr[4] = {0}; 
  TH1F *simcY_hxptar_hp1mr[4] = {0};
  TH1F *simcY_hyptar_hp1mr[4] = {0};
  TH1F *simcY_edelta_hp1mr[4] = {0};
  TH1F *simcY_hdelta_hp1mr[4] = {0};

  //SIMC HMS -1 mr
  TH1F *simcY_exptar_hm1mr[4] = {0}; 
  TH1F *simcY_eyptar_hm1mr[4] = {0}; 
  TH1F *simcY_hxptar_hm1mr[4] = {0};
  TH1F *simcY_hyptar_hm1mr[4] = {0};
  TH1F *simcY_edelta_hm1mr[4] = {0};
  TH1F *simcY_hdelta_hm1mr[4] = {0};

  //Data Yield
  TH1F *dataY_exptar[4] = {0};
  TH1F *dataY_eyptar[4] = {0};
  TH1F *dataY_hxptar[4] = {0};
  TH1F *dataY_hyptar[4] = {0};
  TH1F *dataY_edelta[4] = {0};
  TH1F *dataY_hdelta[4] = {0};

  //Data Yield (NO Coll. Cuts)
  TH1F *dataY_exptar_noColl[4] = {0};
  TH1F *dataY_eyptar_noColl[4] = {0};
  TH1F *dataY_hxptar_noColl[4] = {0};
  TH1F *dataY_hyptar_noColl[4] = {0};
  TH1F *dataY_edelta_noColl[4] = {0};
  TH1F *dataY_hdelta_noColl[4] = {0};

  //Loop over each Em setting for each run
  for (int irun = 0; irun <1; irun++){
   
    //TString simc_filename =  Form("../simc_analysis/D2_heep/Wcheck_simc_histos_%d.root", run[irun]);                                              
    //TString data_filename = Form("../data_analysis/Wcheck_data_histos_%d.root",run[irun)

    TString simc_filename_sm1mr =  Form("../../simc_analysis/D2_heep/Wcheck_simc_histos_%d_-1mrshms_CollCut.root", run[irun]);                                  
    TString simc_filename_sp1mr =  Form("../../simc_analysis/D2_heep/Wcheck_simc_histos_%d_+1mrshms_CollCut.root", run[irun]);                                  
    
    TString simc_filename_hm1mr =  Form("../../simc_analysis/D2_heep/Wcheck_simc_histos_%d_-1mrhms_CollCut.root", run[irun]);                                  
    TString simc_filename_hp1mr =  Form("../../simc_analysis/D2_heep/Wcheck_simc_histos_%d_+1mrhms_CollCut.root", run[irun]);                                  


    TString simc_filename =  Form("../../simc_analysis/D2_heep/Wcheck_simc_histos_%d_CollCut.root", run[irun]);                             
    TString data_filename = Form("../../data_analysis/Wcheck_data_histos_%d_CollCut.root",run[irun]);     
  
    TString simc_filename_noColl =  Form("../../simc_analysis/D2_heep/Wcheck_simc_histos_%d.root", run[irun]);                             
    TString data_filename_noColl = Form("../../data_analysis/Wcheck_data_histos_%d.root",run[irun]);     
  
                                                                                                                        
  //Open SIMC/data ROOT files;                                                                                                                        
  TFile *simc_file = new TFile(simc_filename);  
  TFile *simc_file_noColl = new TFile(simc_filename_noColl);                                                                                        

  TFile *simc_file_sp1mr = new TFile(simc_filename_sp1mr);
  TFile *simc_file_sm1mr = new TFile(simc_filename_sm1mr);
  TFile *simc_file_hp1mr = new TFile(simc_filename_hp1mr);
  TFile *simc_file_hm1mr = new TFile(simc_filename_hm1mr);

  TFile *data_file = new TFile(data_filename);           
  TFile *data_file_noColl = new TFile(data_filename_noColl);           


  //Change to simc file
  simc_file->cd();
  simc_file->GetObject("cut_exptar", simcY_exptar[irun]);
  simc_file->GetObject("cut_eyptar", simcY_eyptar[irun]); 
  simc_file->GetObject("cut_edelta", simcY_edelta[irun]); 
  simc_file->GetObject("cut_hxptar", simcY_hxptar[irun]);                                                                          
  simc_file->GetObject("cut_hyptar", simcY_hyptar[irun]);                                                                           
  simc_file->GetObject("cut_hdelta", simcY_hdelta[irun]); 

  simc_file_noColl->cd();
  simc_file_noColl->GetObject("cut_exptar", simcY_exptar_noColl[irun]);
  simc_file_noColl->GetObject("cut_eyptar", simcY_eyptar_noColl[irun]); 
  simc_file_noColl->GetObject("cut_edelta", simcY_edelta_noColl[irun]); 
  simc_file_noColl->GetObject("cut_hxptar", simcY_hxptar_noColl[irun]);                                                                          
  simc_file_noColl->GetObject("cut_hyptar", simcY_hyptar_noColl[irun]);                                                                           
  simc_file_noColl->GetObject("cut_hdelta", simcY_hdelta_noColl[irun]); 

  

  //Change to simc file SHMS +1mr
  simc_file_sp1mr->cd();
  simc_file_sp1mr->GetObject("cut_exptar", simcY_exptar_sp1mr[irun]);
  simc_file_sp1mr->GetObject("cut_eyptar", simcY_eyptar_sp1mr[irun]); 
  simc_file_sp1mr->GetObject("cut_edelta", simcY_edelta_sp1mr[irun]); 
  simc_file_sp1mr->GetObject("cut_hxptar", simcY_hxptar_sp1mr[irun]);                                                                          
  simc_file_sp1mr->GetObject("cut_hyptar", simcY_hyptar_sp1mr[irun]);                                                                           
  simc_file_sp1mr->GetObject("cut_hdelta", simcY_hdelta_sp1mr[irun]);
  
  //Change to simc file SHMS -1mr
  simc_file_sm1mr->cd();
  simc_file_sm1mr->GetObject("cut_exptar", simcY_exptar_sm1mr[irun]);
  simc_file_sm1mr->GetObject("cut_eyptar", simcY_eyptar_sm1mr[irun]); 
  simc_file_sm1mr->GetObject("cut_edelta", simcY_edelta_sm1mr[irun]); 
  simc_file_sm1mr->GetObject("cut_hxptar", simcY_hxptar_sm1mr[irun]);                                                                          
  simc_file_sm1mr->GetObject("cut_hyptar", simcY_hyptar_sm1mr[irun]);                                                                           
  simc_file_sm1mr->GetObject("cut_hdelta", simcY_hdelta_sm1mr[irun]);


  //Change to simc file HMS +1mr
  simc_file_hp1mr->cd();
  simc_file_hp1mr->GetObject("cut_exptar", simcY_exptar_hp1mr[irun]);
  simc_file_hp1mr->GetObject("cut_eyptar", simcY_eyptar_hp1mr[irun]); 
  simc_file_hp1mr->GetObject("cut_edelta", simcY_edelta_hp1mr[irun]); 
  simc_file_hp1mr->GetObject("cut_hxptar", simcY_hxptar_hp1mr[irun]);                                                                          
  simc_file_hp1mr->GetObject("cut_hyptar", simcY_hyptar_hp1mr[irun]);                                                                           
  simc_file_hp1mr->GetObject("cut_hdelta", simcY_hdelta_hp1mr[irun]);
  
  //Change to simc file HMS -1mr
  simc_file_hm1mr->cd();
  simc_file_hm1mr->GetObject("cut_exptar", simcY_exptar_hm1mr[irun]);
  simc_file_hm1mr->GetObject("cut_eyptar", simcY_eyptar_hm1mr[irun]); 
  simc_file_hm1mr->GetObject("cut_edelta", simcY_edelta_hm1mr[irun]); 
  simc_file_hm1mr->GetObject("cut_hxptar", simcY_hxptar_hm1mr[irun]);                                                                          
  simc_file_hm1mr->GetObject("cut_hyptar", simcY_hyptar_hm1mr[irun]);                                                                           
  simc_file_hm1mr->GetObject("cut_hdelta", simcY_hdelta_hm1mr[irun]);

  //Change to data file                                                                                                                   
  data_file->cd();                                                                                                                         
  data_file->GetObject("cut_exptar", dataY_exptar[irun]);
  data_file->GetObject("cut_eyptar", dataY_eyptar[irun]); 
  data_file->GetObject("cut_edelta", dataY_edelta[irun]);
  data_file->GetObject("cut_hxptar", dataY_hxptar[irun]);                                                                    
  data_file->GetObject("cut_hyptar", dataY_hyptar[irun]);                                                                          
  data_file->GetObject("cut_hdelta", dataY_hdelta[irun]); 

  //Change to data file (NO Collimator)                                                                                                                   
  data_file_noColl->cd();                                                                                                                         
  data_file_noColl->GetObject("cut_exptar", dataY_exptar_noColl[irun]);
  data_file_noColl->GetObject("cut_eyptar", dataY_eyptar_noColl[irun]); 
  data_file_noColl->GetObject("cut_edelta", dataY_edelta_noColl[irun]);
  data_file_noColl->GetObject("cut_hxptar", dataY_hxptar_noColl[irun]);                                                                    
  data_file_noColl->GetObject("cut_hyptar", dataY_hyptar_noColl[irun]);                                                                          
  data_file_noColl->GetObject("cut_hdelta", dataY_hdelta_noColl[irun]); 


  } //end run loop

    //Clone Data  (SHMS +1mr)                                                                                                                    
  TH1F *dataY_exptar2 = (TH1F*)dataY_exptar[0]->Clone("data_exptar");                                                                                  
  TH1F *dataY_eyptar2 = (TH1F*)dataY_eyptar[0]->Clone("data_eyptar");                                                                                          
  TH1F *dataY_edelta2 = (TH1F*)dataY_edelta[0]->Clone("data_edelta");                                                                                       
  TH1F *dataY_hxptar2 = (TH1F*)dataY_hxptar[0]->Clone("data_hxptar");                                                                                          
  TH1F *dataY_hyptar2 = (TH1F*)dataY_hyptar[0]->Clone("data_hyptar");                                                                                          
  TH1F *dataY_hdelta2 = (TH1F*)dataY_hdelta[0]->Clone("data_hdelta");

  //Clone Data (SHMS -1 mr)                                                                                                                     
  TH1F *dataY_exptar3 = (TH1F*)dataY_exptar[0]->Clone("data_exptar");                                                                                  
  TH1F *dataY_eyptar3 = (TH1F*)dataY_eyptar[0]->Clone("data_eyptar");                                                                                          
  TH1F *dataY_edelta3 = (TH1F*)dataY_edelta[0]->Clone("data_edelta");                                                                                       
  TH1F *dataY_hxptar3 = (TH1F*)dataY_hxptar[0]->Clone("data_hxptar");                                                                                          
  TH1F *dataY_hyptar3 = (TH1F*)dataY_hyptar[0]->Clone("data_hyptar");                                                                                          
  TH1F *dataY_hdelta3 = (TH1F*)dataY_hdelta[0]->Clone("data_hdelta");
  
    //Clone Data  (HMS +1mr)                                                                                                                    
  TH1F *dataY_exptar4 = (TH1F*)dataY_exptar[0]->Clone("data_exptar");                                                                                  
  TH1F *dataY_eyptar4 = (TH1F*)dataY_eyptar[0]->Clone("data_eyptar");                                                                                          
  TH1F *dataY_edelta4 = (TH1F*)dataY_edelta[0]->Clone("data_edelta");                                                                                       
  TH1F *dataY_hxptar4 = (TH1F*)dataY_hxptar[0]->Clone("data_hxptar");                                                                                          
  TH1F *dataY_hyptar4 = (TH1F*)dataY_hyptar[0]->Clone("data_hyptar");                                                                                          
  TH1F *dataY_hdelta4 = (TH1F*)dataY_hdelta[0]->Clone("data_hdelta");

  //Clone Data (HMS -1 mr)                                                                                                                     
  TH1F *dataY_exptar5 = (TH1F*)dataY_exptar[0]->Clone("data_exptar");                                                                                  
  TH1F *dataY_eyptar5 = (TH1F*)dataY_eyptar[0]->Clone("data_eyptar");                                                                                          
  TH1F *dataY_edelta5 = (TH1F*)dataY_edelta[0]->Clone("data_edelta");                                                                                       
  TH1F *dataY_hxptar5 = (TH1F*)dataY_hxptar[0]->Clone("data_hxptar");                                                                                          
  TH1F *dataY_hyptar5 = (TH1F*)dataY_hyptar[0]->Clone("data_hyptar");                                                                                          
  TH1F *dataY_hdelta5 = (TH1F*)dataY_hdelta[0]->Clone("data_hdelta");

  

  TCanvas *c1 = new TCanvas("c1", "SHMS +/- 1 mr Variation ", 500,500);
  c1->Divide(3,2);
 
  c1->cd(1);
  dataY_exptar[0]->Divide(simcY_exptar[0]);
  dataY_exptar[0]->SetLineColor(kBlack);
  dataY_exptar[0]->Draw();
  dataY_exptar_noColl[0]->Divide(simcY_exptar_noColl[0]);                                         
  dataY_exptar_noColl[0]->SetLineColor(kRed);
  dataY_exptar_noColl[0]->Draw("sames");
    
  c1->cd(2);
  dataY_eyptar[0]->Divide(simcY_eyptar[0]);
  dataY_eyptar[0]->SetLineColor(kBlack);
  dataY_eyptar[0]->Draw();
  dataY_eyptar_noColl[0]->Divide(simcY_eyptar_noColl[0]);                                         
  dataY_eyptar_noColl[0]->SetLineColor(kRed);
  dataY_eyptar_noColl[0]->Draw("sames");
  
  c1->cd(3);
  dataY_edelta[0]->Divide(simcY_edelta[0]);
  dataY_edelta[0]->SetLineColor(kBlack);
  dataY_edelta[0]->Draw();
  dataY_edelta_noColl[0]->Divide(simcY_edelta_noColl[0]);                                         
  dataY_edelta_noColl[0]->SetLineColor(kRed);
  dataY_edelta_noColl[0]->Draw("sames");
 
  c1->cd(4);
  dataY_hxptar[0]->Divide(simcY_hxptar[0]);
  dataY_hxptar[0]->SetLineColor(kBlack);
  dataY_hxptar[0]->Draw();
  dataY_hxptar_noColl[0]->Divide(simcY_hxptar_noColl[0]);                                         
  dataY_hxptar_noColl[0]->SetLineColor(kRed);
  dataY_hxptar_noColl[0]->Draw("sames");
    
  c1->cd(5);
  dataY_hyptar[0]->Divide(simcY_hyptar[0]);
  dataY_hyptar[0]->SetLineColor(kBlack);
  dataY_hyptar[0]->Draw();
  dataY_hyptar_noColl[0]->Divide(simcY_hyptar_noColl[0]);                                         
  dataY_hyptar_noColl[0]->SetLineColor(kRed);
  dataY_hyptar_noColl[0]->Draw("sames");
  
  c1->cd(6);
  dataY_hdelta[0]->Divide(simcY_hdelta[0]);
  dataY_hdelta[0]->SetLineColor(kBlack);
  dataY_hdelta[0]->Draw();
  dataY_hdelta_noColl[0]->Divide(simcY_hdelta_noColl[0]);                                         
  dataY_hdelta_noColl[0]->SetLineColor(kRed);
  dataY_hdelta_noColl[0]->Draw("sames");



  /*
    // +/- 1 mr offsets in SIMC
  c1->cd(1);

  dataY_exptar[0]->Divide(simcY_exptar[0]);
  dataY_exptar[0]->SetLineColor(kBlack);
  dataY_exptar[0]->Draw();
  dataY_exptar2->Divide(simcY_exptar_sp1mr[0]);                                         
  dataY_exptar2->SetLineColor(kRed);
  dataY_exptar2->Draw("sames");
  dataY_exptar3->Divide(simcY_exptar_sm1mr[0]);                                         
  dataY_exptar3->SetLineColor(kBlue);
  dataY_exptar3->Draw("sames");


  c1->cd(2);                                                                                                                  
  dataY_eyptar[0]->Divide(simcY_eyptar[0]);                   
  dataY_eyptar[0]->SetLineColor(kBlack);                                                      
  dataY_eyptar[0]->Draw();
  dataY_eyptar2->Divide(simcY_eyptar_sp1mr[0]);                                                                                
  dataY_eyptar2->SetLineColor(kRed); 
  dataY_eyptar2->Draw("sames"); 
  dataY_eyptar3->Divide(simcY_eyptar_sm1mr[0]);                                                                                
  dataY_eyptar3->SetLineColor(kBlue); 
  dataY_eyptar3->Draw("sames"); 

  c1->cd(3);                                                                                                                        
  dataY_edelta[0]->Divide(simcY_edelta[0]);
  dataY_edelta[0]->SetLineColor(kBlack);                                                                                        
  dataY_edelta[0]->Draw();
  dataY_edelta2->Divide(simcY_edelta_sp1mr[0]);                                                                 
  dataY_edelta2->SetLineColor(kRed);
  dataY_edelta2->Draw("sames");  
  dataY_edelta3->Divide(simcY_edelta_sm1mr[0]);                                                                 
  dataY_edelta3->SetLineColor(kBlue);
  dataY_edelta3->Draw("sames");

  c1->cd(4);                                                                                                                        
  dataY_hxptar[0]->Divide(simcY_hxptar[0]);
  dataY_hxptar[0]->SetLineColor(kBlack);                                                                                 
  dataY_hxptar[0]->Draw();            
  dataY_hxptar2->Divide(simcY_hxptar_sp1mr[0]);                                                                                                    
  dataY_hxptar2->SetLineColor(kRed);
  dataY_hxptar2->Draw("sames");                                                                                                    
  dataY_hxptar3->Divide(simcY_hxptar_sm1mr[0]);                                                                                                    
  dataY_hxptar3->SetLineColor(kBlue);
  dataY_hxptar3->Draw("sames");
                                                                                                                                          
  c1->cd(5);                                                                                                                     
  dataY_hyptar[0]->Divide(simcY_hyptar[0]);                                                                                      
  dataY_hyptar[0]->SetLineColor(kBlack);
  dataY_hyptar[0]->Draw();            
  dataY_hyptar2->Divide(simcY_hyptar_sp1mr[0]);
  dataY_hyptar2->SetLineColor(kRed);                                                                   
  dataY_hyptar2->Draw("sames"); 
  dataY_hyptar3->Divide(simcY_hyptar_sm1mr[0]);
  dataY_hyptar3->SetLineColor(kBlue);                                                                   
  dataY_hyptar3->Draw("sames");
                                                                                             
                                                                                                                                   
  c1->cd(6);                                                                                                                         
  dataY_hdelta[0]->Divide(simcY_hdelta[0]);
  dataY_hdelta[0]->SetLineColor(kBlack);                                                                                            
  dataY_hdelta[0]->Draw();
  dataY_hdelta2->Divide(simcY_hdelta_sp1mr[0]);
  dataY_hdelta2->SetLineColor(kRed);                                                                                     
  dataY_hdelta2->Draw("sames"); 
  dataY_hdelta3->Divide(simcY_hdelta_sm1mr[0]);
  dataY_hdelta3->SetLineColor(kBlue);                                                                                     
  dataY_hdelta3->Draw("sames"); 


  */

}
