/*
 * Function name: meditate
 * Description:   sit down and meditate
 * Arguments:     str - type of meditation
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
meditate(string str, object tp)
{
    tp->add_prop(RESTING_START, time());
    tp->add_state(MEDITATING | ON_GROUND);
    tp->echo("You take a proper position to meditate.\n");
    tp->add_effect(BLIND_EFFECT, EFF_MEDITATE, 1);  
    tp->add_effect(DEAF_EFFECT, EFF_MEDITATE, 1);  
    tp->remove_extra_desc(" is resting here"); /* possible before sleeping */
    tp->add_extra_desc(" is meditating here");
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " sits down and closes the eyes.\n" }),
		 MSG_SEE, tp);
    tp->add_prop(LIVE_USING_SLA, 1);
    tp->change_int_prop(LIVE_LOST_ROUNDS, 1);
    return 1;
}
