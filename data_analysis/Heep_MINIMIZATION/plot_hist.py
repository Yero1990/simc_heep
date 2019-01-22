import numpy as np
import LT.box as B
import matplotlib.ticker as tk
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D


#Define Some Constants                                                                                                     
dtr = np.pi / 180.    #degrees to radians                                               
Mp = 0.938272           #proton mass
    
f = B.get_file('minChi2.txt')
part = B.get_data(f, 'particle')
chi2 = B.get_data(f, 'Minchi2')
hmsP = B.get_data(f, 'hmsP')
hmsAng = B.get_data(f, 'hmsAngle')
Eb = B.get_data(f, 'Eb')

Eb0 = 10.6005
Eb1 = 6.42765
Eb2 = 4.9309
Eb3 = 3.8335

B.plot_exp(hmsP[(part=='e') & (Eb==Eb0)], chi2[(part=='e') & (Eb==Eb0)], marker='o', color='red', label='Eb = %s' %(Eb0))
B.plot_exp(hmsP[(part=='e') & (Eb==Eb1)], chi2[(part=='e') & (Eb==Eb1)], marker='s', color='green', label='Eb = %s' %(Eb1))
B.plot_exp(hmsP[(part=='e') & (Eb==Eb2)], chi2[(part=='e') & (Eb==Eb2)], marker='^', color='blue', label='Eb = %s' %(Eb2))
B.plot_exp(hmsP[(part=='e') & (Eb==Eb3)], chi2[(part=='e') & (Eb==Eb3)], marker='D', color='magenta', label='Eb = %s' %(Eb3))

B.plot_exp(hmsP[(part=='p') & (Eb==Eb0)], chi2[(part=='p') & (Eb==Eb0)], marker='o', fillstyle='none', color='red', label='Eb = %s' %(Eb0))

B.pl.xlabel('HMS Central Momentum [GeV]')
B.pl.ylabel('Minimum Chi2 per Kinematic/Run')
B.pl.legend()
B.pl.grid(True)
B.pl.show()
