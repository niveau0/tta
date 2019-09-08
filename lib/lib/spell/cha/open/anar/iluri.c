inherit "../../cha_base";

#include <composite.h>
#include <macros.h>
#include <magic.h>
#include <skills.h>
#include <event.h>


string
query_desc()
{
    return "Create light and bind it on some objects. The light moves " +
	"with the targets. Power manipulates the possible number of " +
	"targets.";
}

mixed
query_similar_code()
{
    return "_light";
}

void
message(object tp, object *tar)
{
    tp->catch_tell("You invoke a strong light spell.\n");
}

void
spell_effect(int level, object *tar, object tp)
{
    int    i, sz;
    object ob;
    
    if (sz = sizeof(tar))
    {
	ob = clone_object(MASTER);
	ob->store_spell(tp, level, tar);
	ob->start_call_out("remove_spell", 600 * level);
	
	for (i=0, sz=sizeof(tar); i<sz; i++)
	{
	    tar[i]->add_spell(ob);
	    tar[i]->add_lwvs(3, 0, 0, 0);
	}
	tell_objects(tar, "You start to shine in light.\n", 0);
	tell_objects(LISTEN(E(tp)) - tar,
		     ({ "Suddenly " }) + COMPOSITE_ARRAY(tar) +
		     ({ (sizeof(tar) == 1 ? " starts" : " start") +
				      " to shine in light.\n" }), MSG_SEE, 0);
    }
}

void
remove_spell(object t)
{
    int    i, sz;
    object *tar;

    if (!(sz = sizeof(tar = query_targets())))
    {
	destruct_object();
	return;
    }
    if (!t)
    {
	object ob, *listen;
	
	for (i=0; i<sz; i++)
	{
	    ob = tar[i];
	    ob->add_lwvs(-3, 0, 0, 0);
	    ob->catch_tell("Your magic light fades.\n");
	    tell_objects(LISTEN(E(ob)) - ({ ob }),
			 ({ "The magic light at ", QTNAME(ob),
			    " fades.\n" }), MSG_SEE, 0);
	}
	destruct_object();
	return;
    }
    if (!IN_ARRAY(t, tar))
	return;
    t->add_lwvs(-3, 0, 0, 0);
    t->catch_tell("Your magic light fades.\n");
    tell_objects(LISTEN(E(t)) - ({ t }),
		 ({ "The magic light at ", QTNAME(t), " fades.\n" }),
		 MSG_SEE, 0);
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
    return 18;
}

int *
query_spell_data()
{
    return
	({
	    SP_FORCE,
	    SP_LEVEL_TIME,
	    600,
	    SP_LEVEL_TARGETS,
	    SP_ALL_TARGETS,
	    SK_ML_ASTER,
	});
}
