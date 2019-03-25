import LT.box as B
import numpy as np

#Code to Calculate the angle offsets from the derivative of W

Mp = 0.938272
dtr = np.pi / 180. 

#Read in data 
f = B.get_file('data.txt')
hmsP = B.get_data(f, 'hmsP')
hmsAng = B.get_data(f, 'hmsAng')
shmsP = B.get_data(f, 'shmsP')
shmsAng = B.get_data(f, 'shmsAng')
Eb = B.get_data(f, 'beamE')
dataW = B.get_data(f, 'dataW')
dataW_err = B.get_data(f, 'dataW_err')
simcW = B.get_data(f, 'simcW')
simcW_err = B.get_data(f, 'simcW_err')

th_e = shmsAng * dtr
Ee = shmsP

dW_dth_e = - 2 * Eb * Ee * np.sin(th_e/2) * np.cos(th_e/2) / Mp

#dW = dW_dth_e * (-0.575*1e-3)

#print('dW = ',dW)

dW_meas = simcW - dataW
dW_meas_err = np.sqrt(simcW_err**2 + dataW_err**2)

dth_meas = dW_meas / dW_dth_e 
dth_meas_err =  dW_meas_err / dW_dth_e 

dth_meas_deg = (dW_meas / dW_dth_e) * 1./dtr 
dth_meas_err_deg =  (dW_meas_err / dW_dth_e) * 1./dtr 

dth_avg = np.average(dth_meas)
dth_avg_deg = np.average(dth_meas_deg)

shmsAng_corr = shmsAng + dth_meas_deg


fout = open('dW_dthe_Offset.txt', 'w')
fout.write('SHMS Angle Offsets, using dW/dth_e derivative:  dth_e = the_simc - the_data \n')
fout.write('3288: %s +/- %s \n' % (dth_meas[0], dth_meas_err[0]))
fout.write('3371: %s +/- %s \n' % (dth_meas[1], dth_meas_err[1]))
fout.write('3374: %s +/- %s \n' % (dth_meas[2], dth_meas_err[2]))
fout.write('3377: %s +/- %s \n' % (dth_meas[3], dth_meas_err[3]))
fout.write('\n')
fout.write('Offsets in Degrees\n')
fout.write('3288: %s +/- %s \n' % (dth_meas_deg[0], dth_meas_err_deg[0]))
fout.write('3371: %s +/- %s \n' % (dth_meas_deg[1], dth_meas_err_deg[1]))
fout.write('3374: %s +/- %s \n' % (dth_meas_deg[2], dth_meas_err_deg[2]))
fout.write('3377: %s +/- %s \n' % (dth_meas_deg[3], dth_meas_err_deg[3]))
fout.write('\n')
fout.write('Central Angle Correction [theta_cent + dtheta_meas]\n')
fout.write('3288: %s \n' % (shmsAng_corr[0]))
fout.write('3371: %s \n' % (shmsAng_corr[1]))
fout.write('3374: %s \n' % (shmsAng_corr[2]))
fout.write('3377: %s \n' % (shmsAng_corr[3]))
fout.write('\n')
fout.write('dth_average: %s [%s deg]\n' % (dth_avg, dth_avg_deg))

