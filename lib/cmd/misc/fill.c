/*
 * Function name: fill
 * Description:   fill something in something
 * Arguments:     str - what to fill with what
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
fill(string str, object tp)
{
    string what, with;
    object *list, *list2;
    
    if (!str || sscanf(str, "%s from %s", what, with) != 2)
    {
	tp->catch_tell("Usage: fill <target> from <source>\n");
	return -1;
    }
    list2 = I(tp, 4) + tp->visible(I(E(tp), 1));
    list = parse(what, list2, tp, tp->query_step());
    if (!check_size(list, "No '" + what + "' in range to fill here.\n", tp))
	return -1;
    list2 = parse(with, list2, tp, tp->query_step());
    if (!check_size(list2, "No '" + with + "' in range to fill from.\n", tp))
	return -1;
    if (list[0] == list2[0])
    {
	tp->catch_tell("Huh? Fill into itself?\n");
	return -1;
    }
    if (!list[0]->fill_cmd(list2[0]))
    {
	tp->catch_tell("It cannot be filled.\n");
	return -1;
    }
    tp->change_int_prop(LIVE_LOST_ROUNDS, 1);
    return 1;
}
