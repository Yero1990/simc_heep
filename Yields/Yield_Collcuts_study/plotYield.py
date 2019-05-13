import LT.box as B
import numpy as np
import matplotlib.pyplot as plt

x = [1,2,3,4]
my_xticks = ['76.859', '44.447', '271.992', '588.849']


#HMS Delta/SHMS Delta CUTS
f = B.get_file('yield_hCollCut.data')
Run = B.get_data(f, 'Run')
Wr = B.get_data(f, 'W_cut_range')
dataY = B.get_data(f, 'dataY')    
dataY_err = B.get_data(f, 'dataY_err')     #these are absolute errors, sqrt(N)
simcY = B.get_data(f, 'simcY')
simcY_err = B.get_data(f, 'simcY_err')      #these are absolute errors, sqrt(N)


#data to simc yield ratio
R = dataY / simcY    

#Alternate Errors (Using Error Propagation of ratio  f = a / b --> sig_f = sqrt( sig_a**2/b**2 + a**2*sig_b**2/b**4  )
R_err = np.sqrt( dataY_err**2/simcY**2 + dataY**2*simcY_err**2/simcY**4 )

#Determine the Relative Errors,  err = sqrt(N) / N = 1 / sqrt(N)
#dataY_Rel_err = 1. / dataY_err
#simcY_Rel_err = 1. / simcY_err

#R_err = R * np.sqrt(dataY_Rel_err**2 + simcY_Rel_err**2 )
#print('R=',R)
#print('R_err=',R_err)

Wr0_min = float(Wr[0].split('_')[0])
Wr1_min = float(Wr[1].split('_')[0])
Wr2_min = float(Wr[2].split('_')[0])
Wr3_min = float(Wr[3].split('_')[0])
Wr4_min = float(Wr[4].split('_')[0])
Wr5_min = float(Wr[5].split('_')[0])
Wr0_max = float(Wr[0].split('_')[1])
Wr1_max = float(Wr[1].split('_')[1])
Wr2_max = float(Wr[2].split('_')[1])
Wr3_max = float(Wr[3].split('_')[1])
Wr4_max = float(Wr[4].split('_')[1])
Wr5_max = float(Wr[5].split('_')[1])

B.plot_exp(x[0], R[0], R_err[0], color='blue', marker='s',label='W:(%f,%f)'%(Wr0_min, Wr0_max))
B.plot_exp(x[0], R[1], R_err[1], color='black', marker='s',label='W:(%f,%f)'%(Wr1_min, Wr1_max))
B.plot_exp(x[0], R[2], R_err[2], color='red', marker='s',label='W:(%f,%f)'%(Wr2_min, Wr2_max))
B.plot_exp(x[0], R[3], R_err[3], color='green', marker='s',label='W:(%f,%f)'%(Wr3_min, Wr3_max))
B.plot_exp(x[0], R[4], R_err[4], color='magenta', marker='s',label='W:(%f,%f)'%(Wr4_min, Wr4_max))
B.plot_exp(x[0], R[5], R_err[5], color='cyan', marker='s',label='W:(%f,%f)'%(Wr5_min, Wr5_max))

B.plot_exp(x[1], R[6], R_err[6], color='blue', marker='^',label='W:(%f,%f)'%(Wr0_min, Wr0_max))
B.plot_exp(x[1], R[7], R_err[7], color='black', marker='^',label='W:(%f,%f)'%(Wr1_min, Wr1_max))
B.plot_exp(x[1], R[8], R_err[8], color='red', marker='^',label='W:(%f,%f)'%(Wr2_min, Wr2_max))
B.plot_exp(x[1], R[9], R_err[9], color='green', marker='^',label='W:(%f,%f)'%(Wr3_min, Wr3_max))
B.plot_exp(x[1], R[10], R_err[10], color='magenta', marker='^',label='W:(%f,%f)'%(Wr4_min, Wr4_max))
B.plot_exp(x[1], R[11], R_err[11], color='cyan', marker='^',label='W:(%f,%f)'%(Wr5_min, Wr5_max))

B.plot_exp(x[2], R[12], R_err[12], color='blue', marker='o',label='W:(%f,%f)'%(Wr0_min, Wr0_max))
B.plot_exp(x[2], R[13], R_err[13], color='black', marker='o',label='W:(%f,%f)'%(Wr1_min, Wr1_max))
B.plot_exp(x[2], R[14], R_err[14], color='red', marker='o',label='W:(%f,%f)'%(Wr2_min, Wr2_max))
B.plot_exp(x[2], R[15], R_err[15], color='green', marker='o',label='W:(%f,%f)'%(Wr3_min, Wr3_max))
B.plot_exp(x[2], R[16], R_err[16], color='magenta', marker='o',label='W:(%f,%f)'%(Wr4_min, Wr4_max))
B.plot_exp(x[2], R[17], R_err[17], color='cyan', marker='o',label='W:(%f,%f)'%(Wr5_min, Wr5_max))

B.plot_exp(x[3], R[18], R_err[18], color='blue', marker='D',label='W:(%f,%f)'%(Wr0_min, Wr0_max))
B.plot_exp(x[3], R[19], R_err[19], color='black', marker='D',label='W:(%f,%f)'%(Wr1_min, Wr1_max))
B.plot_exp(x[3], R[20], R_err[20], color='red', marker='D',label='W:(%f,%f)'%(Wr2_min, Wr2_max))
B.plot_exp(x[3], R[21], R_err[21], color='green', marker='D',label='W:(%f,%f)'%(Wr3_min, Wr3_max))
B.plot_exp(x[3], R[22], R_err[22], color='magenta', marker='D',label='W:(%f,%f)'%(Wr4_min, Wr4_max))
B.plot_exp(x[3], R[23], R_err[23], color='cyan', marker='D',label='W:(%f,%f)'%(Wr5_min, Wr5_max))

B.pl.xticks(x, my_xticks)  #label x-axis values




B.pl.grid(linestyle='-')

B.pl.ylabel(r'Yield$_{data}$ / Yield$_{simc}$')
B.pl.xlabel('SHMS 3/4 Rate [kHz]')
plt.axhline(y=1, color='black', linestyle='--')
B.pl.legend()
B.pl.show()


