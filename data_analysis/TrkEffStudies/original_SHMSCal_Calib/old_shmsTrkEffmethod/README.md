The old tracking efficiency method consists of basically counting all
tracks, whether single or multiple tracks with the same weight.

This data was analyzed using the SHMS Calorimeter calibration for each run separaterly, using the
elastic data from D2, 3288, 3371, 3374, 3377

#General Tracking Efficiency
shmsScinShould      shmsScinGood && shmsGoodBetanotrk 
shmsScinDid         shmsScinShould && P.dc.ntrack > 0

#Single Track Efficiency
shmsScinShoulde     shmsScinShould &&  P.cal.etotnorm >= 0.5 && P.cal.etotnorm < 1.5 && P.ngcer.npeSum > 0.5 
shmsScinDide        shmsScinShoulde && P.dc.ntrack > 0 


#Multiple Tracking Efficiency (DETERMINED from D2 Elastic run 3377, but will have to check etotnorm limits of other runs)
shmsScinShoulde_mult     shmsScinShould &&  P.cal.etotnorm >= 1.5  && P.ngcer.npeSum > 0.5 
shmsScinDide_mult        shmsScinShoulde_mult && P.dc.ntrack > 0 

#ALL TRACKS
shmsScinShoulde_all     shmsScinShould &&  P.cal.etotnorm >= 0.5  && P.ngcer.npeSum > 0.5 
shmsScinDide_all        shmsScinShoulde_all && P.dc.ntrack > 0 

