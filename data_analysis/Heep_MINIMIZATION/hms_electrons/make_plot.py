import LT.box as B
import numpy as np
from LT.datafile import dfile

res = dfile('residuals_excludeEb.txt')

run = res['run']
x = res['x']
residual = res['residual']
residual_err = res['residual_err']
dW_obs = res['dW_obs']
dW_obs_err = res['dW_obs_err']
dW_model = res['dW_model']
dW_model_err = res['dW_model_err']

B.plot_exp(x, residual, residual_err, color='red', marker='D', label=r'Residuals = $dW_{meas.} - dW_{model}$')
B.plot_exp(x, dW_obs, dW_obs_err, color='b', marker='o', label=r'$dW_{meas} = W_{data} - W_{simc}$')
B.plot_exp(x, dW_model, dW_model_err, color='g', marker='s', label=r'$dW_{model}$')  

B.pl.legend()
B.pl.xlabel('Kinematic Setting')
B.pl.ylabel(r'Residualsc [GeV]')
B.pl.title('$\chi^{2}$ Min. for HMS Kaon LT Elastics')
B.pl.ylim(-0.015, 0.015)
B.pl.show()
