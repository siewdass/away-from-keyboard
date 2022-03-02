#include "types.h"
#include "shared.h"

class Status {
	public:
		int id,
		    health,
		    mana;
		bool hungry,
		    paralyze,
		    poison,
			isTargeting,
			haste;
		tuple < int, int, bool, bool, bool, bool, int, int, int, string, vector < Point > > update( vector < Position > );
};