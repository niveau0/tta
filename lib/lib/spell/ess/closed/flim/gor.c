inherit "../../ess_base";

#include <combat.h>
#include <composite.h>
#include <event.h>
#include <magic.h>
#include <macros.h>
#include <skills.h>
#include <state.h>
#include <properties.h>

string
query_desc()
{
    return "Makes one or more objects invisible. All objects become at once " +
	"visible again, when it is struck by a blow or makes a blow. Power " +
	"manipulates the number of objects that can be made invisible.";
}

mixed
query_similar_code()
{
    return "_covra"; // AENDERN !
}

void
become_visible(object *obj)
{
    int i, j, sz;
    object env;

    
    if (!(sz = sizeof(obj)))
	return;
    for (i=sz; i--;)
    {
	if ((j=obj[i]->query_prop(OBJ_INVIS)) < 1)
	    return;
	
	obj[i]->change_int_prop(OBJ_INVIS, -1);
 	
	if (j>0)
	    return;
	if (living(obj[i]))
	    obj[i]->catch_tell("You suddenly feel very real again.\n");
	env = E(obj[i]);
	if (living(env))
	    env->catch_tell("Your "+obj[i]->short()+" gets visible.\n");
	else
	{
	    tell_objects(LISTEN(env) - ({ obj[i] }),
			 ({ "Suddenly ", QNAME(obj[i]), " gets visible.\n" }),
			 MSG_SEE);
	}
    }
}


void
spell_effect(int level, object *tar, object tp)
{
    int i, sz;
    object ob, *listen;
    
    listen=LISTEN(E(tp));
    if (sz = sizeof(listen))
    {
	for (i=sz; i--; )
	    listen[i]->catch_msg(({ "Right before you " }) +
				 COMPOSITE_ARRAY(tar) +
				 ({ " fade" + (sizeof(tar)!=1 ? "" : "s") +
					" away from view.\n" }),
				 tp, MSG_SEE);
    }	    
    if (sz = sizeof(tar))
    {
	ob = clone_object(MASTER);
	
	for (i=0; i<sz; i++)
	{
	    tar[i]->add_spell(ob);
	    tar[i]->change_int_prop(OBJ_INVIS, 1);
	}
	ob->store_spell(tp, level, tar);
	ob->start_call_out("remove_spell", 30);
    }
}

void
remove_spell(object t)
{
    object *tar;

    if (!(sizeof(tar = query_targets())))
    {
	destruct_object();
	return;
    }
    if (!t)
    {
	become_visible(tar);
	destruct_object();
	return;
    }
    if (!IN_ARRAY(t, tar))
	return;
    
    become_visible( ({ t }));
    tar -= ({ t });
    if (sizeof(tar))
    {
	store_spell(query_caster(), query_cast_level(), tar);
	return;
    }
    destruct_object();
}

int
query_no_targets(int level)
{
    int i;

    i = level*2/3;
    if (i<1)
	i = 1;
    return i;
}

int
query_level()
{
    return 1;
}

int *
query_spell_data()
{
    return ({
	SP_PASSIVE,
	SP_LEVEL_TIME,
	10,
	SP_LEVEL_TARGETS,
	SP_ALL_TARGETS,
	SK_ML_PETRA,
	});
}
