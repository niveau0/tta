static nomask void
do_height(string str)
{
    int h;

    if (str == "default")
	h = DEFAULT_ROOM_HEIGHT;
    else if (!str || !sscanf(str, "%d", h) || h < 1)
    {
	tp->catch_tell("Wrong syntax, cancelled.\n");
	return;
    }
    height = h; 
    tp->catch_tell("New height set.\n");
}
