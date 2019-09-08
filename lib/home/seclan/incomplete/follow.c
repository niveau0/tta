
private string directioncode;

stop_follow(int id, object tp)
{
    if (!tp)
	return;
    remove_event(E_ARRIVED, "follow_to", this_object());
    tp->catch_tell("You stop walking.\n");
    remove_call_out(id);
}

/*
 * Function name: follow
 * Description:   follow a path, way or road without caring for curves
 * Arguments:     str - what to follow
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
follow(string str, object tp)
{
    int   i, *co;
    mixed list;
    
    if (!tp)
	return -1;
    
    if (!str)
    {
	tp->catch_tell("You haven't chosen a goal.\n");
	return -1;
    }
    
    list = E(tp)->parse_item(str, 1); // exact singular match
    if (!check_size(list, "You cannot make out such a goal.\n", tp))
	return -1;

    /* Already using a directioncode? */
    if (str[0] != '#')
    {
    /* Looking for the adequate direction code */
    for (i = sizeof(list[0][0])-1; i>=0;i--)
    {
	if (list[0][0][i][0] == '#')
	{
	    directioncode = list[0][0][i];
	    break;
	}
    }
    }
    
    if (sizeof(list[0]) > 3) // Does the item have coordinates?
    {
	/* walk to pseudo item with coordinates */
	co = list[0][3];
	tp->echo("You start to follow the " + str + ".\n");
	tp->add_state(WALKING);
	tp->add_prop(LIVE_WALKMODE, MOVE_WALK);
	tp->add_prop(WALKCMD_DATA, 
		     ({ this_object(), "stop_follow",
			    call_out("walk_to", 1, tp, co), tp }));
	//	add_event(E_ARRIVED, "follow_to", this_object());
	add_event(E_MOVE, "follow_toII", tp);
	return 1;
    }
    tp->catch_tell("Your goal has no special location to walk to.\n");
    return -1;
}

/*
 * Function name: follow_to
 * Description:   called by E_ARRIVED, will look for the new pseudoitem to
 *                follow
 * Arguments:     tp - acting living
 * Returns:       target - a direction code (like #123 for example)
 *                         direction codes always start with '#'
 *                fail if true, last move failed
 *                reason - if given, the object casung the failure
 */
nomask void
follow_to(object tp, mixed target, int fail, object reason)
{
    int    *co;
    mixed  list;

    if (!tp)
	return;

    if (reason)
    {
	// This output is not very good! Improvement necessary!
	tp->catch_tell("Your movement is blocked by "+reason->short()+".\n");
	return;
    }

    if (fail)
    {
	tp->catch_tell("Your movement is hindered by an unknown force!.\n");
	return;
    }
    
    if (!target)
    {
	tp->catch_tell("You cannot make out such a goal.\n");
	remove_event(E_ARRIVED, "follow_to", this_object());
	return;
    }

    list = E(tp)->parse_item(directioncode, 1); // exact singular match

    // No new waypoint has been found.
    if (!check_size(list, "You are no longer sure where to go.\n", tp))
    {
	remove_event(E_ARRIVED, "follow_to", this_object());
	return;
    }

    if (sizeof(list[0]) > 3) // Does the item have coordinates?
    {
	/* walk to pseudo item with coordinates */
	co = list[0][3];
	tp->add_state(WALKING);
	tp->add_prop(LIVE_WALKMODE, MOVE_WALK);
	tp->add_prop(WALKCMD_DATA, 
		     ({ this_object(), "stop_follow",
			    call_out("walk_to", 1, tp, co), tp }));
	return;
    }
    tp->catch_tell("Your goal has no special location to walk to.\n");
    remove_event(E_ARRIVED, "follow_to", this_object());
}

nomask void
follow_toII(object tp, object from, object dest)
{
    debug("seclan", "Hurra! E_MOVE kommt durch!");
    if (from != dest)
	debug("seclan", "neuer Raum!");
    follow();
}
