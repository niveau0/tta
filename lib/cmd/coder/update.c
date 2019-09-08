/* **************************************************************************
 * update - update an object
 */
nomask int
update_ob(string str)
{
    int    kick_master;
    object ob, tp;

    CHECK_LEVEL(_BUILDER);

    tp = this_interactive();
    if (!str || !strlen(str))
    {
	tp->catch_tell("Usage: upd <filename>\n");
	return -1;
    }
    str = FPATH(tp->query_path(), str);
    if (!str || !strlen(str))
    {
	tp->catch_tell("Invalid file name.\n");
	return -1;
    }
    ob = find_object(str);
    if (!ob)
    {
	tp->catch_tell("No such object.\n");
	return -1;
    }
    if (ob == find_object(_SYSTEMD))
	kick_master = 1;

    if (ob != this_object())
    {
	remove_object(ob);
	if (ob)
	    destruct_object(ob);

	/* When updating the master object it must be reloaded */
	if (kick_master)
	{
	    if (!catch(compile_object(_SYSTEMD)))
		tp->catch_tell(_SYSTEMD + " was updated and reloaded.\n");
	    else
		tp->catch_tell("Ouch, " + _SYSTEMD +
			       " could not be updated.\n");
	}
	else if (!ob)
	    tp->catch_tell(str + " will be reloaded at next reference.\n");
	else
	{
	    tp->catch_tell("Object could not be updated.\n");
	    return -1;
	}
    }
    else
    {
	tp->catch_tell(str + " will be reloaded at next reference.\n"+
		       "Destructing this object.\n");
	call_out("update_destruct_this", 0);
	return 1;
    }
    return 1;
}

static void
update_destruct_this()
{
    destruct_object();
}
