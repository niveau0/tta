/*
 *  asgurash poison
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

static mapping asgurash_slots;

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
    int     i, j, k, sz, su;
    int     *slots_to_poison;
    int     *slot_connects;
    mapping wounds;
    
    if (!tp || !(poisoned_players[tp]) || !living(tp))
    	return;

    if (!mappingp(asgurash_slots))
	asgurash_slots=([]);
    
    slots_to_poison = tp->query_slot_type(SLT_ARM) +
		      tp->query_slot_type(SLT_LEG) +
		      tp->query_slot_type(SLT_HAND) +
		      tp->query_slot_type(SLT_FOOT);
    
    if (!(su = sizeof(slots_to_poison)))
        return;

    sz = random(su)+1;
    wounds = tp->query_wounds();
    asgurash_slots[tp] = ({ });

    for (i=sz; i--;)
    {
	k = random(su);
        if (!(tp->query_wounds()[slots_to_poison[k]]))
        {
	    asgurash_slots[tp] += ({ slots_to_poison[k] });
	    slot_connects = tp->query_connections(slots_to_poison[k]);
	    for (j=sizeof(slot_connects); j--;)
	    {
		if (!(wounds[slot_connects[j]]))
		    asgurash_slots[tp] += ({ slot_connects[j] });
	    }
            tp->set_wound(slots_to_poison[k], SLOT_UNUSABLE);
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
    return "_asgurash_";
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
    return "Brithagurth, will cause upper body paralysation.";
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

    slots_to_heal = asgurash_slots[tp];
    wounds = tp->query_wounds();
    for(i=sizeof(slots_to_heal); i--;)
    {	
	if(wounds[slots_to_heal[i]] == SLOT_UNUSABLE)
	    tp->set_wound(slots_to_heal[i], 0);
    }
    if (living(tp))
    {
	if (!(tp->query_state() & (SLEEPING | UNCONSCIOUS)))
	{
	    tp->catch_tell("Your limbs feel more relaxed.\n");	
	    tell_objects(LISTEN(E(tp)) - ({ tp }),
			 ({ QCTNAME(tp),
				"'s limbs seem to be more dexterous.\n" }),
			 MSG_SEE);
	}
    }
    asgurash_slots[tp] = 0;
}
