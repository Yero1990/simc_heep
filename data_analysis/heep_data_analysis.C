//Macro to analyze H(e,e'p) data from Hall C: HMS (electron arm), SHMS (hadron arm)

void heep_data_analysis(int run_num, TString electron)
{


  TString hadron;
 
  if (electron == "HMS")
    {
      hadron = "SHMS";
    }
  else
    {
      hadron = "HMS";
      electron = "SHMS";
    }

  //  TString ROOTfiles = "/Users/deuteron/HallC/hallc_replay/ROOTfiles/";
  //  TString file_name = "coin_replay_production_1929_-1.root";
    
  //Open data ROOTfile and call TTree
  TString file_path;
  file_path = Form("../ROOTfiles/coin_replay_production_%d_-1.root", run_num);
  TFile *data_file = new TFile(file_path, "READ");
//    TFile *data_file = new TFile("../ROOTfiles/coin_replay_production_2279_-1.root", "READ");

  TTree *T = (TTree*)data_file->Get("T");

  //Open root file where new histograms will be stored
  TFile *outfile = new TFile(Form("ep_coin_data_%d.root", run_num), "recreate");


  //These histograms binning MUST be exactly the same as those used in SIMC heep.C analysis

  //********* Create 1D Histograms **************
   Int_t bins = 300;

   //Kinematics Quantities
   TH1F *data_Emiss = new TH1F("data_Emiss","missing energy", bins, -0.5, 0.8);  //binwidth = 0.0025
   TH1F *data_pm = new TH1F("data_pm","missing momentum", bins, -0.5, 1.6);
   TH1F *data_Q2 = new TH1F("data_Q2","Q2", 100, 2., 14.);
   TH1F *data_omega = new TH1F("data_omega","Energy Transfer, #omega", bins, 0., 8.);
   TH1F *data_W_inv = new TH1F("data_W_inv", "Invariant Mass, W", bins, 0.6, 1.8);
   TH1F *data_theta_elec = new TH1F("data_theta_elec", "Electron Scatt. Angle", 100, 0.3, 0.45);
   TH1F *data_theta_prot = new TH1F("data_theta_prot", "Proton Scatt. Angle", 100, 0.5, 0.65);

   //Target Reconstruction Variables
   TH1F *data_x_tar = new TH1F("data_x_tar", "x_Target", bins, -0.25, 0.25);
   TH1F *data_y_tar = new TH1F("data_y_tar", "y_Target", bins, -0.25, 0.25);
   TH1F *data_z_tar = new TH1F("data_z_tar", "z_Target", bins, -5.5, 5.5);
   
   //Hadron arm Reconstructed Quantities ( xtar, ytar, xptar, yptar, delta)
   TH1F *data_hytar = new TH1F("data_hytar", hadron + "  Y_{tar}", bins, -4., 4.);
   TH1F *data_hxptar = new TH1F("data_hxptar", hadron + "  X'_{tar}", bins, -0.08, 0.08 );
   TH1F *data_hyptar = new TH1F("data_hyptar", hadron + "  Y'_{tar}", bins, -0.04, 0.04 );
   TH1F *data_hdelta = new TH1F("data_hdelta", hadron + "  Momentum Acceptance, #delta", bins, -10., 10. );

   //Hadron arm Focal Plane Quantities
   TH1F *data_hxfp = new TH1F("data_hxfp", hadron + "  X_{fp}", bins, -30., 20.);
   TH1F *data_hyfp = new TH1F("data_hyfp", hadron + "  Y_{fp}", bins, -15., 10.);
   TH1F *data_hxpfp = new TH1F("data_hxpfp", hadron + "  X'_{fp}", bins, -0.06, 0.06 );
   TH1F *data_hypfp = new TH1F("data_hypfp", hadron + "  Y'_{fp}", bins, -0.03, 0.02);

      
   //Electron Arm Reconstructed Quantities ( xtar, ytar, xptar, yptar, delta)
   TH1F *data_eytar = new TH1F("data_eytar", electron + "  Y_{tar}", bins, -4., 4.);
   TH1F *data_exptar = new TH1F("data_exptar", electron + "  X'_{tar}", bins, -0.08, 0.08);
   TH1F *data_eyptar = new TH1F("data_eyptar", electron + "  Y'_{tar}", bins, -0.04, 0.04);
   TH1F *data_edelta = new TH1F("data_edelta", electron + "  Momentum Acceptance, #delta", bins, -15., 15. );

   //Electron Arm Focal Plane Quantities
   TH1F *data_exfp = new TH1F("data_exfp", electron + "  X_{fp}", bins, -60., 30.);
   TH1F *data_eyfp = new TH1F("data_eyfp", electron + "  Y_{fp}", bins, -20., 30.);
   TH1F *data_expfp = new TH1F("data_expfp", electron + "  X'_{fp}", bins, -0.1, 0.06);
   TH1F *data_eypfp = new TH1F("data_eypfp", electron + "  Y'_{fp}", bins, -0.04, 0.04);



   //***************Create 2D Histograms at the Focal Plane Quantities*****************
   TH2F *data_h_xfp_vs_yfp = new TH2F("data_h_xfp_vs_yfp", "X_{fp} vs Y_{fp}", bins, -50., 20., bins, -10., 20.);
   
   
   /*********************************************/
   

   //Draw the Histograms from the TTree

   //Kinematics Quantities, P.* ->SHMS,  H.* -->HMS
   
   T->Draw("P.kin.secondary.emiss>>data_Emiss", "H.kin.primary.W<1.080");
   T->Draw("P.kin.secondary.pmiss>>data_pm", "H.kin.primary.W<1.080");
   T->Draw("H.kin.primary.Q2>>data_Q2", "H.kin.primary.W<1.080");
   T->Draw("H.kin.primary.nu>>data_omega", "H.kin.primary.W<1.080");
   T->Draw("H.kin.primary.W>>data_W_inv", "");
   T->Draw("H.kin.primary.scat_ang_rad>>data_theta_elec", "H.kin.primary.W<1.080");
   // T->Draw("P.kin.secondary.scat_ang_rad>>data_theta_prot") Missin leaf in data

   //Target Reconstruction Variables ????? What are these in data????
   //T->Draw("");
   //T->Draw("");
   //T->Draw("");

   //Hadron arm Reconstructed Quantities ( xtar, ytar, xptar, yptar, delta), theta: xptar, phi:yptar
   T->Draw("P.gtr.y>>data_hytar", "H.kin.primary.W<1.080");
   T->Draw("P.gtr.th>>data_hxptar", "H.kin.primary.W<1.080");
   T->Draw("P.gtr.ph>>data_hyptar", "H.kin.primary.W<1.080");
   T->Draw("P.gtr.dp>>data_hdelta", "H.kin.primary.W<1.080");

   //Hadron arm Focal Plane Quantities
   T->Draw("P.dc.x_fp>>data_hxfp", "H.kin.primary.W<1.080");
   T->Draw("P.dc.y_fp>>data_hyfp", "H.kin.primary.W<1.080");
   T->Draw("P.dc.xp_fp>>data_hxpfp", "H.kin.primary.W<1.080");
   T->Draw("P.dc.yp_fp>>data_hypfp", "H.kin.primary.W<1.080");
   
   //Electron arm Reconstructed Quantities ( xtar, ytar, xptar, yptar, delta), theta: xptar, phi:yptar
   T->Draw("H.gtr.y>>data_eytar", "H.kin.primary.W<1.080");
   T->Draw("H.gtr.th>>data_exptar", "H.kin.primary.W<1.080");
   T->Draw("H.gtr.ph>>data_eyptar", "H.kin.primary.W<1.080"); 
   T->Draw("H.gtr.dp>>data_edelta", "H.kin.primary.W<1.080");

   //Electron arm Focal Plane Quantities
   T->Draw("H.dc.x_fp>>data_exfp", "H.kin.primary.W<1.080");
   T->Draw("H.dc.y_fp>>data_eyfp", "H.kin.primary.W<1.080");
   T->Draw("H.dc.xp_fp>>data_expfp", "H.kin.primary.W<1.080");
   T->Draw("H.dc.yp_fp>>data_eypfp", "H.kin.primary.W<1.080");

   outfile->Write();

}
