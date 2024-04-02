#!/usr/bin/env python
from pwn import *
context.arch = 'amd64'

y = process( './project-1' )
context.terminal = ['tmux', 'splitw', '-h']

gdb.attach(y)

y.interactive()