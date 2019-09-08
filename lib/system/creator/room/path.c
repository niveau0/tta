static nomask void
do_path(string str)
{
    if (!str || (sscanf(str, "/d/%*s/%*s") != 2 &&
		 sscanf(str, "/home/%*s/%*s") != 2))
    {
	tp->catch_tell("Invalid roompath. Allowed are: " +
		       "/d/<Domain>/... or /home/<yourname>/...\n");
	return;
    }
    room_file = str;
    tp->catch_tell("New path set.\n");
}
