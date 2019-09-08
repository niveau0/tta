inherit "../../cha_base";

#include <macros.h>
#include <magic.h>
#include <skills.h>
#include <properties.h>

string
query_desc()
{
    return "Detect all undead in range.";
}

void
spell_effect(int level, object *tar, object tp)
{
    int    i, sz, c, *co;
    object *obj;

    obj = I(E(tp), 2);
    co  = tp->query_coordinates();
    for (i=0, sz=sizeof(obj); i<sz; i++)
    {
	if (obj[i]->query_prop(LIVE_UNDEAD) &&
	    distance(co, obj[i]->query_coordinates(), 600))
	    c++;
    }
    tp->catch_tell("You sense " + c + " undead creatures.\n");
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
	    SP_NONE,
	    SP_SELF,
	    1,
	    SP_ALL_LIVINGS,
	    SK_ML_CONSCIOUSNESS,
	});
}
