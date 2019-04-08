#Code that calculated the derivatives of the proton calculated momentum, for H(e,e'p)
#Pcalc (Eb, theta)

import LT.box as B
import numpy as np

Mp =0.938272  #GeV
Eb = 10.6005
th_p = 37.338 * np.pi / 180. #proton angle

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
