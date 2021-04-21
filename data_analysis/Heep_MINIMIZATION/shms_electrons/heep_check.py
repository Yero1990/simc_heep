import os
import subprocess as sp
import sys
from sys import argv
import numpy as np
from LT.datafile import dfile
import LT.box as B

# nu = Eb - Ef 
# Q2 = 4.*Eb*Ef*np.sin(th_e/2)**2
# W2 = Mp**2 - 2*Mp*nu + Q2 



def get_dW_variation():
    #Relative Uncertainties                                                     
        
  

    #Parameters with lowest redX2
    p1 = dEb_Eb = -0.002    #dEb / Eb 0.0003914909                       
    p2 = dEf_Ef = -0.0009    #dEf / Ef               
    p3 = dthe =  0.0001     #dthe [rad]         
    
    dEb = p1 * Eb
    dEf = p2 * Ef
    dthe = p3
    
    #Variations 
    dW_1 = dW_dEb * dEb
    dW_2 = dW_dEf * dEf
    dW_3 = dW_dthe * dthe
    dW_tot = dW_1 + dW_2 + dW_3

    dW_diff = dW_tot - dW_obs
    print('dW_pred=',dW_tot)
    print('dW_obs=',dW_obs)
    print('dW_diff=', dW_diff)

    '''
    print('------------------------\n')
    print('----H(e,e\'p) Check-----\n')
    print('------------------------\n')
    print('The changes are in Wshift[in MeV] / MeV in Eb or Ef or Wshift[in MeV]/mr shift in e- angle  \n')
    print('          dW    \n')
    print('dEb       %f    \n'%(dW_1))
    print('dEf       %f    \n'%(dW_2))
    print('dth_e     %f    \n'%(dW_3)) 
    print('-------------------\n')
    print('dW_total: %f'%(dW_tot))
    '''


def get_relative():
    #Code to Determine relative uncertainties based on dW variations observed

    #p1 = dEb / Eb ,  dW_from_Eb = dW_dEb * dEb =  dW_dEb * p1 * Eb
    #p2 = dEf / Ef ,  dW_from_Ef = dW_dEf * dEf = dW_dEf * p2 * Ef
    #p3 = dthe,  dW_from_the = dW_dthe * dthe
    
    #Get Relative uncertainties based on dW observations
    p1 = dW_obs /(dW_dEb * Eb)
    p2 = dW_obs /(dW_dEf * Ef)
    p3 = (dW_obs / dW_dthe) *1e3  #convert from radians to milliradians

    print('dEb_Eb=',p1)
    print('dEf_Ef=',p2)
    print('dth_e=',p3)

    #Add relative errors to kin file
    kin.add_key('dEb_Eb', 'f')
    kin.add_key('dEf_Ef', 'f')
    kin.add_key('dth_e', 'f')

    for i, irun in enumerate(run):
        kin.data[i]['dEb_Eb'] = p1[i] 
        kin.data[i]['dEf_Ef'] = p2[i] 
        kin.data[i]['dth_e'] = p3[i] 
    kin.save('shms_elastics_singles.data')

def main():
  
    #Define constants
    dtr = np.pi / 180.
    Mp = 938.272   #proton mass [MeV]

    #Read Data File containing kinematics and fits
    kin = dfile('../kinfile/elastic_shms.data')
    run = kin['Run']
    Ef = kin['shms_P'] *1e3           #convert to MeV     
    th_e = kin['shms_Angle'] * dtr    #convert to rad
    Eb = kin['beam_e'] *1e3           #convert to MeV
    simcW = kin['simc_W_mean'] *1e3   #convert to MeV
    dataW = kin['data_W_mean'] *1e3   #convert to MeV
    dW_obs = dataW - simcW  #observed dW variation

    dW_obs_err = Eb * 8 * 1e-4 / Ef

    x = np.arange(16) + 1  #array of 1-16
    #derivative of W with respect to beam energy
    dW_dEb = Ef / Eb      
    #derivative of W with respect to final e- energy
    dW_dEf = -Eb / Ef     
    #derivative of W with respect to e- angle
    dW_dthe = -2.*Eb*Ef/Mp * np.sin(th_e/2.) * np.cos(th_e/2.)

    #Get Initial Relative uncertainties based on dW observations
    #These are used to estimate the boundary, or how large should 
    #we set the rel. uncertainty. (pmin, pmax) in the chi2Min code
    p1 = dW_obs /(dW_dEb * Eb)       #p1 = dEb / Eb    
    p2 = dW_obs /(dW_dEf * Ef)
    p3 = (dW_obs / dW_dthe)    

    p1_min = min(p1); p1_max = max(p1)
    p2_min = min(p2); p2_max = max(p2)
    p3_min = min(p3); p3_max = max(p3)

    p1_avg = np.average(p1)
    p2_avg = np.average(p2)
    p3_avg = np.average(p3)

    dW_pred_init = dW_dEb*p1_avg*Eb + dW_dEf*p2_avg*Ef + dW_dthe*p3_avg

    #Add relative errors to kin file
    kin.add_key('dEb_Eb', 'f')
    kin.add_key('dEf_Ef', 'f')
    kin.add_key('dth_e', 'f')

    for i, irun in enumerate(run):
        kin.data[i]['dEb_Eb'] = p1[i] 
        kin.data[i]['dEf_Ef'] = p2[i] 
        kin.data[i]['dth_e'] = p3[i] 
    kin.save('initial_param.data')

    #Plot Error on dw Observed
    B.plot_exp(x, dW_obs_err, marker='o', color='red', label='dW Observed Error') 
    B.pl.xlabel('SHMS H(e,e\'p) Singles Runs')
    B.pl.ylabel('dW Observed Error')
    B.pl.title('dW Errors')
    B.pl.legend()
    B.pl.savefig('dW_obs_err.pdf')
    B.pl.show()

    #PLot Initial Parameters
    B.plot_exp(x, p1, marker='o', color='red', label=r'$dE_{b}/E_{b}$, (min,max)=(%.4f,%.4f), avg=%.4f'%(p1_min, p1_max, p1_avg))
    B.pl.axhline(y=p1_avg, color='red', linestyle='--')
    B.plot_exp(x, p2, marker='s', color='blue', label=r'$dE_{f}/E_{f}$, (min,max)=(%.4f,%.4f),avg=%.4f'%(p2_min, p2_max,p2_avg))
    B.pl.axhline(y=p2_avg, color='blue', linestyle='--')
    B.plot_exp(x, p3, marker='^', color='green', label=r'd$\theta_{e}$, (min,max)=(%.4f,%.4f),avg=%.4f rad'%(p3_min, p3_max,p3_avg))
    B.pl.axhline(y=p3_avg, color='green', linestyle='--')
    B.pl.xlabel('SHMS H(e,e\'p) Singles Runs')
    B.pl.ylabel('Relative Error')
    B.pl.title('Initial Relative Errors from dW Variations')
    B.pl.legend()
    B.pl.savefig('initial_param.pdf')
    B.pl.show()



    #Results from 1st FIT iteration (MinimumX2 p1, p2, p3)
    p1_1 = -0.0032
    p2_1 = -0.0003
    p3_1 = 0.0003
    dW_pred_1 = dW_dEb*p1_1*Eb + dW_dEf*p2_1*Ef + dW_dthe*p3_1

    #Plot  Residuals
    
    w = 1./dW_obs_err**2

    R_init = dW_pred_init - dW_obs
    R_init_err = dW_obs_err
    
    R_1 = dW_pred_1 - dW_obs
    R_1_err = dW_obs_err
    
    R_init_avg = np.average(R_init, weights=w)
    R_1_avg = np.average(R_1, weights=w)

    B.plot_exp(x, R_init, R_init_err, marker='s', color='blue', label=r'Initial Residuals (avg=%.2f)'%(R_init_avg))
    B.pl.axhline(y=R_init_avg, color='blue', linestyle='--')

    B.plot_exp(x, R_1, R_1_err, marker='o', color='red', label=r'1st $\chi^{2}$ Min. Residuals (avg=%.2f)'%(R_1_avg))
    B.pl.axhline(y=R_1_avg, color='red', linestyle='--')

    B.pl.xlabel('SHMS H(e,e\'p) Singles Runs')
    B.pl.ylabel('$dW_{pred}-dW_{obs}$ [MeV]')
    B.pl.title('Residuals from dW Variations')
    B.pl.legend()
    B.pl.savefig('chi2min_residuals.pdf')
    B.pl.show()

    #Plot Observed and predicted dW variations
    B.plot_exp(x, dW_obs, dW_obs_err, marker='o', color='red', label=r'dW observed (DATA-SIMC)')
    B.plot_exp(x, dW_pred_init, marker='s', color='green', label=r'dW predicted (initial)')
    B.plot_exp(x, dW_pred_1, marker='^', color='blue', label=r'dW predicted ($\chi^{2}$ Min.)')
    B.pl.xlabel('SHMS H(e,e\'p) Singles Runs')
    B.pl.ylabel('dW [MeV]')
    B.pl.title('dW Variations [MeV]')
    B.pl.legend()
    B.pl.savefig('dW_variations.pdf')
    B.pl.show()

    #get_relative()
    #get_dW_variation()

if __name__=="__main__":
    main()
