/* **************************************************************************
 * reload - reload a present item
 */
nomask int
reload(string str)
{
    int    dest, *co, *hands;
    string file;
    object tp, ob, ob2, env, *obj;

    CHECK_LEVEL(_BUILDER);
    
    tp = this_interactive();
    if (str && sscanf(str, "-n %s", str))
	dest = 1;
    
    if (!str ||
	!sizeof(obj = parse(str, I(env = E(tp), 0), tp)) &&
	!sizeof(obj = parse(str, I(env = tp, 7), tp)))
    {
	tp->catch_tell("Usage: reload [-n] <objectname>\n");
	return -1;
    }
    file = object_name(ob = obj[0]);
    if (!dest)
    {
	if (catch(compile_object(program_name(ob))))
	    return -1;
	tp->catch_tell(file + " updated.\n");
	return 1;
    }
    if (query_ip_number(ob))
    {
	tp->catch_tell("User is still connected, use destruct instead.\n");
	return -1;
    }
    str = MASTER_OB(ob);
    if (ob2 = OBJ(str))
	destruct_object(ob2);
    if (catch(ob2 = clone_object(str)))
	return -1;
    co = ob->query_coordinates();
    remove_object(ob);
    if (ob)
	destruct_object(ob);
    if (living(env) && sizeof(hands = env->query_slot_type(SLT_HAND)))
	env->add_tool(ob2, hands[0]);
    else
    {
	ob2->set_coordinates(co);
	ob2->move(env);
    }
    tp->catch_tell(file + " destructed and recreated.\n");
    return 1;
}
