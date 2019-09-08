/*
 * Function name: roll
 * Description:   roll an item (e.g. a dice)
 * Arguments:     str - what to roll
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
roll(string str, object tp)
{
    int    i, sz;
    object ob, *list;

    if (!str)
    {
	tp->catch_tell("Usage: roll <item>\n");
	return -1;
    }
    list = tp->visible(tp->query_hold(SLT_ALL));
    list = parse(str, list, 0);
    if (!check_size(list, "You hold no '" + str + "' to roll.\n", tp))
	return -1;
    if (!list[0]->roll_cmd())
    {
	tp->catch_tell("You cannot roll that.\n");
	return -1;
    }
    tp->change_int_prop(LIVE_LOST_ROUNDS, 1);
    return 1;
}
