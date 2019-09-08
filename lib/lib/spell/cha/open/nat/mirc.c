inherit "../../cha_base";

#include <herb.h>
#include <macros.h>
#include <magic.h>
#include <skills.h>
#include <state.h>
#include <properties.h>

string
query_desc()
{
    return "This spells makes one animal obey your orders.";
}

void
spell_effect(int level, object *tar, object tp)
{
    object ob;
    
    if (sizeof(tar))
    {
	if (!tar[0]->query_prop(LIVE_ANIMAL))
	    return;
	ob = clone_object(MASTER);
	ob->store_spell(tp, level, tar);
	ob->start_concentrate(1200);
	tar[0]->add_spell(ob);
	tar[0]->add_prop(LIVE_CONTROLLED_BY, tp);	    
    }
}

void
remove_spell()
{
    object c, *tar;

    if (c = query_caster())
    {
	c->delete_state(CONCENTRATE);
	c->add_prop(CONCENTRATE_DATA, 0);
    }
    if (sizeof(tar = query_targets()))
	tar[0]->add_prop(LIVE_CONTROLLED_BY, 0);
    destruct_object();
}

int
query_level()
{
    return 9;
}

int *
query_spell_data()
{
    return
	({
	    SP_PASSIVE,
	    SP_NONE,
	    1200,
	    1,
	    SP_ALL_LIVINGS,
	    SK_ML_TOTEM,
	});
}
