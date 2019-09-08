inherit "/base/container";

#include <stdproperties.h>
#include <macros.h>
#include <material.h>
#include <language.h>

/* A version of a chest without a lock.
 * A short form of /lib/misc/container/chest.
 */

static void
create_container()
{
    set_name("chest");
    add_adj("wooden");
    set_short("wooden chest");
    set_pshort("wooden chests");
    set_long("A small wooden chest with a moveable lid on its top.\n");
    set_lwvs(0, 5000, 5000, 40);
    set_max_weight(10000);
    set_max_volume(10000);
    set_value(15);
    add_prop(CONT_RIGID, 1);
    add_prop(CONT_CLOSED, random(2));
    add_prop(OBJ_MATERIAL, M_OAK);
}

/*
 * Function name: short
 * Description:   gives modified short description 
 * Returns:       string
 */
string
short()
{
    if (!query_prop(CONT_CLOSED))
	return "open " + ::short();
    return "closed " + ::short();
}

/*
 * Function name: open_cmd
 * Description:   called if someone tries to open this container
 * Returns:       1 if success, -1 else
 */
int
open_cmd()
{
    object tp;
    
    tp = this_player();

    if (!query_prop(CONT_CLOSED))
    {
	tp->catch_tell("It's already open.\n");
	return -1;
    }
    tp->catch_tell("You open it.\n");
    tell_objects( E(tp)->query_listen() - ({ tp }),
		  ({ QCTNAME(tp),
		      " opens ", QNAME(this_object()), ".\n" }),
		  MSG_SEE,
		  tp
	        );
    add_prop(CONT_CLOSED, 0);
    return 1;
}

/*
 * Function name: close_cmd
 * Description:   called if someone tries to close this container
 * Returns:       1 if success, -1 else
 */
int
close_cmd()
{
    object tp;
    
    tp = this_player();

    if (query_prop(CONT_CLOSED))
    {
	tp->catch_tell("It's already closed.\n");
	return -1;
    }
    tp->catch_tell("You close it.\n");
    tell_objects( E(tp)->query_listen() - ({ tp }),
		  ({ QCTNAME(tp), " closes ",
		     QNAME(this_object()), "\n" }),
		  MSG_SEE,
		  tp                                          );
    add_prop(CONT_CLOSED, 1);
    return 1;
}
