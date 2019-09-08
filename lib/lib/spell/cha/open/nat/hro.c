inherit "../../cha_base";

#include <macros.h>
#include <magic.h>
#include <skills.h>
#include <properties.h>
#include <event.h>

string
query_desc()
{
    return "Increase the growing speed of a plant.";
}

void
spell_effect(int level, object *tar, object tp)
{
    object ob;
    
    if (sizeof(tar))
    {
	if (tar[0]->query_prop(IS_PLANT))
	{
	    ob = clone_object(MASTER);
	    ob->store_spell(tp, level, tar);
	    ob->start_call_out("do_growing", 60);
	    tar[0]->add_spell(ob);
	}
	else
	    tp->catch_tell("This kind of magic works only for plants.\n");
    }
}

void
do_growing()
{
    object env, *tar;
    
    if (sizeof(tar = query_targets()))
    {
	if (tar[0]->do_grow(1))
	{
	    if (living(env = E(tar[0])))
		env = E(env);
	    tell_objects(LISTEN(env),
			 ({ QCTNAME(tar[0]), " has grown.\n" }),
			 MSG_SEE, tar[0]);
	}
    }
    destruct_object();
}

void
remove_spell()
{
    destruct_object();
}

int
query_level()
{
    return 6;
}

int *
query_spell_data()
{
    return
	({
	    SP_PASSIVE,
	    60,
	    120,
	    1,
	    SP_ALL_OBJECTS,
	    SK_ML_XZULOU,
	});
}
