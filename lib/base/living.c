inherit "container";
inherit "living/cmd";
inherit "living/combat";
inherit "living/combat_intern";
inherit "living/extra";
inherit "living/magic";
inherit "living/move";
inherit "living/skill";
inherit "living/slot";
inherit "living/stat";
inherit "living/subdesc";

#include <access.h>
#include <composite.h>
#include <event.h>
#include <effects.h>
#include <language.h>
#include <living.h>
#include <macros.h>
#include <material.h>
#include <mudtime.h>
#include <properties.h>
#include <skills.h>
#include "living/carry.c"
#include "living/gender.c"

nomask string query_met_name();

private string       *extra_desc;
private object              user; /* possible a user on this object */

/*
 * Function name: create_living
 * Description:   redefinable create function
 */
static void
create_living()
{
    set_race("living");
}

/*
 * Function name: create_container
 * Description:   Create the living object. (constructor)
 */
static nomask void
create_container()
{
    int i;

    cmd_reset();
    extra_reset();
    magic_reset();
    move_reset();
    skill_reset();
    stat_reset();
    carry_reset();
    slot_reset();
    combat_reset();
    cb_intern_reset();
    reset_extra_desc();
    
    if (query_npc())
    {
	for (i=STAT_FIRST; i<=STAT_LAST; i++)
	{
	    set_tmp_stat(i, 50);
	    set_pot_stat(i, 50);
	}
	add_prop(LIVE_SEE_NO_WALKING, 1);
    }
    add_prop(LIVE_BUTCHERED, 0);
    add_prop(LIVE_SKINNED, 0);
    add_prop(OBJ_MATERIAL, M_FLESH);
    add_event(E_MOVE, "E_move", this_object());
    add_event(E_GOT_HIT, "E_got_hit", this_object());
    enable_commands();
    create_living();
    add_prop(CONT_TRANSP, 1); /* allow looking into corpse if living dies */
}

/*
 * Function name: parse_object(string *cmd, int type, int *co, int d,
 *                             object tp)
 * Description:   this function is called by parse(), check if this object
 *                matches the command string
 *                here: support introduce system
 * Arguments:     cmd - array of exploded cmd string 
 *                type - if 1 match singular name,
 *                       if 2 match plural name,
 *                       if 3 match singular or plural name
 *                co - if not zero, include distance check
 *                d - if not zero, include distance check
 *                tp - if not zero, include remember check
 * Returns:       found name type
 */
nomask int
parse_object(string *cmd, int type, int *co, int d, object tp)
{
    int    i, sz;
    string name, rname, rpname, metname, metpname;

    if (!tp || !(rname = query_met_name()))
	return ::parse_object(cmd, type, co, d);
    sz = i = sizeof(cmd);
    if (!sz)
	return 0;
    rpname = query_pname();
    if (metname = tp->query_met(rname))
    {
	metpname = LANG_PWORD(metname);
	for (name=""; i--;)
	{
	    name = cmd[i] + name;
	    if (name == metname)
	    {
		cmd[i] = rname;
		return ::parse_object(cmd[..i], type, co, d);
	    }
	    if (name == metpname)
	    {
		cmd[i] = rpname;
		return ::parse_object(cmd[..i], type, co, d);
	    }
	    name = " " + name;
	}
    }
    if (cmd[--sz] == rname || cmd[sz] == rpname)
	/* real name is not valid if 'metname' failed */
	return 0;
	
    return ::parse_object(cmd, type, co, d);
}

/*
 * Function name: set_user
 * Description:   set current user for this object
 * Arguments:     u - the user object
 */
void
set_user(object u)
{
    user = u;
}

/*
 * Function name: query_user
 * Description:   gives current user for this object
 * Returns:       the user object
 */
nomask object
query_user()
{
    return user;
}

/*
 * Function name: E_move
 * Description:   event, here: catch own movement and set msg events
 * Arguments:     ob - which object moved
 *                from - where the object comes from
 *                to - where it goes
 */
nomask void
E_move(object ob, object from, object to)
{
    int     i;
    object  *r;
    mapping exits;
	

    if (ob != this_object() || from == to)
	return;

    /* move_inv event is used to initialize combat
     * msg_event to receive seen/heard messages
     */
    if (from)
    {
	remove_event(E_MOVE_INV, "cb_init_fight", from);
	remove_event(E_MSG, "?", from);
	if (exits = from->query_loaded_exit())
	{
	    r = map_indices(from->query_loaded_exit());
	    for (i=sizeof(r); i--;)
	    {
		remove_event(E_MOVE_INV, "cb_init_fight", r[i]);
		remove_event(E_MSG, "?", r[i]);
	    }
	}
    }
    if (to)
    {
	add_event(E_MOVE_INV, "cb_init_fight", to);
	if (exits = to->query_loaded_exit())
	{
	    r = map_indices(to->query_loaded_exit());
	    for (i=sizeof(r); i--;)
		add_event(E_MOVE_INV, "cb_init_fight", r[i]);
	}

	/* add msg event only if any message flag is set */
	if (query_msg_flags() & (MSG_HEAR | MSG_SEE))
        {
            add_event(E_MSG, "?", to);
            for (i=sizeof(r); i--;)
                add_event(E_MSG, "?",  r[i]);
        }
    }
}

/*
 * Function name: catch_tell
 * Description:   deliver message to user
 * Arguments:     msg - the message string
 *                no_prompt - true if no prompt should be send
 */
varargs void
catch_tell(string msg, int no_prompt)
{
    if (!user)
	return;
    if (user->query_player() == this_object())
	user->catch_tell(msg, no_prompt);
    else if (living(this_object()))
	/* controlled object */
	user->catch_tell((no_prompt ? msg : query_name() + "#\n" + msg),
			 no_prompt);
}

/*
 * Function name: catch_msg(mixed msg, object from, int type,
 *                          int dist, int no_prompt))
 * Description:   This function is called for every message sent to this
 *                living that includes met/nonmet/unseen
 * Arguments:     msg - Message to tell the player, could be string or pointer
 *                      ({ ({ object, met, nonmet, unseen }), string, ... }) to
 *                      encourage the met/nonmet system
 *                      Pointer messages will be cut (break_string).
 *                from - The object that is acting (important to determine
 *                       if message can be seen), 0 does no 'senses' check
 *                type - the message type: hear, see
 *                dist - optional argument (distance messages can be heard)
 *                no_prompt - true if no prompt should be send
 */
nomask varargs void
catch_msg(mixed msg, object from, int type, int dist, int no_prompt)
{
    int    i, sz, see;
    string str, name;
    mixed  sub;

    see = 1;
    if (type)
    {
	if (type & MSG_WALK)
	{
	    if (query_prop(LIVE_SEE_NO_WALKING))
		return;
	    type ^= MSG_WALK;
	}
	if (!(type & query_msg_flags()))
	    return;
	if ((type & MSG_HEAR) && from && !can_hear(from, dist))
	    type ^= MSG_HEAR;
	if ((type & MSG_SEE) && from && !can_see(from))
	{
	    see = 0;
	    type ^= MSG_SEE;
	}
	if (!type)
	    return;
    }

    if (stringp(msg))
    {
	catch_tell(msg, no_prompt);
	return;
    }
    if (!pointerp(msg))
	error("Bad argument 1 for function catch_msg");
    str = "";
    for (i=0, sz=sizeof(msg); i<sz; i++)
    {
	if (stringp(sub = msg[i]))
	    str += sub;
	else
	{
	    if (!see || !can_see(sub[0]))
		str += sub[3];
	    else if (sub[1] && (name = query_met(sub[1])))
		str += capitalize(name);
	    else
		str += sub[2];
	}
    }
    catch_tell(BS(str, 77, 0), no_prompt);
}

/*
 * Function name: echo
 * Description:   used to echo a response on cmd, can be switched off
 * Arguments:     msg - what to echo if an echo is wanted
 *                from - The object that is acting (important to determine
 *                       if message can be seen), 0 does no 'senses' check
 *                type - the message type: hear, see
 *                dist - optional argument (distance messages can be heard)
 */
nomask varargs void
echo(mixed msg, object from, int type, int dist)
{
    if (query_prop(LIVE_GET_ECHO))
	catch_msg(msg, from, type, dist);
    else
	catch_tell("Ok.\n");
}

/*
 * Function name: set_linkdead
 * Description:   care for possession if connection gets lost
 * Arguments:     i - linkdead flag
 */
void
set_linkdead(int i)
{
    object u;

    if (i && (u = query_user()) && (u = u->query_splayer()))
	u->add_prop(LIVE_POSSESSED, this_object());
}

/*
 * Function name: query_linkdead
 * Description:   default false, npcs are never linkdead
 * Returns:       0
 */
int
query_linkdead()
{
    return 0;
}

/*
 * Function name: query_adjs
 * Description:   Gives the adjectives of the living, here again to add the
 *                gender string.
 * Returns:       array, all adjectives
 */
string *
query_adjs()
{
    return ({ query_gender_string() }) + ::query_adjs();
}

/*
 * Function name: query_met
 * Description:   Tells if we know a certain living's name.
 * Arguments:     name - Name of living or objectp of living
 * Returns:       True if we know this name otherwise false.
 */
string
query_met(string name)
{
    object u;

    if (u = query_user())
    {
	u = u->query_splayer();
	if (!u)
	    return name;
	return u->query_met(name); /* if monster is controlled */
    }
    return name; /* All uncontrolled npcs know everybody */
}

/*
 * Function name: query_met_name
 * Description:   Gives the name of this living to players who have met
 * Returns:       string, 0 if name equals race name, i.d. no individual name
 */
nomask string
query_met_name()
{
    if (query_name() == query_race())  /* no individual name */
	return 0;
    return query_name();
}

/*
 * Function name: query_nonmet_name
 * Description:   Gives the name of this living to players who have not met
 * Returns:       string
 */
nomask string
query_nonmet_name()
{
    string *adj;
    
    if (sizeof(adj = query_adjs()) > 0)
    {
	if (query_name() != query_race()) 
	    return implode(adj, " ") + " " + query_race();
	/* no individual name, don't show gender string */
	if (sizeof(adj) > 1)
	    return implode(adj[1..], " ") + " " + query_race();
    }
    return query_race();
}

/*
 * Function name: query_presentation
 * Description:   Gives a presentation of the living in one line.
 * Arguments:     tp - the living that wants to know presentation string
 * Returns:       The presentation string
 */
string
query_presentation(object tp)
{
    return (capitalize(tp->query_met(query_name())) + ", a " +
	    query_gender_string() + " " + query_race());
}

/*
 * Function name: add_extra_desc
 * Description:   add an extra description to the short
 * Arguments:     desc - the extra description
 */
nomask void
add_extra_desc(string desc)
{
    extra_desc |= ({ desc });
}

/*
 * Function name: remove_extra_desc
 * Description:   removes an extra description
 * Arguments:     desc - the extra description
 */
nomask void
remove_extra_desc(string desc)
{
    extra_desc -= ({ desc });
}

/*
 * Function name: reset_extra_desc
 * Description:   removes all extra descriptions
 */
nomask void
reset_extra_desc()
{
    extra_desc = ({ });
}

/*
 * Function name: query_extra_desc
 * Description:   gives the extra description to the short
 * Returns:       the extra description
 */
nomask string *
query_extra_desc()
{
    return extra_desc;
}

/*
 * Function name: short
 * Description:   Returns the short-description of this living.
 *                Handles met/nonmet.
 * Arguments:     tp - acting player (optional)
 * Returns:       string, short description
 */
varargs string
short(object tp)
{
    string desc;

    if (!tp && !(tp = this_player()))
	tp = this_object();

    if (!(desc = tp->query_met(query_met_name())))
    {
	if (query_linkdead())
	    desc = "statue of " + LANG_ADDART(query_nonmet_name());
	else if (living(this_object()))
	    desc = query_nonmet_name();
	else
	    desc = "corpse of " + LANG_ADDART(query_race());
    }
    else
    {
	if (query_linkdead())
	    desc = "statue of " + capitalize(desc);
	else if (living(this_object()))
	    desc = capitalize(desc);
	else
	    desc = "corpse of " + LANG_ADDART(query_race());
    }
    if (query_prop(OBJ_INVIS))
	desc = "(" + desc + ")";
    return desc + (sizeof(extra_desc) ? implode(query_extra_desc(), "") : "");
}

/*
 * Function name: plural_short
 * Description:   Returns the plural short-description of this living.
 *                Handles met/nonmet.
 * Arguments:     tp - acting player (optional)
 * Returns:       string, plural short description
 */
varargs string
plural_short(object tp)
{
    string desc;

    if (!tp && !(tp = this_player()))
	tp = this_object();

    if (!(desc = tp->query_met(query_met_name())))
    {
	if (query_linkdead())
	    desc = "statues of " + LANG_PSENT(query_nonmet_name());
	else if (!living(this_object()))
	    desc = "corpses of " + LANG_PSENT(query_race());
	else
	    desc = LANG_PSENT(query_nonmet_name());
    }
    else
    {
	if (query_linkdead())
	    desc = "statues of " + LANG_PSENT(capitalize(desc));
	else if (!living(this_object()))
	    desc = "corpses of " + LANG_PSENT(query_race());
	else
	    desc = LANG_PSENT(capitalize(desc));
    }
    if (query_prop(OBJ_INVIS))
	desc = "(" + desc + ")";
    return desc + (sizeof(extra_desc) ? implode(query_extra_desc(), "") : "");
}

/*
 * Function name: long
 * Description:   Returns the long-description of this living
 * Returns:       The description string
 */
string
long()
{
    string res;
    object tp;

    if (!(tp = this_player()))
	tp = this_object();

    if (tp == this_object())
	res = "You are " + LANG_ADDART(query_nonmet_name()) +
	      ".\n" + query_presentation(tp) + ".\n";
    else
    {
	if (query_npc())
	    res = ::long();
	else if (tp->query_met(query_name()))
	{
	    res = (capitalize(query_pronoun()) + " is " +
		   LANG_ADDART(query_nonmet_name()) +
		   ", you know " + query_objective() + " as:\n" +
		   query_presentation(tp) + ".\n");
	}
	else
	{
	    if (query_linkdead())
		res = "statue of " + LANG_ADDART(query_nonmet_name());
	    else
		res = capitalize(LANG_ADDART(query_nonmet_name())) + ".\n";
	}
    }
    return res + extra_desc(tp);
}

/*
 * Function name: look_room
 * Description:   show the description of a room and its contents
 * Arguments:     room - the room tp looks at
 */
nomask void
look_room(object room)
{
    int    t, l, i, sz, count, sk, *co;
    string tmp, comp;
    object *ob_list;

    tmp = "";
    if (_ACCESS_LEVEL(geteuid()))
	tmp += "(file) " + room->query_room_file() + "\n(object) " +
	       object_name(room) + "\n";
    if ((t = room->query_type()) & TYPE_UNDER_WATER)
	tmp += "You are underwater.\n";
    if (!(query_msg_flags() & MSG_SEE))
    {
	catch_tell(tmp + "You fail to see anything.\n");
	return;
    }
    if (query_prop(LIVE_USING_SLA))
    {
	tmp += "You try to imagine your surroundings.\n";
	sk = query_skill_bonus(SK_SLA);
	co = query_coordinates();
	ob_list = I(room, 1);
	for (i=0, sz=count=sizeof(ob_list); i<sz; i++)
	{
	    if (!distance(co, ob_list[i]->query_coordinates(), sk))
		count--;
	    else if (random(sk) < random(20))
		count += random(3) - 1;
	}
	if (count > 0)
	    tmp += "You sense " + count + " object(s).\n";
	ob_list = I(room, 2) - ({ this_object() });
	for (i=0, sz=count=sizeof(ob_list); i<sz; i++)
	{
	    if (!distance(co, ob_list[i]->query_coordinates(), sk))
		count--;
	    else if (random(sk) < random(20))
		count += random(3) - 1;
	}
	if (count > 0)
	    tmp += "You sense " + count + " living(s) (without yourself).\n";
	catch_tell(tmp);
	return;
    }
    if ((l = room->query_light() + query_prop(LIVE_SEE_DARK)) < 1)
    {
        catch_tell(tmp + "You stand in total darkness.\n");
        return;
    }
    if (!(t & TYPE_INSIDE) && NIGHT)
	tmp += "It's night.\n";
    tmp += room->short() + ".\n";
    if (l > 14 && !query_prop(LIVE_SEE_SHORT_DESC))
        tmp += room->long();
    if (comp = COMPOSITE_ROOM_CONTENTS(room, this_object()))
	tmp += comp;
    catch_tell(tmp);
}
