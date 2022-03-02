#!/usr/bin/env python3
from screenshot import screenshot
from os import system

img = screenshot.get( x = 633, y = 5, width = 106, height = 109, save = True )
system( 'clear' )
print( img ) 
