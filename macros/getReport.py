import numpy as np

#create lists
run = []

#BCMs (Select with Cuts)
pbcm4a_current = []
pbcm4a_charge = []

#Rates
h3of4_rate = []
p3of4_rate = []
coin_rate = []

#Spec. Mispointing  /  BPMs
hMisP_X = []
hMisP_Y = []
pMisP_X = []
pMisP_Y = []
x_bpm = []         #EPICS Coordinates (+X beam right, +Y up)
y_bpm = []         #EPICS Coordinates


#Live Times
cpuLT = []
ptrig6_accepted = []
ptrig6_scaler = []

tLT = []
edtm_accepted = []      #edtm that passed tdc cut and bcm current flag==1
edtm_scaler = []        #edtm scalers that were counted 

#Tracking Efficiencies
ptrk_eff = []
ptrk_eff_err = []
pdid_e = []
pshould_e = []

htrk_eff = []
htrk_eff_err = []
hdid_h = []             #hadrons that did pass (used for trk eff)
hshould_h = []          #hadrons that should have passed (used for trk eff.) 


rfile = open('d2580.txt')

for irun in (rfile):

    irun = int(irun)

    f = open('replay_coin_deep_check_%d_-1.report' % (irun))

    for line in f:
        #Spec. Run /Mispointings / BPMs
        if('Run #' in line):
            run.append(int(line.split(':')[1]))
        if('hHMS X Mispointing' in line):
            hMisP_X.append(float(line.split(':')[1].replace('cm', '')))
        if('hHMS Y Mispointing' in line):
            hMisP_Y.append(float(line.split(':')[1].replace('cm', '')))
        if('pSHMS X Mispointing' in line):
            pMisP_X.append(float(line.split(':')[1].replace('cm', '')))
        if('pSHMS Y Mispointing' in line):
            pMisP_Y.append(float(line.split(':')[1].replace('cm', '')))

        if('pSHMS X BPM' in line):
            x_bpm.append(float(line.split(':')[1].replace('cm', '')))
        if('pSHMS Y BPM' in line):
            y_bpm.append(float(line.split(':')[1].replace('cm', '')))
        #BCMs
        if('pSHMS BCM4A Beam Cut Current' in line):
            pbcm4a_current.append(float(line.split(':')[1].replace('uA', '')))
        if('pSHMS BCM4A Beam Cut Charge' in line):
            pbcm4a_charge.append(float(line.split(':')[1].replace('mC', '')))
        #Rates
        if('SHMS_pTRIG4_corr' in line):
            h3of4_rate.append(float(line.split(':')[1].split('[')[1].split('k')[0]))
        if('SHMS_pTRIG1_corr' in line):
            p3of4_rate.append(float(line.split(':')[1].split('[')[1].split('k')[0]))   
        if('SHMS_pTRIG6_corr' in line):
            coin_rate.append(float(line.split(':')[1].split('[')[1].split('k')[0]))

        #Live Times
        if('ROC2 Pre-Scaled Ps6 ROC2 Computer Live Time' in line):
            cpuLT.append(float(line.split(':')[1].replace('%', '')))
        if('Coin ROC2 pTRIG6 passed' in line):
            ptrig6_accepted.append(int(line.split(':')[1]))
        if('SHMS_pTRIG6' in line and 'SHMS_pTRIG6_corr' not in line):
            ptrig6_scaler.append(int(line.split(':')[1].split('[')[0]))

        if('ROC2 Pre-Scaled Ps6 Total Live Time (EDTM)' in line):
            tLT.append(float(line.split(':')[1].replace('%', '')))
        if('SHMS Accepted EDTM Triggers' in line):
            edtm_accepted.append(int(line.split(':')[1]))
        if('SHMS EDTM Triggers' in line):
            edtm_scaler.append(int(line.split(':')[1]))

        #Tracking Efficiencies
        if('pE ALL FID TRACK EFFIC' in line):
            ptrk_eff.append(float(line.split(':')[1].split('+-')[0]))
            ptrk_eff_err.append(float(line.split(':')[1].split('+-')[1]))
        if('pdide_all' in line):
            pdid_e.append(int(line.split(':')[1]))
        if('pshoulde_all' in line):
            pshould_e.append(int(line.split(':')[1]))


        if('hHADRON SING FID TRACK EFFIC' in line):
            htrk_eff.append(float(line.split(':')[1].split('+-')[0]))
            htrk_eff_err.append(float(line.split(':')[1].split('+-')[1]))
        if('hdidh' in line):
            hdid_h.append(int(line.split(':')[1]))
        if('hscinshouldh' in line):
            hshould_h.append(int(line.split(':')[1]))

#Write to a table
fTable = open('D2_KinTable.txt', 'w')
fTable.write('#!Run[i,0]/   BCM4A_Current[f,1]/    BCM4A_Charge[f,2]/    h3of4_rate[f,3]/    p3of4_rate[f,4]/     coin_rate[f,5]/    htrk_eff[f,6]/   htrk_eff_err[f,7]/  ptrk_eff[f,8]/   ptrk_eff_err[f,9]/    cpuLT[f,10]/    tLT[f,11]/    x_bpm[f,12]/    y_bpm[f,13]/   hMisP_X[f,14]/    hMisP_Y[f,15]/    pMisP_X[f,16]/    pMisP_Y[f,17]/   ptrig6_accepted[i,18]/     ptrig6_scaler[i,19]/    edtm_accepted[i,20]/   edtm_scaler[i,21]/    pdid_e[i,22]/    pshould_e[i,23]/    hdid_h[i,24]/   hshould_h[i,25]/ \n')

output = '\n'.join('\t\t'.join(map(str,row)) for row in zip(run,pbcm4a_current,pbcm4a_charge,h3of4_rate,p3of4_rate,coin_rate,htrk_eff,htrk_eff_err,ptrk_eff,ptrk_eff_err,cpuLT,tLT,x_bpm,y_bpm,hMisP_X,hMisP_Y,pMisP_X,pMisP_Y,ptrig6_accepted,ptrig6_scaler,edtm_accepted,edtm_scaler,pdid_e,pshould_e,hdid_h,hshould_h))
fTable.write(output)

#Calculate Some Averages 
x_bpm_avg = sum(x_bpm) / float(len(x_bpm))
y_bpm_avg = sum(y_bpm) / float(len(y_bpm))

total_charge = sum(pbcm4a_charge) 

cpuLT_avg = sum(ptrig6_accepted) / float(sum(ptrig6_scaler))
tLT_avg = sum(edtm_accepted) / float(sum(edtm_scaler))

htrk_eff_avg = sum(hdid_h) / float(sum(hshould_h))
htrk_eff_avg_err = np.sqrt( float(sum(hshould_h)) -  sum(hdid_h) ) / float(sum(hshould_h)) 

ptrk_eff_avg = sum(pdid_e) / float(sum(pshould_e))
ptrk_eff_avg_err = np.sqrt( float(sum(pshould_e)) -  sum(pdid_e) ) / float(sum(pshould_e)) 

fTable.write('\n')
fTable.write('\n')

fTable.write('#-------------------------------------\n')
fTable.write('#--------- Average Summary -----------\n')
fTable.write('#-------------------------------------\n')
fTable.write('# x_bpm_avg = %f cm\n' %(x_bpm_avg) )
fTable.write('# y_bpm_avg = %f cm\n' %(y_bpm_avg) )
fTable.write('# total_Charge = %f mC\n' %(total_charge) )
fTable.write('# cpuLT_avg = %f\n' %(cpuLT_avg) )
fTable.write('# tLT_avg = %f\n' %(tLT_avg) )
fTable.write('# htrk_eff_avg = %f +/- %f \n' %(htrk_eff_avg, htrk_eff_avg_err) )
fTable.write('# ptrk_eff_avg = %f +/- %f \n' %(ptrk_eff_avg, ptrk_eff_avg_err) )



