#include "TMath.h"
void calc_hElec_simc()
{

  gROOT->SetBatch(kTRUE);
  gStyle->SetOptFit();


  //Define some constants
  Double_t Mp = 0.938272;  //proton mass
  Double_t Eb;
 
  Double_t hP0[11] = {6.1421, 5.8613, 5.7134, 2.131, 2.982, 5.5422, 2.055, 5.205, 5.9166, 5.6732, 4.478};  //central spec. momentum  --Singles
  //Double_t hP0[11] ={3.6096, 3.6096, 1.864, 2.131, 5.5214, 2.982, 4.478, 0.936, 4.3139, 2.8714, 5.5422};

  //Define some variables to be determined inside the entry loop
  Double_t hmsP_calc;   //calculated HMS momentum
  Double_t hmsP_meas;   //measured HMS momentum 


  Float_t Normfac;
  Float_t h_delta;
  Float_t h_yptar;
  Float_t h_xptar;
  Float_t h_ytar;
  
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
 
  TH1F *hist_hPmeas[11];
  TH1F *hist_W[11];
  TH1F *hist_Em[11];
   
  //X,Y array from W fit to be used in TGraph
  Double_t x_arr[11];                                                                                                               
  Double_t W_mean_fit[11];                                                                                                
  Double_t W_mean_fit_err[11];
  Double_t W_sigma[11];

  //Define Canvas to Draw
  TCanvas *c0 = new TCanvas("c0", "Missing Enrgy", 1500, 1000);   
  c0->Divide(4,3);
  TCanvas *c1 = new TCanvas("c1", "Invariant Mass", 1500, 1000);   
  c1->Divide(4,3);
  TCanvas *c2 = new TCanvas("c2", "HMS Measured e- Momentum", 1500,1000);
  c2->Divide(4,3);
  TCanvas *c3 = new TCanvas("c3","Invariant Mass vs. Kin. Group", 1500,1000);
  
  Double_t FullWeight;


  int index = 0;

  //Loop over all kinematic groups
  for(int i = 0; i<11; i++)
    {
      string filename = Form("../worksim_voli/heep_hms_g%d_singles_hElec.root", index+1);                                   
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
      T->SetBranchAddress("q", &q);             
      T->SetBranchAddress("nu", &nu);           
      T->SetBranchAddress("Q2", &Q2);           
      T->SetBranchAddress("W", &W);             
      T->SetBranchAddress("Em", &Em);           
      T->SetBranchAddress("Pm", &Pm);           
      T->SetBranchAddress("Weight", &Weight);
      T->SetBranchAddress("theta_e", &theta_e);
      T->SetBranchAddress("theta_p", &theta_p);
      
       
      
    
      hist_Em[index] = new TH1F(Form("hSingles_Em_g%d", index+1), "", 100, -0.1, 0.2);
      hist_hPmeas[index] = new TH1F(Form("hSingles_Pmeas_g%d", index+1), "", 100, 0,8);
      
      if(index==6){ hist_W[index] = new TH1F(Form("hSingles_Winv_g%d", index+1), "", 100, 0.92, 0.96);}
      
      else{
      hist_W[index] = new TH1F(Form("hSingles_Winv_g%d", index+1), "", 100, 0.85, 1.05);
      }
      //-----------LOOP OVER ALL ENTRIES IN TREE-----------------------
      //Loop over all entries
      for(int i=0; i<T->GetEntries(); i++){
	
	T->GetEntry(i);
	
	//Calculate the cross-sect. weight
	FullWeight = (Normfac*Weight)/T->GetEntries();
	
	//Calculated proton Momentum (Using formula)
	//hmsP_calc = 2.*Mp*Eb*(Eb + Mp)*TMath::Cos(theta_p) / (Mp*Mp + 2.*Mp*Eb + Eb*Eb*TMath::Power(TMath::Sin(theta_p),2));
	//Measured Momentum Obtained from delta
	hmsP_meas = h_delta*hP0[index] / 100. + hP0[index];


	//DEBUG
	//cout << "theta_p = " << theta_p << endl;
	//cout << "numerator = " <<  2.*Mp*Eb*(Eb + Mp)*TMath::Cos(theta_p) << endl;
	//cout << "denominator = " << Mp*Mp + 2.*Mp*Eb + Eb*Eb*TMath::Power(TMath::Sin(theta_p),2) << endl;
	//cout << "hmsP_calc - hmsP_meas = " << hmsP_calc << " - " << hmsP_meas << " = " << hmsP_calc - hmsP_meas  << endl;

	
	//Fill Histograms
	if (Em<0.05&index!=6){

	  hist_W[index]->Fill(W, FullWeight);
	  hist_hPmeas[index]->Fill(hmsP_meas, FullWeight);      //Fill measure momentum
	
	}
	else if (Em<0.03) {hist_W[index]->Fill(W, FullWeight);}

	hist_Em[index]->Fill(Em, FullWeight);
      } //end entry loop
      
      
      //Draw Missing Energy
      c0->cd(index+1);
      hist_Em[index]->Draw("hist");
      //Draw Invariant Mass
      //c1->cd(index+1);
      //hist_W[index]->Draw("hist");
      //Draw Measured Momentum
      c2->cd(index+1);
      hist_hPmeas[index]->Draw("hist");
                                                                                    
      
      //Get maximum bin value/sigma from invariant mass histo to use as fit parameters
      int bin_max = hist_W[index]->GetMaximumBin(); 
      Double_t xmax_val = hist_W[index]->GetXaxis()->GetBinCenter(bin_max);
      Double_t sig_W = hist_W[index]->GetStdDev();

      c1->cd(index+1);                                                                                                        
      hist_W[index]->Draw();
      
      double nSig = 1.6;
      
      TF1 *fit = new TF1("fit", "gaus", xmax_val - nSig*sig_W, xmax_val + nSig*sig_W);
      hist_W[index]->Fit("fit", "R");    
        
   
      //Get W mean/sigma fit parameters
      Double_t mu_W_fit = fit->GetParameter(1);
      Double_t sig_W_fit = fit->GetParameter(2);
      Double_t mu_W_err_fit =  fit->GetParError(1); 



      x_arr[index] = index + 1;                                                                                                               
      W_mean_fit[index] = mu_W_fit;                                                                                                
      W_mean_fit_err[index] = mu_W_err_fit;
      W_sigma[index] = sig_W_fit;
    

      index++;
      
    } // end filename loop


 

  Double_t ex[11] = {0.};
  TGraphErrors* gr = new TGraphErrors(11,x_arr,W_mean_fit, ex, W_mean_fit_err);
  
  c3->cd();

  gr->SetTitle(" Invariant Mass vs. Kin. Group");

  gr->GetYaxis()->SetTitle("Invariant mass, W [GeV]");
  gr->GetYaxis()->CenterTitle();
  
  gr->GetXaxis()->SetTitle("Kinematic Group");
  gr->GetXaxis()->CenterTitle();

  gr->SetMarkerStyle(21);
  gr->SetMarkerColor(kBlack);
  gr->SetMarkerSize(0.9);
  gr->Draw("AP");
  
  c0->SaveAs("Missing_Energy.pdf");
  c1->SaveAs("Invariant_Mass.pdf");
  c2->SaveAs("Measured_Momentum.pdf");
  c3->SaveAs("W_vs_KinGroup_Graph.pdf");

  //Write the Fit Parameters into a file
  ofstream ofile;
  ofile.open("./hms_singles_Wfit.data");
  ofile << "  Kin_Group[f,0]/   " << "  W_mean_fit[f,1]/   " <<  "  W_mean_fit_err[f,2]/   " <<  "  W_sigma_fit[f,3]/  " <<endl;
  for(index=0; index<11; index++)
    {
      ofile << Form(" hms_singles_kg%d    ", index+1) << W_mean_fit[index]  << "      " << W_mean_fit_err[index] << "       " <<  W_sigma[index] << endl;

    }

  ofile.close();
}
