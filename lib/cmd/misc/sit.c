/*
 * Function name: sit
 * Description:   Sit down or on an item (e.g. a chair)
 * Arguments:     str - where to sit on
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
sit(string str, object tp)
{
    string prep, where;
    object here, *obj;
    
    if (!str || str == "down")
	return rest("", tp);

    if (sscanf(str, "%s %s", prep, where) != 2 || prep != "on")
    {
	tp->catch_tell("Usage: sit on <item>\n");
	return -1;
    }
    obj = tp->visible(I(E(tp), 1)) + I(tp, 4);
    obj = parse(where, obj, tp, tp->query_step());
    if (!check_size(obj, "No '" + where + "' in range to sit on.\n", tp))
	return -1;
    if (!obj[0]->sit_cmd())
    {
	tp->catch_tell("You cannot sit down there.\n");
	return -1;
    }
    tp->change_int_prop(LIVE_LOST_ROUNDS, 1);
    return 1;
}
