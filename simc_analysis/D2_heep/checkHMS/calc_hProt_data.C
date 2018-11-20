//Generic code to read and loop through N leafs from a given TTree
//The leafs names are read from a leaf_list.txt file.  For now, they can only be a Double_t (not int or array are yet done)
#include "TMath.h"
void calc_hProt_data()
{

  gROOT->SetBatch(kTRUE);
  gStyle->SetOptFit();


  //Define some constants
  Double_t Mp = 0.938272;  //proton mass
  Double_t Eb = 10.6005;

  //Define some variables to be determined inside the entry loop
  Double_t theta_p;     //proton arm angle (event by event)
  Double_t hmsP_calc;   //calculated proton momentum

  //Define TTree variables
  TString n_xangle = "H.kin.secondary.xangle";
  TString n_theta_e = "P.kin.primary.scat_ang_deg";   //e- scat angle [deg]
  TString n_hmsP_meas = "H.gtr.p";
  TString n_emiss = "H.kin.secondary.emiss";
  TString n_hdelta = "H.gtr.dp";
 
  Double_t xangle;
  Double_t theta_e;   //e- scat angle [rad]
  Double_t hmsP_meas;
  Double_t emiss;
  Double_t hdelta;



  //-------------CREATE EMPTY HISTOGRAMS---------------------------------------------
  
  //Create empty histogram arrays with the appropiate names based on the leaf you want to Fill
  TH1F *hist[5][5];
 
  Double_t xRes_arr[5];
  Double_t yRes_arr[5];
  Double_t yRes_arr_err[5];
  
  //--------------------------------------------------------

  TCanvas *c0 = new TCanvas("c0", "Missing Energy", 1500, 1000);   
  c0->Divide(4,2);
  TCanvas *c1 = new TCanvas("c1", "Calculated/Measured HMS Momentum", 1500, 1000);   
  c1->Divide(4,2);
  TCanvas *c2 = new TCanvas("c2", "HMS Momentum Fract. Dev. From Measured", 1500,1000);
  c2->Divide(4,2);
  TCanvas *c3 = new TCanvas("c3","Frac. Deviation from P_{meas} vs. Kin Group",200,10,500,300);

  

  
  int index = 0;
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
	T->SetBranchAddress(n_theta_e, &theta_e);
	T->SetBranchAddress(n_hmsP_meas, &hmsP_meas);
	T->SetBranchAddress(n_emiss, &emiss);
	T->SetBranchAddress(n_hdelta, &hdelta);


	hist[index][0] = new TH1F(Form("calc_P: Run %d", run[irun]), Form("Run %d",run[irun]), 100, 1.5, 4); 
	hist[index][1] = new TH1F(Form("meas_P: Run %d", run[irun]), Form("Run %d",run[irun]), 100, 1.5, 4);  
      
    
 
	hist[index][2] = new TH1F(Form("pDiff: Run %d",run[irun]), Form("Run %d: HMS (P_{calc} - P_{meas})/P_{meas}", run[irun]), 100, -0.05, 0.05);
	hist[index][3] = new TH1F(Form("Emiss: Run %d",run[irun]), Form("Run %d: E_{miss}", run[irun]), 100, -0.2, 0.3);


   
      
      
      //Define new variables to be calculated in the loop
      
      
      //-----------LOOP OVER ALL ENTRIES IN TREE-----------------------
      //Loop over all entries
      for(int i=0; i<T->GetEntries(); i++){

	T->GetEntry(i);

	//HMS Particle angles relative to central angle
	theta_p = xangle*180./TMath::Pi() - theta_e;

	//HMS Particle calculated momentum (Using only the particle angle and beam energy)
	hmsP_calc = 2.*Mp*Eb*(Eb + Mp)*TMath::Cos(theta_p*TMath::Pi()/180.) / (Mp*Mp + 2.*Mp*Eb + Eb*Eb*TMath::Power(TMath::Sin(theta_p*TMath::Pi()/180.),2)) ;


	
	if(TMath::Abs(theta_p)<100 && TMath::Abs(hmsP_calc)<100 && TMath::Abs(hmsP_meas)<100. && emiss > -0.08 && emiss < 0.05 && abs(hdelta) < 10. )
	  {

	    
	    //DEGUB
	    // cout << "KG: " << index + 1 << endl;
	    //cout << "hmsP_calc - hmsP_meas = " <<  hmsP_calc << " - " << leaf_var[2] << " = " <<  hmsP_calc - leaf_var[2] << endl;

	    //Fill Histograms
	    hist[index][0]->Fill(hmsP_calc);  //calculated momentum from formula (function of theta_p and Ebeam)
	    hist[index][1]->Fill(hmsP_meas);  //measured momentum H.gtr.p
	    hist[index][2]->Fill((hmsP_calc - hmsP_meas) / hmsP_meas);  //HMS (Calculated - Measured)/Measured Momentum (fractonal deviation from measured)
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


      TF1 *fit = new TF1("fit", "gaus", xmax_val - sig_Res, xmax_val + sig_Res);
      hist[index][2]->Fit("fit", "R");
  
      
      Double_t mu_Res_fit = fit->GetParameter(1);
      Double_t sig_Res_fit = fit->GetParameter(2);
      Double_t mu_Res_err_fit =  fit->GetParError(1);

      xRes_arr[index] = index + 1;          //kin group
      yRes_arr[index] = mu_Res_fit;          //mean fit fractional deviation from meas. momentum
      yRes_arr_err[index] =  mu_Res_err_fit;    //error from mean fit

     
      index++;

    } //end loop over runs 
  
  Double_t ex[5] = {0.};
  
  TGraphErrors* gr = new TGraphErrors(5,xRes_arr,yRes_arr, ex, yRes_arr_err);
  
  
  c3->cd();

  gr->SetTitle(" (P_{calc} - P_{meas}) / P_{meas.} vs. Kin. Group");

  gr->GetYaxis()->SetTitle("(P_{calc} - P_{meas}) / P_{measured} [fractional deviation]");
  gr->GetYaxis()->CenterTitle();
  
  gr->GetXaxis()->SetTitle("Kinematic Group");
  gr->GetXaxis()->CenterTitle();

  gr->SetMarkerStyle(21);
  gr->SetMarkerColor(kBlack);
  gr->SetMarkerSize(0.5);
  gr->Draw("AP");
  
  c0->SaveAs("testingMissing_Energy.pdf");
  c1->SaveAs("testingHMS_Calc_MeasP.pdf");
  c2->SaveAs("testingMomentum_Diff.pdf");
  c3->SaveAs("testingResidual_GRaph.pdf");
  
  

} //end function


