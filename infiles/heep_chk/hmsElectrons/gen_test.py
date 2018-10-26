import LT.box as B
import numpy as np

#read kin file
f = B.get_file('hms_heep_summary_simc.data')

radiate = 'norad';

#Get headers
kg = B.get_data(f, 'kin_group')
Run = B.get_data(f, 'Run')
hP0 = B.get_data(f, 'nmr_P')
hAng = B.get_data(f, 'hms_Angle')
sP0 = B.get_data(f, 'shms_P')
sAng = B.get_data(f, 'shms_Angle')
beam = B.get_data(f, 'beam_e')

#CONVERT TO MeV
beam = beam * 1000.  
hP0 = hP0 * 1000.
sP0 = sP0 * 1000.

#Loop over all kinmeatic groups
for i, Run in enumerate(Run):

    #Create new simc input file
    f = open('heep_hms_%s.data' % (kg[i]), 'w')

    if "coin" in kg[i]:
        spec_mode = 0
        extra_file="coin"
    if "singles_hElec" in kg[i]:
        spec_mode = -2
        extra_file="singles"

    f.write(' ! Simulation for coin. H(e,e\'p) Kinematic Group %s, Hall c   \n' % kg[i])
    f.write('! of radiative corrections \n')                                                                                   
    f.write('!               \n')                                                                                              
    f.write('!   \n')                                                                
    f.write('!  \n')                                 
    f.write('! \'TF\' stands for \'this field\    \n')                                                                         
    f.write('! ONE equals TRUE unless specified otherwise   \n')                                                               
    f.write('/   \n')                                                                                                          
    f.write(' =?    \n')                                                                                                       
    f.write(' &EXPERIMENT     \n')                                                                                             
    f.write('     ngen  =  100000			!  POS: # of successe                     \n')                             
    f.write('     EXPER%charge  =   1.0		!  total charge (mC)                      \n')                             
    f.write('     doing_phsp  =  F		!  (T = TRUE)	- If all of the doing_* are       \n')                             
    f.write('     doing_kaon  =  F		!  (T = TRUE)   false, then doing (e,e\'p).       \n')                             
    f.write('     doing_pion  =  F		!  (T = TRUE)                                     \n')                             
    f.write('     doing_decay  =  F		!  1=decay ON, 0=decay OFF.                       \n')                             
    f.write('     ctau  =  780.4			!  decay length (cm)                      \n')                             
    f.write('     extra_dbase_file = \'extra_hydrogen_%s_norad.data\' !  additional input parameters. \n' % extra_file)                          
    f.write('/        \n')                                                                                                     
    f.write(' =?      \n')                                                                                                     
    f.write(' &KINEMATICS_MAIN    \n')                                                                                         
    f.write('    Ebeam  =  %s                       !  (MeV)  \n'   % beam[i])                                                         
    f.write('    dEbeam  =  0.05                                   !  beam energy variation (%)  \n')                          
    f.write('     electron_arm  =  1                                !  1=hms,2=sos,3=hrsr,4=hrsl,5 = shms  \n')                
    f.write('     hadron_arm  =  5                                  !  1=hms,2=sos,3=hrsr,4=hrsl,5 = shms  \n')                
    f.write('     spec%%e%%P  =   %s                    !  e arm central momentum (MeV/c)  \n' % round(hP0[i],5))                            
    f.write('     spec%%e%%theta  =   %s                !  e arm angle setting (degrees)  \n' % hAng[i])                                 
    f.write('     spec%%p%%P  =   %s                !  p arm central momentum (MeV/c)   \n' % round(sP0[i],5))                               
    f.write('     spec%%p%%theta  =  %s                !  p arm angle setting (degrees)  \n' % sAng[i])                               
    f.write('/    \n')                                                                                                         
    f.write('     \n')                                                                                                         
    f.write(' ?=   \n')                                                                                                        
    f.write(' &TARGET      \n')                                                                                                
    f.write('     targ%A  =  1.0			!  target A     \n')                                                       
    f.write('     targ%Z  =  1.0			!  target Z     \n')                                                       
    f.write('     targ%mass_amu  =  1.00794	        !  target mass in amu   \n')                                               
    f.write('     targ%mrec_amu  =  0.	        !  recoil mass in amu (eep=A-1 system,pion=A-2)   \n')                     
    f.write('     targ%rho  =  0.07231		        !  target density (g/cm^3)   \n')                                  
    f.write('     ! targ%thick  =  2505.		!  target thick 15cm (mg/cm^2)       \n')                                  
    f.write('     targ%thick  =  723.1		!  target thick 10cm (mg/cm^2)       \n')                                  
    f.write('     targ%angle  =  0.		        !  target angle (for solid target) (degrees)   \n')                        
    f.write('     targ%abundancy  =  100.		!  target purity (%)       \n')                                            
    f.write('     targ%can  =  1                     !  1=beer can (fpi), 2=pudding can (nucpi)    \n')                        
    f.write('/     \n')                                                                                                        
    f.write('      \n')                                                                                                        
    f.write(' &DEBUG_PARM    \n')                                                                                              
    f.write('     debug(1)  =  F			!  turns on output from brem.f      \n')                                   
    f.write('     debug(2)  =  F			!  into/outa subs.                  \n')                                   
    f.write('     debug(3)  =  F			!  spit out values (init. and main loop).    \n')                          
    f.write('     debug(4)  =  F			!  mostly comp_ev, gen_rad diagnostics.      \n')                          
    f.write('     debug(5)  =  F			!  a bit of everything.                      \n')                          
    f.write('/      \n')                                                                                                       
    f.write('       \n')                                                                                                       
    f.write(' &E_ARM_ACCEPT           \n')                                                                                     
    f.write('     SPedge%e%delta%min  =   -15.0	!  delta min (SPECTROMETER ACCEPTANCE   \n')                               
    f.write('     SPedge%e%delta%max  =   15.0	!  delta max                            \n')                               
    f.write('     SPedge%e%yptar%min  =   -90.0	!  yptar min = {TF} / 1000 (mrad)       \n')                               
    f.write('     SPedge%e%yptar%max  =   90.0	!  yptar max = {TF} / 1000              \n')                               
    f.write('     SPedge%e%xptar%min  =   -100.0	!  xptar min = {TF} / 1000 (mrad)       \n')                               
    f.write('     SPedge%e%xptar%max  =   100.0	!  xptar max = {TF} / 1000              \n')                               
    f.write('/       \n')                                                                                                      
    f.write('        \n')                                                                                                      
    f.write(' &P_ARM_ACCEPT      \n')                                                                                          
    f.write('     SPedge%p%delta%min  =   -15.0	!  delta min (SPECTROMETER ACCEPTANC     \n')                              
    f.write('     SPedge%p%delta%max  =   25.0	!  delta max                             \n')                              
    f.write('     SPedge%p%yptar%min  =   -90.0	!  yptar min = {TF} / 1000 (mrad)        \n')                              
    f.write('     SPedge%p%yptar%max  =   90.0	!  yptar max = {TF} / 1000               \n')                              
    f.write('     SPedge%p%xptar%min  =   -100.0	!  xptar min = {TF} / 1000 (mrad)        \n')                              
    f.write('     SPedge%p%xptar%max  =   100.0	!  xptar max = {TF} / 1000               \n')                              
    f.write('/     \n')

print('EndLoop')                                                                                                        
