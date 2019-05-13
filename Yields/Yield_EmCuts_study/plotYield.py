import LT.box as B
import numpy as np
import matplotlib.pyplot as plt

x = [1,2,3,4]
my_xticks = ['76.859', '44.447', '271.992', '588.849']


#HMS Delta/SHMS Delta CUTS
f = B.get_file('yield.data')
Run = B.get_data(f, 'Run')
em = B.get_data(f, 'Em_cut')
dataY = B.get_data(f, 'dataY')    
dataY_err = B.get_data(f, 'dataY_err')     #these are absolute errors, sqrt(N)
simcY = B.get_data(f, 'simcY')
simcY_err = B.get_data(f, 'simcY_err')      #these are absolute errors, sqrt(N)

#data to simc yield ratio
R = dataY / simcY    

#Determine the Relative Errors,  err = sqrt(N) / N = 1 / sqrt(N)
dataY_Rel_err = 1. / dataY_err
simcY_Rel_err = 1. / simcY_err

R_err = R * np.sqrt(dataY_Rel_err**2 + simcY_Rel_err**2 )

B.plot_exp(x[0], R[0], R_err[0], color='red', marker='s',label='NO Emiss Cut')
B.plot_exp(x[1], R[4], R_err[4], color='red', marker='s')
B.plot_exp(x[2], R[8], R_err[8], color='red', marker='s')
B.plot_exp(x[3], R[12], R_err[12], color='red', marker='s')

B.plot_exp(x[0], R[1], R_err[1], color='blue', marker='^',label='Em < 30 Mev')
B.plot_exp(x[1], R[5], R_err[5], color='blue', marker='^')
B.plot_exp(x[2], R[9], R_err[9], color='blue', marker='^')
B.plot_exp(x[3], R[13], R_err[13], color='blue', marker='^')

B.plot_exp(x[0], R[2], R_err[2], color='cyan', marker='o',label='Em < 60 Mev')
B.plot_exp(x[1], R[6], R_err[6], color='cyan', marker='o')
B.plot_exp(x[2], R[10], R_err[10], color='cyan', marker='o')
B.plot_exp(x[3], R[14], R_err[14], color='cyan', marker='o')

B.plot_exp(x[0], R[3], R_err[3], color='magenta', marker='v',label='Em < 90 Mev')
B.plot_exp(x[1], R[7], R_err[7], color='magenta', marker='v')
B.plot_exp(x[2], R[11], R_err[11], color='magenta', marker='v')
B.plot_exp(x[3], R[15], R_err[15], color='magenta', marker='v')

B.pl.xticks(x, my_xticks)  #label x-axis values




B.pl.grid(linestyle='-')

B.pl.ylabel(r'Yield$_{data}$ / Yield$_{simc}$')
B.pl.xlabel('SHMS 3/4 Rate [kHz]')
plt.axhline(y=1, color='black', linestyle='--')
B.pl.legend()
B.pl.show()
