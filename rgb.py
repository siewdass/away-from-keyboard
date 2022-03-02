import cv2 as cv
import signal
import sys

img = cv.imread( 'screenshot.png' )

cv.namedWindow( 'screenshot.png', cv.WINDOW_NORMAL )
cv.resizeWindow( 'screenshot.png', 1000, 600 )
cv.imshow( 'screenshot.png', img )
cv.waitKey( 0 )
cv.destroyAllWindows( )