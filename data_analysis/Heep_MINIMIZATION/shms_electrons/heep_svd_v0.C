#include <TMatrixD.h>
#include <TVectorD.h>
#include <TDecompSVD.h>

//This code uses equations W, Em and Pmx, Pmz to sole a linear system to determine the
//parameters dEf/Ef and dth_e that minimizes the observed and predicted quantities

//****IMPORTANT*** :: ONLY PARAMETERS a0, a1, a2 --> dEb/Eb,  dEf/Ef,  dth_e
//Here,  the 12 equations aro treated separately, NOT summed over all runs.

void heep_svd_v0()
{

  ifstream ifile;


  
  int runs[4] = {3288, 3371, 3374, 3377};

  //Define the variables
  Double_t dtr = TMath::Pi() / 180.;
  Double_t Mp = 0.938272;
  Double_t Eb = 10.6005;
  Double_t Ef = 8.5342488;
  Double_t Pp[4] = {2.935545, 3.47588, 2.310387, 1.891227};
  Double_t th_e[4] = {12.194, 13.930, 9.928, 8.495};
  Double_t th_p[4] = {37.338, 33.545, 42.9, 47.605};
  //assume out of plane angles in e- and p are zero. (variations are only due to in-plane quantities)
  Double_t ph_e = 0;  
  Double_t ph_p = 0;

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
  Double_t dW_dEb, dW_dEf, dW_dthe, dEm_dEb, dEm_dEf, dEm_dPp, dPmx_dEf, dPmx_dthe, dPmx_dPp, dPmx_dthp, dPmz_dEb, dPmz_dEf, dPmz_dthe, dPmz_dPp, dPmz_dthp;

  //Create Matrix and vectors to store the summed elements
  //General Form: [ m x n ] [n] =  [m],  where [mxn] -> has the coefficients Cij, [n] -> column vector (a1,a2)^Transpose 
  //with param to be minimized and [m] is a column vector of length m that carries the obsereved quantities

  //General Equation:  C * aVec = bVec + eVec

  //STEP1: SOLVE linear least squares via SVD
  
  TMatrixD C(12,3);     //4 quations / run, for 3 elastic runs --> total 12 equations
  TVectorD bVec(12);
  TVectorD bVec_err(12);
  TVectorD bVec_cpy(12);

  TVectorD aVec(3);  //vector to store the parameters (a0, a1, a2)
  TVectorD eVec(12);   //vector to store residuals
  TVectorD eVec_err(12);

  int row1, row2, row3, row4; //keep track of the 12 equations

  TMatrixD N_inv(12,12);   //diagonal elements are 1/sig_meas**2, (inverse of the covariance matrix for measured errors, assumed measured errors are independent)
  
  //Define the parameters to be minimized
  Double_t a0, a1, a2;   //a0 --> dEb/Eb,  a1 --> dEf/Ef,  a2 --> dth_e [rad], a3 --> dPp/Pp,  a4 --> dth_p [rad]

  //Define the observed quantities
  Double_t dW_obs;
  Double_t dEm_obs;
  Double_t dPmx_obs;
  Double_t dPmz_obs;

  //Define the observed errors
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
      th_p[i] = th_p[i] * dtr;

      
      //Calculate the observed differences bewteen DATA and SIMC
      dW_obs = Wdata[i] - Wsimc[i];      
      dEm_obs = Emdata[i] - Emsimc[i];
      dPmx_obs = PmXdata[i] - PmXsimc[i];
      dPmz_obs = PmZdata[i] - PmZsimc[i];
      
      //Calculate errors of the observed differences bewteen DATA and SIMC
      dW_obs_err2 = pow(Wdata_err[i]*err_fact,2) + pow(Wsimc_err[i]*err_fact, 2);  
      dEm_obs_err2 = pow(Emdata_err[i]*err_fact,2) + pow(Emsimc_err[i]*err_fact, 2);
      dPmx_obs_err2 = pow(PmXdata_err[i]*err_fact,2) + pow(PmXsimc_err[i]*err_fact, 2);
      dPmz_obs_err2 = pow(PmZdata_err[i]*err_fact,2) + pow(PmZsimc_err[i]*err_fact, 2);


      //Define all 4 ith rows per run number
      row1 = 4*(i+1) - 4;
      row2 = 4*(i+1) - 3;
      row3 = 4*(i+1) - 2;
      row4 = 4*(i+1) - 1;
      
      cout << "row1: " << row1 << endl;
      cout << "row2: " << row2 << endl;
      cout << "row3: " << row3 << endl;
      cout << "row4: " << row4 << endl;

      //Calculate the derivatives for each run (and the coefficients)

      //---Equation 1---
      dW_dEb = Ef / Eb;
      C[row1][0] = dW_dEb * Eb;

      dW_dEf = - Eb / Ef;
      C[row1][1] = dW_dEf * Ef;   //the '+' is to increment coefficient (sum over all runs)

      dW_dthe = -2.*Eb*Ef/Mp * sin(th_e[i]/2.) * cos(th_e[i]/2.);
      C[row1][2] = dW_dthe;


      //---Equation 2---
      dEm_dEb = 1;
      C[row2][0] = dEm_dEb * Eb;

      dEm_dEf = -1;
      C[row2][1] = dEm_dEf * Ef;

      C[row2][2] = 0.;


      //---Equation 3---
      C[row3][0] = 0.;

      dPmx_dEf = -sin(th_e[i])*cos(th_e[i]);
      C[row3][1] = dPmx_dEf * Ef;

      dPmx_dthe = -Ef*cos(ph_e)*cos(th_e[i]);
      C[row3][2] = dPmx_dthe;


      //---Equation 4---
      dPmz_dEb = 1.;
      C[row4][0] = dPmz_dEb *Eb;

      dPmz_dEf = -cos(th_e[i])*cos(ph_e);
      C[row4][1] = dPmz_dEf * Ef;

      dPmz_dthe = Ef*cos(ph_e)*sin(th_e[i]);
      C[row4][2] = dPmz_dthe;

      //Fill the observed vector
      bVec[row1] = dW_obs;
      bVec[row2] = dEm_obs;
      bVec[row3] = dPmx_obs;
      bVec[row4] = dPmz_obs;

      //Fill observed vector error
      bVec_err[row1] = sqrt(dW_obs_err2);
      bVec_err[row2] = sqrt(dEm_obs_err2);
      bVec_err[row3] = sqrt(dPmx_obs_err2);
      bVec_err[row4] = sqrt(dPmz_obs_err2);

      //Fill copy of the observed vector (to be used to solve linear system)
      bVec_cpy[row1] = dW_obs;
      bVec_cpy[row2] = dEm_obs;
      bVec_cpy[row3] = dPmx_obs;
      bVec_cpy[row4] = dPmz_obs;

      //Fill Inverse Covariance Matrix of Measured Errors
      N_inv[row1][row1] = 1./dW_obs_err2;
      N_inv[row2][row2] = 1./dEm_obs_err2;
      N_inv[row3][row3] = 1./dPmx_obs_err2;
      N_inv[row4][row4] = 1./dPmz_obs_err2;

      
      cout << "dW_obs = " << dW_obs << " +/-  " << sqrt(dW_obs_err2) << endl;
      cout << "dEm_obs = " << dEm_obs << " +/- " << sqrt(dEm_obs_err2) <<endl;
      cout << "dPmx_obs = " << dPmx_obs << " +/- " << sqrt(dPmx_obs_err2) <<endl;
      cout << "dPmz_obs = " << dPmz_obs << " +/- " << sqrt(dPmz_obs_err2) <<endl;

    }
  

  cout << "MODEL MATRIX" << endl;
  C.Print();
  cout << "Observed Vector" << endl;
  bVec.Print();
  cout << "Inverse Observed Covariance Matrix, N^{-1}" << endl;
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

  
  // [C] * aVec = bVec + e -> this is the  general linear equation with residual e,
  //  e = [C] * aVec - bVec   ::  e is a vector with the residuals

  Double_t chi2 = 0.0;
  Double_t chi2dof = 0.0; 

  for(int i=0; i<12; i++)
    {
      //residual vector
      eVec[i] = C[i][0] * aVec[0] + C[i][1] * aVec[1] + C[i][2] * aVec[2]   - bVec[i];

      chi2 +=  eVec[i]*eVec[i]*N_inv[i][i];   //sum over all chi2's
    }
  
  chi2dof = chi2/9.;
  cout << "chi2 = " <<chi2 << endl;
  cout << "chi2/dof = " <<chi2dof<< endl;


  TMatrixD R1(12,3);
  R1.Zero();

  //R1(12x3) = N_inv(12,12) * C(12,3)
  for(int i=0; i<12; i++){

    for(int j=0; j<3; j++){
      
      R1[i][j] = 0.;  //reset to sum over new elements
      
      for(int k=0; k<12; k++)
	{
	  R1[i][j] += N_inv[i][k] * C[k][j];
	}

    }

  }

  N_inv.Print();
  C.Print();
  R1.Print();
  
  TMatrixD CT(3,12);
  CT.Transpose(C);
  CT.Print();
    
  TMatrixD V(3,3);
  V.Zero();

  //Get Covariance Matrix: C(3,3) = CT(3x12) * R1(12x3)
  for(int i=0; i<3; i++){
    //columns of R1 Matrix
    for(int j=0; j<3; j++){
      
      V[i][j] = 0.;  //reset to sum over new elements
      //Columns of CT (equivalently, rows of R1)
      for(int k=0; k<12; k++)
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
  

  cout << "---Optimized Parameters---" << endl;
  cout << "total equations x total runs = 4x3 = 12 observations, # parameters = 3, dof = 9 " << endl;
  cout << "chi2 = " << chi2 << "  chi2/dof = " << chi2dof << endl;
  cout << "dEb / Eb = " << aVec[0] << endl;
  cout << "dEf / Ef = " << aVec[1] << endl;
  cout << "dth_e = "    << aVec[2] << endl;

  //Get Parameter Errors
  Double_t a0_err, a1_err, a2_err, a3_err, a4_err;
  //Off-Diagonal Covariance Errors
  Double_t a01_err, a02_err, a03_err, a04_err, a12_err, a13_err, a14_err, a23_err, a24_err, a34;
  
  a0_err = sqrt(Vinv[0][0]);    //uncertaintiy in dEb/Eb offset
  a1_err = sqrt(Vinv[1][1]);    //uncertaintiy in dEf/Ef offset
  a2_err = sqrt(Vinv[2][2]);    //uncertaintiy in dth_e offset

  cout << "=== Uncertainty in Parameters (Diagonal Elements) ===" << endl;
  cout << "sqrt[cov(0,0)] = dEb / Eb = " << a0_err << endl;
  cout << "sqrt[cov(1,1)] = dEf / Ef = " << a1_err << endl;
  cout << "sqrt[cov(2,2)] = dth_e [rad] = " << a2_err << endl;
  cout << "=== Uncertainty in Parameters (Off-Diagonal Elements) ===" << endl;
  cout << "cov(0,1) = dEb_Eb * dEf_Ef = " << Vinv[0][1] << endl;
  cout << "cov(0,2) = dEb_Eb * dth_e = " << Vinv[0][2] << endl;
  cout << "cov(1,2) = dEf_Ef * dth_e = " << Vinv[1][2] << endl;

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

//Write residuals to file
  ofstream ofs;
  ofs.open("residuals_shms_v0.txt");
  
  ofs << "#!x[i,0]/    residual[f,1]/    residual_err[f,2]/     obs[f,3]/     obs_err[f,4]/\n";
  
 
  Double_t calc_err;
  Double_t obs_err;
  //Calculate residual errors
  for(int i=0; i<12; i++)
    {


      simc_err = sqrt( pow(C[i][0] * a0_err, 2) + pow(C[i][1] * a1_err,2) + pow(C[i][2] * a2_err, 2) + pow(C[i][3] * a3_err, 2));
		       /*+ 2.*C[i][0]*C[i][1]*Vinv[0][1] + 2.*C[i][0]*C[i][2]*Vinv[0][2] + 2.*C[i][0]*C[i][3]*Vinv[0][3]
			 + 2.*C[i][1]*C[i][2]*Vinv[1][2] + 2.*C[i][1]*C[i][3]*Vinv[1][3] + 2.*C[i][2]*C[i][3]*Vinv[2][3]);*/
      obs_err =  bVec_err[i];

      if(i==2 || i==3 || i==6 || i==7 || i==10 || i==11){
	eVec_err[i] = sqrt( pow(simc_err,2) + pow(obs_err,2) );
	cout << "simc_err2 + obs_err2 = " << pow(simc_err,2) << " + " << pow(obs_err,2) << " = " << eVec_err[i]*eVec_err[i] << endl;
      }
      ofs << i << "   " << eVec[i] << "   " << eVec_err[i] << "   " << bVec[i] << "   " << bVec_err[i] << endl;
      
    }
  
  ofs.close();


  
}
