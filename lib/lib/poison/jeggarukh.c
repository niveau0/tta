/*
 * jeggarukh poison
 */
inherit "/base/poison";

#include <effects.h>
#include <event.h>
#include <macros.h>
#include <rr_types.h>
#include <properties.h>

#define INTERVAL 3
#define LEVEL    6

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

    tp->catch_tell("Ouch, you suddenly feel knocked out!\n");
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		({ QCTNAME(tp), " suddenly writhes from pain.\n" }),
		   MSG_SEE);
    tp->check_death(10 + random(90), this_object());
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
    return "_jeggarukh_";
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
    return "Jeggarukh, will weaken your heart";
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
