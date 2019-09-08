/*
 * Function name: count
 * Description:   count items
 * Arguments:     what to count
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
count(string str, object tp)
{
    int    i, sz, heap, num;
    string short;
    object here, *list;
    
    if (!str)
    {
	tp->catch_tell("Usage: count <item>\n");
	return -1;
    }

    here = E(tp);
    list = I(tp, 7) + tp->visible(I(here, 0));
    list = parse(str, list, tp);
    if (!(sz = sizeof(list)))
    {
	tp->catch_tell("Usage: count <item>\n");
	return -1;
    }
    for (i=0; i<sz; i++)
    {
	if ((heap = list[i]->query_heap_size()) > 1)
	{
	    if (sz == 1 && IN_ARRAY(str, list[i]->query_names()))
	    {
		num = 1;
		break;
	    }
	    num += heap;
	}
	else
	    num++;
    }
    tell_objects(LISTEN(here) - ({ tp }),
		 ({ QCTNAME(tp), " starts to count something.\n"}),
		 MSG_SEE, tp);
    tp->change_int_prop(LIVE_LOST_ROUNDS, 1);
    if (num == 1)
    {
	if (living(list[0]))
	    short = list[0]->query_nonmet_name();
	else
	    short = list[0]->short();
	tp->catch_tell("You see one single " + short + ".\n");
	return 1;
    }
    if (str == "all")
	str = "things";
    if (num > 12)
	tp->catch_tell("You count " + num + " " + str + ".\n");
    else
	tp->catch_tell("You count " + LANG_NUMW(num) + " " + str + ".\n");
	
    return 1;
}
