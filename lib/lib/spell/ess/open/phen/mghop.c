inherit "../../ess_base";

#include <composite.h>
#include <magic.h>
#include <macros.h>
#include <properties.h>
#include <skills.h>

string
query_desc()
{
    return "Targets can see in the dark. Power manipulates the endurance of " +
	"the spell and the number of targets the caster can affect.";
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

	for (i=sz; i--; )
	{
	    if (!tar[i]->query_prop(LIVE_SEE_DARK))
		tar[i]->catch_tell("You feel some strange power " +
				   "in your eyes.\n");
	    tar[i]->add_spell(ob);
	    tar[i]->change_int_prop(LIVE_SEE_DARK, 1);
	}
	ob->store_spell(tp, level, tar);
	if (sizeof(tar -= ({ tp })))
	    tp->catch_msg(({"You transfer your spell to " }) +
			  COMPOSITE_ARRAY(tar) + ({ ".\n" }), tp, MSG_SEE);
	ob->start_call_out("remove_spell", 600*(level-17));
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
	    tar[i]->change_int_prop(LIVE_SEE_DARK, -1);
	    if (!tar[i]->query_prop(LIVE_SEE_DARK))
		tar[i]->catch_tell("Some power left your eyes.\n");
	}
	destruct_object();
	return;
    }
    
    if (!IN_ARRAY(t, tar))
	return;

    t->change_int_prop(LIVE_SEE_DARK, -1);
    if (!t->query_prop(LIVE_SEE_DARK))
	t->catch_tell("Some power left your eyes.\n");
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
    return 18;
}

int *
query_spell_data()
{
    return ({
	SP_ELEMENTAL,
	SP_LEVEL_TIME,
	10,
	SP_LEVEL_TARGETS,
	SP_ALL_LIVINGS,
	SK_ML_PETRA,
	});
}
