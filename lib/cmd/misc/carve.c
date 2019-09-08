/*
 * Function name: carve
 * Description:   carve wood, includes fletching
 * Arguments:     str - what to be carved out of what
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
carve(string str, object tp)
{
    int i, j;
    string wood;
    object *list, *fletching_sets, *wood_objs;
    
    if (!str || sscanf(str, "%s from %s", str, wood) != 2)
    {
	tp->catch_tell("Usage: carve <item> from <wood>\n");
	return -1;
    }
    if (str!="arrows" && str!="bolts" && str !="arrow" && str !="bolt" )
    {
	tp->catch_tell("You can only carve 'arrows' or 'bolts'.\n");
	return -1;
    }
    list = tp->visible(I(tp, 1));
    wood_objs=parse(wood ,list, tp, 0);
    if (!check_size(wood_objs, "You don't have a "+wood+".\n", tp))
    	return -1;
    if ( i=wood_objs[0]->query_prop(OBJ_MATERIAL) < M_WOOD_FIRST ||
	 i > M_WOOD_LAST)
    {
	tp->catch_tell("The "+wood+" is not made of wood.\n");
	return -1;
    }
    fletching_sets=parse("fletching set", tp->query_hold(SLT_ALL), tp, 0);
    if (!check_size(fletching_sets, "You don't have a fletching set.\n", tp))
	return -1;
    if (i=(wood_objs[0]->query_volume()/100) < 1 ||
	wood_objs[0]->query_weight() < 250 ||
	wood_objs[0]->query_size() < 8 )
    {
	tp->catch_tell("The wood is too small.\n");
	return -1;
    }
    for(i=sizeof(fletching_sets); i--;)
    {
	if(fletching_sets[i]->query_charges())
	    break;
    }
    if(i==-1)
    {
	tp->catch_tell("You don't have a loaded fletching set.\n");
	return -1;
    }
    if (str=="arrows" || str=="arrow")
	str=CLONE_ARROW;
    else
	str=CLONE_BOLT;
    tp->echo("You start carving.\n");
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " starts carving.\n" }), MSG_SEE, tp);
    tp->add_state(WORKING);
    tp->add_extra_desc(" (carves)");
    if ((j=500-random(tp->query_skill_bonus(SK_FLETCHING))) < 60)
	j = 60;
    tp->add_prop(WORKCMD_DATA,
		 ({ this_object(), "stop_carve",
		    call_out("finish_carve", j/TIME_SCALE,
			     tp, str,fletching_sets[i], wood_objs[0]),
		    tp }));
    return 1;
}

nomask void
stop_carve(int id, object tp)
{
    if (!tp)
	return;
    remove_call_out(id);
    tp->catch_tell("You stop to carve.\n");
    tp->remove_extra_desc(" (carves)");
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " stops to carve.\n" }),
		 MSG_SEE, tp);
}

static nomask void
finish_carve(object tp, string str, object set, object wood)
{
    int i, slot;
    
    if (!tp || !set || !wood || !(tp->query_state() & WORKING))
    {
	tp->delete_state(WORKING);
	tp->remove_extra_desc(" (carves)");
	return;
    }
    if (E(wood)!=tp)
    {
	tp->catch_tell("Seems that you lost your "+wood->short()+".\n");
	tp->delete_state(WORKING);
	tp->remove_extra_desc(" (carves)");
	return;
    }   
    if (!(slot=tp->query_hold_slot(set)))
    {
	tp->catch_tell("Seems that you lost your current fletching set.\n");
	tp->delete_state(WORKING);
	tp->remove_extra_desc(" (carves)");
	return;
    }
    i = 100*100/wood->query_volume();
    wood->add_lwvs(0, -(wood->query_weight()*i/100), -100, 0);
    set->change_charges();
    if (tp->skill_check(SK_FLETCHING, 60, 0) > 0)
    {
	tp->add_tool(clone_object(str), slot);
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		     ({ QCTNAME(tp), " made a nice missile.\n" }),
		     MSG_SEE, tp);
	tp->catch_tell("You made a nice missile.\n");
    }
    else
    {
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		     ({ QCTNAME(tp), " tries to carve something, but fails"+
			    ".\n" }), MSG_SEE, tp);
	tp->catch_tell("You fail to carve something useful.\n");
    }
    if(wood->query_volume() < 100 || wood->query_weight() < 250 )
    {
	tp->catch_tell("The "+wood->short()+" seems to be too small "+
		       "now to work on it.\n");
	tp->delete_state(WORKING);
	tp->remove_extra_desc(" (carves)");
	return;
    }
    if(!set->query_charges())
    {
	tp->catch_tell("It seems that your current fletching set is empty.\n");
	tp->delete_state(WORKING);
	tp->remove_extra_desc(" (carves)");
	return;
    }
    if ((i=500-random(tp->query_skill_bonus(SK_FLETCHING))) < 60 )
	i=60;
    call_out("finish_carve", i/TIME_SCALE, tp, str, set, wood);
}
