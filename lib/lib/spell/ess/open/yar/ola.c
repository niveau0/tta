inherit "../../ess_base";

#include <combat.h>
#include <composite.h>
#include <event.h>
#include <magic.h>
#include <macros.h>
#include <mudtime.h>
#include <skills.h>
#include <state.h>
#include <properties.h>

string
query_desc()
{
    return "Creates a powerful aura around the target. It protects against " +
	"against lightning, heat and cold at the same time. As " +
	"many targets as the level of the spellcaster can be affected. "+
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
    int i, sz;
    
    if (sz = sizeof(tar))
    {
	object ob;
	
	ob = clone_object(MASTER);
	tp->catch_msg(({ "You create a powerful aura around " }) +
		      COMPOSITE_ARRAY(tar)  + ({ ".\n" }), tp, MSG_SEE);
	for (i=sz; i--;)
	{
	    if (tp != tar[i])
		tar[i]->catch_tell("You are anveloped in an aura.\n");
	    tar[i]->add_spell(ob);
	    tar[i]->cb_add_boni(({ CB_RESIST_COLD, CB_RESIST_HEAT,
				       CB_RESIST_ELECTRICITY }),
				({ ([ ob : 20 ]), ([ ob : 20 ]),
				      ([ ob : 20 ]) }));
	}
	ob->store_spell(tp, level, tar);
	ob->start_call_out("remove_spell", (86400/TIME_SCALE) + 60*(level-49));
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
    if(!t)
    {
	for (i=sz; i--;)
	{
	    tar[i]->catch_tell("The aura around you is gone.\n");
	    tar[i]->cb_remove_boni(({ CB_RESIST_COLD, CB_RESIST_HEAT,
					  CB_RESIST_ELECTRICITY}),
				   ({ this_object() }));
	}
	destruct_object();
	return;
    }
    if (!IN_ARRAY(t, tar))
	return;

    t->catch_tell("The aura around you is gone.\n");
    t->cb_remove_boni(({ CB_RESIST_COLD, CB_RESIST_HEAT,
			     CB_RESIST_ELECTRICITY}),
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
    return 50;
}

int *
query_spell_data()
{
    return ({
	SP_DEFENSIVE,
	SP_LEVEL_TIME,
	1000,
	SP_LEVEL_TARGETS,
	SP_ALL_LIVINGS,
	SK_ML_PETRA,
	});
}
