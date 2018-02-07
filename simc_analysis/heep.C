//Analysis for HMS/SHMS coincidence H(e,e'p)


#define heep_cxx
#include "heep.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
//#include <fstream>
//#include <iostream>
void heep::Loop(TString simc_file, TString electron_arm)
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

   TString hadron_arm;

   if (electron_arm=="SHMS")
     {
       hadron_arm ="HMS";
     }
   else{hadron_arm = "SHMS";}

  
    // define histograms
   Double_t pi = 3.141592654;
   Double_t dtr = pi/180.;
   Double_t MP = 0.938272; //GeV
   Double_t MD = 1.87561; //GeV
   Double_t MN = 0.939566; //GeV

   TString ofile_name("weighted_");
   ofile_name.Append(simc_file);
   
   //create output root file
   TFile *outfile = new TFile(ofile_name.Data(), "recreate");

   
   
   //********* Create 1D Histograms **************
   Int_t bins = 100;

   //Kinematics Quantities
   TH1F *Emiss = new TH1F("Emiss","missing energy", bins, -0.5, 0.8);  //binwidth = 0.0025
   TH1F *pm = new TH1F("pm","missing momentum", bins, -0.5, 1.6);
   TH1F *Q_2 = new TH1F("Q_2","Q2", 100, 2., 14.);
   TH1F *omega = new TH1F("omega","Energy Transfer, #omega", bins, 0., 8.);
   TH1F *W_inv = new TH1F("W_inv", "Invariant Mass, W", bins, 0.6, 1.8);
   TH1F *theta_elec = new TH1F("theta_elec", "Electron Scatt. Angle", 100, 0.3, 0.45);
   TH1F *theta_prot = new TH1F("theta_prot", "Proton Scatt. Angle", 100, 0.5, 0.65);

   //Target Reconstruction Variables
   TH1F *x_tar = new TH1F("x_tar", "x_Target", bins, -0.25, 0.25);
   TH1F *y_tar = new TH1F("y_tar", "y_Target", bins, -0.25, 0.25);
   TH1F *z_tar = new TH1F("z_tar", "z_Target", bins, -5.5, 5.5);
   
   //Hadron arm Reconstructed Quantities ( xtar, ytar, xptar, yptar, delta)
   TH1F *hytar = new TH1F("hytar", hadron_arm + " Y_{tar}", bins, -4., 4.);
   TH1F *hxptar = new TH1F("hxptar", hadron_arm + " X'_{tar}", bins, -0.08, 0.08 );
   TH1F *hyptar = new TH1F("hyptar", hadron_arm + " Y'_{tar}", bins, -0.04, 0.04 );
   TH1F *hdelta = new TH1F("hdelta", hadron_arm + " Momentum Acceptance, #delta", bins, -10., 10. );

   //Hadron arm Focal Plane Quantities
   TH1F *hxfp = new TH1F("hxfp", hadron_arm + " X_{fp}", bins, -30., 20.);
   TH1F *hyfp = new TH1F("hyfp", hadron_arm + " Y_{fp}", bins, -15., 10.);
   TH1F *hxpfp = new TH1F("hxpfp", hadron_arm + " X'_{fp}", bins, -0.06, 0.06 );
   TH1F *hypfp = new TH1F("hypfp", hadron_arm + " Y'_{fp}", bins, -0.03, 0.02);

      
   //Electron Arm Reconstructed Quantities ( xtar, ytar, xptar, yptar, delta)
   TH1F *eytar = new TH1F("eytar", electron_arm + " Y_{tar}", bins, -4., 4.);
   TH1F *exptar = new TH1F("exptar", electron_arm + " X'_{tar}", bins, -0.08, 0.08);
   TH1F *eyptar = new TH1F("eyptar", electron_arm + " Y'_{tar}", bins, -0.04, 0.04);
   TH1F *edelta = new TH1F("edelta", electron_arm + " Momentum Acceptance, #delta", bins, -15., 15. );

   //Electron Arm Focal Plane Quantities
   TH1F *exfp = new TH1F("exfp", electron_arm + " X_{fp}", bins, -60., 30.);
   TH1F *eyfp = new TH1F("eyfp", electron_arm + " Y_{fp}", bins, -20., 30.);
   TH1F *expfp = new TH1F("expfp", electron_arm + " X'_{fp}", bins, -0.1, 0.06);
   TH1F *eypfp = new TH1F("eypfp", electron_arm + " Y'_{fp}", bins, -0.04, 0.04);



   //****************Create 2D Histograms at the Focal Plane Quantities*****************
   TH2F *h_xfp_vs_yfp = new TH2F("h_xfp_vs_yfp", "X_{fp} vs Y_{fp}", bins, -50., 20., bins, -10., 20.);
   
   
   /************Define Histos to APPLY CUTS*********************************/
 
     //Kinematics Quantities
   TH1F *cut_Emiss = new TH1F("cut_Emiss","missing energy", bins, -0.5, 0.8);  //binwidth = 0.0025
   TH1F *cut_pm = new TH1F("cut_pm","missing momentum", bins, -0.5, 1.6);
   TH1F *cut_Q_2 = new TH1F("cut_Q_2","Q2", 100, 6., 14.);
   TH1F *cut_omega = new TH1F("cut_omega","Energy Transfer, #omega", bins, 0., 8.);
   TH1F *cut_W_inv = new TH1F("cut_W_inv", "Invariant Mass, W", bins, 0.6, 1.8);
   TH1F *cut_theta_elec = new TH1F("cut_theta_elec", "Electron Scatt. Angle", 100, 0.3, 0.45);
   TH1F *cut_theta_prot = new TH1F("cut_theta_prot", "Proton Scatt. Angle", 100, 0.5, 0.65);

   //Target Reconstruction Variables
   TH1F *cut_x_tar = new TH1F("cut_x_tar", "x_Target", bins, -0.25, 0.25);
   TH1F *cut_y_tar = new TH1F("cut_y_tar", "y_Target", bins, -0.25, 0.25);
   TH1F *cut_z_tar = new TH1F("cut_z_tar", "z_Target", bins, -5.5, 5.5);
   
   //Hadron arm Reconstructed Quantities ( xtar, ytar, xptar, yptar, delta)
   TH1F *cut_hytar = new TH1F("cut_hytar", hadron_arm + " Y_{tar}", bins, -4., 4.);
   TH1F *cut_hxptar = new TH1F("cut_hxptar", hadron_arm + " X'_{tar}", bins, -0.08, 0.08 );
   TH1F *cut_hyptar = new TH1F("cut_hyptar", hadron_arm + " Y'_{tar}", bins, -0.04, 0.04 );
   TH1F *cut_hdelta = new TH1F("cut_hdelta", hadron_arm + " Momentum Acceptance, #delta", bins, -10., 10. );

   //Hadron arm Focal Plane Quantities
   TH1F *cut_hxfp = new TH1F("cut_hxfp", hadron_arm + " X_{fp}", bins, -30., 20.);
   TH1F *cut_hyfp = new TH1F("cut_hyfp", hadron_arm + " Y_{fp}", bins, -15., 10.);
   TH1F *cut_hxpfp = new TH1F("cut_hxpfp", hadron_arm + " X'_{fp}", bins, -0.06, 0.06 );
   TH1F *cut_hypfp = new TH1F("cut_hypfp", hadron_arm + " Y'_{fp}", bins, -0.03, 0.02);

      
   //Electron Arm Reconstructed Quantities ( xtar, ytar, xptar, yptar, delta)
   TH1F *cut_eytar = new TH1F("cut_eytar", electron_arm + " Y_{tar}", bins, -4., 4.);
   TH1F *cut_exptar = new TH1F("cut_exptar", electron_arm + " X'_{tar}", bins, -0.08, 0.08);
   TH1F *cut_eyptar = new TH1F("cut_eyptar", electron_arm + " Y'_{tar}", bins, -0.04, 0.04);
   TH1F *cut_edelta = new TH1F("cut_edelta", electron_arm + " Momentum Acceptance, #delta", bins, -15., 15. );

   //Electron Arm Focal Plane Quantities
   TH1F *cut_exfp = new TH1F("cut_exfp", electron_arm + " X_{fp}", bins, -60., 30.);
   TH1F *cut_eyfp = new TH1F("cut_eyfp", electron_arm + " Y_{fp}", bins, -20., 30.);
   TH1F *cut_expfp = new TH1F("cut_expfp", electron_arm + " X'_{fp}", bins, -0.1, 0.06);
   TH1F *cut_eypfp = new TH1F("cut_eypfp", electron_arm + " Y'_{fp}", bins, -0.04, 0.04);




   //Determine the charge factor:
   //definition: total charge deposited on target over a time period
   //SIMC assumes it is #generated_events / 1mC
   
   //Charge factor is the total integrated charge assuming a beam current and run time
   Double_t Ib = 40;       //beam current in microamps (micro-Coulombs / sec),   1 mC = 1000 uC
   Double_t time = 1.0;     //estimated time (in hours) a run takes (start - end) of run
   Double_t charge_factor = Ib * time * 3600. / 1000.;


   Double_t Q_bcm1;
   Double_t Q_bcm2;

   //Total charge given by BCMs on run 1866:
   // Q_bcm1 = 27323.84;  //uC
   // Q_bcm2 = 27749.918;
   
   
   //Total charge given by BCM1/2 on run 1854
   //Q_bcm1 = 208127.865;
   //Q_bcm2 = 211696.549;

   //Total charge given by BCM1/2 on run 1871
  // Q_bcm1 = 40054.171;
  // Q_bcm2 = 40694.697;

   //Total charge given by BCMs on run 1929
   // Q_bcm1 = 161907.065;       
   //Q_bcm2 = 164453.167;

   
   //Total charge given by BCMs on run 2279
   //Q_bcm1 = 162303.590;       //take into account HMS/SHMS tracking efficiencies, and beam_on time
   //Q_bcm2 = 164865.479;

   //Double_t Q_avg = (Q_bcm1 + Q_bcm2) / 2.;
   //Double_t charge_factor = Q_avg / 1000.;   //in mC

   //Tracking efficiencies and beamON time
   Double_t e_trk_eff;
   Double_t h_trk_eff;
   Double_t beam_time;

   //Tracking efficiencies and beamON time (Run 1929)
    e_trk_eff = 0.9982;
    h_trk_eff = 0.9375;
    //beam_time = 0.89477;

   //Tracking efficiencies and beamON time (Run 2279)
   // e_trk_eff = 0.9990;
   // h_trk_eff = 0.9615;
   // beam_time = 0.6756;

   
   Double_t FullWeight;
   

   //************************************************
   
   Long64_t nentries = fChain->GetEntries();

   
   Long64_t nbytes = 0, nb = 0;

   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;


      
      //The events must be weighted properly, so that they represent true Yield, and
      //can be compare to actual data
      FullWeight = (Normfac*Weight*charge_factor*e_trk_eff*h_trk_eff)/nentries;

      cout << "Normfac: " << Normfac << endl;
      cout << "Weight: " << Weight << endl;
      cout << "charge: " << charge_factor << endl;
      cout << "nentries: " << nentries << endl;
      cout << "FullWeght: " << FullWeight << endl;


      //ANALYSIS OF EVENT-BY-EVENT GOES HERE!!!!!!
      
      //APPLY CUTS: BEGIN CUTS LOOP
      if (W < 1.080)
	{
	  //Kinematics
	  cut_Emiss->Fill(Em, FullWeight);
	  cut_pm->Fill(Pm, FullWeight);
	  cut_Q_2->Fill(Q2, FullWeight);
	  cut_omega->Fill(nu, FullWeight);
	  cut_W_inv->Fill(W, FullWeight);
	  cut_theta_elec->Fill(theta_e, FullWeight);
	  cut_theta_prot->Fill(theta_p, FullWeight);
	  
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
	  
	  
	}//End CUTS LOOP
      


      //Kinematics
      Emiss->Fill(Em, FullWeight);
      pm->Fill(Pm, FullWeight);
      Q_2->Fill(Q2, FullWeight);
      omega->Fill(nu, FullWeight);
      W_inv->Fill(W, FullWeight);
      theta_elec->Fill(theta_e, FullWeight);
      theta_prot->Fill(theta_p, FullWeight);
      
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
      h_xfp_vs_yfp->Fill(h_xfp, h_yfp, FullWeight);
	

	// if (Cut(ientry) < 0) continue;



   }
   outfile->Write();
}
