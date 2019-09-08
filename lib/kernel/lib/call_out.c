/*
 * Function name: call_out
 * Description:   start a delayed local function call
 * Arguments:     func - function to call after delay
 *                delay - call delay (seconds)
 *                arg - arguments to pass to delayed function
 * Returns:       integer, unique call_out id
 */
static nomask varargs int
call_out(string func, int delay, mixed arg...)
{
		object o;
		
    if (!func)
	BADARG(1, call_out);
    
#ifdef _LOG_CALLOUT
		o = find_object(_SYSTEMD);
		if (o != nil)
    	o->add_call_out(this_object());
#endif
    return ::call_out("_F_call_out", delay, func, arg...);
}

nomask varargs void
_F_call_out(string func, mixed arg...)
{
    if (previous_object()) /* no calls from other objects */
	return;
    
    set_this_player(0);
    _GLOBAL->delete_interactive(); /* needed for security */
    
    rlimits(50; 800000)
    {
	call_other(this_object(), func, arg...);
    }
}

/*
 * Function name: remove_call_out
 * Description:   remove a pending call_out
 * Arguments:     value - string: delayed function
 *                        integer: call_out id
 * Returns:       remaining delay time, else -1
 */
static nomask int
remove_call_out(mixed value)
{
    int   i;
    mixed **clist, *c;

    if (!value)
	return -1;
    if (typeof(value) == T_INT)
	return ::remove_call_out(value);
    clist = status(this_object())[O_CALLOUTS];
    for (i=sizeof(clist); i--;)
    {
	if (sizeof(c=clist[i]) > 3 && c[3] == value)
	    return ::remove_call_out(c[0]);
    }
    return -1;
}

/*
 * Function name: find_call_out
 * Description:   search a pending call_out
 * Arguments:     value - string: delayed function
 *                        integer: call_out id
 * Returns:       remaining delay time, else -1
 */
static nomask int
find_call_out(mixed value)
{
    int   i;
    mixed **clist, *c;

    clist = status(this_object())[O_CALLOUTS];
    if (typeof(value) == T_INT)
    {
	for (i=sizeof(clist); i--;)
	{
	    if (sizeof(c=clist[i]) > 3 && c[0] == value) 
		return c[2];
	}
	return -1;
    }
    for (i=sizeof(clist); i--;)
    {
	if (sizeof(c=clist[i]) > 3 && c[3] == value) 
	    return c[2];
    }
    return -1;
}