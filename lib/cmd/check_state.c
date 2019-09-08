
/* included within cmdsouls, part of function do_cmd */

    if (state)
    {
	int    i, check, remove;
	object *obj;
	mixed  data;
	
	check = cmdcheck[cmd];
	
	if ((state & CONCENTRATE) && !(check & CONCENTRATE))
	{
	    tp->change_int_prop(SPELL_CODE, 1); /* if casting, spell is lost */
	    tp->catch_tell("You lost your concentration.\n");
	    data = tp->query_prop(CONCENTRATE_DATA);
	    if (data && data[0] && data[1])
		call_other(data...);
	    remove += CONCENTRATE;
	}
	if ((state & HIDING) && !(check & HIDING))
	{
	    obj = LISTEN(E(tp)) - ({ tp });
	    for (i=sizeof(obj); i--;)
		if (VISIBLE(obj[i], tp))
		    obj[i] = 0;
	    tp->add_prop(OBJ_HIDE, 0);
	    tell_objects(obj - ({ 0 }),
			 ({ "Suddenly a noise catches your attention...\n  ",
				QNAME(tp), " appears from nowhere.\n" }),
			 MSG_SEE, tp);
	    tp->catch_tell("You reveal yourself.\n");
	    remove += HIDING;
	}
	if ((state & WORKING) && !(check & WORKING))
	{
	    data = tp->query_prop(WORKCMD_DATA);
	    if (data && data[0] && data[1])
		call_other(data...);
	    remove += WORKING;
	}
	if ((state & WALKING) && !(check & WALKING))
	{
	    if (previous_object() != this_object()) /*  not walking call_out */
	    {
		data = tp->query_prop(WALKCMD_DATA);
		if (data[0] && data[1])
		    call_other(data...);
		remove += WALKING;
	    }
	}
	if ((state & SEARCHING) && !(check & SEARCHING))
	{
	    tp->catch_tell("You stop searching.\n");
	    remove += SEARCHING;
	}
	tp->delete_state(remove);
	if ((state & MOUNT) && !(check & MOUNT))
	{
	    tp->catch_tell("You can't do that while riding.\n");
	    return -1;
	}
	if ((state & GHOST) && !(check & GHOST))
	{
	    tp->catch_tell("You can't do that as a ghost.\n");
	    return -1;
	}
	if ((state & SLEEPING) && !(check & SLEEPING))
	{
	    tp->catch_tell("You can't do that while sleeping.\n");
	    return -1;
	}
	if ((state & MEDITATING) && !(check & MEDITATING))
	{
	    tp->catch_tell("You can't do that while meditating.\n");
	    return -1;
	}
	if ((state & RESTING) && !(check & RESTING))
	{
	    tp->catch_tell("You can't do that while resting.\n");
	    return -1;
	}
	if ((state & COMBAT) && !(check & COMBAT))
	{
	    tp->catch_tell("You can't do that while fighting.\n");
	    return -1;
	}
	if ((state & UNCONSCIOUS) && !(check & UNCONSCIOUS))
	{
	    tp->catch_tell("But you are unconscious.\n");
	    return -1;
	}
	if ((state & PARALYSED) && !(check & PARALYSED))
	{
	    tp->catch_tell("But you are paralysed.\n");
	    return -1;
	}
	if ((state & ON_GROUND) && !(check & ON_GROUND))
	{
	    tp->catch_tell("You should stand up first.\n");
	    return -1;
	}
    }
