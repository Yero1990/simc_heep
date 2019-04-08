import numpy as np
import LT.box as B
import matplotlib.ticker as tk
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

f = B.get_file('d2_580_total_report.dat')
run = B.get_data(f, 'Run')
current = B.get_data(f, 'BCM4A_Current')
charge = B.get_data(f, 'BCM4A_Charge')
h3of4_rate = B.get_data(f, 'h3of4_rate')
p3of4_rate = B.get_data(f, 'p3of4_rate')
coin_rate = B.get_data(f, 'coin_rate')
htrk_eff = B.get_data(f, 'htrk_eff')
htrk_eff_err = B.get_data(f, 'htrk_eff_err')
ptrk_eff = B.get_data(f, 'ptrk_eff')
ptrk_eff_err = B.get_data(f, 'ptrk_eff_err')
cpuLT = B.get_data(f, 'cpuLT')
tLT = B.get_data(f, 'tLT')
x_bpm = B.get_data(f, 'x_bpm')
y_bpm = B.get_data(f, 'y_bpm')

ptrig6_acc = B.get_data(f, 'ptrig6_accepted')
ptrig6_acc_err = np.sqrt(ptrig6_acc) 

norm_cnts = ptrig6_acc / charge
norm_cnts_err = ptrig6_acc_err / charge



#with error
#B.plot_exp(run[(run>=3291) & (run<=3305)], ptrk_eff[(run>=3291) & (run<=3305)], ptrk_eff_err[(run>=3291) & (run<=3305)], color='r', marker='o', label = 'Pm = 580 MeV: Set 1' )
#B.plot_exp(run[(run>=3341) & (run<=3356)], ptrk_eff[(run>=3341) & (run<=3356)], ptrk_eff_err[(run>=3341) & (run<=3356)], color='b', marker='s', label = 'Pm = 580 MeV: Set 2' )

'''
#no error
B.plot_exp(run[(run>=3291) & (run<=3305)], charge[(run>=3291) & (run<=3305)], color='r', marker='o', label = 'Pm = 580 MeV: Set 1' )
B.plot_exp(run[(run>=3341) & (run<=3356)], charge[(run>=3341) & (run<=3356)], color='b', marker='s', label = 'Pm = 580 MeV: Set 2' )

B.plot_exp(run, h3of4_rate, color='r', marker='o', label = 'HMS 3/4 Rate: Pm = 580 MeV/c')
B.plot_exp(run, p3of4_rate, color='r', marker='s', label = 'SHMS 3/4 Rate: Pm = 580 MeV/c') 
B.plot_exp(run, coin_rate, color='r', marker='^', label = 'Coin 3/4 Rate: Pm = 580 MeV/c')    
                                                                                           
B.plot_exp(run, cpuLT, color='r', marker='o', label = 'Computer Live Time: Pm = 580 MeV/c')   
B.plot_exp(run, tLT, color='b', marker='s', label = 'Total Live Time: Pm = 580 MeV/c')  
'''

B.plot_exp(run, x_bpm, color='r', marker='o', label = 'X BPM: Pm = 580 MeV/c')       
B.plot_exp(run, y_bpm, color='b', marker='s', label = 'Y BPM: Pm = 580 MeV/c')                                   

f = B.get_file('d2_750_total_report.dat')
run = B.get_data(f, 'Run')
current = B.get_data(f, 'BCM4A_Current')
charge = B.get_data(f, 'BCM4A_Charge')
h3of4_rate = B.get_data(f, 'h3of4_rate')
p3of4_rate = B.get_data(f, 'p3of4_rate')
coin_rate = B.get_data(f, 'coin_rate')
htrk_eff = B.get_data(f, 'htrk_eff')
htrk_eff_err = B.get_data(f, 'htrk_eff_err')
ptrk_eff = B.get_data(f, 'ptrk_eff')
ptrk_eff_err = B.get_data(f, 'ptrk_eff_err')
cpuLT = B.get_data(f, 'cpuLT')
tLT = B.get_data(f, 'tLT')
x_bpm = B.get_data(f, 'x_bpm')
y_bpm = B.get_data(f, 'y_bpm')

ptrig6_acc = B.get_data(f, 'ptrig6_accepted')                                                             
ptrig6_acc_err = np.sqrt(ptrig6_acc)       

norm_cnts = ptrig6_acc / charge                                                                                                  
norm_cnts_err = ptrig6_acc_err / charge                                                                                                               

#B.plot_exp(run, h3of4_rate, color='b', marker='o', label = 'HMS 3/4 Rate: Pm = 750 MeV/c')                                                          
#B.plot_exp(run, p3of4_rate, color='b', marker='s', label = 'SHMS 3/4 Rate: Pm = 750 MeV/c')                                                        
#B.plot_exp(run, coin_rate, color='b', marker='^', label = 'Coin 3/4 Rate: Pm = 750 MeV/c')                                                           

#with error
#B.plot_exp(run[(run>=3306) & (run<=3340)], ptrk_eff[(run>=3306) & (run<=3340)], ptrk_eff_err[(run>=3306) & (run<=3340)], color='r', marker='o', markerfacecolor='none', label = 'Pm = 750 MeV: Set 1' )
#B.plot_exp(run[(run>=3357) & (run<=3367)], ptrk_eff[(run>=3357) & (run<=3367)], ptrk_eff_err[(run>=3357) & (run<=3367)], color='b', marker='s', markerfacecolor='none', label = 'Pm = 750 MeV: Set 2' )
#B.plot_exp(run[(run>=3380) & (run<=3387)], ptrk_eff[(run>=3380) & (run<=3387)], ptrk_eff_err[(run>=3380) & (run<=3387)], color='g', marker='^', markerfacecolor='none', label = 'Pm = 750 MeV: Set 3' )
'''
#no error
B.plot_exp(run[(run>=3306) & (run<=3340)], charge[(run>=3306) & (run<=3340)], color='r', marker='o', markerfacecolor='none', label = 'Pm = 750 MeV: Set 1' )
B.plot_exp(run[(run>=3357) & (run<=3367)], charge[(run>=3357) & (run<=3367)], color='b', marker='s', markerfacecolor='none', label = 'Pm = 750 MeV: Set 2' )
B.plot_exp(run[(run>=3380) & (run<=3387)], charge[(run>=3380) & (run<=3387)], color='g', marker='^', markerfacecolor='none', label = 'Pm = 750 MeV: Set 3' )

B.plot_exp(run, cpuLT, color='r', marker='o', markerfacecolor='none', label = 'Computer Live Time: Pm = 750 MeV/c')        
B.plot_exp(run, tLT, color='b', marker='s', markerfacecolor='none', label = 'Total Live Time: Pm = 750 MeV/c')   

'''
B.plot_exp(run, x_bpm, color='r', marker='o', markerfacecolor='none', label = 'X BPM: Pm = 750 MeV/c')    
B.plot_exp(run, y_bpm, color='b', marker='s', markerfacecolor='none', label = 'Y BPM: Pm = 750 MeV/c')    

B.pl.legend()
B.pl.xlabel('Run Number')
B.pl.ylabel(r'Beam Positions [cm]')
#B.pl.yscale('log')
B.pl.title('Beam Position vs Run Number')
B.pl.grid(True)
B.pl.show()

