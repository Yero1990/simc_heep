#ifndef SET_HISTOS_H
#define SET_HISTOS_H

//Header file designed to set global histogram ranges that would apply
//to both simc/data histogram declarations. This way, one makes a single
//configuration of this file, and it will set the histogram ranges of multiple
//files simultaneosuly, if included as a header in that file.

TString hadron_arm="HMS";
TString electron_arm = "SHMS";

// define some constants
static const Double_t pi = 3.141592654;
static const Double_t dtr = pi/180.;
static const Double_t MP = 0.938272; //GeV
static const Double_t MD = 1.87561; //GeV
static const Double_t MN = 0.939566; //GeV
static const Double_t me = 0.000510998;

//Units in GeV, degrees

//=====================================================
// D E F I N E    H I S T O G R A M    B I N N I N G
//=====================================================

Double_t nbins = 100;


//----------Kinematic Quantities---------
//Emiss
Double_t Em_nbins = 100;
Double_t Em_xmin = -0.05;
Double_t Em_xmax = 0.1;

//Pmiss
Double_t Pm_nbins = nbins;
Double_t Pm_xmin = -0.1;
Double_t Pm_xmax = 0.4;

//Q2
Double_t Q2_nbins = nbins;
Double_t Q2_xmin = 0;
Double_t Q2_xmax = 10;

//omega (E-E')
Double_t om_nbins = nbins;
Double_t om_xmin = 0;
Double_t om_xmax = 6;

//W_inv
Double_t W_nbins = nbins;
Double_t W_xmin = 0.7;
Double_t W_xmax = 1.2;

//theta_elec
Double_t the_nbins = nbins;
Double_t the_xmin = 5.;
Double_t the_xmax = 55.;

//theta_prot
Double_t thp_nbins = nbins;
Double_t thp_xmin = 10.;
Double_t thp_xmax = 55.;

//-------Additional Kinematic Variables----------

//W2
Double_t W2_nbins = nbins;
Double_t W2_xmin = 0.4;
Double_t W2_xmax = 2.0;

//xBj
Double_t xbj_nbins = nbins;
Double_t xbj_xmin = 0.8;
Double_t xbj_xmax = 1.1;

//Pf
Double_t Pf_nbins = nbins;
Double_t Pf_xmin = 0.;
Double_t Pf_xmax = 5.;

//kf
Double_t kf_nbins = nbins;
Double_t kf_xmin = 0;
Double_t kf_xmax = 10;

//th_q
Double_t thq_nbins = nbins;
Double_t thq_xmin = 30.;
Double_t thq_xmax = 50.;



//------Target Reconstruction Variables----------
Double_t xtar_nbins = nbins;
Double_t xtar_xmin = -0.5;
Double_t xtar_xmax = 0.5;

Double_t ytar_nbins = nbins;
Double_t ytar_xmin = -0.5;
Double_t ytar_xmax = 0.5;

Double_t ztar_nbins = nbins;
Double_t ztar_xmin = -10.0;
Double_t ztar_xmax = 10.0;

//Hadron arm Reconstructed Quantities ( xtar, ytar, xptar, yptar, delta)
Double_t hytar_nbins = nbins;
Double_t hytar_xmin = -5.;
Double_t hytar_xmax = 5.;

Double_t hxptar_nbins = nbins;
Double_t hxptar_xmin = -0.1;
Double_t hxptar_xmax = 0.1;

Double_t hyptar_nbins = nbins;
Double_t hyptar_xmin = -0.1;
Double_t hyptar_xmax = 0.1;

Double_t hdelta_nbins = nbins;
Double_t hdelta_xmin = -15.;
Double_t hdelta_xmax = 15.;

//Hadron arm Focal Plane Quantities
//X-focal plane
Double_t hxfp_nbins = nbins;
Double_t hxfp_xmin = -50.;
Double_t hxfp_xmax = 50.;
//Y-focal plane
Double_t hyfp_nbins = nbins;
Double_t hyfp_xmin = -40.;
Double_t hyfp_xmax = 40.;
//X'-focal plane
Double_t hxpfp_nbins = nbins;
Double_t hxpfp_xmin = -0.1;
Double_t hxpfp_xmax = 0.1;
//Y'-focal plane
Double_t hypfp_nbins = nbins;
Double_t hypfp_xmin = -0.1;
Double_t hypfp_xmax = 0.1;

//Electron Arm Reconstructed Quantities ( xtar, ytar, xptar, yptar, delta)
Double_t eytar_nbins = nbins;
Double_t eytar_xmin = -4.;
Double_t eytar_xmax = 4.;

Double_t exptar_nbins = nbins;
Double_t exptar_xmin = -0.1;
Double_t exptar_xmax = 0.1;

Double_t eyptar_nbins = nbins;
Double_t eyptar_xmin = -0.1;
Double_t eyptar_xmax = 0.1;

Double_t edelta_nbins = nbins;
Double_t edelta_xmin = -10.;
Double_t edelta_xmax = 10.;

//Electron Arm Focal Plane Quantities
Double_t exfp_nbins = nbins;
Double_t eyfp_nbins = nbins;
Double_t expfp_nbins = nbins;
Double_t eypfp_nbins = nbins;

//Run: 3371





//Run 3288
//Double_t exfp_xmin = -15.;
//Double_t exfp_xmax = 10.;

//Double_t eyfp_xmin = -10.;
//Double_t eyfp_xmax = 10.;

//Double_t expfp_xmin = -0.04;
//Double_t expfp_xmax = 0.04;

//Double_t eypfp_xmin = -0.03;
//Double_t eypfp_xmax = 0.03;

//Run 3371
//Double_t exfp_xmin = -25.;
//Double_t exfp_xmax = 0.;

//Double_t eyfp_xmin = -20.;
//Double_t eyfp_xmax = 15.;

//Double_t expfp_xmin = -0.06;
//Double_t expfp_xmax = 0.02;

//Double_t eypfp_xmin = -0.04;
//Double_t eypfp_xmax = 0.03;

//Run: 3374
//Double_t exfp_xmin = 0.;
//Double_t exfp_xmax = 15.;

//Double_t eyfp_xmin = -7.;
//Double_t eyfp_xmax = 3.;

//Double_t expfp_xmin = -0.01;
//Double_t expfp_xmax = 0.04;

//Double_t eypfp_xmin = -0.02;
//Double_t eypfp_xmax = 0.02;

//Run: 3376/3377
Double_t exfp_xmin = 10.;
Double_t exfp_xmax = 25.;

Double_t eyfp_xmin = -10.;
Double_t eyfp_xmax = 5.;

Double_t expfp_xmin = 0.01;
Double_t expfp_xmax = 0.05;

Double_t eypfp_xmin = -0.01;
Double_t eypfp_xmax = 0.01;

#endif

    
