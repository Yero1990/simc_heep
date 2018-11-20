//Code to check electron (calculated - measured) / measured momentum in the SHMS
//shmsP_calc(Ebeam, electron_angle)---> No delta dependence on the calculated momentum.
//shmsP_meas (delta)--->delta measured from the optics matrix elements

//**NOTE: Assuming the shms angle and beam energy are well known, any deviation of the
//calculated momentum in th electron arm, is due to te delta. In SIMC, this difference
//should be practically zero.  In data, however, the momentum fraction devitation can be 
//larger (>0.1 %), so a momentum correction factor should be applied to the data, which 
//will bring this difference closer to zero (<0.1%)

//The calculated electron energy is:  E_elec_calc = E_beam + Mp - E_prot,    E_prot = sqrt(P_prot^2+Mp^2)
//The calculated electron momentum is: P_elec_calc = Eb + Mp - sqrt(P_prot^2 + Mp^2) , where P_prot is event by event proton momentum, aince
//we have verified it to be OK,in the HMScheck directory. 

#include "TMath.h"
void calc_pElec_PDiff()
{

  gROOT->SetBatch(kTRUE);
  gStyle->SetOptFit();

  

  //Define some constants
  Double_t Mp = 0.938272;  //proton mass
  Double_t Eb = 10.6005;
  Double_t eP0[5] = {8.5640277, 8.5640277, 8.5640277, 8.5640277, 8.5640277};  //central spec. momentum
  Double_t hP0[5] = {2.92331, 3.4626, 2.30076, 1.88423029, 1.88423029};

  //Define some variables to be determined inside the entry loop
  Double_t shms_delta_calc;
  Double_t shmsP_calc;   //calculated SHMS momentum
  Double_t shmsP_meas;   //measured SHMS momentum 

  Double_t hmsP_meas;   //measured HMS momentum from HMS arm, which has been well determined from SIMC
 

  //Define TTree variables
  Float_t Normfac;
  Float_t h_delta;
  Float_t h_yptar;
  Float_t h_xptar;
  Float_t h_ytar;
  
  Float_t e_delta;
  Float_t e_yptar;
  Float_t e_xptar;
  Float_t e_ytar;

  //Focal Plane
  Float_t e_xfp;
  Float_t e_xpfp;
  Float_t e_yfp;
  Float_t e_ypfp;

  Float_t q;
  Float_t nu;
  Float_t Q2;
  Float_t W;
  Float_t Pm;
  Float_t Em;
  Float_t Weight;
  Float_t theta_e;
  Float_t theta_p;



  //Define Histograms
  TH1F *hist_ePcalc[5];
  TH1F *hist_ePmeas[5];
  TH1F *hist_ePDev[5];
  TH1F *hist_Em[5];

  TH2F *hist_ePDev_xfp[5];   //Momentum fraction correlations with SHMS focal plane
  TH2F *hist_ePDev_xpfp[5];  
  TH2F *hist_ePDev_yfp[5]; 
  TH2F *hist_ePDev_ypfp[5];
 
  //(shms_delta_calc - shms_delta_meas) vs. SHMS focal plane
  TH2F *hist_eDelta_xfp[5];
  TH2F *hist_eDelta_xpfp[5];
  TH2F *hist_eDelta_yfp[5];
  TH2F *hist_eDelta_ypfp[5];

  Double_t xRes_arr[5];                                                                                                               
  Double_t yRes_arr[5];                                                                                                
  Double_t yRes_arr_err[5];

  //Define Canvas to Draw
  TCanvas *c0_simc = new TCanvas("c0_simc", "MIssing Enrgy", 1500, 1000);   
  c0_simc->Divide(3,2);
  TCanvas *c1_simc = new TCanvas("c1_simc", "Calculated/Measured SHMS Momentum", 1500, 1000);   
  c1_simc->Divide(3,2);
  TCanvas *c2_simc = new TCanvas("c2_simc", "SHMS Momentum Fractional Dev. From Measured", 1500,1000);
  c2_simc->Divide(3,2);
  TCanvas *c3_simc = new TCanvas("c3_simc","Fractional Deviation from P_{meas} vs. Kin Group",200,10,500,300);
  //Canvas to plot fractional momentum vs. SHMS focal plane
  TCanvas *c4_simc[5];
  
  //Canvas to plot delta_calc - delta_meas vs. shms focal plane
  TCanvas *c5_simc[5];

  Double_t FullWeight;

  int run_num[5] = {3288, 3371, 3374, 3376, 3377};

  int index = 0;

  //Loop over all kinematic groups
  for(int i = 0; i<5; i++)
    {
      string filename = Form("../../../worksim_voli/D2_heep_%d.root", run_num[i]);                                   
      TFile *f1 = new TFile(filename.c_str());                                                                           

      //Get TTree                                                                                                                     
      TTree *T = (TTree*)f1->Get("SNT");   
      
      /*-------DEUT SIMC---------------------*/
      //Set Branch Address for some SIMC variables
      T->SetBranchAddress("Normfac", &Normfac);
      T->SetBranchAddress("h_delta", &h_delta);                                                     
      T->SetBranchAddress("e_delta", &e_delta);
      T->SetBranchAddress("e_yptar", &e_yptar);
      T->SetBranchAddress("e_xptar", &e_xptar);
      T->SetBranchAddress("e_ytar", &e_ytar); 
      T->SetBranchAddress("e_xfp", &e_xfp);                                 
      T->SetBranchAddress("e_xpfp", &e_xpfp);
      T->SetBranchAddress("e_yfp", &e_yfp);                        
      T->SetBranchAddress("e_ypfp", &e_ypfp);
      T->SetBranchAddress("q", &q);             
      T->SetBranchAddress("nu", &nu);           
      T->SetBranchAddress("Q2", &Q2);           
      T->SetBranchAddress("W", &W);             
      T->SetBranchAddress("Em", &Em);           
      T->SetBranchAddress("Pm", &Pm);           
      T->SetBranchAddress("Weight", &Weight);
      T->SetBranchAddress("theta_e", &theta_e);
      T->SetBranchAddress("theta_p", &theta_p);
      
      
      hist_ePcalc[index] = new TH1F(Form("eP_calc_Run%d", run_num[index]), "",  100, 7, 10);                   
      hist_ePmeas[index] = new TH1F(Form("eP_meas_Run%d", run_num[index]), "",  100, 7, 10);
      
      hist_ePDev[index] = new TH1F(Form("eP_Dev_Run%d", run_num[index]), "", 80, -0.01, 0.01);
      hist_Em[index] = new TH1F(Form("h_Em_Run%d", run_num[index]), "", 100, -0.2, 0.3);

      if(index==3 || index==4){
      hist_ePDev_xfp[index] = new TH2F(Form("ePDiff_vs_xfp: Run %d", run_num[index]), "", 100, 10, 25, 80, -0.01, 0.01);
      hist_ePDev_xpfp[index] = new TH2F(Form("ePDiff_vs_xpfp: Run %d", run_num[index]), "", 100, 0.01, 0.05, 80, -0.01, 0.01); 
      hist_ePDev_yfp[index] = new TH2F(Form("ePDiff_vs_yfp: Run %d", run_num[index]), "", 100, -10, 5, 80, -0.01, 0.01);        
      hist_ePDev_ypfp[index] = new TH2F(Form("ePDiff_vs_ypfp: Run %d", run_num[index]), "", 100, -0.015, 0.015, 80, -0.01, 0.01);

      //(shms_delta_calc - shms_delta_meas) vs. shms focal plane                                                                                        
      hist_eDelta_xfp[index] = new TH2F(Form("eDelta_vs_xfp: Run %d", run_num[index]), "", 100, 10., 25, 80, -1., 1.);                                   
      hist_eDelta_xpfp[index] = new TH2F(Form("eDelta_vs_xpfp: Run %d", run_num[index]), "", 100, 0.01, 0.05, 80, -1., 1.);                            
      hist_eDelta_yfp[index] = new TH2F(Form("eDelta_vs_yfp: Run %d", run_num[index]), "", 100, -10., 5, 80, -1., 1.);                                  
      hist_eDelta_ypfp[index] = new TH2F(Form("eDelta_vs_ypfp: Run %d", run_num[index]), "", 100, -0.015, 0.015, 80, -1., 1.); 
  
      }

      if(index==1){                                                                                                                                  
	hist_ePDev_xfp[index] = new TH2F(Form("ePDiff_vs_xfp: Run %d", run_num[index]), "", 100, -30, -5, 80, -0.01, 0.01);                      
	hist_ePDev_xpfp[index] = new TH2F(Form("ePDiff_vs_xpfp: Run %d", run_num[index]), "", 100, -0.07, 0.02, 80, -0.01, 0.01);           
	hist_ePDev_yfp[index] = new TH2F(Form("ePDiff_vs_yfp: Run %d", run_num[index]), "", 100, -25, 15, 80, -0.01, 0.01);                
	hist_ePDev_ypfp[index] = new TH2F(Form("ePDiff_vs_ypfp: Run %d", run_num[index]), "", 100, -0.04, 0.03, 80, -0.01, 0.01);                  
      
	//(shms_delta_calc - shms_delta_meas) vs. shms focal plane                                                                   
        hist_eDelta_xfp[index] = new TH2F(Form("eDelta_vs_xfp: Run %d", run_num[index]), "", 100, -30., -5, 80, -1., 1.);                   
        hist_eDelta_xpfp[index] = new TH2F(Form("eDelta_vs_xpfp: Run %d", run_num[index]), "", 100, -0.07, 0.02, 80, -1., 1.);   
        hist_eDelta_yfp[index] = new TH2F(Form("eDelta_vs_yfp: Run %d", run_num[index]), "", 100, -25., 15, 80, -1., 1.);            
        hist_eDelta_ypfp[index] = new TH2F(Form("eDelta_vs_ypfp: Run %d", run_num[index]), "", 100, -0.04, 0.03, 80, -1., 1.); 
      }

      if(index==2){                                                                                                                            
        hist_ePDev_xfp[index] = new TH2F(Form("ePDiff_vs_xfp: Run %d", run_num[index]), "", 100, 5, 15, 80, -0.01, 0.01);         
        hist_ePDev_xpfp[index] = new TH2F(Form("ePDiff_vs_xpfp: Run %d", run_num[index]), "", 100, 0., 0.04, 80, -0.01, 0.01);    
        hist_ePDev_yfp[index] = new TH2F(Form("ePDiff_vs_yfp: Run %d", run_num[index]), "", 100, -5, 5, 80, -0.01, 0.01);      
        hist_ePDev_ypfp[index] = new TH2F(Form("ePDiff_vs_ypfp: Run %d", run_num[index]), "", 100, -0.02, 0.02, 80, -0.01, 0.01);          
      
	//(shms_delta_calc - shms_delta_meas) vs. shms focal plane                                                                       
        hist_eDelta_xfp[index] = new TH2F(Form("eDelta_vs_xfp: Run %d", run_num[index]), "", 100, 5., 15, 80, -1., 1.);                          
        hist_eDelta_xpfp[index] = new TH2F(Form("eDelta_vs_xpfp: Run %d", run_num[index]), "", 100, 0., 0.04, 80, -1., 1.);            
        hist_eDelta_yfp[index] = new TH2F(Form("eDelta_vs_yfp: Run %d", run_num[index]), "", 100, -5., 5, 80, -1., 1.);                    
        hist_eDelta_ypfp[index] = new TH2F(Form("eDelta_vs_ypfp: Run %d", run_num[index]), "", 100, -0.02, 0.02, 80, -1., 1.); 
      }  

      if(index==0){                                                                                                             
        hist_ePDev_xfp[index] = new TH2F(Form("ePDiff_vs_xfp: Run %d", run_num[index]), "", 100, 10., 5, 80, -0.01, 0.01);      
        hist_ePDev_xpfp[index] = new TH2F(Form("ePDiff_vs_xpfp: Run %d", run_num[index]), "", 100, -0.04, 0.03, 80, -0.01, 0.01);          
        hist_ePDev_yfp[index] = new TH2F(Form("ePDiff_vs_yfp: Run %d", run_num[index]), "", 100, -10, 5, 80, -0.01, 0.01);         
        hist_ePDev_ypfp[index] = new TH2F(Form("ePDiff_vs_ypfp: Run %d", run_num[index]), "", 100, -0.02, 0.02, 80, -0.01, 0.01);            

	//(shms_delta_calc - shms_delta_meas) vs. shms focal plane
	hist_eDelta_xfp[index] = new TH2F(Form("eDelta_vs_xfp: Run %d", run_num[index]), "", 100, 10., 5, 80, -1., 1.);
	hist_eDelta_xpfp[index] = new TH2F(Form("eDelta_vs_xpfp: Run %d", run_num[index]), "", 100, -0.04, 0.03, 80, -1., 1.);
	hist_eDelta_yfp[index] = new TH2F(Form("eDelta_vs_yfp: Run %d", run_num[index]), "", 100, -10., 5, 80, -1., 1.);
	hist_eDelta_ypfp[index] = new TH2F(Form("eDelta_vs_ypfp: Run %d", run_num[index]), "", 100, -0.02, 0.02, 80, -1., 1.);	

      } 

      hist_ePDev_xfp[index]->GetXaxis()->SetTitle("SHMS X_{fp}");
      hist_ePDev_xfp[index]->GetYaxis()->SetTitle("SHMS [P_{calc} - P_{meas}] / P_{meas}");   
      
     
      hist_ePDev_xpfp[index]->GetXaxis()->SetTitle("SHMS X'_{fp}"); 
      hist_ePDev_xpfp[index]->GetYaxis()->SetTitle("SHMS [P_{calc} - P_{meas}] / P_{meas}"); 

      hist_ePDev_yfp[index]->GetXaxis()->SetTitle("SHMS Y_{fp}");                                                        
      hist_ePDev_yfp[index]->GetYaxis()->SetTitle("SHMS [P_{calc} - P_{meas}] / P_{meas}");                              
                  
      hist_ePDev_ypfp[index]->GetXaxis()->SetTitle("SHMS Y'_{fp}");                                                         
      hist_ePDev_ypfp[index]->GetYaxis()->SetTitle("SHMS [P_{calc} - P_{meas}] / P_{meas}");          

      //----set names for delta difference histograms
      hist_eDelta_xfp[index]->GetXaxis()->SetTitle("SHMS X_{fp}");
      hist_eDelta_xfp[index]->GetYaxis()->SetTitle("SHMS #delta_{calc} - #delta_{meas.} [%]");                                             

      hist_eDelta_xpfp[index]->GetXaxis()->SetTitle("SHMS X'_{fp}");                                                                        
      hist_eDelta_xpfp[index]->GetYaxis()->SetTitle("SHMS #delta_{calc} - #delta_{meas.} [%]");  

      hist_eDelta_yfp[index]->GetXaxis()->SetTitle("SHMS Y_{fp}");                                                       
      hist_eDelta_yfp[index]->GetYaxis()->SetTitle("SHMS #delta_{calc} - #delta_{meas.} [%]");                                                 
                                                                                                                                                   
      hist_eDelta_ypfp[index]->GetXaxis()->SetTitle("SHMS Y'_{fp}");                                                                         
      hist_eDelta_ypfp[index]->GetYaxis()->SetTitle("SHMS #delta_{calc} - #delta_{meas.} [%]");                                                 

      
      //-----------LOOP OVER ALL ENTRIES IN TREE-----------------------
      //Loop over all entries
      for(int i=0; i<T->GetEntries(); i++){
	
	T->GetEntry(i);
	
	//Calculate the cross-sect. weight
	FullWeight = (Normfac*Weight)/T->GetEntries();

	hmsP_meas = h_delta*hP0[index] / 100. + hP0[index]; // delta = (P - P0) / P0 * 100

	//Calculated electron Momentum (Using formula --depend on hms measured momentum, which is well known)
	shmsP_calc = Eb + Mp - TMath::Sqrt(hmsP_meas*hmsP_meas + Mp*Mp);
	//Measured Momentum Obtained from delta
	shmsP_meas = e_delta*eP0[index] / 100. + eP0[index];
	
	shms_delta_calc = (shmsP_calc - eP0[index]) / eP0[index] * 100.;  //shms calculated delta in %

	//DEBUG
	//cout << "theta_p = " << theta_p << endl;
	//cout << "numerator = " <<  2.*Mp*Eb*(Eb + Mp)*TMath::Cos(theta_p) << endl;
	//cout << "denominator = " << Mp*Mp + 2.*Mp*Eb + Eb*Eb*TMath::Power(TMath::Sin(theta_p),2) << endl;
	//cout << "shmsP_calc - shmsP_meas = " << shmsP_calc << " - " << shmsP_meas << " = " << shmsP_calc - shmsP_meas  << endl;

	
	//Fill Histograms
	if (Em<0.05){
	hist_ePcalc[index]->Fill(shmsP_calc, FullWeight);      //Fill calculated momentum
	hist_ePmeas[index]->Fill(shmsP_meas, FullWeight);      //Fill measure momentum
	hist_ePDev[index]->Fill( (shmsP_calc - shmsP_meas)/ shmsP_meas, FullWeight);   //Fill Fractional  Deviation of Calculated from Measured Momentum

	hist_ePDev_xfp[index]->Fill(e_xfp, (shmsP_calc - shmsP_meas)/ shmsP_meas, FullWeight);
        hist_ePDev_xpfp[index]->Fill(e_xpfp, (shmsP_calc - shmsP_meas)/ shmsP_meas, FullWeight);  
	hist_ePDev_yfp[index]->Fill(e_yfp, (shmsP_calc - shmsP_meas)/ shmsP_meas, FullWeight);
	hist_ePDev_ypfp[index]->Fill(e_ypfp, (shmsP_calc - shmsP_meas)/ shmsP_meas, FullWeight); 

	hist_eDelta_xfp[index]->Fill(e_xfp, (shms_delta_calc - e_delta), FullWeight);
	hist_eDelta_xpfp[index]->Fill(e_xpfp, (shms_delta_calc - e_delta), FullWeight);                                                       
	hist_eDelta_yfp[index]->Fill(e_yfp, (shms_delta_calc - e_delta), FullWeight);                                                         
        hist_eDelta_ypfp[index]->Fill(e_ypfp, (shms_delta_calc - e_delta), FullWeight);                                                                 

	}

	hist_Em[index]->Fill(Em, FullWeight);  //Fill Missing Energy
      

      } //end entry loop
      

      c0_simc->cd(index+1);
      hist_Em[index]->Draw("hist");

      c1_simc->cd(index+1);
      hist_ePcalc[index]->Draw("hist");
      hist_ePcalc[index]->SetLineColor(kRed);
      hist_ePmeas[index]->Draw("sameshist");


      c4_simc[index] = new TCanvas(Form("c4_simc, Run %d", run_num[index]), "Fract. Momentum vs. SHMS Focal Plane", 1500, 1000);       
      c4_simc[index]->Divide(2,2);  
      c4_simc[index]->cd(1);
      hist_ePDev_xfp[index]->Draw("colz");
      c4_simc[index]->cd(2); 
      hist_ePDev_xpfp[index]->Draw("colz");
      c4_simc[index]->cd(3);
      hist_ePDev_yfp[index]->Draw("colz"); 
      c4_simc[index]->cd(4);
      hist_ePDev_ypfp[index]->Draw("colz");   

      c4_simc[index]->SaveAs(Form("simc_ePdiff_vs_SHMS_FocalPlane_%d.pdf", run_num[index]));



      c5_simc[index] = new TCanvas(Form("c5_simc, Run %d", run_num[index]), "Delta Diff. vs. SHMS Focal Plane", 1500, 1000);                          
      c5_simc[index]->Divide(2,2);
      c5_simc[index]->cd(1);
      hist_eDelta_xfp[index]->Draw("colz");
      c5_simc[index]->cd(2);                                                                                                       
      hist_eDelta_xpfp[index]->Draw("colz"); 
      c5_simc[index]->cd(3);                                                                                
      hist_eDelta_yfp[index]->Draw("colz");                                                          
      c5_simc[index]->cd(4);                                                             
      hist_eDelta_ypfp[index]->Draw("colz");

      c5_simc[index]->SaveAs(Form("simc_DeltaDiff_vs_SHMS_FocalPlane_%d.pdf", run_num[index]));   

      //Get maximum bin value/sigma from momentum difference histo to use as fit parameters
      int bin_max = hist_ePDev[index]->GetMaximumBin(); 
      Double_t xmax_val = hist_ePDev[index]->GetXaxis()->GetBinCenter(bin_max);
      Double_t sig_Res = hist_ePDev[index]->GetStdDev();
      
      c2_simc->cd(index+1);
      hist_ePDev[index]->Draw();
      
      TF1 *fit = new TF1("fit", "gaus", xmax_val - 1.0*sig_Res, xmax_val + 1.0*sig_Res);
      hist_ePDev[index]->Fit("fit", "R");
      
      Double_t mu_Res_fit = fit->GetParameter(1);
      Double_t sig_Res_fit = fit->GetParameter(2);
      Double_t mu_Res_err_fit =  fit->GetParError(1); 

      cout << "mean_fit = " << mu_Res_fit << endl;

      xRes_arr[index] = index + 1;                                                                                                               
      yRes_arr[index] = mu_Res_fit;                                                                                                
      yRes_arr_err[index] = mu_Res_err_fit;
           
      cout << "yRes_arr = " << yRes_arr[index] << endl;
      
      index++;
      
    } // end filename loop


 

  Double_t ex[5] = {0.};
  TGraphErrors* gr = new TGraphErrors(5,xRes_arr,yRes_arr, ex, yRes_arr_err);
  
  c3_simc->cd();

  gr->SetTitle(" (P_calc - P_meas) / P_{meas.} vs. D2 Elastics");

  gr->GetYaxis()->SetTitle("(P_{calc} - P_{meas}) / P_{measured} [fractional deviation]");
  gr->GetYaxis()->CenterTitle();
  
  gr->GetXaxis()->SetTitle("Kinematic Group");
  gr->GetXaxis()->CenterTitle();

  gr->SetMarkerStyle(21);
  gr->SetMarkerColor(kBlack);
  gr->SetMarkerSize(0.5);
  gr->Draw("AP");
  
  c0_simc->SaveAs("simc_Missing_Energy.pdf");
  c1_simc->SaveAs("simc_SHMS_Calc_MeasP.pdf");
  c2_simc->SaveAs("simc_Momentum_Diff.pdf");
  c3_simc->SaveAs("simc_Pdiff_Graph.pdf");


  //---------------------DATA----------------------------
  
  
  
  
 //Define some variables to be determined inside the entry loop
  Double_t htheta_p;     //proton arm angle (event by event)
  //Double_t shmsP_calc;   //calculated proton momentum

  //Define TTree variables
  TString n_xangle = "H.kin.secondary.xangle";
  TString n_theta_e = "P.kin.primary.scat_ang_deg";   //e- scat angle [deg]
  TString n_shmsP_meas = "P.gtr.p";
  TString n_hmsP_meas = "H.gtr.p";                                                                                                  
  TString n_emiss = "H.kin.secondary.emiss";
  TString n_edelta = "P.gtr.dp";

  TString n_exfp = "P.dc.x_fp";
  TString n_expfp = "P.dc.xp_fp"; 
  TString n_eyfp = "P.dc.y_fp";               
  TString n_eypfp = "P.dc.yp_fp"; 

 
  Double_t xangle;
  Double_t ptheta_e;   //e- scat angle [rad]
  Double_t shms_Pmeas;
  Double_t hms_Pmeas;                                                                                                                               
  Double_t emiss;
  Double_t edelta;

  Double_t exfp, expfp, eyfp, eypfp;


  //-------------CREATE EMPTY HISTOGRAMS---------------------------------------------
  
  //Create empty histogram arrays with the appropiate names based on the leaf you want to Fill
  TH1F *hist[5][5];

  TH2F *histePDev_xfp[5];   //Momentum fraction correlations with SHMS focal plane
  TH2F *histePDev_xpfp[5];  
  TH2F *histePDev_yfp[5]; 
  TH2F *histePDev_ypfp[5]; 


  //(shms_delta_calc - shms_delta_meas) vs. SHMS focal plane                                                                         
  TH2F *histeDelta_xfp[5];                                                                            
  TH2F *histeDelta_xpfp[5];                                                                                    
  TH2F *histeDelta_yfp[5];                                                                                                       
  TH2F *histeDelta_ypfp[5];  

  Double_t data_xRes_arr[5];
  Double_t data_yRes_arr[5];
  Double_t data_yRes_arr_err[5];
  
  //--------------------------------------------------------

  TCanvas *c0 = new TCanvas("c0", "Missing Energy", 1500, 1000);   
  c0->Divide(4,2);
  TCanvas *c1 = new TCanvas("c1", "Calculated/Measured SHMS Momentum", 1500, 1000);   
  c1->Divide(4,2);
  TCanvas *c2 = new TCanvas("c2", "SHMS Momentum Fract. Dev. From Measured", 1500,1000);
  c2->Divide(4,2);
  TCanvas *c3 = new TCanvas("c3","Frac. Deviation from P_{meas} vs. Kin Group",200,10,500,300);
  
  TCanvas *c4[5];

  TCanvas *c5[5];

  
  index = 0;
  int run[5] = {3288, 3371, 3374, 3376, 3377};
  //Loop over runs
    for(int irun=0; irun<5; irun++)
      {                                                                                     
	//Open TFile
	string filename = Form("../../../../hallc_replay/ROOTfiles/coin_replay_heep_check_%d_-1.root",run[irun]);
	TFile *f1 = new TFile(filename.c_str());
	
	//Get TTree
	TTree *T = (TTree*)f1->Get("T");
	
	
	//Set Branch Address
	T->SetBranchAddress(n_xangle, &xangle);
	T->SetBranchAddress(n_theta_e, &ptheta_e);
	T->SetBranchAddress(n_shmsP_meas, &shms_Pmeas);
	T->SetBranchAddress(n_hmsP_meas, &hms_Pmeas);                                                
	T->SetBranchAddress(n_emiss, &emiss);
	T->SetBranchAddress(n_edelta, &edelta);
	
	T->SetBranchAddress(n_exfp, &exfp);
	T->SetBranchAddress(n_expfp, &expfp);
	T->SetBranchAddress(n_eyfp, &eyfp);
	T->SetBranchAddress(n_eypfp, &eypfp);

	hist[index][0] = new TH1F(Form("calc_P: Run %d", run[irun]), Form("Run %d",run[irun]), 100, 7, 10); 
	hist[index][1] = new TH1F(Form("meas_P: Run %d", run[irun]), Form("Run %d",run[irun]), 100, 7, 10);  
      
    
 
	hist[index][2] = new TH1F(Form("pDiff: Run %d",run[irun]), Form("Run %d: SHMS (P_{calc} - P_{meas})/P_{meas}", run[irun]), 100, -0.01, 0.01);
	hist[index][3] = new TH1F(Form("Emiss: Run %d",run[irun]), Form("Run %d: E_{miss}", run[irun]), 100, -0.2, 0.3);

  
	
	if(index==3 || index==4){                                                                                                         
	  histePDev_xfp[index] = new TH2F(Form("ePDiff_vs_xfp: Run %d", run_num[index]), "", 100, 10, 25, 80, -0.01, 0.01);                    
	  histePDev_xpfp[index] = new TH2F(Form("ePDiff_vs_xpfp: Run %d", run_num[index]), "", 100, 0.01, 0.05, 80, -0.01, 0.01);        
	  histePDev_yfp[index] = new TH2F(Form("ePDiff_vs_yfp: Run %d", run_num[index]), "", 100, -10, 5, 80, -0.01, 0.01);                   
	  histePDev_ypfp[index] = new TH2F(Form("ePDiff_vs_ypfp: Run %d", run_num[index]), "", 100, -0.015, 0.015, 80, -0.01, 0.01);               
	
	  //(shms_delta_calc - shms_delta_meas) vs. shms focal plane                                                                                        
	  histeDelta_xfp[index] = new TH2F(Form("eDelta_vs_xfp: Run %d", run_num[index]), "", 100, 10., 25, 80, -1., 1.);                                   
	  histeDelta_xpfp[index] = new TH2F(Form("eDelta_vs_xpfp: Run %d", run_num[index]), "", 100, 0.01, 0.05, 80, -1., 1.);                            
	  histeDelta_yfp[index] = new TH2F(Form("eDelta_vs_yfp: Run %d", run_num[index]), "", 100, -10., 5, 80, -1., 1.);                                  
	  histeDelta_ypfp[index] = new TH2F(Form("eDelta_vs_ypfp: Run %d", run_num[index]), "", 100, -0.015, 0.015, 80, -1., 1.);

	}                                                                                                                        
                                                                                                                                                     
	if(index==1){                                                                                                                                
	  histePDev_xfp[index] = new TH2F(Form("ePDiff_vs_xfp: Run %d", run_num[index]), "", 100, -30, -5, 80, -0.01, 0.01);          
	  histePDev_xpfp[index] = new TH2F(Form("ePDiff_vs_xpfp: Run %d", run_num[index]), "", 100, -0.07, 0.02, 80, -0.01, 0.01);                    
	  histePDev_yfp[index] = new TH2F(Form("ePDiff_vs_yfp: Run %d", run_num[index]), "", 100, -25, 15, 80, -0.01, 0.01);                          
	  histePDev_ypfp[index] = new TH2F(Form("ePDiff_vs_ypfp: Run %d", run_num[index]), "", 100, -0.04, 0.03, 80, -0.01, 0.01);                    

	  //(shms_delta_calc - shms_delta_meas) vs. shms focal plane                                                                   
	  histeDelta_xfp[index] = new TH2F(Form("eDelta_vs_xfp: Run %d", run_num[index]), "", 100, -30., -5, 80, -1., 1.);                   
	  histeDelta_xpfp[index] = new TH2F(Form("eDelta_vs_xpfp: Run %d", run_num[index]), "", 100, -0.07, 0.02, 80, -1., 1.);   
	  histeDelta_yfp[index] = new TH2F(Form("eDelta_vs_yfp: Run %d", run_num[index]), "", 100, -25., 15, 80, -1., 1.);            
	  histeDelta_ypfp[index] = new TH2F(Form("eDelta_vs_ypfp: Run %d", run_num[index]), "", 100, -0.04, 0.03, 80, -1., 1.); 

	}                                                                                                                                              
                                                                                                                                                     
	if(index==2){                                                                                                                                  
	  histePDev_xfp[index] = new TH2F(Form("ePDiff_vs_xfp: Run %d", run_num[index]), "", 100, 5, 15, 80, -0.01, 0.01);                            
	  histePDev_xpfp[index] = new TH2F(Form("ePDiff_vs_xpfp: Run %d", run_num[index]), "", 100, 0., 0.04, 80, -0.01, 0.01);                       
	  histePDev_yfp[index] = new TH2F(Form("ePDiff_vs_yfp: Run %d", run_num[index]), "", 100, -5, 5, 80, -0.01, 0.01);                            
	  histePDev_ypfp[index] = new TH2F(Form("ePDiff_vs_ypfp: Run %d", run_num[index]), "", 100, -0.02, 0.02, 80, -0.01, 0.01);                    
	
	  //(shms_delta_calc - shms_delta_meas) vs. shms focal plane                                                                       
	  histeDelta_xfp[index] = new TH2F(Form("eDelta_vs_xfp: Run %d", run_num[index]), "", 100, 5., 15, 80, -1., 1.);                          
	  histeDelta_xpfp[index] = new TH2F(Form("eDelta_vs_xpfp: Run %d", run_num[index]), "", 100, 0., 0.04, 80, -1., 1.);            
	  histeDelta_yfp[index] = new TH2F(Form("eDelta_vs_yfp: Run %d", run_num[index]), "", 100, -5., 5, 80, -1., 1.);                    
	  histeDelta_ypfp[index] = new TH2F(Form("eDelta_vs_ypfp: Run %d", run_num[index]), "", 100, -0.02, 0.02, 80, -1., 1.);

	}                                                                                                                                              
                                                                                                                                                     
	if(index==0){                                                                                                                                  
	  histePDev_xfp[index] = new TH2F(Form("ePDiff_vs_xfp: Run %d", run_num[index]), "", 100, -10., 5, 80, -0.01, 0.01);                           
	  histePDev_xpfp[index] = new TH2F(Form("ePDiff_vs_xpfp: Run %d", run_num[index]), "", 100, -0.04, 0.03, 80, -0.01, 0.01);                    
	  histePDev_yfp[index] = new TH2F(Form("ePDiff_vs_yfp: Run %d", run_num[index]), "", 100, -10, 5, 80, -0.01, 0.01);                           
	  histePDev_ypfp[index] = new TH2F(Form("ePDiff_vs_ypfp: Run %d", run_num[index]), "", 100, -0.02, 0.02, 80, -0.01, 0.01);                    
	
	  //(shms_delta_calc - shms_delta_meas) vs. shms focal plane
	  histeDelta_xfp[index] = new TH2F(Form("eDelta_vs_xfp: Run %d", run_num[index]), "", 100, -10., 5, 80, -1., 1.);
	  histeDelta_xpfp[index] = new TH2F(Form("eDelta_vs_xpfp: Run %d", run_num[index]), "", 100, -0.04, 0.03, 80, -1., 1.);
	  histeDelta_yfp[index] = new TH2F(Form("eDelta_vs_yfp: Run %d", run_num[index]), "", 100, -10., 5, 80, -1., 1.);
	  histeDelta_ypfp[index] = new TH2F(Form("eDelta_vs_ypfp: Run %d", run_num[index]), "", 100, -0.02, 0.02, 80, -1., 1.);
	}    
       
	histePDev_xfp[index]->GetXaxis()->SetTitle("SHMS X_{fp}");
	histePDev_xfp[index]->GetYaxis()->SetTitle("SHMS [P_{calc} - P_{meas}] / P_{meas}");   
	
	histePDev_xpfp[index]->GetXaxis()->SetTitle("SHMS X'_{fp}"); 
	histePDev_xpfp[index]->GetYaxis()->SetTitle("SHMS [P_{calc} - P_{meas}] / P_{meas}"); 
	
	histePDev_yfp[index]->GetXaxis()->SetTitle("SHMS Y_{fp}");                                                        
	histePDev_yfp[index]->GetYaxis()->SetTitle("SHMS [P_{calc} - P_{meas}] / P_{meas}");                              
                  
	histePDev_ypfp[index]->GetXaxis()->SetTitle("SHMS Y'_{fp}");                                                         
	histePDev_ypfp[index]->GetYaxis()->SetTitle("SHMS [P_{calc} - P_{meas}] / P_{meas}");  

	  //----set names for delta difference histograms
	histeDelta_xfp[index]->GetXaxis()->SetTitle("SHMS X_{fp}");
	histeDelta_xfp[index]->GetYaxis()->SetTitle("SHMS #delta_{calc} - #delta_{meas.} [%]");                                             
	
	histeDelta_xpfp[index]->GetXaxis()->SetTitle("SHMS X'_{fp}");                                                                        
	histeDelta_xpfp[index]->GetYaxis()->SetTitle("SHMS #delta_{calc} - #delta_{meas.} [%]");  
	
	histeDelta_yfp[index]->GetXaxis()->SetTitle("SHMS Y_{fp}");                                                       
	histeDelta_yfp[index]->GetYaxis()->SetTitle("SHMS #delta_{calc} - #delta_{meas.} [%]");                                                 
	
	histeDelta_ypfp[index]->GetXaxis()->SetTitle("SHMS Y'_{fp}");                                                                         
	histeDelta_ypfp[index]->GetYaxis()->SetTitle("SHMS #delta_{calc} - #delta_{meas.} [%]");  

      
      //Define new variables to be calculated in the loop
      
      
      //-----------LOOP OVER ALL ENTRIES IN TREE-----------------------
      //Loop over all entries
      for(int i=0; i<T->GetEntries(); i++){

	T->GetEntry(i);

	//SHMS Particle angles relative to central angle
	htheta_p = xangle*180./TMath::Pi() - ptheta_e;
                                                                                                                                            
        //Calculated electron Momentum (Using formula)                                                                                                 
        shmsP_calc = Eb + Mp - TMath::Sqrt(hms_Pmeas*hms_Pmeas + Mp*Mp); 
	
	//Calculated shms delta
	shms_delta_calc = (shmsP_calc - eP0[index]) / eP0[index] * 100.;
	
	if(TMath::Abs(shmsP_calc)<100 && TMath::Abs(hms_Pmeas)<100. &&  emiss < 0.05 )
	  {

	    
	    //DEGUB
	    // cout << "KG: " << index + 1 << endl;
	    //cout << "shmsP_calc - shms_Pmeas = " <<  shmsP_calc << " - " << leaf_var[2] << " = " <<  shmsP_calc - leaf_var[2] << endl;
	    //Fill Histograms
	    hist[index][0]->Fill(shmsP_calc);  //calculated momentum from formula (function of htheta_p and Ebeam)
	    hist[index][1]->Fill(shms_Pmeas);  //measured momentum H.gtr.p
	    hist[index][2]->Fill((shmsP_calc - shms_Pmeas) / shms_Pmeas);  //SHMS (Calculated - Measured)/Measured Momentum (fractonal deviation from measured)

	    //cout << "hxfp = " << hxfp << endl;
	    //cout << "Pfrac = "<< (shmsP_calc - shms_Pmeas)/ shms_Pmeas << endl;

	    histePDev_xfp[index]->Fill(exfp, (shmsP_calc - shms_Pmeas)/ shms_Pmeas);
	    histePDev_xpfp[index]->Fill(expfp, (shmsP_calc - shms_Pmeas)/ shms_Pmeas);  
	    histePDev_yfp[index]->Fill(eyfp, (shmsP_calc - shms_Pmeas)/ shms_Pmeas);
	    histePDev_ypfp[index]->Fill(eypfp, (shmsP_calc - shms_Pmeas)/ shms_Pmeas); 
	  
	    histeDelta_xfp[index]->Fill(exfp, (shms_delta_calc - edelta));
	    histeDelta_xpfp[index]->Fill(expfp, (shms_delta_calc - edelta));                                                       
	    histeDelta_yfp[index]->Fill(eyfp, (shms_delta_calc - edelta));                                                         
	    histeDelta_ypfp[index]->Fill(eypfp, (shms_delta_calc - edelta)); 

	  

	  }
	hist[index][3]->Fill(emiss);   //FIlling Missing Energy Histogram

      }
      
      //---------END ENTRY LOOP ---------------------------------------
      //DRaw HIstograms
      c0->cd(index+1);
      hist[index][3]->Draw();
      

      c1->cd(index+1);
      hist[index][0]->Draw();
      hist[index][0]->SetLineColor(kRed);
      hist[index][1]->Draw("sames");

      //Get max bin value/sigma from momentum difference histo to use as fit parameters
      int bin_max = hist[index][2]->GetMaximumBin();
      Double_t xmax_val = hist[index][2]->GetXaxis()->GetBinCenter(bin_max); 
      Double_t sig_Res = hist[index][2]->GetStdDev();
       
      c2->cd(index+1);
      hist[index][2]->Draw();

      c4[index] = new TCanvas(Form("c4, Run %d", run_num[index]), Form("Fract. Momentum vs. SHMS Focal Plane, Run %d", run_num[index]), 1500, 1000);       
      c4[index]->Divide(2,2);  
      c4[index]->cd(1);
      histePDev_xfp[index]->Draw("colz");
      histePDev_xfp[index]->GetYaxis()->SetRangeUser(-0.01, 0.01);
      c4[index]->cd(2); 
      histePDev_xpfp[index]->Draw("colz");
      c4[index]->cd(3);
      histePDev_yfp[index]->Draw("colz"); 
      c4[index]->cd(4);
      histePDev_ypfp[index]->Draw("colz");   
      c4[index]->SaveAs(Form("data_2D_ePdiff_vs_SHMS_FocalPlane_%d.pdf", run_num[index]));


      c5[index] = new TCanvas(Form("c5, Run %d", run_num[index]), "Delta Diff. vs. SHMS Focal Plane", 1500, 1000);                          
      c5[index]->Divide(2,2);
      c5[index]->cd(1);
      histeDelta_xfp[index]->Draw("colz");
      c5[index]->cd(2);                                                                                                       
      histeDelta_xpfp[index]->Draw("colz"); 
      c5[index]->cd(3);                                                                                
      histeDelta_yfp[index]->Draw("colz");                                                          
      c5[index]->cd(4);                                                             
      histeDelta_ypfp[index]->Draw("colz");

      c5[index]->SaveAs(Form("data_DeltaDiff_vs_SHMS_FocalPlane_%d.pdf", run_num[index]));   


      TF1 *fit = new TF1("fit", "gaus", xmax_val - sig_Res, xmax_val + sig_Res);
      hist[index][2]->Fit("fit", "R");
  
      
      Double_t mu_Res_fit = fit->GetParameter(1);
      Double_t sig_Res_fit = fit->GetParameter(2);
      Double_t mu_Res_err_fit =  fit->GetParError(1);

      data_xRes_arr[index] = index + 1;          //kin group
      data_yRes_arr[index] = mu_Res_fit;          //mean fit fractional deviation from meas. momentum
      data_yRes_arr_err[index] =  mu_Res_err_fit;    //error from mean fit

     
      index++;

    } //end loop over runs 
  
  Double_t ex_data[5] = {0.};
  
  TGraphErrors* data_gr = new TGraphErrors(5,data_xRes_arr,data_yRes_arr, ex_data, data_yRes_arr_err);
  
  
  c3->cd();

  data_gr->SetTitle(" (P_{calc} - P_{meas}) / P_{meas.} vs. Kin. Group");

  data_gr->GetYaxis()->SetTitle("(P_{calc} - P_{meas}) / P_{measured} [fractional deviation]");
  data_gr->GetYaxis()->CenterTitle();
  
  data_gr->GetXaxis()->SetTitle("Kinematic Group");
  data_gr->GetXaxis()->CenterTitle();

  data_gr->SetMarkerStyle(21);
  data_gr->SetMarkerColor(kBlack);
  data_gr->SetMarkerSize(0.5);
  data_gr->Draw("AP");
  
  c0->SaveAs("data_Missing_Energy.pdf");
  c1->SaveAs("data_SHMS_Calc_MeasP.pdf");
  c2->SaveAs("data_Momentum_Diff.pdf");
  c3->SaveAs("data_Residual_Graph.pdf");

 


  //----------------------PLOT THE DIFFERENCE BETWEEN DATA AND SIMC Momentum Dev.-----

  TCanvas *yRes_diff_Canv = new TCanvas("yRes_diff", "", 500, 300);
  yRes_diff_Canv->cd();
  //DATA - SIMC
  Double_t yRes_diff[5]; 
  yRes_diff[0] = data_yRes_arr[0] - yRes_arr[0];
  yRes_diff[1] = data_yRes_arr[1] - yRes_arr[1];
  yRes_diff[2] = data_yRes_arr[2] - yRes_arr[2];
  yRes_diff[3] = data_yRes_arr[3] - yRes_arr[3];
  yRes_diff[4] = data_yRes_arr[4] - yRes_arr[4];
 
  Double_t yRes_diff_err[5]; 
  yRes_diff_err[0] = TMath::Sqrt(data_yRes_arr[0]*data_yRes_arr[0] + yRes_arr[0]*yRes_arr[0]);
  yRes_diff_err[1] = TMath::Sqrt(data_yRes_arr[1]*data_yRes_arr[1] + yRes_arr[1]*yRes_arr[1]);
  yRes_diff_err[2] = TMath::Sqrt(data_yRes_arr[2]*data_yRes_arr[2] + yRes_arr[2]*yRes_arr[2]);
  yRes_diff_err[3] = TMath::Sqrt(data_yRes_arr[3]*data_yRes_arr[3] + yRes_arr[3]*yRes_arr[3]);
  yRes_diff_err[4] = TMath::Sqrt(data_yRes_arr[4]*data_yRes_arr[4] + yRes_arr[4]*yRes_arr[4]);


  TGraphErrors* gr_compare = new TGraphErrors(5,data_xRes_arr,yRes_diff, ex_data, yRes_diff_err);
  gr_compare->SetTitle(" ([DATA - SIMC] Fractional Momentum Deviation vs. D2 H(e,e'p) Runs");

  gr_compare->GetYaxis()->SetTitle("[DATA - SIMC] [fractional deviation]");
  gr_compare->GetYaxis()->CenterTitle();
  
  gr_compare->GetXaxis()->SetTitle("Kinematic Group");
  gr_compare->GetXaxis()->CenterTitle();

  gr_compare->SetMarkerStyle(21);
  gr_compare->SetMarkerColor(kRed);
  gr_compare->SetMarkerSize(0.8);
  gr_compare->Draw("AP");


  yRes_diff_Canv->SaveAs("DATA_SIMC_shmsP_Diff.pdf");
  
}
