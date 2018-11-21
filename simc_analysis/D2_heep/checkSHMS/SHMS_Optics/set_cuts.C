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
  
  
  //Flag 0 ----> View histogram where the cut will be applied
  if (flag==0) {
    TCanvas *histView;
    histView = new TCanvas("histView_%d", "",1500,1500);
    histView->Divide(2,2);
    for (Int_t fp=0;fp<4;fp++) {
   
      TString xlabel= hname[fp];
      histView->cd(fp+1);
      gPad->SetGridx();
      gPad->SetGridy();
      //gPad->SetLogz();
      cout << " Plotting --> " << hname[fp] << endl;
      hist_shmsDelDiff_FP_corr[fp]->SetTitle(hname[fp]);
      hist_shmsDelDiff_FP_corr[fp]->Draw("colz");
    }
    return;
  }
 

  //Set Name of ROOTfile containing polygon Cuts
  TString ouCutFile;
  outCutFile="SHMS_heepData_histos_%d_cut.root";
  
  //------------------ Flag 1 ----> Set the Polygon Cut ----------------------

  if (flag==1) {
    TCanvas *ccompcut; 
    ccompcut= new TCanvas("ccompcut","cut",900,500);
    ccompcut->Divide(1,1);
    ccompcut->cd(1);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogz();    
    
    fInput->cd();
    hist_shmsDelDiff_FP_corr[2]->SetTitle(basename);
    hist_shmsDelDiff_FP_corr[2]->SetMinimum(10);
    hist_shmsDelDiff_FP_corr[2]->Draw("colz");
    
    cout << "outCutFile =  " << outCutFile << endl;
    TFile f(outCutFile,"UPDATE");
    n=0;
    for (Int_t nn=0;nn<11;nn++) {
      t=(TCutG*)gROOT->FindObject(Form("xpfp_xfp_cut_%d",nn));
      if(t) {
	ccompcut->cd(1);
	t->Draw("same");
	ccompcut->Update();
	t->SetLineColor(2);
	cout << " draw cut = " << nn << endl;
      }
      //
      // ccompcut->WaitPrimitive();
    }

    fp=0;
    while (fp!=-1) {
      cout <<"Enter (-1 quit, -10 delete cut) new fp, present fp = " << fp << endl;
      cin >> nfp ;
      if (nfp==-1) fp=nfp;
      if(nfp==-10) {
	cout <<"Cut to delete  enter number" << endl;
	cin >> fp ;	       
	f.Delete(Form("xpfp_xfp_cut_%d;*",fp));
	cout << " delete cut = " << fp << endl;
      }
 
      if (nfp>=0) {
	fp=nfp;        
        t=(TCutG*)gROOT->FindObject(Form("xpfp_xfp_cut_%d",fp));
	if (t) {
	  ccompcut->cd(1);
	  t->Draw("same");
	  ccompcut->Update();
	} 

	if (fp!=-1 && nfp!=-10) {
          cutg=(TCutG*)gPad->WaitPrimitive("CUTG","CutG");
          ccompcut->Update();
          tmpg= (TCutG*)gROOT->GetListOfSpecials()->FindObject("CUTG");
	  mycutg=(TCutG*)(tmpg->Clone(Form("xpfp_xfp_cut_%d",fp)));
          fp++;
	  mycutg->Write("",TObject::kOverwrite);
	  mycutg->Print();
          mycutg->Draw();
	}
      }
    }
    //
    n=0;
    //	 while ((TCutG*)f.Get(Form("mycutg_%d",n))) {
    gDirectory->ls("m");
    t=(TCutG*)gROOT->FindObject(Form("xpfp_xfp_cut_%d",n));
    while(t) {
      t->Draw();
      t->SetLineColor(2);
      n++;
      t=(TCutG*)gROOT->FindObject(Form("xpfp_xfp_cut_%d",n));
      ccompcut->Update();
    }
    //
  } //end flag 1
  
  //------------------------------------------------------------------------------------------
  


  /*
   if (flag==3) {
   TFile f(outCutFile,"UPDATE");
   TCanvas *ccompcut; 
   ccompcut= new TCanvas("ccompcut","cut",900,500);
   ccompcut->Divide(1,1);
   ccompcut->cd(1);
   gPad->SetGridx();
   gPad->SetGridy();
    gPad->SetLogz();    
     fInput->cd();
      hist_shmsDelDiff_FP_corr[2]->SetTitle(basename);
      hist_shmsDelDiff_FP_corr[2]->SetMinimum(10);
       hist_shmsDelDiff_FP_corr[2]->Draw("colz");
       f.cd();
	TCutG *t ;
        Int_t n=0;  
       for (Int_t nn=0;nn<11;nn++) {
        t=(TCutG*)gROOT->FindObject(Form("xpfp_xfp_cut_%d",nn));
	if(t) {
	  cout << " found xpfp_xfp_cut " << nn <<endl;
	  //              t->Write("",TObject::kOverwrite);
		 t->Draw("same");
		 t->SetLineColor(2);
	         ccompcut->Update();
		 }
       }
   TCanvas *ccompcut1; 
   ccompcut1= new TCanvas("ccompcut1","cut1",900,500);
   ccompcut1->Divide(1,1);
   ccompcut1->cd(1);
   gPad->SetGridx();
   gPad->SetGridy();
    gPad->SetLogz();
     fInput->cd();    
      hist_shmsDelDiff_FP_corr[4]->SetTitle(basename);
      hist_shmsDelDiff_FP_corr[4]->SetMinimum(10);
       hist_shmsDelDiff_FP_corr[4]->Draw("colz");
        f.cd();
      for (Int_t nn=0;nn<11;nn++) {
        t=(TCutG*)gROOT->FindObject(Form("ypfp_yfp_cut_%d",nn));
	if(t) {
	  //     t->Write("",TObject::kOverwrite);
		 t->Draw("same");
		 t->SetLineColor(2);
                 ccompcut->Update();
		 }
       }
    }
   //
     Int_t fp=0;
     Int_t nfp=0;
    Int_t n=0;
    TCutG* cutg;
    TCutG *tmpg,*mycutg;
	TCutG *t ;  


   //
   if (flag==2) {
   TCanvas *ccompcut1; 
   ccompcut1= new TCanvas("ccompcut1","cut1",900,500);
   ccompcut1->Divide(1,1);
   ccompcut1->cd(1);
   gPad->SetGridx();
   gPad->SetGridy();
    gPad->SetLogz();
    
      hist_shmsDelDiff_FP_corr[4]->SetTitle(basename);
      hist_shmsDelDiff_FP_corr[4]->SetMinimum(10);
       hist_shmsDelDiff_FP_corr[4]->Draw("colz");
       n=0;
    TFile f(outCutFile,"UPDATE");
       for (Int_t nn=0;nn<11;nn++) {
        t=(TCutG*)gROOT->FindObject(Form("ypfp_yfp_cut_%d",nn));
	if(t) {
	  cout << " draw cut = " << nn << endl;
                 ccompcut1->cd(1);
		 t->Draw("same");
		 t->SetLineColor(2);
                 t=(TCutG*)gROOT->FindObject(Form("ypfp_yfp_cut_%d",nn));
    ccompcut1->Update();
		 }
       }
    fp=0;
        while (fp!=-1) {
             cout <<"enter  (-1 quit, -10 delete cut) fp, present fp = " << fp << endl;
             cin >> nfp ;
        if (nfp==-1) fp=nfp;
        if(nfp==-10) {
                     cout <<"Cut to delete  enter number" << endl;
                     cin >> fp ;	       
	             f.Delete(Form("ypfp_yfp_cut_%d;*",fp));
	             cout << " delete cut = " << fp << endl;
	} 
        if (nfp>=0) {
	  fp=nfp;        
          t=(TCutG*)gROOT->FindObject(Form("ypfp_yfp_cut_%d",fp));
          if (t) {
             ccompcut1->cd(1);
	     t->Draw("same");
	     ccompcut1->Update();
	  }
	if (fp!=-1 && nfp!=-10) {
    cutg=(TCutG*)gPad->WaitPrimitive("CUTG","CutG");
    ccompcut1->Update();
    tmpg= (TCutG*)gROOT->GetListOfSpecials()->FindObject("CUTG");
    mycutg=(TCutG*)(tmpg->Clone(Form("ypfp_yfp_cut_%d",fp)));
    fp++;
    mycutg->Write("",TObject::kOverwrite);
    mycutg->Print();
    mycutg->Draw("same");
	}
	}
	 //
	}
   }

      //
      */
}


