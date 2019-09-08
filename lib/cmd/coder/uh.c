/* **************************************************************************
 * uh - update the environment
 */
nomask int
uh(string str)
{
    int    i, sz;
    string file;
    object env, tp, ob, *all;

    CHECK_LEVEL(_BUILDER);

    tp = this_interactive();
    if (str)
    {
	tp->catch_tell("Usage: uh\n");
	return -1;
    }

    env  = E(tp);
    file = env->query_room_file();
    all  = I(env, 7);
    sz   = sizeof(all);
    
    if (!(ob = OBJ("/base/room")))
    {
	if (catch(compile_object("/base/room")))
	{
	    tp->catch_tell("Failed to compile /base/room.\n");
	    return -1;
	}
	ob = OBJ("/base/room");
    }
    for (i=0; i<sz; i++)
    {
	if (living(all[i]) && !all[i]->query_npc())
	    all[i]->move(ob);
    }
    remove_object(env);
    if (env)
	destruct_object(env);
    if (!(env = _ROOMD->load_room(file)))
    {
	tp->catch_tell("Failed to reload.\n");
	return -1;
    }
    for (i=0; i<sz; i++)
    {
	if (all[i])
	    all[i]->move(env);
    }
    tp->look_room(env);
    return 1;
}
