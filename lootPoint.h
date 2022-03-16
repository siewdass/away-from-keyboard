
using namespace std;

class lootPoint {
	public: 
	int x;
	int y;
	lootPoint( ) {
		x = 0;
		y = 0;
	}
	void add( int X, int Y ) {
		x = X;
		y = Y; 
	}
};