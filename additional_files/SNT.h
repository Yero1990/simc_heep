//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sun Oct 25 17:32:07 2009 by ROOT version 5.25/03
// from TTree SNT/SIMTUPLE
// found on file: pm_0.5_5.0_1.35_11.00.pwia.norad.root
//////////////////////////////////////////////////////////

#ifndef SNT_h
#define SNT_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

class SNT {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Float_t         Normfac;
   Float_t         e_delta;
   Float_t         e_yptar;
   Float_t         e_xptar;
   Float_t         e_ytar;
   Float_t         e_xfp;
   Float_t         e_xpfp;
   Float_t         e_yfp;
   Float_t         e_ypfp;
   Float_t         e_deltai;
   Float_t         e_yptari;
   Float_t         e_xptari;
   Float_t         e_ytari;
   Float_t         h_delta;
   Float_t         h_yptar;
   Float_t         h_xptar;
   Float_t         h_ytar;
   Float_t         h_xfp;
   Float_t         h_xpfp;
   Float_t         h_yfp;
   Float_t         h_ypfp;
   Float_t         h_deltai;
   Float_t         h_yptari;
   Float_t         h_xptari;
   Float_t         h_ytari;
   Float_t         q;
   Float_t         nu;
   Float_t         Q2;
   Float_t         W;
   Float_t         epsilon;
   Float_t         Em;
   Float_t         Pm;
   Float_t         thetapq;
   Float_t         phipq;
   Float_t         corrsing;
   Float_t         Pmx;
   Float_t         Pmy;
   Float_t         Pmz;
   Float_t         PmPar;
   Float_t         PmPer;
   Float_t         PmOop;
   Float_t         fry;
   Float_t         radphot;
   Float_t         sigcc;
   Float_t         Weight;

   // List of branches
   TBranch        *b_Normfac;   //!
   TBranch        *b_e_delta;   //!
   TBranch        *b_e_yptar;   //!
   TBranch        *b_e_xptar;   //!
   TBranch        *b_e_ytar;   //!
   TBranch        *b_e_xfp;   //!
   TBranch        *b_e_xpfp;   //!
   TBranch        *b_e_yfp;   //!
   TBranch        *b_e_ypfp;   //!
   TBranch        *b_e_deltai;   //!
   TBranch        *b_e_yptari;   //!
   TBranch        *b_e_xptari;   //!
   TBranch        *b_e_ytari;   //!
   TBranch        *b_h_delta;   //!
   TBranch        *b_h_yptar;   //!
   TBranch        *b_h_xptar;   //!
   TBranch        *b_h_ytar;   //!
   TBranch        *b_h_xfp;   //!
   TBranch        *b_h_xpfp;   //!
   TBranch        *b_h_yfp;   //!
   TBranch        *b_h_ypfp;   //!
   TBranch        *b_h_deltai;   //!
   TBranch        *b_h_yptari;   //!
   TBranch        *b_h_xptari;   //!
   TBranch        *b_h_ytari;   //!
   TBranch        *b_q;   //!
   TBranch        *b_nu;   //!
   TBranch        *b_Q2;   //!
   TBranch        *b_W;   //!
   TBranch        *b_epsilon;   //!
   TBranch        *b_Em;   //!
   TBranch        *b_Pm;   //!
   TBranch        *b_thetapq;   //!
   TBranch        *b_phipq;   //!
   TBranch        *b_corrsing;   //!
   TBranch        *b_Pmx;   //!
   TBranch        *b_Pmy;   //!
   TBranch        *b_Pmz;   //!
   TBranch        *b_PmPar;   //!
   TBranch        *b_PmPer;   //!
   TBranch        *b_PmOop;   //!
   TBranch        *b_fry;   //!
   TBranch        *b_radphot;   //!
   TBranch        *b_sigcc;   //!
   TBranch        *b_Weight;   //!

   SNT(TTree *tree=0);
   virtual ~SNT();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop(Double_t qmin, Double_t qmax, Int_t type, TString head);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef SNT_cxx
SNT::SNT(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("pm_0.5_5.0_1.35_11.00.pwia.norad.root");
      if (!f) {
         f = new TFile("pm_0.5_5.0_1.35_11.00.pwia.norad.root");
      }
      tree = (TTree*)gDirectory->Get("SNT");

   }
   Init(tree);
}

SNT::~SNT()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t SNT::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t SNT::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (!fChain->InheritsFrom(TChain::Class()))  return centry;
   TChain *chain = (TChain*)fChain;
   if (chain->GetTreeNumber() != fCurrent) {
      fCurrent = chain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void SNT::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("Normfac", &Normfac, &b_Normfac);
   fChain->SetBranchAddress("e_delta", &e_delta, &b_e_delta);
   fChain->SetBranchAddress("e_yptar", &e_yptar, &b_e_yptar);
   fChain->SetBranchAddress("e_xptar", &e_xptar, &b_e_xptar);
   fChain->SetBranchAddress("e_ytar", &e_ytar, &b_e_ytar);
   fChain->SetBranchAddress("e_xfp", &e_xfp, &b_e_xfp);
   fChain->SetBranchAddress("e_xpfp", &e_xpfp, &b_e_xpfp);
   fChain->SetBranchAddress("e_yfp", &e_yfp, &b_e_yfp);
   fChain->SetBranchAddress("e_ypfp", &e_ypfp, &b_e_ypfp);
   fChain->SetBranchAddress("e_deltai", &e_deltai, &b_e_deltai);
   fChain->SetBranchAddress("e_yptari", &e_yptari, &b_e_yptari);
   fChain->SetBranchAddress("e_xptari", &e_xptari, &b_e_xptari);
   fChain->SetBranchAddress("e_ytari", &e_ytari, &b_e_ytari);
   fChain->SetBranchAddress("h_delta", &h_delta, &b_h_delta);
   fChain->SetBranchAddress("h_yptar", &h_yptar, &b_h_yptar);
   fChain->SetBranchAddress("h_xptar", &h_xptar, &b_h_xptar);
   fChain->SetBranchAddress("h_ytar", &h_ytar, &b_h_ytar);
   fChain->SetBranchAddress("h_xfp", &h_xfp, &b_h_xfp);
   fChain->SetBranchAddress("h_xpfp", &h_xpfp, &b_h_xpfp);
   fChain->SetBranchAddress("h_yfp", &h_yfp, &b_h_yfp);
   fChain->SetBranchAddress("h_ypfp", &h_ypfp, &b_h_ypfp);
   fChain->SetBranchAddress("h_deltai", &h_deltai, &b_h_deltai);
   fChain->SetBranchAddress("h_yptari", &h_yptari, &b_h_yptari);
   fChain->SetBranchAddress("h_xptari", &h_xptari, &b_h_xptari);
   fChain->SetBranchAddress("h_ytari", &h_ytari, &b_h_ytari);
   fChain->SetBranchAddress("q", &q, &b_q);
   fChain->SetBranchAddress("nu", &nu, &b_nu);
   fChain->SetBranchAddress("Q2", &Q2, &b_Q2);
   fChain->SetBranchAddress("W", &W, &b_W);
   fChain->SetBranchAddress("epsilon", &epsilon, &b_epsilon);
   fChain->SetBranchAddress("Em", &Em, &b_Em);
   fChain->SetBranchAddress("Pm", &Pm, &b_Pm);
   fChain->SetBranchAddress("thetapq", &thetapq, &b_thetapq);
   fChain->SetBranchAddress("phipq", &phipq, &b_phipq);
   fChain->SetBranchAddress("corrsing", &corrsing, &b_corrsing);
   fChain->SetBranchAddress("Pmx", &Pmx, &b_Pmx);
   fChain->SetBranchAddress("Pmy", &Pmy, &b_Pmy);
   fChain->SetBranchAddress("Pmz", &Pmz, &b_Pmz);
   fChain->SetBranchAddress("PmPar", &PmPar, &b_PmPar);
   fChain->SetBranchAddress("PmPer", &PmPer, &b_PmPer);
   fChain->SetBranchAddress("PmOop", &PmOop, &b_PmOop);
   fChain->SetBranchAddress("fry", &fry, &b_fry);
   fChain->SetBranchAddress("radphot", &radphot, &b_radphot);
   fChain->SetBranchAddress("sigcc", &sigcc, &b_sigcc);
   fChain->SetBranchAddress("Weight", &Weight, &b_Weight);
   Notify();
}

Bool_t SNT::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void SNT::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t SNT::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef SNT_cxx
