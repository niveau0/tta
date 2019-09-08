/*
 * upset stomach
 */
inherit "/base/disease";

#include <effects.h>
#include <macros.h>
#include <rr_types.h>
#include <properties.h>

#define INTERVAL 100+random(200)
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
    tp->add_effect(DISEASE_EFFECT, MASTER, count);
    switch (random(5))
    {
      case 0:
	  tp->command("shiver");
	  break;
      case 1:
	  tp->command("fart");
	  tp->check_death(1, this_object());
	  break;
      case 2:
	  tp->command("burp");
	  break;
      case 3:
	  tp->command("moan");
	  tp->check_death(1, this_object());
	  break;
      case 4:
	  tp->command("puke");
	  tp->check_death(2, this_object());
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
    return "_upset_stomach_";
}

nomask string
subdesc(object tp, object for_ob)
{
    if (!infected_players[tp])
	return "";
    if (tp == for_ob)
	return "You have an upset stomach.\n";
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
    return "Upset stomach";
}
