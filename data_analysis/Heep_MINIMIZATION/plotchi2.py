#Python code for plotting relevant quantities like: 
# --> theoretical derivatives of W or fractional momentum, ... 
# --> measured quantities like chi2, . . .

import numpy as np
import LT.box as B
import matplotlib.ticker as tk
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D


#Define Some Constants                                                                                                     
dtr = np.pi / 180.    #degrees to radians                                                                                                                                 
Mp = 0.938272           #proton mass                                                                                                                                                


#Plotting reduced chi2 vs. relative quantities, while holding fixed other relative quantities, to observe sensitivity
'''
f = B.get_file('sorted_chi2.txt')
dEb_Eb = B.get_data(f, 'dEb_Eb')
dP_P = B.get_data(f, 'dP_P')
dth = B.get_data(f, 'dth')
redchi2 = B.get_data(f, 'total_redChi2')


#-------chi2 vs. HMS Angle Variations, dtheta-------------

#dP_P_arr = [-0.0035, -0.0025, -0.0015, -0.0005, 0.0005, 0.0025]

dP_P_arr = [-0.0025, -0.0015, -0.0005, 0.0, 0.0005, 0.0015, 0.0025]
dEb_Eb_arr = [0.0, 0.001]

color_arr = ['black', 'red', 'blue', 'cyan', 'green', 'magenta', 'purple']
marker_arr = ['X', 'D', '^', 'o', 's', 'v', 'P']


for i in range(0, len(dP_P_arr)):
    for j in range(0, len(dEb_Eb_arr)):
        #print ('dP_P[i] = ', dP_P_arr[i], 'dEb_Eb[j] = ',dEb_Eb_arr[j], 'color[k] = ',color_arr[i])
        if j==0:
            B.plot_exp(dth[(dP_P==dP_P_arr[i]) & (dEb_Eb==dEb_Eb_arr[j])], redchi2[(dP_P==dP_P_arr[i]) & (dEb_Eb==dEb_Eb_arr[j])], color=color_arr[i], marker=marker_arr[i], label=r'$\frac{dEb}{Eb}$ = %s,  $\frac{dP}{P}$ = %s'%(dEb_Eb_arr[j],dP_P_arr[i]))
        if j==1:
            B.plot_exp(dth[(dP_P==dP_P_arr[i]) & (dEb_Eb==dEb_Eb_arr[j])], redchi2[(dP_P==dP_P_arr[i]) & (dEb_Eb==dEb_Eb_arr[j])], color=color_arr[i], markerfacecolor='none', marker=marker_arr[i], label=r'$\frac{dEb}{Eb}$ = %s,  $\frac{dP}{P}$ = %s'%(dEb_Eb_arr[j],dP_P_arr[i]))


B.pl.legend(loc='lower right')
B.pl.grid(True, which='both')
#B.pl.yscale('log')
ax = B.pl.axes()
ax.set_yscale('log')
ax.set_yticks([100, 130, 200, 500, 700, 1000,5000, 10000, 20000])
ax.get_yaxis().set_major_formatter(tk.ScalarFormatter())
B.pl.title(r'Reduced Chi2 ($\chi^{2}_{\nu}$) vs. HMS Angle Variations')
B.pl.xlabel(r'HMS Angle Variations, d$\theta$[rad]')
B.pl.ylabel(r'Total Reduced $\chi^{2}_{\nu}$')
B.pl.xticks(np.arange(-0.0030, 0.0030, step=0.0005))
B.pl.xlim(-0.0022, 0.0032)
B.pl.show()

#----------------------------



#-------chi2 vs. HMS Relative Momentum, dP/P-------------

#dth_arr = [-0.0015, -0.001, -0.0005, 0.0005, 0.001, 0.0015]

dth_arr = [-0.001, -0.0005, 0.0, 0.0005, 0.001]
dEb_Eb_arr = [0.0, 0.001]

color_arr = ['black', 'red', 'blue', 'magenta', 'green']
marker_arr = ['D', '^', 'o', 's', 'v']

for i in range(0, len(dth_arr)):
    for j in range(0, len(dEb_Eb_arr)):
        if j==0:
            B.plot_exp(dP_P[(dth==dth_arr[i]) & (dEb_Eb==dEb_Eb_arr[j])], redchi2[(dth==dth_arr[i]) & (dEb_Eb==dEb_Eb_arr[j])], color=color_arr[i], marker=marker_arr[i], label=r'$\frac{dEb}{Eb}$ = %s,  $\delta\theta$ = %s (rad)'%(dEb_Eb_arr[j],dth_arr[i]))
        if j==1:
            B.plot_exp(dP_P[(dth==dth_arr[i]) & (dEb_Eb==dEb_Eb_arr[j])], redchi2[(dth==dth_arr[i]) & (dEb_Eb==dEb_Eb_arr[j])], color=color_arr[i], markerfacecolor='none', marker=marker_arr[i], label=r'$\frac{dEb}{Eb}$ = %s,  $\delta\theta$ = %s (rad)'%(dEb_Eb_arr[j],dth_arr[i]))


B.pl.legend(loc='lower right')
B.pl.grid(True, which='both')
#B.pl.yscale('log')
ax = B.pl.axes()
ax.set_yscale('log')
ax.set_yticks([100, 130, 200, 500, 700, 1000,5000, 10000, 20000])
ax.get_yaxis().set_major_formatter(tk.ScalarFormatter())
B.pl.title(r'Reduced Chi2 ($\chi^{2}_{\nu}$) vs. HMS Momentum Fraction')
B.pl.xlabel(r'HMS Momentum Fraction, $\frac{dP}{P}$')
B.pl.ylabel(r'Total Reduced $\chi^{2}_{\nu}$')
B.pl.xticks(np.arange(-0.0045, 0.0060, step=0.0005))
B.pl.xlim(-0.0045, 0.0060)
B.pl.show()



#-------------chi2 vs. relative beam energy, dEb / Eb---------
#dth_arr = [-0.001, -0.0005]  
#dP_P_arr = [-0.0015, -0.0005, 0.0005, 0.0015]  #, -0.0015, -0.0005, 0.0005, 0.0025]

dth_arr = [-0.001, -0.0005, 0.]  
dP_P_arr = [-0.0015, -0.001, -0.0005, 0.00]


color_arr = ['red', 'blue', 'green']#, 'magenta']
marker_arr = ['o', 's', 'D'] #, 'P']

for i in range(0, len(dth_arr)):
    for j in range(0, len(dP_P_arr)):
        if j==0:
            B.plot_exp(dEb_Eb[(dth==dth_arr[i]) & (dP_P==dP_P_arr[j])], redchi2[(dth==dth_arr[i]) & (dP_P==dP_P_arr[j])], color=color_arr[i], fillstyle='full', markersize=6, marker=marker_arr[i], label=r'$\frac{dP}{P}$ = %s,  $\delta\theta$ = %s (rad)'%(dP_P_arr[j],dth_arr[i]))
        if j==1:
            B.plot_exp(dEb_Eb[(dth==dth_arr[i]) & (dP_P==dP_P_arr[j])], redchi2[(dth==dth_arr[i]) & (dP_P==dP_P_arr[j])], color=color_arr[i], fillstyle='top', markersize=6, marker=marker_arr[i], label=r'$\frac{dP}{P}$ = %s,  $\delta\theta$ = %s (rad)'%(dP_P_arr[j],dth_arr[i]))
        if j==2:
            B.plot_exp(dEb_Eb[(dth==dth_arr[i]) & (dP_P==dP_P_arr[j])], redchi2[(dth==dth_arr[i]) & (dP_P==dP_P_arr[j])], color=color_arr[i], fillstyle='right',markersize=6, marker=marker_arr[i], label=r'$\frac{dP}{P}$ = %s,  $\delta\theta$ = %s (rad)'%(dP_P_arr[j],dth_arr[i]))
        if j==3:
            B.plot_exp(dEb_Eb[(dth==dth_arr[i]) & (dP_P==dP_P_arr[j])], redchi2[(dth==dth_arr[i]) & (dP_P==dP_P_arr[j])], color=color_arr[i], fillstyle='none', markersize=6,marker=marker_arr[i], label=r'$\frac{dP}{P}$ = %s,  $\delta\theta$ = %s (rad)'%(dP_P_arr[j],dth_arr[i]))


            
B.pl.legend(loc='upper right')
B.pl.grid(True, which='both')
#B.pl.yscale('log')
ax = B.pl.axes()
ax.set_yscale('log')
ax.set_yticks([100, 130, 200, 500, 700, 1000,5000])
ax.get_yaxis().set_major_formatter(tk.ScalarFormatter())
B.pl.title(r'Reduced Chi2 ($\chi^{2}_{\nu}$) vs. Beam Energy Variations')
B.pl.xlabel(r'Beam Energy Fraction, $\frac{dE_{b}}{E_{b}}$')
B.pl.ylabel(r'Total Reduced $\chi^{2}_{\nu}$')
#B.pl.xticks(np.arange(-0.0045, 0.0060, step=0.0005))
B.pl.xlim(-0.0011, 0.00155)
B.pl.show()

#----------------------------
'''

f = B.get_file('hms_kin.data')
run = B.get_data(f, 'Run')
particle = B.get_data(f, 'particle')
hmsP = B.get_data(f, 'nmr_P')
hmsAng = B.get_data(f, 'hms_Angle')
Eb = B.get_data(f, 'beam_e')
data_W = B.get_data(f, 'data_W_mean')
data_W_err = B.get_data(f, 'data_W_mean_err')
simc_W = B.get_data(f, 'simc_W_mean')
simc_W_err = B.get_data(f, 'simc_W_mean_err')
data_Pfr = B.get_data(f, 'data_Pfr_mean')
data_Pfr_err = B.get_data(f, 'data_Pfr_mean_err') 
simc_Pfr = B.get_data(f, 'simc_Pfr_mean')                                                
simc_Pfr_err = B.get_data(f, 'simc_Pfr_mean_err')                                        



#PLOT (W_simc - W_data) vs. E' for difference beam energies (Measured DATA/SIMC quantities)
dW_meas = simc_W - data_W
dW_meas_err = np.sqrt(simc_W_err**2 + data_W_err**2)


B.plot_exp(hmsP[(Eb==10.6005) & (particle=='e')], dW_meas[(Eb==10.6005) & (particle=='e') ], dW_meas_err[(Eb==10.6005) & (particle=='e')], color='r', marker='o', label = 'Eb = 10.6005 GeV' )
B.plot_exp(hmsP[(Eb==6.42765) & (particle=='e')], dW_meas[(Eb==6.42765) & (particle=='e') ], dW_meas_err[(Eb==6.42765) & (particle=='e')], color='b', marker='s', label = 'Eb = 6.42765 GeV' )
B.plot_exp(hmsP[(Eb==3.83350) & (particle=='e')], dW_meas[(Eb==3.83350) & (particle=='e') ], dW_meas_err[(Eb==3.83350) & (particle=='e')], color='g', marker='^', label = 'Eb = 3.83350 GeV' )
B.plot_exp(hmsP[(Eb==4.93090) & (particle=='e')], dW_meas[(Eb==4.93090) & (particle=='e') ], dW_meas_err[(Eb==4.93090) & (particle=='e')], color='c', marker='v', label = 'Eb = 4.93090 GeV' )

#Plot Predicted dW vs. P
#Assume: dP/P = -0.0015,  dth = -0.0005, dEb/Eb = 0.0004
dP_P = -0.0015
dth = -0.0005
dEb_Eb = 0.0008


#Calculated W derivatives

#dW_dEb = hmsP[particle=='e']/Eb[particle=='e']
#dW_dP = -Eb[particle=='e']/hmsP[particle=='e']
#dW_dth = -2 * Eb[particle=='e'] * hmsP[particle=='e'] * np.sin(th/2) * np.cos(th/2) / Mp
Eb_0 = 10.6005
Eb_1 = 6.42765
Eb_2 = 3.83350
Eb_3 = 4.93090

dW_pred0 = hmsP[(Eb==Eb_0) & (particle=='e')] * dEb_Eb - Eb[(Eb==Eb_0) & (particle=='e')]*dP_P - 2 * Eb[(Eb==Eb_0) & (particle=='e')] * hmsP[(Eb==Eb_0) & (particle=='e')] * np.sin(hmsAng[(Eb==Eb_0) & (particle=='e')]*dtr/2) * np.cos(hmsAng[(Eb==Eb_0) & (particle=='e')]/2) / Mp * dth
dW_pred1 = hmsP[(Eb==Eb_1) & (particle=='e')] * dEb_Eb - Eb[(Eb==Eb_1) & (particle=='e')]*dP_P - 2 * Eb[(Eb==Eb_1) & (particle=='e')] * hmsP[(Eb==Eb_1) & (particle=='e')] * np.sin(hmsAng[(Eb==Eb_1) & (particle=='e')]*dtr/2) * np.cos(hmsAng[(Eb==Eb_1) & (particle=='e')]/2) / Mp * dth
dW_pred2 = hmsP[(Eb==Eb_2) & (particle=='e')] * dEb_Eb - Eb[(Eb==Eb_2) & (particle=='e')]*dP_P - 2 * Eb[(Eb==Eb_2) & (particle=='e')] * hmsP[(Eb==Eb_2) & (particle=='e')] * np.sin(hmsAng[(Eb==Eb_2) & (particle=='e')]*dtr/2) * np.cos(hmsAng[(Eb==Eb_2) & (particle=='e')]/2) / Mp * dth
dW_pred3 = hmsP[(Eb==Eb_3) & (particle=='e')] * dEb_Eb - Eb[(Eb==Eb_3) & (particle=='e')]*dP_P - 2 * Eb[(Eb==Eb_3) & (particle=='e')] * hmsP[(Eb==Eb_3) & (particle=='e')] * np.sin(hmsAng[(Eb==Eb_3) & (particle=='e')]*dtr/2) * np.cos(hmsAng[(Eb==Eb_3) & (particle=='e')]/2) / Mp * dth


B.plot_exp(hmsP[(Eb==Eb_0) & (particle=='e')], dW_pred0, color='r', marker='o', markersize=12, markerfacecolor='none', label = 'dW Predicted: Eb = 10.6005 GeV' )
B.plot_exp(hmsP[(Eb==Eb_1) & (particle=='e')], dW_pred1, color='b', marker='s', markersize=12, markerfacecolor='none', label =  'dW Predicted: Eb = 6.42765 GeV' )
B.plot_exp(hmsP[(Eb==Eb_2) & (particle=='e')], dW_pred2, color='g', marker='^', markersize=12, markerfacecolor='none', label =  'dW Predicted: Eb = 3.83350 GeV' )
B.plot_exp(hmsP[(Eb==Eb_3) & (particle=='e')], dW_pred3, color='c', marker='v', markersize=12, markerfacecolor='none', label =  'dW Predicted: Eb = 4.93090 GeV' )

B.pl.legend()
B.pl.xlabel('HMS Central Momentum [GeV]')
B.pl.ylabel(r'W Variations, dW [GeV]')
B.pl.title('HMS Electrons: dW vs. Momentum')
B.pl.grid(True)
B.pl.show()


'''

#Plot Momentum Fraction Measured/Predicted vs. Central Momentum

#Select ONLY protons
Eb = Eb[particle=='p']
P = hmsP[particle=='p']
th = hmsAng[particle=='p'] * dtr

simc_Pfr = simc_Pfr[particle=='p']
data_Pfr = data_Pfr[particle=='p']

simc_Pfr_err = simc_Pfr_err[particle=='p']
data_Pfr_err = data_Pfr_err[particle=='p']

#Measured dPfrac 
dPfr_meas = (simc_Pfr - data_Pfr)*P
dPfr_meas_err = np.sqrt((simc_Pfr_err*P)**2 + (data_Pfr_err*P)**2) 

#dPfr_meas = (simc_Pfr - data_Pfr)
#dPfr_meas_err = np.sqrt((simc_Pfr_err)**2 + (data_Pfr_err)**2) 


#------calculated derivatives for predicted dPfr--------
Pcalc =  2*Mp*Eb*(Eb+Mp)*np.cos(th) / ( Mp**2 + 2*Mp*Eb + pow(Eb*np.sin(th),2) )

dPcalc_dEb = 2.* Mp * (2.*Eb + Mp)* np.cos(th) / (Mp*Mp + 2.*Mp*Eb + np.power(Eb*np.sin(th),2) ) - ( 4. * Mp*Eb*(Eb+Mp)*np.cos(th) * (Mp + Eb*np.sin(th)*np.sin(th)) ) / ( np.power(Mp*Mp + 2.*Mp*Eb+np.power(Eb*np.sin(th),2),2) )
dPcalc_dth = -2.*Mp*Eb*(Eb + Mp)*np.sin(th) / (Mp*Mp + 2.*Mp*Eb + np.power(Eb*np.sin(th),2) ) - 2.*Mp*Eb*(Eb + Mp)*np.cos(th) / (np.power( Mp*Mp + 2.*Mp*Eb + np.power(Eb*np.sin(th),2),2 )) * Eb*Eb*(2.*np.sin(th) * np.cos(th)) 


#dPfr_dEb =  dPcalc_dEb / P
#dPfr_dP = - Pcalc / pow(P,2)
#dPfr_dth = dPcalc_dth / P

dPfr_dEb =  dPcalc_dEb 
dPfr_dP = - 1
dPfr_dth = dPcalc_dth

#Assume relative uncertainties: dP/P = -0.0015,  dth = -0.0005, dEb/Eb = 0.0008
#(For now, these give the lowest reduced chi2)
dP_P = -0.0015
dth = -0.0005
dEb_Eb = 0.0008


dPfr_pred =  (dPfr_dEb * Eb * dEb_Eb  +  dPfr_dP * P * dP_P +  dPfr_dth * dth)*P
#dPfr_pred =  (dPfr_dEb * Eb * dEb_Eb  +  dPfr_dP * P * dP_P +  dPfr_dth * dth)

#Plot Measured dPfr
B.plot_exp(P, dPfr_meas, dPfr_meas_err, marker='o', color='red', label=r'Measured: $E_{b}$=10.6005 GeV')

#Plot Predicted dPfr
B.plot_exp(P, dPfr_pred, marker='s', color='blue', label=r'Predicted: $E_{b}$=10.6005 GeV')
B.pl.title('HMS Momentum Difference vs. Central Momentum')
B.pl.xlabel('HMS Momentum [GeV]')
B.pl.ylabel('HMS Momentum Difference, dP [GeV]')
B.pl.legend()
B.pl.grid(True)
B.pl.show()

'''
#Plot Momentum Correction, Assuming dth = 0 and dEb / Eb = 0, for different beam energies
#Beam Energy Configurations 
Eb0 = 10.6005
Eb1 = 6.42765
Eb2 = 4.93090
Eb3 = 3.83350

#------Electrons--------
 
#dW = Wsimc - Wdata
dW0 = simc_W[(particle=='e') & (Eb==Eb0)] - data_W[(particle=='e') & (Eb==Eb0)]
dW1 = simc_W[(particle=='e') & (Eb==Eb1)] - data_W[(particle=='e') & (Eb==Eb1)]
dW2 = simc_W[(particle=='e') & (Eb==Eb2)] - data_W[(particle=='e') & (Eb==Eb2)]
dW3 = simc_W[(particle=='e') & (Eb==Eb3)] - data_W[(particle=='e') & (Eb==Eb3)]

dW0_err = np.sqrt(simc_W_err[(particle=='e') & (Eb==Eb0)]**2 + data_W_err[(particle=='e') & (Eb==Eb0)]**2)
dW1_err = np.sqrt(simc_W_err[(particle=='e') & (Eb==Eb1)]**2 + data_W_err[(particle=='e') & (Eb==Eb1)]**2)
dW2_err = np.sqrt(simc_W_err[(particle=='e') & (Eb==Eb2)]**2 + data_W_err[(particle=='e') & (Eb==Eb2)]**2)
dW3_err = np.sqrt(simc_W_err[(particle=='e') & (Eb==Eb3)]**2 + data_W_err[(particle=='e') & (Eb==Eb3)]**2)

#Electron Momentum Correction Factor
Pcorr_elec0 = (1 - dW0 / Eb0)
Pcorr_elec1 = (1 - dW1 / Eb1)
Pcorr_elec2 = (1 - dW2 / Eb2)
Pcorr_elec3 = (1 - dW3 / Eb3)

#Error
Pcorr_elec0_err = dW0_err / Eb0
Pcorr_elec1_err = dW1_err / Eb1
Pcorr_elec2_err = dW2_err / Eb2
Pcorr_elec3_err = dW3_err / Eb3


#------Protons--------

#dPf = Pfr_simc - Pfr_data
#Pcorr = P + P*dPf = P*(1+ dPf)

dPf0 = simc_Pfr[(particle=='p') & (Eb==Eb0)] - data_Pfr[(particle=='p') & (Eb==Eb0)]
dPf0_err = np.sqrt(simc_Pfr_err[(particle=='p') & (Eb==Eb0)]**2 + data_Pfr_err[(particle=='p') & (Eb==Eb0)]**2)

#---ONLY Applied when Pfr = (Pcalc - Pmeas) / Pmeas
#Pcorr_prot0 = (1 - dPf0)
#Pcorr_prot0_err = dPf0_err

Pcorr_prot0 = (1 - dPf0/hmsP[(particle=='p') & (Eb==Eb0)])
Pcorr_prot0_err = dPf0_err/hmsP[(particle=='p') & (Eb==Eb0)]


#Plot
B.plot_exp(hmsP[(particle=='e') & (Eb==Eb0)], Pcorr_elec0, Pcorr_elec0_err, marker='o', color='r', label=r'HMS $e^{-}, E_{b}=%s$'%Eb0)
B.plot_exp(hmsP[(particle=='e') & (Eb==Eb1)], Pcorr_elec1, Pcorr_elec1_err, marker='s', color='b', label=r'HMS $e^{-}, E_{b}=%s$'%Eb1)
B.plot_exp(hmsP[(particle=='e') & (Eb==Eb2)], Pcorr_elec2, Pcorr_elec2_err, marker='^', color='g', label=r'HMS $e^{-}, E_{b}=%s$'%Eb2)
B.plot_exp(hmsP[(particle=='e') & (Eb==Eb3)], Pcorr_elec3, Pcorr_elec3_err, marker='D', color='magenta', label=r'HMS $e^{-}, E_{b}=%s$'%Eb3)

B.plot_exp(hmsP[(particle=='p') & (Eb==Eb0)], Pcorr_prot0, Pcorr_prot0_err, marker='o', fillstyle='none', color='r', label=r'HMS $p^{+}, E_{b}=%s$'%Eb0)

B.pl.xlabel('HMS Central Momentum [GeV]')
B.pl.ylabel('HMS Momentum Correction Factor')
B.pl.title('HMS: Momentum Correction Factor vs. Central Momentum')
B.pl.grid(True)
B.pl.legend()
B.pl.show()



'''
#---------------------------------
#---PLOTTING W derivatives--------
#---------------------------------

#-------Possible independent variables-------------

#dW can be plotted as a function of relative uncertainties or absolute values Eb, P, theta

#relative uncertainties
#dEb_Eb = np.linspace(-0.001, 0.001, 100, endpoint=True)    #(-0.1 to 0.1 %)
#dP_P = np.linspace(-0.004, 0.004, 100, endpoint=True)      #(-0.4 to 0.4 %)
#dth = np.linspace(-0.002, 0.002, 100, endpoint=True )      #(-2 mr to 2 mr %)

#Beam, momentum, angle
#Eb = np.linspace(3, 11, 100, endpoint=True)   #beam energy, steps of 1 MeV
#P = np.linspace(1, 5, 100, endpoint=True)      #momentum, steps of 1 MeV
#theta = np.linspace(10, 60, 100, endpoint=True)     #angle, steps of 0.01 deg

#------------------------------------------------

#derivatives
#dW_dEb = P/Eb
#dW_dP = -Eb/P
#dW_dth = -2 * Eb * P * np.sin(theta/2) * np.cos(theta/2) / Mp

#------If the independent variables are absolute quantities-------

#relative beam energy configurations
dEb_Eb1 = -0.01  
dEb_Eb2 = -0.008
dEb_Eb3 = -0.006
dEb_Eb4 = -0.004
dEb_Eb5 = -0.002

dEb_Eb6 = 0.002
dEb_Eb7 = 0.004
dEb_Eb8 = 0.006
dEb_Eb9 = 0.008
dEb_Eb10 = 0.01

dW_beam1 = P * dEb_Eb1
dW_beam2 = P * dEb_Eb2
dW_beam3 = P * dEb_Eb3
dW_beam4 = P * dEb_Eb4
dW_beam5 = P * dEb_Eb5
dW_beam6 = P * dEb_Eb6
dW_beam7 = P * dEb_Eb7
dW_beam8 = P * dEb_Eb8
dW_beam9 = P * dEb_Eb9
dW_beam10 = P * dEb_Eb10


fig1 = plt.figure()
plt.plot(P, dW_beam1, color='r', label='dEb/Eb = -0.01')
plt.plot(P, dW_beam2, color='g', label='dEb/Eb = -0.008')
plt.plot(P, dW_beam3, color='b', label='dEb/Eb = -0.006')
plt.plot(P, dW_beam4, color='cyan', label='dEb/Eb = -0.004')
plt.plot(P, dW_beam5, color='black', label='dEb/Eb = -0.002')

plt.plot(P, dW_beam6, color='black', linestyle='--', label='dEb/Eb = 0.002')
plt.plot(P, dW_beam7, color='cyan', linestyle='--', label='dEb/Eb = 0.004')
plt.plot(P, dW_beam8, color='b', linestyle='--', label='dEb/Eb = 0.006')
plt.plot(P, dW_beam9, color='g', linestyle='--', label='dEb/Eb = 0.008')
plt.plot(P, dW_beam10, color='r', linestyle='--', label='dEb/Eb = 0.01')

plt.title('W variations vs. HMS Central Momentum')
plt.xlabel('HMS Central Momentum [GeV]')
plt.ylabel(r'$\delta$W Variations [GeV]')

plt.legend()

#----------------------------------

#relative momentum configurations
dP_P1 = -0.01  
dP_P2 = -0.008
dP_P3 = -0.006
dP_P4 = -0.004
dP_P5 = -0.002

dP_P6 = 0.002
dP_P7 = 0.004
dP_P8 = 0.006
dP_P9 = 0.008
dP_P10 = 0.01

dW_P1 = -Eb * dP_P1
dW_P2 = -Eb * dP_P2
dW_P3 = -Eb * dP_P3
dW_P4 = -Eb * dP_P4
dW_P5 = -Eb * dP_P5
dW_P6 = -Eb * dP_P6
dW_P7 = -Eb * dP_P7
dW_P8 = -Eb * dP_P8
dW_P9 = -Eb * dP_P9
dW_P10 = -Eb * dP_P10


fig2 = plt.figure()
plt.plot(Eb, dW_P1, color='r', label='dP/P = -0.01')
plt.plot(Eb, dW_P2, color='g', label='dP/P = -0.008')
plt.plot(Eb, dW_P3, color='b', label='dP/P = -0.006')
plt.plot(Eb, dW_P4, color='cyan', label='dP/P = -0.004')
plt.plot(Eb, dW_P5, color='black', label='dP/P = -0.002')

plt.plot(Eb, dW_P6, color='black', linestyle='--', label='dP/P = 0.002')
plt.plot(Eb, dW_P7, color='cyan', linestyle='--', label='dP/P = 0.004')
plt.plot(Eb, dW_P8, color='b', linestyle='--', label='dP/P = 0.006')
plt.plot(Eb, dW_P9, color='g', linestyle='--', label='dP/P = 0.008')
plt.plot(Eb, dW_P10, color='r', linestyle='--', label='dP/P = 0.01')

plt.title('W variations vs. Beam Energy')
plt.xlabel('Beam Energy [GeV]')
plt.ylabel(r'$\delta$W Variations [GeV]')

plt.legend()

plt.show()
'''

#dW_mom = -Eb * dP_P
#dW_ang = -2 * Eb * P * np.sin(theta/2) * np.cos(theta/2) / Mp * dth


#-----------------------------------------------------------------




#fig1 = plt.figure()
#plt.plot(dEb_Eb, dW_beam, color='r')
#plt.show()

#fig2 = plt.figure()
#plt.plot(dP_P, dW_mom, color='b')
#plt.show()

#fig3 = plt.figure()
#plt.plot(dth, dW_ang, color='b')
#plt.show()
