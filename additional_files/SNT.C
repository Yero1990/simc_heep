#define SNT_cxx
#include "SNT.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void SNT::Loop(Double_t qmin, Double_t qmax, Int_t type, TString head)
{
//   In a ROOT session, you can do:
//      Root > .L SNT.C
//      Root > SNT t
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

   // all variables in GeV

   // define histograms
   Double_t pi = 3.141592654;
   Double_t dtr = pi/180.;
   Double_t MP = 0.938272; //GeV
   Double_t MD = 1.87561; //GeV
   Double_t MN = 0.939566; //GeV


   Double_t rfn_min = 0.005; // cut on r-function

   TString type_t(TString::Format("%d",type));
   TString *simc_file = new TString("simc_results_");
   simc_file->Append(head);
   simc_file->Append(type_t);
   simc_file->Append(".root");

   cout << "New output file : " << simc_file->Data() << endl;
   // open output file
   TFile * o = new TFile(simc_file->Data(),"recreate");
   Int_t nbins_pm = 41;
   // create histograms
   TH1F *Emiss = new TH1F("Emiss","missing energy", 40, -40.e-3, 80.e-3);
 
   

   TH1F *pm = new TH1F("pm","missing momentum", nbins_pm, 0., 1.6);
   

   TH1F *pm_sig = new TH1F("pm_sig","missing momentum", nbins_pm, 0., 1.6); //missing momentum weighted by sigma
   TH1F *cpm = new TH1F("cpm","p_{m} cut", nbins_pm, 0., 1.6);

   TH1F *cpm_sig = new TH1F("cpm_sig","missing momentum", nbins_pm, 0., 1.6);

 
   pm->Sumw2();
   pm_sig->Sumw2();
   cpm->Sumw2();
   cpm_sig->Sumw2();
 

   TH1F *x_bj = new TH1F("x_bj","x-Bjorken", 25, 0.5, 2.0);
   TH1F *thnq = new TH1F("thnq","#theta_{nq}", 36, 0., 180);
   TH1F *Qmu2 = new TH1F("Qmu2","Q^2", 100, 0., 10.);

   TH2F *pmx = new TH2F("pmx", "x_{bj} vs. p_{m}", nbins_pm, 0., 1.6, 25, 0.5, 2.0);
   TH2F *pmth = new TH2F("pmth","#theta_{nq} vs. p_{m}", nbins_pm, 0., 1.6, 36., 0., 180.);
   TH2F *cpmth = new TH2F("cpmth","#theta_{nq} vs. p_{m} cut", nbins_pm, 0., 1.6, 36., 0., 180.);

   TH2F *cpmth_ps = new TH2F("cpmth_ps","#theta_{nq} vs. p_{m} cut (PS)", nbins_pm, 0., 1.6, 36., 0., 180.);

   TH1F *cthnq = new TH1F("cthnq","#theta_{nq} cut", 36, 0., 180);
   TH1F *cx_bj = new TH1F("cx_bj","x-Bjorken cut", 25, 0.5, 2.0);

   TH1F *cpm_ps = new TH1F("cpm_ps","p_{m} cut (PS)", nbins_pm, 0., 1.6);

   TH1F *cpm_x =  new TH1F("cpm_x","p_{m} cut,w = X", nbins_pm, 0., 1.6);
   TH1F *cpm_q2 = new TH1F("cpm_q2", "p_{m} cut,w = Q2", nbins_pm, 0., 1.6);
   TH1F *cpm_qlab = new TH1F("cpm_qlab", "p_{m} cut,w = qlab", nbins_pm, 0., 1.6);
   TH1F *cpm_omega = new TH1F("cpm_omega", "p_{m} cut,w = omega", nbins_pm, 0., 1.6);
   TH1F *cpm_Ef = new TH1F("cpm_Ef", "p_{m} cut,w = Ef", nbins_pm, 0., 1.6);
   TH1F *cpm_pf = new TH1F("cpm_pf", "p_{m} cut,w = pf", nbins_pm, 0., 1.6);
   TH1F *cpm_the = new TH1F("cpm_the", "p_{m} cut,w = theta_e", nbins_pm, 0., 1.6);
   TH1F *cpm_thpq = new TH1F("cpm_thpq", "p_{m} cut,w = thpq", nbins_pm, 0., 1.6);
   TH1F *cpm_phpq = new TH1F("cpm_phpq", "p_{m} cut,w = phipq", nbins_pm, 0., 1.6);
   TH1F *cpm_thnq = new TH1F("cpm_thnq", "p_{m} cut,w = thnq", nbins_pm, 0., 1.6);

   // factor by which the charge is increased
   // incident energy in GeV
   Double_t E_inc = 10.6;

   // this is really import and needs to agree with the settings
   // in the SIMC input files
   // charge for 1 hour with 80uA in mC 
   // Double_t charge_factor = 288.;
   // charge for 1 hour with 70uA in mC 
   //   Double_t charge_factor = 252.;
   // number of successes for each set in the chain
   // charge for 1 hour with 50uA in mC 
   // Double_t charge_factor = 180.;

   //Charge factor is the total integrated charge assuming a beam current and run time
   Double_t Ib = 10;       //beam current in microamps (micro-Coulombs / sec),   1 mC = 1000 uC
   Double_t time = 1.;     //estimated time (in hours) a run takes (start - end) of run
   Double_t charge_factor = Ib * time * 3600. / 1000.;

   
   //Double_t nevents = 30000.;
   Double_t nevents = 300000.;

   // theta_nq cuts
   // central values
   Double_t theta_nq = 40.*dtr;
   // theta_nq width
   Double_t dth = 5.*dtr;

   Double_t theta_nq_min = theta_nq - dth;
   Double_t theta_nq_max = theta_nq + dth;
      
   // central x_bj value
   Double_t x_bj_cent = 1.35;
   Double_t dx_bj = 0.05;

   Double_t x_bj_min = x_bj_cent - 0.05;
   Double_t x_bj_max = x_bj_cent + 0.05;

   //GetEntriesFast does not give the right number of events
   Long64_t nentries = fChain->GetEntries(); 

   nevents = (Double_t) nentries;

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
     Long64_t ientry = LoadTree(jentry);
     if ((jentry%10000) == 0) {
       cout << "Event : " << jentry << endl;
     }
     
     if (ientry < 0) {
       cout << "analyzed  " << jentry << " events " << endl;
       break;
     }
     nb = fChain->GetEntry(jentry);   nbytes += nb;
     // if (Cut(ientry) < 0) continue;
     // put my stuff here
     
     // some additional kinematic values
     // calculate x_bj and theta_nq
     Double_t X = Q2/(2.*MP*nu);
     // calculate fina proton momentum (from reconstructed quantities)
     // neutron energy
     Double_t En = sqrt(Pm*Pm + MN*MN);
     // proton momentum
     Double_t Pf2 = pow((nu + MD - En), 2) - MP*MP;
     Double_t Pf = sqrt(Pf2);
     
     // theta_nq
     Double_t cth_nq = (q - Pf*cos(thetapq))/Pm;
     Double_t th_nq = 0.;
     if (fabs(cth_nq) < 1.0) th_nq = acos(cth_nq);

     // electron kinematics
     Double_t E_f = E_inc - nu;
     Double_t theta_e = 2.*asin( sqrt(Q2/(4.*E_inc*E_f)) );
   

     // Normfac SIMC normalization factor for counts
     // nevents : # events per set MUST be the same for all
     // charge_factor real charge in units of mC

     Double_t FullWeight = Weight*Normfac*charge_factor/nevents;

     Emiss->Fill(Em,FullWeight);
    
     pm->Fill(Pm,FullWeight);
     pm_sig->Fill(Pm,Weight); //missing momentum weighted by the actual cross section (using v18 Model for deuteron)
     
     x_bj->Fill(X,FullWeight); // calculate
     thnq->Fill(th_nq/dtr,FullWeight); //calculate
     Qmu2->Fill(Q2);
     
     pmx->Fill(Pm, X, FullWeight);
     pmth->Fill(Pm, th_nq/dtr, FullWeight);
     
     // colid angle cuts
     // e-arm
     Int_t c_e_xptar = fabs(e_xptar) <= 0.05;
     Int_t c_e_yptar = fabs(e_yptar) <= 0.025;
     Int_t c_e_solid = c_e_xptar * c_e_yptar;

     // p-arm
     Int_t c_p_xptar = fabs(h_xptar) <= 0.06;
     Int_t c_p_yptar = fabs(h_yptar) <= 0.035;
     Int_t c_p_solid = c_p_xptar * c_p_yptar;
     
     // momentum acceptance
     Int_t c_e_delta = (-8 <= e_delta) && ( e_delta <= 4);
     Int_t c_p_delta = (-10 <= h_delta) && ( h_delta <= 10);
     
     // acceptance
     Int_t c_accept = c_e_solid * c_p_solid * c_e_delta * c_p_delta;
     
     // cut on theta_pq
     Int_t c_th = (theta_nq_min <= th_nq) && (th_nq <= theta_nq_max);
     // cut on Q2
     Int_t c_q2 = (qmin <= Q2) && (Q2 <= qmax);
     // cut on missing energy
     Int_t c_emiss = (Em >=-10.e-3) && (Em <=25.e-3);

     // cut on x
     Int_t c_x = (x_bj_min <= X) && (X <= x_bj_max);
     
     // acceptance cuts and missing energy cuts
   
     if (c_x && c_q2 && c_emiss && c_accept) {
       cpm->Fill(Pm,FullWeight);
       cpm_sig->Fill(Pm, Weight);  //missing momentum after passing acceptance cuts, weighted by actual cross section
       cpm_ps->Fill(Pm);                  // for phase space estimate
     
      
       cx_bj->Fill(X,FullWeight);
       cthnq->Fill(th_nq/dtr,FullWeight);
       cpmth->Fill(Pm, th_nq/dtr, FullWeight);
       cpmth_ps->Fill(Pm, th_nq/dtr);                 // for phase space estimate
       // histograms for averaged kinematic variables
       cpm_x->Fill(Pm, FullWeight * X);
       cpm_q2->Fill(Pm, FullWeight * Q2);
       cpm_qlab->Fill(Pm, FullWeight * q);
       cpm_omega->Fill(Pm, FullWeight * nu);
       cpm_Ef->Fill(Pm, FullWeight * E_f);
       cpm_pf->Fill(Pm, FullWeight * Pf);
       cpm_the->Fill(Pm, FullWeight * theta_e/dtr);
       cpm_thpq->Fill(Pm, FullWeight * thetapq/dtr);
       cpm_phpq->Fill(Pm, FullWeight * phipq/dtr);
       cpm_thnq->Fill(Pm, FullWeight * th_nq/dtr);
     }
   }
   // write all histograms
   o->Write();
}
