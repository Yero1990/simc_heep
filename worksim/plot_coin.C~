{

  //CHARGE factor
    //Charge factor is the total integrated charge assuming a beam current and run time
  //Double_t Ib = 65;       //beam current in microamps (micro-Coulombs / sec),   1 mC = 1000 uC
   Double_t run_time;     //estimated time (in hours) a run takes (start - end) of run
   // Double_t charge_factor = Ib * time * 3600. / 1000.;

   //Coin. Run 1866
   Double_t Q_bcm1 = 27323.84;  //uC
   Double_t Q_bcm2 = 27749.918;
   Double_t Q_avg = (Q_bcm1 + Q_bcm2) / 2.;
   
   Double_t charge_factor = Q_avg / 1000.;   //in mC
  
   cout << "Total Charge: " << Q_avg << endl;
   cout << "Charge Factor: " << charge_factor << endl;


   //-----------------------------
   //fullweight = Normfac*Weight*charge_factor/generated_events

   /*
   
     In Simulation:
     This is the full weight that has to be applied on an event by event basis.
     So we can correctly account for the actual number of events 
    
     Compare the simulation (Q2, W, ...) with data and any cuts that are applied to data, must also be applied to simulation.
     
     e.g. SIMC    SNT->Draw("e_delta>>(60, -20,10)", "Weight*Normfac*27.5369/100000*(W>0.8&&W<1.2)")
     e.g. Data    T->Draw("P.gtr.dp>>(60, -20, 10)", "P.kin.primary.W>0.8&&P.kin.primary.W<1.2")
   */

   

   //  SNT->Draw("e_delta:e_yptar>>(60, -0.04,0.04, 100, -20, 10)", "", "colz");

   

  
}
