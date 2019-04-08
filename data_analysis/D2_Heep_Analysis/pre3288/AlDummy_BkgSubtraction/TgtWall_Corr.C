#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"


#include "../../../../simc_analysis/D2_heep/set_heep_histos.h"


/*
  Code to correct for the target cell walls.  
  The code reads the LH2 or dummy runs (may be combined runs if desired).
  The Total counts will be normalized by total charge, and corrected
  by efficiencies.


*/

void TgtWall_Corr(int run, string eArm="P", bool Qnorm=true, string target="")
{

  /*User Input:
    run-> Run Number
    eArm -> what is the electron Arm? "P" (SHMS) or "H" (HMS)
    Qnorm -> option to weight the data by charge / efficiencies
    target -> What target being used?  "LH2"  or "dummy"
  */

  TString hadron_arm;
  TString electron_arm;

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

  //Variables Used to obtain Normalized/Corrected Yields
  Double_t charge_factor = 1.;  //BCM4A  
  Double_t e_trkEff = 1.;       //shms e- trk eff 
  Double_t h_trkEff = 1.;        //hms had trk eff
  Double_t c_LT = 1.;           //computer live time 
  Double_t t_LT = 1.;           //total live time
  Double_t pS = 1.;             //pre-Scale Factor
  Double_t FullWeight;

  if (Qnorm){          
    
    //LH2 Runs
    if(run==3259){
      charge_factor = 38.146;                                                                                                                                                                                     
      c_LT = 0.390989;                                                                                       
      t_LT = 0.390225;                                                                                                                                                                                                                 
      e_trkEff = 0.9746;                                                                
      h_trkEff = 0.9907;       
      pS = 2.;
    }
    
    //Dummy Runs
    if(run==3254){
      charge_factor = 43.121;                                                                                                                                                                                     
      c_LT = 0.833357;                                                                                       
      t_LT = 0.819777;                                                                                                                                                                                                                 
      e_trkEff = 0.9820;                                                                
      h_trkEff = 0.9909;       
      pS = 2.;
    }

  }

  


//PREVENT DISPLAY 
  //gROOT->SetBatch(kTRUE);
    
  //Read DATA ROOTfiles  
  TString filename =Form("../../../../../hallc_replay/ROOTfiles/coin_replay_heep_check_%d_-1.root",run);     
  

  TFile *data_file = new TFile(filename, "READ"); 
  TTree *T = (TTree*)data_file->Get("T");
 
  
  //Create output root file where histograms will be stored
  TFile *outROOT = NULL; 
  if(target=="LH2"){
    outROOT = new TFile(Form("%s_%d.root", target.c_str(), run), "recreate");
  }
  else if(target=="dummy"){
    outROOT = new TFile(Form("%s_%d.root", target.c_str(), run), "recreate");
  }
  else {
    cout << "Enter target type (\"LH2\" or \"dummy\")" << endl;
    cin >> target;
    outROOT = new TFile(Form("%s_%d.root", target.c_str(), run), "recreate");

  }

  //********* Create 1D Histograms **************

  TH1F *cut_epCT = new TH1F("cut_epCT","e-Proton Coincidence Time", 100, 0, 20);       //min width = 21.6 (0.0216)MeV,  COUNTS/25 MeV
  TH1F *cut_etotnorm = new TH1F("cut_etot_tracknorm", "SHMS Total Normalized Track Energy", 100, 0.5, 2.3);

  //Kinematics Quantities
  TH1F *cut_MM2 = new TH1F("cut_MM2", "Missing Mass Squared, MM2", MM2_nbins, MM2_xmin, MM2_xmax );
  TH1F *cut_Emiss = new TH1F("cut_Emiss","missing energy", Em_nbins, Em_xmin, Em_xmax);       //min width = 21.6 (0.0216)MeV,  CUT_OUNTS/25 MeV
  TH1F *cut_pm = new TH1F("cut_pm","missing momentum", Pm_nbins, Pm_xmin, Pm_xmax);  //min width = 32 MeV (0.032)
  
  TH1F *cut_pmX_lab = new TH1F("cut_pmX_Lab","Pmiss X (Lab) ", Pmx_nbins, Pmx_xmin, Pmx_xmax); 
  TH1F *cut_pmY_lab = new TH1F("cut_pmY_Lab","Pmiss Y (Lab) ", Pmy_nbins, Pmy_xmin, Pmy_xmax);  
  TH1F *cut_pmZ_lab = new TH1F("cut_pmZ_Lab","Pmiss Z (Lab) ", Pmz_nbins, Pmz_xmin, Pmz_xmax);  
  TH1F *cut_pmX_q = new TH1F("cut_pmX_q","Pmiss X (w.r.t q-vec) ", Pmx_nbins, Pmx_xmin, Pmx_xmax); 
  TH1F *cut_pmY_q = new TH1F("cut_pmY_q","Pmiss Y (w.r.t q-vec) ", Pmy_nbins, Pmy_xmin, Pmy_xmax);  
  TH1F *cut_pmZ_q = new TH1F("cut_pmZ_q","Pmiss Z (w.r.t. q-vec) ", Pmz_nbins, Pmz_xmin, Pmz_xmax);  
  

  TH1F *cut_Q_2 = new TH1F("cut_Q_2","Q2", Q2_nbins, Q2_xmin, Q2_xmax);
  TH1F *cut_omega = new TH1F("cut_omega","Energy Transfer, #omega", om_nbins, om_xmin, om_xmax);
  TH1F *cut_W_inv = new TH1F("cut_W_inv", "Invariant Mass, W", W_nbins, W_xmin, W_xmax);     //min width = 19.9 MeV (0.0199) (bin width = 25 MeV)

  TH1F *cut_theta_elec = new TH1F("cut_theta_elec", "Electron Scatt. Angle", the_nbins, the_xmin, the_xmax);
  TH1F *cut_theta_prot = new TH1F("cut_theta_prot", "Proton Scatt. Angle", thp_nbins, thp_xmin, thp_xmax);
  TH1F *cut_theta_elec_calc = new TH1F("cut_theta_elec_calc", "Calculated Electron Scatt. Angle", the_nbins, the_xmin, the_xmax);
  TH1F *cut_theta_elec_res = new TH1F("cut_theta_elec_res", "Electron Scatt. Angle Residual", 100, -1, 1);

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
  TH1F *cut2_ez_tar = new TH1F("cut2_ez_tar", electron_arm + " z_Target", ztar_nbins, ztar_xmin, ztar_xmax);  

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
  
  //Electron Arm Solid Angle
  TH2F *cut_eSolid = new TH2F("cut_eSolid", electron_arm + "d#Omega_{e}: #theta vs. #phi", eyptar_nbins, eyptar_xmin, eyptar_xmax, exptar_nbins, exptar_xmin, exptar_xmax);
  
  //X vs. Y Focal Plane
  TH2F *cut_h_xfp_vs_yfp = new TH2F("cut_h_xfp_vs_yfp", "X_{fp} vs Y_{fp}", hyfp_nbins, hyfp_xmin, hyfp_xmax, hxfp_nbins, hxfp_xmin, hxfp_xmax);
  TH2F *cut_e_xfp_vs_yfp = new TH2F("cut_e_xfp_vs_yfp", "X_{fp} vs Y_{fp}", eyfp_nbins, eyfp_xmin, eyfp_xmax, exfp_nbins, exfp_xmin, exfp_xmax);
  
  //2D HMS v. SHMS Acceptance Correlations
  TH2F *cut_hxptar_vs_exptar = new TH2F("cut_hxptar_vs_exptar", hadron_arm + " vs."+electron_arm+", X'_{tar}", exptar_nbins, exptar_xmin, exptar_xmax, hxptar_nbins, hxptar_xmin, hxptar_xmax);
  TH2F *cut_hyptar_vs_eyptar = new TH2F("cut_hyptar_vs_eyptar", hadron_arm + " vs."+electron_arm+", Y'_{tar}", eyptar_nbins, eyptar_xmin, eyptar_xmax, hyptar_nbins, hyptar_xmin, hyptar_xmax);
  TH2F *cut_hdelta_vs_edelta = new TH2F("cut_hdelta_vs_edelta", hadron_arm + " vs."+electron_arm+", #delta", edelta_nbins, edelta_xmin, edelta_xmax, hdelta_nbins, hdelta_xmin, hdelta_xmax);
  
  //Histograms Used for proton absorption
  TH1F *W_did = new TH1F("W_did", "Invariant Mass, W", W_nbins, W_xmin, W_xmax);     //min width = 19.9 MeV (0.0199) (bin width = 25 MeV)


  //Set Variable Names and Branches
 
  //------Kinematics
  Double_t  Eb = 10.6005;
  Double_t  theta_e;
  Double_t  theta_e_calc;
  Double_t  W;
  Double_t  Q2;
  Double_t  X;
  Double_t  nu;
  Double_t  q3m;   //q-vect magnitude
  Double_t  th_q;
  Double_t  kf;
  Double_t  Pf;
  Double_t  Em;
  Double_t  Pm;
  Double_t  Pmx_lab;
  Double_t  Pmy_lab;
  Double_t  Pmz_lab;
  Double_t  Pmx_q;
  Double_t  Pmy_q;
  Double_t  Pmz_q;
  Double_t  thbq;
  Double_t  thxq;
  Double_t  xangle;
  Double_t  theta_p;  //to be determined in loop
  Double_t  theta_pq_v2; //to be determined in loop
  Double_t  W2;
  Double_t  Ep;
  Double_t  epCoinTime;
  Double_t  pTRIG6_tdcTime;
  Double_t  pindex;
  Double_t MM_2;

  T->SetBranchAddress("CTime.epCoinTime_ROC2", &epCoinTime);
  T->SetBranchAddress("T.coin.pTRIG6_ROC2_tdcTime", &pTRIG6_tdcTime);

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

  T->SetBranchAddress("H.kin.secondary.Prec_x",&Pmx_lab);   //x-component of recoil momentum (Pmiss_x in Hall or Lab Coordinates)
  T->SetBranchAddress("H.kin.secondary.Prec_y",&Pmy_lab);   //y
  T->SetBranchAddress("H.kin.secondary.Prec_z",&Pmz_lab);   //z
  
  T->SetBranchAddress("H.kin.secondary.pmiss_x",&Pmx_q);   //x-component of recoil momentum (Pmiss_x with respect to the q-vector)
  T->SetBranchAddress("H.kin.secondary.pmiss_y",&Pmy_q);   //y
  T->SetBranchAddress("H.kin.secondary.pmiss_z",&Pmz_q);   //z


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
  Double_t  pcal_etotnorm;
  Double_t  pngcer_npesum;
  Double_t  pdc_ntrack;

  T->SetBranchAddress("P.cal.etotnorm",&pcal_etotnorm);
  T->SetBranchAddress("P.ngcer.npeSum",&pngcer_npesum);
  T->SetBranchAddress("P.dc.ntrack",&pdc_ntrack);

  
  //Calculated Quantities to be use in loop
  Double_t etheta_calc; //calculated electron angle
  Double_t detheta;

 
  //Target Wall Thickness @ EndCaps, from Survey (See HCLOG Log Entry 3481732)
  // 10 cm Liquid Hydrogen (Loop 2, 10cm, as of Spring 2018)
  Double_t LH2_entr = 0.150;   //in mm
  Double_t LH2_entr_err = 0.011;   //in mm
  Double_t LH2_ext = 0.191;
  Double_t LH2_ext_err = 0.019;
  
  //10 cm Aluminum (Foils to mimic the target entrance/exit windows)
  Double_t Al_den = 2.7;  //Al density g/cm3
  
  Double_t Al_entr = (0.1816/Al_den) * 10.;        // (g/cm2) / g/cm3 --> cm*10 --> mm
  Double_t Al_entr_err = (0.0003/Al_den) * 10.;    //in mm
  Double_t Al_ext = (0.1815/Al_den) * 10.;         
  Double_t Al_ext_err = (0.0003/Al_den) * 10.;

  //Total EndCaps Thickness / Error
  Double_t LH2_total = LH2_entr + LH2_ext;
  Double_t LH2_total_err = sqrt( pow(LH2_entr_err,2) + pow(LH2_ext_err,2) );
  
  Double_t Al_total = Al_entr + Al_ext;
  Double_t Al_total_err = sqrt( pow(Al_entr_err,2) + pow(Al_ext_err,2) );

  //Defalts to 1 (if NOT dummy target)
  Double_t R = 1.;
  Double_t R_err = 1.; 

  //Ratio of Al_dummy_thickness / LH2 Thickness  (Put R in dummy FullWeight, to scale to LH2 thickness)
  if(target=="dummy"){
    R = Al_total / LH2_total;
    R_err = sqrt ( pow(Al_total_err/LH2_total,2) +   pow(Al_total * LH2_total_err / pow(LH2_total,2) , 2) ); 
  }
  
  //----------------------------
  //  SHMS Singles Full Weight
  //----------------------------
  //Each data event will be corrected for by dividing it by charge, efficencies and live time
  //If  HMS quantities (H.*) are used in event selection, then hms trk eff. should be included
  FullWeight = pS / (charge_factor*e_trkEff*t_LT * R);     //The ratio R only applies for Al dummy. It is set to R = 1 by default.
  
  //--------------------------------------------
  //  HMS/SHMS Coincidence Trigger Full Weight
  //--------------------------------------------
  //FullWeight = pS / (charge_factor*e_trkEff*h_trkEff*t_LT);

  cout << "****Target: " << target.c_str() << endl;
  cout << "***Ratio R = " << R << endl;

  //Define Boolean for Kinematic Cuts
  Bool_t c_W;
  Bool_t c_Em;
  Bool_t c_ecal;
  Bool_t c_ctime;
  Bool_t c_MM2;
  Bool_t c_pTRIG6;   //check if there was a coincidence, for each single
  Bool_t c_preactz;

  //Acceptance Cuts
  Bool_t c_hdelta;
  Bool_t c_edelta;
  Bool_t c_eSolid;    //electron arm solid angle  
  Bool_t c_hSolid;    //hadron arm solid angle


  //Proton Absorption Specific
  Bool_t c_Ndid;
  Bool_t c_Nshould;


  //Set Cuts Limits
  Double_t Em_min = 0.04;   //Missing Energy Cut
  Double_t exptar_min = -0.01;  //electron arm dx/dz ~ theta_e  [in radians]
  Double_t exptar_max = 0.015;
  Double_t eyptar_min = -0.01;
  Double_t eyptar_max = 0.01;

  


  //======================
  // E V E N T   L O O P 
  //======================
  
  
  Long64_t nentries = T->GetEntries();
  
  cout << "nentries = " << nentries << endl;
  
  
  
  for (Long64_t i=0; i<nentries;i++) {

    T->GetEntry(i);
    
    
    //Determine additional kinematics
    theta_p = xangle - theta_e;
    W2 = W*2;
    theta_pq_v2 = th_q - theta_p;
    Ep = sqrt(MP*MP + Pf*Pf);
    c_Em = Em < Em_min;      

    theta_e_calc = asin( Pf*sin(theta_p) / (Eb + MP - Ep) );
    detheta = theta_e_calc -theta_e;

    //Calculated Quantities
    etheta_calc =  acos((10.6005 - Pf * cos(theta_p))/kf );
    //detheta = (etheta_calc - theta_e);
    MM_2 = Em*Em - Pm*Pm;
  
    //Define Cuts
    c_preactz = abs(etar_z)<2.5;
    c_W = W >0.9&& W < 1.0;           
    c_hdelta = h_delta>-8.&&h_delta<8.;  //good HMS delta range (well known recon. matrix)
    c_ecal = pcal_etotnorm>=0.6;
    c_ctime = epCoinTime>8.5 && epCoinTime<13.5;
    c_edelta = e_delta > -3.5 && e_delta < 1.5;
    c_MM2 = MM_2>-0.0009&&MM_2<0.0004;
    c_eSolid = e_xptar>exptar_min&&e_xptar<exptar_max&&e_yptar>eyptar_min&&e_yptar<eyptar_max;
    c_pTRIG6 = pTRIG6_tdcTime > 0.;
    
    //Define #electrons that should have / Did have a proton coincidence in the HMS
    c_Nshould = c_edelta&&c_ecal&&c_eSolid&&c_W&&c_preactz;   //cut to select purely electrons in the SHMS acceptance
    c_Ndid = c_Nshould&&c_hdelta;                 //cut to select events for which there was a coincident proton in the HMS


    //APPLY CUTS: BEGIN CUTS LOOP
    if (c_Nshould)   
    {     
      
      if(c_Ndid){
	W_did->Fill(W, FullWeight);
      }

	  cut_epCT->Fill(epCoinTime, FullWeight);
	  cut_etotnorm->Fill(pcal_etotnorm, FullWeight);
	  
	  //Kinematics
	  cut_MM2->Fill(MM_2, FullWeight);
	  cut_Emiss->Fill(Em, FullWeight);
	  cut_pm->Fill(Pm, FullWeight);
	  
	  cut_pmX_lab->Fill(Pmx_lab, FullWeight);
	  cut_pmY_lab->Fill(Pmy_lab, FullWeight);
	  cut_pmZ_lab->Fill(Pmz_lab, FullWeight);
	  
	  cut_pmX_q->Fill(Pmx_q, FullWeight);
	  cut_pmY_q->Fill(Pmy_q, FullWeight);
	  cut_pmZ_q->Fill(Pmz_q, FullWeight);

	  cut_Q_2->Fill(Q2, FullWeight);
	  cut_omega->Fill(nu, FullWeight);
      
	  cut_W_inv->Fill(W, FullWeight);
	  
	  cut_theta_elec->Fill(theta_e/dtr, FullWeight);
	  cut_theta_prot->Fill(theta_p/dtr, FullWeight);
	  cut_theta_elec_calc->Fill(theta_e_calc/dtr, FullWeight);
	  cut_theta_elec_res->Fill((theta_e_calc-theta_e)/dtr, FullWeight);
	  
	  //Additional Kinematics Variables
	  cut_W_2->Fill(W2, FullWeight); 
	  cut_xbj->Fill(X, FullWeight); 
	  cut_P_f->Fill(Pf, FullWeight);
	  cut_Ep_f->Fill(Ep, FullWeight);
	  cut_k_f->Fill(kf, FullWeight);
	  cut_theta_q->Fill(th_q/dtr, FullWeight);
	  cut_q_vec->Fill(q3m, FullWeight);
	  cut_thet_pq->Fill(thxq/dtr, FullWeight);
	  cut_thet_pq_v2->Fill(theta_pq_v2/dtr, FullWeight);
      
	  //Reconstructed Target Quantities (Lab Frame)
	  cut_hx_tar->Fill(htar_x, FullWeight);
	  cut_hy_tar->Fill(htar_y, FullWeight);
	  cut_hz_tar->Fill(htar_z, FullWeight);
	  
	  cut_ex_tar->Fill(etar_x, FullWeight); 
          cut_ey_tar->Fill(etar_y, FullWeight);  
          cut_ez_tar->Fill(etar_z, FullWeight);                                              

	  
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
	  
	  //Electron Arm Solid Angle
	  cut_eSolid->Fill(e_xptar, e_yptar, FullWeight);

	  //Fill 2D HMS Focal Plane Quantities
	  cut_h_xfp_vs_yfp->Fill(h_yfp, h_xfp, FullWeight);
	  cut_e_xfp_vs_yfp->Fill(e_yfp, e_xfp, FullWeight);
	  
	  //Fill 2D reconstructed variables
	  cut_hxptar_vs_exptar->Fill(e_xptar, h_xptar, FullWeight);
	  cut_hyptar_vs_eyptar->Fill(e_yptar, h_yptar, FullWeight);
	  cut_hdelta_vs_edelta->Fill(e_delta, h_delta, FullWeight);
	  


	}//End CUTS LOOP

   


    //***********************************************************
    cout << std::setprecision(2) << double(i) / nentries * 100. << "  % " << std::flush << "\r";

  } //end entry loop
  
   
  outROOT->Write();
  
}
