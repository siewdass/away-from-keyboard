#!/usr/bin/env python3
import cv2

img = cv2.imread( '8-map.png' )

cv2.namedWindow('test',cv2.WINDOW_NORMAL)
cv2.imshow("test",img) 
cv2.waitKey(0) 
cv2.destroyAllWindows() 

