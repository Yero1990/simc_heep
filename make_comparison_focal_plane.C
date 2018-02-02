//Script to make comparison between SIMC and Commissioning Data from HallC Spring 2018
//Compare Target Reconstruction Variables Only!!

void make_comparison_focal_plane()
{
  

  TString simc_filename =  "ep_coin_simc_1929.root"; //"ep_coin_simc_1929.root";
  TString data_filename =  "ep_coin_data_1929.root";  //"heep_data_1929.root";


  TString simc_rootfile;
  TString data_rootfile;
  
  simc_rootfile = "simc_analysis/weighted_"+simc_filename;
  data_rootfile = "data_analysis/"+data_filename;
  
  //Open SIMC/data ROOT files;
  TFile *simc_file = new TFile(simc_rootfile);
  TFile *data_file = new TFile(data_rootfile);

  
  //Define SIMC histos ('h'-->hadron arm,  'e'-->electron arm)
  TH1F *simc_hxfp =  0;
  TH1F *simc_hyfp =  0;
  TH1F *simc_hxpfp =  0;
  TH1F *simc_hypfp =  0;

  TH1F *simc_exfp =  0;
  TH1F *simc_eyfp =  0;
  TH1F *simc_expfp =  0;
  TH1F *simc_eypfp =  0;

  
  //Define data histos
  TH1F *data_hxfp =  0;
  TH1F *data_hyfp =  0;
  TH1F *data_hxpfp =  0;
  TH1F *data_hypfp =  0;

  TH1F *data_exfp =  0;
  TH1F *data_eyfp =  0;
  TH1F *data_expfp =  0;
  TH1F *data_eypfp =  0;

  
  //change to simc_file
  simc_file->cd();

  //Get Histogram objects from SIMC rootfile
  simc_file->GetObject("cut_hxfp", simc_hxfp);
  simc_file->GetObject("cut_hyfp", simc_hyfp);
  simc_file->GetObject("cut_hxpfp", simc_hxpfp);
  simc_file->GetObject("cut_hypfp", simc_hypfp);

  simc_file->GetObject("cut_exfp", simc_exfp);
  simc_file->GetObject("cut_eyfp", simc_eyfp);
  simc_file->GetObject("cut_expfp", simc_expfp);
  simc_file->GetObject("cut_eypfp", simc_eypfp);


  //Set SIMC Histo Aesthetics
  simc_hxfp->SetLineColor(kRed);
  simc_hxfp->SetLineWidth(3);

  simc_hyfp->SetLineColor(kRed);
  simc_hyfp->SetLineWidth(3);
  
  simc_hxpfp->SetLineColor(kRed);
  simc_hxpfp->SetLineWidth(3);
  
  simc_hypfp->SetLineColor(kRed);
  simc_hypfp->SetLineWidth(3);
  

  simc_exfp->SetLineColor(kRed);
  simc_exfp->SetLineWidth(3);

  simc_eyfp->SetLineColor(kRed);
  simc_eyfp->SetLineWidth(3);
  
  simc_expfp->SetLineColor(kRed);
  simc_expfp->SetLineWidth(3);
  
  simc_eypfp->SetLineColor(kRed);
  simc_eypfp->SetLineWidth(3);
  
  
  //change to data_file
  data_file->cd();

  //Get Histogram objects from data rootfile
  data_file->GetObject("data_hxfp", data_hxfp);
  data_file->GetObject("data_hyfp", data_hyfp);
  data_file->GetObject("data_hxpfp", data_hxpfp);
  data_file->GetObject("data_hypfp", data_hypfp);

  data_file->GetObject("data_exfp", data_exfp);
  data_file->GetObject("data_eyfp", data_eyfp);
  data_file->GetObject("data_expfp", data_expfp);
  data_file->GetObject("data_eypfp", data_eypfp);

  
  //Set data Histo Aesthetics
  data_hxfp->SetFillColorAlpha(kBlue, 0.35);
  data_hxfp->SetFillStyle(3004);

  data_hyfp->SetFillColorAlpha(kBlue, 0.35);
  data_hyfp->SetFillStyle(3004);
  
  data_hxpfp->SetFillColorAlpha(kBlue, 0.35);
  data_hxpfp->SetFillStyle(3004);
  
  data_hypfp->SetFillColorAlpha(kBlue, 0.35);
  data_hypfp->SetFillStyle(3004);
  
  data_exfp->SetFillColorAlpha(kBlue, 0.35);
  data_exfp->SetFillStyle(3004);

  data_eyfp->SetFillColorAlpha(kBlue, 0.35);
  data_eyfp->SetFillStyle(3004);
  
  data_expfp->SetFillColorAlpha(kBlue, 0.35);
  data_expfp->SetFillStyle(3004);
  
  data_eypfp->SetFillColorAlpha(kBlue, 0.35);
  data_eypfp->SetFillStyle(3004);
  


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
   
   TCanvas *c1 = new TCanvas("c1", "Hadron Arm: Focal Plane", 1000, 1000);
   c1->Divide(2,2);
   
   c1->cd(1);
   data_hxfp->Draw();
   simc_hxfp->Draw("same");

   leg1->AddEntry(data_hxfp,"Data","f");
   leg1->AddEntry(simc_hxfp,"SIMC");
   leg1->Draw();

   c1->cd(2);
   data_hyfp->Draw();
   simc_hyfp->Draw("same");

   leg2->AddEntry(data_hyfp,"Data", "f");
   leg2->AddEntry(simc_hyfp,"SIMC");
   leg2->Draw();

   c1->cd(3);
   data_hxpfp->Draw();
   simc_hxpfp->Draw("same");

   leg3->AddEntry(data_hxpfp,"Data", "f");
   leg3->AddEntry(simc_hxpfp,"SIMC");
   leg3->Draw();
     
   c1->cd(4);
   data_hypfp->Draw();
   simc_hypfp->Draw("same");

   leg4->AddEntry(data_hypfp,"Data", "f");
   leg4->AddEntry(simc_hypfp,"SIMC");
   leg4->Draw();


   
   //Create A Canvas to store Focal Plane. variable comparisons in ELECTRON ARM
   
   TCanvas *c2 = new TCanvas("c2", "Electron Arm: Focal Plane", 1000, 1000);
   c2->Divide(2,2);
   
   c2->cd(1);
   data_exfp->Draw();
   simc_exfp->Draw("same");

   leg5->AddEntry(data_exfp,"Data","f");
   leg5->AddEntry(simc_exfp,"SIMC");
   leg5->Draw();

   c2->cd(2);
   data_eyfp->Draw();
   simc_eyfp->Draw("same");

   leg6->AddEntry(data_eyfp,"Data", "f");
   leg6->AddEntry(simc_eyfp,"SIMC");
   leg6->Draw();

   c2->cd(3);
   data_expfp->Draw();
   simc_expfp->Draw("same");

   leg7->AddEntry(data_expfp,"Data", "f");
   leg7->AddEntry(simc_expfp,"SIMC");
   leg7->Draw();
     
   c2->cd(4);
   data_eypfp->Draw();
   simc_eypfp->Draw("same");

   leg8->AddEntry(data_eypfp,"Data", "f");
   leg8->AddEntry(simc_eypfp,"SIMC");
   leg8->Draw();


  
}
