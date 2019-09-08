inherit "humanoid";

#include <event.h>
#include <macros.h>
#include <shop.h>
#include <state.h>

static string shop;
static object client;

nomask void show_wares(string wares);

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
	    E(this_object())->keeper_talk("So, what can I do for you?\n"+
					  "May I show you some of my wares?",
					  client);
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
varargs void
catch_tell(string msg, int no_prompt)
{
    int i;
    string show_this;

    if (msg && sscanf(lower_case(msg),
		      "%*s in %*s:%*s show me %s\n", show_this) == 4)
	show_wares(show_this);
    ::catch_tell(msg, no_prompt);
}

/*
 * Function name: show_wares
 * Description:   looks for warenames and numbers, that appear in the string
 *                and shows their long desc to the person who asked
 * Arguments:     wares - a string with warenames in it
 */
nomask void
show_wares(string wares)
{
    int     i;
    string  actual_ware, *ind;
    object  shop, tp, shopkeeper;
    mapping shop_offer;
    
    shop = _ROOMD->load_room(query_shop());
    shop_offer = shop->query_offer();
    ind = map_indices(shop_offer);
    tp = this_player();
    shopkeeper = this_object();
    for (i=sizeof(ind); i--;)
    {
	actual_ware = shop_offer[ind[i]][OFFER_NAME];
	if (sscanf(wares, "%*s"+actual_ware+"%*s") ||
	    sscanf(wares, "%*s#"+(i+1)+"%*s"))
	{
	    command("say So you want to see the " + actual_ware + ".");
	    tell_objects(LISTEN(E(shopkeeper))-({ tp, shopkeeper }),
			 ({ QCNAME(shopkeeper), " shows a ", actual_ware,
				" to ", QNAME(tp), ".\n" }),
			 MSG_SEE, shopkeeper, 1);
	    tp->catch_msg(({ QCNAME(shopkeeper)," shows the ", actual_ware,
				 " to you:\n\n", ind[i]->long(), "\n" }),
			  shopkeeper, MSG_SEE, 1);
	}
    }
}
