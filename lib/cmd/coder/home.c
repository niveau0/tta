/* **************************************************************************
 * home - return to workroom or set workroom path
 */
nomask int
home(string str)
{
    object tp, ob;

    CHECK_LEVEL(_BUILDER);
    
    tp = this_interactive();
    if (!str)
    {
	if (!(str = tp->query_prop(CODER_HOME)))
	{
	    tp->catch_tell("You have no home, poor thing.\n");
	    return -1;
	}
	if (!str || !strlen(str) || !(ob = _ROOMD->load_room(str)))
	{
	    tp->catch_tell("Cannot find room: " + str + "\n");
	    return -1;
	}
	tp->move_living(ob, ob->query_coordinates(), "", 1);
	return 1;
    }
    str = FPATH(tp->query_path(), str);
    if (!str || !strlen(str) || !_ROOMD->load_room(str))
    {
	tp->catch_tell("Cannot find room: " + str + "\n");
	return -1;
    }
    tp->add_prop(CODER_HOME, str);
    tp->catch_tell("New home path: " + str + "\n");
    return 1;
}
