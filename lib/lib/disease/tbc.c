/*
 * tbc disease
 */
inherit "/base/disease";

#include <effects.h>
#include <event.h>
#include <macros.h>
#include <rr_types.h>
#include <properties.h>

#define INTERVAL random(200)+150
#define LEVEL    137

/*
 * Function name: disease_effect
 * Description:   this must be redefined to create a disease effect
 * Arguments:     tp - the affected player
 */
static nomask void
disease_effect(object tp)
{
    int count;
    
    if (!tp || !infected_players[tp] || !living(tp))
	return;

    count = infected_players[tp]++;
    
    if (count > 10)
    {
	if (tp->resistance_roll(RR_DISEASE, LEVEL, -50) > 0)
	{
	    remove_disease(tp);
	    return;
	}
    }
    call_out("disease_effect", INTERVAL, tp);
    spread_disease(75, tp);
    tp->add_effect(DISEASE_EFFECT, MASTER, count);
    switch (count)
    {
      case 0..20:
	  tp->command("cough");
	  break;
      case 21..40:
	  tp->catch_tell("You cough heavily.\n");
	  tell_objects(LISTEN(E(tp)) - ({ tp }),
		       ({ QCTNAME(tp), " coughs heavily.\n" }),
		       MSG_SEE);
	  tp->check_death(random(2)+1, this_object());
	  break;
      case 41..50:
	  tp->catch_tell("You cough and spit some blood.\n");
	  tell_objects(LISTEN(E(tp)) - ({ tp }),
		       ({ QCTNAME(tp), " coughs and spits some blood.\n" }),
		       MSG_SEE);
	  tp->check_death(random(5)+2, this_object());
	  break;
      case 51..55:
	  tp->catch_tell("You begin to cough and can't even think of "+
			 "anything else than the\npain in your lungs.\n");
	  tell_objects(LISTEN(E(tp)) - ({ tp }),
		       ({ QCTNAME(tp),
			      " coughs heavily and writhes from pain.\n" }),
		       MSG_SEE);
	  tp->check_death(random(10)+3, this_object());
	  break;
      default:
	  tp->catch_tell("With a last heavy cough your lungs deny to "+
			 "work anymore.\n");
	  tell_objects(LISTEN(E(tp)) - ({ tp }),
		       ({ QCTNAME(tp), " coughs a last time.\n" }),
		       MSG_SEE);
	  tp->do_die();
	  break;
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
    return "_tbc_";
}

nomask string
subdesc(object tp, object for_ob)
{
    if (!infected_players[tp])
	return "";
    if (tp == for_ob)
	return "Your breathing aches.\n";
    return capitalize(tp->query_pronoun()) + " looks a bit pale.\n";
}

/*
 * Function name: query_level
 * Description:   gives disease level
 * Returns:       int
 */
nomask int
query_level()
{
    return LEVEL;
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

/*
 * Function name: query_desc
 * Description:   gives disease description
 * Returns:       string
 */
nomask string
query_desc()
{
    return "Tuberculosis - it destroys your lungs";
}
  
void
remove_disease(object tp)
{
    ::remove_disease(tp);

    if (living(tp))
    {
	tp->catch_tell("You can breath more easily now.\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		    ({ QCTNAME(tp)," breathes more easily now.\n" }),
		    MSG_SEE);
    }
}
