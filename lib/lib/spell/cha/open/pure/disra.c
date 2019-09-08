inherit "../../cha_base";

#include <combat.h>
#include <effects.h>
#include <macros.h>
#include <magic.h>
#include <skills.h>
#include <properties.h>

string
query_desc()
{
    return "Protect someone against disease.";
}

mixed
query_similar_code()
{
    return "_disra";
}

void
spell_effect(int level, object *tar, object tp)
{
    if (sizeof(tar))
    {
	object ob;
	
	ob = clone_object(MASTER);
	ob->store_spell(tp, level, tar);
	ob->start_call_out("remove_spell", 600 * level);
	tar[0]->add_spell(ob);
	tar[0]->cb_add_boni(({ CB_RESIST_DISEASE }),
			    ({ ([ ob : level * 2 + 10 ]) }));
	tar[0]->catch_tell("You feel somehow protected.\n");
    }
}

void
remove_spell()
{
    object *tar;
    
    if (sizeof(tar = query_targets()))
    {
	if (tar[0] == query_caster())
	    tar[0]->catch_tell("Your disease protection is gone.\n");
	else
	    tar[0]->catch_tell("You feel less protected.\n");
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
    return
	({
	    SP_HEALING,
	    SP_LEVEL_TIME,
	    120,
	    1,
	    SP_ALL_LIVINGS,
	    SK_ML_IHYAMA,
	});
}
