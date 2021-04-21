import LT.box as B
import numpy as np
import matplotlib.pyplot as plt




f = B.get_file('D2_heep_kin_original.txt')

Run = B.get_data(f, 'Run')
NMR = B.get_data(f, 'nmr_true')
hP_uncorr  = B.get_data(f, 'nmr_P')   #Un-Corrected HMS Momentum
cF = B.get_data(f, 'hPcorr_factor')
cF_err = B.get_data(f, 'hPcorr_factor_err')

#HMS Momentum resolution is better than: 0.1 % or 0.1/100=1e-3  (This should be the uncertainty in the correction factor)
cF_err = np.array([1e-3, 1e-3, 1e-3, 1e-3])


fig0 = B.pl.figure()
#fit corr. factor
fit = B.linefit(hP_uncorr, cF, cF_err)
B.plot_exp(hP_uncorr,  cF, cF_err,  color='black', marker='s', label='H(e,e\'p) Data')


#Un-Corrected HMS Momentum fomr D(e,e'p)n
x80 = 2.8438
y80 = fit.line(x80)
x580 = 2.194
y580 = fit.line(x580)
x750 = 2.091
y750 = fit.line(x750)
x_deut = np.array([x80, x580, x750])
y_deut = np.array([y80, y580, y750])
B.plot_exp(x80, y80, marker='s', color='blue', markersize=7, label='D(e,e\'p)n 80 MeV' )
B.plot_exp(x580, y580, marker='s', color='green', markersize=7, label='D(e,e\'p)n 580 MeV' )
B.plot_exp(x750, y750, marker='s', color='purple', markersize=7, label='D(e,e\'p)n 750 MeV' )

B.plot_line(fit.xpl, fit.ypl, color='red', label='Linear Fit')

B.pl.xlabel('HMS Un-Corrected Momentum, P$_{uncorr}$ [GeV]')
B.pl.ylabel('Correction Factor')
B.pl.title('HMS Momentum Correction Factor Calibration')
B.pl.grid(True)

B.pl.legend()



fig1 = B.pl.figure()
cF_fit = fit.line(hP_uncorr)
#Residuals (Percent Error)
cF_res = abs(cF - cF_fit)/cF_fit * 100

#Plot residuals
B.pl.ylim(-0.031, 0.031)

B.plot_exp(hP_uncorr, cF_res, color='blue', marker='^', markersize=9, label='H(e,e\'p) Percent (%) Deviation from Fit')


B.pl.xlabel('HMS Un-Corrected Momentum, P$_{uncorr}$ [GeV]')
B.pl.ylabel('Correction Factor Percent Error [$\%$]')
B.pl.title('Correction Factor Percent Error')
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
