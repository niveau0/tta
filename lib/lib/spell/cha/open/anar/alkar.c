inherit "../../cha_base";

#include <combat.h>
#include <macros.h>
#include <magic.h>
#include <description.h>
#include <skills.h>
#include <event.h>

string
query_desc()
{
    return "Alkar creates a shielding aura around someone. Additionally " +
	"the affected person starts to shine and gets a powerful " +
	"appearance. Power manipulates the defense and appearance.";
}

void
spell_effect(int level, object *tar, object tp)
{
    if (sizeof(tar))
    {
	object ob;
	
	if (tar[0] == tp)
	    tp->catch_tell("You shield yourself.\n");
	else
	    tar[0]->catch_msg(({ "You start to shine in light.\n" }),
			      tp, MSG_SEE);
	tell_objects(LISTEN(E(tar[0])) - ({ tar[0] }),
		     ({ QCTNAME(tar[0]), " starts to shine.\n" }),
		     MSG_SEE, tp);
	ob = clone_object(MASTER);
	ob->store_spell(tp, level, tar);
	ob->start_call_out("remove_spell", 600 * level);
	tar[0]->add_spell(ob);
	tar[0]->add_lwvs(1, 0, 0, 0);
	tar[0]->cb_add_boni( ({ CB_DB_BONUS }),
			    ({ ([ ob : 8 + level / 3 ]) }));
	tp->add_subdesc(AURA_DESC, ob);
    }
}

void
remove_spell()
{
    object *tar;
    
    if (sizeof(tar = query_targets()))
    {
	tar[0]->catch_tell("Your magic aura fades.\n");
	tell_objects(LISTEN(E(tar[0])) - ({ tar[0] }),
		     ({ "The magic light at ", QTNAME(tar[0]), " fades.\n" }),
		     MSG_SEE, 0);
	tar[0]->add_lwvs(-1, 0, 0, 0);
    }
    destruct_object();
}

int
query_level()
{
    return 3;
}

/*
 * Function name: subdesc
 * Description:   show subdescription
 * Arguments:     sloc - subdesc id
 *                tp - player with subdesc
 *                for_ob - looking object
 * Returns:       description string
 */
nomask string
subdesc(object tp, object for_ob)
{
    if (tp == for_ob)
	return "Your aura spell gives you a powerful appearance.\n";
    if (query_cast_level() > 29)
	return capitalize(tp->query_pronoun()) + POWER_DESC_3 + "\n";
    if (query_cast_level() > 6)
	return capitalize(tp->query_pronoun()) + POWER_DESC_2 + "\n";
    return capitalize(tp->query_pronoun()) + POWER_DESC_1 + "\n";
}

int *
query_spell_data()
{
    return
	({
	    SP_PASSIVE,
	    SP_LEVEL_TIME,
	    120,
	    1,
	    SP_ALL_TARGETS,
	    SK_ML_ASTER,
	});
}
