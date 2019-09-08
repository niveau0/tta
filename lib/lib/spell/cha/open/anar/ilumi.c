inherit "../../cha_base";

#include <combat.h>
#include <macros.h>
#include <magic.h>
#include <skills.h>
#include <event.h>

string
query_desc()
{
    return "Create light and bind it on a single object. The light moves " +
	"with the target. Power manipulates the brightness.";
}

mixed
query_similar_code()
{
    return "_light";
}

void
spell_effect(int level, object *tar, object tp)
{
    if (sizeof(tar))
    {
	object ob;
	
	tar[0]->catch_tell("You start to shine in light.\n");
	tell_objects(LISTEN(E(tp)) - ({ tar[0] }),
		     ({ QCTNAME(tar[0]), " starts to shine in light.\n" }),
		     MSG_SEE, tar[0]);

	ob = clone_object(MASTER);
	ob->store_spell(tp, level, tar);
	ob->start_call_out("remove_spell", 600 * level);
	tar[0]->add_spell(ob);
	tar[0]->add_lwvs(level, 0, 0, 0);
    }
}

void
remove_spell()
{
    object *tar;
    
    if (sizeof(tar = query_targets()))
    {
	tar[0]->catch_tell("Your magic light fades.\n");
	tell_objects(LISTEN(E(tar[0])) - ({ tar[0] }),
		     ({ "The magic light at ", QTNAME(tar[0]), " fades.\n" }),
		     MSG_SEE, 0);
	tar[0]->add_lwvs(-query_cast_level(), 0, 0, 0);
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
	    600,
	    1,
	    SP_ALL_TARGETS,
	    SK_ML_ASTER,
	});
}
