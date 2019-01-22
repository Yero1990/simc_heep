//Macro to analyze H(e,e'p) E12-10-003 data from Hall C: HMS (Hadron arm), SHMS (electron arm)

#include "../simc_analysis/set_heep_histos.h"

//HMS Momentum correction: P0 = P0*0.9968`
//SHMS Momentum correction: P0 = P0*0.985

void analyze_singlesData(int runNUM, int evtNUM)
{

  
  //prevent root from displaying graphs while executing
  gROOT->SetBatch(1);

  TString hadron_arm;
  TString electron_arm;

  string primary = "H";   //electrons
  string secondary = "P";   //hadrons

  hadron_arm = "SHMS";
  electron_arm = "HMS";
  
  //Open data ROOTfile and call TTree
  TString file_path;
  file_path = Form("../../hallc_replay/ROOTfiles/hms_replay_delta_scan_%d_%d.root", runNUM, evtNUM);
 
  TFile *data_file = new TFile(file_path, "READ");

  TTree *T = (TTree*)data_file->Get("T");

  //Open root file where new histograms will be stored
  TFile *outfile = new TFile(Form("./delta_scan_data_%d_%d.root", runNUM, evtNUM), "recreate");


  //These histograms binning MUST be exactly the same as those used in SIMC heep.C analysis

  //Kinematics Quantities
   TH1F *data_Emiss = new TH1F("data_Emiss","missing energy", Em_nbins, Em_xmin, Em_xmax);      
   TH1F *data_pm = new TH1F("data_pm","missing momentum", Pm_nbins, Pm_xmin, Pm_xmax); 
   TH1F *data_Q2 = new TH1F("data_Q2","Q2", Q2_nbins, Q2_xmin, Q2_xmax);
   TH1F *data_omega = new TH1F("data_omega","Energy Transfer, #omega", om_nbins, om_xmin, om_xmax);
   TH1F *data_W_inv = new TH1F("data_W_inv", "Invariant Mass, W", W_nbins, W_xmin, W_xmax);     
   TH1F *data_theta_elec = new TH1F("data_theta_elec", "Electron Scatt. Angle", the_nbins, the_xmin, the_xmax);
   TH1F *data_theta_prot = new TH1F("data_theta_prot", "Proton Scatt. Angle", thp_nbins, thp_xmin, thp_xmax);

   //Additional Kinematics Variables
   TH1F *data_W2 = new TH1F("data_W2", "Invariant Mass W2", W2_nbins, W2_xmin, W2_xmax);
   TH1F *data_xbj = new TH1F("data_xbj", "x-Bjorken", xbj_nbins, xbj_xmin, xbj_xmax);
   TH1F *data_Pf = new TH1F("data_Pf", "Final Proton Momentum", Pf_nbins, Pf_xmin, Pf_xmax);
   TH1F *data_kf = new TH1F("data_kf", "Final e^{-} Momentum", kf_nbins, kf_xmin, kf_xmax);
   TH1F *data_theta_q = new TH1F("data_theta_q", "q-vector Angle, #theta_{q}", thq_nbins, thq_xmin, thq_xmax);

   
   //Target Reconstruction Variables
   TH1F *data_x_tar = new TH1F("data_x_tar", "x_Target", xtar_nbins, xtar_xmin, xtar_xmax);
   TH1F *data_y_tar = new TH1F("data_y_tar", "y_Target", ytar_nbins, ytar_xmin, ytar_xmax);
   TH1F *data_z_tar = new TH1F("data_z_tar", "z_Target", ztar_nbins, ztar_xmin, ztar_xmax);
   
   //Target_Recon, 2D
   //TH2F *data_reactz_vs_yptar = new TH2F("data_reactz_vs_yptar", electron_arm + "React Z vs. Y'_{tar}", eyptar_nbins, eyptar_xmin, eyptar_xmax, ztar_nbins, ztar_xmin, ztar_xmax);
   
   //Hadron arm Reconstructed Quantities ( xtar, ytar, xptar, yptar, delta)
   TH1F *data_hytar = new TH1F("data_hytar", hadron_arm + " Y_{tar}", hytar_nbins, hytar_xmin, hytar_xmax);
   TH1F *data_hxptar = new TH1F("data_hxptar", hadron_arm + " X'_{tar}", hxptar_nbins, hxptar_xmin, hxptar_xmax);
   TH1F *data_hyptar = new TH1F("data_hyptar", hadron_arm + " Y'_{tar}", hyptar_nbins, hyptar_xmin, hyptar_xmax );
   TH1F *data_hdelta = new TH1F("data_hdelta", hadron_arm + " Momentum Acceptance, #delta", hdelta_nbins, hdelta_xmin, hdelta_xmax);

   //Hadron arm Focal Plane Quantities
   TH1F *data_hxfp = new TH1F("data_hxfp", hadron_arm + " X_{fp}", hxfp_nbins, hxfp_xmin, hxfp_xmax);
   TH1F *data_hyfp = new TH1F("data_hyfp", hadron_arm + " Y_{fp}", hyfp_nbins, hyfp_xmin, hyfp_xmax);
   TH1F *data_hxpfp = new TH1F("data_hxpfp", hadron_arm + " X'_{fp}", hxpfp_nbins, hxpfp_xmin, hxpfp_xmax );
   TH1F *data_hypfp = new TH1F("data_hypfp", hadron_arm + " Y'_{fp}", hypfp_nbins, hypfp_xmin, hypfp_xmax);

      
   //Electron Arm Reconstructed Quantities ( xtar, ytar, xptar, yptar, delta)
   TH1F *data_eytar = new TH1F("data_eytar", electron_arm + " Y_{tar}", eytar_nbins, eytar_xmin, eytar_xmax);
   TH1F *data_exptar = new TH1F("data_exptar", electron_arm + " X'_{tar}", exptar_nbins, exptar_xmin, exptar_xmax);
   TH1F *data_eyptar = new TH1F("data_eyptar", electron_arm + " Y'_{tar}", eyptar_nbins, eyptar_xmin, eyptar_xmax);
   TH1F *data_edelta = new TH1F("data_edelta", electron_arm + " Momentum Acceptance, #delta", edelta_nbins, edelta_xmin, edelta_xmax);

   //Electron Arm Focal Plane Quantities
   TH1F *data_exfp = new TH1F("data_exfp", electron_arm + " X_{fp}", exfp_nbins, exfp_xmin, exfp_xmax);
   TH1F *data_eyfp = new TH1F("data_eyfp", electron_arm + " Y_{fp}", eyfp_nbins, eyfp_xmin, eyfp_xmax);
   TH1F *data_expfp = new TH1F("data_expfp", electron_arm + " X'_{fp}", expfp_nbins, expfp_xmin, expfp_xmax);
   TH1F *data_eypfp = new TH1F("data_eypfp", electron_arm + " Y'_{fp}", eypfp_nbins, eypfp_xmin, eypfp_xmax);

   //Cross-Check correlations
   TH2F *data_emiss_vs_pmiss = new TH2F("data_emiss_vs_pmiss", " E_{miss} vs. P_{miss}", Pm_nbins, Pm_xmin, Pm_xmax, Em_nbins, Em_xmin, Em_xmax);
   TH2F *data_edelta_vs_eyptar = new TH2F("data_edelta_vs_eyptar", electron_arm + " #delta vs. Y'_{tar}", eyptar_nbins, eyptar_xmin, eyptar_xmax, edelta_nbins, edelta_xmin, edelta_xmax);
   
   TH2F *data_emiss_vs_exptar = new TH2F("data_emiss_vs_exptar", " E_{miss} vs. X'_{tar}", exptar_nbins, exptar_xmin, exptar_xmax, Em_nbins, Em_xmin, Em_xmax);
   TH2F *data_emiss_vs_eyptar = new TH2F("data_emiss_vs_eyptar", " E_{miss} vs. Y'_{tar}", eyptar_nbins, eyptar_xmin, eyptar_xmax, Em_nbins, Em_xmin, Em_xmax);


   //Create 2D Histograms at the Focal Plane Quantities
   TH2F *data_hxfp_vs_hyfp = new TH2F("data_hxfp_vs_hyfp", "X_{fp} vs Y_{fp}", hyfp_nbins, hyfp_xmin, hyfp_xmax, hxfp_nbins, hxfp_xmin, hxfp_xmax);
   TH2F *data_exfp_vs_eyfp = new TH2F("data_exfp_vs_eyfp", "X_{fp} vs Y_{fp}", eyfp_nbins, eyfp_xmin, eyfp_xmax, exfp_nbins, exfp_xmin, exfp_xmax);

   //2D HMS v. SHMS Acceptance Correlations
   TH2F *data_hxptar_vs_exptar = new TH2F("data_hxptar_vs_exptar", "HMS vs. SHMS, X'_{tar}", exptar_nbins, exptar_xmin, exptar_xmax, hxptar_nbins, hxptar_xmin, hxptar_xmax);
   TH2F *data_hyptar_vs_eyptar = new TH2F("data_hyptar_vs_eyptar", "HMS vs. SHMS, Y'_{tar}", eyptar_nbins, eyptar_xmin, eyptar_xmax, hyptar_nbins, hyptar_xmin, hyptar_xmax);
   TH2F *data_hdelta_vs_edelta = new TH2F("data_hdelta_vs_edelta", "HMS vs. SHMS, #delta", edelta_nbins, edelta_xmin, edelta_xmax, hdelta_nbins, hdelta_xmin, hdelta_xmax);
   
   
   /*********************************************/
   
   //DEFINE PID CUTS
   TCut pcal = "P.cal.etot>0.1";
   TCut pelec = "P.ngcer.npeSum>1.0";
   
   TCut hprot = "H.cer.npeSum<1.0";
   TCut hcal = "H.cal.etot>0.1";

   //DEFINE KINEMATIC CUTS
   TCut W_cut = Form("%s.kin.W<1.05", primary.c_str());   //select events below pion thresshold
   //TCut em_cut = Form("%s.kin.secondary.emiss>-0.06&&%s.kin.secondary.emiss<0.08", secondary.c_str(), secondary.c_str());  
   TCut Q2_cut = Form("%s.kin.Q2>3&&%s.kin.Q2<5.0", primary.c_str(), primary.c_str());
   TCut xbj_cut = Form("%s.kin.x_bj>0.9", primary.c_str()); 


   //Draw the Histograms from the TTree

   //Kinematics Quantities, P.* ->SHMS,  H.* -->HMS
   //T->Draw(Form("%s.kin.secondary.emiss>>data_Emiss", secondary.c_str()));
   //T->Draw(Form("%s.kin.secondary.pmiss>>data_pm", secondary.c_str()), xbj_cut);
   T->Draw(Form("%s.kin.Q2>>data_Q2", primary.c_str()), xbj_cut);
   T->Draw(Form("%s.kin.nu>>data_omega", primary.c_str()), xbj_cut);
   T->Draw(Form("%s.kin.W>>data_W_inv", primary.c_str()), xbj_cut);
   T->Draw(Form("(%s.kin.scat_ang_rad*180./3.14)>>data_theta_elec", primary.c_str()), xbj_cut);

   //T->Draw(Form("(%s.kin.secondary.xangle - %s.kin.scat_ang_rad)*180./3.14>>data_theta_prot", secondary.c_str(), primary.c_str()), xbj_cut); 
   
   //Additional Kinematic Variables
   T->Draw(Form("%s.kin.W2>>data_W2", primary.c_str()), xbj_cut);
   T->Draw(Form("%s.kin.x_bj>>data_xbj", primary.c_str()), xbj_cut);
   //T->Draw(Form("%s.gtr.p>>data_Pf", secondary.c_str()), xbj_cut);
   T->Draw(Form("%s.gtr.p>>data_kf", primary.c_str()), xbj_cut);
   T->Draw(Form("%s.kin.th_q*(180./3.14)>>data_theta_q", primary.c_str()), xbj_cut ); //theta_q

   //Target Reconstruction Variables ????? What are these in data????
   T->Draw(Form("%s.react.x>>data_x_tar", primary.c_str()), xbj_cut);
   T->Draw(Form("%s.react.y>>data_y_tar", primary.c_str()), xbj_cut);
   T->Draw(Form("%s.react.z>>data_z_tar", primary.c_str()), xbj_cut);
		   
		   //Target Recon, 2d
   //T->Draw(Form("%s.react.z:%s.gtr.ph>>data_reactz_vs_yptar", primary.c_str(), primary.c_str()), xbj_cut);
   
   //Secondary arm Reconstructed Quantities ( xtar, ytar, xptar, yptar, delta), theta: xptar, phi:yptar
   //T->Draw(Form("%s.gtr.y>>data_hytar", secondary.c_str()), xbj_cut);
   //T->Draw(Form("%s.gtr.th>>data_hxptar", secondary.c_str()), xbj_cut);
   //T->Draw(Form("%s.gtr.ph>>data_hyptar", secondary.c_str()), xbj_cut);
   //T->Draw(Form("%s.gtr.dp>>data_hdelta", secondary.c_str()), xbj_cut);

   //Secondary arm Focal Plane Quantities
   //T->Draw(Form("%s.dc.x_fp>>data_hxfp", secondary.c_str()), xbj_cut);
   //T->Draw(Form("%s.dc.y_fp>>data_hyfp", secondary.c_str()), xbj_cut);
   //T->Draw(Form("%s.dc.xp_fp>>data_hxpfp", secondary.c_str()), xbj_cut);
   //T->Draw(Form("%s.dc.yp_fp>>data_hypfp", secondary.c_str()), xbj_cut);
   
   //Primary arm Reconstructed Quantities ( xtar, ytar, xptar, yptar, delta), theta: xptar, phi:yptar
   T->Draw(Form("%s.gtr.y>>data_eytar", primary.c_str()), xbj_cut);
   T->Draw(Form("%s.gtr.th>>data_exptar", primary.c_str()), xbj_cut);
   T->Draw(Form("%s.gtr.ph>>data_eyptar", primary.c_str()), xbj_cut); 
   T->Draw(Form("%s.gtr.dp>>data_edelta", primary.c_str()), xbj_cut);

   //Primary arm Focal Plane Quantities
   T->Draw(Form("%s.dc.x_fp>>data_exfp", primary.c_str()), xbj_cut);
   T->Draw(Form("%s.dc.y_fp>>data_eyfp", primary.c_str()), xbj_cut);
   T->Draw(Form("%s.dc.xp_fp>>data_expfp", primary.c_str()), xbj_cut);
   T->Draw(Form("%s.dc.yp_fp>>data_eypfp", primary.c_str()), xbj_cut);

   
   // TH2F General TTree::Draw Format:  T->Draw(Form("y:x>>(xbins, xmin, xmax, ybins, ymin, ymax)")   
   
   // Cross-Check correlations
   //T->Draw(Form("%s.kin.secondary.emiss:%s.kin.secondary.pmiss>>data_emiss_vs_pmiss", secondary.c_str(), secondary.c_str()), xbj_cut);
   T->Draw(Form("%s.gtr.dp:%s.gtr.ph>>data_edelta_vs_eyptar", primary.c_str(), primary.c_str()), xbj_cut);
   //T->Draw(Form("%s.kin.secondary.emiss:%s.gtr.th>>data_emiss_vs_exptar", secondary.c_str(), primary.c_str()), xbj_cut);
   //T->Draw(Form("%s.kin.secondary.emiss:%s.gtr.ph>>data_emiss_vs_eyptar", secondary.c_str(), primary.c_str()), xbj_cut);
   
   //2D Focal Plane Correlations
   //T->Draw(Form("%s.dc.x_fp:%s.dc.y_fp>>data_hxfp_vs_hyfp", secondary.c_str(), secondary.c_str()), xbj_cut);
   T->Draw(Form("%s.dc.x_fp:%s.dc.y_fp>>data_exfp_vs_eyfp", primary.c_str(), primary.c_str()), xbj_cut);
   
   //2D HMS v. SHMS Acceptance Correlations
   //T->Draw(Form("%s.gtr.th:%s.gtr.th>>data_hxptar_vs_exptar", secondary.c_str(), primary.c_str()), xbj_cut);
   //T->Draw(Form("%s.gtr.ph:%s.gtr.ph>>data_hyptar_vs_eyptar", secondary.c_str(), primary.c_str()), xbj_cut);
   //T->Draw(Form("%s.gtr.dp:%s.gtr.dp>>data_hdelta_vs_edelta", secondary.c_str(), primary.c_str()), xbj_cut);
   
   outfile->Write();
   outfile->Close();
   

   


}
