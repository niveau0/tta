inherit "../../cha_base";

#include <macros.h>
#include <magic.h>
#include <skills.h>

string
query_desc()
{
    return "Count all livings in range, also invisible and hidden.";
}

/*
 * life
 * counts all life forms within spell range
 */

void
spell_effect(int level, object *tar, object tp)
{
    int    i, sz, c, *co;
    object *obj;
    
    obj = I(E(tp), 2);
    co  = tp->query_coordinates();
    for (i=0, sz=sizeof(obj); i<sz; i++)
    {
	if (distance(co, obj[i]->query_coordinates(), 600))
	    c++;
    }
    tp->catch_tell("You sense " + c + " lifeforms.\n");
}

int
query_level()
{
    return 4;
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
