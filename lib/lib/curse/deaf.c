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

    if(!(tp->query_state() & (SLEEPING | UNCONSCIOUS)))
    {
	if(!map_sizeof(tp->query_effects()[DEAF_EFFECT]))
	    tp->catch_tell("Suddenly everything is quiet. You can't even " +
			   "hear your own heartbeat.\n");
	else
	    tp->catch_tell("For a short time you hear an evil curse, but " +
			   "then silence surrounds you\nagain.\n");
	
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		     ({ QCTNAME(tp), " rubs "+(tp)->query_possessive()+
			    " ears.\n" }),
		     MSG_SEE);
    }
    tp->add_effect(BLIND_EFFECT, EFF_CURSE_DEAF, 1);
}


/*
 * Function name: query_subdesc_id
 * Description:   gives name of subdesc
 * Returns:       string
 */
nomask string
query_subdesc_id()
{
    return "_deaf_";
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
    return "deafss the victim";
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

    tp->remove_effect(DEAF_EFFECT, EFF_CURSE_DEAF);
    if(living(tp) && !(tp->query_state() & (SLEEPING | UNCONSCIOUS)))
    {
	if (!map_sizeof(tp->query_effects()[DEAF_EFFECT]))
	    tp->catch_tell("Wonderful. You can hear again.\n");
	else
	    tp->catch_tell("Something evil has left your ears but you are "+
			   "still deaf.\n");
    }
}
