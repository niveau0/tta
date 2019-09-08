/*
 * Function name: aid
 * Description:   treat bloodloss and broken slots
 * Arguments:     str - help whom with what
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
aid(string str, object tp)
{
    int     i, slot, result;
    string  what, whom;
    object  ob, *list, *packs;
    mapping wounds;
    
    if (!str)
    {
	tp->catch_tell("Usage: aid [broken] <limb> of <name>\n");
	return -1;
    }

    packs = parse("aid package", tp->query_hold(SLT_ALL), tp, 0);
    if (sscanf(str, "broken %s", what) == 1)
    {
	if (sscanf(str, "broken %s of %s", what, whom) == 2)
	{
	    list = tp->visible(I(E(tp), 2));
	    list = parse(whom, list, tp, tp->query_step());
	    if (!check_size(list, "No '" + whom +
			    "' in range to aid here.\n", tp))
		return -1;
	    ob = list[0];
	}
	else
	    ob = tp;
	wounds = ob->query_wounds();
	slot   = ob->query_desc_slot(what);
	if (!slot || wounds[slot] != SLOT_BROKEN )
	{
	    if (ob == tp)
		tp->catch_tell("Your " + what + " is not broken.\n");
	    else
		tp->catch_tell("The '" + what + "' of " +
			       ob->query_objective() + " is not broken.\n");
	    return -1;
	}
	result = 0;
	for(i=sizeof(packs); i--;)
	{
	    if (packs[i]->query_charges() > 2)
	    {
		result = 10 + random(11);
		packs[i]->change_charges(-3);
		break;
	    }
	}
	if ((result += tp->skill_check(SK_SECOND_AID, -10, 0)) > 20)
	{
	    if (tp != ob)
	    {
		tp->echo("You bandage " + ob->query_possessive() +
			 " broken " + what + ".\n");
		ob->catch_msg(({ QCTNAME(tp), " bandages your broken " + what +
				     ".\n" }), 0, MSG_SEE);
	    }
	    else
		tp->catch_tell("You bandage your broken " + what + ".\n");
	    tell_objects(LISTEN(E(tp)) - ({ tp, ob }),
			 ({ QCTNAME(tp), " bandages ", QTNAME(ob), ".\n" }),
			 MSG_SEE, tp);
	    /* One week -  2 * result to heal the broken slot */
	    if ((i = 604800 - 2*result) < 600000)
		i = 600000;
	    ob->bandage(slot, i/TIME_SCALE);
	    return 1;
	}
	else if (result > 80)
	{
	    if ( result >= 100)
	    {
		/* One week -  2 * result to heal the broken slot */
		if ((i = 604800 - 2*result) < 600000)
		    i = 600000;
		ob->bandage(slot, i/TIME_SCALE);
	    }
	    else
		ob->remove_bandage(slot);
	    tp->catch_tell("Your bandage is not the best - but it seems "+
		     "to work.\n");
	    if (tp != ob)
		ob->catch_msg(({ QCTNAME(tp), " bandages your " + what +
				     " not very well - but it seems "+
				     "to work.\n" }), 0, 0);
	    tell_objects(LISTEN(E(tp)) - ({ tp, ob }),
			 ({ QCTNAME(tp), " bandages ", QTNAME(ob), " not "+
				"very well - but it seems to work.\n" }),
			 MSG_SEE, tp);
	    return 1;
	}
	else   
	{
	    tp->catch_tell("You ask yourself if the bandage is good "+
			   "enough.\n");
	    if (tp != ob)
		ob->catch_msg(({ QCTNAME(tp), " tries to bandage your " +
				     what + " - but the work looks not "+
				     "very well.\n" }), 0, 0);
	    tell_objects(LISTEN(E(tp)) - ({ tp, ob }),
			 ({ QCTNAME(tp), " tries to bandage ", QTNAME(ob),
				" - but the work looks not very well.\n" }),
			 MSG_SEE, tp);
	    ob->catch_tell("Ouch, pain strikes your body.\n");
	    ob->check_death(2, tp);
	    ob->remove_bandage(slot);
	    return 1;
	}
    }
    
    if (sscanf(str, "%s of %s", what, whom) == 2)
    {
	list = tp->visible(I(E(tp), 2));
	list = parse(whom, list, tp, tp->query_step());
	if (!check_size(list, "No '" + whom + "' in range to aid here.\n", tp))
	    return -1;
	ob = list[0];
    }
    else
    {
	ob = tp;
	what = str;
    }
    wounds = ob->query_bloodwounds();
    slot   = ob->query_desc_slot(what);
    if (!slot || wounds[slot] < 1)
    {
	if (ob == tp)
	    tp->catch_tell("Your " + what + " is not bleeding.\n");
	else
	    tp->catch_tell("The '" + what + "' of " + ob->query_objective() +
			   " is not bleeding.\n");
	return -1;
    }
    result = 0;
    for(i=sizeof(packs); i--;)
    {
	if(packs[i]->query_charges() > 0)
	{
	    result = 10 + random(11);
	    packs[i]->change_charges(1);
	    break;
	}
    }
    if ((result = tp->skill_check(SK_FIRST_AID, -wounds[slot] * 5, 0)) > 0)
    {
	if (tp != ob)
	{
	    tp->echo("You dress " + ob->query_possessive() + " wound.\n");
	    ob->catch_msg(({ QCTNAME(tp), " aids your " + what + ".\n" }), 0,
			  MSG_SEE);
	}
	else
	    tp->catch_tell("You dress your wound.\n");
	tell_objects(LISTEN(E(tp)) - ({ tp, ob }),
		     ({ QCTNAME(tp), " aids ", QTNAME(ob), ".\n" }),
		     MSG_SEE, tp);
	ob->add_bloodloss(-wounds[slot], slot);
	return 1;
    }
    tp->catch_tell("You failed to dress the wound.\n");
    if (tp != ob)
	ob->catch_msg(({ QCTNAME(tp), " fails to aid your " + what +
			     ".\n" }), 0, 0);
    tell_objects(LISTEN(E(tp)) - ({ tp, ob }),
		 ({ QCTNAME(tp), " fails to aid ", QTNAME(ob), ".\n" }),
		 MSG_SEE, tp);
    if (result < 50)
    {
	ob->catch_tell("Ouch, pain strikes your body.\n");
	ob->check_death(2, tp);
    }
    return 1;
}