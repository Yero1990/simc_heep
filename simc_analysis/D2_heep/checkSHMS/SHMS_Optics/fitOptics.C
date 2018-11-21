//This code will plot (delta_calc - delta_meas) vs. SHMS focal plane quantities. From the plots, we
//can determine whether they are correlated. If they are correlated, a TGraphical (polygon) cut is 
//applied to the correlations, to remove any unnecessary events that may be background or outside of the
//SHMS delta acceptance.

//To set the curts, a separate code, "set_optics_cuts.C" needs to be called. This code will produce root files
//with the necessary cuts stored. These files are then read in this code and the cuts applied inside the entry loop.
//The optics matrix fit is then performed inside the event loop. 


#include "TMath.h"
void fitOptics()
{

  gROOT->SetBatch(kTRUE);
  gStyle->SetOptFit();
  
  //Define some constants
  Double_t Mp = 0.938272;  //proton mass
  Double_t Eb = 10.6005;
  Double_t eP0[5] = {8.5640277, 8.5640277, 8.5640277, 8.5640277, 8.5640277};  //central spec. momentum
  Double_t hP0[5] = {2.9221114, 3.46017618, 2.2997660, 1.8827606, 1.8825118};  //central spec. momentum
  
  
  
  int run_num[5] = {3288, 3371, 3374, 3376, 3377};
  
  int index = 0;
 
  //Create an object array to store histograms
  TObjArray HList[5];

  //Create an object to store polygon exotic cuts around (x,y) correlation plots
  TCutG* xfp_cut[5];

    
 //Define some variables to be determined inside the entry loop
  Double_t theta_p;     //proton arm angle (event by event)  
  Double_t shmsP_calc;   //calculated elecron momentum
  Double_t hmsP_calc;    //calculated proton momentum
  Double_t shms_delta_calc;

 //Define TTree variables names
  TString n_xangle = "H.kin.secondary.xangle";        //opening angle between HMS and SHMS
  TString n_theta_e = "P.kin.primary.scat_ang_deg";   //e- scat angle [deg]
  TString n_shmsP_meas = "P.gtr.p";
  TString n_hmsP_meas = "H.gtr.p";                                                                                                  
  TString n_emiss = "H.kin.secondary.emiss";
  TString n_edelta = "P.gtr.dp";
  TString n_hdelta = "H.gtr.dp";
  //SHMS focal plane quantities
  TString n_exfp = "P.dc.x_fp";
  TString n_expfp = "P.dc.xp_fp"; 
  TString n_eyfp = "P.dc.y_fp";               
  TString n_eypfp = "P.dc.yp_fp"; 

  //Define TTree variables
  Double_t xangle;
  Double_t theta_e;  
  Double_t shmsP_meas;
  Double_t hmsP_meas;                                                                                                                               
  Double_t emiss;
  Double_t edelta;
  Double_t hdelta;

  Double_t exfp, expfp, eyfp, eypfp;


  //-------------CREATE EMPTY HISTOGRAMS---------------------------------------------

  //(shms_delta_calc - shms_delta_meas) vs. SHMS focal plane                                                                         
  TH2F *histeDelta_xfp[5];                                                                            
  TH2F *histeDelta_xpfp[5];                                                                                    
  TH2F *histeDelta_yfp[5];                                                                                                       
  TH2F *histeDelta_ypfp[5];  

  
  //--------------------------------------------------------


  TCanvas *delDiff_vs_FP_Canv[5];

  
  index = 0;
  int run[5] = {3288, 3371, 3374, 3376, 3377};
  //Loop over runs
  for(int irun=0; irun<5; irun++)
    {              
      //Initialize histo object array
      HList[irun](0);
      
      /*
      //Create output root file where histograms will be stored to set cuts
      TString outputcut=Form("SHMS_heepData_histos_%d_cut.root",run[irun]);
      TFile fcut_file(outputcut);
      
      xfp_cut[irun] = (TCutG*)gROOT->FindObject(Form("xfp_cut_%d_%d",0,run[irun]));
      if (xfp_cut[irun]) {
      Int_t npt = xfp_cut[irun]->GetN();
      cout << " xfp cut = " << run[irun] << " npts = " << npt << endl;
      } else {
      cout << "No cut file for run = " <<  run[irun] << endl;
      }
      */
      
      //Open Data File
      string filename = Form("../../../../../hallc_replay/ROOTfiles/coin_replay_heep_check_%d_-1.root",run[irun]);
      TFile *f1 = new TFile(filename.c_str());
      
      //Get TTree
      TTree *T = (TTree*)f1->Get("T");
      
      
      //Set Branch Address
      T->SetBranchAddress(n_xangle, &xangle);
      T->SetBranchAddress(n_theta_e, &theta_e);
      T->SetBranchAddress(n_shmsP_meas, &shmsP_meas);
      T->SetBranchAddress(n_hmsP_meas, &hmsP_meas);                                                
      T->SetBranchAddress(n_emiss, &emiss);
      T->SetBranchAddress(n_edelta, &edelta);
      T->SetBranchAddress(n_hdelta, &hdelta);
      T->SetBranchAddress(n_exfp, &exfp);
      T->SetBranchAddress(n_expfp, &expfp);
      T->SetBranchAddress(n_eyfp, &eyfp);
      T->SetBranchAddress(n_eypfp, &eypfp);
      

      if(index==0){                                                                                                                                  
	//(shms_delta_calc - shms_delta_meas) vs. shms focal plane
	histeDelta_xfp[index] = new TH2F(Form("eDelta_vs_xfp: Run %d", run_num[index]), "", 100, -10., 5, 80, -1., 1.);
	histeDelta_xpfp[index] = new TH2F(Form("eDelta_vs_xpfp: Run %d", run_num[index]), "", 100, -0.04, 0.03, 80, -1., 1.);
	histeDelta_yfp[index] = new TH2F(Form("eDelta_vs_yfp: Run %d", run_num[index]), "", 100, -10., 5, 80, -1., 1.);
	histeDelta_ypfp[index] = new TH2F(Form("eDelta_vs_ypfp: Run %d", run_num[index]), "", 100, -0.02, 0.02, 80, -1., 1.);
      }    
      
      if(index==1){                                                                                                                                
	//(shms_delta_calc - shms_delta_meas) vs. shms focal plane                                                                   
	histeDelta_xfp[index] = new TH2F(Form("eDelta_vs_xfp: Run %d", run_num[index]), "", 100, -30., -5, 80, -1., 1.);                   
	histeDelta_xpfp[index] = new TH2F(Form("eDelta_vs_xpfp: Run %d", run_num[index]), "", 100, -0.07, 0.02, 80, -1., 1.);   
	histeDelta_yfp[index] = new TH2F(Form("eDelta_vs_yfp: Run %d", run_num[index]), "", 100, -25., 15, 80, -1., 1.);            
	histeDelta_ypfp[index] = new TH2F(Form("eDelta_vs_ypfp: Run %d", run_num[index]), "", 100, -0.04, 0.03, 80, -1., 1.); 

      }                                                                                                                                              
      
      if(index==2){                                                                                                                                  	
	//(shms_delta_calc - shms_delta_meas) vs. shms focal plane                                                                       
	histeDelta_xfp[index] = new TH2F(Form("eDelta_vs_xfp: Run %d", run_num[index]), "", 100, 5., 15, 80, -1., 1.);                          
	histeDelta_xpfp[index] = new TH2F(Form("eDelta_vs_xpfp: Run %d", run_num[index]), "", 100, 0., 0.04, 80, -1., 1.);            
	histeDelta_yfp[index] = new TH2F(Form("eDelta_vs_yfp: Run %d", run_num[index]), "", 100, -5., 5, 80, -1., 1.);                    
	histeDelta_ypfp[index] = new TH2F(Form("eDelta_vs_ypfp: Run %d", run_num[index]), "", 100, -0.02, 0.02, 80, -1., 1.);
	
      }                                                                                                                                              
      
      if(index==3 || index==4){                                                                                                         
	//(shms_delta_calc - shms_delta_meas) vs. shms focal plane                                                                                        
	histeDelta_xfp[index] = new TH2F(Form("eDelta_vs_xfp: Run %d", run_num[index]), "", 100, 10., 25, 80, -1., 1.);
	histeDelta_xpfp[index] = new TH2F(Form("eDelta_vs_xpfp: Run %d", run_num[index]), "", 100, 0.01, 0.05, 80, -1., 1.);                            
	histeDelta_yfp[index] = new TH2F(Form("eDelta_vs_yfp: Run %d", run_num[index]), "", 100, -10., 5, 80, -1., 1.);                                  
	histeDelta_ypfp[index] = new TH2F(Form("eDelta_vs_ypfp: Run %d", run_num[index]), "", 100, -0.015, 0.015, 80, -1., 1.);
	
      }  
       
      //----set names for delta difference histograms
      histeDelta_xfp[index]->GetXaxis()->SetTitle("SHMS X_{fp}");
      histeDelta_xfp[index]->GetYaxis()->SetTitle("SHMS #delta_{calc} - #delta_{meas.} [%]");                                             
      
      histeDelta_xpfp[index]->GetXaxis()->SetTitle("SHMS X'_{fp}");                                                                        
      histeDelta_xpfp[index]->GetYaxis()->SetTitle("SHMS #delta_{calc} - #delta_{meas.} [%]");  
      
      histeDelta_yfp[index]->GetXaxis()->SetTitle("SHMS Y_{fp}");                                                       
      histeDelta_yfp[index]->GetYaxis()->SetTitle("SHMS #delta_{calc} - #delta_{meas.} [%]");                                                 
      
      histeDelta_ypfp[index]->GetXaxis()->SetTitle("SHMS Y'_{fp}");                                                                         
      histeDelta_ypfp[index]->GetYaxis()->SetTitle("SHMS #delta_{calc} - #delta_{meas.} [%]");  
      
      //Add the histogram objects to the Obj list
      HList[index].Add(histeDelta_xfp[index]);
      HList[index].Add(histeDelta_xpfp[index]);
      HList[index].Add(histeDelta_yfp[index]);
      HList[index].Add(histeDelta_ypfp[index]);

      
      //Define Cuts
      Bool_t hmsPdiff_cut;
      Bool_t hmsDelta_cut;
      Bool_t shmsDelta_cut;
      Bool_t shms_Xfp_cut;
      
      //-----------LOOP OVER ALL ENTRIES IN TREE-----------------------
      //Loop over all entries
      for(int i=0; i<T->GetEntries(); i++){
	
	T->GetEntry(i);
	
	//HMS Particle angles relative to central angle
	theta_p = xangle*180./TMath::Pi() - theta_e;
        
	//HMS Particle calculated momentum (Using only the particle angle and beam energy)
	hmsP_calc = 2.*Mp*Eb*(Eb + Mp)*TMath::Cos(theta_p*TMath::Pi()/180.) / (Mp*Mp + 2.*Mp*Eb + Eb*Eb*TMath::Power(TMath::Sin(theta_p*TMath::Pi()/180.),2)) ;
	   
	//Calculated electron Momentum (Using formula)                                                                                                 
        shmsP_calc = Eb + Mp - TMath::Sqrt(hmsP_meas*hmsP_meas + Mp*Mp); 
	
	//Calculated shms delta
	shms_delta_calc = (shmsP_calc - eP0[index]) / eP0[index] * 100.;
	

	//Define cuts
	hmsPdiff_cut = 	TMath::Abs(hmsP_calc - hmsP_meas) < 0.02;
	hmsDelta_cut = TMath::Abs(hdelta)<8.0;
	shmsDelta_cut = edelta>-10. && edelta<22.;
	shms_Xfp_cut = 1;

                                                                                                         
  
	//if ( xfp_cut[irun]) shms_Xfp_cut = xfp_cut[irun]->IsInside(exfp, (shms_delta_calc - edelta));
	
	if(emiss < 0.05 &&  hmsPdiff_cut && hmsDelta_cut && shmsDelta_cut && shms_Xfp_cut)
	  {

	    histeDelta_xfp[index]->Fill(exfp, (shms_delta_calc - edelta));
	    histeDelta_xpfp[index]->Fill(expfp, (shms_delta_calc - edelta));                                                       
	    histeDelta_yfp[index]->Fill(eyfp, (shms_delta_calc - edelta));                                                         
	    histeDelta_ypfp[index]->Fill(eypfp, (shms_delta_calc - edelta)); 	  

	  }

      }
      
      //---------END ENTRY LOOP ---------------------------------------
      
      //DRaw HIstograms

      delDiff_vs_FP_Canv[index] = new TCanvas(Form("delDiff_vs_FP_Canv, Run %d", run_num[index]), "Delta Diff. vs. SHMS Focal Plane", 1500, 1000);                          
      delDiff_vs_FP_Canv[index]->Divide(2,2);
      delDiff_vs_FP_Canv[index]->cd(1);
      histeDelta_xfp[index]->Draw("colz");
      delDiff_vs_FP_Canv[index]->cd(2);                                                                                                       
      histeDelta_xpfp[index]->Draw("colz"); 
      delDiff_vs_FP_Canv[index]->cd(3);                                                                                
      histeDelta_yfp[index]->Draw("colz");                                                          
      delDiff_vs_FP_Canv[index]->cd(4);                                                             
      histeDelta_ypfp[index]->Draw("colz");

      delDiff_vs_FP_Canv[index]->SaveAs(Form("data_DeltaDiff_vs_SHMS_FocalPlane_%d.pdf", run_num[index]));   


      
      //Write Histograms to a ROOTfile
      TFile *outROOT = new TFile(Form("SHMS_heepData_histos_%d.root", run[irun]), "recreate");
      HList[index].Write();
      outROOT->Close();
      index++;

      } //end loop over runs 
			       
			     
}
