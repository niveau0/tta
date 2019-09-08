/*
 * Function name: write
 * Description:   write something on paper, in books, etc.
 * Arguments:     str - where to write
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
write(string str, object tp)
{
    int    i, sz;
    string subitem;
    object *list;

    if (!str || !sscanf(str, "on %s", str))
    {
	tp->catch_tell("Usage: write on <item> [of <item>]\n");
	return -1;
    }
    sscanf(str, "%s of %s", subitem, str);
    
    list = I(tp, 4) + tp->visible(I(E(tp), 1));
    list = parse(str, list, tp, tp->query_step());
    if (!check_size(list, "No '" + str + "' in range to write on.\n", tp))
	return -1;
    if (!list[0]->write_cmd(subitem))
    {
	tp->catch_tell("You cannot write there.\n");
	return -1;
    }
    return 1;
}
