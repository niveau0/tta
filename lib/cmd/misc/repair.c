/*
 * Function name: repair
 * Description:   repair a broken weapon or armor
 * Arguments:     str - what to be repaired
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
repair(string str, object tp)
{
    int i, skill;
    object broken, tool, *list;
    
    if (!str || sscanf(str, "%s", str) != 1)
    {
	tp->catch_tell("Usage: repair <item>\n");
	return -1;
    }
    list = tp->visible(I(tp, 1));
    list = parse(str ,list, tp, 0);
    if (!check_size(list, "You don't have any '" + str + "'.\n", tp))
    	return -1;
    broken = list[0];
    if (!broken->query_prop(IS_WEAPON) && !broken->query_prop(IS_ARMOR))
    {
	tp->catch_tell("You can only repair weapons or armors.\n");
	return -1;
    }
    if (broken->query_prop(IS_MISSILE_WEAPON))
    {
	list = parse("bow string", tp->query_hold(SLT_ALL), tp, 0);
	if (!check_size(list, "You don't have a bow string.\n", tp))
	    return -1;
	tool = list[0];
    }
    else
    {
	list = tp->query_wield(SLT_ALL);
	if (!check_size(list, "You don't have a wielded weapon.\n", tp))
	    return -1;
	for (i=sizeof(list); i--;)
	{
	    if (list[i] == broken)
		continue;
	    if (list[i]->query_data()[A_SKILL] == SK_ATTACK_WAR_HAMMER)
	    {
		tool = list[i];
		break;
	    }
	}
	if (!tool)
	{
	    tp->catch_tell("You don't wield the proper tool to repair your " +
			   str + ".\n");
	    return -1;
	}
    }
    if ((i = broken->query_prop(OBJ_MATERIAL)) >= M_WOOD_FIRST &&
	i <= M_WOOD_LAST)
	skill = SK_WOOD_CRAFT;
    else if (i >= M_METAL_FIRST && i <= M_METAL_LAST)
	skill = SK_METAL_CRAFT;
    else if (i == M_LEATHER)
	skill = SK_LEATHER_WORK;
    else
    {
	tp->catch_tell("You are not able to repair such a material.\n");
	return -1;
    }
    tp->echo("You start to repair your " + str + ".\n");
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " starts to forge.\n" }), MSG_SEE, tp);
    tp->add_state(WORKING);
    tp->add_extra_desc(" (repairs)");
    if ((i=500-random(tp->query_skill_bonus(skill))) < 100 )
	i=100;
    tp->add_prop(WORKCMD_DATA,
		 ({ this_object(), "stop_repair",
			call_out("finish_repair", i/TIME_SCALE, tp,
				 broken, tool, skill),
			tp }));
    return 1;
}

nomask void
stop_repair(int id, object tp)
{
    if (!tp)
	return;
    remove_call_out(id);
    tp->catch_tell("You stop to repair.\n");
    tp->remove_extra_desc(" (repairs)");
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " stops to repair.\n" }),
		 MSG_SEE, tp);
}

static nomask void
finish_repair(object tp, object broken, object tool, int skill)
{
    int i, j;
    
    if (!tp || !broken || !tool || !(tp->query_state() & WORKING))
    {
	tp->delete_state(WORKING);
	tp->remove_extra_desc(" (repairs)");
	return;
    }
    if (E(broken) != tp)
    {
	tp->catch_tell("Seems that you lost your " + broken->short() + ".\n");
	tp->delete_state(WORKING);
	tp->remove_extra_desc(" (repairs)");
	return;
    }   
    if (broken->query_prop(IS_MISSILE_WEAPON))
    {
	tool->add_heap_size(-1);
	if (!IN_ARRAY(tool, tp->query_hold(SLT_ALL)))
	{
	    tp->catch_tell("Seems that you lost your " + tool->short() +
			   ".\n");
	    tp->delete_state(WORKING);
	    tp->remove_extra_desc(" (repairs)");
	    return;
	}
    }
    else
    {
	if (!IN_ARRAY(tool, tp->query_wield(SLT_ALL)))
	{
	    tp->catch_tell("Seems that you lost your " + tool->short() +
			   ".\n");
	    tp->delete_state(WORKING);
	    tp->remove_extra_desc(" (repairs)");
	    return;
	}
    }
    if ((i = tp->skill_check(skill, 0, 0)) > 0)
    {
	tp->catch_tell("Your " + broken->short() + " is repaired now.\n");
	broken->add_prop(OBJ_DAMAGED, 0);
    }
    else if (i+50 <= 50)
    {
	switch (i+50)
	{
	  case 37..50:
	      i = 5;
	      break;
	  case 28..36:
	      i = 10;
	      break;
	  case 20..27:
	      i = 15;
	      break;
	  case 14..19:
	      i = 20;
	      break;
	  case 8..13:
	      i = 25;
	      break;
	  case 5..7:
	      i = 30;
	      break;
	  case 3..4:
	      i = 35;
	      break;
	  case 1..2:
	      i = 40;
	      break;
	  default:
	      i = 50;
	      break;
	}
	if ((j = random(i)) <= i)
	{
	    tp->catch_tell("It seems you ruined your " + broken->short() +
			   " completely.\n");
	    broken->add_prop(OBJ_DAMAGED, -1);
	}
	else
	{
	    tp->catch_tell("You think you didn't do a good job.\n");
	    broken->change_int_prop(OBJ_DAMAGED, j);
	}
    }
    else
	tp->catch_tell("Seems you couldn't fix too much.\n");
    tp->delete_state(WORKING);
    tp->remove_extra_desc(" (repairs)");
}
