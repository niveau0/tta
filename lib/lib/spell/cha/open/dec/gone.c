inherit "../../cha_base";

#include <macros.h>
#include <magic.h>
#include <skills.h>
#include <properties.h>

string
query_desc()
{
    return "See invisible people for some time. Power manipulates the spell " +
	"duration.";
}

void
spell_effect(int level, object *tar, object tp)
{
    if (sizeof(tar))
    {
	object ob;
	
	if (tar[0]->query_prop(LIVE_SEE_INVIS) > 0)
	    return;
	
	tar[0]->catch_tell("Your eyes flicker for a moment.\n");
	ob = clone_object(MASTER);
	ob->store_spell(tp, level, tar);
	ob->start_call_out("remove_spell", 600 * level);
	tar[0]->add_spell(ob);
	tar[0]->change_int_prop(LIVE_SEE_INVIS, 1);
    }
}

void
remove_spell()
{
    object *tar;
    
    if (sizeof(tar = query_targets()))
    {
	tar[0]->catch_tell("Your eyes flicker for a moment.\n");
	if (tar[0]->query_prop(LIVE_SEE_INVIS))
	    tar[0]->change_int_prop(LIVE_SEE_INVIS, -1);
    }
    destruct_object();
}

int
query_level()
{
    return 9;
}

int *
query_spell_data()
{
    return
	({
	    SP_PASSIVE,
	    SP_LEVEL_TIME,
	    600,
	    1,
	    SP_ALL_TARGETS,
	    SK_ML_CONSCIOUSNESS,
	});
}
