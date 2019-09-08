inherit "../../cha_base";

#include <composite.h>
#include <macros.h>
#include <magic.h>
#include <skills.h>

string
query_desc()
{
    return "Detect traps in range.";
}

void
spell_effect(int level, object *tar, object tp)
{
    if (sizeof(tar))
    {
	int     i, sz, fail, *co, *tar_co;
	string  str, coord;
	object  *traps;

	if (!(sz = sizeof(traps =
			  parse("traps", I(E(tp), 1), 0))))
	{
	    tp->catch_tell("You sense nothing.\n");
	    return;
	}
	fail = 25 - level / 3;
	str  = "";
	co   = tp->query_coordinates();
	for (i=0; i<sz; i++)
	{
	    if (random(100) < fail ||
		!(tar_co = traps[i]->query_coordinates()) ||
		!distance(co, tar_co, 600))
		continue;
	    coord = COORD_SUFFIX(co, tar_co);
	    str += traps[i]->short() + coord + "\n";
	}
	if (str == "")
	{
	    tp->catch_tell("You sense nothing.\n");
	    return;
	}
	tp->catch_tell("You sense the following traps:\n" + str);
    }
}

int
query_level()
{
    return 7;
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
