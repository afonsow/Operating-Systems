#!/usr/bin/env python3

from random import randint
from string import ascii_lowercase
from signal import signal, SIGPIPE
from sys import argv, exit

signal(SIGPIPE, lambda s,f: exit(0))

w = 100
chars = ascii_lowercase

if len(argv)>1:
    M = int(argv[1])
else:
    M = 16

if len(argv)>2:
    m = int(argv[2])
else:
    m = M

r = []
while True:
    if len(r)>10 and randint(0,99)<10:
        e = r[randint(0,len(r)-1)]
    else:
        e = ''
        for j in range(randint(m,M)):
            e += chars[randint(0,len(chars)-1)]
    print(e)

    if len(r) > w:
        r = r[1:]
    r.append(e)



