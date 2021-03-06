*************************************
SHMS eYptar / eDelta Offset Studies
*************************************

It was found, that after correcting for the HMS hYptar and momentum, the
SHMS calculated momentum was affected, since it uses the HMS proton momentum.
It was also found that the eDelta and eYptar appear to a little offset. 

Currently, W is centered, and DATA/SIMC match perfectly.

Mark's idea was to determine the SHMS momentum correction factor. This, of 
course, would shift W. Then one would determine by how much does the angle,
eYptar need to change in order to bring W back to its original  position.

data_Pmeas - simc_Pmeas = (8.467 - 8.455) * 1000 = 12 MeV, so the SHMS
momentum is 12 MeV larger than it should.

After putting the momentum correction offset, dW = 11.796 MeV

Now, dW = -Eb / E' * dE'
Then dth_e = dW / (dW/dth_e) -->Angle variation (eYptar)

dW = + 11.796e-3 GeV,

dth_e = + 5.77866424e-4  ;this overcorrected W, so we must iterate again

dW = 9.38277e-01 - 9.43359e-01 = -5.082e-3 GeV
dth_e = -2.4895872904e-4

After Re-Optimizing the SHMS delta, the SHMS W appears to be a little off as well as yptar.

**The plan is to firt fix yptar (should be the same for all runs), and then fix W by changing the electron momentum.

This may be due to a eYptar_offset. From the data the variations in W are
as follows:

Eb: 10.6005 GeV
Run    Wsimc      Wdata     dW            eYptar_simc[rad]   eYptar_data [rad]   dYptar       Pmeas_simc       Pmeas_data        dPmeas          
3288   0.9436     0.9398    0.003800      -1.681e-3          -1.962e-3                        8.454            8.464              
3371   0.943      0.9466   -0.003600      -7.447e-4          -1.778e-3                        7.944            7.956                    
3374   0.9429     0.9427    0.000200       3.955e-4           7.869e-5                        9.045            9.053                     
3376   0.9422     0.9436   -0.001400      -8.393e-4          -1.123e-3                        9.429            9.437                        
3377   0.9424     0.9433   -0.000900      -8.393e-4          -1.061e-3                        9.43             9.436                      

The question is, by how much would the electron angle eYptar have to change to justify the changes observed in W?

After taking an educated guess, based on eYptar DATA/SIMC comparison, the eYptar correction was determined to be (+ 1e-4 rad)
The data/simc eYptar now matches for all Heep Runs.


The new eYptar_offset in shmsflags.param is: dth_e = + 5.77866424e-4 -2.4895872904e-4 + 1e-4 = 0.000429  ( or 4.29e-4 rad)
