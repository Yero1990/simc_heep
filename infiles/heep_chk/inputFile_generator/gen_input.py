#Python script to generate simc input file, given the kinematics

f = open('example_input.data', 'w')

f.write(' ! Simulation for coin. H(e,e\'p) runs 1854, January 2018, Hall c   \n'
'! of radiative corrections \n'
'!               \n'
'! kinematics from mceep input q3_d20_mp.inp   \n'
'! 5008.550,0.,0.,2750.659,29.235050,-0.060390,3031.415,-30.590997,0.082506   \n'
'!   \n'
'! \'TF\' stands for \'this field\    \n'
'! ONE equals TRUE unless specified otherwise   \n'
'/   \n'
' =?    \n'
' &EXPERIMENT     \n'
'     ngen  =  100000			!  POS: # of successe                     \n'
'     EXPER%charge  =   1.0		!  total charge (mC)                      \n'
'     doing_phsp  =  F		!  (T = TRUE)	- If all of the doing_* are       \n'
'     doing_kaon  =  F		!  (T = TRUE)   false, then doing (e,e\'p).       \n'
'     doing_pion  =  F		!  (T = TRUE)                                     \n'
'     doing_decay  =  F		!  1=decay ON, 0=decay OFF.                       \n'
'     ctau  =  780.4			!  decay length (cm)                      \n'
'     extra_dbase_file = \'extra_hydrogen_rad.data\' !  additional input parameters. \n'
'/        \n'
' =?      \n'
' &KINEMATICS_MAIN    \n'
'    Ebeam  =  6405.0                       !  (MeV)  \n'
'    dEbeam  =  0.05                                   !  beam energy variation (%)  \n'
'     electron_arm  =  5                                !  1=hms,2=sos,3=hrsr,4=hrsl,5 = shms  \n'
'     hadron_arm  =  1                                  !  1=hms,2=sos,3=hrsr,4=hrsl,5 = shms  \n'
'     spec%e%P  =   3554.865                    !  e arm central momentum (MeV/c)  \n'
'     spec%e%theta  =   27.6                !  e arm angle setting (degrees)  \n'
'     spec%p%P  =     3609.0                !  p arm central momentum (MeV/c)   \n'
'     spec%p%theta  =     27.5                !  p arm angle setting (degrees)  \n'
'/    \n'
'     \n'
' ?=   \n'
' &TARGET      \n'
'     targ%A  =  1.0			!  target A     \n'
'     targ%Z  =  1.0			!  target Z     \n'
'     targ%mass_amu  =  1.00794	        !  target mass in amu   \n'
'     targ%mrec_amu  =  0.	        !  recoil mass in amu (eep=A-1 system,pion=A-2)   \n'
'     targ%rho  =  0.07231		        !  target density (g/cm^3)   \n'
'     ! targ%thick  =  2505.		!  target thick 15cm (mg/cm^2)       \n'
'     targ%thick  =  723.1		!  target thick 10cm (mg/cm^2)       \n'
'     targ%angle  =  0.		        !  target angle (for solid target) (degrees)   \n'
'     targ%abundancy  =  100.		!  target purity (%)       \n'
'     targ%can  =  1                     !  1=beer can (fpi), 2=pudding can (nucpi)    \n'
'/     \n'
'      \n'
' &DEBUG_PARM    \n'
'     debug(1)  =  F			!  turns on output from brem.f      \n'
'     debug(2)  =  F			!  into/outa subs.                  \n'
'     debug(3)  =  F			!  spit out values (init. and main loop).    \n'
'     debug(4)  =  F			!  mostly comp_ev, gen_rad diagnostics.      \n'
'     debug(5)  =  F			!  a bit of everything.                      \n'
'/      \n'
'       \n'
' &E_ARM_ACCEPT           \n'
'     SPedge%e%delta%min  =   -15.0	!  delta min (SPECTROMETER ACCEPTANCE   \n'
'     SPedge%e%delta%max  =   25.0	!  delta max                            \n'
'     SPedge%e%yptar%min  =   -90.0	!  yptar min = {TF} / 1000 (mrad)       \n'
'     SPedge%e%yptar%max  =   90.0	!  yptar max = {TF} / 1000              \n'
'     SPedge%e%xptar%min  =   -100.0	!  xptar min = {TF} / 1000 (mrad)       \n'
'     SPedge%e%xptar%max  =   100.0	!  xptar max = {TF} / 1000              \n'
'/       \n'
'        \n'
' &P_ARM_ACCEPT      \n'
'     SPedge%p%delta%min  =   -15.0	!  delta min (SPECTROMETER ACCEPTANC     \n'
'     SPedge%p%delta%max  =   15.0	!  delta max                             \n'
'     SPedge%p%yptar%min  =   -90.0	!  yptar min = {TF} / 1000 (mrad)        \n'
'     SPedge%p%yptar%max  =   90.0	!  yptar max = {TF} / 1000               \n'
'     SPedge%p%xptar%min  =   -100.0	!  xptar min = {TF} / 1000 (mrad)        \n'
'     SPedge%p%xptar%max  =   100.0	!  xptar max = {TF} / 1000               \n'
'/     \n'















)
