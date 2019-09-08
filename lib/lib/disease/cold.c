/*
 * cold disease
 */
inherit "/base/disease";

#include <effects.h>
#include <macros.h>
#include <rr_types.h>
#include <properties.h>
#include <state.h>

#define INTERVAL 240
#define LEVEL    1

/*
 * Function name: disease_effect
 * Description:   this must be redefined to create a disease effect
 * Arguments:     tp - the affected player
 */
static nomask void
disease_effect(object tp)
{
    int count;
    
    if (!tp || !living(tp) || !infected_players[tp])
	return;
    count = infected_players[tp]++;
    if (count > 20)
    {
	if (tp->resistance_roll(RR_DISEASE, LEVEL, -10) > 0)
	{
	    remove_disease(tp);
	    return;
	}
    }
    call_out("disease_effect", INTERVAL, tp);
    spread_disease(50, tp);

    if (tp->query_state() & (SLEEPING | UNCONSCIOUS))
	return;
    tp->add_effect(DISEASE_EFFECT, MASTER, count);
    switch (random(4))
    {
      case 0:
	tp->command("sneeze");
	break;
      case 1:
	tp->command("cough");
	tp->check_death(1, this_object());
	break;
      case 2:
	tp->command("sniff");
	break;
      case 3:
	tp->command("moan");
	tp->check_death(1, this_object());
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
    return "_cold_";
}

nomask string
subdesc(object tp, object for_ob)
{
    if (!infected_players[tp])
	return "";
    if (tp == for_ob)
	return "You feel ill.\n";
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
    return "bad cold";
}
