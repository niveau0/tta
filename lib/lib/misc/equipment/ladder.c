inherit "/base/stair";

#include <event.h>
#include <macros.h>
#include <material.h>
#include <math.h>
#include <properties.h>

#define LENGTH 120 // in inches

void
create_stair()
{
    set_name("ladder");
    set_pname("ladders");
    set_short("ladder");
    set_pshort("ladders");
    set_long_bs("Two wooden ladders have been connected in a way that allows to "+
	     "set them up in a securely standing angle. The height is about "+
    	     MATH_INCHES_TO_STRING(LENGTH)+".\n");
    set_lwvs(0, 7500, 10000, LENGTH);
    set_value(3);
    add_prop(OBJ_MATERIAL, M_OAK);
    set_connection("");
    add_event(E_MOVE, "set_up", this_object());
}

int *
query_range()
{
    return ({ 70, 70, LENGTH });
}

void
set_up()
{
    object env;
    int * coord;

    env = E(this_object());
    if ( env->query_prop(IS_ROOM) )
	 {// ladder was dropped into a room
	     if (env->query_height() <= LENGTH)
	     {// ladder reches ceiling->look for exit
		 coord = this_object()->query_coordinates();
		 coord[2] = LENGTH+10; // +10 to be sure the exit is passed
		 set_connection(env->search_exit('u', coord)
				->query_room_file() );
	     }
	 }
    else
	set_connection("");
}
