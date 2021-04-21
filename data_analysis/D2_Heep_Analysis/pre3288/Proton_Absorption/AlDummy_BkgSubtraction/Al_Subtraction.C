void Al_Subtraction()
{
  //gROOT->SetBatch(kTRUE);  
  gStyle->SetOptStat(1001111);
  //TString simc_filename =  "weighted_ep_coin_simc_1854.root"; //"ep_coin_simc_1929.root";
  
  TString dummy_filename =  "dummy_3254.root";
                      
    
 
  TString LH2_filename = "LH2_3259.root"; 

  
  //Open SIMC/data ROOT files;
  TFile *dummy_file = new TFile(dummy_filename);
  TFile *LH2_file = new TFile(LH2_filename);

  //Histograms

  TH1F *dummy_Wdid = 0;
  TH1F *dummy_Wshould = 0;

  TH1F *LH2_Wdid = 0;
  TH1F *LH2_Wshould = 0;

  dummy_file->cd();
  dummy_file->GetObject("cut_W_inv", dummy_Wshould);
  dummy_file->GetObject("W_did", dummy_Wdid);

  LH2_file->cd();
  LH2_file->GetObject("cut_W_inv", LH2_Wshould);
  LH2_file->GetObject("W_did", LH2_Wdid);

  //Subtract Dummy
  LH2_Wshould->Add(dummy_Wshould, -1);
  LH2_Wdid->Add(dummy_Wdid, -1);

  TCanvas *c1 = new TCanvas("c1", "", 1000,1000);
  c1->Divide(1,2);
  c1->cd(1);
  LH2_Wshould->Draw();
  c1->cd(2);
  LH2_Wdid->Draw();


}
