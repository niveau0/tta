#include <net.h>

static nomask void
create()
{
  open_port("telnet", PORT_TELNET);
}


static nomask void
message(string msg)
{
  _DRIVER->message(msg);
}

nomask void
open(int p)
{
  call_out("message", 0, "Port " + PORT_TELNET +
	   " (telnet) successfully opened.\n");
}

static nomask void
close()
{
  call_out("message", 0, "Port " + PORT_TELNET + " (telnet) closed.\n");
}

static nomask object
connection(string ipnum, int p)
{
  _DRIVER->message(ipnum + " connected on port " + PORT_TELNET +
		   " (from " + p + ").\n");
  return clone_object(_USER);
}

nomask int
_Q_destruct()
{
  return _ADMIN;
}
