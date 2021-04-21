#include <TMatrixD.h>
#include <TVectorD.h>
#include <TDecompSVD.h>

//This code uses equations W, Em and Pmx, Pmz to sole a linear system to determine the
//parameters dEf/Ef and dth_e that minimizes the observed and predicted quantities

//****IMPORTANT*** :: ONLY PARAMETERS a0, a1, a2 --> dEb/Eb,  dEf/Ef,  dth_e are considered

void heep_svd()
{

  ifstream ifile;


  
  int runs[4] = {3288, 3371, 3374, 3377};

  //Define the variables
  Double_t dtr = TMath::Pi() / 180.;
  Double_t Mp = 0.938272;
  Double_t Eb = 10.6005;
  Double_t Ef = 8.5342488;
  Double_t th_e[4] = {12.194, 13.930, 9.928, 8.495};
  Double_t ph_e = 0;

  //Define the measured DATA, SIMC variables
  Double_t Wdata[4] = {9.42152e-01, 9.40229e-01, 9.43825e-01, 9.55773e-01};
  Double_t Wdata_err[4] = {9.71583e-05,  3.77469e-04, 6.01623e-05, 4.13486e-05};

  Double_t Wsimc[4] = {9.44690e-01, 9.44480e-01, 9.43591e-01, 9.42933e-01};
  Double_t Wsimc_err[4] = {1.38566e-04, 6.68696e-05,  2.81437e-05, 2.23158e-05};

  Double_t Emdata[4] = {6.58010e-03, 5.28983e-03, 6.26642e-03, 5.32021e-03};
  Double_t Emdata_err[4] = {4.69566e-05,  1.47900e-04,  3.65940e-05, 4.14000e-05}; 

  Double_t Emsimc[4] = {5.51066e-03, 6.06886e-03, 6.58531e-03, 6.47356e-03};
  Double_t Emsimc_err[4] = {9.05496e-05, 3.84833e-05,  1.74941e-05, 1.37620e-05}; 

  Double_t PmXdata[4] = {-1.29097e-03, -9.93560e-04, -5.53961e-04, 7.13300e-03};
  Double_t PmXdata_err[4] = {3.30069e-05, 1.75464e-04, 2.87979e-05, 2.08787e-05};

  Double_t PmXsimc[4] = {-5.88228e-04, -5.91870e-04, -6.80114e-04, -1.02935e-03};
  Double_t PmXsimc_err[4] = {7.19549e-05, 2.48719e-05,  2.31853e-05, 1.11287e-05};

  Double_t PmZdata[4] = {5.77521e-03, 5.42333e-03, 5.33585e-03, 3.43146e-03};
  Double_t PmZdata_err[4] = {7.85239e-05, 1.13337e-04, 3.97398e-05, 3.45173e-05};

  Double_t PmZsimc[4] = {5.82773e-03, 6.50794e-03, 6.35112e-03, 6.02246e-03};
  Double_t PmZsimc_err[4] = {1.06200e-04, 2.89548e-05, 1.55699e-05, 1.62459e-05};



  //Define derivatives ( and matrix coefficients)
  Double_t dW_dEb, dW_dEf, dW_dthe, dEm_dEb, dEm_dEf, dPmx_dEf, dPmx_dthe, dPmz_dEb, dPmz_dEf, dPmz_dthe;
  
  Double_t C00=0;   //Cij-> ith row, jth column
  Double_t C01=0;
  Double_t C02=0;
  Double_t C10=0;
  Double_t C11=0; 
  Double_t C12=0;
  Double_t C20=0;
  Double_t C21=0; 
  Double_t C22=0;
  Double_t C30=0;
  Double_t C31=0; 
  Double_t C32=0; 


  //Define the parameters to be minimized
  Double_t a0, a1, a2;   //a0 --> dEb/Eb,  a1 --> dEf/Ef,  a2 --> dth_e [rad]

  //Define the observed quantities
  Double_t dW_obs;
  Double_t dEm_obs;
  Double_t dPmx_obs;
  Double_t dPmz_obs;

  //Define the measured observed errors
  Double_t dW_obs_err2;
  Double_t dEm_obs_err2;
  Double_t dPmx_obs_err2;
  Double_t dPmz_obs_err2;

  //Input the observed variables

  Double_t err_fact = 1.;  //make the errors more reasonable

  //Loop over each run
  for (int i=0; i<3; i++)
    {

      cout << "Analyzing Run " << runs[i] << endl;
      
      //Convert to radians
      th_e[i] = th_e[i] * dtr;

      
      //Calculate the observed differences bewteen DATA and SIMC
      dW_obs += Wdata[i] - Wsimc[i];      //the '+' is to sum over all observed runs
      dEm_obs += Emdata[i] - Emsimc[i];
      dPmx_obs += PmXdata[i] - PmXsimc[i];
      dPmz_obs += PmZdata[i] - PmZsimc[i];

      dW_obs_err2 += 1./(pow(Wdata_err[i]*err_fact,2) + pow(Wsimc_err[i]*err_fact, 2));   //the '+' is to sum over all errors squared
      dEm_obs_err2 +=1./( pow(Emdata_err[i]*err_fact,2) + pow(Emsimc_err[i]*err_fact, 2));
      dPmx_obs_err2 +=1./( pow(PmXdata_err[i]*err_fact,2) + pow(PmXsimc_err[i]*err_fact, 2));
      dPmz_obs_err2 +=1./( pow(PmZdata_err[i]*err_fact,2) + pow(PmZsimc_err[i]*err_fact, 2));
      
      //Calculate the derivatives for each run (and the coefficients)
      dW_dEb = Ef / Eb;
      C00 += dW_dEb * Eb;

      dW_dEf = - Eb / Ef;
      C01 += dW_dEf * Ef;   //the '+' is to increment coefficient (sum over all runs)

      dW_dthe = -2.*Eb*Ef/Mp * sin(th_e[i]/2.) * cos(th_e[i]/2.);
      C02 += dW_dthe;

      dEm_dEb = 1;
      C10 += dEm_dEb * Eb;

      dEm_dEf = -1;
      C11 += dEm_dEf * Ef;

      C12 += 0.;

      C20 += 0.;

      dPmx_dEf = -sin(th_e[i]);
      C21 += dPmx_dEf * Ef;

      dPmx_dthe = -Ef*cos(ph_e)*cos(th_e[i]);
      C22 += dPmx_dthe;

      dPmz_dEb = 1.;
      C30 += dPmz_dEb *Eb;

      dPmz_dEf = -cos(th_e[i])*cos(ph_e);
      C31 += dPmz_dEf * Ef;

      dPmz_dthe = Ef*cos(ph_e)*sin(th_e[i]);
      C32 += dPmz_dthe;

      
      cout << "C00 = " << C00 << endl;
      cout << "C01 = " << C01 << endl;
      cout << "C02 = " << C02 << endl;
      cout << "C10 = " << C10 << endl;
      cout << "C11 = " << C11 << endl;
      cout << "C12 = " << C12 << endl;
      cout << "C20 = " << C20 << endl;
      cout << "C21 = " << C21 << endl;
      cout << "C22 = " << C22 << endl;
      cout << "C30 = " << C30 << endl;
      cout << "C31 = " << C31 << endl;
      cout << "C32 = " << C32 << endl;

      cout << "dW_obs = " << dW_obs << " +/- sqrt " << dW_obs_err2 << endl;
      cout << "dEm_obs = " << dEm_obs << " +/- sqrt" << dEm_obs_err2 <<endl;
      cout << "dPmx_obs = " << dPmx_obs << " +/- sqrt" << dPmx_obs_err2 <<endl;
      cout << "dPmz_obs = " << dPmz_obs << " +/- sqrt" << dPmz_obs_err2 <<endl;

    }

  
  //Create Matrix and vectors to store the summed elements
  //General Form: [ m x n ] [n] =  [m],  where [mxn] -> has the coefficients Cij, [n] -> column vector (a1,a2)^Transpose 
  //with param to be minimized and [m] is a column vector of length m that carries the obsereved quantities

  TMatrixD C(4,3);
  TVectorD bVec(4);
  TVectorD bVec_cpy(4);

  TVectorD aVec(3);  //vector to store the parameters (a0, a1, a2)
  TVectorD eVec(4);   //vector to store residuals

  C.Zero();
  bVec.Zero();
  bVec_cpy.Zero();
  aVec.Zero();
  eVec.Zero();

  //Fill the matrix
  C[0][0] = C00;
  C[0][1] = C01;
  C[0][2] = C02;
  C[1][0] = C10;
  C[1][1] = C11;
  C[1][2] = C12;
  C[2][0] = C20;
  C[2][1] = C21;
  C[2][2] = C22;
  C[3][0] = C30;
  C[3][1] = C31;
  C[3][2] = C32;

  //Fill the observed vector
  bVec[0] = dW_obs;
  bVec[1] = dEm_obs;
  bVec[2] = dPmx_obs;
  bVec[3] = dPmz_obs;
  
  //Fill copy of the observed vector (to be used to solve linear system)
  bVec_cpy[0] = dW_obs;
  bVec_cpy[1] = dEm_obs;
  bVec_cpy[2] = dPmx_obs;
  bVec_cpy[3] = dPmz_obs;
  
  //Single - Value Decomposition
  TDecompSVD svd_decomp(C);
  bool ok = svd_decomp.Decompose();
  cout << "decomposition ok? " << ok << endl;
  ok = svd_decomp.Solve(bVec_cpy);  //the solution x = A^-1 *bVec is retured in the bVec as: x[0] = bVec[0] . . .in the first n elements of the original  (m x n)

  cout << "solving linear system ok? " << ok << endl;

  //Get the Optimized Parameters
  aVec[0] = bVec_cpy[0];   //dEb / Eb
  aVec[1] = bVec_cpy[1];  //dEf / Ef
  aVec[2] = bVec_cpy[2];   //dth_e



  // [C] * aVec = bVec + e -> this is the  general linear equation with residual e,
  //  e = [C] * aVec - bVec   ::  e is a vector with the residuals
  eVec[0] = C[0][0] * aVec[0] + C[0][1] * aVec[1] + C[0][2] * aVec[2] - bVec[0];
  eVec[1] = C[1][0] * aVec[0] + C[1][1] * aVec[1] + C[1][2] * aVec[2] - bVec[1];
  eVec[2] = C[2][0] * aVec[0] + C[2][1] * aVec[1] + C[2][2] * aVec[2] - bVec[2];
  eVec[3] = C[3][0] * aVec[0] + C[3][1] * aVec[1] + C[3][2] * aVec[2] - bVec[3];

  TMatrixD N_inv(4,4);   //diagonal elements are 1/sig_meas**2, (inverse of the covariance matrix for measured errors, assumed measured errors are independent)
  N_inv.Zero();
 
  N_inv[0][0] = dW_obs_err2;
  N_inv[1][1] = dEm_obs_err2;
  N_inv[2][2] = dPmx_obs_err2;
  N_inv[3][3] = dPmz_obs_err2;

  //Get Chi2
  //chi2 = eVec^T * N^-1 * eVec,  where N^-1 are the diagonal matrix observed errors {1/sig**2} ==> chi2 = eVec * eVec / sig**2
  Double_t chi2 = eVec[0]*eVec[0] * N_inv[0][0] + eVec[1]*eVec[1] * N_inv[1][1] + eVec[2]*eVec[2] * N_inv[2][2] + eVec[3]*eVec[3] * N_inv[3][3];
  cout << "chi2 = " <<chi2 << endl;

 
  TMatrixD R1(4,3);
  R1.Zero();

  //  V_inv = ROOT::Math::SimilarityT(C, N_inv);  //THis calculates: Matrix Product V = (C^{T} * N_inv *C) = C^{T} * R1

  for(int i=0; i<4; i++){

    for(int j=0; j<3; j++){
      
      R1[i][j] = 0.;  //reset to sum over new elements
      
      for(int k=0; k<4; k++)
	{
	  R1[i][j] += N_inv[i][k] * C[k][j];
	}

    }

  }

  N_inv.Print();
  C.Print();
  R1.Print();
  
  TMatrixD CT(3,4);
  CT.Transpose(C);
  CT.Print();
    
  TMatrixD V(3,3);
  V.Zero();

  //Get Covariance Matrix:  3 x 3 =  CT(3x4) * R1(4x3)
  for(int i=0; i<3; i++){

    for(int j=0; j<3; j++){
      
      V[i][j] = 0.;  //reset to sum over new elements
      
      for(int k=0; k<4; k++)
	{
	  V[i][j] += CT[i][k] * R1[k][j];
	}

    }

  }

  V.Print();




  TMatrixD Vinv(3,3);
  Vinv.Zero();
  Vinv = V.Invert();


  
   
  //INvert V matrix
  cout << "****COVARIANCE MATRIX*****" << endl;

  Vinv.Print();
  

  double chi2dof = chi2 / 9.;
  cout << "---Optimized Parameters---" << endl;
  cout << "total equations x total runs = 4x3 = 12 observations, # parameters = 3, dof = 9 ";
  cout << "chi2 = " << chi2 << "  chi2/dof = " << chi2dof << endl;
  cout << "dEb / Eb = " << aVec[0] << endl;
  cout << "dEf / Ef = " << aVec[1] << endl;
  cout << "dth_e = "    << aVec[2] << endl;


  //Get Parameter Errors
  Double_t a0_err, a1_err, a2_err;

  a0_err = sqrt(Vinv[0][0]);    //uncertaintiy in dEb/Eb offset
  a1_err = sqrt(Vinv[1][1]);    //uncertaintiy in dEb/Eb offset
  a2_err = sqrt(Vinv[2][2]);    //uncertaintiy in dEb/Eb offset

  cout << "=== Uncertainty in Parameters ===" << endl;
  cout << "dEb / Eb = " << a0_err << endl;
  cout << "dEf / Ef = " << a1_err << endl;
  cout << "dth_e [rad] = " << a2_err << endl;

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
