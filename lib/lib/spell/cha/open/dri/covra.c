inherit "../../cha_base";

#include <combat.h>
#include <magic.h>
#include <macros.h>
#include <skills.h>
#include <state.h>
#include <properties.h>

string
query_desc()
{
    return "Creates a protection aura against elemental attacks (for up " +
	"to 10 targets).";
}

mixed
query_similar_code()
{
    return "_covra";
}

void
spell_effect(int level, object *tar, object tp)
{
    if (sizeof(tar))
    {
	int    i, sz;
	object ob;
	
	tp->catch_tell("You create an aura of protection.\n");
	tell_objects(tar, "You feel safety covering your body.\n", 0, 0);
	ob = clone_object(MASTER);
	ob->store_spell(tp, level, tar);
	ob->start_concentrate(120);
	for (i=0, sz=sizeof(tar); i<sz; i++)
	{
	    tar[i]->add_spell(ob);
	    tar[i]->cb_add_boni(({ CB_ELEMENTAL_DEFENSE }),
				({ ([ ob : level * 3 ]) }));
	}
    }
}

void
remove_spell(object t)
{
    int    i, sz;
    object c, *tar;

    if (!(sz = sizeof(tar = query_targets())))
    {
	destruct_object();
	return;
    }
    if (!t)
    {
	for (i=0; i<sz; i++)
	    tar[i]->catch_tell("Some magical protection seems to be gone.\n");
	if (c = query_caster())
	{
	    c->delete_state(CONCENTRATE);
	    c->add_prop(CONCENTRATE_DATA, 0);
	}
	destruct_object();
	return;
    }
    if (!IN_ARRAY(t, tar))
	return;
    tar -= ({ t });
    if (sizeof(tar))
    {
	t->cb_remove_boni(({ CB_ELEMENTAL_DEFENSE }), ({ this_object() }));
	t->catch_tell("Some magical protection seems to be gone.\n");
	store_spell(query_caster(), query_cast_level(), tar);
	return;
    }
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
    return 2;
}

int *
query_spell_data()
{
    return ({
	SP_DEFENSIVE,
	SP_CONCENTRATE,
	120,
	10,
	SP_ALL_LIVINGS,
	SK_ML_PETRA,
	});
}
