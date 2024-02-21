#include "screenshot.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>

int a = XInitThreads( );

Display *display = XOpenDisplay( NULL );

Mat Screenshot::get( int tibia, int x, int y, int width, int height, bool minimap ) {

    XImage* img = XGetImage( display, tibia, x, y, width, height, AllPlanes, ZPixmap );

    tmp = Mat( height, width, CV_8UC4, img->data );

    if ( minimap ) resize( tmp, tmp, Size( width / 4, height / 4 ) );

	cvtColor( tmp, tmp, COLOR_BGRA2BGR );

    XDestroyImage( img );

	return tmp;
}
