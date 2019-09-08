#define SETTLER_PORT 3400

nomask void
create()
{
    open_port("tcp", SETTLER_PORT);
}

nomask void
open(int p)
{
    _DRIVER->message("Port " + SETTLER_PORT +
			" (settler tcp) successfully opened.\n");
}

static nomask void
close()
{
    _DRIVER->message("Port " + SETTLER_PORT + " closed.\n");
}

static nomask object
connection(string ipnum, int p)
{
    _DRIVER->message(ipnum + " connected on port " + SETTLER_PORT + 
			" (from " + p + ").\n");
    return clone_object("/kernel/net/settlerreceive");
}
