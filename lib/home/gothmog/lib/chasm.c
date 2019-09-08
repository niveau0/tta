inherit "/base/stair";

#include <door.h>
#include <event.h>
#include <material.h>
#include <properties.h>

#define DOMAINPATH "/home/gothmog/"

static void
create_stair()
{
    set_name("chasm");
    add_name("protrusion");
    add_name("wall");
    add_adj("slippery");
    set_short("slippery wall");
    set_pshort("slippery walls");
    set_long_bs("At this part of the chasms are a lot of protrusions"+
	     " in the wall. It seems to be climbable.\n");
    add_lwvs(0, 3200000, 2800000, 100);
    set_value(0);
    add_prop(OBJ_NO_GET, 1);
    add_prop(OBJ_MATERIAL, M_STONE);
    set_connection(DOMAINPATH+"home");
    add_event(E_MOVE, "initial_connect", this_object());
}

int *
query_range()
{
    return ({ 120, 40, 400 });
}

void
init_connect(object obj, object dest, object old)
{
    if (!dest)
	return;    
    if (dest->query_room_file() == (DOMAINPATH+"room/chasm"))
	set_connection(DOMAINPATH+"home");
    else
	set_connection(DOMAINPATH+"room/chasm");
}

