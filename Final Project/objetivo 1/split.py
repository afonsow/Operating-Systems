#!/usr/bin/env python3

from random import randint
from sys import stdin, argv

o = [ open(n,'w') for n in argv[1:] ]

for i in stdin.readlines():
    o[randint(0,len(o)-1)].write(i)
