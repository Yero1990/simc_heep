//Code to check electron (calculated - measured) / measured momentum in the SHMS
//shmsP_calc(Ebeam, proton momentum)---> No delta dependence on the calculated momentum.
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

  //Create an object array to store histograms 
  TObjArray data_HList[5]; 
  
  TObjArray simc_HList[5]; 


  //Define some constants
  Double_t Mp = 0.938272;  //proton mass
  Double_t Eb = 10.6005;
  //Double_t eP0[5] = {8.5640277, 8.5640277, 8.5640277, 8.5640277, 8.5640277};  //central spec. momentum (UnCorrected SHMS Central Momentum)
  //Double_t eP0[5] = {8.554008, 8.562092, 8.559258, 8.560713, 8.560338};  //central spec. momentum (Corrected SHMS Central Momentum --After delta optimization)
  // Double_t eP0[5] = {8.525027, 8.525232, 8.528170, 8.529253, 8.528954};   //central spec momentum (Used for Emiss Correction)
  Double_t eP0[5] = {8.520256, 8.526357, 8.526643, 8.529176, 8.528391};   //central spec momentum (Final PCorr)

  //Double_t hP0[5] = {2.922111, 3.46017618, 2.2997660, 1.8827606, 1.8825118};  //central spec. momentum (no hYptar Offset)
  Double_t hP0[5] = {2.931170, 3.4709027, 2.3068953, 1.8885972, 1.8883477};    //central spec momentum (Momentum COrrection after hYptar Offset --3288 for now ONLY) 


  //Define some variables to be determined inside the entry loop
  Double_t shms_delta_calc;
  Double_t shmsP_calc;   //calculated SHMS momentum
  Double_t hmsP_calc;    //calculated HMS momentum
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
  TH1F *hist_ePcalc[5];
  TH1F *hist_ePmeas[5];
  TH1F *hist_ePDev[5];
  TH1F *hist_Em[5];

  //cROSS cHECK
  TH2F *hist_hPDev_hxfp[5];   //HMS Momentum fraction correlations with HMS focal plane
  TH2F *hist_hPDev_hxpfp[5];  
  TH2F *hist_hPDev_hyfp[5]; 
  TH2F *hist_hPDev_hypfp[5];

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
  TCanvas *c0_simc = new TCanvas("c0_simc", "Missing Enrgy", 1500, 1000);   
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

  //Canvas to plot HMS fractional momentum vs. HMS focal Plane (As cross check)
  TCanvas *c6_simc[5];

  Double_t FullWeight;

  int run_num[5] = {3288, 3371, 3374, 3376, 3377};

  //  int irun = 0;

  
 
  //Loop over all kinematic groups
  for(int irun = 0; irun<5; irun++)
    {
      
      //Initialize histo object array
      simc_HList[irun](0);

      //string filename = Form("../../../worksim_voli/D2_pUnCorr/D2_heep_%d.root", run_num[irun]);                                   
      //string filename = Form("../../../worksim_voli/D2_pCorr/D2_heep_%d.root", run_num[irun]);                                   
      string filename = Form("../../../worksim_voli/D2_heep_%d.root", run_num[irun]);                                   

      TFile *f1 = new TFile(filename.c_str());                                                                           

      //Get TTree                                                                                                                     
      TTree *T = (TTree*)f1->Get("SNT");   
      
      //-------DEUT SIMC---------------------/
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
      
      
      hist_ePcalc[irun] = new TH1F(Form("eP_calc_Run%d", run_num[irun]), "",  100, 7, 10);                   
      hist_ePmeas[irun] = new TH1F(Form("eP_meas_Run%d", run_num[irun]), "",  100, 7, 10);
      
      hist_ePDev[irun] = new TH1F(Form("eP_Dev_Run%d", run_num[irun]), "", 100, -0.01, 0.01);
      hist_Em[irun] = new TH1F(Form("h_Em_Run%d", run_num[irun]), "", 100, -0.2, 0.3);

      //HMS hPDiff vs. HMS focal planes 
      hist_hPDev_hxfp[irun] = new TH2F(Form("hPDiff_vs_hxfp Run %d", run_num[irun]), "", 100, -40, 40, 80, -0.05, 0.05);
      hist_hPDev_hxpfp[irun]= new TH2F(Form("hPDiff_vs_hxpfp Run %d", run_num[irun]), "", 100, -0.06, 0.06, 80, -0.05, 0.05);
      hist_hPDev_hyfp[irun] = new TH2F(Form("hPDiff_vs_hyfp Run %d", run_num[irun]), "", 100, -10, 30, 80, -0.05, 0.05);
      hist_hPDev_hypfp[irun]= new TH2F(Form("hPDiff_vs_hypfp Run %d", run_num[irun]), "", 100, -0.02, 0.04, 80, -0.05, 0.05);
 
      if(irun==3 || irun==4){
      hist_ePDev_xfp[irun] = new TH2F(Form("ePDiff_vs_xfp: Run %d", run_num[irun]), "", 100, 10, 25, 80, -0.01, 0.01);
      hist_ePDev_xpfp[irun] = new TH2F(Form("ePDiff_vs_xpfp: Run %d", run_num[irun]), "", 100, 0.01, 0.05, 80, -0.01, 0.01); 
      hist_ePDev_yfp[irun] = new TH2F(Form("ePDiff_vs_yfp: Run %d", run_num[irun]), "", 100, -10, 5, 80, -0.01, 0.01);        
      hist_ePDev_ypfp[irun] = new TH2F(Form("ePDiff_vs_ypfp: Run %d", run_num[irun]), "", 100, -0.015, 0.015, 80, -0.01, 0.01);

      //(shms_delta_calc - shms_delta_meas) vs. shms focal plane                                                                                        
      hist_eDelta_xfp[irun] = new TH2F(Form("eDelta_vs_xfp: Run %d", run_num[irun]), "", 100, 10., 25, 80, -1., 1.);                                   
      hist_eDelta_xpfp[irun] = new TH2F(Form("eDelta_vs_xpfp: Run %d", run_num[irun]), "", 100, 0.01, 0.05, 80, -1., 1.);                            
      hist_eDelta_yfp[irun] = new TH2F(Form("eDelta_vs_yfp: Run %d", run_num[irun]), "", 100, -10., 5, 80, -1., 1.);                                  
      hist_eDelta_ypfp[irun] = new TH2F(Form("eDelta_vs_ypfp: Run %d", run_num[irun]), "", 100, -0.015, 0.015, 80, -1., 1.); 
  
      }

      if(irun==1){                                                                                                                                  
	hist_ePDev_xfp[irun] = new TH2F(Form("ePDiff_vs_xfp: Run %d", run_num[irun]), "", 100, -30, -5, 80, -0.01, 0.01);                      
	hist_ePDev_xpfp[irun] = new TH2F(Form("ePDiff_vs_xpfp: Run %d", run_num[irun]), "", 100, -0.07, 0.02, 80, -0.01, 0.01);           
	hist_ePDev_yfp[irun] = new TH2F(Form("ePDiff_vs_yfp: Run %d", run_num[irun]), "", 100, -25, 15, 80, -0.01, 0.01);                
	hist_ePDev_ypfp[irun] = new TH2F(Form("ePDiff_vs_ypfp: Run %d", run_num[irun]), "", 100, -0.04, 0.03, 80, -0.01, 0.01);                  
      
	//(shms_delta_calc - shms_delta_meas) vs. shms focal plane                                                                   
        hist_eDelta_xfp[irun] = new TH2F(Form("eDelta_vs_xfp: Run %d", run_num[irun]), "", 100, -30., -5, 80, -1., 1.);                   
        hist_eDelta_xpfp[irun] = new TH2F(Form("eDelta_vs_xpfp: Run %d", run_num[irun]), "", 100, -0.07, 0.02, 80, -1., 1.);   
        hist_eDelta_yfp[irun] = new TH2F(Form("eDelta_vs_yfp: Run %d", run_num[irun]), "", 100, -25., 15, 80, -1., 1.);            
        hist_eDelta_ypfp[irun] = new TH2F(Form("eDelta_vs_ypfp: Run %d", run_num[irun]), "", 100, -0.04, 0.03, 80, -1., 1.); 
      }

      if(irun==2){                                                                                                                            
        hist_ePDev_xfp[irun] = new TH2F(Form("ePDiff_vs_xfp: Run %d", run_num[irun]), "", 100, 5, 15, 80, -0.01, 0.01);         
        hist_ePDev_xpfp[irun] = new TH2F(Form("ePDiff_vs_xpfp: Run %d", run_num[irun]), "", 100, 0., 0.04, 80, -0.01, 0.01);    
        hist_ePDev_yfp[irun] = new TH2F(Form("ePDiff_vs_yfp: Run %d", run_num[irun]), "", 100, -5, 5, 80, -0.01, 0.01);      
        hist_ePDev_ypfp[irun] = new TH2F(Form("ePDiff_vs_ypfp: Run %d", run_num[irun]), "", 100, -0.02, 0.02, 80, -0.01, 0.01);          
      
	//(shms_delta_calc - shms_delta_meas) vs. shms focal plane                                                                       
        hist_eDelta_xfp[irun] = new TH2F(Form("eDelta_vs_xfp: Run %d", run_num[irun]), "", 100, 5., 15, 80, -1., 1.);                          
        hist_eDelta_xpfp[irun] = new TH2F(Form("eDelta_vs_xpfp: Run %d", run_num[irun]), "", 100, 0., 0.04, 80, -1., 1.);            
        hist_eDelta_yfp[irun] = new TH2F(Form("eDelta_vs_yfp: Run %d", run_num[irun]), "", 100, -5., 5, 80, -1., 1.);                    
        hist_eDelta_ypfp[irun] = new TH2F(Form("eDelta_vs_ypfp: Run %d", run_num[irun]), "", 100, -0.02, 0.02, 80, -1., 1.); 
      }  

      if(irun==0){                                                                                                             
        hist_ePDev_xfp[irun] = new TH2F(Form("ePDiff_vs_xfp: Run %d", run_num[irun]), "", 100, 10., 5, 80, -0.01, 0.01);      
        hist_ePDev_xpfp[irun] = new TH2F(Form("ePDiff_vs_xpfp: Run %d", run_num[irun]), "", 100, -0.04, 0.03, 80, -0.01, 0.01);          
        hist_ePDev_yfp[irun] = new TH2F(Form("ePDiff_vs_yfp: Run %d", run_num[irun]), "", 100, -10, 5, 80, -0.01, 0.01);         
        hist_ePDev_ypfp[irun] = new TH2F(Form("ePDiff_vs_ypfp: Run %d", run_num[irun]), "", 100, -0.02, 0.02, 80, -0.01, 0.01);            

	//(shms_delta_calc - shms_delta_meas) vs. shms focal plane
	hist_eDelta_xfp[irun] = new TH2F(Form("eDelta_vs_xfp: Run %d", run_num[irun]), "", 100, 10., 5, 80, -1., 1.);
	hist_eDelta_xpfp[irun] = new TH2F(Form("eDelta_vs_xpfp: Run %d", run_num[irun]), "", 100, -0.04, 0.03, 80, -1., 1.);
	hist_eDelta_yfp[irun] = new TH2F(Form("eDelta_vs_yfp: Run %d", run_num[irun]), "", 100, -10., 5, 80, -1., 1.);
	hist_eDelta_ypfp[irun] = new TH2F(Form("eDelta_vs_ypfp: Run %d", run_num[irun]), "", 100, -0.02, 0.02, 80, -1., 1.);	

      } 

      hist_ePDev_xfp[irun]->GetXaxis()->SetTitle("SHMS X_{fp}");
      hist_ePDev_xfp[irun]->GetYaxis()->SetTitle("SHMS [P_{calc} - P_{meas}] / P_{meas}");   
      
     
      hist_ePDev_xpfp[irun]->GetXaxis()->SetTitle("SHMS X'_{fp}"); 
      hist_ePDev_xpfp[irun]->GetYaxis()->SetTitle("SHMS [P_{calc} - P_{meas}] / P_{meas}"); 

      hist_ePDev_yfp[irun]->GetXaxis()->SetTitle("SHMS Y_{fp}");                                                        
      hist_ePDev_yfp[irun]->GetYaxis()->SetTitle("SHMS [P_{calc} - P_{meas}] / P_{meas}");                              
                  
      hist_ePDev_ypfp[irun]->GetXaxis()->SetTitle("SHMS Y'_{fp}");                                                         
      hist_ePDev_ypfp[irun]->GetYaxis()->SetTitle("SHMS [P_{calc} - P_{meas}] / P_{meas}");          

      //----set names for delta difference histograms
      hist_eDelta_xfp[irun]->GetXaxis()->SetTitle("SHMS X_{fp}");
      hist_eDelta_xfp[irun]->GetYaxis()->SetTitle("SHMS #delta_{calc} - #delta_{meas.} [%]");                                             

      hist_eDelta_xpfp[irun]->GetXaxis()->SetTitle("SHMS X'_{fp}");                                                                        
      hist_eDelta_xpfp[irun]->GetYaxis()->SetTitle("SHMS #delta_{calc} - #delta_{meas.} [%]");  

      hist_eDelta_yfp[irun]->GetXaxis()->SetTitle("SHMS Y_{fp}");                                                       
      hist_eDelta_yfp[irun]->GetYaxis()->SetTitle("SHMS #delta_{calc} - #delta_{meas.} [%]");                                                 
                                                                                                                                                   
      hist_eDelta_ypfp[irun]->GetXaxis()->SetTitle("SHMS Y'_{fp}");                                                                         
      hist_eDelta_ypfp[irun]->GetYaxis()->SetTitle("SHMS #delta_{calc} - #delta_{meas.} [%]");                                                 
	

      //Add the histogram objects to the Obj list
      simc_HList[irun].Add(hist_ePcalc[irun]);
      simc_HList[irun].Add(hist_ePmeas[irun]);
      simc_HList[irun].Add(hist_ePDev[irun]);
      simc_HList[irun].Add(hist_Em[irun]);


      simc_HList[irun].Add(hist_eDelta_xfp[irun]);
      simc_HList[irun].Add(hist_eDelta_xpfp[irun]);
      simc_HList[irun].Add(hist_eDelta_yfp[irun]);
      simc_HList[irun].Add(hist_eDelta_ypfp[irun]);

      simc_HList[irun].Add(hist_ePDev_xfp[irun]);
      simc_HList[irun].Add(hist_ePDev_xpfp[irun]);  
      simc_HList[irun].Add(hist_ePDev_yfp[irun]);                                                    
      simc_HList[irun].Add(hist_ePDev_ypfp[irun]); 



      //Define some cuts
      Bool_t simc_hmsDelta_cut;
      Bool_t simc_shmsDelta_cut;
      Bool_t simc_hmsPDiff_cut;
    
      //-----------LOOP OVER ALL ENTRIES IN TREE-----------------------
      //Loop over all entries
      for(int i=0; i<T->GetEntries(); i++){
	
	T->GetEntry(i);
	
	//Calculate the cross-sect. weight
	FullWeight = (Normfac*Weight)/T->GetEntries();

	//Define some cuts
      

	hmsP_meas = h_delta*hP0[irun] / 100. + hP0[irun]; // delta = (P - P0) / P0 * 100

        //Calculated proton Momentum (Using formula)                                                           
        hmsP_calc = 2.*Mp*Eb*(Eb + Mp)*TMath::Cos(theta_p) / (Mp*Mp + 2.*Mp*Eb + Eb*Eb*TMath::Power(TMath::Sin(theta_p),2)); 

	//Calculated electron Momentum (Using formula --depend on hms measured momentum, which is well known)
	shmsP_calc = Eb + Mp - TMath::Sqrt(hmsP_meas*hmsP_meas + Mp*Mp);
	//Measured Momentum Obtained from delta
	shmsP_meas = e_delta*eP0[irun] / 100. + eP0[irun];
	
	shms_delta_calc = (shmsP_calc - eP0[irun]) / eP0[irun] * 100.;  //shms calculated delta in %

	//Define some cuts    
        simc_hmsDelta_cut = TMath::Abs(h_delta)<8.0;                                                                                                 
        simc_shmsDelta_cut = e_delta > -10. && e_delta < 22.;                                                                         
        simc_hmsPDiff_cut = TMath::Abs((hmsP_calc - hmsP_meas) ) < 0.02;

	//Fill Histograms
	if (Em<0.03 && simc_hmsDelta_cut && simc_shmsDelta_cut && simc_hmsPDiff_cut){
	hist_ePcalc[irun]->Fill(shmsP_calc, FullWeight);      //Fill calculated momentum
	hist_ePmeas[irun]->Fill(shmsP_meas, FullWeight);      //Fill measure momentum
	hist_ePDev[irun]->Fill( (shmsP_calc - shmsP_meas)/ shmsP_meas, FullWeight);   //Fill Fractional  Deviation of Calculated from Measured Momentum

	hist_hPDev_hxfp[irun]->Fill(h_xfp,(hmsP_calc - hmsP_meas)/ hmsP_meas, FullWeight );
	hist_hPDev_hxpfp[irun]->Fill(h_xpfp,(hmsP_calc - hmsP_meas)/ hmsP_meas, FullWeight );
	hist_hPDev_hyfp[irun]->Fill(h_yfp,(hmsP_calc - hmsP_meas)/ hmsP_meas, FullWeight );
	hist_hPDev_hypfp[irun]->Fill(h_ypfp,(hmsP_calc - hmsP_meas)/ hmsP_meas, FullWeight );

	hist_ePDev_xfp[irun]->Fill(e_xfp, (shmsP_calc - shmsP_meas)/ shmsP_meas, FullWeight);
        hist_ePDev_xpfp[irun]->Fill(e_xpfp, (shmsP_calc - shmsP_meas)/ shmsP_meas, FullWeight);  
	hist_ePDev_yfp[irun]->Fill(e_yfp, (shmsP_calc - shmsP_meas)/ shmsP_meas, FullWeight);
	hist_ePDev_ypfp[irun]->Fill(e_ypfp, (shmsP_calc - shmsP_meas)/ shmsP_meas, FullWeight); 

	hist_eDelta_xfp[irun]->Fill(e_xfp, (shms_delta_calc - e_delta), FullWeight);
	hist_eDelta_xpfp[irun]->Fill(e_xpfp, (shms_delta_calc - e_delta), FullWeight);                                                       
	hist_eDelta_yfp[irun]->Fill(e_yfp, (shms_delta_calc - e_delta), FullWeight);                                                         
        hist_eDelta_ypfp[irun]->Fill(e_ypfp, (shms_delta_calc - e_delta), FullWeight);                                                                 

	}

	hist_Em[irun]->Fill(Em, FullWeight);  //Fill Missing Energy
      

      } //end entry loop
      

      c0_simc->cd(irun+1);
      hist_Em[irun]->Draw("hist");

      c1_simc->cd(irun+1);
      hist_ePcalc[irun]->Draw("hist");
      hist_ePcalc[irun]->SetLineColor(kRed);
      hist_ePmeas[irun]->Draw("sameshist");


      c4_simc[irun] = new TCanvas(Form("c4_simc, Run %d", run_num[irun]), "Fract. Momentum vs. SHMS Focal Plane", 1500, 1000);       
      c4_simc[irun]->Divide(2,2);  
      c4_simc[irun]->cd(1);
      hist_ePDev_xfp[irun]->Draw("colz");
      c4_simc[irun]->cd(2); 
      hist_ePDev_xpfp[irun]->Draw("colz");
      c4_simc[irun]->cd(3);
      hist_ePDev_yfp[irun]->Draw("colz"); 
      c4_simc[irun]->cd(4);
      hist_ePDev_ypfp[irun]->Draw("colz");   

      c4_simc[irun]->SaveAs(Form("simc_ePdiff_vs_SHMS_FocalPlane_%d.pdf", run_num[irun]));



      c5_simc[irun] = new TCanvas(Form("c5_simc, Run %d", run_num[irun]), "Delta Diff. vs. SHMS Focal Plane", 1500, 1000);                          
      c5_simc[irun]->Divide(2,2);
      c5_simc[irun]->cd(1);
      hist_eDelta_xfp[irun]->Draw("colz");
      c5_simc[irun]->cd(2);                                                                                                       
      hist_eDelta_xpfp[irun]->Draw("colz"); 
      c5_simc[irun]->cd(3);                                                                                
      hist_eDelta_yfp[irun]->Draw("colz");                                                          
      c5_simc[irun]->cd(4);                                                             
      hist_eDelta_ypfp[irun]->Draw("colz");

      c5_simc[irun]->SaveAs(Form("simc_DeltaDiff_vs_SHMS_FocalPlane_%d.pdf", run_num[irun]));   

      
      c6_simc[irun] = new TCanvas(Form("c6_simc, Run %d", run_num[irun]), "HMS: Fractional Momentum Difference. vs. Focal Plane", 1500, 1000);                          
      c6_simc[irun]->Divide(2,2);
      c6_simc[irun]->cd(1);
      hist_hPDev_hxfp[irun]->Draw("colz");
      c6_simc[irun]->cd(2);                                                                                                       
      hist_hPDev_hxpfp[irun]->Draw("colz"); 
      c6_simc[irun]->cd(3);                                                                                
      hist_hPDev_hyfp[irun]->Draw("colz");                                                          
      c6_simc[irun]->cd(4);                                                             
      hist_hPDev_hypfp[irun]->Draw("colz");

      c6_simc[irun]->SaveAs(Form("simc_hPDev_vs_HMS_FocalPlane_%d.pdf", run_num[irun]));   


      //Get maximum bin value/sigma from momentum difference histo to use as fit parameters
      int bin_max = hist_ePDev[irun]->GetMaximumBin(); 
      Double_t xmax_val = hist_ePDev[irun]->GetXaxis()->GetBinCenter(bin_max);
      Double_t sig_Res = hist_ePDev[irun]->GetStdDev();
      
      c2_simc->cd(irun+1);
      hist_ePDev[irun]->Draw();
      
      TF1 *fit = new TF1("fit", "gaus", xmax_val - 1.0*sig_Res, xmax_val + 1.0*sig_Res);
      hist_ePDev[irun]->Fit("fit", "R");
      
      Double_t mu_Res_fit = fit->GetParameter(1);
      Double_t sig_Res_fit = fit->GetParameter(2);
      Double_t mu_Res_err_fit =  fit->GetParError(1); 

      cout << "mean_fit = " << mu_Res_fit << endl;

      xRes_arr[irun] = irun + 1;                                                                                                               
      yRes_arr[irun] = mu_Res_fit;                                                                                                
      yRes_arr_err[irun] = mu_Res_err_fit;
           
      cout << "yRes_arr = " << yRes_arr[irun] << endl;
      
      //Write Histograms to a ROOTfile
      TFile *outROOT = new TFile(Form("SHMS_heepSIMC_histos_%d.root", run_num[irun]), "recreate");
      simc_HList[irun].Write();
      outROOT->Close();
 
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
  
  //Em cut array
   Double_t Em_cut_arr[5] = {0.01, 0.03, 0., 0., 0.};
  
 //Define some variables to be determined inside the entry loop
  Double_t htheta_p;     //proton arm angle (event by event)

  //Define TTree variables
  TString n_xangle = "H.kin.secondary.xangle";        //opening angle between HMS and SHMS
  TString n_theta_e = "P.kin.primary.scat_ang_deg";   //e- scat angle [deg]
  TString n_shmsP_meas = "P.gtr.p";
  TString n_hmsP_meas = "H.gtr.p";                                                                                                  
  TString n_emiss = "H.kin.secondary.emiss";
  TString n_edelta = "P.gtr.dp";
  TString n_hdelta = "H.gtr.dp";

  TString n_exfp = "P.dc.x_fp";
  TString n_expfp = "P.dc.xp_fp"; 
  TString n_eyfp = "P.dc.y_fp";               
  TString n_eypfp = "P.dc.yp_fp"; 
  
  TString n_hxfp = "H.dc.x_fp";
  TString n_hxpfp = "H.dc.xp_fp";
  TString n_hyfp = "H.dc.y_fp";
  TString n_hypfp = "H.dc.yp_fp";

  Double_t xangle;
  Double_t ptheta_e;   //e- scat angle [rad]
  Double_t shms_Pmeas;
  Double_t hms_Pmeas;                                                                                                                               
  Double_t emiss;
  Double_t edelta;
  Double_t hdelta;

  Double_t exfp, expfp, eyfp, eypfp;
  Double_t hxfp, hxpfp, hyfp, hypfp;


  //-------------CREATE EMPTY HISTOGRAMS---------------------------------------------
  
  TH1F *hist[5][5];

  TH1F *histData_hPcalc[5];
  TH1F *histData_hPmeas[5];
  TH1F *histData_hPDev[5];
  TH1F *histData_Em[5];
  
  //Momentum fraction correlations with HMS focal plane
  TH2F *histhPDev_hxfp[5];  
  TH2F *histhPDev_hxpfp[5];  
  TH2F *histhPDev_hyfp[5]; 
  TH2F *histhPDev_hypfp[5]; 
  
  //Momentum fraction correlations with SHMS focal plane
  TH2F *histePDev_xfp[5];  
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
 
  //Canvas to plot fractional momentum vs. SHMS focal plane
  TCanvas *c4[5];
  
  //Canvas to plot delta_calc - delta_meas vs. shms focal plane
  TCanvas *c5[5];
  
  //Canvas to plot HMS fractional momentum vs. HMS focal Plane (As cross check)
  TCanvas *c6[5];

  //irun = 0;
  int run[5] = {3288, 3371, 3374, 3376, 3377};
  cout << "Entering Run Loop " << endl;
  //Loop over runs
    for(int irun=0; irun<5; irun++)
      {                                 
                            
	//Initialize histo object array
	data_HList[irun](0);

                        
	//Open TFile
	//string filename = Form("../../../../hallc_replay/ROOTfiles/D2_heep/delta_uncorr/coin_replay_heep_check_%d_-1.root",run[irun]);
	//string filename = Form("../../../../hallc_replay/ROOTfiles/D2_heep/delta_corr/pCorr/coin_replay_heep_check_%d_-1.root",run[irun]);
	//string filename = Form("../../../../hallc_replay/ROOTfiles/D2_heep/delta_corr/pUnCorr/coin_replay_heep_check_%d_-1.root",run[irun]);
	string filename = Form("../../../../hallc_replay/ROOTfiles/coin_replay_heep_check_%d_50000.root",run[irun]);

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
	T->SetBranchAddress(n_hdelta, &hdelta);
	T->SetBranchAddress(n_exfp, &exfp);
	T->SetBranchAddress(n_expfp, &expfp);
	T->SetBranchAddress(n_eyfp, &eyfp);
	T->SetBranchAddress(n_eypfp, &eypfp);

	T->SetBranchAddress(n_hxfp, &hxfp);
	T->SetBranchAddress(n_hxpfp, &hxpfp);
	T->SetBranchAddress(n_hyfp, &hyfp);
	T->SetBranchAddress(n_hypfp, &hypfp);
	
	histData_hPcalc[irun] = new TH1F(Form("data_Pcalc_Run%d", run[irun]), Form("Run %d",run[irun]), 100, 7, 10); 
	histData_hPmeas[irun] = new TH1F(Form("data_Pmeas_Run%d", run[irun]), Form("Run %d",run[irun]), 100, 7, 10); ;
    	histData_hPDev[irun] = new TH1F(Form("data_pDiff_Run%d",run[irun]), Form("Run %d: SHMS (P_{calc} - P_{meas})/P_{meas}", run[irun]), 100, -0.01, 0.01);
	histData_Em[irun] = new TH1F(Form("Emiss_Run%d",run[irun]), Form("Run %d: E_{miss}", run[irun]), 100, -0.2, 0.3);
	  
	//HMS hPDiff vs. HMS focal planes 
	histhPDev_hxfp[irun] = new TH2F(Form("hPDiff_vs_hxfp: Run %d", run_num[irun]), "", 100, -40, 40, 80, -0.05, 0.05);
	histhPDev_hxpfp[irun]= new TH2F(Form("hPDiff_vs_hxpfp: Run %d", run_num[irun]), "", 100, -0.06, 0.06, 80, -0.05, 0.05);
	histhPDev_hyfp[irun] = new TH2F(Form("hPDiff_vs_hyfp: Run %d", run_num[irun]), "", 100, -10, 30, 80, -0.05, 0.05);
	histhPDev_hypfp[irun]= new TH2F(Form("hPDiff_vs_hypfp: Run %d", run_num[irun]), "", 100, -0.02, 0.04, 80, -0.05, 0.05);
 
	if(irun==3 || irun==4){                                                                                                         
	  histePDev_xfp[irun] = new TH2F(Form("ePDiff_vs_xfp: Run %d", run_num[irun]), "", 100, 10, 25, 80, -0.01, 0.01);                    
	  histePDev_xpfp[irun] = new TH2F(Form("ePDiff_vs_xpfp: Run %d", run_num[irun]), "", 100, 0.01, 0.05, 80, -0.01, 0.01);        
	  histePDev_yfp[irun] = new TH2F(Form("ePDiff_vs_yfp: Run %d", run_num[irun]), "", 100, -10, 5, 80, -0.01, 0.01);                   
	  histePDev_ypfp[irun] = new TH2F(Form("ePDiff_vs_ypfp: Run %d", run_num[irun]), "", 100, -0.015, 0.015, 80, -0.01, 0.01);               
	
	  //(shms_delta_calc - shms_delta_meas) vs. shms focal plane                                                                                        
	  histeDelta_xfp[irun] = new TH2F(Form("eDelta_vs_xfp: Run %d", run_num[irun]), "", 100, 10., 25, 80, -1., 1.);                                   
	  histeDelta_xpfp[irun] = new TH2F(Form("eDelta_vs_xpfp: Run %d", run_num[irun]), "", 100, 0.01, 0.05, 80, -1., 1.);                            
	  histeDelta_yfp[irun] = new TH2F(Form("eDelta_vs_yfp: Run %d", run_num[irun]), "", 100, -10., 5, 80, -1., 1.);                                  
	  histeDelta_ypfp[irun] = new TH2F(Form("eDelta_vs_ypfp: Run %d", run_num[irun]), "", 100, -0.015, 0.015, 80, -1., 1.);

	}                                                                                                                        
                                                                                                                                                     
	if(irun==1){                                                                                                                                
	  histePDev_xfp[irun] = new TH2F(Form("ePDiff_vs_xfp: Run %d", run_num[irun]), "", 100, -30, -5, 80, -0.01, 0.01);          
	  histePDev_xpfp[irun] = new TH2F(Form("ePDiff_vs_xpfp: Run %d", run_num[irun]), "", 100, -0.07, 0.02, 80, -0.01, 0.01);                    
	  histePDev_yfp[irun] = new TH2F(Form("ePDiff_vs_yfp: Run %d", run_num[irun]), "", 100, -25, 15, 80, -0.01, 0.01);                          
	  histePDev_ypfp[irun] = new TH2F(Form("ePDiff_vs_ypfp: Run %d", run_num[irun]), "", 100, -0.04, 0.03, 80, -0.01, 0.01);                    

	  //(shms_delta_calc - shms_delta_meas) vs. shms focal plane                                                                   
	  histeDelta_xfp[irun] = new TH2F(Form("eDelta_vs_xfp: Run %d", run_num[irun]), "", 100, -30., -5, 80, -1., 1.);                   
	  histeDelta_xpfp[irun] = new TH2F(Form("eDelta_vs_xpfp: Run %d", run_num[irun]), "", 100, -0.07, 0.02, 80, -1., 1.);   
	  histeDelta_yfp[irun] = new TH2F(Form("eDelta_vs_yfp: Run %d", run_num[irun]), "", 100, -25., 15, 80, -1., 1.);            
	  histeDelta_ypfp[irun] = new TH2F(Form("eDelta_vs_ypfp: Run %d", run_num[irun]), "", 100, -0.04, 0.03, 80, -1., 1.); 

	}                                                                                                                                              
                                                                                                                                                     
	if(irun==2){                                                                                                                                  
	  histePDev_xfp[irun] = new TH2F(Form("ePDiff_vs_xfp: Run %d", run_num[irun]), "", 100, 5, 15, 80, -0.01, 0.01);                            
	  histePDev_xpfp[irun] = new TH2F(Form("ePDiff_vs_xpfp: Run %d", run_num[irun]), "", 100, 0., 0.04, 80, -0.01, 0.01);                       
	  histePDev_yfp[irun] = new TH2F(Form("ePDiff_vs_yfp: Run %d", run_num[irun]), "", 100, -5, 5, 80, -0.01, 0.01);                            
	  histePDev_ypfp[irun] = new TH2F(Form("ePDiff_vs_ypfp: Run %d", run_num[irun]), "", 100, -0.02, 0.02, 80, -0.01, 0.01);                    
	
	  //(shms_delta_calc - shms_delta_meas) vs. shms focal plane                                                                       
	  histeDelta_xfp[irun] = new TH2F(Form("eDelta_vs_xfp: Run %d", run_num[irun]), "", 100, 5., 15, 80, -1., 1.);                          
	  histeDelta_xpfp[irun] = new TH2F(Form("eDelta_vs_xpfp: Run %d", run_num[irun]), "", 100, 0., 0.04, 80, -1., 1.);            
	  histeDelta_yfp[irun] = new TH2F(Form("eDelta_vs_yfp: Run %d", run_num[irun]), "", 100, -5., 5, 80, -1., 1.);                    
	  histeDelta_ypfp[irun] = new TH2F(Form("eDelta_vs_ypfp: Run %d", run_num[irun]), "", 100, -0.02, 0.02, 80, -1., 1.);

	}                                                                                                                                              
                                                                                                                                                     
	if(irun==0){                                                                                                                                  
	  histePDev_xfp[irun] = new TH2F(Form("ePDiff_vs_xfp: Run %d", run_num[irun]), "", 100, -10., 5, 80, -0.01, 0.01);                           
	  histePDev_xpfp[irun] = new TH2F(Form("ePDiff_vs_xpfp: Run %d", run_num[irun]), "", 100, -0.04, 0.03, 80, -0.01, 0.01);                    
	  histePDev_yfp[irun] = new TH2F(Form("ePDiff_vs_yfp: Run %d", run_num[irun]), "", 100, -10, 5, 80, -0.01, 0.01);                           
	  histePDev_ypfp[irun] = new TH2F(Form("ePDiff_vs_ypfp: Run %d", run_num[irun]), "", 100, -0.02, 0.02, 80, -0.01, 0.01);                    
	
	  //(shms_delta_calc - shms_delta_meas) vs. shms focal plane
	  histeDelta_xfp[irun] = new TH2F(Form("eDelta_vs_xfp: Run %d", run_num[irun]), "", 100, -10., 5, 80, -1., 1.);
	  histeDelta_xpfp[irun] = new TH2F(Form("eDelta_vs_xpfp: Run %d", run_num[irun]), "", 100, -0.04, 0.03, 80, -1., 1.);
	  histeDelta_yfp[irun] = new TH2F(Form("eDelta_vs_yfp: Run %d", run_num[irun]), "", 100, -10., 5, 80, -1., 1.);
	  histeDelta_ypfp[irun] = new TH2F(Form("eDelta_vs_ypfp: Run %d", run_num[irun]), "", 100, -0.02, 0.02, 80, -1., 1.);
	}    
       
	histePDev_xfp[irun]->GetXaxis()->SetTitle("SHMS X_{fp}");
	histePDev_xfp[irun]->GetYaxis()->SetTitle("SHMS [P_{calc} - P_{meas}] / P_{meas}");   
	
	histePDev_xpfp[irun]->GetXaxis()->SetTitle("SHMS X'_{fp}"); 
	histePDev_xpfp[irun]->GetYaxis()->SetTitle("SHMS [P_{calc} - P_{meas}] / P_{meas}"); 
	
	histePDev_yfp[irun]->GetXaxis()->SetTitle("SHMS Y_{fp}");                                                        
	histePDev_yfp[irun]->GetYaxis()->SetTitle("SHMS [P_{calc} - P_{meas}] / P_{meas}");                              
                  
	histePDev_ypfp[irun]->GetXaxis()->SetTitle("SHMS Y'_{fp}");                                                         
	histePDev_ypfp[irun]->GetYaxis()->SetTitle("SHMS [P_{calc} - P_{meas}] / P_{meas}");  

	  //----set names for delta difference histograms
	histeDelta_xfp[irun]->GetXaxis()->SetTitle("SHMS X_{fp}");
	histeDelta_xfp[irun]->GetYaxis()->SetTitle("SHMS #delta_{calc} - #delta_{meas.} [%]");                                             
	
	histeDelta_xpfp[irun]->GetXaxis()->SetTitle("SHMS X'_{fp}");                                                                        
	histeDelta_xpfp[irun]->GetYaxis()->SetTitle("SHMS #delta_{calc} - #delta_{meas.} [%]");  
	
	histeDelta_yfp[irun]->GetXaxis()->SetTitle("SHMS Y_{fp}");                                                       
	histeDelta_yfp[irun]->GetYaxis()->SetTitle("SHMS #delta_{calc} - #delta_{meas.} [%]");                                                 
	
	histeDelta_ypfp[irun]->GetXaxis()->SetTitle("SHMS Y'_{fp}");                                                                         
	histeDelta_ypfp[irun]->GetYaxis()->SetTitle("SHMS #delta_{calc} - #delta_{meas.} [%]");  
      
	//Add the histogram objects to the Obj list
	data_HList[irun].Add(histData_hPcalc[irun]);
	data_HList[irun].Add(histData_hPmeas[irun]);
	data_HList[irun].Add(histData_hPDev[irun]);
	data_HList[irun].Add(histData_Em[irun]);
       	
	data_HList[irun].Add(histhPDev_hxfp[irun]);
	data_HList[irun].Add(histhPDev_hxpfp[irun]);
	data_HList[irun].Add(histhPDev_hyfp[irun]);
	data_HList[irun].Add(histhPDev_hypfp[irun]);


	data_HList[irun].Add(histeDelta_xfp[irun]);
	data_HList[irun].Add(histeDelta_xpfp[irun]);
	data_HList[irun].Add(histeDelta_yfp[irun]);
	data_HList[irun].Add(histeDelta_ypfp[irun]);
	
	data_HList[irun].Add(histePDev_xfp[irun]);
	data_HList[irun].Add(histePDev_xpfp[irun]);  
	data_HList[irun].Add(histePDev_yfp[irun]);                                                    
	data_HList[irun].Add(histePDev_ypfp[irun]); 
      
      	//Define Cuts
	Bool_t data_hmsDelta_cut;
	Bool_t data_shmsDelta_cut;
	Bool_t data_hmsPdiff_cut;
      
      //-----------LOOP OVER ALL ENTRIES IN TREE-----------------------
      //Loop over all entries
      for(int i=0; i<T->GetEntries(); i++){

	T->GetEntry(i);

	//SHMS Particle angles relative to central angle
	htheta_p = xangle*180./TMath::Pi() - ptheta_e;
                                                                                                                                            
        //Calculated electron Momentum (Using formula)                                                                                                 
        shmsP_calc = Eb + Mp - TMath::Sqrt(hms_Pmeas*hms_Pmeas + Mp*Mp); 
	hmsP_calc = 2.*Mp*Eb*(Eb + Mp)*TMath::Cos(htheta_p*TMath::Pi()/180.) / (Mp*Mp + 2.*Mp*Eb + Eb*Eb*TMath::Power(TMath::Sin(htheta_p*TMath::Pi()/180.),2));
	//Calculated shms delta
	shms_delta_calc = (shmsP_calc - eP0[irun]) / eP0[irun] * 100.;
	

	//Define some cuts                                                                                                           
        data_hmsDelta_cut = TMath::Abs(hdelta)<8.0;         
        data_shmsDelta_cut = edelta>-10. && edelta<22.;                                                                      
        data_hmsPdiff_cut = TMath::Abs( (hmsP_calc - hms_Pmeas) ) < 0.02;
	
	if(emiss < 0.03 &&  data_hmsDelta_cut && data_shmsDelta_cut &&  data_hmsPdiff_cut)
	  {


	    //Fill Histograms
	    histData_hPcalc[irun]->Fill(shmsP_calc);
	    histData_hPmeas[irun]->Fill(shms_Pmeas);
	    histData_hPDev[irun]->Fill((shmsP_calc - shms_Pmeas) / shms_Pmeas);

	    histhPDev_hxfp[irun]->Fill(hxfp, (hmsP_calc - hms_Pmeas)/ hms_Pmeas);
	    histhPDev_hxpfp[irun]->Fill(hxpfp, (hmsP_calc - hms_Pmeas)/ hms_Pmeas);  
	    histhPDev_hyfp[irun]->Fill(hyfp, (hmsP_calc - hms_Pmeas)/ hms_Pmeas);
	    histhPDev_hypfp[irun]->Fill(hypfp, (hmsP_calc - hms_Pmeas)/ hms_Pmeas);

	    histePDev_xfp[irun]->Fill(exfp, (shmsP_calc - shms_Pmeas)/ shms_Pmeas);
	    histePDev_xpfp[irun]->Fill(expfp, (shmsP_calc - shms_Pmeas)/ shms_Pmeas);  
	    histePDev_yfp[irun]->Fill(eyfp, (shmsP_calc - shms_Pmeas)/ shms_Pmeas);
	    histePDev_ypfp[irun]->Fill(eypfp, (shmsP_calc - shms_Pmeas)/ shms_Pmeas); 
	  
	    histeDelta_xfp[irun]->Fill(exfp, (shms_delta_calc - edelta));
	    histeDelta_xpfp[irun]->Fill(expfp, (shms_delta_calc - edelta));                                                       
	    histeDelta_yfp[irun]->Fill(eyfp, (shms_delta_calc - edelta));                                                         
	    histeDelta_ypfp[irun]->Fill(eypfp, (shms_delta_calc - edelta)); 

	  

	  }
	//hist[irun][3]->Fill(emiss);   //FIlling Missing Energy Histogram
	histData_Em[irun]->Fill(emiss);
      }
      
      //---------END ENTRY LOOP ---------------------------------------
      //DRaw HIstograms
      c0->cd(irun+1);
      histData_Em[irun]->Draw();
      
      c1->cd(irun+1);
      histData_hPcalc[irun]->Draw();
      histData_hPcalc[irun]->SetLineColor(kRed);
      histData_hPmeas[irun]->Draw("sames");

      
      int bin_max = histData_hPDev[irun]->GetMaximumBin();
      Double_t xmax_val = histData_hPDev[irun]->GetXaxis()->GetBinCenter(bin_max); 
      Double_t sig_Res = histData_hPDev[irun]->GetStdDev();
      
      c2->cd(irun+1);
      histData_hPDev[irun]->Draw();

      c4[irun] = new TCanvas(Form("c4, Run %d", run_num[irun]), Form("Fract. Momentum vs. SHMS Focal Plane, Run %d", run_num[irun]), 1500, 1000);       
      c4[irun]->Divide(2,2);  
      c4[irun]->cd(1);
      histePDev_xfp[irun]->Draw("colz");
      c4[irun]->cd(2); 
      histePDev_xpfp[irun]->Draw("colz");
      c4[irun]->cd(3);
      histePDev_yfp[irun]->Draw("colz"); 
      c4[irun]->cd(4);
      histePDev_ypfp[irun]->Draw("colz");   
      c4[irun]->SaveAs(Form("data_2D_ePdiff_vs_SHMS_FocalPlane_%d.pdf", run_num[irun]));


      c5[irun] = new TCanvas(Form("c5, Run %d", run_num[irun]), "Delta Diff. vs. SHMS Focal Plane", 1500, 1000);                          
      c5[irun]->Divide(2,2);
      c5[irun]->cd(1);
      histeDelta_xfp[irun]->Draw("colz");
      c5[irun]->cd(2);                                                                                                       
      histeDelta_xpfp[irun]->Draw("colz"); 
      c5[irun]->cd(3);                                                                                
      histeDelta_yfp[irun]->Draw("colz");                                                          
      c5[irun]->cd(4);                                                             
      histeDelta_ypfp[irun]->Draw("colz");

      c5[irun]->SaveAs(Form("data_DeltaDiff_vs_SHMS_FocalPlane_%d.pdf", run_num[irun]));   

      
      c6[irun] = new TCanvas(Form("c6, Run %d", run_num[irun]), "HMS Fractional Momentum Diff. vs. Focal Plane", 1500, 1000);                          
      c6[irun]->Divide(2,2);
      c6[irun]->cd(1);
      histhPDev_hxfp[irun]->Draw("colz");
      c6[irun]->cd(2);                                                                                                       
      histhPDev_hxpfp[irun]->Draw("colz"); 
      c6[irun]->cd(3);                                                                                
      histhPDev_hyfp[irun]->Draw("colz");                                                          
      c6[irun]->cd(4);                                                             
      histhPDev_hypfp[irun]->Draw("colz");

      c6[irun]->SaveAs(Form("data_hPDev_vs_HMS_FocalPlane_%d.pdf", run_num[irun]));   


      TF1 *fit = new TF1("fit", "gaus", xmax_val - sig_Res, xmax_val + sig_Res);
      histData_hPDev[irun]->Fit("fit","R");
      
      Double_t mu_Res_fit = fit->GetParameter(1);
      Double_t sig_Res_fit = fit->GetParameter(2);
      Double_t mu_Res_err_fit =  fit->GetParError(1);

      data_xRes_arr[irun] = irun + 1;          //kin group
      data_yRes_arr[irun] = mu_Res_fit;          //mean fit fractional deviation from meas. momentum
      data_yRes_arr_err[irun] =  mu_Res_err_fit;    //error from mean fit

     
      //Write Histograms to a ROOTfile
      TFile *data_outROOT = new TFile(Form("SHMS_heepDATA_histos_%d.root", run[irun]), "recreate");
      data_HList[irun].Write();
      data_outROOT->Close();
      
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
  yRes_diff_err[0] = TMath::Sqrt(data_yRes_arr_err[0]*data_yRes_arr_err[0] + yRes_arr_err[0]*yRes_arr_err[0]);
  yRes_diff_err[1] = TMath::Sqrt(data_yRes_arr_err[1]*data_yRes_arr_err[1] + yRes_arr_err[1]*yRes_arr_err[1]);
  yRes_diff_err[2] = TMath::Sqrt(data_yRes_arr_err[2]*data_yRes_arr_err[2] + yRes_arr_err[2]*yRes_arr_err[2]);
  yRes_diff_err[3] = TMath::Sqrt(data_yRes_arr_err[3]*data_yRes_arr_err[3] + yRes_arr_err[3]*yRes_arr_err[3]);
  yRes_diff_err[4] = TMath::Sqrt(data_yRes_arr_err[4]*data_yRes_arr_err[4] + yRes_arr_err[4]*yRes_arr_err[4]);


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
