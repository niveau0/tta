nomask void /* needs to be non-static for editor call-back */
do_long(string str)
{
    if (!str || !strlen(str))
    {
	tp->catch_tell("Cancelled, nothing changed.\n");
	return;
    }
    room_long = str;
    tp->catch_tell("New long description set.\n");
}

static nomask void
do_short(string str)
{
    room_short = BS(str, 78, 0);
    tp->catch_tell("New short description set.\n");
}
