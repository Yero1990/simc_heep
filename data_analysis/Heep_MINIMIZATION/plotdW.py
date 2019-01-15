import numpy as np
import LT.box as B
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

#Define Some Constants                                                                                                     
dtr = np.pi / 180.    #degrees to radians                                                                                                                                 
Mp = 0.938272           #proton mass                                                                                                                                                
     
f = B.get_file('hms_kin.data')


kg = B.get_data(f, 'kin_group')
part = B.get_data(f, 'particle')
Eb = B.get_data(f, 'beam_e')
hmsP = B.get_data(f, 'nmr_P')
hmsAng = B.get_data(f, 'hms_Angle')
dataW = B.get_data(f, 'data_W_mean')
dataW_err = B.get_data(f, 'data_W_mean_err')
simcW = B.get_data(f, 'simc_W_mean')
simcW_err = B.get_data(f, 'simc_W_mean_err')

dW_meas = simcW - dataW

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

#Eb0 = 10.6005
#Eb0 = 6.42765
#Eb0 = 3.83350
Eb0 = 4.93090

th = hmsAng * dtr 

#fit parameters
p1 = 0.0008      #dEb/Eb
p2 = -0.0015     #dP/P
p3 = -0.0005     #dth

dEb = p1 * Eb0
dP = p2 * hmsP[Eb==Eb0]
dth = p3

#Derivative dW/dEb|_{Eb==Eb0}                                                                                                        
dW_dEb = hmsP[Eb==Eb0]/Eb0                                                                                                                                                 

#Derivative dW/dE',                                                                                                                                                       
dW_dP = -Eb[Eb==Eb0]/hmsP[Eb==Eb0]                                                                                                                                                 

#Derivative dW/dhmsAngle                                                                                                                                                  
dW_dth = -2 * Eb[Eb==Eb0] * hmsP[Eb==Eb0] * np.sin(th[Eb==Eb0]/2) * np.cos(th[Eb==Eb0]/2) / Mp                                                                                                       

#Predicted Variations in W (Total Derivative)                                                                                                                             
dW_pred =  dW_dEb * dEb  +  dW_dP * dP +  dW_dth * dth        
#fit function


#ax.scatter(hmsAng, hmsP, dW_meas, c='red', marker='o')
ax.scatter(hmsAng[Eb==Eb0], hmsP[Eb==Eb0], dW_meas[Eb==Eb0], c='blue', marker='o', label='Eb = 4.93090 GeV') 
plt.legend()
#plot fit function
ax.plot3D(hmsAng[Eb==Eb0], hmsP[Eb==Eb0], dW_pred, 'red')

ax.set_xlabel('HMS Angle[deg]')
ax.set_ylabel('HMS Momentum [GeV]')                                                                                                                                 
ax.set_zlabel('Measured dW [GeV]')                                                                                                                                       


#ax.scatter(hmsAng[Eb==Eb1], hmsP[Eb==Eb1], dW_meas[Eb==Eb1], c='red', marker='^')                                                           
plt.show()
#print(dW[Eb>7])
#B.plot_exp(Eb[part=='p'], dW[part=='p'],0, color='black')
#B.plot_exp(Eb[part=='e'], dW[part=='e'],0, color='red')
#B.pl.show("same")



