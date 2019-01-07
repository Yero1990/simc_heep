***********************
SHMS Emiss COrrection
***********************

December 05, 2018:

After the HMS yPtar and delta were well established, Emiss offset, as well
as W (from SHMS).

From the formulas: Emiss(Pb, E', Ep),  W(Eb, E', theta_e)

Since proton momentum Pp from HMS is well established, any 
offset in Emiss must be due to electron momentum E', assuming Eb is well known.

It is best to correct Emiss, by taking the derivative:
dEmiss / dE' = -1 ----> dEmiss = -dE',   dEmiss =  Emiss_simc - Emiss_data

From the Fit of Emiss:
Run     Emiss_simc    Emiss_data    dE_miss       E'              (1-dE'/E')
3288    0.004441      -0.02454	    0.028981	  8.554008	  0.996612
3371    0.00399       -0.03287      0.036860	  8.562092	  0.995695	
3374    0.004527      -0.02656      0.031087	  8.559258	  0.996368
3376    0.004429      -0.02703	    0.031459	  8.560713	  0.996325
3377    0.004516      -0.02692	    0.031436	  8.560338	  0.996328


Then the electron corrected momentum is:  E'corr =  E' + dE' = E' (1 + dE'/E')
Since dEmiss = -dE'  ---> E'corr = E' - dE' = E'(1 - dE'/E')


Now, W should be aligned as Well. If W from DATA/SIMC are not aligned, then
that means there must be an electron angle offset that is needed.
