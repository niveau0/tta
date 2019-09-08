nomask void
stop_follow(int id, object tp)
{
    if (!tp)
	return;
    tp->catch_tell("You stop walking.\n");
    remove_call_out(id);
    remove_event(E_CHANGE_ENV, "follow_to", tp);
    tp->add_prop(FOLLOWCMD_DATA, 0);
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
    string directioncode;
    int   i, *co;
    mixed list;
    
    if (!tp)
	return -1;
    
    if (!str)
    {
	tp->catch_tell("Follow what?.\n");
	return -1;
    }

    list = E(tp)->parse_item(str, 1); /* exact singular match */
    if (!check_size(list, "You cannot make out such a goal.\n", tp))
	return -1;
    
    directioncode = "";
    /* Looking for the adequate direction code */
    for (i = sizeof(list[0][0])-1; i>=0;i--)
    {
	if (list[0][0][i][0] == '#')
	{
	    directioncode = list[0][0][i];
	    break;
	}   
    }

    if (sizeof(list[0]) > 3) /* Does the item have coordinates? */
    {
	/* start walk to pseudo item with coordinates */
	co = list[0][3];
	tp->add_prop(FOLLOWCMD_DATA, ({ directioncode, co }) );
	tp->echo("You start to follow the " + str + ".\n");
	tp->add_state(WALKING);
	tp->add_prop(LIVE_WALKMODE, MOVE_WALK);
	tp->add_prop(WALKCMD_DATA, 
		     ({ this_object(), "stop_follow",
			    call_out("walk_to", 1, tp, co), tp }));
	add_event(E_CHANGE_ENV, "follow_to", tp);
	return 1;
    }
    tp->catch_tell("Your goal has no special location to walk to.\n");
    return -1;
}

/*
 * Function name: follow_to
 * Description:   called by E_CHANGE_ENV, will look for the new pseudoitem to
 *                follow
 * Arguments:     tp - acting living
 *                from - old environment
 *                dest - new environment
 */
nomask void
follow_to(object tp, object from, object dest)
{
    mixed list, data;
    string dircode;

    if (!(data = tp->query_prop(FOLLOWCMD_DATA)))
	return;
    if ((dircode = data[0]) != "")
    {
	list = E(tp)->parse_item(dircode, 1); /* exact singular match */
	if (check_size(list, "You can no longer make out your goal.\n", tp))
	{
	    if (sizeof(list[0]) > 3) /* Does the item have coordinates? */
	    {
		tp->add_prop(FOLLOWCMD_DATA, ({ dircode, list[0][3] }) );
		return;
	    }
	    tp->catch_tell("Your goal has no special location to walk to.\n");
	}
    }
    else
	tp->catch_tell("You are no longer sure where to go.\n");

    tp->delete_state(WALKING);
    tp->add_prop(LIVE_WALKMODE, MOVE_WALK);
    remove_event(E_CHANGE_ENV, "follow_to", tp);
    tp->add_prop(FOLLOWCMD_DATA, 0);
}