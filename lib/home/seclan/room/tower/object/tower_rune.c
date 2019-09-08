/* This file contains code for a speech-controlled,
 * bidirectional transporting device. ;)
 */

inherit "/base/misc/listen";
inherit "/base/misc/openclose";
inherit "/base/object";

#include <door.h>
#include <event.h>
#include <macros.h>
#include <material.h>
#include <move.h>
#include <properties.h>

int status, in_tower;
string dest_path; // path of current destination
object other_door; // contains other door, if existing, else 0

void
create_object()
{
    status = DOOR_CLOSED;
    dest_path = "/d/Standard/Bree/room/marketplace";
    in_tower = 0;
    other_door = 0;
    
    set_name("rune");
    add_name("carving");
    add_adj("complex");
    add_adj("oval");
    set_short("rune");
    set_pshort("runes");
    set_long_bs("You see a pattern of runes, filled with complex ... things? "+
	     "Every closer look you take makes you feeling lightheaded.\n");
    add_lwvs(50, 0, 0, 100);

    add_prop(OBJ_NO_SHOW, 1);
    add_prop(OBJ_NO_GET, -1);
    add_prop(OBJ_MATERIAL, M_AIR);
    if (IS_CLONE)
    {
	add_event(E_MOVE, "initial_move", this_object());
    }
}

/*
 * Function name: initial_move
 * Description:   sets initial values after the first movement of the object;
 *                delay to creation is necessary to have an environment for
 *                start_listen
 */
void
initial_move()
{
    int dummy;
    object to, env;

    to = this_object();
    env = E(to);

    if ( sscanf(env->query_room_file(), "/home/seclan/room/tower/%*s") )
	in_tower = 1;

    if (in_tower)
    {
	start_listen();
	tell_objects( LISTEN(env)-({ to }),
		      " The rune emits: Powerup initiated ... all systems nominal ... "+
		      "security clamps unlocked ... awaiting commands.\n",
		      MSG_HEAR, to, 500, 1);
    }
    remove_event(E_MOVE, "initial_move", to);
}

/*
 * Function name: catch_tell
 * Description:   let the door react on speaking
 * Arguments:     msg - the msg spoken
 */
void
catch_tell(string msg)
{
    int *goalsize, *goalcoord;
    string told_message;
    object to, *listener, other_room;

    if (!msg)
	return;
    
    to = this_object();
    listener = LISTEN(E(to))-({ to });
    
    tell_objects( listener,
		 "The rune vibrates in the rythm of the words.\n",
		 MSG_SEE, to, 1);
    if ( sscanf(msg, "%*s in %*s:  %s\n", told_message) != 3 )
    {
	return;
    }

    // act according to command
    switch (told_message)
    {
      case "commands":
      {
	  // give a list and explanation of commands
	  tell_objects( listener,
			"Usage of the Beam-O-Trans is currently restricted to "+
			"X commands ...\n"+
			"1. 'preptrans' - activate Hyperlink connection\n ..."+
			"",
		 MSG_HEAR, to, 1);
	  break;
      }
      case "powerdown":
      {
	  // stop listening, shut down and remove other door
	  tell_objects( listener,
			"Securing hyperlink clamps ... check! ... "+
			"shutting down matter-core...3...2...1 ... check!\n"+
			"... for restart-sequence refer to manual ... exiting "+
			"*buzz*\n",
			MSG_HEAR, to, 1);
	  stop_listen();
	  break;
      }
      case "preptrans":
      {
	  if (!dest_path)
	  {
	      // no destinationpath is set
	      tell_objects( listener,
			"No allowed transferpoint locked ... \n",
			MSG_HEAR, to, 1);
	      return;
	  }
	  if (other_door)
	  {
	      tell_objects( listener,
			"Trying to refocus open hyperlink ... failed.\n",
			MSG_HEAR, to, 1);
	  }
	  else
	      tell_objects(listener,
			   "The rune instantly fills with lightbeams.\n",
			   MSG_SEE, to, 1);
	  // create other rune in goalroom
	  if ( !(other_room = _ROOMD->query_room(dest_path)) )
	  {
	      other_room = _ROOMD->load_room(dest_path);
	  }
	  if ( !(other_door = clone_object(MASTER)) )
	      //failed to create door
	      return;
	  // Rune will appear in the middle of the room.
	  goalcoord = other_room->query_coordinates();
	  goalsize = other_room->query_room_size();
	  other_door->set_coordinates(
	      ({ goalsize[0]/2 + goalcoord[0],
		     goalsize[1]/2 + goalcoord[1],
		     goalcoord[2] }) );
	  // rune will appear as object in other room
	  other_door->move(other_room);
	  other_door->add_prop(OBJ_NO_SHOW, 0);

	  tell_objects( LISTEN(other_room) - ({ other_door }),
			"A complex rune of lights appears.\n",
			MSG_SEE, other_door, 1);
	  other_door->set_other_door(to);
	  break;
      }
      case "endtrans":
      {
	  if(other_door)
	  {
	      // Zweite Tuer ausserhalb des Turmes vernichten.
	      tell_objects(LISTEN(E(other_door)) - ({ other_door }),
			   "The rune disappears in a flash of sparks.\n",
			   MSG_SEE, other_door, 1);
	      tell_objects(listener, "The rune flickers and shines "+
			   "a bit dimmer.\n", MSG_SEE, to, 1);
	      remove_object(other_door);
	      other_door = 0;
	  }
	  else
	      tell_objects( listener,
			"No transferchannel aktivated.\n",
			MSG_HEAR, to, 1);
	  break;
      }
      default:
	  break;
    }
    // new destination has been said?
    if ( sscanf(msg, "%*s in %*s:%*sgoal %s\n", told_message) != 4 )
        return;
    tell_objects( listener, "Target "+told_message+
		  " acknowledged ...\n", MSG_HEAR, to, 500, 1);
    if (!_ROOMD->query_room(told_message))
    {
	tell_objects( listener,
		      "According to available database this is "+
		      "no allowed transferpoint.\n", MSG_HEAR, to, 500, 1);
	return;
    }
        
    tell_objects( listener,
		  "The rune shifts through the rainbow colors.\n",
		  MSG_SEE, to, 1);
    tell_objects( listener, "New target locked.\n", MSG_HEAR, to, 500, 1);
    dest_path = told_message;
}

/*
 * Function name: enter_cmd
 * Description:   handle enter cmd
 * Arguments:     str - command arguments
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
int
enter_cmd(string str, object tp)
{
    object env, to;

    to = this_object();

    if( other_door )
    {
	env = E(other_door);
	
	tell_objects(LISTEN(E(to)) - ({ to, tp }),
		     ({ QCTNAME(tp), " disappears within the rune.\n" }),
		     MSG_SEE | MSG_WALK, tp);
	tp->move_living(env, (other_door->query_coordinates()),
			" disappears within the rune.\n", MOVE_TELEPORT );
	tell_objects(LISTEN(env) - ({ tp }),
		     ({ QCTNAME(tp), " appears within the rune.\n" }),
		     MSG_SEE | MSG_WALK, tp);
    }
    else
    {
  	tell_objects( LISTEN(E(to))-({ to }),
		      "The rune emits: Presencetrigger failed due to inaktive "+
		      "hyperlink.\n",
		      MSG_HEAR, to, 500, 1);
    }
  return 1;
}

/*
 * Function name: set_other_door
 * Description:   sets the corresponding door for this one 
 * Arguments:     door - the new exit-door
 */
void
set_other_door(object door)
{
    if ( !door )
	return;
    other_door = door;
}
