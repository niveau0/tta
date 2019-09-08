#include <net.h>

static int    timestamp;
static string lastip;

static nomask void
create()
{
    open_port("tcp", PORT_HTTP);
}

static nomask void
message(string msg)
{
    _DRIVER->message(msg);
}

nomask void
open(int p)
{
    call_out("message", 0, "Port " + PORT_HTTP +
	     " (http) successfully opened.\n");
}

static nomask void
close()
{
    call_out("message", 0, "Port " + PORT_HTTP + " (http) closed.\n");
}

static nomask object
connection(string ipnum, int p)
{
    /* don't log each and every connection from same site */
    if (lastip != ipnum || time() > timestamp + 10)
    {
	_DRIVER->message(ipnum + " connected on port " + PORT_HTTP + 
			 " (from " + p + ").\n");
	lastip = ipnum;
	timestamp = time();
    }
    return clone_object(_HTTP);
}

nomask int
_Q_destruct()
{
    return _ADMIN;
}
