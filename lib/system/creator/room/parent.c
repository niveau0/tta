static nomask void
do_parent(string str)
{
    if (!str || sscanf(str, "/d/%*s/%*s") != 2)
    {
	tp->catch_tell("Invalid parentpath. Allowed is: " +
		       "/d/<Domain>/...\n");
	return;
    }
    parent_room = str;
    tp->catch_tell("New parent set.\n");
}
