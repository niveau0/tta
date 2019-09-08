/*
 * Function name: enter
 * Description:   enter an item (e.g. a door)
 * Arguments:     str - what to enter
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
enter(string str, object tp)
{
    object *list;

    if (!str)
    {
	tp->catch_tell("Enter what?\n");
	return -1;
    }
    list = tp->visible(I(E(tp), 1));
    list = parse(str, list, tp, tp->query_step());
    if (!check_size(list, "No '" + str + "' in range to enter here.\n", tp))
	return -1;
    if (!list[0]->enter_cmd(str, tp))
    {
	tp->catch_tell("You cannot enter that.\n");
	return -1;
    }
    return 1;
}
