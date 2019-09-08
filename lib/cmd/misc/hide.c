/*
 * Function name: hide 
 * Description:   hide somewhere, hide something
 * Arguments:     str - what or where to hide
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
hide(string str, object tp)
{
    int    i;
    object ob, here, split, *list;
    
    here = E(tp);
    if (!str)
    {
	tp->add_state(HIDING);
	tp->add_prop(OBJ_HIDE, tp->skill_check(SK_HIDE, 0, 0));
	tp->echo("You try to hide yourself.\n");
	tell_objects(LISTEN(here) - ({ tp }),
		     ({ "You notice ", QTNAME(tp), " trying to hide.\n" }),
		     MSG_SEE, tp);
	return 1;
    }
    list = parse(str, I(tp, 4), 0);
    if (!check_size(list, "You have no '" + str + "' to hide.\n", tp))
	return -1;
    ob = list[0];
    ob->set_coordinates(tp->query_coordinates());
    ob->move(here);
    if (split = ob->last_split_obj())
	ob = split;
    ob->add_prop(OBJ_HIDE, tp->skill_check(SK_HIDE_ITEM, 0, 0));
    tp->echo("You try to hide it as good as you can think of.\n");
    list = LISTEN(here) - ({ tp });
    for (i=sizeof(list); i--;)
	if (!VISIBLE(list[i], ob))
	    list[i] = 0;
    list -= ({ 0 });
    tell_objects(list, ({ "You notice ", QTNAME(tp), " trying to hide ",
			      QNAME(ob), ".\n" }), MSG_SEE, tp);
    return 1;
}
