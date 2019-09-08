#include <net.h>

static nomask void
create()
{
    open_port("tcp", PORT_MAPSERVER);
}

static nomask void
message(string msg)
{
    _DRIVER->message(msg);
}

nomask void
open(int p)
{
    call_out("message", 0, "Port " + PORT_MAPSERVER +
	     " (mapserver) successfully opened.\n");
}

static nomask void
close()
{
    call_out("message", 0, "Port " + PORT_MAPSERVER + " (tcp) closed.\n");
}

static nomask object
connection(string ipnum, int p)
{
    _DRIVER->message(ipnum + " connected on port " + PORT_MAPSERVER + 
			" (from " + p + ").\n");
    return clone_object(_MAPSERVER);
}

nomask int
_Q_destruct()
{
    return _ADMIN;
}
