//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Aug 10 15:24:53 2018 by ROOT version 5.34/36
// from TTree h666/SIMTUPLE
// found on file: current.root
//////////////////////////////////////////////////////////

#ifndef singles_gfor_h
#define singles_gfor_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class singles_gfor {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Float_t         hsdelta;
   Float_t         hsyptar;
   Float_t         hsxptar;
   Float_t         hsytar;
   Float_t         hsxfp;
   Float_t         hsxpfp;
   Float_t         hsyfp;
   Float_t         hsypfp;
   Float_t         hsdeltai;
   Float_t         hsyptari;
   Float_t         hsxptari;
   Float_t         hsytari;
   Float_t         ssdelta;
   Float_t         ssyptar;
   Float_t         ssxptar;
   Float_t         ssytar;
   Float_t         ssxfp;
   Float_t         ssxpfp;
   Float_t         ssyfp;
   Float_t         ssypfp;
   Float_t         ssdeltai;
   Float_t         ssyptari;
   Float_t         ssxptari;
   Float_t         ssytari;
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
   Float_t         Pmpar;
   Float_t         Pmper;
   Float_t         Pmoop;
   Float_t         fry;
   Float_t         radphot;
   Float_t         sigcc;
   Float_t         Weight;

   // List of branches
   TBranch        *b_hsdelta;   //!
   TBranch        *b_hsyptar;   //!
   TBranch        *b_hsxptar;   //!
   TBranch        *b_hsytar;   //!
   TBranch        *b_hsxfp;   //!
   TBranch        *b_hsxpfp;   //!
   TBranch        *b_hsyfp;   //!
   TBranch        *b_hsypfp;   //!
   TBranch        *b_hsdeltai;   //!
   TBranch        *b_hsyptari;   //!
   TBranch        *b_hsxptari;   //!
   TBranch        *b_hsytari;   //!
   TBranch        *b_ssdelta;   //!
   TBranch        *b_ssyptar;   //!
   TBranch        *b_ssxptar;   //!
   TBranch        *b_ssytar;   //!
   TBranch        *b_ssxfp;   //!
   TBranch        *b_ssxpfp;   //!
   TBranch        *b_ssyfp;   //!
   TBranch        *b_ssypfp;   //!
   TBranch        *b_ssdeltai;   //!
   TBranch        *b_ssyptari;   //!
   TBranch        *b_ssxptari;   //!
   TBranch        *b_ssytari;   //!
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
   TBranch        *b_Pmpar;   //!
   TBranch        *b_Pmper;   //!
   TBranch        *b_Pmoop;   //!
   TBranch        *b_fry;   //!
   TBranch        *b_radphot;   //!
   TBranch        *b_sigcc;   //!
   TBranch        *b_Weight;   //!

   singles_gfor(TTree *tree=0);
   virtual ~singles_gfor();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef singles_gfor_cxx
singles_gfor::singles_gfor(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
     
     string simc_file = "hms_single_1272_gfor.root";   //added
     string simc_file_path = "../worksim_voli/"+simc_file;
     TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(simc_file_path.c_str());
      if (!f || !f->IsOpen()) {
         f = new TFile(simc_file_path.c_str());
      }
      f->GetObject("h666",tree);

   }
   Init(tree);
}

singles_gfor::~singles_gfor()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t singles_gfor::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t singles_gfor::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void singles_gfor::Init(TTree *tree)
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

   fChain->SetBranchAddress("hsdelta", &hsdelta, &b_hsdelta);
   fChain->SetBranchAddress("hsyptar", &hsyptar, &b_hsyptar);
   fChain->SetBranchAddress("hsxptar", &hsxptar, &b_hsxptar);
   fChain->SetBranchAddress("hsytar", &hsytar, &b_hsytar);
   fChain->SetBranchAddress("hsxfp", &hsxfp, &b_hsxfp);
   fChain->SetBranchAddress("hsxpfp", &hsxpfp, &b_hsxpfp);
   fChain->SetBranchAddress("hsyfp", &hsyfp, &b_hsyfp);
   fChain->SetBranchAddress("hsypfp", &hsypfp, &b_hsypfp);
   fChain->SetBranchAddress("hsdeltai", &hsdeltai, &b_hsdeltai);
   fChain->SetBranchAddress("hsyptari", &hsyptari, &b_hsyptari);
   fChain->SetBranchAddress("hsxptari", &hsxptari, &b_hsxptari);
   fChain->SetBranchAddress("hsytari", &hsytari, &b_hsytari);
   fChain->SetBranchAddress("ssdelta", &ssdelta, &b_ssdelta);
   fChain->SetBranchAddress("ssyptar", &ssyptar, &b_ssyptar);
   fChain->SetBranchAddress("ssxptar", &ssxptar, &b_ssxptar);
   fChain->SetBranchAddress("ssytar", &ssytar, &b_ssytar);
   fChain->SetBranchAddress("ssxfp", &ssxfp, &b_ssxfp);
   fChain->SetBranchAddress("ssxpfp", &ssxpfp, &b_ssxpfp);
   fChain->SetBranchAddress("ssyfp", &ssyfp, &b_ssyfp);
   fChain->SetBranchAddress("ssypfp", &ssypfp, &b_ssypfp);
   fChain->SetBranchAddress("ssdeltai", &ssdeltai, &b_ssdeltai);
   fChain->SetBranchAddress("ssyptari", &ssyptari, &b_ssyptari);
   fChain->SetBranchAddress("ssxptari", &ssxptari, &b_ssxptari);
   fChain->SetBranchAddress("ssytari", &ssytari, &b_ssytari);
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
   fChain->SetBranchAddress("Pmpar", &Pmpar, &b_Pmpar);
   fChain->SetBranchAddress("Pmper", &Pmper, &b_Pmper);
   fChain->SetBranchAddress("Pmoop", &Pmoop, &b_Pmoop);
   fChain->SetBranchAddress("fry", &fry, &b_fry);
   fChain->SetBranchAddress("radphot", &radphot, &b_radphot);
   fChain->SetBranchAddress("sigcc", &sigcc, &b_sigcc);
   fChain->SetBranchAddress("Weight", &Weight, &b_Weight);
   Notify();
}

Bool_t singles_gfor::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void singles_gfor::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t singles_gfor::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef singles_gfor_cxx
