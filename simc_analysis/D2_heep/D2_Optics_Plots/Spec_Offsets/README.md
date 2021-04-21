***************************
  SPECTROMETER OFFSETS
***************************

The spectrometer offsets have been determined roughly by looking at the
D2 elastic run 3288, but a more in depth study should be done.

The relevant varibales to keep in mind are:


-----------------------
CENTRAL Angle OFFSETS:
-----------------------

--In-Plane Angle Offset--
This offset refers to the central angle of the spectrometer as determined
from the camera might need to be offset.  

If the spectrometer is set to e-: It can be determiend by looking
at a shift in W, and assuming it is entirely due to the electron angle,
or it can be obtained from the Missing Momentum X-component (In-Plane component)
by taking the difference (in Lab Fram):  (PmissX_simc - PmissX_data)/ P_cent, where P_cent
is the spectrometer central momentum.
 
--Out-of-Plane Angle Offset--
This offset can be determined from the Missing Momentum Y-component 
(Out-of-plane component) by taking the difference (in Lab Frame):  
offset = (PmissY_simc - PmissY_data) / P_cent  [rad]

-----------------------
Relative Angle OFFSETS:
-----------------------

----X'tar / Y'tar offsets----
This varibales represent the particle angles relative to the central spectrometer ray.  So they are a 
distribution of relative angles,  with the "0", representing a particle parallel to the central ray.
X'tar = dx / dz  (where X'tar are relative out of plane angles in the spectrometer coordinat system)
Y'tar = dy / dz  (where Y'tar are relative in plane angles in the spectrometer coordinate system)
These were determined by "eye", by comparing data to simc.





