#include <net.h>

static nomask void
create()
{
    open_port("tcp", PORT_TCP);
}

static nomask void
message(string msg)
{
    _DRIVER->message(msg);
}

nomask void
open(int p)
{
    call_out("message", 0, "Port " + PORT_TCP +
	     " (tcp) successfully opened.\n");
}

static nomask void
close()
{
    call_out("message", 0, "Port " + PORT_TCP + " (tcp) closed.\n");
}

static nomask object
connection(string ipnum, int p)
{
    message(ipnum + " connected on port " + PORT_TCP + " (from " + p + ").\n");
    return clone_object(_BINARY);
}

nomask int
_Q_destruct()
{
    return _ADMIN;
}
