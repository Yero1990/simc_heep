void check_simc()
{
  
  int run = 3288;
   
  Double_t charge_factor = 146.908;       //Units: mC   :: beam_current(uA) * time (hrs) * 3600. / 1000.  3600--> hrs to sec,  1000--> uC to mC
  Double_t e_trkEff = 0.9856;
  Double_t h_trkEff = 0.9864;           
  Double_t t_LT = 0.945919;

 
  //Histograms to check
  TH1F *deut_Emiss = new TH1F("deutEmiss","missing energy", 100, -0.02, 0.05);       //min width = 21.6 (0.0216)MeV,  COUNTS/25 MeV
  TH1F *deut_W = new TH1F("deutW", "Invariant Mass, W", 100, 0.8, 1.1);     //min width = 19.9 MeV (0.0199) (bin width = 25 MeV)
  
  TH1F *gfor_Emiss = new TH1F("gforEmiss","missing energy", 100, -0.02, 0.05);       //min width = 21.6 (0.0216)MeV,  COUNTS/25 MeV
  TH1F *gfor_W = new TH1F("gforW", "Invariant Mass, W", 100, 0.8, 1.1);     //min width = 19.9 MeV (0.0199) (bin width = 25 MeV)
 
  TH1F *deut_Weight = new TH1F("deutWeight","", 100, 0, 0.00135);       //min width = 21.6 (0.0216)MeV,  COUNTS/25 MeV
  TH1F *gfor_Weight = new TH1F("gforWeight", "", 100, 0., 0.00135); 
  
  //Variables to fill
  Float_t Weight;
  Float_t Normfac;
  Float_t W;
  Float_t Em;
  Double_t FullWeight;

  //Read simc ROOTfiles 
  TString deut_fname = Form("../worksim_voli/D2_heep_%d.root",run);                                 
  TString gfor_fname = Form("../worksim/D2_heep_%d_gfor.root",run);                                 

  TFile *deut_file = new TFile(deut_fname, "READ"); 
  TFile *gfor_file = new TFile(gfor_fname, "READ"); 

  //Analyze deut_simc
  deut_file->cd();
  
  TTree *SNT = (TTree*)deut_file->Get("SNT");
  

  //Set Branch Address
  SNT->SetBranchAddress("Normfac", &Normfac);
  SNT->SetBranchAddress("W", &W);
  SNT->SetBranchAddress("Em", &Em);
  SNT->SetBranchAddress("Weight", &Weight);
 
  Long64_t nentries = SNT->GetEntries();
 
  for (Long64_t i=0; i<nentries;i++) {
    
    SNT->GetEntry(i);
    
    //FullWeight =  (Normfac*Weight*charge_factor*e_trkEff*h_trkEff*t_LT)/nentries;
    FullWeight =  (Normfac*Weight)/nentries;

    deut_Emiss->Fill(Em, FullWeight);
    deut_W->Fill(W, FullWeight);
    deut_Weight->Fill(Weight);
  }



 //Analyze gfor_simc
  gfor_file->cd();
  
  TTree *gfor = (TTree*)gfor_file->Get("h666");
  
  Normfac = 0.243687e07;
  //Set Branch Address
  gfor->SetBranchAddress("W", &W);
  gfor->SetBranchAddress("Em", &Em);
  gfor->SetBranchAddress("Weight", &Weight);
 
  nentries = gfor->GetEntries();
 
  for (Long64_t i=0; i<nentries;i++) {
    
    gfor->GetEntry(i);
    
    //FullWeight =  (Normfac*Weight*charge_factor*e_trkEff*h_trkEff*t_LT)/nentries;
    FullWeight =  (Normfac*Weight)/nentries;

    gfor_Emiss->Fill(Em, FullWeight);
    gfor_W->Fill(W, FullWeight);
    gfor_Weight->Fill(Weight);

  }

  TCanvas *c1 = new TCanvas("c1", "", 1000, 1500);
  c1->Divide(2,2);

  c1->cd(1);
  gfor_W->SetLineColor(kRed);
  gfor_W->Draw();
  deut_W->Draw("sames");

  c1->cd(2);
  gfor_Emiss->SetLineColor(kRed);
  gfor_Emiss->Draw();
  deut_Emiss->Draw("sames");

  c1->cd(3);
  gfor_Weight->SetLineColor(kRed);
  gfor_Weight->Draw();
  deut_Weight->Draw("sames");

}
