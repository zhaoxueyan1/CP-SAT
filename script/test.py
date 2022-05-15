#!/usr/bin/python3
# -*- encoding: utf-8 -*-
'''
@file         : test.py
@author       : Xueyan Zhao (zhaoxueyan131@gmail.com)
@brief        : 
@version      : 0.1
@date         : 2022-05-14 21:59:05
@copyright    : Copyright (c) 2021-2022 PCNL EDA.
'''

import os

if __name__ == '__main__':
    path = os.getcwd() + '/bench/benchmark/'
    output_path = ''
    bin_command = os.getcwd() + '/build/microsat'
    for root, dirs, files in os.walk(path, topdown=False):
        for name in files:
            # os.system(bin_command + ' ' + root + name + ' > ' +
            #           os.getcwd() + '/bench/output/' + name + '.out')
            print(bin_command + ' ' + root + name + ' > ' +
                  os.getcwd() + '/bench/output/' + name + '.out')
            print(os.path.join(root, name))
        for name in dirs:
            print(os.path.join(root, name))
