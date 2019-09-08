/*
 * klytun poison
 */
inherit "/base/poison";

#include <effects.h>
#include <event.h>
#include <macros.h>
#include <mudtime.h>
#include <rr_types.h>
#include <properties.h>
#include <state.h>

#define INTERVAL random(5)
#define LEVEL    5


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

    if (!(tp->query_state() & (SLEEPING | UNCONSCIOUS)))
    {
	tp->catch_tell("Everything seems to become unreal. You think "+
		       "about your fate,\nwhen everything gets dark.\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		     ({ QCTNAME(tp), " stumbles a bit, looks dizzy and " +
			    "seems to leave the world.\n"}),
		     MSG_SEE);
    }
    tp->add_unconscious((1 + random(10)) * 86400 / TIME_SCALE );
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
    return "_klytun_";
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
    return "Klytun, it will leave you in coma";
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
