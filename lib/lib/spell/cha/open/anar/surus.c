inherit "../../cha_base";

#include <combat.h>
#include <macros.h>
#include <magic.h>
#include <skills.h>
#include <event.h>

string
query_desc()
{
    return "Create a blinding flash of light, all beside the flash must " +
	"resist or will be stunned for short. Power manipulates " +
	"the required resistance.";
}

void
spell_effect(int level, int *co, object tp)
{
    int    i, sz, *range;
    object *tar;
    
    tar = I(E(tp), 2);
    range = ({ 120, 120, 120 });
    for (i=0, sz=sizeof(tar); i<sz; i++)
	if (!in_range(tar[i]->query_coordinates(), co, range))
	    tar[i] = 0;
    tar = resist(tp, tar - ({ tp, 0 }), level, query_spell_data()[SP_TYPE]);
    tell_objects(LISTEN(E(tp)) - tar,
		 "A blinding light flashes up for a short moment.\n",
		 MSG_SEE, tp);
    tell_objects(tar, "A blinding light flashes up beside you. You are " +
		 "shocked and stunned.\n", MSG_SEE, 0);
    for (i=0, sz=sizeof(tar); i<sz; i++)
	tar[i]->cb_add_boni(({ CB_STUNNED }), ({ ([ time() + 20 : 1 ]) }));
}

int
query_level()
{
    return 5;
}

int *
query_spell_data()
{
    return
	({
	    SP_FORCE,
	    SP_NONE,
	    1200,
	    SP_DIRECTION,
	    0,
	    SK_ML_ASTER,
	});
}
