import LT.box as B
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.pylab as pylab

#csfont = {'fontname':'Comic Sans MS'}
#plt.rcParams["font.family"] = "Courier"

x = [1,2,3,4]
my_xticks = ['3288', '3371', '3374', '3377']

#HMS Momentum Resolution
dP_P = 0.1 / 100.

#Final HMS Momentum
#3288: 2.935545,  3371: 3.47588,  3374: 2.310387, 3377: 1.891227
#2.935545 * dP_P = 

y_err_band = np.array([0.002936,0.003476,0.002310,0.001891]) #HMS Momentum error band relative to (Pcalc-Pmeas) = 0
plt.fill_between(x, -y_err_band, y_err_band, color='blue', alpha=0.2)


f = B.get_file('../ORIGINAL_HEEP/momentum_fraction_diff.data')
Run = B.get_data(f, 'Run')
Pf_data = B.get_data(f, 'Pf_data')   #momentum fraction data DELTA (Pcalc - Pmeas)
Pf_data_err = B.get_data(f, 'Pf_data_err')
Pf_simc = B.get_data(f, 'Pf_simc')
Pf_simc_err = B.get_data(f, 'Pf_simc_err')

#(SIMC - DATA)
Pdiff = Pf_simc - Pf_data
Pdiff_err = np.sqrt(Pf_simc_err**2 + Pf_data_err**2)

B.plot_exp(x, Pdiff, Pdiff_err, marker='s', color='blue', label='No Optimization')


f = B.get_file('../FINAL_HEEP/momentum_fraction_diff.data')
Run = B.get_data(f, 'Run')
Pf_data = B.get_data(f, 'Pf_data')   #momentum fraction data DELTA (Pcalc - Pmeas)
Pf_data_err = B.get_data(f, 'Pf_data_err')
Pf_simc = B.get_data(f, 'Pf_simc')
Pf_simc_err = B.get_data(f, 'Pf_simc_err')

#(SIMC - DATA)
Pdiff = Pf_simc - Pf_data
Pdiff_err = np.sqrt(Pf_simc_err**2 + Pf_data_err**2)

B.plot_exp(x, Pdiff, Pdiff_err, marker='^', color='red', label='Full Optimization')




B.pl.xticks(x, my_xticks)  #label x-axis values
B.pl.title('HMS Momentum Correction', fontsize=15, fontweight='bold')

B.pl.grid(linestyle='-')
B.pl.ylabel(r'$\Delta P_{diff}$ [GeV]', fontsize=10)
B.pl.xlabel('H(e,e\'p) Elastic Runs', fontsize=10)
plt.axhline(y=0, color='black', linestyle='--')
B.pl.legend()
B.pl.show()
