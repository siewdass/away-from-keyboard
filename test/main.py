#!/usr/bin/env python3
import cv2


img = cv2.imread( 'located.png' )
#img = cv2.imread( 'z.png' )

cv2.namedWindow('test',cv2.WINDOW_NORMAL)
cv2.imshow("test",img) 
cv2.waitKey(0) 
cv2.destroyAllWindows() 
