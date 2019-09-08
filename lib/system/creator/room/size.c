static nomask void
do_size(string str)
{
    int x, y;
    
    if (!str || sscanf(str, "%d,%d", x, y) != 2)
    {
	tp->catch_tell("Wrong syntax, cancelled.\n");
	return;
    }
    xsize = x;
    ysize = y;
    tp->catch_tell("New size set.\n");
}
