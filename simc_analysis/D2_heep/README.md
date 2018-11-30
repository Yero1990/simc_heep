=======================
SHMS Electrons Studies
=======================

**This directory contains simulations for H(e,e'p) elastics taken 
  during E12-10-003 experiment. 

**The main goal is to study the shifts in W observed between data and SIMC
  and determine the correct momentum correction factor to get data W to
  align with SIMC.

**It is assumed for now, that any shift in W is caused only by the spectrometer
  momentum setting. Since the angle and beam energy were obtained from camera 
  and beam energy studies respectively.

-----------------
---Directories---
-----------------
checkHMS : Contains code to check the HMS momentum by verifying there
are no correlations in (Pcalc - Pmeas) / Pmeas vs. HMS Focal Plane Quantities.

checkSHMS : Contains code to check SHMS momentum, assuming the HMS momentum 
is well understood. The correlations between (delta_calc - delta_meas) vs.
SHMS Focal Plane Quantities are investigated.

pCorr/pUnCorr : Containes SIMC root files before and after correcting for
the SHMS central momentum. (After the SHMS delta has been optimized)