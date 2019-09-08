/*
 * Function name: wear
 * Description:   wear an object
 * Arguments:     str - what to wear or objects still to go
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
wear(mixed str, object tp)
{
    int    i, sz, slot;
    string prep, tool_name, slot_name;
    object ob, *obj, *held;
    mixed  result;
    
    if (!str)
    {
	tp->catch_tell("Usage: wear <object> [on <bodypart>]\n");
	return -1;
    }
    if (pointerp(str))
    {
	obj = str & tp->query_hold(SLT_ALL);
	if (!(sz = sizeof(obj)))
	    return -1;
	result = tp->wear_tool(obj[0]);
    }
    else
    {
	held = tp->query_hold(SLT_ALL);
	slot = 0;
	if (sscanf(str, "%s on %s", tool_name, slot_name) == 2)
	{
	    slot = tp->query_desc_slot(slot_name);
	    if (!slot)
	    {
		tp->catch_tell("Your body has no '" + slot_name + "'.\n");
		return -1;
	    }
	    str = tool_name;
	}
	if (!(sz = sizeof(obj = parse(str, held, 0))))
	{
	    tp->catch_tell("You don't hold such equipment.\n");
	    return -1;
	}
	result = tp->wear_tool(obj[0], slot);
    }
    ob = obj[0];
    prep = ob->wear_preposition();
    if (stringp(result))
    {
	tp->catch_tell(BS("You wear the " + ob->short() + " " + prep +
			  " your " + result + ".\n", 77, 0));
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		     ({ QCTNAME(tp), " wears ", QNAME(ob), " " + prep +
			    " the " + result + ".\n" }), MSG_SEE, tp);
    }
    else
    {
	str = 0;
	switch (result)
	{
	  case 0:
	      str = "You cannot wear the " + ob->short() + ".\n";
	      break;
	  case -1:
	      str = "The " + ob->short() + " doesn't fit your body.\n";
	      break;
	  case -2:
	      str = "The " + ob->short() + " is too big.\n";
	      break;
	  case -3:
	      str = "The " + ob->short() +
		     " cannot be worn, something is in the way.\n";
	      break;
	  case -4:
	      str = "The " + ob->short() + " is already worn.\n";
	      break;
	  case -5:
	      str = "The " + ob->short() + " is too small.\n";
	      break;
	}
	if (str)
	    tp->catch_tell(str);
     }
     if (sz > 1)
	 tp->add_command(({ this_object(), "wear", obj[1..] }));
     tp->change_int_prop(LIVE_LOST_ROUNDS, 1);
     return 1;
}
