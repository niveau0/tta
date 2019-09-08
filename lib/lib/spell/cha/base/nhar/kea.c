inherit "../../cha_base";

#include <combat.h>
#include <macros.h>
#include <magic.h>
#include <properties.h>
#include <skills.h>
#include <event.h>

string
query_desc()
{
    return "Stuns an opponent.";
}

void
spell_effect(int level, object *tar, object tp)
{
    if (sizeof(tar))
    {
	tar[0]->catch_tell("Uh, you feel dizzy, stunned.\n");
	tell_objects(LISTEN(E(tp)) - ({ tar[0] }),
		     ({ QCTNAME(tar[0]), " stumbles for a moment.\n" }),
		     MSG_SEE, tar[0]);
	tar[0]->cb_add_boni(({ CB_STUNNED }),
			    ({ ([ (time() -
				   tar[0]->query_prop(LIVE_LAST_RR_RESULT))
				 : 1 ]) }) );
    }
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
	    (SP_FORCE | SP_INSTANTLY),
	    SP_NONE,
	    600,
	    1,
	    SP_ALL_LIVINGS,
	    SK_ML_QADOSH,
	});
}
