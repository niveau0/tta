inherit "/base/object";

#include <macros.h>
#include <mudtime.h>
#include <properties.h>

static void
create_object()
{
    set_name("tower clock");
    add_name("clock");
    set_short("tower clock");
    set_pshort("tower clocks");
    set_lwvs(0, 100000, 1000000, 36);
    set_value(100000);
    add_prop(OBJ_NO_GET, 1);
    add_prop(OBJ_NO_SHOW, 1);
}


string
long()
{
    if (NIGHT)
	return "You try in vain to make out the hands of the clock. "+
	    "It's too dark.\n";

    return "It has two large hands, pointing at "+
	"numbers that are arranged in a circle.\n"+
	"The hands of the clock tell you:\n" + TIMEDESC;
}
