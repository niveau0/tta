inherit "../../cha_base";

#include <composite.h>
#include <language.h>
#include <macros.h>
#include <magic.h>
#include <skills.h>

string
query_desc()
{
    return "Detect a specified object in range.";
}

void
spell_effect(int level, object *tar, object tp, string extra)
{
    int    i, sz, dist, *co, *tar_co;
    string coord;
    object *obj;

    if (!extra)
    {
        tp->catch_tell("You must specify an object description.\n");
        return;
    }

    if (!sizeof(obj = parse(extra, I(E(tp), 0), tp)) ||
        !(co = obj[0]->query_coordinates()))
    {
        tp->catch_tell("You locate nothing.\n");
        return;
    }
    dist   = 3600 * level;
    tar_co = tp->query_coordinates();
    for (i=0, sz=sizeof(obj); i<sz; i++)
    	if (distance(tar_co, co, dist))
    	    break;
    if (i == sz)
    {
        tp->catch_tell("You locate nothing.\n");
        return;
    }
    coord = COORD_SUFFIX(co, tar_co);
    tp->catch_msg(({ "You locate ", QNAME(obj[0]), coord + ".\n" }), 0, 0);
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
	    SP_PASSIVE,
	    SP_SELF,
	    SP_LEVEL_RANGE,
	    1,
	    SP_ALL_LIVINGS,
	    SK_ML_CONSCIOUSNESS,
	});
}
