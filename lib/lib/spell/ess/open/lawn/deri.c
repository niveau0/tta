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
    return "Creates a protecting shield against elemental attacks. Power " +
	"manipulates the strength of the shield.";
}

mixed
query_similar_code()
{
    return "_covre";
}

void
spell_effect(int level, object *tar, object tp)
{
    if (sizeof(tar))
    {
	int i;
	object ob;

	tar[0]->catch_tell("You feel safety covering your body.\n");
	i = level/5;
	if (i<1)
	    i=1;
	
	ob = clone_object(MASTER);
	ob->store_spell(tp, level, tar);
	tp->add_spell(ob);
	if (tp != tar[0])
	    tar[0]->add_spell(ob);
	tar[0]->cb_add_boni(({ CB_ELEMENTAL_DEFENSE }),
			    ({ ([ ob : i * 3 ]) }));
	ob->start_call_out("remove_spell", 60*(level-query_level()+1));
    }
}

void
remove_spell()
{
    object *tar;
    
    if (sizeof(tar = query_targets()))
	tar[0]->catch_tell("You feel that some safety is gone.\n");
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
	100,
	1,
	SP_ALL_LIVINGS,
	SK_ML_PETRA,
	});
}
