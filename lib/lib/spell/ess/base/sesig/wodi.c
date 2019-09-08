inherit "../../ess_base";

#include <combat.h>
#include <macros.h>
#include <magic.h>
#include <skills.h>
#include <properties.h>

string
query_desc()
{
    return "Let the target appear in shadowform. It helps to move very " +
	"quit and to hide easy. Power manipulates the endurance of the spell.";
}

mixed
query_similar_code()
{
    return "_covra"; // AENDERN !
}

void
spell_effect(int level, object *tar, object tp)
{
    int hide_bonus, stalk_bonus;
    object ob;
    
    if (!sizeof(tar))
	return;

    hide_bonus = 25 + random(50);
    stalk_bonus = 25 + random(50);
    ob = clone_object(MASTER);
    tar[0]->catch_tell("You feel somehow transparent.\n");
    tar[0]->add_tmp_skill(SK_HIDE, hide_bonus);
    tar[0]->add_tmp_skill(SK_STALK, stalk_bonus);

    tar[0]->add_spell(ob);
    ob->store_spell(tp, level, ({ tar[0] }), ({ hide_bonus, stalk_bonus }) );
    ob->start_call_out("remove_spell", 60*(level-1));
}
void
remove_spell()
{
    int *qd;
    object *tar;
    
    qd = query_data();
    
    if (sizeof(tar = query_targets()))
    {
	tar[0]->add_tmp_skill(SK_HIDE, qd[0]*(-1));
	tar[0]->add_tmp_skill(SK_STALK, qd[1]*(-1));
	tar[0]->catch_tell("You feel solid again.\n");
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
	    SP_ELEMENTAL,
	    SP_LEVEL_TIME,
	    1200,
	    1,
	    SP_ALL_LIVINGS,
	    SK_ML_ASTER,
	});
}
