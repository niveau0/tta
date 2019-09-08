inherit "/base/room";

#include <access.h>
#include <config.h>
#include <macros.h>
#include <move.h>
#include <slots.h>
#include <properties.h>

static int    finish, g_lev, hand;
static object pl;

void 
create_room()
{
    set_room_file(MASTER);
}
    
/* never save a player in this room */
int
query_no_save()
{
    return 1;
}

static void
leave_to_arda()
{
    int    x, y, z, *co;
    string loc;
    object ob;
    
    pl->undefine_primary("catch_all_cmd", this_object());

    if (!(loc = pl->query_last_location()) ||
	sscanf(loc, "%s:%d,%d,%d", loc, x, y, z) != 4 ||
	!(ob = _ROOMD->load_room(loc)))
    {
	loc = DEFAULT_START;
	if (!(ob = _ROOMD->load_room(loc)))
	{
	    pl->catch_tell("Something is wrong with the rooms.\n"+
			   "You will be removed from Arda to save your " +
			   "character from being damaged.\n", 1);
	    remove_object(pl);
	    remove_object();
	    return;
	}
	pl->catch_tell("You have been teleported.\n", 1);
	pl->save();
	co = ob->query_coordinates();
    }
    else
	co = ({ x, y, z });
    
    set_this_player(pl);
    pl->move_living(ob, co, "", 1);
    pl->start_call_out();
    remove_object();
}

int
catch_all_cmd(string cmd, string arg)
{
    if (!pl)
    {
	remove_object();
	return -1;
    }
    if (cmd == "quit")
    {
	remove_object(pl);
	remove_object();
	return 1;
    }
    if (finish != -1)
    {
	pl->catch_tell("Please be patient.\n");
	return -1;
    }
    if (cmd == "enter")
    {
	string loc;
	
	if (arg != "arda" && arg != "Arda")
	{
	    pl->catch_tell("Enter what? Try Arda.\n");
	    return -1;
	}
	leave_to_arda();
	return 1;
    }
    return 0;
}

static nomask void
finish()
{
    if (!pl)
    {
	remove_object();
	return;
    }
    if (g_lev)
    {
	leave_to_arda();
	return;
    }
    pl->look_room(this_object());
    pl->catch_tell("\nNow get your stuff and if you feel equipped "+
		   "you could 'enter Arda'.\nMake sure you forgot "+
		   "nothing.\n");
    finish = -1;
}

static nomask void
move_to_container(object ob, mixed inv)
{
    int    i, sz;
    string file;
    mixed  arg;
    object ob2;
    
    if (!pl)
    {
	remove_object();
	return;
    }
    
    finish--;
    if (!ob || !(sz = sizeof(inv)))
	return;
    
    for (i=0; i<sz; i++)
    {
	if (sizeof(inv[i]))
	{
	    if (!ob2) continue;
	    finish++;
	    call_out("move_to_container", 1, ob2, inv[i]);
	    continue;
	}
	if (i > 5)
	{
	    finish++;
	    call_out("move_to_container", 1, ob, inv[i..]);
	    return;
	}
	file = inv[i++];
	arg = inv[i];
	if (catch(ob2 = clone_object(file)))
	    continue;
	if (ob2->move(ob) != MOVE_OK)
	{
	    if (ob->attach_check(ob2))
		ob2->attach(ob);
	}
	if (catch(ob2->init_reload(arg)))
	    remove_object(ob2);
    }
    if (finish < 1)
	finish();
}

static nomask void
create_inventory(mixed inv)
{
    int    i, sz;
    string file;
    mixed  arg;
    object ob;

    if (!pl)
    {
	remove_object();
	return;
    }
    finish--;
    for (i=0, sz=sizeof(inv); i<sz; i++)
    {
	if (sizeof(inv[i]))
	{
	    if (!ob) continue; /* no container to move to, strange */
	    finish++;
	    call_out("move_to_container", 1, ob, inv[i]);
	    continue;
	}
	if (i > 5)
	{
	    finish++;
	    call_out("create_inventory", 1, inv[i..]);
	    return;
	}
	file = inv[i++];
	if (i >= sz)
	    break;
	arg = inv[i];
	if (catch(ob = clone_object(file)))
	    continue;
	if (g_lev) /* coder login */
	{
	    if (catch(ob->init_reload(arg)))
		remove_object(ob);
	    else if (ob)
	    {
		if (hand)
		    pl->add_tool(ob, hand);
		else
		    ob->move(pl);
	    }
	}
	else
	{
	    if (catch(ob->init_reload(arg)))
		remove_object(ob);
	    else if (ob)
	    {
		ob->set_coordinates(query_coordinates());
		ob->move(this_object());
	    }
	}
    }
    if (finish < 1)
	finish();
}

/*
 * Function name: load_inventory
 * Description:   load the inventory of a player after login
 * Arguments:     inv - the inventory filenames
 *                player - the login player
 */
nomask void
load_inventory(mixed inv, object player)
{
    int *hands;
    
    if (!player || player->query_npc())
    {
	remove_object();
	error("Illegal call to load_inventory");
    }
    pl = player;
    pl->define_primary("catch_all_cmd", this_object());
    if (!sizeof(inv))
    {
	leave_to_arda();
	return;
    }
    finish = 1;
    if ((g_lev = _ACCESS_LEVEL(geteuid(pl))) &&
	sizeof(hands = pl->query_slot_type(SLT_HAND)))
	hand = hands[0];
	
    create_inventory(inv);
}
