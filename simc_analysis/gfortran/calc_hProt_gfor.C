#include "TMath.h"
void calc_hProt_gfor()
{

  gROOT->SetBatch(kTRUE);
  gStyle->SetOptFit();


  //Define some constants
  Double_t Mp = 0.938272;  //proton mass
  Double_t Me = 0.00511; 
  Double_t Eb[8] = {2.221, 6.430, 6.430, 10.58794, 10.58794,10.58794, 10.58794, 10.58794 };
  Double_t hP0[8] = {1.136, 3.6196, 3.6, 2.9382, 2.938, 3.48, 2.31, 1.8899};  //central HMS spec. momentum
  Double_t sP0[8] = {1.555, 3.609, 3.6, 8.7, 8.7, 8.7, 8.7, 8.7};
  Double_t Normfac[8] = {0.522738E+07,0.801790E+07,0.798040E+07,0.160016E+07, 0.159546E+07, 0.239990E+07, 0.917906E+06, 0.607672E+06};

  //Define some variables to be determined inside the entry loop
  Double_t hmsP_calc;   //calculated HMS momentum
  Double_t hmsP_meas;   //measured HMS momentum 
  Double_t shmsP_meas;   //measured SHMS momentum 
  Double_t shmsE_meas;

  Float_t h_delta;
  Float_t h_yptar;
  Float_t h_xptar;
  Float_t h_ytar;
  Float_t s_delta;
  Float_t e_delta;
  Float_t e_yptar;
  Float_t e_xptar;
  Float_t e_ytar;
  

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
  TH1F *hist_hPcalc[8];
  TH1F *hist_hPmeas[8];
  TH1F *hist_hPDev[8];
  TH1F *hist_Em[8];
   
  Double_t xRes_arr[8];                                                                                                               
  Double_t yRes_arr[8];                                                                                                
  Double_t yRes_arr_err[8];

  //Define Canvas to Draw
  TCanvas *c0 = new TCanvas("c0", "MIssing Enrgy", 1500, 1000);   
  c0->Divide(4,2);
  TCanvas *c1 = new TCanvas("c1", "Calculated/Measured HMS Momentum", 1500, 1000);   
  c1->Divide(4,2);
  TCanvas *c2 = new TCanvas("c2", "HMS Momentum Perc Dev. From Measured", 1500,1000);
  c2->Divide(4,2);
  TCanvas *c3 = new TCanvas("c3","Percent Deviation from P_{meas} vs. Kin Group",200,10,500,300);
  
  Double_t FullWeight;


  int index = 0;

  //Loop over all kinematic groups
  for(int i = 0; i<8; i++)
    {
      string filename = Form("../worksim_voli/ep_coin_gfor_hProt_kg%d.root", index+1);                                   
      TFile *f1 = new TFile(filename.c_str());                                                                           

      //Get TTree                                                                                                                     
      TTree *T = (TTree*)f1->Get("h666");   
      
      /*-------DEUT SIMC---------------------*/
      //Set Branch Address for some SIMC variables
      T->SetBranchAddress("hsdelta", &h_delta);
      T->SetBranchAddress("hsyptar", &h_yptar);
      T->SetBranchAddress("hsxptar", &h_xptar);
      T->SetBranchAddress("hsytar", &h_ytar); 
      T->SetBranchAddress("ssdelta", &s_delta);
      T->SetBranchAddress("q", &q);             
      T->SetBranchAddress("nu", &nu);           
      T->SetBranchAddress("Q2", &Q2);           
      T->SetBranchAddress("W", &W);             
      T->SetBranchAddress("Em", &Em);           
      T->SetBranchAddress("Pm", &Pm);           
      T->SetBranchAddress("Weight", &Weight);
   
      
	
      if(index==0) {
	
	
	hist_hPcalc[index] = new TH1F(Form("hP_calc_g%d", index+1), "",  100, 0.5, 1.5);                    
	hist_hPmeas[index] = new TH1F(Form("hP_meas_g%d", index+1), "",  100, 0.5, 1.5);  
      }
      else if(index>=1 && index<=2){
	
	hist_hPcalc[index] = new TH1F(Form("hP_calc_g%d", index+1), "",  100, 2, 4);                    
	hist_hPmeas[index] = new TH1F(Form("hP_meas_g%d", index+1), "",  100, 2, 4);
      }
      else if(index>2){

	hist_hPcalc[index] = new TH1F(Form("hP_calc_g%d", index+1), "",  100, 1.5, 4);                    
	hist_hPmeas[index] = new TH1F(Form("hP_meas_g%d", index+1), "",  100, 1.5, 4);
	
      }
      
      hist_hPDev[index] = new TH1F(Form("hP_Dev_g%d", index+1), "", 80, -4, 4);
      hist_Em[index] = new TH1F(Form("h_Em_g%d", index+1), "", 100, -0.2, 0.3);

      //-----------LOOP OVER ALL ENTRIES IN TREE-----------------------
      //Loop over all entries
      for(int i=0; i<T->GetEntries(); i++){
	
	T->GetEntry(i);
	
	//Calculate the cross-sect. weight
	FullWeight = (Normfac[index]*Weight)/T->GetEntries();
	
	//Measured Momentum Obtained from delta
	hmsP_meas = h_delta*hP0[index] / 100. + hP0[index];
	shmsP_meas = s_delta*sP0[index] / 100. + sP0[index];
	shmsE_meas = TMath::Sqrt(Me*Me + shmsP_meas*shmsP_meas);
	
	//Measured Spec Angles
	theta_e = 2.*TMath::ASin(TMath::Sqrt(Q2/(4.*shmsE_meas*Eb[index])));
	theta_p = TMath::ASin(shmsP_meas * TMath::Sin(theta_e) / hmsP_meas);

	//Calculated proton Momentum (Using formula)
	hmsP_calc = 2.*Mp*Eb[index]*(Eb[index] + Mp)*TMath::Cos(theta_p) / (Mp*Mp + 2.*Mp*Eb[index] + Eb[index]*Eb[index]*TMath::Power(TMath::Sin(theta_p),2));

	//DEBUG
	cout << "theta_p = " << theta_p << endl;
	cout << "theta_e = " << theta_e << endl;
	cout << "hP_meas = " << hmsP_meas << endl;
	cout << "sP_meas = " << shmsP_meas << endl;

	//cout << "numerator = " <<  2.*Mp*Eb[index]*(Eb[index] + Mp)*TMath::Cos(theta_p) << endl;
	//cout << "denominator = " << Mp*Mp + 2.*Mp*Eb[index] + Eb[index]*Eb[index]*TMath::Power(TMath::Sin(theta_p),2) << endl;
	//cout << "hmsP_calc - hmsP_meas = " << hmsP_calc << " - " << hmsP_meas << " = " << hmsP_calc - hmsP_meas  << endl;

	
	//Fill Histograms
	if (Em<0.05 && abs(h_delta) < 8.0){
	hist_hPcalc[index]->Fill(hmsP_calc, FullWeight);      //Fill calculated momentum
	hist_hPmeas[index]->Fill(hmsP_meas, FullWeight);      //Fill measure momentum
	hist_hPDev[index]->Fill( (hmsP_calc - hmsP_meas)/ hmsP_meas*100. , FullWeight);   //Fille % Deviation of Calculated from Measured Momentum
	}

	hist_Em[index]->Fill(Em, FullWeight);
      } //end entry loop
      

      c0->cd(index+1);
      hist_Em[index]->Draw("hist");

      c1->cd(index+1);
      hist_hPcalc[index]->Draw("hist");
      hist_hPcalc[index]->SetLineColor(kRed);
      hist_hPmeas[index]->Draw("sameshist");
      
      //Get maximum bin value/sigma from momentum difference histo to use as fit parameters
      int bin_max = hist_hPDev[index]->GetMaximumBin(); 
      Double_t xmax_val = hist_hPDev[index]->GetXaxis()->GetBinCenter(bin_max);
      Double_t sig_Res = hist_hPDev[index]->GetStdDev();
      
      c2->cd(index+1);
      hist_hPDev[index]->Draw();
      
      TF1 *fit = new TF1("fit", "gaus", xmax_val - 1.0*sig_Res, xmax_val + 1.0*sig_Res);
      hist_hPDev[index]->Fit("fit", "R");
      
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


 

  Double_t ex[8] = {0.};
  TGraphErrors* gr = new TGraphErrors(8,xRes_arr,yRes_arr, ex, yRes_arr_err);
  
  c3->cd();

  gr->SetTitle(" (P_calc - P_meas) / P_{meas.} vs. Kin. Group");

  gr->GetYaxis()->SetTitle("(P_{calc} - P_{meas}) / P_{measured} [% deviation]");
  gr->GetYaxis()->CenterTitle();
  
  gr->GetXaxis()->SetTitle("Kinematic Group");
  gr->GetXaxis()->CenterTitle();

  gr->SetMarkerStyle(21);
  gr->SetMarkerColor(kBlack);
  gr->SetMarkerSize(0.5);
  gr->Draw("AP");
  
  c0->SaveAs("Missing_Energy_gfor.pdf");
  c1->SaveAs("HMS_Calc_MeasP_gfor.pdf");
  c2->SaveAs("Momentum_Diff_gfor.pdf");
  c3->SaveAs("Residual_GRaph_gfor.pdf");

  

}
