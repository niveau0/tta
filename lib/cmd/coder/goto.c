/* **************************************************************************
 * goto_dest - teleport somewhere
 */
nomask int
goto_dest(string dest)
{
    int    *co;
    object ob, tp, env;
    
    CHECK_LEVEL(_BUILDER);

    tp = this_interactive();
    if (!dest)
    {
	tp->catch_tell("Usage: goto <name>|<roompath>\n");
	return -1;
    }

    ob = find_living(dest);
    if (ob && (env = E(ob)))
    {
	if (!living(ob))
	{
	    tp->catch_tell("Player is linkdead.\n");
	    return -1;
	}
	co = ob->query_coordinates()[..];
	co[2] = env->query_coordinates()[2];
	tp->move_living(env, co, "", 1);
	return 1;
    }

    dest = FPATH(tp->query_path(), dest);
    
    if (!dest || !strlen(dest) || !(ob = _ROOMD->load_room(dest)))
    {
	tp->catch_tell("Invalid living or file name: " + dest + "\n");
	return -1;
    }
    tp->move_living(ob, ob->query_coordinates(), "", 1);
    return 1;
}
