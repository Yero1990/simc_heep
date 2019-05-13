import LT.box as B
import numpy as np
import matplotlib.pyplot as plt



f = B.get_file('D2_heep_kin.txt')

Run = B.get_data(f, 'Run')
NMR = B.get_data(f, 'nmr_true')
hP_uncorr  = B.get_data(f, 'nmr_P')   #Un-Corrected HMS Momentum
cF = B.get_data(f, 'hPcorr_factor')
cF_err = B.get_data(f, 'hPcorr_factor_err')

#fit NMR
#fit = B.linefit(hP_uncorr, NMR, 0.0005)
#B.plot_exp(hP_uncorr, NMR, 0.0005,  color='black', marker='s', label='HMS P Corr. Factor')
#B.plot_line(fit.xpl, fit.ypl)

fig0 = B.pl.figure()
#fit corr. factor
fit = B.linefit(hP_uncorr, cF, cF_err)
#fit = B.linefit(NMR, cF, cF_err)

B.plot_exp(hP_uncorr,  cF, cF_err,  color='black', marker='s', label='HMS P Corr. Factor')
B.pl.xlabel('HMS P [GeV]')
B.pl.ylabel('Correction Factor')
B.pl.title('Momentum Corr. Factor Calibration')
B.pl.grid(True)

B.plot_line(fit.xpl, fit.ypl, color='red', label='Linear Fit')
B.pl.legend()



fig1 = B.pl.figure()

#Determine the corr. from the fit
cF_fit = fit.line(NMR)
 
#Residuals
cF_res = (cF_fit - cF)/cF

#Plot residuals
B.plot_exp(hP_uncorr, cF_res, color='blue', marker='^', label='Corr. Factor Residuals')

B.pl.xlabel('HMS P [GeV]')
B.pl.ylabel('Fit Residual, (Fit - Data)/Data')
B.pl.title('Momentum Corr. Factor Residuals')
B.pl.grid(True)
B.pl.legend()

#-------------Uncorrected Momentum(AFTER 3288)-------------------

#80 MeV
#HMS P = 2.8438

#580 MeV
#HMS P = 2.194 GeV

#750 MeV
#HMS P = 2.091 GeV


print ('HMS P Corr_Factor (80 MeV)  = ',  fit.line(2.8438))
print ('HMS P Corr_Factor (580 MeV set1) = ',  fit.line(2.194))
print ('HMS P Corr_Factor (750 MeV set1) = ',  fit.line(2.091))
print ('HMS P Corr_Factor (580 MeV set2) = ',  fit.line(2.194))
print ('HMS P Corr_Factor (750 MeV set2) = ',  fit.line(2.091))
print ('HMS P Corr_Factor (750 MeV set3) = ',  fit.line(2.091))

#print ('HMS P Corr_Factor (oPTICS)  = ',  fit.line(2.1939))
#OUTPUT (Only Runs after 3288)
#('HMS P Corr_Factor (80 MeV)  = ', 0.997047387698619)
#('HMS P Corr_Factor (580 MeV) = ', 0.9978267626184862)
#('HMS P Corr_Factor (750 MeV) = ', 0.9979503015793147)

B.pl.show()
