//Script to make comparison between SIMC and Commissioning Data from HallC Spring 2018
//Compare Target Reconstruction Variables Only!!

void make_comparison_targetrecon()
{
  

//  TString simc_filename =  "ep_coin_simc_2279_noshift.root"; //"ep_coin_simc_1929.root";
  TString simc_filename =  "ep_coin_simc_2279_shift.root"; //"ep_coin_simc_1929.root";

  TString data_filename =  "ep_coin_data_2279.root";  //"heep_data_1929.root";

  TString simc_rootfile;
  TString data_rootfile;
  
  simc_rootfile = "simc_analysis/weighted_simc_"+simc_filename;
  data_rootfile = "data_analysis/"+data_filename;
  
  //Open SIMC/data ROOT files;
  TFile *simc_file = new TFile(simc_rootfile);
  TFile *data_file = new TFile(data_rootfile);

  
  //Define SIMC histos ('h'-->hadron arm,  'e'-->electron arm)
  TH1F *simc_hytar =  0;
  TH1F *simc_hxptar =  0;
  TH1F *simc_hyptar =  0;
  TH1F *simc_hdelta =  0;

  TH1F *simc_eytar =  0;
  TH1F *simc_exptar =  0;
  TH1F *simc_eyptar =  0;
  TH1F *simc_edelta =  0;

  
  //Define data histos
  TH1F *data_hytar = 0;
  TH1F *data_hxptar =  0;
  TH1F *data_hyptar =  0;
  TH1F *data_hdelta =  0;

  TH1F *data_eytar = 0;
  TH1F *data_exptar =  0;
  TH1F *data_eyptar =  0;
  TH1F *data_edelta =  0;

  
  //change to simc_file
  simc_file->cd();

  //Get Histogram objects from SIMC rootfile
  simc_file->GetObject("hytar", simc_hytar);
  simc_file->GetObject("hxptar", simc_hxptar);
  simc_file->GetObject("hyptar", simc_hyptar);
  simc_file->GetObject("hdelta", simc_hdelta);

  simc_file->GetObject("eytar", simc_eytar);
  simc_file->GetObject("exptar", simc_exptar);
  simc_file->GetObject("eyptar", simc_eyptar);
  simc_file->GetObject("edelta", simc_edelta);


  //Set SIMC Histo Aesthetics
  simc_hytar->SetLineColor(kRed);
  simc_hytar->SetLineWidth(3);

  simc_hxptar->SetLineColor(kRed);
  simc_hxptar->SetLineWidth(3);
  
  simc_hyptar->SetLineColor(kRed);
  simc_hyptar->SetLineWidth(3);
  
  simc_hdelta->SetLineColor(kRed);
  simc_hdelta->SetLineWidth(3);
  

  simc_eytar->SetLineColor(kRed);
  simc_eytar->SetLineWidth(3);

  simc_exptar->SetLineColor(kRed);
  simc_exptar->SetLineWidth(3);
  
  simc_eyptar->SetLineColor(kRed);
  simc_eyptar->SetLineWidth(3);
  
  simc_edelta->SetLineColor(kRed);
  simc_edelta->SetLineWidth(3);
  
  
  //change to data_file
  data_file->cd();

  //Get Histogram objects from data rootfile
  data_file->GetObject("data_hytar", data_hytar);
  data_file->GetObject("data_hxptar", data_hxptar);
  data_file->GetObject("data_hyptar", data_hyptar);
  data_file->GetObject("data_hdelta", data_hdelta);

  data_file->GetObject("data_eytar", data_eytar);
  data_file->GetObject("data_exptar", data_exptar);
  data_file->GetObject("data_eyptar", data_eyptar);
  data_file->GetObject("data_edelta", data_edelta);
  
  
  //Set data Histo Aesthetics
  data_hytar->SetFillColorAlpha(kBlue, 0.35);
  data_hytar->SetFillStyle(3004);

  data_hxptar->SetFillColorAlpha(kBlue, 0.35);
  data_hxptar->SetFillStyle(3004);
  
  data_hyptar->SetFillColorAlpha(kBlue, 0.35);
  data_hyptar->SetFillStyle(3004);
  
  data_hdelta->SetFillColorAlpha(kBlue, 0.35);
  data_hdelta->SetFillStyle(3004);
  
  data_eytar->SetFillColorAlpha(kBlue, 0.35);
  data_eytar->SetFillStyle(3004);

  data_exptar->SetFillColorAlpha(kBlue, 0.35);
  data_exptar->SetFillStyle(3004);
  
  data_eyptar->SetFillColorAlpha(kBlue, 0.35);
  data_eyptar->SetFillStyle(3004);
  
  data_edelta->SetFillColorAlpha(kBlue, 0.35);
  data_edelta->SetFillStyle(3004);
  


  //Overlay SIMC/data plots (*** VERY IMPORTANT ***: Range and #bins must be same)

   //Set Legend
   auto leg1 = new TLegend(0.1,0.7,0.48,0.9);
   auto leg2 = new TLegend(0.1,0.7,0.48,0.9);
   auto leg3 = new TLegend(0.1,0.7,0.48,0.9);
   auto leg4 = new TLegend(0.1,0.7,0.48,0.9);

      //Set Legend
   auto leg5 = new TLegend(0.1,0.7,0.48,0.9);
   auto leg6 = new TLegend(0.1,0.7,0.48,0.9);
   auto leg7 = new TLegend(0.1,0.7,0.48,0.9);
   auto leg8 = new TLegend(0.1,0.7,0.48,0.9);

   //Create A Canvas to store Target Recon. variable comparisons in HADRON ARM
   
   TCanvas *c1 = new TCanvas("c1", "Hadron Arm: Target Reconstruction", 1000, 1000);
   c1->Divide(2,2);
   
   c1->cd(1);
   data_hytar->Draw();
   simc_hytar->Draw("same");

   leg1->AddEntry(data_hytar,"Data","f");
   leg1->AddEntry(simc_hytar,"SIMC");
   leg1->Draw();

   c1->cd(2);
   data_hxptar->Draw();
   simc_hxptar->Draw("same");

   leg2->AddEntry(data_hxptar,"Data", "f");
   leg2->AddEntry(simc_hxptar,"SIMC");
   leg2->Draw();

   c1->cd(3);
   data_hyptar->Draw();
   simc_hyptar->Draw("same");

   leg3->AddEntry(data_hyptar,"Data", "f");
   leg3->AddEntry(simc_hyptar,"SIMC");
   leg3->Draw();
     
   c1->cd(4);
   data_hdelta->Draw();
   simc_hdelta->Draw("same");

   leg4->AddEntry(data_hdelta,"Data", "f");
   leg4->AddEntry(simc_hdelta,"SIMC");
   leg4->Draw();


   
   //Create A Canvas to store Target Recon. variable comparisons in ELECTRON ARM
   
   TCanvas *c2 = new TCanvas("c2", "Electron Arm: Target Reconstruction", 1000, 1000);
   c2->Divide(2,2);
   
   c2->cd(1);
   data_eytar->Draw();
   simc_eytar->Draw("same");

   leg5->AddEntry(data_eytar,"Data","f");
   leg5->AddEntry(simc_eytar,"SIMC");
   leg5->Draw();

   c2->cd(2);
   data_exptar->Draw();
   simc_exptar->Draw("same");

   leg6->AddEntry(data_exptar,"Data", "f");
   leg6->AddEntry(simc_exptar,"SIMC");
   leg6->Draw();

   c2->cd(3);
   data_eyptar->Draw();
   simc_eyptar->Draw("same");

   leg7->AddEntry(data_eyptar,"Data", "f");
   leg7->AddEntry(simc_eyptar,"SIMC");
   leg7->Draw();
     
   c2->cd(4);
   data_edelta->Draw();
   simc_edelta->Draw("same");

   leg8->AddEntry(data_edelta,"Data", "f");
   leg8->AddEntry(simc_edelta,"SIMC");
   leg8->Draw();


  
}
