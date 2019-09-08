/* **************************************************************************
 * updir - update a directory
 */
nomask int
updir(string str)
{
    int    i, sz, l;
    string curr, *conts;
    object ob, tp;
    
    CHECK_LEVEL(_BUILDER);

    tp = this_interactive();
    if (!str || !strlen(str))
	str = tp->query_path();
    else 
	str = FPATH(tp->query_path(), str);
    
    if (file_size(str) != -2)
    {
	tp->catch_tell("Invalid directory name.\n");
	return -1;
    }
    if (str[strlen(str)-1] != '/')
	str += "/";
    conts = get_dir(str);
    if (!(sz = sizeof(conts)))
    {
	tp->catch_tell("No files to update.\n");
	return -1;
    }
    for (i=0; i<sz; i++)
    {
	curr = conts[i];
	if ((l = strlen(curr)) < 3 || curr[l-2..] != ".c")
	    continue;
	curr = str+curr[..l-3];
	if (!(ob = find_object(curr)))
	    continue;
	if (ob == this_object())
	{
	    tp->catch_tell(curr + " will be reloaded at next reference.\n"+
			   "Destructing this object.\n");
	    call_out("update_destruct_this", 0);
	    continue;
	}
	destruct_object(ob);
	
	/* When updating the master object it must be reloaded */
	if (curr == _SYSTEMD)
	{
	    if (!catch(compile_object(_SYSTEMD)))
		tp->catch_tell(_SYSTEMD + " was updated and reloaded.\n");
	    else
		tp->catch_tell("Ouch, " + _SYSTEMD +
			       " could not be updated.\n");
	}
	else if (!ob)
	    tp->catch_tell(curr + " will be reloaded at next reference.\n");
    }
    return 1;
}
