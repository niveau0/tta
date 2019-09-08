/*
 * Function name: wake
 * Description:   wake up, awake someone
 * Arguments:     str - whom to awake
 *                tp - acting living
 *                f - if 1 no penalties for awake time
 * Returns:       1 if success, -1 else
 */
nomask varargs int 
wake(string str, object tp, int f)
{
    int    state;
    object here, ob, *list;

    here = E(tp);

    if (str && strlen(str))
    {
	list = tp->visible(I(here, 2));
	list = parse(str, list, tp, tp->query_step());			     
	if (!check_size(list, "No '" + str + "' in range to wake here.\n", tp))
	    return -1;
	ob = list[0];
    }
    if (!ob || tp == ob)
    {
	if (!((state = tp->query_state()) & (SLEEPING | MEDITATING)))
	{
	    tp->catch_tell("You are already awake.\n");
	    return -1;
	}
	LEARN_OB->improve_skills(tp);
	tp->delete_state(MEDITATING | SLEEPING);
	if (state & MEDITATING)
	{
	    tp->remove_effect(BLIND_EFFECT, EFF_MEDITATE);  
	    tp->remove_effect(DEAF_EFFECT, EFF_MEDITATE);  
	    tp->remove_extra_desc(" is meditating here");
	    tp->add_prop(LIVE_USING_SLA, 0);
	    tp->echo("You raise from your trance.\n");
	}
	else
	{
	    tp->remove_effect(BLIND_EFFECT, EFF_SLEEP);  
	    tp->remove_effect(DEAF_EFFECT, EFF_SLEEP);  
	    tp->remove_extra_desc(" is sleeping here");
	    tp->echo("You wake up.\n");
	}
	if (state & RESTING)
	    tp->add_extra_desc(" is resting here");
	tell_objects(LISTEN(here) - ({ tp }),
		     ({ QCTNAME(tp), " wakes up.\n" }), MSG_SEE, tp);
	if (!f)
	    tp->cb_add_boni(({ CB_ACTION_MOD }),
			    ({ ([ time()+10 : -20, time()+20 : -10 ]) }));
	tp->change_int_prop(LIVE_LOST_ROUNDS, 1);
	return 1;
    }
    if ((state = ob->query_state()) & UNCONSCIOUS)
    {
	tp->catch_tell(capitalize(ob->query_pronoun()) +
		       " is unconscious.\n");
	return -1;
    }
    if (!(state & (SLEEPING | MEDITATING)))
    {
	tp->catch_tell(capitalize(ob->query_pronoun()) +
		       " is already awake.\n");
	return -1;
    }
    LEARN_OB->improve_skills(ob);
    ob->delete_state(MEDITATING | SLEEPING);
    if (state & MEDITATING)
    {
	ob->remove_effect(BLIND_EFFECT, EFF_MEDITATE);  
	ob->remove_effect(DEAF_EFFECT, EFF_MEDITATE);  
	ob->remove_extra_desc(" is meditating here");
	ob->add_prop(LIVE_USING_SLA, 0);
    }
    else
    {
	ob->remove_effect(BLIND_EFFECT, EFF_SLEEP);  
	ob->remove_effect(DEAF_EFFECT, EFF_SLEEP);  
	ob->remove_extra_desc(" is sleeping here");
    }
    if (state & RESTING)
	ob->add_extra_desc(" is resting here");
    ob->catch_msg(({ QCTNAME(tp), " awakes you.\n" }), 0, 0);
    tp->echo("You wake " + ob->query_objective() + " up.\n");
    tell_objects(LISTEN(here) - ({ tp, ob }),
		 ({ QCTNAME(tp), " awakes ", QTNAME(ob), ".\n" }),
		 MSG_SEE, tp);
    if (!f)
	ob->cb_add_boni(({ CB_ACTION_MOD }),
			({ ([ time()+10 : -20, time()+20 : -10 ]) }));
    tp->change_int_prop(LIVE_LOST_ROUNDS, 1);
    return 1;
}
