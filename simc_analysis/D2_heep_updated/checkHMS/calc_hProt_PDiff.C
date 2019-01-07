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


  //Define some constants
  Double_t Mp = 0.938272;  //proton mass
  Double_t Eb = 10.6005;
  //  Double_t hP0[4] = {2.931169, 3.470903, 2.306895, 1.888348};    //Staring HMS Central Momentum
    Double_t hP0[4] = {2.9444540, 3.4772470, 2.3160550, 1.8951340};        //Corrected HMS Central Momentum
  // Double_t hP0[4] = {2.9379372, 3.4772470, 2.3160550, 1.8951340}; 
  
  //Define some variables to be determined inside the entry loop
  Double_t hmsP_calc;   //calculated HMS momentum
  Double_t hmsP_meas;   //measured HMS momentum 

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
  Float_t h_xfp;
  Float_t h_xpfp;
  Float_t h_yfp;
  Float_t h_ypfp;

  Float_t q;
  Float_t nu;
  Float_t Q2;
  Float_t W;
  Float_t Pm;
  Float_t Em;
  Float_t Weight;
  Float_t theta_e;
  Float_t theta_p;
  
  Float_t hPf;
  Float_t ePf;


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
  TCanvas *c2_simc = new TCanvas("c2_simc", "HMS Momentum Fractional Dev. From Measured", 2000,1000);
  c2_simc->Divide(2,2);
  TCanvas *c3_simc = new TCanvas("c3_simc","Fractional Deviation from P_{meas} vs. Kinematics Group", 2500,1000);
  
  TCanvas *c4_simc[4];                                                                            
 
  Double_t FullWeight;

  int run[4] = {3288, 3371, 3374, 3377};



  //Loop over all kinematic groups
  for(int index = 0; index<4; index++)
    {

      simc_HList[index](0);


      string filename = Form("../../../worksim_voli/D2_heep_%d.root", run[index]);                                   

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

      T->SetBranchAddress("h_pf", &hPf);   
      T->SetBranchAddress("e_pf", &ePf); 
      
      hist_hPcalc[index] = new TH1F(Form("hP_calc_Run%d", run[index]), Form("Run %d: HMS Calc./Meas. Momentum", run[index]),  100, 1.5, 4);                    
      hist_hPmeas[index] = new TH1F(Form("hP_meas_Run%d", run[index]), "",  100, 1.5, 4);
      
      hist_hPDev[index] = new TH1F(Form("hP_Dev_Run%d", run[index]), Form("Run %d: HMS Momentum Fraction", run[index]), 100, -0.05, 0.05);
      hist_Em[index] = new TH1F(Form("h_Em_Run%d", run[index]), Form("Run %d: Missing Energy", run[index]), 100, -0.2, 0.3);

      hist_hPDev_xfp[index] = new TH2F(Form("hPDiff_vs_xfp: Run %d", run[index]), "", 100, -40, 40, 80, -0.05, 0.05);
      hist_hPDev_xpfp[index] = new TH2F(Form("hPDiff_vs_xpfp: Run %d", run[index]), "", 100, -0.06, 0.06, 80, -0.05, 0.05); 
      hist_hPDev_yfp[index] = new TH2F(Form("hPDiff_vs_yfp: Run %d", run[index]), "", 100, -10, 30, 80, -0.05, 0.05);        
      hist_hPDev_ypfp[index] = new TH2F(Form("hPDiff_vs_ypfp: Run %d", run[index]), "", 100, -0.02, 0.04, 80, -0.05, 0.05);  

      hist_hPcalc[index]->GetXaxis()->SetTitle("HMS Momentum [GeV/c]");
      hist_hPDev[index]->GetXaxis()->SetTitle("HMS [P_{calc} - P_{meas}] / P_{meas}");

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
	
	//Calculate the cross-sect. weight
	FullWeight = (Normfac*Weight)/T->GetEntries();
	
	//Calculated proton Momentum (Using formula)
	hmsP_calc = 2.*Mp*Eb*(Eb + Mp)*TMath::Cos(theta_p) / (Mp*Mp + 2.*Mp*Eb + Eb*Eb*TMath::Power(TMath::Sin(theta_p),2));
	//Measured Momentum Obtained from delta
	hmsP_meas = hPf / 1000.;//h_delta*hP0[index] / 100. + hP0[index];
	//cout << "hPf = " << hPf << endl;

	//Fill Histograms
	if (Em<0.04){
	hist_hPcalc[index]->Fill(hmsP_calc, FullWeight);      //Fill calculated momentum
	hist_hPmeas[index]->Fill(hmsP_meas, FullWeight);      //Fill measure momentum
	hist_hPDev[index]->Fill( (hmsP_calc - hmsP_meas)/ hmsP_meas, FullWeight);   //Fill Fractional  Deviation of Calculated from Measured Momentum

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
      int bin_max = hist_hPDev[index]->GetMaximumBin(); 
      Double_t xmax_val = hist_hPDev[index]->GetXaxis()->GetBinCenter(bin_max);
      Double_t sig_Res = hist_hPDev[index]->GetStdDev();
      
      c2_simc->cd(index+1);
      hist_hPDev[index]->Draw();
      
      TF1 *fit = new TF1("fit", "gaus", xmax_val - 2.0*sig_Res, xmax_val + 2.0*sig_Res);
      hist_hPDev[index]->Fit("fit", "R");
      
      Double_t mu_Res_fit = fit->GetParameter(1);
      Double_t sig_Res_fit = fit->GetParameter(2);
      Double_t mu_Res_err_fit =  fit->GetParError(1); 

      cout << "mean_fit = " << mu_Res_fit << endl;

      xRes_arr[index] = index + 1;                                                                                                               
      yRes_arr[index] = mu_Res_fit;                                                                                                
      yRes_arr_err[index] = mu_Res_err_fit;
           
      cout << "yRes_arr = " << yRes_arr[index] << endl;

      //Write Histograms to a ROOTfile
      TFile *simc_outROOT = new TFile(Form("HMS_heepSIMC_histos_%d.root", run[index]), "recreate");
      simc_HList[index].Write();
      simc_outROOT->Close();

      
    } // end filename loop


 

  Double_t ex[4] = {0.};
  TGraphErrors* gr = new TGraphErrors(4,xRes_arr,yRes_arr, ex, yRes_arr_err);
  
  c3_simc->cd();

  gr->SetTitle(" (P_calc - P_meas) / P_{meas.} vs. D2 Elastics");

  gr->GetYaxis()->SetTitle("(P_{calc} - P_{meas}) / P_{measured} [fractional deviation]");
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

  TString n_hxfp = "H.dc.x_fp";
  TString n_hxpfp = "H.dc.xp_fp"; 
  TString n_hyfp = "H.dc.y_fp";               
  TString n_hypfp = "H.dc.yp_fp"; 

 
  Double_t xangle;
  Double_t ptheta_e;   //e- scat angle [rad]
  Double_t hms_Pmeas;
  Double_t emiss;
  Double_t hdelta;

  Double_t hxfp, hxpfp, hyfp, hypfp;


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
  TCanvas *c2 = new TCanvas("c2", "HMS Momentum Fract. Dev. From Measured", 2000,1000);
  c2->Divide(2,2);
  TCanvas *c3 = new TCanvas("c3","Frac. Deviation from P_{meas} vs. Kinematic Group", 2500,1000);
  
  TCanvas *c4[4];
  

  //Loop over runs
    for(int index=0; index<4; index++)
      {          

	data_HList[index](0);

                                                                           
	//Open TFile
	string filename = Form("../../../../hallc_replay/ROOTfiles/coin_replay_heep_check_%d_-1.root",run[index]);

	TFile *f1 = new TFile(filename.c_str());
	
	//Get TTree
	TTree *T = (TTree*)f1->Get("T");
	
	
	//Set Branch Address
	T->SetBranchAddress(n_xangle, &xangle);
	T->SetBranchAddress(n_theta_e, &ptheta_e);
	T->SetBranchAddress(n_hmsP_meas, &hms_Pmeas);
	T->SetBranchAddress(n_emiss, &emiss);
	T->SetBranchAddress(n_hdelta, &hdelta);
	
	T->SetBranchAddress(n_hxfp, &hxfp);
	T->SetBranchAddress(n_hxpfp, &hxpfp);
	T->SetBranchAddress(n_hyfp, &hyfp);
	T->SetBranchAddress(n_hypfp, &hypfp);


	histData_hPcalc[index] =  new TH1F(Form("data_Pcalc_Run%d", run[index]), Form("Run %d: HMS Calc./Meas. Momentum",run[index]), 100, 1.5, 4);
	histData_hPmeas[index] = new TH1F(Form("data_Pmeas_Run%d", run[index]), "", 100, 1.5, 4);

	histData_hPDev[index] = new TH1F(Form("data_pDiff_Run%d",run[index]), Form("Run %d: HMS Momentum Fraction", run[index]), 100, -0.05, 0.05);
	histData_Em[index] = new TH1F(Form("Emiss: Run %d",run[index]), Form("Run %d: Missing Energy", run[index]), 100, -0.2, 0.3);

	histhPDev_xfp[index] = new TH2F(Form("hPDiff_vs_xfp: Run %d", run[index]), "", 100, -40, 40, 80, -0.05, 0.05);
	histhPDev_xpfp[index] = new TH2F(Form("hPDiff_vs_xpfp: Run %d", run[index]), "", 100, -0.06, 0.06, 80, -0.05, 0.05); 
	histhPDev_yfp[index] = new TH2F(Form("hPDiff_vs_yfp: Run %d", run[index]), "", 100, -10, 30, 80, -0.05, 0.05);        
	histhPDev_ypfp[index] = new TH2F(Form("hPDiff_vs_ypfp: Run %d", run[index]), "", 100, -0.02, 0.04, 80, -0.05, 0.05);  

	histData_hPcalc[index]->GetXaxis()->SetTitle("HMS Momentum [GeV/c]");                                                                                                             
	histData_hPDev[index]->GetXaxis()->SetTitle("HMS [P_{calc} - P_{meas}] / P_{meas}");                                                                       
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
      
      
      //Define new variables to be calculated in the loop
      
      
      //-----------LOOP OVER ALL ENTRIES IN TREE-----------------------
      //Loop over all entries
	for(int i=0; i<T->GetEntries(); i++){
	  //for(int i=0; i<1000; i++){

	T->GetEntry(i);

	//HMS Particle angles relative to central angle
	htheta_p = xangle*180./TMath::Pi() - ptheta_e;

	//HMS Particle calculated momentum (Using only the particle angle and beam energy)
	hmsP_calc = 2.*Mp*Eb*(Eb + Mp)*TMath::Cos(htheta_p*TMath::Pi()/180.) / (Mp*Mp + 2.*Mp*Eb + Eb*Eb*TMath::Power(TMath::Sin(htheta_p*TMath::Pi()/180.),2)) ;


	
	if(TMath::Abs(htheta_p)<100 && TMath::Abs(hmsP_calc)<100 && TMath::Abs(hms_Pmeas)<100. &&  emiss < 0.06 )
	  {

	    //Fill Histograms
	    histData_hPcalc[index]->Fill(hmsP_calc);  //calculated momentum from formula (function of htheta_p and Ebeam)
	    histData_hPmeas[index]->Fill(hms_Pmeas);  //measured momentum H.gtr.p
	    histData_hPDev[index]->Fill((hmsP_calc - hms_Pmeas) / hms_Pmeas);  //HMS (Calculated - Measured)/Measured Momentum (fractonal deviation from measured)


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




      TF1 *fit = new TF1("fit", "gaus", xmax_val - sig_Res, xmax_val + sig_Res);
      histData_hPDev[index]->Fit("fit", "R");
  
      
      Double_t mu_Res_fit = fit->GetParameter(1);
      Double_t sig_Res_fit = fit->GetParameter(2);
      Double_t mu_Res_err_fit =  fit->GetParError(1);

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

  data_gr->SetTitle(" (P_{calc} - P_{meas}) / P_{meas.} vs. Kin. Group");

  data_gr->GetYaxis()->SetTitle("(P_{calc} - P_{meas}) / P_{measured} [fractional deviation]");
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

  ofile << "This file contains the momentum fraction difference between DATA/SIMC " << endl;
  ofile << "The momentum fraction is defined as: dP = (Pcalc - Pmeas) / Pmeas" << endl;
  ofile << "The mean of this distribution is fitted for data and simc, and the difference is taken " << endl;
  ofile << "" << endl;

  TCanvas *yRes_diff_Canv = new TCanvas("yRes_diff", "", 2500, 1000);
  yRes_diff_Canv->cd();
  //DATA - SIMC
  Double_t yRes_diff[5]; 
  yRes_diff[0] = data_yRes_arr[0] - yRes_arr[0];
  yRes_diff[1] = data_yRes_arr[1] - yRes_arr[1];
  yRes_diff[2] = data_yRes_arr[2] - yRes_arr[2];
  yRes_diff[3] = data_yRes_arr[3] - yRes_arr[3];
 
  Double_t yRes_diff_err[5]; 
  yRes_diff_err[0] = TMath::Sqrt(data_yRes_arr_err[0]*data_yRes_arr_err[0] + yRes_arr_err[0]*yRes_arr_err[0]);
  yRes_diff_err[1] = TMath::Sqrt(data_yRes_arr_err[1]*data_yRes_arr_err[1] + yRes_arr_err[1]*yRes_arr_err[1]);
  yRes_diff_err[2] = TMath::Sqrt(data_yRes_arr_err[2]*data_yRes_arr_err[2] + yRes_arr_err[2]*yRes_arr_err[2]);
  yRes_diff_err[3] = TMath::Sqrt(data_yRes_arr_err[3]*data_yRes_arr_err[3] + yRes_arr_err[3]*yRes_arr_err[3]);

  ofile << "**(DATA - SIMC)***" << endl;
  ofile << "Run 3288: mean: " << yRes_diff[0] << " err: " << yRes_diff_err[0] << endl;
  ofile << "Run 3371: mean: " << yRes_diff[1] << " err: " << yRes_diff_err[1] << endl;
  ofile << "Run 3374: mean: " << yRes_diff[2] << " err: " << yRes_diff_err[2] << endl;
  ofile << "Run 3377: mean: " << yRes_diff[3] << " err: " << yRes_diff_err[3] << endl;

  ofile.close();

  TGraphErrors* gr_compare = new TGraphErrors(4,data_xRes_arr,yRes_diff, ex_data, yRes_diff_err);
  gr_compare->SetTitle(" ([DATA - SIMC] Fractional Momentum Deviation vs. D2 H(e,e'p) Runs");

  gr_compare->GetYaxis()->SetTitle("[DATA - SIMC] [fractional deviation]");
  gr_compare->GetYaxis()->CenterTitle();
  
  gr_compare->GetXaxis()->SetTitle("Kinematic Group");
  gr_compare->GetXaxis()->CenterTitle();

  gr_compare->SetMarkerStyle(21);
  gr_compare->SetMarkerColor(kRed);
  gr_compare->SetMarkerSize(1.8);
  gr_compare->Draw("AP");


  yRes_diff_Canv->SaveAs("DATA_SIMC_hmsP_Diff.pdf");


  
}
