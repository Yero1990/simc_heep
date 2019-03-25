import LT.box as B
import numpy as np
import matplotlib.pyplot as plt

f = B.get_file('table.txt')
Run = B.get_data(f, 'Run')
TLT = B.get_data(f, 'tLT')
CLT = B.get_data(f, 'cpuLT')
hTrk = B.get_data(f, 'hms_trkEff')
hTrk_err = B.get_data(f, 'hms_trkEff_err')

p1Trk = B.get_data(f, 'shms_1trkEff')
p1Trk_err = B.get_data(f, 'shms_1trkEff_err')

p_multTrk = B.get_data(f, 'shms_mult_trkEff')
p_multTrk_err = B.get_data(f, 'shms_mult_trkEff_err')

p_allTrk = B.get_data(f, 'shms_all_trkEff')             #Tracking Efficiency Using Pre-Existing Method
p_allTrk_err = B.get_data(f, 'shms_all_trkEff_err')

p_allTrk_corr = B.get_data(f, 'shms_corr_trkEff')        #Weighted Tracking Efficiency using T. Horn's Method
p_allTrk_err_corr = B.get_data(f, 'shms_corr_trkEff_err')

h3of4_rate = B.get_data(f, 'hms3of4_rate')
p3of4_rate = B.get_data(f, 'shms3of4_rate')
coin_rate = B.get_data(f, 'coin_rate')
S1X_rate = B.get_data(f, 'S1X_rate')


fig0 = B.pl.figure()
B.plot_exp(coin_rate,  TLT, color='black', marker='D', label='Total Live Time')
B.plot_exp(coin_rate,  CLT, color='red', marker='D', label='Computer Live Time')
B.pl.ylim(0.7, 1)
B.pl.xlabel('Coincidence Rate [kHz]')
B.pl.ylabel('Live Time')

B.pl.title('Total/Computer Live Time')
B.pl.grid(True)
B.pl.legend()

#Plotting Tracking Efficiencies
fig1 = B.pl.figure()
#B.plot_exp(S1X_rate,  p_allTrk, p_allTrk_err, color='black', marker='s', label='Trk Eff. (Original)')
#B.plot_exp(S1X_rate,  p_allTrk_corr, p_allTrk_err_corr, color='red', marker='o', label='Trk Eff.(T. Horn Method)')
#B.plot_exp(S1X_rate,  p1Trk, p1Trk_err, color='blue', marker='o', label='Trk Eff. (Single Tracks)')

B.plot_exp(p3of4_rate,  p_allTrk, p_allTrk_err, color='black', marker='s', label='Trk Eff. (Original)')
B.plot_exp(p3of4_rate,  p_allTrk_corr, p_allTrk_err_corr, color='red', marker='o', label='Trk Eff.(T. Horn Method)')
B.plot_exp(p3of4_rate,  p1Trk, p1Trk_err, color='blue', marker='o', label='Trk Eff. (Single Tracks)')


#B.plot_exp(S1X_rate,  p_multTrk, p_multTrk_err, color='green', marker='o', label='Trk Eff. (Multiple Tracks)')
B.pl.ylim(0.7, 1)
B.pl.xlim(0, 650)
B.pl.xlabel('SHMS 3/4 Rate [kHz]')
B.pl.ylabel('Tracking Efficiencies')
B.pl.title('SHMS Tracking Efficiencies vs. 3/4 Rate')
B.pl.grid(True)
B.pl.legend()


'''
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
'''

B.pl.show()
