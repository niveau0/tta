#include <magic.h>
#include <skills.h>

inherit "pollo";

int
query_level()
{
    return 30;
}

int *
query_spell_data()
{
    return
	({
	    SP_FORCE,
	    SP_NONE,
	    10000,
	    1,
	    SP_ALL_LIVINGS,
	    SK_ML_ASTER,
	});
}
