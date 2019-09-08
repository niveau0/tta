/* 
 * This module maintains the list of banned sites
 */

#define LOCKED  0
#define BLOCKED 1

string Lockedlist,  /* locked sites for new player logins */
       Blockedlist; /* totally blocked sites */

/*
 * Function name: banish
 * Description:   Banish a name, info about name, remove a banishment.
 * Arguments:     name - the name to perform banish operation on.
 *		  what - what to do.
 * Returns:       array - A list with information.
 */
nomask mixed *
banish(string name, int what)
{
    string file;
    mixed  *info;

    info = allocate(2);

    if (object_name(previous_object()) != CODER_SOUL &&
	previous_object() != this_object())
	return ({ });

    file = "/syslog/banish/" + extract(name, 0, 0) + "/" + name;
    if (file_size(file) > -1)
    {
	info[0] = read_file(file);
	info[1] = file_time(file);
    }
    switch (what)
    {
      case 0: /* Information */
	if (file_size(file) > -1)
	    return info;
	break;
      case 1: /* Remove */
	if (file_size(file) > -1)
	{
	    remove_file(file);
	    return info;
	}
	break;
      case 2: /* Add */
	if (file_size(file) > -1)
	    return info;
	write_file(file, geteuid(previous_object()));
	break;
      default: /* Strange */
	break;
    }
    return ({ });
}

/*
 * Function name: checklist
 * Description:   check if site to query is in data string
 * Arguments:     data - all blocked or locked sites
 *                ipnumber - ipnumber to check
 * Returns:       1 if found, 0 if not in data string
 */
static nomask int
checklist(string data, string ipnumber)
{
    int    i, j, sz, sz2;
    string *list, *locklist, *cmplist;

    if (!data)
	return 0;
    
    list    = explode(data, " ");
    cmplist = explode(ipnumber, ".");
    
    for (i=0, sz=sizeof(list); i<sz; i++)
    {
	locklist = explode(list[i], ".");
	for (j=0, sz2=sizeof(locklist); j<sz2; j++)
	{
	    if (locklist[j] != cmplist[j])
		break;
	}
	if (j == sz2)
	    return 1;
    }
    return 0;
}

/*
 * Function name: check_newplayer
 * Description:   Check if ipnumber may log in. Called from _LOGIN.
 * Arguments:     ipnumber - ipnumber of player that logs in
 * Returns:       1 if no entry from site allowed,
 *                2 if no new players allowed
 *                0 else
 */
nomask int
check_newplayer(string ipnumber)
{
    if (checklist(Blockedlist, ipnumber))
	return 1;
    if (checklist(Lockedlist, ipnumber))
	return 2;
    return 0;
}

/*
 * Function name: add_numbers
 * Description:   Adds a site to the list
 * Arguments:     which - flag if lock or block data
 *                ipnumber - ipnumber to add
 */
static nomask int
add_numbers(int which, string ipnumber)
{
    object tp;

    tp = this_player();
    if (!ipnumber)
    {
	tp->catch_tell("Usage: lockout/blocksite add ipnumber\n");
	return -1;
    }
    if (which == LOCKED)
    {
	if (Lockedlist)
	    Lockedlist += " " + ipnumber;
	else
	    Lockedlist = ipnumber;
    }
    else
    {
	if (Blockedlist)
	    Blockedlist += " " + ipnumber;
	else
	    Blockedlist = ipnumber;
    }
    save_object(SAVEFILE);
    tp->catch_tell("Added site '" + ipnumber + "' to list.\n");
    return 1;
}

/*
 * Function name: remove_numbers
 * Description:   Removes a site from the list
 * Arguments:     which - flag if lock or block data
 *                number - listindex to remove
 */
static nomask int
remove_numbers(int which, string number)
{
    int    nr;
    string *list;
    object tp;
    
    tp = this_player();
    if (!number)
    {
	tp->catch_tell("Usage: lockout/blocksite remove <listindex>\n");
	return -1;
    }
    if (which == LOCKED)
    {
	if(!Lockedlist)
	{
	    tp->catch_tell("No ipnumbers stored in list.\n");
	    return -1;
	}
	list = explode(Lockedlist, " ");
    }
    else
    {
	if(!Blockedlist)
	{
	    tp->catch_tell("No ipnumbers stored in list.\n");
	    return -1;
	}
	list = explode(Blockedlist, " ");
    }
    sscanf(number, "%d", nr);
    if (nr > sizeof(list))
    {
	tp->catch_tell("There are only " + sizeof(list) +
		       " ipnumbers stored in the list.\n");
	return -1;
    }
    if (nr < 1)
    {
	tp->catch_tell("There are only " + sizeof(list) +
		       " ipnumbers stored in the list.\n");
	return -1;
    }
    tp->catch_tell("Site #" + nr + ": '" + list[nr-1] + "' removed.\n");
    list = list[..nr-2] + list[nr..];
    if (which == LOCKED)
    {
	if (!sizeof(list))
	    Lockedlist = 0;
	else
	    Lockedlist = implode(list, " ");
    }
    else
    {
	if (!sizeof(list))
	    Blockedlist = 0;
	else
	    Blockedlist = implode(list, " ");
    }
    save_object(SAVEFILE);
    return 1;
}

/*
 * Function name: list_numbers
 * Description:   List the stored numbers.
 * Arguments:     which - flag if lock or block data
 */
static nomask int
list_numbers(int which)
{
    int    i, sz;
    string str, *list;
    object tp;

    tp = this_player();
    if (which == LOCKED)
    {
	if (!Lockedlist)
	{
	    tp->catch_tell("No numbers stored.\n");
	    return -1;
	}
	list = explode(Lockedlist, " ");
    }
    else
    {
	if (!Blockedlist)
	{
	    tp->catch_tell("No numbers stored.\n");
	    return -1;
	}
	list = explode(Blockedlist, " ");
    }
    str = "";
    for (i=0, sz=sizeof(list); i<sz; i++)
	str += "#" + (i + 1) + ": " + list[i] + "\n";
    tp->catch_tell(str);
    return 1;
}

/*
 * Function name: lockout_site
 * Description:   Lock out newplayer logins from certain sites.
 * Arguments:     arg - what to lockout
 */
nomask int
lockout_site(string arg)
{
    int    i, sz;
    string args, cmd;
    object tp;
    mixed  list;

    tp = this_player();
    if (_Q_coder_level(geteuid(previous_object())) < _CREATOR)
    {
	tp->catch_tell("You are not authorized to do this.\n");
	return -1;
    }
    if (arg)
    {
	list = explode(arg, " ");
	cmd  = list[0];
	if ((sz = sizeof(list)) > 1)
	{
	    for (i=0; i<sz - 1; i++)
		list[i] = list[i + 1];
	    list[i] = "";
	    args = implode(list, " ");
	}
	if (cmd == "list")
	    return list_numbers(LOCKED);
	if (cmd == "add")
	    return add_numbers(LOCKED, args);
	if (cmd == "remove")
	    return remove_numbers(LOCKED, args);
    }
    tp->catch_tell("Usage: lockout add <ipnum>\n" +
		   "       lockout remove <listnum>\n" +
		   "       lockout list\n");
    return -1;
}

/*
 * Function name: block_site
 * Description:   Lock out player logins from certain sites.
 * Arguments:     arg - what to block
 */
nomask int
block_site(string arg)
{
    int    i, sz;
    string cmd, args;
    object tp;
    mixed  list;

    tp = this_player();
    if (_Q_coder_level(geteuid(previous_object())) < _CREATOR)
    {
	tp->catch_tell("You are not authorized to do this.\n");
	return -1;
    }
    if (arg)
    {
	list = explode(arg, " ");
	cmd  = list[0];
	if ((sz = sizeof(list)) > 1)
	{
	    for (i=0, --sz; i<sz; i++)
		list[i] = list[i + 1];
	    list[i] = "";
	    args = implode(list, " ");
	}
	if (cmd == "list") 
	    return list_numbers(BLOCKED);
	if (cmd == "add") 
	    return add_numbers(BLOCKED, args);
	if (cmd == "remove") 
	    return remove_numbers(BLOCKED, args);
    }    
    tp->catch_tell("Usage: blocksite add <ipnum>\n" +
		   "       blocksite remove <listnum>\n" +
		   "       blocksite list\n");
    return -1;
}
