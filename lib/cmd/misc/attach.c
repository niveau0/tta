/*
 * Function name: attach
 * Description:   attach an item if possible
 * Arguments:     str - what to attach
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
attach(string str, object tp)
{
    string at;
    object item, *list;

    if (!str || sscanf(str, "%s to %s", str, at) != 2)
    {
	tp->catch_tell("Usage: attach <item> to <item>\n");
	return -1;
    }
    list = parse(str, tp->query_hold(SLT_ALL), 0);
    if (!check_size(list, "You are not holding any '" + str +
		    "' to attach.\n", tp))
	return -1;
    item = list[0];
    list = I(tp, 4) + tp->visible(I(E(tp), 1));
    list = parse(at, list, tp, tp->query_step());
    if (!check_size(list, "No '" + str + "' in range to attach " +
		    "something to.\n", tp))
	return -1;
    if (!list[0]->attach_cmd(item, tp))
    {
	tp->catch_tell("You cannot attach anything there.\n");
	return -1;
    }
    tp->change_int_prop(LIVE_LOST_ROUNDS, 1);
    return 1;
}
