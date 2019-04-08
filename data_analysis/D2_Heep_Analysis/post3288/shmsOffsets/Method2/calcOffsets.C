#include "TMath.h"
//Code to calculate electron arm angle offsets from the formula:
// Eb = Pp * cos(th_p) + E'*cos(th_e_calc)

void calcOffsets()
{
    
  gROOT->SetBatch(kTRUE);
  gStyle->SetOptFit();


  TObjArray data_HList[4]; 

  //Define some variables to be determined inside the entry loop
  Double_t htheta_p;     //proton arm angle (event by event)

  Double_t Eb = 10.6005;
  Double_t dtr = TMath::Pi()/180.;
  //Define TTree variables
  TString n_xangle = "H.kin.secondary.xangle";        //opening angle between HMS and SHMS
  TString n_theta_e = "P.kin.primary.scat_ang_deg";   //e- scat angle [deg]
  TString n_shmsP_meas = "P.gtr.p";
  TString n_hmsP_meas = "H.gtr.p";                                                                                                  


  Double_t xangle;
  Double_t ptheta_e;   //e- scat angle [rad]
  Double_t shms_Pmeas;
  Double_t hms_Pmeas;                                                                                                                   
  
            
  Double_t emiss;
  Double_t hdelta;
  Double_t edelta;
  Double_t epCoinTime;
  Double_t ptheta_e_calc;  //calculate electron angle (to be determined in entry loop)
  Double_t pcal_etottracknorm;   


  //CREATE EMPTY HISTOGRAMS
  TH1F *histAngDiff[4];

  //Create TCanvas
  TCanvas *c0 = new TCanvas("c0", "SHMS Angle Offsets", 1900, 1500);   
  c0->Divide(2,2);
 
  int run[4] = {3288, 3371, 3374, 3377};

  //Loop over runs
  for(int irun=0; irun<4; irun++)
    {                                 
    
      //Initialize histo object array
      data_HList[irun](0);
    	
      string filename = Form("../../../../../hallc_replay/ROOTfiles/coin_replay_heep_check_%d_-1.root",run[irun]);
      TFile *f1 = new TFile(filename.c_str());

      //Get TTree
      TTree *T = (TTree*)f1->Get("T");
	
      //Set Branch Address
      T->SetBranchAddress(n_xangle, &xangle);
      T->SetBranchAddress(n_theta_e, &ptheta_e);
      T->SetBranchAddress(n_shmsP_meas, &shms_Pmeas);
      T->SetBranchAddress(n_hmsP_meas, &hms_Pmeas);                                                
      T->SetBranchAddress("H.kin.secondary.emiss", &emiss);
      
      T->SetBranchAddress("H.gtr.dp", &hdelta);
      T->SetBranchAddress("P.gtr.dp", &edelta);
      T->SetBranchAddress("CTime.epCoinTime_ROC2", &epCoinTime); 
      T->SetBranchAddress("P.cal.etottracknorm",&pcal_etottracknorm);                                                                                         


      //Histogram
      histAngDiff[irun] = new TH1F(Form("data_AngDiff_Run%d",run[irun]), Form("Run %d: SHMS (#theta_{e, calc} - #theta_{e,meas})", run[irun]), 100, -0.04, 0.04);
	
      //Add the histogram objects to the Obj list
      data_HList[irun].Add(histAngDiff[irun]);


      //Define Boolean for Cuts
      Bool_t c_Em;
      Bool_t c_hdelta;                                                  
      Bool_t c_edelta;                                                                     
      Bool_t c_ecal;                                                                            
      Bool_t c_ctime; 


      //-----------LOOP OVER ALL ENTRIES IN TREE-----------------------
      //Loop over all entries
      for(int i=0; i<T->GetEntries(); i++){
	
	T->GetEntry(i);


	c_Em = emiss < 0.035;
	c_hdelta = hdelta>-8.&&hdelta<8.;  //good HMS delta range (well known recon. matrix)    
	c_ecal = pcal_etottracknorm > 0.85 &&  pcal_etottracknorm < 1.2;   //reject pions   
	c_ctime = epCoinTime>8.5 && epCoinTime<13.5;                                               
	c_edelta = edelta > -10 && edelta < 22.;                                                                      
	
	//HMS Particle angles relative to central angle
	htheta_p = xangle*180./TMath::Pi() - ptheta_e;
      
	//Calculated electron angle
	// Eb = Pp * cos(th_p) + E'*cos(th_e_calc)

	ptheta_e_calc = acos((Eb - hms_Pmeas * cos(htheta_p*TMath::Pi()/180.))/shms_Pmeas );
	ptheta_e = ptheta_e*TMath::Pi()/180.;

	if (c_Em&&c_hdelta&&c_edelta&&c_ecal&&c_ctime)
	  {
	    //Fill Histogram
	    histAngDiff[irun]->Fill((ptheta_e_calc-ptheta_e));
	  }

      } //end entry loop

      //Draw TO Canvas
      c0->cd(irun+1);
      histAngDiff[irun]->GetXaxis()->SetTitle("SHMS #theta_{e,calc} - #theta_{e,meas} [radians]");
      histAngDiff[irun]->Draw();

      //Write Histograms to a ROOTfile
      TFile *outROOT = new TFile(Form("SHMS_AngleOffsets_%d.root", run[irun]), "recreate");
      data_HList[irun].Write();
      outROOT->Close();


    } //end run loop

  c0->SaveAs("SHMS_AngleOffsets.pdf");




  //------------------SIMC-----------------

  TObjArray simc_HList[4];

  //TString filename = Form("../../../../worksim_voli/D2_heep_%d.root",irun);                                                                                  
  //TFile *simc_file = new TFile(filename, "READ");                                                                                                            
  //TTree *SNT = (TTree*)simc_file->Get("SNT");  

  //Variables
  Float_t Normfac;
  Float_t Weight;
  Float_t h_delta;
  Float_t e_delta;
  Float_t Em;
  Float_t theta_e;      //electron angle [rad]
  Float_t theta_p;      //proton angle [rad]
  Float_t h_pf;         //hadron momentum
  Float_t e_pf;         //electron momentum

  Double_t th_e_calc;
  Double_t simc_theta_e;

  //CREATE EMPTY HISTOGRAMS                                                                                                                                        
  TH1F *hist_simcAngDiff[4];                                                                                                                                       
                                                                                                                                                                   
  //Create TCanvas                                                                                                                                                 
  TCanvas *c1 = new TCanvas("c1", "SIMC SHMS Angle Offsets", 1900, 1500);                                                                                          
  c1->Divide(2,2);                                                                                                                                                 
                   

  //Loop over runs                                                                                            
  for(int irun=0; irun<4; irun++)                                                                                          
    {                                                                                                               
      TString filename = Form("../../../../worksim_voli/D2_heep_%d.root",run[irun]);
                                                                  
      TFile *simc_file = new TFile(filename, "READ");        
                                                               
      TTree *SNT = (TTree*)simc_file->Get("SNT");                                                                                                                                                       
      //Initialize histo object array                                                                                                                
      simc_HList[irun](0);            

  //Set Branch Address
  SNT->SetBranchAddress("Normfac", &Normfac);   
  SNT->SetBranchAddress("Weight", &Weight); 
  SNT->SetBranchAddress("h_delta", &h_delta); 
  SNT->SetBranchAddress("e_delta", &e_delta);
  SNT->SetBranchAddress("Em", &Em);
  SNT->SetBranchAddress("theta_e", &theta_e);                                                                                                    
  SNT->SetBranchAddress("theta_p", &theta_p);  
  SNT->SetBranchAddress("h_pf", &h_pf);                                                                                               
  SNT->SetBranchAddress("e_pf", &e_pf); 

  
  //HISTOGRAMS     
  hist_simcAngDiff[irun] = new TH1F(Form("simc_AngDiff_Run%d",run[irun]), Form("Run %d: SIMC SHMS (#theta_{e, calc} - #theta_{e,meas})", run[irun]), 100, -0.04, 0.04);  
                                                                                                                                
  //Add the histogram objects to the Obj list                                                                    
  simc_HList[irun].Add(hist_simcAngDiff[irun]);                                                                                                        
                                                                                                                                         
                                                                                                                                              
  //Define Boolean for Cuts                                                                                                                                    
  Bool_t cs_Em;                                                                                                                                                 
  Bool_t cs_hdelta;                                                                                                                                             
  Bool_t cs_edelta;                                                                                                                                             
  Bool_t cs_ecal;                                                                                                                                               
  Bool_t cs_ctime;                                                                                                                                              
                       

      //-----------LOOP OVER ALL ENTRIES IN TREE-----------------------
      //Loop over all entries
      for(int i=0; i<SNT->GetEntries(); i++){
	
	SNT->GetEntry(i);


	cs_Em = Em < 0.03;
	cs_hdelta = h_delta>-8.&&h_delta<8.;  //good HMS delta range (well known recon. matrix)                                                
	cs_edelta = e_delta > -10. && e_delta < 22.;                                                                      
	
      
	//Calculated electron angle
	// Eb = Pp * cos(th_p) + E'*cos(th_e_calc)
	
	h_pf = h_pf / 1000.;
	e_pf = e_pf / 1000.;

       
	th_e_calc = acos((Eb - h_pf * cos(theta_p))/e_pf );
	

	if (cs_Em&&cs_hdelta&&cs_edelta)
	  {
	    //Fill Histogram
	    hist_simcAngDiff[irun]->Fill((th_e_calc-theta_e));
	  }

      } //end entry loop

      //Draw TO Canvas
      c1->cd(irun+1);
      hist_simcAngDiff[irun]->GetXaxis()->SetTitle("SIMC SHMS #theta_{e,calc} - #theta_{e,meas} [radians]");
      hist_simcAngDiff[irun]->Draw();

      //Write Histograms to a ROOTfile
      TFile *simcoutROOT = new TFile(Form("SIMC_SHMS_AngleOffsets_%d.root", run[irun]), "recreate");
      simc_HList[irun].Write();
      simcoutROOT->Close();



    }//end loop over run



}
