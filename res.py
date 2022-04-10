
#!/usr/bin/python
# -*- encoding: utf-8 -*-
'''
@file         : res.py
@author       : Xueyan Zhao (zhaoxueyan131@gmail.com)
@brief        : 
@version      : 0.1
@date         : 2022-04-10 19:41:23
@copyright    : Copyright (c) 2021-2022 PCNL EDA.
'''

import os
import numpy as np
import math


class baseStep:
    def __init__(self, name, num):
        self.var = [i for i in range(num)]

    def get_clause(self):
        return "1"


def ge():
    pass


def solve(n, maze_map):
    num = math.ceil(math.log2(2))


if __name__ == '__main__':
    file_name = os.getcwd() + '/bench/1.in'
    with open(file_name, 'r') as f:
        n = int(f.readline().strip())
        maze_map = f.readlines()
    solve(n, maze_map)
