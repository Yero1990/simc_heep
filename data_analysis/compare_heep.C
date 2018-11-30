//Script to make comparison between SIMC and Commissioning Data from HallC Spring 2018
//Compare Target Reconstruction/FOCAL PLANE/ Kinematics Variables

void compare_heep(int run)
{

  gROOT->SetBatch(kTRUE);  
  gStyle->SetOptStat(1001111);
  //TString simc_filename =  "weighted_ep_coin_simc_1854.root"; //"ep_coin_simc_1929.root";
  
  //Pre-defined SIMC/data root file names containing histogram object to comapare
  TString simc_filename =  Form("Wcheck_simc_histos_%d.root", run);
  //TString simc_filename =  Form("Weighted_simc_hms_single_%d_gfor.root", runNUM);
  TString data_filename = Form("Wcheck_data_histos_%d.root",run); 

  TString simc_path;
  TString data_path;
  
  simc_path = "../simc_analysis/D2_heep/pUnCorr/"+simc_filename;
  data_path = "./" + data_filename;
  
  //Open SIMC/data ROOT files;
  TFile *simc_file = new TFile(simc_path);
  TFile *data_file = new TFile(data_path);

  //---------------Target ----------------
  //Define SIMC histos ('h'-->hadron arm,  'e'-->electron arm)
  
  TH1F *simc_xtar =  0;
  TH1F *simc_ytar =  0;
  TH1F *simc_ztar =  0;

  //Define data histos
  TH1F *data_xtar = 0;
  TH1F *data_ytar = 0;
  TH1F *data_ztar = 0;

  //---------------Target Reconstruction Variables----------------
  //Define SIMC histos ('h'-->hadron arm,  'e'-->electron arm)
  TH1F *simc_eytar =  0;
  TH1F *simc_exptar =  0;
  TH1F *simc_eyptar =  0;
  TH1F *simc_edelta =  0;

  TH1F *simc_hytar =  0;
  TH1F *simc_hxptar =  0;
  TH1F *simc_hyptar =  0;
  TH1F *simc_hdelta =  0;

  //Define data histos
  TH1F *data_eytar = 0;
  TH1F *data_exptar =  0;
  TH1F *data_eyptar =  0;
  TH1F *data_edelta =  0;

  TH1F *data_hytar = 0;
  TH1F *data_hxptar =  0;
  TH1F *data_hyptar =  0;
  TH1F *data_hdelta =  0;

  //-----------------------------------------------------------
 
  //--------------FOCAL PLANE VARIABLES------------------------

 //Define SIMC histos ('h'-->hadron arm,  'e'-->electron arm)
  TH1F *simc_exfp =  0;
  TH1F *simc_eyfp =  0;
  TH1F *simc_expfp =  0;
  TH1F *simc_eypfp =  0;

  TH1F *simc_hxfp =  0;
  TH1F *simc_hyfp =  0;
  TH1F *simc_hxpfp =  0;
  TH1F *simc_hypfp =  0;
  
  //Define data histos
  TH1F *data_exfp =  0;
  TH1F *data_eyfp =  0;
  TH1F *data_expfp =  0;
  TH1F *data_eypfp =  0;

  TH1F *data_hxfp =  0;
  TH1F *data_hyfp =  0;
  TH1F *data_hxpfp =  0;
  TH1F *data_hypfp =  0;

  //--------------------------------------------------------------

  //-------------------------KINEMATICS---------------------------
  TH1F *simc_Q2 =  0;
  TH1F *simc_omega =  0;
  TH1F *simc_W =  0;
  TH1F *simc_thq = 0;

  TH1F *simc_xbj = 0;
  TH1F *simc_th_elec = 0;                                  
  TH1F *simc_kf = 0;  
  TH1F *simc_emiss = 0;

  //Define data histos
  TH1F *data_Q2 =  0;
  TH1F *data_omega =  0;
  TH1F *data_W =  0;
  TH1F *data_thq = 0;

  TH1F *data_xbj = 0;
  TH1F *data_th_elec = 0;
  TH1F *data_kf = 0;
  TH1F *data_emiss = 0;

  //---------------------------------------------------------------

 //change to simc_file
  simc_file->cd();

  
  //----------Get Target Histograms------------------
  //Get Histogram objects from SIMC rootfile
  simc_file->GetObject("cut_x_tar", simc_xtar);
  simc_file->GetObject("cut_y_tar", simc_ytar);
  simc_file->GetObject("cut_z_tar", simc_ztar);


  //Set SIMC Histo Aesthetics
  simc_xtar->SetLineColor(kRed);
  simc_xtar->SetLineWidth(2);
  simc_ytar->SetLineColor(kRed);
  simc_ytar->SetLineWidth(2);
  simc_ztar->SetLineColor(kRed);
  simc_ztar->SetLineWidth(2);

  
  //change to data_file
  data_file->cd();

  //Get Histogram objects from data rootfile
  data_file->GetObject("cut_x_tar", data_xtar);
  data_file->GetObject("cut_y_tar", data_ytar);
  data_file->GetObject("cut_z_tar", data_ztar);

  
  //Set data Histo Aesthetics
  data_xtar->SetFillColorAlpha(kBlue, 0.35);
  data_xtar->SetFillStyle(3004);
  data_ytar->SetFillColorAlpha(kBlue, 0.35);
  data_ytar->SetFillStyle(3004);
  data_ztar->SetFillColorAlpha(kBlue, 0.35);
  data_ztar->SetFillStyle(3004);

  //-----------------------------------------------------------------


  //---------------------------------------------------------------

 //change to simc_file
  simc_file->cd();

  //----------Get Target Reconstructed Histograms------------------
  //Get Histogram objects from SIMC rootfile
  simc_file->GetObject("cut_eytar", simc_eytar);
  simc_file->GetObject("cut_exptar", simc_exptar);
  simc_file->GetObject("cut_eyptar", simc_eyptar);
  simc_file->GetObject("cut_edelta", simc_edelta);

  simc_file->GetObject("cut_hytar", simc_hytar);
  simc_file->GetObject("cut_hxptar", simc_hxptar);
  simc_file->GetObject("cut_hyptar", simc_hyptar);
  simc_file->GetObject("cut_hdelta", simc_hdelta);

  //Set SIMC Histo Aesthetics
  simc_eytar->SetLineColor(kRed);
  simc_eytar->SetLineWidth(2);
  simc_exptar->SetLineColor(kRed);
  simc_exptar->SetLineWidth(2);
  simc_eyptar->SetLineColor(kRed);
  simc_eyptar->SetLineWidth(2);
  simc_edelta->SetLineColor(kRed);
  simc_edelta->SetLineWidth(2);
  
  simc_hytar->SetLineColor(kRed);
  simc_hytar->SetLineWidth(2);
  simc_hxptar->SetLineColor(kRed);
  simc_hxptar->SetLineWidth(2);
  simc_hyptar->SetLineColor(kRed);
  simc_hyptar->SetLineWidth(2);
  simc_hdelta->SetLineColor(kRed);
  simc_hdelta->SetLineWidth(2);

  //change to data_file
  data_file->cd();

  //Get Histogram objects from data rootfile
  data_file->GetObject("cut_eytar", data_eytar);
  data_file->GetObject("cut_exptar", data_exptar);
  data_file->GetObject("cut_eyptar", data_eyptar);
  data_file->GetObject("cut_edelta", data_edelta);
  
  data_file->GetObject("cut_hytar", data_hytar);
  data_file->GetObject("cut_hxptar", data_hxptar);
  data_file->GetObject("cut_hyptar", data_hyptar);
  data_file->GetObject("cut_hdelta", data_hdelta);

  //Set data Histo Aesthetics
  data_eytar->SetFillColorAlpha(kBlue, 0.35);
  data_eytar->SetFillStyle(3004);
  data_exptar->SetFillColorAlpha(kBlue, 0.35);
  data_exptar->SetFillStyle(3004);
  data_eyptar->SetFillColorAlpha(kBlue, 0.35);
  data_eyptar->SetFillStyle(3004);
  data_edelta->SetFillColorAlpha(kBlue, 0.35);
  data_edelta->SetFillStyle(3004);

  data_hytar->SetFillColorAlpha(kBlue, 0.35);
  data_hytar->SetFillStyle(3004);
  data_hxptar->SetFillColorAlpha(kBlue, 0.35);
  data_hxptar->SetFillStyle(3004);
  data_hyptar->SetFillColorAlpha(kBlue, 0.35);
  data_hyptar->SetFillStyle(3004);
  data_hdelta->SetFillColorAlpha(kBlue, 0.35);
  data_hdelta->SetFillStyle(3004);

  //-----------------------------------------------------------------

  

  //---------------Get FOCAL PLANE Histograms------------------------

   //change to simc_file
  simc_file->cd();

  //Get Histogram objects from SIMC rootfile
  simc_file->GetObject("cut_exfp", simc_exfp);
  simc_file->GetObject("cut_eyfp", simc_eyfp);
  simc_file->GetObject("cut_expfp", simc_expfp);
  simc_file->GetObject("cut_eypfp", simc_eypfp);

  simc_file->GetObject("cut_hxfp", simc_hxfp);
  simc_file->GetObject("cut_hyfp", simc_hyfp);
  simc_file->GetObject("cut_hxpfp", simc_hxpfp);
  simc_file->GetObject("cut_hypfp", simc_hypfp);
  //Set SIMC Histo Aesthetics
  simc_exfp->SetLineColor(kRed);
  simc_exfp->SetLineWidth(2);
  simc_eyfp->SetLineColor(kRed);
  simc_eyfp->SetLineWidth(2);
  simc_expfp->SetLineColor(kRed);
  simc_expfp->SetLineWidth(2);
  simc_eypfp->SetLineColor(kRed);
  simc_eypfp->SetLineWidth(2);
  
  simc_hxfp->SetLineColor(kRed);
  simc_hxfp->SetLineWidth(2);
  simc_hyfp->SetLineColor(kRed);
  simc_hyfp->SetLineWidth(2);
  simc_hxpfp->SetLineColor(kRed);
  simc_hxpfp->SetLineWidth(2);
  simc_hypfp->SetLineColor(kRed);
  simc_hypfp->SetLineWidth(2);
  //change to data_file
  data_file->cd();

  //Get Histogram objects from data rootfile
  data_file->GetObject("cut_exfp", data_exfp);
  data_file->GetObject("cut_eyfp", data_eyfp);
  data_file->GetObject("cut_expfp", data_expfp);
  data_file->GetObject("cut_eypfp", data_eypfp);

  data_file->GetObject("cut_hxfp", data_hxfp);
  data_file->GetObject("cut_hyfp", data_hyfp);
  data_file->GetObject("cut_hxpfp", data_hxpfp);
  data_file->GetObject("cut_hypfp", data_hypfp);
  //Set data Histo Aesthetics
  data_exfp->SetFillColorAlpha(kBlue, 0.35);
  data_exfp->SetFillStyle(3004);
  data_eyfp->SetFillColorAlpha(kBlue, 0.35);
  data_eyfp->SetFillStyle(3004);
  data_expfp->SetFillColorAlpha(kBlue, 0.35);
  data_expfp->SetFillStyle(3004);
  data_eypfp->SetFillColorAlpha(kBlue, 0.35);
  data_eypfp->SetFillStyle(3004);

  data_hxfp->SetFillColorAlpha(kBlue, 0.35);
  data_hxfp->SetFillStyle(3004);
  data_hyfp->SetFillColorAlpha(kBlue, 0.35);
  data_hyfp->SetFillStyle(3004);
  data_hxpfp->SetFillColorAlpha(kBlue, 0.35);
  data_hxpfp->SetFillStyle(3004);
  data_hypfp->SetFillColorAlpha(kBlue, 0.35);
  data_hypfp->SetFillStyle(3004);

  //--------------------------------------------------------------
  
  //------------------Get KINEMATICS VARIABLES--------------------

   //change to simc_file
  simc_file->cd();

  //Get Histogram objects from SIMC rootfile
  simc_file->GetObject("cut_Q_2", simc_Q2);
  simc_file->GetObject("cut_omega", simc_omega);
  simc_file->GetObject("cut_W_inv", simc_W);
  simc_file->GetObject("cut_theta_q", simc_thq);

  simc_file->GetObject("cut_xbj", simc_xbj);
  simc_file->GetObject("cut_theta_elec", simc_th_elec);
  simc_file->GetObject("cut_kf", simc_kf);
  simc_file->GetObject("Emiss", simc_emiss);

  
  
  //Set SIMC Histo Aesthetics
  simc_Q2->SetLineColor(kRed);
  simc_Q2->SetLineWidth(2);
  simc_omega->SetLineColor(kRed);
  simc_omega->SetLineWidth(2);
  simc_W->SetLineColor(kRed);
  simc_W->SetLineWidth(2);
  simc_thq->SetLineColor(kRed);
  simc_thq->SetLineWidth(2);
  
  simc_xbj->SetLineColor(kRed);
  simc_xbj->SetLineWidth(2);
  simc_th_elec->SetLineColor(kRed);
  simc_th_elec->SetLineWidth(2);
  simc_kf->SetLineColor(kRed);
  simc_kf->SetLineWidth(2);
  simc_emiss->SetLineColor(kRed);
  simc_emiss->SetLineWidth(2);

  //change to data_file
  data_file->cd();
  
  //Get Histogram objects from data rootfile
  data_file->GetObject("cut_Q_2", data_Q2);
  data_file->GetObject("cut_omega", data_omega);
  data_file->GetObject("cut_W_inv", data_W);
  data_file->GetObject("cut_theta_q", data_thq);

  
  data_file->GetObject("cut_xbj", data_xbj);
  data_file->GetObject("cut_theta_elec", data_th_elec);
  data_file->GetObject("cut_kf", data_kf);
  data_file->GetObject("Emiss", data_emiss);

  //Set data Histo Aesthetics
  data_Q2->SetFillColorAlpha(kBlue, 0.35);
  data_Q2->SetFillStyle(3004);
  data_omega->SetFillColorAlpha(kBlue, 0.35);
  data_omega->SetFillStyle(3004);
  data_W->SetFillColorAlpha(kBlue, 0.35);
  data_W->SetFillStyle(3004);
  data_thq->SetFillColorAlpha(kBlue, 0.35);
  data_thq->SetFillStyle(3004);

  data_xbj->SetFillColorAlpha(kBlue, 0.35);
  data_xbj->SetFillStyle(3004);
  data_th_elec->SetFillColorAlpha(kBlue, 0.35);
  data_th_elec->SetFillStyle(3004);
  data_kf->SetFillColorAlpha(kBlue, 0.35);
  data_kf->SetFillStyle(3004);
  data_emiss->SetFillColorAlpha(kBlue, 0.35);
  data_emiss->SetFillStyle(3004);
  
  //Overlay SIMC/data plots (*** VERY IMPORTANT ***: Range and #bins must be same)

   //Set Legend
   auto leg1 = new TLegend(0.1,0.8,0.28,0.9);
   auto leg2 = new TLegend(0.1,0.8,0.28,0.9);
   auto leg3 = new TLegend(0.1,0.8,0.28,0.9);
   auto leg4 = new TLegend(0.1,0.8,0.28,0.9);

   //Set Legend
   auto leg5 = new TLegend(0.1,0.8,0.28,0.9);
   auto leg6 = new TLegend(0.1,0.8,0.28,0.9);
   auto leg7 = new TLegend(0.1,0.8,0.28,0.9);
   auto leg8 = new TLegend(0.1,0.8,0.28,0.9);

  
   //-----------------PLOT Target Reconstructed Variables SIMC/Data comparison-----------------------

   //Create A Canvas to store Target Recon. variable comparisons in HADRON ARM
   
   TCanvas *c1 = new TCanvas("c1", "Electron Arm: Target Reconstruction", 2000, 1000);
   c1->Divide(2,2);

   c1->cd(1);
   simc_eytar->DrawNormalized();
   data_eytar->DrawNormalized("sameshist");
   leg5->AddEntry(data_eytar,"Data","f");
   leg5->AddEntry(simc_eytar,"SIMC");
   leg5->Draw();

   c1->cd(2);
   simc_exptar->DrawNormalized();
   data_exptar->DrawNormalized("sameshist");
   leg6->AddEntry(data_exptar,"Data", "f");
   leg6->AddEntry(simc_exptar,"SIMC");
   leg6->Draw();

   c1->cd(3);
   simc_eyptar->DrawNormalized();
   data_eyptar->DrawNormalized("sameshist");
   leg7->AddEntry(data_eyptar,"Data", "f");
   leg7->AddEntry(simc_eyptar,"SIMC");
   leg7->Draw();
     
   c1->cd(4);
   simc_edelta->DrawNormalized();
   data_edelta->DrawNormalized("sameshist");
   leg8->AddEntry(data_edelta,"Data", "f");
   leg8->AddEntry(simc_edelta,"SIMC");
   leg8->Draw();

   c1->SaveAs(Form("eArm_TargRecon_%d.pdf", run));

   //------------------------------------------------------------------------------

   
   //-----------------PLOT FOCAL PLANE  Variables SIMC/Data comparison-----------------------

  //Set Legend
   auto leg9 = new TLegend(0.1,0.8,0.28,0.9);
   auto leg10 = new TLegend(0.1,0.8,0.28,0.9);
   auto leg11 = new TLegend(0.1,0.8,0.28,0.9);
   auto leg12 = new TLegend(0.1,0.8,0.28,0.9);

      //Set Legend
   auto leg13 = new TLegend(0.1,0.8,0.28,0.9);
   auto leg14 = new TLegend(0.1,0.8,0.28,0.9);
   auto leg15 = new TLegend(0.1,0.8,0.28,0.9);
   auto leg16 = new TLegend(0.1,0.8,0.28,0.9);

   TCanvas *c2 = new TCanvas("c2", "Electron Arm: Focal Plane", 2000, 1000);
   c2->Divide(2,2);

   c2->cd(1);
   simc_exfp->DrawNormalized();
   data_exfp->DrawNormalized("sameshist");
   leg13->AddEntry(data_exfp,"Data","f");
   leg13->AddEntry(simc_exfp,"SIMC");
   leg13->Draw();
   
   c2->cd(2);
   simc_eyfp->DrawNormalized();
   data_eyfp->DrawNormalized("sameshist");
   leg14->AddEntry(data_eyfp,"Data", "f");
   leg14->AddEntry(simc_eyfp,"SIMC");
   leg14->Draw();

   c2->cd(3);
   simc_expfp->DrawNormalized();
   data_expfp->DrawNormalized("sameshist");
   leg15->AddEntry(data_expfp,"Data", "f");
   leg15->AddEntry(simc_expfp,"SIMC");
   leg15->Draw();
     
   c2->cd(4);
   simc_eypfp->DrawNormalized();
   data_eypfp->DrawNormalized("sameshist");
   leg16->AddEntry(data_eypfp,"Data", "f");
   leg16->AddEntry(simc_eypfp,"SIMC");
   leg16->Draw();

   c2->SaveAs(Form("eArm_FocalPlane_%d.pdf", run));                                                                                   

   //----------------------------------------------------------- 
 
   
   //-----------------PLOT KINEMATICS SIMC/Data comparison---------------

//Set Legend
   auto leg17 = new TLegend(0.1,0.8,0.28,0.9);
   auto leg18 = new TLegend(0.1,0.8,0.28,0.9);
   auto leg19 = new TLegend(0.1,0.8,0.28,0.9);
   auto leg20 = new TLegend(0.1,0.8,0.28,0.9);
   auto leg21 = new TLegend(0.1,0.8,0.28,0.9);
   auto leg22 = new TLegend(0.1,0.8,0.28,0.9);
   auto leg23 = new TLegend(0.1,0.8,0.28,0.9);
   auto leg24 = new TLegend(0.1,0.8,0.28,0.9);
   auto leg25 = new TLegend(0.1,0.8,0.28,0.9);
   auto leg26 = new TLegend(0.1,0.8,0.28,0.9);
   auto leg27 = new TLegend(0.1,0.8,0.28,0.9);
   


   //Create A Canvas to store kinematic variable comparisons
   TCanvas *c3 = new TCanvas("c3", "Kinematics", 2000, 1000);
   c3->Divide(4,2);

   c3->cd(1);
   data_Q2->GetXaxis()->SetTitle("Q^{2} [GeV^{2}]");
   data_Q2->GetXaxis()->CenterTitle();
   simc_Q2->DrawNormalized();
   data_Q2->DrawNormalized("sameshist");
   leg19->AddEntry(data_Q2,"Data", "f");
   leg19->AddEntry(simc_Q2,"SIMC");
   leg19->Draw();
     
   c3->cd(2);
   data_omega->GetXaxis()->SetTitle("Energy Transfer, #omega [GeV]");
   data_omega->GetXaxis()->CenterTitle();  
   simc_omega->DrawNormalized();
   data_omega->DrawNormalized("sameshist");
   leg20->AddEntry(data_omega,"Data", "f");
   leg20->AddEntry(simc_omega,"SIMC");
   leg20->Draw();

   c3->cd(3);
   data_W->GetXaxis()->SetTitle("Invariant Mass, W [GeV]");
   data_W->GetXaxis()->CenterTitle();
   simc_W->DrawNormalized();
   data_W->DrawNormalized("sameshist");
   leg21->AddEntry(data_W,"Data", "f");
   leg21->AddEntry(simc_W,"SIMC");
   leg21->Draw();

   c3->cd(4);
   data_thq->GetXaxis()->SetTitle("q-vector Angle, #theta_{q} [deg]");
   data_thq->GetXaxis()->CenterTitle();
   simc_thq->DrawNormalized();
   data_thq->DrawNormalized("sameshist");
   leg22->AddEntry(data_thq,"Data", "f");
   leg22->AddEntry(simc_thq,"SIMC");
   leg22->Draw();

   c3->cd(5);
   simc_xbj->DrawNormalized();
   data_xbj->DrawNormalized("sameshist");
   leg23->AddEntry(data_xbj,"Data","f");
   leg23->AddEntry(simc_xbj,"SIMC");
   leg23->Draw();

   c3->cd(6);
   data_th_elec->GetXaxis()->SetTitle("Electron Scatt. Angle, #theta_{e} [deg]");
   data_th_elec->GetXaxis()->CenterTitle();
   simc_th_elec->DrawNormalized();
   data_th_elec->DrawNormalized("sameshist");
   leg24->AddEntry(data_th_elec,"Data","f");
   leg24->AddEntry(simc_th_elec,"SIMC");
   leg24->Draw();

   c3->cd(7);
   data_kf->GetXaxis()->SetTitle("Electron Final Momentum, k_{f} [GeV/c] ");
   data_kf->GetXaxis()->CenterTitle();   
   simc_kf->DrawNormalized();
   data_kf->DrawNormalized("sameshist");
   leg25->AddEntry(data_kf,"Data","f");
   leg25->AddEntry(simc_kf,"SIMC");
   leg25->Draw();

   c3->cd(8);
   data_emiss->GetXaxis()->SetTitle("Missing Energy, E_{m} [GeV/c] ");
   data_emiss->GetXaxis()->CenterTitle();   
   simc_emiss->DrawNormalized("hist");
   data_emiss->DrawNormalized("sameshist");
   leg25->AddEntry(data_emiss,"Data","f");
   leg25->AddEntry(simc_emiss,"SIMC");
   leg25->Draw();

   c3->SaveAs(Form("eArm_Kinematics_%d.pdf", run));                                                                   


 //-----------------PLOT TARGET  Variables SIMC/Data comparison-----------------------

  //Set Legend
   auto leg28 = new TLegend(0.1,0.8,0.28,0.9);
   auto leg29 = new TLegend(0.1,0.8,0.28,0.9);
   auto leg30 = new TLegend(0.1,0.8,0.28,0.9);
   auto leg31 = new TLegend(0.1,0.8,0.28,0.9);


   TCanvas *c4 = new TCanvas("c4", "Target Variables", 2000, 1000);
   c4->Divide(3,1);

   c4->cd(1);
   simc_xtar->DrawNormalized();
   data_xtar->DrawNormalized("sameshist");
   leg28->AddEntry(data_xtar,"Data","f");
   leg28->AddEntry(simc_xtar,"SIMC");
   leg28->Draw();
  
   c4->cd(2);
   simc_ytar->DrawNormalized();
   data_ytar->DrawNormalized("sameshist");
   leg29->AddEntry(data_ytar,"Data","f");
   leg29->AddEntry(simc_ytar,"SIMC");
   leg29->Draw();

   c4->cd(3);
   simc_ztar->DrawNormalized();
   data_ztar->DrawNormalized("sameshist");
   leg30->AddEntry(data_ztar,"Data","f");
   leg30->AddEntry(simc_ztar,"SIMC");
   leg30->Draw();
  
   c4->SaveAs(Form("eArm_TargVar_%d.pdf", run));                                                                                              


   //--------PLOT HADRON ARM QUANTITIES--------


   
   //-----------------PLOT Target Reconstructed Variables SIMC/Data comparison-----------------------
 
   //Set Legend
   auto htr_l1 = new TLegend(0.1,0.8,0.28,0.9);
   auto htr_l2 = new TLegend(0.1,0.8,0.28,0.9);
   auto htr_l3 = new TLegend(0.1,0.8,0.28,0.9);
   auto htr_l4 = new TLegend(0.1,0.8,0.28,0.9);
   
   //Create A Canvas to store Target Recon. variable comparisons in HADRON ARM
   
   TCanvas *htr = new TCanvas("htr", "Hadron Arm: Target Reconstruction", 2000, 1000);
   htr->Divide(2,2);

   htr->cd(1);
   simc_hytar->DrawNormalized();
   data_hytar->DrawNormalized("sameshist");
   htr_l1->AddEntry(data_hytar,"Data","f");
   htr_l1->AddEntry(simc_hytar,"SIMC");
   htr_l1->Draw();

   htr->cd(2);
   simc_hxptar->DrawNormalized();
   data_hxptar->DrawNormalized("sameshist");
   htr_l2->AddEntry(data_hxptar,"Data", "f");
   htr_l2->AddEntry(simc_hxptar,"SIMC");
   htr_l2->Draw();

   htr->cd(3);
   simc_hyptar->DrawNormalized();
   data_hyptar->DrawNormalized("sameshist");
   htr_l3->AddEntry(data_hyptar,"Data", "f");
   htr_l3->AddEntry(simc_hyptar,"SIMC");
   htr_l3->Draw();
     
   htr->cd(4);
   simc_hdelta->DrawNormalized();
   data_hdelta->DrawNormalized("sameshist");
   htr_l4->AddEntry(data_hdelta,"Data", "f");
   htr_l4->AddEntry(simc_hdelta,"SIMC");
   htr_l4->Draw();

   htr->SaveAs(Form("hArm_TargRecon_%d.pdf", run));

   //------------------------------------------------------------------------------

   
   //-----------------PLOT FOCAL PLANE  Variables SIMC/Data comparison-----------------------

   //Set Legend
   auto hfp_l1 = new TLegend(0.1,0.8,0.28,0.9);
   auto hfp_l2 = new TLegend(0.1,0.8,0.28,0.9);
   auto hfp_l3 = new TLegend(0.1,0.8,0.28,0.9);
   auto hfp_l4 = new TLegend(0.1,0.8,0.28,0.9);

   TCanvas *hfp = new TCanvas("hfp", "Hadron Arm: Focal Plane", 2000, 1000);
   hfp->Divide(2,2);

   hfp->cd(1);
   simc_hxfp->DrawNormalized();
   data_hxfp->DrawNormalized("sameshist");
   hfp_l1->AddEntry(data_hxfp,"Data","f");
   hfp_l1->AddEntry(simc_hxfp,"SIMC");
   hfp_l1->Draw();
   
   hfp->cd(2);
   simc_hyfp->DrawNormalized();
   data_hyfp->DrawNormalized("sameshist");
   hfp_l2->AddEntry(data_hyfp,"Data", "f");
   hfp_l2->AddEntry(simc_hyfp,"SIMC");
   hfp_l2->Draw();

   hfp->cd(3);
   simc_hxpfp->DrawNormalized();
   data_hxpfp->DrawNormalized("sameshist");
   hfp_l3->AddEntry(data_hxpfp,"Data", "f");
   hfp_l3->AddEntry(simc_hxpfp,"SIMC");
   hfp_l3->Draw();
     
   hfp->cd(4);
   simc_hypfp->DrawNormalized();
   data_hypfp->DrawNormalized("sameshist");
   hfp_l4->AddEntry(data_hypfp,"Data", "f");
   hfp_l4->AddEntry(simc_hypfp,"SIMC");
   hfp_l4->Draw();

   hfp->SaveAs(Form("hArm_FocalPlane_%d.pdf", run));                                                                                   

   //----------------------------------------------------------- 
 
  
}
