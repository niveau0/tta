inherit "../../ess_base";

#include <combat.h>
#include <macros.h>
#include <magic.h>
#include <skills.h>
#include <state.h>
#include <properties.h>

string
query_desc()
{
    return "Creates a protecting shield against elemental attacks on all " +
	"persons in spell range. Power manipulates the strength of the " +
	"shield.";
}

mixed
query_similar_code()
{
    return "_covre";
}

void
spell_effect(int level, object *tar, object tp)
{
    int i, sz;
    object ob;

    if (!(sz=sizeof(tar)))
	return;

    ob = clone_object(MASTER);

    for(i=sz; i--;)
    {
	tar[i]->catch_tell("You feel safety covering your body.\n");
	tar[i]->add_spell(ob);
	tar[i]->cb_add_boni(({ CB_ELEMENTAL_DEFENSE }),
			    ({ ([ ob : level * 3 ]) }));
    }
    ob->store_spell(tp, level, tar);
    ob->start_call_out("remove_spell", 60*level);
}

void
remove_spell(object t)
{
    int i, sz;
    object *tar;
    
    if (!(sz=sizeof(tar = query_targets())))
    {
	destruct_object();
	return ;
    }
    if (!t)
    {
        for (i=sz; i--;)
	    tar[i]->catch_tell("You feel that some safety is gone.\n");
	destruct_object();
	return;
    }

    if (!IN_ARRAY(t, tar))
        return;

    t->catch_tell("You feel that some safety is gone.\n");
    t->cb_remove_boni( ({ CB_ELEMENTAL_DEFENSE }) ,
		       ({ this_object() }) );
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
    return 3;
}

int *
query_spell_data()
{
    return ({
	SP_DEFENSIVE,
	SP_LEVEL_TIME,
	100,
	SP_LEVEL_RANGE,
	SP_ALL_LIVINGS,
	SK_ML_PETRA,
	});
}
