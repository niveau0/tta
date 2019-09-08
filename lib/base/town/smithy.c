/* the smith should deal only with weapon and armor */

inherit "shop";

/*
 * Function name: create_smithy
 * Description:   redefinable create function
 */
static void
create_smithy()
{
}

/*
 * Function name: create_shop
 * Description:   std constructor
 */
static nomask void
create_shop()
{
    create_smithy();
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
	if (!ob->query_prop(IS_WEARABLE) && !ob->query_prop(IS_WEAPON))
	{
	    items[i] = 0;
	    if (!flag)
	    {
		flag = 1;
		out += "I only trade with cloth, weapons and armor. ";
	    }
	}
    }
    if (out)
	keeper_talk(out, tp);
    return items - ({ 0 });
}
