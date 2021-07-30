#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Jul 29 00:16:14 2021

@author: artsin
"""

from ctypes import *

e1701_library = '/lib64/libe1701.so.0.0.0'
e1701_funcs = CDLL(e1701_library)