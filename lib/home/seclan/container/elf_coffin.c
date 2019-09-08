inherit "/base/container";

#include <stdproperties.h>
#include <macros.h>
#include <language.h>
#include <race.h>
 

static void
create_container()
{
    float average_size, volume, weight;

    average_size = ( (float)ELF_MIN + (float)ELF_MAX ) / 2.0;
    volume = (float)((average_size/4.0)*(average_size/7.0)*(average_size+3.0)
	*2.5*2.5*2.5);
    weight = volume / 8.0;

    set_name("coffin");
    add_adj("wooden");
    set_short("wooden coffin (elven size)");
    set_pshort("wooden coffins(elven size)");
    set_long("A wooden coffin with the fitting lid to close it."+
	     " This coffin is obviously a cheap piece of work, "+
	     "but due to seldom complains this type is seen quite often.\n");
    add_prop(CONT_RIGID, 1);
    add_prop(CONT_CLOSED, 1);
    set_lwvs(0, (int)weight, (int)volume, (int)average_size+3);
    set_max_weight((int)weight*4);
    set_max_volume((int)volume+100);
    set_value(20);
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
