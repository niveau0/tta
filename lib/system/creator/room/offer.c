static nomask void
do_offer(string str)
{
#ifdef SHOP
    int    count, max, t;
    string file;
    
    if (!str || sscanf(str, "%s,%d,%d,%d", file, count, max, t) != 4)
    {
	if (str && sscanf(str, "%s,remove", file))
	{
	    remove_offer(file);
	    tp->catch_tell("Removed " + file + " from shop offer.\n");
	}
	else
	    tp->catch_tell("Wrong syntax, cancelled\n");
	return;
    }
    add_offer(file, count, max, t);
    tp->catch_tell("New offer added.\n");
#endif
}
