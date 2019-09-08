inherit "living";
inherit "player/consume";
inherit "player/extra";
inherit "player/met";
inherit "player/skill";
inherit "player/slot";

#include <access.h>
#include <event.h>
#include <effects.h>
#include <living.h>
#include <language.h>
#include <macros.h>
#include <mudtime.h>
#include <config.h>
#include <properties.h>

string     name, *adj;     /* player name and adjectives */
mapping    saved_props;    /* the saved properties */

private int     is_linkdead;
private string previous_cmd; /* the last command (to repeat it) */

/*
 * Function name: create_living
 * Description:   init variables if necessary
 */
static void
create_living()
{
    adj = ({ });
    saved_props = ([ ]);
}

/*
 * Function name: set_name
 * Description:   Name of the player. Called only at death or creation.
 * Arguments:     n - the name
 */
nomask void
set_name(string n)
{
    string poname;
    
    if ((poname = MASTER_OB(previous_object())) != CREATE_PLAYER &&
	poname != _SYSTEMD) 
	error("Illegal call to set_name");
    name = n;
    ::set_name(n);
}

/*
 * Function name: add_adj
 * Description:   adjectives of the player.
 * Arguments:     a - adjective
 */
nomask void
add_adj(string a)
{
    string poname;
    
    if ((poname = MASTER_OB(previous_object())) != CREATE_PLAYER &&
	poname != _SYSTEMD) 
	error("Illegal call to add_adj");
    adj += ({ a });
    ::add_adj(a);
}

/*
 * Function name: init_saved_props
 * Description:   add the values of all saved properties
 */
static nomask void
init_saved_props()
{
    int   i;
    mixed ind;

    if (!saved_props)
	return;
    ind = map_indices(saved_props);
    for (i=sizeof(ind); i--;)
	add_prop(ind[i], saved_props[ind[i]]);
    saved_props = 0;
}

/*
 * Function name: fix_saved_props
 * Description:   construct the mapping of saved properties
 */
static nomask int
fix_saveprop_list()
{
    int   i;
    mixed ind;
    
    ind = SAVE_PROPS;
    saved_props = ([ ]);
    for (i=sizeof(SAVE_PROPS); i--;)
	saved_props[ind[i]] = query_prop(ind[i]);
}

/*
 * Function name: enter_game
 * Description:   Enter the player into the game
 */
static nomask void
enter_game()
{
    int     i, j, *e;
    object  ob;
    mapping map, effects;
    mixed   *files;

    ::set_name(name);
    add_name(query_race());
    for (i=sizeof(adj); i--;)
	::add_adj(adj[i]);
    
    if ((i = _ACCESS_LEVEL(geteuid())) > 0)
    {
	if (i == _BUILDER)
	{
	    if (query_gender() == G_FEMALE)
		add_name("maie");
	    else
		add_name("maia");
	}
	else
	{
	    if (query_gender() == G_FEMALE)
		add_name("valie");
	    else
		add_name("vala");
	    add_event(E_MOVE_INV, "E_move_inv", this_object());
	}
    }
    
    if (MASTER == DEAD_SOUL)
	set_living_name(name + ".dead");
    else
    {
	slot_player_reset();
	set_living_name(name);
    }
    
    init_saved_props();
    
    if (catch(ob = clone_object(LOGIN_ROOM)))
    {
	catch_tell("There seems to be a login problem. Please use the "+
		   "'guest' character\nand try to contact an ainu.\n", 1);
	remove_object();
	return;
    }
    ob->load();
    set_coordinates(ob->query_coordinates());
    init_msg_flags();
    move(ob);
    catch_tell("\n...please wait, reloading your character...\n", 1);
    change_ticks(ob, 10000000, "load_inventory", query_auto_load(),
		 this_object());
    
    if (MASTER == DEAD_SOUL)
    {
	set_login_time();
	set_login_from();
	return;
    }
    if ((i = query_login_time()) > 0)
    {
	add_prop(RESTING_START, i); 
	LEARN_OB->improve_skills(this_object());
	if ((i = time() - i) > 0)
	    add_active_time(i / 10);
    }
    set_login_time();
    set_login_from();
    
    effects = query_effects();
    catch
    {
	for (i=sizeof(e = map_indices(effects)); i--;)
	{
	    files = map_indices(map = effects[e[i]]);
	    for (j=sizeof(files); j--;)
		if (stringp(files[j]))
		    files[j]->restart_effect(this_object(), map[files[j]]);
	}
    }
}

/*
 * Function name: start_call_out
 * Description:   starts save and consume call_out after login was successful
 */
nomask void
start_call_out()
{
    if (!query_prop(CODER_NO_CONSUME))
    {
	if (find_call_out("consume") == -1)
	    call_out("consume", CONSUME_TIME);
    }
    if (find_call_out("autosave") == -1)
	call_out("autosave", AUTOSAVE_TIME);
    /* start a possible blood event after login */
    call_out("bloodloss_event", 2); 
}

/*
 * Function name: stop_call_out
 * Description:   stops save and consume call_out after login out
 */
static nomask void
stop_call_out()
{
    remove_call_out("consume");
    remove_call_out("autosave");
}

/*
 * Function name: command
 * Description:	  Makes the player execute a command. Handles alias expanding.
 * Arguments:	  cmd - string containing the command
 * Returns:	  integer, 1 success, -1 failed, 0 no such command
 */
nomask int
command(string cmd)
{
    int    status, result;
    string arg, prompt;
    object pobj, oplayer, user;
    
    if (!cmd || !this_object() || !living(this_object()))
	return 0;
    
    pobj = previous_object();
    user = query_user();
    /* execute command if called by user or called by object with
     * same euid or if called by a higher leveled object
     */
    if (pobj && pobj != user && geteuid(pobj) != geteuid() &&
	_ACCESS_LEVEL(geteuid()) && !_ACCESS_FORCE(pobj, this_object()))
	return 0;
    
    if (cmd == "$")
    {
	if (!(cmd = previous_cmd))
	    cmd = "";
    }
    sscanf(cmd, "%s %s", cmd, arg);
    if (arg)
	cmd = resolve_alias(cmd) + " " + arg;
    else
	cmd = resolve_alias(cmd);
    sscanf(cmd, "%s %s", cmd, arg);
    if (!strlen(cmd))
    {
	prompt = query_prompt();
	catch_tell(prompt ? prompt : "> ", 1);
	return 1;
    }
    
    if (arg)
	previous_cmd = cmd + " " + arg;
    else
	previous_cmd = cmd;

    oplayer = this_player();
    set_this_player(this_object());

    _SYSTEMD->user_command();
    user->set_interactive(1);

    if (result = action(cmd, arg)) /* cmd successful? */
    {
	set_this_player(oplayer);
	if (!this_object() || !user)
	    return result;
	user->set_interactive(0);
	if (status = user->query_status())
	{
	    /* input_to or editor, then stop processing of cmdqueue */
	    stop_cmdqueue();
	    if (status == 1)
		catch_tell(":", 1);
	}
	else
	{
	    prompt = query_prompt();
	    catch_tell(prompt ? prompt : "> ", 1);
	}
	return result;
    }
    
    set_this_player(oplayer);
    if (!this_object()) 
	return result;
    user->set_interactive(0);
    catch_tell("It's not possible to '" + cmd + "'.\n" +
	       (prompt ? prompt : "> "), 1);
    return 0;
}

/*
 * Function name: E_move_inv
 * Description:   event: notify creators if something enters the inventory
 * Arguments:     ob - what object
 *                from - where it comes from
 *                to - where it goes
 */
nomask void
E_move_inv(object ob, object from, object to)
{
    if (ob && from != to && to == this_object() &&
	_ACCESS_LEVEL(geteuid()) > _BUILDER)
	catch_tell("* inventory: " + object_name(ob) + " *\n");
}

nomask mixed *
get_reload_data(object *obj)
{
    int    i;
    object *inv, *att;
    mixed  *load;
    
    load = ({ });
    for (i=sizeof(obj); i--;)
    {
	load += ({ MASTER_OB(obj[i]), obj[i]->reload_data() });
	inv = I(obj[i], 7);
	if (att = obj[i]->attached())
	    inv += att;
	if (sizeof(inv))
	    load += ({ get_reload_data(inv) });
    }
    return load;
}

/*
 * Function name: save
 * Description:   function to call for saving a player
 */
nomask void
save()
{
    string file;
    object *inv, *att;
    mixed  *reload;
    
    if (!name)
	error("Illegal call to save");
    if (E(this_object()) && E(this_object())->query_no_save())
	return;
    rlimits(500; 10000000)
    {
	set_last_location();
	fix_saveprop_list();
	
	inv = I(this_object(), 7);
	if (att = attached())
	    inv += att;
	reload = get_reload_data(inv);
	set_auto_load(reload);

	if (MASTER == DEAD_SOUL)
	    file = PLAYER_FILE(name) + ".dead";
	else
	    file = PLAYER_FILE(name);
	rename_file(file + ".o", file + ".bak");
	save_object(file);
	remove_file(file + ".bak");
	
	saved_props = 0;
    }
}

/*
 * Function name: autosave
 * Description:   cares for autosaving
 */
static nomask void
autosave()
{
    call_out("autosave", AUTOSAVE_TIME);
    if (query_user())
    {
	if (query_idle(this_object()) < AUTOSAVE_TIME / 2)
	    add_active_time(AUTOSAVE_TIME);
	else
	    add_active_time(AUTOSAVE_TIME / 10);
    }
    if (query_active_time() > IMPROVE_TIME)
    {
	improve_resistance();
	improve_stats();
	calc_dev_points();
	debug("manwe", "improved " + query_name() + "'s stats");
	reset_active_time();
    }
    save();
}

/*
 * Function name: load
 * Description:   function to call to load a player
 *                only allowed if called by master (used in finger_player)
 * Arguments:     n - name of player to load
 */
nomask void
load(string n)
{
    if (previous_program() != _SYSTEMD || !n)
	error("Illegal call to load");
    restore_object(PLAYER_FILE(n));
    ::set_name(name);
}

/*
 * Function name: start_new_player
 * Description:   called after creation of a new player
 */
nomask void
start_new_player()
{
    if (MASTER_OB(previous_object()) != CREATE_PLAYER || !name)
	error("Illegal call to start_new_player");

    set_birth(time());
    calc_dev_points(); /* called again, double development points at start */
    _SYSTEMD->add_race(query_race()); /* race statistics */
    save();
}

/*
 * Function name: start_player
 * Description:   called if an existing player enters the game
 * Arguments:     n - the players name
 */
void
start_player(string n)
{
    if (MASTER_OB(previous_object()) != _LOGIN || !n)
	error("Illegal call to start_player");

    restore_object(PLAYER_FILE(n));
    enter_game();
}

/*
 * Function name: quit
 * Description:   local quit function, so player can quit if cmdsouls bug
 */
static nomask int
quit()
{
    if (::quit()) /* try to break a possession first */
	return 1;

    catch_tell("Saving " + query_name() + ".\n", 1);
    if (_ACCESS_LEVEL(geteuid()) > 0)
    {
	save(); /* mortals are saved in set_linkdead */
	tell_objects(LISTEN(E(this_object())) - ({ this_object() }),
		     ({ QCTNAME(this_object()), " disappears.\n" }),
		     MSG_SEE, this_object());

	remove_object();
    }
    else if (query_user())
	query_user()->close();
    return 1;
}

/*
 * Function name: query_presentation
 * Description:   Gives a presentation of the living in one line. Including
 *                name, race, guild titles and experience level.
 *                This should only be displayed to met players.
 * Arguments:     tp - the living that wants to know presentation string
 * Returns:       The description string
 */
nomask string
query_presentation(object tp)
{
    if (!_ACCESS_LEVEL(geteuid()))
	return ::query_presentation(tp);
    return (_SYSTEMD->query_coder_pretitle(name) + " " +
	    ::query_presentation(tp));
}

/*
 * Function name:   query_met(string name)
 * Description:     Tells if we know a certain living's name.
 * Arguments:       name - Name of living
 * Returns:         the remembered name
 */
nomask string
query_met(string name)
{
    if (!name || coder_unmet)
	return 0;
    if (name == query_name())
	return name;
    /* Coders know everyone, unless they explicitely do not want to */ 
    if (_ACCESS_LEVEL(geteuid()) > 0)
	return name;      
    return query_remembered()[name];
}


/*
 * Function name: set_user
 * Description:   set current user for this object, here for security
 * Arguments:     u - the user object
 */
nomask void
set_user(object u)
{
    if (u && u->query_player() != this_object())
	return;
    ::set_user(u);
}

/*
 * Function name: set_linkdead
 * Description:   cares for linkdeath message and possession if link gets lost
 * Arguments:     i - true or false
 */
nomask void
set_linkdead(int i)
{
    string loc;
    object old, env, possessed, *all;

    if (i)
    {
	if (is_linkdead || find_call_out("delay_linkdead") != -1)
	    return;
	add_prop(STATUE_TIME, time());
	call_out("delay_linkdead", 10);
	if ((env = query_user()) && (env = env->query_splayer()))
	    /* lost link breaks possession on players */
	    break_possession(this_object(), env);
	save();
	return;
    }
    if (is_linkdead)
    {    
	if ((i = query_prop(STATUE_TIME)) > 0)
	{
	    add_prop(RESTING_START, i); 
	    LEARN_OB->improve_skills(this_object());
	    if ((i = time() - i) > 0)
		add_active_time(i / 10);
	}
	if (loc = query_prop(STATUE_LOCATION))
	{
	    if (!(old = _ROOMD->load_room(loc)))
	    {
		catch_tell("Ouch, your old location made problems, " +
			   "please login as 'guest'.\n");
		return;
	    }
	    env = E(this_object());
	    move(old);
	    destruct_object(env);
	
	    all = LISTEN(old) - ({ this_object() });
	    tell_objects(all, "Suddenly the ground starts to tremble. As "+
			 "you look down you see the head\nof a statue "+
			 "appearing from the ground, then the body "+
			 "and finally the legs.\n", MSG_SEE, this_object());
	    add_prop(STATUE_LOCATION, 0);
	}
	start_call_out();
	enable_commands();
	add_prop(CONT_CLOSED, 0);
	is_linkdead = 0;
	
	if (!all)
	    all = LISTEN(E(this_object())) - ({ this_object() });
	tell_objects(all, ({ "Suddenly the statue of ", QNAME(this_object()),
			     " turns alive.\n" }), MSG_SEE, this_object());
    }
    if ((possessed = query_prop(LIVE_POSSESSED)) &&
	possession(possessed, this_object()))
	look_room(E(possessed));
    else
	look_room(E(this_object()));
}

/*
 * Function name: move_statue
 * Description:   move linkdead players into a statue room
 */
static nomask void
move_statue()
{
    object statue_room;
    
    if (!is_linkdead)
	return;
    
    catch(statue_room = clone_object(STATUE_ROOM));
    if (statue_room)
    {
	add_prop(STATUE_LOCATION, E(this_object())->query_room_file());
	statue_room->load();
	tell_objects(LISTEN(E(this_object())) - ({ this_object() }),
		     ({ QCTNAME(this_object()), " crumbles to dust.\n" }),
		     MSG_SEE, this_object());
	move(statue_room);
    }
}

static nomask void
delay_linkdead()
{
    if (is_linkdead || query_ip_number(this_object()))
	return;
    
    if (E(this_object()))
    {
	tell_objects(LISTEN(E(this_object())) - ({ this_object() }),
		     ({ QCTNAME(this_object()),
			    " turns to stone.\n" }), MSG_SEE, this_object());
    }
    is_linkdead = 1; /* must be after message */
    disable_commands();
    add_prop(CONT_CLOSED, 1);
    stop_call_out();
    call_out("move_statue", 60);
}

nomask int
query_linkdead()
{
    return is_linkdead;
}
