/*
 * Function name: steal
 * Description:   steal something from someone/somewhere
 * Arguments:     str - what to steal
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
steal(string str, object tp)
{
    int    i, sz, pp, range, inside;
    string what, from;
    object ob, here, *list, *listen;
    mixed  tpqname, obqname;

    if (!str || sscanf(str, "%s of %s", what, from) != 2)
    {
	tp->catch_tell("Usage: steal [from] <what> of <whom>\n");
	return 1;
    }
    here = E(tp);
    list = tp->visible(I(here, 2));
    list = parse(from, list, tp, tp->query_step());
    if (!check_size(list, "No '" + from + "' in range to steal from.\n", tp))
	return -1;
    if ((ob = list[0]) == tp)
    {
	tp->catch_tell("What? Yes, you noticed your attempt.\n");
	return -1;
    }
    if (sscanf(what, "from %s", what)) /* steal from worn container? */
    {
	inside = 1;
	list = tp->visible(I(ob, 1));
    }
    else
	list = tp->visible(ob->query_worn(SLT_ALL));
    list = parse(what, list, 0);
    if (!check_size(list, "No '" + what + "' to steal" +
		    (inside ? " from" : "") + ".\n", tp))
	return -1;
    if (list[0]->check_armor())
    {
	tp->catch_tell("That is much too difficult.\n");
	return -1;
    }
    tpqname = QNAME(tp),
    obqname = QNAME(ob);
    listen  = LISTEN(here) - ({ tp, ob });
    pp = tp->skill_check(SK_PICK_POCKET, 0, 0);
    /* people around may notice the steal attempt */
    for (i=sizeof(listen); i--;)
    {
	if (listen[i]->skill_check(SK_PERCEPTION, -pp - 80, 1) > 0)
	    listen[i]->catch_msg(({ "You notice ", tpqname,
					" stealing from ", obqname, ".\n" }),
				 tp, MSG_SEE);
    }
    if (!VISIBLE(ob, tp))
	range = 70; /* stealing unseen is easier */
    else
	range = 50;
    if (inside)
	range -= 20; /* stealing from container is harder */
    else
	/* cutting bags will be noticed if heavy */
	range -= list[0]->query_weight() / 50;
    
    /* check if stealing was failure */
    if (ob->skill_check(SK_PERCEPTION, -pp - range) > 0)
    {
	tp->delete_state(HIDING);
	tp->add_prop(OBJ_HIDE, 0);
	tp->catch_tell("Your victim has noticed your attempt.\n");
	ob->catch_msg(({ "You notice ", tpqname, " touching you.\n" }), 0, 0);
	if (ob->query_npc())
	{
	    ob->cb_attack_object(tp);
	    tell_objects(listen, ({ QCTNAME(tp), " attacks ", obqname,
				    ".\n" }), MSG_SEE, tp);
	}
	return 1;
    }
    if (inside)
    {
	if (!(sz = sizeof(list = I(list[0], 7))))
	{
	    tp->catch_tell("There seems to be nothing inside.\n");
	    return -1;
	}
	list = ({ list[random(sz)] });
    }
    
    /* stealing heaps: amount depends on size */
    if (list[0]->query_heap_size())
    {
	switch (list[0]->query_size())
	{
	  case 0..2:
	    list[0]->split_heap(random(20) + 1);
	    break;
	  case 3..5:
	    list[0]->split_heap(random(10) + 1);
	    break;
	  default:
	    list[0]->split_heap(1);
	}
    }
    list = try_get(list, tp, tp->query_coordinates());
    if (!sizeof(list))
	return 1;
    tp->catch_msg(({ "You stole " }) + COMPOSITE_ARRAY(list) +
		  ({ ".\n" }), 0, 0);
    return 1;
}
