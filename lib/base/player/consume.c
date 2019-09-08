#include <config.h>
#include <liquid.h>
#include <macros.h>
#include <mudtime.h>
#include <properties.h>
#include <state.h>
#include "/base/living/prototypes.h"

int hunger,      /* how hungry is the player */
    thirst;      /* how thirsty is the player */

/*
 * Function name: add_food
 * Description:   satisfy the stomach of the player
 * Arguments:     f - how much food to add
 * Returns:       how much of given amount can be eaten
 */
nomask int
add_food(int f)
{
    if (f > 0 && hunger < 1)
	return -1;
    hunger -= f;
    if (hunger < 0)
    {
	f += hunger;
	hunger = 0;
    }
    return f;
}

/*
 * Function name: add_water
 * Description:   satisfy the stomach of the player
 * Arguments:     w - how much water to add
 * Returns:       how much of given amount can be drunken
 */
nomask int
add_water(int w)
{
    if (w > 0 && thirst < 1)
	return -1;
    thirst -= w;
    if (thirst < 0)
    {
	w += thirst;
	thirst = 0;
    }
    return w;
}

/*
 * Function name: query_hunger
 * Description:   how hungry is the player
 * Returs:        integer (higher -> more hunger)
 */
nomask int
query_hunger()
{
    return hunger;
}

/*
 * Function name: query_thirst
 * Description:   how thirsty is the player
 * Returs:        integer (higher -> more thirst)
 */
nomask int
query_thirst()
{
    return thirst;
}

/*
 * Function name: auto_eat
 * Description:   try to find something edable and eat it
 */
nomask void
auto_eat()
{
    int    i;
    string name;
    object env, *obj;
    
    obj = this_object()->deep_inventory();
    for (i=sizeof(obj); i--;)
    {
	if (obj[i]->query_prop(IS_POISON) &&
	    obj[i]->query_analysed(this_object()))
	    continue;
	if (function_object("eat_cmd", obj[i]) &&
	    ((env = E(obj[i])) == this_object() || E(env) == this_object()))
	    break;
    }
    if (i < 0)
	return;
    name = obj[i]->query_pname();
    if (env == this_object() ||
	command("get " + name + " from " + env->query_name()) != -1)
	command("eat " + name);
    else
	this_object()->catch_tell("You tried to get something edable.\n");
}

/*
 * Function name: auto_drink
 * Description:   try to find something drinkable and drink it
 */
nomask void
auto_drink()
{
    int    i, liquid;
    string name;
    object env, *obj;
    
    obj = this_object()->deep_inventory();
    for (i=sizeof(obj); i--;)
    {
	if ((liquid = obj[i]->query_liquid_int()) >= LIQ_POISON_FIRST &&
	    liquid <= LIQ_POISON_LAST &&
	    obj[i]->query_analysed(this_object()))
	    continue;
	if (function_object("drink_cmd", obj[i]) &&
	    obj[i]->query_internal_liquid() > 50 &&
	    ((env = E(obj[i])) == this_object() || E(env) == this_object()))
	    break;
    }
    if (i < 0)
	return;
    name = obj[i]->query_name();
    if (env == this_object() ||
	command("get " + name + " from " + env->query_name()) != -1)
	command("drink from " + name);
    else
	this_object()->catch_tell("You tried to get something to drink.\n");
}

/*
 * Function name: consume
 * Description:   handle consumption of food and water
 */
static nomask void
consume()
{
    if (query_prop(CODER_NO_CONSUME))
	return;
    
    call_out("consume", CONSUME_TIME);
    
    if (query_prop(LIVE_NO_CONSUME))
	return;
    if (E(this_object()) &&
	sscanf(object_name(E(this_object())), OOCAREA + "/%*s"))
	/* no starving problems within 'out of character' areas */
	return;
    
    /*
     * hunger
     */
    switch (hunger)
    {
      case 900..100000: /* after 6 days */
	  catch_tell("Due to lack of food the final curtain now covers your " +
		     "sight.\n");
	  do_die(this_object());
	  break;
      case 750..899: /* after 4 days */
	  hunger += 150 * CONSUME_TIME * TIME_SCALE / 129600 + 1;
	  if (!random(3))
	  {
	      catch_tell("Argh, your body cannot ignore the hunger " +
			 "anymore... your senses fade.\n");
	      add_unconscious(random(20) + 10);
	  }
	  break;
      case 600..749: /* after 2 days */
	  hunger += 150 * CONSUME_TIME * TIME_SCALE / 129600 + 1;
	  if (!random(3))
	      catch_tell("Ugh, sometimes your senses fade, you need " +
			 "something to eat!\n");
	  if (query_prop(LIVE_AUTO_EAT) &&
	      !(query_state() & (SLEEPING | UNCONSCIOUS)))
	      auto_eat();
	  break;
      case 450..599: /* after 12 hours */
	  hunger += 150 * CONSUME_TIME * TIME_SCALE / 14400 + 1;
	  if (!random(5))
	  {
	      if (!random(3))
		  catch_tell("Umm, you better find something to eat, the " +
			     "feeling is terrible.\n");
	      else
		  catch_tell("Your belly growls.\n");
	  }
	  break;
      case 300..449: /* after 8 hours */
	  hunger += 150 * CONSUME_TIME * TIME_SCALE / 14400 + 1;
	  if (!random(5))
	      catch_tell("You get an inconsiderable feeling of hunger.\n");
      case 100..299: /* after 4 hours */
	  hunger += 200 * CONSUME_TIME * TIME_SCALE / 14400 + 1;
	  if (!random(5))
	      catch_tell("You feel some appetite.\n");
      default: 
	  hunger += 100 * CONSUME_TIME * TIME_SCALE / 14400 + 1;
	  break;
    }

    /*
     * thirst
     */
    switch (thirst)
    {
      case 4000..1000000: /* after 5 days */
	  catch_tell("Due to lack of water the final curtain now covers " +
		     "your sight.\n");
	  do_die(this_object());
	  break;
      case 3330..3999: /* after 4 days */
	  thirst += 666 * CONSUME_TIME * TIME_SCALE / 86400 + 1;
	  if (!random(3))
	  {
	      catch_tell("Argh, your body cannot ignore the thirst " +
			 "anymore... your senses fade.\n");
	      add_unconscious(random(20) + 10);
	  }
	  break;
      case 2664..3329: /* after 3 days */
	  thirst += 666 * CONSUME_TIME * TIME_SCALE / 86400 + 1;
	  if(!random(3)) 
	      catch_tell("Ugh, sometimes your senses fade, you need " +
			 "something to drink!\n");
	  if (query_prop(LIVE_AUTO_DRINK) &&
	      !(query_state() & (SLEEPING | UNCONSCIOUS)))
	      auto_drink();
	  break;
      case 1998..2663: /* after 2 days */
	  thirst += 666 * CONSUME_TIME * TIME_SCALE / 86400 + 1;
	  if (!random(4)) 
	      catch_tell("Umm, you better find something to drink, " +
			 "you feel like a prune.\n");
	  break;
      case 1332..1997: /* after 1 day */
	  thirst += 666 * CONSUME_TIME * TIME_SCALE / 86400 + 1;
	  if (!random(5))
	      catch_tell("Your tongue feels like leather, thirst!\n");
	  break;
      case 666..1331: /* after 4 hours */
	  thirst += 666 * CONSUME_TIME * TIME_SCALE / 72000 + 1;
	  if (!random(5)) 
	      catch_tell("You get an inconsiderable feeling of thirst.\n");
	  break;
      default:
	  thirst += 666 * CONSUME_TIME * TIME_SCALE / 14400 + 1;
	  break;
    }
}
