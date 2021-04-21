#include <TMatrixD.h>
#include <TVectorD.h>
#include <TDecompSVD.h>


//This code uses HMS singles Elastic data taken from Kaon LT - Fall 2018 to minimize chi2 for kinematical offsets determination 
//MODEL 1:  This model useds derivatives of W in terms of Eb, Ef, th_e to describe any changes observed in the obsereved dW=data-simc

void heep_svd_hms_model1()
{

  ifstream ifile;

  const int runs = 11;
  const int sel = 11;
  
  int run[runs] = {6595, 6601, 6602, 6609, 6611, 6634, 6871, 6875, 6876, 6879, 6881};
  
  //Define the variables
  Double_t dtr = TMath::Pi() / 180.;
  Double_t Mp = 0.938272;
  Double_t Eb[runs] = {3.83350, 3.83350, 3.83350, 3.83350, 3.83350, 3.83350, 4.93090, 4.93090, 4.93090, 4.93090, 4.93090};
  
  Double_t Ef[runs] = {3.007, 2.834, 2.713, 2.583, 2.478, 2.026, 4.371, 4.184, 3.923, 2.583, 3.124};	      
  Double_t th_e[runs] = {21.12, 23.980, 25.970, 29.185, 29.985, 38.60, 12.710, 15.00, 18.02, 34.23, 27.17};  

  //assume out of plane angles in e- and p are zero. (variations are only due to in-plane quantities)
  Double_t ph_e = 0;  

  //Define the measured DATA, SIMC variables
  Double_t Wdata[runs] = {
    0.936719, 
    0.935986, 
    0.93657,  
    0.936574, 
    0.936519, 
    0.939933, 
    0.927885, 
    0.929253, 
    0.928928, 
    0.931178, 
    0.929532 
  };
  
  Double_t Wdata_err[runs] = {
    4.99386e-05,
    6.69785e-05,
    5.48703e-05,
    6.96459e-05,
    6.25866e-05,
    0.000157911,
    5.70411e-05,
    8.05051e-05,
    7.80849e-05,
    0.000290841,
    0.00026315 
  };
  
  Double_t Wsimc[runs] = {
    0.942444, 
    0.942659, 
    0.942793, 
    0.942835, 
    0.942942, 
    0.943459, 
    0.942053, 
    0.94229,  
    0.942498, 
    0.943798, 
    0.943418 
  };
  
  Double_t Wsimc_err[runs] = {
    2.77374e-05,	
    2.95417e-05,	
    3.03856e-05,	
    3.14021e-05,	
    3.20503e-05,	
    3.45032e-05,	
    2.97292e-05,	
    3.22833e-05,	
    3.53766e-05,	
    5.19906e-05,	
    4.1672e-05 	
  };

  //Define derivatives ( and matrix coefficients)
  Double_t dW_dEb, dW_dEf, dW_dthe;

  //Create Matrix and vectors to store the summed elements
  //General Form: [ m x n ] [n] =  [m],  where [mxn] -> has the coefficients Cij, [n] -> column vector (a1,a2)^Transpose 
  //with param to be minimized and [m] is a column vector of length m that carries the obsereved quantities

  //General Equation:  C * aVec = bVec + eVec

  //STEP1: SOLVE linear least squares via SVD
  
  TMatrixD C(sel,3);     //11 runs, 11 equations, 3 parameters
  TVectorD bVec(sel);     //observed vector
  TVectorD bVec_err(sel);     //observed vector
  
  TVectorD bVec_cpy(sel);  //copy
  
  TVectorD aVec(3);  //vector to store the parameters (a0, a1, a2)
  TVectorD eVec(sel);   //vector to store residuals
  TVectorD eVec_err(sel);   //vector to store residuals error

  TMatrixD N_inv(sel,sel);   //diagonal elements are 1/sig_meas**2, (inverse of the covariance matrix for measured errors, assumed measured errors are independent)
  
  //Define the parameters to be minimized
  Double_t a0, a1, a2;   //a0 --> dEb/Eb,  a1 --> dEf/Ef,  a2 --> dth_e [rad]

  //Define the observed quantities
  Double_t dW_obs;

  //Define the observed errors
  Double_t dW_obs_err2;
 
  //Systematic Error from Magnet CYcling PROCEDURE (Estimated to be 0.1 MeV or dEf/Ef = 1e-4 GeV)
  Double_t p_syst = 1e-4;

  //Input the observed variables
  //Loop over each run
  for (int i=0; i<sel; i++)
    {

      cout << "Analyzing Run " << run[i] << endl;
      
      //Convert to radians
      th_e[i] = th_e[i] * dtr;

      
      //Calculate the observed differences bewteen DATA and SIMC
      dW_obs = Wdata[i] - Wsimc[i];      
      
      //Calculate errors of the observed differences bewteen DATA and SIMC
      dW_obs_err2 = pow(Wdata_err[i],2) + pow(Wsimc_err[i], 2) + pow(p_syst*Ef[i], 2);  

      
      //Calculate the derivatives for each run (and the coefficients)

      //---Equation 1---
      dW_dEb = Ef[i] / Eb[i];
      C[i][0] = dW_dEb * Eb[i];

      dW_dEf = - Eb[i] / Ef[i];
      C[i][1] = dW_dEf * Ef[i];  

      dW_dthe = -2.*Eb[i]*Ef[i]/Mp * sin(th_e[i]/2.) * cos(th_e[i]/2.);
      C[i][2] = dW_dthe;
      
      
      //Fill the observed vector
      bVec[i] = dW_obs;
      bVec_err[i] = sqrt(dW_obs_err2); 
      
      //Fill copy of the observed vector (to be used to solve linear system)
      bVec_cpy[i] = dW_obs;
      
      N_inv[i][i] = 1./dW_obs_err2;
      
      
      cout << "dW_obs = " << dW_obs << " +/-  " << sqrt(dW_obs_err2) << endl;

      
    }
  
  
  C.Print();
  bVec.Print();
  N_inv.Print();
  //Single - Value Decomposition
  TDecompSVD svd_decomp(C);
  bool ok = svd_decomp.Decompose();
  cout << "decomposition ok? " << ok << endl;
  ok = svd_decomp.Solve(bVec_cpy);  //the solution x = A^-1 *bVec is retured in the bVec as: x[0] = bVec[0] . . .in the first n elements of the original  (m x n)
  
  cout << "solving linear system ok? " << ok << endl;
  bVec_cpy.Print();
  //Get the Optimized Parameters
  aVec[0] = bVec_cpy[0];   //dEb / Eb
  aVec[1] = bVec_cpy[1];  //dEf / Ef
  aVec[2] = bVec_cpy[2];  //dth_e
  


  TMatrixD R1(sel,3);
  R1.Zero();

  // R1 = Ninv(11x11) * C(11x3)

  for(int i=0; i<sel; i++){

    for(int j=0; j<3; j++){
      
      R1[i][j] = 0.;  //reset to sum over new elements
      
      for(int k=0; k<sel; k++)
	{
	  R1[i][j] += N_inv[i][k] * C[k][j];
	}

    }

  }

  N_inv.Print();
  C.Print();
  R1.Print();
  
  TMatrixD CT(3,sel);
  CT.Transpose(C);
  CT.Print();
    
  TMatrixD V(3,3);
  V.Zero();

  //V(3x3) =  CT(3x11) * R1(11x3)
  for(int i=0; i<3; i++){

    for(int j=0; j<3; j++){
      
      V[i][j] = 0.;  //reset to sum over new elements
      
      for(int k=0; k<sel; k++)
	{
	  V[i][j] += CT[i][k] * R1[k][j];
	}

    }

  }

  V.Print();




  TMatrixD Vinv(3,3);
  Vinv.Zero();
  Vinv = V.Invert();

  //Get Parameter Errors
  Double_t a0_err, a1_err, a2_err;
  
  a0_err = sqrt(Vinv[0][0]);    //uncertaintiy in dEb/Eb offset
  a1_err = sqrt(Vinv[1][1]);    //uncertaintiy in dEf/Ef offset
  a2_err = sqrt(Vinv[2][2]);    //uncertaintiy in dth_e offset


  //Write residuals to file
  ofstream ofs;
  ofs.open("hms_residuals_model1.txt");
  ofs << "#!run[i,0]/   x[i,1]/    residual[f,2]/    residual_err[f,3]/   dW_obs[f,4]/   dW_obs_err[f,5]/    dW_model[f,6]/    dW_model_err[f,7]/\n";
  
  Double_t dW_model, dW_model_err, residual, residual_err;
  Double_t chi2_init, chi2dof_init; 
  Double_t chi2, chi2dof; 
  chi2=0;
  chi2_init=0;

  for(int i=0; i<sel; i++)
    {

      //Model (Using otimized parameters as input)
      dW_model = C[i][0] * aVec[0] + C[i][1] * aVec[1] + C[i][2] * aVec[2];
      dW_model_err = sqrt( pow(C[i][0] * a0_err, 2) + pow(C[i][1] * a1_err,2) + pow(C[i][2] * a2_err, 2) 
			   + 2.*C[i][0]*C[i][1]*Vinv[0][1] + 2.*C[i][0]*C[i][2]*Vinv[0][2] + 2.*C[i][1]*C[i][2]*Vinv[1][2] ); //correlated errors

      //initial chi2 is obtained by setting the parameters to zero --> eVec = -bVEc
      chi2_init += bVec[i]*bVec[i]*N_inv[i][i];

      //Residuals: data - model
      eVec[i] = bVec[i] - dW_model;

      //Residuals Errors: sqrt(data_rr**2 + model_err**2)
      eVec_err[i] = sqrt( pow(bVec_err[i], 2) + pow(dW_model_err, 2) );


      //final chi2
      chi2 +=  eVec[i]*eVec[i]*N_inv[i][i];   //sum over all chi2's

      ofs << run[i] << "  " << i << "   " << eVec[i] << "   " << eVec_err[i] << "   " << bVec[i] << "   " << bVec_err[i] << "   " << dW_model << "   " << dW_model_err << endl;

    }

  ofs.close();


  chi2dof_init = chi2_init/8.;
  chi2dof = chi2/8.;

   
  cout << "****COVARIANCE MATRIX*****" << endl;

  Vinv.Print();
  

  cout << "=== Optimized Parameters ===" << endl;
  cout << "total equations = 11 observations, # parameters = 3, dof = 8 " << endl;
  cout << "initial chi2 = " << chi2_init << " initial chi2/dof = " << chi2dof_init << endl;
  cout << "chi2 = " << chi2 << "  chi2/dof = " << chi2dof << endl;
  cout << "dEb / Eb = " << aVec[0] << endl;
  cout << "dEf / Ef = " << aVec[1] << endl;
  cout << "dth_e = "    << aVec[2] << endl;


  cout << "=== Uncertainty in Parameters (Diagonal Elements) ===" << endl;
  cout << "dEb / Eb = " << a0_err << endl;
  cout << "dEf / Ef = " << a1_err << endl;
  cout << "dth_e [rad] = " << a2_err << endl;

  cout << "=== Uncertainty in Parameters (Off-Diagonal Elements) ===" << endl;
  cout << "cov[0,1] (Eb,Ef) = " << Vinv[0][1] << endl;
  cout << "cov[0,2] (Eb, th_e) = " << Vinv[0][2] << endl;
  cout << "cov[1,2] (Ef, th_e) = " << Vinv[1][2] << endl;
  
  //Get the Correlation Matrix
  //For covariance matrix, Sij, where the diagonals are Si **2, 

  //The correlation matrix is obtained by:  Cm_ij = S_ij / (Si * Sj), where Si, Sj are diagonal terms corresponding to (irow,icol), (jrow,jcol)
  //The diagonals are then:  Cm_ii = S_ii / (Si*Si) = 1 (100% correlation with itself)

  
  TMatrixD Cm(3,3);
  Cm.Zero();

  for(int i=0; i<3; i++){

    for(int j=0; j<3; j++){

      Cm[i][j] =  Vinv[i][j] / (sqrt(Vinv[i][i]) * sqrt(Vinv[j][j]) );

    }

  }


  cout << "****CORRELATION MATRIX*****" << endl;

  Cm.Print();




}
