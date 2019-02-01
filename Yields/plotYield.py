import LT.box as B
import numpy as np
import matplotlib.pyplot as plt


#NO CUTS ON SIMC/DATA
f = B.get_file('yield0.data')
Run = B.get_data(f, 'Run')
dataY = B.get_data(f, 'dataY')
dataY_err = B.get_data(f, 'dataY_err')
simcY = B.get_data(f, 'simcY')
simcY_err = B.get_data(f, 'simcY_err')
#data to simc yield ratio
R = dataY / simcY    
R_err = np.sqrt( (dataY_err/dataY)**2 + (simcY_err/simcY)**2  )  #relative error dR/R
x = [1,2,3,4]
B.plot_exp(x, R, R_err, color='green', label='NO CUTS')

#HMS Delta/SHMS Delta CUTS
f = B.get_file('yield1.data')
Run = B.get_data(f, 'Run')
dataY = B.get_data(f, 'dataY')
dataY_err = B.get_data(f, 'dataY_err')
simcY = B.get_data(f, 'simcY')
simcY_err = B.get_data(f, 'simcY_err')
#data to simc yield ratio
R = dataY / simcY    
R_err = np.sqrt( (dataY_err/dataY)**2 + (simcY_err/simcY)**2  )  #relative error dR/R
x = [1,2,3,4]
B.plot_exp(x, R, R_err, color='red', label='CUTS: HMS/SHMS Delta')


B.pl.ylabel(r'Yield$_{data}$ / Yield$_{simc}$')
B.pl.xlabel('D2 Heep Runs')
plt.axhline(y=1, color='black', linestyle='--')
B.pl.legend()
B.pl.show()
