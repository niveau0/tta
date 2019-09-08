/*
 * Function name: ichlist
 * Description:   list channel members
 * Arguements:    str - optional ipname               
 * Returns:       1 if success, -1 else
 */
nomask int
ichlist(string str)
{
    object  tp;
    
    CHECK_LEVEL(_BUILDER);

    tp = this_interactive();
    if (!str)
    {
	tp->catch_tell("Usage: ilist <ipname>\n");
	return -1;
    }
    return _IMUDD->send_channellist(str, tp);
}

/*
 * Function name: iinfo
 * Description:   query info string
 * Arguments:     str - ipname
 * Returns:       1 if success, -1 else
 */
nomask int
iinfo(string str)
{
    int    type;
    string mudname;
    object tp;
    
    CHECK_LEVEL(_BUILDER);

    tp = this_interactive();
    if (!str || sscanf(str, "%d %s", type, mudname) != 2)
    {
	tp->catch_tell("Usage: iinfo <imudtype> <mudname>\n" +
		       "       (check 'ilist' for possible mudnames)\n");
	return -1;
    }
    return _IMUDD->query_info(type, mudname, tp);
}

/*
 * Function name: ijoin
 * Description:   join a channel
 * Arguments:     str - channelname
 * Returns:       1 if success, -1 else
 */
nomask int
ijoin(string str)
{
    object tp;
    
    CHECK_LEVEL(_BUILDER);
    
    tp = this_interactive();
    if (!str)
    {
	tp->catch_tell("Usage: ijoin <channelname>\n");
	return -1;
    }
    return _IMUDD->send_join(str, tp);
}

/*
 * Function name: ileave
 * Description:   leave a channel
 * Arguments:     str - channelname, ipname
 * Returns:       1 if success, -1 else
 */
nomask int
ileave(string str)
{
    object tp;
    
    CHECK_LEVEL(_BUILDER);

    tp = this_interactive();
    if (!str)
    {
	tp->catch_tell("Usage: ileave <channelname>\n");
	return -1;
    }
    return _IMUDD->send_leave(str, tp);
}

/*
 * Function name: ilist
 * Description:   list muds
 * Arguments:     str - imud protocol
 * Returns:       1 if success, -1 else
 */
nomask int
ilist(string str)
{
    int     type;
    object  tp;
    
    CHECK_LEVEL(_BUILDER);

    tp = this_interactive();
    if (!str || !sscanf(str, "%d", type))
    {
	tp->catch_tell("Usage: ilist <imudtype>\n");
	return -1;
    }
    return _IMUDD->query_mudlist(type, tp);
}

/*
 * Function name: imsg
 * Description:   send a message
 * Arguments:     str - channelname, message
 * Returns:       1 if success, -1 else
 */
nomask int
imsg(string str)
{
    string channel, msg;
    object tp;
    
    CHECK_LEVEL(_BUILDER);

    tp = this_interactive();
    if (!str || sscanf(str, "%s: %s", channel, msg) != 2)
    {
	if (!(channel = tp->query_prop("_udp_last_ip")))
	{
	    tp->catch_tell("Usage: imsg <channel>: <msg>\n" +
			   "       imsg <msg> to talk on previous " +
			   "used channel\n");
	    return -1;
	}
	msg = str;
    }
    return _IMUDD->send_channel_msg(channel, msg, 0, tp);
}

/*
 * Function name: iping
 * Description:   send ping request
 * Arguments:     str - ipname
 * Returns:       1 if success, -1 else
 */
nomask int
iping(string str)
{
    int    type;
    string ipname;
    object tp;
    
    CHECK_LEVEL(_BUILDER);

    tp = this_interactive();
    if (!str || sscanf(str, "%d %s", type, ipname) != 2)
    {
	tp->catch_tell("Usage: iping <imudtype> <ipname> <no. packets>\n");
	return -1;
    }
    return _IMUDD->send_ping(type, ipname, tp);
}

/*
 * Function name: itell
 * Description:   send message to someone
 * Arguments:     str - player (to tell to) and message to send
 * Returns:       1 if success, -1 else 
 */
nomask int
itell(string str)
{
    int    type;
    string name, msg, ipname;
    object tp;
    
    CHECK_LEVEL(_BUILDER);

    tp = this_interactive();
    if (!str || sscanf(str, "%d %s@%s %s", type, name, ipname, msg) != 4)
    {
	tp->catch_tell("Usage: itell <imudtype> <playername>@<ipname> " +
		       "<msg>\n");
	return -1;
    }
    return _IMUDD->send_tell(type, ipname, msg, name, tp);
}

/*
 * Function name: iwho
 * Description:   send remote-who request
 * Arguments:     str - ipname
 * Returns:       1 if success, -1 else
 */
nomask int
iwho(string str)
{
    int    type;
    string ipname;
    object tp;
    
    CHECK_LEVEL(_BUILDER);

    tp = this_interactive();
    if (!str || sscanf(str, "%d %s", type, ipname) != 2)
    {
	tp->catch_tell("Usage: iwho <imudtype> <ipname>\n");
	return -1;
    }
    return _IMUDD->send_who(type, ipname, tp);
}
