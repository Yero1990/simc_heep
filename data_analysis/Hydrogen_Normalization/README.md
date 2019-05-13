************************
 Hydrogen Normalization
************************

The Hydrogen Normalization consists of determined some offset after
the H(e,e'p) data has been corrected for detector inefficiencies, livetime,
etc.  If after these corrections, the data / simc yield ratio is still NOT ~1,
then the value required to make it ~1 is taken as the Normalization Factor.

It is very important, however, that if this factor is to be used for other 
deuteron runs in this experiment, that the correction to the data yield
consist of the same elements.  For example, if the H(e,e'p) elastics was 
corrected for  live_time,  hms_trkEff, and shms_trkEff,  the deuteron
data also be corrected for those quantities.  

Target boiling corrections for example, are different between Hydrogen and 
Deuteron, therefore, they must be treated as separate corrections. NOT include
them in the normalization factor.

From runs 3288 and 3371, the ratios were relatively the same, so these
are averaged out to determine the overall normalization factor.
The other runs still have to be studies for tracking efficiencies, as their
ratio drops with rate.



--->Below this line needs to be updated from the plots in this directory.
No Proton absorption is taken into account, so it must be corrected for 
separately.
--------------------
Taking The Average:

Run  R         R_err
3288 0.  0.005855 
3371 0.  0.008630 


--------------------------------------------------------------------
Nfac = R_avg = (0.936603 +  0.936120) / 2. = 0.936361
Nfac_err = R_avg_err = sqrt(0.005855*0.005855 + 0.008630*0.008630) = 0.010429
--------------------------------------------------------------------

The Deuteron Data Yield Can now be Normalized to the Hydrogen Elastics

Y_corr = N_counts / (Q*LT*hTrkEff*eTrkEff*Nfac)
