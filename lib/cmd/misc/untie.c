/*
 * Function name: untie
 * Description:   untie an object from a rope
 * Arguments:     str - untie what from what?
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
untie(string str, object tp)
{
    int    result;
    string rope_name, obj_name;
    object *inv, *rope_list, *obj_list;
    
    if (!str || sscanf(str, "%s from %s", obj_name, rope_name) != 2)
    {
	tp->catch_tell("Usage: untie <object> from <rope>\n");
	return -1;
    }
    inv = I(tp, 0) + I(E(tp), 0);
    rope_list = parse(rope_name, inv, tp, tp->query_step());
    if (!check_size(rope_list, "No '" + rope_name +
		    "' in range to untie.\n", tp))
	return -1;
    obj_list = parse(obj_name, inv, tp, tp->query_step());
    if (!check_size(obj_list, "No '" + obj_name +
		    "' in range to untie from " + rope_name + ".\n", tp))
	return -1;
    if (!(result = rope_list[0]->untie_cmd(obj_list[0], tp)))
    {
	tp->catch_tell("You cannot untie the " + obj_list[0]->short() +
		       " from the " + rope_list[0]->short() + ".\n");
	return -1;
    }
    if (result < 0)
	return -1;
    tp->catch_tell("You untie the " + obj_name + ".\n");
    tp->change_int_prop(LIVE_LOST_ROUNDS, 1);
    if (living(obj_list[0]))
    {
	if (obj_list[0] == tp)
	    obj_list[0]->catch_msg(({ "You untie yourself from ", 
					  QNAME(rope_list[0]), ".\n" }),
				   tp, MSG_SEE);
	else
	    obj_list[0]->catch_msg(({ QCTNAME(tp), " unties you from ", 
					  QNAME(rope_list[0]), ".\n" }),
				   tp, MSG_SEE);
    }
    tell_objects(LISTEN(E(tp)) - ({ tp, obj_list[0] }),
		 ({ QCTNAME(tp), " unties ", QNAME(obj_list[0]), " from ",
			QNAME(rope_list[0]), ".\n" }), MSG_SEE, tp);
    return 1;
    
}
