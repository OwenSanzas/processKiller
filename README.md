# Time Shredder
Author: Ze Sheng (Owen Sanazas)


## Submitted Source Files
* processShredder.c
* shredder-behaviour.c
* shredder-behaviour.h
* utils.c
* utils.h

## Progress
* ✓ - Finished
* O - In progess
* ? - Have problem
* blank - Not started

Section | State
:----: |:----:
ctrl + c  | ✓
EOF  | ✓
'\n' handle | ✓
All-space input handle | ✓
Timeout parser check | ✓
Parser check | ✓
Signal handler check | ✓
mode 1: use kill()| ✓
mode 2: no kill() | ✓
SIGALRM handler check | ✓
Memory leaks check | ✓
Invalid R/W check | ✓



## Overview of work accomplished
* This time shredder can be run with a time input, i.e. ./shredder 4 means the timeout is 4 and all children processes who runs over 4 secs will be terminated.
* All commands are parsed by parser.c
* Ctrl-C, Ctrl-D and other signal are handled correctly.
