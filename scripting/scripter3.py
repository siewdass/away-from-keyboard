#!/usr/bin/env python3
from pynput.keyboard import Listener, KeyCode, Key
from pynput.mouse import Button as Click, Controller as Mouse
from time import time, sleep
import screenshot
from fix import fix
from screenshot import screenshot
import json
from sys import exit
import threading
from os import system, getcwd

config = { }
x = 17
y = 4
z = 0
map = [ ]
pressed = ""
script = 'tmp.json'

mouse = Mouse( )

elapsed = 0

keys = [ KeyCode.from_char( c ) for c in '8426' ]
with open( 'tmp.json' ) as f:
	config = json.load( f )
	x = config[ 'start' ][ 'x' ]
	y = config[ 'start' ][ 'y' ]
	z = config[ 'start' ][ 'z' ]
	map = config[ 'map' ][ z ]

old = None

def on_press( key ):
	global elapsed, pressed, x, y, map, config, old
	try:
		if time( ) > elapsed:
			if key in keys:
				if key == keys[ 0 ]:
					pressed = 'north'
					mouse.position = ( 525, 225 )
				elif key == keys[ 1 ]:
					pressed = 'west'
					mouse.position = ( 465, 265 )
				elif key == keys[ 2 ]:
					pressed = 'south'
					mouse.position = ( 525, 301 )
				elif key == keys[ 3 ]:
					pressed = 'east'
					mouse.position = ( 565, 265 )

				mouse.press( Click.left )
				mouse.release( Click.left )
				system( 'clear' )
				print( pressed )
				mouse.position = ( 200, 600 )
				
				sleep( 1 )
				img = screenshot.get( x = 845, y = 5, width = 106, height = 109 )

				if not old == img:
					if pressed == 'north':
						if y > 1:
							y -= 1
						map[ y ][ x ] = 1
						if y == 1:
							y += 1
							map.insert( 0, [ 0 ] * len( map[ 0 ] ) )
							for p in config[ 'positions' ]: p[ 'y' ] += 1
							#for w in config[ 'waypoints' ]: w[ 'y' ] += 1
					elif pressed == 'west':
						if x > 1:
							x -= 1
						map[ y ][ x ] = 1
						if x == 1:
							x += 1
							for m in map: m.insert( 0, 0 )
							for p in config[ 'positions' ]: p[ 'x' ] += 1
							#for w in config[ 'waypoints' ]: w[ 'x' ] += 1
					elif pressed == 'south':
						y += 1
						if y == len( map ) - 2:
							map.append( [ 0 ] * len( map[ 0 ] ) )
						map[ y ][ x ] = 1
					elif pressed == 'east':
						x += 1
						if x == len( map[ 0 ] ) - 2:
							for m in map: m.append( 0 )
						map[ y ][ x ] = 1

					position = { 'x': x, 'y': y, 'z': z, 'img': img }
					can = True
					
					for p in config[ 'positions' ]:
						if p[ 'x' ] == position[ 'x' ] and p[ 'y' ] == position[ 'y' ]:
							can = False
							break
					if can:
						config[ 'positions' ].append( position )
						print( 'New Position, Added!' )
					else:
						print( 'Old Position, Omitted!' )
					output = json.dumps( config, indent = 2 )
					output = fix.json( output )
					with open ( script, 'w' ) as file:
						file.write( output )
						#print(getcwd())
					pressed = ''
					old = img
				else:
					print( 'Can\'t Walk' )
				
				MAP = [ ROW[ x-2:x+3 ] for ROW in map[ y-2:y+3 ] ]
				MAP[ 2 ][ 2 ] = 2
				for m in MAP:
					print( m )
				elapsed = time( ) + 1
	except:
		pass

with Listener( on_press = on_press ) as listener:
	listener.join( )
