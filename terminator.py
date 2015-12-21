#!/usr/bin/env python

import sys
import subprocess
import time

prog = sys.argv[0]
if len(sys.argv)==1:
    for i in range(200):
        subprocess.Popen([prog, str(i)])
else:
    time.sleep(1)
    range(0, 20000000)
    time.sleep(60)
