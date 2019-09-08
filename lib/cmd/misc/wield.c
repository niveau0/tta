/*
 * Function name: wield
 * Description:   wield an object
 * Arguments:     str - what to wield
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
wield(string str, object tp)
{
    int    i, sz;
    object *obj;
    mixed  result, other_msg;
    
    if (!str)
    {
	tp->catch_tell("Wield what?\n");
	return -1;
    }
    if (!(sz = sizeof(obj = parse(str, tp->query_hold(SLT_ALL), 0))))
    {
	tp->catch_tell("You don't hold such a weapon.\n");
	return -1;
    }
    
    str = "";
    other_msg = ({ QCTNAME(tp), " wields " });
    for (i=0; i<sz; i++)
    {
	if (stringp(result = tp->wield_tool(obj[i])))
	{
	    str += BS("You wield the " + obj[i]->short() + " in your " +
		      result + ".\n", 77, 0);
	    other_msg += ({ QNAME(obj[i]), " in the " + result, ", " });
	}
	else
	{
	    switch (result)
	    {
	      case 0:
		  str += "You cannot wield the " + obj[i]->short() + ".\n";
		  continue;
	      case -1:
		  str += "The " + obj[i]->short() + " doesn't fit.\n";
		  continue;
	      case -2:
		  str += "The " + obj[i]->short() +
			 " is too big or something is in the way.\n";
		  continue;
	      case -3:
		  str += "The " + obj[i]->short() + " is already wielded.\n";
		  continue;
	    }
	}
    }
    if (strlen(str))
	tp->catch_tell(str);
    if ((sz = sizeof(other_msg)) > 2)
    {
	other_msg[sz-1] = ".\n";
	if (sz != 5)
	    other_msg[sz-4] = " and ";
	tell_objects(LISTEN(E(tp)) - ({ tp }), other_msg, MSG_SEE, tp);
    }
    tp->change_int_prop(LIVE_LOST_ROUNDS, 1);
    return 1;
}
