#include <TString.h>
#include <TLegend.h>
#include <TFile.h>
#include <TNtuple.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TROOT.h>
#include <TCutG.h>
#include <TMath.h>
#include <TProfile.h>
#include <TObjArray.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <math.h>
#include <iomanip>
#include <fstream>
using namespace std;


void set_cuts(int run,Int_t flag=0) {
  
  gROOT->Reset();
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(11);
  gStyle->SetTitleOffset(1.,"Y");
  gStyle->SetTitleOffset(.7,"X");
  gStyle->SetLabelSize(0.04,"XY");
  gStyle->SetTitleSize(0.06,"XY");
  gStyle->SetPadLeftMargin(0.14);
  
  //Read input ROOTfile containing Histogram Objects
  TString ifilename = Form("SHMS_heepData_histos_%d.root", run);
  TFile *fInput;
  fInput = new TFile(ifilename);

  //Create new Histograms
  TH2F *hist_shmsDelDiff_FP_corr[4] = {0};
  
  //Set Histogram Objects Names
  TString hname[4]; 

  hname[0] = Form("eDelta_vs_xfp: Run %d", run);
  hname[1] = Form("eDelta_vs_xpfp: Run %d", run);
  hname[2] = Form("eDelta_vs_yfp: Run %d", run);
  hname[3] = Form("eDelta_vs_ypfp: Run %d", run);

  TString hname_cut[4]; 

  hname_cut[0] = Form("eDelta_vs_xfp_cut: Run %d", run);
  hname_cut[1] = Form("eDelta_vs_xpfp_cut: Run %d", run);
  hname_cut[2] = Form("eDelta_vs_yfp_cut: Run %d", run);
  hname_cut[3] = Form("eDelta_vs_ypfp_cut: Run %d", run);


  fInput->ls("d");
  
  //Loop over focal plane variables to get Histogram Objects from ROOTfile
  for (Int_t fp=0;fp<4;fp++) {
    
    hist_shmsDelDiff_FP_corr[fp] = (TH2F*)fInput->Get(hname[fp]);
    
    if (hist_shmsDelDiff_FP_corr[fp]) {cout << ifilename << " : " << fp << " : " << hname[fp] << endl;}
    
    if (!hist_shmsDelDiff_FP_corr[fp]) {
      cout << ifilename << " : " << fp << " No Hist Found --> " << hname[fp] << endl;
      cout << " quit" <<endl;
      return;
    }
  }
  
  
  
  
  //Set Name of ROOTfile containing polygon Cuts
  TString outCutFile;
  outCutFile=Form("SHMS_heepData_histos_%d_cut.root", run);

  //------------------ Flag 1 ----> Set the Polygon Cut ----------------------

  

    TCanvas *histView_Cut; 
    histView_Cut= new TCanvas("histView_Cut","cut",1500,1500);
    histView_Cut->Divide(1,1);
    //Loop over focal plane variables
	histView_Cut->cd(flag+1);
	gPad->SetGridx();
	gPad->SetGridy();
	//gPad->SetLogz();    
	
	fInput->cd();
	
	hist_shmsDelDiff_FP_corr[flag]->SetTitle(hname[flag]);
	hist_shmsDelDiff_FP_corr[flag]->Draw("colz");
    
    cout << "outCutFile =  " << outCutFile << endl;
    TFile f(outCutFile,"UPDATE");
   
 
    TCutG*t;

    t=(TCutG*)gROOT->FindObject(hname_cut[flag]);
        if(!t){cout << "No cut  =  " <<hname_cut[flag]  << endl;}
 	if(t) {
	  cout << " draw cut = " <<hname_cut[flag]  << endl;
 		 t->Draw("same");
		 t->SetLineColor(2);
                  histView_Cut->Update();
		 }
	Int_t nloop=0;
	while( nloop!=-1) {
          cout <<"Enter (-1 quit, -10 delete cut) "  << endl;
          cin >> nloop ;
          if(nloop==-10) {
	      f.Delete(hname_cut[flag]);
  	      cout << " delete cut = " <<hname_cut[flag]  << endl;
          }
	  if (nloop!=-1 && nloop!=-10) {
          TCutG*cutg=(TCutG*)gPad->WaitPrimitive("CUTG","CutG");
          histView_Cut->Update();
          TCutG*tmpg= (TCutG*)gROOT->GetListOfSpecials()->FindObject("CUTG");
	  TCutG*mycutg=(TCutG*)(tmpg->Clone(hname_cut[flag]));
	  f.cd();
	  mycutg->Write("",TObject::kOverwrite);
	  mycutg->Print();
          mycutg->Draw();
          histView_Cut->Update();
	  }
	} // while

}


