/*
 * Function name: stop
 * Description:   stop something
 * Arguments:     str - what to stop
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
stop(string str, object tp)
{
    int    i;
    object *e, *inv;;

    if (!str && (tp->query_state() & WALKING))
    {
	tp->delete_state(WALKING);
	tp->add_prop(LIVE_WALKMODE, MOVE_WALK);
	tp->catch_tell("Ok.\n");
	/* Remove followdata */
	remove_event(E_CHANGE_ENV, "follow_to", tp);
	tp->add_prop(FOLLOWCMD_DATA, 0);
	return 1;
    }
    if ((str == "search" || str == "searching") &&
	(tp->query_state() & SEARCHING))
    {
	tp->delete_state(SEARCHING);
	tp->catch_tell("Ok.\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		     ({ QCTNAME(tp), " stops searching.\n" }), MSG_SEE, tp);
	return 1;
    }
    if (_ACCESS_LEVEL(geteuid(tp)) < 1 || str != "fights")
    {
	tp->catch_tell("Stop what?\n");
	return -1;
    }
    inv = I(E(tp), 2);
    
    str = "Trying to stop all fights.\n";
    for (i=sizeof(inv); i--;)
    {
	if (living(inv[i]) && sizeof(e = inv[i]->cb_query_enemies()))
	{
	    str += ("Removing " + capitalize(inv[i]->query_name()) +
		    "'s enemies.\n" + COMPOSITE(e, tp) + "\n");
	    inv[i]->cb_stop_fight(e);
	}
    }
    tp->catch_tell(str);
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " orders you to stop all fights.\n" }),
		 MSG_SEE, tp);
    return 1;
}