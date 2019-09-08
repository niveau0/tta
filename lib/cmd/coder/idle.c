/* **************************************************************************
 * idle - show idle-time of players
 */
nomask int
do_idle(string name)
{
    int    i, idle;
    string out;
    object tp, ob, *obj;
  
    CHECK_LEVEL(_BUILDER);

    tp = this_interactive();
    if (!name || !strlen(name))
	obj = sort_array(users(), "sort_idle", this_object());
    else if (!(ob = find_player(name)))
    {
	tp->catch_tell("Wrong name, or player not logged on.\n");
	return -1;
    }
    else
	obj = ({ ob });
    out = "Idle:\n";
    for (i=sizeof(obj); i--;)
    {
	if (!query_ip_number(obj[i]))
	    continue;
	idle = query_idle(obj[i]);
	if (!(name = obj[i]->query_name()))
	    continue;
	out += sprintf("%_/-12s_%__/2dh__%_/2dm__%_/2ds\n",
		       name, idle/3600, (idle/60) % 60, idle % 60);
    }
    tp->catch_tell(out);
    return 1;
}

static nomask int
sort_idle(object a, object b)
{
    return (query_idle(a) - query_idle(b));
}
