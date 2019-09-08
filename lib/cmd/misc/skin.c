/*
 * Function name: skin
 * Description:   skin a corpse
 * Arguments:     str - the corpse and the weapon
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
skin(string str, object tp)
{
    int    i, skill;
    object *obj_weapon, *corpses, used_weapon;

    if (!str)
    {
	tp->catch_tell("Usage: skin <corpse>\n");
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
	tp->catch_tell("You cannot skin a living.\n");
	return -1;
    }
    if (!sizeof(obj_weapon = tp->query_wield(SLT_ALL)))
    {
	tp->catch_tell("You don't have a wielded weapon.\n");
	return -1;
    }
    if ((i=corpses[0]->query_prop(LIVE_SKINNED)) < 0)
    {
	if (i == -2)
	    tp->catch_tell("You can't skin it a second time.\n");
	else
	    tp->catch_msg( ({ "You can't skin ",QTNAME(corpses[0]),
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
	tp->catch_tell("You don't wield a proper tool to skin.\n");
	return -1;
    }
    tp->echo("You start to skin.\n");
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " starts skinning ",QTNAME(corpses[0]),
			".\n" }), MSG_SEE, tp);
    tp->add_state(WORKING);
    tp->add_extra_desc(" (skins)");
    i = corpses[0]->query_size()*(random(15)+5);
    tp->add_prop(WORKCMD_DATA,
		 ({ this_object(), "stop_skin",
		    call_out("finish_skin", i / TIME_SCALE,
			     tp, used_weapon, corpses[0], skill),
		    tp }));
    return 1;
}

nomask void
stop_skin(int id, object tp)
{
    if (!tp)
	return;
    remove_call_out(id);
    tp->catch_tell("You stop to skin.\n");
    tp->remove_extra_desc(" (skins)");
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " stops to skin.\n" }),
		 MSG_SEE, tp);
}

static nomask void
finish_skin(object tp, object weapon, object corpse, int skill)
{
    int    i, weight, vol, size;
    object obj;
    string name;

    if (!tp || !weapon || !corpse || !(tp->query_state() & WORKING))
    {
	tp->delete_state(WORKING);
	tp->remove_extra_desc(" (skins)");
	return;
    }
    if (E(weapon) != tp)
    {
	tp->catch_tell("Seems that you lost your " + weapon->short() + ".\n");
	tp->delete_state(WORKING);
	tp->remove_extra_desc(" (skins)");
	return;
    }   
    if (IN_ARRAY(weapon, tp->query_wield(SLT_ALL)) < 1)
    {
	tp->catch_tell("You don't wield " + LANG_ADDART(weapon->short()) +
		       ".\n");
	tp->delete_state(WORKING);
	tp->remove_extra_desc(" (skins)");
	return;
    }
    if ((i = corpse->query_prop(LIVE_SKINNED)) < 0)
    {
	if (i==-2)
	    tp->catch_tell("You can't skin it a second time.\n");
	else
	    tp->catch_msg( ({ "You can't skin ",QTNAME(corpse),
				  ".\n" }), 0, 0);
	tp->delete_state(WORKING);
	tp->remove_extra_desc(" (skins)");
	return;
    }
    if (tp->skill_check(SK_FAUNA_LORE, 25, 0) >= 0 &&
	tp->skill_check(skill, 10, 0) >= 0)
    {
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		     ({ QCTNAME(tp), " skinned ",QTNAME(corpse),
			    ".\n" }), MSG_SEE, tp);
	obj = clone_object(CLONE_SKIN);
	weight = corpse->query_weight()/10;
	vol = corpse->query_volume()/10;
	size = corpse->query_size()/20;
	name = LANG_ADDART(corpse->query_race());
	obj->set_lwvs(obj->query_light(), weight, vol, size);
	obj->set_short(obj->short() + " of " + name);
	obj->set_pshort(obj->plural_short() + " of " + name);
	obj->set_long(obj->long() + "It is made from " + name+".\n");
	obj->set_race_string(name);
	tp->catch_msg(({ "You skinned ", QTNAME(corpse), ".\n"}), 0, 0);
	if (!sizeof(try_give(({ obj }), corpse, tp)))
	{
	    obj->set_coordinates(tp->query_coordinates());
	    obj->move(E(tp));
	    tp->catch_tell("But your hands are full and you drop the skin "+
			   "to your feets.\n");
	}
    }
    else
    {
	tp->catch_msg(({ "You failed to skin ", QTNAME(corpse),".\n" }), 0, 0);
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		     ({ QCTNAME(tp), " failed to skin ",QTNAME(corpse),
			    ".\n" }), MSG_SEE, tp);
    }
    corpse->add_prop(LIVE_SKINNED, -2);
    ADD_GENERAL_SUBDESC("Somebody skinned it completely and "+
			"left the bloody remains.\n", SKINNED_DESC, corpse);
    tp->delete_state(WORKING);
    tp->remove_extra_desc(" (skins)");
}
