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
  int num_recon_terms = 0;
  
  //Open sample data cointaining the structure of the terms (1000, 01000, 00100, etc.)
  ifstream infile("sample_optics.dat");
  
  int nfit_max=500000;
  int nfit = 0;
 
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
    
    num_recon_terms++;
  }
  
  cout<<"recon terms: "<<num_recon_terms<<endl;
  TMatrixD Ay(num_recon_terms,num_recon_terms);
  TMatrixD lambda(num_recon_terms,nfit_max);
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
  TH2F *histeDelta_xfp;                                                                            
  TH2F *histeDelta_xpfp;                                                                                    
  TH2F *histeDelta_yfp;                                                                                                       
  TH2F *histeDelta_ypfp;  

  
  //--------------------------------------------------------


  TCanvas *delDiff_vs_FP_Canv[5];

  
  index = 0;
  int run[5] = {3288, 3371, 3374, 3376, 3377};
  //Loop over runs
  for(int irun=0; irun<5; irun++)
    {              
      //Initialize histo object array
      HList[irun](0);
      
      
      //Create output root file where histograms will be stored to set cuts
      //TString outputcut=Form("SHMS_heepData_histos_%d_cut.root",run[irun]);
      //TFile fcut_file(outputcut);
      
      //xfp_cut[irun] = (TCutG*)gROOT->FindObject(Form("xfp_cut_%d_%d",0,run[irun]));
      //if (xfp_cut[irun]) {
      //Int_t npt = xfp_cut[irun]->GetN();
      //cout << " xfp cut = " << run[irun] << " npts = " << npt << endl;
      //} else {
      //cout << "No cut file for run = " <<  run[irun] << endl;
    }
      
      
      //Open Data File
      string filename = "../../../../../hallc_replay/ROOTfiles/coin_replay_heep_check_combined.root";
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
                                                                                                                             
      //(shms_delta_calc - shms_delta_meas) vs. shms focal plane
      histeDelta_xfp = new TH2F("eDelta_vs_xfp", "", 100, -30., 30, 80, -1., 1.);
      histeDelta_xpfp = new TH2F("eDelta_vs_xpfp", "", 100, -0.1, 0.1, 80, -1., 1.);
      histeDelta_yfp = new TH2F("eDelta_vs_yfp", "", 100, -30., 30, 80, -1., 1.);
      histeDelta_ypfp = new TH2F("eDelta_vs_ypfp", "", 100, -0.1, 0.1, 80, -1., 1.);

       
      //----set names for delta difference histograms
      histeDelta_xfp->GetXaxis()->SetTitle("SHMS X_{fp}");
      histeDelta_xfp->GetYaxis()->SetTitle("SHMS #delta_{calc} - #delta_{meas.} [%]");                                             
      
      histeDelta_xpfp->GetXaxis()->SetTitle("SHMS X'_{fp}");                                                                        
      histeDelta_xpfp->GetYaxis()->SetTitle("SHMS #delta_{calc} - #delta_{meas.} [%]");  
      
      histeDelta_yfp->GetXaxis()->SetTitle("SHMS Y_{fp}");                                                       
      histeDelta_yfp->GetYaxis()->SetTitle("SHMS #delta_{calc} - #delta_{meas.} [%]");                                                 
      
      histeDelta_ypfp->GetXaxis()->SetTitle("SHMS Y'_{fp}");                                                                         
      histeDelta_ypfp->GetYaxis()->SetTitle("SHMS #delta_{calc} - #delta_{meas.} [%]");  
      
      //Add the histogram objects to the Obj list
      HList.Add(histeDelta_xfp);
      HList.Add(histeDelta_xpfp);
      HList.Add(histeDelta_yfp);
      HList.Add(histeDelta_ypfp);

      
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
	shms_delta_calc = (shmsP_calc - eP0) / eP0 * 100.;
	

	//Define cuts
	hmsPdiff_cut = 	TMath::Abs(hmsP_calc - hmsP_meas) < 0.02;
	hmsDelta_cut = TMath::Abs(hdelta)<8.0;
	shmsDelta_cut = edelta>-10. && edelta<22.;
	shms_Xfp_cut = 1;

                                                                                                         
  
	//if ( xfp_cut[irun]) shms_Xfp_cut = xfp_cut[irun]->IsInside(exfp, (shms_delta_calc - edelta));
	
	if(emiss < 0.05 &&  hmsPdiff_cut && hmsDelta_cut && shmsDelta_cut && shms_Xfp_cut)
	  {
	    //
		Double_t etemp;
		for( int icoeff_fit=0; icoeff_fit<num_recon_terms; icoeff_fit++ ){
		  etemp= 
		    pow( exfp / 100.0, xfpexpon[icoeff_fit] ) * 
		    pow( eyfp / 100.0, yfpexpon[icoeff_fit] ) * 
		    pow( expfp, xpfpexpon[icoeff_fit] ) * 
		    pow( eypfp, ypfpexpon[icoeff_fit] ) ;
		  if (nfit<nfit_max){
		    lambda[icoeff_fit][nfit] = etemp;
		    b_delta[icoeff_fit] += (shms_delta_calc - edelta) * etemp/100.;
		  }
		}
		  if (nfit<nfit_max)  nfit++;
	    //
	    histeDelta_xfp->Fill(exfp, (shms_delta_calc - edelta));
	    histeDelta_xpfp->Fill(expfp, (shms_delta_calc - edelta));                                                       
	    histeDelta_yfp->Fill(eyfp, (shms_delta_calc - edelta));                                                         
	    histeDelta_ypfp->Fill(eypfp, (shms_delta_calc - edelta)); 	  

	  }

      }
      
      //---------END ENTRY LOOP ---------------------------------------
     
      //DRaw HIstograms

      delDiff_vs_FP_Canv = new TCanvas(Form("delDiff_vs_FP_Canv, Run %d", run_num), "Delta Diff. vs. SHMS Focal Plane", 1500, 1000);                          
      delDiff_vs_FP_Canv->Divide(2,2);
      delDiff_vs_FP_Canv->cd(1);
      histeDelta_xfp->Draw("colz");
      delDiff_vs_FP_Canv->cd(2);                                                                                                       
      histeDelta_xpfp->Draw("colz"); 
      delDiff_vs_FP_Canv->cd(3);                                                                                
      histeDelta_yfp->Draw("colz");                                                          
      delDiff_vs_FP_Canv->cd(4);                                                             
      histeDelta_ypfp->Draw("colz");

      delDiff_vs_FP_Canv->SaveAs(Form("data_DeltaDiff_vs_SHMS_FocalPlane_%d.pdf", run_num));   


      
      //Write Histograms to a ROOTfile
      TFile *outROOT = new TFile(Form("combinedSHMS_heepData_histos_%d.root", run[irun]), "recreate");
      HList.Write();
      outROOT->Close();
      index++;
			       
			       
    if (nfit!=0){
      for(int i=0; i<num_recon_terms; i++){
	for(int j=0; j<num_recon_terms; j++){
	  Ay[i][j] = 0.0;
	}
      }
      for( int ifit=0; ifit<nfit; ifit++){
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

      }
    }
 			     
}
