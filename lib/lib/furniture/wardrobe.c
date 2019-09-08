inherit "/base/container";

#include <event.h>
#include <language.h>
#include <macros.h>
#include <material.h>
#include <properties.h>

static void
create_container()
{
    set_name("wardrobe");
    add_name("closet");
    add_adj("wooden");
    set_short("wooden wardrobe");
    set_pshort("wooden wardrobe");
    set_long_bs("This is a big wardrobe made from planks of brown wood. "+
	     "A door at its front allows to close it.\n");
    set_lwvs(0, 150000, 1250000, 100);
    set_max_weight(300000);
    set_max_volume(1000000);
    set_value(40);
    add_prop(CONT_RIGID, 1);
    add_prop(CONT_CLOSED, 1);
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
    tell_objects(LISTEN( E(tp)) - ({ tp }),
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
    tell_objects(LISTEN( E(tp)) - ({ tp }),
		  ({ QCTNAME(tp), " closes ",
		     QNAME(this_object()), "\n" }),
		  MSG_SEE,
		  tp                                          );
    add_prop(CONT_CLOSED, 1);
    return 1;
}
