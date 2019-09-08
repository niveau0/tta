inherit "/base/curse";

#include <effects.h>
#include <event.h>
#include <living.h>
#include <macros.h>
#include <rr_types.h>
#include <properties.h>
#include <state.h>

#define INTERVAL 0
#define LEVEL    30


static nomask void
curse_effect(object tp)
{
    if (!tp || !(cursed_players[tp]) || !living(tp))
	return;

    if(!map_sizeof(tp->query_effects()[PARALYSE_EFFECT]))
    {
	tp->catch_tell("Suddenly every muscle in your body seems to stiften."+
		       " You can't\neven move a finger.\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		     ({ QCTNAME(tp), " seems to stiften.\n" }), 
		     MSG_SEE);
    } 
    else
	
	tp->catch_tell("For a short moment a shockwave runs through your "+
		       "body.\n");

    tp->add_effect(PARALYSE_EFFECT, EFF_CURSE_PARALYSE, 1);
    tp->add_state(PARALYSED);
}


/*
 * Function name: query_subdesc_id
 * Description:   gives name of subdesc
 * Returns:       string
 */
nomask string
query_subdesc_id()
{
    return "_blind_";
}

/*
 * Function name: query_level
 * Description:   gives curse level
 * Returns:       int
 */
nomask int
query_level()
{
    return LEVEL;
}
     
/*
 * Function name: query_desc
 * Description:   gives curse description
 * Returns:       string
 */
nomask string
query_desc()
{
    return "paralyses the victim";
}

/*
 * Function name: query_interval
 * Description:   gives call_out interval
 * Returns:       int
 */
nomask int
query_interval()
{
    return INTERVAL;
}

void
remove_curse(object tp)
{
    ::remove_curse(tp);

    tp->remove_effect(PARALYSE_EFFECT, EFF_CURSE_PARALYSE);
    if(living(tp))
    {
	if (!map_sizeof(tp->query_effects()[PARALYSE_EFFECT]))
	{
	    tp->catch_tell("Great, you can move again.\n");
	    tell_objects(LISTEN(E(tp)) - ({ tp }),
			 ({ QCTNAME(tp), " seems to be more dexterous.\n" }),
			 MSG_SEE);
	    tp->delete_state(PARALYSED);
	}
	else
	    tp->catch_tell("Something evil has left your body, but you are "+
			   "still paralysed.\n");
    }
    else
	if (!map_sizeof(tp->query_effects()[PARALYSE_EFFECT]))
	    tp->delete_state(PARALYSED);
}
