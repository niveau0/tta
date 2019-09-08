inherit "../../ess_base";

#include <combat.h>
#include <macros.h>
#include <magic.h>
#include <skills.h>
#include <properties.h>

string
query_desc()
{
    return "Give the target a better chance to disarm a target. Power " +
	" increases the disarm trap skill."; 
}

int
query_s()
{
    return SK_DISARM_TRAP;
}

void
spell_effect(int level, object *tar, object tp)
{
    object ob;
    
    if (!sizeof(tar))
	return;
    
    ob = clone_object(MASTER);
    tar[0]->catch_tell("You fingers feel very nimble.\n");
    tar[0]->add_tmp_skill(query_s(), 20+level-query_level());
    tar[0]->add_spell(ob);
    ob->store_spell(tp, level, ({ tar[0] }), 20+level-query_level());
    ob->start_call_out("remove_spell", 60);
}
void
remove_spell()
{
    object *tar;

    if (sizeof(tar = query_targets()))
    {
	tar[0]->catch_tell("You fingers feel normal again.\n");
	tar[0]->add_tmp_skill(query_s(), query_data() * (-1));
    }
    destruct_object();
}

int
query_level()
{
    return 5;
}

int *
query_spell_data()
{
    return
	({
	    SP_FORCE,
	    SP_LEVEL_TIME,
	    SP_TOUCH,
	    1,
	    SP_ALL_LIVINGS,
	    SK_ML_ASTER,
	});
}
