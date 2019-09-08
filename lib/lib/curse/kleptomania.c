inherit "/base/curse";

#include <effects.h>
#include <living.h>
#include <macros.h>
#include <properties.h>
#include <rr_types.h>
#include <slots.h>
#include <state.h>

#define INTERVAL random(200)+50
#define LEVEL    30

static nomask void
curse_effect(object tp)
{
    int    i, step, sz;
    int    *co, *range;
    object victim, stealing;
    object *seen_players, *to_steal;
    
    if (!tp || !(cursed_players[tp]) || !living(tp))
	return;

    call_out("curse_effect", INTERVAL, tp);

    if(tp->query_state() & (SLEEPING | UNCONSCIOUS))
	return;
    
    if (random(100)+40 < tp->query_stat(STAT_SD))
	return;
    
    seen_players = tp->visible(I(E(tp), 2) - ({ tp }));

    co = tp->query_coordinates()[..];
    step = tp->query_step();
    co[0] -= step;
    co[1] -= step;
    co[2] -= step;
    range = ({ step*2, step*2, step*2 });
    
    for (i=sizeof(seen_players); i--;)
    {
	if (!in_range(seen_players[i]->query_coordinates(), co, range))
	    seen_players[i] = 0;
    }
    seen_players -= ({ 0 });
    if (!(sz=sizeof(seen_players)))
	return;
    
    victim = seen_players[random(sz)];
    if (!(sz = sizeof(to_steal = victim->visible(I(victim, 0)))))
    {
	tp->catch_msg(({ "You try to steal something from ",
			     QTNAME(victim),
			     ", but there is nothing valuable.\n" }), 0, 0);
	return;
    }

    for(i=sz; i--;)
    {
	if (!to_steal[i]->query_prop(IS_CONT))
	    to_steal[i] = 0;
    }
    to_steal = tp->visible(to_steal - ({ 0 }));
    if (!(sz = sizeof(to_steal)))
	return;
    stealing = to_steal[random(sz)];
    
    if (IN_ARRAY(stealing, victim->query_worn(SLT_ALL)))
    {
	if (!random(2))
	{
	    tp->catch_msg(({ "You suddenly decide to steal something from ",
				 QTNAME(victim), ".\n" }), 0, 0);
	    tp->command("steal " + stealing->query_name() + " of " +
			victim->query_name());
	    return;
	}
    }
    tp->catch_msg(({ "You suddenly decide to steal something from ",
			 QTNAME(victim), ".\n" }), 0, 0);
    if (!sizeof(I(stealing, 0)))
    {
	tp->catch_tell("While trying to empty " +
		       victim->query_possessive() + " " +
		       stealing->short() + ", you discover that "+
		       "there's nothing inside.\n");
	return;
    }	
    tp->catch_tell("You examine " + victim->query_possessive() +
		   " " + stealing->short() + ".\n");
    tp->command("steal from " + stealing->query_name() + " of " +
		victim->query_name());
}


/*
 * Function name: query_subdesc_id
 * Description:   gives name of subdesc
 * Returns:       string
 */
nomask string
query_subdesc_id()
{
    return "_kleptomania_";
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
    return "Forces you to steal from nearby persons";
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

    if (living(tp))
	tp->catch_tell("You feel some control of yourself is back.\n");
}
