inherit "../../ess_base";

#include <combat.h>
#include <composite.h>
#include <event.h>
#include <magic.h>
#include <macros.h>
#include <skills.h>
#include <state.h>
#include <properties.h>

string
query_desc()
{
    return "Creates a cold aura against heat on every living in the " +
	"spellrange of the caster. Power manipulates the endurance of " +
	"the spell.";
}

mixed
query_similar_code()
{
    return "_covra"; // AENDERN !
}

void
spell_effect(int level, object *tar, object tp)
{
    int i, sz;
    
    if (sz = sizeof(tar))
    {
	object ob;

	ob = clone_object(MASTER);
	tp->catch_msg(({ "You create a cold aura around " }) +
		      COMPOSITE_ARRAY(tar) + ({ ".\n" }), tp, MSG_SEE);
	for (i=sz; i--;)
	{
	    if (tar[i] != tp)
		tar[i]->catch_tell("You are anveloped in a cold veil.\n");
	    tar[i]->add_spell(ob);
	    tar[i]->cb_add_boni(({ CB_RESIST_HEAT }),
				({ ([ ob : 10 ]) }));
	}
	ob->store_spell(tp, level, tar);
	ob->start_call_out("remove_spell", 60*(level-4));
    }
}

void
remove_spell(object t)
{
    int i, sz;
    object *tar;

    if (!(sz = sizeof(tar = query_targets())))
    {
	destruct_object();
	return;
    }
    if (!t)
    {
	for (i=sz; i--;)
	{
	    tar[i]->catch_tell("The cold veil around you is gone.\n");
	    tar[i]->cb_remove_boni(({ CB_RESIST_HEAT }),
				   ({ this_object() }));
	}
	destruct_object();
	return;
    }
    if (!IN_ARRAY(t, tar))
	return;

    t->catch_tell("The cold veil around you is gone.\n");
    t->cb_remove_boni(({ CB_RESIST_HEAT }),
		      ({ this_object() }));
    tar -= ({ t });
    if (sizeof(tar))
    {
        store_spell(query_caster(), query_cast_level(), tar);
        return;
    }
    destruct_object();
}

int
query_level()
{
    return 5;
}

int *
query_spell_data()
{
    return ({
	SP_DEFENSIVE,
	SP_LEVEL_TIME,
	10,
	SP_LEVEL_RANGE,
	SP_ALL_LIVINGS,
	SK_ML_PETRA,
	});
}
