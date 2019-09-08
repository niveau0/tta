inherit "../../ess_base";

#include <combat.h>
#include <composite.h>
#include <macros.h>
#include <magic.h>
#include <skills.h>
#include <properties.h>

string
query_desc()
{
    return "Blurs the victims and gives some defense boni. Power manipulates" +
	" the endurance of the spell and the number of targets.";
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
    object ob;
    
    if (!(sz = sizeof(tar)))
	return;
    
    ob = clone_object(MASTER);

    for(i=sz; i--;)
    {
	tar[i]->cb_add_boni( ({ CB_DB_BONUS }),
			     ({ ([ob : 10]) }) );
	tar[i]->add_spell(ob);
	if (tp != tar[i])
	    tar[i]->catch_tell("Suddenly you feel very unreal.\n");
    }
    tp->catch_msg(({ "After finishing your spell, " }) +
		  COMPOSITE_ARRAY(tar) + ({ " seem" +
						(sizeof(tar)!=1 ? "" : "s") +
						" to be unreal.\n" }),
		  tp, MSG_SEE);
    
    ob->store_spell(tp, level, tar);
    ob->start_call_out("remove_spell", 60*(level-12));
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
	     tar[i]->catch_tell("You feel real again.\n");
	     tar[i]->cb_remove_boni(({ CB_DB_BONUS }),
				    ({ this_object() }));
	 }
	 destruct_object();
	 return;
    }

    if (!IN_ARRAY(t, tar))
        return;

    t->catch_tell("You feel real again.\n");
    t->cb_remove_boni(({ CB_DB_BONUS }),
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
    return 13;
}

int *
query_spell_data()
{
    return
	({
	    SP_ELEMENTAL,
	    SP_LEVEL_TIME,
	    1200,
	    SP_LEVEL_RANGE,
	    SP_ALL_LIVINGS,
	    SK_ML_ASTER,
	});
}
