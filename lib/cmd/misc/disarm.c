/*
 * Function name: disarm
 * Description:   disarm a person or disarm a trap
 * Arguments:     str - whom or what to disarm
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
disarm(string str, object tp)
{
    int    i;
    object ob, *list;

    if (!str)
    {
	tp->catch_tell("Usage: disarm <item|living>\n");
	return -1;
    }
    list = I(tp, 0) + tp->visible(I(E(tp), 0));
    list = parse(str, list, tp, tp->query_step());
    if (!check_size(list, "No '" + str + "' in range to disarm here.\n", tp))
	return -1;
    if (!living(list[0]))
    {
	if (!list[0]->disarm_cmd(tp))
	{
	    tp->catch_tell("It cannot be disarmed.\n");
	    return -1;
	}
    }
    else
    {
	tp->catch_tell("Not implemented yet.\n");
    }
    tp->change_int_prop(LIVE_LOST_ROUNDS, 1);
    return 1;
}
