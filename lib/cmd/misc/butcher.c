/*
 * Function name: butcher
 * Description:   butcher meat out of a corpse
 * Arguments:     str - the corpse to be butchered out of
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
butcher(string str, object tp)
{
    int    i, skill;
    object *obj_weapon, *corpses, used_weapon;

    if (!str)
    {
	tp->catch_tell("Usage: butcher <corpse>\n");
	return -1;
    }
    corpses = tp->visible(I(E(tp), 0));
    if (!sizeof(corpses = parse(str, corpses, tp, tp->query_step())))
    {
	tp->catch_tell("No '"+ str +"' in range.\n");
	return -1;
    }
    if (living(corpses[0]))
    {
	tp->catch_tell("You cannot butcher a living.\n");
	return -1;
    }
    if (!sizeof(obj_weapon = tp->query_wield(SLT_ALL)))
    {
	tp->catch_tell("You don't have a wielded weapon.\n");
	return -1;
    }
    if (corpses[0]->query_prop(LIVE_BUTCHERED) < 0)
    {
	if (i == -2)
	    tp->catch_tell("There is no usable meat anymore.\n");
	else
	    tp->catch_msg( ({ "You can't butcher ",QTNAME(corpses[0]),
				  ".\n" }), 0, 0);
	return -1;
    }
    for (i=sizeof(obj_weapon); i--;)
    {
	if (obj_weapon[i]->query_size() >= 16)
	    continue;
	skill = obj_weapon[i]->query_data()[A_SKILL];
	if (skill < SK_WEAPON_TYPE_1S_FIRST ||
	    skill > SK_WEAPON_TYPE_1S_LAST)
	    continue;
	used_weapon = obj_weapon[i];
	break;
    }
    if (!used_weapon)
    {
	tp->catch_tell("You don't wield a proper tool to butcher.\n");
	return -1;
    }
    tp->echo("You start to butcher.\n");
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " starts butchering ",QTNAME(corpses[0]),
			".\n" }), MSG_SEE, tp);
    tp->add_extra_desc(" (butchers)");
    tp->add_state(WORKING);
    tp->add_prop(WORKCMD_DATA,
		 ({ this_object(), "stop_butcher",
		    call_out("finish_butcher", (150-random(60)) /TIME_SCALE,
			     tp, used_weapon, corpses[0], skill),
		    tp }));
    return 1;
}
    
nomask void
stop_butcher(int id, object tp)
{
    if (!tp)
	return;
    remove_call_out(id);
    tp->catch_tell("You stop to butcher.\n");
    tp->remove_extra_desc(" (butchers)");
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " stops to butcher.\n" }),
		 MSG_SEE, tp);
}
 
static nomask void
finish_butcher(object tp, object weapon, object corpse, int skill)
{
    int    i, weight, vol, size, ps, amount;
    object obj;
    string meat;
    
    if (!tp || !weapon || !corpse || !(tp->query_state() & WORKING))
    {
	tp->delete_state(WORKING);
	tp->remove_extra_desc(" (butchers)");
	return;
    }
    if (E(weapon) != tp)
    {
	tp->catch_tell("Seems that you lost your " + weapon->short() + ".\n");
	tp->delete_state(WORKING);
	tp->remove_extra_desc(" (butchers)");
	return;
    }   
    if (IN_ARRAY(weapon, tp->query_wield(SLT_ALL)) < 1)
    {
	tp->catch_tell("You don't wield "+ LANG_ADDART(weapon->short()) +
		       ".\n");
	tp->delete_state(WORKING);
	tp->remove_extra_desc(" (butchers)");
	return;
    }
    if ((i = corpse->query_prop(LIVE_BUTCHERED)) < 0)
    {
	if (i==-2)
	    tp->catch_tell("There is no usable meat anymore.\n");
	else
	    tp->catch_msg( ({ "You can't butcher ",QTNAME(corpse),".\n" }),
			   0, 0);
	tp->delete_state(WORKING);
	tp->remove_extra_desc(" (butchers)");
	return;
    }
    
    if (tp->skill_check(SK_FAUNA_LORE, 50, 0) >= 0 &&
	tp->skill_check(skill, 80, 0) >= 0)
    {
	if ((i=tp->skill_check(SK_COOKERY, 0, 0)) < 1)
	    i = 1;
	switch (i)
	{
	  case 1..60:
	      meat = CLONE_MEAT;
	      break;
	  case 61..89:
	      meat = CLONE_STEAK;
	      break;
	  default:
	      meat = CLONE_FILET;
	      break;
	}
	obj = clone_object(meat);
	i = obj->query_weight();
	weight = corpse->query_weight()/200;
	if (weight < i)
	{
	    ps = weight * 100 / i;
	    vol = obj->query_volume() * ps / 100;
	    size = obj->query_size() * ps / 100;
	    obj->set_amount(obj->query_amount() * ps / 100);
	    obj->set_lwvs(obj->query_light(), weight, vol, size);
	}
	amount = random(10) + 8;
	tp->catch_msg(({ "You butcher ",QNAME(obj)," out of ",QTNAME(corpse),
			     ".\n"}), 0, 0);
	if (!sizeof(try_give(({ obj }), corpse, tp)))
	{
	    obj->set_coordinates(tp->query_coordinates());
	    obj->move(E(tp));
	    tp->catch_tell("But your hands are full and you drop it to your "+
			   "feets.\n");
	}
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		     ({ QCTNAME(tp), " butchers some meat "+
			    "from ",QTNAME(corpse),".\n" }), MSG_SEE, tp);
    }
    else
    {
	amount = random(5)+8;
    	tp->catch_tell("You fail to butcher something useful.\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		     ({ QCTNAME(tp), " fails to butcher something from ",
			    QTNAME(corpse),".\n" }), MSG_SEE, tp);
    }
    corpse->change_int_prop(LIVE_BUTCHERED, amount);
    if (corpse->query_prop(LIVE_BUTCHERED) > 100)
    {
	tp->catch_tell("You got all usable meat from the corpse.\n");
	corpse->add_prop(LIVE_BUTCHERED,-2);
	ADD_GENERAL_SUBDESC("It seems to be completely butchered.\n",
			    BUTCHERED_DESC, corpse);
	tp->delete_state(WORKING);
	tp->remove_extra_desc(" (butchers)");
	return;
    }
    call_out("finish_butcher", (150-random(60))/TIME_SCALE, tp, weapon,
	     corpse, skill);
}
