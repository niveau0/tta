/*
 * Function name: remember
 * Description:   try to keep someones name in mind
 * Arguments:     str - the one to remember
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
remember(string str, object tp)
{
    int     i, sz;
    string  rname, who, *ind;
    object  *obj;
    mapping map;
    
    if (!str)
    {
	map = tp->query_remembered();
	if(!map_sizeof(map))
	{
	    tp->catch_tell("You do not seem to remember anyone.\n");
	    return 1;
	}
	smore("These are the people you remember:\n" + 
	      BS(COMPOSITE_WORDS(map_values(map)), 77, 1) + "\n", tp);
	return 1;
    }
    if (sscanf(str, "%s as %s", str, rname) != 2)
    {
	tp->catch_tell("Remember whom as whom?\n");
	return -1;
    }
    if (sizeof(explode(rname, " ")) > 1)
    {
	tp->catch_tell("'" + rname + "' is not a single name. You must " +
		       "choose one word.\n");
	return -1;
    }
    obj = tp->visible(I(E(tp), 2));
    obj = parse(str, obj, tp);
    if (!sizeof(obj))
    {
	map = tp->query_remembered();
	ind = map_indices(map);
	for (i=0, sz=sizeof(ind); i<sz; i++)
	{
	    if (map[ind[i]] == str)
	    {
		who = ind[i];
		break;
	    }
	}
	if (i == sz)
	{
	    tp->catch_tell("You see noone called '" + str + "'.\n");
	    return -1;
	}
    }
    else
    {
	if (obj[0] == tp)
	{
	    tp->catch_tell("You know your name, yes.\n");
	    return -1;
	}
	if (sizeof(obj[0]->query_names()) < 2)
	{
	    tp->catch_tell("This is no individual person so you don't " +
			   "succeed to remember a name for it.\n");
	    return -1;
	}
	who = obj[0]->query_name();
    }
    switch(tp->add_remembered(who, lower_case(rname)))
    {
      case -1:
	tp->catch_tell("Your mind can't remember any more people.\n");
	return 1;
      case 1:
	tp->echo(({ "You now remember ", QTNAME(obj[0]), " as " +
			str }), 0, 0);
 	return 1;
      case 2:
	tp->catch_tell("You changed your memory to " + capitalize(rname) +
		       ".\n");
        return 1;
    }
    return 1;
}
