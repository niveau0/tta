inherit "../../cha_base";
inherit "/base/misc/listen";

#include <combat.h>
#include <macros.h>
#include <magic.h>
#include <mudtime.h>
#include <skills.h>
#include <event.h>

static int started;

string
query_desc()
{
    return "Bind a magic word or a time delay to the creation of light. " +
	"If the trigger word is spoken or the delay runs out the spells " +
	"target starts to shine. Power manipulation can be used to increase " +
	"the allowed time delay.";
}

mixed
query_similar_code()
{
    return "_light";
}

void
start_spell()
{
    object env, *tar;

    remove_call_out("remove_spell");
    if (sizeof(tar = query_targets()) && query_level() > 1)
    {
	call_out("remove_spell", 1800 * query_level() / TIME_SCALE);
	
	if (living(env = E(tar[0])))
	    env = E(env);
	else
	    tar[0]->catch_tell("You start to shine in light.\n");
	
	tell_objects(LISTEN(env) - ({ tar[0] }),
		     ({ QCTNAME(tar[0]), " starts to shine in light.\n" }),
		     MSG_SEE, tar[0]);
	started = 1;
	stop_listen();
	tar[0]->add_lwvs(1, 0, 0, 0);
    }
}

void
catch_tell(string msg)
{
    if (msg && sscanf(msg, "%*s" + query_data() + "%*s") == 2)
	start_spell();
}

void
spell_effect(int level, object *tar, object tp, string extra)
{
    int    seconds;
    string word;
    object ob;
    
    if (!sizeof(tar))
	return;
    if (extra)
    {
	if (sscanf(extra, "delay %d", seconds))
	{
	    if (seconds > 1700 * level / TIME_SCALE)
	    {
		tp->catch_tell("That spell delay is too long.\n");
		return;
	    }
	}
	else
	    sscanf(extra, "word %s", word);
    }
    ob = clone_object(MASTER);
    ob->start_call_out("remove_spell", 1800 * level / TIME_SCALE);
    tar[0]->add_spell(ob);
    if (seconds > 0)
    {
	ob->store_spell(tp, level, tar);
	ob->start_call_out("start_spell", seconds);
    }
    else if (word && strlen(word))
    {
	ob->store_spell(tp, level, tar, word);
	ob->start_listen(tar[0]);
    }
    else
    {
	ob->store_spell(tp, level, tar);
	ob->start_spell();
    }
}

void
remove_spell()
{
    object env, *tar;
    
    if (started && sizeof(tar = query_targets()))
    {
	if (living(env = E(tar[0])))
	    env = E(env);
	else
	    tar[0]->catch_tell("Your magic light fades.\n");
	tell_objects(LISTEN(env) - ({ tar[0] }),
		     ({ "The magic light at ", QTNAME(tar[0]), " fades.\n" }),
		     MSG_SEE, 0);
	tar[0]->add_lwvs(-1, 0, 0, 0);
    }
    destruct_object();
}

int
query_level()
{
    return 10;
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
	    SP_ALL_TARGETS,
	    SK_ML_ASTER,
	});
}
