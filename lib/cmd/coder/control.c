/****************************************************************************
 * control - control a living
 */
nomask int
control(string str)
{
    object tp, ob, *obj;

    CHECK_LEVEL(_BUILDER);
    
    tp = this_interactive();
    if (!str)
    {
        tp->catch_tell("Usage: control <name>\n");
	return -1;
    }
    if (sizeof(obj = parse(str, I(E(tp), 2), 0)))
	ob = obj[0];
    else if (!(ob = find_living(str)))
    {
	tp->catch_tell("No such living.\n");
	return -1;
    }
    if (ob->query_user() || objectp(ob->query_prop(LIVE_CONTROLLED_BY)))
    {
	tp->catch_tell("This living is already controlled.\n");
	return -1;
    }
    ob->set_user(tp->query_user());
    ob->init_msg_flags();
    ob->add_prop(LIVE_CONTROLLED_BY, tp);
    tp->add_prop(CODER_CURRENT_CONTROL, ob);
    tp->catch_tell("You are now controlling: " + str + ".\n");
    return -1;
}

/*
 * Function name: control_cmd
 * Description:   cmd a controlled creature
 * Arguments:     cmd - what command
 * Returns:       1 if successful
 */
nomask int
control_cmd(string cmd)
{
    object tp, ob;
    
    CHECK_LEVEL(_BUILDER);

    tp = this_player();
    if (!cmd || !strlen(cmd))
    {
	tp->catch_tell("With 'control' and ';' you can observe and steer " +
		       "the actions of\n"+
		       "npcs. But you can't stop any coded "+
		       "actions which the\n"+
		       "npc does by itself. How to control:\n"+
		       ";<command>           The npc will do action.\n"+
		       "                      Like: smile\n"+
		       ";silence             The npc returns no more "+
		       "messages till\n"+
		       "                      next command.\n"+
		       ";remove              Stop control.\n");
	return 1;
    }

    if (!(ob = tp->query_prop(CODER_CURRENT_CONTROL)))
    {
	tp->catch_tell("You are not controlling an npc.\n");
	return -1;
    }
    if (cmd == "silence")
    {
	ob->set_user(0);
	return 1;
    }
    if (cmd == "remove")
    {
	tp->add_prop(CODER_CURRENT_CONTROL, 0);
	ob->add_prop(LIVE_CONTROLLED_BY, 0);
	ob->set_user(0);
	return 1;
    }
    ob->set_user(tp);
    return ob->command(cmd);
}
