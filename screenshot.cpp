#include "screenshot.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>

int a = XInitThreads( );

Display *display = XOpenDisplay( NULL );
Mat BGRA, BGR;

Mat Screenshot::get( int tibia, int x, int y, int width, int height ) {

    XImage* img = XGetImage( display, tibia, x, y, width, height, AllPlanes, ZPixmap );

    BGRA = Mat( height, width, CV_8UC4, img->data );

	cvtColor( BGRA, BGR, CV_BGRA2BGR );

    XDestroyImage( img );

	return BGR;
}
