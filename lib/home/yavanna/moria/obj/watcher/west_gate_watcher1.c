inherit "watcher";

#include <macros.h>
#include <material.h>
#include <mudtime.h>
#include <properties.h>
#include <std.h>
#include "../../include/watcher.h"

#define WATCHER_LONG "A statue made of black basalt. It looks like"+\
"the eye\ninlays are composed of small, polished gems, "+\
"opals maybe.\nA really fine dwarvish craftmanship.\n"

static void
create_watcher()
{
    set_name("statue");
    set_long(WATCHER_LONG);
    set_direction(NORTH);
    set_distance(150);
    set_watcher_type(BOTH);
    add_prop(OBJ_MATERIAL, M_STONE);
    add_lwvs(0, 1000000, 250000, 1000);
    set_receiver(find_player("yavanna"));
}

void
stop_glowing()
{
    if (E(receiver) == E(this_object()))
	call_out("stop_glowing", random(200)+50);
    else
    {
	tell_objects(LISTEN(E(this_object())) - ({ this_object() }),
		     ({ "The eyes of ", QTNAME(this_object()),
			    "stopped glowing.\n" }),
		     MSG_SEE, this_object());
	add_lwvs(-300,0,0,0);
	add_prop("_EYES_STARTED_TO_GLOW",0);
    }
}

varargs void
move_reaction(object ob, object to)
{
    if(!(ob=receiver))
	::move_reaction(ob, to);
    else
    {
	if (query_prop("_EYES_STARTED_TO_GLOW"))
	    return;
	add_prop("_EYES_STARTED_TO_GLOW",1);
	add_lwvs(300,0,0,0);
	tell_objects(LISTEN(E(this_object())) - ({ ob, this_object() }),
		     ({ "As ", QTNAME(ob), " walks past the ",
			    QTNAME(this_object()),
			    ", its eyes start to glow in a red light.\n" }),
		     MSG_SEE, this_object());
	ob->catch_msg( ({"As you walk past ", QTNAME(this_object()) ,
			     ",its eyes start to glow in a red light.\n"}),
		        this_object(), MSG_SEE);
	call_out("stop_glowing", random(200)+50);
    }
    
}



    
    
