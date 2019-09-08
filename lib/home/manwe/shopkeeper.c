inherit "humanoid";
// inherit "/base/misc/listen";

#include <event.h>
#include <macros.h>
#include <state.h>

static string shop;
static object client;

static nomask void
init_shopkeeper()
{
    init_humanoid();
    add_event(E_MOVE, "keeper_move", this_object());
    add_event(E_ARRIVED, "arrival", this_object());
}

/*
 * Function name: set_shop
 * Description:   set the shop path that belongs to this keeper
 * Arguments:     path - the shop room
 */
static nomask void
set_shop(string path)
{
    shop = path;
}

/*
 * Function name: query_shop
 * Description:   get the shop path that belongs to this keeper
 * Returns:       string, the shop room
 */
nomask string
query_shop()
{
    return shop;
}

nomask void
walk_to_client(object ob)
{
    client = ob;
    if (!client || E(this_object())->query_room_file() != shop ||
	E(client)->query_room_file() != shop || (query_state() & WALKING))
	return;
    
    command("walk " + client->query_name());
}

nomask void
arrival(object o, mixed dest, int flag, object block)
{
    if (client)
    {
	if (flag)
	    command("confused");
	else
	{
	    E(this_object())->keeper_talk("So, what can I do for you?", client);
	    client = 0;
	}
    }
}

nomask void
keeper_move(object ob, object from, object to)
{
    if (ob != this_object() || !to || to->query_room_file() != shop)
	return;
    to->set_keeper(this_object());
}

/*
 * Function name: catch_tell
 * Description:   let the shopkeeper react on speaking
 * Arguments:     msg - the msg spoken
 */
void
catch_tell(string msg)
{
    debug("seclan","im catch_tell");
    //if (!msg || sscanf(msg, "%*s in Quenya:  %s\n", told_message) != 2)
    //    return;
    // HIER ENDLOSSCHLEIFE !!!!!  command("say I heard:" + msg);
}
