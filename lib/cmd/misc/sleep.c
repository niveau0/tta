/*
 * Function name: sleep
 * Description:   lay down and sleep
 * Arguments:     str - not used
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
sleep(string str, object tp)
{
    tp->add_prop(SLEEP_START, time());
    tp->add_state(SLEEPING | ON_GROUND);
    tp->echo("You lay down and fall asleep.\n");
    tp->add_effect(BLIND_EFFECT, EFF_SLEEP, 1);  
    tp->add_effect(DEAF_EFFECT, EFF_SLEEP, 1);  
    tp->remove_extra_desc(" is resting here"); /* possible before sleeping */
    tp->add_extra_desc(" is sleeping here");
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " lays down and falls asleep.\n" }),
		 MSG_SEE, tp);
    tp->change_int_prop(LIVE_LOST_ROUNDS, 1);
    return 1;
}
