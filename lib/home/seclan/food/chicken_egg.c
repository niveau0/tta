inherit "/base/object";

#include <stdproperties.h>
#include <material.h>
#include <language.h>
#include <macros.h>
#include <mudtime.h>
#include <slots.h>

#define SHELL_PATH "/lib/misc/container/chicken_eggshell"
#define CHICK_PATH "/home/seclan/incomplete/chick"

/* This egg will always hatch after some time. For nonhatching eggs
 * look in /lib/food/. These eggs DON'T build heaps!
 * Possible ToDos:
 * breaking upon hard strikes,
 */

nomask void set_amount(int a);

private static int amount;


/*
 * Function name: set_amount
 * Description:   the 'feeding', nourishing value of this food
 * Arguments:     a - the amount
 */
nomask void
set_amount(int a)
{
    amount = a;
}

/*
 * Function name: query_amount
 * Description:   gives the feeding value, see set_amount
 * Returns:       integer
 */
nomask int
query_amount()
{
    return amount;
}

static void
create_object()
{
    set_name("egg");  
    add_adj("chicken");
    add_adj("brown");
    set_short("egg");
    set_pshort("eggs");
    set_long("A chicken egg with a brown shell.\n");
    set_value(3);
    set_amount(30);  /* food value */
    add_lwvs(0, 35, 40, 3);
    add_prop(OBJ_MATERIAL, M_FOOD);
    
    if (IS_CLONE)/* prevents RELOAD from producing eggshells
		  * in the nowhere.*/
    {
	call_out("hatching",
		 (int)((691200+random(345600))/TIME_SCALE),
		 /* needs about 8 (+/- 4) days to hatch */
		 this_object() );
    }
}

/* Function name: hatching
 * Description:   Puts a chicken into the current location and two
 *                shells into hand, container or room, whereever the egg lies.
 * Returns:       
 */
static void
hatching(object egg )
{
    object eggshell, chick, egg_env;

    egg_env = E(egg);
    if (!egg)
	return;
    else
    {
	if ( !(egg_env->query_room_file()) )
	{/* Egg lies in a non-room. */
	    if ( living(egg_env) )
	    {/* Egg is held by a living. */
		tell_objects(egg_env->query_listen() + ({ egg_env }),
			     ({ "The egg breaks and the hatched chick "+
				    "falls down.\n" }),
			     MSG_SEE, egg);
		eggshell = clone_object(SHELL_PATH);
		egg_env->add_tool( eggshell, SLT_HAND+1 );
		eggshell = clone_object(SHELL_PATH);
		egg_env->add_tool( eggshell, SLT_HAND+1 );
		chick = E(egg_env)->clone_here(CHICK_PATH);
		chick->set_coordinates( egg_env->query_coordinates() );
	    }
	    else
	    {/* Environment is neither room nor living
	      * -> It is a container(?)->egg gets foul, only shells remain
	      */
		eggshell = clone_object(SHELL_PATH);
		eggshell->move(egg_env);
		eggshell = clone_object(SHELL_PATH);
		eggshell->move(egg_env);
	    }
	}
	else
	{/* Egg lies in a room. */
	    tell_objects(egg_env->query_listen(),
		     ({ "The egg breaks and a chick hatches.\n" }),
			 MSG_SEE, egg);
	    eggshell = egg_env->clone_here(SHELL_PATH);
	    eggshell->set_coordinates( egg->query_coordinates() );
	    eggshell = egg_env->clone_here(SHELL_PATH);
	    eggshell->set_coordinates( egg->query_coordinates() );
	    chick = egg_env->clone_here(CHICK_PATH);
	    chick->set_coordinates( egg->query_coordinates() );
	}
	remove_object(egg);
    }
}

int
eat_cmd()
{
    object tp, to;

    tp = this_player();
    to = this_object();
    tp->add_food(query_amount());
    tp->catch_tell("You eat one " + short() + ".\n");
    /*Meldung ueberarbeiten*/
    tell_objects(E(tp)->query_listen() - ({ tp }),
		     ({ QCTNAME(tp), " eats ", QNAME(to),
			".\n" }), MSG_SEE, tp);
    return 1;
}
