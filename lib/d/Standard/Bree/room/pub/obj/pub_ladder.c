inherit "/base/stair";

#include <door.h>
#include <event.h>
#include <material.h>
#include <properties.h>

#define DOMAINPATH "/d/Standard/Bree/"

static void
create_stair()
{
    set_name("ladder");
    add_adj("wooden");
    set_short("ladder");
    set_pshort("ladders");
    set_long_bs("You see an old ladder which has been crafted from wood. "+
	     "It is fixed to the wall.\n");
    add_lwvs(0, 40000, 25000, 65);
    set_value(20);
    add_prop(OBJ_NO_GET, 1);
    add_prop(OBJ_MATERIAL, M_OAK);
    set_connection(DOMAINPATH+"room/pub/back");
    add_event(E_MOVE, "initial_connect", this_object());
}

int *
query_range()
{
    return ({ 20, 6, 65 });
}

void
initial_connect(object obj, object dest, object old)
{
    if (!dest)
	return;    
    if (dest->query_room_file() == (DOMAINPATH+"room/pub/cellar"))
	set_connection(DOMAINPATH+"room/pub/back");
    else
	set_connection(DOMAINPATH+"room/pub/cellar");
}
