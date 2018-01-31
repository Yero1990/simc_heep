//Script to make comparison between SIMC and Commissioning Data from HallC Spring 2018
//Compare Kinematics Only!!

void make_comparison_kinematics()
{
  

  TString simc_filename = "ep_coin_simc_1929.root";
  TString data_filename = "heep_data_1929.root";

  TString simc_rootfile;
  TString data_rootfile;
  
  simc_rootfile = "simc_analysis/weighted_simc_"+simc_filename;
  data_rootfile = "data_analysis/"+data_filename;
  
  //Open SIMC/data ROOT files;
  TFile *simc_file = new TFile(simc_rootfile);
  TFile *data_file = new TFile(data_rootfile);

  
  //Define SIMC histos ('h'-->hadron arm,  'e'-->electron arm)
  TH1F *simc_emiss =  0;
  TH1F *simc_pmiss =  0;
  TH1F *simc_Q2 =  0;
  TH1F *simc_omega =  0;
  TH1F *simc_W =  0;


  //Define data histos
  TH1F *data_emiss = 0;
  TH1F *data_pmiss =  0;
  TH1F *data_Q2 =  0;
  TH1F *data_omega =  0;
  TH1F *data_W =  0;

  
  //change to simc_file
  simc_file->cd();

  //Get Histogram objects from SIMC rootfile
  simc_file->GetObject("Emiss", simc_emiss);
  simc_file->GetObject("pm", simc_pmiss);
  simc_file->GetObject("Q_2", simc_Q2);
  simc_file->GetObject("omega", simc_omega);
  simc_file->GetObject("W_inv", simc_W);


  //Set SIMC Histo Aesthetics
  simc_emiss->SetLineColor(kRed);
  simc_emiss->SetLineWidth(3);

  simc_pmiss->SetLineColor(kRed);
  simc_pmiss->SetLineWidth(3);
  
  simc_Q2->SetLineColor(kRed);
  simc_Q2->SetLineWidth(3);
  
  simc_omega->SetLineColor(kRed);
  simc_omega->SetLineWidth(3);
  
   simc_W->SetLineColor(kRed);
   simc_W->SetLineWidth(3);


  
  //change to data_file
  data_file->cd();

  //Get Histogram objects from data rootfile
  data_file->GetObject("data_Emiss", data_emiss);
  data_file->GetObject("data_pm", data_pmiss);
  data_file->GetObject("data_Q2", data_Q2);
  data_file->GetObject("data_omega", data_omega);
  data_file->GetObject("data_W_inv", data_W);

  //Set data Histo Aesthetics
  data_emiss->SetFillColorAlpha(kBlue, 0.35);
  data_emiss->SetFillStyle(3004);

  data_pmiss->SetFillColorAlpha(kBlue, 0.35);
  data_pmiss->SetFillStyle(3004);
  
  data_Q2->SetFillColorAlpha(kBlue, 0.35);
  data_Q2->SetFillStyle(3004);
  
  data_omega->SetFillColorAlpha(kBlue, 0.35);
  data_omega->SetFillStyle(3004);
  
   data_W->SetFillColorAlpha(kBlue, 0.35);
   data_W->SetFillStyle(3004);
   

  //Overlay SIMC/data plots (*** VERY IMPORTANT ***: Range and #bins must be same)

   //Set Legend
   auto leg1 = new TLegend(0.1,0.7,0.48,0.9);
   auto leg2 = new TLegend(0.1,0.7,0.48,0.9);
   auto leg3 = new TLegend(0.1,0.7,0.48,0.9);
   auto leg4 = new TLegend(0.1,0.7,0.48,0.9);
   auto leg5 = new TLegend(0.1,0.7,0.48,0.9);
   
   //Create A Canvas to store kinematic variable comparisons
   TCanvas *ckine = new TCanvas("ckine", "Kinematics", 1000, 1000);
   ckine->Divide(3,2);
   
   ckine->cd(1);
   data_emiss->Draw();
   simc_emiss->Draw("same");

   leg1->AddEntry(data_emiss,"Data","f");
   leg1->AddEntry(simc_emiss,"SIMC");
   leg1->Draw();

   ckine->cd(2);
   data_pmiss->Draw();
   simc_pmiss->Draw("same");

   leg2->AddEntry(data_pmiss,"Data", "f");
   leg2->AddEntry(simc_pmiss,"SIMC");
   leg2->Draw();

   ckine->cd(3);
   data_Q2->Draw();
   simc_Q2->Draw("same");

   leg3->AddEntry(data_Q2,"Data", "f");
   leg3->AddEntry(simc_Q2,"SIMC");
   leg3->Draw();
     
   ckine->cd(4);
   data_omega->Draw();
   simc_omega->Draw("same");

   leg4->AddEntry(data_omega,"Data", "f");
   leg4->AddEntry(simc_omega,"SIMC");
   leg4->Draw();

   ckine->cd(5);
   data_W->Draw();
   simc_W->Draw("same");

   leg5->AddEntry(data_W,"Data", "f");
   leg5->AddEntry(simc_W,"SIMC");
   leg5->Draw();


  
}
