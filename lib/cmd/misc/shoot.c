/*
 * Function name: shoot
 * Description:   release a missile from a missile weapon
 * Arguments:     str - target
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
shoot(string str, object tp)
{
    int     i, mtype, diff, fumble, penalty, ob_roll, *co_tp, *co_tar;
    string  weapon, target;
    object  at, wob, mob, quiver, room, here, split, *list;
    mapping at_data;
    
    if (!str || sscanf(str, "at %s with %s", target, weapon) != 2)
    {
	tp->catch_tell("Usage: shoot at <whom> with <what>\n");
	return -1;
    }
    list = tp->query_wield(SLT_ALL);
    list = parse(weapon, list, 0);
    if (!check_size(list, "You wield no weapon '" + weapon + "'.\n", tp))
	return -1;
    wob = list[0];
    list = tp->visible(I(here = E(tp), 0));
    list = parse(target, list, tp);
    if (!check_size(list, "You see no '" + target + "'.\n", tp))
	return -1;
    at = list[0];
    co_tp = tp->query_coordinates(co_tp);
    co_tar = at->query_coordinates(co_tp);
    if ((penalty = range_attack_check(at, tp, wob, co_tar, co_tp)) == -1)
	return -1;
    if (!(quiver = tp->query_prop(LIVE_ATTACHED_QUIVER)))
    {
	list = parse("quiver", I(tp, 4), 0);
	if (!sizeof(list))
	{
	    tp->catch_tell("You have no quiver.\n");
	    return -1;
	}
	quiver = list[0];
	tp->add_prop(LIVE_ATTACHED_QUIVER, quiver);
    }
    list = I(quiver, 6);
    mtype = wob->query_missile_type();
    for (i=sizeof(list); i--;)
	if (list[0]->query_missile_type() == mtype)
	    break;
    if (i < 0)
    {
	tp->catch_tell("Your quiver is empty.\n");
	return -1;
    }
    if (!wob->check_missile_weapon())
    {
	tp->catch_tell("You cannot fire with your " + weapon + ".\n");
	return -1;
    }
    if (!(room = here->query_leaving_room(co_tar)))
	room = here;
    mob = list[0];
    mob->split_heap(1);
    at_data = wob->query_missile_data();
    if (!(fumble = at_data[A_FRANGE]))
	fumble = 6;
    if ((ob_roll = roll_dice(-1)) <= fumble)
    {
	mob->set_coordinates(co_tp);
	mob->move(room);
	FUMBLE(at_data[A_SKILL], wob, tp, at);
	return -1;
    }
    mob->set_coordinates(co_tar);
    if (mob->move(room))
	return -1;
    if (split = mob->last_split_obj())
	mob = split;
    
    ob_roll += at_data[A_OBMOD];
    ob_roll -= ob_roll * penalty / 100;
    
    tp->change_int_prop(LIVE_LOST_ROUNDS, 1);
    tp->skill_check(at_data[A_SKILL], 0, 0); /* care for skill usage */

    wob->check_weapon_damage(tp);

    if (living(at))
    {
	diff = ob_roll - at->cb_query_missile_db();
	if (diff > 0)
	{
	    tp->catch_msg(({ "You hit ", QTNAME(at), ".\n" }), tp, MSG_SEE);
	    tell_objects(LISTEN(E(tp)) - ({ tp, at }),
			 ({ QCTNAME(tp), " hits ", QNAME(at), " with ",
				QNAME(mob), ".\n" }), MSG_SEE, at);
	    at->catch_msg(({ QCTNAME(tp), " hits you with ", QNAME(mob),
				 ".\n" }), 0, MSG_SEE | MSG_HEAR);
	    at->cb_attacked_by(tp);
	    at->check_death(DAMAGE_LIVING(tp, at, diff, at_data,
					  at->cb_random_hitloc()), tp);
	    return 1;
	}
	at->catch_msg(({ QCTNAME(tp), " shoots ", QNAME(mob),
			     ", but misses you.\n" }), 0, MSG_HEAR);
    }
    else
    {
	diff = ob_roll - (50 - at->query_size());
	if (diff > 0)
	{	
	    tp->catch_msg(({ "You hit ", QTNAME(at), ".\n" }),
			  tp, MSG_SEE);
	    tell_objects(LISTEN(E(tp)) - ({ tp, at }),
			 ({ QCTNAME(tp), " hits ", QTNAME(at), " with ",
				QNAME(mob), ".\n" }), MSG_SEE, at);
	    return 1;
	}
    }
    tp->catch_msg(({ "You missed ", QTNAME(at), ".\n" }), tp, MSG_SEE);
    tell_objects(LISTEN(E(tp)) - ({ tp, at }),
		 ({ QCTNAME(tp), " missed ", QNAME(at), " with ",
			QNAME(mob), ".\n" }), MSG_SEE, at);
    return 1;
}
