import LT.box as B
import numpy as np
import matplotlib.pyplot as plt



#HMS Delta/SHMS Delta CUTS
f = B.get_file('../../data_analysis/W_delta_bins_3288.txt')
dataY = B.get_data(f, 'W_edelta_bin')
dataY_Rel_err = B.get_data(f, 'W_rel_err')
dx_axis = B.get_data(f, 'edelta_xval')

f = B.get_file('../../simc_analysis/D2_heep/W_delta_bins_3288.txt')
simcY = B.get_data(f, 'W_edelta_bin')
simcY_Rel_err = B.get_data(f, 'W_rel_err')
sx_axis = B.get_data(f, 'edelta_xval')



R = []
R_err = []
x = []
for i, sx_axis in enumerate(sx_axis):
    #print(i)
    if(simcY[i]!=0.0):
        xi = sx_axis
        Ri = dataY[i]/simcY[i]
        Ri_err = Ri*np.sqrt(simcY_Rel_err[i]*simcY_Rel_err[i] + dataY_Rel_err[i]*dataY_Rel_err[i])
        R.append(Ri)
        R_err.append(Ri_err)
        x.append(xi)

B.plot_exp(x, R, R_err, color='blue', label='delta')
B.pl.ylabel(r'Yield$_{data}$ / Yield$_{simc}$')
B.pl.xlabel('SHMS Delta [%]')
B.pl.title('DATA/SIMC Yield Binned in SHMS Delta')

B.pl.show()

#---------------------------------------------

#W binned in SHMS X'tar
f = B.get_file('../../data_analysis/W_xptar_bins_3288.txt')
dataY = B.get_data(f, 'W_xptar_bin')
dataY_Rel_err = B.get_data(f, 'W_rel_err')
dx_axis = B.get_data(f, 'exptar_xval')

f = B.get_file('../../simc_analysis/D2_heep/W_xptar_bins_3288.txt')
simcY = B.get_data(f, 'W_xptar_bin')
simcY_Rel_err = B.get_data(f, 'W_rel_err')
sx_axis = B.get_data(f, 'exptar_xval')



R = []
R_err = []
x = []
for i, sx_axis in enumerate(sx_axis):
    #print(i)
    if(simcY[i]!=0.0):
        xi = sx_axis
        Ri = dataY[i]/simcY[i]
        Ri_err = Ri*np.sqrt(simcY_Rel_err[i]*simcY_Rel_err[i] + dataY_Rel_err[i]*dataY_Rel_err[i])
        R.append(Ri)
        R_err.append(Ri_err)
        x.append(xi)

B.plot_exp(x, R, R_err, color='red', label='delta')
B.pl.ylabel(r'Yield$_{data}$ / Yield$_{simc}$')
B.pl.xlabel('SHMS X\'tar [rad]')
B.pl.title('DATA/SIMC Yield Binned in SHMS X\'tar')

B.pl.show()


#-----------------------------------------


#W binned in SHMS Y'tar
f = B.get_file('../../data_analysis/W_yptar_bins_3288.txt')
dataY = B.get_data(f, 'W_yptar_bin')
dataY_Rel_err = B.get_data(f, 'W_rel_err')
dx_axis = B.get_data(f, 'eyptar_xval')

f = B.get_file('../../simc_analysis/D2_heep/W_yptar_bins_3288.txt')
simcY = B.get_data(f, 'W_yptar_bin')
simcY_Rel_err = B.get_data(f, 'W_rel_err')
sx_axis = B.get_data(f, 'eyptar_xval')



R = []
R_err = []
x = []
for i, sx_axis in enumerate(sx_axis):
    #print(i)
    if(simcY[i]!=0.0):
        xi = sx_axis
        Ri = dataY[i]/simcY[i]
        Ri_err = Ri*np.sqrt(simcY_Rel_err[i]*simcY_Rel_err[i] + dataY_Rel_err[i]*dataY_Rel_err[i])
        R.append(Ri)
        R_err.append(Ri_err)
        x.append(xi)

B.plot_exp(x, R, R_err, color='red', label='delta')
B.pl.ylabel(r'Yield$_{data}$ / Yield$_{simc}$')
B.pl.xlabel('SHMS Y\'tar [rad]')
B.pl.title('DATA/SIMC Yield Binned in SHMS Y\'tar')

B.pl.show()
