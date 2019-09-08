/*
 * Function name: read
 * Description:   read a text on something
 * Arguments:     str - what to read where
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
read(string str, object tp)
{
    int    sz;
    string what, where;
    object *inv;
    mixed  list;

    if (!str)
    {
	tp->catch_tell("Read what [of what]?\n");
	return -1;
    }
    inv = tp->visible(I(tp, 4) + I(E(tp), 1));
    if (sscanf(str, "%s of %s", what, where) == 2)
	list = parse(where, inv, 0);
    else
    {
	what = 0;
	list = parse(str, inv, 0);
    }
    if (!(sz = sizeof(list)))
    {
	list = parse_items(str, ({ E(tp) }) + inv);
	if (!check_size(list, "No '" + str + "' to read from.\n", tp))
	    return -1;
	tp->catch_tell(list[0]);
	tell_objects(LISTEN(this_object()) - ({ tp }),
		     ({ QCTNAME(tp), " looks at the " + str + ".\n" }),
		     MSG_SEE, tp);
	tp->change_int_prop(LIVE_LOST_ROUNDS, 1);
	return 1;
    }
    if (sz > 1)
    {
	tp->catch_tell("You better choose one item to read from.\n");
	return -1;
    }
    if (!list[0]->read_cmd(what))
    {
	tp->catch_tell("There is nothing to read.\n");
	return -1;
    }
    tp->change_int_prop(LIVE_LOST_ROUNDS, 1);
    return 1;
}
