/* display function for I and E cmd */
private int
show_list(string str, int flag)
{
    int    i, sz;
    object tp, ob, *list;

    tp = this_interactive();
    if (!str)
	ob = tp;
    else if (sizeof(list = parse(str, I(tp, 7) + I(E(tp), 0), 0)))
	ob = list[0];
    else if (!(ob = find_living(str)))
    {
	tp->catch_tell("No such object.\n");
	return -1;
    }
    if (!flag)
	list = I(ob, 0);
    else
	list = I(E(ob), 0);
    
    str = ("Name        Weight  Volume  Size Heap  Filename\n================"+
	   "==============================================================\n");
    for (i=0, sz=sizeof(list); i<sz; i++)
    {
	ob = list[i];
	str += sprintf("%./-10s.%./7d.%./7d.%./5d.%./4d..",
		       ob->query_name()+"", ob->query_weight(),
		       ob->query_volume(), ob->query_size(),
		       ob->query_heap_size()) +
	       BS(object_name(ob), 39, 0) + "\n";
    }
    tp->catch_tell(str);
    return 1;
}

/* **************************************************************************
 * I - show the inventory of someone
 */
nomask int
I_list(string str)
{
    CHECK_LEVEL(_BUILDER);
    
    return show_list(str, 0);
}

/* **************************************************************************
 * E - show the inventory of someones environment
 */
nomask int
E_list(string str)
{
    CHECK_LEVEL(_BUILDER);
    
    return show_list(str, 1);
}
