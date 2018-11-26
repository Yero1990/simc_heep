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
  
  //Delta Corrections output text file
  ofstream fCorr;  
  fCorr.open("corrections.txt");

  //Create Vectors to store focal plane quantities
  vector<int> xfpexpon;
  vector<int> xpfpexpon;
  vector<int> yfpexpon;
  vector<int> ypfpexpon;
  vector<int> xtarexpon;

  TString currentline;
  int num_recon_terms = 0;   //5 reconstructed terms
  
  //Open sample data cointaining the structure of the terms (1000, 01000, 00100, etc.)
  ifstream infile("sample_optics.dat");
  
  int nfit_max=100000;      //maximum number of events in fit per run
  int nfit_max_total = 100000;
  
 
  //int nfit_total = 0;
  //read in the infile to get the number of terms we will extract for fitting
  while( currentline.ReadLine(infile,kFALSE)){
    
    int expontemp[5];
    
    cout << "current_line = " << currentline << endl;

    //Read each of the 5 terms starting at marker 66, in each line of sample_optics.dat
    for(int expon=0; expon<5; expon++){
      
      TString stemp(currentline(66+expon,1));
      expontemp[expon] = stemp.Atoi();
      
    }  
    
    xfpexpon.push_back(expontemp[0]);
    xpfpexpon.push_back(expontemp[1]);
    yfpexpon.push_back(expontemp[2]);
    ypfpexpon.push_back(expontemp[3]);
    xtarexpon.push_back(expontemp[4]);
    cout<<"Terms: "<<expontemp[0]<<expontemp[1]<<expontemp[2]<<expontemp[3]<<expontemp[4]<<endl;
    //    cout << "xfpexpon_vec = " << display(xfpexpon) << endl;
    num_recon_terms++;
  }
  
                                                                                   

  cout<<"recon terms: "<<num_recon_terms<<endl;
  TMatrixD Ay(num_recon_terms,num_recon_terms);
  TMatrixD lambda(num_recon_terms,nfit_max_total);
  TVectorD b_delta(num_recon_terms);
  TMatrixD By(num_recon_terms,num_recon_terms);
  TMatrixD lambda2(num_recon_terms,nfit_max);
  TVectorD b_yptar(num_recon_terms);
  TMatrixD Cy(num_recon_terms,num_recon_terms);
  TMatrixD lambda3(num_recon_terms,nfit_max);
  TVectorD b_ytar(num_recon_terms);
 
  
  
  //Define some constants
  Double_t Mp = 0.938272;  //proton mass
  Double_t Eb = 10.6005;
  Double_t eP0[5] = {8.5640277, 8.5640277, 8.5640277, 8.5640277, 8.5640277};  //central spec. momentum
  Double_t hP0[5] = {2.9221114, 3.46017618, 2.2997660, 1.8827606, 1.8825118};  //central spec. momentum
  
  
  
  int run[5] = {3288, 3371, 3374, 3376, 3377};
  
  
 
  //Create an object array to store histograms
  TObjArray HList[5];

  //Create an object to store polygon exotic cuts around (x,y) correlation plots
  TCutG* xfp_cut[5];
  TCutG* xpfp_cut[5];
  TCutG* yfp_cut[5];
  TCutG* ypfp_cut[5];

    
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
  
  //(shms_delta_calc - shms_delta_meas) vs. SHMS focal plane, with Graphical CUts                                                                                                                   
  TH2F *histeDelta_xfp_cut[5];                                                                               
  TH2F *histeDelta_xpfp_cut[5];                                                                                                                                    
  TH2F *histeDelta_yfp_cut[5];
  TH2F *histeDelta_ypfp_cut[5];  
  //--------------------------------------------------------
  
  
  TCanvas *delDiff_vs_FP_Canv[5];
  TCanvas *delDiff_vs_FPCut_Canv[5];
  
  
 


  int nfit;
  //Reset counter over all events over all runs
  int nfit_total = 0; 


  //Loop over runs
  for(int irun=0; irun<5; irun++)
    {              

      if(irun!=0) continue;
      //      cout << "**** Run ***** : " << run[irun] << endl;
      //cout << ">>>> Resset Counter: " << endl;
      //nfit = 0;
      
      //Initialize histo object array
      HList[irun](0);
      
      
      //----------SET THE Focal Plane Graphical CUTS per Run ---------------
      
      //Read in the cuts file produced by set_cut.C
      TString cutfilename=Form("SHMS_heepData_histos_%d_cut.root",run[irun]);
      TFile fCut_file(cutfilename);
      
      xfp_cut[irun] = (TCutG*)gROOT->FindObject(Form("eDelta_vs_xfp_cut: Run %d",run[irun]));
      xpfp_cut[irun] = (TCutG*)gROOT->FindObject(Form("eDelta_vs_xpfp_cut: Run %d",run[irun]));                                
      yfp_cut[irun] = (TCutG*)gROOT->FindObject(Form("eDelta_vs_yfp_cut: Run %d",run[irun]));                                                  
      ypfp_cut[irun] = (TCutG*)gROOT->FindObject(Form("eDelta_vs_ypfp_cut: Run %d",run[irun]));  
                                                                


      //if (xfp_cut[irun]) {
      //Int_t npt = xfp_cut[irun]->GetN();
      //cout << " xfp cut = " << run[irun] << " npts = " << npt << endl;
      //} else {
      //cout << "No cut file for run = " <<  run[irun] << endl;
    
      
      	
	

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
      

      if(irun==0){                                                                                                                                  
	//(shms_delta_calc - shms_delta_meas) vs. shms focal plane
	histeDelta_xfp[irun] = new TH2F(Form("eDelta_vs_xfp: Run %d", run[irun]), "", 100, -10., 5, 80, -1., 1.);
	histeDelta_xpfp[irun] = new TH2F(Form("eDelta_vs_xpfp: Run %d", run[irun]), "", 100, -0.04, 0.03, 80, -1., 1.);
	histeDelta_yfp[irun] = new TH2F(Form("eDelta_vs_yfp: Run %d", run[irun]), "", 100, -10., 5, 80, -1., 1.);
	histeDelta_ypfp[irun] = new TH2F(Form("eDelta_vs_ypfp: Run %d", run[irun]), "", 100, -0.02, 0.02, 80, -1., 1.);
	
	histeDelta_xfp_cut[irun] = new TH2F(Form("eDelta_vs_xfp_cut: Run %d", run[irun]), "", 100, -10., 5, 80, -1., 1.);  
        histeDelta_xpfp_cut[irun] = new TH2F(Form("eDelta_vs_xpfp_cut: Run %d", run[irun]), "", 100, -0.04, 0.03, 80, -1., 1.);           
        histeDelta_yfp_cut[irun] = new TH2F(Form("eDelta_vs_yfp_cut: Run %d", run[irun]), "", 100, -10., 5, 80, -1., 1.);                                
        histeDelta_ypfp_cut[irun] = new TH2F(Form("eDelta_vs_ypfp_cut: Run %d", run[irun]), "", 100, -0.02, 0.02, 80, -1., 1.);    

      }    
      
      if(irun==1){                                                                                                                                
	//(shms_delta_calc - shms_delta_meas) vs. shms focal plane                                                                   
	histeDelta_xfp[irun] = new TH2F(Form("eDelta_vs_xfp: Run %d", run[irun]), "", 100, -30., -5, 80, -1., 1.);                   
	histeDelta_xpfp[irun] = new TH2F(Form("eDelta_vs_xpfp: Run %d", run[irun]), "", 100, -0.07, 0.02, 80, -1., 1.);   
	histeDelta_yfp[irun] = new TH2F(Form("eDelta_vs_yfp: Run %d", run[irun]), "", 100, -25., 15, 80, -1., 1.);            
	histeDelta_ypfp[irun] = new TH2F(Form("eDelta_vs_ypfp: Run %d", run[irun]), "", 100, -0.04, 0.03, 80, -1., 1.); 


        histeDelta_xfp_cut[irun] = new TH2F(Form("eDelta_vs_xfp_cut: Run %d", run[irun]), "", 100, -30., -5, 80, -1., 1.);                
        histeDelta_xpfp_cut[irun] = new TH2F(Form("eDelta_vs_xpfp_cut: Run %d", run[irun]), "", 100, -0.07, 0.02, 80, -1., 1.);                                
        histeDelta_yfp_cut[irun] = new TH2F(Form("eDelta_vs_yfp_cut: Run %d", run[irun]), "", 100, -25., 15, 80, -1., 1.);                                                          
        histeDelta_ypfp_cut[irun] = new TH2F(Form("eDelta_vs_ypfp_cut: Run %d", run[irun]), "", 100, -0.04, 0.03, 80, -1., 1.); 

      }                                                                                                                                              

      
      if(irun==2){                                                                                                                                  	
	//(shms_delta_calc - shms_delta_meas) vs. shms focal plane                                                                       
	histeDelta_xfp[irun] = new TH2F(Form("eDelta_vs_xfp: Run %d", run[irun]), "", 100, 5., 15, 80, -1., 1.);                          
	histeDelta_xpfp[irun] = new TH2F(Form("eDelta_vs_xpfp: Run %d", run[irun]), "", 100, 0., 0.04, 80, -1., 1.);            
	histeDelta_yfp[irun] = new TH2F(Form("eDelta_vs_yfp: Run %d", run[irun]), "", 100, -5., 5, 80, -1., 1.);                    
	histeDelta_ypfp[irun] = new TH2F(Form("eDelta_vs_ypfp: Run %d", run[irun]), "", 100, -0.02, 0.02, 80, -1., 1.);
	
	histeDelta_xfp_cut[irun] = new TH2F(Form("eDelta_vs_xfp_cut: Run %d", run[irun]), "", 100, 5., 15, 80, -1., 1.); 
        histeDelta_xpfp_cut[irun] = new TH2F(Form("eDelta_vs_xpfp_cut: Run %d", run[irun]), "", 100, 0., 0.04, 80, -1., 1.);                                               
        histeDelta_yfp_cut[irun] = new TH2F(Form("eDelta_vs_yfp_cut: Run %d", run[irun]), "", 100, -5., 5, 80, -1., 1.);                                                   
        histeDelta_ypfp_cut[irun] = new TH2F(Form("eDelta_vs_ypfp_cut: Run %d", run[irun]), "", 100, -0.02, 0.02, 80, -1., 1.); 
      }                                                                                                                                              
      
      if(irun==3 || irun==4){                                                                                                         
	//(shms_delta_calc - shms_delta_meas) vs. shms focal plane                                                                                        
	histeDelta_xfp[irun] = new TH2F(Form("eDelta_vs_xfp: Run %d", run[irun]), "", 100, 10., 25, 80, -1., 1.);
	histeDelta_xpfp[irun] = new TH2F(Form("eDelta_vs_xpfp: Run %d", run[irun]), "", 100, 0.01, 0.05, 80, -1., 1.);                            
	histeDelta_yfp[irun] = new TH2F(Form("eDelta_vs_yfp: Run %d", run[irun]), "", 100, -10., 5, 80, -1., 1.);                                  
	histeDelta_ypfp[irun] = new TH2F(Form("eDelta_vs_ypfp: Run %d", run[irun]), "", 100, -0.015, 0.015, 80, -1., 1.);


        histeDelta_xfp_cut[irun] = new TH2F(Form("eDelta_vs_xfp_cut: Run %d", run[irun]), "", 100, 10., 25, 80, -1., 1.); 
        histeDelta_xpfp_cut[irun] = new TH2F(Form("eDelta_vs_xpfp_cut: Run %d", run[irun]), "", 100, 0.01, 0.05, 80, -1., 1.);  
        histeDelta_yfp_cut[irun] = new TH2F(Form("eDelta_vs_yfp_cut: Run %d", run[irun]), "", 100, -10., 5, 80, -1., 1.);   
        histeDelta_ypfp_cut[irun] = new TH2F(Form("eDelta_vs_ypfp_cut: Run %d", run[irun]), "", 100, -0.015, 0.015, 80, -1., 1.); 	
      }  
       
      //----set names for delta difference histograms
      histeDelta_xfp[irun]->GetXaxis()->SetTitle("SHMS X_{fp}");
      histeDelta_xfp[irun]->GetYaxis()->SetTitle("SHMS #delta_{calc} - #delta_{meas.} [%]");                                             
      
      histeDelta_xpfp[irun]->GetXaxis()->SetTitle("SHMS X'_{fp}");                                                                        
      histeDelta_xpfp[irun]->GetYaxis()->SetTitle("SHMS #delta_{calc} - #delta_{meas.} [%]");  
      
      histeDelta_yfp[irun]->GetXaxis()->SetTitle("SHMS Y_{fp}");                                                       
      histeDelta_yfp[irun]->GetYaxis()->SetTitle("SHMS #delta_{calc} - #delta_{meas.} [%]");                                                 
      
      histeDelta_ypfp[irun]->GetXaxis()->SetTitle("SHMS Y'_{fp}");                                                                         
      histeDelta_ypfp[irun]->GetYaxis()->SetTitle("SHMS #delta_{calc} - #delta_{meas.} [%]"); 

      histeDelta_xfp_cut[irun]->GetXaxis()->SetTitle("SHMS X_{fp}");                 
      histeDelta_xfp_cut[irun]->GetYaxis()->SetTitle("SHMS #delta_{calc} - #delta_{meas.} [%]");                                                                         
                                                                                                                                                                                              
      histeDelta_xpfp_cut[irun]->GetXaxis()->SetTitle("SHMS X'_{fp}");                                                                                      
      histeDelta_xpfp_cut[irun]->GetYaxis()->SetTitle("SHMS #delta_{calc} - #delta_{meas.} [%]");                                               
                                                                                                                                                                                                  
      histeDelta_yfp_cut[irun]->GetXaxis()->SetTitle("SHMS Y_{fp}");                                                                                                              
      histeDelta_yfp_cut[irun]->GetYaxis()->SetTitle("SHMS #delta_{calc} - #delta_{meas.} [%]");                                  
                                                                                                                                  
      histeDelta_ypfp_cut[irun]->GetXaxis()->SetTitle("SHMS Y'_{fp}");                                                                                                                    
      histeDelta_ypfp_cut[irun]->GetYaxis()->SetTitle("SHMS #delta_{calc} - #delta_{meas.} [%]");         

      //Add the histogram objects to the Obj list
      HList[irun].Add(histeDelta_xfp[irun]);
      HList[irun].Add(histeDelta_xpfp[irun]);
      HList[irun].Add(histeDelta_yfp[irun]);
      HList[irun].Add(histeDelta_ypfp[irun]);

      HList[irun].Add(histeDelta_xfp_cut[irun]);
      HList[irun].Add(histeDelta_xpfp_cut[irun]);  
      HList[irun].Add(histeDelta_yfp_cut[irun]);                                                    
      HList[irun].Add(histeDelta_ypfp_cut[irun]); 

      
      //Define Cuts
      Bool_t hmsPdiff_cut;
      Bool_t hmsDelta_cut;
      Bool_t shmsDelta_cut;
      
      //Graphical Polygon CUts
      Bool_t shms_Xfp_cut;
      Bool_t shms_Xpfp_cut;
      Bool_t shms_Yfp_cut;
      Bool_t shms_Ypfp_cut;

      cout << "**** Run ***** : " << run[irun] << endl;                                                                 
      cout << ">>>> Resset Counter: " << endl;                                                                                                                                   
      nfit = 0; 

 
      cout << "Starting LOOP: >>>>>>>> NFIT = " << nfit << endl;

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
	shms_delta_calc = (shmsP_calc - eP0[irun]) / eP0[irun] * 100.;
	

	//Define cuts
	hmsPdiff_cut = 	TMath::Abs(hmsP_calc - hmsP_meas) < 0.02;
	hmsDelta_cut = TMath::Abs(hdelta)<8.0;
	shmsDelta_cut = edelta>-10. && edelta<22.;
	shms_Xfp_cut = 0;
	shms_Xpfp_cut = 0;
	shms_Yfp_cut = 0;
	shms_Ypfp_cut = 0;
                                                                                                         
  
	if (xfp_cut[irun]) { shms_Xfp_cut = xfp_cut[irun]->IsInside(exfp, (shms_delta_calc - edelta)); }
	if (xpfp_cut[irun]) { shms_Xpfp_cut = xpfp_cut[irun]->IsInside(expfp, (shms_delta_calc - edelta)); }     
        if (yfp_cut[irun]) { shms_Yfp_cut = yfp_cut[irun]->IsInside(eyfp, (shms_delta_calc - edelta)); }                                    
        if (ypfp_cut[irun]) { shms_Ypfp_cut = ypfp_cut[irun]->IsInside(eypfp, (shms_delta_calc - edelta)); }   
                                                                

	
	//Apply kinematic cuts
	if(emiss < 0.05 &&  hmsPdiff_cut && hmsDelta_cut && shmsDelta_cut)
	  {
	    
	    //Apply Graphical Cuts on SHMS Focal Plane
	    if(shms_Xfp_cut && shms_Xpfp_cut && shms_Yfp_cut && shms_Ypfp_cut){
	      
	    
	            
	      //Fill Histograms deltaDiff vs. SHMS Focal Plane (with Graphical Cuts)
	      histeDelta_xfp_cut[irun]->Fill(exfp, (shms_delta_calc - edelta));         
	      histeDelta_xpfp_cut[irun]->Fill(expfp, (shms_delta_calc - edelta));     
	      histeDelta_yfp_cut[irun]->Fill(eyfp, (shms_delta_calc - edelta));                                                        
	      histeDelta_ypfp_cut[irun]->Fill(eypfp, (shms_delta_calc - edelta));  

	      
	      //	      while(nfit<nfit_max && nfit_total<nfit_max_total)
	      //	{

	      
		  Double_t etemp;
		  for( int icoeff_fit=0; icoeff_fit<num_recon_terms; icoeff_fit++ ){
		    etemp= 
		      pow( exfp / 100.0, xfpexpon[icoeff_fit] ) * 
		      pow( eyfp / 100.0, yfpexpon[icoeff_fit] ) * 
		      pow( expfp, xpfpexpon[icoeff_fit] ) * 
		      pow( eypfp, ypfpexpon[icoeff_fit] ) ;
		    		      
		      //cout << "Run :" << run[irun] << "nfit: " << nfit << " nfit_total: " << nfit_total << endl;
		    if(nfit<nfit_max){
		    lambda[icoeff_fit][nfit_total] = etemp;
		      b_delta[icoeff_fit] += (shms_delta_calc - edelta) * etemp/100.;
		    }
		    
		  }// end loop over icoeff_fit (xfp, x'fp, yfp, y'fp, xtar)
		  
		  if(nfit<nfit_max){
		  nfit++;    //increment event per run 
		  cout << "# events per run = " << nfit << endl;
		  }
		  if(nfit_total<nfit_max_total){
		  nfit_total++;  //INCREMENTS number of events over all runs
		  cout << "total # evts = " << nfit_total << endl;
		  }
		  //if(nfit==nfit_max){
		  // cout << "Reached the max number of evts " << endl;
		    
		    
		    
		     
		    
		  
		  //}//end while loop
	    
		
	      
		  } //end graphical cut
	    
	    
       
	  
	    
	    //Fill deltaDiff vs. SHMS focal plane (no Graphical Cuts)
	    histeDelta_xfp[irun]->Fill(exfp, (shms_delta_calc - edelta));
	    histeDelta_xpfp[irun]->Fill(expfp, (shms_delta_calc - edelta));                                                       
	    histeDelta_yfp[irun]->Fill(eyfp, (shms_delta_calc - edelta));                                                         
	    histeDelta_ypfp[irun]->Fill(eypfp, (shms_delta_calc - edelta)); 	  
	    
	  } //end kinematics cuts
	
      } //end entry loop
	
      //---------END ENTRY LOOP ---------------------------------------
     
	  

      //DRaw HIstograms

      delDiff_vs_FP_Canv[irun] = new TCanvas(Form("delDiff_vs_FP_Canv, Run %d", run[irun]), "Delta Diff. vs. SHMS Focal Plane", 1500, 1000);                          
      delDiff_vs_FP_Canv[irun]->Divide(2,2);
      delDiff_vs_FP_Canv[irun]->cd(1);
      histeDelta_xfp[irun]->Draw("colz");
      delDiff_vs_FP_Canv[irun]->cd(2);                                                                                                       
      histeDelta_xpfp[irun]->Draw("colz"); 
      delDiff_vs_FP_Canv[irun]->cd(3);                                                                                
      histeDelta_yfp[irun]->Draw("colz");                                                          
      delDiff_vs_FP_Canv[irun]->cd(4);                                                             
      histeDelta_ypfp[irun]->Draw("colz");

      delDiff_vs_FP_Canv[irun]->SaveAs(Form("data_DeltaDiff_vs_SHMS_FocalPlane_%d.pdf", run[irun]));

      delDiff_vs_FPCut_Canv[irun] = new TCanvas(Form("delDiff_vs_FPCut_Canv, Run %d", run[irun]), "Delta Diff. vs. SHMS Focal Plane", 1500, 1000);                      
      delDiff_vs_FPCut_Canv[irun]->Divide(2,2);                                                                                                                         
      delDiff_vs_FPCut_Canv[irun]->cd(1);                                                                                                                               
      histeDelta_xfp_cut[irun]->Draw("colz");                                                                                                               
      delDiff_vs_FPCut_Canv[irun]->cd(2);                                               
      histeDelta_xpfp_cut[irun]->Draw("colz");                                                                                                                        
      delDiff_vs_FPCut_Canv[irun]->cd(3);                                                                                                                                     
      histeDelta_yfp_cut[irun]->Draw("colz");                                                                                                             
      delDiff_vs_FPCut_Canv[irun]->cd(4);                                                                                                                                                                  
      histeDelta_ypfp_cut[irun]->Draw("colz");                                                                                                                                                                                                                                                                                                                                                                     
      delDiff_vs_FPCut_Canv[irun]->SaveAs(Form("data_DeltaDiff_vs_SHMS_FocalPlane_CUTS_%d.pdf", run[irun]));                                                  
      

      
      //Write Histograms to a ROOTfile
      TFile *outROOT = new TFile(Form("SHMS_heepData_histos_%d.root", run[irun]), "recreate");
      HList[irun].Write();
      outROOT->Close();
      
     
	
      
			       
    } //end run loop

      
  
  if (nfit!=0){
    for(int i=0; i<num_recon_terms; i++){
      for(int j=0; j<num_recon_terms; j++){
	Ay[i][j] = 0.0;
      }
    }
    
    //Get Ay to be a square 5x5 matrix 
    for( int ifit=0; ifit<nfit_total; ifit++){
      if( ifit % 100000 == 0 ) cout << ifit << endl;
      for( int ipar=0; ipar<num_recon_terms; ipar++){
	for( int jpar=0; jpar<num_recon_terms; jpar++){
	  Ay[ipar][jpar] += lambda[ipar][ifit] * lambda[jpar][ifit];
	}
      }
    }
    
      TDecompSVD Ay_svd(Ay);
      bool ok;
      ok = Ay_svd.Solve( b_delta );
      cout << "delta solution ok = " << ok << endl;
      b_delta.Print();
      char coeffstring[100];
      Double_t tt;
      fCorr<<"delta correction"<<endl;

      for( int icoeff_fit=0; icoeff_fit<num_recon_terms; icoeff_fit++ ){
        cout<< " ";
	tt=b_delta[icoeff_fit];
	sprintf( coeffstring, "%16.9g", tt );
	cout << coeffstring; 
	fCorr<< coeffstring;

	cout << "      "<< xfpexpon[icoeff_fit]; 
	cout << xpfpexpon[icoeff_fit]; 
	cout << yfpexpon[icoeff_fit]; 
	cout << ypfpexpon[icoeff_fit]; 
	cout << xtarexpon[icoeff_fit]; 
	cout << endl;

	fCorr << "      "<< xfpexpon[icoeff_fit]; 
	fCorr << xpfpexpon[icoeff_fit]; 
	fCorr << yfpexpon[icoeff_fit]; 
	fCorr << ypfpexpon[icoeff_fit]; 
	fCorr << xtarexpon[icoeff_fit]; 
	fCorr << endl;

      } //end loop to write matrix to file

    } //end nfit!=0 requirement
      
	
      }
