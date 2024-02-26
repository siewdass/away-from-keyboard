#include "path.h"
#include <vector>

using namespace std;

Array < Point > Path::find( vector < vector < int > > map, Point from, Point to ) {
	pf::Astar astar;
	astar.loadMap( map );
	return astar.findPath( from, to );
}