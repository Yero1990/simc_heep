********************************
SHMS/HMS Momentum Calibrations
********************************

------------------
February 27, 2019
------------------

Brief: The SHMS moemntum was kept constant throughout the Experiment, so
it has ONLY one momentum correction factor. 

The HMS momentum varies for the Deuteron kinematics, therefore a calibration
between the momentum correction factor and uncorrected momentum must be done.
It should be a simple line fit of : 

y ( x ) = a * x
Corr_Factor (P_uncorr) = a * P_uncorr

If the plot follows a simple line, the slope can be used to determine the
correction factor for other momenta.