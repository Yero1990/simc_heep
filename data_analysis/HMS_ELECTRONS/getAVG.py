#Python script to get average of REPORT_FILE quantities

import LT.box as B
import sys
import pandas as pd    #used for data frame
import numpy as np
import re


#counter
cnt = 0

#create empty lists
bpmx_arr = [] 

#open run list
runlist = open('runlist.txt')

for irun in runlist:
    irun = irun.strip()
    #report file name
    reportFile = "/w/hallc-scifs17exp/E12-10-003/REPORT_OUTPUT/COIN/PRODUCTION/replay_coin_heep_check_%s_-1.report"% (irun)
    
    report=open(reportFile)  
    for line in report:
        data = line.split(':')  #split line into data[0]:data[1]
        if("HMS X Mispointing" in data[0]):
            if(cnt==0):
                #print(data[0], (data[1].replace(' cm','')).strip())
                bpmx = float((data[1].replace(' cm','').strip()))
                print(bpmx)
                bpmx_arr[cnt] = bpmx
                #np.append(bpmx, bpmx_arr)
            cnt = cnt+1    #count repetitions
    
    cnt=0 #reset counter each run (closes 'for irun in runlist')

print((bpmx_arr[0]))
