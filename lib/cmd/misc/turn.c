/*
 * Function name: turn
 * Description:   turn an item (e.g. a wheel)
 * Arguments:     str - what to turn
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
turn(string str, object tp)
{
    int    i;
    string dir;
    object ob, *list;

    if (!str)
    {
	tp->catch_tell("Usage: turn <item> [<direction>]\n");
	return -1;
    }
    sscanf(str, "%s %s", str, dir);
    list = I(tp, 4);
    list = list + tp->visible(I(E(tp), 1));
    list = parse(str, list, tp, tp->query_step());
    if (!check_size(list, "No '" + str + "' in range to turn here.\n", tp))
	return -1;
    if (!list[0]->turn_cmd(dir))
    {
	tp->catch_tell("It cannot be turned.\n");
	return -1;
    }
    tp->change_int_prop(LIVE_LOST_ROUNDS, 1);
    return 1;
}
