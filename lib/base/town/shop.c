inherit "../room";
inherit "/system/parse";

#include <composite.h>
#include <event.h>
#include <language.h>
#include <living.h>
#include <macros.h>
#include <money.h>
#include <shop.h>
#include <skills.h>
#include <slots.h>
#include <properties.h>

mapping offer;

private int     production_time;
private object  keeper;
private mapping traders, names;

/*
 * Function name: create_shop
 * Description:   redefinable create function
 */
static void
create_shop()
{
}

/*
 * Function name: create_room
 * Description:   std constructor
 */
static nomask void
create_room()
{
    add_event(E_MOVE_INV, "E_shop_move", this_object());
    traders = ([ ]);
    create_shop();
    if (!offer)
	offer = ([ ]);
    if (!names)
	names = ([ ]);
}

/*
 * Function name: load
 * Description:   load room and init keeper
 */
varargs void
load()
{
    int    i;
    string *ind;
    
    ::load();

    if (!offer)
	offer = ([ ]);
    else
    {
	names = ([ ]);
	ind = map_indices(offer);
	for (i=sizeof(ind); i--;)
	    names[offer[ind[i]][OFFER_NAME]] = ind[i];
    }
}

/*
 * Function name: E_shop_move
 * Description:   event: notice movement in shop
 * Arguments:     ob - the moving object
 *                from - from where it comes
 *                to - where it goes
 */
nomask void
E_shop_move(object ob, object from, object to)
{
    int    i, sz, t, prod;
    string *files;
    mixed  cur;

    if (!keeper || E(keeper) != this_object())
	/* no shopkeeper -> no production */
	return;
    if (time() < production_time)
	return;
    production_time = time() + 3600;
    if (sz = sizeof(files = map_indices(offer)))
    {
	t = time();
	for (i=0; i<sz; i++)
	{
	    if (!(cur = offer[files[i]]) || !cur[OFFER_TIME] ||
		cur[OFFER_COUNT] >= cur[OFFER_MAX] ||
		!(prod = (t - cur[OFFER_STAMP]) / cur[OFFER_TIME]))
		continue;
	    prod += cur[OFFER_COUNT];
	    if (prod > cur[OFFER_MAX])
		offer[files[i]][OFFER_COUNT] = cur[OFFER_MAX];
	    else
		offer[files[i]][OFFER_COUNT] = prod;
	    offer[files[i]][OFFER_STAMP] = t;
	}
    }
}

/*
 * Function name: set_keeper
 * Description:   who is the keeper of this shop? should only be called by the
 *                npc shopkeeper
 * Arguments:     k - the object pointer of the keeper
 */
nomask void
set_keeper(object k)
{
    keeper = k;
}

/*
 * Function name: query_keeper
 * Description:   gives keeper object
 * Return:        object
 */
nomask object
query_keeper()
{
    return keeper;
}

/*
 * Function name: keeper_talk
 * Description:   let the shopkeeper talk in players language if the keeper is
 *                skilled enough
 * Arguments:     str - sentence to say
 *                tp - player to talk to
 */
nomask void
keeper_talk(string str, object tp)
{
    int lang;
    
    if (!keeper)
	return;
    if (tp)
    {
	lang = tp->query_prop(LIVE_USED_LANGUAGE);
	if (keeper->query_skill_rank(lang) > 5)
	    keeper->add_prop(LIVE_USED_LANGUAGE, lang);
    }
    keeper->command("say " + str);
}

/*
 * Function name: calc_price
 * Description:   calculate shops price, depends on supply and demand
 * Arguments:     orig - original price value
 *                count - current amount
 *                max - maximum amount
 * Returns:       the shops price
 */
int
calc_price(int orig, int count, int max)
{
    int   price;
    float c, m;

    c = (float)(max - count);
    m = (float)max;
    
    /* calculate supply and demand */
    price = (int)((float)orig * ((c * c * c) / (m * m * m) + 1.0));
    
    return price + 1;
}

/*
 * Function name: sell_filter
 * Description:   check if items can be sold here
 * Arguments:     items - the items player wants to sell
 *                tp - the trader
 * Returns:       array of objects allowed to sell
 */
object *
sell_filter(object *items, object tp)
{
    int    i, flag;
    string out;
    object ob;

    out = "";
    for (i=sizeof(items); i--;)
    {
	ob = items[i];
	if (ob->query_coin_type())
	{
	    items[i] = 0;
	    continue;
	}
	if (!ob->query_value())
	{
	    items[i] = 0;
	    out += "That " + ob->short() + " has no value for me. ";
	}
	if (ob->query_prop(IS_WEAPON) || ob->query_prop(IS_WEARABLE))
	{
	    items[i] = 0;
	    if (!flag)
	    {
		flag = 1;
		out += "I don't trade with cloth, armor or weapons. ";
	    }
	}
    }
    if (out)
	keeper_talk(out, tp);
    return items - ({ 0 });
}

/*
 * Function name: buy_trade
 * Description:   modify price by trading
 * Arguments:     price - the price to trade with
 *                tp - the trading player
 * Returns:       the new price modifier (in %)
 */
static int
buy_trade(int price, object tp)
{
    int trade, sk;
    
    sk = 0;
    switch (price)
    {
      case 0..10000:
	break;
      case 10001..100000:
	sk -= 10;
	break;
      case 100001..500000:
	sk -= 40;
	break;
      case 500001..1000000:
	sk -= 75;
	break;
      default:
	sk -= 100;
    }

    trade = 200 - tp->skill_check(SK_TRADING, -sk, 0) +
	    keeper->skill_check(SK_TRADING,
				-tp->query_stat_bonus(STAT_AP), 0);
    if (trade < 70)
	trade = 70;
    else if (trade > 200)
	trade = 200;

    return trade;
}

/*
 * Function name: sell_trade
 * Description:   modify price with trading
 * Arguments:     price - the price to trade with
 *                tp - the trading player
 * Returns:       the new price modifier (in %)
 */
static int
sell_trade(int price, object tp)
{
    int trade, sk;
    
    switch (price)
    {
      case 0..10000:
	sk -= 10;
	break;
      case 10001..100000:
	sk -= 25;
	break;
      case 100001..500000:
	sk -= 50;
	break;
      case 500001..1000000:
	sk -= 75;
	break;
      default:
	sk -= 100;
    }
    
    trade = 70 + tp->skill_check(SK_TRADING, -sk, 0) -
	    keeper->skill_check(SK_TRADING,
				-tp->query_stat_bonus(STAT_AP), 0);
    
    if (trade > 150)
	trade = 150;
    else if (trade < 25)
	trade = 25;

    return trade;
}

/*
 * Function name: in_shop
 * Description:   check if an item is available
 * Arguments:     str - item name or number in list (prefix must be '#')
 * Returns:       filename of wanted item if available
 */
nomask string
in_shop(string str)
{
    int    num;
    string *files;
    
    if (!str || !strlen(str))
    	return 0;
    if (str[0] == '#')
    {
        sscanf(str[1..], "%d", num);
        if (!num)
            return 0;
        files = map_indices(offer);
        if (sizeof(files) < num)
            return 0;
        if (!offer[files[--num]][OFFER_COUNT])
            return 0;
        return files[num];
    }
    if (!(str = names[str]))
    	return 0;
    if (!offer[str][OFFER_COUNT])
        return 0;
    return str;
}

/*
 * Function name: add_offer
 * Description:   add an item to the shops offer
 * Arguments:     file - the file name
 *                count - current amount
 *                max - how many of this is maximum production
 *                t - how long does it take to produce the item
 */
nomask void
add_offer(string file, int count, int max, int t)
{
    string name;

    if (max < 1)
    	return;

    name = file->short();

    offer[file] = ({ count, name, max, t, time() });
    names[name] = file;
}

/*
 * Function name: remove_offer
 * Description:   remove an item totally from shops offer
 * Arguments:     file - what item (filename)
 */
nomask void
remove_offer(string file)
{
    if (offer[file])
    {
	names[offer[file][OFFER_NAME]] = 0;
	offer[file] = 0;
    }
}

/*
 * Function name: query_offer
 * Description:   gives offer map
 * Return:        mapping
 */
nomask mapping
query_offer()
{
    return offer;
}

/*
 * Function name: list_offer
 * Description:   list the shops offer
 * Returns:       the complete list as string
 */
nomask string
list_offer()
{
    int    i, sz;
    string list, *files;
    mixed  cur;

    list = ("\n==========================================="+
    	    "===================================\n"+
	    " Available                       Quantity  Price\n"+
    	    "==========================================="+
    	    "===================================\n");

    for (i=0, sz=sizeof(files = map_indices(offer)); i<sz; i++)
    {
	cur = offer[files[i]];
	if (!cur[OFFER_COUNT])
	    continue;
    	list += sprintf("#%-2d %_/-32s__%_/3d  %-36s\n",
			i+1,
			cur[OFFER_NAME],
			cur[OFFER_COUNT],
			MONEY_SHORT_DESC(calc_price(files[i]->query_value(),
						    cur[OFFER_COUNT],
						    cur[OFFER_MAX])));
    }
    return list;
}

/*
 * Function name: buy_cmd
 * Description:   buy item(s)
 * Arguments:     str - the item name or number in list
 * Returns:       1 if success, -1 else
 */
int
buy_cmd(string str)
{
    int    i, tp_money, price, num, trade, dropped, *hands, *cotp, *cokeeper;
    string file, amount;
    object tp, ob;
    mixed  cur;

    tp = this_player();

    if (!keeper || E(keeper) != this_object())
    {
        tp->catch_tell("The shopkeeper is not here.\n");
        return -1;
    }
    if (!VISIBLE(keeper, tp))
    {
        keeper_talk("Hey! Who's there?", tp);
        return -1;
    }
    if (traders[tp] == -1)
    {
        keeper_talk("Go away, leave my shop!", tp);
        return -1;
    }
    if (!distance(cotp = tp->query_coordinates(),
		  cokeeper = keeper->query_coordinates(), tp->query_step()))
    {
	keeper_talk("Then come over here!", tp);
	return -1;
    }
    if (!distance(cotp, cokeeper, keeper->query_step()))
    {
	keeper->walk_to_client(tp);
	return -1;
    }
    sscanf(str, "%s of %s", amount, str);
    
    tell_objects(LISTEN(this_object()) - ({ tp }),
		 ({ QCTNAME(tp), " is trading with the shopkeeper.\n" }),
		 MSG_HEAR, tp, 400);
    
    if (!(file = in_shop(str)))
    {
        keeper_talk("I'm sorry, but we don't offer such an item.", tp);
        return -1;
    }
    cur = offer[file];
    if (amount == "all")
    	num = cur[OFFER_COUNT];
    else
    {
        if (amount)
      	    sscanf(amount, "%d", num);
        else
    	    num = 1;
        if (num < 1)
        {
            keeper_talk("Huh? How much, please?", tp);
            return -1;
        }
        if (num > cur[OFFER_COUNT])
        {
            keeper_talk("Oh I don't have so many of that kind.", tp);
            return -1;
        }
    }
    price = calc_price(file->query_value(), cur[OFFER_COUNT], cur[OFFER_MAX]);
    tp_money = MONEY_TOTAL(tp);

    if (tp->query_prop(LIVE_TRADING))
    {
	tp->catch_tell("\nAfter a short trading practice...\n\n");
	
	trade = buy_trade(price, tp);
	if (trade > 100)
	{
	    traders[tp] = -1;
	    switch (trade)
	    {
	      case 101..120:
		  keeper_talk("You fool. This is ridiculous!", tp);
		  return 1;
	      case 121..150:
		  keeper_talk("More of that and I'll drop you outta here.",
			      tp);
		  return 1;
	      default:
		  keeper_talk("Pah, go away or I call some buddies!", tp);
		  return 1;
	    }
	}
	price = trade * price / 100 + 1;
    }
    if (tp_money < num * price)
    {
	tp->catch_tell("You notice, you don't have the money.\n");
	return -1;
    }
    keeper_talk("Thank you. Ok, let's see what we have here...", tp);
    hands = keeper->query_slot_type(SLT_HAND);
    if (!sizeof(hands))
    {
	for (i=0; i<num; i++)
	{
	    ob = clone_object(file);
	    ob->add_prop(OBJ_BUYER, tp);
	    dropped++; 
	    ob->set_coordinates(cokeeper);
	    ob->move(this_object());
	}
    }
    else
    {
	for (i=0; i<num; i++)
	{
	    ob = clone_object(file);
	    ob->add_prop(OBJ_BUYER, tp);
	    if (keeper->add_tool(ob, hands[0]))
	    {
		dropped++; 
		ob->set_coordinates(cokeeper);
		ob->move(this_object());
	    }
	}
    }
	
    offer[file][OFFER_COUNT] -= num;
    MONEY_CHANGE(-price, tp);
    tp->catch_tell("You pay " + MONEY_TEXT(price) + ".\n");
    
    if (dropped > 0)
	tell_objects(I(this_object(), 2),
		     ({ QCTNAME(keeper), " drops " + dropped + " item" +
			    (dropped > 1 ? "s" : "") + ".\n" }),
		     MSG_SEE, keeper);
    
    num -= dropped;
    if (num > 0)
    {
	keeper->command("give " + num + " " +
			(num > 1 ? ob->query_pname() : ob->query_name()) +
			" to " + tp->query_name());
	call_out("drop_bought_stuff", 20, tp);
    }
    return 1;
}

static nomask void
drop_bought_stuff(object tp)
{
    int    i, dropped, *cokeeper;
    object ob, *inv;

    if (!keeper || !living(keeper))
	return;
    
    cokeeper = keeper->query_coordinates();
    inv = I(keeper, 7);
    for (i=sizeof(inv); i--;)
    {
	if ((ob = inv[i])->query_prop(OBJ_BUYER) == tp)
	{
	    ob->set_coordinates(cokeeper);
	    ob->move(this_object());
	    dropped++;
	}
    }
    if (dropped > 0)
	tell_objects(I(this_object(), 2),
		     ({ QCTNAME(keeper), " drops " + dropped + " item" +
			    (dropped > 1 ? "s" : "") + ".\n" }),
		     MSG_SEE, keeper);
}

/*
 * Function name: sell_cmd
 * Description:   sell item(s)
 * Arguments:     str - the item name(s)
 * Returns:       1 if success, -1 else
 */
int
sell_cmd(string str)
{
    int     i, sz, price, trade, is_trading, gained, value, *cotp, *cokeeper;
    string  file, short, *sold;
    object  tp, ob, *items;
    mixed   cur;

    tp = this_player();
    if (!keeper || E(keeper) != this_object())
    {
        tp->catch_tell("The shopkeeper is not here.\n");
        return -1;
    }
    if (!VISIBLE(keeper, tp))
    {
        keeper_talk("Hey! Who's there?", tp);
        return -1;
    }
    if (traders[tp] == -1)
    {
        keeper_talk("Leave me alone!", tp);
        return -1;
    }
    if (!distance(cotp = tp->query_coordinates(),
		  cokeeper = keeper->query_coordinates(), tp->query_step()))
    {
	keeper_talk("Then come over here!", tp);
	return -1;
    }
    if (!distance(cotp, cokeeper, keeper->query_step()))
    {
	keeper->walk_to_client(tp);
	return -1;
    }

    tell_objects(LISTEN(this_object()) - ({ tp }),
		 ({ QCTNAME(tp), " is trading with the shopkeeper.\n" }),
		 MSG_HEAR, tp, 400);
    
    items = keeper->visible(tp->query_hold(SLT_ALL));
    items = parse(str, items, 0);
    if (!sizeof(items))
    {
        keeper_talk("Err, sorry, I don't see that.", tp);
        return -1;
    }
    items = sell_filter(items, tp);
    if ((sz = sizeof(items)) > 10)
    {
	keeper_talk("Oops, not that much, please. Let's first handle "+
		    "a part of those.", tp);
	sz = 10;
    }
    if (is_trading = tp->query_prop(LIVE_TRADING))
	tp->catch_tell("\nAfter a short trading practice...\n\n");
    
    for (i=0, sold=allocate(sz); i<sz; i++)
    {
	ob = items[i];
	file = MASTER_OB(ob);
	if (cur = offer[file])
	    price = calc_price(ob->query_value(), cur[OFFER_COUNT] + 1,
			       cur[OFFER_MAX]);
	else
	    price = calc_price(ob->query_value(), 1, 20);

	/* care for profit. Selling may give 50% more and buying may be
	 * 30% cheaper. Keeper offers a price so selling and buying of the
	 * same item repeatedly gives no profit.
	 */
	price = price * 45 / 100;

	if (is_trading)
	{
	    trade = sell_trade(price, tp);
	    price = trade * price / 100;
	}
	short = ob->short();
	if (!price)
	{
	    keeper_talk("The " + short + " is worthless for me.", tp);
	    continue;
	}
	if (offer[file])
	    offer[file][OFFER_COUNT]++;
	else
	    add_offer(file, 1, 20, 0);
	
	sold[i] = LANG_ADDART(short);
	gained += price;
	remove_object(ob);
    }
    sold -= ({ 0 });
    if (gained)
    {
	keeper_talk("Ok, thank you.", tp);
	MONEY_CHANGE(gained, tp);
	tp->catch_msg(({ "You gained " + MONEY_TEXT(gained) + " and sold " +
			     COMPOSITE_WORDS(sold) + ".\n" }), 0, 0);
    }
    else
	tp->catch_tell("You sold nothing.\n");
    return 1;
}
