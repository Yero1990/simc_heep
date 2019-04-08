*******************************
Proton Absorption Measurements
*******************************

----------------
March 28, 2019
----------------
Proton absorption corrections to the data yield consists of estimating
what fraction of the protons that should have made it (but did NOT) to
the hodoscopes, and form a trigger was absorbed via nuclear interactions
with the various media it travels.

As an example, when the proton is knocked out of a deuteron, it has to
traverse: 
**  part of the liquid deuterium target
**  the target cell wall
**  Scattering Chamber exit windows
**  Entrance/Exit Windows of magnets
**  Various detector materials (before reaching the 4th hodoscope plane)

A small fraction is expected NOT to make it all the way though, so this
must be corrected in the data Yields.

This fraction can be determined theoretically by using the Bethe-Bloch
equations, <dE/dx> energy loss per gram/cm^2,  along with the cross sections
of interactions of each material. (See Derek Van Westrum thesis. P. 79) 

If one uses H(e,e'p) elastics, this fraction may also be determined empirically.

The procedure is as follows, for electrons in SHMS and protons in HMS:

    ---STEP 1------

    Take a coincidence ONLY run:  For each electron in the SHMS, there must have been 
    a coincident proton in the HMS to form a coincidence trigger. After making the appropiate
    PID cuts to ensure good tracking reconstruction (delta cuts), and electron in SHMS (cal. cut), and 
    proton in HMS (Emiss cut),  plot the SHMS solid angle acceptance,  
    	   
	   SHMS_Solid_Angle = P.gtr.th vs.  P.gtr.ph
	   
    This correlation should show the acceptance region in the SHMS for electrons that ONLY scattered via the elasctic
    interaction with a proton. This should give an idea of where to place solid angle acceptance cuts when looking at 
    the singles ONLY run next.


    ---STEP 2------
    
    Take an SHMS [ONLY e- singles] run (exact same kinematics, NO spectrometer change), and look at the same solid
    angle acceptance region as determined in step 1.  This time, if we only put a calorimeter cut and hms/shms delta cut 
    to ensure an electron in the acceptance. There should be ONLY the electrons that correspond to a proton in the HMS,
    as we are looking at the H(e,e'p) elastic region solid angle determined in step 1.  The Number of electrons, Ne_singles
    should be the number of entries shown after the cuts are applied.

    Now, put additional cuts to select a proton in HMS. That is, put an HMS delta cut and hms hTRIG1_tdcTime cut to select 3/4 triggers
    within the acceptance. Since we are using an HMS quantity, we should correct for the HMS tracking efficiency.
    


