import LT.box as B
import numpy as np
import matplotlib.pyplot as plt

x = [1,2,3,4]
my_xticks = ['76.859', '44.447', '271.992', '588.849']


#HMS Delta/SHMS Delta CUTS
f = B.get_file('yield_oldtrk.data')
Run = B.get_data(f, 'Run')
dataY = B.get_data(f, 'dataY')
dataY_err = B.get_data(f, 'dataY_err')
simcY = B.get_data(f, 'simcY')
simcY_err = B.get_data(f, 'simcY_err')
#data to simc yield ratio
R = dataY / simcY    
R_err = np.sqrt( (dataY_err/dataY)**2 + (simcY_err/simcY)**2  )  #relative error dR/R

B.plot_exp(x, R, R_err, color='blue', label='Original TrkEff Calculation')
B.pl.xticks(x, my_xticks)  #label x-axis values

#-----------------------------------------------------------------------------------------------
f = B.get_file('yield_singleTrk.data')
Run = B.get_data(f, 'Run')
dataY = B.get_data(f, 'dataY')
dataY_err = B.get_data(f, 'dataY_err')
simcY = B.get_data(f, 'simcY')
simcY_err = B.get_data(f, 'simcY_err')
#data to simc yield ratio
R = dataY / simcY    
R_err = np.sqrt( (dataY_err/dataY)**2 + (simcY_err/simcY)**2  )  #relative error dR/R


B.plot_exp(x, R, R_err, color='black', label='New TrkEff Calculation (ONLY single tracks)')
B.pl.xticks(x, my_xticks)  #label x-axis values

#-----------------------------------------------------------------------------------------------

f = B.get_file('yield_newtrk_THorn.data')
Run = B.get_data(f, 'Run')
dataY = B.get_data(f, 'dataY')
dataY_err = B.get_data(f, 'dataY_err')
simcY = B.get_data(f, 'simcY')
simcY_err = B.get_data(f, 'simcY_err')
#data to simc yield ratio
R = dataY / simcY    
R_err = np.sqrt( (dataY_err/dataY)**2 + (simcY_err/simcY)**2  )  #relative error dR/R

B.plot_exp(x, R, R_err, color='red', label='New TrkEff Calculation (T. Horn)')
B.pl.xticks(x, my_xticks)  #label x-axis values



B.pl.grid(linestyle='-')

B.pl.ylabel(r'Yield$_{data}$ / Yield$_{simc}$')
B.pl.xlabel('SHMS 3/4 Rate [kHz]')
plt.axhline(y=1, color='black', linestyle='--')
B.pl.legend()
B.pl.show()
