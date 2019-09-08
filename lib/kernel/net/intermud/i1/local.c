/*
 * Function name: local_init
 * Description:   initialize function
 */
static nomask void
local_init()
{
}

/*
 * Function name: local_access
 * Description:   this function checks if the calling object is allowed
 *                to send an udp request
 * Arguments:     obj - the calling object
 * Returns:       1 if allowed, 0 else
 */
static nomask int
local_access(object obj)
{
    if (object_name(obj) != _IMUDD)
	return 0;
    return 1;
}

/*
 * Function name: local_info
 * Description:   function should return an info string in a proper
 *                output format
 * Arguments:     ipnum - the ip that queries for info
 *                port - the port where the query comes from
 *                name - the username that sent the query
 * Returns:       info string, or 0 to send no info
 */
static nomask string
local_info(string ipnum, int port, string name)
{
    mixed st;

    st = status();
    return ("\nUDP version:           " + UDP_VERSION + 
	    "\nLoaded objects:        " + st[ST_NOBJECTS] + 
	    "\nDGD Version (net):     " + st[ST_VERSION] + 
	    "\nLocal time:            " + ctime(time()) +
	    "\nStart time:            " + ctime(st[ST_STARTTIME]) +
	    "\nBoot  time:            " + ctime(st[ST_BOOTTIME]) +
	    "\nUp time:               " +
	    ("/system/mudtime"->conv_real_time(st[ST_UPTIME])) + "\n");
}

/*
 * Function name: local_who
 * Description:   function should return a user list in a proper
 *                output format
 * Arguments:     ipnum - the ip that queries for user list
 *                port - the port where the query comes from
 *                name - the username that sent the query
 * Returns:       user list, or 0 to send no info
 */
static nomask string
local_who(string ipnum, int port, string name)
{
    int    i, sz;
    string out, n;
    object *u;

    u = users();
    out = "";
    for (i=0, sz=sizeof(u); i<sz; i++)
    {
	if (!(n = u[i]->query_name()))
	    continue;
	out += sprintf("  %s%s - %-25s\n",
		       (_ACCESS_LEVEL(geteuid(u[i])) > 1 ? "C" : " "),
		       (query_idle(u[i]) > 300 ? "I" : " "),
		       capitalize(n));
    }
    out = "User list of " + IMUD_MUDNAME +
	  "  ---  Flags: (C)oder, (I)dle\n" +
	  sprintf("%   *75s", out);
    return out;
}

/*
 * Function name: local_ch_list
 * Description:   function should return a channel list in a proper
 *                output format
 * Arguments:     ipnum - the ip that queries for user list
 *                port - the port where the query comes from
 *                name - the username that sent the query
 * Returns:       channel list, or 0 to send no info
 */
static nomask string
local_ch_list(string ipnum, int port, string name)
{
    int    i, j, szi, szj;
    string out, *ind, *members;
    object u;

    out = "";
    ind = map_indices(channels);
    for (i=0, szi=sizeof(ind); i<szi; i++)
    {
	for (j=0, szj=sizeof(members=channels[ind[i]]); j<szj; j++)
	    if (!(u = USER(members[j])) || !query_ip_number(u))
		members[j] = 0;
	members -= ({ 0 });
	if (!(szj=sizeof(members)))
	{
	    channels[ind[i]] = 0;
	    continue;
	}
	channels[ind[i]] = members;
	out += ind[i] + "\n";
	for (j=0; j<szj; j++)
	    out += " - " + members[j];
	out += "\n";
    }
    out = "Channel list of " + IMUD_MUDNAME + "\n" + out;
    return out;
}

/*
 * Function name: local_ch_tell
 * Description:   format a channel message
 * Arguments:     ipnum - the ip that sent a tell to a channel
 *                port - the port where the tell comes from
 *                ch - the channel name
 *                msg - the message string
 *                name - the username that sent the tell
 * Returns:       formated tell string, 0 to send nothing
 */
static nomask string
local_ch_tell(string ipnum, int port, string ch, string msg, string name)
{
    int    i;
    string ipname, *ind;
    
    ind = map_indices(iplist);
    for (i=sizeof(ind); i--;)
	if (iplist[ind[i]][LIST_IP] == ipnum &&
	    iplist[ind[i]][LIST_PORT] == port)
	{
	    ipname = ind[i];
	    break;
	}
    if (i < 0)
	return 0;
    return "<" + ch + "> " + capitalize(name) + "@" + ipname +
	": " + msg + "\n";
}

/*
 * Function name: local_tell
 * Description:   format a tell message
 * Arguments:     ipnum - the ip that sent a tell
 *                port - the port where the tell comes from
 *                msg - the message string
 *                name - the username that sent the tell
 *                receiver - the receiver that should receive the tell
 * Returns:       formated tell string, 0 to send nothing
 */
static nomask string
local_tell(string ipnum, int port, string msg, string name, string receiver)
{
    int    i;
    string ipname, *ind;
    
    ind = map_indices(iplist);
    for (i=sizeof(ind); i--;)
	if (iplist[ind[i]][LIST_IP] == ipnum &&
	    iplist[ind[i]][LIST_PORT] == port)
	{
	    ipname = ind[i];
	    break;
	}
    if (i < 0)
	return 0;
    return capitalize(name) + "@" + ipname + " tells you: " + msg + "\n";
}

/*
 * Function name: local_tell_ack
 * Description:   format a tell answer message
 * Arguments:     ipnum - the ip that sent a tell
 *                port - the port where the tell comes from
 *                msg - the message string
 *                name - the username that gets the answer now
 *                receiver - the receiver that got the tell
 * Returns:       formated tell string, 0 to send nothing
 */
static nomask string
local_tell_ack(string ipnum, int port, string msg, string name,
	       string receiver)
{
    int    i;
    string ipname, *ind;
    
    ind = map_indices(iplist);
    for (i=sizeof(ind); i--;)
	if (iplist[ind[i]][LIST_IP] == ipnum &&
	    iplist[ind[i]][LIST_PORT] == port)
	{
	    ipname = ind[i];
	    break;
	}
    if (i < 0)
	return 0;
    return "You told " + capitalize(receiver) + "@" + ipname + ": " + msg +
	"\n";
}
