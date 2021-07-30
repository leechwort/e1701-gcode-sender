#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Jul 28 23:19:38 2021

@author: artsin
"""

filename = '/home/artsin/Dev/e1701-gcode-sender/gcodes/pcb-F.Cu.nc'

import re

#with open(filename, 'r') as f:
#    data = f.readlines()
#    print(data)

laser_active = False  
    
with open(filename) as gcode:
    for line in gcode:
        line = line.strip()
        if line == 'M05' or line == 'M5':
            print("Laser off")
            laser_active = False
        if line == 'M03' or line == 'M3':
            print("Laser on")
            laser_active = True
        coord = re.findall(r'[XY].?\d+.\d+', line)
        if coord:
            print("{}: {} - {}".format(laser_active, coord[0], coord[1]))
