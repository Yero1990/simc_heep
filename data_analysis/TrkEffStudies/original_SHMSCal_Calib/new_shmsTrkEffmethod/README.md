The new tracking efficiency method consists weighting single and 
multiple tracks, based on the DC rate and gate width.

This data was analyzed using the SHMS Calorimeter calibration for each run separaterly, using the
elastic data from D2, 3288, 3371, 3374, 3377

Brief: Consider electron tracking efficiencies as a single tracking efficiency and multi-tracking efficiency components.
       Using the following formula: 

       e_trk = P1 * e1_trk  + P2 * e2_trk,   where Pi = Probability for a single or multiple particles passing through the Drift Chambers.
       	       	    	      	   	     and   ei_trk = tracking efficiency for a single or multiple tracks

       To estimate the probability of finding multiple tracks, assume the drift chambers have a similar rate to the 1st hodoscopoe plane in front of them, say S1X.
       Using this approximation, then:
       	     
	     P2 = 1 -  e^{-Rt} ~ Rt,  where R ~ S1X Rate, and t = DC Gate Width (found by taking difference between tdc_min_win and tdc_max_win from pdc_cuts.param file)

	     P1 + P2 = 1 ---> P1 = 1 - P2

      The general expression for electron tracking efficiency can then be written as:
      	    
	     e_trk = (1 - Rt)*e1_trk + Rt*e2_trk 


      To determine the single/multiple tracking efficiencies, the CUTS file has been modified to include the different ranges of the total normalized calorimeter energy.
      If one plot this variable, there will typically be two bumps.  The first one, a few order larger than the second. These represent the single and multiple tracks 
      detected by the calorimeter. As multiple tracks will deposit more energy than single tracks.

See Tanja Horn's thesis section on tracking efficiencies. 