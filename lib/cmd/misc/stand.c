/*
 * Function name: stand
 * Description:   stand up
 * Arguments:     str - where to stand
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
stand(string str, object tp)
{
    int state;

    if (str == "up")
    {
	if (!(state = tp->query_state() & (RESTING | ON_GROUND)))
	{
	    tp->catch_tell("You are already standing.\n");
	    return -1;
	}
	LEARN_OB->improve_skills(tp);
	tp->delete_state(state);
	tp->echo("You stand up.\n");
	tp->remove_extra_desc(" is resting here");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		     ({ QCTNAME(tp), " stands up.\n" }), MSG_SEE, tp);
	tp->change_int_prop(LIVE_LOST_ROUNDS, 1);
	return 1;
    }
    tp->catch_tell("Stand where?\n");
    return -1;
}
