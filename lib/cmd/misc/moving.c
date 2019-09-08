nomask void
stop_walk(int id, object tp)
{
    if (!tp)
	return;
    tp->catch_tell("You stop walking.\n");
    remove_call_out(id);
}

/*
 * Function name: walk_to
 * Description:   constantly walk somewhere
 * Arguments:     tp - walking player
 *                target - target object, target coordinates or direction
 */
static nomask void
walk_to(object tp, mixed target)
{
    int    ns, ew, step, *tp_co, *ob_co;
    mixed foll_data;
    string dir;
    object ob;

    if (!tp || !(tp->query_state() & WALKING))
	return;
    if (ob = tp->query_prop(LIVE_MOUNTED))
	step = ob->query_step(); /* walk with the mount's steps */
    else
	step = tp->query_step();
    
    if (!target || !step)
    {
	tp->catch_tell("Your target is somehow unreachable.\n");
	tp->delete_state(WALKING);
	tp->add_prop(LIVE_WALKMODE, MOVE_WALK);
	return;
    }
    tp_co = tp->query_coordinates();

    if (stringp(target))
	dir = target;
    else
    {
	if (objectp(target))
	{
	    if (!VISIBLE(tp, target))
	    {
		tp->catch_tell("Your target is out of sight.\n");
		tp->delete_state(WALKING);
		tp->add_prop(LIVE_WALKMODE, MOVE_WALK);
		return;
	    }
	    /* get tp-relative object coordinates */
	    if (!(ob_co = target->query_coordinates(tp_co)))
	    {
		tp->catch_tell("Your target has no position to walk to.\n");
		return;
	    }
	}
	else if (pointerp(target))
	    ob_co = target;
	
	dir = "";
	if ((ns = tp_co[1] - ob_co[1]) < 0)
	{
	    ns = -ns;
	    if (ns >= step)
		dir = "north";
	}
	else if (ns >= step)
	    dir = "south";
	
	if ((ew = tp_co[0] - ob_co[0]) < 0)
	{
	    ew = -ew;
	    if (ew >= step)
		dir += "east";
	}
	else if (ew >= step)
	    dir += "west";
	
	if (tp->query_prop(LIVE_WALKMODE) == MOVE_RUN)
	{
	    /* slow down if reaching target */
	    if (ns <= step * MOVE_RUNSPEED && ew <= step * MOVE_RUNSPEED)
		tp->add_prop(LIVE_WALKMODE, MOVE_WALK);
	}
    }
    
    if (!strlen(dir)) /* no more directions? arrival! */
    {
	if (foll_data = tp->query_prop(FOLLOWCMD_DATA))
	{
	    debug("seclan", E(tp)->query_leaving_room(foll_data[1]));
	    tp->move_object(E(tp)->query_leaving_room(foll_data[1]));
	    debug("seclan", E(tp));
	    /* it is necessary to get the updated followdata !! */
	    call_out("walk_to", 1, tp, tp->query_prop(FOLLOWCMD_DATA)[1]);
	    return;
	}
	if (objectp(target))
	    direct_approach(tp, target);
	tp->look_room(E(tp));
	tp->delete_state(WALKING);
	tp->add_prop(LIVE_WALKMODE, MOVE_WALK);
	tp->catch_tell("You arrived.\n");
	call_event(E_ARRIVED, tp, target, 0, 0);
	return;
    }

    set_this_player(tp);
    /* call do_command for state checks (possibly changed) */
    if (do_command(dir, "", tp->query_state()) == -1)
    {
	tp->delete_state(WALKING);
	tp->add_prop(LIVE_WALKMODE, MOVE_WALK);
	tp->catch_tell("You cannot go further.\n");
	call_event(E_ARRIVED, tp, target, 1, 0);
	return;
    }
    if (tp->query_prop(FOLLOWCMD_DATA))
	target = tp->query_prop(FOLLOWCMD_DATA)[1];
    call_out("walk_to", 1, tp, target);
}

/*
 * Function name: run_or_walk
 * Description:   movement
 * Arguments:     str - where to run/walk
 *                mode - walk or run flag
 *                tp - acting living
 * Returns:       1 if successful
 */
static nomask int
run_or_walk(string str, int mode, object tp)
{
    int    id, *co;
    string how;
    mixed  list;

    if (mode == MOVE_RUN)
	how = "run";
    else if (mode == MOVE_WALK)
	how = "walk";
    
    if (!str)
    {
	tp->catch_tell(capitalize(how) + " where?\n");
	return -1;
    }
    switch (str)
    {
      case "n": str = "north"; break;
      case "s": str = "south"; break;
      case "e": str = "east"; break;
      case "w": str = "west"; break;
      case "nw": str = "northwest"; break;
      case "ne": str = "northeast"; break;
      case "sw": str = "southwest"; break;
      case "se": str = "southeast"; break;
    }
    switch (str)
    {
      case "north":
      case "south":
      case "west":
      case "east":
      case "northwest":
      case "northeast":
      case "southwest":
      case "southeast":
	  tp->echo("You start to " + how + " " + str + ".\n");
	  tp->add_state(WALKING);
	  tp->add_prop(LIVE_WALKMODE, mode);
	  tp->add_prop(WALKCMD_DATA,
		       ({ this_object(), "stop_walk",
			  call_out("walk_to", 1, tp, str), tp }));
	  return 1;
    }

    if (sizeof(list = DIRDESC_TO_INT(str)))
    {
	/* walk a specific distance */
	co = tp->query_coordinates()[..];
	tp->echo("You start to " + how + " " + str + ".\n");
	co[1] += 12 * list[0];
	co[0] += 12 * list[1];
	tp->add_state(WALKING);
	tp->add_prop(LIVE_WALKMODE, mode);
	tp->add_prop(WALKCMD_DATA,
		     ({ this_object(), "stop_walk",
			call_out("walk_to", 1, tp, co), tp }));
	return 1;
    }

    /* try to find a specific item or object */
    list = tp->visible(I(E(tp), 0));
    list = parse(str, list, tp);
    if (!sizeof(list))
    {
	list = E(tp)->parse_item(str, 3);
	if (!check_size(list, "No '" + str + "' to " + how +
			" to.\n", tp))
	    return -1;
	if (sizeof(list[0]) > 3)
	{
	    /* walk to pseudo item with coordinates */
	    co = list[0][3];
	    tp->echo("You start to " + how + " to the " + str + ".\n");
	    tp->add_state(WALKING);
	    tp->add_prop(LIVE_WALKMODE, mode);
	    tp->add_prop(WALKCMD_DATA, 
			 ({ this_object(), "stop_walk",
			    call_out("walk_to", 1, tp, co), tp }));
	    return 1;
	}
	tp->catch_tell("That's not possible.\n");
	return -1;
    }
	  
    if (sizeof(list) > 1)
    {
	tp->catch_tell("You have to choose a single target.\n");
	return -1;
    }
    /* walk to an object or living */
    tp->echo(({ "You start to " + how + " to ", QTNAME(list[0]), ".\n" }));
    list[0]->catch_msg(({ QCTNAME(tp), " heads towards you.\n" }),
		       tp, MSG_SEE);
    tp->add_state(WALKING);
    tp->add_prop(LIVE_WALKMODE, mode);
    tp->add_prop(WALKCMD_DATA, 
		 ({ this_object(), "stop_walk",
		    call_out("walk_to", 1, tp, list[0]), tp }));
    return 1;
}

/*
 * Function name: go
 * Description:   handles the direction commands
 * Arguments:     dir - the direction (north, south, etc.)
 *                tp - moving player
 * Returns:       1 if success, -1 else
 */
static nomask int
go(string dir, object tp)
{
    int    i, step, mode, *co, *oco;
    object here, mount, block, next, *worn;

    here = E(tp);
    if (mount = tp->query_prop(LIVE_MOUNTED))
    {
	co = mount->query_coordinates();
	step = mount->query_step();
    }
    else
    {
	co = tp->query_coordinates()[..];
	step = tp->query_step();
    }
    mode = tp->query_prop(LIVE_WALKMODE);
    
    if (mode == MOVE_RUN)
	step *= MOVE_RUNSPEED;
    else if (mode == MOVE_SNEAK)
    {
	step /= 2;
	if (step < 5)
	    step = 5;
    }
    else if (mode == MOVE_STEP)
    {
	step /= 10;
	if (step < 5)
	    step = 5;
    }
    
    oco = co[..]; /* store old coordinates */
    switch (dir)
    {
      case "north":
	co[1] += step;
	break;
      case "south":
	co[1] -= step;
	break;
      case "east":
	co[0] += step;
	break;
      case "west":
	co[0] -= step;
	break;
      case "northeast":
	co[1] += step;
	co[0] += step;
	break;
      case "northwest":
	co[1] += step;
	co[0] -= step;
	break;
      case "southeast":
	co[1] -= step;
	co[0] += step;
	break;
      case "southwest":
	co[1] -= step;
	co[0] -= step;
	break;
      default:
	tp->catch_tell("You cannot move to that direction.\n");
	return -1;
    }
    if ((block = here->collision(oco, co)) && here->call_event(E_BLOCK, tp))
    {
	/* living was blocked */
	return -1;
    }
    if (!(next = here->query_leaving_room(co)))
    {
	/* no exit, so only try to reach the border of the room */
	if (!(co = here->determine_border_coords(oco, co, dir)))
	{
	    /* there is no space between living and border */
	    tp->catch_tell("There is no exit to that direction.\n");
	    return -1;
	}
	next = here;
    }
    if (next != here && (block = next->collision(oco, co)) &&
	next->call_event(E_BLOCK, tp))
	/* living tried to leave the room, but was blocked */
	return -1;
    if (tp->query_state() & COMBAT)
    {
	/* living tried move, but failed to escape his/her enemy */
	if (random(tp->cb_armor_penalty()) > random(50))
	{
	    tp->catch_tell("You failed to escape.\n");
	    return -1;
	}
    }

    if (mount) 
    {
	/* riding, move_mount() moves rider and horse */
	if (mount->move_mount(next, co, dir, mode, tp) == -1)
	    return -1;
    }
    else if (tp->move_living(next, co, dir, mode))
	/* if tp is mounted, move_living() should move rider and horse */
	return -1;
    return 1;
}

/*
 * Function name: run
 * Description:   fast movement
 * Arguments:     str - where to run
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
run(string str, object tp)
{
    return run_or_walk(str, MOVE_RUN, tp);
}

/*
 * Function name: walk
 * Description:   slow movement
 * Arguments:     str - where to go
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
walk(string str, object tp)
{
    return run_or_walk(str, MOVE_WALK, tp);
}

/*
 * Function name: step
 * Description:   make a small step to some direction
 * Arguments:     str - where to go
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
step(string str, object tp)
{
    int result;
    
    switch (str)
    {
      case "n": str = "north"; break;
      case "s": str = "south"; break;
      case "e": str = "east"; break;
      case "w": str = "west"; break;
      case "nw": str = "northwest"; break;
      case "ne": str = "northeast"; break;
      case "sw": str = "southwest"; break;
      case "se": str = "southeast"; break;
    }
    tp->add_prop(LIVE_WALKMODE, MOVE_STEP);
    result = go(str, tp);
    tp->add_prop(LIVE_WALKMODE, MOVE_WALK);
    return result;
}

/*
 * Function name: north and other directions
 * Description:   first called for each direction command
 * Arguments:     str - not used
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
north(string str, object tp)
{
    return go("north", tp);
}

nomask int
south(string str, object tp)
{
    return go("south", tp);
}

nomask int
east(string str, object tp)
{
    return go("east", tp);
}

nomask int
west(string str, object tp)
{
    return go("west", tp);
}

nomask int
northeast(string str, object tp)
{
    return go("northeast", tp);
}

nomask int
northwest(string str, object tp)
{
    return go("northwest", tp);
}

nomask int
southeast(string str, object tp)
{
    return go("southeast", tp);
}

nomask int
southwest(string str, object tp)
{
    return go("southwest", tp);
}