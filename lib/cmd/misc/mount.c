/*
 * Function name: mount
 * Description:   try to mount an object (a horse)
 * Arguments:     str - what to mount
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
mount(string str, object tp)
{
    object *list;

    if (!str)
    {
	tp->catch_tell("Usage: mount <creature>\n");
	return -1;
    }
    list = tp->visible(I(E(tp), 2));
    list = parse(str, list, tp, tp->query_step());
    if (!check_size(list, "No '" + str + "' in range to mount here.\n", tp))
	return -1;
    if (!list[0]->mount_cmd(tp))
    {
	tp->catch_tell("You cannot mount that.\n");
	return -1;
    }
    return 1;
}
