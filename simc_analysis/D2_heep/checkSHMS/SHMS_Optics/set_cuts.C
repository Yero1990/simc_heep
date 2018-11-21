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
void set_cuts(TString basename,Int_t flag=0) {
 gROOT->Reset();
 gStyle->SetOptStat(0);
 gStyle->SetOptFit(11);
 gStyle->SetTitleOffset(1.,"Y");
 gStyle->SetTitleOffset(.7,"X");
 gStyle->SetLabelSize(0.04,"XY");
 gStyle->SetTitleSize(0.06,"XY");
 gStyle->SetPadLeftMargin(0.14);
 //
     TString inputroot;
    inputroot="hist/"+basename+"_hist_flag0.root";
      TFile *fhistroot;
     TH2F *fhist[6];
     TH1F *fdelta[10];
    TString hname[6] = {"h_xfp_yfp","h_xfp_ypfp","h_xfp_xpfp","h_yfp_xpfp","h_yfp_ypfp","h_xpfp_ypfp"};
     fhistroot =  new TFile(inputroot);
     fhistroot->ls("d");
     for (Int_t nc=0;nc<6;nc++) {
       fhist[nc] = (TH2F*)fhistroot->Get(hname[nc]);
       if (fhist[nc]) cout << inputroot << " " << nc << " " << hname[nc] << endl;
       if (!fhist[nc]) {
           cout << inputroot << " " << nc << " no hist = " << hname[nc] << endl;
	   cout << " quit" <<endl;
	   return;
       }}
    //
   if (flag==0) {
   TCanvas *ccomp[6];
   for (Int_t nc=0;nc<6;nc++) {
   TString xlabel= hname[nc];
   ccomp[nc] = new TCanvas(Form("ccomp_%d",nc),xlabel,700,800);
   ccomp[nc]->Divide(1,1);
   ccomp[nc]->cd(1);
   gPad->SetGridx();
   gPad->SetGridy();
   //gPad->SetLogz();
   cout << " plot = " << hname[nc] << endl;
      fhist[nc]->SetTitle(basename);
      fhist[nc]->SetMinimum(10);
       fhist[nc]->Draw("colz");
   }
   return;
   }
    TString outputhist;
   outputhist="hist/"+basename+"_cut.root";
       cout << "outputhist =  " << outputhist << endl;
   if (flag==3) {
   TFile f(outputhist,"UPDATE");
   TCanvas *ccompcut; 
   ccompcut= new TCanvas("ccompcut","cut",900,500);
   ccompcut->Divide(1,1);
   ccompcut->cd(1);
   gPad->SetGridx();
   gPad->SetGridy();
    gPad->SetLogz();    
     fhistroot->cd();
      fhist[2]->SetTitle(basename);
      fhist[2]->SetMinimum(10);
       fhist[2]->Draw("colz");
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
     fhistroot->cd();    
      fhist[4]->SetTitle(basename);
      fhist[4]->SetMinimum(10);
       fhist[4]->Draw("colz");
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
     Int_t nc=0;
     Int_t nnc=0;
    Int_t n=0;
    TCutG* cutg;
    TCutG *tmpg,*mycutg;
	TCutG *t ;  

   if (flag==1) {
   TCanvas *ccompcut; 
   ccompcut= new TCanvas("ccompcut","cut",900,500);
   ccompcut->Divide(1,1);
   ccompcut->cd(1);
   gPad->SetGridx();
   gPad->SetGridy();
    gPad->SetLogz();    
     fhistroot->cd();
      fhist[2]->SetTitle(basename);
      fhist[2]->SetMinimum(10);
       fhist[2]->Draw("colz");
    //
       cout << "outputhist =  " << outputhist << endl;
    TFile f(outputhist,"UPDATE");
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
      nc=0;
       while (nc!=-1) {
        cout <<"Enter (-1 quit, -10 delete cut) new nc, present nc = " << nc << endl;
        cin >> nnc ;
        if (nnc==-1) nc=nnc;
        if(nnc==-10) {
                     cout <<"Cut to delete  enter number" << endl;
                     cin >> nc ;	       
	             f.Delete(Form("xpfp_xfp_cut_%d;*",nc));
	             cout << " delete cut = " << nc << endl;
	} 
        if (nnc>=0) {
	  nc=nnc;        
        t=(TCutG*)gROOT->FindObject(Form("xpfp_xfp_cut_%d",nc));
	if (t) {
             ccompcut->cd(1);
	     t->Draw("same");
	     ccompcut->Update();
	} 
	if (nc!=-1 && nnc!=-10) {
          cutg=(TCutG*)gPad->WaitPrimitive("CUTG","CutG");
          ccompcut->Update();
          tmpg= (TCutG*)gROOT->GetListOfSpecials()->FindObject("CUTG");
           mycutg=(TCutG*)(tmpg->Clone(Form("xpfp_xfp_cut_%d",nc)));
          nc++;
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
   }
   //
   if (flag==2) {
   TCanvas *ccompcut1; 
   ccompcut1= new TCanvas("ccompcut1","cut1",900,500);
   ccompcut1->Divide(1,1);
   ccompcut1->cd(1);
   gPad->SetGridx();
   gPad->SetGridy();
    gPad->SetLogz();
    
      fhist[4]->SetTitle(basename);
      fhist[4]->SetMinimum(10);
       fhist[4]->Draw("colz");
       n=0;
    TFile f(outputhist,"UPDATE");
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
    nc=0;
        while (nc!=-1) {
             cout <<"enter  (-1 quit, -10 delete cut) nc, present nc = " << nc << endl;
             cin >> nnc ;
        if (nnc==-1) nc=nnc;
        if(nnc==-10) {
                     cout <<"Cut to delete  enter number" << endl;
                     cin >> nc ;	       
	             f.Delete(Form("ypfp_yfp_cut_%d;*",nc));
	             cout << " delete cut = " << nc << endl;
	} 
        if (nnc>=0) {
	  nc=nnc;        
          t=(TCutG*)gROOT->FindObject(Form("ypfp_yfp_cut_%d",nc));
          if (t) {
             ccompcut1->cd(1);
	     t->Draw("same");
	     ccompcut1->Update();
	  }
	if (nc!=-1 && nnc!=-10) {
    cutg=(TCutG*)gPad->WaitPrimitive("CUTG","CutG");
    ccompcut1->Update();
    tmpg= (TCutG*)gROOT->GetListOfSpecials()->FindObject("CUTG");
    mycutg=(TCutG*)(tmpg->Clone(Form("ypfp_yfp_cut_%d",nc)));
    nc++;
    mycutg->Write("",TObject::kOverwrite);
    mycutg->Print();
    mycutg->Draw("same");
	}
	}
	 //
	}
   }
      //
}


