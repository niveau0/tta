/* **************************************************************************
 * trans - teleport someone somewhere
 */
nomask int
trans(string str)
{
    int    *co;
    string *args;
    object tp, ob, *all;

    CHECK_LEVEL(_BUILDER);

    tp = this_interactive();
    if (!str || !strlen(str))
    {
	tp->catch_tell("Usage: trans <living>\n");
	return -1;
    }
    args = explode(str, " ");
    ob = find_living(args[0]);
    if (!ob)
    {
	tp->catch_tell("No such living thing.\n");
	return -1;
    }
    
    ob->catch_tell("You feel yourself magically transfered.\n");
    co = tp->query_coordinates();
    if (sizeof(args) > 1)
    {
	object dest;
	
	if (!(dest = _ROOMD->load_room(FPATH(tp->query_path(), args[1]))))
	{
	    tp->catch_tell("Invalid room path.\n");
	    return -1;
	}
	ob->move_living(dest, co, "", 1);
    }
    else
	ob->move_living(E(tp), co, "", 1);
    return 1;
}
