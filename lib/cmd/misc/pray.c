/*
 * Function name: pray
 * Description:   pray to the gods
 * Arguments:     str - what to pray [to whom]
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
pray(string str, object tp)
{
    string god, msg;
    object g;
    
    if (!str)
    {
	tp->catch_tell("Usage: pray [to <god>] <text>\n");
	return -1;
    }
    if (sscanf(str, "to %s %s", god, msg) != 2)
	msg = str;
    if (g = find_player(god))
	g->catch_tell(BS(capitalize(tp->query_name()) + " prays to you: " +
			 msg, 75, 1)[1..] + "\n"); 
    tp->echo("You send a quick prayer.\n");
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " closes the eyes and seems " +
			"to send a quick prayer.\n" }), MSG_SEE, tp);
    tp->change_int_prop(LIVE_LOST_ROUNDS, 1);
    return 1;
}
