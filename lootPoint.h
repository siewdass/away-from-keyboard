
using namespace std;

class lootPoint {
	public: 
	int x;
	int y;
	lootPoint( ) {
		this->x = 0;
		this->y = 0;
	}
	void add( int x, int y ) {
		this->x = x;
		this->y = y; 
	}
};