//Code to check H(e,e'p) elastics from SIMC

#include "set_heep_histos.h"

void checkW_simc()
{
  //PREVENT DISPLAY 
  //gROOT->SetBatch(kTRUE);
    
  //Read SIMC ROOTfiles
  TString filename = "../../worksim_voli/D2_heep_3288_realisticDC_res350um.root";                                 
  TFile *data_file = new TFile(filename, "READ"); 
  TTree *SNT = (TTree*)data_file->Get("SNT");
 
  //Create output root file where histograms will be stored
  TFile *outROOT = new TFile("Wcheck_histos.root", "recreate");
  

  //********* Create 1D Histograms **************
 
//Kinematics Quantities
TH1F *Emiss = new TH1F("Emiss","missing energy", Em_nbins, Em_xmin, Em_xmax);       //min width = 21.6 (0.0216)MeV,  COUNTS/25 MeV
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
TH1F *k_f = new TH1F("kf", "Final e^{-} Momentum", kf_nbins, kf_xmin, kf_xmax);
TH1F *theta_q = new TH1F("theta_q", "q-vector Angle, #theta_{q}", thq_nbins, thq_xmin, thq_xmax);


//Target Reconstruction Variables
TH1F *x_tar = new TH1F("x_tar", "x_Target", xtar_nbins, xtar_xmin, xtar_xmax);
TH1F *y_tar = new TH1F("y_tar", "y_Target", ytar_nbins, ytar_xmin, ytar_xmax);
TH1F *z_tar = new TH1F("z_tar", "z_Target", ztar_nbins, ztar_xmin, ztar_xmax);

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
TH2F *hxptar_vs_exptar = new TH2F("hxptar_vs_exptar", "HMS vs. SHMS, X'_{tar}", exptar_nbins, exptar_xmin, exptar_xmax, hxptar_nbins, hxptar_xmin, hxptar_xmax);
TH2F *hyptar_vs_eyptar = new TH2F("hyptar_vs_eyptar", "HMS vs. SHMS, Y'_{tar}", eyptar_nbins, eyptar_xmin, eyptar_xmax, hyptar_nbins, hyptar_xmin, hyptar_xmax);
TH2F *hdelta_vs_edelta = new TH2F("hdelta_vs_edelta", "HMS vs. SHMS, #delta", edelta_nbins, edelta_xmin, edelta_xmax, hdelta_nbins, hdelta_xmin, hdelta_xmax);

//OPTICS CHECK (W correlations with Focal Plane / Target Quantities)
TH2F *W_vs_xfp = new TH2F("W_vs_xfp", "W vs X_{fp}", exfp_nbins, exfp_xmin, exfp_xmax, W_nbins, W_xmin, W_xmax);
TH2F *W_vs_xpfp = new TH2F("W_vs_xpfp", "W vs X'_{fp}", expfp_nbins, expfp_xmin, expfp_xmax, W_nbins, W_xmin, W_xmax);
TH2F *W_vs_yfp = new TH2F("W_vs_yfp", "W vs Y_{fp}", eyfp_nbins, eyfp_xmin, eyfp_xmax, W_nbins, W_xmin, W_xmax);
TH2F *W_vs_ypfp = new TH2F("W_vs_ypfp", "W vs Y'_{fp}", eypfp_nbins, eypfp_xmin, eypfp_xmax, W_nbins, W_xmin, W_xmax);

TH2F *W_vs_ytar = new TH2F("W_vs_ytar", "W vs Y_{tar}", eytar_nbins, eytar_xmin, eytar_xmax, W_nbins, W_xmin, W_xmax);
TH2F *W_vs_xptar = new TH2F("W_vs_xptar", "W vs X'_{tar}", exptar_nbins, exptar_xmin, exptar_xmax, W_nbins, W_xmin, W_xmax);
TH2F *W_vs_yptar = new TH2F("W_vs_yptar", "W vs Y'_{tar}", eyptar_nbins, eyptar_xmin, eyptar_xmax, W_nbins, W_xmin, W_xmax);
TH2F *W_vs_delta = new TH2F("W_vs_delta", "W vs delta", edelta_nbins, edelta_xmin, edelta_xmax, W_nbins, W_xmin, W_xmax);


/************Define Histos to APPLY CUTS*********************************/
 
//Kinematics Quantities
TH1F *cut_Emiss = new TH1F("cut_Emiss","missing energy", Em_nbins, Em_xmin, Em_xmax);       //min width = 21.6 (0.0216)MeV,  CUT_OUNTS/25 MeV
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
TH1F *cut_k_f = new TH1F("cut_kf", "Final e^{-} Momentum", kf_nbins, kf_xmin, kf_xmax);
TH1F *cut_theta_q = new TH1F("cut_theta_q", "q-vector Angle, #theta_{q}", thq_nbins, thq_xmin, thq_xmax);


//Target Reconstruction Variables
TH1F *cut_x_tar = new TH1F("cut_x_tar", "x_Target", xtar_nbins, xtar_xmin, xtar_xmax);
TH1F *cut_y_tar = new TH1F("cut_y_tar", "y_Target", ytar_nbins, ytar_xmin, ytar_xmax);
TH1F *cut_z_tar = new TH1F("cut_z_tar", "z_Target", ztar_nbins, ztar_xmin, ztar_xmax);

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
TH2F *cut_hxptar_vs_exptar = new TH2F("cut_hxptar_vs_exptar", "HMS vs. SHMS, X'_{tar}", exptar_nbins, exptar_xmin, exptar_xmax, hxptar_nbins, hxptar_xmin, hxptar_xmax);
TH2F *cut_hyptar_vs_eyptar = new TH2F("cut_hyptar_vs_eyptar", "HMS vs. SHMS, Y'_{tar}", eyptar_nbins, eyptar_xmin, eyptar_xmax, hyptar_nbins, hyptar_xmin, hyptar_xmax);
TH2F *cut_hdelta_vs_edelta = new TH2F("cut_hdelta_vs_edelta", "HMS vs. SHMS, #delta", edelta_nbins, edelta_xmin, edelta_xmax, hdelta_nbins, hdelta_xmin, hdelta_xmax);

//OPTICS CHECK (W correlations with Focal Plane / Target Quantities)
TH2F *cut_W_vs_xfp = new TH2F("cut_W_vs_xfp", "cut_W vs X_{fp}", exfp_nbins, exfp_xmin, exfp_xmax, W_nbins, W_xmin, W_xmax);
TH2F *cut_W_vs_xpfp = new TH2F("cut_W_vs_xpfp", "cut_W vs X'_{fp}", expfp_nbins, expfp_xmin, expfp_xmax, W_nbins, W_xmin, W_xmax);
TH2F *cut_W_vs_yfp = new TH2F("cut_W_vs_yfp", "cut_W vs Y_{fp}", eyfp_nbins, eyfp_xmin, eyfp_xmax, W_nbins, W_xmin, W_xmax);
TH2F *cut_W_vs_ypfp = new TH2F("cut_W_vs_ypfp", "cut_W vs Y'_{fp}", eypfp_nbins, eypfp_xmin, eypfp_xmax, W_nbins, W_xmin, W_xmax);

TH2F *cut_W_vs_ytar = new TH2F("cut_W_vs_ytar", "cut_W vs Y_{tar}", eytar_nbins, eytar_xmin, eytar_xmax, W_nbins, W_xmin, W_xmax);
TH2F *cut_W_vs_xptar = new TH2F("cut_W_vs_xptar", "cut_W vs X'_{tar}", exptar_nbins, exptar_xmin, exptar_xmax, W_nbins, W_xmin, W_xmax);
TH2F *cut_W_vs_yptar = new TH2F("cut_W_vs_yptar", "cut_W vs Y'_{tar}", eyptar_nbins, eyptar_xmin, eyptar_xmax, W_nbins, W_xmin, W_xmax);
TH2F *cut_W_vs_delta = new TH2F("cut_W_vs_delta", "cut_W vs delta", edelta_nbins, edelta_xmin, edelta_xmax, W_nbins, W_xmin, W_xmax);



 

  Float_t  Normfac;
  Float_t  h_delta;
  Float_t  h_yptar;
  Float_t  h_xptar;
  Float_t  h_ytar;
  Float_t  h_xfp;
  Float_t  h_xpfp;
  Float_t  h_yfp;
  Float_t  h_ypfp;
  Float_t  h_deltai;
  Float_t  h_yptari;
  Float_t  h_xptari;
  Float_t  h_ytari;
  Float_t  e_delta;
  Float_t  e_yptar;
  Float_t  e_xptar;
  Float_t  e_ytar;
  Float_t  e_xfp;
  Float_t  e_xpfp;
  Float_t  e_yfp;
  Float_t  e_ypfp;
  Float_t  e_deltai;
  Float_t  e_yptari;
  Float_t  e_xptari;
  Float_t  e_ytari;
  Float_t  q;
  Float_t  nu;
  Float_t  Q2;
  Float_t  W;
  Float_t  epsilon;
  Float_t  Em;
  Float_t  Pm;
  Float_t  theta_pq;
  Float_t  phi_pq;
  Float_t  corrsing;
  Float_t  Pmx;
  Float_t  Pmy;
  Float_t  Pmz;
  Float_t  PmPar;
  Float_t  PmPer;
  Float_t  PmOop;
  Float_t  fry;
  Float_t  radphot;
  Float_t  sigcc;
  Float_t  Weight;
  Float_t  Jacobian;
  Float_t  theta_e;
  Float_t  theta_p;
  Float_t  tar_x;
  Float_t  tar_y;
  Float_t  tar_z;
  Float_t  Genweight;
  Float_t  SF_weight;
  Float_t  Jacobian_corr;
  Float_t  sig;
  Float_t  sig_recon;
  Float_t  sigcc_recon;
  Float_t  coul_corr;
  Float_t  h_zv;
  Float_t  h_yv;
  Float_t  e_zv;
  Float_t  e_yv;
  Float_t  h_pf;
  Float_t  e_pf;
  Float_t  Ein;
  Float_t  theta_rq;
  Float_t  SF_weight_recon;
  Float_t  h_Thf;
  Float_t  Ein_v;
  
  //Define SIMC TTree Variables
  SNT->SetBranchAddress("Normfac", &Normfac);
  SNT->SetBranchAddress("h_delta", &h_delta);
  SNT->SetBranchAddress("h_yptar", &h_yptar);
  SNT->SetBranchAddress("h_xptar", &h_xptar);
  SNT->SetBranchAddress("h_ytar", &h_ytar);
  SNT->SetBranchAddress("h_xfp", &h_xfp);
  SNT->SetBranchAddress("h_xpfp", &h_xpfp);
  SNT->SetBranchAddress("h_yfp", &h_yfp);
  SNT->SetBranchAddress("h_ypfp", &h_ypfp);
  SNT->SetBranchAddress("h_deltai", &h_deltai);
  SNT->SetBranchAddress("h_yptari", &h_yptari);
  SNT->SetBranchAddress("h_xptari", &h_xptari);
  SNT->SetBranchAddress("h_ytari", &h_ytari);
  SNT->SetBranchAddress("e_delta", &e_delta);
  SNT->SetBranchAddress("e_yptar", &e_yptar);
  SNT->SetBranchAddress("e_xptar", &e_xptar);
  SNT->SetBranchAddress("e_ytar", &e_ytar);
  SNT->SetBranchAddress("e_xfp", &e_xfp);
  SNT->SetBranchAddress("e_xpfp", &e_xpfp);
  SNT->SetBranchAddress("e_yfp", &e_yfp);
  SNT->SetBranchAddress("e_ypfp", &e_ypfp);
  SNT->SetBranchAddress("e_deltai", &e_deltai);
  SNT->SetBranchAddress("e_yptari", &e_yptari);
  SNT->SetBranchAddress("e_xptari", &e_xptari);
  SNT->SetBranchAddress("e_ytari", &e_ytari);
  SNT->SetBranchAddress("q", &q);
  SNT->SetBranchAddress("nu", &nu);
  SNT->SetBranchAddress("Q2", &Q2);
  SNT->SetBranchAddress("W", &W);
  SNT->SetBranchAddress("epsilon", &epsilon);
  SNT->SetBranchAddress("Em", &Em);
  SNT->SetBranchAddress("Pm", &Pm);
  SNT->SetBranchAddress("theta_pq", &theta_pq);
  SNT->SetBranchAddress("phi_pq", &phi_pq);
  SNT->SetBranchAddress("corrsing", &corrsing);
  SNT->SetBranchAddress("Pmx", &Pmx);
  SNT->SetBranchAddress("Pmy", &Pmy);
  SNT->SetBranchAddress("Pmz", &Pmz);
  SNT->SetBranchAddress("PmPar", &PmPar);
  SNT->SetBranchAddress("PmPer", &PmPer);
  SNT->SetBranchAddress("PmOop", &PmOop);
  SNT->SetBranchAddress("fry", &fry);
  SNT->SetBranchAddress("radphot", &radphot);
  SNT->SetBranchAddress("sigcc", &sigcc);
  SNT->SetBranchAddress("Weight", &Weight);
  SNT->SetBranchAddress("Jacobian", &Jacobian);
  SNT->SetBranchAddress("theta_e", &theta_e);
  SNT->SetBranchAddress("theta_p", &theta_p);
  SNT->SetBranchAddress("tar_x", &tar_x);
  SNT->SetBranchAddress("tar_y", &tar_y);
  SNT->SetBranchAddress("tar_z", &tar_z);
  SNT->SetBranchAddress("Genweight", &Genweight);
  SNT->SetBranchAddress("SF_weight", &SF_weight);
  SNT->SetBranchAddress("Jacobian_corr", &Jacobian_corr);
  SNT->SetBranchAddress("sig", &sig);
  SNT->SetBranchAddress("sig_recon", &sig_recon);
  SNT->SetBranchAddress("sigcc_recon", &sigcc_recon);
  SNT->SetBranchAddress("coul_corr", &coul_corr);
  SNT->SetBranchAddress("h_zv", &h_zv);
  SNT->SetBranchAddress("h_yv", &h_yv);
  SNT->SetBranchAddress("e_zv", &e_zv);
  SNT->SetBranchAddress("e_yv", &e_yv);
  SNT->SetBranchAddress("h_pf", &h_pf);
  SNT->SetBranchAddress("e_pf", &e_pf);
  SNT->SetBranchAddress("Ein", &Ein);
  SNT->SetBranchAddress("theta_rq", &theta_rq);
  SNT->SetBranchAddress("SF_weight_recon", &SF_weight_recon);
  SNT->SetBranchAddress("h_Thf", &h_Thf);
  SNT->SetBranchAddress("Ein_v", &Ein_v);
  
  
  //Define Additional Kinematic Variables
  Double_t W2;             //Invarianrt Mass Squared
  Double_t X;              //B-jorken X
  Double_t Pf;             //Final Proton Momentum 
  Double_t ki;             //Incident e- momentum
  Double_t kf;             //Final electron momentum
  Double_t Ep;             //proton final energy
  Double_t Ee;             //electron final energy
  Double_t th_q;         //Angle between q-vector and beamline (+z axis --lab)
  

  //Determine Full Weight Quantities (Assume one for heep check)
  Double_t FullWeight;
  Double_t charge_factor = 1.;       //Units: mC   :: beam_current(uA) * time (hrs) * 3600. / 1000.  3600--> hrs to sec,  1000--> uC to mC
  Double_t e_trkEff = 1.;
  Double_t h_trkEff = 1.;           
  Double_t c_LT = 1.;


  //Define Boolean for Kin. Cuts
  Bool_t c_Em;

  //=======================
  // E N T R Y    L O O P
  //=======================

  Long64_t nentries = SNT->GetEntries();
  
  cout << "nentries = " << nentries << endl;
  
  

  for (Long64_t i=0; i<nentries;i++) {
    
    SNT->GetEntry(i);

    
    //-----Define Additional Kinematic Variables--------
    Ein = Ein / 1000.;
    W2 = W*W;
    X = Q2 / (2.*MP*nu);
    Pf = sqrt(pow(nu+MP,2) - MP*MP);
    Ep = sqrt(MP*MP + Pf*Pf);
    ki = sqrt(Ein*Ein - me*me);
    kf = Q2 / (4.*ki*pow(sin(theta_e/2.), 2));
    Ee = sqrt(me*me + kf*kf);
    th_q = acos( (ki - kf*cos(theta_e))/q ); //th_q = theta_pq + theta_p;

    //Define cuts
    c_Em = Em < 0.03;
    
    //Full Weight
    FullWeight = (Normfac*Weight*charge_factor*e_trkEff*h_trkEff*c_LT)/nentries;
    
    //APPLY CUTS: BEGIN CUTS LOOP
      if (c_Em)
	{
	  //Kinematics
	  cut_Emiss->Fill(Em, FullWeight);
	  cut_pm->Fill(Pm, FullWeight);
	  cut_Q_2->Fill(Q2, FullWeight);
	  cut_omega->Fill(nu, FullWeight);
	  cut_W_inv->Fill(W, FullWeight);
	  cut_theta_elec->Fill(theta_e/dtr, FullWeight);
	  cut_theta_prot->Fill(theta_p/dtr, FullWeight);

	  
	  //Additional Kinematics Variables
	  cut_W_2->Fill(W2, FullWeight); 
	  cut_xbj->Fill(X, FullWeight); 
	  cut_P_f->Fill(Pf, FullWeight);
	  cut_k_f->Fill(kf, FullWeight);
	  cut_theta_q->Fill(th_q/dtr, FullWeight);
		  
	  //Reconstructed Target Quantities (Lab Frame)
	  cut_x_tar->Fill(tar_x, FullWeight);
	  cut_y_tar->Fill(tar_y, FullWeight);
	  cut_z_tar->Fill(tar_z, FullWeight);
	  
	  
	  //Hadron-Arm Target Reconstruction 
	  cut_hytar->Fill(h_ytar, FullWeight);
	  cut_hxptar->Fill(h_xptar, FullWeight);
	  cut_hyptar->Fill(h_yptar, FullWeight);
	  cut_hdelta->Fill(h_delta, FullWeight);
	  
	  //Hadron-Arm Focal Plane
	  cut_hxfp->Fill(h_xfp, FullWeight);
	  cut_hyfp->Fill(h_yfp, FullWeight);
	  cut_hxpfp->Fill(h_xpfp, FullWeight);
	  cut_hypfp->Fill(h_ypfp, FullWeight);
	  
	  //Electron-Arm Target Reconstruction
	  cut_eytar->Fill(e_ytar, FullWeight);
	  cut_exptar->Fill(e_xptar, FullWeight);
	  cut_eyptar->Fill(e_yptar, FullWeight);
	  cut_edelta->Fill(e_delta, FullWeight);
	  
	  //Electron-Arm Focal Plane
	  cut_exfp->Fill(e_xfp, FullWeight);
	  cut_eyfp->Fill(e_yfp, FullWeight);
	  cut_expfp->Fill(e_xpfp, FullWeight);
	  cut_eypfp->Fill(e_ypfp, FullWeight);
	  

	  //Fill 2D HMS Focal Plane Quantities
	  cut_h_xfp_vs_yfp->Fill(h_yfp, h_xfp, FullWeight);
	  cut_e_xfp_vs_yfp->Fill(e_yfp, e_xfp, FullWeight);
	  
	  //Fill 2D reconstructed variables
	  cut_hxptar_vs_exptar->Fill(e_xptar, h_xptar, FullWeight);
	  cut_hyptar_vs_eyptar->Fill(e_yptar, h_yptar, FullWeight);
	  cut_hdelta_vs_edelta->Fill(e_delta, h_delta, FullWeight);

	  
	  //Heep cross check
	  cut_emiss_vs_pmiss->Fill(Pm, Em, FullWeight);
	  cut_edelta_vs_eyptar->Fill(e_yptar, e_delta, FullWeight);
	  
	  //OPTICS CHECK
	  cut_W_vs_xfp->Fill(e_xfp, W, FullWeight);
	  cut_W_vs_xpfp->Fill(e_xpfp, W, FullWeight);
	  cut_W_vs_yfp->Fill(e_yfp, W, FullWeight);
	  cut_W_vs_ypfp->Fill(e_ypfp, W, FullWeight);
	  
	  cut_W_vs_ytar->Fill(e_ytar, W, FullWeight);
	  cut_W_vs_xptar->Fill(e_xptar, W, FullWeight);
	  cut_W_vs_yptar->Fill(e_yptar, W, FullWeight);
	  cut_W_vs_delta->Fill(e_delta, W, FullWeight);

	  
	}//End CUTS LOOP
      
      
      
      //Kinematics
      Emiss->Fill(Em, FullWeight);
      pm->Fill(Pm, FullWeight);
      Q_2->Fill(Q2, FullWeight);
      omega->Fill(nu, FullWeight);
      W_inv->Fill(W, FullWeight);
      theta_elec->Fill(theta_e/dtr, FullWeight);
      theta_prot->Fill(theta_p/dtr, FullWeight);


      //Additional Kinematics Variables
      W_2->Fill(W2, FullWeight); 
      xbj->Fill(X, FullWeight); 
      P_f->Fill(Pf, FullWeight);
      k_f->Fill(kf, FullWeight);
      theta_q->Fill(th_q/dtr, FullWeight);

      
      //Reconstructed Target Quantities (Lab Frame)
      x_tar->Fill(tar_x, FullWeight);
      y_tar->Fill(tar_y, FullWeight);
      z_tar->Fill(tar_z, FullWeight);

      
      //Hadron-Arm Target Reconstruction 
      hytar->Fill(h_ytar, FullWeight);
      hxptar->Fill(h_xptar, FullWeight);
      hyptar->Fill(h_yptar, FullWeight);
      hdelta->Fill(h_delta, FullWeight);
      
      //Hadron-Arm Focal Plane
      hxfp->Fill(h_xfp, FullWeight);
      hyfp->Fill(h_yfp, FullWeight);
      hxpfp->Fill(h_xpfp, FullWeight);
      hypfp->Fill(h_ypfp, FullWeight);
      
      //Electron-Arm Target Reconstruction
      eytar->Fill(e_ytar, FullWeight);
      exptar->Fill(e_xptar, FullWeight);
      eyptar->Fill(e_yptar, FullWeight);
      edelta->Fill(e_delta, FullWeight);
      
      //Electron-Arm Focal Plane
      exfp->Fill(e_xfp, FullWeight);
      eyfp->Fill(e_yfp, FullWeight);
      expfp->Fill(e_xpfp, FullWeight);
      eypfp->Fill(e_ypfp, FullWeight);

      
      //Fill 2D HMS Focal Plane Quantities
      h_xfp_vs_yfp->Fill(h_yfp, h_xfp, FullWeight);
      e_xfp_vs_yfp->Fill(e_yfp, e_xfp, FullWeight);

      //Fill 2D reconstructed variables
      hxptar_vs_exptar->Fill(e_xptar, h_xptar, FullWeight);
      hyptar_vs_eyptar->Fill(e_yptar, h_yptar, FullWeight);
      hdelta_vs_edelta->Fill(e_delta, h_delta, FullWeight);

      //Heep cross check
      emiss_vs_pmiss->Fill(Pm, Em, FullWeight);
      edelta_vs_eyptar->Fill(e_yptar, e_delta, FullWeight);
     	 
      //OPTICS CHECK
      W_vs_xfp->Fill(e_xfp, W, FullWeight);
      W_vs_xpfp->Fill(e_xpfp, W, FullWeight);
      W_vs_yfp->Fill(e_yfp, W, FullWeight);
      W_vs_ypfp->Fill(e_ypfp, W, FullWeight);
      
      W_vs_ytar->Fill(e_ytar, W, FullWeight);
      W_vs_xptar->Fill(e_xptar, W, FullWeight);
      W_vs_yptar->Fill(e_yptar, W, FullWeight);
      W_vs_delta->Fill(e_delta, W, FullWeight);
	  
    

      
  } //end entry loop

  
  //Write to a ROOTfile
  outROOT->Write();

  
}
