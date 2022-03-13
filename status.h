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
<<<<<<< HEAD
	tuple < int, int, bool, bool, bool, bool, int, int, int, string, vector < Point >, bool, bool, bool, bool > update( vector < Position > );
=======
	tuple < int, int, bool, bool, bool, bool, int, int, int, string, vector < Point >, bool, bool > update( vector < Position > );
	bool valuableLoot();
>>>>>>> 69d6d77a7ff23c8c7a7da9f40d03cecd99d443be
};