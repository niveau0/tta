/*
 * brithagurth poison
 */
inherit "/base/poison";

#include <effects.h>
#include <event.h>
#include <macros.h>
#include <rr_types.h>
#include <properties.h>
#include <slots.h>
#include <state.h>

#define INTERVAL 2+random(10)
#define LEVEL    2

static mapping brithagurth_slots;

static void
renew_poison(object tp)
{
    tp->catch_tell("You feel hurt by something.\n");
    tp->check_death(random(2)+2, this_object());
}

/*
 * Function name: poison_effect
 * Description:   this must be redefined to create a poison effect
 * Arguments:     tp - the affected player
 */

static nomask void
poison_effect(object tp)
{
    int    sz, i, j, sy;
    int    *arms_to_poison;
    int    *arm_connects;
    mapping wounds;
    
    if (!tp || !(poisoned_players[tp]) || !living(tp))
    	return;

    if (!mappingp(brithagurth_slots))
	brithagurth_slots=([]);
    
    brithagurth_slots[tp] = (tp->query_slot_type(SLT_ARM));

    wounds = tp->query_wounds();

    arms_to_poison = brithagurth_slots[tp];

    if (!(sz = sizeof(arms_to_poison)))
        return;
	
    for (i=0; i<sz; i++)
    {
        if (!(tp->query_wounds()[arms_to_poison[i]]))
        {
	    sy = sizeof(arm_connects =
			tp->query_connections(arms_to_poison[i]));
	    for (j=0; j<sy; j++)
	    {
		if (!(wounds[arm_connects[j]]))
		    brithagurth_slots[tp] += ({ arm_connects[j] });
	    }
            tp->set_wound(arms_to_poison[i], SLOT_UNUSABLE);
        }
    }
    if (!(tp->query_state() & (SLEEPING | UNCONSCIOUS)))
    {
	tp->catch_tell("Suddenly you can not move your limbs anymore.\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		     ({ QCTNAME(tp), "'s limbs seem to stiffen.\n" }),
		     MSG_SEE);
    }
}

/*
 * Function name: query_subdesc_id
 * Description:   gives name of subdesc
 * Returns:       string
 */
nomask string
query_subdesc_id()
{
    return "_brithagurth_";
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
    return "Brithagurth, will cause upper body paralysis.";
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
remove_poison(object tp)
{
    int     i, *slots_to_heal;
    mapping wounds;
    
    ::remove_poison(tp);

    slots_to_heal = brithagurth_slots[tp];
    wounds = tp->query_wounds();
    
    for(i=sizeof(slots_to_heal); i--;)
    {	
	if(wounds[slots_to_heal[i]] == SLOT_UNUSABLE)
	    tp->set_wound(slots_to_heal[i], 0);
    }
    if (living(tp) && !(tp->query_state() & (SLEEPING | UNCONSCIOUS)))
    {
	tp->catch_tell("Your arms feel more relaxed.\n");	
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		     ({ QCTNAME(tp),
			    "'s limbs seem to be more dexterous.\n" }),
		     MSG_SEE);
    }
    brithagurth_slots[tp]=0;
}
