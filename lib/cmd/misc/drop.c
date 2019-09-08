/*
 * Function name: drop
 * Description:   drop an item if possible
 * Arguments:     str - what to drop
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
drop(string str, object tp)
{
    object here, *items;
    mixed  comp;

    if (!str)
    {
	tp->catch_tell("Usage: drop <item>\n");
	return -1;
    }
    here  = E(tp);
    items = parse(str, I(tp, 7), 0);
    if (!sizeof(items))
    {
	tp->catch_tell("You don't have that item.\n");
	return -1;
    }
    items = try_drop(items, here, tp, tp->query_coordinates()[..]);
    if (sizeof(items))
    {
	tell_objects(LISTEN(here) - ({ tp }),
		     ({ QCTNAME(tp), " drops " }) +
		     (comp = COMPOSITE_ARRAY(items)) +
		     ({ ".\n" }), MSG_SEE, tp);
	tp->catch_msg(({ "You drop " }) + comp + ({ ".\n" }), 0, 0);
	tp->change_int_prop(LIVE_LOST_ROUNDS, 1);
    }
    else
	tp->catch_tell(query_drop_error());
    return 1;
}
