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

dth_avg = np.average(dth_meas)

fout = open('dW_dthe_Offset.txt', 'w')
fout.write('SHMS Angle Offsets, using dW/dth_e derivative:  dth_e = the_simc - the_data \n')
fout.write('3288: %s\n' % (dth_meas[0]))
fout.write('3371: %s\n' % (dth_meas[1]))
fout.write('3374: %s\n' % (dth_meas[2]))
fout.write('3377: %s\n' % (dth_meas[3]))
fout.write('dth_average: %s\n' % (dth_avg))

