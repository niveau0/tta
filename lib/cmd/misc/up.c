/*
 * Function name: up
 * Description:   special direction command (only on stairs, flying, etc)
 * Arguments:     str - object to go up (e.g. stairs)
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
up(string str, object tp)
{
    object ob, *list;
    
    if (!str)
    {
	if (!(ob = tp->query_prop(UPDOWN_OBJ)))
	{
	    /* check flying or climbing here */
	    tp->catch_tell("Usage: up <item>\n       e.g.: up stair\n");
	    return -1;
	}
    }
    else
    {
	list = tp->visible(I(E(tp), 1));
	list = parse(str, list, tp, tp->query_step());
	if (!check_size(list, "No '" + str + "' in range to walk up.\n", tp))
	    return -1;
	ob = list[0];
    }
    if (!ob->up_cmd(tp))
    {
	tp->catch_tell("You cannot go up there.\n");
	return -1;
    }
    return 1;
}
