import sys
import numpy
import os

mins = [0.1,
        0.1,
        0.02,
        0.02,
        -0.05,
        0.0,
        -100000,
        500,
        1.0,
        0.05,
        0.0,
        0.2,
        0.5,
        10.0,
        2.0,
        2.0]

maxs = [ 1.0,
         1.0,
         1.0,
         1.0,
         0.0,
         0.05,
         -1000,
         1000,
         50.0,
         0.95,
         0.1,
         5.0,
         10.0,
         40.0,
         20.0,
         30.0];

if (len(sys.argv) == 2): 
    pars = numpy.loadtxt(sys.argv[1])
    for i in range(len(pars)):
        print(mins[i]+(maxs[i]-mins[i])*(1.0+pars[i])/2.0)
else:
    all_pars = os.listdir(sys.argv[1])
    for par in all_pars:
        pars = numpy.loadtxt(sys.argv[1]+par)
        for i in range(len(pars)):
            print("%10.5f   "%(mins[i]+(maxs[i]-mins[i])*(1.0+pars[i])/2.0), end="") 
        print()


    
