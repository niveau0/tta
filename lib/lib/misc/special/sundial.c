inherit "/base/object";

#include <event.h>
#include <mudtime.h>
#include <macros.h>

static void
create_object()
{
    set_name("sundial");
    add_name("dial");
    add_name("clock");
    add_name("sunclock");
    set_short("sundial");
    set_pshort("sundials");
    set_long("It's a well-crafted little sundial.\n"+
	     "Try to use it to determine the time of day.\n");
    set_lwvs(0,100,50,3);
    set_value(500);
}

int
use_cmd()
{
    object tp;

    tp = this_player();

    if (NIGHT)
    {
	tp->catch_tell("You play with the sundial for while, "+
		       "preparing it to be exposed to the light of\n"+
		       "the sun, but then again, it's a long time"+
		       "until dawn.\n"+
		       "Maybe you'd better try again in a few hours.\n");
	return -1;
    }

    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " holds ", QNAME(this_object()),
		    " into the light.\n" }), MSG_SEE, tp);
    tp->catch_tell("A friendly beam of sunlight shows you "+
		   "the time:\n"+
		   TIMESTRING(TIMEARRAY));
    return 1;
}

