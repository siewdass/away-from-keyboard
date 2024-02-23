#include "acknow.h"
#include "screenshot.h"

class Viewport {
public:
	Acknow acknow;
	Screenshot screenshot;
	int id;
	int sqm = 41; 
	Point xy( int, int );
	tuple < int, vector < Point >, bool, string, bool, Point, bool > check( );
	Point TARGET;
};