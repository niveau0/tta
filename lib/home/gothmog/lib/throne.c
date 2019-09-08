inherit "/base/seat";

#include <material.h>
#include <properties.h>
#include <macros.h>
#include <event.h>
#include <state.h>
#include <living.h>

static object thisp;

static void
create_seat()
{
    set_name("throne");
    set_pname("thrones");
    set_short("throne");
    set_pshort("thrones");
    set_long("This throne is huge, black and emiting an aura of darkness.\nRays of silver flow through the stone, pulsating like veins.\nIt seems the throne is alive, better not touch him!");
    set_lwvs(10, 1000000, 20000, 20);
    set_free_seats(1);
    set_value(1);
    add_prop(OBJ_MATERIAL, M_EOG);
    add_prop(OBJ_NO_GET, 1);   
}



/*
 * Function name: sit_cmd
 * Description:   Called by cmdsoul, let someone sit down on the seat
 * Returns:       1 success, -1 else
 */
int
sit_cmd()
{
    object tp;
    int     x;
    
    tp = this_player();
    if (sizeof(used_by) >= free) 
    {
	tp->catch_msg(({ QCTNAME(this_object()), " is fully occupied.\n" }),
		      0, 0);
	return -1;
    }
    if (E(this_object()) == tp)
    {
	tp->catch_tell("You should drop it first.\n");
	return -1;
    }
    used_by += ({ tp });
    if (sizeof(used_by) >= free)
    {
	remove_adj("free");
	add_adj("occupied");
    }
    if ((tp->query_name()) != "gothmog")
    {   tp->catch_tell("A dark voice drones in your Head :\n");
	tp->catch_tell("How dare you sitting on the throne of Gothmog ?\n");
        tp->catch_tell("You feel a hot, burning pain running through your body!\n");
        x = (tp->query_max_hp())+(tp->query_stat(0))+10;
	tp->check_death(x , this_object()); 	
        return -1;
    }
    add_event(E_MOVE, "E_move", tp);
    tp->set_coordinates(query_coordinates());
    tp->define_primary("catch_cmd", this_object());
    tp->add_state(RESTING);
    tp->add_extra_desc(" is sitting here");
    tp->echo("You sit down on the " + short() + ".\n");
    
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " sits down on ", QNAME(this_object()),
			".\n"}), MSG_SEE ,tp);
    add_prop(OBJ_NO_GET, 1);
    return 1;
}



