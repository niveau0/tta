/*
 * Function name: throw
 * Description:   throw items
 * Arguments:     str - what to throw at whom
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
throw(string str, object tp)
{
    int     skill, diff, ob_roll, fumble, penalty, *co_tp, *co_tar;
    string  whom;
    object  ob, at, room, here, split, *list;
    mapping at_data;
    mixed   *am;
    
    if (!str || sscanf(str, "%s at %s", str, whom) != 2)
    {
	tp->catch_tell("Usage: throw <what> at <whom>\n");
	return -1;
    }
    if (!(co_tp = tp->query_coordinates()))
	return -1;
    list = tp->query_hold(SLT_ALL) + tp->query_wield(SLT_ALL);
    list = parse(str, list, 0);
    if (!check_size(list, "You don't hold any '" + str + "'.\n", tp))
	return -1;
    ob = list[0];
    if (call_event(E_REMOVE_TOOL, tp, ob))
	return -1;
    here = E(tp);
    if (whom == "ground" || whom == "floor")
    {
	ob->set_coordinates(co_tp);
	if (ob->move(here))
	    return -1;
	if (split = ob->last_split_obj())
	    ob = split;
	tp->echo("You throw your " + str + " on the ground.\n");
	tell_objects(LISTEN(here) - ({ tp }),
		     ({ QCTNAME(tp), " throws ",
			QNAME(ob), " on the ground.\n" }), MSG_SEE, tp);
	tp->change_int_prop(LIVE_LOST_ROUNDS, 1);
	return 1;
    }
    list = tp->visible(I(here, 0));
    list = parse(whom, list, tp);
    if (!check_size(list, "No '" + whom + "' to throw at.\n", tp))
	return -1;
    at = list[0];
    co_tar = at->query_coordinates(co_tp);
    if ((penalty = range_attack_check(at, tp, ob, co_tar, co_tp)) == -1)
	return -1;
    if (!(room = here->query_leaving_room(co_tar)))
	room = here;
    if (!(skill = ob->query_throw_skill()) ||
	!mappingp(at_data = ob->query_data()))
    {
	/* create attack data if thrown object is not a throw-weapon */
	am = ATMOD_THROW;
	at_data = ([
	    A_SKILL     : SK_ATTACK_THROW,
	    A_OBMOD     : OBMOD_THROW,
	    A_DMOD      : am[0],
	    A_SKIN      : am[1],
	    A_S_LEATHER : am[2],
	    A_R_LEATHER : am[3],
	    A_CHAIN     : am[4],
	    A_PLATE     : am[5],
	    A_CRIT      : am[6],
	    A_FRANGE    : 6,
	    ]);
    }
    else
	at_data[A_SKILL] = skill;
    
    if ((ob_roll = roll_dice(-1)) <= at_data[A_FRANGE])
    {
	ob->set_coordinates(co_tp);
	ob->move(room);
	FUMBLE(at_data[A_SKILL], ob, tp, at);
	return -1;
    }
    ob->set_coordinates(co_tar);
    if (ob->move(room))
	return -1;
    if (split = ob->last_split_obj())
	ob = split;
    
    ob_roll = tp->cb_query_missile_ob(at_data[A_SKILL], ob) + ob_roll;
    ob_roll -= ob_roll * penalty / 100;
    
    tp->change_int_prop(LIVE_LOST_ROUNDS, 1);
    tp->skill_check(at_data[A_SKILL], 0, 0); /* care for skill usage */
    
    if (living(at))
    {
	diff = ob_roll - at->cb_query_missile_db();
	if (diff > 0)
	{
	    tp->catch_msg(({ "You throw your " + str + " and hit ",
			     QTNAME(at), ".\n" }), tp, MSG_SEE);
	    tell_objects(LISTEN(here) - ({ tp, at }),
			 ({ QCTNAME(tp), " throws ", QNAME(ob), " and hits ",
			    QTNAME(at), ".\n" }), MSG_SEE, tp);
	    at->catch_msg(({ QCTNAME(tp), " throws ",
			     QNAME(ob), " and hits you.\n" }), tp, MSG_SEE);
	    at->cb_attacked_by(tp);
	    at->check_death(DAMAGE_LIVING(tp, at, diff, at_data, 
					  at->cb_random_hitloc()), tp);
	    return 1;
	}
	at->catch_msg(({ QCTNAME(tp), " throws ",
			 QNAME(ob), ", but misses you.\n" }), tp, MSG_SEE);
    }
    else
    {
	diff = ob_roll - (50 - at->query_size());
	if (diff > 0)
	{	
	    tp->catch_msg(({ "You throw your " + str + " and hit ",
				 QTNAME(at), ".\n" }), tp, MSG_SEE);
	    tell_objects(LISTEN(here) - ({ tp, at }),
			 ({ QCTNAME(tp), " throws ", QNAME(ob), " and hits ",
				QTNAME(at), ".\n" }), MSG_SEE, tp);
	    return 1;
	}
    }
    tp->catch_msg(({ "You throw your " + str + " at ",
		     QTNAME(at), ".\n" }), tp, MSG_SEE);
    tell_objects(LISTEN(here) - ({ tp, at }),
		 ({ QCTNAME(tp), " throws ", QNAME(ob), ", but misses ",
		    QTNAME(at), ".\n" }), MSG_SEE, tp);
    return 1;
}
