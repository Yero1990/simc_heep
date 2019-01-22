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

  TObjArray data_HList[5]; 
  TObjArray simc_HList[5]; 


  //Define some constants
  Double_t Mp = 0.938272;  //proton mass
  Double_t Eb = 10.6005;
  //Double_t hP0[5] = {2.922111, 3.46017618, 2.2997660, 1.8827606, 1.8825118};  //central spec. momentum (no momentum Corr. after hYptar Offset)
  Double_t hP0[5] = {2.931170, 3.4709027, 2.3068953, 1.8885972, 1.8883477};    //central spec momentum (Momentum COrrection after hYptar Offset) 
 
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



  //Define Histograms
  TH1F *hist_hPcalc[5];
  TH1F *hist_hPmeas[5];
  TH1F *hist_hPDev[5];
  TH1F *hist_Em[5];

  TH2F *hist_hPDev_xfp[5];   //Momentum fraction correlations with HMS focal plane
  TH2F *hist_hPDev_xpfp[5];  
  TH2F *hist_hPDev_yfp[5]; 
  TH2F *hist_hPDev_ypfp[5]; 

  Double_t xRes_arr[5];                                                                                                               
  Double_t yRes_arr[5];                                                                                                
  Double_t yRes_arr_err[5];

  //Define Canvas to Draw
  TCanvas *c0_simc = new TCanvas("c0_simc", "MIssing Enrgy", 1500, 1000);   
  c0_simc->Divide(3,2);
  TCanvas *c1_simc = new TCanvas("c1_simc", "Calculated/Measured HMS Momentum", 1500, 1000);   
  c1_simc->Divide(3,2);
  TCanvas *c2_simc = new TCanvas("c2_simc", "HMS Momentum Fractional Dev. From Measured", 1500,1000);
  c2_simc->Divide(3,2);
  TCanvas *c3_simc = new TCanvas("c3_simc","Fractional Deviation from P_{meas} vs. Kin Group",200,10,500,300);
  
  TCanvas *c4_simc[5]; // = new TCanvas("c4_simc", "Fract. Momentum vs. HMS Focal Plane", 1500, 1000);          
  //c4_simc->Divide(2,2);                                                                                     
 


  Double_t FullWeight;

  int run_num[5] = {3288, 3371, 3374, 3376, 3377};

  int index = 0;

  //Loop over all kinematic groups
  for(int i = 0; i<5; i++)
    {

      simc_HList[i](0);


      //string filename = Form("../../../worksim_voli/D2_pCorr/D2_heep_%d.root", run_num[i]);                                   
      string filename = Form("../../../worksim_voli/D2_heep_%d.root", run_num[i]);                                   

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
      
      hist_hPcalc[index] = new TH1F(Form("hP_calc_Run%d", run_num[index]), "",  100, 1.5, 4);                    
      hist_hPmeas[index] = new TH1F(Form("hP_meas_Run%d", run_num[index]), "",  100, 1.5, 4);
      
      hist_hPDev[index] = new TH1F(Form("hP_Dev_Run%d", run_num[index]), "", 100, -0.05, 0.05);
      hist_Em[index] = new TH1F(Form("h_Em_Run%d", run_num[index]), "", 100, -0.2, 0.3);

      hist_hPDev_xfp[index] = new TH2F(Form("hPDiff_vs_xfp: Run %d", run_num[index]), "", 100, -40, 40, 80, -0.05, 0.05);
      hist_hPDev_xpfp[index] = new TH2F(Form("hPDiff_vs_xpfp: Run %d", run_num[index]), "", 100, -0.06, 0.06, 80, -0.05, 0.05); 
      hist_hPDev_yfp[index] = new TH2F(Form("hPDiff_vs_yfp: Run %d", run_num[index]), "", 100, -10, 30, 80, -0.05, 0.05);        
      hist_hPDev_ypfp[index] = new TH2F(Form("hPDiff_vs_ypfp: Run %d", run_num[index]), "", 100, -0.02, 0.04, 80, -0.05, 0.05);  
      
      hist_hPDev_xfp[index]->GetXaxis()->SetTitle("HMS X_{fp}");
      hist_hPDev_xfp[index]->GetYaxis()->SetTitle("HMS [P_{calc} - P_{meas}] / P_{meas}");   
     
      hist_hPDev_xpfp[index]->GetXaxis()->SetTitle("HMS X'_{fp}"); 
      hist_hPDev_xpfp[index]->GetYaxis()->SetTitle("HMS [P_{calc} - P_{meas}] / P_{meas}"); 

      hist_hPDev_yfp[index]->GetXaxis()->SetTitle("HMS Y_{fp}");                                                        
      hist_hPDev_yfp[index]->GetYaxis()->SetTitle("HMS [P_{calc} - P_{meas}] / P_{meas}");                              
                  
      hist_hPDev_ypfp[index]->GetXaxis()->SetTitle("HMS Y'_{fp}");                                                         
      hist_hPDev_ypfp[index]->GetYaxis()->SetTitle("HMS [P_{calc} - P_{meas}] / P_{meas}");          
	

      simc_HList[i].Add(hist_hPcalc[index]);
      simc_HList[i].Add(hist_hPmeas[index]);
      simc_HList[i].Add(hist_hPDev[index]);
      simc_HList[i].Add(hist_Em[index]);

      simc_HList[i].Add(hist_hPDev_xfp[index]);
      simc_HList[i].Add(hist_hPDev_xpfp[index]);
      simc_HList[i].Add(hist_hPDev_yfp[index]);
      simc_HList[i].Add(hist_hPDev_ypfp[index]);



      //-----------LOOP OVER ALL ENTRIES IN TREE-----------------------
      //Loop over all entries
      for(int i=0; i<T->GetEntries(); i++){
	
	T->GetEntry(i);
	
	//Calculate the cross-sect. weight
	FullWeight = (Normfac*Weight)/T->GetEntries();
	
	//Calculated proton Momentum (Using formula)
	hmsP_calc = 2.*Mp*Eb*(Eb + Mp)*TMath::Cos(theta_p) / (Mp*Mp + 2.*Mp*Eb + Eb*Eb*TMath::Power(TMath::Sin(theta_p),2));
	//Measured Momentum Obtained from delta
	hmsP_meas = h_delta*hP0[index] / 100. + hP0[index];


	//DEBUG
	//cout << "theta_p = " << theta_p << endl;
	//cout << "numerator = " <<  2.*Mp*Eb*(Eb + Mp)*TMath::Cos(theta_p) << endl;
	//cout << "denominator = " << Mp*Mp + 2.*Mp*Eb + Eb*Eb*TMath::Power(TMath::Sin(theta_p),2) << endl;
	//cout << "hmsP_calc - hmsP_meas = " << hmsP_calc << " - " << hmsP_meas << " = " << hmsP_calc - hmsP_meas  << endl;

	
	//Fill Histograms
	if (Em<0.05){
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


      c4_simc[index] = new TCanvas(Form("c4_simc, Run %d", run_num[index]), "Fract. Momentum vs. HMS Focal Plane", 1900, 1400);       
      c4_simc[index]->Divide(2,2);  
      c4_simc[index]->cd(1);
      hist_hPDev_xfp[index]->Draw("colz");
      c4_simc[index]->cd(2); 
      hist_hPDev_xpfp[index]->Draw("colz");
      c4_simc[index]->cd(3);
      hist_hPDev_yfp[index]->Draw("colz"); 
      c4_simc[index]->cd(4);
      hist_hPDev_ypfp[index]->Draw("colz");   

      c4_simc[index]->SaveAs(Form("simc_hPdiff_vs_HMS_FocalPlane_%d.pdf", run_num[index]));
      
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
      TFile *simc_outROOT = new TFile(Form("HMS_heepSIMC_histos_%d.root", run_num[index]), "recreate");
      simc_HList[i].Write();
      simc_outROOT->Close();

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
  c1_simc->SaveAs("simc_HMS_Calc_MeasP.pdf");
  c2_simc->SaveAs("simc_Momentum_Diff.pdf");
  c3_simc->SaveAs("simc_Pdiff_Graph.pdf");


  //---------------------DATA----------------------------
  

  

 //Define some variables to be determined inside the entry loop
  Double_t htheta_p;     //proton arm angle (event by event)
  //Double_t hmsP_calc;   //calculated proton momentum

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

  Double_t hreact_y;
  Double_t hxfp, hxpfp, hyfp, hypfp;


  //-------------CREATE EMPTY HISTOGRAMS---------------------------------------------
  
  
  TH1F *histData_hPcalc[5];
  TH1F *histData_hPmeas[5];
  TH1F *histData_hPDev[5];
  TH1F *histData_Em[5];
  
  //Momentum fraction correlations with HMS focal plane
  TH2F *histhPDev_xfp[5];   
  TH2F *histhPDev_xpfp[5];  
  TH2F *histhPDev_yfp[5]; 
  TH2F *histhPDev_ypfp[5]; 
 
  TH2F *histhPDev_reacty[5]; 

  Double_t data_xRes_arr[5];
  Double_t data_yRes_arr[5];
  Double_t data_yRes_arr_err[5];
  
  //--------------------------------------------------------

  TCanvas *c0 = new TCanvas("c0", "Missing Energy", 1500, 1000);   
  c0->Divide(4,2);
  TCanvas *c1 = new TCanvas("c1", "Calculated/Measured HMS Momentum", 1500, 1000);   
  c1->Divide(4,2);
  TCanvas *c2 = new TCanvas("c2", "HMS Momentum Fract. Dev. From Measured", 1500,1000);
  c2->Divide(4,2);
  TCanvas *c3 = new TCanvas("c3","Frac. Deviation from P_{meas} vs. Kin Group",200,10,500,300);
  
  TCanvas *c4[5];
  

  
  index = 0;
  int run[5] = {3288, 3371, 3374, 3376, 3377};
  //Loop over runs
    for(int irun=0; irun<5; irun++)
      {          

	data_HList[irun](0);

                                                                           
	//Open TFile
	//string filename = Form("../../../../hallc_replay/ROOTfiles/coin_replay_heep_check_%d_50000_poshYptarOffset.root",run[irun]);
	//string filename = Form("../../../../hallc_replay/ROOTfiles/coin_replay_heep_check_%d_50000_noYptarOffset.root",run[irun]);
	string filename = Form("../../../../hallc_replay/ROOTfiles/coin_replay_heep_check_%d_50000.root",run[irun]);

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

	T->SetBranchAddress("H.react.y", &hreact_y);

	//hist[index][0] = new TH1F(Form("calc_P: Run %d", run[irun]), Form("Run %d",run[irun]), 100, 1.5, 4); 
	histData_hPcalc[index] =  new TH1F(Form("data_Pcalc_Run%d", run[irun]), Form("Run %d",run[irun]), 100, 1.5, 4);

	//hist[index][1] = new TH1F(Form("meas_P: Run %d", run[irun]), Form("Run %d",run[irun]), 100, 1.5, 4);  
	histData_hPmeas[index] = new TH1F(Form("data_Pmeas_Run%d", run[irun]), Form("Run %d",run[irun]), 100, 1.5, 4);
 
	//hist[index][2] = new TH1F(Form("pDiff: Run %d",run[irun]), Form("Run %d: HMS (P_{calc} - P_{meas})/P_{meas}", run[irun]), 100, -0.05, 0.05);
	histData_hPDev[index] = new TH1F(Form("data_pDiff_Run%d",run[irun]), Form("Run %d: HMS (P_{calc} - P_{meas})/P_{meas}", run[irun]), 100, -0.05, 0.05);

	//hist[index][3] = new TH1F(Form("Emiss: Run %d",run[irun]), Form("Run %d: E_{miss}", run[irun]), 100, -0.2, 0.3);
	histData_Em[index] = new TH1F(Form("Emiss: Run %d",run[irun]), Form("Run %d: E_{miss}", run[irun]), 100, -0.2, 0.3);

	histhPDev_xfp[index] = new TH2F(Form("hPDiff_vs_xfp: Run %d", run_num[index]), "", 100, -40, 40, 80, -0.05, 0.05);
	histhPDev_xpfp[index] = new TH2F(Form("hPDiff_vs_xpfp: Run %d", run_num[index]), "", 100, -0.06, 0.06, 80, -0.05, 0.05); 
	histhPDev_yfp[index] = new TH2F(Form("hPDiff_vs_yfp: Run %d", run_num[index]), "", 100, -10, 30, 80, -0.05, 0.05);        
	histhPDev_ypfp[index] = new TH2F(Form("hPDiff_vs_ypfp: Run %d", run_num[index]), "", 100, -0.02, 0.04, 80, -0.05, 0.05);  


	histhPDev_reacty[index] = new TH2F(Form("hPDiff_vs_hreacty: Run %d", run_num[index]), "", 100, -.5, .5, 80, -0.05, 0.05);
	
	data_HList[irun].Add(histData_hPcalc[index]);
	data_HList[irun].Add(histData_hPmeas[index]);
       	data_HList[irun].Add(histData_hPDev[index]);
	data_HList[irun].Add(histData_Em[index]);

	data_HList[irun].Add(histhPDev_xfp[index]);
	data_HList[irun].Add(histhPDev_xpfp[index]);
	data_HList[irun].Add(histhPDev_yfp[index]);
	data_HList[irun].Add(histhPDev_ypfp[index]);

	data_HList[irun].Add(histhPDev_reacty[index]);


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

	T->GetEntry(i);

	//HMS Particle angles relative to central angle
	htheta_p = xangle*180./TMath::Pi() - ptheta_e;

	//HMS Particle calculated momentum (Using only the particle angle and beam energy)
	hmsP_calc = 2.*Mp*Eb*(Eb + Mp)*TMath::Cos(htheta_p*TMath::Pi()/180.) / (Mp*Mp + 2.*Mp*Eb + Eb*Eb*TMath::Power(TMath::Sin(htheta_p*TMath::Pi()/180.),2)) ;


	
	if(TMath::Abs(htheta_p)<100 && TMath::Abs(hmsP_calc)<100 && TMath::Abs(hms_Pmeas)<100. &&  emiss < 0.05 )
	  {

	    
	    //DEGUB
	    // cout << "KG: " << index + 1 << endl;
	    //cout << "hmsP_calc - hms_Pmeas = " <<  hmsP_calc << " - " << leaf_var[2] << " = " <<  hmsP_calc - leaf_var[2] << endl;

	    //Fill Histograms
	    histData_hPcalc[index]->Fill(hmsP_calc);  //calculated momentum from formula (function of htheta_p and Ebeam)
	    histData_hPmeas[index]->Fill(hms_Pmeas);  //measured momentum H.gtr.p
	    histData_hPDev[index]->Fill((hmsP_calc - hms_Pmeas) / hms_Pmeas);  //HMS (Calculated - Measured)/Measured Momentum (fractonal deviation from measured)

	    //cout << "hxfp = " << hxfp << endl;
	    //cout << "Pfrac = "<< (hmsP_calc - hms_Pmeas)/ hms_Pmeas << endl;

	    histhPDev_xfp[index]->Fill(hxfp, (hmsP_calc - hms_Pmeas)/ hms_Pmeas);
	    histhPDev_xpfp[index]->Fill(hxpfp, (hmsP_calc - hms_Pmeas)/ hms_Pmeas);  
	    histhPDev_yfp[index]->Fill(hyfp, (hmsP_calc - hms_Pmeas)/ hms_Pmeas);
	    histhPDev_ypfp[index]->Fill(hypfp, (hmsP_calc - hms_Pmeas)/ hms_Pmeas); 

	    histhPDev_reacty[index]->Fill(hreact_y, (hmsP_calc - hms_Pmeas)/ hms_Pmeas);

	  }
	histData_Em[index]->Fill(emiss);   //FIlling Missing Energy Histogram

      }
      
      //---------END ENTRY LOOP ---------------------------------------
      //DRaw HIstograms
      c0->cd(index+1);
      histData_Em[index]->Draw();
      

      c1->cd(index+1);
      histData_hPcalc[index]->Draw();
      histData_hPcalc[index] ->SetLineColor(kRed);
      histData_hPmeas[index]->Draw("sames");

      //Get max bin value/sigma from momentum difference histo to use as fit parameters
      int bin_max = histData_hPDev[index]->GetMaximumBin();
      Double_t xmax_val = histData_hPDev[index]->GetXaxis()->GetBinCenter(bin_max); 
      Double_t sig_Res = histData_hPDev[index]->GetStdDev();
       
      c2->cd(index+1);
      histData_hPDev[index]->Draw();

      c4[index] = new TCanvas(Form("c4, Run %d", run_num[index]), Form("Fract. Momentum vs. HMS Focal Plane, Run %d", run_num[index]), 1500, 1000);       
      c4[index]->Divide(2,2);  
      c4[index]->cd(1);
      histhPDev_xfp[index]->Draw("colz");
      c4[index]->cd(2); 
      histhPDev_xpfp[index]->Draw("colz");
      c4[index]->cd(3);
      histhPDev_yfp[index]->Draw("colz"); 
      c4[index]->cd(4);
      histhPDev_ypfp[index]->Draw("colz");   
      c4[index]->SaveAs(Form("data_hPdiff_vs_HMS_FocalPlane_%d.pdf", run_num[index]));




      TF1 *fit = new TF1("fit", "gaus", xmax_val - sig_Res, xmax_val + sig_Res);
      histData_hPDev[index]->Fit("fit", "R");
  
      
      Double_t mu_Res_fit = fit->GetParameter(1);
      Double_t sig_Res_fit = fit->GetParameter(2);
      Double_t mu_Res_err_fit =  fit->GetParError(1);

      data_xRes_arr[index] = index + 1;          //kin group
      data_yRes_arr[index] = mu_Res_fit;          //mean fit fractional deviation from meas. momentum
      data_yRes_arr_err[index] =  mu_Res_err_fit;    //error from mean fit

       //Write Histograms to a ROOTfile
      TFile *data_outROOT = new TFile(Form("HMS_heepDATA_histos_%d.root", run_num[index]), "recreate");
      data_HList[irun].Write();
      data_outROOT->Close();

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
  c1->SaveAs("data_HMS_Calc_MeasP.pdf");
  c2->SaveAs("data_Momentum_Diff.pdf");
  c3->SaveAs("data_Pdiff_Graph.pdf");



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
  yRes_diff_err[0] = TMath::Sqrt(data_yRes_arr_err[0]*data_yRes_arr_err[0] + yRes_arr_err[0]*yRes_arr_err[0]);
  yRes_diff_err[1] = TMath::Sqrt(data_yRes_arr_err[1]*data_yRes_arr_err[1] + yRes_arr_err[1]*yRes_arr_err[1]);
  yRes_diff_err[2] = TMath::Sqrt(data_yRes_arr_err[2]*data_yRes_arr_err[2] + yRes_arr_err[2]*yRes_arr_err[2]);
  yRes_diff_err[3] = TMath::Sqrt(data_yRes_arr_err[3]*data_yRes_arr_err[3] + yRes_arr_err[3]*yRes_arr_err[3]);
  yRes_diff_err[4] = TMath::Sqrt(data_yRes_arr_err[4]*data_yRes_arr_err[4] + yRes_arr_err[4]*yRes_arr_err[4]);

  cout << "**(DATA -  SIMC)***" << endl;
  cout << "Run 3288: " << yRes_diff[0] << endl;
  cout << "Run 3371: " << yRes_diff[1] << endl;
  cout << "Run 3374: " << yRes_diff[2] << endl;
  cout << "Run 3376: " << yRes_diff[3] << endl;
  cout << "Run 3377: " << yRes_diff[4] << endl;


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


  yRes_diff_Canv->SaveAs("DATA_SIMC_hmsP_Diff.pdf");
  
}
