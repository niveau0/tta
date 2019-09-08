inherit "../../ess_base";

#include <composite.h>
#include <door.h>
#include <event.h>
#include <macros.h>
#include <magic.h>
#include <properties.h>
#include <skills.h>

string
query_desc()
{
    return "Locks every lock caster can see within 100'. Power manipulates " +
	"the number of locks the caster can cause.";
}

int
spell_effect(int level, object *tar, object tp)
{
    int i, j, sz;
    string *open, *closed;
    
    if (!(sz = sizeof(tar)))
	return -1;

    open = ({ });
    closed = ({ });
    
    for (i=0; i<sz; i++)
    {
	
	if (tar[i]->query_key())
	{
	    if (tar[i]->query_prop(CONT_LOCKED))
		continue;
	    if (!tar[i]->query_prop(CONT_CLOSED))
	    {
		open += ({ tar[i] });
		tar[i]->add_prop(CONT_CLOSED, 1);
		tar[i]->add_prop(CONT_LOCKED, 1);
	    }
	    else
	    {
		closed += ({ tar[i] });
		tar[i]->add_prop(CONT_LOCKED, 1);
	    }
	    tar[i]->set_status(DOOR_LOCKED);
	}
    }
    if ((j=sizeof(open)))
    {
	tp->catch_msg(({ "In a second " }) + COMPOSITE_ARRAY_ALL(open) +
	({ " close" + (j==1 ? "s" : "") + ", and lock" + (j==1 ? "s" : "") +
			 " with a *click*.\n" }),
	tp, MSG_SEE); 
    }
    if ((i=sizeof(closed)))
    {
	tp->catch_msg( ({ "You hear a *click* from " }) +
		      COMPOSITE_ARRAY_ALL(closed) + ({ ".\n" }) ,
	tp, MSG_SEE); 
    }
    if (j+i>1)
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		     ({ "You hear some *clicks* as if some locks close.\n" }),
		     MSG_HEAR, tp, 100);
    else
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		     ({ "You hear a *click* as if a locks close.\n" }),
		     MSG_HEAR, tp, 100);
    return 1;
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
	    SP_FORCE,
	    SP_NONE,
	    100,
	    SP_LEVEL_TARGETS,
	    SP_ALL_OBJECTS,
	    SK_ML_CONSCIOUSNESS,
	});
}
