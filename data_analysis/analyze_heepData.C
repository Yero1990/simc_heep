#include "../simc_analysis/D2_heep_updated/set_heep_histos.h"

void analyze_heepData(int run, string eArm="")
{
  
  TString hadron_arm;
  TString electron_arm;
  //eArm = "H" or "P"
  string hArm;
  if(eArm=="P"){
    hArm="H";
    hadron_arm="HMS";
    electron_arm = "SHMS";
  }
  else if (eArm=="H")
    {
      hArm="P";
      hadron_arm="SHMS";
      electron_arm = "HMS";
    }

//PREVENT DISPLAY 
  //gROOT->SetBatch(kTRUE);
    
  //Read DATA ROOTfiles  
  TString filename =Form("../../hallc_replay/ROOTfiles/coin_replay_heep_check_%d_-1.root",run);        

  //HMS electron Heep Check data
  //TString filename =Form("../../hallc_replay/ROOTfiles/g%d_coin.root",run);        

  TFile *data_file = new TFile(filename, "READ"); 
  TTree *T = (TTree*)data_file->Get("T");
 
  //Create output root file where histograms will be stored
  TFile *outROOT = new TFile(Form("Wcheck_data_histos_%d.root",run), "recreate");
  

  //********* Create 1D Histograms **************
   
  TH1F *epCT = new TH1F("epCT","e-Proton Coincidence Time", 100, 0, 20);       //min width = 21.6 (0.0216)MeV,  COUNTS/25 MeV

  //Kinematics Quantities
  TH1F *Emiss = new TH1F("Emiss","missing energy", Em_nbins, Em_xmin, Em_xmax);       //min width = 21.6 (0.0216)MeV,  COUNTS/25 MeV
  TH1F *Emissv2 = new TH1F("Emissv2","missing energy", Em_nbins, Em_xmin, Em_xmax); 
  TH1F *pm = new TH1F("pm","missing momentum", Pm_nbins, Pm_xmin, Pm_xmax);  //min width = 32 MeV (0.032)
  TH1F *Q_2 = new TH1F("Q_2","Q2", Q2_nbins, Q2_xmin, Q2_xmax);
  TH1F *omega = new TH1F("omega","Energy Transfer, #omega", om_nbins, om_xmin, om_xmax);
  TH1F *W_inv = new TH1F("W_inv", "Invariant Mass, W", W_nbins, W_xmin, W_xmax);     //min width = 19.9 MeV (0.0199) (bin width = 25 MeV)
  TH1F *theta_elec = new TH1F("theta_elec", "Electron Scatt. Angle", the_nbins, the_xmin, the_xmax);
  TH1F *theta_prot = new TH1F("theta_prot", "Proton Scatt. Angle", thp_nbins, thp_xmin, thp_xmax);
  
  //Additional Kinematics Variables
  TH1F *W_2 = new TH1F("W2", "Invariant Mass W2", W2_nbins, W2_xmin, W2_xmax);
  TH1F *xbj = new TH1F("xbj", "x-Bjorken", xbj_nbins, xbj_xmin, xbj_xmax);
  TH1F *P_f = new TH1F("P_f", "Final Proton Momentum", Pf_nbins, Pf_xmin, Pf_xmax);
  TH1F *Ep_f = new TH1F("Ep_f", "Final Proton Energy", Ep_nbins, Ep_xmin, Ep_xmax);
  TH1F *k_f = new TH1F("kf", "Final e^{-} Momentum", kf_nbins, kf_xmin, kf_xmax);
  TH1F *theta_q = new TH1F("theta_q", "q-vector Angle, #theta_{q}", thq_nbins, thq_xmin, thq_xmax);
  TH1F *q_vec = new TH1F("q", "q-vector, |q|", q_nbins, q_xmin, q_xmax);
  TH1F *thet_pq = new TH1F("theta_pq", "(Proton, q-vector) Angle, #theta_{pq}", thpq_nbins, thpq_xmin, thpq_xmax);
  TH1F *thet_pq_v2 = new TH1F("theta_pq_v2", "(Proton, q-vector) Angle, #theta_{pq}", thpq_nbins, thpq_xmin, thpq_xmax);
  
  //Target Reconstruction Variables
  TH1F *hx_tar = new TH1F("hx_tar", hadron_arm + " x_Target", xtar_nbins, xtar_xmin, xtar_xmax);
  TH1F *hy_tar = new TH1F("hy_tar", hadron_arm + " y_Target", ytar_nbins, ytar_xmin, ytar_xmax);
  TH1F *hz_tar = new TH1F("hz_tar", hadron_arm + " z_Target", ztar_nbins, ztar_xmin, ztar_xmax);

  //Target Reconstruction Variables                                                                 
  TH1F *ex_tar = new TH1F("ex_tar", electron_arm + " x_Target", xtar_nbins, xtar_xmin, xtar_xmax);                  
  TH1F *ey_tar = new TH1F("ey_tar", electron_arm + " y_Target", ytar_nbins, ytar_xmin, ytar_xmax);                   
  TH1F *ez_tar = new TH1F("ez_tar", electron_arm + " z_Target", ztar_nbins, ztar_xmin, ztar_xmax);    

  //Hadron arm Reconstructed Quantities ( xtar, ytar, xptar, yptar, delta)
  TH1F *hytar = new TH1F("hytar", hadron_arm + " Y_{tar}", hytar_nbins, hytar_xmin, hytar_xmax);
  TH1F *hxptar = new TH1F("hxptar", hadron_arm + " X'_{tar}", hxptar_nbins, hxptar_xmin, hxptar_xmax);
  TH1F *hyptar = new TH1F("hyptar", hadron_arm + " Y'_{tar}", hyptar_nbins, hyptar_xmin, hyptar_xmax );
  TH1F *hdelta = new TH1F("hdelta", hadron_arm + " Momentum Acceptance, #delta", hdelta_nbins, hdelta_xmin, hdelta_xmax);
  
  //Hadron arm Focal Plane Quantities
  TH1F *hxfp = new TH1F("hxfp", hadron_arm + " X_{fp}", hxfp_nbins, hxfp_xmin, hxfp_xmax);
  TH1F *hyfp = new TH1F("hyfp", hadron_arm + " Y_{fp}", hyfp_nbins, hyfp_xmin, hyfp_xmax);
  TH1F *hxpfp = new TH1F("hxpfp", hadron_arm + " X'_{fp}", hxpfp_nbins, hxpfp_xmin, hxpfp_xmax );
  TH1F *hypfp = new TH1F("hypfp", hadron_arm + " Y'_{fp}", hypfp_nbins, hypfp_xmin, hypfp_xmax);

  
  //Electron Arm Reconstructed Quantities ( xtar, ytar, xptar, yptar, delta)
  TH1F *eytar = new TH1F("eytar", electron_arm + " Y_{tar}", eytar_nbins, eytar_xmin, eytar_xmax);
  TH1F *exptar = new TH1F("exptar", electron_arm + " X'_{tar}", exptar_nbins, exptar_xmin, exptar_xmax);
  TH1F *eyptar = new TH1F("eyptar", electron_arm + " Y'_{tar}", eyptar_nbins, eyptar_xmin, eyptar_xmax);
  TH1F *edelta = new TH1F("edelta", electron_arm + " Momentum Acceptance, #delta", edelta_nbins, edelta_xmin, edelta_xmax);
  
  //Electron Arm Focal Plane Quantities
  TH1F *exfp = new TH1F("exfp", electron_arm + " X_{fp}", exfp_nbins, exfp_xmin, exfp_xmax);
  TH1F *eyfp = new TH1F("eyfp", electron_arm + " Y_{fp}", eyfp_nbins, eyfp_xmin, eyfp_xmax);
  TH1F *expfp = new TH1F("expfp", electron_arm + " X'_{fp}", expfp_nbins, expfp_xmin, expfp_xmax);
  TH1F *eypfp = new TH1F("eypfp", electron_arm + " Y'_{fp}", eypfp_nbins, eypfp_xmin, eypfp_xmax);
  
  //Cross-Check correlations
  TH2F *emiss_vs_pmiss = new TH2F("emiss_vs_pmiss", " E_{miss} vs. P_{miss}", Pm_nbins, Pm_xmin, Pm_xmax, Em_nbins, Em_xmin, Em_xmax);
  TH2F *edelta_vs_eyptar = new TH2F("edelta_vs_eyptar", electron_arm + " #delta vs. Y'_{tar}", eyptar_nbins, eyptar_xmin, eyptar_xmax, edelta_nbins, edelta_xmin, edelta_xmax);
  
  
  //Create 2D Histograms at the Focal Plane Quantities
  TH2F *h_xfp_vs_yfp = new TH2F("h_xfp_vs_yfp", "X_{fp} vs Y_{fp}", hyfp_nbins, hyfp_xmin, hyfp_xmax, hxfp_nbins, hxfp_xmin, hxfp_xmax);
  TH2F *e_xfp_vs_yfp = new TH2F("e_xfp_vs_yfp", "X_{fp} vs Y_{fp}", eyfp_nbins, eyfp_xmin, eyfp_xmax, exfp_nbins, exfp_xmin, exfp_xmax);
  
  //2D HMS v. SHMS Acceptance Correlations
  TH2F *hxptar_vs_exptar = new TH2F("hxptar_vs_exptar", hadron_arm + " vs."+electron_arm+", X'_{tar}", exptar_nbins, exptar_xmin, exptar_xmax, hxptar_nbins, hxptar_xmin, hxptar_xmax);
  TH2F *hyptar_vs_eyptar = new TH2F("hyptar_vs_eyptar", hadron_arm + " vs."+electron_arm+ ", Y'_{tar}", eyptar_nbins, eyptar_xmin, eyptar_xmax, hyptar_nbins, hyptar_xmin, hyptar_xmax);
  TH2F *hdelta_vs_edelta = new TH2F("hdelta_vs_edelta", hadron_arm + " vs."+electron_arm+ ", #delta", edelta_nbins, edelta_xmin, edelta_xmax, hdelta_nbins, hdelta_xmin, hdelta_xmax);
  
  //OPTICS CHECK (W and Emiss Correlations with elestron-Arm Focal Plane / Target Quantities)
  TH2F *W_vs_exfp = new TH2F("W_vs_exfp", "W vs eX_{fp}", exfp_nbins, exfp_xmin, exfp_xmax, W_nbins, W_xmin, W_xmax);
  TH2F *W_vs_expfp = new TH2F("W_vs_expfp", "W vs eX'_{fp}", expfp_nbins, expfp_xmin, expfp_xmax, W_nbins, W_xmin, W_xmax);
  TH2F *W_vs_eyfp = new TH2F("W_vs_eyfp", "W vs eY_{fp}", eyfp_nbins, eyfp_xmin, eyfp_xmax, W_nbins, W_xmin, W_xmax);
  TH2F *W_vs_eypfp = new TH2F("W_vs_eypfp", "W vs eY'_{fp}", eypfp_nbins, eypfp_xmin, eypfp_xmax, W_nbins, W_xmin, W_xmax);
  
  TH2F *W_vs_eytar = new TH2F("W_vs_eytar", "W vs eY_{tar}", eytar_nbins, eytar_xmin, eytar_xmax, W_nbins, W_xmin, W_xmax);
  TH2F *W_vs_exptar = new TH2F("W_vs_exptar", "W vs eX'_{tar}", exptar_nbins, exptar_xmin, exptar_xmax, W_nbins, W_xmin, W_xmax);
  TH2F *W_vs_eyptar = new TH2F("W_vs_eyptar", "W vs eY'_{tar}", eyptar_nbins, eyptar_xmin, eyptar_xmax, W_nbins, W_xmin, W_xmax);
  TH2F *W_vs_edelta = new TH2F("W_vs_edelta", "W vs edelta", edelta_nbins, edelta_xmin, edelta_xmax, W_nbins, W_xmin, W_xmax);
  
  TH2F *Em_vs_exfp = new TH2F("Em_vs_exfp", "Em vs eX_{fp}", exfp_nbins, exfp_xmin, exfp_xmax, Em_nbins, Em_xmin, Em_xmax);
  TH2F *Em_vs_expfp = new TH2F("Em_vs_expfp", "Em vs eX'_{fp}", expfp_nbins, expfp_xmin, expfp_xmax, Em_nbins, Em_xmin, Em_xmax);
  TH2F *Em_vs_eyfp = new TH2F("Em_vs_eyfp", "Em vs eY_{fp}", eyfp_nbins, eyfp_xmin, eyfp_xmax, Em_nbins, Em_xmin, Em_xmax);
  TH2F *Em_vs_eypfp = new TH2F("Em_vs_eypfp", "Em vs eY'_{fp}", eypfp_nbins, eypfp_xmin, eypfp_xmax, Em_nbins, Em_xmin, Em_xmax);
  
  TH2F *Em_vs_eytar = new TH2F("Em_vs_eytar", "Em vs eY_{tar}", eytar_nbins, eytar_xmin, eytar_xmax, Em_nbins, Em_xmin, Em_xmax);
  TH2F *Em_vs_exptar = new TH2F("Em_vs_exptar", "Em vs eX'_{tar}", exptar_nbins, exptar_xmin, exptar_xmax, Em_nbins, Em_xmin, Em_xmax);
  TH2F *Em_vs_eyptar = new TH2F("Em_vs_eyptar", "Em vs eY'_{tar}", eyptar_nbins, eyptar_xmin, eyptar_xmax, Em_nbins, Em_xmin, Em_xmax);
  TH2F *Em_vs_edelta = new TH2F("Em_vs_edelta", "Em vs edelta", edelta_nbins, edelta_xmin, edelta_xmax, Em_nbins, Em_xmin, Em_xmax);
  

  //OPTICS CHECK (W and Emiss Correlations with hadron-Arm Focal Plane / Target Quantities)
  TH2F *W_vs_hxfp = new TH2F("W_vs_hxfp", "W vs hX_{fp}", hxfp_nbins, hxfp_xmin, hxfp_xmax, W_nbins, W_xmin, W_xmax);
  TH2F *W_vs_hxpfp = new TH2F("W_vs_hxpfp", "W vs hX'_{fp}", hxpfp_nbins, hxpfp_xmin, hxpfp_xmax, W_nbins, W_xmin, W_xmax);
  TH2F *W_vs_hyfp = new TH2F("W_vs_hyfp", "W vs hY_{fp}", hyfp_nbins, hyfp_xmin, hyfp_xmax, W_nbins, W_xmin, W_xmax);
  TH2F *W_vs_hypfp = new TH2F("W_vs_hypfp", "W vs hY'_{fp}", hypfp_nbins, hypfp_xmin, hypfp_xmax, W_nbins, W_xmin, W_xmax);
  
  TH2F *W_vs_hytar = new TH2F("W_vs_hytar", "W vs hY_{tar}", hytar_nbins, hytar_xmin, hytar_xmax, W_nbins, W_xmin, W_xmax);
  TH2F *W_vs_hxptar = new TH2F("W_vs_hxptar", "W vs hX'_{tar}", hxptar_nbins, hxptar_xmin, hxptar_xmax, W_nbins, W_xmin, W_xmax);
  TH2F *W_vs_hyptar = new TH2F("W_vs_hyptar", "W vs hY'_{tar}", hyptar_nbins, hyptar_xmin, hyptar_xmax, W_nbins, W_xmin, W_xmax);
  TH2F *W_vs_hdelta = new TH2F("W_vs_hdelta", "W vs hdelta", hdelta_nbins, hdelta_xmin, hdelta_xmax, W_nbins, W_xmin, W_xmax);
  
  TH2F *Em_vs_hxfp = new TH2F("Em_vs_hxfp", "Em vs hX_{fp}", hxfp_nbins, hxfp_xmin, hxfp_xmax, Em_nbins, Em_xmin, Em_xmax);
  TH2F *Em_vs_hxpfp = new TH2F("Em_vs_hxpfp", "Em vs hX'_{fp}", hxpfp_nbins, hxpfp_xmin, hxpfp_xmax, Em_nbins, Em_xmin, Em_xmax);
  TH2F *Em_vs_hyfp = new TH2F("Em_vs_hyfp", "Em vs hY_{fp}", hyfp_nbins, hyfp_xmin, hyfp_xmax, Em_nbins, Em_xmin, Em_xmax);
  TH2F *Em_vs_hypfp = new TH2F("Em_vs_hypfp", "Em vs hY'_{fp}", hypfp_nbins, hypfp_xmin, hypfp_xmax, Em_nbins, Em_xmin, Em_xmax);
  
  TH2F *Em_vs_hytar = new TH2F("Em_vs_hytar", "Em vs hY_{tar}", hytar_nbins, hytar_xmin, hytar_xmax, Em_nbins, Em_xmin, Em_xmax);
  TH2F *Em_vs_hxptar = new TH2F("Em_vs_hxptar", "Em vs hX'_{tar}", hxptar_nbins, hxptar_xmin, hxptar_xmax, Em_nbins, Em_xmin, Em_xmax);
  TH2F *Em_vs_hyptar = new TH2F("Em_vs_hyptar", "Em vs hY'_{tar}", hyptar_nbins, hyptar_xmin, hyptar_xmax, Em_nbins, Em_xmin, Em_xmax);
  TH2F *Em_vs_hdelta = new TH2F("Em_vs_hdelta", "Em vs hdelta", hdelta_nbins, hdelta_xmin, hdelta_xmax, Em_nbins, Em_xmin, Em_xmax);
 
  TH2F *dP_vs_reacty = new TH2F("dP_vs_reacty", "dP vs hreacsty", 100, -0.5, 0.5, 100 , -0.5, 0.5);


  /************Define Histos to APPLY CUTS*********************************/
  TH1F *cut_epCT = new TH1F("cut_epCT","e-Proton Coincidence Time", 100, 0, 20);       //min width = 21.6 (0.0216)MeV,  COUNTS/25 MeV

  //Kinematics Quantities
  TH1F *cut_Emiss = new TH1F("cut_Emiss","missing energy", Em_nbins, Em_xmin, Em_xmax);       //min width = 21.6 (0.0216)MeV,  CUT_OUNTS/25 MeV
  TH1F *cut_Emissv2 = new TH1F("cut_Emissv2","missing energy", Em_nbins, Em_xmin, Em_xmax);       //min width = 21.6 (0.0216)MeV,  CUT_OUNTS/25 MeV    
  TH1F *cut_pm = new TH1F("cut_pm","missing momentum", Pm_nbins, Pm_xmin, Pm_xmax);  //min width = 32 MeV (0.032)
  TH1F *cut_Q_2 = new TH1F("cut_Q_2","Q2", Q2_nbins, Q2_xmin, Q2_xmax);
  TH1F *cut_omega = new TH1F("cut_omega","Energy Transfer, #omega", om_nbins, om_xmin, om_xmax);
  TH1F *cut_W_inv = new TH1F("cut_W_inv", "Invariant Mass, W", W_nbins, W_xmin, W_xmax);     //min width = 19.9 MeV (0.0199) (bin width = 25 MeV)
  TH1F *cut_theta_elec = new TH1F("cut_theta_elec", "Electron Scatt. Angle", the_nbins, the_xmin, the_xmax);
  TH1F *cut_theta_prot = new TH1F("cut_theta_prot", "Proton Scatt. Angle", thp_nbins, thp_xmin, thp_xmax);
  
  //Additional Kinematics Variables
  TH1F *cut_W_2 = new TH1F("cut_W2", "Invariant Mass W2", W2_nbins, W2_xmin, W2_xmax);
  TH1F *cut_xbj = new TH1F("cut_xbj", "x-Bjorken", xbj_nbins, xbj_xmin, xbj_xmax);
  TH1F *cut_P_f = new TH1F("cut_P_f", "Final Proton Momentum", Pf_nbins, Pf_xmin, Pf_xmax);
  TH1F *cut_Ep_f = new TH1F("cut_Ep_f", "Final Proton Energy", Ep_nbins, Ep_xmin, Ep_xmax); 
  TH1F *cut_k_f = new TH1F("cut_kf", "Final e^{-} Momentum", kf_nbins, kf_xmin, kf_xmax);
  TH1F *cut_q_vec = new TH1F("cut_q", "q-vector, |q|", q_nbins, q_xmin, q_xmax);
  TH1F *cut_theta_q = new TH1F("cut_theta_q", "q-vector Angle, #theta_{q}", thq_nbins, thq_xmin, thq_xmax);
  TH1F *cut_thet_pq = new TH1F("cut_theta_pq", "(Proton, q-vector) Angle, #theta_{pq}", thpq_nbins, thpq_xmin, thpq_xmax);
  TH1F *cut_thet_pq_v2 = new TH1F("cut_theta_pq_v2", "(Proton, q-vector) Angle, #theta_{pq}", thpq_nbins, thpq_xmin, thpq_xmax);

  
  //Hadron Arm Target Reconstruction Variables
  TH1F *cut_hx_tar = new TH1F("cut_hx_tar", hadron_arm + " x_Target", xtar_nbins, xtar_xmin, xtar_xmax);
  TH1F *cut_hy_tar = new TH1F("cut_hy_tar", hadron_arm + " y_Target", ytar_nbins, ytar_xmin, ytar_xmax);
  TH1F *cut_hz_tar = new TH1F("cut_hz_tar", hadron_arm + " z_Target", ztar_nbins, ztar_xmin, ztar_xmax);

  //Electron Arm Target Reconstruction Variables    
  TH1F *cut_ex_tar = new TH1F("cut_ex_tar", electron_arm + " x_Target", xtar_nbins, xtar_xmin, xtar_xmax);     
  TH1F *cut_ey_tar = new TH1F("cut_ey_tar", electron_arm + " y_Target", ytar_nbins, ytar_xmin, ytar_xmax);      
  TH1F *cut_ez_tar = new TH1F("cut_ez_tar", electron_arm + " z_Target", ztar_nbins, ztar_xmin, ztar_xmax);  
  
  //Hadron arm Reconstructed Quantities ( xtar, ytar, xptar, yptar, delta)
  TH1F *cut_hytar = new TH1F("cut_hytar", hadron_arm + " Y_{tar}", hytar_nbins, hytar_xmin, hytar_xmax);
  TH1F *cut_hxptar = new TH1F("cut_hxptar", hadron_arm + " X'_{tar}", hxptar_nbins, hxptar_xmin, hxptar_xmax);
  TH1F *cut_hyptar = new TH1F("cut_hyptar", hadron_arm + " Y'_{tar}", hyptar_nbins, hyptar_xmin, hyptar_xmax );
  TH1F *cut_hdelta = new TH1F("cut_hdelta", hadron_arm + " Momentum Acceptance, #delta", hdelta_nbins, hdelta_xmin, hdelta_xmax);
  
  //Hadron arm Focal Plane Quantities
  TH1F *cut_hxfp = new TH1F("cut_hxfp", hadron_arm + " X_{fp}", hxfp_nbins, hxfp_xmin, hxfp_xmax);
  TH1F *cut_hyfp = new TH1F("cut_hyfp", hadron_arm + " Y_{fp}", hyfp_nbins, hyfp_xmin, hyfp_xmax);
  TH1F *cut_hxpfp = new TH1F("cut_hxpfp", hadron_arm + " X'_{fp}", hxpfp_nbins, hxpfp_xmin, hxpfp_xmax );
  TH1F *cut_hypfp = new TH1F("cut_hypfp", hadron_arm + " Y'_{fp}", hypfp_nbins, hypfp_xmin, hypfp_xmax);
  
  
  //Electron Arm Reconstructed Quantities ( xtar, ytar, xptar, yptar, delta)
  TH1F *cut_eytar = new TH1F("cut_eytar", electron_arm + " Y_{tar}", eytar_nbins, eytar_xmin, eytar_xmax);
  TH1F *cut_exptar = new TH1F("cut_exptar", electron_arm + " X'_{tar}", exptar_nbins, exptar_xmin, exptar_xmax);
  TH1F *cut_eyptar = new TH1F("cut_eyptar", electron_arm + " Y'_{tar}", eyptar_nbins, eyptar_xmin, eyptar_xmax);
  TH1F *cut_edelta = new TH1F("cut_edelta", electron_arm + " Momentum Acceptance, #delta", edelta_nbins, edelta_xmin, edelta_xmax);
  
  //Electron Arm Focal Plane Quantities
  TH1F *cut_exfp = new TH1F("cut_exfp", electron_arm + " X_{fp}", exfp_nbins, exfp_xmin, exfp_xmax);
  TH1F *cut_eyfp = new TH1F("cut_eyfp", electron_arm + " Y_{fp}", eyfp_nbins, eyfp_xmin, eyfp_xmax);
  TH1F *cut_expfp = new TH1F("cut_expfp", electron_arm + " X'_{fp}", expfp_nbins, expfp_xmin, expfp_xmax);
  TH1F *cut_eypfp = new TH1F("cut_eypfp", electron_arm + " Y'_{fp}", eypfp_nbins, eypfp_xmin, eypfp_xmax);
  
  //Cross-Check correlations
  TH2F *cut_emiss_vs_pmiss = new TH2F("cut_emiss_vs_pmiss", " E_{miss} vs. P_{miss}", Pm_nbins, Pm_xmin, Pm_xmax, Em_nbins, Em_xmin, Em_xmax);
  TH2F *cut_edelta_vs_eyptar = new TH2F("cut_edelta_vs_eyptar", electron_arm + " #delta vs. Y'_{tar}", eyptar_nbins, eyptar_xmin, eyptar_xmax, edelta_nbins, edelta_xmin, edelta_xmax);
  
  
  //Create 2D Histograms at the Focal Plane Quantities
  TH2F *cut_h_xfp_vs_yfp = new TH2F("cut_h_xfp_vs_yfp", "X_{fp} vs Y_{fp}", hyfp_nbins, hyfp_xmin, hyfp_xmax, hxfp_nbins, hxfp_xmin, hxfp_xmax);
  TH2F *cut_e_xfp_vs_yfp = new TH2F("cut_e_xfp_vs_yfp", "X_{fp} vs Y_{fp}", eyfp_nbins, eyfp_xmin, eyfp_xmax, exfp_nbins, exfp_xmin, exfp_xmax);
  
  //2D HMS v. SHMS Acceptance Correlations
  TH2F *cut_hxptar_vs_exptar = new TH2F("cut_hxptar_vs_exptar", hadron_arm + " vs."+electron_arm+", X'_{tar}", exptar_nbins, exptar_xmin, exptar_xmax, hxptar_nbins, hxptar_xmin, hxptar_xmax);
  TH2F *cut_hyptar_vs_eyptar = new TH2F("cut_hyptar_vs_eyptar", hadron_arm + " vs."+electron_arm+", Y'_{tar}", eyptar_nbins, eyptar_xmin, eyptar_xmax, hyptar_nbins, hyptar_xmin, hyptar_xmax);
  TH2F *cut_hdelta_vs_edelta = new TH2F("cut_hdelta_vs_edelta", hadron_arm + " vs."+electron_arm+", #delta", edelta_nbins, edelta_xmin, edelta_xmax, hdelta_nbins, hdelta_xmin, hdelta_xmax);
  
  //OPTICS CHECK (W correlations with electron arm Focal Plane / Target Quantities)
  TH2F *cut_W_vs_exfp = new TH2F("cut_W_vs_exfp", "cut_W vs eX_{fp}", exfp_nbins, exfp_xmin, exfp_xmax, W_nbins, W_xmin, W_xmax);
  TH2F *cut_W_vs_expfp = new TH2F("cut_W_vs_expfp", "cut_W vs eX'_{fp}", expfp_nbins, expfp_xmin, expfp_xmax, W_nbins, W_xmin, W_xmax);
  TH2F *cut_W_vs_eyfp = new TH2F("cut_W_vs_eyfp", "cut_W vs eY_{fp}", eyfp_nbins, eyfp_xmin, eyfp_xmax, W_nbins, W_xmin, W_xmax);
  TH2F *cut_W_vs_eypfp = new TH2F("cut_W_vs_eypfp", "cut_W vs eY'_{fp}", eypfp_nbins, eypfp_xmin, eypfp_xmax, W_nbins, W_xmin, W_xmax);
  
  TH2F *cut_W_vs_eytar = new TH2F("cut_W_vs_eytar", "cut_W vs eY_{tar}", eytar_nbins, eytar_xmin, eytar_xmax, W_nbins, W_xmin, W_xmax);
  TH2F *cut_W_vs_exptar = new TH2F("cut_W_vs_exptar", "cut_W vs eX'_{tar}", exptar_nbins, exptar_xmin, exptar_xmax, W_nbins, W_xmin, W_xmax);
  TH2F *cut_W_vs_eyptar = new TH2F("cut_W_vs_eyptar", "cut_W vs eY'_{tar}", eyptar_nbins, eyptar_xmin, eyptar_xmax, W_nbins, W_xmin, W_xmax);
  TH2F *cut_W_vs_edelta = new TH2F("cut_W_vs_edelta", "cut_W vs edelta", edelta_nbins, edelta_xmin, edelta_xmax, W_nbins, W_xmin, W_xmax);
  
  TH2F *cut_Em_vs_exfp = new TH2F("cut_Em_vs_exfp", "cut_Em vs eX_{fp}", exfp_nbins, exfp_xmin, exfp_xmax, Em_nbins, Em_xmin, Em_xmax);
  TH2F *cut_Em_vs_expfp = new TH2F("cut_Em_vs_expfp", "cut_Em vs eX'_{fp}", expfp_nbins, expfp_xmin, expfp_xmax, Em_nbins, Em_xmin, Em_xmax);
  TH2F *cut_Em_vs_eyfp = new TH2F("cut_Em_vs_eyfp", "cut_Em vs eY_{fp}", eyfp_nbins, eyfp_xmin, eyfp_xmax, Em_nbins, Em_xmin, Em_xmax);
  TH2F *cut_Em_vs_eypfp = new TH2F("cut_Em_vs_eypfp", "cut_Em vs eY'_{fp}", eypfp_nbins, eypfp_xmin, eypfp_xmax, Em_nbins, Em_xmin, Em_xmax);
  
  TH2F *cut_Em_vs_eytar = new TH2F("cut_Em_vs_eytar", "cut_Em vs eY_{tar}", eytar_nbins, eytar_xmin, eytar_xmax, Em_nbins, Em_xmin, Em_xmax);
  TH2F *cut_Em_vs_exptar = new TH2F("cut_Em_vs_exptar", "cut_Em vs eX'_{tar}", exptar_nbins, exptar_xmin, exptar_xmax, Em_nbins, Em_xmin, Em_xmax);
  TH2F *cut_Em_vs_eyptar = new TH2F("cut_Em_vs_eyptar", "cut_Em vs eY'_{tar}", eyptar_nbins, eyptar_xmin, eyptar_xmax, Em_nbins, Em_xmin, Em_xmax);
  TH2F *cut_Em_vs_edelta = new TH2F("cut_Em_vs_edelta", "cut_Em vs edelta", edelta_nbins, edelta_xmin, edelta_xmax, Em_nbins, Em_xmin, Em_xmax);
  
  //OPTICS CHECK (W correlations with hadron arm Focal Plane / Target Quantities)
  TH2F *cut_W_vs_hxfp = new TH2F("cut_W_vs_hxfp", "cut_W vs hX_{fp}", hxfp_nbins, hxfp_xmin, hxfp_xmax, W_nbins, W_xmin, W_xmax);
  TH2F *cut_W_vs_hxpfp = new TH2F("cut_W_vs_hxpfp", "cut_W vs hX'_{fp}", hxpfp_nbins, hxpfp_xmin, hxpfp_xmax, W_nbins, W_xmin, W_xmax);
  TH2F *cut_W_vs_hyfp = new TH2F("cut_W_vs_hyfp", "cut_W vs hY_{fp}", hyfp_nbins, hyfp_xmin, hyfp_xmax, W_nbins, W_xmin, W_xmax);
  TH2F *cut_W_vs_hypfp = new TH2F("cut_W_vs_hypfp", "cut_W vs hY'_{fp}", hypfp_nbins, hypfp_xmin, hypfp_xmax, W_nbins, W_xmin, W_xmax);
  
  TH2F *cut_W_vs_hytar = new TH2F("cut_W_vs_hytar", "cut_W vs hY_{tar}", hytar_nbins, hytar_xmin, hytar_xmax, W_nbins, W_xmin, W_xmax);
  TH2F *cut_W_vs_hxptar = new TH2F("cut_W_vs_hxptar", "cut_W vs hX'_{tar}", hxptar_nbins, hxptar_xmin, hxptar_xmax, W_nbins, W_xmin, W_xmax);
  TH2F *cut_W_vs_hyptar = new TH2F("cut_W_vs_hyptar", "cut_W vs hY'_{tar}", hyptar_nbins, hyptar_xmin, hyptar_xmax, W_nbins, W_xmin, W_xmax);
  TH2F *cut_W_vs_hdelta = new TH2F("cut_W_vs_hdelta", "cut_W vs hdelta", hdelta_nbins, hdelta_xmin, hdelta_xmax, W_nbins, W_xmin, W_xmax);
  
  TH2F *cut_Em_vs_hxfp = new TH2F("cut_Em_vs_hxfp", "cut_Em vs hX_{fp}", hxfp_nbins, hxfp_xmin, hxfp_xmax, Em_nbins, Em_xmin, Em_xmax);
  TH2F *cut_Em_vs_hxpfp = new TH2F("cut_Em_vs_hxpfp", "cut_Em vs hX'_{fp}", hxpfp_nbins, hxpfp_xmin, hxpfp_xmax, Em_nbins, Em_xmin, Em_xmax);
  TH2F *cut_Em_vs_hyfp = new TH2F("cut_Em_vs_hyfp", "cut_Em vs hY_{fp}", hyfp_nbins, hyfp_xmin, hyfp_xmax, Em_nbins, Em_xmin, Em_xmax);
  TH2F *cut_Em_vs_hypfp = new TH2F("cut_Em_vs_hypfp", "cut_Em vs hY'_{fp}", hypfp_nbins, hypfp_xmin, hypfp_xmax, Em_nbins, Em_xmin, Em_xmax);
  
  TH2F *cut_Em_vs_hytar = new TH2F("cut_Em_vs_hytar", "cut_Em vs hY_{tar}", hytar_nbins, hytar_xmin, hytar_xmax, Em_nbins, Em_xmin, Em_xmax);
  TH2F *cut_Em_vs_hxptar = new TH2F("cut_Em_vs_hxptar", "cut_Em vs hX'_{tar}", hxptar_nbins, hxptar_xmin, hxptar_xmax, Em_nbins, Em_xmin, Em_xmax);
  TH2F *cut_Em_vs_hyptar = new TH2F("cut_Em_vs_hyptar", "cut_Em vs hY'_{tar}", hyptar_nbins, hyptar_xmin, hyptar_xmax, Em_nbins, Em_xmin, Em_xmax);
  TH2F *cut_Em_vs_hdelta = new TH2F("cut_Em_vs_hdelta", "cut_Em vs hdelta", hdelta_nbins, hdelta_xmin, hdelta_xmax, Em_nbins, Em_xmin, Em_xmax);
  

  //Set Variable Names and Branches
 
  //------Kinematics
  Double_t  theta_e;
  Double_t  W;
  Double_t  Q2;
  Double_t  X;
  Double_t  nu;
  Double_t  q3m;   //q-vect magnitude
  Double_t  th_q;
  Double_t  kf;
  Double_t  Pf;
  Double_t  Em;
  Double_t  Emv2;
  Double_t  Pm;
  Double_t  thbq;
  Double_t  thxq;
  Double_t  xangle;
  Double_t  theta_p;  //to be determined in loop
  Double_t  theta_pq_v2; //to be determined in loop
  Double_t  W2;
  Double_t  Ep;
  Double_t  epCoinTime;
  Double_t  pindex;
  
  T->SetBranchAddress("CTime.epCoinTime_ROC2", &epCoinTime);

  T->SetBranchAddress(Form("%s.kin.primary.scat_ang_rad", eArm.c_str()),&theta_e);
  T->SetBranchAddress(Form("%s.kin.primary.W", eArm.c_str()),&W);
  T->SetBranchAddress(Form("%s.kin.primary.Q2", eArm.c_str()),&Q2);
  T->SetBranchAddress(Form("%s.kin.primary.x_bj", eArm.c_str()),&X);
  T->SetBranchAddress(Form("%s.kin.primary.nu", eArm.c_str()),&nu);
  T->SetBranchAddress(Form("%s.kin.primary.q3m", eArm.c_str()),&q3m);
  T->SetBranchAddress(Form("%s.kin.primary.th_q", eArm.c_str()),&th_q);
  T->SetBranchAddress(Form("%s.gtr.p", eArm.c_str()),&kf);
  T->SetBranchAddress(Form("%s.gtr.p", hArm.c_str()),&Pf);
  T->SetBranchAddress(Form("%s.gtr.index",eArm.c_str()),&pindex);

  T->SetBranchAddress(Form("%s.kin.secondary.emiss", hArm.c_str()),&Em);
  T->SetBranchAddress(Form("%s.kin.secondary.pmiss", hArm.c_str()),&Pm);
  T->SetBranchAddress(Form("%s.kin.secondary.th_bq", hArm.c_str()),&thbq);      //Polar angle of recoil system with q (rad)
  T->SetBranchAddress(Form("%s.kin.secondary.th_xq", hArm.c_str()),&thxq);     //Polar angle of detected particle with q
  T->SetBranchAddress(Form("%s.kin.secondary.xangle", hArm.c_str()),&xangle);  //Angle of detected particle with scattered electron (Used to determine hadron angle)
  


  //-------Electron Arm Focal Plane / Target
  Double_t  e_xfp;
  Double_t  e_xpfp;
  Double_t  e_yfp;
  Double_t  e_ypfp;
  Double_t  e_ytar;
  Double_t  e_yptar;
  Double_t  e_xptar;
  Double_t  e_delta;
  
  T->SetBranchAddress(Form("%s.dc.x_fp", eArm.c_str()),&e_xfp);
  T->SetBranchAddress(Form("%s.dc.xp_fp", eArm.c_str()),&e_xpfp);
  T->SetBranchAddress(Form("%s.dc.y_fp", eArm.c_str()),&e_yfp);
  T->SetBranchAddress(Form("%s.dc.yp_fp", eArm.c_str()),&e_ypfp);
  
  T->SetBranchAddress(Form("%s.gtr.y", eArm.c_str()),&e_ytar);
  T->SetBranchAddress(Form("%s.gtr.ph", eArm.c_str()),&e_yptar);
  T->SetBranchAddress(Form("%s.gtr.th", eArm.c_str()),&e_xptar);
  T->SetBranchAddress(Form("%s.gtr.dp", eArm.c_str()),&e_delta);
  
  //-------Hadron Focal Plane / Target
  Double_t  h_xfp;
  Double_t  h_xpfp;
  Double_t  h_yfp;
  Double_t  h_ypfp;
  
  Double_t  h_ytar;
  Double_t  h_yptar;
  Double_t  h_xptar;
  Double_t  h_delta;
  
  T->SetBranchAddress(Form("%s.dc.x_fp", hArm.c_str()),&h_xfp);
  T->SetBranchAddress(Form("%s.dc.xp_fp", hArm.c_str()),&h_xpfp);
  T->SetBranchAddress(Form("%s.dc.y_fp", hArm.c_str()),&h_yfp);
  T->SetBranchAddress(Form("%s.dc.yp_fp", hArm.c_str()),&h_ypfp);
  
  T->SetBranchAddress(Form("%s.gtr.y", hArm.c_str()),&h_ytar);
  T->SetBranchAddress(Form("%s.gtr.ph", hArm.c_str()),&h_yptar);
  T->SetBranchAddress(Form("%s.gtr.th", hArm.c_str()),&h_xptar);
  T->SetBranchAddress(Form("%s.gtr.dp", hArm.c_str()),&h_delta);

  
  //--------Target Quantities (tarx, tary, tarz)
  Double_t  htar_x;
  Double_t  htar_y;
  Double_t  htar_z;
  
  Double_t  etar_x;                                                                                           
  Double_t  etar_y;                                                                             
  Double_t  etar_z;

  T->SetBranchAddress(Form("%s.react.x", hArm.c_str()),&htar_x);
  T->SetBranchAddress(Form("%s.react.y", hArm.c_str()),&htar_y);
  T->SetBranchAddress(Form("%s.react.z", hArm.c_str()),&htar_z);
  
  T->SetBranchAddress(Form("%s.react.x", eArm.c_str()),&etar_x);
  T->SetBranchAddress(Form("%s.react.y", eArm.c_str()),&etar_y);
  T->SetBranchAddress(Form("%s.react.z", eArm.c_str()),&etar_z);


  //------SHMS Detector Quantities
  Double_t  pcal_etottracknorm;
  Double_t  pngcer_npesum;

  T->SetBranchAddress("P.cal.etottracknorm",&pcal_etottracknorm);
  T->SetBranchAddress("P.ngcer.npeSum",&pngcer_npesum);
  
 
  //Define Boolean for Kin. Cuts
  Bool_t c_Em;
  Bool_t c_hdelta;
  Bool_t c_ecal;
  Bool_t c_ctime;

  //======================
  // E V E N T   L O O P 
  //======================
  
  
  Long64_t nentries = T->GetEntries();
  
  cout << "nentries = " << nentries << endl;
  
  
  
   for (Long64_t i=0; i<nentries;i++) {
  //for (Long64_t i=0; i<100000;i++) {

    T->GetEntry(i);
    
    
    //Determine additional kinematics
    theta_p = xangle - theta_e;
    W2 = W*2;
    theta_pq_v2 = th_q - theta_p;
    Ep = TMath::Sqrt(MP*MP + Pf*Pf);
    Emv2 = nu + MP - Ep;

    c_Em = Em < -0.04;                     //Missing energy < 0.04 (40 MeV)
    c_hdelta = h_delta>-8.&&h_delta<8.;  //good HMS delta range (well known recon. matrix)
    c_ecal = pcal_etottracknorm > 0.85;   //reject pions
    c_ctime = epCoinTime>8.6 && epCoinTime<13.6;

    //APPLY CUTS: BEGIN CUTS LOOP
    //if (c_Em&&c_ctime&&c_hdelta&&c_ecal&&pindex>-1&&e_delta>-10.&&e_delta<22.)
    if (c_Em)
    {     

	  cut_epCT->Fill(epCoinTime);

	  //Kinematics
	  cut_Emiss->Fill(Em);
	  cut_Emissv2->Fill(Emv2);
	  cut_pm->Fill(Pm);
	  cut_Q_2->Fill(Q2);
	  cut_omega->Fill(nu);
	  cut_W_inv->Fill(W);
	  cut_theta_elec->Fill(theta_e/dtr);
	  cut_theta_prot->Fill(theta_p/dtr);

	  
	  //Additional Kinematics Variables
	  cut_W_2->Fill(W2); 
	  cut_xbj->Fill(X); 
	  cut_P_f->Fill(Pf);
	  cut_Ep_f->Fill(Ep);
	  cut_k_f->Fill(kf);
	  cut_theta_q->Fill(th_q/dtr);
	  cut_q_vec->Fill(q3m);
	  cut_thet_pq->Fill(thxq/dtr);
	  cut_thet_pq_v2->Fill(theta_pq_v2/dtr);

	  //Reconstructed Target Quantities (Lab Frame)
	  cut_hx_tar->Fill(htar_x);
	  cut_hy_tar->Fill(htar_y);
	  cut_hz_tar->Fill(htar_z);
	  
	  cut_ex_tar->Fill(etar_x); 
          cut_ey_tar->Fill(etar_y);  
          cut_ez_tar->Fill(etar_z);                                              
	  
	  //Hadron-Arm Target Reconstruction 
	  cut_hytar->Fill(h_ytar);
	  cut_hxptar->Fill(h_xptar);
	  cut_hyptar->Fill(h_yptar);
	  cut_hdelta->Fill(h_delta);
	  
	  //Hadron-Arm Focal Plane
	  cut_hxfp->Fill(h_xfp);
	  cut_hyfp->Fill(h_yfp);
	  cut_hxpfp->Fill(h_xpfp);
	  cut_hypfp->Fill(h_ypfp);
	  
	  //Electron-Arm Target Reconstruction
	  cut_eytar->Fill(e_ytar);
	  cut_exptar->Fill(e_xptar);
	  cut_eyptar->Fill(e_yptar);
	  cut_edelta->Fill(e_delta);
	  
	  //Electron-Arm Focal Plane
	  cut_exfp->Fill(e_xfp);
	  cut_eyfp->Fill(e_yfp);
	  cut_expfp->Fill(e_xpfp);
	  cut_eypfp->Fill(e_ypfp);
	  

	  //Fill 2D HMS Focal Plane Quantities
	  cut_h_xfp_vs_yfp->Fill(h_yfp, h_xfp);
	  cut_e_xfp_vs_yfp->Fill(e_yfp, e_xfp);
	  
	  //Fill 2D reconstructed variables
	  cut_hxptar_vs_exptar->Fill(e_xptar, h_xptar);
	  cut_hyptar_vs_eyptar->Fill(e_yptar, h_yptar);
	  cut_hdelta_vs_edelta->Fill(e_delta, h_delta);

	  
	  //Heep cross check
	  cut_emiss_vs_pmiss->Fill(Pm, Em);
	  cut_edelta_vs_eyptar->Fill(e_yptar, e_delta);
	  
	  //OPTICS CHECK (W / Emiss vs. electron Arm Quantities)
	  cut_W_vs_exfp->Fill(e_xfp, W);
	  cut_W_vs_expfp->Fill(e_xpfp, W);
	  cut_W_vs_eyfp->Fill(e_yfp, W);
	  cut_W_vs_eypfp->Fill(e_ypfp, W);
	  
	  cut_W_vs_eytar->Fill(e_ytar, W);
	  cut_W_vs_exptar->Fill(e_xptar, W);
	  cut_W_vs_eyptar->Fill(e_yptar, W);
	  cut_W_vs_edelta->Fill(e_delta, W);

	  cut_Em_vs_exfp->Fill(e_xfp, Em);
	  cut_Em_vs_expfp->Fill(e_xpfp, Em);
	  cut_Em_vs_eyfp->Fill(e_yfp, Em);
	  cut_Em_vs_eypfp->Fill(e_ypfp, Em);
	  
	  cut_Em_vs_eytar->Fill(e_ytar, Em);
	  cut_Em_vs_exptar->Fill(e_xptar, Em);
	  cut_Em_vs_eyptar->Fill(e_yptar, Em);
	  cut_Em_vs_edelta->Fill(e_delta, Em);
	  
	  //OPTICS CHECK (W / Emiss vs. hadron Arm Quantities)
	  cut_W_vs_hxfp->Fill(h_xfp, W);
	  cut_W_vs_hxpfp->Fill(h_xpfp, W);
	  cut_W_vs_hyfp->Fill(h_yfp, W);
	  cut_W_vs_hypfp->Fill(h_ypfp, W);
	  
	  cut_W_vs_hytar->Fill(h_ytar, W);
	  cut_W_vs_hxptar->Fill(h_xptar, W);
	  cut_W_vs_hyptar->Fill(h_yptar, W);
	  cut_W_vs_hdelta->Fill(h_delta, W);

	  cut_Em_vs_hxfp->Fill(h_xfp, Em);
	  cut_Em_vs_hxpfp->Fill(h_xpfp, Em);
	  cut_Em_vs_hyfp->Fill(h_yfp, Em);
	  cut_Em_vs_hypfp->Fill(h_ypfp, Em);
	  
	  cut_Em_vs_hytar->Fill(h_ytar, Em);
	  cut_Em_vs_hxptar->Fill(h_xptar, Em);
	  cut_Em_vs_hyptar->Fill(h_yptar, Em);
	  cut_Em_vs_hdelta->Fill(h_delta, Em);
	  
	}//End CUTS LOOP
      
      
      epCT->Fill(epCoinTime);

      //Kinematics
      Emiss->Fill(Em);
      Emissv2->Fill(Emv2);
      pm->Fill(Pm);
      Q_2->Fill(Q2);
      omega->Fill(nu);
      W_inv->Fill(W);
      theta_elec->Fill(theta_e/dtr);
      theta_prot->Fill(theta_p/dtr);


      //Additional Kinematics Variables
      W_2->Fill(W2); 
      xbj->Fill(X); 
      P_f->Fill(Pf);
      Ep_f->Fill(Ep);
      k_f->Fill(kf);
      theta_q->Fill(th_q/dtr);
      q_vec->Fill(q3m);
      thet_pq->Fill(thxq/dtr);
      thet_pq_v2->Fill(theta_pq_v2/dtr);

      
      //Reconstructed Target Quantities (Lab Frame)
      hx_tar->Fill(htar_x);
      hy_tar->Fill(htar_y);
      hz_tar->Fill(htar_z);
                                                                                         
      ex_tar->Fill(etar_x);                                                                                                                         
      ey_tar->Fill(etar_y);                                                                                                              
      ez_tar->Fill(etar_z);
      
      //Hadron-Arm Target Reconstruction 
      hytar->Fill(h_ytar);
      hxptar->Fill(h_xptar);
      hyptar->Fill(h_yptar);
      hdelta->Fill(h_delta);
      
      //Hadron-Arm Focal Plane
      hxfp->Fill(h_xfp);
      hyfp->Fill(h_yfp);
      hxpfp->Fill(h_xpfp);
      hypfp->Fill(h_ypfp);
      
      //Electron-Arm Target Reconstruction
      eytar->Fill(e_ytar);
      exptar->Fill(e_xptar);
      eyptar->Fill(e_yptar);
      edelta->Fill(e_delta);
      
      //Electron-Arm Focal Plane
      exfp->Fill(e_xfp);
      eyfp->Fill(e_yfp);
      expfp->Fill(e_xpfp);
      eypfp->Fill(e_ypfp);

      
      //Fill 2D HMS Focal Plane Quantities
      h_xfp_vs_yfp->Fill(h_yfp, h_xfp);
      e_xfp_vs_yfp->Fill(e_yfp, e_xfp);

      //Fill 2D reconstructed variables
      hxptar_vs_exptar->Fill(e_xptar, h_xptar);
      hyptar_vs_eyptar->Fill(e_yptar, h_yptar);
      hdelta_vs_edelta->Fill(e_delta, h_delta);

      //Heep cross check
      emiss_vs_pmiss->Fill(Pm, Em);
      edelta_vs_eyptar->Fill(e_yptar, e_delta);
     	 
      //OPTICS CHECK (W / Em vs. electron arm)
      W_vs_exfp->Fill(e_xfp, W);
      W_vs_expfp->Fill(e_xpfp, W);
      W_vs_eyfp->Fill(e_yfp, W);
      W_vs_eypfp->Fill(e_ypfp, W);
      
      W_vs_eytar->Fill(e_ytar, W);
      W_vs_exptar->Fill(e_xptar, W);
      W_vs_eyptar->Fill(e_yptar, W);
      W_vs_edelta->Fill(e_delta, W);

      Em_vs_exfp->Fill(e_xfp, Em);
      Em_vs_expfp->Fill(e_xpfp, Em);
      Em_vs_eyfp->Fill(e_yfp, Em);
      Em_vs_eypfp->Fill(e_ypfp, Em);
      
      Em_vs_eytar->Fill(e_ytar, Em);
      Em_vs_exptar->Fill(e_xptar, Em);
      Em_vs_eyptar->Fill(e_yptar, Em);
      Em_vs_edelta->Fill(e_delta, Em);
      
      //OPTICS CHECK (W / Em vs. hadron arm)
      W_vs_hxfp->Fill(h_xfp, W);
      W_vs_hxpfp->Fill(h_xpfp, W);
      W_vs_hyfp->Fill(h_yfp, W);
      W_vs_hypfp->Fill(h_ypfp, W);
      
      W_vs_hytar->Fill(h_ytar, W);
      W_vs_hxptar->Fill(h_xptar, W);
      W_vs_hyptar->Fill(h_yptar, W);
      W_vs_hdelta->Fill(h_delta, W);

      Em_vs_hxfp->Fill(h_xfp, Em);
      Em_vs_hxpfp->Fill(h_xpfp, Em);
      Em_vs_hyfp->Fill(h_yfp, Em);
      Em_vs_hypfp->Fill(h_ypfp, Em);
      
      Em_vs_hytar->Fill(h_ytar, Em);
      Em_vs_hxptar->Fill(h_xptar, Em);
      Em_vs_hyptar->Fill(h_yptar, Em);
      Em_vs_hdelta->Fill(h_delta, Em);
      
    
  } //end entry loop
  
   
  outROOT->Write();
  
}
