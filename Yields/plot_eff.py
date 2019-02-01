import LT.box as B
import numpy as np

f = B.get_file('weight.data')
Run = B.get_data(f, 'Run')
TLT = B.get_data(f, 'TLT')
hTrk = B.get_data(f, 'hTrkEff')
pTrk = B.get_data(f, 'pTrkEff')
Q = B.get_data(f, 'Charge')
h3of4_rate = B.get_data(f, 'h_3of4_rate')
p3of4_rate = B.get_data(f, 'p_3of4_rate')
coin_rate = B.get_data(f, 'coin_rate')
run_time = B.get_data(f, 'run_time')

x = [1,2,3,4]

fig1 = B.pl.figure()
B.plot_exp(x,  TLT, color='black', marker='D', label='Total Live Time')
B.plot_exp(x,  hTrk, color='blue', marker='s', label='HMS Track. Eff.')
B.plot_exp(x,  pTrk, color='red', marker='o', label='SHMS Track. Eff.')
B.pl.xlabel('D2 Elastic Runs')
B.pl.ylabel('Live Time, Tracking Efficiencies')
B.pl.title('Live Time and Tracking Efficiencies')
B.pl.grid(True)
B.pl.legend()

fig2 = B.pl.figure()
B.plot_exp(x, Q, color='black', marker='D', label='Total Charge')
B.pl.xlabel('D2 Elastic Runs')
B.pl.ylabel('Total Charge [mC]')
B.pl.title(r'Total Accumulated Charge, Beam Current > 5 $\mu$A ')
B.pl.grid(True)
B.pl.legend()

fig3 = B.pl.figure()
B.pl.yscale('log')
B.plot_exp(x,  h3of4_rate, color='blue', marker='o', label='HMS 3/4 Rate (kHz)')
B.plot_exp(x,  p3of4_rate, color='red', marker='s', label='SHMS 3/4 Rate (kHz)')
B.plot_exp(x,  coin_rate, color='black', marker='D', label='Coincidence Rate (kHz)')
B.pl.xlabel('D2 Elastic Runs')
B.pl.ylabel('Trigger Rate [kHz]')
B.pl.title(r'Trigger Rates')
B.pl.grid(True)
B.pl.legend()

B.pl.show()
