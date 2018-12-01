#!/usr/bin/python3

import numpy
import subprocess

params = [0.4666,
          0.65,
          0.13,
          0.13,
          -0.003,
          0.002,
          -10000,
          900,
          7.0,
          0.5,
          0.0001,
          0.88,
          4.0,
          20.0,
          7.0]


loop_param = 2

i=0
for param in numpy.linspace(0.05, 4, 10):
    subprocess.call(["./deregister_bot.sh", "bot_v6.1_tp"+str(i)])
    f = open("mnt/bots/bot_v6.1/params_tp"+str(i)+".txt", "w")
    for ind in range(0, len(params)):
        if ind==loop_param:            
            f.write(str(param)+"\n")
        else:
            f.write(str(params[ind])+"\n")
    f.close()

    subprocess.call(["./register_bot.sh", "mnt", "bot_v6.1_tp"+str(i), "bot_v6.1/bot_v6.1", "bot_v6.1/params_tp"+str(i)+".txt"])
    i=i+1
    
    

