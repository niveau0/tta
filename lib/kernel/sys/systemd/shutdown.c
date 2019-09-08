/*****************************************************************************
 * shutdown dgd in graceful manner
 */
static void
tellall(string str, string coder)
{
    int    i;
    object *u;
    
    u = users();
    for (i=sizeof(u); i--;)
	if (_ACCESS_LEVEL(u[i]->query_name()) > 0)
	    u[i]->catch_tell(coder);
	else
	    u[i]->catch_tell(str);
}

static void
do_shut_down(int count)
{
    if (count)
    {
	tellall("A cold voice echos in your mind: I'll send you to deep " +
		"sleep in " + count + " minute" +
		(count > 1 ? "s!\n" : "!\n"),
		"Shutdown in " + count + " minute" +
		(count > 1 ? "s!\n" : "!\n") +
		"Reason: " + reason + "\n");
	count--;
	call_out("do_shut_down", 60, count);
	return;
    }
    tellall("Suddenly, darkness falls over you...\n",
	    "Shutdown by " + name + "\nReason: " + reason + "\n");
#ifdef _LOG_SHUTDOWN
    syslog("SHUTDOWN", break_string(name + ", Reason: " + reason, 76, 1));
#endif
    shutdown();
}
	    
void
shutdown_game(object s, string delay, string r)
{
    int count;

    if (!r || !s)
	return;

    if (delay == "now")
	count = 0;
    else if (!sscanf(delay, "%d", count))
	return;
    
    reason = r;
    if (!(name = s->query_name()))
	name = object_name(s);
    
    remove_call_out("do_shut_down");
    call_out("do_shut_down", 0, count);
}

void
stop_shutdown()
{
    remove_call_out("do_shut_down");
    tellall("A cold voice echos in your mind: I changed my mind!\n",
	    "Shutdown aborted.\n");
}
