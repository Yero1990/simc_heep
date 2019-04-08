void get_absorption()
{

  // Upper Limit Cuts Determined from H(e,e'p) Coincidence
  
  // Emiss < 30 MeV
  TCut em_cut="H.kin.secondary.emiss<0.03";
  TCut ecal_cut = "P.cal.etotnorm>0.6";

  // SHMS delta: (-3.5, 1.5) %,     
  // HMS delta: (-8,8)%
  TCut edelta_cut="P.gtr.dp>-3.5&&P.gtr.dp<1.5";
  TCut hdelta_cut="abs(H.gtr.dp)<8.";
  
  // SHMS xptar (P.gtr.th): |X'tar| <  0.024 rad 
  // SHMS yptar (P.gtr.ph): |Y'tar| <  0.013 rad
  TCut exptar_cut="P.gtr.th>-0.01&&P.gtr.th<0.015";   //upper limit: "P.gtr.th>-0.024&&P.gtr.th<0.024"
  TCut eyptar_cut="P.gtr.ph>-0.01&&P.gtr.ph<0.01";    //upper limit: "P.gtr.ph>-0.013&&P.gtr.th<0.013"
  TCut eSolid_cut = exptar_cut&&eyptar_cut;

  // HMS xptar (H.gtr.th): |X'tar| <  0.07 rad 
  // HMS yptar (H.gtr.ph): |Y'tar| <  0.03 rad
  TCut hxptar_cut="H.gtr.th>-0.07&&H.gtr.th<0.07";
  TCut hyptar_cut="H.gtr.ph>-0.03&&H.gtr.ph<0.03";
  TCut hSolid_cut = hxptar_cut&&hyptar_cut;

  TCut W_cut="P.kin.primary.W>0.9&&P.kin.primary.W<1.";
  TCut hTRIG1_cut="T.coin.hTRIG1_ROC1_tdcTime>0";


  // SHMS React Z: |P.react.z| < 2.5 cm  //Al. dummy endcaps cut
  TCut preactz_cut="abs(P.react.z)<2.5";
  
  //TString filename = "../../../../../hallc_replay/ROOTfiles/coin_replay_heep_check_3248_-1.root";    //LH2 e- Coincidence
  TString filename = "../../../../../hallc_replay/ROOTfiles/coin_replay_heep_check_3254_-1.root";    //DUMMY RUN
  //TString filename = "../../../../../hallc_replay/ROOTfiles/coin_replay_heep_check_3259_-1.root";     // LH2 e- singles run

  //TString filename = "../../../../worksim_voli/D2_heep_3248.root";

  TFile *data_file = new TFile(filename, "READ"); 
  TTree *T = (TTree*)data_file->Get("T");
  //TTree *T = (TTree*)data_file->Get("SNT");

  //Create output root file where histograms will be stored
  TFile *outROOT = new TFile("proton_abs_plots.root", "recreate");

  //TH1F *h_Emiss = new TH1F("h_Emiss", "", 100, -0.02,0.1);
  TH1F *h_eytar = new TH1F("h_eytar", "", 100, -8, 8);

  //2D Acceptance Correlations
  //TH2F *h_edelta_vs_hdelta = new TH2F("h_edelta_vs_hdelta", "", 100, -9, 9, 100, -4, 2);
  //TH2F *h_exptar_vs_eyptar = new TH2F("h_exptar_vs_eyptar", "", 100, -0.03, 0.03, 100, -0.03, 0.03);
  //TH2F *h_hxptar_vs_hyptar = new TH2F("h_hxptar_vs_hyptar", "", 100, -0.1, 0.1, 100, -0.1, 0.1);
  TH2F *h_eytar_vs_eyptar = new TH2F("h_eytar_vs_eyptar", "", 100, -0.1, 0.1, 100, -8, 8);

  //Dummy  W vs. React Z
  //TH2F *h_W_vs_preactz_nocut = new TH2F("h_W_vs_preactz_nocut", "", 100, -10, 10, 100, 0.7, 1.8);
  TH2F *h_W_vs_preactz_deltacut = new TH2F("h_W_vs_preactz_deltacut", "", 100, -10, 10, 100, 0.7, 1.8);
  //TH2F *h_W_vs_preactz_accptcut = new TH2F("h_W_vs_preactz_accptcut", "", 100, -10, 10, 100, 0.7, 1.8);
  TH2F *h_W_vs_preactz_allcuts = new TH2F("h_W_vs_preactz_allcuts", "", 100, -10, 10, 100, 0.7, 1.8);

  //Dummy SHMS React Z 1D
  //TH1F *h_preactz_nocut = new TH1F("h_preactz_nocut", "", 100, -10, 10);
  //TH1F *h_preactz_deltacut = new TH1F("h_preactz_deltacut", "", 100, -10, 10);
  //TH1F *h_preactz_accptcut = new TH1F("h_preactz_accptcut", "", 100, -10, 10);
  TH1F *h_preactz_allcuts = new TH1F("h_preactz_allcuts", "", 100, -10, 10);

  //Proton Absorption DID e-, SHOULD e-, RATIO
  //TH1F *h_W = new TH1F("h_W", "", 100, 0.8, 1.2);
  
  TH1F *h_exptar_did = new TH1F("h_exptar_did", "", 25, -0.06,  0.06);
  TH1F *h_exptar_should = new TH1F("h_exptar_should", "", 25, -0.06,  0.06);
  TH1F *h_exptar_ratio = new TH1F("h_exptar_ratio", "", 25, -0.06,  0.06);

  TH1F *h_eyptar_did = new TH1F("h_eyptar_did", "", 25, -0.06,  0.06);
  TH1F *h_eyptar_should = new TH1F("h_eyptar_should", "", 25, -0.06,  0.06);
  TH1F *h_eyptar_ratio = new TH1F("h_eyptar_ratio", "", 25, -0.06,  0.06);

  TH1F *h_preactz_did = new TH1F("h_preactz_did", "", 50, -10,  10);
  TH1F *h_preactz_should = new TH1F("h_preactz_should", "", 50, -10,  10);
  TH1F *h_preactz_ratio = new TH1F("h_preactz_ratio", "", 50, -10,  10);
  
  TH1F *h_W_did = new TH1F("h_W_did", "", 50, 0.8,  1.2);
  TH1F *h_W_should = new TH1F("h_W_should", "", 50, 0.8,  1.2);
  TH1F *h_W_ratio = new TH1F("h_W_ratio", "", 50, 0.8,  1.2);

  h_exptar_did->Sumw2();
  h_exptar_should->Sumw2();
  h_exptar_ratio->Sumw2();

  h_eyptar_did->Sumw2();
  h_eyptar_should->Sumw2();
  h_eyptar_ratio->Sumw2();

  h_W_did->Sumw2();
  h_W_should->Sumw2();
  h_W_ratio->Sumw2();

  //--------------------SIMC---------------------------------
  /*
  TCut em_cut = "Em<0.02";
  TCut delta_cut = "e_delta>-3.5&&e_delta<1.5&&abs(h_delta)<8.";

  //Draw Acceptance Plots
  //T->Draw("Em>>h_Emiss", "Weight");
  //T->Draw("e_delta:h_delta>>h_edelta_vs_hdelta", (em_cut)*"Weight", "colz");
  //T->Draw("e_xptar:e_yptar>>h_exptar_vs_eyptar", (em_cut&&delta_cut)*"Weight", "colz");
  //T->Draw("h_xptar:h_yptar>>h_hxptar_vs_hyptar", (em_cut&&delta_cut)*"Weight", "colz");
 
  //T->Draw("Em>>h_Emiss", "Weight");
  //T->Draw("e_delta:h_delta>>h_edelta_vs_hdelta", "Weight", "colz");
  //T->Draw("e_xptar:e_yptar>>h_exptar_vs_eyptar", "Weight", "colz");
  //T->Draw("h_xptar:h_yptar>>h_hxptar_vs_hyptar", "Weight", "colz");
  */
  //---------------------------------------------------------


  //--------------------DATA---------------------------------
  
  //----DUMMY DATA----
  //T->Draw("P.kin.primary.W:P.react.z>>h_W_vs_preactz_nocut", "", "colz");
  //T->Draw("P.kin.primary.W:P.react.z>>h_W_vs_preactz_deltacut", "P.gtr.dp>-3.5&&P.gtr.dp<1.5", "colz");
  //T->Draw("P.kin.primary.W:P.react.z>>h_W_vs_preactz_accptcut", "P.gtr.dp>-3.5&&P.gtr.dp<1.5&&abs(P.gtr.th)<0.024&&abs(P.gtr.ph)<0.013", "colz");
  T->Draw("P.kin.primary.W:P.react.z>>h_W_vs_preactz_allcuts",edelta_cut&&eSolid_cut&&W_cut&&ecal_cut , "colz");

  //T->Draw("P.react.z>>h_preactz_nocut");
  T->Draw("P.react.z>>h_preactz_deltacut", "P.gtr.dp>-3.5&&P.gtr.dp<1.5");
  //T->Draw("P.react.z>>h_preactz_accptcut", "P.gtr.dp>-3.5&&P.gtr.dp<1.5&&abs(P.gtr.th)<0.024&&abs(P.gtr.ph)<0.013");
  T->Draw("P.react.z>>h_preactz_allcuts", edelta_cut&&eSolid_cut&&W_cut&&ecal_cut);

  //Draw Emiss and 2D Acceptance Histograms
  //T->Draw("P.gtr.dp:H.gtr.dp>>h_edelta_vs_hdelta", edelta_cut&&eSolid_cut&&preactz_cut&&W_cut&&ecal_cut&&hTRIG1_cut&&hdelta_cut, "colz");
  //T->Draw("P.gtr.th:P.gtr.ph>>h_exptar_vs_eyptar", edelta_cut&&eSolid_cut&&preactz_cut&&W_cut&&ecal_cut&&hTRIG1_cut&&hdelta_cut, "colz");
  //T->Draw("H.gtr.th:H.gtr.ph>>h_hxptar_vs_hyptar", edelta_cut&&eSolid_cut&&preactz_cut&&W_cut&&ecal_cut&&hTRIG1_cut&&hdelta_cut, "colz");


  //Estimating Proton Absorption

  //Define Some Cuts to determine clean electron sample
  TCut should_cut = edelta_cut&&eSolid_cut&&preactz_cut&&W_cut&&ecal_cut; 
  TCut did_cut = should_cut&&hTRIG1_cut&&hdelta_cut;   //since we are making a cut on HMS variable, we have to correct for HMS tracking efficiency,
                                                      //as it would NOT cancel out when taking the ratio did_e- / should_e-
  
  //TCut should_exptar_cut = edelta_cut&&eyptar_cut&&preactz_cut&&W_cut; 
  //TCut did_exptar_cut = should_exptar_cut&&hTRIG1_cut&&hdelta_cut; 

  //TCut should_eyptar_cut = edelta_cut&&exptar_cut&&preactz_cut&&W_cut; 
  //TCut did_eyptar_cut = should_eyptar_cut&&hTRIG1_cut&&hdelta_cut; 
  
  //TCut should_preactz_cut = edelta_cut&&eSolid_cut&&W_cut; 
  //TCut did_preactz_cut = should_preactz_cut&&hTRIG1_cut&&hdelta_cut; 

  

  //HMS Tracking Efficiency for SHMS e- Singles Run 3259:  
  //HMS Tracking Eff = 0.9907
  Double_t hTrk_eff = 0.9907;

  //T->Draw("P.kin.primary.W>>h_W", edelta_cut&&eSolid_cut&&preactz_cut&&hdelta_cut);
  T->Draw("P.gtr.y>>h_eytar", edelta_cut);
  T->Draw("P.gtr.y:P.gtr.ph>>h_eytar_vs_eyptar", edelta_cut);

  T->Draw("P.gtr.th>>h_exptar_should", should_cut);
  T->Draw("P.gtr.th>>h_exptar_did", did_cut);

  T->Draw("P.gtr.ph>>h_eyptar_should", should_cut);
  T->Draw("P.gtr.ph>>h_eyptar_did", did_cut);

  T->Draw("P.react.z>>h_preactz_should", should_cut);
  T->Draw("P.react.z>>h_preactz_did", did_cut);

  T->Draw("P.kin.primary.W>>h_W_should", should_cut);
  T->Draw("P.kin.primary.W>>h_W_did", did_cut);

  
  //Scale the "did" by HMS tracking efficiency
  h_exptar_did->Scale(1./hTrk_eff);
  h_eyptar_did->Scale(1./hTrk_eff);
  h_preactz_did->Scale(1./hTrk_eff);
  h_W_did->Scale(1./hTrk_eff);
  
  
  //Calculate error bin-by-bin
  Double_t exptar_should_content;
  Double_t exptar_did_content;
  Double_t exptar_err_temp;
  Double_t exptar_err[25];

  Double_t eyptar_should_content;
  Double_t eyptar_did_content;
  Double_t eyptar_err_temp;
  Double_t eyptar_err[25];

  Double_t preactz_should_content;
  Double_t preactz_did_content;
  Double_t preactz_err_temp;
  Double_t preactz_err[50];
  
  Double_t W_should_content;
  Double_t W_did_content;
  Double_t W_err_temp;
  Double_t W_err[50];
  

  //Loop over eX'tar / eY'tar bins
  for(int ibin=0; ibin<25; ibin++){

    //X'tar
    exptar_should_content = h_exptar_should->GetBinContent(ibin);
    exptar_did_content = h_exptar_did->GetBinContent(ibin);  

    exptar_err_temp =  sqrt( exptar_should_content -  exptar_did_content) / exptar_should_content;
    exptar_err[ibin] =  exptar_err_temp;
        
    if(exptar_should_content==0){ exptar_err[ibin]=0.;}

    //Y'tar
    eyptar_should_content = h_eyptar_should->GetBinContent(ibin);
    eyptar_did_content = h_eyptar_did->GetBinContent(ibin);   

    eyptar_err_temp =  sqrt( eyptar_should_content -  eyptar_did_content) / eyptar_should_content;
    eyptar_err[ibin] =  eyptar_err_temp;

    if(eyptar_should_content==0){ eyptar_err[ibin]=0.;}

  }

  
  for(int ibin=0; ibin<50; ibin++){
  
    //SHMS React Z (Lab Frame)
    preactz_should_content = h_preactz_should->GetBinContent(ibin);
    preactz_did_content = h_preactz_did->GetBinContent(ibin);  


    preactz_err_temp =  sqrt( preactz_should_content -  preactz_did_content) / preactz_should_content;
    preactz_err[ibin] =  preactz_err_temp;
     
    if(preactz_should_content==0){ preactz_err[ibin]=0.;}

    //SHMS W
    W_should_content = h_W_should->GetBinContent(ibin);
    W_did_content = h_W_did->GetBinContent(ibin);  


    W_err_temp =  sqrt( W_should_content -  W_did_content) / W_should_content;
    W_err[ibin] =  W_err_temp;
    
    if(W_should_content==0){ W_err[ibin]=0.;}

  }
  

  
  h_exptar_ratio->Divide(h_exptar_did,h_exptar_should);
  h_eyptar_ratio->Divide(h_eyptar_did,h_eyptar_should);
  h_preactz_ratio->Divide(h_preactz_did,h_preactz_should);
  h_W_ratio->Divide(h_W_did,h_W_should);


    for(int ibin=0; ibin<25; ibin++){

      h_exptar_ratio->SetBinError(ibin, exptar_err[ibin]);
      h_eyptar_ratio->SetBinError(ibin, eyptar_err[ibin]);

    }
    
    for(int ibin=0; ibin<100; ibin++){
      h_preactz_ratio->SetBinError(ibin, preactz_err[ibin]);
      h_W_ratio->SetBinError(ibin, W_err[ibin]);

    }
  //----------------------------------------------------------
  

outROOT->Write();	
outROOT->Close();  
}
