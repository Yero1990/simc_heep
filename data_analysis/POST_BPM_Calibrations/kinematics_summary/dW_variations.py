import LT.box as B
import numpy as np
import matplotlib.pyplot as plt


'''
#--------W DERIVATIVES for H(e,e'p) Elastics ONLY----------
Definitions: 
E' electron momentum  
Eb beam energy
Mp proton mass
W invariant mass
th_e  electron angle

Invariant Mass: W2 = Mp**2 + 2*M*(Eb-E') - Q2,   

# dW / dEb = E' / Eb

# dW / dE' = - Eb / E'

# dW / dth_e = - 2 * Eb * E' * sin(th_e/2) * cos(th_e/2) / Mp

In general,  dV = V_final - V_initial = V_corr - V_uncorr
dW = W_simc - W_data (fitted peak)
dEb = Eb_f - Eb_i
dth_e = th_e_f - th_e_i

--------------------------------------------------

HMS Momentum Correction Using Protons

Brief: Since HMS detects protons, its momentum cannot be corrected using W peak.
Here, calculated and measured (Pcalc, Pmeas) momenta are determined by a code.
Pcalc--> Uses formula that does NOT depend on the focal plane quantities
Pmeas--> Used measured delta which depende on focal plane quantities
(Pcalc - Pmeas) / Pmeas is fitted and determined for both DATA/SIMC.
Then, a momentum correction is determined so as to get DATA to match SIMC.


'''

'''
#Define Some Constants
dtr = np.pi / 180.    #degrees to radians
Mp = 0.938272           #proton mass


#---------------------------------------
# ANALYZE HMS ELECTRON MOMENTUM, W peak
#---------------------------------------

#Read in data file with the kinematics and fit information
f = B.get_file('kg.data')
kg = B.get_data(f, 'kin_group')         #kin. group
hNMR = B.get_data(f, 'nmr_true')        #HMS Dipole NMR [T]
hmsP = B.get_data(f, 'nmr_P')           #HMS Momentum [GeV]
hmsAng = B.get_data(f, 'hms_Angle')
shmsP = B.get_data(f, 'shms_P')
shmsAng = B.get_data(f, 'shms_Angle')
beamE = B.get_data(f, 'beam_e')
dataW = B.get_data(f, 'data_W_mean')
dataW_err = B.get_data(f, 'data_W_mean_err')
simcW = B.get_data(f, 'simc_W_mean')
simcW_err = B.get_data(f, 'simc_W_mean_err')

#Add Keys to existing data file
f.add_key('dW', 'f')
f.add_key('dW_err', 'f')

f.add_key('dEe', 'f')
f.add_key('dEe_err', 'f')

f.add_key('dEb', 'f')
f.add_key('dEb_err', 'f')

f.add_key('dth_e', 'f')
f.add_key('dth_e_err', 'f')

f.add_key('Ee_corr_factor')
f.add_key('Ee_corr_factor_err')

f.add_key('Ee_corr')


#define lists to store values
dW_list = []
dW_err_list = []

dEb_list = []
dEb_err_list = []

dEe_list = []
dEe_err_list = []

dthe_list = []
dthe_err_list = []

Ee_factor_list = []
Ee_factor_err_list = []


#Loop over kinematic group 
for i, ikg in enumerate(kg):
    
    th_e = hmsAng[i] * dtr     #convert electron angle to radians

    #Variation in W peak
    dW = simcW[i] -  dataW[i]
    dW_err = np.sqrt(simcW_err[i]**2 + dataW_err[i]**2)
    
    #Variations in Eb, 
    dEb = dW / (hmsP[i]/beamE[i])
    dEb_err = dW_err / (hmsP[i]/beamE[i])
    

    #Variations in E', 
    dEe = -dW / (beamE[i]/hmsP[i])
    dEe_err = -dW_err / (beamE[i]/hmsP[i])

    #Variations in Angle
    dth_e = -dW / (2 * beamE[i] * hmsP[i] * np.sin(th_e/2) * np.cos(th_e/2) / Mp)
    dth_e_err = -dW_err / (2 * beamE[i] * hmsP[i] * np.sin(th_e/2) * np.cos(th_e/2) / Mp)

    #Calculate Correction factor for E'  (E' corr = E' + dE')
    Ee_corr_factor = 1 - (dW / beamE[i])
    Ee_corr_factor_err = (dW_err / beamE[i])

    Ee_corr = hmsP[i] * Ee_corr_factor 

    #Add Values to keys
    f.data[i]['dW'] = round(dW,4)
    f.data[i]['dW_err'] = round(dW_err,4)
    f.data[i]['dEb'] = round(dEb,4)
    f.data[i]['dEb_err'] = round(dEb_err,4)
    f.data[i]['dEe'] = round(dEe,4)
    f.data[i]['dEe_err'] = round(dEe_err,4)
    f.data[i]['dth_e'] = round(dth_e,4)
    f.data[i]['dth_e_err'] = round(dth_e_err,4)
    f.data[i]['Ee_corr_factor'] =round( Ee_corr_factor,6)
    f.data[i]['Ee_corr_factor_err'] =round( Ee_corr_factor_err,6)
    f.data[i]['Ee_corr'] =round( Ee_corr,6)

    #Append to list
    dW_list.append(dW)
    dW_err_list.append(dW_err)
    
    dEb_list.append(dEb/beamE[i])
    dEb_err_list.append(dEb_err/beamE[i])

    dEe_list.append(dEe/hmsP[i])
    dEe_err_list.append(dEe_err/hmsP[i])

    dthe_list.append(dth_e/th_e)
    dthe_err_list.append(dth_e_err/th_e)
    
    Ee_factor_list.append(Ee_corr_factor)
    Ee_factor_err_list.append(Ee_corr_factor_err)


#f.save('hElecMomentum_Corr.txt')
'''

#-----------------------------------
# ANALYZE HMS PROTON MOMENTUM
# (Determine the Momentum Corr. Factor)
#-----------------------------------

#Read in data file with the kinematics and fit information
f1 = B.get_file('d2_heep.data')
run = B.get_data(f1, 'Run')         #kin. group
hNMR_prot = B.get_data(f1, 'nmr_true')        #HMS Dipole NMR [T]
hmsP_prot = B.get_data(f1, 'nmr_P')           #HMS Momentum [GeV]
hmsAng_prot = B.get_data(f1, 'hms_Angle')
shmsP_elec = B.get_data(f1, 'shms_P')
shmsAng_elec = B.get_data(f1, 'shms_Angle')
beamE2 = B.get_data(f1, 'beam_e')
data_simc_Pfrac = B.get_data(f1, 'data_simc_Pfrac')
data_simc_Pfrac_err = B.get_data(f1, 'data_simc_Pfrac_err')


#Add additional headers
f1.add_key('hms_Pcorr_factor', 'f')
f1.add_key('hms_Pcorr_factor_err', 'f')


#Create Lists to store values
Ep_corr_factor_list = []
Ep_corr_factor_err_list = []


#Loop over D2 heep runs 
for i, irun in enumerate(run):

    #Calculate HMS Proton Momentum Correction
    Ep_corr_factor = 1. + data_simc_Pfrac[i]
    Ep_corr_factor_err =  data_simc_Pfrac_err[i]

    #Append to list
    Ep_corr_factor_list.append(Ep_corr_factor)
    Ep_corr_factor_err_list.append(Ep_corr_factor_err)

    #Add Values to keys
    f1.data[i]['hms_Pcorr_factor'] = round(Ep_corr_factor,6)
    f1.data[i]['hms_Pcorr_factor_err'] = round(Ep_corr_factor_err,6)


#Save D2 heep Kin File
f1.save('D2_heep_kin.txt')


#---------------
#------PLOT-----
#---------------

'''
plt.figure(1)
B.plot_exp(hmsP, Ee_factor_list, Ee_factor_err_list, color='black', label='HMS Electron Correction Factor (3-PASS, except 2.9 GeV)')
B.plot_exp(hmsP_prot, Ep_corr_factor_list, Ep_corr_factor_err_list, color='red', label='HMS Proton Correction Factor (5-PASS)')

B.pl.title('Momentum Corr. Factor vs. HMS Central Momentum')
B.pl.xlabel('HMS Central Momentum [GeV]')
B.pl.ylabel('Momentum Corr. Factor')

B.pl.legend()
plt.grid(linestyle='dotted')

B.pl.show()


plt.figure(1)
B.plot_exp(hmsP, dthe_list, dthe_err_list, color='blue', label=r'Relative Electron Angle Variation  $\delta\theta_{e}/\theta_{e}$')
B.pl.title('Electron Angle Variation vs. HMS Central Momentum')
B.pl.xlabel('HMS Central Momentum [GeV]')
#B.pl.ylabel(r'Relative Electron Angle Variation  $\delta\theta_{e}/\theta_{e}$')
B.pl.legend()
plt.grid(linestyle='dotted')
#B.pl.show()

#plt.figure(2)
B.plot_exp(hmsP, dEb_list, dEb_err_list, color='black', label=r'Relative Beam Energy Variation  $\delta E_{b}/E_{b}$')         
B.pl.title('Beam Energy Variation vs. HMS Central Momentum')   
B.pl.xlabel('HMS Central Momentum [GeV]')                                                 
#B.pl.ylabel(r'Relative Beam Energy Variation  $\delta E_{b}/E_{b}$')         
B.pl.legend()
plt.grid(linestyle='dotted') 

#B.pl.show()

#plt.figure(3)
B.plot_exp(hmsP, dEe_list, dEe_err_list, color='red', label=r'Relative Electron Momentum Variation  $\delta E_{e}/E_{e}$')
B.pl.title('Electron Momentum Variation vs. HMS Central Momentum')
B.pl.xlabel('HMS Central Momentum [GeV]')                        
#B.pl.ylabel(r'Relative Electron Momentum Variation  $\delta E_{e}/E_{e}$')           
plt.grid(linestyle='dotted') 
B.pl.legend()
B.pl.ylabel('Relative Variations')

                         
B.pl.show()
'''

'''
#Code that calculated the derivatives of the proton calculated momentum, for H(e,e'p)
#Pcalc (Eb, theta)

Mp =0.938272  #GeV
Eb = 10.6005
Pe = 8.554008  #final electron momentum [GeV]
th = 12.194 * np.pi / 180. #proton angle [rad]
dEe = -4.705e-3   # dEe = Pcorr - P  [GeV]  electron momentum variation

#dW = -Eb / Pe * dEe
#dW = 11.796e-3
dW = -5.082e-3
dW_dth = - 2. *Eb*Pe / Mp * np.sin(th/2.) * np.cos(th/2.)    

dth = dW / (dW_dth)
 
print ('dW = ', dW)
print ('dth = ', dth)

dEb = np.linspace(0.,0.01, 100) #np.linspace(0,0.005, 100)
dth = np.linspace(0, 0.0005, 100)   #rad

#proton calculated momentum
dPpcalc = 2.* Mp * (2.*Eb + Mp)* np.cos(th_p) / (Mp*Mp + 2.*Mp*Eb + np.power(Eb*np.sin(th_p),2) ) - ( 4. * Mp*Eb*(Eb+Mp)*np.cos(th_p) * (Mp + Eb*np.sin(th_p)*np.sin(th_p)) ) / ( np.power(Mp*Mp + 2.*Mp*Eb+np.power(Eb*np.sin(th_p),2),2) ) * dEb

#dP_Var = 0.3/100. *2.938*0.995*0.99959 ---> 0.0087663343287 GeV ( OR 8.766 Mev)  dP / dEb = 0.074094311
#print ('dEb = ', 0.0087663343287 / dPpcalc_dEb)

B.pl.plot(dEb, dPpcalc)
B.pl.xlabel('Beam Energy Variation, dEb [GeV]')
B.pl.ylabel('Proton Momentum Variation, dP_calc [GeV]')
B.pl.title('Variation in Momentum with Beam Energy')
B.pl.show()



#Variations in Pcalc with proton angle
dPpcalc_dth = -2.*Mp*Eb*(Eb + Mp)*np.sin(th_p) / (Mp*Mp + 2.*Mp*Eb + np.power(Eb*np.sin(th_p),2) ) - 2.*Mp*Eb*(Eb + Mp)*np.cos(th_p) / (np.power( Mp*Mp + 2.*Mp*Eb + np.power(Eb*np.sin(th_p),2),2 )) * Eb*Eb*(2.*np.sin(th_p) * np.cos(th_p)) *dth

B.pl.plot(dth, dPpcalc_dth)
B.pl.xlabel('Angle Variation Variation, dth [rad]')
B.pl.ylabel('Proton Momentum Variation, dP_calc [GeV]')
B.pl.title('Variation in Momentum with Proton Angle')
B.pl.show()
'''
