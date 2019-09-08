/* **************************************************************************
 * destruct - destruct an object
 */
nomask int
destruct_ob(string str)
{
    int    dflag;
    string what;
    object ob, tp, *obj;

    CHECK_LEVEL(_BUILDER);

    tp = this_interactive();
    if (!str || !strlen(str))
    {
	tp->catch_tell("Usage: destruct [-d] <objectname>\n"+
		       "Option -d uses destruct_object(), " +
		       "not remove_object(), "+
		       "use it with care.\n");
	return -1;
    }
    if (sscanf(str,"-d %s", what))
    {
	dflag = 1;
	str = what;
    }
    if (sizeof(obj = parse(str, I(tp, 7) + I(E(tp), 0), 0)))
	ob = obj[0];
    else if (!(ob = find_object(str)))
    {
	tp->catch_tell("No " + str + " here.\n");
	return 1;
    }
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(ob),
			(ob->query_heap_size() > 2 ? " are" : " is") +
			" disintegrated by ", QTNAME(tp), ".\n" }),
		 MSG_SEE, ob);
    if (ob != this_object())
    {
	if (dflag)
	    destruct_object(ob);
	else
	    remove_object(ob);
    }
    else
    {
	tp->echo("You destruct the object.\n");
	destruct_object();
	return 1;
    }
    if (tp)
    {
	if (ob)
	{
	    tp->catch_tell("You failed to destruct the object.\n");
	    return -1;
	}
	tp->echo("You destructed the object.\n");
    }
    return 1;
}
