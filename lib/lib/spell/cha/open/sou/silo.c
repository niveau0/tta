inherit "../../cha_base";

#include <macros.h>
#include <magic.h>
#include <skills.h>
#include <properties.h>

string
query_desc()
{
    return "Creates a sound barrier around someone. Power manipulates the " +
	"spell duration.";
}

mixed
query_similar_code()
{
    return "_silo";
}

void
spell_effect(int level, object *tar, object tp)
{
    if (sizeof(tar))
    {
	object ob;
	
	tar[0]->catch_msg("Suddenly all noises are gone.\n", 0, MSG_HEAR, 10);

	ob = clone_object(MASTER);
	ob->store_spell(tp, level, tar);
	ob->start_call_out("remove_spell", 600 * level);
	tar[0]->add_spell(ob);
	tar[0]->change_int_prop(LIVE_NOISE_REDUCTION, 100000);
	tar[0]->add_effect(DEAF_EFFECT, EFF_SILO, 1);
    }
}

void
remove_spell()
{
    object *tar;
    
    if (sizeof(tar = query_targets()))
    {
	tar[0]->change_int_prop(LIVE_NOISE_REDUCTION, -100000);
	tar[0]->remove_effect(DEAF_EFFECT, EFF_SILO);
	if (living(tp))
	    if (!map_sizeof(tp->query_effects()[DEAF_EFFECT]))
		tar[0]->catch_msg("You seem to hear again.\n", 0,
				  MSG_HEAR, 10);
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
    return
	({
	    SP_FORCE,
	    SP_LEVEL_TIME,
	    1200,
	    1,
	    SP_ALL_LIVINGS,
	    SK_ML_LOGOS,
	});
}
