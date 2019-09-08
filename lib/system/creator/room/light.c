static nomask void
do_light(string str)
{
    int l;
    
    if (!str || !sscanf(str, "%d", l))
    {
	tp->catch_tell("Wrong syntax, cancelled.\n");
	return;
    }
    light = l;
    tp->catch_tell("New light set.\n");
}
