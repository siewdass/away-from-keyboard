#include "types.h"
#include "shared.h"

class Status {
public:
	int id,
		width,
		health,
		mana,
		max;
	bool hungry,
		paralyze,
		poison,
		isTargeting,
		haste;
	tuple < int, int, bool, bool, bool, bool, string, vector < Point >, bool, bool, bool, bool, bool > update( );
	bool valuableLoot( );
};