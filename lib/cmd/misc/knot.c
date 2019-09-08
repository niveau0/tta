/*
 * Function name: knot
 * Description:   knot something to an object
 * Arguments:     str - knotting what to what?
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
knot(string str, object tp)
{
    int    result;
    string rope_name, obj_name, position;
    object *inv, *rope_list, *obj_list;
    
    if (!str || sscanf(str, "%s to %s", obj_name, rope_name) != 2)
    {
	tp->catch_tell("Usage: knot <object> to [<position> of] <rope>\n");
	return -1;
    }
    sscanf(rope_name, "%s of %s", position, rope_name);
    inv = I(tp, 0) + I(E(tp), 0);
    rope_list = parse(rope_name, inv, tp, tp->query_step());
    if (!check_size(rope_list, "No '" + rope_name +
		    "' in range to knot something to.\n", tp))
	return -1;
    obj_list = parse(obj_name, inv, tp, tp->query_step());
    if (!check_size(obj_list, "No '" + obj_name +
		    "' in range to knot to the " + rope_name + ".\n", tp))
	return -1;
    if (!(result = rope_list[0]->knot_cmd(obj_list[0], position, tp)))
    {
	tp->catch_tell("You cannot knot the " + obj_list[0]->short() +
		       " to the " + rope_list[0]->short() + ".\n");
	return -1;
    }
    if (result < 0)
	return -1;
    tp->change_int_prop(LIVE_LOST_ROUNDS, 1);
    tp->catch_tell("You made a fine knot.\n");
    if (living(obj_list[0]))
    {
	if (obj_list[0] == tp)
	    obj_list[0]->catch_msg(({ "You knot yourself to ", 
					  QNAME(rope_list[0]), ".\n" }),
				   tp, MSG_SEE);
	else
	    obj_list[0]->catch_msg(({ QCTNAME(tp), " knots you to ", 
					  QNAME(rope_list[0]), ".\n" }),
				   tp, MSG_SEE);
    }
    tell_objects(LISTEN(E(tp)) - ({ tp, obj_list[0] }),
		 ({ QCTNAME(tp), " knots ", QNAME(obj_list[0]), " to ",
			QNAME(rope_list[0]), ".\n" }), MSG_SEE, tp);
    return 1;
}
