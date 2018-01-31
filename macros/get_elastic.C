void get_elastic(TString target, Double_t Ebeam, Double_t theta_elec)
{


  double rad2deg = 180./3.14159265359;
  
  //units: energy: GeV, angle: degree,  set speed of light c = 1.  


  //Electron Kinematics
  double Q2;           //4-momentum transfer squared
  double E_beam;               //INITIAL e- beam energy
  double E_ep;              //SCATTERED e- beam energy
  double ki;               //initial e- 3-momentum magnitude
  double kf;              //scattered e- 3-momentum magnitude 
  double theta_e;           //e- scattering angle
  double w;                 //energy transfer

  //Proton Kinematics
  double E_p;                //scattered proton energy
  double p_p;               //scattered proton momentum magnitude
  double theta_p;           //proton scattering angle
  double theta_pq;           //proton angle relative to virtual photon 3-vec |q|
  double theta_q;            //virtual photon exchanged angle relative to beam (+z)
  double W;                 //invariant mass
  double q;                 //3-momentum transfer (virtual photon) magnitude |q|

  //Particle Masses [GeV]
  double m_e;                //electron
  double m_p;                //proton 
  double m_n;                //neutron
  double m_d;                //deuteron
  double m_c;
  double m_targ;
  double x_bj;               //B-jorken scale
  
  //Define Particle Masses in GeV/c2
  m_p = 0.938272;
  m_n = 0.939565;
  m_d = 1.875612;
  m_e = 0.000511;
  m_c = 11.187898;
 
  
  if(target=="hydrogen") {m_targ = m_p;}
  else if(target=="carbon") {m_targ = m_c;}

  E_beam = Ebeam;
  theta_e = theta_elec;

TString ofile = target+"_elastics_kin.dat";
  ofstream ofs;
  ofs.open(ofile);

  //in elastic scattering, e-scatt energy is fixed by e-scatt angle
  E_ep = E_beam * m_targ / (m_targ + 2.*E_beam * pow(sin(theta_e/(2.*rad2deg)), 2));   
  
  w = E_beam - E_ep;

  Q2 = 4. * E_beam * E_ep * pow(sin(theta_e/(2.*rad2deg)),2);

  x_bj = Q2 / (2. * m_targ * w);

  ki = sqrt(pow(E_beam,2) - pow(m_e,2));

  kf = sqrt(pow(E_ep,2) - pow(m_e,2));

  E_p = E_beam + m_targ - E_ep;

  p_p = sqrt(pow(E_p,2) - pow(m_targ,2));
  
  theta_p = asin( kf * sin(theta_e/rad2deg) / p_p )  ;


  //Cout results to screen
  cout << "*****" << target + "Elastics ! ***** " << endl;
  cout << "*************************" << endl;
  cout << " Electron Kinematics " << endl;
  cout << "*************************" << endl;
  cout << "Beam Energy: " << E_beam << " GeV" << endl;
  cout << "e- Scattered Energy = " << E_ep << " GeV" << endl;
  cout << "e- Scattered Angle = " << theta_e << " deg " << endl;
  cout << "Energy Transfer (E-E') = " << w  << " GeV" << endl;		 
  cout << "Q2 = " << Q2 << " GeV2" << endl;		 
  cout << "B-jorken X = " << x_bj << endl; 
  cout << "Beam Momentum = " << ki << " GeV/c" << endl;		 
  cout << "e-Scattered Momentum = " << kf << " GeV/c" << endl;		 
  cout << "*************************" << endl;
  cout << " \n";
  cout << "*************************" << endl;
  cout << " Proton Kinematics " << endl;
  cout << "*************************" << endl;
  cout << "p-Scattered Energy = " << E_p << " GeV" << endl;		 
  cout << "p-Scattered Momentum = " << p_p << " GeV/c" << endl;		 
  cout << "p-Scattered Angle = " << theta_p *rad2deg<< " deg " << endl;
  cout << "*************************q" << endl;

  //Write results to a file
  ofs << "*****" << target + "Elastics ! ***** " << endl;
  ofs << "*************************" << endl;
  ofs << " Electron Kinematics " << endl;
  ofs << "*************************" << endl;
  ofs << "Beam Energy: " << E_beam << " GeV" << endl;
  ofs << "e- Scattered Energy = " << E_ep << " GeV" << endl;
  ofs << "e- Scattered Angle = " << theta_e << " deg " << endl;
  ofs << "Energy Transfer (E-E') = " << w  << " GeV" << endl;		 
  ofs << "Q2 = " << Q2 << " GeV2" << endl;		 
  ofs << "B-jorken X = " << x_bj << endl; 
  ofs << "Beam Momentum = " << ki << " GeV/c" << endl;		 
  ofs << "e-Scattered Momentum = " << kf << " GeV/c" << endl;		 
  ofs << "*************************" << endl;
  ofs << " \n";
  ofs << "*************************" << endl;
  ofs << " Proton Kinematics " << endl;
  ofs << "*************************" << endl;
  ofs << "p-Scattered Energy = " << E_p << " GeV" << endl;		 
  ofs << "p-Scattered Momentum = " << p_p << " GeV/c" << endl;		 
  ofs << "p-Scattered Angle = " << theta_p *rad2deg<< " deg " << endl;
  ofs << "*************************q" << endl;

  
  
  
}
