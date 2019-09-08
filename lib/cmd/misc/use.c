/*
 * Function name: use
 * Description:   use an item if possible
 * Arguments:     str - what to use
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
use(string str, object tp)
{
    string whom;
    object *list;

    if (!str)
    {
	tp->catch_tell("Usage: use <item> [at|on <someone|something>]\n");
	return -1;
    }
    if (!sscanf(str, "%s at %s", str, whom))
	sscanf(str, "%s on %s", str, whom);
    list = I(tp, 4) + tp->visible(I(E(tp), 1));
    list = parse(str, list, tp, tp->query_step());
    if (!check_size(list, "No '" + str + "' in range to use here.\n", tp))
	return -1;
    if (!list[0]->use_cmd(whom))
    {
	tp->catch_tell("Seems you cannot do anything special with it.\n");
	return -1;
    }
    tp->change_int_prop(LIVE_LOST_ROUNDS, 1);
    return 1;
}
