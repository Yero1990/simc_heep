void fitP()
{
  
  //Define Means of W and Emiss from fit
  double W_simc[4] = {0.9442, 0.9435, 0.9433, 0.9425};
  double W_simc_err[4] = {0.0002, 0.0003, 0.0001, 0.0001};

  double W_data[4] = {0.9512, 0.9550, 0.9488, 0.9486};
  double W_data_err[4] = {0.0001, 0., 0.0002, 0.0001};

  double Em_simc[4] = {0.004975, 0.004536, 0.005060, 0.004874};
  double Em_simc_err[4] = {0.000061, 0.000111, 0.00007, 0.000071};

  double Em_data[4] = {0.01826, 0.01088, 0.01422, 0.01166};
  double Em_data_err[4] = {0.00010, 0.00014, 0.00009, 0.00012};


  double eD_I[4] = {2732.87, 2732.87, 2732.87, 2732.87};            //SHMS Dipole Current [Amps]  RUns 3288, 3371, 3374, 3377
  double hD_NMR[4] = {0.804333, 0.952715, 0.632393, 0.517404};      //HMS Dipole Read NMR [Tesla]

  //Define some useful variables for calculations
  
  //Uncorrected / Corrected hadron/electron arm central momenta 
  double Ph[4] = {2.931169, 3.470903, 2.306895, 1.888348}; 
  double Pe[4] = {8.520526, 8.5261521, 8.526644, 8.528391};
  double Ph_corr[4]; 
  double Pe_corr[4];
  double Ph_corr_err[4];                                       
  double Pe_corr_err[4];   
  double corr_ftr[4];    //W correction factor
  double corr_ftr_err[4];

  double Eb = 10.6005;     //beam energy
  double Eb_err = 0.00415;        //error in beam energy
  double dW[4];    //W_simc - W_data
  double dW_err[4];   //error in dW
  double dEm[4];      //Em_simc - Em_data
  double dEm_err[4];

  double x_arr[4] = {1., 2., 3., 4.};

  //Loop over all runs
  for (int i=0; i<4; i++)
    {

      //-------W Alignment Calculations---------

      //calculate correction factor and its error
      dW[i] = W_simc[i] - W_data[i];
      dW_err[i] = sqrt( W_simc_err[i]*W_simc_err[i] + W_data_err[i]*W_data_err[i] );

      corr_ftr[i] = 1 - (dW[i] / Eb);
      corr_ftr_err[i] = sqrt( TMath::Power((1./Eb)*dW_err[i], 2) + TMath::Power((dW[i]/(Eb*Eb))*Eb_err,2) );
      
      Pe_corr[i] = Pe[i] * corr_ftr[i];
      Pe_corr_err[i] = Pe[i] * corr_ftr_err[i];


      //---------Emiss Alignment Calculations

      dEm[i] = Em_simc[i] - Em_data[i];
      dEm_err[i] =  sqrt(Em_simc_err[i]*Em_simc_err[i] + Em_data_err[i]*Em_data_err[i]);

      Ph_corr[i] = Ph[i] - dEm[i];       //dEp = -dEm,  Ep-->proton momentum
      Ph_corr_err[i] = dEm_err[i];

    }

  //Make Plots

  //W corr. factor vs. Run Number
  TCanvas *c1 = new TCanvas("c1", "W Corr. Factor", 1500, 1000);
  Double_t ex[4] = {0.};  //error in X is zero
  TGraphErrors* gr1 = new TGraphErrors(4,x_arr,corr_ftr, ex, corr_ftr_err); 
  c1->cd();

  gr1->SetTitle(" Electron Momentum Correction Factor");               
  gr1->GetYaxis()->SetTitle("Correction Factor");                                    
  gr1->GetYaxis()->CenterTitle();                                                     

  gr1->GetXaxis()->SetTitle("Kinematic Group"); 
  gr1->GetXaxis()->CenterTitle();                                                                                   
                                                                                                                             
  gr1->SetMarkerStyle(21);                                                                                                   
  gr1->SetMarkerColor(kBlack);                                                              
  gr1->SetMarkerSize(1.8);                                                                                                   
  gr1->Draw("AP");  

  //SHMS Corrected Momentum vs. Dipole Current
  TCanvas *c2 = new TCanvas("c2", "SHMS Corrected Momentum vs. Dipole Set Current", 1500, 1000);                                 
                                             
  TGraphErrors* gr2 = new TGraphErrors(4, eD_I, Pe_corr, ex, Pe_corr_err);                                                 
  c2->cd();                                                                                                 
                                                                                                                             
  gr2->SetTitle("SHMS Corr. Momentum vs. Dipole Current");                                                                     
  gr2->GetYaxis()->SetTitle("SHMS Corrected Momentum [GeV]");                                                                            
  gr2->GetYaxis()->CenterTitle();                                                                                            
                                                                                                                            
  gr2->GetXaxis()->SetTitle("SHMS Dipole Current [Amps]");                                                              
  gr2->GetXaxis()->CenterTitle();                                                                                           
                                                                                                                            
  gr2->SetMarkerStyle(21);                                                                                                   
  gr2->SetMarkerColor(kBlack);                                                                                               
  gr2->SetMarkerSize(1.8);                                                                                                   
  gr2->Draw("AP");        


  //HMS Corrected Momentum vs. NMR Setting                                                                                                                                           

  TCanvas *c3 = new TCanvas("c3", "HMS Corrected Momentum vs. Dipole NMR Field", 1500, 1000);                                 
  TGraphErrors* gr3 = new TGraphErrors(4, hD_NMR, Ph_corr, ex, Ph_corr_err);                                                                                                              
  c3->cd();                                                                                                                               
                                                                                                                                                                                         
  gr3->SetTitle("HMS Corr. Momentum vs. Dipole NMR Field");                                                
  gr3->GetYaxis()->SetTitle("HMS Corrected Momentum [GeV]");                                                                                                                            
  gr3->GetYaxis()->CenterTitle();                                                                                                                                                         
                                                                                                                                                                                          
  gr3->GetXaxis()->SetTitle("HMS Dipole NMR [T]");                                                                                                                                 
  gr3->GetXaxis()->CenterTitle();                                                                                                                                                         
                                                                                                                                                                                          
  gr3->SetMarkerStyle(21);                                                                                                                                                                
  gr3->SetMarkerColor(kBlack);                                                                                                                                                            
  gr3->SetMarkerSize(1.8);                                                                                                                                                                
  gr3->Draw("AP");  

  //Created Fit Function
  TF1 *lfit = new TF1("lfit", "[0] + x*[1]", 0.517404, 0.952715);

  lfit->SetParameters(1., 1.);
  gr3->Fit("lfit", "R");
  


}
