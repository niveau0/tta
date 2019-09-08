inherit "/base/door";
inherit "/base/misc/listen";

#include <door.h>
#include <macros.h>
#include <material.h>
#include <mudtime.h>
#include <config.h> 
#include <properties.h>
#include <weather.h>
#include <event.h>

#define INSCRIPTION \
"Written in Feanorian characters you read:\n"+\
"The doors of Durin, Lord of Moria. Speak friend, and enter...\n"+\
"I, Narvi, made them. Celebrimbor of Hollin drew these signs.\n"

#define PATTERN \
"The silver pattern seems to glow somehow. It shows outlines of\n"+\
"a 6 feet wide, 9 feet tall gate. Surrounding the gate there are\n"+\
"two trees, one on each side. At the center, near the top you\n"+\
"see a crown with seven stars overhead it. Beneath it there is\n"+\
"a anvil and a sixteen-pointed blaze. The top of the gate is\n"+\
"surmounted by arching bands of Feanorean script.\n"

#define INSIDE_LONG \
"A great stone gate, nearly 6 feet wide and 9 feet tall.\n"

#define INVIS_OPEN_LONG \
"A great stone gate is leading into the mountain. You can see shallow stairs"+\
"\nleading up behind the gate.\n"

#define INVIS_CLOSED_LONG \
"This cliff looks like it had been polished somehow. You can't see even\n"+\
"a single crack or trace of his age on it./nIt would be very difficult "+\
"to scale this cliff.\n"

#define CLOSED_LONG \
"You can see a engraved silvern pattern glowing on the cliff, shaped like a"+\
"\n6 feet wide, 9 feet tall gate. Two trees surround the gate and at the "+\
"center\nnear the top, there's a crown and seven stars overhead it. Beneath "+\
"the\ncrown there is a anvil and a sixteen-pointed blaze. The top of the "+\
"gate is\nsurmounted by arching bands of Feanorean script.\n"

#define OPEN_LONG \
"You can see a engraved silvern pattern glowing on the 6 feet wide,\n"+\
"9 feet tall gate. Two trees surround the gate and at the center, near the\n"+\
"top, there's a crown and seven stars overhead it. Beneath the crown there "+\
"is\na anvil and a sixteen-pointed blaze. The top of the gate is surmounted "+\
"by an \narching bands of Feanorean script.\n"

#define INVIS_LONG \
"This cliff looks like it had been polished somehow. You can't see even\n"+\
"a single crack or trace of his age on it.\nIt would be very difficult\n"+\
"to scale this cliff.\n"

static int visible;

nomask void set_visible(int st, int no_msg);
int query_outside(object env);

static void
create_door()
{
    set_name("gate");
    set_short("gate");
    set_pshort("gates");
    set_long("A Gate");
    add_prop(OBJ_NO_GET, 1);
    add_prop(OBJ_MATERIAL, M_STONE);
    add_lwvs(0, 1000000, 900000, 80);
    if (!IS_CLONE)
	return;
    set_status(DOOR_LOCKED);
    add_event(E_MOVE, "enter_env", this_object());
}

nomask void
enter_env(object ob, object from, object to)
{
   if (!to || ob != this_object() || living(to))
	return;
    remove_name("gate");
    if (query_outside(to))
    {
	set_name("cliff");
	set_short("polished cliff");
	set_pshort("polished cliffs");
	set_direction_desc("closing one side of this canyon");
	set_open_close_prefix("open", 0);
	visible = 0;
	set_connection("../level0/w_entry", 1);
	WEATHER_MASTER->notify_hour_event("hour",DAWN);
	WEATHER_MASTER->notify_hour_event("hour",NIGHTTIME);
	if(HOUR >= DAWN && HOUR < NIGHTTIME)
	    set_visible(0, 1);
	else
	    set_visible(1, 1);
    } 
    else 
    {
	set_name("gate");
	set_short("two-winged stone gate");
	set_pshort("two-winged stones gates");
	set_direction_desc("leading outside Moria");
	set_open_close_prefix("open", "closed");
	set_connection("../outside/outside_w", 1);
    }
    call_out("start_listen",2);
    set_status(query_status());
}

nomask string
long()
{
    if (E(this_object()))
    {
	if (query_outside(E(this_object())))
	{
	    if (!visible)
	    {
		if (query_status() == DOOR_OPEN)
		    return INVIS_OPEN_LONG;
		return INVIS_CLOSED_LONG;
	    }
	    if (query_status() == DOOR_OPEN)
		return OPEN_LONG;
	    return CLOSED_LONG;
	}
	return INSIDE_LONG;
    }
    return "";
}

int
open_cmd(string str)
{
    return 0; /* the gate cannot be open manually */
}

int
close_cmd(string str)
{
    return 0; /* the gate cannot be closed manually */
}

/*
 * Function name: set_visible
 * Description:   set ornament visibility
 * Arguments:     st - visibility
 */
nomask void
set_visible(int st, int no_msg)   
{
    string n;
    
    if (query_status() == DOOR_OPEN)
	n = "stone gate";
    else
	n = "cliff";
    
    visible = st;
    if (!visible)
    {
	if (!no_msg)
	    tell_objects(LISTEN(E(this_object())) - ({ this_object() }),
			 "The silver engravement on the cliff disappears "+
			 "slowly.\n" +
			 "After some seconds you can't see it anymore.\n",
		     MSG_SEE, this_object());
	remove_adj("silver-engraved");
	remove_adj("engraved");
	add_adj("polished");
	set_short("polished " + n);
	remove_item(2);
	remove_item(1);
    }
    else
    {
	if (!no_msg)
	    tell_objects(LISTEN(E(this_object())) - ({ this_object() }),
			 ({ "An silver engravement starts to shine on ",
				QTNAME(this_object()), ".\n" }),
			 MSG_SEE, this_object());
	add_adj("silver-engraved");
	add_adj("engraved");
	remove_adj("polished");
	set_short("silver-engraved " + n);
	add_item( ({ "inscription","script","band" }), 
		  ({"bands", "characters", }), INSCRIPTION, 1);
	add_item( ({ "engravement", "pattern" }), 
		  ({ "engravements" }), PATTERN, 2);
	

    }
    set_status(query_status()); /* update room desc */
}



/*
 * Function name: close_gate
 * Description:   the gate closes
 */
static void
close_gate()
{
    object other;
    string msg_i, msg_o;
 
    if (query_status() != DOOR_OPEN)
	return;
    if (!(other = find_other_door()))
	return;
    msg_i = "With a slamming noise the gate closes.\n";
    msg_o = "With a slamming noise the gate closes and melts " +
	"with the cliff.\n";
   
    if (query_outside(E(this_object())))
    {
	tell_objects(LISTEN(E(other)) - ({ other }),
		     msg_i,
		     MSG_SEE | MSG_HEAR, other);
	tell_objects(LISTEN(E(this_object())) - ({ this_object() }),
		     msg_o,
		     MSG_SEE | MSG_HEAR, this_object());
	
	set_short((visible ? "silver-engraved" : "polished") + " cliff");
	set_direction_desc("closing one side of this canyon");
	remove_name("gate");
	other->add_lwvs(-40,0,0,0);
    } 
    else 
    {
	tell_objects(LISTEN(E(other)) - ({ other }),
		     msg_o,
		     MSG_SEE | MSG_HEAR, other);
	tell_objects(LISTEN(E(this_object())) - ({ this_object() }),
		     msg_i,
		     MSG_SEE | MSG_HEAR, this_object()); 
	other->set_short((visible ? "silver-engraved" : "polished") + 
			 " cliff");
	other->set_direction_desc("closing one side of this canyon");
	other->remove_name("gate");
	add_lwvs(-40,0,0,0);
    }
    set_status(DOOR_LOCKED);
    other->set_status(DOOR_LOCKED);
    add_prop("_DOOR_MOVING",0);
}

/*
 * Function name: open_gate
 * Description:   the gate opens
 */
static void
open_gate()
{
    object other;
    string msg_i, msg_o;
    
    if (query_status() == DOOR_OPEN)
	return;
    if (!(other = find_other_door()))
	return;

    msg_i = "The gatewings stop to move and you can see " +
	"an exit out of the mountain.\n";
    msg_o ="As the gatewings stop to move, you notice an entrance " +
	"into the mountain.\n";

    if (query_outside(E(this_object())))
    {
	tell_objects(LISTEN(E(other)) - ({ other }),
		     msg_i,
		     MSG_SEE | MSG_HEAR, other);
	tell_objects(LISTEN(E(this_object())) - ({ this_object() }),
		     msg_o,
		     MSG_SEE | MSG_HEAR, this_object());
	set_short((visible ? "silver-engraved" : "polished") + " stone gate");
	set_direction_desc("leading into the mountain");
	add_name("gate");
    } 
    else
    {
	tell_objects(LISTEN(E(other)) - ({ other }),
		     msg_o,
		     MSG_SEE | MSG_HEAR, other);
	tell_objects(LISTEN(E(this_object())) - ({ this_object() }),
		     msg_i,
		     MSG_SEE | MSG_HEAR, this_object());
	other->set_short((visible ? "silver-engraved" : "polished") +
			 " stone gate");
	other->set_direction_desc("leading into the mountain");
	other->add_name("gate");
    }
    set_status(DOOR_OPEN);
    other->set_status(DOOR_OPEN);
    add_prop("_DOOR_MOVING",0);
}

void
open_word()
{
    object other;
    string msg_i, msg_o;
    
    if (query_status() == DOOR_OPEN)
	return;
    if (!(other = find_other_door()))
	return;

    msg_i = "The gatewings slowly start to open.\n";
    msg_o = "The two gatewings slowly start to open in the polished cliff.\n";
    
    if (query_outside(E(this_object())))
    {
	other->add_lwvs(40,0,0,0);
	tell_objects(LISTEN(E(other)) - ({ other}),
		     msg_i,
		     MSG_SEE | MSG_HEAR, other);
	tell_objects(LISTEN(E(this_object())) - ({ this_object() }),
		     msg_o,
		     MSG_SEE | MSG_HEAR, this_object());
    } 
    else 
    {
	add_lwvs(40,0,0,0);
	tell_objects(LISTEN(E(other)) - ({ other}),
		     msg_o,
		     MSG_SEE | MSG_HEAR, other);
	tell_objects(LISTEN(E(this_object())) - ({ this_object() }),
		     msg_i,
		     MSG_SEE | MSG_HEAR, this_object());
    }
    call_out("open_gate", 10);
    add_name("gate");
    other->add_name("gate");
    add_prop("_DOOR_MOVING",1);
}

void
close_word()
{
    object other;
    string msg;
    
    msg = "The gatewings slowly starts to close.\n";
 
    if (query_status() != DOOR_OPEN)
	return;
    if (!(other = find_other_door()))
	return;
    tell_objects(LISTEN(E(other)) - ({ other}),
		 msg,
		 MSG_SEE | MSG_HEAR, other);
    tell_objects(LISTEN(E(this_object())) - ({ this_object() }),
		 msg,
		 MSG_SEE | MSG_HEAR, this_object());
    call_out("close_gate", 10);
    add_prop("_DOOR_MOVING",1);
}

/*
 * Function name: hour
 * Description:   let the door act when the moon shines, called by
 *                WEATHER_MASTER
 * Arguments:     h - hour
 * Returns:       1 if success, -1 else
 */
void
hour(int h)
{
    if (WEATHER_MASTER->query_moon() == MOON_PHASE_1)
	return;
    
    switch(h)
    {
      case DAWN:
	  set_visible(0, 0);
	  break;
      case NIGHTTIME:
	  set_visible(1, 0);
	  break;
    }	  
}
/*
 * Function name: catch_tell
 * Description:   let the door react on speaking                   
 * Arguments:     msg - the msg spoken
 */
void 
catch_tell(string msg)
{
    string what, msg_o;
    
    if (!msg || sscanf(msg, "%*s in Sindarin: %s\n", what) != 2)
	return;
    
    msg_o = "A glimmer of light appears on the "+
	short()+ ", showing \nthe outline of a"+
	" great, two-winged gate.\n";
    switch (what)
    {
    case "friend":
	if (query_status() != DOOR_OPEN)
	    if (query_outside(E(this_object())))
		tell_objects(LISTEN(E(this_object())) - ({ this_object() }),
			     msg_o,
			     MSG_SEE | MSG_HEAR, this_object()); 
	call_out("open_word", 2);
	break;
    case "celebrimbor":
	call_out("close_word", 2);
	break;
    default:
	break;
    }
}
	
int 
query_outside(object env)					 
{
    if (env->query_room_file() == 
	"/home/yavanna/moria/room/outside/outside_w")
	return 1;
    return 0;
}

int
enter_cmd(string str, object tp)
{
    if(query_prop("_DOOR_MOVING"))
    {
	tp->catch_tell("There's no place to move through the gatewings "+
				"while they move.\n");
	return 1;
    }
    return ::enter_cmd(str, tp);
}






