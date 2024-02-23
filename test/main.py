#!/usr/bin/env python3
import cv2
import sys

img = cv2.imread( 'test/' + sys.argv[1] )

cv2.namedWindow('test',cv2.WINDOW_NORMAL)
cv2.imshow("test",img) 
cv2.waitKey(0) 
cv2.destroyAllWindows() 

