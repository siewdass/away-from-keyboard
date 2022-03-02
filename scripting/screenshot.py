from Xlib import display, X
import numpy as np
import cv2 as cv
from base64 import b64encode

class screenshot:
	def get( **kwargs ):
		
		DISPLAY = display.Display( )
		root = DISPLAY.screen( ).root
		raw = root.get_image( kwargs[ "x" ], kwargs[ "y" ], kwargs[ "width" ], kwargs[ "height" ], X.ZPixmap, 0xffffffff )
		img = np.frombuffer( raw.data, dtype = np.uint8 ).reshape( kwargs[ "height" ], kwargs[ "width" ], 4 )[ :, :, :3 ]
		
		encode = cv.imencode( '.png', img )[ 1 ]
		base64 = b64encode( encode ).decode( 'utf-8' )
		
		#if kwargs[ "save" ]:
		#	cv.imwrite( 'screenshot.png', img )
		return base64
