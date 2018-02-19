//Analysis for HMS/SHMS coincidence H(e,e'p)


#define heep_cxx
#include "heep.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
//#include <fstream>
//#include <iostream>
void heep::Loop(TString simc_file, Double_t Ib, Double_t time)
{
//   In a ROOT session, you can do:
//      root> .L heep.C
//      root> heep t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop("HMS", "SHMS");       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   TString hadron_arm="HMS";
   TString electron_arm = "SHMS";

    // define histograms
   Double_t pi = 3.141592654;
   Double_t dtr = pi/180.;
   Double_t MP = 0.938272; //GeV
   Double_t MD = 1.87561; //GeV
   Double_t MN = 0.939566; //GeV
   Double_t me = 0.000510998;
   
   TString ofile_name("weighted_");
   ofile_name.Append(simc_file);
   
   //create output root file
   TFile *outfile = new TFile(ofile_name.Data(), "recreate");

   
   
   //********* Create 1D Histograms **************
   Int_t bins =100;

   //Kinematics Quantities
   TH1F *Emiss = new TH1F("Emiss","missing energy", bins, -0.2, 0.8); 
   TH1F *pm = new TH1F("pm","missing momentum", bins, -0.05, 0.8);  //binwidth = 8.5 MeV
   TH1F *Q_2 = new TH1F("Q_2","Q2", 100, 3., 5.);
   TH1F *omega = new TH1F("omega","Energy Transfer, #omega", bins, 1.8, 3.4);
   TH1F *W_inv = new TH1F("W_inv", "Invariant Mass, W", bins, 0.4, 2.0);
   TH1F *theta_elec = new TH1F("theta_elec", "Electron Scatt. Angle", bins, 8, 15);
   TH1F *theta_prot = new TH1F("theta_prot", "Proton Scatt. Angle", bins, 35, 45);

   //Additional Kinematics Variables
   TH1F *W_2 = new TH1F("W2", "Invariant Mass W2", bins, 0.4, 2.0);
   TH1F *xbj = new TH1F("xbj", "x-Bjorken", bins, 0.6, 1.3);
   TH1F *P_f = new TH1F("P_f", "Final Proton Momentum", bins, 2.5, 3.5);
   TH1F *k_i = new TH1F("ki", "Initial e^{-} Momentum", bins, 8., 12.);
   TH1F *k_f = new TH1F("kf", "Final e^{-} Momentum", bins, 7., 9.5);
   
   //Target Reconstruction Variables
   TH1F *x_tar = new TH1F("x_tar", "x_Target", bins, -0.25, 0.25);
   TH1F *y_tar = new TH1F("y_tar", "y_Target", bins, -0.25, 0.25);
   TH1F *z_tar = new TH1F("z_tar", "z_Target", bins, -5.5, 5.5);
   
   //Hadron arm Reconstructed Quantities ( xtar, ytar, xptar, yptar, delta)
   TH1F *hytar = new TH1F("hytar", hadron_arm + " Y_{tar}", bins, -5., 5.);
   TH1F *hxptar = new TH1F("hxptar", hadron_arm + " X'_{tar}", bins, -0.1, 0.1 );
   TH1F *hyptar = new TH1F("hyptar", hadron_arm + " Y'_{tar}", bins, -0.05, 0.04 );
   TH1F *hdelta = new TH1F("hdelta", hadron_arm + " Momentum Acceptance, #delta", bins, -12., 10. );

   //Hadron arm Focal Plane Quantities
   TH1F *hxfp = new TH1F("hxfp", hadron_arm + " X_{fp}", bins, -50., 40.);
   TH1F *hyfp = new TH1F("hyfp", hadron_arm + " Y_{fp}", bins, -10., 25.);
   TH1F *hxpfp = new TH1F("hxpfp", hadron_arm + " X'_{fp}", bins, -0.06, 0.06 );
   TH1F *hypfp = new TH1F("hypfp", hadron_arm + " Y'_{fp}", bins, -0.015, 0.03);

      
   //Electron Arm Reconstructed Quantities ( xtar, ytar, xptar, yptar, delta)
   TH1F *eytar = new TH1F("eytar", electron_arm + " Y_{tar}", bins, -2., 2.);
   TH1F *exptar = new TH1F("exptar", electron_arm + " X'_{tar}", bins, -0.03, 0.03);
   TH1F *eyptar = new TH1F("eyptar", electron_arm + " Y'_{tar}", bins, -0.02, 0.025);
   TH1F *edelta = new TH1F("edelta", electron_arm + " Momentum Acceptance, #delta", bins, -16., 5. );

   //Electron Arm Focal Plane Quantities
   TH1F *exfp = new TH1F("exfp", electron_arm + " X_{fp}", bins, -35., 5.);
   TH1F *eyfp = new TH1F("eyfp", electron_arm + " Y_{fp}", bins, -25., 15.);
   TH1F *expfp = new TH1F("expfp", electron_arm + " X'_{fp}", bins, -0.08, 0.02);
   TH1F *eypfp = new TH1F("eypfp", electron_arm + " Y'_{fp}", bins, -0.04, 0.02);



   //Create 2D Histograms at the Focal Plane Quantities
   TH2F *h_xfp_vs_yfp = new TH2F("h_xfp_vs_yfp", "X_{fp} vs Y_{fp}", bins, -10., 25., bins, -50., 40.);
   TH2F *e_xfp_vs_yfp = new TH2F("e_xfp_vs_yfp", "X_{fp} vs Y_{fp}", bins, -25., 15., bins, -35., 5.);

   //2D HMS v. SHMS Acceptance Correlations
   TH2F *hxptar_vs_exptar = new TH2F("hxptar_vs_exptar", "HMS vs. SHMS, X'_{tar}", bins, -0.03, 0.03, bins, -0.1, 0.1);
   TH2F *hyptar_vs_eyptar = new TH2F("hyptar_vs_eyptar", "HMS vs. SHMS, Y'_{tar}", bins, -0.02, 0.025, bins, -0.05, 0.04);
   TH2F *hdelta_vs_edelta = new TH2F("hdelta_vs_edelta", "HMS vs. SHMS, #delta", bins, -16., 5, bins, -12., 10.);

  

   
   
   /************Define Histos to APPLY CUTS*********************************/
 
     //Kinematics Quantities
   TH1F *cut_Emiss = new TH1F("cut_Emiss","missing energy", bins, -0.2, 0.8);  //binwidth = 0.0025
   TH1F *cut_pm = new TH1F("cut_pm","missing momentum", bins, -0.05, 0.8);
   TH1F *cut_Q_2 = new TH1F("cut_Q_2","Q2", 100, 3., 5.);
   TH1F *cut_omega = new TH1F("cut_omega","Energy Transfer, #omega", bins, 1.8, 3.4);
   TH1F *cut_W_inv = new TH1F("cut_W_inv", "Invariant Mass, W", bins, 0.4, 2.0);
   TH1F *cut_theta_elec = new TH1F("cut_theta_elec", "Electron Scatt. Angle", 100, 8, 15);
   TH1F *cut_theta_prot = new TH1F("cut_theta_prot", "Proton Scatt. Angle", 100, 35, 45);

   //Additional Kinematics Variables
   TH1F *cut_W_2 = new TH1F("cut_W2", "Invariant Mass W2", bins, 0.4, 2.0);
   TH1F *cut_xbj = new TH1F("cut_xbj", "x-Bjorken", bins, 0.6, 1.3);
   TH1F *cut_P_f = new TH1F("cut_P_f", "Final Proton Momentum", bins, 2.5, 3.5);
   TH1F *cut_k_i = new TH1F("cut_ki", "Initial e^{-} Momentum", bins, 8., 12.);
   TH1F *cut_k_f = new TH1F("cut_kf", "Final e^{-} Momentum", bins, 7., 9.5);
   
   //Target Reconstruction Variables
   TH1F *cut_x_tar = new TH1F("cut_x_tar", "x_Target", bins, -0.25, 0.25);
   TH1F *cut_y_tar = new TH1F("cut_y_tar", "y_Target", bins, -0.25, 0.25);
   TH1F *cut_z_tar = new TH1F("cut_z_tar", "z_Target", bins, -5.5, 5.5);
   
   //Hadron arm Reconstructed Quantities ( xtar, ytar, xptar, yptar, delta)
   TH1F *cut_hytar = new TH1F("cut_hytar", hadron_arm + " Y_{tar}", bins, -5., 5.);
   TH1F *cut_hxptar = new TH1F("cut_hxptar", hadron_arm + " X'_{tar}", bins, -0.1, 0.1 );
   TH1F *cut_hyptar = new TH1F("cut_hyptar", hadron_arm + " Y'_{tar}", bins, -0.05, 0.04 );
   TH1F *cut_hdelta = new TH1F("cut_hdelta", hadron_arm + " Momentum Acceptance, #delta", bins, -12., 10. );

   //Hadron arm Focal Plane Quantities
   TH1F *cut_hxfp = new TH1F("cut_hxfp", hadron_arm + " X_{fp}", bins, -50., 40.);
   TH1F *cut_hyfp = new TH1F("cut_hyfp", hadron_arm + " Y_{fp}", bins, -10., 25.);
   TH1F *cut_hxpfp = new TH1F("cut_hxpfp", hadron_arm + " X'_{fp}", bins, -0.06, 0.06 );
   TH1F *cut_hypfp = new TH1F("cut_hypfp", hadron_arm + " Y'_{fp}", bins, -0.015, 0.03);

      
   //Electron Arm Reconstructed Quantities ( xtar, ytar, xptar, yptar, delta)
   TH1F *cut_eytar = new TH1F("cut_eytar", electron_arm + " Y_{tar}", bins, -2., 2.);
   TH1F *cut_exptar = new TH1F("cut_exptar", electron_arm + " X'_{tar}", bins, -0.03, 0.03);
   TH1F *cut_eyptar = new TH1F("cut_eyptar", electron_arm + " Y'_{tar}", bins, -0.02, 0.025);
   TH1F *cut_edelta = new TH1F("cut_edelta", electron_arm + " Momentum Acceptance, #delta", bins, -16., 5. );

   //Electron Arm Focal Plane Quantities
   TH1F *cut_exfp = new TH1F("cut_exfp", electron_arm + " X_{fp}", bins, -35., 5.);
   TH1F *cut_eyfp = new TH1F("cut_eyfp", electron_arm + " Y_{fp}", bins, -25., 15.);
   TH1F *cut_expfp = new TH1F("cut_expfp", electron_arm + " X'_{fp}", bins, -0.08, 0.02);
   TH1F *cut_eypfp = new TH1F("cut_eypfp", electron_arm + " Y'_{fp}", bins, -0.04, 0.02);


   //Create 2D Histograms at the Focal Plane Quantities
   TH2F *cut_h_xfp_vs_yfp = new TH2F("cut_h_xfp_vs_yfp", "X_{fp} vs Y_{fp}", bins, -10., 25., bins, -50., 40.);
   TH2F *cut_e_xfp_vs_yfp = new TH2F("cut_e_xfp_vs_yfp", "X_{fp} vs Y_{fp}", bins, -25., 15., bins, -35., 5.);

   //2D HMS v. SHMS Acceptance Correlations
   TH2F *cut_hxptar_vs_exptar = new TH2F("cut_hxptar_vs_exptar", "HMS vs. SHMS, X'_{tar}", bins, -0.03, 0.03, bins, -0.1, 0.1);
   TH2F *cut_hyptar_vs_eyptar = new TH2F("cut_hyptar_vs_eyptar", "HMS vs. SHMS, Y'_{tar}", bins, -0.02, 0.025, bins, -0.05, 0.04);
   TH2F *cut_hdelta_vs_edelta = new TH2F("cut_hdelta_vs_edelta", "HMS vs. SHMS, #delta", bins, -16., 5, bins, -12., 10.);

   //-------------------------------------------------------------------------------------------------------------------------

   //Define Additional Kinematic Variables
   Double_t W2;             //Invarianrt Mass Squared
   Double_t X;              //B-jorken X
   Double_t Pf;             //Final Proton Momentum 
   Double_t ki;             //Incident e- momentum
   Double_t kf;             //Final electron momentum

   //Define Kinematic Limits
   Double_t W_min = 0.8;    //GeV
   Double_t W_max = 1.04;     //pion production threhsold?

   //Q2_cent = 4.25 +/- 0.25 limits
   Double_t Q2_min = 3.0;
   Double_t Q2_max = 5.0; //4.5;

   //x-Bjorken Limits x = 1.35 +/- 0.05
   Double_t xbj_min = 0.7;
   Double_t xbj_max = 1.3; //1.40;

   //Missing Energy, Em = 0 MeV (-15 MeV, 15 MeV)
   Double_t Em_min = -15.e-3;
   Double_t Em_max = 15.e-3;
   

   //Determine the charge factor:
   //definition: total charge deposited on target over a time period
   //SIMC assumes it is #generated_events / 1mC
   
   //Charge factor is the total integrated charge assuming a beam current and run time
   //Double_t Ib = 40;       //beam current in microamps (micro-Coulombs / sec),   1 mC = 1000 uC
   //Double_t time = 1.0;     //estimated time (in hours) a run takes (start - end) of run

   Double_t charge_factor = Ib * time * 3600. / 1000.;


   //Double_t Q_bcm1;
   //Double_t Q_bcm2;


   //Double_t Q_avg = (Q_bcm1 + Q_bcm2) / 2.;
   //Double_t charge_factor = Q_avg / 1000.;   //in mC

   //Tracking efficiencies and beamON time
   Double_t e_trk_eff;
   Double_t h_trk_eff;
   Double_t beam_time;

 
   e_trk_eff = 1.0;
   h_trk_eff = 1.0;


   
   Double_t FullWeight;
   

   //************************************************
   
   Long64_t nentries = fChain->GetEntries();

   
   Long64_t nbytes = 0, nb = 0;

   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;

      //-----Define Additional Kinematic Variables--------
      Ein = Ein / 1000.;
      W2 = W*W;
      X = Q2 / (2.*MP*nu);
      Pf = sqrt(pow(nu+MP,2) - MP*MP);
      ki = sqrt(Ein*Ein - me*me);
      kf = Q2 / (4.*ki*pow(sin(theta_e/2.), 2));
  
      //---------------------------------------------------


      //Define Kinematic Cuts
      Bool_t c_Em = Em>Em_min&&Em<Em_max;
      Bool_t c_Xbj = X>xbj_min&&X<xbj_max;
      Bool_t c_Q2 = Q2>Q2_min&&Q2<Q2_max;
      Bool_t c_W = W<W_max;
      
       //-----------Define Acceptance Limit Cuts------------
      
      // e-arm
      Bool_t c_e_xptar = fabs(e_xptar) <= 0.05;
      Bool_t c_e_yptar = fabs(e_yptar) <= 0.025;
      Bool_t c_e_solid = c_e_xptar * c_e_yptar;
      
      // p-arm
      Bool_t c_p_xptar = fabs(h_xptar) <= 0.06;
      Bool_t c_p_yptar = fabs(h_yptar) <= 0.035;
      Bool_t c_p_solid = c_p_xptar * c_p_yptar;
      
      // momentum acceptance
      Bool_t c_e_delta = (-8 <= e_delta) && ( e_delta <= 4);
      Bool_t c_p_delta = (-10 <= h_delta) && ( h_delta <= 12);
      
      // acceptance
      Bool_t c_accept = c_e_solid * c_p_solid * c_e_delta * c_p_delta;
      
      
      //The events must be weighted properly, so that they represent true Yield, and
      //can be compare to actual data
      FullWeight = (Normfac*Weight*charge_factor*e_trk_eff*h_trk_eff)/nentries;

      //cout << "Normfac: " << Normfac << endl;
      //cout << "Weight: " << Weight << endl;
      //cout << "charge: " << charge_factor << endl;
      //cout << "nentries: " << nentries << endl;
      //cout << "FullWeght: " << FullWeight << endl;


      //ANALYSIS OF EVENT-BY-EVENT GOES HERE!!!!!!
      
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
	  cut_k_i->Fill(ki, FullWeight); 
	  cut_k_f->Fill(kf, FullWeight);
	  
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
      k_i->Fill(ki, FullWeight); 
      k_f->Fill(kf, FullWeight);
   
      
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
      
	// if (Cut(ientry) < 0) continue;


   }

   Double_t Yield = W_inv->Integral();
   Double_t c_Yield = cut_W_inv->Integral();
   cout << "Invariant Mass Integral: " << Yield << endl;
   cout << "Estimated RATE: " << Yield /( time *3600. )<< " events/sec (Hz) " << endl;
   cout << "Estimated RATE (hrs): " << Yield /( time  )<< " events/hr " << endl;

   cout << "With Missing Energy Cut (-10 10) MeV" << endl;
   cout << "Invariant Mass Integral: " << c_Yield << endl;
   cout << "Estimated RATE: " << c_Yield /( time *3600. )<< " events/sec (Hz) " << endl;
   cout << "Estimated RATE (hrs): " << c_Yield /( time  )<< " events/hr " << endl;

   
   outfile->Write();
}
