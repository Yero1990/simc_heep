import numpy as np
import LT.box as B

Ee = 8.546443
#Ee = 8.7     #nominal SHMS momentum
Eb = 10.6005

f = B.get_file('data.txt')

dataEm = B.get_data(f, 'dataEm')
dataEm_err = B.get_data(f, 'dataEm_err')
simcEm = B.get_data(f, 'simcEm')
simcEm_err = B.get_data(f, 'simcEm_err')

dataEm_avg = np.average(dataEm)
simcEm_avg = np.average(simcEm)

#dEm = Em_simc - Em_data
dEm = simcEm_avg - dataEm_avg
dEe = -dEm     #electron momentum correction

corr_factor = 1 - dEm/Ee     # corr_factor = 1 + D0,  D0 = corr_factor - 1  (ppcentral_offset in shmsflags.param)
Ee_corr = Ee * corr_factor
D0 = corr_factor - 1.

fout = open('output.txt', 'w')

fout.write('original_P = %s\n' % (Ee))
fout.write('corrected_P = %s\n' % (Ee_corr))
fout.write('corr_factor = %s\n' % (corr_factor))
fout.write('Coeff D0 = %s\n' % (D0))
