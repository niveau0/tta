/*
 * galenaana poison
 */
inherit "/base/poison";

#include <effects.h>
#include <event.h>
#include <macros.h>
#include <mudtime.h>
#include <rr_types.h>
#include <properties.h>
#include <state.h>

#define INTERVAL 5
#define LEVEL    9

/*
 * Function name: poison_effect
 * Description:   this must be redefined to create a poison effect
 * Arguments:     tp - the affected player
 */
static nomask void
poison_effect(object tp)
{
    if (!tp || !(poisoned_players[tp]) || !living(tp))
	return;
    
    if (tp->id("elf"))
    {
	tp->catch_tell("You feel something evil that was especially "+
		       "made for your race\npoisons your body. You "+
		       "feel your lungs explode in a second and\n"+
		       "everything gets dark.\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		     ({ QCTNAME(tp), " suddenly stops breathing."+
			    " It seems that "+(tp)->query_pronoun()+
			    " thinks about something and forgot it."+
			    " You know that nothing could remember "+
			    (tp)->query_objective()+" to breath again.\n" }),
		     MSG_SEE);
	tp->do_die();
    }
    else
    {
	if (!(tp->query_state() & (SLEEPING | UNCONSCIOUS)))
	{
	    tp->catch_tell("You feel that this fate isn't yours, "+
			   "but anyhow you feel something\nwrong with you. "+
			   "Breathing is very hard and ...\n");
	    tell_objects(LISTEN(E(tp)) - ({ tp }),
			 ({ QCTNAME(tp), " looks irritated around and begins "+
				"to cough. Something seems to block "+
				(tp)->query_possessive()+
				" breathing.\n"}), MSG_SEE);
	}
	tp->add_unconscious((1 + random(10)) * 86400 / TIME_SCALE );
    }
    remove_poison(tp);
}

/*
 * Function name: query_subdesc_id
 * Description:   gives name of subdesc
 * Returns:       string
 */
nomask string
query_subdesc_id()
{
    return "_galenaana_";
}

/*
 * Function name: query_level
 * Description:   gives poison level
 * Returns:       int
 */
nomask int
query_level()
{
    return LEVEL;
}
     
/*
 * Function name: query_desc
 * Description:   gives poison description
 * Returns:       string
 */
nomask string
query_desc()
{
    return "Galenaana, it kills elves and leaves others in coma";
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
