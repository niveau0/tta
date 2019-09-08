inherit "/base/stair";

#include <door.h>
#include <event.h>
#include <material.h>
#include <properties.h>

#define DOMAINPATH "/d/Standard/Bree/"

static void
create_stair()
{
    set_name("staircase");
    add_name("stairs");
    add_name("stair");
    add_adj("wooden");
    set_short("wooden staircase");
    set_pshort("wooden staircases");
    set_long_bs("The wooden stairs look old and worn. A railing at its "+
	     "side shall prevent accidental steps off the stair.\n");
    add_lwvs(0, 3200000, 2800000, 100);
    set_value(120);
    add_prop(OBJ_NO_GET, 1);
    add_prop(OBJ_MATERIAL, M_HAZEL);
    set_connection(DOMAINPATH+"room/pub/roof");
    add_event(E_MOVE, "initial_connect", this_object());
}

int *
query_range()
{
    return ({ 40, 80, 100 });
}

void
init_connect(object obj, object dest, object old)
{
    if (!dest)
	return;    
    if (dest->query_room_file() == (DOMAINPATH+"room/pub/back"))
	set_connection(DOMAINPATH+"room/pub/roof");
    else
	set_connection(DOMAINPATH+"room/pub/back");
}
