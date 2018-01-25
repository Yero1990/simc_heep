// run an analysis based on SNT.C the analyysis script for the simc n-tuple
//
// radiated results have a type offset of > 1000
//
// this script is setup for the proposed commissioning runs. the steps in pm are 0.15 GeV/c

void analyze_all_comm()
{
  gROOT->Reset();
  // gSystem->Load("libPhysics.so");
  gROOT->LoadMacro("SNT.C"); //compile if necessary

  // create a chain of output files

  // select analysis type
  Int_t type = 0;
  
  // strings for names :
  TString res_head = "comm_10.6_";
  TString pm = "pm_0.5";
  TString xbj="_1.35";
  //TString einc = "_11.00";
  TString einc = "_10.60";
  TString calc = ".pwia";

  Bool_t radiated = kTRUE;
  
  TString rad;
  TString ext = ".root";
  Int_t rad_offset = 0;

  // for Q2 4.25 calculations
  TString Q2="_4.25";

  // with radiation
  if (radiated)
    {rad = ".rad";}
  else
    {rad = ".norad";}

  TString datadir = "./worksim/";

  Int_t type_offset = 200;

  Double_t qmin = 4.0;
  Double_t qmax = 4.5;

  if (radiated) rad_offset = 1000;

  type_offset += rad_offset;
  
  // from *.hist file
  TChain *chain = new TChain("SNT");
  type = type_offset + 50;
  pm = "pm_0.50";
  TString file = datadir + pm + Q2 + xbj + einc + calc + rad + ext;
  cout << "Analyzing : " << file << " type = " << endl;

  /*
  chain->Add(file);
  SNT *simc = new SNT(&chain);
  simc->Loop(qmin, qmax, type, res_head); // do all e-p events
  chain->Reset();

  type = type_offset + 65;
  pm = "pm_0.65";
  file = datadir + pm + Q2 + xbj + einc + calc + rad + ext;
  cout << "Analyzing : " << file << endl;
  chain->Add(file);
  simc->Init(&chain);
  simc->Loop(qmin, qmax, type, res_head); // do all e-p events
  chain->Reset();

  type = type_offset + 80;
  pm = "pm_0.80";
  file = datadir + pm + Q2 + xbj + einc + calc + rad + ext;
  cout << "Analyzing : " << file << endl;
  chain->Add(file);
  simc->Init(&chain);
  simc->Loop(qmin, qmax, type, res_head); // do all e-p events
  chain->Reset();

  */




}


