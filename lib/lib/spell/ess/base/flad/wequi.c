#include <magic.h>
#include <skills.h>

inherit "pollo";

int
query_level()
{
    return 6;
}

int *
query_spell_data()
{
    return
	({
	    SP_FORCE,
	    SP_NONE,
	    2000,
	    1,
	    SP_ALL_LIVINGS,
	    SK_ML_ASTER,
	});
}
