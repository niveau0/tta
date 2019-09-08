inherit "../../cha_base";

#include <combat.h>
#include <macros.h>
#include <magic.h>
#include <skills.h>
#include <properties.h>
#include <state.h>

string
query_desc()
{
    return "Strong protection shield against 'cha' spells.";
}

mixed
query_similar_code()
{
    return "_shada";
}

int
query_resist_realm()
{
    return CB_RESIST_CHA;
}

void
spell_effect(int level, object *tar, object tp)
{
    if (sizeof(tar))
    {
	object ob;
	
	tar[0]->catch_tell("You feel safety covering your body.\n");

	ob = clone_object(MASTER);
	ob->store_spell(tp, level, tar);
	ob->start_concentrate(1200);
	tp->add_spell(ob);
	if (tp != tar[0])
	    tar[0]->add_spell(ob);
	tar[0]->cb_add_boni(({ query_resist_realm() }), ({ ([ ob : 50 ]) }));
    }
}

void
remove_spell()
{
    object c;
    
    if (c = query_caster())
    {
	c->delete_state(CONCENTRATE);
	c->add_prop(CONCENTRATE_DATA, 0);
    }
    destruct_object();
}

int
query_level()
{
    return 20;
}

int *
query_spell_data()
{
    return
	({
	    SP_DEFENSIVE,
	    SP_CONCENTRATE,
	    1200,
	    1,
	    SP_ALL_LIVINGS,
	    SK_ML_PETRA,
	});
}
