
Double_t get_Ep(double *x, double *par)
{

  double rad2deg = 180./3.14159265359;

  double E_ep;
  
  //E0 par[0]
  //Mt par[1];
  Float_t theta_e = x[0];
  E_ep = par[0] * par[1] / (par[1] + 2.*par[0] * pow(sin(theta_e/(2.*rad2deg)), 2));   

  return E_ep*(-1.0);
  
}


void plot_elastic_energy()
{


  //amu to GeV/c2 conversion
  double amu2gev = 0.93149432;

 //Particle Masses [GeV]
  double m_e;                //electron
  double m_p;                //proton 
  double m_n;                //neutron
  double m_d;                //deuteron

  double m_c;
  double m_pb;
  
  //Beam Energy
  double Eb;
  
  //particle masses in amu
  m_c = 12.0;
  m_pb = 208.;
  Eb = 6.404;
  
  //Define Particle Masses
  //
  m_p = 0.938272;
  m_n = 0.939565;
  m_d = 1.875612;
  m_e = 0.000511;
  m_c = m_c*amu2gev; //GeV, Carbon mass
 
  
  
  
  TCanvas *T1 = new TCanvas ("T1","T1", 1000, 700);


  
  TF1 *f_H = new TF1("f_H", get_Ep, 25, 30., 2);
  TF1 *f_c = new TF1("f_c", get_Ep, 25., 30., 2);
  
  
  f_H->SetParameter(0, Eb);   //set initial beam energy [GeV]
  f_H->SetParameter(1, m_p);   //set target mass [GeV/c2]

  f_c->SetParameter(0, Eb);   //set initial beam energy [GeV]
  f_c->SetParameter(1, m_c);   //set target mass [GeV/c2]

  f_H->SetLineColor(kBlue);




  
  f_H->Draw();
  f_c->Draw("same");

  cout << "E_p at 27.5 deg: " << f_H->Eval(27.5) << endl;
  
  auto legend = new TLegend(0.1,0.7,0.48,0.9);
  legend->AddEntry("f_H","Hydrogen","l");
  legend->AddEntry("f_c","Carbon","l");
  legend->Draw();
}
