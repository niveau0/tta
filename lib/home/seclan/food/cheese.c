inherit "/base/object";

#include <language.h>
#include <stdproperties.h>
#include <macros.h>
#include <material.h>
#include <slots.h>

/* Besonderheit: Es wird nicht der ganze Kaese gegessen, sondern nur
 * bis man satt ist. Der Rest bleibt zurueck. Volumen und Gewicht sinken
 * entsprechend. (Sollte hier Sinn machen, da der Kaese sehr nahrhaft ist.)
 * Nachteil: Noch keine Zeit fuer's Essen noetig
 */

#define MAX_AMOUNT 2000
#define MAX_WEIGHT 6000
#define MAX_VOLUME 4300
#define MAX_SIZE 10

nomask void set_amount(int a);

private static int amount;

nomask void
set_amount(int a)
{
    amount = a;
}

nomask int
query_amount()
{
    return amount;
}

static void
create_object()
{
    set_name("cheese");
    add_adj("yellow");
    set_short("nutritious cheese");
    set_pshort("nutritious cheeses");
    set_long("A round of cheese with a cover of yellow wax. "+
	     "According to its weight it should be very nourishing. ");
    set_value(30);
    set_amount(MAX_AMOUNT);
    add_lwvs(0, MAX_WEIGHT, MAX_VOLUME, MAX_SIZE);
    add_prop(OBJ_MATERIAL, M_FOOD);
}


int
eat_cmd()
{
    object tp, to, cheesepart;
    int hunger, remaining, quarters;

    tp = this_player();
    to = this_object();
    remaining = query_amount();
    hunger = tp->query_hunger();
    if (amount == -1)
	return 0;
    if (hunger >= remaining)
    {
	tp->add_food(remaining);
	tp->catch_tell("You eat from the " + short() + ".\n");
	tell_objects(E(tp)->query_listen() - ({ tp }),
		     ({ QCTNAME(tp), " eats from ", QNAME(this_object()),
			    ".\n" }), MSG_SEE, tp);
	remove_object(to);
	return 1;
    }
    else
    {
	cheesepart = clone_object(program_name(to));
	to->set_amount( remaining - hunger );
	quarters = (int)((remaining - hunger)/(MAX_AMOUNT/4));
	cheesepart->set_amount( hunger );
	to->set_lwvs(0,
		     (int)((MAX_WEIGHT/4) * quarters),
		     (int)((MAX_VOLUME/4) * quarters),
		     (int)((MAX_SIZE/4) * quarters) );
	cheesepart->move(E(tp));
	tp->add_tool(cheesepart, (tp->query_hold_slot(to)) );
        tp->catch_tell("You separate a part from the cheese and start "+
		       "to eat it.\n");
	return ( cheesepart->eat_cmd() );
    }
}


/* File: /base/object.c
 * Function name: long
 * Description:   Describe the object
 * Returns:       string, long description
 */

string
long()
{
    object to;
    int remaining;
    
    to = this_object();
    remaining = to->query_amount();
    switch( (int)(remaining/(MAX_AMOUNT/4)) )
    {
      case 0:
	  return ::long() + "Less then a quarter is left of it.\n";
	  break;

      case 1:
	  return ::long() + "About a quarter is left of it.\n";
	  break;

      case 2:
	  return ::long() + "About the half of it is left.\n";
	  break;

      case 3:
	  return ::long() + "More than three quarters of the round still "+
	      "exist.\n";
	  break;

      default:
	  return ::long() + "\n";
	  break;
    }
}
