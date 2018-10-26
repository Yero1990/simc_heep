#define singles_gfor_cxx
#include "singles_gfor.h"
#include "set_heep_histos.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void singles_gfor::Loop()
{
  //   In a ROOT session, you can do:
  //      Root > .L singles_gfor.C
  //      Root > singles_gfor t
  //      Root > t.GetEntry(12); // Fill t data members with entry number 12
  //      Root > t.Show();       // Show values of entry 12
  //      Root > t.Show(16);     // Read and show values of entry 16
  //      Root > t.Loop();       // Loop on all entries
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
  
  TString hadron_arm="SHMS";
  TString electron_arm = "HMS";


  //Read Normfac
  Long64_t Normfac = 0.105308E+08;  //1272
  //Long64_t Normfac = 0.105371E+08;  //1161

  // define histograms
  Double_t pi = 3.141592654;
  Double_t dtr = pi/180.;
  Double_t MP = 0.938272; //GeV
  Double_t MD = 1.87561; //GeV
  Double_t MN = 0.939566; //GeV
  Double_t me = 0.000510998;
  
  string simc_file = "hms_single_1272_gfor.root";
  
  TString ofile_name = "Weighted_simc_";
  ofile_name.Append(simc_file);
   
  //create output root file
   TFile *outfile = new TFile(ofile_name.Data(), "recreate");
   
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

   
   

   
   //-------------------------------------------------------------------------------------------------------------------------

   //Define Additional Kinematic Variables
   Double_t W2;             //Invarianrt Mass Squared
   Double_t X;              //B-jorken X
   Double_t Pf;             //Final Proton Momentum 
   Double_t ki;             //Incident e- momentum
   Double_t kf;             //Final electron momentum
   Double_t Ep;             //proton final energy
   Double_t Ee;             //electron final energy
   Double_t th_q;        //Angle between q-vector and beamline (+z axis --lab)
   Double_t hP0;         //HMS Central Momentum
   Double_t theta_e;
   Double_t Ein;

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
   Double_t Em_min = -60.e-3;
   Double_t Em_max = 80.e-3;
   

   //Determine the charge factor:
   //definition: total charge deposited on target over a time period
   //SIMC assumes it is #generated_events / 1mC

   Double_t charge_factor;
   Double_t e_trkEff;
   Double_t h_trkEff;
   Double_t c_LT;


   //charge_factor = 149.189 / 1000.;   //total charge in mC
   
   //Set Efficiencies/computer Live Time to user input
   //e_trkEff = 0.9713;
   //h_trkEff = 1.;
   //c_LT = 0.028247;

   //hms run 1272
   charge_factor = 0.5*(1356.930 + 1402.242) / 1000.;   //total charge in mC
   e_trkEff = 0.9865;
   c_LT = 0.058813;

   Double_t FullWeight;
   

   //************************************************

   Long64_t nentries = fChain->GetEntries();

   Long64_t nbytes = 0, nb = 0;

   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;



     //-----Define Additional Kinematic Variables--------
      Ein = 2.221;
      hP0 = 2.055;
      W2 = W*W;
      X = Q2 / (2.*MP*nu);
      Pf = sqrt(pow(nu+MP,2) - MP*MP);
      Ep = sqrt(MP*MP + Pf*Pf);
      ki = sqrt(Ein*Ein - me*me);
      kf = hP0*(hsdelta/100. + 1.0);
      theta_e = 2.*asin(sqrt(Q2 / (4.*Ein*kf)));
      // kf = Q2 / (4.*ki*pow(sin(theta_e/2.), 2));
      Ee = sqrt(me*me + kf*kf);
      th_q = acos( (ki - kf*cos(theta_e))/q ); //th_q = theta_pq + theta_p;


      //Define Kinematic Cuts
      Bool_t c_Em = Em>Em_min&&Em<Em_max;
      Bool_t c_Xbj = X>xbj_min&&X<xbj_max;
      Bool_t c_Q2 = Q2>Q2_min&&Q2<Q2_max;
      Bool_t c_W = W<W_max;
      
       //-----------Define Acceptance Limit Cuts------------
      
      // e-arm
      Bool_t c_e_xptar = fabs(hsxptar) <= 0.05;
      Bool_t c_e_yptar = fabs(hsyptar) <= 0.025;
      Bool_t c_e_solid = c_e_xptar * c_e_yptar;
      
      // p-arm
      Bool_t c_p_xptar = fabs(ssxptar) <= 0.06;
      Bool_t c_p_yptar = fabs(ssyptar) <= 0.035;
      Bool_t c_p_solid = c_p_xptar * c_p_yptar;
      
      // momentum acceptance
      Bool_t c_e_delta = (-8 <= hsdelta) && ( hsdelta <= 4);
      Bool_t c_p_delta = (-10 <= ssdelta) && ( ssdelta <= 12);
      
      // acceptance
      Bool_t c_accept = c_e_solid * c_p_solid * c_e_delta * c_p_delta;
      
      
      //The events must be weighted properly, so that they represent true Yield, and
      //can be compare to actual data
      FullWeight = (Normfac*Weight*charge_factor*e_trkEff*c_LT)/nentries;

      if (W>0.938 && W<0.939)
	{

      cout << "FUll Weight: " << FullWeight << endl;
      cout << "Weight: " << Weight << endl;
      cout << "Charge Factor: " << charge_factor << endl;
      cout << "Normfac: " << Normfac << endl;
      cout << "nentries: " << nentries << endl;
      cout << "CLT: " << c_LT << endl;
      cout << "e-_eff: " << e_trkEff << endl;
      cout << "W: " << W << endl;
	}
      //ANALYSIS OF EVENT-BY-EVENT GOES HERE!!!!!!
      
      //APPLY CUTS: BEGIN CUTS LOOP
      if (X>0.9)
	{
	  //Kinematics
	  cut_Emiss->Fill(Em, FullWeight);
	  cut_pm->Fill(Pm, FullWeight);
	  cut_Q_2->Fill(Q2, FullWeight);
	  cut_omega->Fill(nu, FullWeight);
	  cut_W_inv->Fill(W, FullWeight);
	  cut_theta_elec->Fill(theta_e/dtr, FullWeight);
	  //cut_theta_prot->Fill(theta_p/dtr, FullWeight);

	  
	  //Additional Kinematics Variables
	  cut_W_2->Fill(W2, FullWeight); 
	  cut_xbj->Fill(X, FullWeight); 
	  cut_P_f->Fill(Pf, FullWeight);
	  cut_k_f->Fill(kf, FullWeight);
	  cut_theta_q->Fill(th_q/dtr, FullWeight);
		  
	  //Reconstructed Target Quantities (Lab Frame)
	  //cut_x_tar->Fill(-1.*tar_x, FullWeight); //MULT. by -1.0 to compare to H.react.Y
	  //cut_y_tar->Fill(tar_y, FullWeight);  //tar_y = H.react.x  
	  //cut_z_tar->Fill(tar_z, FullWeight);
	  
	  
	  //Hadron-Arm Target Reconstruction 
	  cut_hytar->Fill(ssytar, FullWeight);
	  cut_hxptar->Fill(ssxptar, FullWeight);
	  cut_hyptar->Fill(ssyptar, FullWeight);
	  cut_hdelta->Fill(ssdelta, FullWeight);
	  
	  //Hadron-Arm Focal Plane
	  cut_hxfp->Fill(ssxfp, FullWeight);
	  cut_hyfp->Fill(ssyfp, FullWeight);
	  cut_hxpfp->Fill(ssxpfp, FullWeight);
	  cut_hypfp->Fill(ssypfp, FullWeight);
	  
	  //Electron-Arm Target Reconstruction
	  cut_eytar->Fill(hsytar, FullWeight);
	  cut_exptar->Fill(hsxptar, FullWeight);
	  cut_eyptar->Fill(hsyptar, FullWeight);
	  cut_edelta->Fill(hsdelta, FullWeight);
	  
	  //Electron-Arm Focal Plane
	  cut_exfp->Fill(hsxfp, FullWeight);
	  cut_eyfp->Fill(hsyfp, FullWeight);
	  cut_expfp->Fill(hsxpfp, FullWeight);
	  cut_eypfp->Fill(hsypfp, FullWeight);
	  

	  //Fill 2D HMS Focal Plane Quantities
	  cut_h_xfp_vs_yfp->Fill(ssyfp, ssxfp, FullWeight);
	  cut_e_xfp_vs_yfp->Fill(hsyfp, hsxfp, FullWeight);
	  
	  //Fill 2D reconstructed variables
	  cut_hxptar_vs_exptar->Fill(hsxptar, ssxptar, FullWeight);
	  cut_hyptar_vs_eyptar->Fill(hsyptar, ssyptar, FullWeight);
	  cut_hdelta_vs_edelta->Fill(hsdelta, ssdelta, FullWeight);

	  
	  //Heep cross check
	  cut_emiss_vs_pmiss->Fill(Pm, Em, FullWeight);
	  cut_edelta_vs_eyptar->Fill(hsyptar, hsdelta, FullWeight);
      

	  
	  
	}//End CUTS LOOP
      


      //Kinematics
      Emiss->Fill(Em, FullWeight);
      pm->Fill(Pm, FullWeight);
      Q_2->Fill(Q2, FullWeight);
      omega->Fill(nu, FullWeight);
      W_inv->Fill(W, FullWeight);
      theta_elec->Fill(theta_e/dtr, FullWeight);
      //theta_prot->Fill(theta_p/dtr, FullWeight);


      //Additional Kinematics Variables
      W_2->Fill(W2, FullWeight); 
      xbj->Fill(X, FullWeight); 
      P_f->Fill(Pf, FullWeight);
      k_f->Fill(kf, FullWeight);
      theta_q->Fill(th_q/dtr, FullWeight);

      
      //Reconstructed Target Quantities (Lab Frame)
      //x_tar->Fill(tar_x, FullWeight);
      //y_tar->Fill(tar_y, FullWeight);
      //z_tar->Fill(tar_z, FullWeight);

      
      //Hadron-Arm Target Reconstruction 
      hytar->Fill(ssytar, FullWeight);
      hxptar->Fill(ssxptar, FullWeight);
      hyptar->Fill(ssyptar, FullWeight);
      hdelta->Fill(ssdelta, FullWeight);
      
      //Hadron-Arm Focal Plane
      hxfp->Fill(ssxfp, FullWeight);
      hyfp->Fill(ssyfp, FullWeight);
      hxpfp->Fill(ssxpfp, FullWeight);
      hypfp->Fill(ssypfp, FullWeight);
      
      //Electron-Arm Target Reconstruction
      eytar->Fill(hsytar, FullWeight);
      exptar->Fill(hsxptar, FullWeight);
      eyptar->Fill(hsyptar, FullWeight);
      edelta->Fill(hsdelta, FullWeight);
      
      //Electron-Arm Focal Plane
      exfp->Fill(hsxfp, FullWeight);
      eyfp->Fill(hsyfp, FullWeight);
      expfp->Fill(hsxpfp, FullWeight);
      eypfp->Fill(hsypfp, FullWeight);

      
      //Fill 2D HMS Focal Plane Quantities
      h_xfp_vs_yfp->Fill(ssyfp, ssxfp, FullWeight);
      e_xfp_vs_yfp->Fill(hsyfp, hsxfp, FullWeight);

      //Fill 2D reconstructed variables
      hxptar_vs_exptar->Fill(hsxptar, ssxptar, FullWeight);
      hyptar_vs_eyptar->Fill(hsyptar, ssyptar, FullWeight);
      hdelta_vs_edelta->Fill(hsdelta, ssdelta, FullWeight);

      //Heep cross check
      emiss_vs_pmiss->Fill(Pm, Em, FullWeight);
      edelta_vs_eyptar->Fill(hsyptar, hsdelta, FullWeight);
      
	// if (Cut(ientry) < 0) continue;


   } //end entry loop

   outfile->Write();
    
}
