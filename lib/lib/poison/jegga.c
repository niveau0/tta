/*
 * jegga poison
 */
inherit "/base/poison";

#include <effects.h>
#include <event.h>
#include <macros.h>
#include <rr_types.h>
#include <properties.h>

#define INTERVAL 5
#define LEVEL    7

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

    tp->catch_tell("Your feel your lungs explode!\n");
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		({ QCTNAME(tp), " snaps for some fresh air.\n" }),
		   MSG_SEE);
    tp->check_death(1+random(100), this_object());
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
    return "_jegga_";
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
    return "Jegga, will hurt your lungs";
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
