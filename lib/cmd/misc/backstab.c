/*
 * Function name: backstab
 * Description:   ambush attack
 * Arguments:     str - whom to attack
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
backstab(string str, object tp)
{
    int     damage, skill, diff;
    string  weap;
    object  ob, *list;
    mapping at_data;
    mixed   hloc;

    if (!str || sscanf(str, "%s with %s", str, weap) != 2)
    {
        tp->catch_tell("Usage: backstab <name> with <weapon>\n");
        return -1;
    }
    list = tp->visible(I(E(tp), 2));
    list = parse(str, list, tp, tp->query_step());
    if (!check_size(list, "Noone called '" + str + "' in range.\n", tp))
    	return -1;
    ob = list[0];
    if (ob == tp)
    {
        tp->catch_tell("You cannot backstab yourself.\n");
        return -1;
    }
    list = parse(weap, tp->query_wield(SLT_ALL), 0);
    if (!check_size(list, "You don't wield any weapon '" + str + "'.\n", tp))
    	return -1;
    if (list[0]->query_size() > 20)
    {
	tp->catch_tell("That weapon is much too long for such an " +
		       "ambush attack.\n");
	return -1;
    }
    if (VISIBLE(ob, tp) || 
        ob->skill_check(SK_PERCEPTION, 0, 0) >
        tp->skill_check(SK_AMBUSH, 0, 0) ||
	!(hloc = ob->cb_random_hitloc()))
    {
    	tp->catch_tell("Your victim noticed your attempt.\n");
    	ob->catch_msg(({ QCTNAME(tp), " tried to attack you from behind.\n" }),
    	    	      tp, MSG_SEE);
    	tell_objects(LISTEN(E(tp)) - ({ tp, ob }),
    	    	     ({ QCTNAME(tp), " tried to backstab ", QTNAME(ob),
			    ", but the attempt was noticed.\n" }),
		     MSG_SEE, tp);
	tp->cb_attack_object(ob);
    	return -1;
    }
    diff = roll_dice(-1);
    if (diff < 4)
    {
    	tp->catch_tell("You completely missed your victim.\n");
	return 1;
    }
    at_data = list[0]->query_data();
    tp->skill_check(at_data[A_SKILL], 0, 0); /* care for skill usage */
    tp->catch_tell("Your attack was successful.\n");
    ob->catch_msg(({ "Arg, suddenly something sharp tears into your " +
			 hloc[HITLOC_DESC] +
			 "! Someone attacked you from a hidden position.\n" }),
		  0, 0);
    tell_objects(LISTEN(E(tp)) - ({ tp, ob }),
		 ({ "Suddenly", QTNAME(ob), " screams, finding " +
			ob->query_objective() +
			"self as victim of an ambush.\n" }), 0, 0);
    damage = DAMAGE_LIVING(tp, ob, diff, at_data, hloc);
    if (ob)
    {
	ob->check_death(damage * 2, tp);
	tp->cb_attack_object(ob);
    }
    return 1;
}
