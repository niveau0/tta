/*
 * Function name: rest
 * Description:   sit down and rest
 * Arguments:     str - not used
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
rest(string str, object tp)
{
    tp->add_prop(RESTING_START, time());
    tp->add_state(RESTING | ON_GROUND);
    tp->echo("You take a proper position to rest a while.\n");
    tp->add_extra_desc(" is resting here");
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " sits down to rest.\n" }), MSG_SEE, tp);
    tp->change_int_prop(LIVE_LOST_ROUNDS, 1);
    return 1;
}
