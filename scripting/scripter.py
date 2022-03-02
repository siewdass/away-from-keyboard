#!/usr/bin/env python3
from pynput import keyboard
from os import system
import screenshot
from fix import fix
from screenshot import screenshot
import json
from sys import exit

config = { }
x = 0
y = 0
z = 0
map = [ ]
pressed = ""
script = '../maps/map01.json'

with open( 'config.json' ) as f:
    config = json.load( f )
    x = config[ 'start' ][ 'x' ]
    y = config[ 'start' ][ 'y' ]
    z = config[ 'start' ][ 'z' ]
    map = config[ 'map' ][ z ]

def on_press( key ):
    global pressed, x, y, map, config
    try:
        if key == key.up:
            if not pressed == "up":
                pressed = "up"
                print( pressed )
        elif key == key.right:
            if not pressed == "right":
                pressed = "right"
                print( pressed )
        elif key == key.down:
            if not pressed == "down":
                pressed = "down"
                print( pressed )
        elif key == key.left:
            if not pressed == "left":
                pressed = "left"
                print( pressed )
        elif key == key.f2:
            config[ 'waypoints' ].append( { 'x': x, 'y': y, 'z': z } )
            output = json.dumps( config, indent = 2 )
            output = fix.json( output )
            with open ( script, 'w' ) as file:
                file.write( output )
            print( "New Waypoint, Added!" )
        elif key == key.f1:
            if pressed == "up":
                if y > 0:
                    y -= 1
                map[ y ][ x ] = 1
                if y == 0:
                    y += 1
                    map.insert( 0, [ 0 ] * len( map[ 0 ] ) )
                    for p in config[ 'positions' ]: p[ 'y' ] += 1
                    #for w in config[ 'waypoints' ]: w[ 'y' ] += 1
            elif pressed == "right":
                x += 1
                if x == len( map[ 0 ] ) - 1:
                    for m in map: m.append( 0 )
                map[ y ][ x ] = 1
            elif pressed == "down":
                y += 1
                if y == len( map ) - 1:
                    map.append( [ 0 ] * len( map[ 0 ] ) )
                map[ y ][ x ] = 1
            elif pressed == "left":
                if x > 0:
                    x -= 1
                map[ y ][ x ] = 1
                if x == 0:
                    x += 1
                    for m in map: m.insert( 0, 0 )
                    for p in config[ 'positions' ]: p[ 'x' ] += 1
                    #for w in config[ 'waypoints' ]: w[ 'x' ] += 1
            
            if pressed in [ 'up', 'right', 'down', 'left' ]:
                pressed = ''
                #system( "clear" )
                print( "x:", x, "y:", y, "z:", z )
                #for m in map:
                #   print( m )
                
                img = screenshot.get( x = 633, y = 5, width = 106, height = 109 )
                position = { 'x': x, 'y': y, 'z': z, 'img': img }
                can = True
                for p in config[ 'positions' ]:
                    if p[ 'x' ] == position[ 'x' ] and p[ 'y' ] == position[ 'y' ]:
                        can = False
                        break
                if can:
                    config[ 'positions' ].append( position )
                    print( 'New Position, Added!')
                else:
                    print( 'Old Position, Omitted!' )
                output = json.dumps( config, indent = 2 )
                output = fix.json( output )
                with open ( script, 'w' ) as file:
                    file.write( output )
            else:
                print( "Not Saved!" )
        elif key == key.esc:
            print( "Script Finished!." )
            exit( )
    except Exception as err:
        pass
        #print( err )

with keyboard.Listener( on_press = on_press, ) as listener:
    listener.join( )
    
