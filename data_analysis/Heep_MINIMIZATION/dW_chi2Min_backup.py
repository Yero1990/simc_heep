#This code uses Chi2 Minimization procedure to simultaneously determine the
#parameters p1 = dEb / Eb, p2 = dP /P, and p3 = d_th  which represent the
#relative beam energy, spectrometer momentum and angle. For now, we focus 
#on HMS. HMS elastics electrons and protons data are used. For electrons,
#the invariant mass W is used to determine chi2. For protons, the difference
#between calculated and measured momentum is used to determine chi2. The
#2 data sets (protons and electrons) are combined so there can be more data
#points to fit, and get a better chi2.


import LT.box as B
import numpy as np
import matplotlib.pyplot as plt

#define function to loop over floats
#-------------------------------------
def frange(start, stop, step):
    i = start
    while i < stop:
        yield i
        i += step
#-------------------------------------


#Define Some Constants
dtr = np.pi / 180.    #degrees to radians
Mp = 0.938272           #proton mass


#---------------------------------------
# ANALYZE HMS ELECTRON MOMENTUM, W peak
#---------------------------------------

#Read in data file with the kinematics and fit information
f = B.get_file('hms_kin.data')
kg = B.get_data(f, 'kin_group')                  #kin. group
run = B.get_data(f, 'Run')
hNMR = B.get_data(f, 'nmr_true')                 #HMS Dipole NMR [T]
hmsP = B.get_data(f, 'nmr_P')                    #HMS Momentum [GeV]
hmsAng = B.get_data(f, 'hms_Angle')              #HMS Angle [deg]
shmsP = B.get_data(f, 'shms_P')
shmsAng = B.get_data(f, 'shms_Angle')
beamE = B.get_data(f, 'beam_e')
dataW = B.get_data(f, 'data_W_mean')
dataW_err = B.get_data(f, 'data_W_mean_err')
simcW = B.get_data(f, 'simc_W_mean')
simcW_err = B.get_data(f, 'simc_W_mean_err')
dataPfr = B.get_data(f, 'data_Pfr_mean')          #Data fractional momentum (Pcacl - Pmeas) / Pmeas mean of fit
dataPfr_err = B.get_data(f, 'data_Pfr_mean_err')
simcPfr = B.get_data(f, 'simc_Pfr_mean')          
simcPfr_err = B.get_data(f, 'simc_Pfr_mean_err')

#------Create Lists to store values-------

#parameters
p1_list = []
p2_list = []
p3_list = []

#chi2 / reduced chi2
dW_chi2_list = []
dW_red_chi2_list = []

dPfr_chi2_list = []
dPfr_red_chi2_list = []

total_chi2_list = []
total_red_chi2_list = []



#Step size of Relative Variables, p1 = dEb /Eb, p2 = dP /P, p3 = d_th
step_p1 = 0.0002     #(steps of 0.02%, for example) 
step_p2 = 0.0005 
step_p3 = 0.0005

#Parameters Range  
p1_min = -0.001
p1_max =  0.001

p2_min = -0.004
p2_max =  0.004

p3_min =  -0.002
p3_max =  0.002


nsteps_p1 = (p1_max - p1_min) / step_p1 + 1   # +1 is to include the first step (otherwise, it will count after the first step)
nsteps_p2 = (p2_max - p2_min) / step_p2 + 1
nsteps_p3 = (p3_max - p3_min) / step_p3 + 1


print('total p1: ',nsteps_p1)
print('total p2: ',nsteps_p2)
print('total p3: ',nsteps_p3)

#Output file
fout = open('chi2_fit_results.txt', 'w')
fout2 = open('sorted_chi2.txt', 'w')

fout.write('#! kg[f,0]/  Run[f,1]/  dW_meas[f,2]/   dW_meas_err[f,3]/   dW_dEb[f,4]/    dW_dP[f,5]/     dW_dth[f,6]/     dW_pred[f,7]/     dW_diff[f,8]/   dW_chi2_i[f,9]/    dW_chi2_sum[f,10]/   dPfr_meas[f,11]/   dPfr_meas_err[f,12]/   dPfr_dEb[f,13]/    dPfr_dP[f,14]/     dPfr_dth[f,15]/     dPfr_pred[f,16]/     dPfr_diff[f,17]/   dPfr_chi2_i[f,18]/    dPfr_chi2_sum[f,19]/    total_chi2_i[f,20]/   total_chi2_sum[f, 21]/   dEb_Eb[f,22]/      dP_P[f,23]/     dth_th[f,24]/ \n')
fout2.write('#! dEb_Eb[f,0]/  dP_P[f,1]/   dth[f,2]/    total_chi2[f,3]/    total_redChi2[f,4]/ \n')

#Chi2 Counter
dW_chi2 = 0
dPfr_chi2 = 0
total_chi2 = 0


#Number of Observations Counter
N_obs = len(kg)     #number of data points
N_total = nsteps_p1 * nsteps_p2 * nsteps_p3  #total number of possible parameter configurations
print ('N_total = ', N_total)

#Number of Parameters
N_parm = 3

for m in frange(p1_min, p1_max+step_p1, step_p1):
    for n in frange(p2_min, p2_max+step_p2, step_p2):
        for l in frange(p3_min, p3_max+step_p3, step_p3):
            
            #Reset Chi2 Counter per Configuration
            dW_chi2 = 0
            dPfr_chi2 = 0
            total_chi2 = 0
            
            #Loop over kinematic group 
            for i, ikg in enumerate(kg):
    
                th = hmsAng[i] * dtr     #convert HMS angle to radians

                p1 = m    #relative uncertainty in Beam Energy (%/100)
                p2 = n    #relative uncertainty in HMS momentum (either proton or electron, %/100)
                p3 = l    #relative uncertainty in HMS angle (radians)


                #Parameters to be determined (by CHi2 Minimization)
                dEb = p1 * beamE[i]
                dP = p2 * hmsP[i]
                dth = p3


                #------------HMS ELECTRONS-------------

                #Measured Variations in W
                dW_meas = simcW[i] -  dataW[i]
                dW_err = np.sqrt(simcW_err[i]**2 + dataW_err[i]**2)
                
                #Derivative dW/dEb, 
                dW_dEb = hmsP[i]/beamE[i]
                
                #Derivative dW/dE', 
                dW_dP = -beamE[i]/hmsP[i]
                
                #Derivative dW/dhmsAngle
                dW_dth = -2 * beamE[i] * hmsP[i] * np.sin(th/2) * np.cos(th/2) / Mp
                
                #Predicted Variations in W (Total Derivative)
                dW_pred =  dW_dEb * dEb  +  dW_dP * dP +  dW_dth * dth
                
                dW_diff = dW_meas - dW_pred

                #Quantity to be minimized, chi2_i  (ith run or kin. group)
                dW_chi2_i = (dW_diff / dW_err)**2

                dW_chi2 = dW_chi2 + dW_chi2_i

                
                #---------------HMS PROTONS-----------------
                
                #Measured Variations in fractional momentum
                dPfr_meas = simcPfr[i] - dataPfr[i]
                dPfr_err = np.sqrt(simcPfr_err[i]**2 + dataPfr_err[i]**2)
                
                # Formula for fractional momentum:
                # ---> Pfr = (Pcalc (Eb, th) - Pmeas) / Pmeas  :: measured proton momentum is taken as spectrometer central momentum
                # ---> Pcalc =  2*Mp*Eb*(Eb+Mp)*cos(theta_p) / ( Mp^2 + 2*Mp*Eb + Eb^2*sin^2(theta_p) ) :: calculated proton momentum 

                Pcalc =  2*Mp*beamE[i]*(beamE[i]+Mp)*np.cos(th) / ( Mp**2 + 2*Mp*beamE[i] + pow(beamE[i]*np.sin(th),2) )

                #Take derivatives with respect to calculated momentum
                
                #Derivative dPcalc / dEb,  with respect to beam energy 
                dPcalc_dEb = 2.* Mp * (2.*beamE[i] + Mp)* np.cos(th) / (Mp*Mp + 2.*Mp*beamE[i] + np.power(beamE[i]*np.sin(th),2) ) - ( 4. * Mp*beamE[i]*(beamE[i]+Mp)*np.cos(th) * (Mp + beamE[i]*np.sin(th)*np.sin(th)) ) / ( np.power(Mp*Mp + 2.*Mp*beamE[i]+np.power(beamE[i]*np.sin(th),2),2) )
            
                #Derivative dPcalc / dth, with respect to angle
                dPcalc_dth = -2.*Mp*beamE[i]*(beamE[i] + Mp)*np.sin(th) / (Mp*Mp + 2.*Mp*beamE[i] + np.power(beamE[i]*np.sin(th),2) ) - 2.*Mp*beamE[i]*(beamE[i] + Mp)*np.cos(th) / (np.power( Mp*Mp + 2.*Mp*beamE[i] + np.power(beamE[i]*np.sin(th),2),2 )) * beamE[i]*beamE[i]*(2.*np.sin(th) * np.cos(th)) 

                #Take derivatives wih respect to fractional momentum

                #Derivative dPfr/dPmeas 
                dPfr_dP = - Pcalc / pow(hmsP[i],2)
                
                #Derivative dPfr/dEb
                dPfr_dEb = dPcalc_dEb / hmsP[i]

                #Derivative dPfr/dth
                dPfr_dth = dPcalc_dth / hmsP[i]

                #Predicted Variations in Pfr
                dPfr_pred =  dPfr_dEb * dEb  +  dPfr_dP * dP +  dPfr_dth * dth
                
                dPfr_diff = dPfr_meas - dPfr_pred

                #Quantity to be minimized, chi2_i  (ith run or kin. group)
                dPfr_chi2_i = (dPfr_diff / dPfr_err)**2

                dPfr_chi2 = dPfr_chi2 + dPfr_chi2_i

                #-------------------------------------
                # Combine ELECTRONS AND PROTONS CHI2
                #-------------------------------------
                
                total_chi2_i = dW_chi2_i + dPfr_chi2_i 

                total_chi2 = total_chi2 + total_chi2_i

                #Write all possible configurations of p1,p2,p3 to a txt file
                fout.write('%s     %s     %s     %s     %s     %s     %s     %s     %s     %s     %s     %s     %s    %s     %s     %s     %s     %s     %s     %s     %s     %s     %s     %s\n' % (int(kg[i]), int(run[i]), round(dW_meas, 6), round(dW_err, 6), round(dW_dEb,6), round(dW_dP,6), round(dW_dth,6), round(dW_pred, 6), round((dW_diff),6), round(dW_chi2_i ,6), round(dW_chi2 ,6), round(dPfr_meas, 6), round(dPfr_err, 6), round(dPfr_dEb, 6), round(dPfr_dP, 6), round(dPfr_dth, 6), round(dPfr_pred, 6), round(dPfr_diff, 6), round(dPfr_chi2_i, 6), round(dPfr_chi2, 6), round(total_chi2_i, 6),  round(total_chi2, 6), round(p1,6), round(p2,6), round(p3,6) ))


            total_red_chi2 = total_chi2 /(N_obs - N_parm)
            
            #Append to lists
            p1_list.append(p1)
            p2_list.append(p2)
            p3_list.append(p3)
            total_chi2_list.append(total_chi2)
            total_red_chi2_list.append(total_red_chi2)



          

                

            #dW_chi2_list.append(dW_chi2)
            #dW_red_chi2_list.append(dW_red_chi2)



#Sort general lists (a, b, c, ...) according to a, where a = dW_chi2_list to sort from smallest to largest chi2
s = sorted(zip(total_chi2_list,total_red_chi2_list,p1_list,p2_list,p3_list))
total_chi2_list,total_red_chi2_list,p1_list,p2_list,p3_list = map(list, zip(*s))


#Write Lists to text file, enumerate in chi2 
for item, chi2 in enumerate(total_chi2_list):
    #print('item = ',item, ' chi2 = ', chi2)
    #print('redChi2 = ',dW_red_chi2_list[item])
    fout2.write('%s    %s    %s    %s    %s\n' % (round(p1_list[item],4), round(p2_list[item],4), round(p3_list[item],4), round(chi2,4), round(total_red_chi2_list[item],4)))

fout.close()
fout2.close()



#--------IMPORTANT TO ALIGN COLUMNS.  STILL NEEDS TO BE LOOKED AT . . . ------------------------
#Open txt files to align columns
#with open('sorted_chi2.txt', 'r+') as fout:
#    for line in fout:
#        data = line.split()
       # print '{0[0]:<5}{0[1]:<10}{0[2]:<10}{0[3]:<10}{0[4]:<10}'.format(data)   #aligns all five columsn [0], [1], ...,  the '<5' ,means left-align spacing between the columns
#        letter = '{0[0]:<5}{0[1]:<10}{0[2]:<10}{0[3]:<10}{0[4]:<10}'.format(data) 
#        print ('%s\n' % (letter))
#    fout.write(letter)
        #line = '{0[0]:<5}{0[1]:<10}{0[2]:<10}{0[3]:<10}{0[4]:<10}'.format(data)
        #fout.write('%s\n' % '{0[0]:<5}{0[1]:<10}{0[2]:<10}{0[3]:<10}{0[4]:<10}'.format(data))
#-------------------------------------------------------------------------------------------------

'''
plt.figure(1)
B.plot_exp(p1_list, dW_chi2_list, 0, color='blue', label='p1')

plt.figure(2)
B.plot_exp(p2_list, dW_chi2_list, 0, color='red', label='p2')

plt.figure(3)
B.plot_exp(p3_list, dW_chi2_list, 0, color='black', label='p3')
B.pl.show()
'''
