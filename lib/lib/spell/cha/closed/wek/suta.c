inherit "../../cha_base";

#include <macros.h>
#include <magic.h>
#include <mudtime.h>
#include <skills.h>
#include <properties.h>

string
query_desc()
{
    return "Stop your need for food and water a whole day. This doesn't " +
	"feed, after the spell wears off you are as hungry or thirsty as " +
	"before.";
}

mixed
query_similar_code()
{
    return "_suta";
}

void
spell_effect(int level, object *tar, object tp)
{
    if (sizeof(tar))
    {
	object ob;
	
	tp->catch_tell("You will now have no need for food or water for one " +
		       "day.\n");
	
	ob = clone_object(MASTER);
	ob->store_spell(tp, level, tar);
	ob->start_call_out("remove_spell", 24 * 3600 / TIME_SCALE);
	tp->add_spell(ob);
	tp->change_int_prop(LIVE_NO_CONSUME, 1);
    }
}

void
remove_spell()
{
    object *tar;
    
    if (sizeof(tar = query_targets()))
    {
	tar[0]->catch_tell("You feel, the sustain spell is gone.\n");
	tar[0]->change_int_prop(LIVE_NO_CONSUME, -1);
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
	    SP_PASSIVE,
	    86400 / TIME_SCALE,
	    SP_SELF,
	    1,
	    SP_ALL_LIVINGS,
	    SK_ML_QADOSH,
	});
}
