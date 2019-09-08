/*
 * Function name: open
 * Description:   open an item if possible
 * Arguments:     str - what to open
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
open(string str, object tp)
{
    string with;
    object *list;

    if (!str)
    {
	tp->catch_tell("Usage: open <item> [with <item>]\n");
	return -1;
    }
    sscanf(str, "%s with %s", str, with);
    list = I(tp, 4) + tp->visible(I(E(tp), 1));
    list = parse(str, list, tp, tp->query_step());
    if (!check_size(list, "No '" + str + "' in range to open here.\n", tp))
	return -1;
    if (!list[0]->open_cmd(with, tp))
    {
	tp->catch_tell("You cannot open that.\n");
	return -1;
    }
    tp->change_int_prop(LIVE_LOST_ROUNDS, 1);
    return 1;
}
