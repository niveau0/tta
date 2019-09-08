inherit "../../men_base";

#include <combat.h>
#include <event.h>
#include <magic.h>
#include <macros.h>
#include <skills.h>
#include <state.h>
#include <properties.h>

string
query_desc()
{
    return "Increases the casters heat resistance by surrounding him with " +
	   "with a cold aura. Power manipulates the endurance of the spell.";
}

mixed
query_similar_code()
{
    return "_covra"; // AENDERN !
}

void
spell_effect(int level, object *tar, object tp)
{
	object ob;
	
	tp->catch_tell("You are surrounded by a cold veil.");
	ob = clone_object(MASTER);
	ob->store_spell(tp, level, ({ tar[0] }));
	ob->start_call_out("remove_spell", 60*(level));
	tar[0]->add_spell(ob);
	tar[0]->cb_add_boni(({ CB_RESIST_HEAT }),
			    ({ ([ ob : 20 ]) }));
}

void
remove_spell()
{
    object *tar;

    if (sizeof(tar = query_targets()))
    {
	tar[0]->catch_tell("The cold veil around you is gone.\n");
	tar[0]->cb_remove_boni(({ CB_RESIST_HEAT }),
			       ({ this_object() }));
    }
    destruct_object();
}

int
query_level()
{
    return 1;
}

int *
query_spell_data()
{
    return ({
	SP_DEFENSIVE,
	SP_LEVEL_TIME,
	SP_SELF,
	1,
	SP_ALL_LIVINGS,
	SK_ML_PETRA,
	});
}
