/*
 * Function name: tell_objects
 * Description:   send a message to listening objects through catch_msg  
 * Arguments:     obj - all message receiving objects
 *                str - message
 *                type - message type (MSG_HEAR, etc)
 *                actor - responsible object (e.g. talking player)
 *                d - loudness (if audible message)
 *                no_prompt - true if no prompt should be sent after message
 */
static nomask varargs void
tell_objects(object *obj, mixed str, int type, object actor, int d,
	     int no_prompt)
{
    int i;

    if (typeof(obj) != T_ARRAY)
	BADARG(1, tell_objects);
    if (!str)
	BADARG(2, tell_objects);

    for (i=sizeof(obj); i--;)
    {
	if (obj[i])
	    obj[i]->catch_msg(str, actor, type, d, no_prompt);
    }
}

/*
 * Function name: input_to
 * Description:   redirect user input to a function
 * Arguments:     func - function to call with user input
 *                flag - if true input echoing is switched off
 *                args - additional arguments to 'func' 
 * Returns:       true if success, 0 else
 */
static nomask varargs int
input_to(string func, int flag, mixed args...)
{
    int    res;
    object player;

    if (!func)
	BADARG(1, input_to);

    player = this_player()->query_user();
    if (player && ::query_ip_number(player))
    {
	rlimits(-1; -1)
	{
	    res = player->_F_set_input_to(func, flag, args...);
	}
	return res;
    }
    error("input_to called for non-interactive object");
}

/*
 * Function name: possession
 * Description:   take control over someone
 * Arguments:     whom - living to control
 *                bywhom - possessing living
 * Returns:       1 if success, 0 else
 */
static nomask int
possession(object whom, object bywhom)
{
    object usr, puser, snooper;

    if (!whom)
	BADARG(1, possession);
    if (!bywhom)
	BADARG(2, possession);
    if (!(puser = bywhom->query_user())) 
	error("Bad argument 2 for function possession, not interactive");
    if (whom == bywhom) 
	error("Tried to possess same object");
    if (!_ACCESS_FORCE(bywhom, whom))
	return 0;
    if (!(usr = whom->query_user())) 
    {
	/* possessing non-interactive */
	rlimits(-1; -1)
        {
	    puser->switch_player(whom);
	}
	whom->set_user(puser);
	bywhom->set_user(0);
	return 1;
    }
    if (usr->query_splayer()) 
	return 0; /* already possessed */
    rlimits(-1; -1)
    {
	snooper = usr->_Q_snoop();
    }
    if (snooper == puser)
	error("Cannot possess snooped living");
    rlimits(-1; -1)
    {
	usr->switch_player(0); /* save possessed persons player */
	puser->switch_player(whom);
	puser->_F_snoop(usr);
    }
    whom->set_user(puser); /* we are controlled by demon now */
    bywhom->set_user(0);
    return 1;
}

/*
 * Function name: break_possession
 * Description:   stop a possession
 * Arguments:     possessed - controlled living
 *                breaker - living that tries to stop the possession
 * Returns:       1 if success, 0 else
 */
static nomask int
break_possession(object possessed, object breaker)
{
    int    i;
    object usr, puser, *all, pl, opl;

    if (!possessed)
	BADARG(1, break_possession);
    if (!breaker)
	BADARG(2, break_possession);

    if (!_ACCESS_FORCE(breaker, possessed))
	return 0;
    if (!(puser = possessed->query_user())) /* get demons user */
	return 0;
    if (!(opl = puser->query_splayer())) /* original player of demon */
	return 0;
    all=::users();
    for (i=sizeof(all); i--;)
    {
	if (all[i] && all[i]->query_splayer() == possessed)
	{
	    usr = all[i]; /* found the possessed user */
	    break;
	}
    }
    if (!usr) /* not found, so it was an npc, or lost connection */
    {
	rlimits(-1; -1)
	{
	    puser->switch_player(opl); /* switch demon to own player */
	    puser->_F_reset_splayer();
	}    
	opl->set_user(puser); 
	possessed->set_user(0);
	return 1;
    }
    usr->catch_tell("The evil soul left your body.\n");
    rlimits(-1; -1)
    {
	usr->switch_player(possessed); /* switch possessed to own player */
	usr->_F_reset_splayer(); 
	puser->switch_player(opl); /* switch demon to own player */
	puser->_F_reset_splayer();
	puser->_F_snoop(0);
    }
    possessed->set_user(usr);
    opl->set_user(puser);
    puser->catch_tell("You return to your old body.\n");
    return 1;
}

/*
 * Function name: snoop
 * Description:   watch someones input
 * Arguments:     snooper - watching living
 *                snoopee - victim
 * Returns:       1 if success, 0 else, object if another snoop was broken
 */
static nomask mixed
snoop(object snooper, object snoopee)
{
    object current, user, suser;
    
    if (!snooper || !snoopee || snoopee == snooper ||
	!::query_ip_number(user = snooper->query_user()))
	return 0;
    if (!_ACCESS_SNOOP(this_object(), snooper, snoopee))
	return 0;
    suser = snoopee->query_user();
    rlimits(-1; -1)
    {
	current = suser->_Q_snoop(); /* get current snooper */
    }
    if (current && (current = current->query_player()))
    {
	/* if already snooped, try to break */
	if (current != snooper &&
	    !_ACCESS_SNOOP(this_object(), snooper, current))
	    return 0;
	rlimits(-1; -1)
	{
	    suser->_F_snoop(0);
	}
	return current;
    }
    rlimits(-1; -1)
    {
	current = user->_Q_snoop(); /* get snooper's current snooper */
    }
    if (current == suser) /* avoid recursive snoop */
	return 0;
    rlimits(-1; -1)
    {
	suser->_F_snoop(user); /* set snoop on snoopee */
    }
    return 1;
}

/*
 * Function name: query_snoop
 * Description:   give snooper of a player
 * Arguments:     obj - living to query
 * Returns:       object if found, 0 else
 */
static nomask object
query_snoop(object obj)
{
    object snooper;

    if (!obj)
	BADARG(1, query_snoop);

    if (!::query_ip_number(obj->query_user()))
	return 0;
    rlimits(-1; -1)
    {
	snooper = obj->query_user()->_Q_snoop();
    }
    if (!snooper)
	return 0;
    snooper = snooper->query_player();
    if (!snooper || !_ACCESS_SNOOP(this_object(), snooper, snooper))
	return 0;
    return snooper;
}
