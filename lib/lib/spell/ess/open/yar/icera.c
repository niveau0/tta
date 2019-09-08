inherit "../../ess_base";

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
    return "Creates an aura of warmth around one target, that increases " +
	"it's cold resistance. It is more powerful then the 'shiv' spell. " +
	"Power manipulates the endurance of the spell.";
}

mixed
query_similar_code()
{
    return "_covra"; // AENDERN !
}

void
spell_effect(int level, object *tar, object tp)
{
    if (sizeof(tar))
    {
	object ob;
	
	tp->catch_msg(({ "You create a strange aura of warmth around " }) +
		      ({ QCTNAME(tar[0]) }) + ({ ".\n" }));
	if (tp != tar[0])
	    tar[0]->catch_tell("You are anveloped in a warm veil.\n");
	ob = clone_object(MASTER);
	ob->store_spell(tp, level, ({ tar[0] }));
	ob->start_call_out("remove_spell", 60*(level-9));
	tar[0]->add_spell(ob);
	tar[0]->cb_add_boni(({ CB_RESIST_COLD }),
			    ({ ([ ob : 20 ]) }));
    }
}

void
remove_spell()
{
    object *tar;

    if (sizeof(tar = query_targets()))
    {
	tar[0]->catch_tell("The warm veil around you is gone.\n");
	tar[0]->cb_remove_boni(({ CB_RESIST_COLD }),
			       ({ this_object() }));
    }
    destruct_object();
}

int
query_level()
{
    return 10;
}

int *
query_spell_data()
{
    return ({
	SP_DEFENSIVE,
	SP_LEVEL_TIME,
	10,
	1,
	SP_ALL_LIVINGS,
	SK_ML_PETRA,
	});
}
