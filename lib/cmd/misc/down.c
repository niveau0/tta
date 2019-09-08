/*
 * Function name: down
 * Description:   special direction command (only on stairs, flying, etc)
 * Arguments:     str - object to go down (e.g. stairs)
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
down(string str, object tp)
{
    object ob, *list;
    
    if (!str)
    {
	if (!(ob = tp->query_prop(UPDOWN_OBJ)))
	{
	    /* check flying or climbing here */
	    tp->catch_tell("Usage: down <item>\n       e.g.: down stairs\n");
	    return -1;
	}
    }
    else
    {
	list = tp->visible(I(E(tp), 1));
	list = parse(str, list, tp, tp->query_step());
	if (!check_size(list, "No '" + str + "' in range to walk down.\n", tp))
	    return -1;
	ob = list[0];
    }
    if (!ob->down_cmd(tp))
    {
	tp->catch_tell("You cannot go down there.\n");
	return -1;
    }
    return 1;
}
