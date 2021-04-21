//Code to check proton (calculated - measured) / measured momentum in the HMS
//hmsP_calc(Ebeam, proton_angle)---> No delta dependence on the calculated momentum.
//hmsP_meas (delta)--->delta measured from the optics matrix elements

//**NOTE: Assuming the hms angle and beam energy are well known, any deviation of the
//calculated momentum in th hadron arm, is due to te delta. In SIMC, this difference
//should be practically zero.  In data, however, the momentum fraction devitation can be 
//larger (>0.1 %), so a momentum correction factor should be applied to the data, which 
//will bring this difference closer to zero (<0.1%)


#include "TMath.h"
void calc_hProt_PDiff()
{

  gROOT->SetBatch(kTRUE);
  gStyle->SetOptFit();

  //Object array to store histograms
  TObjArray data_HList[4]; 
  TObjArray simc_HList[4]; 

  //Central Momentum Used
   Double_t Pc[4] = {2.938,  3.48, 2.31, 1.8899};  //UnCorrected, EPICS
  //  Double_t Pc[4] = {2.935545,  3.47588, 2.310387, 1.891227};  //Corrected HMS Momentum

  //Define some constants
  Double_t Mp = 0.938272;  //proton mass

  Double_t Eb = 10.6005;// D2 Heep Data

  //Double_t Eb[4] = {2.221728, 6.42765, 6.42765, 10.6005};


  //Define some variables to be determined inside the entry loop
  Double_t hmsP_calc;   //calculated HMS momentum
  Double_t hmsP_meas;   //measured HMS momentum 

  //Define TTree variables
  Double_t Normfac;
  Double_t h_delta;
  Double_t h_yptar;
  Double_t h_xptar;
  Double_t h_ytar;
  
  Double_t e_delta;
  Double_t e_yptar;
  Double_t e_xptar;
  Double_t e_ytar;

  //Focal Plane
  Double_t h_xfp;
  Double_t h_xpfp;
  Double_t h_yfp;
  Double_t h_ypfp;

  Double_t q;
  Double_t nu;
  Double_t Q2;
  Double_t W;
  Double_t Pm;
  Double_t Em;
  Double_t Weight;
  Double_t theta_e;
  Double_t theta_p;
  
  Double_t hPf;
  Double_t ePf;


  //Define Histograms
  TH1F *hist_hPcalc[4];
  TH1F *hist_hPmeas[4];
  TH1F *hist_hPDev[4];
  TH1F *hist_Em[4];

  TH2F *hist_hPDev_xfp[4];   //Momentum fraction correlations with HMS focal plane
  TH2F *hist_hPDev_xpfp[4];  
  TH2F *hist_hPDev_yfp[4]; 
  TH2F *hist_hPDev_ypfp[4]; 

  Double_t xRes_arr[4];                                                                                                               
  Double_t yRes_arr[4];                                                                                                
  Double_t yRes_arr_err[4];

  //Define Canvas to Draw
  TCanvas *c0_simc = new TCanvas("c0_simc", "Missing Enrgy", 2000, 1000);   
  c0_simc->Divide(2,2);
  TCanvas *c1_simc = new TCanvas("c1_simc", "Calculated/Measured HMS Momentum", 2000, 1000);   
  c1_simc->Divide(2,2);
  TCanvas *c2_simc = new TCanvas("c2_simc", "HMS Momentum Difference", 2000,1000);
  c2_simc->Divide(2,2);
  TCanvas *c3_simc = new TCanvas("c3_simc","HMS Momentum Difference vs. Kinematics Group", 2500,1000);
  
  TCanvas *c4_simc[4];                                                                            

  //Define Some Cuts
  Bool_t c_simcEm;
  Bool_t c_simc_edelta;
  Bool_t c_simc_hdelta;
 
  Double_t FullWeight;

  int run[4] = {3288, 3371, 3374, 3377};
  //int run[4] = {1, 2, 3, 4};




  //Loop over all kinematic groups
  for(int index = 0; index<4; index++)
    {

      simc_HList[index](0);

      string filename = Form("../../../worksim_voli/D2_heep_%d_ORIGINAL.root", run[index]);             
      //string filename = Form("../../../worksim_voli/hmsProtons/UnCorrected/D2_heep_%d.root", run[index]);                                   
      
      //Additional proton data
      //string filename = Form("../../../worksim_voli/heep_kg%dcoin.root", run[index]);                                   

      TFile *f1 = new TFile(filename.c_str());                                                                           

      //Get TTree                                                                                                                     
      TTree *T = (TTree*)f1->Get("SNT");   
      
      /*-------DEUT SIMC---------------------*/
      //Set Branch Address for some SIMC variables
      T->SetBranchAddress("Normfac", &Normfac);
      T->SetBranchAddress("h_delta", &h_delta);
      T->SetBranchAddress("h_yptar", &h_yptar);
      T->SetBranchAddress("h_xptar", &h_xptar);
      T->SetBranchAddress("h_ytar", &h_ytar); 
      T->SetBranchAddress("h_xfp", &h_xfp);                                 
      T->SetBranchAddress("h_xpfp", &h_xpfp);
      T->SetBranchAddress("h_yfp", &h_yfp);                        
      T->SetBranchAddress("h_ypfp", &h_ypfp);
      T->SetBranchAddress("q", &q);             
      T->SetBranchAddress("nu", &nu);           
      T->SetBranchAddress("Q2", &Q2);           
      T->SetBranchAddress("W", &W);             
      T->SetBranchAddress("Em", &Em);           
      T->SetBranchAddress("Pm", &Pm);           
      T->SetBranchAddress("Weight", &Weight);
      T->SetBranchAddress("theta_e", &theta_e);
      T->SetBranchAddress("theta_p", &theta_p);
      T->SetBranchAddress("e_delta", &e_delta);
      T->SetBranchAddress("h_pf", &hPf);   
      T->SetBranchAddress("e_pf", &ePf); 
      

      hist_hPcalc[index] = new TH1F(Form("hP_calc_Run%d", run[index]), Form("Run %d: HMS Calc./Meas. Momentum", run[index]),  100, 1.5, 4);                    
      hist_hPmeas[index] = new TH1F(Form("hP_meas_Run%d", run[index]), "",  100, 1.5, 4);
      
      //hist_hPDev[index] = new TH1F(Form("hP_Dev_Run%d", run[index]), Form("Run %d: HMS Momentum Fraction", run[index]), 100, -0.05, 0.05);
            
      hist_hPDev[index] = new TH1F(Form("hP_Dev_Run%d", run[index]), Form("Run %d: HMS Momentum Difference", run[index]), 100, -0.1, 0.1);


      hist_Em[index] = new TH1F(Form("h_Em_Run%d", run[index]), Form("Run %d: Missing Energy", run[index]), 100, -0.2, 0.3);

      hist_hPDev_xfp[index] = new TH2F(Form("hPDiff_vs_xfp: Run %d", run[index]), "", 100, -40, 40, 80, -0.05, 0.05);
      hist_hPDev_xpfp[index] = new TH2F(Form("hPDiff_vs_xpfp: Run %d", run[index]), "", 100, -0.06, 0.06, 80, -0.05, 0.05); 
      hist_hPDev_yfp[index] = new TH2F(Form("hPDiff_vs_yfp: Run %d", run[index]), "", 100, -10, 30, 80, -0.05, 0.05);        
      hist_hPDev_ypfp[index] = new TH2F(Form("hPDiff_vs_ypfp: Run %d", run[index]), "", 100, -0.02, 0.04, 80, -0.05, 0.05);  

      hist_hPcalc[index]->GetXaxis()->SetTitle("HMS Momentum [GeV/c]");
      hist_hPDev[index]->GetXaxis()->SetTitle("HMS [P_{calc} - P_{meas}] [GeV]");

      hist_Em[index]->GetXaxis()->SetTitle("Missing Energy, E{miss} [GeV]");
      

      hist_hPDev_xfp[index]->GetXaxis()->SetTitle("HMS X_{fp}");
      hist_hPDev_xfp[index]->GetYaxis()->SetTitle("HMS [P_{calc} - P_{meas}] / P_{meas}");   
     
      hist_hPDev_xpfp[index]->GetXaxis()->SetTitle("HMS X'_{fp}"); 
      hist_hPDev_xpfp[index]->GetYaxis()->SetTitle("HMS [P_{calc} - P_{meas}] / P_{meas}"); 

      hist_hPDev_yfp[index]->GetXaxis()->SetTitle("HMS Y_{fp}");                                                        
      hist_hPDev_yfp[index]->GetYaxis()->SetTitle("HMS [P_{calc} - P_{meas}] / P_{meas}");                              
                  
      hist_hPDev_ypfp[index]->GetXaxis()->SetTitle("HMS Y'_{fp}");                                                         
      hist_hPDev_ypfp[index]->GetYaxis()->SetTitle("HMS [P_{calc} - P_{meas}] / P_{meas}");          
	

      simc_HList[index].Add(hist_hPcalc[index]);
      simc_HList[index].Add(hist_hPmeas[index]);
      simc_HList[index].Add(hist_hPDev[index]);
      simc_HList[index].Add(hist_Em[index]);

      simc_HList[index].Add(hist_hPDev_xfp[index]);
      simc_HList[index].Add(hist_hPDev_xpfp[index]);
      simc_HList[index].Add(hist_hPDev_yfp[index]);
      simc_HList[index].Add(hist_hPDev_ypfp[index]);



      //-----------LOOP OVER ALL ENTRIES IN TREE-----------------------
      //Loop over all entries
      for(int i=0; i<T->GetEntries(); i++){
      //for(int i=0; i<1000; i++){

	T->GetEntry(i);

	//Define CUts
	c_simcEm = Em < 0.04;
	c_simc_edelta = e_delta > -10. && e_delta < 22.;
	c_simc_hdelta = h_delta > -8. && h_delta < 8.; 
 
	//Calculate the cross-sect. weight
	FullWeight = (Normfac*Weight)/T->GetEntries();
	
	//Calculated proton Momentum (Using formula)
	hmsP_calc = 2.*Mp*Eb*(Eb + Mp)*TMath::Cos(theta_p) / (Mp*Mp + 2.*Mp*Eb + Eb*Eb*TMath::Power(TMath::Sin(theta_p),2));
	//Measured Momentum Obtained from delta
	hmsP_meas = hPf / 1000.;

	//Fill Histograms
	if (c_simcEm&&c_simc_edelta&&c_simc_hdelta ){
	hist_hPcalc[index]->Fill(hmsP_calc, FullWeight);      //Fill calculated momentum
	hist_hPmeas[index]->Fill(hmsP_meas, FullWeight);      //Fill measure momentum
	//hist_hPDev[index]->Fill( (hmsP_calc - hmsP_meas)/ hmsP_meas, FullWeight);   //Fill Fractional  Deviation of Calculated from Measured Momentum

	hist_hPDev[index]->Fill( (hmsP_calc - hmsP_meas), FullWeight);   //Fill Fractional  Deviation of Calculated from Measured Momentum


	hist_hPDev_xfp[index]->Fill(h_xfp, (hmsP_calc - hmsP_meas)/ hmsP_meas, FullWeight);
        hist_hPDev_xpfp[index]->Fill(h_xpfp, (hmsP_calc - hmsP_meas)/ hmsP_meas, FullWeight);  
	hist_hPDev_yfp[index]->Fill(h_yfp, (hmsP_calc - hmsP_meas)/ hmsP_meas, FullWeight);
	hist_hPDev_ypfp[index]->Fill(h_ypfp, (hmsP_calc - hmsP_meas)/ hmsP_meas, FullWeight); 

	}

	hist_Em[index]->Fill(Em, FullWeight);  //Fill Missing Energy
      } //end entry loop
      

      c0_simc->cd(index+1);
      hist_Em[index]->Draw("hist");

      c1_simc->cd(index+1);
      hist_hPcalc[index]->Draw("hist");
      hist_hPcalc[index]->SetLineColor(kRed);
      hist_hPmeas[index]->Draw("sameshist");


      c4_simc[index] = new TCanvas(Form("c4_simc, Run %d", run[index]), "Fract. Momentum vs. HMS Focal Plane", 2000, 1000);       
      c4_simc[index]->Divide(2,2);  
      c4_simc[index]->cd(1);
      hist_hPDev_xfp[index]->Draw("colz");
      c4_simc[index]->cd(2); 
      hist_hPDev_xpfp[index]->Draw("colz");
      c4_simc[index]->cd(3);
      hist_hPDev_yfp[index]->Draw("colz"); 
      c4_simc[index]->cd(4);
      hist_hPDev_ypfp[index]->Draw("colz");   

      c4_simc[index]->SaveAs(Form("simc_hPdiff_vs_HMS_FocalPlane_%d.pdf", run[index]));
      
      //Get maximum bin value/sigma from momentum difference histo to use as fit parameters
      int simc_bin_max = hist_hPDev[index]->GetMaximumBin(); 
      Double_t simc_xmax_val = hist_hPDev[index]->GetXaxis()->GetBinCenter(simc_bin_max);
      Double_t simc_sig_Res = hist_hPDev[index]->GetStdDev();
      
      c2_simc->cd(index+1);
      hist_hPDev[index]->Draw();
      
      TF1 *fit = new TF1("fit", "gaus", simc_xmax_val - 2.0*simc_sig_Res, simc_xmax_val + 2.0*simc_sig_Res);
      hist_hPDev[index]->Fit("fit", "R");
      
      Double_t simc_mu_Res_fit = fit->GetParameter(1);
      Double_t simc_sig_Res_fit = fit->GetParameter(2);
      Double_t simc_mu_Res_err_fit =  fit->GetParError(1); 

      cout << "simc mean_fit = " << simc_mu_Res_fit << endl;
      cout << "simc mean_fit_err = " << simc_mu_Res_err_fit << endl;

      xRes_arr[index] = index + 1;                                                                                                               
      yRes_arr[index] = simc_mu_Res_fit;                                                                                                
      yRes_arr_err[index] = simc_mu_Res_err_fit;
           
      //cout << "yRes_arr = " << yRes_arr[index] << endl;

      //Write Histograms to a ROOTfile
      TFile *simc_outROOT = new TFile(Form("HMS_heepSIMC_histos_%d.root", run[index]), "recreate");
      simc_HList[index].Write();
      simc_outROOT->Close();

      
    } // end filename loop


 

  Double_t ex[4] = {0.};
  TGraphErrors* gr = new TGraphErrors(4,xRes_arr,yRes_arr, ex, yRes_arr_err);
  
  c3_simc->cd();

  gr->SetTitle(" (P_calc - P_meas) vs. D2 Elastics");

  gr->GetYaxis()->SetTitle("(P_{calc} - P_{meas}) [GeV]");
  gr->GetYaxis()->CenterTitle();
  
  gr->GetXaxis()->SetTitle("Kinematic Group");
  gr->GetXaxis()->CenterTitle();

  gr->SetMarkerStyle(21);
  gr->SetMarkerColor(kBlack);
  gr->SetMarkerSize(1.8);
  gr->Draw("AP");
  
  c0_simc->SaveAs("simc_Missing_Energy.pdf");
  c1_simc->SaveAs("simc_HMS_Calc_MeasP.pdf");
  c2_simc->SaveAs("simc_Momentum_Diff.pdf");
  c3_simc->SaveAs("simc_Pdiff_Graph.pdf");


  //---------------------DATA----------------------------
  

  

 //Define some variables to be determined inside the entry loop
  Double_t htheta_p;     //proton arm angle (event by event)

  //Define TTree variables
  TString n_xangle = "H.kin.secondary.xangle";
  TString n_theta_e = "P.kin.primary.scat_ang_deg";   //e- scat angle [deg]
  TString n_hmsP_meas = "H.gtr.p";
  TString n_emiss = "H.kin.secondary.emiss";
  TString n_hdelta = "H.gtr.dp";
  TString n_edelta = "P.gtr.dp";

  TString n_hxfp = "H.dc.x_fp";
  TString n_hxpfp = "H.dc.xp_fp"; 
  TString n_hyfp = "H.dc.y_fp";               
  TString n_hypfp = "H.dc.yp_fp"; 

  

 
  Double_t xangle;
  Double_t ptheta_e;   //e- scat angle [rad]
  Double_t hms_Pmeas;
  Double_t emiss;
  Double_t hdelta;
  Double_t edelta;
  Double_t hxfp, hxpfp, hyfp, hypfp;

  
  //----DEFINE SOME CUTS
  double Em_min[4] = {-0.125, -0.115, -0.14, -0.14}; //Emiss < Em_min Determined using EPICS momenta data D2 hEEP
  
  //double Em_min[4] = {-0.11, -0.08, -0.13, -0.135}; //Emiss < Em_min Determined after D2 Heep HMSP COrr. and SHMS CAL Calibration
  //double Em_min[4] = {0.04, 0.04, 0.04, 0.04};

  //double Em_min[4] = {-0.12, -0.1, -0.13, -0.13}; //Emiss < Em_min Determined after Using dP/P = -0.001, dth = 0.0005 (yptar offset), dEb/Eb = 8e-4 rad momenta data D2 hEEP

  //double Em_min[4] = {-0.015, -0.045, -0.05, -0.12}; //Emiss < Em_min Determined using EPICS momenta hprotkg1,2,3,4

  Bool_t c_dataEm;
  Bool_t c_data_edelta;
  Bool_t c_data_hdelta;   
         

  //-------------CREATE EMPTY HISTOGRAMS---------------------------------------------
  
  
  TH1F *histData_hPcalc[4];
  TH1F *histData_hPmeas[4];
  TH1F *histData_hPDev[4];
  TH1F *histData_Em[4];
  
  //Momentum fraction correlations with HMS focal plane
  TH2F *histhPDev_xfp[4];   
  TH2F *histhPDev_xpfp[4];  
  TH2F *histhPDev_yfp[4]; 
  TH2F *histhPDev_ypfp[4]; 
 
  Double_t data_xRes_arr[4];
  Double_t data_yRes_arr[4];
  Double_t data_yRes_arr_err[4];
  
  //--------------------------------------------------------

  TCanvas *c0 = new TCanvas("c0", "Missing Energy", 2000, 1000);   
  c0->Divide(2,2);
  TCanvas *c1 = new TCanvas("c1", "Calculated/Measured HMS Momentum", 2000, 1000);   
  c1->Divide(2,2);
  TCanvas *c2 = new TCanvas("c2", "HMS Momentum Difference", 2000,1000);
  c2->Divide(2,2);
  TCanvas *c3 = new TCanvas("c3","HMS Momentum Difference vs. Kinematic Group", 2500,1000);
  
  TCanvas *c4[4];
  

  //Loop over runs
    for(int index=0; index<4; index++)
      {          

	data_HList[index](0);

                                                                           
	//Open TFile (D2 proton heep data)
	
	string filename = Form("../../../../hallc_replay/ROOTfiles/coin_replay_heep_check_%d_-1.root",run[index]);
	//string filename = Form("../../../../hallc_replay/ROOTfiles/good_Heep_hmsProt/UnCorrected/coin_replay_heep_check_%d_-1.root",run[index]);
	
	//Additional Proton data
	//string filename = Form("../../../../hallc_replay/ROOTfiles/good_Heep_hmsProt/hprot_kg%d.root",run[index]);


	TFile *f1 = new TFile(filename.c_str());
	
	//Get TTree
	TTree *T = (TTree*)f1->Get("T");
	
	
	//Set Branch Address
	T->SetBranchAddress(n_xangle, &xangle);
	T->SetBranchAddress(n_theta_e, &ptheta_e);
	T->SetBranchAddress(n_hmsP_meas, &hms_Pmeas);
	T->SetBranchAddress(n_emiss, &emiss);
	T->SetBranchAddress(n_hdelta, &hdelta);
	T->SetBranchAddress(n_edelta, &edelta);
	T->SetBranchAddress(n_hxfp, &hxfp);
	T->SetBranchAddress(n_hxpfp, &hxpfp);
	T->SetBranchAddress(n_hyfp, &hyfp);
	T->SetBranchAddress(n_hypfp, &hypfp);


	histData_hPcalc[index] =  new TH1F(Form("data_Pcalc_Run%d", run[index]), Form("Run %d: HMS Calc./Meas. Momentum",run[index]), 100, 1.5, 4);
	histData_hPmeas[index] = new TH1F(Form("data_Pmeas_Run%d", run[index]), "", 100, 1.5, 4);

	//histData_hPDev[index] = new TH1F(Form("data_pDiff_Run%d",run[index]), Form("Run %d: HMS Momentum Fraction", run[index]), 100, -0.05, 0.05);
	histData_hPDev[index] = new TH1F(Form("data_pDiff_Run%d",run[index]), Form("Run %d: HMS Momentum Difference", run[index]), 100, -0.1, 0.1);


	histData_Em[index] = new TH1F(Form("Emiss: Run %d",run[index]), Form("Run %d: Missing Energy", run[index]), 100, -0.2, 0.3);

	histhPDev_xfp[index] = new TH2F(Form("hPDiff_vs_xfp: Run %d", run[index]), "", 100, -40, 40, 80, -0.05, 0.05);
	histhPDev_xpfp[index] = new TH2F(Form("hPDiff_vs_xpfp: Run %d", run[index]), "", 100, -0.06, 0.06, 80, -0.05, 0.05); 
	histhPDev_yfp[index] = new TH2F(Form("hPDiff_vs_yfp: Run %d", run[index]), "", 100, -10, 30, 80, -0.05, 0.05);        
	histhPDev_ypfp[index] = new TH2F(Form("hPDiff_vs_ypfp: Run %d", run[index]), "", 100, -0.02, 0.04, 80, -0.05, 0.05);  

	histData_hPcalc[index]->GetXaxis()->SetTitle("HMS Momentum [GeV/c]");                                                                                                             
	histData_hPDev[index]->GetXaxis()->SetTitle("HMS [P_{calc} - P_{meas}] [GeV]");                                                                       
	histData_Em[index]->GetXaxis()->SetTitle("Missing Energy, E{miss} [GeV]");   
	
	data_HList[index].Add(histData_hPcalc[index]);
	data_HList[index].Add(histData_hPmeas[index]);
       	data_HList[index].Add(histData_hPDev[index]);
	data_HList[index].Add(histData_Em[index]);

	data_HList[index].Add(histhPDev_xfp[index]);
	data_HList[index].Add(histhPDev_xpfp[index]);
	data_HList[index].Add(histhPDev_yfp[index]);
	data_HList[index].Add(histhPDev_ypfp[index]);



	histhPDev_xfp[index]->GetXaxis()->SetTitle("HMS X_{fp}");
	histhPDev_xfp[index]->GetYaxis()->SetTitle("HMS [P_{calc} - P_{meas}] / P_{meas}");   
	
	histhPDev_xpfp[index]->GetXaxis()->SetTitle("HMS X'_{fp}"); 
	histhPDev_xpfp[index]->GetYaxis()->SetTitle("HMS [P_{calc} - P_{meas}] / P_{meas}"); 
	
	histhPDev_yfp[index]->GetXaxis()->SetTitle("HMS Y_{fp}");                                                        
	histhPDev_yfp[index]->GetYaxis()->SetTitle("HMS [P_{calc} - P_{meas}] / P_{meas}");                              
                  
	histhPDev_ypfp[index]->GetXaxis()->SetTitle("HMS Y'_{fp}");                                                         
	histhPDev_ypfp[index]->GetYaxis()->SetTitle("HMS [P_{calc} - P_{meas}] / P_{meas}");  
      
      
  
      //-----------LOOP OVER ALL ENTRIES IN TREE-----------------------
      //Loop over all entries
	for(int i=0; i<T->GetEntries(); i++){
	//for(int i=0; i<50000; i++){

	T->GetEntry(i);
 
	//Define the CUTS
	c_dataEm = emiss < Em_min[index];
	c_data_edelta = edelta > -10. && edelta < 22.;
	c_data_hdelta = hdelta > -8. && hdelta < 8.; 
	//cout << "c_DATAEm = " << c_dataEm << " Emiss = " << emiss << endl;


	//HMS Particle angles relative to central angle
	htheta_p = xangle*180./TMath::Pi() - ptheta_e;

	//HMS Particle calculated momentum (Using only the particle angle and beam energy)
	hmsP_calc = 2.*Mp*Eb*(Eb + Mp)*TMath::Cos(htheta_p*TMath::Pi()/180.) / (Mp*Mp + 2.*Mp*Eb + Eb*Eb*TMath::Power(TMath::Sin(htheta_p*TMath::Pi()/180.),2)) ;

	
	if(TMath::Abs(htheta_p)<100 && TMath::Abs(hmsP_calc)<100 && TMath::Abs(hms_Pmeas)<100.&& c_dataEm && c_data_edelta && c_data_hdelta )
	  {

	    //Fill Histograms
	    histData_hPcalc[index]->Fill(hmsP_calc);  //calculated momentum from formula (function of htheta_p and Ebeam)
	    histData_hPmeas[index]->Fill(hms_Pmeas);  //measured momentum H.gtr.p
	    //histData_hPDev[index]->Fill((hmsP_calc - hms_Pmeas) / hms_Pmeas);  //HMS (Calculated - Measured)/Measured Momentum (fractonal deviation from measured)
	    
	    histData_hPDev[index]->Fill(hmsP_calc - hms_Pmeas);  //HMS (Calculated - Measured)/


	    histhPDev_xfp[index]->Fill(hxfp, (hmsP_calc - hms_Pmeas)/ hms_Pmeas);
	    histhPDev_xpfp[index]->Fill(hxpfp, (hmsP_calc - hms_Pmeas)/ hms_Pmeas);  
	    histhPDev_yfp[index]->Fill(hyfp, (hmsP_calc - hms_Pmeas)/ hms_Pmeas);
	    histhPDev_ypfp[index]->Fill(hypfp, (hmsP_calc - hms_Pmeas)/ hms_Pmeas); 


	  }
	histData_Em[index]->Fill(emiss);   //FIlling Missing Energy Histogram

	}
      
      //---------END ENTRY LOOP ---------------------------------------
      //DRaw HIstograms
      c0->cd(index+1);
      histData_Em[index]->Draw();
      

      c1->cd(index+1);
      histData_hPcalc[index]->Draw();
      histData_hPcalc[index]->SetLineColor(kRed);
      histData_hPmeas[index]->Draw("sames");

      //Get max bin value/sigma from momentum difference histo to use as fit parameters
      int bin_max = histData_hPDev[index]->GetMaximumBin();
      Double_t xmax_val = histData_hPDev[index]->GetXaxis()->GetBinCenter(bin_max); 
      Double_t sig_Res = histData_hPDev[index]->GetStdDev();
       
      c2->cd(index+1);
      histData_hPDev[index]->Draw();

      c4[index] = new TCanvas(Form("c4, Run %d", run[index]), Form("Fract. Momentum vs. HMS Focal Plane, Run %d", run[index]), 2000, 1000);       
      c4[index]->Divide(2,2);  
      c4[index]->cd(1);
      histhPDev_xfp[index]->Draw("colz");
      c4[index]->cd(2); 
      histhPDev_xpfp[index]->Draw("colz");
      c4[index]->cd(3);
      histhPDev_yfp[index]->Draw("colz"); 
      c4[index]->cd(4);
      histhPDev_ypfp[index]->Draw("colz");   
      c4[index]->SaveAs(Form("data_hPdiff_vs_HMS_FocalPlane_%d.pdf", run[index]));




      TF1 *dfit = new TF1("dfit", "gaus", xmax_val - sig_Res, xmax_val + sig_Res);
      histData_hPDev[index]->Fit("dfit", "R");
  
      
      Double_t mu_Res_fit = dfit->GetParameter(1);
      Double_t sig_Res_fit = dfit->GetParameter(2);
      Double_t mu_Res_err_fit =  dfit->GetParError(1);

      cout << "data_mu_Res_fit = " << mu_Res_fit << endl;
      cout << "data_mu_Res_err_fit = " << mu_Res_err_fit << endl;

      data_xRes_arr[index] = index + 1;          //kin group
      data_yRes_arr[index] = mu_Res_fit;          //mean fit fractional deviation from meas. momentum
      data_yRes_arr_err[index] =  mu_Res_err_fit;    //error from mean fit

       //Write Histograms to a ROOTfile
      TFile *data_outROOT = new TFile(Form("HMS_heepDATA_histos_%d.root", run[index]), "recreate");
      data_HList[index].Write();
      data_outROOT->Close();

      

    } //end loop over runs 
  
  Double_t ex_data[4] = {0.};
  
  TGraphErrors* data_gr = new TGraphErrors(4,data_xRes_arr,data_yRes_arr, ex_data, data_yRes_arr_err);
  
  
  c3->cd();

  data_gr->SetTitle(" (P_{calc} - P_{meas}) vs. Kin. Group");

  data_gr->GetYaxis()->SetTitle("(P_{calc} - P_{meas})  [GeV]");
  data_gr->GetYaxis()->CenterTitle();
  
  data_gr->GetXaxis()->SetTitle("Kinematic Group");
  data_gr->GetXaxis()->CenterTitle();

  data_gr->SetMarkerStyle(21);
  data_gr->SetMarkerColor(kBlack);
  data_gr->SetMarkerSize(1.8);
  data_gr->Draw("AP");
  
  c0->SaveAs("data_Missing_Energy.pdf");
  c1->SaveAs("data_HMS_Calc_MeasP.pdf");
  c2->SaveAs("data_Momentum_Diff.pdf");
  c3->SaveAs("data_Pdiff_Graph.pdf");



  //----------------------PLOT THE DIFFERENCE BETWEEN DATA AND SIMC Momentum Dev.-----

  ofstream ofile;
  ofile.open("momentum_fraction_diff.data");

  ofile << "#This file contains the momentum fraction difference between DATA/SIMC " << endl;
  ofile << "#The momentum fraction is defined as: dP = (Pcalc - Pmeas)" << endl;
  ofile << "#The mean of this distribution is fitted for data and simc, and the difference is taken " << endl;
  ofile << "" << endl;

  TCanvas *yRes_diff_Canv = new TCanvas("yRes_diff", "", 2500, 1000);
  yRes_diff_Canv->cd();
  //SIMC - DATA
  Double_t yRes_diff[4]; 
  yRes_diff[0] = yRes_arr[0] - data_yRes_arr[0];
  yRes_diff[1] = yRes_arr[1] - data_yRes_arr[1];
  yRes_diff[2] = yRes_arr[2] - data_yRes_arr[2];
  yRes_diff[3] = yRes_arr[3] - data_yRes_arr[3];
 
  Double_t yRes_diff_err[4]; 
  yRes_diff_err[0] = TMath::Sqrt(data_yRes_arr_err[0]*data_yRes_arr_err[0] + yRes_arr_err[0]*yRes_arr_err[0]);
  yRes_diff_err[1] = TMath::Sqrt(data_yRes_arr_err[1]*data_yRes_arr_err[1] + yRes_arr_err[1]*yRes_arr_err[1]);
  yRes_diff_err[2] = TMath::Sqrt(data_yRes_arr_err[2]*data_yRes_arr_err[2] + yRes_arr_err[2]*yRes_arr_err[2]);
  yRes_diff_err[3] = TMath::Sqrt(data_yRes_arr_err[3]*data_yRes_arr_err[3] + yRes_arr_err[3]*yRes_arr_err[3]);

  ofile << "#!Run[i,0]/    Pf_data[f,1]/     Pf_data_err[f,2]/     Pf_simc[f,3]/     Pf_simc_err[f,4]/" << endl;
  ofile << run[0] << "    " <<  data_yRes_arr[0] << "    " << data_yRes_arr_err[0] << "    " << yRes_arr[0] << "     " << yRes_arr_err[0] << endl;
  ofile << run[1] << "    " <<  data_yRes_arr[1] << "    " << data_yRes_arr_err[1] << "    " << yRes_arr[1] << "     " << yRes_arr_err[1] << endl;
  ofile << run[2] << "    " <<  data_yRes_arr[2] << "    " << data_yRes_arr_err[2] << "    " << yRes_arr[2] << "     " << yRes_arr_err[2] << endl;
  ofile << run[3] << "    " <<  data_yRes_arr[3] << "    " << data_yRes_arr_err[3] << "    " << yRes_arr[3] << "     " << yRes_arr_err[3] << endl;

  ofile << "" << endl;
  ofile << "#SIMC-DATA"<<endl;
  ofile << "#Run 3288: mean: " << yRes_diff[0] << " err: " << yRes_diff_err[0] << endl;
  ofile << "#Run 3371: mean: " << yRes_diff[1] << " err: " << yRes_diff_err[1] << endl;
  ofile << "#Run 3374: mean: " << yRes_diff[2] << " err: " << yRes_diff_err[2] << endl;
  ofile << "#Run 3377: mean: " << yRes_diff[3] << " err: " << yRes_diff_err[3] << endl;

  ofile << "" << endl;
  ofile << "#Momentum Correction Factor: " << endl;
  ofile << "#del = (simc - data)" << endl;
  ofile << "#corr_factor = 1 - (del/Pcentral)" << endl;
  ofile << "#3288: "<< 1. - (yRes_diff[0]/Pc[0]) << "   +/-  " <<  yRes_diff_err[0]/Pc[0] << "    " <<  endl;
  ofile << "#3371: "<< 1. - (yRes_diff[1]/Pc[1]) << "   +/-  " <<  yRes_diff_err[1]/Pc[1] << "    " <<  endl;
  ofile << "#3374: "<< 1. - (yRes_diff[2]/Pc[2]) << "   +/-  " <<  yRes_diff_err[2]/Pc[2] << "    " <<  endl;
  ofile << "#3377: "<< 1. - (yRes_diff[3]/Pc[3]) << "   +/-  " <<  yRes_diff_err[3]/Pc[3] << "    " <<  endl;

  ofile.close();

  TGraphErrors* gr_compare = new TGraphErrors(4,data_xRes_arr,yRes_diff, ex_data, yRes_diff_err);
  gr_compare->SetTitle(" ([SIMC - DATA] Momentum Difference vs. D2 H(e,e'p) Runs");

  gr_compare->GetYaxis()->SetTitle("[SIMC - DATA] [GeV]");
  gr_compare->GetYaxis()->CenterTitle();
  
  gr_compare->GetXaxis()->SetTitle("Kinematic Group");
  gr_compare->GetXaxis()->CenterTitle();

  gr_compare->SetMarkerStyle(21);
  gr_compare->SetMarkerColor(kRed);
  gr_compare->SetMarkerSize(1.8);
  gr_compare->Draw("AP");


  yRes_diff_Canv->SaveAs("SIMC_DATA_hmsP_Diff.pdf");


  
}
