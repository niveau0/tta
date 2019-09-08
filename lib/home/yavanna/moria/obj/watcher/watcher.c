inherit "/base/object";
inherit "/base/misc/listen";

#include <language.h>
#include <macros.h>
#include <material.h>
#include <mudtime.h>
#include <properties.h>
#include <std.h>
#include <event.h>
#include "../../include/watcher.h"

private static int    watcher_type, watch_dir, watch_distance;
private static int*   watch_range, watch_base;
private static string direction_desc;
private static object receiver;

static void
create_watcher()
{
    set_name("watcher");
    set_short("stone statue");
    set_pshort("stone statues");
    set_long("A small statue made of stone.\n");
    set_direction(NORTH);
    set_distance(150);
    set_watcher_type(NONE);
    add_prop(OBJ_MATERIAL, M_STONE);
    add_lwvs(0, 100000, 25000, 20);
}

static nomask void
create_object()
{
    create_watcher();
}

/*
 * Function name: set_watch_base
 * Description:   sets the base-view of the watcher   
 * Arguments:     base_view - the base-view-coordinates ({ x, y, z })     
 */
nomask void
set_watch_base(int *base)
{
    watch_base = base;
}

/*
 * Function name: query_watch_base
 * Description:   returns the base_view-coordinates of the Watcher 
 * Returns:       the base_view
 */
nomask int *
query_watch_base()
{
    return watch_base;
}

/*
 * Function name: set_watch_range
 * Description:   sets the range, within the Watcher reacts           
 * Arguments:     range - the range ({ x, y, z })     
 */
nomask void
set_watch_range(int* range)
{
    watch_range = range;
}


/*
 * Function name: query_watch_range
 * Description:   returns the range, within the Watcher reacts on moves
 * Returns:       integer array
 */
nomask int *
query_watch_range()
{
    return watch_range;
}

/*
 * Function name: set_direction
 * Description:   sets the direction the watcher watches                
 * Arguments:     dir - the direction (defined in watcher.h)
 * Returns:       1 - direction set
 *                0 - failed
 */
nomask int
set_direction(int dir)
{
    if (!dir || dir < NORTH || dir > NORTHWEST)
	return 0;
    
    watch_dir = dir;
    
    switch(dir)
    {
      case NORTH:
	  direction_desc = "north";
	  break;
      case NORTHEAST:
	  direction_desc = "northeast";
	  break;
      case EAST:
	  direction_desc = "east";
	  break; 
      case SOUTHEAST:
	  direction_desc = "southeast";
	  break;
      case SOUTH:
	  direction_desc = "south";
	  break;
      case SOUTHWEST:
	  direction_desc = "southwest";
	  break;
      case WEST:
	  direction_desc = "west";
	  break;
      case NORTHWEST:
	  direction_desc = "northwest";
	  break;
    }      
    return 1;
}

/*
 * Function name: query_direction()     
 * Description:   returns the direction the Watcher is watching         
 * Returns:       integer, the direction-code the watcher looks to          
 */
nomask int
query_direction()
{
    return watch_dir;
}

/*
 * Function name: query_direction_desc
 * Description:   returns the direction description where the watcher
 *                is looking to
 * Returns:       string, the direction-description
 */
nomask string
query_direction_desc()
{
    return direction_desc;
}

/*
 * Function name: set_distance(int dir) 
 * Description:   sets the distance the watcher can notice things       
 * Arguments:     dir - the distance in inch                  
 */
nomask int
set_distance(int dis)
{
    int blind, *coord;
    
    if (!(coord = query_coordinates()))
	return;
    if (!dis || !watch_dir)
	return 0;
    watch_distance = dis;
    
    if (!E(this_object()) || living(E(this_object())))
	return 0;
    blind = 10;
    
    switch (watch_dir)
    {
      case NORTH:
	  set_base_view( ({coord[0]-dis/2, coord[1]+blind, coord[2]}));
	  set_range( ({ coord[0]+dis/2, coord[1]+dis+blind, coord[2]+100}));
	  return 1;     
      case NORTHEAST:
	  set_base_view( ({coord[0]+blind, coord[1]+blind, coord[2]}) );
	  set_range( ({coord[0]+dis+blind, coord[1]+dis+blind, coord[2]+100}));
	  return 1;  
      case EAST:
	  set_base_view( ({coord[0]+blind, coord[1]-dis/2, coord[2]}));
	  set_range( ({coord[0]+dis+blind, coord[1]+dis/2, coord[2]+100}));
	  return 1;  
      case SOUTHEAST:
	  set_base_view( ({coord[0]+blind, coord[1]-dis-blind, coord[2]}));
	  set_range( ({coord[0]+dis+blind, coord[1]+blind, coord[2]+100}));
	  return 1;  
      case SOUTH:
	  set_base_view( ({coord[0]-dis/2, coord[1]-dis-blind, coord[2]}));
	  set_range( ({coord[0]+dis/2, coord[1]-blind, coord[2]+100}));
	  return 1;  
      case SOUTHWEST:
	  set_base_view( ({coord[0]-dis-blind, coord[1]-dis-blind, coord[2]}));
	  set_range( ({coord[0]-10, coord[1]-10, coord[2]+100}));
	  return 1;  
      case WEST:
	  set_base_view( ({ coord[0]-dis-blind, coord[1]-dis/2, coord[2]}));
	  set_range( ({coord[0]-blind, coord[1]+dis/2, coord[2]+100}));
	  return 1;  
      case NORTHWEST:
	  set_base_view( ({coord[0]-dis-blind, coord[1]+blind, coord[2]}));
	  set_range( ({coord[0]-blind, coord[1]+dis+blind, coord[2]+100})); 
	  return 1;  
      default:
	  return 0;
    }
}

/*
 * Function name: turn_cmd(string dir) 
 * Description:   turn the watcher in direction dir                          
 * Arguments:     dir - the direction to be turned to
 * Returns:       1 - if successfull
 *                0 - else
 */
int
turn_cmd(string dir)
{
    object tp;
    tp = this_player();

    if(this_object()->query_prop("NO_STATUE_TURN"))
    {
	tp->catch_tell("You cannot turn this.\n");
	return 1;
    }
    
    if(living(E(this_object())))
    {
	tp->catch_tell("You cannot turn something you are carrying!\n");
	return 1;
    }
    
    if (!dir)
    {
	tp->catch_tell("Turn " + query_name() +
		       " in which direction?\n");
	return 1;
    }
    tp->catch_tell("You turn the " + query_name() + " " + dir + ".\n");
    tell_objects(LISTEN(E(this_object())) - ({ tp }),
		({ QCTNAME(this_player()),
		       " turns ", QTNAME(this_object()),
		       " ", dir, ".\n" }), MSG_SEE, tp);
    
    if (dir == "right")
    {
	if (++watcher_direction > NORTHWEST)
	    watcher_direction = NORTH;
	set_direction(watcher_direction);
    }
    else
    {
	if (--watcher_direction < NORTH)
	    watcher_direction = NORTHWEST;
	set_direction(watcher_direction);
    }
    set_distance(watcher_distance);
    return 1;
    
}

/*
 * Function name: set_receiver
 * Description:   sets the object that should be informed by the watcher
 * Arguments:     ob - the object that should be informed
 */
nomask void
set_receiver(object ob)
{
    receiver = ob;
}

/*
 * Function name: query_receiver
 * Description:   returns the object that should be informed by the watcher
 * Returns:       the receiver of the Watcher
 */
nomask object
query_receiver()
{
    return receiver;
}

/*
 * Function name: set_watcher_type
 * Description:   sets the watcher-type       
 * Arguments:     type - the watcher-type:
 *                  1 react on Moves within range
 *                  2 react on spoken words
 *                  3 react on both
 */
nomask void
set_watcher_type(int type)
{
    if (type < 1 || type > 3)
	return;
    watcher_type = type;
}

/*
 * Function name: query_watcher_type
 * Description:   returns the watcher-type    
 * Returns:       the watcher-type:
 *                  1 react on moves within range
 *                  2 react on spoken words
 *                  3 react on both
 */
nomask int
query_watcher_type()
{
    return watcher_type;
}

/*
 * Function name: stop_watch
 * Description:   let the watcher stop its work
 */
nomask void
stop_watch()
{
    if (watcher_type & 1)
	remove_event(E_MOVE_INV, "notify_moves", E(this_object()));
    if (watcher_type & 2)
	stop_listen();
}

/*
 * Function name: start_watch
 * Description:   let the watcher begin its work
 */
nomask void
start_watch()
{
    if (!E(this_object()) || living(E(this_object())) || receiver == this_object())
	return 0;
    if (watcher_type & 1)
	add_event(E_MOVE_INV, "notify_moves", E(this_object()));
    if (watcher_type & 2)
	start_listen();
}

/*
 * Function name: notify_own_move
 * Description:   catch the movement of this object
 * Arguments:     ob - moving object
 *                from - old environment
 *                to - new environment
 */
nomask void
notify_own_move(object ob, object from, object to)
{
    if (ob != this_object())
	return;
    if (to)
	start_watch();
    remove_event(E_MOVE, "notify_own_move", this_object());
}

/*
 * Function name: notify_moves
 * Description:   moves in environment
 * Arguments:     ob - moving object
 *                from - old environment
 *                to - new environment
 */
nomask void
notify_moves(object ob, object from, object to)
{
    
}

/*
 * Function name: glow_eyes()
 * Description:   let the eyes of the Watcher glow in red light.
 */
void
glow_eyes()
{
    add_lwvs(30,0,0,0);
    tell_objects(LISTEN(E(this_object())) - ({ this_object() }),
		      ({ "The eyes of ", QTNAME(this_object()),
			     " glow in a red light for a moment.\n"}),
		      MSG_SEE, this_object());
    add_lwvs(-30,0,0,0);
}

/*
 * Function name: move_reaction(object ob)
 * Description:   the reaction on moves within the same room
 * Arguments:     ob - the moving object
 */
varargs void
move_reaction(object ob, object to)
{
    string msg, name;
    if (!ob)
	return;
    name = ob->query_name();
    if (!(name = receiver->query_met(name)))
	name = capitalize(LANG_ADDART(ob->query_nonmet_name()));
    else
	name = capitalize(name);
    if (to = E(this_object()))
	msg = "I can see " + name + " walking around.\n";
    else
	msg = name + " leaves the room.\n";
    receiver->catch_msg( ({ "#", capitalize(query_name()),":", msg }), 0,
    MSG_HEAR );
}

/*
 * Function name: E_move_inv(object ob, object from, object to)
 * Description:   let the watcher react on moves within the same room     
 * Arguments:     ob - the Object moving
 *                from - from where it comes
 *                tp - to where it goes
 */
void
E_move_inv(object ob, object from, object to)
{
    int *coord_ob;
    
    if (!ob || ob == this_object() || !from ||
	receiver == this_object() || !living(ob)
	|| !(check_visible(E(this_object())->query_light()+30, 0, 100,
			   query_coordinates())))
	return;
    coord_ob = ob->query_coordinates()[..];
    
    if(in_range(coord_ob, query_watcher_base_view(), query_watcher_range()))
	move_reaction(ob, to);
}

/*
 * Function name: speak_reaction(string msg)
 * Description:   the reaction on catch_tells to this object          
 * Arguments:     msg - the telled msg
 */

void
speak_reaction(string msg)
{
    string received_msg;
    if (!msg)
	return;
    
    received_msg = "#" + capitalize(query_name()) + ":";
    receiver->catch_tell(break_string(received_msg + msg, 80));
    glow_eyes();
}

/*
 * Function name: catch_tell
 * Description:   let the watcher react on speaking                   
 * Arguments:     msg - the msg spoken
 */
void 
catch_tell(string msg)
{

    string who, msg2;
    
    if (receiver == this_object() || !msg)
	return;
    
    speak_reaction(msg);
}

/*
 * Function name: long
 * Description:   Describe the object
 * Returns:	  string, long description
 */
string
long()
{
    if (!E(this_object()))
	return ::long();
    return ::long() + (living(E(this_object())) ? "" : "It's looking to the "+ direction_desc + ".\n");   
}

/*
 * Function name: short
 * Description:   Return the short description of the object.
 * Returns:       string, short description
 */
string
short()
{
    if (!E(this_object()))
	return ::short();
    return ::short()+(living(E(this_object())) ? "" : "(looking "+ direction_desc+")");
}
    
/*
 * Function name: remove_object
 * Description:   Removes this object from the game.
 * Returns:       True if the object was removed.
 */
varargs void
remove_object(object ob)
{
    stop_watch();
    ::remove_object(ob);
}




