inherit "../../ess_base";

#include <magic.h>
#include <macros.h>
#include <skills.h>
#include <properties.h>

string
query_desc()
{
    return "Target can see in the dark. Power manipulates the endurance of " +
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
    
    if (sizeof(tar))
    {
	object ob;

	ob = clone_object(MASTER);
	if (!tar[0]->query_prop(LIVE_SEE_DARK))
	    tar[0]->catch_tell("You feel some strange power in your eyes.\n");
	if (tar[0] != tp)
	    tp->catch_tell("You transfer your spell.\n");
	tar[0]->add_spell(ob);
	tar[0]->change_int_prop(LIVE_SEE_DARK, 1);
	ob->store_spell(tp, level, ({ tar[0] }));
	ob->start_call_out("remove_spell", 600*(level-3));
    }
}

void
remove_spell()
{
    int i, sz;
    object *tar;

    if (sz = sizeof(tar = query_targets()))
    {
	for (i=sz; i--;)
	{
	    tar[i]->change_int_prop(LIVE_SEE_DARK, -1);
	    if (!tar[i]->query_prop(LIVE_SEE_DARK))
		tar[i]->catch_tell("Some power left your eyes.\n");
	}
    }
    destruct_object();
}

int
query_level()
{
    return 4;
}

int *
query_spell_data()
{
    return ({
	SP_ELEMENTAL,
	SP_LEVEL_TIME,
	10,
	1,
	SP_ALL_LIVINGS,
	SK_ML_PETRA,
	});
}
