import LT.box as B
import numpy as np
from LT.datafile import dfile

res = dfile('residuals_model3.txt')

x = res['x']
residual = res['residual']
residual_err = res['residual_err']
dObs_meas = res['dObs_meas']
dObs_meas_err = res['dObs_meas_err']
dObs_model = res['dObs_model']
dObs_model_err = res['dObs_model_err']

B.plot_exp(x, residual, residual_err, color='red', marker='D', label=r'Residuals = $dObs_{meas.} - dObs_{model}$')
B.plot_exp(x, dObs_meas, dObs_meas_err, color='b', marker='o', label=r'$dObs_{meas} = Obs_{data} - Obs_{simc}$')
B.plot_exp(x, dObs_model, dObs_model_err, color='g', marker='s', label=r'$dObs_{model}$')  

B.pl.legend()
B.pl.xlabel('Kinematic Setting: dObs=[dW, dEm, dPmx, dPmz]')
B.pl.ylabel(r'Residuals [GeV]')
B.pl.title('$\chi^{2}$ Min. for Coin. H(e,e\'p) Elastics')
B.pl.ylim(-0.015, 0.015)
B.pl.show()
