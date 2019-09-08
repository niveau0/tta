inherit "/base/curse";

#include <effects.h>
#include <event.h>
#include <macros.h>
#include <properties.h>
#include <rr_types.h>
#include <slots.h>
#include <state.h>

#define INTERVAL 0
#define LEVEL    30

nomask int
E_wield(object tp, object *weapon)
{
    tp->catch_tell("You shiver when you think of wielding one of these "+
		   "bloodthirsty things.\n");
    return 1;
}

static nomask void
curse_effect(object tp)
{
    int i, sz;
    object *weapons;
    
    if (!tp || !(cursed_players[tp]) || !living(tp))
	return;

    add_event(E_WIELD, "E_wield", tp);

    if (tp->query_state() & (SLEEPING | UNCONSCIOUS))
	return;
    
    if (!(sz = sizeof((weapons = tp->query_wield(SLT_ALL)))))
    {
	tp->catch_tell("You feel very pacifistic and will never ever wield "+
		       "a weapon anymore!\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		     ({ QCTNAME(tp), " looks confused on " +
			    tp->query_possessive() + " hands.\n" }),
		     MSG_SEE);
    }
    else 
    {
	for(i=sz; i--;)
	    tp->remove_tool(weapons[i], 1);

	tp->catch_tell("You unwield your weapon" + (sz==1 ? "" : "s") +
		       ". You have the feeling " +
		       "that you won't ever wield\none of these bloodthirsty "+
		       "things anymore.\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		     ({ QCTNAME(tp), " unwields " +
			    tp->query_possessive() + " weapon" +
			    (sz==1 ? "" : "s" ) +
			    " and looks with a mixture of disgust and hate "+
			    "on " + (sz==1 ? "it" : "them") + ".\n" }),
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
    return "_no_weapon_";
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
    return "Forces you to never ever wield a weapon anymore";
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

    remove_event(E_WIELD, "E_wield", tp);

    if (living(tp))
	tp->catch_tell("You feel in need of a weapon in your hands now.\n");
}
