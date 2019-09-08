/*
 * Function name: pick
 * Description:   pick locks
 * Arguments:     str - what to pick
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
pick(string str, object tp)
{
    string with;
    object *list;
    
    if (!str)
    {
	tp->catch_tell("Usage: pick <item> [with what]\n");
	return -1;
    }
    sscanf(str, "%s with %s", str, with);
    list = I(tp, 4) + tp->visible(I(E(tp), 1));
    list = parse(str, list, tp, tp->query_step());
    if (!check_size(list, "No '" + str + "' in range to pick here.\n", tp))
	return -1;
    if (!list[0]->pick_cmd(with, tp))
    {
	tp->catch_tell("There is no lock.\n");
	return -1;
    }
    return 1;
}
