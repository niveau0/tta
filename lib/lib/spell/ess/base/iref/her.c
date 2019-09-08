inherit "../../ess_base";

#include <combat.h>
#include <macros.h>
#include <magic.h>
#include <skills.h>
#include <properties.h>
#include <event.h>

string
query_desc()
{
    return "Throw a firebolt at your enemy. Power manipulates " +
	"the damage.";
}

void
spell_effect(int level, object *tar, object tp)
{
    int diff;

    if (!sizeof(tar))
	return;
    
    if (tar[0] == tp)
    {
	tp->catch_tell("Want to kill yourself?\n");
	return;
    }
    if ((diff =
	 directed_spell(tp, tar[0], SK_MATTACK_FIRE_BOLT,
			tar[0]->cb_query_bonus(CB_ELEMENTAL_DEFENSE))) == -1)
    {
	tar[0]->catch_msg("A bolt of fire darts off, " +
			  "but misses you.\n", tp, MSG_SEE);
	tell_objects(LISTEN(E(tp)) - ({ tar[0] }),
		     "A bolt of fire darts off, but misses.\n",
		     MSG_SEE, tp);
	return;
    }
    tp->catch_tell("A bolt of fire darts off and hits " +
		   tar[0]->query_objective() + ".\n");
    tar[0]->catch_tell("A bolt of fire darts off and hits " +
		       "you, argh!\n");
    tell_objects(LISTEN(E(tp)) - ({ tp, tar[0] }),
		 ({ "A bolt of fire darts off and hits ",
		    QTNAME(tar[0]), ".\n" }), MSG_SEE, tp);
    directed_damage(tp, tar[0], diff, level,
		    ([
			A_SKILL     : SK_MATTACK_FIRE_BOLT,
			A_DMOD      : 45,
			A_OBMOD     : 10,
			A_SKIN      : 70,
			A_S_LEATHER : 70,
			A_R_LEATHER : 75,
			A_CHAIN     : 75,
			A_PLATE     : 80,
			A_CRIT      : ({ HEAT }),
			]));
}

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
	    SP_ELEMENTAL,
	    SP_NONE,
	    1200,
	    1,
	    SP_ALL_LIVINGS,
	    SK_ML_ASTER,
	});
}
