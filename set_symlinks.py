import os.path
import subprocess as sp
import sys

simc = sys.argv[1]


#remove existing links
cmd2 = "rm simc"
cmd3 = "rm hms"
cmd4 = "rm shms"
cmd5 = "rm run_simc.sh"
cmd6 = "rm nml_default.data"
cmd7 = "rm h2.theory"

sp.call(cmd2, shell=True)
sp.call(cmd3, shell=True)
sp.call(cmd4, shell=True)
sp.call(cmd5, shell=True)
sp.call(cmd6, shell=True)
sp.call(cmd7, shell=True)


if simc == 'deut':
#Create symlinks with deut_simc
    cmd2 = "ln -s ../deut_simc/simc"
    cmd3 = "ln -s ../deut_simc/hms"
    cmd4 = "ln -s ../deut_simc/shms"
    cmd5 = "ln -s ../deut_simc/run_simc.sh"
    cmd6 = "ln -s ../deut_simc/nml_default.data"
    cmd7 = "ln -s ../deut_simc/h2.theory"

    sp.call(cmd2, shell=True)
    sp.call(cmd3, shell=True)
    sp.call(cmd4, shell=True)
    sp.call(cmd5, shell=True)
    sp.call(cmd6, shell=True)
    sp.call(cmd7, shell=True)
    
if simc == 'gfor':    
#Create symlinks with simc_gfortran
    cmd2 = "ln -s ../simc_gfortran/simc"
    cmd3 = "ln -s ../simc_gfortran/hms"
    cmd4 = "ln -s ../simc_gfortran/shms"
    cmd5 = "ln -s ../simc_gfortran/run_simc.sh"
    cmd6 = "ln -s ../simc_gfortran/nml_default.data"
    cmd7 = "ln -s ../simc_gfortran/h2.theory"
    
    sp.call(cmd2, shell=True)
    sp.call(cmd3, shell=True)
    sp.call(cmd4, shell=True)
    sp.call(cmd5, shell=True)
    sp.call(cmd6, shell=True)
    sp.call(cmd7, shell=True)
