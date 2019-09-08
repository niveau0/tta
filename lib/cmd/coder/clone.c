/* **************************************************************************
 * clone - clone an object
 */
nomask int
clone(string arg)
{
    int    num, *hands, *co;
    string str;
    object ob, tp;

    CHECK_LEVEL(_BUILDER);

    tp = this_interactive();
    if (!arg || !strlen(arg))
    {
	tp->catch_tell("Usage: clone <path> [<heap>]\n");
	return -1;
    }
    sscanf(arg, "%s %d", arg, num);
    str = FPATH(tp->query_path(), arg);
    if (!str)
    {
	tp->catch_tell("No such file.\n");
	return -1;
    }
    if (strlen(str) > 3 && str[strlen(str)-2..] == "/.")
	str = str[..strlen(str)-3];
    if (file_size(str + ".c") < 0)
    {
	if (!(str = find_file(arg + ".c")))
	{
	    tp->catch_tell("No such file.\n");
	    return -1;
	}
	str = str[..strlen(str)-3];
    }
    if (!strlen(str) || (strlen(str) > 4 && str[..4] == "/base"))
    {
	tp->catch_tell("Invalid file.\n");
	return -1;
    }
    if (catch(ob = clone_object(str)))
    {
	tp->catch_tell("Failed.\n");
	return -1;
    }
    if (num > 0)
	ob->set_heap_size(num);
    if (living(ob) || !sizeof(hands = tp->query_slot_type(SLT_HAND)) ||
	tp->add_tool(ob, hands[0]))
    {
	/* move into room if cloned object is living, has no hands or
	   object cannot be moved to 'cloner' */
	if (co = tp->query_coordinates()[..])
	{
	    /* move to the ground */
	    co[2] = E(tp)->query_coordinates()[2];
	    ob->set_coordinates(co);
	}
        ob->move(E(tp));
    }
    tp->catch_msg(({ "You summon ", QNAME(ob), ".\n" }), 0, 0);
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " summons ", QNAME(ob), ".\n" }),
		 MSG_SEE, tp);
    return 1;
}
