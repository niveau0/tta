inherit "../../men_base";

#include <combat.h>
#include <macros.h>
#include <magic.h>
#include <skills.h>
#include <properties.h>

string
query_desc()
{
    return "Blurs the victim and gives some defense boni. Power manipulates " +
	"the endurance of the spell.";
}

mixed
query_similar_code()
{
    return "_covra"; // AENDERN !
}

void
spell_effect(int level, object *tar, object tp)
{
    object ob;
    
    if (!sizeof(tar))
	return;
    
    ob = clone_object(MASTER);
    tp->catch_tell("Suddenly you feel very unreal.\n");
    tp->cb_add_boni( ({ CB_DB_BONUS }),
			  ({ ([ob : 10]) }) );
    tp->add_spell(ob);
    ob->store_spell(tp, level, tp);
    ob->start_call_out("remove_spell", 60*level);
}
void
remove_spell()
{
    object *tar;

    if (sizeof(tar = query_targets()))
    {
	tar[0]->catch_tell("You feel real again.\n");
        tar[0]->cb_remove_boni( ({ CB_DB_BONUS }),
				({ this_object() }));
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
    return
	({
	    SP_FORCE,
	    SP_LEVEL_TIME,
	    SP_SELF,
	    1,
	    SP_ALL_LIVINGS,
	    SK_ML_ASTER,
	});
}
