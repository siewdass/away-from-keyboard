#include "screenshot.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>

int init =  XInitThreads( );
Display *display;
Window root;

Screenshot::Screenshot( ) {
}

Screenshot::~Screenshot( ) {
}

void Screenshot::load( string value ) {
	display = XOpenDisplay( value.c_str( ) );
	root = DefaultRootWindow( display );
}

void Screenshot::unload( ) {
	XCloseDisplay( display );
}

Mat Screenshot::get( int x, int y, int width, int height ) {
	XImage* img = XGetImage( display, root, x, y, width, height, AllPlanes, ZPixmap );
	Mat tmp = Mat( height, width, CV_8UC4, img->data );
	cvtColor( tmp, tmp, COLOR_BGRA2BGR );
	XDestroyImage( img );
	return tmp;
}
