/*
 * Function name: extinguish
 * Description:   extinguish an item (e.g. a torch)
 * Arguments:     str - what to extinguish
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
extinguish(string str, object tp)
{
    object *list;

    if (!str)
    {
	tp->catch_tell("Usage: extinguish <item>\n");
	return -1;
    }
    list = I(tp, 4) + tp->visible(I(E(tp), 1));
    list = parse(str, list, tp, tp->query_step());
    if (!check_size(list, "No '" + str + "' in range to extinguish.\n", tp))
	return -1;
    if (!list[0]->extinguish_cmd())
    {
	tp->catch_tell("It is not lit.\n");
	return -1;
    }
    tp->change_int_prop(LIVE_LOST_ROUNDS, 1);
    return 1;
}
