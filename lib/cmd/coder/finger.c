/*****************************************************************************
 * finger - display information about player/codertype
 */
nomask int
finger(string name)
{
    int    i, sz, birth, online, lev;
    string pron, str, ret, *arr;
    object tp, ob;

    CHECK_LEVEL(_BUILDER);

    tp = this_interactive();
    if (!name)
    {
	tp->catch_tell("Usage: finger valar|maiar|admin|<playername>\n");
	return -1;
    }
    ret = "";
    if (ob = find_player(name))
    {
	ret += (capitalize(name) +
		" is currently in the game, Location: " +
		object_name(E(ob)) + "\n");
	online = 1;
    }
    else
	ob = _SYSTEMD->finger_player(name);
    
    if (!ob)
    {
	if (name == "admin")
	    arr = _SYSTEMD->query_coder_list(_ADMIN);
	else if (name == "valar")
	    arr = _SYSTEMD->query_coder_list(_ADMIN) +
		  _SYSTEMD->query_coder_list(_CREATOR);
	else if (name == "maiar")
	    arr = _SYSTEMD->query_coder_list(_BUILDER);
	else
	{
	    tp->catch_tell("No such player: " + name + "\n");
	    return -1;
	}
	str = "";
	for (i=0, sz=sizeof(arr); i<sz; i++)
	    str += sprintf("> %./-16s\n", arr[i]);
	tp->catch_tell(sprintf("% *78s\n", str));
	return 1;
    }

    pron = capitalize(ob->query_pronoun());
    ret	+= ob->long();
    if ((lev = _ACCESS_LEVEL(geteuid(ob))) > 0)
    {
	ret += ((tp == ob ? "You are " : pron + " is ") +
		(lev == _BUILDER ? "builder"
		 : (lev == _CREATOR ? "creator" : "administrator")) +
		 " (" + _SYSTEMD->query_coder_pretitle(name) + ").\n");
    }
    if (birth = ob->query_birth())
	ret += (tp == ob ? "You are " : pron + " is ") +
	       CONV_REALTIME(time()-birth) + " old.\n";
    ret += "Email: " + ob->query_email() + "\n";
    if (online) 
    {
	ret += ("Currently logged on from: " + ob->query_login_from() + "\n");
	if (query_ip_number(ob))
	{
	    if (query_idle(ob) > 0)
		ret += "Idle time: " + CONV_REALTIME(query_idle(ob)) + ".\n";
	}
	else
	    ret += (tp == ob ? "You are" : pron + " is") + " link dead.\n";
    }
    else 
    {
	ret += BS("Last login: " + ctime(i = ob->query_login_time()) + ", " +
		  CONV_REALTIME(time() - i) + " ago from " +
		  ob->query_login_from() + "\n", 90, 12)[12..];
    }
    if (str = _SYSTEMD->query_plan_file(name))
    {
	ret += "------------------------------------------------------------"+
	       "-----------------\n" + str;
    }
    tp->catch_tell(ret);
    return 1;
}
