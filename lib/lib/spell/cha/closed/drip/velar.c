inherit "../../cha_base";

#include <macros.h>
#include <magic.h>
#include <skills.h>
#include <properties.h>
#include <event.h>

static int stopvalue;

string
query_desc()
{
    return "Stops the flow of blood. The target cannot move, otherwise the " +
	"wound begins to bleed again. It must be closed. This helps to " +
	"survive.";
}

void
set_value(int v)
{
    stopvalue = v;
}

void
message(object tp, object *tar)
{
    if (sizeof(tar))
    {
	if (tar[0] == tp)
	    tp->catch_tell("You invoke velar.\n");
	else
	    ::message(tp, tar);
    }
}

void
spell_effect(int level, object *tar, object tp)
{
    if (sizeof(tar))
    {
	int     blood;
	object  ob;
	
	if (!(blood = tar[0]->query_bloodloss()) ||
	    (blood - tar[0]->query_prop(LIVE_BLOODFLOW_STOP)) < 1)
	{
	    if (tar[0] == tp)
		tp->catch_tell("You are not bleeding.\n");
	    else
		tp->catch_tell(capitalize(tar[0]->query_pronoun()) +
			       " is not bleeding.\n");
	    return;
	}
	ob = clone_object(MASTER);
	ob->store_spell(tp, level, tar, tar[0]->query_coordinates()[..]);
	add_event(E_MOVE, "E_move", tar[0]);
	tar[0]->add_spell(ob);
	if (level > 12)
	{
	    tar[0]->change_int_prop(LIVE_BLOODFLOW_STOP, blood);
	    ob->set_value(blood);
	}
	else
	{
	    tar[0]->change_int_prop(LIVE_BLOODFLOW_STOP, level);
	    ob->set_value(level);
	}
    }
}

void
remove_spell()
{
    object *tar;
    
    if (!sizeof(tar = query_targets()))
    {
	destruct_object();
	return;
    }
    tar[0]->change_int_prop(LIVE_BLOODFLOW_STOP, -stopvalue);
    if (tar[0]->query_bloodloss() > tar[0]->query_prop(LIVE_BLOODFLOW_STOP))
	tar[0]->catch_tell("Your wounds start to bleed again.\n");
    destruct_object();
}

nomask void
E_move(object ob, object from, object to)
{
    object  *tar;
    
    if (!sizeof(tar = query_targets()) ||
	(tar[0] == ob && !distance(ob->query_coordinates(), query_data(), 0)))
    {
	remove_spell();
	return;
    }
}

int
query_level()
{
    return 1;
}

int *
query_spell_data()
{
    return
	({
	    SP_HEALING,
	    SP_PERMANENT,
	    SP_TOUCH,
	    1,
	    SP_ALL_LIVINGS,
	    SK_ML_IHYAMA,
	});
}
