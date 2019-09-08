/*
 * Function name: lock
 * Description:   lock an item if possible
 * Arguments:     str - what to lock
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
lock(string str, object tp)
{
    string with;
    object *list;

    if (!str)
    {
	tp->catch_tell("Usage: lock <item> [with <item?>]\n");
	return -1;
    }
    sscanf(str, "%s with %s", str, with);
    list = I(tp, 4) + tp->visible(I(E(tp), 1));
    list = parse(str, list, tp, tp->query_step());
    if (!check_size(list, "No '" + str + "' in range to lock here.\n", tp))
	return -1;
    if (!list[0]->lock_cmd(with, tp))
    {
	tp->catch_tell("You cannot lock that.\n");
	return -1;
    }
    tp->change_int_prop(LIVE_LOST_ROUNDS, 1);
    return 1;
}
